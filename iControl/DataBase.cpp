// DataBase.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "sqlite3.h"
#include "DataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataBase

CDataBase::CDataBase()
{

}

CDataBase::~CDataBase()
{
}


BEGIN_MESSAGE_MAP(CDataBase, CWnd)
	//{{AFX_MSG_MAP(CDataBase)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDataBase message handlers

sqlite3* CDataBase::SqliteOpenCreate(char *pFileName, int eDBType)
{
	int res;
	sqlite3 *pSqlite;

	if (pFileName == NULL)
	{
		return NULL;
	}

	switch (eDBType)
	{
	case MEMORY_FILE:
		res = sqlite3_open_v2(":memory", &pSqlite, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);
		break;
		
	case TEMP_FILE:
		res = sqlite3_open_v2(NULL, &pSqlite, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);
		break;
		
	case NORMAL_FILE:
		res = sqlite3_open_v2(pFileName, &pSqlite, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);
		break;
		
	default:
		return NULL;
		break;
	}
	
	if (res != SQLITE_OK)
	{
		sqlite3_close(pSqlite);
		return NULL;
	}


	CStdioFile file;
	if (file.Open(_T(".\\db\\Initial.sql"), CFile::modeReadWrite, NULL) == FALSE)
	{
		MessageBox("导入数据库文件未找到", "数据库");
		sqlite3_close(pSqlite);
	}

	
	char *zErr;
	CString str;
	while (file.ReadString(str))			// 逐行读取SQL语句
	{
		res = sqlite3_exec(pSqlite, str.GetBuffer(MAX_PATH), NULL, NULL, &zErr);
		if (res != SQLITE_OK)
		{
			str.Format("导入数据库文件错误\r\n%s", zErr);
			MessageBox(str, "数据库错误");
			sqlite3_free(zErr);				// 错误信息资源
			sqlite3_close(pSqlite);		
			file.Close();
			return NULL;
		}
	}

	file.Close();
	return pSqlite;
}

sqlite3* CDataBase::SqliteOpen(char *pFileName, int eDBType)
{
	int res;
	sqlite3 *pSqlite;

	if (pFileName == NULL)
	{
		return NULL;
	}
	
	switch (eDBType)
	{
	case MEMORY_FILE:
		res = sqlite3_open_v2(":memory", &pSqlite, SQLITE_OPEN_READWRITE, NULL);
		break;
		
	case TEMP_FILE:
		res = sqlite3_open_v2(NULL, &pSqlite, SQLITE_OPEN_READWRITE, NULL);
		break;
		
	case NORMAL_FILE:
		res = sqlite3_open_v2(pFileName, &pSqlite, SQLITE_OPEN_READWRITE, NULL);
		break;
		
	default:
		return NULL;
		break;
	}
	
	if (res != SQLITE_OK)
	{
		sqlite3_close(pSqlite);
		return NULL;
	}
	
	return pSqlite;
}

BOOL CDataBase::SqliteQuery(sqlite3 *pSqlite, char *sql, LPQUERY_INFO pQueryInfo)
{
	int res;
	char *zErr;
	char **pResult;
	int iRows, iCols;

	if (pSqlite == NULL || sql == NULL && pQueryInfo == NULL)
	{
		return FALSE;
	}

	res = sqlite3_get_table(pSqlite, sql, &pResult, &iRows, &iCols, &zErr);
	if (res != SQLITE_OK)
	{
		sqlite3_free(zErr);				// 错误信息资源
		return FALSE;
	}

	pQueryInfo->iRows = iRows;
	pQueryInfo->iCols = iCols;
	pQueryInfo->pResult = pResult;
	return TRUE;
}

void CDataBase::SqliteFreeQueryInfo(LPQUERY_INFO pQueryInfo)
{
	if (pQueryInfo->pResult == NULL)
	{
		return;
	}

	sqlite3_free_table(pQueryInfo->pResult);
}

void CDataBase::SqliteClose(sqlite3 *pSqlite)
{
	sqlite3_close(pSqlite);
}

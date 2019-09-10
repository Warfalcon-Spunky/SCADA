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
		MessageBox("�������ݿ��ļ�δ�ҵ�", "���ݿ�");
		sqlite3_close(pSqlite);
	}

	
	char *zErr;
	CString str;
	while (file.ReadString(str))			// ���ж�ȡSQL���
	{
		res = sqlite3_exec(pSqlite, str.GetBuffer(MAX_PATH), NULL, NULL, &zErr);
		if (res != SQLITE_OK)
		{
			str.Format("�������ݿ��ļ�����\r\n%s", zErr);
			MessageBox(str, "���ݿ����");
			sqlite3_free(zErr);				// ������Ϣ��Դ
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
		sqlite3_free(zErr);				// ������Ϣ��Դ
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

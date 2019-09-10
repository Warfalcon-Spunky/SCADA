// HisTbl.cpp : implementation file
//

#include "stdafx.h"
#include "HisTbl.h"

CHisTbl::CHisTbl()
{

}

CHisTbl::CHisTbl(int iCols)
:m_iCols(iCols)
{
	if (m_iCols > 0)
	{
		m_pColsName = new void *[m_iCols];			// 创建字段名指针存储空间
		for (int i = 0; i < m_iCols; i++)
		{
			m_pColsName[i] = NULL;
		}
	}
	else
	{
		m_pColsName = NULL;
	}
}

CHisTbl::~CHisTbl()
{
	for (int i = 0; i < m_iCols; i++)
	{
		if (m_pColsName[i] != NULL)
		{
			delete m_pColsName[i];
		}
	}

	if (m_pColsName != NULL)
	{
		delete m_pColsName;
	}
}

bool CHisTbl::Open(const char* pFile)
{
	if (SQLiteTbl::Open(pFile))
	{
		if (Query())		// 进行一次查询
		{
			return true;
		}
    }

	Close();				// 任何错误都进行数据库关闭和错误返回
	return false;
}

BOOL CHisTbl::SetTableName(const char *pTblName)
{
	if (pTblName == NULL)
	{
		return FALSE;
	}

	memset(m_TblName, '\0', sizeof(m_TblName));
	memcpy(m_TblName, pTblName, strlen(pTblName));
	return TRUE;
}

BOOL CHisTbl::SetItem(int item, const char* pName, MYDB_TYPE type)
{
	if (m_pColsName[item] != NULL)
	{
		return FALSE;
	}

	// 根据字段类型创建类型变量
	switch (type)
	{
	case MYDB_TYPE_TEXT:
		m_pColsName[item] = new SQLiteString;
		break;
	case MYDB_TYPE_INT:
		m_pColsName[item] = new int;
		break;
	case MYDB_TYPE_DOUBLE:
		m_pColsName[item] = new double;
		break;
    case MYDB_TYPE_BLOB:
		m_pColsName[item] = new SQLiteBlob;
		break;
	default:
		return FALSE;
		break;
	}

	SQLiteTbl::SetItem(item, pName, type, m_pColsName[item]);		// 重载父类函数
	return TRUE;
}

const char* CHisTbl::TableName() 
{ 
	return m_TblName;
}

int CHisTbl::ItemCount()
{
	return m_iCols;
}

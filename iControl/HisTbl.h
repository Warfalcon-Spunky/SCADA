// HisTbl.h : header file
//

#ifndef _HISTBL_H
#define _HISTBL_H

#include "SQLiteTbl.h"

class CHisTbl : public SQLiteTbl
{
public:
	CHisTbl();
    CHisTbl(int iCols);

	virtual ~CHisTbl();
	
    virtual bool Open(const char* pFile);
    virtual const char* TableName();
    virtual int ItemCount();

	BOOL SetTableName(const char *pTblName);
	BOOL SetItem(int item, const char* pName, MYDB_TYPE type);

	int  m_iCols;
	char m_TblName[100];
	void **m_pColsName;
};



#endif
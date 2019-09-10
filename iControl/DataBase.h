#if !defined(AFX_DATABASE_H__CEE81FBE_E54D_4747_94D8_CB1E35088097__INCLUDED_)
#define AFX_DATABASE_H__CEE81FBE_E54D_4747_94D8_CB1E35088097__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataBase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataBase window

enum DB_TYPE
{
	MEMORY_FILE,
	TEMP_FILE,
	NORMAL_FILE
};

typedef struct tagQUERY_INFO
{
	int    iRows;
	int    iCols;
	char **pResult;
} QUERY_INFO, *LPQUERY_INFO;

class CDataBase : public CWnd
{
// Construction
public:
	CDataBase();

// Attributes
public:

// Operations
public:
	sqlite3* SqliteOpenCreate(char *pFileName, int eDBType);
	sqlite3* SqliteOpen(char *pFileName, int eDBType);
	BOOL SqliteQuery(sqlite3 *pSqlite, char *sql, LPQUERY_INFO pQueryInfo);
	void SqliteFreeQueryInfo(LPQUERY_INFO pQueryInfo);
	void SqliteClose(sqlite3 *pSqlite);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataBase)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDataBase();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDataBase)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATABASE_H__CEE81FBE_E54D_4747_94D8_CB1E35088097__INCLUDED_)

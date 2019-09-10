#if !defined(AFX_LOGINDIALOG_H__6CB8359C_4E62_4AAB_903E_9413399865E8__INCLUDED_)
#define AFX_LOGINDIALOG_H__6CB8359C_4E62_4AAB_903E_9413399865E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginDialog.h : header file
//
#include "HisTbl.h"

/////////////////////////////////////////////////////////////////////////////
// CLoginDialog dialog

class CLoginDialog : public CDialog
{
// Construction
public:
	CLoginDialog(CWnd* pParent = NULL);   // standard constructor

	CFont   *m_pCaptionFont;
	CFont   *m_pStaticFont;
	CFont   *m_pEditFont;
	CHisTbl *m_pLoginTbl;

	BOOL     m_bDBOpen;

// Dialog Data
	//{{AFX_DATA(CLoginDialog)
	enum { IDD = IDD_LOGIN_DIALOG };
	CStatic	m_DlgCaption;
	CString	m_EditCount;
	CString	m_EditPwd;
	CString	m_StaticCount;
	CString	m_StaticPwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDIALOG_H__6CB8359C_4E62_4AAB_903E_9413399865E8__INCLUDED_)

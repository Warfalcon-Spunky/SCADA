#if !defined(AFX_REGISTERDLG_H__4BB92379_4378_4CC6_8342_433917CFB7C6__INCLUDED_)
#define AFX_REGISTERDLG_H__4BB92379_4378_4CC6_8342_433917CFB7C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegisterDlg.h : header file
//
#include "RegCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CRegisterDlg dialog

class CRegisterDlg : public CDialog
{
// Construction
public:
	CRegisterDlg(CWnd* pParent = NULL);   // standard constructor

	CRegCtrl m_RegCtrl;

	CFont *m_pStaticAFont;
	CFont *m_pStaticBFont;
	CFont *m_pEditAFont;
	CFont *m_pEditBFont;

	BOOL   m_bIsRegister;
	BOOL   m_bIsModifyResgister;

	BYTE   m_EncodeBuff[16];

	int    m_iAccessDays;

	int  GetAccessDays(void);
	void CleanRegedit(void);
	BOOL StrToHex(BYTE *pHexBuff, int HexCnt, char *pStrBuff);

// Dialog Data
	//{{AFX_DATA(CRegisterDlg)
	enum { IDD = IDD_REGISTER_DIALOG };
	CEdit	m_DayEdit;
	CEdit	m_PrivateCodeEdit;
	CEdit	m_PublicCodeEdit;
	CString	m_strACode;
	CString	m_strECode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegisterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegisterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRegister();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTERDLG_H__4BB92379_4378_4CC6_8342_433917CFB7C6__INCLUDED_)

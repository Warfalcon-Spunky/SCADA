#if !defined(AFX_AUTOCTRLDLG_H__E979A472_2C09_406F_8A33_DC9D573C2AD4__INCLUDED_)
#define AFX_AUTOCTRLDLG_H__E979A472_2C09_406F_8A33_DC9D573C2AD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoCtrlDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoCtrlDlg dialog

class CAutoCtrlDlg : public CDialog
{
// Construction
public:
	CAutoCtrlDlg(CWnd* pParent = NULL);   // standard constructor

	int m_iSelKilnIndex;

// Dialog Data
	//{{AFX_DATA(CAutoCtrlDlg)
	enum { IDD = IDD_AUTOCTRL };
	CComboBox	m_SelKilnIdx;
	BOOL	m_bAutoCtrlEnable;
	CString	m_CtrlTime;
	CString	m_OutDnLimit;
	CString	m_FrontPts;
	CString	m_IncCoeff;
	CString	m_RearPts;
	CString	m_OutUpLimit;
	CString	m_GrossWaitTime;
	CString	m_GrossProcTime;
	CString	m_GrossProcMaxTime;
	CString	m_DnWarn;
	CString	m_UpWarn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokComboSelKiln();
	afx_msg void OnEditchangeComboSelKiln();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCTRLDLG_H__E979A472_2C09_406F_8A33_DC9D573C2AD4__INCLUDED_)

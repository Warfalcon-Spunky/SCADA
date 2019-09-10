#if !defined(AFX_TIMEQUERYDLG_H__D75DEE92_2DCC_48FE_BAC9_4519485FC017__INCLUDED_)
#define AFX_TIMEQUERYDLG_H__D75DEE92_2DCC_48FE_BAC9_4519485FC017__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeQueryDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTimeQueryDlg dialog

class CTimeQueryDlg : public CDialog
{
// Construction
public:
	CTimeQueryDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTimeQueryDlg)
	enum { IDD = IDD_QUERY };
	CDateTimeCtrl	m_DateCtrl;
	UINT	m_SetHour;
	UINT	m_SetMin;
	CTime	m_SetDate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeQueryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTimeQueryDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMEQUERYDLG_H__D75DEE92_2DCC_48FE_BAC9_4519485FC017__INCLUDED_)

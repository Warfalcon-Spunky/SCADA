#if !defined(AFX_SERIALDLG_H__6C6EAD71_8F3A_44B7_A87B_408C48F1F146__INCLUDED_)
#define AFX_SERIALDLG_H__6C6EAD71_8F3A_44B7_A87B_408C48F1F146__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SerialDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSerialDlg dialog

class CSerialDlg : public CDialog
{
// Construction
public:
	CSerialDlg(CWnd* pParent = NULL);   // standard constructor

	int m_iComPort;
	int m_iBaudrate;
	int m_iDtuPort;
	int m_iDtuBaudrate;
	int m_iDataBits;
	int m_iStopBits;
	char m_cPrity;
	DWORD m_lDtuPortID;
	int m_iTimeout;
	int m_iScanTime;
	int m_iGrossTime;
	int m_iBellTime;

// Dialog Data
	//{{AFX_DATA(CSerialDlg)
	enum { IDD = IDD_SERIAL_DIALOG };
	CComboBox	m_DtuPort;
	CComboBox	m_DtuBaudSet;
	CComboBox	m_StopBits;
	CComboBox	m_Parity;
	CComboBox	m_ComPort;
	CComboBox	m_DataBits;
	CComboBox	m_BaudSet;
	CString	m_ScanTime;
	CString	m_Timeout;
	CString	m_GrossTime;
	CString	m_BellTime;
	CString	m_DtuPortID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSerialDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALDLG_H__6C6EAD71_8F3A_44B7_A87B_408C48F1F146__INCLUDED_)

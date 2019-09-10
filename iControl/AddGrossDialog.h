#if !defined(AFX_ADDGROSSDIALOG_H__65099EA8_0833_4792_B46B_6B7D706D000A__INCLUDED_)
#define AFX_ADDGROSSDIALOG_H__65099EA8_0833_4792_B46B_6B7D706D000A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddGrossDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddGrossDialog dialog

class CAddGrossDialog : public CDialog
{
// Construction
public:
	CAddGrossDialog(CWnd* pParent = NULL);   // standard constructor

public:
	CFont   *m_pCaptionFont;
	CFont   *m_pStaticFont;
	CFont   *m_pEditFont;

	int      m_iRoastID;
	double   m_fHeatValue;

	int      m_iRoastFurnaceNum, m_iBakeFurnaceNum;
	BOOL     m_bIsGrossTbl;
	LPRTPP  *m_pPortPageBuff;

	CHisTbl    *m_pGrossTbl;
	CHisTbl   **m_pCarHeatTbl;

// Dialog Data
	//{{AFX_DATA(CAddGrossDialog)
	enum { IDD = IDD_ADD_GROSS };
	CComboBox	m_RoastCombo;
	CStatic	m_DlgCaption;
	CString	m_HeatEdit;
	CString	m_HeatCaption;
	CString	m_RoastCaption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddGrossDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddGrossDialog)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDGROSSDIALOG_H__65099EA8_0833_4792_B46B_6B7D706D000A__INCLUDED_)

#if !defined(AFX_MOTORCTRLDLG_H__564C9C1E_B96F_4C8B_842B_DE1538CB7879__INCLUDED_)
#define AFX_MOTORCTRLDLG_H__564C9C1E_B96F_4C8B_842B_DE1538CB7879__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotorCtrlDlg.h : header file
//
#include "iangulargaugex.h"
#include "MainFrm.h"
#include "iControlDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CMotorCtrlDlg dialog

class CMotorCtrlDlg : public CDialog
{
// Construction
public:
	CMotorCtrlDlg(CWnd* pParent = NULL);   // standard constructor

	CiAngularGaugeX *m_pAngularGaugeX;
	CSliderCtrl	    *m_pSlider;
	CToolTipCtrl    *m_pToolTips;

	float  *m_pMotorValue;
	int     m_iMotorIndex;
	int     m_iActiveKilnIndex;
	int     m_iKilnMotorIdx;
	int     m_iTotalMotorNum;

// Dialog Data
	//{{AFX_DATA(CMotorCtrlDlg)
	enum { IDD = IDD_MOTOR_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotorCtrlDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMotorCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnCustomdrawSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnNext();
	virtual void OnOK();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTORCTRLDLG_H__564C9C1E_B96F_4C8B_842B_DE1538CB7879__INCLUDED_)

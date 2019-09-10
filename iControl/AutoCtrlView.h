#if !defined(AFX_AUTOCTRLVIEW_H__7B3E4904_14AC_478F_84DE_654B17E1DD75__INCLUDED_)
#define AFX_AUTOCTRLVIEW_H__7B3E4904_14AC_478F_84DE_654B17E1DD75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoCtrlView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoCtrlView view

class CAutoCtrlView : public CView
{
protected:
	CAutoCtrlView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAutoCtrlView)

// Attributes
public:
	int *m_pAutoCtrlCntBuff;

	long m_lAutoCtrlLogIdx;
	CStdioFile m_hFile;
	BOOL       m_bFileOpen;

	int        *m_pAutoCtrlFlagCnt;
	int        *m_pFrontDec;
	int        *m_pRearDec;

	int        *m_pWarnTimerCnt;

// Operations
public:
	int AutoCtrlProc(int iKilnIdx);
	void OpenFile(const char *pFileName);
	void CloseFile(void);
	void WriteString(const char *pString);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoCtrlView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAutoCtrlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAutoCtrlView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCTRLVIEW_H__7B3E4904_14AC_478F_84DE_654B17E1DD75__INCLUDED_)

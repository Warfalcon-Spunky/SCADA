#if !defined(AFX_CURCURVEVIEW_H__A3720B6D_E63A_4D77_8133_1C2300BB79F7__INCLUDED_)
#define AFX_CURCURVEVIEW_H__A3720B6D_E63A_4D77_8133_1C2300BB79F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurCurveView.h : header file
//
#include "MainFrm.h"
#include "Curve.h"

typedef struct tagRTCURVE
{
	CCurve    *m_pCurve;
} RTCURVE, *LPRTCURVE;

/////////////////////////////////////////////////////////////////////////////
// CCurCurveView view
class CCurCurveView : public CView
{
protected:
	CCurCurveView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCurCurveView)

// Attributes
public:
	int         m_iKilnNum;
	LPRTCURVE  *m_pRealTimeCurve;

	int         m_iCurrentFurnaceNum;	

// Operations
public:
	void UpdateView(void);
	void ActiveView(int iNewFurnaceNum);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurCurveView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCurCurveView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCurCurveView)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURCURVEVIEW_H__A3720B6D_E63A_4D77_8133_1C2300BB79F7__INCLUDED_)

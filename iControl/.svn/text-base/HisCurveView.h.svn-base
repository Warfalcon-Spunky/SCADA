#if !defined(AFX_HISCURVEVIEW_H__ABB83223_6816_4653_8006_1556BD39D819__INCLUDED_)
#define AFX_HISCURVEVIEW_H__ABB83223_6816_4653_8006_1556BD39D819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HisCurveView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHisCurveView view

#include "Curve.h"

class CHisCurveView : public CView
{
protected:
	CHisCurveView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHisCurveView)

// Attributes
public:
	CCurve    *m_pCurve[2];

	int        iActiveType;

// Operations
public:
	void SetCurveType(int iType);
	void CurveUpdate(void);
	void SetTotalCars(int iTotalCars);
	void SetCurvePoint(int iIndex, int iCarNum, int iTemperatureVal);
	void SetCurveCaption(const char *pCaption);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHisCurveView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHisCurveView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CHisCurveView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISCURVEVIEW_H__ABB83223_6816_4653_8006_1556BD39D819__INCLUDED_)

#if !defined(AFX_REALTIMEVIEW_H__F87D4326_EB9D_4F20_A302_8EED7C67C980__INCLUDED_)
#define AFX_REALTIMEVIEW_H__F87D4326_EB9D_4F20_A302_8EED7C67C980__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealTimeView.h : header file
//
#include "MainFrm.h"
#include "isevensegmentanalogx.h"

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView view

typedef struct tagRTV
{
	CiSevenSegmentAnalogX **m_pSegPort;
} RTV, *LPRTV;

typedef struct tagPortRect
{
	int cx;
	int cy;
} PortRect, *LPPortRect;

class CRealTimeView : public CView
{
public:	
	CRealTimeView();           // protected constructor used by dynamic creation
//	CRealTimeView(int iRoastFurnaceNum, int iBakeFurnaceNum, LPRTPP *pPortPageBuff);
	DECLARE_DYNCREATE(CRealTimeView)

// Attributes
public:
	int     m_iKilnNum;

	CDC    *m_pDC;
	CDC     m_MemDcDraw;
	HBITMAP m_HbtMap;

	LPRTV *m_pRTPort;
	int    m_iCurrentFurnaceNum, m_iOldFurnaceNum;	
	int   *m_pPortNum;

	LPPortRect m_pPortRect;

	CRect  m_MaxClientRect;
	BOOL   m_bFirstPaint;

// Operations
public:
	void InitSegPort(void);
	void UpdateView(void);
	void ActiveView(int iNewFurnaceNum);
	void DrawCaption(CDC *pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealTimeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRealTimeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CRealTimeView)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALTIMEVIEW_H__F87D4326_EB9D_4F20_A302_8EED7C67C980__INCLUDED_)

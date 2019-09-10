#if !defined(AFX_HISGRIDVIEW_H__919C4912_6A5A_4944_9674_2A414F4A0033__INCLUDED_)
#define AFX_HISGRIDVIEW_H__919C4912_6A5A_4944_9674_2A414F4A0033__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HisGridView.h : header file
//

#include "MainFrm.h"
#include "IGridCtrl.h"
#include "HisTbl.h"
#include "iControlDoc.h"
#include "HisCurveView.h"

/////////////////////////////////////////////////////////////////////////////
// CHisGridView view

class CHisGridView : public CView
{
protected:
	CHisGridView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHisGridView)

// Attributes
public:
	int     m_iActiveFurnaceNum;
	int     m_iKilnNum;

	int    *m_pTempSetting;

	CIGridCtrl *m_pGrid;
	LOGFONT m_lfFixCellFont;
	LOGFONT m_lfCellFont;

	int        m_iSelectRow;

	CDC     m_MemDcDraw;	// 兼容设备内存指针, 作为显示缓存
	HBITMAP m_HbtMap;

// Operations
public:
	void ActiveView(int iFurnaceNum);
	void SetHistoryGridOnly(void);
	void SetGridItem(GV_DISPINFO *pDispInfo);
	void SetCurveItem(GV_DISPINFO *pDispInfo);
	void UpdateView(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHisGridView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHisGridView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CHisGridView)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnInsertAndApp(void);
	afx_msg void OnInsert(void);
	afx_msg void OnQueryHistory(void);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISGRIDVIEW_H__919C4912_6A5A_4944_9674_2A414F4A0033__INCLUDED_)

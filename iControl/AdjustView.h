#if !defined(AFX_ADJUSTVIEW_H__89BACFB1_B9F0_4717_A15F_1F7C364AB0BC__INCLUDED_)
#define AFX_ADJUSTVIEW_H__89BACFB1_B9F0_4717_A15F_1F7C364AB0BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdjustView.h : header file
//

#include "MainFrm.h"
// #include "GridCtrl_src/GridCtrl.h"
#include "HisTbl.h"
#include "iControlDoc.h"
#include "IGridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CAdjustView view

class CAdjustView : public CView
{
protected:
	CAdjustView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdjustView)

// Attributes
public:
	int         m_iActiveFurnaceNum;
	int         m_iKilnNum;

	CIGridCtrl *m_pGrid;
	LOGFONT     m_lfFixCellFont;
	LOGFONT     m_lfCellFont;

	CString     m_strEdit;
	int         m_iSelectRow;

// Operations
public:
	void SetAdjustActiveView(int iFurnace);
	void SetAdjustGrid(void);
	void ModifyGridItem(GV_DISPINFO *pDispInfo);
	void SetGridItem(GV_DISPINFO *pDispInfo);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdjustView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdjustView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdjustView)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADJUSTVIEW_H__89BACFB1_B9F0_4717_A15F_1F7C364AB0BC__INCLUDED_)

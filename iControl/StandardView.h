#if !defined(AFX_STANDARDVIEW_H__8AB4A61D_0521_47FE_86D6_AEEC43414E47__INCLUDED_)
#define AFX_STANDARDVIEW_H__8AB4A61D_0521_47FE_86D6_AEEC43414E47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StandardView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStandardView view
#include "MainFrm.h"
// #include "GridCtrl_src/GridCtrl.h"
#include "HisTbl.h"
#include "iControlDoc.h"
#include "IGridCtrl.h"

class CStandardView : public CView
{
protected:
	CStandardView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CStandardView)

// Attributes
public:
	int     m_iActiveFurnaceNum;
	int     m_iKilnNum;
	int     m_iRecordRowNum;
	int     m_iRecordColNum;
	
	CIGridCtrl *m_pGrid;
	LOGFONT    m_lfFixCellFont;
	LOGFONT    m_lfCellFont;

	CDC        m_MemDcDraw;
	HBITMAP    m_HbtMap;
	CRect      m_MaxClientRect;

	int        m_iSelectRow;

	CString    m_strEdit;

	CButton	   *m_pSetButton;
	CButton    *m_pDelButton;

// Operations
public:
	void SetStandardGridOnly(void);
	void ActiveView(int iFurnaceNum);
	void SetGridItem(GV_DISPINFO *pDispInfo);
	void ModifyGridItem(GV_DISPINFO *pDispInfo);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStandardView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStandardView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CStandardView)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSetStandard();
	afx_msg void OnDelStandard();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STANDARDVIEW_H__8AB4A61D_0521_47FE_86D6_AEEC43414E47__INCLUDED_)

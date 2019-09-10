// iControlView.h : interface of the CIControlView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICONTROLVIEW_H__D0122E4C_4CCE_43F4_9D31_BEB0280C0AA5__INCLUDED_)
#define AFX_ICONTROLVIEW_H__D0122E4C_4CCE_43F4_9D31_BEB0280C0AA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CIControlView : public CView
{
protected: // create from serialization only
	CIControlView();
	DECLARE_DYNCREATE(CIControlView)

// Attributes
public:
	CIControlDoc* GetDocument();

// Operations
public:
//	CBrush m_BackgroundBrush;
	void SetActiveView(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIControlView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIControlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CIControlView)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in iControlView.cpp
inline CIControlDoc* CIControlView::GetDocument()
   { return (CIControlDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONTROLVIEW_H__D0122E4C_4CCE_43F4_9D31_BEB0280C0AA5__INCLUDED_)

#if !defined(AFX_MYSPLITTERWND_H__FA5B48F6_31AB_4C48_BC1F_002A5BE26E19__INCLUDED_)
#define AFX_MYSPLITTERWND_H__FA5B48F6_31AB_4C48_BC1F_002A5BE26E19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySplitterWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySplitterWnd frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMySplitterWnd : public CSplitterWnd
{
	DECLARE_DYNCREATE(CMySplitterWnd)
public:
	CMySplitterWnd();           // protected constructor used by dynamic creation
	void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);

// Attributes
protected:
// 	CSplitterWnd    m_wndSplitter;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySplitterWnd)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMySplitterWnd();

	// Generated message map functions
	//{{AFX_MSG(CMySplitterWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSPLITTERWND_H__FA5B48F6_31AB_4C48_BC1F_002A5BE26E19__INCLUDED_)

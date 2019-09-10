#if !defined(AFX_SPLITEFRAME_H__475BE9E5_7404_48FF_9C98_49A6C7553FCD__INCLUDED_)
#define AFX_SPLITEFRAME_H__475BE9E5_7404_48FF_9C98_49A6C7553FCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpliteFrame.h : header file
//

#include "HisGridView.h"
#include "HisCurveView.h"
#include "MySplitterWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CSpliteFrame frame
class CHisGridView;
class CHisCurveView;
class CSpliteFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CSpliteFrame)
public:
	CSpliteFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	BOOL m_bSplitterCreated;

	CMySplitterWnd m_wndSplitter;
	CHisGridView  *m_pHisGridView;
	CHisCurveView *m_pHisCurveView;

// Operations
public:
	CHisCurveView* GetHisCurveViewPoint(void);
	CHisGridView* GetHisGridViewPoint(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpliteFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpliteFrame();
protected:
	// Generated message map functions
	//{{AFX_MSG(CSpliteFrame)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITEFRAME_H__475BE9E5_7404_48FF_9C98_49A6C7553FCD__INCLUDED_)

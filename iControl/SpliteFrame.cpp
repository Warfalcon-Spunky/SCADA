// SpliteFrame.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "SpliteFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpliteFrame

IMPLEMENT_DYNCREATE(CSpliteFrame, CFrameWnd)

CSpliteFrame::CSpliteFrame()
{
	m_bSplitterCreated = FALSE;
}

CSpliteFrame::~CSpliteFrame()
{
}


BEGIN_MESSAGE_MAP(CSpliteFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CSpliteFrame)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpliteFrame message handlers

BOOL CSpliteFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	// 创建2行1列的分割条
	m_wndSplitter.CreateStatic(this, 2, 1, AFX_WS_DEFAULT_VIEW & ~WS_BORDER);
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CHisGridView), CSize(0, 0), pContext);
	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CHisCurveView), CSize(0, 0), pContext);

	m_wndSplitter.SetActivePane(0, 0);
	m_pHisGridView = (CHisGridView *)GetActiveView();

 	m_wndSplitter.SetActivePane(1, 0);
 	m_pHisCurveView = (CHisCurveView *)GetActiveView();

	m_bSplitterCreated = TRUE;
	
	return m_bSplitterCreated;
}

void CSpliteFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_bSplitterCreated)				// 必须保证分割被建立
	{
		CRect rect;
		GetWindowRect(&rect);
		
		m_wndSplitter.SetRowInfo(0, (rect.Height() * 2 / 10) + 1, 10);	// 占20%的高度
		m_wndSplitter.SetRowInfo(1, (rect.Height() * 8 / 10) + 1, 10);	// 占80%的高度
		m_wndSplitter.RecalcLayout();
	}
	
}

CHisGridView* CSpliteFrame::GetHisGridViewPoint(void)
{
	if (m_bSplitterCreated == FALSE)
	{
		return NULL;
	}

	return m_pHisGridView;
}

CHisCurveView* CSpliteFrame::GetHisCurveViewPoint(void)
{
	if (m_bSplitterCreated == FALSE)
	{
		return NULL;
	}

	return m_pHisCurveView;
}

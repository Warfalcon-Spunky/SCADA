// MySplitterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "MySplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySplitterWnd

IMPLEMENT_DYNCREATE(CMySplitterWnd, CSplitterWnd)

CMySplitterWnd::CMySplitterWnd()
{
}

CMySplitterWnd::~CMySplitterWnd()
{
}

BOOL CMySplitterWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
// 	return m_wndSplitter.Create(this,
// 		2, 2,       // TODO: adjust the number of rows, columns
// 		CSize(10, 10),  // TODO: adjust the minimum pane size
// 		pContext);

	return CMySplitterWnd::OnCreateClient(lpcs, pContext);
}


BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CMySplitterWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySplitterWnd message handlers

void CMySplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
	if (pDC == NULL || nType != splitBorder)
	{
		CSplitterWnd::OnDrawSplitter(pDC, nType, rect);
		return;
	}
	
	ASSERT_VALID(pDC);
	pDC->Draw3dRect(rect, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHIGHLIGHT));
	
	// 	if (pDC == NULL)
	// 	{
	// 		RedrawWindow(rect, NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
	// 		return;
	// 	}
	// 
	// 	ASSERT_VALID(pDC);
	// 	CRect rc = rect;
	// 
	// 	switch(nType)
	// 	{
	// 	case splitBorder:
	// 		// 设置边框样式
	//  		pDC->DrawEdge(rc, EDGE_ETCHED, BF_RECT);
	// 
	// 		// 重画分割线边沿颜色
	//  		pDC->Draw3dRect(rc, RGB(0, 200, 255), RGB(0, 200, 255));
	//  		rc.InflateRect(-CX_BORDER, -CY_BORDER);
	//  		pDC->Draw3dRect(rc, RGB(0, 200, 255), RGB(0, 200, 255));
	// 		return;
	// 
	// 	case splitBox:
	// 		pDC->Draw3dRect(rc, RGB(0, 200, 255), RGB(0, 200, 255));
	// 		rc.InflateRect(-CX_BORDER, -CY_BORDER);
	// 		pDC->Draw3dRect(rc, RGB(0, 200, 255), RGB(0, 200, 255));
	// 		rc.InflateRect(-CX_BORDER, -CY_BORDER);
	// 		pDC->FillSolidRect(rc, RGB(0, 200, 255));
	// 		pDC->Draw3dRect(rc, RGB(0, 200, 255), RGB(0, 200, 255));
	// 		return;
	// 
	// 	case splitBar:
	// 		// 重画分割线内部颜色
	// 		pDC->FillSolidRect(rc, RGB(0, 200, 255));
	// 		rc.InflateRect(-CX_BORDER, -CY_BORDER);
	// 		pDC->Draw3dRect(rc, RGB(0, 200, 255), RGB(0, 200, 255));
	// 		return;
	// 
	// 	default:
	// 		ASSERT(FALSE);
	// 	}
}

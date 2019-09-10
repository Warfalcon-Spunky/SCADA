// iControlView.cpp : implementation of the CIControlView class
//

#include "stdafx.h"
#include "iControl.h"

#include "MainFrm.h"
#include "iControlDoc.h"
#include "iControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIControlView

IMPLEMENT_DYNCREATE(CIControlView, CView)

BEGIN_MESSAGE_MAP(CIControlView, CView)
	//{{AFX_MSG_MAP(CIControlView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIControlView construction/destruction

CIControlView::CIControlView()
{
	// TODO: add construction code here
}

CIControlView::~CIControlView()
{
}

BOOL CIControlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CIControlView drawing

void CIControlView::OnDraw(CDC* pDC)
{
	CIControlDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CIControlView diagnostics

#ifdef _DEBUG
void CIControlView::AssertValid() const
{
	CView::AssertValid();
}

void CIControlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIControlDoc* CIControlView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIControlDoc)));
	return (CIControlDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIControlView message handlers

void CIControlView::SetActiveView(void)
{
	CIControlDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect rect;
	GetClientRect(rect);
	
	CClientDC dc(this);
	Graphics imgraphics(dc.m_hDC);
	
	char PicFileName[MAX_PATH];
	sprintf(PicFileName, "%s", pDoc->m_strBKPicPath.GetBuffer(MAX_PATH));
	
	WCHAR wcharPicFileName[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, PicFileName, sizeof(PicFileName), wcharPicFileName, sizeof(wcharPicFileName));
	
	Image image(wcharPicFileName);
 	imgraphics.DrawImage(&image, rect.left, rect.top, rect.Width(), rect.Height());

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
	pMainFrame->SetFrameCaption("»¶Ó­ÄúÊ¹ÓÃ");
}

void CIControlView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	SetActiveView();
	
	// Do not call CView::OnPaint() for painting messages
}

BOOL CIControlView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
//	return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

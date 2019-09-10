// HisCurveView.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "HisCurveView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHisCurveView
#define THE_CURVE_BACKGROUND_COLOR	RGB(219, 219, 219)		// 设置界面背景颜色
#define THE_CURVE_FRONT_COLOR		RGB(0, 0, 0)			// 设置界面前景颜色
#define THE_CURVE_CAPTION_COLOR		RGB(0, 64, 255)			// 设置标题颜色
#define THE_CURVE_1_COLOR			RGB(255, 0, 0)			// 设置1号曲线颜色
#define THE_CURVE_2_COLOR			RGB(0, 0, 255)			// 设置2号曲线颜色
#define THE_TIP_BACKGROUND_COLOR	RGB(255, 0xfa, 0xcd)	// 设置提示标签的背景颜色
#define THE_TIP_FRONT_COLOR			RGB(32, 32, 32)			// 设置提示标签的背景颜色


IMPLEMENT_DYNCREATE(CHisCurveView, CView)

CHisCurveView::CHisCurveView()
{
	m_pCurve[0] = NULL;
	m_pCurve[1] = NULL;

	iActiveType = 0;
}

CHisCurveView::~CHisCurveView()
{
}


BEGIN_MESSAGE_MAP(CHisCurveView, CView)
	//{{AFX_MSG_MAP(CHisCurveView)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHisCurveView drawing

void CHisCurveView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CHisCurveView diagnostics

#ifdef _DEBUG
void CHisCurveView::AssertValid() const
{
	CView::AssertValid();
}

void CHisCurveView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHisCurveView message handlers

void CHisCurveView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	if (pDoc->m_bDBOpen == FALSE)		// 相关数据库打开和数据初始化为成功
	{
		PostQuitMessage(1);
		return;
	}

	CRect cRect;									
	GetClientRect(&cRect);										// 获取客户端区域大小
	int iwidth = GetSystemMetrics(SM_CXSCREEN);
	int iheight = GetSystemMetrics(SM_CYSCREEN);

	CDC *pDC = GetDC();

	// 同时建立带车位和米数型曲线
	m_pCurve[0] = new CCurve(GetSafeHwnd(), pDC, iwidth, iheight, 34, 24, 60, 40, 60, 40, 0);
	m_pCurve[0]->SetClientRect(cRect);									// 设置显示区域大小
	m_pCurve[0]->SetBackGround(THE_CURVE_BACKGROUND_COLOR);				// 设置界面背景颜色
	m_pCurve[0]->SetFrontGround(THE_CURVE_FRONT_COLOR);					// 设置界面前景颜色
	m_pCurve[0]->SetCaptionColor(THE_CURVE_CAPTION_COLOR);				// 设置标题颜色
	m_pCurve[0]->SetCurveColor(0, THE_CURVE_1_COLOR);					// 设置1号曲线颜色			
	m_pCurve[0]->SetCurveColor(1, THE_CURVE_2_COLOR);					// 设置2号曲线颜色
	m_pCurve[0]->SetCaptionStr("");										// 设置界面标题
	m_pCurve[0]->SetTipBkColor(THE_TIP_BACKGROUND_COLOR);				// 设置提示标签的背景色
	m_pCurve[0]->SetTipFtColor(THE_TIP_FRONT_COLOR);					// 设置提示标签的前景色

	m_pCurve[1] = new CCurve(GetSafeHwnd(), pDC, iwidth, iheight, 34, 24, 60, 40, 60, 40, 3);
	m_pCurve[1]->SetClientRect(cRect);									// 设置显示区域大小
	m_pCurve[1]->SetBackGround(THE_CURVE_BACKGROUND_COLOR);				// 设置界面背景颜色
	m_pCurve[1]->SetFrontGround(THE_CURVE_FRONT_COLOR);					// 设置界面前景颜色
	m_pCurve[1]->SetCaptionColor(THE_CURVE_CAPTION_COLOR);				// 设置标题颜色
	m_pCurve[1]->SetCurveColor(0, THE_CURVE_1_COLOR);					// 设置1号曲线颜色			
	m_pCurve[1]->SetCurveColor(1, THE_CURVE_2_COLOR);					// 设置2号曲线颜色
	m_pCurve[1]->SetCaptionStr("");										// 设置界面标题
	m_pCurve[1]->SetTipBkColor(THE_TIP_BACKGROUND_COLOR);				// 设置提示标签的背景色
	m_pCurve[1]->SetTipFtColor(THE_TIP_FRONT_COLOR);					// 设置提示标签的前景色
	ReleaseDC(pDC);
}

void CHisCurveView::SetCurveType(int iType)
{
	if (iType != 0)
	{	
		iActiveType = 1;
	}
	else
	{
		iActiveType = 0;
	}
}

void CHisCurveView::CurveUpdate(void)
{
	CRect cRect;
	GetClientRect(cRect);
	m_pCurve[iActiveType]->SetClientRect(cRect);
	
	SendMessage(WM_PAINT, 0, 0);
}

void CHisCurveView::SetCurvePoint(int iIndex, int iCarNum, int iTemperatureVal)
{
	// 清除上一次的曲线数据
	if (iIndex < 0)
	{
		m_pCurve[iActiveType]->ClearPoint(0);
		m_pCurve[iActiveType]->ClearPoint(1);
		return;
	}

	// 非法温度代表断线
	if (iTemperatureVal < (-100) || iTemperatureVal > 1200)
	{
		// 断线温度显示为0
		m_pCurve[iActiveType]->AddPoint(iIndex, iCarNum, 0);
	}
	else
	{
		// 绘温度曲线
		m_pCurve[iActiveType]->AddPoint(iIndex, iCarNum, iTemperatureVal);
	}
}

void CHisCurveView::SetTotalCars(int iTotalCars)
{
	if (iActiveType == 0)
	{
		m_pCurve[0]->SetTotalCars(iTotalCars);
	}
	else
	{
		m_pCurve[1]->SetTotalCars(iTotalCars / 10);
	}
}

void CHisCurveView::SetCurveCaption(const char *pCaption)
{
	m_pCurve[iActiveType]->SetCaptionStr(pCaption);
}

void CHisCurveView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_pCurve[iActiveType] != NULL)				// 测试曲线对象是否被建立
	{
		CRect cRect(0, 0, cx, cy);
		m_pCurve[iActiveType]->SetClientRect(cRect);		
	}
}

void CHisCurveView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CDC *pDC = GetDC();
	m_pCurve[iActiveType]->PaintUpdate(pDC);				// 更新当前曲线显示
	ReleaseDC(pDC);
	
	// Do not call CView::OnPaint() for painting messages
}

void CHisCurveView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT MousePoint;
	MousePoint.x = point.x;
	MousePoint.y = point.y;
	
	m_pCurve[iActiveType]->UpdateMousePoint(MousePoint);
	SendMessage(WM_PAINT, 0, 0);
	
	CView::OnMouseMove(nFlags, point);
}

void CHisCurveView::OnDestroy() 
{
	CView::OnDestroy();
	
	delete m_pCurve[0];
	delete m_pCurve[1];
}

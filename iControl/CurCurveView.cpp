// CurCurveView.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "iControlDoc.h"
#include "CurCurveView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCurCurveView
#define THE_CURVE_BACKGROUND_COLOR	RGB(219, 219, 219)		// ���ý��汳����ɫ
#define THE_CURVE_FRONT_COLOR		RGB(0, 0, 0)			// ���ý���ǰ����ɫ
#define THE_CURVE_CAPTION_COLOR		RGB(0, 64, 255)			// ���ñ�����ɫ
#define THE_CURVE_1_COLOR			RGB(255, 0, 0)			// ����1��������ɫ
#define THE_CURVE_2_COLOR			RGB(0, 0, 255)			// ����2��������ɫ
#define THE_TIP_BACKGROUND_COLOR	RGB(255, 0xfa, 0xcd)	// ������ʾ��ǩ�ı�����ɫ
#define THE_TIP_FRONT_COLOR			RGB(32, 32, 32)			// ������ʾ��ǩ�ı�����ɫ

#define THE_UPDATE_TIME				2						// ���Ҹ��¶�ʱ����

IMPLEMENT_DYNCREATE(CCurCurveView, CView)

CCurCurveView::CCurCurveView()
{
	m_pRealTimeCurve = NULL;

	m_iCurrentFurnaceNum = 0;
}

CCurCurveView::~CCurCurveView()
{
}


BEGIN_MESSAGE_MAP(CCurCurveView, CView)
	//{{AFX_MSG_MAP(CCurCurveView)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurCurveView drawing

void CCurCurveView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CCurCurveView diagnostics

#ifdef _DEBUG
void CCurCurveView::AssertValid() const
{
	CView::AssertValid();
}

void CCurCurveView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCurCurveView message handlers

void CCurCurveView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	if (pDoc->m_bDBOpen == FALSE)		// ������ݿ�򿪺����ݳ�ʼ��Ϊ�ɹ�
	{
		PostQuitMessage(1);
		return;
	}

	m_iKilnNum = pDoc->m_iKilnNum;

	CRect cRect;									
	GetClientRect(&cRect);										// ��ȡ�ͻ��������С
	int iwidth = GetSystemMetrics(SM_CXSCREEN);
	int iheight = GetSystemMetrics(SM_CYSCREEN);

	CDC *pDC;

	m_pRealTimeCurve = new LPRTCURVE[m_iKilnNum];				// �����ṹ��ָ�뻺����
	for (int i = 0; i < m_iKilnNum; i++)
	{
		m_pRealTimeCurve[i] = new RTCURVE;
		pDC                 = GetDC();

		if (pDoc->m_pKilnBuff[i].type == 3)													// �ƶ�Ҥ
		{
			m_pRealTimeCurve[i]->m_pCurve = new CCurve(GetSafeHwnd(), pDC, iwidth, iheight, pDoc->m_pKilnBuff[i].TotalKiln / 10, 24, 60, 40, 60, 40, pDoc->m_pKilnBuff[i].type);
		}
		else
		{
			m_pRealTimeCurve[i]->m_pCurve = new CCurve(GetSafeHwnd(), pDC, iwidth, iheight, pDoc->m_pKilnBuff[i].TotalKiln, 24, 60, 40, 60, 40, pDoc->m_pKilnBuff[i].type);
		}

		m_pRealTimeCurve[i]->m_pCurve->SetClientRect(cRect);								// ������ʾ�����С
		m_pRealTimeCurve[i]->m_pCurve->SetBackGround(THE_CURVE_BACKGROUND_COLOR);			// ���ý��汳����ɫ
		m_pRealTimeCurve[i]->m_pCurve->SetFrontGround(THE_CURVE_FRONT_COLOR);				// ���ý���ǰ����ɫ
		m_pRealTimeCurve[i]->m_pCurve->SetCaptionColor(THE_CURVE_CAPTION_COLOR);			// ���ñ�����ɫ
		m_pRealTimeCurve[i]->m_pCurve->SetCurveColor(0, THE_CURVE_1_COLOR);					// ����1��������ɫ			
		m_pRealTimeCurve[i]->m_pCurve->SetCurveColor(1, THE_CURVE_2_COLOR);					// ����2��������ɫ
		m_pRealTimeCurve[i]->m_pCurve->SetCaptionStr(pDoc->m_pKilnBuff[i].strCurveCaption);	// ���ý������
		m_pRealTimeCurve[i]->m_pCurve->SetTipBkColor(THE_TIP_BACKGROUND_COLOR);				// ������ʾ��ǩ�ı���ɫ
		m_pRealTimeCurve[i]->m_pCurve->SetTipFtColor(THE_TIP_FRONT_COLOR);					// ������ʾ��ǩ��ǰ��ɫ

		if (pDoc->m_pKilnBuff[i].type == 3)													// �ƶ�Ҥ
		{
			m_pRealTimeCurve[i]->m_pCurve->SetTotalCars(pDoc->m_pKilnBuff[i].TotalKiln / 10);// �����ܾ���
		}
		else
		{
			m_pRealTimeCurve[i]->m_pCurve->SetTotalCars(pDoc->m_pKilnBuff[i].TotalKiln);	// �����ܳ�λ
		}

		ReleaseDC(pDC);
	}

//	SetTimer(0, THE_UPDATE_TIME * 1000, NULL);
}

void CCurCurveView::ActiveView(int iNewFurnaceNum)
{
	m_iCurrentFurnaceNum = iNewFurnaceNum;
	
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	
	CString str;
	
	switch (pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].type)
	{
	case 0:
		str.Format("%d# <Mobile Kiln> Realtime Curve", m_iCurrentFurnaceNum + 1);
		break;
	case 1:
		str.Format("%d# <Drying Kiln> Realtime Curve", m_iCurrentFurnaceNum + 1);
		break;
	case 2:
		str.Format("%d# <Through Kiln> Realtime Curve", m_iCurrentFurnaceNum + 1);
		break;
	case 3:
		str.Format("%d# <Roasting Kiln> Realtime Curve", m_iCurrentFurnaceNum + 1);
		break;
	default:
		break;
	}
	
	pMainFrame->SetFrameCaption(str);
}

void CCurCurveView::UpdateView(void)
{
	int i;
	int car_loc_offset;

	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	// ���ʵ���¶����ߺͱ�׼�¶����ߵ�����
	m_pRealTimeCurve[m_iCurrentFurnaceNum]->m_pCurve->ClearPoint(0);
	m_pRealTimeCurve[m_iCurrentFurnaceNum]->m_pCurve->ClearPoint(1);

	// �ƶ�Ҥ, λ�ò��ü�1
	if (pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].type == 3)
	{
		car_loc_offset = 0;
	}
	else
	{
		car_loc_offset = 1;
	}
	
	for (i = 0; i < pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].TemperatureNum; i++)
	{
		// �Ƿ��¶ȴ�������
		if ((pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].Value < (-100)) 
			|| (pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].Value > 1200))
		{
			// �����¶���ʾΪ0
			m_pRealTimeCurve[m_iCurrentFurnaceNum]->m_pCurve->AddPoint(0, 
								pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].CarIdex + car_loc_offset, 
								0);
		}
		else
		{
			// ��ʵʱ�¶�����
			m_pRealTimeCurve[m_iCurrentFurnaceNum]->m_pCurve->AddPoint(0, 
								pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].CarIdex + car_loc_offset, 
								pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].Value);
		}				
	}

	for (i = 0; i < pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].TemperatureNum; i++)
	{
		// �Ƿ��¶ȴ�������
		if ((pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].SValue < (-100)) 
			|| (pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].SValue > 1200))
		{
			// �����¶���ʾΪ0
			m_pRealTimeCurve[m_iCurrentFurnaceNum]->m_pCurve->AddPoint(1, 
								pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].CarIdex + car_loc_offset, 
								0);
		}
		else
		{
			// ��ʵʱ�¶�����
			m_pRealTimeCurve[m_iCurrentFurnaceNum]->m_pCurve->AddPoint(1, 
								pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].CarIdex + car_loc_offset, 
								pDoc->m_pKilnBuff[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].SValue);
		}				
	}

	CRect cRect;
	GetClientRect(cRect);
	m_pRealTimeCurve[m_iCurrentFurnaceNum]->m_pCurve->SetClientRect(cRect);
	
	SendMessage(WM_PAINT, 0, 0);
} 

void CCurCurveView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CDC *pDC = GetDC();
	m_pRealTimeCurve[m_iCurrentFurnaceNum]->m_pCurve->PaintUpdate(pDC);				// ���µ�ǰ������ʾ
	ReleaseDC(pDC);

	// Do not call CView::OnPaint() for painting messages
}

void CCurCurveView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (AfxIsValidAddress(m_pRealTimeCurve, 10))		// �������߶����Ƿ񱻽���
	{
		CRect cRect(0, 0, cx, cy);
		m_pRealTimeCurve[m_iCurrentFurnaceNum]->m_pCurve->SetClientRect(cRect);
		
//		SendMessage(WM_PAINT, 0, 0);		
	}
	
}

void CCurCurveView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT MousePoint;
	MousePoint.x = point.x;
	MousePoint.y = point.y;

	m_pRealTimeCurve[m_iCurrentFurnaceNum]->m_pCurve->UpdateMousePoint(MousePoint);
	SendMessage(WM_PAINT, 0, 0);
	
	CView::OnMouseMove(nFlags, point);
}

void CCurCurveView::OnDestroy() 
{
	CView::OnDestroy();

//	KillTimer(0);
	
	for (int i = 0; i < m_iKilnNum; i++)
	{
		delete m_pRealTimeCurve[i]->m_pCurve;
		delete m_pRealTimeCurve[i];
	}
	
	delete [] m_pRealTimeCurve;
}

void CCurCurveView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	UpdateView();
	
	CView::OnTimer(nIDEvent);
}
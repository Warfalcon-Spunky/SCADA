// RealTimeView.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "iControlDoc.h"
#include "RealTimeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_LINE_PORT_NUM		6				// 一行显示的表头个数
#define THE_BACKGROUD_COLOR		RGB(0, 0, 0)	// 视图背景颜色

#define THE_IN_TEMP_SEGMENT_COLOR		RGB(0, 255, 0)
#define THE_IN_TEMP_SEGMENTBK_COLOR		RGB(0, 0, 0)

#define THE_OUT_TEMP_SEGMENT_COLOR		RGB(255, 0, 0)
#define THE_OUT_TEMP_SEGMENTBK_COLOR	RGB(0, 0, 0)

#define THE_OTHER_SEGMENT_COLOR			RGB(0, 255, 0)
#define THE_OTHER_SEGMENTBK_COLOR		RGB(0, 0, 0)

#define THE_CAPTION_FT_COLOR			RGB(0, 128, 255)

#define THE_UPDATE_TIME					2					// 自我更新定时设置

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView

IMPLEMENT_DYNCREATE(CRealTimeView, CView)

CRealTimeView::CRealTimeView()
{
	m_iCurrentFurnaceNum = -1;
	m_iOldFurnaceNum = -1;

	m_bFirstPaint = FALSE;
}

CRealTimeView::~CRealTimeView()
{
}


BEGIN_MESSAGE_MAP(CRealTimeView, CView)
	//{{AFX_MSG_MAP(CRealTimeView)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOVE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView drawing

void CRealTimeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView diagnostics

#ifdef _DEBUG
void CRealTimeView::AssertValid() const
{
	CView::AssertValid();
}

void CRealTimeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRealTimeView message handlers

void CRealTimeView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	if (pDoc->m_bDBOpen == FALSE)		// 相关数据库打开和数据初始化为成功
	{
		PostQuitMessage(1);				// 退出程序
		return;
	}

	m_iKilnNum = pDoc->m_iKilnNum;

	m_pPortNum = new int[m_iKilnNum];
	for (int n = 0; n < m_iKilnNum; n++)
	{
		// 计算每条窑表头总数
		m_pPortNum[n] = pDoc->m_pKilnBuffForCurve[n].TemperatureNum 
						+ pDoc->m_pKilnBuffForCurve[n].TemperatureANum
						+ pDoc->m_pKilnBuffForCurve[n].PressNum 
						+ pDoc->m_pKilnBuffForCurve[n].HumidityNum 
						+ pDoc->m_pKilnBuffForCurve[n].MotorNum;		
	}

	m_pPortRect = new PortRect[m_pPortNum[0]]; 

	pMainFrame->GetClientRect(m_MaxClientRect);

	int mid = m_MaxClientRect.Height() / 2 - 50;
	int start = 10;//35;
	int space = 120;//125;
	int vspace = 150;

	// 5 10 15 35 50 65 78 84 90
	m_pPortRect[0].cx = start + (space) * 0; m_pPortRect[0].cy = mid;
	m_pPortRect[1].cx = start + (space) * 1; m_pPortRect[1].cy = mid;
	m_pPortRect[2].cx = start + (space) * 2; m_pPortRect[2].cy = mid;
	m_pPortRect[3].cx = start + (space) * 3; m_pPortRect[3].cy = mid;
	m_pPortRect[4].cx = start + (space) * 4; m_pPortRect[4].cy = mid;
	m_pPortRect[5].cx = start + (space) * 5; m_pPortRect[5].cy = mid;
	m_pPortRect[6].cx = start + (space) * 6; m_pPortRect[6].cy = mid;
	m_pPortRect[7].cx = start + (space) * 7; m_pPortRect[7].cy = mid;
	m_pPortRect[8].cx = start + (space) * 8; m_pPortRect[8].cy = mid;

	// 96 96 102 102 108 108 114 114 120 120
	m_pPortRect[9].cx  = start + space * 9; m_pPortRect[9].cy = mid - vspace;
	m_pPortRect[10].cx = start + space * 9; m_pPortRect[10].cy = mid + vspace;
	m_pPortRect[11].cx = start + space * 10; m_pPortRect[11].cy = mid - vspace;
	m_pPortRect[12].cx = start + space * 10; m_pPortRect[12].cy = mid + vspace;
	m_pPortRect[13].cx = start + space * 11; m_pPortRect[13].cy = mid - vspace;
	m_pPortRect[14].cx = start + space * 11; m_pPortRect[14].cy = mid + vspace;
	m_pPortRect[15].cx = start + space * 12; m_pPortRect[15].cy = mid - vspace;
	m_pPortRect[16].cx = start + space * 12; m_pPortRect[16].cy = mid + vspace;

	// 126 132
	m_pPortRect[19].cx = start + (space) * 13; m_pPortRect[19].cy = mid;
	m_pPortRect[20].cx = start + (space) * 14; m_pPortRect[20].cy = mid;

	// 102 102 108 108 114 114 
	m_pPortRect[21].cx = start + space * 9; m_pPortRect[21].cy = mid - vspace - vspace;
	m_pPortRect[22].cx = start + space * 9; m_pPortRect[22].cy = mid + vspace + vspace;
	m_pPortRect[23].cx = start + space * 10; m_pPortRect[23].cy = mid - vspace - vspace;
	m_pPortRect[24].cx = start + space * 10; m_pPortRect[24].cy = mid + vspace + vspace;
	m_pPortRect[25].cx = start + space * 11; m_pPortRect[25].cy = mid - vspace - vspace;
	m_pPortRect[26].cx = start + space * 11; m_pPortRect[26].cy = mid + vspace + vspace;

	// 内排潮 外排潮
	m_pPortRect[27].cx = start + (space) * 1; m_pPortRect[27].cy = mid - vspace - 20;
	m_pPortRect[28].cx = start + (space) * 2; m_pPortRect[28].cy = mid - vspace - 20;
	m_pPortRect[17].cx = start + (space) * 3; m_pPortRect[17].cy = mid - vspace - 20;

	// 内抽风 外抽风
	m_pPortRect[29].cx = start + (space) * 1; m_pPortRect[29].cy = mid + vspace + 20;
	m_pPortRect[30].cx = start + (space) * 3; m_pPortRect[30].cy = mid + vspace + 20;
	m_pPortRect[18].cx = start + (space) * 7; m_pPortRect[18].cy = mid - vspace;

	// 压力1 压力2
	m_pPortRect[31].cx = start + (space) * 1; m_pPortRect[31].cy = mid - vspace / 2 - 10;
	m_pPortRect[32].cx = start + (space) * 1; m_pPortRect[32].cy = mid + vspace / 2 + 10;

	// 湿度1
	m_pPortRect[33].cx = start + (space) * 4; m_pPortRect[33].cy = mid + vspace / 2 + 10;

	// 1号电机 2号电机
	m_pPortRect[34].cx = start + (space) * 8; m_pPortRect[34].cy = mid - vspace - vspace;
	m_pPortRect[35].cx = start + (space) * 8; m_pPortRect[35].cy = mid + vspace + vspace;

	BITMAPINFO bmpinfo;
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = GetSystemMetrics(SM_CXSCREEN);   // 创建最大的图片, 尺寸宽度为屏幕尺寸宽度
	bmpinfo.bmiHeader.biHeight = GetSystemMetrics(SM_CYSCREEN);  // 创建最大的图片, 尺寸高度为屏幕尺寸宽度
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 32;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = 0;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;
	
	CDC *pDC = GetDC();
	m_MemDcDraw.CreateCompatibleDC(pDC);
	m_HbtMap = CreateDIBSection(m_MemDcDraw, &bmpinfo, DIB_RGB_COLORS, NULL, NULL, 0);
	m_MemDcDraw.SelectObject(m_HbtMap);
	m_MemDcDraw.SetBkMode(TRANSPARENT);
	ReleaseDC(pDC);

//	SetTimer(0, THE_UPDATE_TIME * 1000, NULL);
}

void CRealTimeView::InitSegPort(void)
{
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int i, j, k, m;
	CRect port_rect;
	int wid = m_MaxClientRect.Width() / MAX_LINE_PORT_NUM;
	int hid;
 	if (m_pPortNum[m_iCurrentFurnaceNum] > 24)
	{
		hid = (int)((float)wid * 0.5F);
	}
	else
	{
		hid = (int)((float)wid * 0.618F);
	}

	m_pRTPort = new LPRTV[m_iKilnNum];	// 建立结构体指针缓冲区
	for (i = 0; i < m_iKilnNum; i++)
	{
		m_pRTPort[i] = new RTV;			// 建立结构体实体

		// 创建表头指针缓冲区
		m_pRTPort[i]->m_pSegPort = new CiSevenSegmentAnalogX *[m_pPortNum[i]];
		
		for (k = 0, m = 0, j = 0; j < m_pPortNum[i]; j++)
		{
// 			PortRect.left = 6 + m * wid;
// 			PortRect.right = PortRect.left + wid;
// 			PortRect.top = 150 + k * hid;
// 			PortRect.bottom = PortRect.top + hid;
// 			
// 			m = (m + 1) % MAX_LINE_PORT_NUM;
// 			if (m == 0)
// 			{
// 				k++;			
// 			}

			port_rect.left  = m_pPortRect[j].cx;
			port_rect.right = port_rect.left + 100;
			port_rect.top   = m_pPortRect[j].cy;
			port_rect.bottom = port_rect.top + 50;
			
			// 创建表头实体
			m_pRTPort[i]->m_pSegPort[j] = new CiSevenSegmentAnalogX;
			m_pRTPort[i]->m_pSegPort[j]->Create("", WS_CHILD | WS_VISIBLE, port_rect, this, 50800 + j);	
		}

		// 设置温度表头的相关属性
		int iPortNum = 0;
		for (j = iPortNum; j < pDoc->m_pKilnBuffForCurve[i].TemperatureNum; j++)
		{
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentColor(THE_OUT_TEMP_SEGMENT_COLOR);
			m_pRTPort[i]->m_pSegPort[j]->SetBackGroundColor(THE_OUT_TEMP_SEGMENTBK_COLOR);	
			m_pRTPort[i]->m_pSegPort[j]->SetBorderStyle(1);
			m_pRTPort[i]->m_pSegPort[j]->SetShowOffSegments(FALSE);
//			m_pRTPort[i]->m_pSegPort[j]->SetSegmentSize(3);
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentSize(2);
//			m_pRTPort[i]->m_pSegPort[j]->SetSegmentSeperation(2);
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentSeperation(0);
			m_pRTPort[i]->m_pSegPort[j]->SetShowSign(FALSE);
			m_pRTPort[i]->m_pSegPort[j]->SetPrecision(0);
//			m_pRTPort[i]->m_pSegPort[j]->SetDigitSpacing(10);
			m_pRTPort[i]->m_pSegPort[j]->SetDigitSpacing(3);
			m_pRTPort[i]->m_pSegPort[j]->SetDigitCount(4);			
			m_pRTPort[i]->m_pSegPort[j]->SetValue(10000);
			if (i == 0)
			{
				m_pRTPort[i]->m_pSegPort[j]->SetVisible(TRUE);			
			}
			else
			{
				m_pRTPort[i]->m_pSegPort[j]->SetVisible(FALSE);	
			}	
		}

		iPortNum = iPortNum + pDoc->m_pKilnBuffForCurve[i].TemperatureNum;
		for (j = iPortNum; j < (iPortNum + pDoc->m_pKilnBuffForCurve[i].TemperatureANum); j++)
		{
			if (strstr(pDoc->m_pKilnBuffForCurve[i].m_pTempAObjectBuff[j - iPortNum].strName, "抽") != NULL)
			{
				m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[j]->SetSegmentColor(RGB(0, 128, 128));
			}
			else if (strstr(pDoc->m_pKilnBuffForCurve[i].m_pTempAObjectBuff[j - iPortNum].strName, "风") != NULL)
			{
				m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[j]->SetSegmentColor(RGB(128, 128, 0));
			}
			else
			{
				m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[j]->SetSegmentColor(THE_OUT_TEMP_SEGMENT_COLOR);
			}
//			m_pRTPort[i]->m_pSegPort[j]->SetSegmentColor(THE_OUT_TEMP_SEGMENT_COLOR);
			m_pRTPort[i]->m_pSegPort[j]->SetBackGroundColor(THE_OUT_TEMP_SEGMENTBK_COLOR);			
			m_pRTPort[i]->m_pSegPort[j]->SetBorderStyle(1);
			m_pRTPort[i]->m_pSegPort[j]->SetShowOffSegments(FALSE);
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentSize(2);
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentSeperation(0);
			m_pRTPort[i]->m_pSegPort[j]->SetShowSign(FALSE);
			m_pRTPort[i]->m_pSegPort[j]->SetPrecision(0);
			m_pRTPort[i]->m_pSegPort[j]->SetDigitSpacing(2);
			m_pRTPort[i]->m_pSegPort[j]->SetDigitCount(4);			
			m_pRTPort[i]->m_pSegPort[j]->SetValue(10000);
			if (i == 0)
			{
				m_pRTPort[i]->m_pSegPort[j]->SetVisible(TRUE);			
			}
			else
			{
				m_pRTPort[i]->m_pSegPort[j]->SetVisible(FALSE);	
			}		
		}

		iPortNum = iPortNum + pDoc->m_pKilnBuffForCurve[i].TemperatureANum;
		for (j = iPortNum; j < (iPortNum + pDoc->m_pKilnBuffForCurve[i].PressNum); j++)
		{
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentColor(THE_OTHER_SEGMENT_COLOR);
			m_pRTPort[i]->m_pSegPort[j]->SetBackGroundColor(THE_OUT_TEMP_SEGMENTBK_COLOR);
			m_pRTPort[i]->m_pSegPort[j]->SetBorderStyle(1);
			m_pRTPort[i]->m_pSegPort[j]->SetShowOffSegments(FALSE);
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentSize(2);
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentSeperation(0);
			m_pRTPort[i]->m_pSegPort[j]->SetShowSign(TRUE);
			m_pRTPort[i]->m_pSegPort[j]->SetPrecision(0);
			m_pRTPort[i]->m_pSegPort[j]->SetDigitSpacing(2);
			m_pRTPort[i]->m_pSegPort[j]->SetDigitCount(4);
			m_pRTPort[i]->m_pSegPort[j]->SetValue(10000);
			if (i == 0)
			{
				m_pRTPort[i]->m_pSegPort[j]->SetVisible(TRUE);			
			}
			else
			{
				m_pRTPort[i]->m_pSegPort[j]->SetVisible(FALSE);	
			}
		}

		iPortNum = iPortNum + pDoc->m_pKilnBuffForCurve[i].PressNum;
		for (j = iPortNum; j < (iPortNum + pDoc->m_pKilnBuffForCurve[i].HumidityNum); j++)
		{
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentColor(THE_OTHER_SEGMENT_COLOR);
			m_pRTPort[i]->m_pSegPort[j]->SetBackGroundColor(THE_OUT_TEMP_SEGMENTBK_COLOR);
			m_pRTPort[i]->m_pSegPort[j]->SetBorderStyle(1);
			m_pRTPort[i]->m_pSegPort[j]->SetShowOffSegments(FALSE);
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentSize(2);
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentSeperation(0);
			m_pRTPort[i]->m_pSegPort[j]->SetShowSign(FALSE);
			m_pRTPort[i]->m_pSegPort[j]->SetPrecision(0);
			m_pRTPort[i]->m_pSegPort[j]->SetDigitSpacing(2);
			m_pRTPort[i]->m_pSegPort[j]->SetDigitCount(4);
			m_pRTPort[i]->m_pSegPort[j]->SetValue(10000);			
			if (i == 0)
			{
				m_pRTPort[i]->m_pSegPort[j]->SetVisible(TRUE);			
			}
			else
			{
				m_pRTPort[i]->m_pSegPort[j]->SetVisible(FALSE);	
			}
		}

		iPortNum = iPortNum + pDoc->m_pKilnBuffForCurve[i].HumidityNum;
		for (j = iPortNum; j < (iPortNum + pDoc->m_pKilnBuffForCurve[i].MotorNum); j++)
		{
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentColor(THE_OTHER_SEGMENT_COLOR);
			m_pRTPort[i]->m_pSegPort[j]->SetBackGroundColor(THE_OUT_TEMP_SEGMENTBK_COLOR);
			m_pRTPort[i]->m_pSegPort[j]->SetBorderStyle(1);
			m_pRTPort[i]->m_pSegPort[j]->SetShowOffSegments(FALSE);
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentSize(2);
			m_pRTPort[i]->m_pSegPort[j]->SetSegmentSeperation(0);
			m_pRTPort[i]->m_pSegPort[j]->SetShowSign(FALSE);
			m_pRTPort[i]->m_pSegPort[j]->SetPrecision(1);
			m_pRTPort[i]->m_pSegPort[j]->SetDigitSpacing(10);
			m_pRTPort[i]->m_pSegPort[j]->SetDigitCount(3);
			m_pRTPort[i]->m_pSegPort[j]->SetValue(100);				
			if (i == 0)
			{
				m_pRTPort[i]->m_pSegPort[j]->SetVisible(TRUE);			
			}
			else
			{
				m_pRTPort[i]->m_pSegPort[j]->SetVisible(FALSE);	
			}
		}		
	}
}

void CRealTimeView::UpdateView(void)
{
	int i;

	if (m_bFirstPaint == FALSE)
	{
		return;		
	}	

	// 需要显示新的窑数据时才更新表头数量
	if (m_iOldFurnaceNum != m_iCurrentFurnaceNum)
	{
		if (m_iOldFurnaceNum < 0)
		{
			m_iOldFurnaceNum = 0;
		}

		for (i = 0; i < m_pPortNum[m_iOldFurnaceNum]; i++)
		{
 			m_pRTPort[m_iOldFurnaceNum]->m_pSegPort[i]->ShowWindow(SW_SHOW);
 			m_pRTPort[m_iOldFurnaceNum]->m_pSegPort[i]->ShowWindow(SW_HIDE);
		}
		
		for (i = 0; i < m_pPortNum[m_iCurrentFurnaceNum]; i++)
		{
 			m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[i]->ShowWindow(SW_SHOW);
		}

		m_iOldFurnaceNum = m_iCurrentFurnaceNum;

		SendMessage(WM_PAINT, 0, 0);
	}

	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int PortIndex = 0;
	
	/************************************************************************/
	/* 实时温度表头显示                                                     */
	/************************************************************************/
	for (i = 0; i < pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureNum; i++, PortIndex++)
	{
		// 非法温度代表断线
		if ((pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].Value < (-100)) 
			|| (pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].Value > 1370))
		{
			m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetValue(10000);
			m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(THE_OUT_TEMP_SEGMENT_COLOR);
			m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetBackGroundColor(THE_OUT_TEMP_SEGMENTBK_COLOR);
		}
		else
		{
			// 显示实时温度值
			if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetValue() != pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].Value)
			{
				m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetValue(pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].Value);
			}			
			
			// 超过标准温度, 红色
//			if (pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].Value > pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].SValue)
			if (pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].Value > 700)
			{
				if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetSegmentColor() != THE_OUT_TEMP_SEGMENT_COLOR)
				{
					m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(THE_OUT_TEMP_SEGMENT_COLOR);
				}
				
				if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetBackGroundColor() != THE_OUT_TEMP_SEGMENTBK_COLOR)
				{
					m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetBackGroundColor(THE_OUT_TEMP_SEGMENTBK_COLOR);					
				}
			}
			// 未超过标准温度, 绿色
			else
			{
				if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetSegmentColor() != THE_IN_TEMP_SEGMENT_COLOR)
				{
					m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(THE_IN_TEMP_SEGMENT_COLOR);
				}
				
				if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetBackGroundColor() != THE_IN_TEMP_SEGMENTBK_COLOR)
				{
					m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetBackGroundColor(THE_OUT_TEMP_SEGMENTBK_COLOR);
				}				
			}
		}
	} /* End of for (i = 0; i < pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureNum; i++, PortIndex++) */

	/************************************************************************/
	/* 实时边温度表头显示                                                     */
	/************************************************************************/
	for (i = 0; i < pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureANum; i++, PortIndex++)
	{
		// 非法温度代表断线
		if ((pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempAObjectBuff[i].Value < (-100)) 
			|| (pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempAObjectBuff[i].Value > 1370))
		{
			m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetValue(10000);

			if (strstr(pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempAObjectBuff[i].strName, "抽") != NULL)
			{
				m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(RGB(0, 0, 255));
			}
			else if (strstr(pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempAObjectBuff[i].strName, "风") != NULL)
			{
				m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(RGB(0, 128, 128));
			}
			else
			{
				m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(THE_OUT_TEMP_SEGMENT_COLOR);
			}
//			m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(THE_OUT_TEMP_SEGMENT_COLOR);

			m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetBackGroundColor(THE_OUT_TEMP_SEGMENTBK_COLOR);
		}
		else
		{
			// 显示实时温度值
			if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetValue() != pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempAObjectBuff[i].Value)
			{
				m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetValue(pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempAObjectBuff[i].Value);
			}	
			
			if (pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempAObjectBuff[i].Value > 700)
			{
				if (strstr(pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempAObjectBuff[i].strName, "抽") != NULL)
				{
					m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(RGB(0, 0, 255));
				}
				else if (strstr(pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempAObjectBuff[i].strName, "风") != NULL)
				{
					m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(RGB(0, 128, 128));
				}
				else
				{
					if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetSegmentColor() != THE_OUT_TEMP_SEGMENT_COLOR)
					{
						m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(THE_OUT_TEMP_SEGMENT_COLOR);
					}
				}

// 				if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetSegmentColor() != THE_OUT_TEMP_SEGMENT_COLOR)
// 				{
// 					m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(THE_OUT_TEMP_SEGMENT_COLOR);
// 				}
				
				if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetBackGroundColor() != THE_OUT_TEMP_SEGMENTBK_COLOR)
				{
					m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetBackGroundColor(THE_OUT_TEMP_SEGMENTBK_COLOR);					
				}				
			}
			// 未超过标准温度, 绿色
			else
			{
				if (strstr(pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempAObjectBuff[i].strName, "抽") != NULL)
				{
					m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(RGB(0, 0, 255));
				}
				else if (strstr(pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempAObjectBuff[i].strName, "风") != NULL)
				{
					m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(RGB(0, 128, 128));
				}
				else
				{
					if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetSegmentColor() != THE_IN_TEMP_SEGMENT_COLOR)
					{
						m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(THE_IN_TEMP_SEGMENT_COLOR);
					}
				}
// 				if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetSegmentColor() != THE_IN_TEMP_SEGMENT_COLOR)
// 				{
// 					m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetSegmentColor(THE_IN_TEMP_SEGMENT_COLOR);
// 				}
				
				if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetBackGroundColor() != THE_IN_TEMP_SEGMENTBK_COLOR)
				{
					m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetBackGroundColor(THE_OUT_TEMP_SEGMENTBK_COLOR);
				}				
			}
		}
	} /* End of for (i = 0; i < pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureANum; i++, PortIndex++) */
	
	/************************************************************************/
	/* 实时压力表头显示                                                     */
	/************************************************************************/
	int pressure_val;
	for (i = 0; i < pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].PressNum; i++, PortIndex++)
	{		
		pressure_val = pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pPressObjectBuff[i].Value;

// 		if ((pressure_val < (-32768)) || (pressure_val > 32767))
// 		{
// 			pressure_val = 0xffff;
// 		}
// 		else
		{
			pressure_val = (int)(((((float)(pressure_val) * 60.0 / 32767.0) - 12.0) * 1000.0 / 48.0) - 500.0);
		}

		if ((pressure_val < (-500)) || (pressure_val > 500))
		{
			m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetValue(500);
		}
		else
		{
			if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetValue() != pressure_val)
			{
				m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetValue(pressure_val);
			}
		}
	}

	/************************************************************************/
	/* 实时湿度表头显示                                                     */
	/************************************************************************/
	int humidity_val;
	for (i = 0; i < pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].HumidityNum; i++, PortIndex++)
	{
		humidity_val = pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pHumiObjectBuff[i].Value;

// 		if ((humidity_val < (-32768)) || (humidity_val > 32767))
// 		{
// 			humidity_val = 0xffff;
// 		}
// 		else
		{
			humidity_val = (int)(((((float)(humidity_val) * 60.0 / 32767.0) - 12.0) * 100.0 / 48.0) - 0.0);
		}

		if ((humidity_val < 0) || (humidity_val > 100.0))
		{
			m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetValue(100);
		}
		else
		{
			if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetValue() != humidity_val)
			{
				m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetValue(humidity_val);
			}
		}
	}

	for (i = 0; i < pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].MotorNum; i++, PortIndex++)
	{
		if ((pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pMotorObjectBuff[i].Value < 0.0) 
			|| (pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pMotorObjectBuff[i].Value > 500.0))
		{
			m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetValue(10000);
		}
		else
		{
			// 显示实时变频器值
			if (m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->GetValue() != (pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pMotorObjectBuff[i].Value / 10.0))
			{
				m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[PortIndex]->SetValue(pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pMotorObjectBuff[i].Value / 10.0);
			}
		}
	}

	// 重绘表头
	for (i = 0; i < m_pPortNum[m_iCurrentFurnaceNum]; i++)
	{
		m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[i]->RepaintAll();
	}
}

void CRealTimeView::ActiveView(int iNewFurnaceNum)
{
	m_iCurrentFurnaceNum = iNewFurnaceNum;

	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	CString str;

	switch (pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].type)
	{
	case 0:
		str.Format("%d# <Moblie Kiln> Realtime Data", m_iCurrentFurnaceNum + 1);
		break;
	case 1:
		str.Format("%d# <Drying Kiln> Realtime Data", m_iCurrentFurnaceNum + 1);
		break;
	case 2:
		str.Format("%d# <Through Kiln> Realtime Data", m_iCurrentFurnaceNum + 1);
		break;
	case 3:
		str.Format("%d# <Roasting Kiln> Realtime Data", m_iCurrentFurnaceNum + 1);
		break;
	default:
		break;
	}
	
	pMainFrame->SetFrameCaption(str);
}

void CRealTimeView::DrawCaption(CDC *pDC)
{
	CRect ClientRect = m_MaxClientRect;

	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	
	pDC->SetBkMode(TRANSPARENT);

	// 创建字体
	CFont Font, *pOldFont;
	Font.CreateFont(60, 0, 0, 0, FW_BOLD, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "楷体_GB2312");
	pOldFont = pDC->SelectObject(&Font);

	// 显示提示文字且设置文字颜色
	ClientRect.top = 20;
	ClientRect.bottom = 80;
	pDC->SetTextColor(THE_CAPTION_FT_COLOR);
	pDC->DrawText(pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].strCaption, &ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// 释放字体内存操作
	pDC->SelectObject(pOldFont);
	Font.DeleteObject();

	//////////////////////////////
	Font.CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Consolas");
	pOldFont = pDC->SelectObject(&Font);

	ClientRect.top = m_MaxClientRect.bottom - 60;
	ClientRect.bottom = m_MaxClientRect.bottom - 30;
	ClientRect.left = m_MaxClientRect.Width() / 8;//250;
	pDC->SetTextColor(THE_CAPTION_FT_COLOR);
	pDC->DrawText("Drying Segment", &ClientRect, DT_VCENTER | DT_SINGLELINE);

	ClientRect.top = m_MaxClientRect.bottom - 60;
	ClientRect.bottom = m_MaxClientRect.bottom - 30;
	ClientRect.left = m_MaxClientRect.Width() * 40 / 100;//675;
	pDC->SetTextColor(THE_CAPTION_FT_COLOR);
	pDC->DrawText("Preheat Segment", &ClientRect, DT_VCENTER | DT_SINGLELINE);

	ClientRect.top = m_MaxClientRect.bottom - 60;
	ClientRect.bottom = m_MaxClientRect.bottom - 30;
	ClientRect.left = m_MaxClientRect.Width() * 6 / 10;//1200;
	pDC->SetTextColor(THE_CAPTION_FT_COLOR);
	pDC->DrawText("Roasting Segment", &ClientRect, DT_VCENTER | DT_SINGLELINE);

	ClientRect.top = m_MaxClientRect.bottom - 60;
	ClientRect.bottom = m_MaxClientRect.bottom - 30;
	ClientRect.left = m_MaxClientRect.Width() * 9 / 11;//1625;
	pDC->SetTextColor(THE_CAPTION_FT_COLOR);
	pDC->DrawText("Cooling Segment", &ClientRect, DT_VCENTER | DT_SINGLELINE);

	// 释放字体内存操作
	pDC->SelectObject(pOldFont);
	Font.DeleteObject();
	//////////////////////////////


//	Font.CreateFont(30, 9, 0, 0, FW_BOLD, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "楷体_GB2312");
	Font.CreateFont(13, 0, 0, 0, FW_BOLD, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Consolas");
	pOldFont = pDC->SelectObject(&Font);

	int height;
	CString str;
	WINDOWPLACEMENT lpWnd;
	for (int i = 0; i < m_pPortNum[m_iCurrentFurnaceNum]; i++)
	{
		lpWnd.length = sizeof(WINDOWPLACEMENT);
		m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[i]->GetWindowPlacement(&lpWnd);

		height = lpWnd.rcNormalPosition.bottom - lpWnd.rcNormalPosition.top + 1;
		ClientRect = lpWnd.rcNormalPosition;

		ClientRect.left = ClientRect.left - 16;
		ClientRect.right = ClientRect.right + 16;
		ClientRect.top = ClientRect.top - (height * 3 / 4) - 10;
		ClientRect.bottom = ClientRect.bottom - (height * 3 / 4) - 10;
		
		pDC->SetTextColor(THE_CAPTION_FT_COLOR);
		str.Empty();

		// 显示温度测温点指示
		if (i < pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureNum)
		{
			if (pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].type == 3)
			{
				if (strcmp(pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].strName, "自动排号") != 0)
				{
					str.Format("%s(℃)", pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].strName);
				}
				else
				{
					str.Format("%dm(℃)", pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].CarIdex);
				}
			}
			else
			{
				str.Format("%d#(℃)", pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempObjectBuff[i].CarIdex + 1);
			}
			
			pDC->DrawText(str, &ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else if (i < (pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureNum 
					  + pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureANum))
		{
			str.Format("%s(℃)", pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pTempAObjectBuff[i - pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureNum].strName);
			pDC->DrawText(str, &ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		// 显示压力测试点指示
		else if (i < (pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureNum 
					  + pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureANum
					  + pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].PressNum))
		{
			str.Format("%s(Pa)", pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pPressObjectBuff[i 
																								- pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureNum 
																								- pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureANum].strName);
			pDC->DrawText(str, &ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		// 显示湿度测试点指示
		else if (i < (pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureNum 
					   + pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureANum
					   + pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].PressNum 
					   + pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].HumidityNum))
		{
			str.Format("%s(%%RH)", pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pHumiObjectBuff[i 
																							   - pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureNum 
																							   - pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureANum 
																							   - pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].PressNum].strName);
			pDC->DrawText(str, &ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		// 显示变频器测试点指示
		else if (i < (pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureNum 
					   + pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureANum
					   + pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].PressNum 
					   + pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].HumidityNum 
					   + pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].MotorNum))
		{
			str.Empty();
			str.Format("%s(Hz)", pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].m_pMotorObjectBuff[i 
																									- pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureNum 
																									- pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].TemperatureANum 
																									- pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].PressNum 
																									- pDoc->m_pKilnBuffForCurve[m_iCurrentFurnaceNum].HumidityNum].strName);
			pDC->DrawText(str, &ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	// 释放字体内存操作
	pDC->SelectObject(pOldFont);
	Font.DeleteObject();

	// 表头重绘
	for (i = 0; i < m_pPortNum[m_iCurrentFurnaceNum]; i++)
	{
		m_pRTPort[m_iCurrentFurnaceNum]->m_pSegPort[i]->RepaintAll();
	}
} 

void CRealTimeView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (m_bFirstPaint == FALSE)
	{
		InitSegPort();
		UpdateView();
		m_bFirstPaint = TRUE;
	}

	GetClientRect(m_MaxClientRect);

 	CBrush brBack, *pOldcrBack;
	// 创建刷子并绘制填充矩形, 尺寸为父窗口的客户区大小
	brBack.CreateSolidBrush(THE_BACKGROUD_COLOR);
	m_MemDcDraw.FillRect(&m_MaxClientRect, &brBack);	
	pOldcrBack = m_MemDcDraw.SelectObject(&brBack);	
	
	m_MemDcDraw.SelectObject(pOldcrBack);
	brBack.DeleteObject();	

	// 画分区线: 烘干 预热 焙烧 冷却
	CPen peLine, *pOldpeLine;
	peLine.CreatePen(PS_DASHDOT, 1, RGB(128, 255, 128));
	pOldpeLine = m_MemDcDraw.SelectObject(&peLine);

	MoveToEx(m_MemDcDraw.m_hDC, 720, 100, NULL);//520
	LineTo(m_MemDcDraw.m_hDC, 720, m_MaxClientRect.bottom - 80 + 80);

	MoveToEx(m_MemDcDraw.m_hDC, 1080, 100, NULL);//895
	LineTo(m_MemDcDraw.m_hDC, 1080, m_MaxClientRect.bottom - 80 + 80);

	MoveToEx(m_MemDcDraw.m_hDC, 1560, 100, NULL);//1520
	LineTo(m_MemDcDraw.m_hDC, 1560, m_MaxClientRect.bottom - 80 + 80);

	m_MemDcDraw.SelectObject(pOldpeLine);
	peLine.DeleteObject();
	////////////////////////////////////////////////////

	// 重绘标题
	DrawCaption(&m_MemDcDraw);
	
	// 送入显存
	CDC *pDC = GetDC();
	pDC->StretchBlt(m_MaxClientRect.left, m_MaxClientRect.top, m_MaxClientRect.Width(), m_MaxClientRect.Height(), &m_MemDcDraw, m_MaxClientRect.left, m_MaxClientRect.top, m_MaxClientRect.Width(), m_MaxClientRect.Height(), SRCCOPY);
	ReleaseDC(pDC);
	
	// Do not call CView::OnPaint() for painting messages
}

BOOL CRealTimeView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
// 	cs.cx = GetSystemMetrics(SM_CXFULLSCREEN); 
// 	cs.cy = GetSystemMetrics(SM_CYFULLSCREEN);

	return CView::PreCreateWindow(cs);
}

void CRealTimeView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here	
}

BOOL CRealTimeView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CView::OnEraseBkgnd(pDC);
}

void CRealTimeView::OnMove(int x, int y) 
{
	CView::OnMove(x, y);
	
	// TODO: Add your message handler code here
}

void CRealTimeView::OnDestroy() 
{
	CView::OnDestroy();

	KillTimer(0);

	if (m_bFirstPaint == FALSE)
	{
		delete [] m_pPortNum;
		return;
	}
	
	for (int i = 0; i < m_iKilnNum; i++)
	{
		// 释放每条窑的表头资源
		for (int j = 0; j < m_pPortNum[i]; j++)
		{
			delete m_pRTPort[i]->m_pSegPort[j];
		}

		// 释放每条窑的表头指针列表
		delete [] m_pRTPort[i]->m_pSegPort;

		// 释放每条窑的实时数据结构
		delete m_pRTPort[i];
	}

	delete [] m_pPortNum;

	// 释放窑的实时数据结构指针列表
	delete [] m_pRTPort;

	DeleteObject(m_HbtMap);
	DeleteDC(m_MemDcDraw);
}

void CRealTimeView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	UpdateView();				// 自己定时更新
	
	CView::OnTimer(nIDEvent);
}

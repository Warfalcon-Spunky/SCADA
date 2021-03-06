// HisGridView.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "HisGridView.h"
#include "TimeQueryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define THE_GRID_BACKGROUND_COLOR	RGB(255, 255, 224)
#define THE_VIEW_BACKGROUND_COLOR	RGB(219, 219, 219)

#define THE_FIXED_ROW_FT_COLOR		RGB(127, 127, 127)
#define THE_FIXED_COL_FT_COLOR		RGB(127, 127, 127)

#define THE_IN_GRID_FT_COLOR		RGB(0, 0, 255)
#define THE_OUT_GRID_FT_COLOR		RGB(255, 0, 0)
#define THE_ERR_GRID_FT_COLOR		RGB(255, 0, 255)

#define THE_TEMP_FRONT_NUM			2						// 除测试点外的字段数(序号和存储时间)
#define THE_DEFAULT_CELL_HEIGHT		28						// 默认的表格高度
#define THE_DEFAULT_CELL_WIDTH		150						// 默认的表格宽度

/////////////////////////////////////////////////////////////////////////////
// CHisGridView

IMPLEMENT_DYNCREATE(CHisGridView, CView)

CHisGridView::CHisGridView()
{
	m_iActiveFurnaceNum = 0;
	m_iSelectRow = 1;

	m_pGrid = NULL;
}

CHisGridView::~CHisGridView()
{
}


BEGIN_MESSAGE_MAP(CHisGridView, CView)
	//{{AFX_MSG_MAP(CHisGridView)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_INSERT_AND_APP, OnInsertAndApp)
	ON_COMMAND(IDM_INSERT, OnInsert)
	ON_COMMAND(IDM_QUERY_HIS, OnQueryHistory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHisGridView drawing

void CHisGridView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CHisGridView diagnostics

#ifdef _DEBUG
void CHisGridView::AssertValid() const
{
	CView::AssertValid();
}

void CHisGridView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHisGridView message handlers

void CHisGridView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	if (pDoc->m_bDBOpen == FALSE)		// 相关数据库打开和数据初始化为成功
	{
		PostQuitMessage(1);
		return;
	}

	m_iKilnNum = pDoc->m_iKilnNum;
	
	CRect rect;
	GetClientRect(rect);

	m_pGrid = new CIGridCtrl;
	m_pGrid->Create(rect, this, 5001, WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);

	CFont Font;
	Font.CreateFont(14, 7, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "宋体");
	VERIFY(Font.GetLogFont(&m_lfFixCellFont));
    m_lfFixCellFont.lfWeight = FW_BOLD;
	Font.DeleteObject();

	Font.CreateFont(16, 0, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Arail narrow");
	VERIFY(Font.GetLogFont(&m_lfCellFont));
    m_lfCellFont.lfWeight = FW_BOLD;
	Font.DeleteObject();

	//////////////////////////////////////////////////////////////////////////
	// 建立此背板是为了消除上一个视图留下来的残影
	//////////////////////////////////////////////////////////////////////////
	BITMAPINFO bmpinfo;
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = GetSystemMetrics(SM_CXSCREEN);	// 创建最大的图片, 尺寸宽度为屏幕尺寸宽度
	bmpinfo.bmiHeader.biHeight = GetSystemMetrics(SM_CYSCREEN);	// 创建最大的图片, 尺寸高度为屏幕尺寸宽度
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 32;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = 0;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;
	
	CDC *pDC = GetDC();
	m_MemDcDraw.CreateCompatibleDC(pDC);												// 创建兼容画布, 关联父窗口
	m_HbtMap = CreateDIBSection(m_MemDcDraw, &bmpinfo, DIB_RGB_COLORS, NULL, NULL, 0);	// 创建与设备无关的内存
	m_MemDcDraw.SelectObject(m_HbtMap);													// 选中指定内存
	m_MemDcDraw.SetBkMode(TRANSPARENT);													// 背景设置为透明
	ReleaseDC(pDC);
}

void CHisGridView::ActiveView(int iFurnaceNum)
{
	m_iActiveFurnaceNum = iFurnaceNum;

	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	CHisCurveView *pHisCurve = ((CSpliteFrame *)(pMainFrame->m_ViewPoint[3]))->GetHisCurveViewPoint();	

	CString str;
	
	switch (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].type)
	{
	case 0:
		pHisCurve->SetCurveType(0);

		str.Format("%d# <Mobile Kiln> History+Curve", m_iActiveFurnaceNum + 1);
		break;
	case 1:
		pHisCurve->SetCurveType(0);

		str.Format("%d# <Drying Kiln> History+Curve", m_iActiveFurnaceNum + 1);
		break;
	case 2:
		pHisCurve->SetCurveType(0);
		
		str.Format("%d# <Through Kiln> History+Curve", m_iActiveFurnaceNum + 1);
		break;
	case 3:
		pHisCurve->SetCurveType(1);

		str.Format("%d# <Roasting Kiln> History+Curve", m_iActiveFurnaceNum + 1);
		break;
	default:
		break;
	}
	
	pHisCurve->SetTotalCars(pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TotalKiln);	// 设置曲线的总车位
	pHisCurve->SetCurveCaption("");												// 清除标题
	pHisCurve->SetCurvePoint(-1, 0, 0);											// 清除上一次的数据
	pMainFrame->SetFrameCaption(str);

	SetHistoryGridOnly();
}

void CHisGridView::UpdateView(void)
{
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	if (pDoc->m_strHistoryQuery.IsEmpty())
	{
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->SetSort("[id] DESC");
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->SetFilter("");
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Query();
	}
	else
	{
		CString str;
		str.Format("strftime(\'%%Y-%%m-%%d %%H:%%M:%%S\',save_time)>\'%s\'", pDoc->m_strHistoryQuery);
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->SetSort("[id] DESC");
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->SetFilter(str);
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Query();
	}

	// 获取记录条数, 以便设置表格参数
	int iRecordNum = pDoc->m_pHisTbl[m_iActiveFurnaceNum]->GetCount();

	// 更新行列数
	m_pGrid->SetRowCount(iRecordNum + 1);
	m_pGrid->SetColumnCount(pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum + 
							pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureANum +
							pDoc->m_pKilnBuff[m_iActiveFurnaceNum].PressNum + 
							pDoc->m_pKilnBuff[m_iActiveFurnaceNum].HumidityNum + 
							pDoc->m_pKilnBuff[m_iActiveFurnaceNum].MotorNum + 1 + 2);
}

void CHisGridView::SetHistoryGridOnly(void)
{
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	if (pDoc->m_strHistoryQuery.IsEmpty())
	{
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->SetSort("[id] DESC");
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->SetFilter("");
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Query();
	}
	else
	{
		CString str;
 		str.Format("strftime(\'%%Y-%%m-%%d %%H:%%M:%%S\',save_time)>\'%s\'", pDoc->m_strHistoryQuery);
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->SetSort("[id] DESC");
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->SetFilter(str);
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Query();
	}

	// 获取记录条数, 以便设置表格参数
	int iRecordNum = pDoc->m_pHisTbl[m_iActiveFurnaceNum]->GetCount();

	// 设置网格的背景颜色
	m_pGrid->GetDefaultCell(FALSE, FALSE)->SetBackClr(THE_GRID_BACKGROUND_COLOR);
	// 设置视图的背景颜色
	m_pGrid->SetGridBkColor(THE_VIEW_BACKGROUND_COLOR);

	m_pGrid->SetDefCellHeight(THE_DEFAULT_CELL_HEIGHT);
	m_pGrid->SetDefCellWidth(THE_DEFAULT_CELL_WIDTH);
	
	// 设置虚拟方式
	m_pGrid->SetVirtualMode(TRUE);
	// 设置固定表格头的行列数
	m_pGrid->SetFixedRowCount(1);
	m_pGrid->SetFixedColumnCount(1);

	m_pGrid->SetRowCount(iRecordNum + 1);

	int iRecordColNum = pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum + 
						pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureANum +
						pDoc->m_pKilnBuff[m_iActiveFurnaceNum].PressNum + 
						pDoc->m_pKilnBuff[m_iActiveFurnaceNum].HumidityNum + 
						pDoc->m_pKilnBuff[m_iActiveFurnaceNum].MotorNum + 1 + 2;
	m_pGrid->SetColumnCount(iRecordColNum);

	// 由于历史数据库的数据记录比较多不使用自动调整宽度方式
	m_pGrid->SetColumnWidth(0, 30);				// 单独设置序号列宽度
	m_pGrid->SetColumnWidth(1, 60);				// 单独设置记录号列宽度
	m_pGrid->SetColumnWidth(2, 200);			// 单独设置时间列宽度
	
	m_pGrid->SetEditable(FALSE);				// 不允许编辑内容
	m_pGrid->EnableSelection(TRUE);				// 允许选择单元
	
	m_pGrid->SetRowResize(FALSE);				// 不允许行宽调整
	m_pGrid->SetColumnResize(TRUE);				// 允许列宽调整	
	
	m_pGrid->SetFixedRowSelection(TRUE);		// 允许行选择
    m_pGrid->SetFixedColumnSelection(FALSE);	// 不允许列选择
	m_pGrid->SetTrackFocusCell(FALSE);			// 固定单元不能有焦点

	m_pGrid->SendMessageToParent(1, 1, GVN_SELCHANGED);

	m_iSelectRow = 1;
} 

BOOL CHisGridView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// 网格空间发出的消息
	if (wParam == (WPARAM)m_pGrid->GetDlgCtrlID())
	{
		*pResult = 1;

		GV_DISPINFO *pDispInfo = (GV_DISPINFO *)lParam;

		// 遍历显示消息
		if (GVN_GETDISPINFO == pDispInfo->hdr.code)			
		{
			SetGridItem(pDispInfo);
			return TRUE;
		}
		// 起始单元选择消息
		else if (GVN_SELCHANGING == pDispInfo->hdr.code)
		{
			// 数据单元
//			if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
			{
				m_iSelectRow = pDispInfo->item.row;
			}
			
			return TRUE;
		}
		// 结束单元选择消息
		else if (GVN_SELCHANGED == pDispInfo->hdr.code)
		{
			SetCurveItem(pDispInfo);

			return TRUE;
		}
	}
	
	return CView::OnNotify(wParam, lParam, pResult);
}

void CHisGridView::SetGridItem(GV_DISPINFO *pDispInfo)
{
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int temp_idx;
	int temp;
	double f_temp;
	MYDB_ITEM *pItems = pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Items();

	// 虚模式下表格内容居中
	pDispInfo->item.nFormat = DT_CENTER | DT_VCENTER;

	// 数据单元
	if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
	{
//		pDispInfo->item.row += 10;		// 改变显示行偏移

		pDispInfo->item.lfFont = m_lfCellFont;
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Move(pDispInfo->item.row);	// 移动取记录表头到指定位置, move函数的参数从1开始

		switch (pItems[pDispInfo->item.col - 1].type)
		{
		case MYDB_TYPE_TEXT:
			pDispInfo->item.crFgClr = THE_IN_GRID_FT_COLOR;
			pDispInfo->item.strText.Format("%s", *((SQLiteString *)(pDoc->m_pHisTbl[m_iActiveFurnaceNum]->m_pColsName[pDispInfo->item.col - 1])));
			break;

		case MYDB_TYPE_INT:
			temp = *((int *)(pDoc->m_pHisTbl[m_iActiveFurnaceNum]->m_pColsName[pDispInfo->item.col - 1]));
			if (temp >= 10000)
			{
				temp = 0;
				pDispInfo->item.crFgClr = THE_ERR_GRID_FT_COLOR;
			}
			
			pDispInfo->item.strText.Format("%d", temp);
			break;

		case MYDB_TYPE_DOUBLE:
			f_temp = *((double *)(pDoc->m_pHisTbl[m_iActiveFurnaceNum]->m_pColsName[pDispInfo->item.col - 1]));
			if (f_temp >= 10000.0)
			{
				f_temp = 0.0;
				pDispInfo->item.crFgClr = THE_OUT_GRID_FT_COLOR;
			}

			pDispInfo->item.strText.Format("%3.2f", f_temp);
			break;

		case MYDB_TYPE_BLOB:
			break;
		default:
			break;
		}	
	}
	// 表头单元
	else
	{
		pDispInfo->item.lfFont = m_lfFixCellFont;

		// 行表头单元设置
		if (pDispInfo->item.row == 0 && pDispInfo->item.col > 0)
		{	
			// 列表头最小从1开始, 因为0被固定表格占有
			if (pDispInfo->item.col == 1)
			{
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("Num");
			}
			else if (pDispInfo->item.col == 2)
			{
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("Save Time");
			}
			else if (pDispInfo->item.col <= (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum 
											+ THE_TEMP_FRONT_NUM))
			{			
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;

				temp_idx = pDispInfo->item.col - THE_TEMP_FRONT_NUM - 1;
				if (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].type == 3)
				{
					if (strcmp(pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[temp_idx].strName, "Automatic Numbering") != 0)
					{
						pDispInfo->item.strText.Format("%s TEMP(℃)", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[temp_idx].strName);
					}
					else
					{
						pDispInfo->item.strText.Format("%03d m TEMP(℃)", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[temp_idx].CarIdex);
					}
				}
				else
				{
					pDispInfo->item.strText.Format("%02d# POS TEMP(℃)", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[temp_idx].CarIdex + 1);
				}
			}
			else if (pDispInfo->item.col <= (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum 
											+ pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureANum 
											+ THE_TEMP_FRONT_NUM))
			{			
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;

				temp_idx = pDispInfo->item.col - pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum 
											   - THE_TEMP_FRONT_NUM - 1;
				pDispInfo->item.strText.Format("%s Temperature", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempAObjectBuff[temp_idx].strName);
			}
			else if (pDispInfo->item.col <= (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum 
											+ pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureANum 
											+ pDoc->m_pKilnBuff[m_iActiveFurnaceNum].PressNum
											+ THE_TEMP_FRONT_NUM))
			{
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;

				temp_idx = pDispInfo->item.col - pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum 
											   - pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureANum 
											   - THE_TEMP_FRONT_NUM - 1;
				pDispInfo->item.strText.Format("%s Pressure", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pPressObjectBuff[temp_idx].strName);
			}
			else if (pDispInfo->item.col <= (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum 
											+ pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureANum 
											+ pDoc->m_pKilnBuff[m_iActiveFurnaceNum].PressNum 
											+ pDoc->m_pKilnBuff[m_iActiveFurnaceNum].HumidityNum
											+ THE_TEMP_FRONT_NUM))
			{
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;

				temp_idx = pDispInfo->item.col - pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum 
											   - pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureANum 
											   - pDoc->m_pKilnBuff[m_iActiveFurnaceNum].PressNum 
											   - THE_TEMP_FRONT_NUM - 1;
				pDispInfo->item.strText.Format("%s Humidity", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pHumiObjectBuff[temp_idx].strName);				
			}
			else if (pDispInfo->item.col <= (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum 
											+ pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureANum 
											+ pDoc->m_pKilnBuff[m_iActiveFurnaceNum].PressNum 
											+ pDoc->m_pKilnBuff[m_iActiveFurnaceNum].HumidityNum 
											+ pDoc->m_pKilnBuff[m_iActiveFurnaceNum].MotorNum
											+ THE_TEMP_FRONT_NUM))
			{
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;

				temp_idx = pDispInfo->item.col - pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum 
											   - pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureANum 
											   - pDoc->m_pKilnBuff[m_iActiveFurnaceNum].PressNum 
											   - pDoc->m_pKilnBuff[m_iActiveFurnaceNum].HumidityNum 
											   - THE_TEMP_FRONT_NUM - 1;
				pDispInfo->item.strText.Format("%s Motor", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pMotorObjectBuff[temp_idx].strName);
			}
		}
		// 列表头单元设置
		else if (pDispInfo->item.col == 0 && pDispInfo->item.row > 0)
		{
			pDispInfo->item.crFgClr = THE_FIXED_ROW_FT_COLOR;
			pDispInfo->item.strText.Format("%d", pDispInfo->item.row);	
		}
	}
}

// 显示指定记录的曲线
void CHisGridView::SetCurveItem(GV_DISPINFO *pDispInfo)
{
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	// 数据单元有效
	if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
	{
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Move(pDispInfo->item.row);	// 移动取记录表头到指定位置, move函数的参数从1开始		
		
		// 获得曲线指针
		CHisCurveView *pHisCurve = ((CSpliteFrame *)(pMainFrame->m_ViewPoint[pMainFrame->m_iTopViewID]))->GetHisCurveViewPoint();	

		CString str;

		// 绘制标题
		switch (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].type)
		{
		case 0:
			pHisCurve->SetCurveType(0);

			str.Format("%d# <Mobile Kiln>  ", m_iActiveFurnaceNum + 1);
			break;
		case 1:
			pHisCurve->SetCurveType(0);

			str.Format("%d# <Drying Kiln>  ", m_iActiveFurnaceNum + 1);
			break;
		case 2:
			pHisCurve->SetCurveType(0);

			str.Format("%d# <Through Kiln>  ", m_iActiveFurnaceNum + 1);
			break;
		case 3:
			pHisCurve->SetCurveType(1);

			str.Format("%d# <Roasting Kiln>  ", m_iActiveFurnaceNum + 1);
			break;
		default:
			break;
		}

		pHisCurve->SetCurveCaption(str + *((SQLiteString *)(pDoc->m_pHisTbl[m_iActiveFurnaceNum]->m_pColsName[1])));

		pHisCurve->SetCurvePoint(-1, 0, 0);									// 清除上一次的数据

		int car_loc_offset;
		if (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].type == 3)
		{
			car_loc_offset = 0;
		}
		else
		{
			car_loc_offset = 1;
		}

		for (int i = 0; i < pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum; i++)
		{
			// 设置实际曲线
			pHisCurve->SetCurvePoint(0, 
									 pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[i].CarIdex + car_loc_offset, 
									 *((int *)(pDoc->m_pHisTbl[m_iActiveFurnaceNum]->m_pColsName[THE_TEMP_FRONT_NUM + i])));
			
			// 设置标准曲线
			pHisCurve->SetCurvePoint(1, 
									 pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[i].CarIdex + car_loc_offset, 
									 pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[i].SValue);
		}
		
		pHisCurve->CurveUpdate();			// 更新曲线
	}
}

void CHisGridView::OnDestroy() 
{
	CView::OnDestroy();
	
	delete m_pGrid;
	
	DeleteObject(m_HbtMap);
	DeleteDC(m_MemDcDraw);
}

void CHisGridView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush brBack, *pOldcrBack;

	CRect rect;
	GetClientRect(rect);
	
	// 创建刷子并绘制填充矩形, 尺寸为父窗口的客户区大小
	brBack.CreateSolidBrush(RGB(219, 219, 219));
	m_MemDcDraw.FillRect(&rect, &brBack);	
	pOldcrBack = m_MemDcDraw.SelectObject(&brBack);	
	
	m_MemDcDraw.SelectObject(pOldcrBack);
	brBack.DeleteObject();	

// 	CDC *pDC = GetDC();
// 	pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &m_MemDcDraw, rect.left, rect.top, rect.Width(), rect.Height(), SRCCOPY);
// 	ReleaseDC(pDC);

	m_pGrid->MoveWindow(rect, TRUE);
	
	// Do not call CView::OnPaint() for painting messages
}

void CHisGridView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu menuPopup;
	if (menuPopup.CreatePopupMenu())
	{
		CString str;

		//向菜单 menuPopup 中添加菜单项
//		str.Format("添加并应用%d号记录为标准曲线", m_iSelectRow);		
		str.Format("add and apply %d# recorder to standard curve", m_iSelectRow);
		menuPopup.AppendMenu(MF_STRING, IDM_INSERT_AND_APP, str);

//		str.Format("仅添加%d号记录到标准曲线库", m_iSelectRow);
		str.Format("add %d# recorder to standard curve", m_iSelectRow);
		menuPopup.AppendMenu(MF_STRING, IDM_INSERT, str);

//		str.Format("通过时间查询历史数据");
		str.Format("Query history data by time");
		menuPopup.AppendMenu(MF_STRING, IDM_QUERY_HIS, str);

		//显示弹出式菜单,并对用户选择的菜单项作出反应
		menuPopup.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}
}

// 将选中的历史数据记录插入标准库并应用
void CHisGridView::OnInsertAndApp(void)
{
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int i;
	int iSTemperatureNum = pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum;

	int *pTemperatureBuff = new int[iSTemperatureNum];			// 创建缓冲区

	pDoc->m_pHisTbl[m_iActiveFurnaceNum]->SetSort("[id] DESC");
	pDoc->m_pHisTbl[m_iActiveFurnaceNum]->SetFilter("");
	pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Query();
	pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Move(m_iSelectRow);
	for (i = 0; i < iSTemperatureNum; i++)
	{
		pTemperatureBuff[i] = *((int *)(pDoc->m_pHisTbl[m_iActiveFurnaceNum]->m_pColsName[i + 2]));
	}

	pDoc->AppendStandardRecord(m_iActiveFurnaceNum, pTemperatureBuff, TRUE);
	

	/************************************************************************/
	/* 将选中的记录显示在曲线上                                             */
	/************************************************************************/
	CHisCurveView *pHisCurve = ((CSpliteFrame *)(pMainFrame->m_ViewPoint[pMainFrame->m_iTopViewID]))->GetHisCurveViewPoint();	

	/************************************************************************/
	/* 绘制曲线标题                                                         */
	/************************************************************************/
	CString str;
	switch (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].type)
	{
	case 0:
		pHisCurve->SetCurveType(0);
		str.Format("%d# <Mobile Kiln>  ", m_iActiveFurnaceNum + 1);
		break;
	case 1:
		pHisCurve->SetCurveType(0);
		str.Format("%d# <Drying Kiln>  ", m_iActiveFurnaceNum + 1);
		break;
	case 2:
		pHisCurve->SetCurveType(0);
		str.Format("%d# <Through Kiln>  ", m_iActiveFurnaceNum + 1);
		break;
	case 3:
		pHisCurve->SetCurveType(1);
		str.Format("%d# <Roasting Kiln>  ", m_iActiveFurnaceNum + 1);
		break;
	default:
		break;
	}

	pHisCurve->SetCurveCaption(str + *((SQLiteString *)(pDoc->m_pHisTbl[m_iActiveFurnaceNum]->m_pColsName[1])));

	pHisCurve->SetCurvePoint(-1, 0, 0);								// 清除上一次的数据

	int car_loc_offset;
	if (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].type == 3)
	{
		car_loc_offset = 0;
	}
	else
	{
		car_loc_offset = 1;
	}
	
	for (i = 0; i < iSTemperatureNum; i++)
	{
		// 设置实际曲线=标准曲线
		pHisCurve->SetCurvePoint(0, 
								 pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[i].CarIdex + car_loc_offset, 
								 pTemperatureBuff[i]);
		
		// 设置标准曲线=实际曲线
		pHisCurve->SetCurvePoint(1, 
								 pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[i].CarIdex + car_loc_offset, 
								 pTemperatureBuff[i]);
	}
	pHisCurve->CurveUpdate();			// 更新曲线		
	
	delete [] pTemperatureBuff;
}

void CHisGridView::OnInsert(void)
{
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int i;
	int iSTemperatureNum = pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum;

	int *pTemperatureBuff = new int[iSTemperatureNum];			// 创建缓冲区

	pDoc->m_pHisTbl[m_iActiveFurnaceNum]->SetSort("[id] DESC");
	pDoc->m_pHisTbl[m_iActiveFurnaceNum]->SetFilter("");
	pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Query();
	pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Move(m_iSelectRow);
	for (i = 0; i < iSTemperatureNum; i++)
	{
		pTemperatureBuff[i] = *((int *)(pDoc->m_pHisTbl[m_iActiveFurnaceNum]->m_pColsName[i + 2]));
	}
	
	pDoc->AppendStandardRecord(m_iActiveFurnaceNum, pTemperatureBuff, FALSE);
	
	delete [] pTemperatureBuff;
}

void CHisGridView::OnQueryHistory(void)
{
	CTimeQueryDlg dlg;

	dlg.DoModal();

	SetHistoryGridOnly();
}
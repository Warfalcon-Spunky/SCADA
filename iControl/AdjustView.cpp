// AdjustView.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "AdjustView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdjustView
#define THE_GRID_BACKGROUND_COLOR	RGB(255, 255, 224)
#define THE_VIEW_BACKGROUND_COLOR	RGB(219, 219, 219)

#define THE_FIXED_ROW_FT_COLOR		RGB(127, 127, 127)
#define THE_FIXED_COL_FT_COLOR		RGB(127, 127, 127)

#define THE_IN_GRID_FT_COLOR		RGB(0, 0, 255)
#define THE_OUT_GRID_FT_COLOR		RGB(255, 0, 0)

#define THE_TEMP_FRONT_NUM			2						// 除测试点外的字段数(序号和设置时间)
#define THE_DEFAULT_CELL_HEIGHT		28						// 默认的表格高度
#define THE_DEFAULT_CELL_WIDTH		150						// 默认的表格宽度

#define THE_BOTTOM_GAP_HEIGHT		80

IMPLEMENT_DYNCREATE(CAdjustView, CView)

CAdjustView::CAdjustView()
{
	m_iActiveFurnaceNum = 0;
	m_iSelectRow        = 0;
	m_pGrid             = NULL;
}

CAdjustView::~CAdjustView()
{
}


BEGIN_MESSAGE_MAP(CAdjustView, CView)
	//{{AFX_MSG_MAP(CAdjustView)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdjustView drawing

void CAdjustView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdjustView diagnostics

#ifdef _DEBUG
void CAdjustView::AssertValid() const
{
	CView::AssertValid();
}

void CAdjustView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdjustView message handlers

void CAdjustView::OnInitialUpdate() 
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
	m_pGrid->Create(rect, this, 5004, WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
	
	CFont Font;
	Font.CreateFont(14, 7, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "宋体");
	VERIFY(Font.GetLogFont(&m_lfFixCellFont));
    m_lfFixCellFont.lfWeight = FW_BOLD;
	Font.DeleteObject();
	
	Font.CreateFont(16, 0, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Arail narrow");
	VERIFY(Font.GetLogFont(&m_lfCellFont));
    m_lfCellFont.lfWeight = FW_BOLD;
	Font.DeleteObject();
}

void CAdjustView::SetAdjustActiveView(int iFurnace)
{
	m_iActiveFurnaceNum = iFurnace;
	
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	
	CString str;
	
	switch (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].type)
	{
	case 0:
		str.Format("%d# <Mobile Kiln>Adjust Table", m_iActiveFurnaceNum + 1);
		break;
	case 1:
		str.Format("%d# <Drying Kiln>Adjust Table", m_iActiveFurnaceNum + 1);
		break;
	case 2:
		str.Format("%d# <Through Kiln>Adjust Table", m_iActiveFurnaceNum + 1);
		break;
	case 3:
		str.Format("%d# <Roasting Kiln>Adjust Table", m_iActiveFurnaceNum + 1);
		break;
	default:
		break;
	}
	
	pMainFrame->SetFrameCaption(str);

	SetAdjustGrid();
}

void CAdjustView::SetAdjustGrid(void)
{
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	CString str;

	/************************************************************************/
	/* 获取温度测试点的数量                                                 */
	/************************************************************************/
	str.Empty();
	str.Format("[kiln_idx]=%d and [chn_type]=0", m_iActiveFurnaceNum + 1);
	pDoc->m_pChannelCfgTbl->SetFilter(str);
	pDoc->m_pChannelCfgTbl->Query();
	int iTemperatureRecordNum = pDoc->m_pChannelCfgTbl->GetCount();		// 获取记录条数, 以便设置表格参数

	/************************************************************************/
	/* 获取边温度测试点的数量                                               */
	/************************************************************************/
	str.Empty();
	str.Format("[kiln_idx]=%d and [chn_type]=1", m_iActiveFurnaceNum + 1);
	pDoc->m_pChannelCfgTbl->SetFilter(str);
	pDoc->m_pChannelCfgTbl->Query();
	int iTemperatureARecordNum = pDoc->m_pChannelCfgTbl->GetCount();	// 获取记录条数, 以便设置表格参数

	/************************************************************************/
	/* 获取压力测试点的数量                                                 */
	/************************************************************************/
	str.Empty();
	str.Format("[kiln_idx]=%d and [chn_type]=2", m_iActiveFurnaceNum + 1);
	pDoc->m_pChannelCfgTbl->SetFilter(str);
	pDoc->m_pChannelCfgTbl->Query();
	int iPressRecordNum = pDoc->m_pChannelCfgTbl->GetCount();			// 获取记录条数, 以便设置表格参数

	/************************************************************************/
	/* 获取湿度测试点的数量                                                 */
	/************************************************************************/
	str.Empty();
	str.Format("[kiln_idx]=%d and [chn_type]=3", m_iActiveFurnaceNum + 1);
	pDoc->m_pChannelCfgTbl->SetFilter(str);
	pDoc->m_pChannelCfgTbl->Query();
	int iHumidityRecordNum = pDoc->m_pChannelCfgTbl->GetCount();		// 获取记录条数, 以便设置表格参数

	/************************************************************************/
	/* 获取电机测试点的数量                                                 */
	/************************************************************************/
	str.Empty();
	str.Format("[kiln_idx]=%d and [chn_type]=4", m_iActiveFurnaceNum + 1);
	pDoc->m_pChannelCfgTbl->SetFilter(str);
	pDoc->m_pChannelCfgTbl->Query();
	int iMotorRecordNum = pDoc->m_pChannelCfgTbl->GetCount();			// 获取记录条数, 以便设置表格参数
	
	// 设置网格的背景颜色
	m_pGrid->GetDefaultCell(FALSE, FALSE)->SetBackClr(THE_GRID_BACKGROUND_COLOR);
	// 设置视图的背景颜色
	m_pGrid->SetGridBkColor(THE_VIEW_BACKGROUND_COLOR);
	
	// 设置固定行列宽度
// 	m_pGrid->SetDefCellHeight(THE_DEFAULT_CELL_HEIGHT);
// 	m_pGrid->SetDefCellWidth(THE_DEFAULT_CELL_WIDTH);
	
	// 设置虚拟方式
	m_pGrid->SetVirtualMode(TRUE);
	// 设置固定表格头的行列数
	m_pGrid->SetFixedRowCount(1);
	m_pGrid->SetFixedColumnCount(1);
	
	int MaxRecordNum;
	if ((iTemperatureRecordNum >= iTemperatureARecordNum)
		&& (iTemperatureRecordNum >= iPressRecordNum) 
		&& (iTemperatureRecordNum >= iHumidityRecordNum) 
		&& (iTemperatureRecordNum >= iMotorRecordNum))
	{
		MaxRecordNum = iTemperatureRecordNum;
	}
	else if ((iTemperatureARecordNum >= iTemperatureRecordNum) 
		&& (iTemperatureARecordNum >= iPressRecordNum)
		&& (iTemperatureARecordNum >= iHumidityRecordNum) 
		&& (iTemperatureARecordNum >= iMotorRecordNum))
	{
		MaxRecordNum = iTemperatureARecordNum;
	}
	else if ((iPressRecordNum >= iTemperatureRecordNum) 
		&& (iPressRecordNum >= iTemperatureARecordNum)
		&& (iPressRecordNum >= iHumidityRecordNum) 
		&& (iPressRecordNum >= iMotorRecordNum))
	{
		MaxRecordNum = iPressRecordNum;
	}
	else if ((iHumidityRecordNum >= iTemperatureRecordNum) 
		&& (iHumidityRecordNum >= iTemperatureARecordNum) 
		&& (iHumidityRecordNum >= iPressRecordNum) 
		&& (iHumidityRecordNum >= iMotorRecordNum))
	{
		MaxRecordNum = iHumidityRecordNum;
	}
	else
	{
		MaxRecordNum = iMotorRecordNum;
	}	

	// +1代表固定表格头行列个数
	m_pGrid->SetRowCount(MaxRecordNum + 1);
	m_pGrid->SetColumnCount(2 * 5 + 1);			// 2=范围和零点 5=温度 边温 压力 湿度 电机

	// 表格自动调整列宽度
	m_pGrid->AutoSizeColumns(GVS_BOTH);
	// 表格自动调整行高度
	m_pGrid->AutoSizeRows();
		
	m_pGrid->SetEditable(TRUE);					// 允许编辑内容
	m_pGrid->EnableSelection(TRUE);				// 允许选择单元
	
	m_pGrid->SetRowResize(FALSE);				// 不允许行宽调整
	m_pGrid->SetColumnResize(TRUE);				// 允许列宽调整	
	
	m_pGrid->SetFixedRowSelection(FALSE);		// 不允许行选择
    m_pGrid->SetFixedColumnSelection(FALSE);	// 不允许列选择
	m_pGrid->SetTrackFocusCell(FALSE);			// 固定单元不能有焦点
	
	m_pGrid->Refresh();							// 刷新表格
}

BOOL CAdjustView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// 网格空间发出的消息
	if (wParam == (WPARAM)m_pGrid->GetDlgCtrlID())
	{
		*pResult = 0;
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
			if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
			{
				m_iSelectRow = pDispInfo->item.row;
			}
			
			return TRUE;
		}
		else if (GVN_BEGINLABELEDIT == pDispInfo->hdr.code)
		{
			return TRUE;
		}
		// 结束单元选择消息
		else if (GVN_ENDLABELEDIT == pDispInfo->hdr.code)
		{			
			ModifyGridItem(pDispInfo);
			return TRUE;			
		}
	}
	
	return CView::OnNotify(wParam, lParam, pResult);
}

void CAdjustView::ModifyGridItem(GV_DISPINFO *pDispInfo)
{
	int         iRecordNum;
	int         type, item, offset;
	MYDB_ITEM  *pItems;
	CString     str;

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	// 修改温度校正参数
	if (pDispInfo->item.col >= 1 && pDispInfo->item.col <= 2)
	{
		type = 0;
		item = 0;
	}
	// 修改边温度校正参数
	else if (pDispInfo->item.col >= 3 && pDispInfo->item.col <= 4)
	{
		type = 1;
		item = 2;
	}
	// 修改压力校正参数
	else if (pDispInfo->item.col >= 5 && pDispInfo->item.col <= 6)
	{
		type = 2;
		item = 4;
	}
	// 修改湿度校正参数
	else if (pDispInfo->item.col >= 7 && pDispInfo->item.col <= 8)
	{
		type = 3;
		item = 6;
	}
	// 修改电机校正参数
	else if (pDispInfo->item.col >= 9 && pDispInfo->item.col <= 10)
	{
		type = 4;
		item = 8;
	}
	else
	{
		pDispInfo->item.strText.Format("");
		return;
	}

	offset = pDispInfo->item.col - item + 6;

	/************************************************************************/
	/* 找出对应类型的通道配置记录                                           */
	/************************************************************************/
	str.Empty();
	str.Format("[kiln_idx]=%d and [chn_type]=%d", m_iActiveFurnaceNum + 1, type);
	pDoc->m_pChannelCfgTbl->SetFilter(str);
	pDoc->m_pChannelCfgTbl->SetSort("[id] ASC");
	pDoc->m_pChannelCfgTbl->Query();

	iRecordNum = pDoc->m_pChannelCfgTbl->GetCount();	
	if (iRecordNum < pDispInfo->item.row)
	{
		pDispInfo->item.strText.Format("");
		return;
	}

	// 不能用虚模式, 如果要使用则自己重载父类的OnEndEditCell函数
	m_strEdit = m_pGrid->GetItemText(pDispInfo->item.row, pDispInfo->item.col);	
	pItems    = pDoc->m_pChannelCfgTbl->Items();

	pDoc->m_pChannelCfgTbl->Move(pDispInfo->item.row);		// 移动到指定位置
	pDoc->m_pChannelCfgTbl->Edit();							// 进入编辑模式

	// 这里没有使用pDispInfo->item.col - 1, 是因为不显示每张表的ID号(即第一列)
	switch (pItems[offset].type)
	{
	case MYDB_TYPE_TEXT:
		*((SQLiteString *)(pDoc->m_pChannelCfgTbl->m_pColsName[offset])) = m_strEdit;
		break;
	case MYDB_TYPE_INT:
		*((int *)(pDoc->m_pChannelCfgTbl->m_pColsName[offset])) = atoi(m_strEdit.GetBuffer(m_strEdit.GetLength()));
		break;
	case MYDB_TYPE_DOUBLE:
		*((double *)(pDoc->m_pChannelCfgTbl->m_pColsName[offset])) = atof(m_strEdit.GetBuffer(m_strEdit.GetLength()));
		break;
	case MYDB_TYPE_BLOB:
		break;
	default:
		break;
	}

	pDoc->m_pChannelCfgTbl->Update();	

	// 更新后自动应用
	pDoc->SetAdjustValue(m_iActiveFurnaceNum, type);

	m_pGrid->Refresh();				// 刷新表格
}

void CAdjustView::SetGridItem(GV_DISPINFO *pDispInfo)
{
	int         iRecordNum;
	int         type, item, offset;
	MYDB_ITEM  *pItems;
	CString     str;

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	// 虚模式下表格内容居中
	pDispInfo->item.nFormat = DT_CENTER | DT_VCENTER;

	// 数据单元
	if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
	{
		pDispInfo->item.lfFont  = m_lfCellFont;
		pDispInfo->item.crFgClr = THE_IN_GRID_FT_COLOR;		// 设置前景色

		// 修改温度校正参数
		if (pDispInfo->item.col >= 1 && pDispInfo->item.col <= 2)
		{
			type = 0;
			item = 0;
		}
		// 修改边温度校正参数
		else if (pDispInfo->item.col >= 3 && pDispInfo->item.col <= 4)
		{
			type = 1;
			item = 2;
		}
		// 修改压力校正参数
		else if (pDispInfo->item.col >= 5 && pDispInfo->item.col <= 6)
		{
			type = 2;
			item = 4;
		}
		// 修改湿度校正参数
		else if (pDispInfo->item.col >= 7 && pDispInfo->item.col <= 8)
		{
			type = 3;
			item = 6;
		}
		// 修改电机校正参数
		else if (pDispInfo->item.col >= 9 && pDispInfo->item.col <= 10)
		{
			type = 4;
			item = 8;
		}
		else
		{
			pDispInfo->item.strText.Format("");
			return;
		}
		
		offset = pDispInfo->item.col - item + 6;

		str.Format("[kiln_idx]=%d and [chn_type]=%d", m_iActiveFurnaceNum + 1, type);
		pDoc->m_pChannelCfgTbl->SetFilter(str);
		pDoc->m_pChannelCfgTbl->Query();
	
		iRecordNum = pDoc->m_pChannelCfgTbl->GetCount();
		if (iRecordNum < pDispInfo->item.row)
		{
			pDispInfo->item.strText.Format("");
			return;
		}

		pDoc->m_pChannelCfgTbl->Move(pDispInfo->item.row);
		pItems = pDoc->m_pChannelCfgTbl->Items();

		// 这里没有使用pDispInfo->item.col - 1, 是因为不显示每张表的ID号(即第一列)
		switch (pItems[offset].type)
		{
		case MYDB_TYPE_TEXT:
			pDispInfo->item.strText.Format("%s", *((SQLiteString *)(pDoc->m_pChannelCfgTbl->m_pColsName[offset])));
			break;
		case MYDB_TYPE_INT:
			pDispInfo->item.strText.Format("%d", *((int *)(pDoc->m_pChannelCfgTbl->m_pColsName[offset])));
			break;
		case MYDB_TYPE_DOUBLE:
			pDispInfo->item.strText.Format("%3.2f", *((double *)(pDoc->m_pChannelCfgTbl->m_pColsName[offset])));
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
			switch (pDispInfo->item.col)
			{
			case 1:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("温度量程系数");
				break;
			case 2:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("温度零点系数");
				break;
			case 3:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("边温量程系数");
				break;
			case 4:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("边温零点系数");
				break;
			case 5:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("压力量程系数");
				break;
			case 6:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("压力零点系数");
				break;
			case 7:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("湿度量程系数");
				break;
			case 8:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("湿度零点系数");
				break;
			case 9:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("电机量程系数");
				break;
			case 10:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("电机零点系数");
				break;
			default:
				break;
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

void CAdjustView::OnDestroy() 
{
	CView::OnDestroy();
	
	delete m_pGrid;
}

void CAdjustView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(rect);
	
	m_pGrid->MoveWindow(rect, TRUE);
	
	// Do not call CView::OnPaint() for painting messages
}

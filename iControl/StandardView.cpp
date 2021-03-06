// StandardView.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "StandardView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStandardView
#define THE_GRID_BACKGROUND_COLOR	RGB(255, 255, 224)
#define THE_VIEW_BACKGROUND_COLOR	RGB(219, 219, 219)

#define THE_FIXED_ROW_FT_COLOR		RGB(127, 127, 127)
#define THE_FIXED_COL_FT_COLOR		RGB(127, 127, 127)

#define THE_IN_GRID_FT_COLOR		RGB(0, 0, 255)
#define THE_OUT_GRID_FT_COLOR		RGB(255, 0, 0)

#define THE_TEMP_FRONT_NUM			2						// 除测试点外的字段数(序号和设置时间)
#define THE_DEFAULT_CELL_HEIGHT		28						// 默认的表格高度
#define THE_DEFAULT_CELL_WIDTH		100						// 默认的表格宽度

#define THE_BOTTOM_GAP_HEIGHT		80

IMPLEMENT_DYNCREATE(CStandardView, CView)

CStandardView::CStandardView()
{
	m_iActiveFurnaceNum = 0;
	m_iSelectRow = 0;

	m_pGrid = NULL;
}

CStandardView::~CStandardView()
{
}


BEGIN_MESSAGE_MAP(CStandardView, CView)
	//{{AFX_MSG_MAP(CStandardView)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_COMMAND(IDB_SET_STANDARD, OnSetStandard)
	ON_COMMAND(IDB_DEL_STANDARD, OnDelStandard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStandardView drawing

void CStandardView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CStandardView diagnostics

#ifdef _DEBUG
void CStandardView::AssertValid() const
{
	CView::AssertValid();
}

void CStandardView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStandardView message handlers

void CStandardView::OnInitialUpdate() 
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
	rect.bottom = rect.bottom - THE_BOTTOM_GAP_HEIGHT;
	
	m_pGrid = new CIGridCtrl;
	m_pGrid->Create(rect, this, 5002, WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
	
	CFont Font;
	Font.CreateFont(14, 7, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "宋体");
	VERIFY(Font.GetLogFont(&m_lfFixCellFont));
    m_lfFixCellFont.lfWeight = FW_BOLD;
	Font.DeleteObject();
	
	Font.CreateFont(16, 0, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Arail narrow");
	VERIFY(Font.GetLogFont(&m_lfCellFont));
    m_lfCellFont.lfWeight = FW_BOLD;
	Font.DeleteObject();

	m_pSetButton = new CButton;
	m_pSetButton->Create("选定标准", WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDB_SET_STANDARD);

	m_pDelButton = new CButton;
	m_pDelButton->Create("删除标准", WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDB_DEL_STANDARD);

	GetClientRect(m_MaxClientRect);
	BITMAPINFO bmpinfo;
	bmpinfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth         = GetSystemMetrics(SM_CXSCREEN);		// 创建最大的图片, 尺寸宽度为屏幕尺寸宽度
	bmpinfo.bmiHeader.biHeight        = GetSystemMetrics(SM_CYSCREEN);		// 创建最大的图片, 尺寸高度为屏幕尺寸宽度
	bmpinfo.bmiHeader.biPlanes        = 1;
	bmpinfo.bmiHeader.biBitCount      = 32;
	bmpinfo.bmiHeader.biCompression   = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage     = 0;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed       = 0;
	bmpinfo.bmiHeader.biClrImportant  = 0;
	
	CDC *pDC = GetDC();
	m_MemDcDraw.CreateCompatibleDC(pDC);
	m_HbtMap = CreateDIBSection(m_MemDcDraw, &bmpinfo, DIB_RGB_COLORS, NULL, NULL, 0);
	m_MemDcDraw.SelectObject(m_HbtMap);
	m_MemDcDraw.SetBkMode(TRANSPARENT);
	ReleaseDC(pDC);
}

void CStandardView::ActiveView(int iFurnaceNum)
{
	m_iActiveFurnaceNum = iFurnaceNum;
	
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	
	CString str;
	
	switch (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].type)
	{
	case 0:
		str.Format("%d# <Mobile Kiln> Standard Curve", m_iActiveFurnaceNum + 1);
		break;
	case 1:
		str.Format("%d# <Drying Kiln> Standard Curve", m_iActiveFurnaceNum + 1);
		break;
	case 2:
		str.Format("%d# <Through Kiln> Standard Curve", m_iActiveFurnaceNum + 1);
		break;
	case 3:
		str.Format("%d# <Roasting Kiln> Standard Curve", m_iActiveFurnaceNum + 1);
		break;
	default:
		break;
	}
	
	pMainFrame->SetFrameCaption(str);
	
	SetStandardGridOnly();
}

void CStandardView::SetStandardGridOnly(void)
{
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetFilter("");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetSort("[set_time] DESC");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Query();			// 查询记录

	// 获取记录条数, 以便设置表格参数
	m_iRecordRowNum = pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->GetCount();

	// 获取标准温度个数, 以便设置表格参数
	m_iRecordColNum = pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum;
	
	// 设置网格的背景颜色
	m_pGrid->GetDefaultCell(FALSE, FALSE)->SetBackClr(THE_GRID_BACKGROUND_COLOR);
	// 设置视图的背景颜色
	m_pGrid->SetGridBkColor(THE_VIEW_BACKGROUND_COLOR);
	
	// 设定固定行列宽度
// 	m_pGrid->SetDefCellHeight(THE_DEFAULT_CELL_HEIGHT);
// 	m_pGrid->SetDefCellWidth(THE_DEFAULT_CELL_WIDTH);
	
	// 设置虚拟方式
	m_pGrid->SetVirtualMode(TRUE);
	// 设置固定表格头的行列数
	m_pGrid->SetFixedRowCount(1);
	m_pGrid->SetFixedColumnCount(1);
	
	// +1代表固定表格头行列个数
	m_pGrid->SetRowCount(m_iRecordRowNum + 1);
	m_pGrid->SetColumnCount(m_iRecordColNum + 1 + 2);

	// 表格自动调整列宽度
	m_pGrid->AutoSizeColumns(GVS_BOTH);
	// 表格自动调整行高度
	m_pGrid->AutoSizeRows();
	
	m_pGrid->SetEditable(TRUE);					// 允许编辑内容
	m_pGrid->EnableSelection(TRUE);				// 允许选择单元
	
	m_pGrid->SetRowResize(FALSE);				// 不允许行宽调整
	m_pGrid->SetColumnResize(TRUE);				// 允许列宽调整	
	
	m_pGrid->SetFixedRowSelection(TRUE);		// 允许行选择
    m_pGrid->SetFixedColumnSelection(FALSE);	// 不允许列选择
	m_pGrid->SetTrackFocusCell(FALSE);			// 固定单元不能有焦点

	m_pGrid->Refresh();							// 刷新表格
} 

BOOL CStandardView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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
//			if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
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

void CStandardView::ModifyGridItem(GV_DISPINFO *pDispInfo)
{
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	// 不能用虚模式, 如果要使用则自己重载父类的OnEndEditCell函数
	m_strEdit = m_pGrid->GetItemText(pDispInfo->item.row, pDispInfo->item.col);	

	MYDB_ITEM *pItems = pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Items();

	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetFilter("");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetSort("[set_time] DESC");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Query();
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Move(pDispInfo->item.row);
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Edit();

	/* 不允许修改id号和设置时间 */
	if (pDispInfo->item.col <= 2)
	{
		m_pGrid->Refresh();				// 刷新表格
		return;
	}

	switch (pItems[pDispInfo->item.col - 1].type)
	{
	case MYDB_TYPE_TEXT:
		*((SQLiteString *)(pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->m_pColsName[pDispInfo->item.col - 1])) = m_strEdit;
		break;
	case MYDB_TYPE_INT:
		*((int *)(pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->m_pColsName[pDispInfo->item.col - 1])) = atoi(m_strEdit.GetBuffer(m_strEdit.GetLength()));
		break;
	case MYDB_TYPE_DOUBLE:
		*((double *)(pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->m_pColsName[pDispInfo->item.col - 1])) = atof(m_strEdit.GetBuffer(m_strEdit.GetLength()));
		break;
	case MYDB_TYPE_BLOB:
		break;
	default:
		break;
	}

	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Update();	// 更新数据库

	// 如果修改的参数是当前的标准曲线, 设置一次标准曲线, 以便更新曲线和缓冲区
	if (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].StandardIndex == *((int *)(pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->m_pColsName[0])))
	{
		OnSetStandard();
	}

	m_pGrid->Refresh();										// 刷新表格
}

void CStandardView::SetGridItem(GV_DISPINFO *pDispInfo)
{
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int temp;
	MYDB_ITEM *pItems = pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Items();

	// 虚模式下表格内容居中
	pDispInfo->item.nFormat = DT_CENTER | DT_VCENTER;

	// 数据单元
	if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
	{
		pDispInfo->item.lfFont = m_lfCellFont;

		pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetFilter("");
		pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetSort("[set_time] DESC");
		pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Query();
		pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Move(pDispInfo->item.row);			// 移动取记录表头到指定位置, move函数的参数从1开始
		temp = *((int *)(pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->m_pColsName[0]));

		if (temp == pDoc->m_pKilnBuff[m_iActiveFurnaceNum].StandardIndex)
		{
			pDispInfo->item.crFgClr = THE_OUT_GRID_FT_COLOR;
		}
		else
		{
			pDispInfo->item.crFgClr = THE_IN_GRID_FT_COLOR;
		}
		
		switch (pItems[pDispInfo->item.col - 1].type)
		{
		case MYDB_TYPE_TEXT:
			pDispInfo->item.strText.Format("%s", *((SQLiteString *)(pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->m_pColsName[pDispInfo->item.col - 1])));
			break;
		case MYDB_TYPE_INT:
			temp = *((int *)(pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->m_pColsName[pDispInfo->item.col - 1]));					
			pDispInfo->item.strText.Format("%d", temp);
			break;
		case MYDB_TYPE_DOUBLE:
			pDispInfo->item.strText.Format("%3.2f", *((double *)(pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->m_pColsName[pDispInfo->item.col - 1])));
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
				pDispInfo->item.strText.Format("Index");
			}
			else if (pDispInfo->item.col == 2)
			{
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("Config Time");
			}
			else if (pDispInfo->item.col <= (m_iRecordColNum + THE_TEMP_FRONT_NUM))
			{			
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;

				if (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].type == 3)
				{
					if (strcmp(pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[pDispInfo->item.col - THE_TEMP_FRONT_NUM - 1].strName, "Automatic Numbering") != 0)
					{
						pDispInfo->item.strText.Format("%s TEMP(℃)", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[pDispInfo->item.col - THE_TEMP_FRONT_NUM - 1].strName);
					}
					else
					{
						pDispInfo->item.strText.Format("%03dm TEMP(℃)", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[pDispInfo->item.col - THE_TEMP_FRONT_NUM - 1].CarIdex);
					}
				}
				else
				{
					pDispInfo->item.strText.Format("%02d# POS TEMP", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[pDispInfo->item.col - THE_TEMP_FRONT_NUM - 1].CarIdex + 1);
				}
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

void CStandardView::OnDestroy() 
{
	CView::OnDestroy();

	delete m_pGrid;
	delete m_pSetButton;
	delete m_pDelButton;

	DeleteObject(m_HbtMap);
	DeleteDC(m_MemDcDraw);
}

void CStandardView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	GetClientRect(m_MaxClientRect);

	CRect rect = m_MaxClientRect;
	rect.bottom = rect.bottom - THE_BOTTOM_GAP_HEIGHT;
	m_pGrid->MoveWindow(rect, TRUE);
	
	// 创建刷子并绘制填充矩形, 尺寸为父窗口的客户区大小
	m_MaxClientRect.top = m_MaxClientRect.bottom - THE_BOTTOM_GAP_HEIGHT;
	CBrush brBack, *pOldcrBack;
	brBack.CreateSolidBrush(RGB(219, 219, 219));
	m_MemDcDraw.FillRect(&m_MaxClientRect, &brBack);	
	pOldcrBack = m_MemDcDraw.SelectObject(&brBack);	
	m_MemDcDraw.SelectObject(pOldcrBack);
	brBack.DeleteObject();	
	
	// 送入显存
	CDC *pDC = GetDC();
	pDC->StretchBlt(m_MaxClientRect.left, m_MaxClientRect.top, m_MaxClientRect.Width(), m_MaxClientRect.Height(), &m_MemDcDraw, m_MaxClientRect.left, m_MaxClientRect.top, m_MaxClientRect.Width(), m_MaxClientRect.Height(), SRCAND);
	ReleaseDC(pDC);

	CRect brect;
	brect.left = 450;
	brect.right = brect.left + 100;
	brect.top = rect.bottom + 20;
	brect.bottom = brect.top + 40;
	m_pSetButton->MoveWindow(brect, TRUE);

	brect.left = 750;
	brect.right = brect.left + 100;
	brect.top = rect.bottom + 20;
	brect.bottom = brect.top + 40;
	m_pDelButton->MoveWindow(brect, TRUE);

	m_pSetButton->Invalidate();
	m_pDelButton->Invalidate();
	
	// Do not call CView::OnPaint() for painting messages
}

void CStandardView::OnSetStandard()
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	// 找到被选中的记录的id号
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetFilter("");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetSort("[set_time] DESC");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Query();
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Move(m_iSelectRow);
	int index = *((int *)(pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->m_pColsName[0]));

	if (pDoc->SetStandardValue(m_iActiveFurnaceNum, index) == FALSE)	// 标准表是以id排序了的
	{
		MessageBox("设置标准曲线失败", "数据库操作错误", MB_OK);
	}

	m_pGrid->Refresh();
}

void CStandardView::OnDelStandard()
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc = (CIControlDoc *)pMainFrame->GetActiveDocument();

	// 找到被选中的记录的id号
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetFilter("");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetSort("[set_time] DESC");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Query();
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Move(m_iSelectRow);
	int index = *((int *)(pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->m_pColsName[0]));

	if (pDoc->DelStandardValue(m_iActiveFurnaceNum, index) == FALSE)	// 标准表是以id排序了的
	{
//		MessageBox("不能删除被选中的记录", "数据库操作错误", MB_OK);
		MessageBox("Can not delete recorders of selected", "Database error", MB_OK);
	}
	
	SetStandardGridOnly();								// 删除记录后，需要重新制定表格
}

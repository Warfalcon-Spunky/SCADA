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

#define THE_TEMP_FRONT_NUM			2						// �����Ե�����ֶ���(��źʹ洢ʱ��)
#define THE_DEFAULT_CELL_HEIGHT		28						// Ĭ�ϵı���߶�
#define THE_DEFAULT_CELL_WIDTH		150						// Ĭ�ϵı������

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

	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	if (pDoc->m_bDBOpen == FALSE)		// ������ݿ�򿪺����ݳ�ʼ��Ϊ�ɹ�
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
	Font.CreateFont(14, 7, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "����");
	VERIFY(Font.GetLogFont(&m_lfFixCellFont));
    m_lfFixCellFont.lfWeight = FW_BOLD;
	Font.DeleteObject();

	Font.CreateFont(16, 0, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Arail narrow");
	VERIFY(Font.GetLogFont(&m_lfCellFont));
    m_lfCellFont.lfWeight = FW_BOLD;
	Font.DeleteObject();

	//////////////////////////////////////////////////////////////////////////
	// �����˱�����Ϊ��������һ����ͼ�������Ĳ�Ӱ
	//////////////////////////////////////////////////////////////////////////
	BITMAPINFO bmpinfo;
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = GetSystemMetrics(SM_CXSCREEN);	// ��������ͼƬ, �ߴ����Ϊ��Ļ�ߴ����
	bmpinfo.bmiHeader.biHeight = GetSystemMetrics(SM_CYSCREEN);	// ��������ͼƬ, �ߴ�߶�Ϊ��Ļ�ߴ����
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 32;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = 0;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;
	
	CDC *pDC = GetDC();
	m_MemDcDraw.CreateCompatibleDC(pDC);												// �������ݻ���, ����������
	m_HbtMap = CreateDIBSection(m_MemDcDraw, &bmpinfo, DIB_RGB_COLORS, NULL, NULL, 0);	// �������豸�޹ص��ڴ�
	m_MemDcDraw.SelectObject(m_HbtMap);													// ѡ��ָ���ڴ�
	m_MemDcDraw.SetBkMode(TRANSPARENT);													// ��������Ϊ͸��
	ReleaseDC(pDC);
}

void CHisGridView::ActiveView(int iFurnaceNum)
{
	m_iActiveFurnaceNum = iFurnaceNum;

	// ��ȡ�ĵ���ز���
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
	
	pHisCurve->SetTotalCars(pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TotalKiln);	// �������ߵ��ܳ�λ
	pHisCurve->SetCurveCaption("");												// �������
	pHisCurve->SetCurvePoint(-1, 0, 0);											// �����һ�ε�����
	pMainFrame->SetFrameCaption(str);

	SetHistoryGridOnly();
}

void CHisGridView::UpdateView(void)
{
	// ��ȡ�ĵ���ز���
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

	// ��ȡ��¼����, �Ա����ñ������
	int iRecordNum = pDoc->m_pHisTbl[m_iActiveFurnaceNum]->GetCount();

	// ����������
	m_pGrid->SetRowCount(iRecordNum + 1);
	m_pGrid->SetColumnCount(pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum + 
							pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureANum +
							pDoc->m_pKilnBuff[m_iActiveFurnaceNum].PressNum + 
							pDoc->m_pKilnBuff[m_iActiveFurnaceNum].HumidityNum + 
							pDoc->m_pKilnBuff[m_iActiveFurnaceNum].MotorNum + 1 + 2);
}

void CHisGridView::SetHistoryGridOnly(void)
{
	// ��ȡ�ĵ���ز���
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

	// ��ȡ��¼����, �Ա����ñ������
	int iRecordNum = pDoc->m_pHisTbl[m_iActiveFurnaceNum]->GetCount();

	// ��������ı�����ɫ
	m_pGrid->GetDefaultCell(FALSE, FALSE)->SetBackClr(THE_GRID_BACKGROUND_COLOR);
	// ������ͼ�ı�����ɫ
	m_pGrid->SetGridBkColor(THE_VIEW_BACKGROUND_COLOR);

	m_pGrid->SetDefCellHeight(THE_DEFAULT_CELL_HEIGHT);
	m_pGrid->SetDefCellWidth(THE_DEFAULT_CELL_WIDTH);
	
	// �������ⷽʽ
	m_pGrid->SetVirtualMode(TRUE);
	// ���ù̶�����ͷ��������
	m_pGrid->SetFixedRowCount(1);
	m_pGrid->SetFixedColumnCount(1);

	m_pGrid->SetRowCount(iRecordNum + 1);

	int iRecordColNum = pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum + 
						pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureANum +
						pDoc->m_pKilnBuff[m_iActiveFurnaceNum].PressNum + 
						pDoc->m_pKilnBuff[m_iActiveFurnaceNum].HumidityNum + 
						pDoc->m_pKilnBuff[m_iActiveFurnaceNum].MotorNum + 1 + 2;
	m_pGrid->SetColumnCount(iRecordColNum);

	// ������ʷ���ݿ�����ݼ�¼�Ƚ϶಻ʹ���Զ��������ȷ�ʽ
	m_pGrid->SetColumnWidth(0, 30);				// ������������п���
	m_pGrid->SetColumnWidth(1, 60);				// �������ü�¼���п���
	m_pGrid->SetColumnWidth(2, 200);			// ��������ʱ���п���
	
	m_pGrid->SetEditable(FALSE);				// �������༭����
	m_pGrid->EnableSelection(TRUE);				// ����ѡ��Ԫ
	
	m_pGrid->SetRowResize(FALSE);				// �������п�����
	m_pGrid->SetColumnResize(TRUE);				// �����п�����	
	
	m_pGrid->SetFixedRowSelection(TRUE);		// ������ѡ��
    m_pGrid->SetFixedColumnSelection(FALSE);	// ��������ѡ��
	m_pGrid->SetTrackFocusCell(FALSE);			// �̶���Ԫ�����н���

	m_pGrid->SendMessageToParent(1, 1, GVN_SELCHANGED);

	m_iSelectRow = 1;
} 

BOOL CHisGridView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// ����ռ䷢������Ϣ
	if (wParam == (WPARAM)m_pGrid->GetDlgCtrlID())
	{
		*pResult = 1;

		GV_DISPINFO *pDispInfo = (GV_DISPINFO *)lParam;

		// ������ʾ��Ϣ
		if (GVN_GETDISPINFO == pDispInfo->hdr.code)			
		{
			SetGridItem(pDispInfo);
			return TRUE;
		}
		// ��ʼ��Ԫѡ����Ϣ
		else if (GVN_SELCHANGING == pDispInfo->hdr.code)
		{
			// ���ݵ�Ԫ
//			if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
			{
				m_iSelectRow = pDispInfo->item.row;
			}
			
			return TRUE;
		}
		// ������Ԫѡ����Ϣ
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
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int temp_idx;
	int temp;
	double f_temp;
	MYDB_ITEM *pItems = pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Items();

	// ��ģʽ�±������ݾ���
	pDispInfo->item.nFormat = DT_CENTER | DT_VCENTER;

	// ���ݵ�Ԫ
	if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
	{
//		pDispInfo->item.row += 10;		// �ı���ʾ��ƫ��

		pDispInfo->item.lfFont = m_lfCellFont;
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Move(pDispInfo->item.row);	// �ƶ�ȡ��¼��ͷ��ָ��λ��, move�����Ĳ�����1��ʼ

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
	// ��ͷ��Ԫ
	else
	{
		pDispInfo->item.lfFont = m_lfFixCellFont;

		// �б�ͷ��Ԫ����
		if (pDispInfo->item.row == 0 && pDispInfo->item.col > 0)
		{	
			// �б�ͷ��С��1��ʼ, ��Ϊ0���̶�����ռ��
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
						pDispInfo->item.strText.Format("%s TEMP(��)", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[temp_idx].strName);
					}
					else
					{
						pDispInfo->item.strText.Format("%03d m TEMP(��)", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[temp_idx].CarIdex);
					}
				}
				else
				{
					pDispInfo->item.strText.Format("%02d# POS TEMP(��)", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[temp_idx].CarIdex + 1);
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
		// �б�ͷ��Ԫ����
		else if (pDispInfo->item.col == 0 && pDispInfo->item.row > 0)
		{
			pDispInfo->item.crFgClr = THE_FIXED_ROW_FT_COLOR;
			pDispInfo->item.strText.Format("%d", pDispInfo->item.row);	
		}
	}
}

// ��ʾָ����¼������
void CHisGridView::SetCurveItem(GV_DISPINFO *pDispInfo)
{
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	// ���ݵ�Ԫ��Ч
	if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
	{
		pDoc->m_pHisTbl[m_iActiveFurnaceNum]->Move(pDispInfo->item.row);	// �ƶ�ȡ��¼��ͷ��ָ��λ��, move�����Ĳ�����1��ʼ		
		
		// �������ָ��
		CHisCurveView *pHisCurve = ((CSpliteFrame *)(pMainFrame->m_ViewPoint[pMainFrame->m_iTopViewID]))->GetHisCurveViewPoint();	

		CString str;

		// ���Ʊ���
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

		pHisCurve->SetCurvePoint(-1, 0, 0);									// �����һ�ε�����

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
			// ����ʵ������
			pHisCurve->SetCurvePoint(0, 
									 pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[i].CarIdex + car_loc_offset, 
									 *((int *)(pDoc->m_pHisTbl[m_iActiveFurnaceNum]->m_pColsName[THE_TEMP_FRONT_NUM + i])));
			
			// ���ñ�׼����
			pHisCurve->SetCurvePoint(1, 
									 pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[i].CarIdex + car_loc_offset, 
									 pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[i].SValue);
		}
		
		pHisCurve->CurveUpdate();			// ��������
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
	
	// ����ˢ�Ӳ�����������, �ߴ�Ϊ�����ڵĿͻ�����С
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

		//��˵� menuPopup �����Ӳ˵���
//		str.Format("���Ӳ�Ӧ��%d�ż�¼Ϊ��׼����", m_iSelectRow);		
		str.Format("add and apply %d# recorder to standard curve", m_iSelectRow);
		menuPopup.AppendMenu(MF_STRING, IDM_INSERT_AND_APP, str);

//		str.Format("������%d�ż�¼����׼���߿�", m_iSelectRow);
		str.Format("add %d# recorder to standard curve", m_iSelectRow);
		menuPopup.AppendMenu(MF_STRING, IDM_INSERT, str);

//		str.Format("ͨ��ʱ���ѯ��ʷ����");
		str.Format("Query history data by time");
		menuPopup.AppendMenu(MF_STRING, IDM_QUERY_HIS, str);

		//��ʾ����ʽ�˵�,�����û�ѡ��Ĳ˵���������Ӧ
		menuPopup.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}
}

// ��ѡ�е���ʷ���ݼ�¼�����׼�ⲢӦ��
void CHisGridView::OnInsertAndApp(void)
{
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int i;
	int iSTemperatureNum = pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum;

	int *pTemperatureBuff = new int[iSTemperatureNum];			// ����������

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
	/* ��ѡ�еļ�¼��ʾ��������                                             */
	/************************************************************************/
	CHisCurveView *pHisCurve = ((CSpliteFrame *)(pMainFrame->m_ViewPoint[pMainFrame->m_iTopViewID]))->GetHisCurveViewPoint();	

	/************************************************************************/
	/* �������߱���                                                         */
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

	pHisCurve->SetCurvePoint(-1, 0, 0);								// �����һ�ε�����

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
		// ����ʵ������=��׼����
		pHisCurve->SetCurvePoint(0, 
								 pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[i].CarIdex + car_loc_offset, 
								 pTemperatureBuff[i]);
		
		// ���ñ�׼����=ʵ������
		pHisCurve->SetCurvePoint(1, 
								 pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[i].CarIdex + car_loc_offset, 
								 pTemperatureBuff[i]);
	}
	pHisCurve->CurveUpdate();			// ��������		
	
	delete [] pTemperatureBuff;
}

void CHisGridView::OnInsert(void)
{
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int i;
	int iSTemperatureNum = pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum;

	int *pTemperatureBuff = new int[iSTemperatureNum];			// ����������

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
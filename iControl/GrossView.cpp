// GrossView.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "GrossView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrossView

#define THE_GRID_BACKGROUND_COLOR	RGB(255, 255, 224)
#define THE_VIEW_BACKGROUND_COLOR	RGB(219, 219, 219)

#define THE_FIXED_ROW_FT_COLOR		RGB(127, 127, 127)
#define THE_FIXED_COL_FT_COLOR		RGB(127, 127, 127)

#define THE_IN_GRID_FT_COLOR		RGB(0, 0, 255)
#define THE_OUT_GRID_FT_COLOR		RGB(255, 0, 0)

#define THE_TEMP_FRONT_NUM			2						// �����Ե�����ֶ���(��ź�����ʱ��)
#define THE_DEFAULT_CELL_HEIGHT		28						// Ĭ�ϵı��߶�
#define THE_DEFAULT_CELL_WIDTH		250						// Ĭ�ϵı����

#define THE_GROSS_TABLE_COL			4						// ����������

IMPLEMENT_DYNCREATE(CGrossView, CView)

CGrossView::CGrossView()
{
	m_iActiveFurnaceNum = 0;
	
	m_pGrid = NULL;
	m_pGrossTbl = NULL;
	m_bDayDisp = FALSE;
}

CGrossView::~CGrossView()
{
}


BEGIN_MESSAGE_MAP(CGrossView, CView)
	//{{AFX_MSG_MAP(CGrossView)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrossView drawing

void CGrossView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CGrossView diagnostics

#ifdef _DEBUG
void CGrossView::AssertValid() const
{
	CView::AssertValid();
}

void CGrossView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGrossView message handlers

void CGrossView::OnInitialUpdate() 
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

	m_pGrossTbl = pDoc->m_pGrossTbl;
	
	CRect rect;
	GetClientRect(rect);
	
	m_pGrid = new CIGridCtrl;
	m_pGrid->Create(rect, this, 5000, WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
	
	CFont Font;
	Font.CreateFont(14, 7, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "����");
	VERIFY(Font.GetLogFont(&m_lfFixCellFont));
    m_lfFixCellFont.lfWeight = FW_BOLD;
	Font.DeleteObject();
	
	Font.CreateFont(16, 0, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Arail narrow");
	VERIFY(Font.GetLogFont(&m_lfCellFont));
    m_lfCellFont.lfWeight = FW_BOLD;
	Font.DeleteObject();
}

void CGrossView::SetGrossActiveView(int iFurnace)
{
	m_iActiveFurnaceNum = iFurnace;

	CString str;
	str.Format("%d# Kiln Gross", m_iActiveFurnaceNum + 1);
	
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
	pMainFrame->SetFrameCaption(str);
	
	SetGrossGrid();
}

void CGrossView::CollectDayCnt(void)
{
	int     total_cnt, day_idx;
	CString str, str1, str2;
	GROSSDAYMAP GrossDayMap;

	m_GrossDayMap.clear();
	
	str.Format("[kiln_idx]=%d", m_iActiveFurnaceNum + 1);
	m_pGrossTbl->SetFilter(str);
	m_pGrossTbl->SetSort("[out_time] DESC");
	m_pGrossTbl->Query();						// ��ѯ��¼
	total_cnt = m_pGrossTbl->GetCount();		// �õ��ܵ���������
	str.Format("%s", *((SQLiteString *)(m_pGrossTbl->m_pColsName[1])));

	for (day_idx = 0; total_cnt > 0; day_idx++)
	{
		str1.Empty();
		str1.Format("[kiln_idx]=%d and date(out_time) <= date(\'%s\',\'start of day\',\'-%d day\') and date(out_time) > date(\'%s\',\'start of day\',\'-%d day\')", m_iActiveFurnaceNum + 1, str, day_idx, str, day_idx + 1);
		m_pGrossTbl->SetFilter(str1);
		m_pGrossTbl->Query();

		if (m_pGrossTbl->GetCount() <= 0)
		{
			continue;
		}

		GrossDayMap.KlinIdx = m_iActiveFurnaceNum + 1;

		str2.Empty();
		str2.Format("%s", *((SQLiteString *)(m_pGrossTbl->m_pColsName[1])));
		strncpy(GrossDayMap.DateTime, str2.GetBuffer(str2.GetLength()), 12);
		GrossDayMap.DateTime[10] = '\0';

		GrossDayMap.CodeCnt = m_pGrossTbl->GetCount();
		m_GrossDayMap.push_back(GrossDayMap);

		total_cnt = total_cnt - GrossDayMap.CodeCnt;
	}
}

void CGrossView::CollectBaseDayCnt(void)
{
	CString str;
	GROSSMINMAP GrossMinMap;
	CTime t1, t2;
	CTimeSpan MinSpan;
	int nYear, nMonth, nDate, nHour, nMin, nSec; 

	m_GrossMinMap.clear();

	str.Format("[kiln_idx]=%d", m_iActiveFurnaceNum + 1);
	m_pGrossTbl->SetFilter(str);
	m_pGrossTbl->SetSort("[out_time] DESC");
	m_pGrossTbl->Query();						// ��ѯ��¼
	m_pGrossTbl->MoveFirst();
	// ���ݿ��һ���ļ��ʱ��ʼ����0
	GrossMinMap.KlinIdx = m_iActiveFurnaceNum + 1;
	GrossMinMap.Internal = 0;
	m_GrossMinMap.push_back(GrossMinMap);

	for (int i = 1; i < m_pGrossTbl->GetCount(); i++)
	{
		str.Empty();
		str.Format("%s", *((SQLiteString *)(m_pGrossTbl->m_pColsName[1])));
		sscanf(str, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec); 
		t1 = CTime(nYear, nMonth, nDate, nHour, nMin, nSec);

		m_pGrossTbl->MoveNext();
		str.Empty();
		str.Format("%s", *((SQLiteString *)(m_pGrossTbl->m_pColsName[1])));
		sscanf(str, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec); 
		t2 = CTime(nYear, nMonth, nDate, nHour, nMin, nSec);

		MinSpan = t1 - t2;
		GrossMinMap.KlinIdx = m_iActiveFurnaceNum + 1;
		GrossMinMap.Internal = MinSpan.GetTotalMinutes();
		m_GrossMinMap.push_back(GrossMinMap);
	}
}

void CGrossView::SetGrossGrid(void)
{
	CString str;
	int iRecordNum;

	if (m_bDayDisp == FALSE)
	{
// 		str.Format("[kiln_idx]=%d", m_iActiveFurnaceNum + 1);
// 		m_pGrossTbl->SetFilter(str);
// 		m_pGrossTbl->SetSort("[out_time] DESC");
// 		m_pGrossTbl->Query();						// ��ѯ��¼

		CollectBaseDayCnt();
		
		// ��ȡ��¼����, �Ա����ñ�����
//		iRecordNum = m_pGrossTbl->GetCount();
		iRecordNum = m_GrossMinMap.size();
	}
	else
	{
		CollectDayCnt();

		// ��ȡ��¼����, �Ա����ñ�����
		iRecordNum = m_GrossDayMap.size();
	}

	
	// ��������ı�����ɫ
	m_pGrid->GetDefaultCell(FALSE, FALSE)->SetBackClr(THE_GRID_BACKGROUND_COLOR);
	// ������ͼ�ı�����ɫ
	m_pGrid->SetGridBkColor(THE_VIEW_BACKGROUND_COLOR);

	// ���ù̶��߶�
// 	m_pGrid->SetDefCellHeight(THE_DEFAULT_CELL_HEIGHT);
// 	m_pGrid->SetDefCellWidth(THE_DEFAULT_CELL_WIDTH);
	
	// �������ⷽʽ
	m_pGrid->SetVirtualMode(TRUE);
	// ���ù̶����ͷ��������
	m_pGrid->SetFixedRowCount(1);
	m_pGrid->SetFixedColumnCount(1);
	
	// +1����̶����ͷ���и���
	m_pGrid->SetRowCount(iRecordNum + 1);
	m_pGrid->SetColumnCount(THE_GROSS_TABLE_COL + 1);

	// ����Զ������п��
	m_pGrid->AutoSizeColumns(GVS_BOTH);
	m_pGrid->AutoSizeRows();

	
	m_pGrid->SetEditable(FALSE);				// ������༭����
	m_pGrid->EnableSelection(TRUE);				// ����ѡ��Ԫ
	
	m_pGrid->SetRowResize(FALSE);				// �������п����
	m_pGrid->SetColumnResize(FALSE);				// �����п����	
	
	m_pGrid->SetFixedRowSelection(TRUE);		// ������ѡ��
    m_pGrid->SetFixedColumnSelection(FALSE);	// ��������ѡ��
	m_pGrid->SetTrackFocusCell(FALSE);			// �̶���Ԫ�����н���
	
	m_pGrid->Refresh();							// ˢ�±��
} 

BOOL CGrossView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	// ����ռ䷢������Ϣ
	if (wParam == (WPARAM)m_pGrid->GetDlgCtrlID())
	{
		*pResult = 0;
		GV_DISPINFO *pDispInfo = (GV_DISPINFO *)lParam;
		
		// ������ʾ��Ϣ
		if (GVN_GETDISPINFO == pDispInfo->hdr.code)			
		{
			if (m_bDayDisp == FALSE)
			{
				SetGrossGridItem(pDispInfo);
			}
			else
			{
				SetGrossDayGridItem(pDispInfo);
			}
			return TRUE;
		}
		// ��ʼ��Ԫѡ����Ϣ
		else if (GVN_SELCHANGING == pDispInfo->hdr.code)
		{					
			return TRUE;
		}
		else if (GVN_BEGINLABELEDIT == pDispInfo->hdr.code)
		{
			return TRUE;
		}
		// ������Ԫѡ����Ϣ
		else if (GVN_ENDLABELEDIT == pDispInfo->hdr.code)
		{			
			return TRUE;			
		}
	}
	
	return CView::OnNotify(wParam, lParam, pResult);
}

void CGrossView::SetGrossDayGridItem(GV_DISPINFO *pDispInfo)
{
	CString str;
	
	// ��ģʽ�±�����ݾ���
	pDispInfo->item.nFormat = DT_CENTER | DT_VCENTER;
	
	// ���ݵ�Ԫ
	if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
	{
		pDispInfo->item.lfFont = m_lfCellFont;
		
		pDispInfo->item.crFgClr = THE_IN_GRID_FT_COLOR;		// ����������ɫ
		
		switch (pDispInfo->item.col)
		{
		case 1:
			pDispInfo->item.strText.Format("%d", m_GrossDayMap[pDispInfo->item.row - 1].KlinIdx);
			break;
		case 2:					
			pDispInfo->item.strText.Format("%s", m_GrossDayMap[pDispInfo->item.row - 1].DateTime);
			break;
		case 3:
			pDispInfo->item.strText.Format("%d", m_GrossDayMap[pDispInfo->item.row - 1].CodeCnt);
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
			// �б�ͷ��С��1��ʼ, ��Ϊ0���̶����ռ��
			if (pDispInfo->item.col == 1)
			{
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("Kiln Index");
			}
			else if (pDispInfo->item.col == 2)
			{
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("Push Time(Day)");
			}
			else if (pDispInfo->item.col == 3)
			{
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("Today Gross");
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

void CGrossView::SetGrossGridItem(GV_DISPINFO *pDispInfo)
{
	int temp;
	CString str;
	MYDB_ITEM *pItems = m_pGrossTbl->Items();

	// ��ģʽ�±�����ݾ���
	pDispInfo->item.nFormat = DT_CENTER | DT_VCENTER;

	// ���ݵ�Ԫ
	if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
	{
		pDispInfo->item.lfFont = m_lfCellFont;
		
		m_pGrossTbl->Move(pDispInfo->item.row);				// �ƶ�ȡ��¼��ͷ��ָ��λ��, move�����Ĳ�����1��ʼ
		pDispInfo->item.crFgClr = THE_IN_GRID_FT_COLOR;		// ����������ɫ
		
		switch (pItems[pDispInfo->item.col - 1].type)
		{
		case MYDB_TYPE_TEXT:
			str.Format("%s", *((SQLiteString *)(m_pGrossTbl->m_pColsName[pDispInfo->item.col - 1])));
			pDispInfo->item.strText.Format("%s", str);
			break;
		case MYDB_TYPE_INT:
			if (pDispInfo->item.col == 3)
			{
				pDispInfo->item.strText.Format("%d", m_GrossMinMap[pDispInfo->item.row - 1].Internal);
			}
			else
			{
				temp = *((int *)(m_pGrossTbl->m_pColsName[pDispInfo->item.col - 1]));					
				pDispInfo->item.strText.Format("%d", temp);
			}
			break;
		case MYDB_TYPE_DOUBLE:
			pDispInfo->item.strText.Format("%3.2f", *((double *)(m_pGrossTbl->m_pColsName[pDispInfo->item.col - 1])));
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
			// �б�ͷ��С��1��ʼ, ��Ϊ0���̶����ռ��
			if (pDispInfo->item.col == 1)
			{
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("Kiln Index");
			}
			else if (pDispInfo->item.col == 2)
			{
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("Push Time");
			}
			else if (pDispInfo->item.col == 3)
			{
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("Push Internal(Min)");
			}
			else if (pDispInfo->item.col == 4)
			{
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("Operation Mode");
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

void CGrossView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_pGrid;
}

void CGrossView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(rect);
	
	m_pGrid->MoveWindow(rect, TRUE);
	
	// Do not call CView::OnPaint() for painting messages
}

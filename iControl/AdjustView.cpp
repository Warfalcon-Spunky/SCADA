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

#define THE_TEMP_FRONT_NUM			2						// �����Ե�����ֶ���(��ź�����ʱ��)
#define THE_DEFAULT_CELL_HEIGHT		28						// Ĭ�ϵı��߶�
#define THE_DEFAULT_CELL_WIDTH		150						// Ĭ�ϵı����

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
	m_pGrid->Create(rect, this, 5004, WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
	
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

void CAdjustView::SetAdjustActiveView(int iFurnace)
{
	m_iActiveFurnaceNum = iFurnace;
	
	// ��ȡ�ĵ���ز���
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
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	CString str;

	/************************************************************************/
	/* ��ȡ�¶Ȳ��Ե������                                                 */
	/************************************************************************/
	str.Empty();
	str.Format("[kiln_idx]=%d and [chn_type]=0", m_iActiveFurnaceNum + 1);
	pDoc->m_pChannelCfgTbl->SetFilter(str);
	pDoc->m_pChannelCfgTbl->Query();
	int iTemperatureRecordNum = pDoc->m_pChannelCfgTbl->GetCount();		// ��ȡ��¼����, �Ա����ñ�����

	/************************************************************************/
	/* ��ȡ���¶Ȳ��Ե������                                               */
	/************************************************************************/
	str.Empty();
	str.Format("[kiln_idx]=%d and [chn_type]=1", m_iActiveFurnaceNum + 1);
	pDoc->m_pChannelCfgTbl->SetFilter(str);
	pDoc->m_pChannelCfgTbl->Query();
	int iTemperatureARecordNum = pDoc->m_pChannelCfgTbl->GetCount();	// ��ȡ��¼����, �Ա����ñ�����

	/************************************************************************/
	/* ��ȡѹ�����Ե������                                                 */
	/************************************************************************/
	str.Empty();
	str.Format("[kiln_idx]=%d and [chn_type]=2", m_iActiveFurnaceNum + 1);
	pDoc->m_pChannelCfgTbl->SetFilter(str);
	pDoc->m_pChannelCfgTbl->Query();
	int iPressRecordNum = pDoc->m_pChannelCfgTbl->GetCount();			// ��ȡ��¼����, �Ա����ñ�����

	/************************************************************************/
	/* ��ȡʪ�Ȳ��Ե������                                                 */
	/************************************************************************/
	str.Empty();
	str.Format("[kiln_idx]=%d and [chn_type]=3", m_iActiveFurnaceNum + 1);
	pDoc->m_pChannelCfgTbl->SetFilter(str);
	pDoc->m_pChannelCfgTbl->Query();
	int iHumidityRecordNum = pDoc->m_pChannelCfgTbl->GetCount();		// ��ȡ��¼����, �Ա����ñ�����

	/************************************************************************/
	/* ��ȡ������Ե������                                                 */
	/************************************************************************/
	str.Empty();
	str.Format("[kiln_idx]=%d and [chn_type]=4", m_iActiveFurnaceNum + 1);
	pDoc->m_pChannelCfgTbl->SetFilter(str);
	pDoc->m_pChannelCfgTbl->Query();
	int iMotorRecordNum = pDoc->m_pChannelCfgTbl->GetCount();			// ��ȡ��¼����, �Ա����ñ�����
	
	// ��������ı�����ɫ
	m_pGrid->GetDefaultCell(FALSE, FALSE)->SetBackClr(THE_GRID_BACKGROUND_COLOR);
	// ������ͼ�ı�����ɫ
	m_pGrid->SetGridBkColor(THE_VIEW_BACKGROUND_COLOR);
	
	// ���ù̶����п��
// 	m_pGrid->SetDefCellHeight(THE_DEFAULT_CELL_HEIGHT);
// 	m_pGrid->SetDefCellWidth(THE_DEFAULT_CELL_WIDTH);
	
	// �������ⷽʽ
	m_pGrid->SetVirtualMode(TRUE);
	// ���ù̶����ͷ��������
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

	// +1����̶����ͷ���и���
	m_pGrid->SetRowCount(MaxRecordNum + 1);
	m_pGrid->SetColumnCount(2 * 5 + 1);			// 2=��Χ����� 5=�¶� ���� ѹ�� ʪ�� ���

	// ����Զ������п��
	m_pGrid->AutoSizeColumns(GVS_BOTH);
	// ����Զ������и߶�
	m_pGrid->AutoSizeRows();
		
	m_pGrid->SetEditable(TRUE);					// ����༭����
	m_pGrid->EnableSelection(TRUE);				// ����ѡ��Ԫ
	
	m_pGrid->SetRowResize(FALSE);				// �������п����
	m_pGrid->SetColumnResize(TRUE);				// �����п����	
	
	m_pGrid->SetFixedRowSelection(FALSE);		// ��������ѡ��
    m_pGrid->SetFixedColumnSelection(FALSE);	// ��������ѡ��
	m_pGrid->SetTrackFocusCell(FALSE);			// �̶���Ԫ�����н���
	
	m_pGrid->Refresh();							// ˢ�±��
}

BOOL CAdjustView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// ����ռ䷢������Ϣ
	if (wParam == (WPARAM)m_pGrid->GetDlgCtrlID())
	{
		*pResult = 0;
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
		// ������Ԫѡ����Ϣ
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

	// �޸��¶�У������
	if (pDispInfo->item.col >= 1 && pDispInfo->item.col <= 2)
	{
		type = 0;
		item = 0;
	}
	// �޸ı��¶�У������
	else if (pDispInfo->item.col >= 3 && pDispInfo->item.col <= 4)
	{
		type = 1;
		item = 2;
	}
	// �޸�ѹ��У������
	else if (pDispInfo->item.col >= 5 && pDispInfo->item.col <= 6)
	{
		type = 2;
		item = 4;
	}
	// �޸�ʪ��У������
	else if (pDispInfo->item.col >= 7 && pDispInfo->item.col <= 8)
	{
		type = 3;
		item = 6;
	}
	// �޸ĵ��У������
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
	/* �ҳ���Ӧ���͵�ͨ�����ü�¼                                           */
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

	// ��������ģʽ, ���Ҫʹ�����Լ����ظ����OnEndEditCell����
	m_strEdit = m_pGrid->GetItemText(pDispInfo->item.row, pDispInfo->item.col);	
	pItems    = pDoc->m_pChannelCfgTbl->Items();

	pDoc->m_pChannelCfgTbl->Move(pDispInfo->item.row);		// �ƶ���ָ��λ��
	pDoc->m_pChannelCfgTbl->Edit();							// ����༭ģʽ

	// ����û��ʹ��pDispInfo->item.col - 1, ����Ϊ����ʾÿ�ű��ID��(����һ��)
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

	// ���º��Զ�Ӧ��
	pDoc->SetAdjustValue(m_iActiveFurnaceNum, type);

	m_pGrid->Refresh();				// ˢ�±��
}

void CAdjustView::SetGridItem(GV_DISPINFO *pDispInfo)
{
	int         iRecordNum;
	int         type, item, offset;
	MYDB_ITEM  *pItems;
	CString     str;

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	// ��ģʽ�±�����ݾ���
	pDispInfo->item.nFormat = DT_CENTER | DT_VCENTER;

	// ���ݵ�Ԫ
	if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
	{
		pDispInfo->item.lfFont  = m_lfCellFont;
		pDispInfo->item.crFgClr = THE_IN_GRID_FT_COLOR;		// ����ǰ��ɫ

		// �޸��¶�У������
		if (pDispInfo->item.col >= 1 && pDispInfo->item.col <= 2)
		{
			type = 0;
			item = 0;
		}
		// �޸ı��¶�У������
		else if (pDispInfo->item.col >= 3 && pDispInfo->item.col <= 4)
		{
			type = 1;
			item = 2;
		}
		// �޸�ѹ��У������
		else if (pDispInfo->item.col >= 5 && pDispInfo->item.col <= 6)
		{
			type = 2;
			item = 4;
		}
		// �޸�ʪ��У������
		else if (pDispInfo->item.col >= 7 && pDispInfo->item.col <= 8)
		{
			type = 3;
			item = 6;
		}
		// �޸ĵ��У������
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

		// ����û��ʹ��pDispInfo->item.col - 1, ����Ϊ����ʾÿ�ű��ID��(����һ��)
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
	// ��ͷ��Ԫ
	else
	{
		pDispInfo->item.lfFont = m_lfFixCellFont;
		
		// �б�ͷ��Ԫ����
		if (pDispInfo->item.row == 0 && pDispInfo->item.col > 0)
		{	
			// �б�ͷ��С��1��ʼ, ��Ϊ0���̶����ռ��
			switch (pDispInfo->item.col)
			{
			case 1:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("�¶�����ϵ��");
				break;
			case 2:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("�¶����ϵ��");
				break;
			case 3:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("��������ϵ��");
				break;
			case 4:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("�������ϵ��");
				break;
			case 5:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("ѹ������ϵ��");
				break;
			case 6:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("ѹ�����ϵ��");
				break;
			case 7:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("ʪ������ϵ��");
				break;
			case 8:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("ʪ�����ϵ��");
				break;
			case 9:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("�������ϵ��");
				break;
			case 10:
				pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
				pDispInfo->item.strText.Format("������ϵ��");
				break;
			default:
				break;
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

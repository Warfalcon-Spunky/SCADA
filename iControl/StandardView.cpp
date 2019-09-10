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

#define THE_TEMP_FRONT_NUM			2						// �����Ե�����ֶ���(��ź�����ʱ��)
#define THE_DEFAULT_CELL_HEIGHT		28						// Ĭ�ϵı���߶�
#define THE_DEFAULT_CELL_WIDTH		100						// Ĭ�ϵı������

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
	rect.bottom = rect.bottom - THE_BOTTOM_GAP_HEIGHT;
	
	m_pGrid = new CIGridCtrl;
	m_pGrid->Create(rect, this, 5002, WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
	
	CFont Font;
	Font.CreateFont(14, 7, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "����");
	VERIFY(Font.GetLogFont(&m_lfFixCellFont));
    m_lfFixCellFont.lfWeight = FW_BOLD;
	Font.DeleteObject();
	
	Font.CreateFont(16, 0, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Arail narrow");
	VERIFY(Font.GetLogFont(&m_lfCellFont));
    m_lfCellFont.lfWeight = FW_BOLD;
	Font.DeleteObject();

	m_pSetButton = new CButton;
	m_pSetButton->Create("ѡ����׼", WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDB_SET_STANDARD);

	m_pDelButton = new CButton;
	m_pDelButton->Create("ɾ����׼", WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDB_DEL_STANDARD);

	GetClientRect(m_MaxClientRect);
	BITMAPINFO bmpinfo;
	bmpinfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth         = GetSystemMetrics(SM_CXSCREEN);		// ��������ͼƬ, �ߴ����Ϊ��Ļ�ߴ����
	bmpinfo.bmiHeader.biHeight        = GetSystemMetrics(SM_CYSCREEN);		// ��������ͼƬ, �ߴ�߶�Ϊ��Ļ�ߴ����
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
	
	// ��ȡ�ĵ���ز���
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
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetFilter("");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetSort("[set_time] DESC");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Query();			// ��ѯ��¼

	// ��ȡ��¼����, �Ա����ñ������
	m_iRecordRowNum = pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->GetCount();

	// ��ȡ��׼�¶ȸ���, �Ա����ñ������
	m_iRecordColNum = pDoc->m_pKilnBuff[m_iActiveFurnaceNum].TemperatureNum;
	
	// ��������ı�����ɫ
	m_pGrid->GetDefaultCell(FALSE, FALSE)->SetBackClr(THE_GRID_BACKGROUND_COLOR);
	// ������ͼ�ı�����ɫ
	m_pGrid->SetGridBkColor(THE_VIEW_BACKGROUND_COLOR);
	
	// �趨�̶����п���
// 	m_pGrid->SetDefCellHeight(THE_DEFAULT_CELL_HEIGHT);
// 	m_pGrid->SetDefCellWidth(THE_DEFAULT_CELL_WIDTH);
	
	// �������ⷽʽ
	m_pGrid->SetVirtualMode(TRUE);
	// ���ù̶�����ͷ��������
	m_pGrid->SetFixedRowCount(1);
	m_pGrid->SetFixedColumnCount(1);
	
	// +1�����̶�����ͷ���и���
	m_pGrid->SetRowCount(m_iRecordRowNum + 1);
	m_pGrid->SetColumnCount(m_iRecordColNum + 1 + 2);

	// �����Զ������п���
	m_pGrid->AutoSizeColumns(GVS_BOTH);
	// �����Զ������и߶�
	m_pGrid->AutoSizeRows();
	
	m_pGrid->SetEditable(TRUE);					// �����༭����
	m_pGrid->EnableSelection(TRUE);				// ����ѡ��Ԫ
	
	m_pGrid->SetRowResize(FALSE);				// �������п�����
	m_pGrid->SetColumnResize(TRUE);				// �����п�����	
	
	m_pGrid->SetFixedRowSelection(TRUE);		// ������ѡ��
    m_pGrid->SetFixedColumnSelection(FALSE);	// ��������ѡ��
	m_pGrid->SetTrackFocusCell(FALSE);			// �̶���Ԫ�����н���

	m_pGrid->Refresh();							// ˢ�±���
} 

BOOL CStandardView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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
		// ������Ԫѡ����Ϣ
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
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	// ��������ģʽ, ���Ҫʹ�����Լ����ظ����OnEndEditCell����
	m_strEdit = m_pGrid->GetItemText(pDispInfo->item.row, pDispInfo->item.col);	

	MYDB_ITEM *pItems = pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Items();

	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetFilter("");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetSort("[set_time] DESC");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Query();
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Move(pDispInfo->item.row);
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Edit();

	/* �������޸�id�ź�����ʱ�� */
	if (pDispInfo->item.col <= 2)
	{
		m_pGrid->Refresh();				// ˢ�±���
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

	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Update();	// �������ݿ�

	// ����޸ĵĲ����ǵ�ǰ�ı�׼����, ����һ�α�׼����, �Ա�������ߺͻ�����
	if (pDoc->m_pKilnBuff[m_iActiveFurnaceNum].StandardIndex == *((int *)(pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->m_pColsName[0])))
	{
		OnSetStandard();
	}

	m_pGrid->Refresh();										// ˢ�±���
}

void CStandardView::SetGridItem(GV_DISPINFO *pDispInfo)
{
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int temp;
	MYDB_ITEM *pItems = pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Items();

	// ��ģʽ�±������ݾ���
	pDispInfo->item.nFormat = DT_CENTER | DT_VCENTER;

	// ���ݵ�Ԫ
	if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
	{
		pDispInfo->item.lfFont = m_lfCellFont;

		pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetFilter("");
		pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetSort("[set_time] DESC");
		pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Query();
		pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Move(pDispInfo->item.row);			// �ƶ�ȡ��¼��ͷ��ָ��λ��, move�����Ĳ�����1��ʼ
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
						pDispInfo->item.strText.Format("%s TEMP(��)", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[pDispInfo->item.col - THE_TEMP_FRONT_NUM - 1].strName);
					}
					else
					{
						pDispInfo->item.strText.Format("%03dm TEMP(��)", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[pDispInfo->item.col - THE_TEMP_FRONT_NUM - 1].CarIdex);
					}
				}
				else
				{
					pDispInfo->item.strText.Format("%02d# POS TEMP", pDoc->m_pKilnBuff[m_iActiveFurnaceNum].m_pTempObjectBuff[pDispInfo->item.col - THE_TEMP_FRONT_NUM - 1].CarIdex + 1);
				}
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
	
	// ����ˢ�Ӳ�����������, �ߴ�Ϊ�����ڵĿͻ�����С
	m_MaxClientRect.top = m_MaxClientRect.bottom - THE_BOTTOM_GAP_HEIGHT;
	CBrush brBack, *pOldcrBack;
	brBack.CreateSolidBrush(RGB(219, 219, 219));
	m_MemDcDraw.FillRect(&m_MaxClientRect, &brBack);	
	pOldcrBack = m_MemDcDraw.SelectObject(&brBack);	
	m_MemDcDraw.SelectObject(pOldcrBack);
	brBack.DeleteObject();	
	
	// �����Դ�
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

	// �ҵ���ѡ�еļ�¼��id��
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetFilter("");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetSort("[set_time] DESC");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Query();
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Move(m_iSelectRow);
	int index = *((int *)(pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->m_pColsName[0]));

	if (pDoc->SetStandardValue(m_iActiveFurnaceNum, index) == FALSE)	// ��׼������id�����˵�
	{
		MessageBox("���ñ�׼����ʧ��", "���ݿ��������", MB_OK);
	}

	m_pGrid->Refresh();
}

void CStandardView::OnDelStandard()
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc = (CIControlDoc *)pMainFrame->GetActiveDocument();

	// �ҵ���ѡ�еļ�¼��id��
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetFilter("");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->SetSort("[set_time] DESC");
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Query();
	pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->Move(m_iSelectRow);
	int index = *((int *)(pDoc->m_pStandardTbl[m_iActiveFurnaceNum]->m_pColsName[0]));

	if (pDoc->DelStandardValue(m_iActiveFurnaceNum, index) == FALSE)	// ��׼������id�����˵�
	{
//		MessageBox("����ɾ����ѡ�еļ�¼", "���ݿ��������", MB_OK);
		MessageBox("Can not delete recorders of selected", "Database error", MB_OK);
	}
	
	SetStandardGridOnly();								// ɾ����¼����Ҫ�����ƶ�����
}
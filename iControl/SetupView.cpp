// SetupView.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "MainFrm.h"
#include "iControlDoc.h"
#include "SetupView.h"

#include "NewCellTypes/GridCellCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupView

#define THE_BACKGROUND_COLOR		RGB(219, 219, 219)		// ���ý��汳����ɫ

#define THE_GRID_BACKGROUND_COLOR	RGB(255, 255, 224)
#define THE_VIEW_BACKGROUND_COLOR	RGB(219, 219, 219)

#define THE_FIXED_ROW_FT_COLOR		RGB(127, 127, 127)
#define THE_FIXED_COL_FT_COLOR		RGB(127, 127, 127)

#define THE_IN_GRID_FT_COLOR		RGB(0, 0, 255)
#define THE_OUT_GRID_FT_COLOR		RGB(255, 0, 0)

#define THE_DEFAULT_CELL_HEIGHT		28						// Ĭ�ϵı��߶�
#define THE_DEFAULT_CELL_WIDTH		100						// Ĭ�ϵı����

#define THE_BOTTOM_GAP_HEIGHT		80

#define THE_KILN_INFO_TBL_COL		10
#define THE_TERM_INFO_TBL_COL		5
#define THE_CHAN_INFO_TBL_COL		8

IMPLEMENT_DYNCREATE(CSetupView, CView)

CSetupView::CSetupView()
{
	m_bFileOpen = FALSE;

	m_iViewIndex = -1;
	m_iSelectRow = 0;
}

CSetupView::~CSetupView()
{
}


BEGIN_MESSAGE_MAP(CSetupView, CView)
	//{{AFX_MSG_MAP(CSetupView)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_COMMAND(IDB_APPEND_CFG, OnAppendCfgRecord)
	ON_COMMAND(IDB_DELETE_CFG, OnDeleteCfgRecord)
	ON_COMMAND(IDB_NEXT_CFG, OnNextCfgRecord)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupView drawing

void CSetupView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CSetupView diagnostics

#ifdef _DEBUG
void CSetupView::AssertValid() const
{
	CView::AssertValid();
}

void CSetupView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSetupView message handlers

void CSetupView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	CRect rect;
	GetClientRect(rect);
	rect.bottom = rect.bottom - THE_BOTTOM_GAP_HEIGHT;
	
	m_pGrid = new CIGridCtrl;
	m_pGrid->Create(rect, this, 5003, WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
	
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
	m_pSetButton->Create("���", WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDB_APPEND_CFG);
	
	m_pDelButton = new CButton;
	m_pDelButton->Create("ɾ��", WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDB_DELETE_CFG);
	
	m_pNextButton = new CButton;
	m_pNextButton->Create("��һ��", WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDB_NEXT_CFG);

	GetClientRect(m_MaxClientRect);
	BITMAPINFO bmpinfo;
	bmpinfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth         = GetSystemMetrics(SM_CXSCREEN);		// ��������ͼƬ, �ߴ���Ϊ��Ļ�ߴ���
	bmpinfo.bmiHeader.biHeight        = GetSystemMetrics(SM_CYSCREEN);		// ��������ͼƬ, �ߴ�߶�Ϊ��Ļ�ߴ���
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

	OnNextCfgRecord();
}

// ��Ӱ�����Ӧ����
void CSetupView::OnAppendCfgRecord()
{
	switch (m_iViewIndex)
	{
	case 0:
		AppendKilnInfoGridView();
		break;

	case 1:
		AppendTerminalInfoGridView();
		break;

	default:
		break;
	}
}

// ɾ��������Ӧ����
void CSetupView::OnDeleteCfgRecord()
{
	switch (m_iViewIndex)
	{
	case 0:
		DeleteKilnInfoGridView();
		break;

	case 1:
		DeleteTerminalInfoGridView();
		break;
		
	default:
		break;
	}
}

// ��һ��������Ӧ����
void CSetupView::OnNextCfgRecord()
{
	int i, j;
	int kiln_chn, tem_chn;

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	if (m_iViewIndex < 4)
	{
		m_iViewIndex++;
	}

	switch (m_iViewIndex)
	{
	case 0:					// שҤ���ý���
		pMainFrame->SetFrameCaption("שҤ���ý���");
		m_pSetButton->EnableWindow(TRUE);
		m_pDelButton->EnableWindow(TRUE);
		m_pNextButton->EnableWindow(TRUE);
		SetKilnInfoGridView(0, THE_KILN_INFO_TBL_COL);
		break;

	case 1:
		// �ն���Ϣ��������Ϊ0
		if (m_KilnInfoMap.size() <= 0)
		{
			MessageBox("שҤ��������Ϊ0, ����������", "���ô���", MB_OK);
			m_iViewIndex = 0;			// ����֮ǰ����
			break;
		}

		pMainFrame->SetFrameCaption("�ն����ý���");
		m_pSetButton->EnableWindow(TRUE);
		m_pDelButton->EnableWindow(TRUE);
		m_pNextButton->EnableWindow(TRUE);
		SetTemInfoGridView(0, THE_TERM_INFO_TBL_COL);
		break;

	case 2:	
		// �ն���Ϣ��������Ϊ0
		if (m_TerminalInfoMap.size() <= 0)
		{
			MessageBox("�ն˸�������Ϊ0, ����������", "���ô���", MB_OK);
			m_iViewIndex = 1;			// ����֮ǰ����
			break;
		}

		for (tem_chn = 0, i = 0; i < m_TerminalInfoMap.size(); i++)
		{
			tem_chn += m_TerminalInfoMap[i].iTemChn;
		}

		for (kiln_chn = 0, i = 0; i < m_KilnInfoMap.size(); i++)
		{
			kiln_chn += m_KilnInfoMap[i].iTempNum 
						+ m_KilnInfoMap[i].iTempANum 
						+ m_KilnInfoMap[i].iPressNum 
						+ m_KilnInfoMap[i].iHumidityNum
						+ (m_KilnInfoMap[i].iMotorNum * 2)
						+ (m_KilnInfoMap[i].iGrossNum * 2);
		}

		if (kiln_chn > tem_chn)
		{
			MessageBox("�ն�ͨ������С��שҤ��Ҫ��ͨ����, ����������", "���ô���", MB_OK);
			m_iViewIndex = 1;			// ����֮ǰ����
			break;
		}
		
		pMainFrame->SetFrameCaption("ͨ�����ý���");
		m_pSetButton->EnableWindow(FALSE);
		m_pDelButton->EnableWindow(FALSE);
		m_pNextButton->EnableWindow(TRUE);	
		SetChannelInfoGridView(kiln_chn, THE_CHAN_INFO_TBL_COL);
		SetChannelInfoCellStyle();
		for (j = 0; j < kiln_chn; j++)
		{
			SetChannelInfoComboStyle(j + 1, FALSE);
		}

		// ����Զ��������п��
		m_pGrid->AutoSizeColumns(GVS_BOTH);
		m_pGrid->AutoSizeRows();
		for (i = 0; i < (kiln_chn + 1); i++)
		{
			for (j = 0; j < (THE_CHAN_INFO_TBL_COL + 1); j++)
			{
				m_pGrid->SetItemFormat(i, j, DT_CENTER | DT_VCENTER);
			}
		}
		break;

	case 3:
		OpenFile(pDoc->m_strCFGPath);
		WriteCfgHeader();
		
		CreateLoginTable();								// ����LoginInfo��
		CreateComCfgTable();							// ����ComCfgInfo��
		CreateTerminalCfgTable();						// ����TerminalCfgInfo�������˼�¼
		CreateKilnInfoCfgTable();						// ����KilnInfo�������˼�¼
		CreateAutoCtrlCfgTable();						// ����AutoCtrlInfo�������¼
		CreateStandardTable();							// ����StandardCfgInfo�������˼�¼
		CreateChannelCfgTable();						// ����ͨ�����ñ�
		CreateOutputsTable();							// ����������¼��
		CreateHistoryTable();							// ����History��
		WriteCfgLast();
		CloseFile();
			
		pMainFrame->SetFrameCaption("���ݿ������ļ��Ѿ�����");
		
		m_pSetButton->EnableWindow(FALSE);
		m_pDelButton->EnableWindow(FALSE);
		m_pNextButton->EnableWindow(FALSE);
		break;

	default:
		break;
	}
}

// ���שҤ��Ϣ��¼
void CSetupView::AppendKilnInfoGridView(void)
{
	if (m_iViewIndex != 0)
	{
		return;
	}

	int new_row = m_pGrid->InsertRow(NULL, -1);
	SetKilnInfoCellStyle(m_pGrid->GetRowCount() - 1);
	SetKilnInfoComboStyle(m_pGrid->GetRowCount() - 1);

	m_pGrid->AutoSizeColumns(GVS_BOTH);
	m_pGrid->AutoSizeRow(new_row, TRUE);

	for (int i = 0; i < m_pGrid->GetColumnCount(); i++)
	{
		m_pGrid->SetItemFormat(new_row, i, DT_CENTER | DT_VCENTER);
	}
	m_pGrid->Refresh();					// ˢ�±��
}

// ����ն���Ϣ��¼
void CSetupView::AppendTerminalInfoGridView(void)
{
	if (m_iViewIndex != 1)
	{
		return;
	}
	
	int new_row = m_pGrid->InsertRow(NULL, -1);
	SetTemInfoCellStyle(m_pGrid->GetRowCount() - 1);
	SetTemInfoComboStyle(m_pGrid->GetRowCount() - 1);

	m_pGrid->AutoSizeColumns(GVS_BOTH);
	m_pGrid->AutoSizeRow(new_row, TRUE);

	for (int i = 0; i < m_pGrid->GetColumnCount(); i++)
	{
		m_pGrid->SetItemFormat(new_row, i, DT_CENTER | DT_VCENTER);
	}
	m_pGrid->Refresh();					// ˢ�±��
}

// ɾ���û���Ϣ��¼
void CSetupView::DeleteKilnInfoGridView(void)
{
	if (m_iViewIndex != 0 || m_iSelectRow == 0)
	{
		return;
	}
	
	if (m_KilnInfoMap.size() <= 0)
	{
		return;
	}	
	
	//////////////////////////////////////////////////////////////////////////
	// ˼·: ��������ȫ������, Ȼ�������д��
	//////////////////////////////////////////////////////////////////////////
	KILN_INFO *p_kiln_info = NULL;
	int cnt = m_KilnInfoMap.size();
	if (cnt <= 1)						// ���ֻ��1�����ݾ�ֱ���������
	{
		m_KilnInfoMap.clear();
	}
	else if ((m_iSelectRow - 1) < cnt)	// ָ����ɾ���б���Ϸ�, m_iSelectRow������Ϊ0, ��Ϊ��ͷ����Ϊ����ѡ��
	{
		p_kiln_info = new KILN_INFO[cnt - 1];
		for (int i = 0, j = 0; (i < cnt) && (j < (cnt - 1)); i++)
		{
			// m_iSelectRow������Ϊ0, ��Ϊ��ͷ����Ϊ����ѡ��
			if (i != (m_iSelectRow - 1))
			{
				p_kiln_info[j++] = m_KilnInfoMap[i];
			}
		}
		
		m_KilnInfoMap.clear();
		for (i = 0; i < j; i++)
		{
			m_KilnInfoMap.push_back(p_kiln_info[i]);
		}
	}
	
	if (p_kiln_info != NULL)
	{
		delete [] p_kiln_info;
	}
	
	m_pGrid->DeleteRow(m_iSelectRow);			// ɾ��ѡ����
	m_pGrid->Refresh();							// ˢ�±��
}

// ɾ���ն���Ϣ��¼
void CSetupView::DeleteTerminalInfoGridView(void)
{
	if (m_iViewIndex != 1 || m_iSelectRow == 0)
	{
		return;
	}
	
	if (m_TerminalInfoMap.size() <= 0)
	{
		return;
	}	
	
	//////////////////////////////////////////////////////////////////////////
	// ˼·: ��������ȫ������, Ȼ�������д��
	//////////////////////////////////////////////////////////////////////////
	LPTERM_INFO pTerminalInfo = NULL;
	int cnt = m_TerminalInfoMap.size();
	if (cnt <= 1)						// ���ֻ��1�����ݾ�ֱ���������
	{
		m_TerminalInfoMap.clear();
	}
	else if ((m_iSelectRow - 1) < cnt)	// ָ����ɾ���б���Ϸ�, m_iSelectRow������Ϊ0, ��Ϊ��ͷ����Ϊ����ѡ��
	{
		pTerminalInfo = new TERM_INFO[cnt - 1];
		for (int i = 0, j = 0; (i < cnt) && (j < (cnt - 1)); i++)
		{
			// m_iSelectRow������Ϊ0, ��Ϊ��ͷ����Ϊ����ѡ��
			if (i != (m_iSelectRow - 1))
			{
				pTerminalInfo[j++] = m_TerminalInfoMap[i];
			}
		}
		
		m_TerminalInfoMap.clear();
		for (i = 0; i < j; i++)
		{
			m_TerminalInfoMap.push_back(pTerminalInfo[i]);
		}
	}
	
	if (pTerminalInfo != NULL)
	{
		delete [] pTerminalInfo;
	}
	
	m_pGrid->DeleteRow(m_iSelectRow);			// ɾ��ѡ����
	m_pGrid->Refresh();							// ˢ�±��
}

void CSetupView::SetKilnInfoCellStyle(int iRow)
{
	for (int i = 0; i < 11; i++)
	{
		m_pGrid->SetItemFont(iRow, i, &m_lfCellFont);				// ��������
		m_pGrid->SetItemFgColour(iRow, i, THE_IN_GRID_FT_COLOR);	// ����������ɫ
	}

	KILN_INFO kiln_info;
	CString str;

	str.Format("%d", iRow);
	m_pGrid->SetItemText(iRow, 0, str);

	kiln_info.KilnType = 0;

	kiln_info.iTotalCars = 50;
	str.Format("%d", kiln_info.iTotalCars);
	m_pGrid->SetItemText(iRow, 2, str);

	kiln_info.iTempNum = 16;
	str.Format("%d", kiln_info.iTempNum);
	m_pGrid->SetItemText(iRow, 3, str);

	kiln_info.iTempANum = 8;
	str.Format("%d", kiln_info.iTempANum);
	m_pGrid->SetItemText(iRow, 4, str);

	kiln_info.iPressNum = 0;
	str.Format("%d", kiln_info.iPressNum);
	m_pGrid->SetItemText(iRow, 5, str);

	kiln_info.iHumidityNum = 0;
	str.Format("%d", kiln_info.iHumidityNum);
	m_pGrid->SetItemText(iRow, 6, str);

	kiln_info.iMotorNum = 1;
	str.Format("%d", kiln_info.iMotorNum);
	m_pGrid->SetItemText(iRow, 7, str);

	kiln_info.iGrossNum = 0;
	kiln_info.iCtrlNum = 0;

	kiln_info.iStandardIndex = 1;
	str.Format("%d", kiln_info.iStandardIndex);
	m_pGrid->SetItemText(iRow, 10, str);

	m_KilnInfoMap.push_back(kiln_info);
}

void CSetupView::SetKilnInfoComboStyle(int iRow)
{
	CGridCellCombo *pCell;
	CStringArray options[3];

	/************************************************************************/
	/* Ҥ��������                                                           */
	/************************************************************************/
	if (!m_pGrid->SetCellType(iRow, 1, RUNTIME_CLASS(CGridCellCombo)))
		return;
	
	m_pGrid->SetItemText(iRow, 1, _T("0 = ����Ҥ"));
	
	options[0].Add(_T("0 = ����Ҥ"));
	options[0].Add(_T("1 = ���Ҥ"));
	options[0].Add(_T("2 = ֱͨҤ"));
	options[0].Add(_T("3 = �ƶ�Ҥ"));
	
	pCell = (CGridCellCombo*) m_pGrid->GetCell(iRow, 1);
	pCell->SetOptions(options[0]);
	pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE

	/************************************************************************/
	/* �Ƿ������������                                                     */
	/************************************************************************/
	if (!m_pGrid->SetCellType(iRow, 8, RUNTIME_CLASS(CGridCellCombo)))
		return;
	
	m_pGrid->SetItemText(iRow, 8, _T("0 = ����������"));
	
	options[1].Add(_T("0 = ����������"));
	options[1].Add(_T("1 = ����Ҫ����"));
	
	pCell = (CGridCellCombo*) m_pGrid->GetCell(iRow, 8);
	pCell->SetOptions(options[1]);
	pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE

	/************************************************************************/
	/* �Ƿ��Զ���������                                                     */
	/************************************************************************/
	if (!m_pGrid->SetCellType(iRow, 9, RUNTIME_CLASS(CGridCellCombo)))
		return;
	
	m_pGrid->SetItemText(iRow, 9, _T("0 = ��Ҫ�Զ�����"));
	
	options[2].Add(_T("0 = ��Ҫ�Զ�����"));
	options[2].Add(_T("1 = ��Ҫ�Զ�����"));
	
	pCell = (CGridCellCombo*) m_pGrid->GetCell(iRow, 9);
	pCell->SetOptions(options[2]);
	pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
}

void CSetupView::SetKilnInfoGridView(int iRowNum, int iColNum)
{
	// ��������ı�����ɫ
	m_pGrid->GetDefaultCell(FALSE, FALSE)->SetBackClr(THE_GRID_BACKGROUND_COLOR);
	// ������ͼ�ı�����ɫ
	m_pGrid->SetGridBkColor(THE_VIEW_BACKGROUND_COLOR);
	
// 	m_pGrid->SetDefCellHeight(iCellHeight);
// 	m_pGrid->SetDefCellWidth(iCellWidth);
	
	// �������ⷽʽ
	m_pGrid->SetVirtualMode(FALSE);
	// ���ù̶����ͷ��������
	m_pGrid->SetFixedRowCount(1);
	m_pGrid->SetFixedColumnCount(1);
	
	// +1����̶����ͷ���и���
	m_pGrid->SetRowCount(iRowNum + 1);			
	m_pGrid->SetColumnCount(iColNum + 1);
	
	m_pGrid->SetEditable(TRUE);					// ����༭����
	m_pGrid->EnableSelection(TRUE);				// ����ѡ��Ԫ
	
	m_pGrid->SetRowResize(FALSE);				// �������п����
	m_pGrid->SetColumnResize(TRUE);				// �����п����	
	
	m_pGrid->SetFixedRowSelection(TRUE);		// ������ѡ��
    m_pGrid->SetFixedColumnSelection(FALSE);	// ��������ѡ��
	m_pGrid->SetTrackFocusCell(FALSE);			// �̶���Ԫ�����н���	

	for (int i = 0; i < (iColNum + 1); i++)
	{
		m_pGrid->SetItemFont(0, i, &m_lfCellFont);				// ��������
		m_pGrid->SetItemFgColour(0, i, THE_IN_GRID_FT_COLOR);	// ����������ɫ
	}
	
	m_pGrid->SetItemText(0, 0,  "Ҥ���");
	m_pGrid->SetItemText(0, 1,  "Ҥ����");
	m_pGrid->SetItemText(0, 2,  "�ܳ�λ(����)");
	m_pGrid->SetItemText(0, 3,  "�¶�����");
	m_pGrid->SetItemText(0, 4,  "��������");
	m_pGrid->SetItemText(0, 5,  "ѹ������");
	m_pGrid->SetItemText(0, 6,  "ʪ������");
	m_pGrid->SetItemText(0, 7,  "�������");
	m_pGrid->SetItemText(0, 8,  "��������");
	m_pGrid->SetItemText(0, 9, "�Զ�����");
	m_pGrid->SetItemText(0, 10, "��׼����");

	// ����Զ��������п��
	m_pGrid->AutoSizeColumns(GVS_BOTH);
	m_pGrid->AutoSizeRow(0, TRUE);
	for (i = 0; i < (iColNum + 1); i++)
	{
		m_pGrid->SetItemFormat(0, i, DT_CENTER | DT_VCENTER);
	}

	m_pGrid->Refresh();							// ˢ�±��
}

void CSetupView::SetTemInfoCellStyle(int iRow)
{
	for (int i = 0; i < 6; i++)
	{
		m_pGrid->SetItemFont(iRow, i, &m_lfCellFont);				// ��������
		m_pGrid->SetItemFgColour(iRow, i, THE_IN_GRID_FT_COLOR);	// ����������ɫ
	}
	
	TERM_INFO term_info;
	CString str;
	
	str.Format("%d", iRow);
	m_pGrid->SetItemText(iRow, 0, str);
	
	term_info.iTemId = 14 + iRow;
	str.Format("%d", term_info.iTemId);
	m_pGrid->SetItemText(iRow, 1, str);
	
	term_info.iTemCmd = 4;
	str.Format("%d", term_info.iTemCmd);
	m_pGrid->SetItemText(iRow, 2, str);
	
	term_info.iTemChn = 10;
	str.Format("%d", term_info.iTemChn);
	m_pGrid->SetItemText(iRow, 3, str);
	
	term_info.iTemAddr = 30001;
	str.Format("%d", term_info.iTemAddr);
	m_pGrid->SetItemText(iRow, 4, str);
	
	term_info.iTemType = 0;
	
	m_TerminalInfoMap.push_back(term_info);
}

void CSetupView::SetTemInfoComboStyle(int iRow)
{
	CGridCellCombo *pCell;
	CStringArray options;
	
	/************************************************************************/
	/* Ҥ��������                                                           */
	/************************************************************************/
	if (!m_pGrid->SetCellType(iRow, 5, RUNTIME_CLASS(CGridCellCombo)))
		return;
	
	m_pGrid->SetItemText(iRow, 5, _T("0 = ������"));
	
	options.Add(_T("0 = ������"));
	options.Add(_T("1 = �¼���"));
	
	pCell = (CGridCellCombo*) m_pGrid->GetCell(iRow, 5);
	pCell->SetOptions(options);
	pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
}

void CSetupView::SetTemInfoGridView(int iRowNum, int iColNum)
{
	// ��������ı�����ɫ
	m_pGrid->GetDefaultCell(FALSE, FALSE)->SetBackClr(THE_GRID_BACKGROUND_COLOR);
	// ������ͼ�ı�����ɫ
	m_pGrid->SetGridBkColor(THE_VIEW_BACKGROUND_COLOR);
	
// 	m_pGrid->SetDefCellHeight(iCellHeight);
// 	m_pGrid->SetDefCellWidth(iCellWidth);
	
	// �������ⷽʽ
	m_pGrid->SetVirtualMode(FALSE);
	// ���ù̶����ͷ��������
	m_pGrid->SetFixedRowCount(1);
	m_pGrid->SetFixedColumnCount(1);
	
	// +1����̶����ͷ���и���
	m_pGrid->SetRowCount(iRowNum + 1);			
	m_pGrid->SetColumnCount(iColNum + 1);

	m_pGrid->SetEditable(TRUE);					// ����༭����
	m_pGrid->EnableSelection(TRUE);				// ����ѡ��Ԫ
	
	m_pGrid->SetRowResize(FALSE);				// �������п����
	m_pGrid->SetColumnResize(TRUE);				// �����п����	
	
	m_pGrid->SetFixedRowSelection(TRUE);		// ������ѡ��
    m_pGrid->SetFixedColumnSelection(FALSE);	// ��������ѡ��
	m_pGrid->SetTrackFocusCell(FALSE);			// �̶���Ԫ�����н���	
	
	for (int i = 0; i < (iColNum + 1); i++)
	{
		m_pGrid->SetItemFont(0, i, &m_lfCellFont);				// ��������
		m_pGrid->SetItemFgColour(0, i, THE_IN_GRID_FT_COLOR);	// ����������ɫ
	}
	
	m_pGrid->SetItemText(0, 0,  "ģ�����");
	m_pGrid->SetItemText(0, 1,  "ģ���ַ");
	m_pGrid->SetItemText(0, 2,  "ģ������");
	m_pGrid->SetItemText(0, 3,  "ģ��ͨ����");
	m_pGrid->SetItemText(0, 4,  "ģ��Ĵ�����ַ");
	m_pGrid->SetItemText(0, 5,  "ģ������");

	// ����Զ��������п��
	m_pGrid->AutoSizeColumns(GVS_BOTH);
	m_pGrid->AutoSizeRow(0, TRUE);
	for (i = 0; i < (iColNum + 1); i++)
	{
		m_pGrid->SetItemFormat(0, i, DT_CENTER | DT_VCENTER);
	}
	
	m_pGrid->Refresh();							// ˢ�±��
}

void CSetupView::SetChannelInfoCellStyle(void)
{
	int i, j, total = 1;
	CHANNEL_INFO channel_info;
	CString str;

	for (i = 0; i < m_KilnInfoMap.size(); i++)
	{
		/************************************************************************/
		/* �¶�����˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iTempNum; j++, total++)
		{
			channel_info.iKilnIdx = i + 1;
			str.Format("%d", channel_info.iKilnIdx);
			m_pGrid->SetItemText(total, 1, str);

			channel_info.iTemIdx = 0;
			channel_info.iPhyIdx = 0;
			
			channel_info.iChnTyp = 0;
			str.Format("%d", channel_info.iChnTyp);
			m_pGrid->SetItemText(total, 4, str);

			channel_info.iCarIdx = j;
			str.Format("%d", channel_info.iCarIdx);
			m_pGrid->SetItemText(total, 5, str);

			channel_info.strName[0] = '\0';
			strcpy(channel_info.strName, "�Զ��ź�");
			m_pGrid->SetItemText(total, 6, "�Զ��ź�");

			channel_info.AdjGain = 1.0;
			str.Format("%3.2f", channel_info.AdjGain);
			m_pGrid->SetItemText(total, 7, str);

			channel_info.AdjZero = 0.0;
			str.Format("%3.2f", channel_info.AdjZero);
			m_pGrid->SetItemText(total, 8, str);

			m_ChannelInfoMap.push_back(channel_info);
		}
	
		/************************************************************************/
		/* ��������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iTempANum; j++, total++)
		{
			channel_info.iKilnIdx = i + 1;
			str.Format("%d", channel_info.iKilnIdx);
			m_pGrid->SetItemText(total, 1, str);
			
			channel_info.iTemIdx = 0;
			channel_info.iPhyIdx = 0;
			
			channel_info.iChnTyp = 1;
			str.Format("%d", channel_info.iChnTyp);
			m_pGrid->SetItemText(total, 4, str);
			
			channel_info.iCarIdx = j;
			str.Format("%d", channel_info.iCarIdx);
			m_pGrid->SetItemText(total, 5, str);
			
			channel_info.strName[0] = '\0';
			strcpy(channel_info.strName, "��������");
			m_pGrid->SetItemText(total, 6, "��������");
			
			channel_info.AdjGain = 1.0;
			str.Format("%3.2f", channel_info.AdjGain);
			m_pGrid->SetItemText(total, 7, str);
			
			channel_info.AdjZero = 0.0;
			str.Format("%3.2f", channel_info.AdjZero);
			m_pGrid->SetItemText(total, 8, str);
			
			m_ChannelInfoMap.push_back(channel_info);
		}

		/************************************************************************/
		/* ѹ������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iPressNum; j++, total++)
		{
			channel_info.iKilnIdx = i + 1;
			str.Format("%d", channel_info.iKilnIdx);
			m_pGrid->SetItemText(total, 1, str);
			
			channel_info.iTemIdx = 0;
			channel_info.iPhyIdx = 0;
			
			channel_info.iChnTyp = 2;
			str.Format("%d", channel_info.iChnTyp);
			m_pGrid->SetItemText(total, 4, str);
			
			channel_info.iCarIdx = j;
			str.Format("%d", channel_info.iCarIdx);
			m_pGrid->SetItemText(total, 5, str);
			
			channel_info.strName[0] = '\0';
			strcpy(channel_info.strName, "ѹ������");
			m_pGrid->SetItemText(total, 6, "ѹ������");
			
			channel_info.AdjGain = 1.0;
			str.Format("%3.2f", channel_info.AdjGain);
			m_pGrid->SetItemText(total, 7, str);
			
			channel_info.AdjZero = 0.0;
			str.Format("%3.2f", channel_info.AdjZero);
			m_pGrid->SetItemText(total, 8, str);
			
			m_ChannelInfoMap.push_back(channel_info);
		}

		/************************************************************************/
		/* ʪ������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iHumidityNum; j++, total++)
		{
			channel_info.iKilnIdx = i + 1;
			str.Format("%d", channel_info.iKilnIdx);
			m_pGrid->SetItemText(total, 1, str);
			
			channel_info.iTemIdx = 0;
			channel_info.iPhyIdx = 0;
			
			channel_info.iChnTyp = 3;
			str.Format("%d", channel_info.iChnTyp);
			m_pGrid->SetItemText(total, 4, str);
			
			channel_info.iCarIdx = j;
			str.Format("%d", channel_info.iCarIdx);
			m_pGrid->SetItemText(total, 5, str);
			
			channel_info.strName[0] = '\0';
			strcpy(channel_info.strName, "ʪ������");
			m_pGrid->SetItemText(total, 6, "ʪ������");
			
			channel_info.AdjGain = 1.0;
			str.Format("%3.2f", channel_info.AdjGain);
			m_pGrid->SetItemText(total, 7, str);
			
			channel_info.AdjZero = 0.0;
			str.Format("%3.2f", channel_info.AdjZero);
			m_pGrid->SetItemText(total, 8, str);
			
			m_ChannelInfoMap.push_back(channel_info);
		}

		/************************************************************************/
		/* �������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iMotorNum; j++, total++)
		{
			channel_info.iKilnIdx = i + 1;
			str.Format("%d", channel_info.iKilnIdx);
			m_pGrid->SetItemText(total, 1, str);
			
			channel_info.iTemIdx = 0;
			channel_info.iPhyIdx = 0;
			
			channel_info.iChnTyp = 4;
			str.Format("%d", channel_info.iChnTyp);
			m_pGrid->SetItemText(total, 4, str);
			
			channel_info.iCarIdx = j;
			str.Format("%d", channel_info.iCarIdx);
			m_pGrid->SetItemText(total, 5, str);
			
			channel_info.strName[0] = '\0';
			strcpy(channel_info.strName, "�������");
			m_pGrid->SetItemText(total, 6, "�������");
			
			channel_info.AdjGain = 1.0;
			str.Format("%3.2f", channel_info.AdjGain);
			m_pGrid->SetItemText(total, 7, str);
			
			channel_info.AdjZero = 0.0;
			str.Format("%3.2f", channel_info.AdjZero);
			m_pGrid->SetItemText(total, 8, str);
			
			m_ChannelInfoMap.push_back(channel_info);
		}

		/************************************************************************/
		/* ��������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iGrossNum; j++, total++)
		{
			channel_info.iKilnIdx = i + 1;
			str.Format("%d", channel_info.iKilnIdx);
			m_pGrid->SetItemText(total, 1, str);
			
			channel_info.iTemIdx = 0;
			channel_info.iPhyIdx = 0;
			
			channel_info.iChnTyp = 5;
			str.Format("%d", channel_info.iChnTyp);
			m_pGrid->SetItemText(total, 4, str);
			
			channel_info.iCarIdx = j;
			str.Format("%d", channel_info.iCarIdx);
			m_pGrid->SetItemText(total, 5, str);
			
			channel_info.strName[0] = '\0';
			strcpy(channel_info.strName, "�������");
			m_pGrid->SetItemText(total, 6, "�������");
			
			channel_info.AdjGain = 1.0;
			str.Format("%3.2f", channel_info.AdjGain);
			m_pGrid->SetItemText(total, 7, str);
			
			channel_info.AdjZero = 0.0;
			str.Format("%3.2f", channel_info.AdjZero);
			m_pGrid->SetItemText(total, 8, str);
			
			m_ChannelInfoMap.push_back(channel_info);
		}

		/************************************************************************/
		/* ��������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iGrossNum; j++, total++)
		{
			channel_info.iKilnIdx = i + 1;
			str.Format("%d", channel_info.iKilnIdx);
			m_pGrid->SetItemText(total, 1, str);
			
			channel_info.iTemIdx = 0;
			channel_info.iPhyIdx = 0;
			
			channel_info.iChnTyp = 6;
			str.Format("%d", channel_info.iChnTyp);
			m_pGrid->SetItemText(total, 4, str);
			
			channel_info.iCarIdx = j;
			str.Format("%d", channel_info.iCarIdx);
			m_pGrid->SetItemText(total, 5, str);
			
			channel_info.strName[0] = '\0';
			strcpy(channel_info.strName, "�������");
			m_pGrid->SetItemText(total, 6, "�������");
			
			channel_info.AdjGain = 1.0;
			str.Format("%3.2f", channel_info.AdjGain);
			m_pGrid->SetItemText(total, 7, str);
			
			channel_info.AdjZero = 0.0;
			str.Format("%3.2f", channel_info.AdjZero);
			m_pGrid->SetItemText(total, 8, str);
			
			m_ChannelInfoMap.push_back(channel_info);
		}

		/************************************************************************/
		/* �������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iMotorNum; j++, total++)
		{
			channel_info.iKilnIdx = i + 1;
			str.Format("%d", channel_info.iKilnIdx);
			m_pGrid->SetItemText(total, 1, str);
			
			channel_info.iTemIdx = 0;
			channel_info.iPhyIdx = 0;
			
			channel_info.iChnTyp = 7;
			str.Format("%d", channel_info.iChnTyp);
			m_pGrid->SetItemText(total, 4, str);
			
			channel_info.iCarIdx = j;
			str.Format("%d", channel_info.iCarIdx);
			m_pGrid->SetItemText(total, 5, str);
			
			channel_info.strName[0] = '\0';
			strcpy(channel_info.strName, "�������");
			m_pGrid->SetItemText(total, 6, "�������");
			
			channel_info.AdjGain = 1.0;
			str.Format("%3.2f", channel_info.AdjGain);
			m_pGrid->SetItemText(total, 7, str);
			
			channel_info.AdjZero = 0.0;
			str.Format("%3.2f", channel_info.AdjZero);
			m_pGrid->SetItemText(total, 8, str);
			
			m_ChannelInfoMap.push_back(channel_info);
		}
	}

	for (i = 0; i < m_ChannelInfoMap.size(); i++)
	{
		str.Format("%d", i + 1);
		m_pGrid->SetItemText(i + 1, 0, str);
	}

	for (i = 0; i < m_ChannelInfoMap.size(); i++)
	{
		for (j = 0; j < 9; j++)
		{
			m_pGrid->SetItemFont(i + 1, j, &m_lfCellFont);				// ��������
			m_pGrid->SetItemFgColour(i + 1, j, THE_IN_GRID_FT_COLOR);	// ����������ɫ
		}
	}

	m_pGrid->Refresh();
}

void CSetupView::SetChannelInfoComboStyle(int iRow, BOOL only_fresh_phy)
{
	int i;
	CString str, str1;
	CGridCellCombo *pCell;
	CStringArray options[2];
	
	if (only_fresh_phy == FALSE)
	{
		/************************************************************************/
		/* ����ģ���������                                                     */
		/************************************************************************/
		if (!m_pGrid->SetCellType(iRow, 2, RUNTIME_CLASS(CGridCellCombo)))
			return;
		
		for (i = 0; i < m_TerminalInfoMap.size(); i++)
		{
			if ((m_TerminalInfoMap[i].iTemCmd == 0x04) && (m_TerminalInfoMap[i].iTemType == 0x00) && (m_TerminalInfoMap[i].iTemAddr == 30001))
			{
				str1.Format(" AI ���ģ��");
			}
			else if ((m_TerminalInfoMap[i].iTemCmd == 0x03) && (m_TerminalInfoMap[i].iTemType == 0x00) && (m_TerminalInfoMap[i].iTemAddr == 40212))
			{
				str1.Format("�������ģ��");
			}
			else if ((m_TerminalInfoMap[i].iTemCmd == 0x06) && (m_TerminalInfoMap[i].iTemType == 0x01) && (m_TerminalInfoMap[i].iTemAddr == 40312))
			{
				str1.Format("�������ģ��");
			}
			else if ((m_TerminalInfoMap[i].iTemCmd == 0x03) && (m_TerminalInfoMap[i].iTemType == 0x00) && (m_TerminalInfoMap[i].iTemAddr == 40001))
			{
				str1.Format("������ģ��");
			}
			else if ((m_TerminalInfoMap[i].iTemCmd == 0x06) && (m_TerminalInfoMap[i].iTemType == 0x01) && (m_TerminalInfoMap[i].iTemAddr == 40001))
			{
				str1.Format("�������ģ��");
			}
			else
			{
				str1.Format("����=%d, ����=%d, �Ĵ���:%d", m_TerminalInfoMap[i].iTemCmd, 
														   m_TerminalInfoMap[i].iTemType, 
														   m_TerminalInfoMap[i].iTemAddr);
			}

			str.Format("%02d = [��ַ=%02d, %s]", i, 
												 m_TerminalInfoMap[i].iTemId, 
												 str1);
			options[0].Add(str);

			if (i == 0)
			{
				m_pGrid->SetItemText(iRow, 2, str);
			}
		}
		
		pCell = (CGridCellCombo*) m_pGrid->GetCell(iRow, 2);
		pCell->SetOptions(options[0]);
		pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
	}

	
	/************************************************************************/
	/* ��������ͨ������                                                     */
	/************************************************************************/
	if (!m_pGrid->SetCellType(iRow, 3, RUNTIME_CLASS(CGridCellCombo)))
		return;
	
	for (i = 0; i < m_TerminalInfoMap[m_ChannelInfoMap[iRow - 1].iTemIdx].iTemChn; i++)
	{
		str.Format("%02d = [%dͨ��]", i, i + 1);
		options[1].Add(str);
		
		if (i == 0)
		{
			m_pGrid->SetItemText(iRow, 3, str);
		}
	}
	
	pCell = (CGridCellCombo*) m_pGrid->GetCell(iRow, 3);
	pCell->SetOptions(options[1]);
	pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
}

void CSetupView::SetChannelInfoGridView(int iRowNum, int iColNum)
{
	// ��������ı�����ɫ
	m_pGrid->GetDefaultCell(FALSE, FALSE)->SetBackClr(THE_GRID_BACKGROUND_COLOR);
	// ������ͼ�ı�����ɫ
	m_pGrid->SetGridBkColor(THE_VIEW_BACKGROUND_COLOR);
	
// 	m_pGrid->SetDefCellHeight(iCellHeight);
// 	m_pGrid->SetDefCellWidth(iCellWidth);
	
	// �������ⷽʽ
	m_pGrid->SetVirtualMode(FALSE);
	// ���ù̶����ͷ��������
	m_pGrid->SetFixedRowCount(1);
	m_pGrid->SetFixedColumnCount(1);
	
	// +1����̶����ͷ���и���
	m_pGrid->SetRowCount(iRowNum + 1);			
	m_pGrid->SetColumnCount(iColNum + 1);
	
	m_pGrid->SetEditable(TRUE);					// ����༭����
	m_pGrid->EnableSelection(TRUE);				// ����ѡ��Ԫ
	
	m_pGrid->SetRowResize(FALSE);				// �������п����
	m_pGrid->SetColumnResize(TRUE);				// �����п����	
	
	m_pGrid->SetFixedRowSelection(TRUE);		// ������ѡ��
    m_pGrid->SetFixedColumnSelection(FALSE);	// ��������ѡ��
	m_pGrid->SetTrackFocusCell(FALSE);			// �̶���Ԫ�����н���	
	
	m_pGrid->SetItemText(0, 0,  "ͨ�����");
	m_pGrid->SetItemText(0, 1,  "����Ҥ��");
	m_pGrid->SetItemText(0, 2,  "����ģ���");
	m_pGrid->SetItemText(0, 3,  "����ͨ����");
	m_pGrid->SetItemText(0, 4,  "ͨ������");
	m_pGrid->SetItemText(0, 5,  "��λ��(����)");
	m_pGrid->SetItemText(0, 6,  "ͨ������");
	m_pGrid->SetItemText(0, 7,  "����ϵ��");
	m_pGrid->SetItemText(0, 8,  "���ϵ��");

	// ����Զ��������п��
	m_pGrid->AutoSizeColumns(GVS_BOTH);
	m_pGrid->AutoSizeRows();

	for (int i = 0; i < (iColNum + 1); i++)
	{
		m_pGrid->SetItemFont(0, i, &m_lfCellFont);				// ��������
		m_pGrid->SetItemFgColour(0, i, THE_IN_GRID_FT_COLOR);	// ����������ɫ
	}

	m_pGrid->Refresh();							// ˢ�±��
}

// ��ʼ�����
void CSetupView::SetGridView(int iRowNum, int iColNum)
{					
	// ��������ı�����ɫ
	m_pGrid->GetDefaultCell(FALSE, FALSE)->SetBackClr(THE_GRID_BACKGROUND_COLOR);
	// ������ͼ�ı�����ɫ
	m_pGrid->SetGridBkColor(THE_VIEW_BACKGROUND_COLOR);
	
	m_pGrid->SetDefCellHeight(THE_DEFAULT_CELL_HEIGHT);
	m_pGrid->SetDefCellWidth(THE_DEFAULT_CELL_WIDTH);
	
	// �������ⷽʽ
	m_pGrid->SetVirtualMode(TRUE);
	// ���ù̶����ͷ��������
	m_pGrid->SetFixedRowCount(1);
	m_pGrid->SetFixedColumnCount(1);
	
	// +1����̶����ͷ���и���
	m_pGrid->SetRowCount(iRowNum + 1);			
	m_pGrid->SetColumnCount(iColNum + 1);
	
	m_pGrid->SetEditable(TRUE);					// ����༭����
	m_pGrid->EnableSelection(TRUE);				// ����ѡ��Ԫ
	
	m_pGrid->SetRowResize(FALSE);				// �������п����
	m_pGrid->SetColumnResize(TRUE);				// �����п����	
	
	m_pGrid->SetFixedRowSelection(TRUE);		// ������ѡ��
    m_pGrid->SetFixedColumnSelection(FALSE);	// ��������ѡ��
	m_pGrid->SetTrackFocusCell(FALSE);			// �̶���Ԫ�����н���
	
	m_pGrid->Refresh();							// ˢ�±��
}

BOOL CSetupView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// ����ռ䷢������Ϣ
	if (wParam == (WPARAM)m_pGrid->GetDlgCtrlID())
	{
		*pResult = 0;
		GV_DISPINFO *pDispInfo = (GV_DISPINFO *)lParam;
		
		// ������ʾ��Ϣ, ����ģʽ����Ч
		if (GVN_GETDISPINFO == pDispInfo->hdr.code)			
		{
			SetGridItem(pDispInfo);
			return TRUE;
		}
		// ��ʼ��Ԫѡ����Ϣ
		else if (GVN_SELCHANGING == pDispInfo->hdr.code)
		{			
			// ���ݵ�Ԫ
// 			if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
// 			{	
// 			}
			m_iSelectRow = pDispInfo->item.row;
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

void CSetupView::ModifyGridItem(GV_DISPINFO *pDispInfo)
{
	// ��������ģʽ, ���Ҫʹ�����Լ����ظ����OnEndEditCell����
	CString str = m_pGrid->GetItemText(pDispInfo->item.row, pDispInfo->item.col);	

	m_pGrid->AutoSizeColumns(GVS_BOTH);

	switch (m_iViewIndex)
	{
	case 0:			// שҤ���ý���		
		if (pDispInfo->item.col == 1)
		{
			m_KilnInfoMap[pDispInfo->item.row - 1].KilnType = atoi(str.Left(1));
		}
		else if (pDispInfo->item.col == 2)
		{
			m_KilnInfoMap[pDispInfo->item.row - 1].iTotalCars = atoi(str);
		}
		else if (pDispInfo->item.col == 3)
		{
			m_KilnInfoMap[pDispInfo->item.row - 1].iTempNum = atoi(str);
		}
		else if (pDispInfo->item.col == 4)
		{
			m_KilnInfoMap[pDispInfo->item.row - 1].iTempANum = atoi(str);
		}
		else if (pDispInfo->item.col == 5)
		{
			m_KilnInfoMap[pDispInfo->item.row - 1].iPressNum = atoi(str);
		}
		else if (pDispInfo->item.col == 6)
		{
			m_KilnInfoMap[pDispInfo->item.row - 1].iHumidityNum = atoi(str);
		}
		else if (pDispInfo->item.col == 7)
		{
			m_KilnInfoMap[pDispInfo->item.row - 1].iMotorNum = atoi(str);
		}
		else if (pDispInfo->item.col == 8)
		{
			m_KilnInfoMap[pDispInfo->item.row - 1].iGrossNum = atoi(str.Left(1));
		}
		else if (pDispInfo->item.col == 9)
		{
			m_KilnInfoMap[pDispInfo->item.row - 1].iCtrlNum = atoi(str.Left(1));
		}
		else if (pDispInfo->item.col == 10)
		{
			m_KilnInfoMap[pDispInfo->item.row - 1].iStandardIndex = atoi(str);

			// ��׼���ߺŴ�1��ʼ
			if (m_KilnInfoMap[pDispInfo->item.row - 1].iStandardIndex <= 0)
			{
				m_KilnInfoMap[pDispInfo->item.row - 1].iStandardIndex = 1;
			}
		}
		break;

	case 1:				// �ն����ý���
		if (pDispInfo->item.col == 1)
		{
			m_TerminalInfoMap[pDispInfo->item.row - 1].iTemId = atoi(str);
		}
		else if (pDispInfo->item.col == 2)
		{
			m_TerminalInfoMap[pDispInfo->item.row - 1].iTemCmd = atoi(str);
		}
		else if (pDispInfo->item.col == 3)
		{
			m_TerminalInfoMap[pDispInfo->item.row - 1].iTemChn = atoi(str);
		}
		else if (pDispInfo->item.col == 4)
		{
			m_TerminalInfoMap[pDispInfo->item.row - 1].iTemAddr = atoi(str);
		}
		else if (pDispInfo->item.col == 5)
		{
			m_TerminalInfoMap[pDispInfo->item.row - 1].iTemType = atoi(str.Left(1));
		}
		break;
	
	case 2:			// ͨ�����ý���
		if (pDispInfo->item.col == 1)
		{
			/* ��ֵ�����޸� */
			str.Format("%d", m_ChannelInfoMap[pDispInfo->item.row - 1].iKilnIdx);
			m_pGrid->SetItemText(pDispInfo->item.row, pDispInfo->item.col, str);
		}
		else if (pDispInfo->item.col == 2)
		{
			m_ChannelInfoMap[pDispInfo->item.row - 1].iTemIdx = atoi(str.Left(2));
			SetChannelInfoComboStyle(pDispInfo->item.row, TRUE);						// ��������ͨ����Comboѡ��
		}
		else if (pDispInfo->item.col == 3)
		{
			m_ChannelInfoMap[pDispInfo->item.row - 1].iPhyIdx = atoi(str.Left(2));
		}
		else if (pDispInfo->item.col == 4)
		{
			/* ��ֵ�����޸� */
			str.Format("%d", m_ChannelInfoMap[pDispInfo->item.row - 1].iChnTyp);
			m_pGrid->SetItemText(pDispInfo->item.row, pDispInfo->item.col, str);
		}
		else if (pDispInfo->item.col == 5)
		{
			m_ChannelInfoMap[pDispInfo->item.row - 1].iCarIdx = atoi(str);
		}
		else if (pDispInfo->item.col == 6)
		{
			m_ChannelInfoMap[pDispInfo->item.row - 1].strName[0] = '\0';
			strcpy(m_ChannelInfoMap[pDispInfo->item.row - 1].strName, str.GetBuffer(str.GetLength()));
		}
		else if (pDispInfo->item.col == 7)
		{
			m_ChannelInfoMap[pDispInfo->item.row - 1].AdjGain = atof(str);
		}
		else if (pDispInfo->item.col == 8)
		{
			m_ChannelInfoMap[pDispInfo->item.row - 1].AdjZero = atof(str);
		}
		break;

	default:
		break;
	}
	
	m_pGrid->Refresh();				// ˢ�±��
}

void CSetupView::SetGridItem(GV_DISPINFO *pDispInfo)
{
	// ���ݵ�Ԫ
	if (m_pGrid->IsCellFixed(pDispInfo->item.row, pDispInfo->item.col) == FALSE)
	{
		pDispInfo->item.lfFont = m_lfCellFont;				// ��������
		pDispInfo->item.crFgClr = THE_IN_GRID_FT_COLOR;		// ����������ɫ

		switch (m_iViewIndex)
		{
		case 0:			// שҤ���ý���	
			// �����������û�����ݻ�����ʾ�д������ݸ�������������ʾ
			if (m_KilnInfoMap.size() > 0 && pDispInfo->item.row <= m_KilnInfoMap.size())
			{
				if (pDispInfo->item.col == 1)
				{
					pDispInfo->item.strText.Format("%d", m_KilnInfoMap[pDispInfo->item.row - 1].KilnType);	
				}
				else if (pDispInfo->item.col == 2)
				{
					pDispInfo->item.strText.Format("%d", m_KilnInfoMap[pDispInfo->item.row - 1].iTotalCars);	
				}
				else if (pDispInfo->item.col == 3)
				{
					pDispInfo->item.strText.Format("%d", m_KilnInfoMap[pDispInfo->item.row - 1].iTempNum);	
				}
				else if (pDispInfo->item.col == 4)
				{
					pDispInfo->item.strText.Format("%d", m_KilnInfoMap[pDispInfo->item.row - 1].iTempANum);
				}
				else if (pDispInfo->item.col == 5)
				{
					pDispInfo->item.strText.Format("%d", m_KilnInfoMap[pDispInfo->item.row - 1].iPressNum);
				}
				else if (pDispInfo->item.col == 6)
				{
					pDispInfo->item.strText.Format("%d", m_KilnInfoMap[pDispInfo->item.row - 1].iHumidityNum);
				}
				else if (pDispInfo->item.col == 7)
				{
					pDispInfo->item.strText.Format("%d", m_KilnInfoMap[pDispInfo->item.row - 1].iMotorNum);
				}
				else if (pDispInfo->item.col == 8)
				{
					pDispInfo->item.strText.Format("%d", m_KilnInfoMap[pDispInfo->item.row - 1].iGrossNum);
				}
				else if (pDispInfo->item.col == 9)
				{
					pDispInfo->item.strText.Format("%d", m_KilnInfoMap[pDispInfo->item.row - 1].iCtrlNum);
				}
				else if (pDispInfo->item.col == 10)
				{
					pDispInfo->item.strText.Format("%d", m_KilnInfoMap[pDispInfo->item.row - 1].iStandardIndex);
				}
			}
			break;

		case 1:				// �ն����ý���
			// �����������û�����ݻ�����ʾ�д������ݸ�������������ʾ
			if (m_TerminalInfoMap.size() > 0 && pDispInfo->item.row <= m_TerminalInfoMap.size())
			{
				if (pDispInfo->item.col == 1)
				{
					pDispInfo->item.strText.Format("%d", m_TerminalInfoMap[pDispInfo->item.row - 1].iTemId);	
				}
				else if (pDispInfo->item.col == 2)
				{
					pDispInfo->item.strText.Format("%d", m_TerminalInfoMap[pDispInfo->item.row - 1].iTemCmd);
				}
				else if (pDispInfo->item.col == 3)
				{
					pDispInfo->item.strText.Format("%d", m_TerminalInfoMap[pDispInfo->item.row - 1].iTemChn);
				}
				else if (pDispInfo->item.col == 4)
				{
					pDispInfo->item.strText.Format("%d", m_TerminalInfoMap[pDispInfo->item.row - 1].iTemAddr);
				}
				else if (pDispInfo->item.col == 5)
				{
					pDispInfo->item.strText.Format("%d", m_TerminalInfoMap[pDispInfo->item.row - 1].iTemType);
				}
			}
			break;

		case 2:				// ͨ�����ý���
			if (pDispInfo->item.col == 1)
			{
				pDispInfo->item.strText.Format("%d", m_ChannelInfoMap[pDispInfo->item.row - 1].iKilnIdx);	
			}
			else if (pDispInfo->item.col == 2)
			{
				pDispInfo->item.strText.Format("%d", m_ChannelInfoMap[pDispInfo->item.row - 1].iTemIdx);
			}
			else if (pDispInfo->item.col == 3)
			{
				pDispInfo->item.strText.Format("%d", m_ChannelInfoMap[pDispInfo->item.row - 1].iPhyIdx);
			}
			else if (pDispInfo->item.col == 4)
			{
				pDispInfo->item.strText.Format("%d", m_ChannelInfoMap[pDispInfo->item.row - 1].iChnTyp);
			}
			else if (pDispInfo->item.col == 5)
			{
				pDispInfo->item.strText.Format("%d", m_ChannelInfoMap[pDispInfo->item.row - 1].iCarIdx);
			}
			else if (pDispInfo->item.col == 6)
			{
				pDispInfo->item.strText.Format("%s", m_ChannelInfoMap[pDispInfo->item.row - 1].strName);
			}
			else if (pDispInfo->item.col == 7)
			{
				pDispInfo->item.strText.Format("%3.2f", m_ChannelInfoMap[pDispInfo->item.row - 1].AdjGain);
			}
			else if (pDispInfo->item.col == 8)
			{
				pDispInfo->item.strText.Format("%3.2f", m_ChannelInfoMap[pDispInfo->item.row - 1].AdjZero);
			}
			break;

		default:
			break;
		}
	}
	// ��ͷ��Ԫ
	else
	{
		pDispInfo->item.lfFont = m_lfFixCellFont;

		switch (m_iViewIndex)
		{
		case 0:			// שҤ���ý���	
			// �б�ͷ��Ԫ����
			if (pDispInfo->item.row == 0 && pDispInfo->item.col >= 0)
			{	
				if (pDispInfo->item.col == 0)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("Ҥ���");
				}
				else if (pDispInfo->item.col == 1)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("Ҥ����");
				}
				else if (pDispInfo->item.col == 2)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("�ܳ�λ(����)");
				}
				else if (pDispInfo->item.col == 3)
				{			
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("�¶�����");
				}
				else if (pDispInfo->item.col == 4)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("��������");
				}
				else if (pDispInfo->item.col == 5)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("ѹ������");
				}
				else if (pDispInfo->item.col == 6)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("ʪ������");
				}
				else if (pDispInfo->item.col == 7)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("�������");
				}
				else if (pDispInfo->item.col == 8)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("�Ƿ��������");
				}
				else if (pDispInfo->item.col == 9)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("�Ƿ��Զ�����");
				}
				else if (pDispInfo->item.col == 10)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("��׼�¶Ⱥ�");
				}
			}
			// �б�ͷ��Ԫ����
			else if (pDispInfo->item.col == 0 && pDispInfo->item.row > 0)
			{
				pDispInfo->item.crFgClr = THE_FIXED_ROW_FT_COLOR;
				pDispInfo->item.strText.Format("%d", pDispInfo->item.row);	
			}
			break;

		case 1:				// �ն����ý���
			// �б�ͷ��Ԫ����
			if (pDispInfo->item.row == 0 && pDispInfo->item.col >= 0)
			{	
				if (pDispInfo->item.col == 0)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("ģ�����");
				}
				else if (pDispInfo->item.col == 1)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("ģ���ַ");
				}
				else if (pDispInfo->item.col == 2)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("ģ������");
				}
				else if (pDispInfo->item.col == 3)
				{			
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("ģ��ͨ����");
				}
				else if (pDispInfo->item.col == 4)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("ģ��Ĵ�����ַ");
				}
				else if (pDispInfo->item.col == 5)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("ģ������");
				}
			}
			// �б�ͷ��Ԫ����
			else if (pDispInfo->item.col == 0 && pDispInfo->item.row > 0)
			{
				pDispInfo->item.crFgClr = THE_FIXED_ROW_FT_COLOR;
				pDispInfo->item.strText.Format("%d", pDispInfo->item.row);	
			}
			break;

		case 2:		// ͨ�����ý���
			// �б�ͷ��Ԫ����
			if (pDispInfo->item.row == 0 && pDispInfo->item.col > 0)
			{	
				if (pDispInfo->item.col == 0)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("ͨ�����");
				}
				else if (pDispInfo->item.col == 1)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("����Ҥ��");
				}
				else if (pDispInfo->item.col == 2)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("�����ն˺�");
				}
				else if (pDispInfo->item.col == 3)
				{			
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("����ͨ����");
				}
				else if (pDispInfo->item.col == 4)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("ͨ������");
				}
				else if (pDispInfo->item.col == 5)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("��λ��(����)");
				}
				else if (pDispInfo->item.col == 6)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("ͨ������");
				}
				else if (pDispInfo->item.col == 7)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("����ϵ��");
				}
				else if (pDispInfo->item.col == 8)
				{
					pDispInfo->item.crFgClr = THE_FIXED_COL_FT_COLOR;
					pDispInfo->item.strText.Format("���ϵ��");
				}
			}
			// �б�ͷ��Ԫ����
			else if (pDispInfo->item.col == 0 && pDispInfo->item.row > 0)
			{
				pDispInfo->item.crFgClr = THE_FIXED_ROW_FT_COLOR;
				pDispInfo->item.strText.Format("%d", pDispInfo->item.row);	
			}
			break;

		default:
			break;
		}	
	}
}

void CSetupView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	GetClientRect(m_MaxClientRect);

	CRect rect = m_MaxClientRect;
	rect.bottom = rect.bottom - THE_BOTTOM_GAP_HEIGHT;
	m_pGrid->MoveWindow(rect, TRUE);

	// ����ˢ�Ӳ�����������, �ߴ�Ϊ�����ڵĿͻ�����С
	m_MaxClientRect.top = m_MaxClientRect.bottom - THE_BOTTOM_GAP_HEIGHT;
	CBrush brBack, *pOldcrBack;
	brBack.CreateSolidBrush(THE_BACKGROUND_COLOR);
	m_MemDcDraw.FillRect(&m_MaxClientRect, &brBack);	
	pOldcrBack = m_MemDcDraw.SelectObject(&brBack);	
	m_MemDcDraw.SelectObject(pOldcrBack);
	brBack.DeleteObject();	

	// �����Դ�
	CDC *pDC = GetDC();
	pDC->StretchBlt(m_MaxClientRect.left, m_MaxClientRect.top, m_MaxClientRect.Width(), m_MaxClientRect.Height(), &m_MemDcDraw, m_MaxClientRect.left, m_MaxClientRect.top, m_MaxClientRect.Width(), m_MaxClientRect.Height(), SRCAND);
	ReleaseDC(pDC);

	CRect brect;
	brect.left = 250;
	brect.right = brect.left + 100;
	brect.top = rect.bottom + 20;
	brect.bottom = brect.top + 40;
	m_pSetButton->MoveWindow(brect, TRUE);
	
	brect.left = 550;
	brect.right = brect.left + 100;
	brect.top = rect.bottom + 20;
	brect.bottom = brect.top + 40;
	m_pDelButton->MoveWindow(brect, TRUE);
	
	brect.left = 850;
	brect.right = brect.left + 100;
	brect.top = rect.bottom + 20;
	brect.bottom = brect.top + 40;
	m_pNextButton->MoveWindow(brect, TRUE);	

	m_pSetButton->Invalidate();
	m_pDelButton->Invalidate();
	m_pNextButton->Invalidate();

	// Do not call CView::OnPaint() for painting messages
}

void CSetupView::OnDestroy() 
{
	CView::OnDestroy();

	if (m_bFileOpen == TRUE)
	{
		m_bFileOpen = FALSE;
		m_hFile.Close();
	}

	delete m_pGrid;
	delete m_pSetButton;
	delete m_pDelButton;
	delete m_pNextButton;

	DeleteObject(m_HbtMap);
	DeleteDC(m_MemDcDraw);
}

void CSetupView::OpenFile(const char *pFileName)
{
	if (m_hFile.Open(pFileName, CFile::modeReadWrite | CFile::modeCreate, NULL) == FALSE)
	{
		MessageBox("�����ļ���������", "���ô���", MB_OK);
		m_bFileOpen = FALSE;
		return;
	}
	else
	{
		m_bFileOpen = TRUE;
	}
}

void CSetupView::CloseFile(void)
{
	if (m_bFileOpen == TRUE)
	{
		m_bFileOpen = FALSE;
		m_hFile.Close();
	}
}

void CSetupView::WriteCfgHeader(void)
{
	if (m_bFileOpen == FALSE)
	{
		return;
	}

	m_hFile.WriteString("PRAGMA foreign_keys=OFF;\r\n");
	m_hFile.WriteString("BEGIN TRANSACTION;\r\n");
}

void CSetupView::WriteCfgLast(void)
{
	if (m_bFileOpen == FALSE)
	{
		return;
	}
	
	m_hFile.WriteString("COMMIT;\r\n");
}

void CSetupView::CreateLoginTable(void)
{
	if (m_bFileOpen == FALSE)
	{
		return;
	}

	m_hFile.WriteString("CREATE TABLE LoginInfo(id integer primary key, name text, pwd text, pwr integer, phone text, mail text);\r\n");
	m_hFile.WriteString("INSERT INTO \"LoginInfo\" VALUES(1,'admin','spunky',1,'13983089826','imf_no.1@hotmail.com');\r\n");		// д�����Ա����
}

void CSetupView::CreateComCfgTable(void)
{
	if (m_bFileOpen == FALSE)
	{
		return;
	}

	m_hFile.WriteString("CREATE TABLE ComCfgInfo(port_num integer, baudrate integer, parity text, data_bits integer, stop_bits integer, timeout integer, scan_time integer, m_serve_ip integer, m_serve_port integer, s_serve_ip integer, s_serve_port integer, gross_time integer, bell_time integer);\r\n");
	m_hFile.WriteString("INSERT INTO \"ComCfgInfo\" VALUES(2,19200,'N',8,1,100,100,-1062731775,65003,-1062731774,65004, 15, 15000);\r\n");
}

void CSetupView::CreateAutoCtrlCfgTable(void)
{
	if (m_bFileOpen == FALSE)
	{
		return;
	}
	
	m_hFile.WriteString("CREATE TABLE AutoCtrlInfo(id integer primary key, out_up double, out_dn double, front_pts integer, rear_pts integer, ctrl_time integer, inc_coeff double, gross_waittime integer, gross_proctime integer);\r\n");

	CString str;
	for (int i = 0; i < m_KilnInfoMap.size(); i++)
	{
		str.Format("INSERT INTO \"AutoCtrlInfo\" VALUES(%d,50.0,30.0,3,2,5,0.1,10,30);\r\n", i + 1);
		m_hFile.WriteString(str);
	}
}

void CSetupView::CreateTerminalCfgTable(void)
{
	if (m_bFileOpen == FALSE)
	{
		return;
	}
	
	m_hFile.WriteString("CREATE TABLE TerminalCfgInfo(id integer primary key, dev_id integer, dev_cmd integer, dev_chn integer, dev_addr integer, dev_type integer);\r\n");

	CString str;
	for (int i = 0; i < m_TerminalInfoMap.size(); i++)
	{
		str.Empty();
		str.Format("INSERT INTO \"TerminalCfgInfo\" VALUES(%d,", i + 1);
		m_hFile.WriteString(str);
		str.Format("%d,", m_TerminalInfoMap[i].iTemId);
		m_hFile.WriteString(str);
		str.Format("%d,", m_TerminalInfoMap[i].iTemCmd);
		m_hFile.WriteString(str);
		str.Format("%d,", m_TerminalInfoMap[i].iTemChn);
		m_hFile.WriteString(str);
		str.Format("%d,", m_TerminalInfoMap[i].iTemAddr);
		m_hFile.WriteString(str);
		str.Format("%d);\r\n", m_TerminalInfoMap[i].iTemType);
		m_hFile.WriteString(str);
	}
}

void CSetupView::CreateKilnInfoCfgTable(void)
{
	if (m_bFileOpen == FALSE)
	{
		return;
	}
	
	m_hFile.WriteString("CREATE TABLE KilnInfo(kiln_id integer, kiln_type integer, kiln_total integer, kiln_temps integer, kiln_tempas integer, kiln_press integer, kiln_humis integer, kiln_motor integer, kiln_gross integer, kiln_ctrl integer, kiln_std integer);\r\n");

	CString str;
	for (int i = 0; i < m_KilnInfoMap.size(); i++)
	{
		str.Format("INSERT INTO \"KilnInfo\" VALUES(%d,", i + 1);
		m_hFile.WriteString(str);
		str.Format("%d,", m_KilnInfoMap[i].KilnType);
		m_hFile.WriteString(str);
		str.Format("%d,", m_KilnInfoMap[i].iTotalCars);
		m_hFile.WriteString(str);
		str.Format("%d,", m_KilnInfoMap[i].iTempNum);
		m_hFile.WriteString(str);
		str.Format("%d,", m_KilnInfoMap[i].iTempANum);
		m_hFile.WriteString(str);
		str.Format("%d,", m_KilnInfoMap[i].iPressNum);
		m_hFile.WriteString(str);
		str.Format("%d,", m_KilnInfoMap[i].iHumidityNum);
		m_hFile.WriteString(str);
		str.Format("%d,", m_KilnInfoMap[i].iMotorNum);
		m_hFile.WriteString(str);
		str.Format("%d,", m_KilnInfoMap[i].iGrossNum);
		m_hFile.WriteString(str);
		str.Format("%d,", m_KilnInfoMap[i].iCtrlNum);
		m_hFile.WriteString(str);
		str.Format("%d);\r\n", m_KilnInfoMap[i].iStandardIndex);
		m_hFile.WriteString(str);
	}
}

void CSetupView::CreateStandardTable(void)
{
	if (m_bFileOpen == FALSE)
	{
		return;
	}
	
	CString str;
	int i, j;
	for (i = 0; i < m_KilnInfoMap.size(); i++)
	{
		str.Format("CREATE TABLE StandardCfgInfo_%d(id integer,set_time text,", i + 1);
		m_hFile.WriteString(str);
		
		for (j = 0; j < (m_KilnInfoMap[i].iTempNum - 1); j++)
		{
			str.Format("temperature_%d integer,", j + 1);
			m_hFile.WriteString(str);
		}
		
		str.Format("temperature_%d integer);\r\n", j + 1);
		m_hFile.WriteString(str);
		
		str.Format("INSERT INTO \"StandardCfgInfo_%d\" VALUES(1,'2013-01-04 20:19:00',", i + 1);
		m_hFile.WriteString(str);
		
		for (j = 0; j < (m_KilnInfoMap[i].iTempNum - 1); j++)
		{
			str.Format("0,");
			m_hFile.WriteString(str);
		}
		
		str.Format("0);\r\n");
		m_hFile.WriteString(str);
	}
}

void CSetupView::CreateHistoryTable(void)
{
	if (m_bFileOpen == FALSE)
	{
		return;
	}
	
	CString str;
	int i, j;
	for (i = 0; i < m_KilnInfoMap.size(); i++)
	{
		str.Format("CREATE TABLE History_%d(id integer primary key,save_time text,", i + 1);
		m_hFile.WriteString(str);
		
		if (m_KilnInfoMap[i].iTempNum > 0)
		{
			for (j = 0; j < (m_KilnInfoMap[i].iTempNum - 1); j++)
			{
				str.Format("temperature_%d integer,", j + 1);
				m_hFile.WriteString(str);
			}

			if ((m_KilnInfoMap[i].iTempANum == 0) 
				&& (m_KilnInfoMap[i].iPressNum == 0) 
				&& (m_KilnInfoMap[i].iHumidityNum == 0) 
				&& (m_KilnInfoMap[i].iMotorNum == 0))
			{
				str.Format("temperature_%d integer);\r\n", j + 1);
				m_hFile.WriteString(str);
				return;
			}
			else
			{
				str.Format("temperature_%d integer,", j + 1);
				m_hFile.WriteString(str);
			}
		}


		if (m_KilnInfoMap[i].iTempANum > 0)
		{
			for (j = 0; j < (m_KilnInfoMap[i].iTempANum - 1); j++)
			{
				str.Format("temperaturea_%d integer,", j + 1);
				m_hFile.WriteString(str);
			}
			
			if ((m_KilnInfoMap[i].iPressNum == 0) 
				&& (m_KilnInfoMap[i].iHumidityNum == 0) 
				&& (m_KilnInfoMap[i].iMotorNum == 0))
			{
				str.Format("temperaturea_%d integer);\r\n", j + 1);
				m_hFile.WriteString(str);
				return;
			}
			else
			{
				str.Format("temperaturea_%d integer,", j + 1);
				m_hFile.WriteString(str);
			}
		}

		if (m_KilnInfoMap[i].iPressNum > 0)
		{
			for (j = 0; j < (m_KilnInfoMap[i].iPressNum - 1); j++)
			{
				str.Format("press_%d integer,", j + 1);
				m_hFile.WriteString(str);
			}
			
			if ((m_KilnInfoMap[i].iHumidityNum == 0) 
				&& (m_KilnInfoMap[i].iMotorNum == 0))
			{
				str.Format("press_%d integer);\r\n", j + 1);
				m_hFile.WriteString(str);
				return;
			}
			else
			{
				str.Format("press_%d integer,", j + 1);
				m_hFile.WriteString(str);
			}
		}

		if (m_KilnInfoMap[i].iHumidityNum > 0)
		{
			for (j = 0; j < (m_KilnInfoMap[i].iHumidityNum - 1); j++)
			{
				str.Format("humidity_%d double,", j + 1);
				m_hFile.WriteString(str);
			}
			
			if (m_KilnInfoMap[i].iMotorNum == 0)
			{
				str.Format("humidity_%d double);\r\n", j + 1);
				m_hFile.WriteString(str);
				return;
			}
			else
			{
				str.Format("humidity_%d double,", j + 1);
				m_hFile.WriteString(str);
			}
		}

		for (j = 0; j < (m_KilnInfoMap[i].iMotorNum - 1); j++)
		{
			str.Format("motor_%d double,", j + 1);
			m_hFile.WriteString(str);
		}
		
		str.Format("motor_%d double);\r\n", j + 1);
		m_hFile.WriteString(str);
	}
}

void CSetupView::CreateChannelCfgTable(void)
{
	if (m_bFileOpen == FALSE)
	{
		return;
	}
	
	CString str;
	str.Format("CREATE TABLE ChannelCfgInfo(id integer primary key,tem_idx integer,phy_idx integer,kiln_idx integer,chn_type integer,car_loc integer,chn_name text,adj_gain double,adj_zero double);\r\n");
	m_hFile.WriteString(str);

	int i, j;
	int total_chn;
	for (total_chn = 0, i = 0; i < m_KilnInfoMap.size(); i++)
	{
		/************************************************************************/
		/* �¶�����˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iTempNum; j++)
		{
			str.Format("INSERT INTO \"ChannelCfgInfo\" VALUES(%d,%d,%d,%d,%d,%d,\'%s\',%3.2f,%3.2f);\r\n", total_chn + 1, 
																					   m_ChannelInfoMap[total_chn].iTemIdx,
																					   m_ChannelInfoMap[total_chn].iPhyIdx,
																					   m_ChannelInfoMap[total_chn].iKilnIdx,
																					   m_ChannelInfoMap[total_chn].iChnTyp,
																					   m_ChannelInfoMap[total_chn].iCarIdx,
																					   m_ChannelInfoMap[total_chn].strName,
																					   m_ChannelInfoMap[total_chn].AdjGain,
																					   m_ChannelInfoMap[total_chn].AdjZero);
			m_hFile.WriteString(str);
			total_chn++;
		}
		
		/************************************************************************/
		/* ��������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iTempANum; j++)
		{
			str.Format("INSERT INTO \"ChannelCfgInfo\" VALUES(%d,%d,%d,%d,%d,%d,\'%s\',%3.2f,%3.2f);\r\n", total_chn + 1, 
																					   m_ChannelInfoMap[total_chn].iTemIdx,
																					   m_ChannelInfoMap[total_chn].iPhyIdx,
																					   m_ChannelInfoMap[total_chn].iKilnIdx,
																					   m_ChannelInfoMap[total_chn].iChnTyp,
																					   m_ChannelInfoMap[total_chn].iCarIdx,
																					   m_ChannelInfoMap[total_chn].strName,
																					   m_ChannelInfoMap[total_chn].AdjGain,
																					   m_ChannelInfoMap[total_chn].AdjZero);
			m_hFile.WriteString(str);
			total_chn++;
		}

		/************************************************************************/
		/* ѹ������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iPressNum; j++)
		{
			str.Format("INSERT INTO \"ChannelCfgInfo\" VALUES(%d,%d,%d,%d,%d,%d,\'%s\',%3.2f,%3.2f);\r\n", total_chn + 1, 
																						m_ChannelInfoMap[total_chn].iTemIdx,
																						m_ChannelInfoMap[total_chn].iPhyIdx,
																						m_ChannelInfoMap[total_chn].iKilnIdx,
																						m_ChannelInfoMap[total_chn].iChnTyp,
																						m_ChannelInfoMap[total_chn].iCarIdx,
																						m_ChannelInfoMap[total_chn].strName,
																						m_ChannelInfoMap[total_chn].AdjGain,
																						m_ChannelInfoMap[total_chn].AdjZero);
			m_hFile.WriteString(str);
			total_chn++;
		}

		/************************************************************************/
		/* ʪ������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iHumidityNum; j++)
		{
			str.Format("INSERT INTO \"ChannelCfgInfo\" VALUES(%d,%d,%d,%d,%d,%d,\'%s\',%3.2f,%3.2f);\r\n", total_chn + 1, 
																						m_ChannelInfoMap[total_chn].iTemIdx,
																						m_ChannelInfoMap[total_chn].iPhyIdx,
																						m_ChannelInfoMap[total_chn].iKilnIdx,
																						m_ChannelInfoMap[total_chn].iChnTyp,
																						m_ChannelInfoMap[total_chn].iCarIdx,
																						m_ChannelInfoMap[total_chn].strName,
																						m_ChannelInfoMap[total_chn].AdjGain,
																						m_ChannelInfoMap[total_chn].AdjZero);
			m_hFile.WriteString(str);
			total_chn++;
		}

		/************************************************************************/
		/* �������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iMotorNum; j++)
		{
			str.Format("INSERT INTO \"ChannelCfgInfo\" VALUES(%d,%d,%d,%d,%d,%d,\'%s\',%3.2f,%3.2f);\r\n", total_chn + 1, 
																						m_ChannelInfoMap[total_chn].iTemIdx,
																						m_ChannelInfoMap[total_chn].iPhyIdx,
																						m_ChannelInfoMap[total_chn].iKilnIdx,
																						m_ChannelInfoMap[total_chn].iChnTyp,
																						m_ChannelInfoMap[total_chn].iCarIdx,
																						m_ChannelInfoMap[total_chn].strName,
																						m_ChannelInfoMap[total_chn].AdjGain,
																						m_ChannelInfoMap[total_chn].AdjZero);
			m_hFile.WriteString(str);
			total_chn++;
		}

		/************************************************************************/
		/* ��������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iGrossNum; j++)
		{
			str.Format("INSERT INTO \"ChannelCfgInfo\" VALUES(%d,%d,%d,%d,%d,%d,\'%s\',%3.2f,%3.2f);\r\n", total_chn + 1, 
																						m_ChannelInfoMap[total_chn].iTemIdx,
																						m_ChannelInfoMap[total_chn].iPhyIdx,
																						m_ChannelInfoMap[total_chn].iKilnIdx,
																						m_ChannelInfoMap[total_chn].iChnTyp,
																						m_ChannelInfoMap[total_chn].iCarIdx,
																						m_ChannelInfoMap[total_chn].strName,
																						m_ChannelInfoMap[total_chn].AdjGain,
																						m_ChannelInfoMap[total_chn].AdjZero);
			m_hFile.WriteString(str);
			total_chn++;
		}

		/************************************************************************/
		/* ��������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iGrossNum; j++)
		{
			str.Format("INSERT INTO \"ChannelCfgInfo\" VALUES(%d,%d,%d,%d,%d,%d,\'%s\',%3.2f,%3.2f);\r\n", total_chn + 1, 
																						m_ChannelInfoMap[total_chn].iTemIdx,
																						m_ChannelInfoMap[total_chn].iPhyIdx,
																						m_ChannelInfoMap[total_chn].iKilnIdx,
																						m_ChannelInfoMap[total_chn].iChnTyp,
																						m_ChannelInfoMap[total_chn].iCarIdx,
																						m_ChannelInfoMap[total_chn].strName,
																						m_ChannelInfoMap[total_chn].AdjGain,
																						m_ChannelInfoMap[total_chn].AdjZero);
			m_hFile.WriteString(str);
			total_chn++;
		}

		/************************************************************************/
		/* �������˿����ü�¼                                                 */
		/************************************************************************/
		for (j = 0; j < m_KilnInfoMap[i].iMotorNum; j++)
		{
			str.Format("INSERT INTO \"ChannelCfgInfo\" VALUES(%d,%d,%d,%d,%d,%d,\'%s\',%3.2f,%3.2f);\r\n", total_chn + 1, 
																						m_ChannelInfoMap[total_chn].iTemIdx,
																						m_ChannelInfoMap[total_chn].iPhyIdx,
																						m_ChannelInfoMap[total_chn].iKilnIdx,
																						m_ChannelInfoMap[total_chn].iChnTyp,
																						m_ChannelInfoMap[total_chn].iCarIdx,
																						m_ChannelInfoMap[total_chn].strName,
																						m_ChannelInfoMap[total_chn].AdjGain,
																						m_ChannelInfoMap[total_chn].AdjZero);
			m_hFile.WriteString(str);
			total_chn++;
		}
	}
}

void CSetupView::CreateOutputsTable(void)
{
	if (m_bFileOpen == FALSE)
	{
		return;
	}

	m_hFile.WriteString("CREATE TABLE OutputsInfo(kiln_idx integer, out_time text, condition integer, up_serve integer);\r\n");
}

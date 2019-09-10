// AddGrossDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "MainFrm.h"
#include "iControlDoc.h"
#include "AddGrossDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddGrossDialog dialog


CAddGrossDialog::CAddGrossDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAddGrossDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddGrossDialog)
	m_HeatEdit = _T("");
	m_HeatCaption = _T("");
	m_RoastCaption = _T("");
	//}}AFX_DATA_INIT
}


void CAddGrossDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddGrossDialog)
	DDX_Control(pDX, IDC_ROAST_COMBO, m_RoastCombo);
	DDX_Control(pDX, IDC_CAPTION_STATIC, m_DlgCaption);
	DDX_Text(pDX, IDC_HEAT_EDIT, m_HeatEdit);
	DDV_MaxChars(pDX, m_HeatEdit, 4);
	DDX_Text(pDX, IDC_HEAT_STATIC, m_HeatCaption);
	DDX_Text(pDX, IDC_ROAST_STATIC, m_RoastCaption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddGrossDialog, CDialog)
	//{{AFX_MSG_MAP(CAddGrossDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddGrossDialog message handlers

BOOL CAddGrossDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pCaptionFont = new CFont;
	m_pCaptionFont->CreateFont(25, 0, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "楷体_GB2312");
	m_DlgCaption.SetFont(m_pCaptionFont, TRUE);
	m_DlgCaption.SetWindowText("推车操作界面");
	
	m_pStaticFont = new CFont;
	m_pStaticFont->CreateFont(18, 0, 0, 0, FW_MEDIUM, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "新宋体");
	m_RoastCaption.Format("焙烧窑号:");
	m_HeatCaption.Format("热值输入:");

	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	m_iRoastFurnaceNum = pDoc->m_iRoastFurnaceNum;
	m_iBakeFurnaceNum = pDoc->m_iBakeFurnaceNum;
	m_pPortPageBuff = pDoc->m_pPortPageBuff;
	m_pGrossTbl = pDoc->m_pGrossTbl;
	m_pCarHeatTbl = pDoc->m_pCarHeatTbl;
	
	// 热值输入对话框
	m_fHeatValue = 0;
	m_pEditFont = new CFont;
	m_pEditFont->CreateFont(18, 0, 0, 0, FW_MEDIUM, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Fixedsys");
	m_HeatEdit.Format("%3.1f", m_fHeatValue);

	// 添加焙烧窑的选择项目
	CString str;
	m_iRoastID = 0;
	for (int i = 0; i < m_iRoastFurnaceNum; i++)
	{
		str.Empty();
		str.Format("%d号焙烧窑", i + 1);
		m_RoastCombo.AddString(str);
	}
	m_RoastCombo.SetCurSel(0);
	
	UpdateData(FALSE);
	ModifyStyleEx(WS_EX_APPWINDOW, 0);  
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAddGrossDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int CtrlID = pWnd->GetDlgCtrlID();
	if (CtrlID == IDC_CAPTION_STATIC && nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 128, 255));		// 字体颜色
        pDC->SetBkColor(RGB(0, 0, 255));		    // 字体背景色
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	else if ((CtrlID == IDC_ROAST_STATIC || CtrlID == IDC_HEAT_STATIC) && (nCtlColor == CTLCOLOR_STATIC))
	{
		pDC->SelectObject(m_pStaticFont);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 24, 255));			// 字体颜色
        pDC->SetBkColor(RGB(0, 0, 255));		    // 字体背景色
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	else if (CtrlID == IDC_EDIT_COUNT && nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SelectObject(m_pEditFont);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(64, 64, 64));			// 字体颜色
        pDC->SetBkColor(RGB(225, 225, 225));		// 字体背景色
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	else if (CtrlID == IDC_HEAT_EDIT && nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SelectObject(m_pEditFont);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(64, 64, 64));			// 字体颜色
        pDC->SetBkColor(RGB(225, 225, 225));		// 字体背景色
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	else if (nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CAddGrossDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_pCaptionFont->DeleteObject();
	m_pStaticFont->DeleteObject();
	m_pEditFont->DeleteObject();
	delete m_pCaptionFont;
	delete m_pStaticFont;
	delete m_pEditFont;
}

void CAddGrossDialog::OnOK() 
{
	while (1)
	{
		UpdateData(TRUE);
		m_iRoastID = m_RoastCombo.GetCurSel();		// 取得所选的焙烧窑号
		if (m_iRoastID < 0)
		{
			MessageBox("请选择需要操作的窑号", "参数输入错误", MB_OK);
			return;
		}

		m_fHeatValue = atoi(m_HeatEdit);
		if (m_fHeatValue < 0.0F || m_fHeatValue > 9999.9F)
		{
			MessageBox("发热量输入范围: 0 ~ 9999, 请重新输入", "参数输入错误", MB_OK);
			return;
		}
		else
		{
			break;
		}
	}


	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	pDoc->AppendGrossRecord(m_iRoastID, m_fHeatValue);

	
	CDialog::OnOK();
}

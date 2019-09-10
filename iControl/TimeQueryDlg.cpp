// TimeQueryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "TimeQueryDlg.h"
#include "HisGridView.h"
#include "MainFrm.h"
#include "iControlDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimeQueryDlg dialog


CTimeQueryDlg::CTimeQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeQueryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimeQueryDlg)
	m_SetHour = 0;
	m_SetMin = 0;
	m_SetDate = 0;
	//}}AFX_DATA_INIT
}


void CTimeQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeQueryDlg)
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateCtrl);
	DDX_Text(pDX, IDC_EDIT_HOUR, m_SetHour);
	DDV_MinMaxUInt(pDX, m_SetHour, 0, 23);
	DDX_Text(pDX, IDC_EDIT_MIN, m_SetMin);
	DDV_MinMaxUInt(pDX, m_SetMin, 0, 59);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_SetDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimeQueryDlg, CDialog)
	//{{AFX_MSG_MAP(CTimeQueryDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeQueryDlg message handlers

BOOL CTimeQueryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_DateCtrl.SetFormat(_T("yyyy年MM月dd日"));
//	m_DateCtrl.SetTime(&CTime::GetCurrentTime());		// 获取系统日期

	m_SetDate = CTime::GetCurrentTime();	// 获取系统日期
	m_SetHour = m_SetDate.GetHour();
	m_SetMin  = m_SetDate.GetMinute();
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTimeQueryDlg::OnOK() 
{
	UpdateData(TRUE);

	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	CTime time;
	m_DateCtrl.GetTime(time);
	

	pDoc->m_strHistoryQuery.Format("%04d-%02d-%02d %02d:%02d:00", time.GetYear(), 
																  time.GetMonth(), 
																  time.GetDay(),
																  m_SetHour,
																  m_SetMin);
	
	CDialog::OnOK();
}

void CTimeQueryDlg::OnCancel() 
{
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	pDoc->m_strHistoryQuery = "";
	
	CDialog::OnCancel();
}

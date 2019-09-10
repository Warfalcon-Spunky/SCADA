// GrossInfo.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "GrossInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrossInfo dialog


CGrossInfo::CGrossInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CGrossInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGrossInfo)
	m_InfoString = _T("");
	//}}AFX_DATA_INIT
}


void CGrossInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGrossInfo)
	DDX_Control(pDX, IDC_INFO_STRING, m_InfoCtrl);
	DDX_Text(pDX, IDC_INFO_STRING, m_InfoString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGrossInfo, CDialog)
	//{{AFX_MSG_MAP(CGrossInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrossInfo message handlers

BOOL CGrossInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CFont *m_pStrFont = new CFont;
	m_pStrFont->CreateFont(20, 0, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "¿¬Ìå_GB2312");
	m_InfoCtrl.SetFont(m_pStrFont, TRUE);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

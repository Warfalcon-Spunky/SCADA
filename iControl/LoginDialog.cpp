// LoginDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "iControlDoc.h"
#include "MainFrm.h"
#include "LoginDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define THE_LOGIN_TABLE_COL			6

/////////////////////////////////////////////////////////////////////////////
// CLoginDialog dialog


CLoginDialog::CLoginDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDialog)
	m_EditCount = _T("");
	m_EditPwd = _T("");
	m_StaticCount = _T("");
	m_StaticPwd = _T("");
	//}}AFX_DATA_INIT
}


void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDialog)
	DDX_Control(pDX, IDC_STATIC_CAPTION, m_DlgCaption);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_EditCount);
	DDX_Text(pDX, IDC_EDIT_PWS, m_EditPwd);
	DDX_Text(pDX, IDC_STATIC_COUNT, m_StaticCount);
	DDX_Text(pDX, IDC_STATIC_PWD, m_StaticPwd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDialog, CDialog)
	//{{AFX_MSG_MAP(CLoginDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDialog message handlers

BOOL CLoginDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pCaptionFont = new CFont;
	m_pCaptionFont->CreateFont(25, 0, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "楷体_GB2312");
	m_DlgCaption.SetFont(m_pCaptionFont, TRUE);
	m_DlgCaption.SetWindowText("Intelligent Monitoring System");
	
	m_pStaticFont = new CFont;
	m_pStaticFont->CreateFont(18, 0, 0, 0, FW_MEDIUM, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "新宋体");
	m_StaticPwd.Format("Password:");
	m_StaticCount.Format("Account:");

	m_pEditFont = new CFont;
	m_pEditFont->CreateFont(18, 0, 0, 0, FW_MEDIUM, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Fixedsys");
	m_EditCount.Format("");
	m_EditPwd.Format("");

	UpdateData(FALSE);

	m_pLoginTbl = new CHisTbl(THE_LOGIN_TABLE_COL);				// 创建数据库句柄	
	m_pLoginTbl->SetTableName("LoginInfo");							// 指定数据库的历史表名称
	
	// 指定数据库的登陆数据表的字段名
	int col_index = 0;
	m_pLoginTbl->SetItem(col_index++, "id",    MYDB_TYPE_INT);
	m_pLoginTbl->SetItem(col_index++, "name",  MYDB_TYPE_TEXT);
	m_pLoginTbl->SetItem(col_index++, "pwd",   MYDB_TYPE_TEXT);
	m_pLoginTbl->SetItem(col_index++, "pwr",   MYDB_TYPE_INT);
	m_pLoginTbl->SetItem(col_index++, "phone", MYDB_TYPE_TEXT);
	m_pLoginTbl->SetItem(col_index++, "mail",  MYDB_TYPE_TEXT);

	m_bDBOpen = FALSE;

	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc = (CIControlDoc *)pMainFrame->GetActiveDocument();
	CString strDBPath = pDoc->MbcsToUtf8(pDoc->m_strDBPath.GetBuffer(MAX_PATH));

	if (!m_pLoginTbl->Open(strDBPath))
	{
//		MessageBox("数据库路径错误", "数据库错误", MB_OK);
		MessageBox("Path of database error", "Database error", MB_OK);
		return m_bDBOpen;
	}
	
	if (m_pLoginTbl->GetCount() < 1)
	{
		MessageBox("Format of database error", "Database error", MB_OK);
		return m_bDBOpen;
	}

	ModifyStyleEx(WS_EX_APPWINDOW, 0);    

	m_bDBOpen = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_pCaptionFont->DeleteObject();
	m_pStaticFont->DeleteObject();
	m_pEditFont->DeleteObject();
	delete m_pCaptionFont;
	delete m_pStaticFont;
	delete m_pEditFont;

	m_pLoginTbl->Close();
	delete m_pLoginTbl;
}

HBRUSH CLoginDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	int CtrlID = pWnd->GetDlgCtrlID();
	if (CtrlID == IDC_STATIC_CAPTION && nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 128, 255));		// 字体颜色
        pDC->SetBkColor(RGB(0, 0, 255));		    // 字体背景色
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	else if ((CtrlID == IDC_STATIC_COUNT || CtrlID == IDC_STATIC_PWD) && (nCtlColor == CTLCOLOR_STATIC))
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
	else if (CtrlID == IDC_EDIT_PWS && nCtlColor == CTLCOLOR_EDIT)
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

void CLoginDialog::OnOK() 
{
	// TODO: Add extra validation here
	if (m_bDBOpen == FALSE)
	{
		CDialog::EndDialog(-2);
		return;
	}

	UpdateData(TRUE);
	
	CString strFilter;
	strFilter.Format("[name]=\'%s\'", m_EditCount);

	m_pLoginTbl->SetFilter(strFilter);
	m_pLoginTbl->GetSQL(false, true, false);
	m_pLoginTbl->Query();

	if (m_pLoginTbl->GetCount() > 0)
	{			
		if (strcmp(*((SQLiteString *)(m_pLoginTbl->m_pColsName[2])), m_EditPwd) == 0) // 密码正确
		{			
//			CDialog::EndDialog(*((int *)(m_pLoginTbl->m_pColsName[3])));			  // 返回代码
			CDialog::EndDialog(0);													  // 返回代码
			return;
		}
	}
	
	CDialog::EndDialog(-1);			// 用-1代表密码错误
}

void CLoginDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::EndDialog(-2);			// 退出
}

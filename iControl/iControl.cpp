// iControl.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "iControl.h"
#include "RegisterDlg.h"
#include "MainFrm.h"
#include "iControlDoc.h"
#include "iControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIControlApp

BEGIN_MESSAGE_MAP(CIControlApp, CWinApp)
	//{{AFX_MSG_MAP(CIControlApp)
	ON_COMMAND(IDI_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIControlApp construction

CIControlApp::CIControlApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIControlApp object

CIControlApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CIControlApp initialization

BOOL CIControlApp::InitInstance()
{
	int res;
	CRegisterDlg dlg;

	dlg.m_bIsModifyResgister = FALSE;
	
	while (1)
	{
		res = dlg.DoModal();
		if (res == IDOK)
		{
			// 验证密码是否正确
			if (dlg.m_bIsRegister == TRUE)
			{
				m_iAccessDays = dlg.GetAccessDays();
				break;
			}
		}
		else	// 按取消直接退出
		{
			PostQuitMessage(1);
			return FALSE;
		}
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CIControlDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CIControlView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);	
	m_pMainWnd->UpdateWindow();

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	if (dlg.m_bIsRegister == TRUE)
	{
		pMainFrame->m_iAccessDays = dlg.GetAccessDays();
		pMainFrame->m_bTimerReady = TRUE;
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CIControlApp message handlers

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	CFont *m_pCaptionFont;
	CFont *m_pStaticFont;
	CFont *m_pEditFont;
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CEdit	m_AboutEditCtrl;
	CString	m_AboutEdit;
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_AboutEdit = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_EDIT_A_LISENCE, m_AboutEditCtrl);
	DDX_Text(pDX, IDC_EDIT_A_LISENCE, m_AboutEdit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CIControlApp::OnAppAbout()
{
 	CAboutDlg aboutDlg;
 	aboutDlg.DoModal();
}


HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	int CtrlID = pWnd->GetDlgCtrlID();
	if (CtrlID == IDC_STATIC_A_CAPTION && nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(m_pCaptionFont);
		pDC->SetTextColor(RGB(0, 128, 255));		// 字体颜色
        pDC->SetBkColor(RGB(0, 0, 255));		    // 字体背景色
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	else if ((CtrlID == IDC_STATIC_A_COMPANY || CtrlID == IDC_STATIC_A_LISENCE) && (nCtlColor == CTLCOLOR_STATIC))
	{
		pDC->SelectObject(m_pStaticFont);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 24, 255));			// 字体颜色
        pDC->SetBkColor(RGB(0, 0, 255));		    // 字体背景色
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	else if (CtrlID == IDC_EDIT_A_LISENCE && nCtlColor == CTLCOLOR_EDIT)
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

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pCaptionFont = new CFont;
	m_pCaptionFont->CreateFont(25, 0, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "楷体_GB2312");
	
	m_pStaticFont = new CFont;
	m_pStaticFont->CreateFont(18, 0, 0, 0, FW_MEDIUM, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "新宋体");

	m_pEditFont = new CFont;
	m_pEditFont->CreateFont(14, 0, 0, 0, FW_MEDIUM, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "黑体");

	m_AboutEditCtrl.SetFont(m_pEditFont, TRUE);			// 改变控件行高, 以便适应字体大小

//	m_AboutEdit.Format("本软件售后和技术支持: 王先生\r\n\r\n电话: 13983089826");
	m_AboutEdit.Format("Ver: 1.2");
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnDestroy() 
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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "iControl.h"

#include "MainFrm.h"
#include "iControlDoc.h"
#include "iControlView.h"
#include "RealTimeView.h"
#include "CurCurveView.h"
#include "StandardView.h"
#include "AdjustView.h"
#include "GrossView.h"
#include "SetupView.h"
#include "AutoCtrlDlg.h"
#include "AutoCtrlView.h"
#include "RegisterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
#define THE_SUB_VIEW_COUNT			4				// ÿ��Ҥ������ʾ�������

#define THE_TOOLBAR_BUTTON_WIDTH	32
#define THE_TOOLBAR_BUTTON_HEIGHT	32

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(IDI_CONFIG, OnConfigView)
	ON_COMMAND(IDI_CHANGE_VIEW, OnChangeView)
	ON_COMMAND(IDI_MOTOR_CTRL, OnMotorCtrl)
	ON_COMMAND(IDM_SERIAL_SETTING, OnSerialSetting)
	ON_COMMAND(IDM_OUTPUT_DISP, OnOutputDisp)
	ON_COMMAND(IDM_DB_BAK, OnDataBaseBak)
	ON_COMMAND(IDI_AUTO_ENABLE, OnAutoCtrl)
	ON_COMMAND(IDM_ADMIN_SET, OnAdminSet)
	ON_COMMAND(IDM_OPEN_MAINFACE, OnOpenMainFace)
	ON_COMMAND(IDI_ADJUST_EDIT1, OnAdjustEdit)
	ON_MESSAGE(WM_RX_TERMINAL, OnRXTerminal)
	ON_COMMAND(IDI_SERIAL_CFG, OnSerialSetting)
	ON_COMMAND(IDI_GROSS_OUTPUT, OnOutputDisp)
	ON_COMMAND(IDI_DATABASE_BAK, OnDataBaseBak)
	ON_COMMAND(IDI_AUTO_STOP, OnAutoCtrl)
	ON_COMMAND(IDI_SWITCH_USER, OnAdminSet)
	ON_COMMAND(IDI_RETURN_MAIN, OnOpenMainFace)
	ON_COMMAND(IDI_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDM_FURNACE_1, IDM_FURNACE_100, OnOpenFurnaceFace)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
// 	ID_INDICATOR_CAPS,
// 	ID_INDICATOR_NUM,
// 	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// ��ʼ��GDI+�Ŀ⺯��
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	m_bIsUserMode = TRUE;			// ��ʼΪ�û�ģʽ
	m_bIsAutoCtrl = FALSE;			// ��ʼΪ���Զ�����ģʽ
	m_bSave = FALSE;

	m_pGrossInfo = NULL;
	GrossInfoTimerCnt = 0;

	m_iRealTimeTimeout = MAX_REALTIME_TIMEOUT;

	m_iWatchDogTimerCnt = 1180;

	m_bTimerReady = FALSE;
}

CMainFrame::~CMainFrame()
{
}

/************************************************************************/
/* ��������Ա�˵�                                                       */
/************************************************************************/
void CMainFrame::CreateAdminMenu(void)
{
	m_MainMenu.Detach();
	m_MainMenu.DestroyMenu();

	m_MainMenu.CreateMenu();
	SetMenu(&m_MainMenu);
	
	CMenu SetMenu, BottomMenu;

	SetMenu.CreatePopupMenu();
	m_MainMenu.AppendMenu(MF_STRING | MF_ENABLED | MF_POPUP, (UINT)SetMenu.m_hMenu, "Ӳ������");
	SetMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_SERIAL_SETTING, "��������");
	SetMenu.Detach();
	SetMenu.DestroyMenu();
	
// 	SetMenu.CreatePopupMenu();
// 	m_MainMenu.AppendMenu(MF_STRING | MF_ENABLED | MF_POPUP, (UINT)SetMenu.m_hMenu, "�������");
// 	SetMenu.Detach();
// 	SetMenu.DestroyMenu();

	SetMenu.CreatePopupMenu();
	m_MainMenu.AppendMenu(MF_STRING | MF_ENABLED | MF_POPUP, (UINT)SetMenu.m_hMenu, "��������");
	SetMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_OUTPUT_DISP, "����ͳ��");
	SetMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_DB_BAK, "���ݱ���");
	SetMenu.Detach();
	SetMenu.DestroyMenu();

	SetMenu.CreatePopupMenu();
	m_MainMenu.AppendMenu(MF_STRING | MF_ENABLED | MF_POPUP, (UINT)SetMenu.m_hMenu, "�����л�");

	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	
	CString str;
	SetMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_OPEN_MAINFACE, "������ҳ");
	
	for (int i = 0; i < (pDoc->m_iKilnNum * THE_SUB_VIEW_COUNT); i = i + THE_SUB_VIEW_COUNT)
	{
		str.Empty();
		switch(pDoc->m_pKilnBuff[i / THE_SUB_VIEW_COUNT].type)
		{
		case 0:
			str.Format("%d# <�ƶ�Ҥ>", i / THE_SUB_VIEW_COUNT + 1);
			break;
		case 1:
			str.Format("%d# <����Ҥ>", i / THE_SUB_VIEW_COUNT + 1);
			break;
		case 2:
			str.Format("%d# <ֱͨҤ>", i / THE_SUB_VIEW_COUNT + 1);
			break;
		case 3:
			str.Format("%d# <���Ҥ>", i / THE_SUB_VIEW_COUNT + 1);
			break;
		default:
			break;
		}
		
		BottomMenu.CreatePopupMenu();
		SetMenu.AppendMenu(MF_STRING | MF_ENABLED | MF_POPUP, (UINT)BottomMenu.m_hMenu, str);
		BottomMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_FURNACE_1 + i + 0, "ʵʱ����");
		BottomMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_FURNACE_1 + i + 1, "ʵʱ����");
		BottomMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_FURNACE_1 + i + 2, "��ʷ���ݺ�����");
		BottomMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_FURNACE_1 + i + 3, "��׼���ݺ�����");
		BottomMenu.Detach();
		BottomMenu.DestroyMenu();
	}
	
	SetMenu.Detach();
	SetMenu.DestroyMenu();
	
 	DrawMenuBar();
}

/************************************************************************/
/* �����û��˵�                                                         */
/************************************************************************/
void CMainFrame::CreateGuestMenu(void)
{
	m_MainMenu.Detach();
	m_MainMenu.DestroyMenu();

	m_MainMenu.CreateMenu();
	SetMenu(&m_MainMenu);
	
	CMenu SetMenu, BottomMenu;
	
	SetMenu.CreatePopupMenu();
	m_MainMenu.AppendMenu(MF_STRING | MF_ENABLED | MF_POPUP, (UINT)SetMenu.m_hMenu, "��������");
	SetMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_ADMIN_SET, "�������");
	SetMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_OUTPUT_DISP, "����ͳ��");
	SetMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_DB_BAK, "���ݱ���");
	SetMenu.Detach();
	SetMenu.DestroyMenu();
	
	SetMenu.CreatePopupMenu();
	m_MainMenu.AppendMenu(MF_STRING | MF_ENABLED | MF_POPUP, (UINT)SetMenu.m_hMenu, "�����л�");
	
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc = (CIControlDoc *)pMainFrame->GetActiveDocument();
	
	CString str;
	SetMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_OPEN_MAINFACE, "������ҳ");
	
	for (int i = 0; i < (pDoc->m_iKilnNum * THE_SUB_VIEW_COUNT); i = i + THE_SUB_VIEW_COUNT)
	{
		str.Empty();
		switch(pDoc->m_pKilnBuff[i / THE_SUB_VIEW_COUNT].type)
		{
		case 0:
			str.Format("%d# <�ƶ�Ҥ>", i / THE_SUB_VIEW_COUNT + 1);
			break;
		case 1:
			str.Format("%d# <����Ҥ>", i / THE_SUB_VIEW_COUNT + 1);
			break;
		case 2:
			str.Format("%d# <ֱͨҤ>", i / THE_SUB_VIEW_COUNT + 1);
			break;
		case 3:
			str.Format("%d# <���Ҥ>", i / THE_SUB_VIEW_COUNT + 1);
			break;
		}
		
		BottomMenu.CreatePopupMenu();
		SetMenu.AppendMenu(MF_STRING | MF_ENABLED | MF_POPUP, (UINT)BottomMenu.m_hMenu, str);
		BottomMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_FURNACE_1 + i + 0, "ʵʱ����");
		BottomMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_FURNACE_1 + i + 1, "ʵʱ����");
		BottomMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_FURNACE_1 + i + 2, "��ʷ���ݺ�����");
		BottomMenu.AppendMenu(MF_STRING | MF_ENABLED, IDM_FURNACE_1 + i + 3, "��׼���ݺ�����");
		BottomMenu.Detach();
		BottomMenu.DestroyMenu();
	}
	
	SetMenu.Detach();
	SetMenu.DestroyMenu();
	
 	DrawMenuBar();
}

void CMainFrame::CreateMyMenu(int iIndex)
{
	if (iIndex == 0)
	{
		CreateAdminMenu();

		if (m_bIsAutoCtrl == TRUE)
		{
			m_wndAdminAutoStatusBar.ShowWindow(SW_SHOW);
			m_wndAdminUnAutoStatusBar.ShowWindow(SW_HIDE);
		}
		else
		{
			m_wndAdminAutoStatusBar.ShowWindow(SW_HIDE);
			m_wndAdminUnAutoStatusBar.ShowWindow(SW_SHOW);
		}
		
		m_wndUserAutoStatusBar.ShowWindow(SW_HIDE);
		m_wndUserUnAutoStatusBar.ShowWindow(SW_HIDE);
	} 
	else
	{
		CreateGuestMenu();

		if (m_bIsAutoCtrl == TRUE)
		{
			m_wndUserAutoStatusBar.ShowWindow(SW_SHOW);
			m_wndUserUnAutoStatusBar.ShowWindow(SW_HIDE);
		}
		else
		{
			m_wndUserAutoStatusBar.ShowWindow(SW_HIDE);
			m_wndUserUnAutoStatusBar.ShowWindow(SW_SHOW);
		}

		m_wndAdminAutoStatusBar.ShowWindow(SW_HIDE);
		m_wndAdminUnAutoStatusBar.ShowWindow(SW_HIDE);
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
// 	if(!CreateUserToolBar())					//����CreateUserToolBar������
// 	{
// 		TRACE0("Failed to create toolbar\n");
// 		return -1;      // fail to create
// 	}
	
	
	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}


	CreateAdminAutoToolBar();
	CreateAdminUnAutoToolBar();
	CreateUserAutoToolBar();
	CreateUserUnAutoToolBar();

	m_wndAdminAutoStatusBar.ShowWindow(SW_HIDE);
	m_wndAdminUnAutoStatusBar.ShowWindow(SW_HIDE);
	m_wndUserAutoStatusBar.ShowWindow(SW_HIDE);
	m_wndUserUnAutoStatusBar.ShowWindow(SW_SHOW);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
//  	m_wndUserUnAutoStatusBar.EnableDocking(CBRS_ALIGN_ANY);
//  	EnableDocking(CBRS_ALIGN_ANY);
//  	DockControlBar(&m_wndUserUnAutoStatusBar);

	return 0;
}

BOOL CMainFrame::CreateUserAutoToolBar(void)
{
	CImageList m_Img;
	CString str;
	
	// ���û�д����������ʹ���
	if (!m_wndUserAutoStatusBar.GetSafeHwnd())
	{
		if (!m_wndUserAutoStatusBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_SIZE_DYNAMIC | CBRS_GRIPPER))
		{
			return -1;
		}
	}
	
	//����Ĵ��������ð�ť����С��Ⱥ������
	m_wndUserAutoStatusBar.GetToolBarCtrl().SetButtonWidth(32, 128);
	
	//����Ĵ���������"��"��λͼ,����˳��:���x, ���y, ��ʽnFlag, ��ʼʱ�������nInitial, ����������nGrow
	m_Img.Create(THE_TOOLBAR_BUTTON_WIDTH, THE_TOOLBAR_BUTTON_HEIGHT, ILC_COLOR32| ILC_MASK, 1, 1);
	
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_SWITCH_USER));		// �����л�
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_GROSS_OUTPUT));		// ����ͳ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_DATABASE_BAK));		// ���ݱ���
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_RETURN_MAIN));		// ����������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_CHANGE_VIEW));		// �л�����
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_MOTOR_CTRL));		// ���Ƶ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_AUTO_ENABLE));		// �Զ�����

	m_Img.Add(AfxGetApp()->LoadIcon(IDI_ABOUT));			// ����
	m_wndUserAutoStatusBar.GetToolBarCtrl().SetHotImageList(&m_Img);
	m_Img.Detach();

	//����Ĵ���������"��"��λͼ,����˳��:���x, ���y, ��ʽnFlag, ��ʼʱ�������nInitial, ����������nGrow
	m_Img.Create(THE_TOOLBAR_BUTTON_WIDTH, THE_TOOLBAR_BUTTON_HEIGHT, ILC_COLOR32| ILC_MASK, 1, 1);
	
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_SWITCH_USER));		// �����л�
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_GROSS_OUTPUT));		// ����ͳ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_DATABASE_BAK));		// ���ݱ���
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_RETURN_MAIN));		// ����������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_CHANGE_VIEW));		// �л�����
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_MOTOR_CTRL));		// ���Ƶ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_AUTO_ENABLE));	// �Զ�����

	m_Img.Add(AfxGetApp()->LoadIcon(IDI_ABOUT));			// ����
	m_wndUserAutoStatusBar.GetToolBarCtrl().SetImageList(&m_Img);
	m_Img.Detach();
	
	//���ù�������ť����
	m_wndUserAutoStatusBar.SetButtons(NULL, 12);
	
	m_wndUserAutoStatusBar.SetButtonInfo(0, IDI_SWITCH_USER, TBBS_BUTTON, 0);
	m_wndUserAutoStatusBar.SetButtonText(0, "�������");
	
	m_wndUserAutoStatusBar.SetButtonInfo(1, IDI_GROSS_OUTPUT, TBBS_BUTTON, 1);
	m_wndUserAutoStatusBar.SetButtonText(1, "����ͳ��");
	
	m_wndUserAutoStatusBar.SetButtonInfo(2, IDI_DATABASE_BAK, TBBS_BUTTON, 2);
	m_wndUserAutoStatusBar.SetButtonText(2, "���ݱ���");
	
	m_wndUserAutoStatusBar.SetButtonInfo(3, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���
	
	m_wndUserAutoStatusBar.SetButtonInfo(4, IDI_RETURN_MAIN, TBBS_BUTTON, 3);
	m_wndUserAutoStatusBar.SetButtonText(4, "������ҳ");
	
	m_wndUserAutoStatusBar.SetButtonInfo(5, IDI_CHANGE_VIEW, TBBS_BUTTON, 4);
	m_wndUserAutoStatusBar.SetButtonText(5, "�����л�");
	
	m_wndUserAutoStatusBar.SetButtonInfo(6, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���

	m_wndUserAutoStatusBar.SetButtonInfo(7, IDI_MOTOR_CTRL, TBBS_BUTTON, 5);
	m_wndUserAutoStatusBar.SetButtonText(7, "�������");
	m_wndUserAutoStatusBar.SetButtonInfo(8, IDI_AUTO_ENABLE, TBBS_BUTTON, 6);
	m_wndUserAutoStatusBar.SetButtonText(8, "�Կز�������");

	m_wndUserAutoStatusBar.SetButtonInfo(9, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���
	
	m_wndUserAutoStatusBar.SetButtonInfo(10, IDI_ABOUT, TBBS_BUTTON, 7);
	m_wndUserAutoStatusBar.SetButtonText(10, "����");
	
	m_wndUserAutoStatusBar.SetButtonInfo(11, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���
	
	CRect m_RectToolBar;
	//�õ���������һ����ť�Ĵ�С
	m_wndUserAutoStatusBar.GetItemRect(0, &m_RectToolBar);
	//���ð�ť�Ĵ�С�Ͱ�ť��ͼƬ�Ĵ�С
	m_wndUserAutoStatusBar.SetSizes(m_RectToolBar.Size(), CSize(THE_TOOLBAR_BUTTON_WIDTH, THE_TOOLBAR_BUTTON_HEIGHT));
	
	return TRUE;
}

BOOL CMainFrame::CreateUserUnAutoToolBar(void)
{
	CImageList m_Img;
	CString str;
	
	// ���û�д����������ʹ���
	if (!m_wndUserUnAutoStatusBar.GetSafeHwnd())
	{
		if (!m_wndUserUnAutoStatusBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_SIZE_DYNAMIC | CBRS_GRIPPER))
		{
			return -1;
		}
	}
	
	//����Ĵ��������ð�ť����С��Ⱥ������
	m_wndUserUnAutoStatusBar.GetToolBarCtrl().SetButtonWidth(32, 128);
	
	//����Ĵ���������"��"��λͼ,����˳��:���x, ���y, ��ʽnFlag, ��ʼʱ�������nInitial, ����������nGrow
	m_Img.Create(THE_TOOLBAR_BUTTON_WIDTH, THE_TOOLBAR_BUTTON_HEIGHT, ILC_COLOR32| ILC_MASK, 1, 1);
	
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_SWITCH_USER));		// �����л�
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_GROSS_OUTPUT));		// ����ͳ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_DATABASE_BAK));		// ���ݱ���
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_RETURN_MAIN));		// ����������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_CHANGE_VIEW));		// �л�����
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_MOTOR_CTRL));		// ���Ƶ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_AUTO_STOP));		// �Զ�����
	
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_ABOUT));			// ����
	m_wndUserUnAutoStatusBar.GetToolBarCtrl().SetHotImageList(&m_Img);
	m_Img.Detach();
	
	//����Ĵ���������"��"��λͼ,����˳��:���x, ���y, ��ʽnFlag, ��ʼʱ�������nInitial, ����������nGrow
	m_Img.Create(THE_TOOLBAR_BUTTON_WIDTH, THE_TOOLBAR_BUTTON_HEIGHT, ILC_COLOR32| ILC_MASK, 1, 1);
	
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_SWITCH_USER));		// �����л�
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_GROSS_OUTPUT));		// ����ͳ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_DATABASE_BAK));		// ���ݱ���
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_RETURN_MAIN));		// ����������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_CHANGE_VIEW));		// �л�����
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_MOTOR_CTRL));		// ���Ƶ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_AUTO_STOP));		// �Զ�����
	
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_ABOUT));			// ����
	m_wndUserUnAutoStatusBar.GetToolBarCtrl().SetImageList(&m_Img);
	m_Img.Detach();
	
	//���ù�������ť����
	m_wndUserUnAutoStatusBar.SetButtons(NULL, 12);
	
	m_wndUserUnAutoStatusBar.SetButtonInfo(0, IDI_SWITCH_USER, TBBS_BUTTON, 0);
	m_wndUserUnAutoStatusBar.SetButtonText(0, "�������");
	
	m_wndUserUnAutoStatusBar.SetButtonInfo(1, IDI_GROSS_OUTPUT, TBBS_BUTTON, 1);
	m_wndUserUnAutoStatusBar.SetButtonText(1, "����ͳ��");
	
	m_wndUserUnAutoStatusBar.SetButtonInfo(2, IDI_DATABASE_BAK, TBBS_BUTTON, 2);
	m_wndUserUnAutoStatusBar.SetButtonText(2, "���ݱ���");
	
	m_wndUserUnAutoStatusBar.SetButtonInfo(3, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���
	
	m_wndUserUnAutoStatusBar.SetButtonInfo(4, IDI_RETURN_MAIN, TBBS_BUTTON, 3);
	m_wndUserUnAutoStatusBar.SetButtonText(4, "������ҳ");
	
	m_wndUserUnAutoStatusBar.SetButtonInfo(5, IDI_CHANGE_VIEW, TBBS_BUTTON, 4);
	m_wndUserUnAutoStatusBar.SetButtonText(5, "�����л�");
	
	m_wndUserUnAutoStatusBar.SetButtonInfo(6, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���
	
	m_wndUserUnAutoStatusBar.SetButtonInfo(7, IDI_MOTOR_CTRL, TBBS_BUTTON, 5);
	m_wndUserUnAutoStatusBar.SetButtonText(7, "�������");
	m_wndUserUnAutoStatusBar.SetButtonInfo(8, IDI_AUTO_STOP, TBBS_BUTTON, 6);
	m_wndUserUnAutoStatusBar.SetButtonText(8, "�Կز�������");
	
	m_wndUserUnAutoStatusBar.SetButtonInfo(9, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���
	
	m_wndUserUnAutoStatusBar.SetButtonInfo(10, IDI_ABOUT, TBBS_BUTTON, 7);
	m_wndUserUnAutoStatusBar.SetButtonText(10, "����");
	
	m_wndUserUnAutoStatusBar.SetButtonInfo(11, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���
	
	CRect m_RectToolBar;
	//�õ���������һ����ť�Ĵ�С
	m_wndUserUnAutoStatusBar.GetItemRect(0, &m_RectToolBar);
	//���ð�ť�Ĵ�С�Ͱ�ť��ͼƬ�Ĵ�С
	m_wndUserUnAutoStatusBar.SetSizes(m_RectToolBar.Size(), CSize(THE_TOOLBAR_BUTTON_WIDTH, THE_TOOLBAR_BUTTON_HEIGHT));
	
	return TRUE;
}

BOOL CMainFrame::CreateAdminAutoToolBar(void)
{
	CImageList m_Img;
	CString str;
	
	// ���û�д����������ʹ���
	if (!m_wndAdminAutoStatusBar.GetSafeHwnd())
	{
		if (!m_wndAdminAutoStatusBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_SIZE_DYNAMIC | CBRS_GRIPPER))
		{
			return -1;
		}
	}	

	//����Ĵ��������ð�ť����С��Ⱥ������
	m_wndAdminAutoStatusBar.GetToolBarCtrl().SetButtonWidth(32, 128);
	
	//����Ĵ���������"��"��λͼ,����˳��:���x, ���y, ��ʽnFlag, ��ʼʱ�������nInitial, ����������nGrow
	m_Img.Create(THE_TOOLBAR_BUTTON_WIDTH, THE_TOOLBAR_BUTTON_HEIGHT, ILC_COLOR32| ILC_MASK, 1, 1);
	
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_CONFIG));			// ��������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_SERIAL_CFG));		// ��������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_GROSS_OUTPUT));		// ����ͳ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_DATABASE_BAK));		// ���ݱ���
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_RETURN_MAIN));		// ����������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_CHANGE_VIEW));		// �л�����
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_ADJUST_EDIT1));		// У����༭
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_MOTOR_CTRL));		// ���Ƶ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_AUTO_ENABLE));		// �Զ�����

	m_Img.Add(AfxGetApp()->LoadIcon(IDI_ABOUT));			// ����
	m_wndAdminAutoStatusBar.GetToolBarCtrl().SetHotImageList(&m_Img);
	m_Img.Detach();
	
	//����Ĵ���������"��"��λͼ,����˳��:���x, ���y, ��ʽnFlag, ��ʼʱ�������nInitial, ����������nGrow
	m_Img.Create(THE_TOOLBAR_BUTTON_WIDTH, THE_TOOLBAR_BUTTON_HEIGHT, ILC_COLOR32| ILC_MASK, 1, 1);
	
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_CONFIG));			// ��������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_SERIAL_CFG));		// ��������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_GROSS_OUTPUT));		// ����ͳ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_DATABASE_BAK));		// ���ݱ���
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_RETURN_MAIN));		// ����������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_CHANGE_VIEW));		// �л�����
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_ADJUST_EDIT1));		// У����༭
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_MOTOR_CTRL));		// ���Ƶ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_AUTO_ENABLE));		// �Զ�����

	m_Img.Add(AfxGetApp()->LoadIcon(IDI_ABOUT));			// ����
	m_wndAdminAutoStatusBar.GetToolBarCtrl().SetImageList(&m_Img);
	m_Img.Detach();
	
	//���ù�������ť����
	m_wndAdminAutoStatusBar.SetButtons(NULL, 15);
	
	m_wndAdminAutoStatusBar.SetButtonInfo(0, IDI_CONFIG, TBBS_BUTTON, 0);
	m_wndAdminAutoStatusBar.SetButtonText(0, "���ݿ�����");

	m_wndAdminAutoStatusBar.SetButtonInfo(1, IDI_SERIAL_CFG, TBBS_BUTTON, 1);
	m_wndAdminAutoStatusBar.SetButtonText(1, "��������");

	m_wndAdminAutoStatusBar.SetButtonInfo(2, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���
	
	m_wndAdminAutoStatusBar.SetButtonInfo(3, IDI_GROSS_OUTPUT, TBBS_BUTTON, 2);
	m_wndAdminAutoStatusBar.SetButtonText(3, "����ͳ��");

	m_wndAdminAutoStatusBar.SetButtonInfo(4, IDI_DATABASE_BAK, TBBS_BUTTON, 3);
	m_wndAdminAutoStatusBar.SetButtonText(4, "���ݿⱸ��");
	
	m_wndAdminAutoStatusBar.SetButtonInfo(5, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���
	
	m_wndAdminAutoStatusBar.SetButtonInfo(6, IDI_RETURN_MAIN, TBBS_BUTTON, 4);
	m_wndAdminAutoStatusBar.SetButtonText(6, "������ҳ");

	m_wndAdminAutoStatusBar.SetButtonInfo(7, IDI_CHANGE_VIEW, TBBS_BUTTON, 5);
	m_wndAdminAutoStatusBar.SetButtonText(7, "�����л�");
	
	m_wndAdminAutoStatusBar.SetButtonInfo(8, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���

	m_wndAdminAutoStatusBar.SetButtonInfo(9, IDI_ADJUST_EDIT1, TBBS_BUTTON, 6);
	m_wndAdminAutoStatusBar.SetButtonText(9, "У����");

	m_wndAdminAutoStatusBar.SetButtonInfo(10, IDI_MOTOR_CTRL, TBBS_BUTTON, 7);
	m_wndAdminAutoStatusBar.SetButtonText(10, "�������");

	m_wndAdminAutoStatusBar.SetButtonInfo(11, IDI_AUTO_ENABLE, TBBS_BUTTON, 8);
	m_wndAdminAutoStatusBar.SetButtonText(11, "�Կز�������");

	m_wndAdminAutoStatusBar.SetButtonInfo(12, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���

	m_wndAdminAutoStatusBar.SetButtonInfo(13, IDI_ABOUT, TBBS_BUTTON, 9);
	m_wndAdminAutoStatusBar.SetButtonText(13, "����");

	m_wndAdminAutoStatusBar.SetButtonInfo(14, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���
	
	CRect m_RectToolBar;
	//�õ���������һ����ť�Ĵ�С
	m_wndAdminAutoStatusBar.GetItemRect(0, &m_RectToolBar);
	//���ð�ť�Ĵ�С�Ͱ�ť��ͼƬ�Ĵ�С
	m_wndAdminAutoStatusBar.SetSizes(m_RectToolBar.Size(), CSize(THE_TOOLBAR_BUTTON_WIDTH, THE_TOOLBAR_BUTTON_HEIGHT));
	
	return TRUE;
}

BOOL CMainFrame::CreateAdminUnAutoToolBar(void)
{
	CImageList m_Img;
	CString str;
	
	// ���û�д����������ʹ���
	if (!m_wndAdminUnAutoStatusBar.GetSafeHwnd())
	{
		if (!m_wndAdminUnAutoStatusBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_SIZE_DYNAMIC | CBRS_GRIPPER))
		{
			return -1;
		}
	}	

	//����Ĵ��������ð�ť����С��Ⱥ������
	m_wndAdminUnAutoStatusBar.GetToolBarCtrl().SetButtonWidth(32, 128);
	
	//����Ĵ���������"��"��λͼ,����˳��:���x, ���y, ��ʽnFlag, ��ʼʱ�������nInitial, ����������nGrow
	m_Img.Create(THE_TOOLBAR_BUTTON_WIDTH, THE_TOOLBAR_BUTTON_HEIGHT, ILC_COLOR32| ILC_MASK, 1, 1);
	
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_CONFIG));			// ��������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_SERIAL_CFG));		// ��������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_GROSS_OUTPUT));		// ����ͳ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_DATABASE_BAK));		// ���ݱ���
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_RETURN_MAIN));		// ����������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_CHANGE_VIEW));		// �л�����
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_ADJUST_EDIT1));		// У����༭
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_MOTOR_CTRL));		// ���Ƶ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_AUTO_STOP));		// �Զ�����

	m_Img.Add(AfxGetApp()->LoadIcon(IDI_ABOUT));			// ����
	m_wndAdminUnAutoStatusBar.GetToolBarCtrl().SetHotImageList(&m_Img);
	m_Img.Detach();
	
	//����Ĵ���������"��"��λͼ,����˳��:���x, ���y, ��ʽnFlag, ��ʼʱ�������nInitial, ����������nGrow
	m_Img.Create(THE_TOOLBAR_BUTTON_WIDTH, THE_TOOLBAR_BUTTON_HEIGHT, ILC_COLOR32| ILC_MASK, 1, 1);
	
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_CONFIG));			// ��������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_SERIAL_CFG));		// ��������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_GROSS_OUTPUT));		// ����ͳ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_DATABASE_BAK));		// ���ݱ���
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_RETURN_MAIN));		// ����������
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_CHANGE_VIEW));		// �л�����
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_ADJUST_EDIT1));		// У����༭
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_MOTOR_CTRL));		// ���Ƶ��
	m_Img.Add(AfxGetApp()->LoadIcon(IDI_AUTO_STOP));		// �Զ�����

	m_Img.Add(AfxGetApp()->LoadIcon(IDI_ABOUT));			// ����
	m_wndAdminUnAutoStatusBar.GetToolBarCtrl().SetImageList(&m_Img);
	m_Img.Detach();
	
	//���ù�������ť����
	m_wndAdminUnAutoStatusBar.SetButtons(NULL, 15);
	
	m_wndAdminUnAutoStatusBar.SetButtonInfo(0, IDI_CONFIG, TBBS_BUTTON, 0);
	m_wndAdminUnAutoStatusBar.SetButtonText(0, "���ݿ�����");

	m_wndAdminUnAutoStatusBar.SetButtonInfo(1, IDI_SERIAL_CFG, TBBS_BUTTON, 1);
	m_wndAdminUnAutoStatusBar.SetButtonText(1, "��������");

	m_wndAdminUnAutoStatusBar.SetButtonInfo(2, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���
	
	m_wndAdminUnAutoStatusBar.SetButtonInfo(3, IDI_GROSS_OUTPUT, TBBS_BUTTON, 2);
	m_wndAdminUnAutoStatusBar.SetButtonText(3, "����ͳ��");

	m_wndAdminUnAutoStatusBar.SetButtonInfo(4, IDI_DATABASE_BAK, TBBS_BUTTON, 3);
	m_wndAdminUnAutoStatusBar.SetButtonText(4, "���ݿⱸ��");
	
	m_wndAdminUnAutoStatusBar.SetButtonInfo(5, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���
	
	m_wndAdminUnAutoStatusBar.SetButtonInfo(6, IDI_RETURN_MAIN, TBBS_BUTTON, 4);
	m_wndAdminUnAutoStatusBar.SetButtonText(6, "������ҳ");

	m_wndAdminUnAutoStatusBar.SetButtonInfo(7, IDI_CHANGE_VIEW, TBBS_BUTTON, 5);
	m_wndAdminUnAutoStatusBar.SetButtonText(7, "�����л�");
	
	m_wndAdminUnAutoStatusBar.SetButtonInfo(8, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���

	m_wndAdminUnAutoStatusBar.SetButtonInfo(9, IDI_ADJUST_EDIT1, TBBS_BUTTON, 6);
	m_wndAdminUnAutoStatusBar.SetButtonText(9, "У����");

	m_wndAdminUnAutoStatusBar.SetButtonInfo(10, IDI_MOTOR_CTRL, TBBS_BUTTON, 7);
	m_wndAdminUnAutoStatusBar.SetButtonText(10, "�������");

	m_wndAdminUnAutoStatusBar.SetButtonInfo(11, IDI_AUTO_STOP, TBBS_BUTTON, 8);
	m_wndAdminUnAutoStatusBar.SetButtonText(11, "�Կز�������");

	m_wndAdminUnAutoStatusBar.SetButtonInfo(12, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���

	m_wndAdminUnAutoStatusBar.SetButtonInfo(13, IDI_ABOUT, TBBS_BUTTON, 9);
	m_wndAdminUnAutoStatusBar.SetButtonText(13, "����");

	m_wndAdminUnAutoStatusBar.SetButtonInfo(14, IDC_MSG_BUTTONSPLI, TBBS_SEPARATOR, 14);	//�ָ���
	
	CRect m_RectToolBar;
	//�õ���������һ����ť�Ĵ�С
	m_wndAdminUnAutoStatusBar.GetItemRect(0, &m_RectToolBar);
	//���ð�ť�Ĵ�С�Ͱ�ť��ͼƬ�Ĵ�С
	m_wndAdminUnAutoStatusBar.SetSizes(m_RectToolBar.Size(), CSize(THE_TOOLBAR_BUTTON_WIDTH, THE_TOOLBAR_BUTTON_HEIGHT));
	
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.lpszName = "SDY-01B ���Ҥ���ܼ��ϵͳ";
	cs.style = WS_OVERLAPPEDWINDOW;
 	cs.cx = GetSystemMetrics(SM_CXSCREEN);
 	cs.cy = GetSystemMetrics(SM_CYSCREEN);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{	
	m_iTopViewID = AddView(pContext);
	
	pContext->m_pNewViewClass = RUNTIME_CLASS(CRealTimeView);
	AddView(pContext);

	pContext->m_pNewViewClass = RUNTIME_CLASS(CCurCurveView);
	AddView(pContext);

	pContext->m_pNewViewClass = RUNTIME_CLASS(CSpliteFrame);
	AddView(pContext);

	pContext->m_pNewViewClass = RUNTIME_CLASS(CStandardView);
	AddView(pContext);

	pContext->m_pNewViewClass = RUNTIME_CLASS(CGrossView);
	AddView(pContext);

	pContext->m_pNewViewClass = RUNTIME_CLASS(CSetupView);
	AddView(pContext);

	pContext->m_pNewViewClass = RUNTIME_CLASS(CAdjustView);
	AddView(pContext);

	pContext->m_pNewViewClass = RUNTIME_CLASS(CAutoCtrlView);
	AddView(pContext);

	SetTopView(m_iTopViewID);			// ���ó�ʼ��ʾ��ͼ=����ͼ
	m_iChangeViewID      = 0;			// ѭ����ʾ����ͼָ���ʼΪ0
	m_iChangeGrossView   = 0;			// ѭ��������ָ���ʼΪ0
	m_iChangeAdjustView  = 0;			// ѭ��У����ָ���ʼΪ0

	SetTimer(0, 500, NULL);	
	return TRUE;
}

// ���ö���ͼ����ʾ
void CMainFrame::SetTopView(int iTopViewID)
{
	if (iTopViewID < 0 || iTopViewID >= m_ViewPoint.size())
	{
		return;
	}

	CView *pOldTopView = (CView *)m_ViewPoint[m_iTopViewID];
	CView *pNewTopView = (CView *)m_ViewPoint[iTopViewID];
	
//	::SetWindowLong(pOldTopView->m_hWnd, GWL_ID, AFX_IDW_PANE_LAST - m_ViewPoint.size());
	::SetWindowLong(pOldTopView->m_hWnd, GWL_ID, AFX_IDW_PANE_LAST - m_iTopViewID);
	pOldTopView->ShowWindow(SW_HIDE);	
	
	::SetWindowLong(pNewTopView->m_hWnd, GWL_ID, AFX_IDW_PANE_FIRST);
	pNewTopView->ShowWindow(SW_SHOW);
	
	m_iTopViewID = iTopViewID;
	
	RecalcLayout();
}

// ������ͼ
int CMainFrame::AddView(CCreateContext* pContext)
{
	CView *pView = NULL;
	
	if (m_ViewPoint.size() > 255)
	{
		return -1;
	}
	
	if (pContext != NULL && pContext->m_pNewViewClass != NULL)
	{
		pView = (CView *)CreateView(pContext, AFX_IDW_PANE_FIRST - m_ViewPoint.size());
		if (pView == NULL)
		{
			return -2;
		}
		
		pView->ShowWindow(SW_SHOW);
		m_ViewPoint.push_back(pView);
		return (m_ViewPoint.size() - 1);
	}
	else
	{
		return -3;
	}
}

// �����Ƴ���ʾ�Ի���
void CMainFrame::CreateGrossInfo(CString str_info)
{
	if (m_pGrossInfo != NULL)
	{
		delete m_pGrossInfo;
		m_pGrossInfo = NULL;
	}

	m_pGrossInfo = new CGrossInfo;
	m_pGrossInfo->m_InfoString = str_info;
	m_pGrossInfo->Create(IDD_GROSS_INFO, this);
	
	CRect DlgRect, ClientRect;
	GetWindowRect(&ClientRect);
	m_pGrossInfo->GetWindowRect(&DlgRect);
	m_pGrossInfo->MoveWindow(ClientRect.Width() - DlgRect.Width() - 7, ClientRect.Height() - DlgRect.Height() - 30, DlgRect.Width(), DlgRect.Height());

	m_pGrossInfo->ShowWindow(SW_SHOW);
	m_pGrossInfo->UpdateWindow();

	GrossInfoTimerCnt = 30;				// 15�����ʧ�Ի���
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if (m_bTimerReady == FALSE)
	{
		return;
	}
	
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	if (pDoc->m_bDBOpen == FALSE)		// ������ݿ�򿪺����ݳ�ʼ��Ϊ�ɹ�
	{
		return;
	}
	
	int Onlines = 0;
	for (int i = 0; i < pDoc->m_TerminalCfg.iTerminalNum; i++)
	{
		if (m_pCom->m_pTemObjectBuff[i].TemSts)
		{
			Onlines++;
		}
	}

	CString str, str1;
	for (i = 0; i < pDoc->m_iKilnNum; i++)
	{
		if (pDoc->m_pKilnBuff[i].ControlNum > 0)
		{
			str1.Empty();

			if (pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].bControl == TRUE)
			{
				str1.Format("[%d# : Auto] ", i + 1);
			}
			else
			{
				str1.Format("[%d# : Manual] ", i + 1);
			}

			str += str1;
		}
	}

	CString str2;
	if (m_pCom->GetDtuLoginStatus() == TRUE)
	{
		str2 = "[DTU Login]";
	}
	else
	{
		str2 = "[DTU Unlogin]";
	}
	
	m_strStatus.Empty();

	if (pDoc->m_iDevPower > 0)// ��ʾ����Ϊͣ��
	{
		m_strStatus.Format("[DTU:Com%d, %d, %c, %d, %d, %d] %s [Com%d, %d, %c, %d, %d] [Terminals: %d] [Online: %d] [Offline: %d] %s", 
																						pDoc->m_ComCfg.iDtuPortNum,
																						pDoc->m_ComCfg.iDtuBaudrate,
																						pDoc->m_ComCfg.cParity, 
																						pDoc->m_ComCfg.iDataBits, 
																						pDoc->m_ComCfg.iStopBits,
																						pDoc->m_ComCfg.lDtuPort,
																						str2,
																						
																						pDoc->m_ComCfg.iPortNum, 
																						pDoc->m_ComCfg.iBaudrate, 
																						pDoc->m_ComCfg.cParity, 
																						pDoc->m_ComCfg.iDataBits, 
																						pDoc->m_ComCfg.iStopBits, 
																						pDoc->m_TerminalCfg.iTerminalNum,
																						Onlines, 
																						(pDoc->m_TerminalCfg.iTerminalNum - Onlines),
																						str);
	}
	else// ��ʾ��Ϊͣ��
	{
		m_strStatus.Format("[DTU:Com%d, %d, %c, %d, %d, %d] %s [Com%d, %d, %c, %d, %d] [Terminals: %d] [Online: %d] [Offline: %d] %s", 
																						pDoc->m_ComCfg.iDtuPortNum,
																						pDoc->m_ComCfg.iDtuBaudrate,
																						pDoc->m_ComCfg.cParity, 
																						pDoc->m_ComCfg.iDataBits, 
																						pDoc->m_ComCfg.iStopBits,
																						pDoc->m_ComCfg.lDtuPort,
																						str2,
																						
																						pDoc->m_ComCfg.iPortNum, 
																						pDoc->m_ComCfg.iBaudrate, 
																						pDoc->m_ComCfg.cParity, 
																						pDoc->m_ComCfg.iDataBits, 
																						pDoc->m_ComCfg.iStopBits, 
																						pDoc->m_TerminalCfg.iTerminalNum,
																						Onlines, 
																						(pDoc->m_TerminalCfg.iTerminalNum - Onlines),
																						str);
	}


	m_wndStatusBar.SetPaneText(0, m_strStatus, TRUE);

	CString info_str;
	if (GrossInfoTimerCnt > 0)
	{
		if ((GrossInfoTimerCnt % 2) == 0)
		{
			info_str.Format("Push information Tips (%d s Dialog close)", GrossInfoTimerCnt / 2);
			m_pGrossInfo->SetWindowText(info_str);
		}

		GrossInfoTimerCnt--;
		if (GrossInfoTimerCnt == 0)			// ��ʱʱ�䵽�رնԻ���
		{
			if (m_pGrossInfo != NULL)
			{
				delete m_pGrossInfo;
				m_pGrossInfo = NULL;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (m_iRealTimeTimeout > 0)
	{
		m_iRealTimeTimeout--;
	}

	// 10���Ӽ��һ����ʷ���ݿ�ĸ���, �ж�����ʱ�䣬�ж��Ƿ�رճ���;
	int history_days;
	m_iWatchDogTimerCnt++;
	if (m_iWatchDogTimerCnt > 1200)	
	{
		m_iWatchDogTimerCnt = 0;
		history_days = (pDoc->GetHistoryRecordCnt() * pDoc->m_iHisSaveInternal) / 86400;
		if (history_days >= m_iAccessDays)
		{
			CString str_aes;
			str_aes.Format("�������������� = %d, %d, %d, %d", history_days, m_iAccessDays, pDoc->GetHistoryRecordCnt(), pDoc->m_iHisSaveInternal);
			MessageBox(str_aes);

			CRegisterDlg dlg;
			dlg.CleanRegedit();
			PostQuitMessage(1);
			return;
		}
	}

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();

	KillTimer(0);						// �ͷŶ�ʱ��һ��Ҫ�ڴ��ͷ�, ���ܷ������������
	m_MainMenu.DestroyMenu();

	m_pCom->DestroyWindows();
	delete m_pCom;

	if (m_pGrossInfo != NULL)
	{
		delete m_pGrossInfo;
	}

	GdiplusShutdown(m_gdiplusToken);	// �ر�GDI+������
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

// ͨѶ���öԻ���
void CMainFrame::OnSerialSetting()
{
	CSerialDlg dlg;

	dlg.DoModal();
}

void CMainFrame::OnMotorCtrl()
{
	CMotorCtrlDlg dlg;

	dlg.DoModal();
}

// ��Ϣ����
LONG CMainFrame::OnRXTerminal(WPARAM cmd, LPARAM para)
{
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int iKilnIdx;

	BOOL res = FALSE;

	if (cmd == 0)	// ʵʱ���ݸ���
	{
		m_iRealTimeTimeout = MAX_REALTIME_TIMEOUT;

		/* �豸��ʱ���������ʵʱ���� */
		if (pDoc->m_iDevPower > 0)
		{
			res = pDoc->UpdateRealTimeValue();							// ����ʵʱ��������
		}
		
		/* ��ģ��δͨѶ�ɹ�������ʷ��¼����2160��(7.5��/5����) */
		if (res == FALSE)
		{
			if (pDoc->GetHistoryRecordCnt() > 2160)
			{
				for (int k = 0; k < pDoc->m_iKilnNum; k++)
				{
					int ramdon_idx = rand() % (pDoc->m_pKilnBuff[k].TemperatureNum + 1);
					pDoc->m_pKilnBuff[k].m_pTempObjectBuff[ramdon_idx - 1].Value = 0xffffffff;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// ÿ����һ��ʵʱ���ݾ͸���һ��Զ�̷�����������
		//////////////////////////////////////////////////////////////////////////
		int i, j;
		WORD *pTempValuebuff, *pTempAValuebuff, *pPressValuebuff, *pHumiValuebuff, *pMotorValuebuff;

		CTime tt = CTime::GetCurrentTime();	// ��ȡϵͳ����

		for (i = 0; i < pDoc->m_iKilnNum; i++)
		{
			pTempValuebuff  = new WORD[pDoc->m_pKilnBuff[i].TemperatureNum];
			pTempAValuebuff = new WORD[pDoc->m_pKilnBuff[i].TemperatureANum];
			pPressValuebuff = new WORD[pDoc->m_pKilnBuff[i].PressNum];
			pHumiValuebuff  = new WORD[pDoc->m_pKilnBuff[i].HumidityNum];
			pMotorValuebuff = new WORD[pDoc->m_pKilnBuff[i].MotorNum];

			for (j = 0; j < pDoc->m_pKilnBuff[i].TemperatureNum; j++)
			{
				pTempValuebuff[j] = (WORD)(pDoc->m_pKilnBuff[i].m_pTempObjectBuff[j].Value);
			}
			
			for (j = 0; j < pDoc->m_pKilnBuff[i].TemperatureANum; j++)
			{
				pTempAValuebuff[j] = (WORD)(pDoc->m_pKilnBuff[i].m_pTempAObjectBuff[j].Value);
			}

			for (j = 0; j < pDoc->m_pKilnBuff[i].PressNum; j++)
			{
				pPressValuebuff[j] = (WORD)(pDoc->m_pKilnBuff[i].m_pPressObjectBuff[j].Value);
			}

			for (j = 0; j < pDoc->m_pKilnBuff[i].HumidityNum; j++)
			{
				pHumiValuebuff[j] = (WORD)(pDoc->m_pKilnBuff[i].m_pHumiObjectBuff[j].Value);
			}

			for (j = 0; j < pDoc->m_pKilnBuff[i].MotorNum; j++)
			{
				pMotorValuebuff[j] = (WORD)(pDoc->m_pKilnBuff[i].m_pMotorObjectBuff[j].Value);		
			}

			m_pCom->SetRealTimeTransmit(i,
										pDoc->m_pKilnBuff[i].TemperatureNum, pTempValuebuff,
										pDoc->m_pKilnBuff[i].TemperatureANum, pTempAValuebuff,
										pDoc->m_pKilnBuff[i].PressNum, pPressValuebuff,
										pDoc->m_pKilnBuff[i].HumidityNum, pHumiValuebuff,
										pDoc->m_pKilnBuff[i].MotorNum, pMotorValuebuff,
										(WORD)tt.GetYear(), 
										(BYTE)tt.GetMonth(), 
										(BYTE)tt.GetDay(), 
										(BYTE)tt.GetHour(), 
										(BYTE)tt.GetMinute(), 
										(BYTE)tt.GetSecond());			// ע������ֻ�Ǹ��»���������δ�������ݵ�������

			delete [] pTempValuebuff;
			delete [] pTempAValuebuff;
			delete [] pPressValuebuff;
			delete [] pHumiValuebuff;
			delete [] pMotorValuebuff;
		}

		//////////////////////////////////////////////////////////////////////////
		// ��ʱ�洢��ʷ����
		//////////////////////////////////////////////////////////////////////////
		int internal;
		CTime t = CTime::GetCurrentTime();
		if (pDoc->m_iHisSaveInternal < 60)
		{
			internal = pDoc->m_iHisSaveInternal;
			if ((t.GetSecond() % internal) == 0 && m_bSave == FALSE)
			{
				pDoc->AppendHistoryRecord();				// �����ʷ����, ��ʱ�洢
				m_tLastSaveTime = t;
				m_bSave = TRUE;
			}
			
			if (m_bSave == TRUE)
			{
				if ((t.GetSecond() % internal) != 0)
				{
					m_bSave = FALSE;
				}	
			}
		}
		else
		{
			internal = pDoc->m_iHisSaveInternal / 60;
			if ((t.GetMinute() % internal) == 0 && m_bSave == FALSE)
			{
				pDoc->AppendHistoryRecord();				// �����ʷ����, ��ʱ�洢
				m_tLastSaveTime = t;
				m_bSave = TRUE;
			}
			
			if (m_bSave == TRUE)
			{
				CTimeSpan time_diff = t - m_tLastSaveTime;
				if (time_diff.GetTotalMinutes() > 0)
				{
					m_bSave = FALSE;
				}	
			}
		}
		//////////////////////////////////////////////////////////////////////////

		if (m_iTopViewID == 1)				// ʵʱ���ݽ���
		{
			CRealTimeView *pRealTimeView = (CRealTimeView *)m_ViewPoint[1];
			pRealTimeView->UpdateView();
		}
		else if (m_iTopViewID == 2)			// ʵʱ���߽���
		{
			CCurCurveView *pCurCurveView = (CCurCurveView *)m_ViewPoint[2];
			pCurCurveView->UpdateView();
		}
		else if (m_iTopViewID == 3)
		{
			CSpliteFrame *pSpliteFrame = (CSpliteFrame *)m_ViewPoint[3];
			((CHisGridView *)(pSpliteFrame->GetHisGridViewPoint()))->UpdateView();
		}
	}
	else if (cmd == 1)	// �������ݸ���
	{
		GROSS_FRAME_INFO gross_frame_buffer = *((LPGROSS_FRAME_INFO)(para));

		// ��δ���Ĳ������ݱ�Ϊ���ϴ�
		pDoc->SetGrossUpService(gross_frame_buffer.TunnelKilnId, gross_frame_buffer.GrossType,
							    gross_frame_buffer.Year, gross_frame_buffer.Month, gross_frame_buffer.Day,
								gross_frame_buffer.Hour, gross_frame_buffer.Min, gross_frame_buffer.Sec);
	}
	else if (cmd == 2)	// ������ݸ���
	{
		BYTE *p_buff;

		p_buff = (BYTE *)(para);

		iKilnIdx = p_buff[0];
		if (iKilnIdx >= pDoc->m_iKilnNum)
		{
			return 0;
		}

		int  BufIdx = p_buff[1];
		if (BufIdx >= pDoc->m_pKilnBuff[iKilnIdx].MotorNum)
		{
			return 0;
		}

		WORD OutVal = ((WORD)(p_buff[2]) << 8) | (WORD)(p_buff[3]);
		if (OutVal > 500)
		{
			OutVal = 500;
		}
		else if (OutVal <= 0)
		{
			OutVal = 0;
		}

		int  DevIdx  = pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[BufIdx].TemOutIdx;
		WORD RegAddr = pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[BufIdx].BufOutIdx + 40001;
		m_pCom->SetWriteTerminalInfo(DevIdx, RegAddr, 1, &OutVal);
	}
	else if (cmd == 3)	// ��ͣ���ݸ���
	{
		if ((int)(para) <= 0)
		{
			pDoc->DevPowerOff();
		}
		else
		{
			pDoc->DevPowerOn();
		}
	}
	else if (cmd == 4)	// �������ݸ���
	{
		OnUpdateDTUParaModify((BYTE *)(para));
	}

	return 0;
}

// ��������������Կز�������
void CMainFrame::OnUpdateDTUParaModify(BYTE *p_buff)
{
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	
	CtrlObject ctrl_info;

	int iKilnIdx = p_buff[0];

	// ����Ҫ���[��Ƶ������]��[��Ƶ������]ֵ�Żᱻ����, ��������ֵ������ڵ�������ֵ
	ctrl_info.UpLimitOut = (float)(((WORD)(p_buff[1]) << 8) | (WORD)(p_buff[2])) / 10.0;
	ctrl_info.DnLimitOut = (float)(((WORD)(p_buff[3]) << 8) | (WORD)(p_buff[4])) / 10.0;
	if (((ctrl_info.UpLimitOut >= 0.0) && (ctrl_info.UpLimitOut <= 50.0)) 
		&& ((ctrl_info.DnLimitOut >= 0.0) && (ctrl_info.DnLimitOut <= 50.0))
		&& (ctrl_info.UpLimitOut >= ctrl_info.DnLimitOut))
	{
		pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].UpLimitOut = ctrl_info.UpLimitOut;
		pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].DnLimitOut = ctrl_info.DnLimitOut;
	}

	ctrl_info.iFrontPoint = ((WORD)(p_buff[5]) << 8) | (WORD)(p_buff[6]);
	if ((ctrl_info.iFrontPoint >= 1) && (ctrl_info.iFrontPoint <= pDoc->m_pKilnBuff[iKilnIdx].TemperatureNum))
	{
		pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iFrontPoint = ctrl_info.iFrontPoint;
	}

	ctrl_info.iRearPoint  = ((WORD)(p_buff[7]) << 8) | (WORD)(p_buff[8]);
	if ((ctrl_info.iRearPoint >= 1) && (ctrl_info.iRearPoint <= pDoc->m_pKilnBuff[iKilnIdx].TemperatureNum))
	{
		pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iRearPoint = ctrl_info.iRearPoint;
	}

	ctrl_info.IncOutCoeff = (float)(((WORD)(p_buff[9]) << 8) | (WORD)(p_buff[10])) / 10.0;
	if ((ctrl_info.IncOutCoeff >= 0.0) && (ctrl_info.IncOutCoeff <= 50.0))
	{
		pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].IncOutCoeff = ctrl_info.IncOutCoeff;
	}

	ctrl_info.iCtrlTime = ((WORD)(p_buff[11]) << 8) | (WORD)(p_buff[12]);
	if ((ctrl_info.iCtrlTime >= 1) && (ctrl_info.iCtrlTime <= 30))
	{
		pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iCtrlTime = ctrl_info.iCtrlTime;
	}

	ctrl_info.iGrossWaitTime = ((WORD)(p_buff[13]) << 8) | (WORD)(p_buff[14]);
	if ((ctrl_info.iGrossWaitTime >= 5) && (ctrl_info.iGrossWaitTime <= 30))
	{
		pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iGrossWaitTime = ctrl_info.iGrossWaitTime;
	}

	ctrl_info.iGrossProcTime = ((WORD)(p_buff[15]) << 8) | (WORD)(p_buff[16]);
	if ((ctrl_info.iGrossProcTime >= 2) && (ctrl_info.iGrossProcTime <= 1440))
	{
		// �����ǰֵ������ֵ����ͬ�����̸��¿�������TIMER
		if (ctrl_info.iGrossProcTime != pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iGrossProcTime)
		{
			ctrl_info.iProcTimerCnt = ctrl_info.iGrossProcTime * 60;
		}

		pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iGrossProcTime = ctrl_info.iGrossProcTime;
	}

	// �������ݿ�����
	pDoc->SaveAutoCtrlInfoRecord(iKilnIdx);

	// ����Ƴ����ʱ������
	int GrossProcMaxTime = ((WORD)(p_buff[17]) << 8) | (WORD)(p_buff[18]);
	if ((GrossProcMaxTime < 2) && (GrossProcMaxTime > 1440))
	{
		return;
	}

	// �¶����ޱ���ֵ����, ���Ϸ����˳�
	int UpWarnVal = ((WORD)(p_buff[19]) << 8) | (WORD)(p_buff[20]);
	if ((UpWarnVal < 500) && (UpWarnVal > 1370))
	{
		return;
	}

	// �¶����ޱ���ֵ����, ���Ϸ����˳�
	int DnWarnVal = ((WORD)(p_buff[21]) << 8) | (WORD)(p_buff[22]);
	if ((DnWarnVal < 0) && (DnWarnVal > 1000))
	{
		return;
	}
	
	CString strDefaultPath, str_ini;
	char file_path[MAX_PATH];
	GetModuleFileName(NULL, file_path, MAX_PATH);
	strDefaultPath = file_path;
	int nTemp = strDefaultPath.ReverseFind('\\');
	strDefaultPath = strDefaultPath.Left(nTemp);
	str_ini.Format("%s\\setting.ini", strDefaultPath);
	
	CString str;
	// �������Ⱦ͸���INI�ļ�
	if (GrossProcMaxTime != pDoc->m_iGrossProcMaxTime[iKilnIdx])
	{	
		str.Empty();
		str.Format("%d", GrossProcMaxTime);
		if (iKilnIdx == 0)
		{
			WritePrivateProfileString("GrossProcMaxTime", "������ʱ��1", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iGrossProcMaxTime[iKilnIdx] = GetPrivateProfileInt("GrossProcMaxTime", "������ʱ��1", 150, str_ini.GetBuffer(MAX_PATH));
		}
		else if (iKilnIdx == 1)
		{
			WritePrivateProfileString("GrossProcMaxTime", "������ʱ��2", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iGrossProcMaxTime[iKilnIdx] = GetPrivateProfileInt("GrossProcMaxTime", "������ʱ��2", 150, str_ini.GetBuffer(MAX_PATH));
		}
		else if (iKilnIdx == 2)
		{
			WritePrivateProfileString("GrossProcMaxTime", "������ʱ��3", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iGrossProcMaxTime[iKilnIdx] = GetPrivateProfileInt("GrossProcMaxTime", "������ʱ��3", 150, str_ini.GetBuffer(MAX_PATH));
		}
		else
		{
			WritePrivateProfileString("GrossProcMaxTime", "������ʱ��4", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iGrossProcMaxTime[iKilnIdx] = GetPrivateProfileInt("GrossProcMaxTime", "������ʱ��4", 150, str_ini.GetBuffer(MAX_PATH));
		}
	}
	
	// �������Ⱦ͸���INI�ļ�
	if (UpWarnVal != pDoc->m_iUpWarn[iKilnIdx])
	{	
		str.Empty();
		str.Format("%d", UpWarnVal);
		if (iKilnIdx == 0)
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������1", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iUpWarn[iKilnIdx] = GetPrivateProfileInt("Warn", "�¶ȱ�������1", 1000, str_ini.GetBuffer(MAX_PATH));
		}
		else if (iKilnIdx == 1)
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������2", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iUpWarn[iKilnIdx] = GetPrivateProfileInt("Warn", "�¶ȱ�������2", 1000, str_ini.GetBuffer(MAX_PATH));
		}
		else if (iKilnIdx == 2)
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������3", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iUpWarn[iKilnIdx] = GetPrivateProfileInt("Warn", "�¶ȱ�������3", 1000, str_ini.GetBuffer(MAX_PATH));
		}
		else
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������4", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iUpWarn[iKilnIdx] = GetPrivateProfileInt("Warn", "�¶ȱ�������4", 1000, str_ini.GetBuffer(MAX_PATH));
		}
	}
	
	// �������Ⱦ͸���INI�ļ�
	if (DnWarnVal != pDoc->m_iDnWarn[iKilnIdx])
	{	
		str.Empty();
		str.Format("%d", DnWarnVal);
		if (iKilnIdx == 0)
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������1", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iDnWarn[iKilnIdx] = GetPrivateProfileInt("Warn", "�¶ȱ�������1", 800, str_ini.GetBuffer(MAX_PATH));
		}
		else if (iKilnIdx == 1)
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������2", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iDnWarn[iKilnIdx] = GetPrivateProfileInt("Warn", "�¶ȱ�������2", 800, str_ini.GetBuffer(MAX_PATH));
		}
		else if (iKilnIdx == 2)
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������3", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iDnWarn[iKilnIdx] = GetPrivateProfileInt("Warn", "�¶ȱ�������3", 800, str_ini.GetBuffer(MAX_PATH));
		}
		else
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������4", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iDnWarn[iKilnIdx] = GetPrivateProfileInt("Warn", "�¶ȱ�������4", 800, str_ini.GetBuffer(MAX_PATH));
		}
	}
}

void CMainFrame::OnAdminSet()
{
	CLoginDialog LoginDlg;

	int m_LoginPwr = -1;
	while (m_LoginPwr == -1)
	{
		m_LoginPwr = LoginDlg.DoModal();
	}

	// ģʽδ�ı�, �����в˵��͹��������ػ�
	if ((m_bIsUserMode == TRUE && m_LoginPwr == 0) || (m_bIsUserMode == FALSE && m_LoginPwr != 0))
	{
		m_bIsUserMode = FALSE;
		CreateMyMenu(m_LoginPwr);
	}

	RecalcLayout();
}

void CMainFrame::OnOutputDisp()
{
	SetTopView(5);
	CGrossView *pGrossView = (CGrossView *)m_ViewPoint[5];
	pGrossView->SetGrossActiveView(m_iChangeGrossView);

	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	m_iChangeViewID = 0;
	m_iChangeGrossView = (m_iChangeGrossView + 1) % pDoc->m_iGrossTotalNum;
}

void CMainFrame::OnConfigView()
{
	SetTopView(6);
}

// ���ݿⱸ��
void CMainFrame::OnDataBaseBak()
{
// 	// ��ȡ�ĵ���ز���
// 	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
// 	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
// 
// 	CString sql, para1, para2, para3, para4;
// 	
// 	sql   = "";
// 	para1 = "copy ";
// 	para2 = pDoc->m_strDBPath;
// 	para3 = pDoc->m_strDBBakPath;
// 	para4 = ".db";
// 	sql.Format("%s %s %s%s", para1, para2, para3, para4);
// 	system(sql.GetBuffer(MAX_PATH));
// 
// 	sql   = "";
// 	para1 = pDoc->m_strShellPath;
// 	para2 = pDoc->m_strDBBakPath;
// 	para3 = ".db .dump > ";
// 	para4 = pDoc->m_strDBBakPath;
// 	sql.Format("%s %s%s %s", para1, para2, para3, para4);
//  	system(sql.GetBuffer(MAX_PATH));
// 
// 	sql   = "";
// 	para1 = "del ";
// 	para2 = pDoc->m_strDBBakPath;
// 	para3 = ".db";
// 	sql.Format("%s %s%s", para1, para2, para3);
//  	system(sql.GetBuffer(MAX_PATH));

	system("bak.bat");
}

void CMainFrame::OnAutoCtrl()
{
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	BOOL bCtrlKiln = FALSE;
	for (int i = 0; i < pDoc->m_iKilnNum; i++)
	{
		if (pDoc->m_pKilnBuff[i].ControlNum > 0)
		{
			bCtrlKiln = TRUE;
			break;
		}
	}

	if (bCtrlKiln == FALSE)
	{
		MessageBox("��ǰû�������Զ����ƹ���", "��ʾ");
		return;
	}

	CAutoCtrlDlg dlg;
	int res = -1;

	while (res == -1)
	{
		res = dlg.DoModal();
	}

// 	if (m_bIsAutoCtrl == FALSE)
// 	{
// 		m_bIsAutoCtrl = TRUE;
// 	}
// 	else
// 	{
// 		m_bIsAutoCtrl = FALSE;
// 	}
// 
// 	// �ػ���в˵��͹�����
// 	if (m_bIsUserMode == FALSE)
// 	{
// 		CreateMyMenu(0);
// 	}
// 	else
// 	{
// 		CreateMyMenu(1);
// 	}
// 
// 	SetFrameCaption(NULL);
// 
// 	RecalcLayout();
}

// �����ʼ������
void CMainFrame::OnOpenMainFace()
{
	SetTopView(0);
	m_iChangeViewID      = 0;
	m_iChangeGrossView   = 0;
	m_iChangeAdjustView  = 0;

	CIControlView *pControlView = (CIControlView *)m_ViewPoint[0];
	pControlView->SetActiveView();
}

// ѭ����ʾ����ͼ
void CMainFrame::OnChangeView()
{
	OnOpenFurnaceFace(m_iChangeViewID + IDM_FURNACE_1);

	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	m_iChangeGrossView = 0;
	m_iChangeViewID = (m_iChangeViewID + 1) % (pDoc->m_iKilnNum * THE_SUB_VIEW_COUNT);	
}

// У����༭
void CMainFrame::OnAdjustEdit()
{
	SetTopView(7);
	CAdjustView *pAdjustView = (CAdjustView *)m_ViewPoint[7];
	pAdjustView->SetAdjustActiveView(m_iChangeAdjustView);

	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	m_iChangeAdjustView = (m_iChangeAdjustView + 1) % pDoc->m_iKilnNum;
}

// ��̬�˵���Ӧ����
void CMainFrame::OnOpenFurnaceFace(UINT nID)
{
	CRealTimeView *pRealTimeView = (CRealTimeView *)m_ViewPoint[1];
	CCurCurveView *pCurCurveView = (CCurCurveView *)m_ViewPoint[2];
	CSpliteFrame *pSpliteFrame   = (CSpliteFrame *)m_ViewPoint[3];
	CStandardView *pStandardView = (CStandardView *)m_ViewPoint[4];

	m_iChangeViewID = nID - IDM_FURNACE_1;		// ���¶���ͼ��ʾָ��

	int num = (nID - IDM_FURNACE_1) / THE_SUB_VIEW_COUNT;
	int face = (nID - IDM_FURNACE_1) % THE_SUB_VIEW_COUNT;

	SetTopView(face + 1);

	switch (face)
	{
	case 0:
		pRealTimeView->ActiveView(num);	 
		pRealTimeView->UpdateView();
		break;

	case 1:
		pCurCurveView->ActiveView(num);
		pCurCurveView->UpdateView();
		break;

	case 2:
 		((CHisGridView *)(pSpliteFrame->GetHisGridViewPoint()))->ActiveView(num);
		break;

	case 3:
		pStandardView->ActiveView(num);
		break;

	default:
		break;
	}
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	if (pDoc->m_bDBOpen == FALSE)		// ������ݿ�򿪺����ݳ�ʼ��Ϊ�ɹ�
	{
		PostQuitMessage(1);
		return;
	}
	
	// ����DLL���ڴ����, ����ʼ�����
	m_pCom = new CiComAPI(this,
						  pDoc->m_ComCfg.iPortNum, 
						  pDoc->m_ComCfg.cParity, 
						  pDoc->m_ComCfg.iBaudrate, 
						  pDoc->m_ComCfg.iDataBits, 
						  pDoc->m_ComCfg.iStopBits, 
						  pDoc->m_ComCfg.iTimeout, 
						  pDoc->m_ComCfg.iScanTime,
						  pDoc->m_TerminalCfg.iTerminalNum,

						  pDoc->m_ComCfg.iDtuPortNum, 
						  pDoc->m_ComCfg.cParity, 
						  pDoc->m_ComCfg.iDtuBaudrate, 
						  pDoc->m_ComCfg.iDataBits, 
						  pDoc->m_ComCfg.iStopBits,
						  5000,								// ��¼���ʱ��
						  5000,								// ���ڴ��ͼ��ʱ��
						  10000,							// ͨѶ��ʱʱ��
						  pDoc->m_ComCfg.lDtuPort,			// DTU�˿�ID
						  pDoc->m_iKilnNum);				// Ҥ����

	int  i;
	BOOL res;

	for (i = 0; i < pDoc->m_TerminalCfg.iTerminalNum; i++)
	{
		res = m_pCom->ImportDevInfo(i, 
									pDoc->m_TerminalCfg.m_pTerminalObjectBuff[i].DevId, 
									pDoc->m_TerminalCfg.m_pTerminalObjectBuff[i].DevCmd,
									pDoc->m_TerminalCfg.m_pTerminalObjectBuff[i].DevChn,
									pDoc->m_TerminalCfg.m_pTerminalObjectBuff[i].DevAddr,
									pDoc->m_TerminalCfg.m_pTerminalObjectBuff[i].DevType);

		if (res == FALSE)
		{
			MessageBox("Communication Create failed",  "Warning", MB_OK);
			m_strStatus.Format("Com Failed.....");
		}
	}

	if (res == TRUE)
	{
		if (m_pCom->Create(this, DLL_COMAPI_ID, WS_CHILD) == FALSE)
		{
			MessageBox("Communication Create failed",  "Warning", MB_OK);
			m_strStatus.Format("Com Failed.....");
		}
		else
		{
			m_strStatus.Format("[DTU:Com%d, %d, %c, %d, %d, %d]  [Serial:Com%d, %d, %c, %d, %d] [Terminals: %d]",																														
																			pDoc->m_ComCfg.iDtuPortNum,
																			pDoc->m_ComCfg.iDtuBaudrate,
																			pDoc->m_ComCfg.cParity, 
																			pDoc->m_ComCfg.iDataBits, 
																			pDoc->m_ComCfg.iStopBits,
																			pDoc->m_ComCfg.lDtuPort,

																			pDoc->m_ComCfg.iPortNum,		
																			pDoc->m_ComCfg.iBaudrate, 
																			pDoc->m_ComCfg.cParity, 
																			pDoc->m_ComCfg.iDataBits, 
																			pDoc->m_ComCfg.iStopBits, 
																			pDoc->m_TerminalCfg.iTerminalNum);		
		}
	}

	pDoc->GetGrossNotUpService();

	CreateMyMenu(1);			// ��ʼ����̬�˵�
	
// 	// ʹ������ʾ�ڶ���
// 	SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
// 
// 	// ��ô��ڽ���
// 	//�����ǵ�Ϊ���㴰��
// 	AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(), NULL), GetCurrentThreadId(), TRUE);  
// 	SetForegroundWindow();
// 	SetFocus();   
// 	//�ͷ�thread
// 	AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(), NULL), GetCurrentThreadId(), FALSE);

	CFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::SetFrameCaption(LPCTSTR lpszString)
{
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc = (CIControlDoc *)pMainFrame->GetActiveDocument();

	if (lpszString == NULL)
	{
		return;
	}

	CString str;
	str.Format("���Ҥ���ܼ��ϵͳ %s --- %s", pDoc->m_strSalePhone, lpszString);
	SetWindowText(str);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	static int cnt = 0, cnt1 = 0, cnt3 = 0;
	static int change_id = 0;

	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		// ��ȡ�ĵ���ز���
		CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
		CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

		if((pMsg->wParam >= VK_F1) && (pMsg->wParam <= VK_F9))
		{
			change_id = (pMsg->wParam - VK_F1) * THE_SUB_VIEW_COUNT;
			if (change_id < (pDoc->m_iKilnNum * THE_SUB_VIEW_COUNT))
			{
				m_iChangeViewID = change_id;
				OnChangeView();
			}

			return TRUE;
		}

		else if (pMsg->wParam == VK_F10)
		{
			CString str;
			if (pDoc->AutoSetStandardRecord(change_id / THE_SUB_VIEW_COUNT) == TRUE)
			{
				str.Format("%d# Standard Curve Configuration Success", change_id + 1);
				MessageBox(str, "Standard Curve Configuration", MB_OK);
			}
			else
			{
				str.Format("%d# Standard Curve Configuration Failed", change_id + 1);
				MessageBox(str, "Standard Curve Configuration", MB_OK);
			}

			return TRUE;
		}

		else if ((pMsg->wParam == VK_F11)  && (m_iTopViewID == 5))
		{
			cnt3++;
			if ((cnt3 % 2) == 0)
			{
				CGrossView *pGrossView = (CGrossView *)m_ViewPoint[5];
				
				if (pGrossView->m_bDayDisp == FALSE)
				{
					pGrossView->m_bDayDisp = TRUE;
				}
				else
				{
					pGrossView->m_bDayDisp = FALSE;
				}
				
				pGrossView->SetGrossActiveView(m_iChangeGrossView);
			}
			return TRUE;
		}

		else if((pMsg->wParam == VK_SPACE) && (m_iTopViewID <= 4))
		{			
			cnt++;
			if ((cnt % 2) == 0)
			{
				cnt1 = (cnt1 + 1) % THE_SUB_VIEW_COUNT;
				if (cnt1 == 0)
				{
					m_iChangeViewID = change_id;
				}

				OnChangeView();
				return TRUE;
			}
		}
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnAppAbout() 
{
	int res;
	CRegisterDlg dlg;

	dlg.m_bIsModifyResgister = TRUE;

	while (1)
	{
		res = dlg.DoModal();
		if (res == IDOK)
		{
			// ��֤�����Ƿ���ȷ
			if (dlg.m_bIsRegister == TRUE)
			{
				MessageBox("Register Success");
				break;
			}
			else
			{
				MessageBox("Register Failed");
			}
		}
		else	// ��ȡ��ֱ���˳�
		{
			return;
		}
	}
}

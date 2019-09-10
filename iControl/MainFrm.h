// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__EBBE1CDC_56E4_4D86_8B55_30D37C3C9A92__INCLUDED_)
#define AFX_MAINFRM_H__EBBE1CDC_56E4_4D86_8B55_30D37C3C9A92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>
using namespace std;

#define MAX_ACTIVE_VIEWS		4					// ������ͼ����

#define MAX_REALTIME_TIMEOUT	6					// ʵʱ���ݸ��³�ʱ�������ڴ�ʱ����û�и���ʵʱ���ݣ����̱任Ϊ�����
													// ��λ500ms

#include "SQLiteTbl.h"
#include "HisCurveView.h"
#include "HisGridView.h"
#include "SpliteFrame.h"
#include "SerialDlg.h"
#include "LoginDialog.h"
#include "MotorCtrlDlg.h"
#include "../iCom/iComAPI.h"
#include "GrossInfo.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	BOOL m_bIsUserMode;				// ��ʾ��ǰ�Ĳ���ģʽ: �û�ģʽ�����Աģʽ
	BOOL m_bIsAutoCtrl;				// ��ʾ��ǰ�Ƿ�Ϊ�Զ�����ģʽ
	int m_iTopViewID;				// ��ǰ��ʾ����ͼ���
	int m_iChangeViewID;			// ѭ����ʾ����ͼָ��
	int m_iChangeGrossView;			// ѭ����ʾ������Ҥ�Ĳ�����
	int m_iChangeAdjustView;		// ѭ����ʾ������Ҥ��У����
	vector<CView *> m_ViewPoint;	// ��ͼ����ָ��

	CMenu m_MainMenu;

	ULONG_PTR m_gdiplusToken;

	CString m_strStatus;

	CiComAPI *m_pCom;

	BOOL      m_bSave;
	CTime     m_tLastSaveTime;

	int       m_iRealTimeTimeout;

	int       m_iAccessDays;
	int       m_iWatchDogTimerCnt;

	BOOL      m_bTimerReady;

// Operations
public:
	BOOL CreateUserAutoToolBar(void);
	BOOL CreateUserUnAutoToolBar(void);
	BOOL CreateAdminAutoToolBar(void);
	BOOL CreateAdminUnAutoToolBar(void);
	void CreateMyMenu(int iIndex);
	void CreateAdminMenu();
	void CreateGuestMenu();
	int AddView(CCreateContext* pContext);
	void SetTopView(int iTopViewID);
	void SetFrameCaption(LPCTSTR lpszString);
	void CreateGrossInfo(CString str_info);
	void OnUpdateDTUParaModify(BYTE *p_buff);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	CGrossInfo* m_pGrossInfo;
	int GrossInfoTimerCnt;
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar m_wndStatusBar;
 	CToolBar  m_wndAdminAutoStatusBar;
	CToolBar  m_wndAdminUnAutoStatusBar;
	CToolBar  m_wndUserAutoStatusBar;
	CToolBar  m_wndUserUnAutoStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnConfigView();
	afx_msg void OnChangeView();
	afx_msg void OnMotorCtrl();
	afx_msg void OnSerialSetting();
	afx_msg void OnOutputDisp();		
	afx_msg void OnDataBaseBak();
	afx_msg void OnAutoCtrl();
	afx_msg void OnAdminSet();
	afx_msg void OnOpenMainFace();	
	afx_msg void OnAdjustEdit();
	afx_msg LONG OnRXTerminal(WPARAM cmd, LPARAM para);
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	afx_msg void OnOpenFurnaceFace(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__EBBE1CDC_56E4_4D86_8B55_30D37C3C9A92__INCLUDED_)

#if !defined(AFX_SETUPVIEW_H__3BBEC097_515B_4F98_83C2_7236034C89A1__INCLUDED_)
#define AFX_SETUPVIEW_H__3BBEC097_515B_4F98_83C2_7236034C89A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupView.h : header file
//
#include "sqlite_inc/sqlite3.h"
#include "IGridCtrl.h"

#include <vector>
#include <map>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CSetupView view
typedef struct tagGV_KILNINFO
{
	int KilnType;
	int iTempNum;
	int iTempANum;
	int iPressNum;
	int iHumidityNum;
	int iMotorNum;
	int iGrossNum;
	int iCtrlNum;
	int iTotalCars;
	int iStandardIndex;
} KILN_INFO, *LPKILN_INFO;

typedef struct tagGV_TERMINFO 
{
	int iTemId;
	int iTemCmd;
	int iTemChn;
	int iTemAddr;
	int iTemType;
} TERM_INFO, *LPTERM_INFO;

typedef struct tagGV_CHANNELINFO 
{
	int iKilnIdx;
	int iTemIdx;
	int iPhyIdx;
	int iChnTyp;
	int iCarIdx;
	char strName[51];
	double AdjGain;
	double AdjZero;
} CHANNEL_INFO, *LPCHANNEL_INFO;

class CSetupView : public CView
{
protected:
	CSetupView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSetupView)

// Attributes
public:
	CStdioFile m_hFile;
	BOOL       m_bFileOpen;

	CDC        m_MemDcDraw;
	HBITMAP    m_HbtMap;
	CRect      m_MaxClientRect;

	int         m_iSelectRow;
	int         m_iViewIndex;
	CIGridCtrl *m_pGrid;
	LOGFONT     m_lfFixCellFont;
	LOGFONT     m_lfCellFont;

	CButton	   *m_pSetButton;
	CButton    *m_pDelButton;
	CButton	   *m_pNextButton;

	vector<KILN_INFO>    m_KilnInfoMap;				// 砖窑配置信息队列
	vector<TERM_INFO>    m_TerminalInfoMap;			// 终端配置信息队列
	vector<CHANNEL_INFO> m_ChannelInfoMap;			// 通道配置信息队列

// Operations
public:
	void SetKilnInfoCellStyle(int iRow);
	void SetKilnInfoComboStyle(int iRow);
	void SetKilnInfoGridView(int iRowNum, int iColNum);

	void SetTemInfoCellStyle(int iRow);
	void SetTemInfoComboStyle(int iRow);
	void SetTemInfoGridView(int iRowNum, int iColNum);

	void SetChannelInfoCellStyle(void);
	void SetChannelInfoComboStyle(int iRow, BOOL only_fresh_phy);
	void SetChannelInfoGridView(int iRowNum, int iColNum);

	void SetGridView(int iRowNum, int iColNum);
	void SetGridItem(GV_DISPINFO *pDispInfo);
	void ModifyGridItem(GV_DISPINFO *pDispInfo);
	void AppendKilnInfoGridView(void);
	void DeleteKilnInfoGridView(void);
	void AppendTerminalInfoGridView(void);
	void DeleteTerminalInfoGridView(void);

	void OpenFile(const char *pFileName);
	void CloseFile(void);
	void WriteCfgHeader(void);
	void WriteCfgLast(void);
	void CreateLoginTable(void);
	void CreateComCfgTable(void);
	void CreateAutoCtrlCfgTable(void);
	void CreateTerminalCfgTable(void);
	void CreateKilnInfoCfgTable(void);
	void CreateStandardTable(void);
	void CreateHistoryTable(void);
	void CreateChannelCfgTable(void);
	void CreateOutputsTable(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSetupView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSetupView)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnAppendCfgRecord();
	afx_msg void OnDeleteCfgRecord();
	afx_msg void OnNextCfgRecord();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPVIEW_H__3BBEC097_515B_4F98_83C2_7236034C89A1__INCLUDED_)

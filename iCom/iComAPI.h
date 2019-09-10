// iComAPI.h: interface for the CiComAPI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICOMAPI_H__BAA63A3B_A495_4AEF_BE80_D9EE6DFC7545__INCLUDED_)
#define AFX_ICOMAPI_H__BAA63A3B_A495_4AEF_BE80_D9EE6DFC7545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _ICOM_EXPORT_
#include "iCom.h"
#include "ModBus.h"
#include "DtuBus.h"

#define WM_RX_TERMINAL			WM_USER + 102		// 自定义的更新终端数据消息

#include <vector>
#include <map>
using namespace std;

#define ICOMAPI_CLASSNAME	_T("MFCiComAPI")

class ICOM_API CiComAPI  : public CWnd
{
public:
	CiComAPI();
	CiComAPI(CWnd *pOwner,
			 int iComPort, char cPrity, int iBaudrate, int iDataBits, int iStopBits, int iTimeout, int iScanTime, int DevNum,
		     int iDtuPort, char cDtuPrity, int iDtuBaudrate, int iDtuDataBits, int iDtuStopBits, DWORD lLoginInternal, DWORD lCycleInternal, DWORD lDtuTimeout, DWORD lLocalID,
			 int iKilnNum);
	virtual ~CiComAPI();

	BOOL RegisterWindowClass();
	BOOL ImportDevInfo(int TemIdx, BYTE TemId, BYTE TemCmd, BYTE TemChn, WORD TemAddr, BYTE TemTyp);
	BOOL Create(CWnd* parent, UINT nID, DWORD dwStyle = WS_CHILD);
	void SetWriteTerminalInfo(int iTemIdx, WORD iTemWriteAddr, WORD iTemWriteRegs, WORD *pTemRegBuff);
	void DestroyWindows();

	BOOL GetDtuLoginStatus(void);
	BOOL GetDtuEchoStatus(void);
	BOOL SetRealTimeTransmit(BYTE TunnelKilnId,
							 BYTE TempNum, WORD *p_TempBuff,				// 每条窑的最大正温度点为64个
							 BYTE SideTempNum, WORD *p_SideTempBuff,		// 每条窑的最大边温度点为32个
							 BYTE PressNum, WORD *p_PressBuff,				// 每条窑的最大压力点为16个
							 BYTE HumidityNum, WORD *p_HumidityBuff,		// 每条窑的最大压力点为8个
							 BYTE MotorNum, WORD *p_MotorBuff,				// 每条窑的最大电机点为8个
							 WORD Year, BYTE Month, BYTE Day, BYTE Hour, BYTE Min, BYTE Sec);
	BOOL SetGrossTransmit(BYTE TunnelKilnId, BYTE GrossType, WORD Year, BYTE Month, BYTE Day, BYTE Hour, BYTE Min, BYTE Sec);

public:
	CWnd    *m_pParentWnd;
	CWnd    *m_pOwner;
	CModBus *m_pModBus;
	CDtuBus *m_pDtuBus;

	int      m_iComPort;
	int      m_iBaudrate;
	int      m_iDataBits;
	int      m_iStopBits;
	char     m_cPrity;
	int      m_iTimeout;
	int      m_iScanTime;

	int m_iDtuPort;
	char m_cDtuPrity; 
	int m_iDtuBaudrate; 
	int m_iDtuDataBits; 
	int m_iDtuStopBits; 
	DWORD m_lLoginInternal; 
	DWORD m_lCycleInternal;
	DWORD m_lDtuTimeout;
	DWORD m_lLocalID;

	int m_iKilnNum;

	BYTE		m_DevNum;
	LPTemObject m_pTemObjectBuff;

	REALTIME_FRAME_INFO RealTimeFrameBuffer[10];		// 建立最大10条窑的实时数据空间
//	GROSS_FRAME_INFO    GrossFrameBuffer[10];			// 建立最大10条窑的产量数据空间
	vector<GROSS_FRAME_INFO> GrossFrameBuffer;			// 产量数据队列定义

	BOOL IsLockDtuTransmint;

protected:
	static UINT	ManageThread(LPVOID pParam);

	CWinThread*			m_ManageThread;					// 管理线程指针

	// 线程异步变量
	CRITICAL_SECTION	m_csCommunicationSync;			// 临界区变量
	BOOL				m_bManageThreadAlive;			// 指示管理线程是否建立

	// 线程句柄
	HANDLE	m_hManageShutdownEvent;						// 关闭管理线程句柄, 用于关闭管理线程使用
	HANDLE	m_hManageEvent;								// 管理线程句柄
	
	HANDLE	m_hManageEventArray[2];						// 管理线程句柄数组. 分别对应2个线程句柄

protected:
	//{{AFX_MSG(CModBus)
	// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg LONG OnUpdateTerminal(WPARAM index, LPARAM len);
	afx_msg LONG OnDtuTransmitEcho(WPARAM cmd, LPARAM res);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_ICOMAPI_H__BAA63A3B_A495_4AEF_BE80_D9EE6DFC7545__INCLUDED_)

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

#define WM_RX_TERMINAL			WM_USER + 102		// �Զ���ĸ����ն�������Ϣ

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
							 BYTE TempNum, WORD *p_TempBuff,				// ÿ��Ҥ��������¶ȵ�Ϊ64��
							 BYTE SideTempNum, WORD *p_SideTempBuff,		// ÿ��Ҥ�������¶ȵ�Ϊ32��
							 BYTE PressNum, WORD *p_PressBuff,				// ÿ��Ҥ�����ѹ����Ϊ16��
							 BYTE HumidityNum, WORD *p_HumidityBuff,		// ÿ��Ҥ�����ѹ����Ϊ8��
							 BYTE MotorNum, WORD *p_MotorBuff,				// ÿ��Ҥ���������Ϊ8��
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

	REALTIME_FRAME_INFO RealTimeFrameBuffer[10];		// �������10��Ҥ��ʵʱ���ݿռ�
//	GROSS_FRAME_INFO    GrossFrameBuffer[10];			// �������10��Ҥ�Ĳ������ݿռ�
	vector<GROSS_FRAME_INFO> GrossFrameBuffer;			// �������ݶ��ж���

	BOOL IsLockDtuTransmint;

protected:
	static UINT	ManageThread(LPVOID pParam);

	CWinThread*			m_ManageThread;					// �����߳�ָ��

	// �߳��첽����
	CRITICAL_SECTION	m_csCommunicationSync;			// �ٽ�������
	BOOL				m_bManageThreadAlive;			// ָʾ�����߳��Ƿ���

	// �߳̾��
	HANDLE	m_hManageShutdownEvent;						// �رչ����߳̾��, ���ڹرչ����߳�ʹ��
	HANDLE	m_hManageEvent;								// �����߳̾��
	
	HANDLE	m_hManageEventArray[2];						// �����߳̾������. �ֱ��Ӧ2���߳̾��

protected:
	//{{AFX_MSG(CModBus)
	// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg LONG OnUpdateTerminal(WPARAM index, LPARAM len);
	afx_msg LONG OnDtuTransmitEcho(WPARAM cmd, LPARAM res);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_ICOMAPI_H__BAA63A3B_A495_4AEF_BE80_D9EE6DFC7545__INCLUDED_)

#if !defined(AFX_DTUBUS_H__88FDC47F_EE19_424E_B0CC_F0501620CDA4__INCLUDED_)
#define AFX_DTUBUS_H__88FDC47F_EE19_424E_B0CC_F0501620CDA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DtuBus.h : header file
//

#include <vector>
using namespace std;

#define WM_DTU_TRANSMIT_ECHO	WM_USER + 103		// �Զ�����Ϣ, ���������ϴ����

#include "SerialPort.h"

#define THE_LOGIN_INTERNAL		5000			// ��¼���ʱ�䣨Ĭ��ֵ��
#define THE_CYCLE_INTERNAL		8000			// ������������ʱ�䣨Ĭ��ֵ��
#define THE_DTU_TIMEOUT			15000			// DTU�������ͨѶ��ʱʱ�䣨Ĭ��ֵ��

#define THE_CMD_LOGNIN			0x01			// ��¼����(������<�����ն�)
#define THE_CMD_GET_RTDAT		0x02			// ʵʱ�����ϴ�����(������<�����ն�)			
#define THE_CMD_GET_GROSS		0x03			// ���������ϴ�����(������<�����ն�)
#define THE_CMD_SET_DEAD		0x04			// ֹͣ�豸����(����������>�ն�)
#define THE_CMD_SET_LIVE		0x05			// �����豸����(����������>�ն�)
#define THE_CMD_SET_PARA		0x06			// ����������������(����������>�ն�)
#define THE_CMD_SET_ADJ			0x07			// У��������������(����������>�ն�)
#define THE_CMD_SET_MOTOR		0x08			// ��Ƶ����������(����������>�ն�)

#define THE_FRAME_HEAD			0x2323
#define THE_FRAME_REAR			0x2424

#define THE_MAX_RECV_BUFF		512
#define THE_MAX_RECALL_CNT		5				// ���Ի��ƴ���

typedef struct tagSerialPortPara
{
	int iPortNum;				// ���ں�
	int iBaudrate;				// ������
	char cParity;				// У�鷽ʽ
	int iDataBits;				// ����λ��
	int iStopBits;				// ֹͣλ��
	int iBufferSize;			// ��������С
	
	DWORD lLoginInternal;		// ��¼���ʱ��
	DWORD lCycleInternal;		// ������������ʱ��
	DWORD lDtuTimeout;			// DTU�������ͨѶ��ʱʱ��

	DWORD lLocalID;				// ����DTU��
} DTU_PORT_PARA, *LPDTU_PORT_PARA;

typedef struct tagDtuBusFrame
{
	WORD  FrameHead;
	WORD  FrameLen;
	BYTE  Cmd;
	WORD  FrameNum;
	DWORD SendPort;
	DWORD RecvPort;
	BYTE  FrameBuff[256];
	WORD  CRC;
	WORD  FrameRear;
} DTUBUS_FRAME_INFO, *LPDTUBUS_FRAME_INFO;

typedef struct tagRealTimeFrame
{
	BYTE TunnelKilnId; 
	BYTE TempNum;
	WORD TempBuff[64];				// ÿ��Ҥ��������¶ȵ�Ϊ64��
	BYTE SideTempNum;	
	WORD SideTempBuff[32];			// ÿ��Ҥ�������¶ȵ�Ϊ32��
	BYTE PressNum;
    WORD PressBuff[16];				// ÿ��Ҥ�����ѹ����Ϊ16��
	BYTE HumidityNum;
	WORD HumidityBuff[8];			// ÿ��Ҥ�����ѹ����Ϊ8��
	BYTE MotorNum;
	WORD MotorBuff[8];				// ÿ��Ҥ���������Ϊ8��
	WORD Year; 
	BYTE Month; 
	BYTE Day; 
	BYTE Hour; 
	BYTE Min; 
	BYTE Sec;
} REALTIME_FRAME_INFO, *LPREALTIME_FRAME_INFO;

typedef struct tagGrossFrame
{
	BYTE TunnelKilnId; 
	BYTE GrossType;
	WORD Year; 
	BYTE Month; 
	BYTE Day; 
	BYTE Hour; 
	BYTE Min; 
	BYTE Sec;
} GROSS_FRAME_INFO, *LPGROSS_FRAME_INFO;

typedef enum
{
	DTU_RECV_HEAD = 0x00,
	DTU_RECV_LEN,
	DTU_RECV_CMD,
	DTU_RECV_FNUM,
	DTU_RECV_SPORT,
	DTU_RECV_RPORT,
	DTU_RECV_DAT,
	DTU_RECV_CRC,
	DTU_RECV_REAR
} DTUBUS_RECV_PROC;

/////////////////////////////////////////////////////////////////////////////
// CDtuBus window

#define IDTU_CLASSNAME		_T("MFCiDtu")

class CDtuBus : public CWnd
{
// Construction
public:
	CDtuBus(CWnd *pOwner, int iPortNum, int iBaudrate, char cParity, int iDataBits, int iStopBits, int iBufferSize,
		    DWORD lLoginInternal, DWORD lCycleInternal,	DWORD lDtuTimeout, DWORD LocalID);

protected:
	static UINT	LoginThread(LPVOID pParam);
	static UINT	ComThread(LPVOID pParam);
	
protected:
	
	CWinThread*			m_LoginThread;				// ��¼�߳�ָ��
	CWinThread*			m_ComThread;				// ͨѶ�߳�ָ��
	
	// �߳��첽����
	CRITICAL_SECTION	m_csLoginCommunicationSync;	// �ٽ�������
	CRITICAL_SECTION	m_csComCommunicationSync;	// �ٽ�������
	BOOL				m_bLoginThreadAlive;		// ָʾ��¼�߳��Ƿ���
	BOOL				m_bComThreadAlive;			// ָʾͨѶ�߳��Ƿ���
	
	// �߳̾��
	HANDLE				m_hLoginShutdownEvent;		// �رյ�¼�߳̾��, ���ڹر��߳�ʹ��
	HANDLE				m_hComShutdownEvent;		// �ر�ͨѶ�߳̾��, ���ڹر��߳�ʹ��
	HANDLE				m_hLoginEvent;				// ��¼�߳̾��, ���ڵ�¼ͨѶʹ��
	HANDLE				m_hComEvent;				// ͨѶ�߳̾��, ����DTUͨѶʹ��
	
	HANDLE				m_hLoginEventArray[2];		// ��¼�߳̾������. �ֱ��Ӧ2���߳̾��
	HANDLE				m_hComEventArray[2];		// ͨѶ�߳̾������. �ֱ��Ӧ2���߳̾��

public:
	CWnd *m_pOwner;									// ����ָ��	
	
	CSerialPort   m_Port;							// ���ڶ���
	DTU_PORT_PARA m_PortPara;						// ���ڲ���
	BOOL  m_bOpen;									// �����Ƿ��
	BOOL  m_bRecv;									// �Ƿ��ڽ���״̬
	BOOL  m_bLogin;									// �Ƿ��¼
	BOOL  m_bEchoPositive;							// �Ƿ�����Ӧ

	REALTIME_FRAME_INFO DtuRealTimeFrame;			// ʵʱ���ݷ���֡�ṹ
	GROSS_FRAME_INFO    DtuGrossFrame;				// �������ݷ���֡�ṹ

	BYTE                DtuSendCmd;

	BYTE                DtuRecvHR[2];
	WORD                DtuRecvPoint;
	WORD                DtuRecvLen;
	BYTE                DtuRecvCmd;
	DWORD               DtuRecvFNum;
	DWORD               DtuSPortNum, DtuRPortNum;
	WORD                DtuCRC;
	BYTE                DtuRecvBuffer[THE_MAX_RECV_BUFF];
	DTUBUS_RECV_PROC    DtuRecvProc;

//	BYTE                DtuTempBuffer[1024];		// ������ʱ���ݷ���

public:
	BOOL RegisterWindowClass();
	BOOL Create(CWnd* pParentWnd, UINT nID, DWORD dwStyle);
	void DestroyWindows();

	void LoginRequest(void);
	void LoginTransmit(void);

	BOOL ComTransmit(BYTE cmd, void *p_buff);

	void ComRealtimeLoad(void);
	void ComGrossLoad(void);
	void ComEchoPositive(BYTE cmd, WORD frame_num);
	void ComEchoNegitive(BYTE cmd, WORD frame_num);

	WORD  WordSwitch(WORD Value);
	DWORD DWordSwitch(DWORD Value);
	unsigned int CRC16Make(unsigned char *pucFrame, unsigned int usLen);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDtuBus)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDtuBus();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDtuBus)
	afx_msg void OnDestroy();
	afx_msg LONG OnComm(WPARAM ch, LPARAM port);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DTUBUS_H__88FDC47F_EE19_424E_B0CC_F0501620CDA4__INCLUDED_)

#if !defined(AFX_DTUBUS_H__88FDC47F_EE19_424E_B0CC_F0501620CDA4__INCLUDED_)
#define AFX_DTUBUS_H__88FDC47F_EE19_424E_B0CC_F0501620CDA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DtuBus.h : header file
//

#include <vector>
using namespace std;

#define WM_DTU_TRANSMIT_ECHO	WM_USER + 103		// 自定义消息, 反馈数据上传结果

#include "SerialPort.h"

#define THE_LOGIN_INTERNAL		5000			// 登录间隔时间（默认值）
#define THE_CYCLE_INTERNAL		8000			// 周期性命令间隔时间（默认值）
#define THE_DTU_TIMEOUT			15000			// DTU与服务器通讯超时时间（默认值）

#define THE_CMD_LOGNIN			0x01			// 登录命令(服务器<——终端)
#define THE_CMD_GET_RTDAT		0x02			// 实时数据上传命令(服务器<——终端)			
#define THE_CMD_GET_GROSS		0x03			// 产量数据上传命令(服务器<——终端)
#define THE_CMD_SET_DEAD		0x04			// 停止设备命令(服务器——>终端)
#define THE_CMD_SET_LIVE		0x05			// 启动设备命令(服务器——>终端)
#define THE_CMD_SET_PARA		0x06			// 参数数据设置命令(服务器——>终端)
#define THE_CMD_SET_ADJ			0x07			// 校正数据设置命令(服务器——>终端)
#define THE_CMD_SET_MOTOR		0x08			// 变频器设置命令(服务器——>终端)

#define THE_FRAME_HEAD			0x2323
#define THE_FRAME_REAR			0x2424

#define THE_MAX_RECV_BUFF		512
#define THE_MAX_RECALL_CNT		5				// 重试机制次数

typedef struct tagSerialPortPara
{
	int iPortNum;				// 串口号
	int iBaudrate;				// 波特率
	char cParity;				// 校验方式
	int iDataBits;				// 数据位数
	int iStopBits;				// 停止位数
	int iBufferSize;			// 缓冲区大小
	
	DWORD lLoginInternal;		// 登录间隔时间
	DWORD lCycleInternal;		// 周期性命令间隔时间
	DWORD lDtuTimeout;			// DTU与服务器通讯超时时间

	DWORD lLocalID;				// 本地DTU号
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
	WORD TempBuff[64];				// 每条窑的最大正温度点为64个
	BYTE SideTempNum;	
	WORD SideTempBuff[32];			// 每条窑的最大边温度点为32个
	BYTE PressNum;
    WORD PressBuff[16];				// 每条窑的最大压力点为16个
	BYTE HumidityNum;
	WORD HumidityBuff[8];			// 每条窑的最大压力点为8个
	BYTE MotorNum;
	WORD MotorBuff[8];				// 每条窑的最大电机点为8个
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
	
	CWinThread*			m_LoginThread;				// 登录线程指针
	CWinThread*			m_ComThread;				// 通讯线程指针
	
	// 线程异步变量
	CRITICAL_SECTION	m_csLoginCommunicationSync;	// 临界区变量
	CRITICAL_SECTION	m_csComCommunicationSync;	// 临界区变量
	BOOL				m_bLoginThreadAlive;		// 指示登录线程是否建立
	BOOL				m_bComThreadAlive;			// 指示通讯线程是否建立
	
	// 线程句柄
	HANDLE				m_hLoginShutdownEvent;		// 关闭登录线程句柄, 用于关闭线程使用
	HANDLE				m_hComShutdownEvent;		// 关闭通讯线程句柄, 用于关闭线程使用
	HANDLE				m_hLoginEvent;				// 登录线程句柄, 用于登录通讯使用
	HANDLE				m_hComEvent;				// 通讯线程句柄, 用于DTU通讯使用
	
	HANDLE				m_hLoginEventArray[2];		// 登录线程句柄数组. 分别对应2个线程句柄
	HANDLE				m_hComEventArray[2];		// 通讯线程句柄数组. 分别对应2个线程句柄

public:
	CWnd *m_pOwner;									// 父类指针	
	
	CSerialPort   m_Port;							// 串口对象
	DTU_PORT_PARA m_PortPara;						// 串口参数
	BOOL  m_bOpen;									// 串口是否打开
	BOOL  m_bRecv;									// 是否处于接收状态
	BOOL  m_bLogin;									// 是否登录
	BOOL  m_bEchoPositive;							// 是否正相应

	REALTIME_FRAME_INFO DtuRealTimeFrame;			// 实时数据发送帧结构
	GROSS_FRAME_INFO    DtuGrossFrame;				// 产量数据发送帧结构

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

//	BYTE                DtuTempBuffer[1024];		// 用于临时数据发送

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

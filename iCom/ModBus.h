#if !defined(AFX_MODBUS_H__DAA50659_AFF9_4531_AC6D_46F05E183BE2__INCLUDED_)
#define AFX_MODBUS_H__DAA50659_AFF9_4531_AC6D_46F05E183BE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModBus.h : header file
//

#include <vector>
using namespace std;

#include "SerialPort.h"

typedef struct tagSerialPort
{
	int iPortNum;				// 串口号
	int iBaudrate;				// 波特率
	char cParity;				// 校验方式
	int iDataBits;				// 数据位数
	int iStopBits;				// 停止位数
	int iBufferSize;			// 缓冲区大小

	int iTerminals;				// 终端个数
	int iTotalChannels;			// 总通通道数
	DWORD lTimeout;				// 超时时间
	DWORD lScanInternal;		// 模块扫描时间间隔
} PORT_PARA;

typedef struct tagModBus
{
	BYTE  TmID;
	BYTE  OpCmd;
	BYTE  Quantity;
	WORD  RegAddr;
	WORD *pRegBuff;
	WORD  RecvPoint;
	WORD  CRC;
} MODBUS_FRAME_INFO;

typedef struct tagTemObject
{
	BYTE       TemId;			// 设备地址
	BYTE       TemCmd;			// 设备命令
	BYTE       TemChn;			// 设备通道数
	WORD       TemAddr;			// 设备寄存器起始地址
	WORD       TemOffsetAddr;	// 设备寄存器偏移地址
	BYTE       TemSts;			// 设备状态
	BYTE       TemErr;			// 错误代码
	BYTE       TemTyp;			// 0 = 周期型设备, 1 = 事件型设备
	BOOL       TemTrig;			// 触发标志, 只针对事件型设备有用
	WORD      *pTemRegBuff;
} TemObject, *LPTemObject;

typedef enum
{
	RECV_ID = 0x00,
	RECV_CMD,
	RECV_ERR,
	RECV_S_ADDR,
	RECV_M_ADDR,
	RECV_LEN,
	RECV_DAT,
	RECV_CRC
} MODBUS_RECV_PROC;

#define MODBUS_INPUT_HREG_CMD		0x03
#define MODBUS_INPUT_REG_CMD		0x04
#define MODBUS_WRITE_COIL_CMD		0x05
#define MODBUS_WRITE_REG_CMD		0x06
#define MODBUS_WRITE_MCOIL_CMD		0x0f
#define MODBUS_WRITE_MREG_CMD		0x10

#define WM_UPDATE_TERMINAL			WM_USER + 100	// 自定义的更新终端数据消息
#define WM_ERROR_TERMINAL			WM_USER + 101	// 自定义的更新终端数据消息

/////////////////////////////////////////////////////////////////////////////
// CModBus window
#define ICOM_CLASSNAME		_T("MFCiCom")

class CModBus : public CWnd
{
// Construction
public:
	CModBus(CWnd *pOwner, int iPortNum, int iBaudrate, char cParity, int iDataBits, int iStopBits, int iBufferSize, 
			DWORD lTimeout, DWORD lScanInternal, int iTerminals, LPTemObject pTemObjectBuff);

protected:
	static UINT	CommThread(LPVOID pParam);

protected:
	// 线程指针
	CWinThread*			m_Thread;
	
	// 线程异步变量
	CRITICAL_SECTION	m_csCommunicationSync;		// 临界区变量
	BOOL				m_bThreadAlive;				// 指示线程是否建立
	
	// 线程句柄
	HANDLE				m_hShutdownEvent;			// 关闭线程句柄, 用于关闭线程使用
	HANDLE				m_hSendEvent;				// 发送数据句柄
	HANDLE				m_hErrEvent;				// 接错误句柄
	HANDLE              m_hSendWriteEvent;			// 发送写数据数据句柄

	// 线程句柄数组. 分别对应3个线程句柄
	HANDLE				m_hEventArray[4];

	CWnd *m_pOwner;									// 父类指针	

	CSerialPort m_Port;								// 串口对象

	BOOL  m_bOpen;
	BOOL  m_bRecv;
	BOOL  m_bScan;
	
	MODBUS_FRAME_INFO m_SendFrameInfo;
	MODBUS_RECV_PROC  m_RecvProc;
	PORT_PARA         m_PortPara;
	LPTemObject       m_pTemObjectBuff;
	LPTemObject       m_pFatherTemObjectBuff;

	BYTE    m_TmpComBuffer[512];

// Operations
public:
	BOOL RegisterWindowClass();
	BOOL Create(CWnd* pParentWnd, UINT nID, DWORD dwStyle);
 	void DestroyWindows();

	void Request(void);
	BOOL Request(BYTE iTerminalIndex);
	void ResetComm(void);
	WORD WordSwitch(WORD Value);
	BOOL GetRegisterValue(void);
	unsigned int CRC16Make(unsigned char *pucFrame, unsigned int usLen);
	void SetWriteTerminalInfo(int iTemIdx, WORD iTemWriteAddr, WORD iTemWriteRegs, WORD *pTemRegBuff);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModBus)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CModBus();

	// Generated message map functions
protected:
	//{{AFX_MSG(CModBus)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg LONG OnComm(WPARAM ch, LPARAM port);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODBUS_H__DAA50659_AFF9_4531_AC6D_46F05E183BE2__INCLUDED_)

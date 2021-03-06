// ModBus.cpp : implementation file
//
#include "stdafx.h"
#include <assert.h>
#include "ModBus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const unsigned char  auchCRCHi[] = {
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40
};
const unsigned char  auchCRCLo[] = {
		0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
		0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
		0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
		0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
		0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
		0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
		0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
		0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
		0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
		0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
		0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
		0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
		0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
		0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
		0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
		0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
		0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
		0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
		0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
		0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
		0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
		0x41, 0x81, 0x80, 0x40
};

/////////////////////////////////////////////////////////////////////////////
// CModBus

CModBus::CModBus(CWnd *pOwner, 
				 int iPortNum, 
				 int iBaudrate, 
				 char cParity, 
				 int iDataBits, 
				 int iStopBits, 
				 int iBufferSize, 
				 DWORD lTimeout, 
				 DWORD lScanInternal, 
				 int iTerminals, 
				 LPTemObject pTemObjectBuff)
{
	RegisterWindowClass();						// 注册窗口, 以便获得消息

	m_pOwner = pOwner;
	m_PortPara.iPortNum    = iPortNum;
	m_PortPara.iBaudrate   = iBaudrate;
	m_PortPara.cParity     = cParity;
	m_PortPara.iDataBits   = iDataBits;
	m_PortPara.iStopBits   = iStopBits;
	m_PortPara.iBufferSize = iBufferSize;
	m_PortPara.iTerminals  = iTerminals;
	m_PortPara.lTimeout    = lTimeout;

	int i;

	if (iTerminals <= 0)
	{
		m_pTemObjectBuff = NULL;
	}
	else
	{
		m_pFatherTemObjectBuff = pTemObjectBuff;		// 存储父类存储区指针
		m_pTemObjectBuff       = new TemObject[iTerminals];
		for (i = 0; i < iTerminals; i++)
		{
			m_pTemObjectBuff[i].TemId         = m_pFatherTemObjectBuff[i].TemId;
			m_pTemObjectBuff[i].TemCmd        = m_pFatherTemObjectBuff[i].TemCmd;
			m_pTemObjectBuff[i].TemChn        = m_pFatherTemObjectBuff[i].TemChn;
			m_pTemObjectBuff[i].TemAddr       = m_pFatherTemObjectBuff[i].TemAddr;
			m_pTemObjectBuff[i].TemOffsetAddr = 0;
			m_pTemObjectBuff[i].TemSts        = 1;
			m_pTemObjectBuff[i].TemErr        = 0;
			m_pTemObjectBuff[i].TemTyp        = m_pFatherTemObjectBuff[i].TemTyp;
			m_pTemObjectBuff[i].TemTrig       = FALSE;
			
			if (m_pTemObjectBuff[i].TemChn <= 0)
			{
				m_pTemObjectBuff[i].pTemRegBuff = NULL;
			}
			else
			{
// 				if (i < 4)
// 				{
// 					m_pTemObjectBuff[i].pTemRegBuff = new WORD[m_pTemObjectBuff[i].TemChn];
// 					for (int j = 0; j < m_pTemObjectBuff[i].TemChn; j++)
// 					{
// 						m_pTemObjectBuff[i].pTemRegBuff[j] = WordSwitch(j * 10);
// 					}
// 				}
// 				else
// 				{
// 					m_pTemObjectBuff[i].pTemRegBuff = new WORD[m_pTemObjectBuff[i].TemChn];
// 					for (int j = 0; j < m_pTemObjectBuff[i].TemChn; j++)
// 					{
// 						m_pTemObjectBuff[i].pTemRegBuff[j] = WordSwitch(0);
// 					}
// 				}

				m_pTemObjectBuff[i].pTemRegBuff = new WORD[m_pTemObjectBuff[i].TemChn];
				memset((BYTE *)(m_pTemObjectBuff[i].pTemRegBuff), WordSwitch(32768), m_pTemObjectBuff[i].TemChn * 2);
			}
		}
	}

	// 模块扫描时间
	assert(lScanInternal >= 100);
	m_PortPara.lScanInternal = lScanInternal;


	m_bOpen = FALSE;
	m_bRecv = FALSE;
	m_bScan = FALSE;
	m_RecvProc = RECV_ID;

	// 相关线程变量初始化
	m_hShutdownEvent = NULL;
	m_hSendEvent = NULL;	
	m_hErrEvent = NULL;	
	m_hSendWriteEvent = NULL;

	m_bThreadAlive = FALSE;	
}

CModBus::~CModBus()
{
}


BEGIN_MESSAGE_MAP(CModBus, CWnd)
	//{{AFX_MSG_MAP(CModBus)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_WM_TIMER()
		ON_MESSAGE(WM_COMM_RXCHAR, OnComm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CModBus message handlers

// Register the window class if it has not already been registered.
BOOL CModBus::RegisterWindowClass()
{
    WNDCLASS wndcls;
    //HINSTANCE hInst = AfxGetInstanceHandle();
    HINSTANCE hInst = AfxGetResourceHandle();
	
    if (!(::GetClassInfo(hInst, ICOM_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = 0;
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = ICOM_CLASSNAME;
		
        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }
	
    return TRUE;
}

BOOL CModBus::Create(CWnd* pParentWnd, UINT nID, DWORD dwStyle)
{
	assert(pParentWnd->GetSafeHwnd());
	
    if (!CWnd::Create(ICOM_CLASSNAME, NULL, dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID))
	{
        return FALSE;
	}

	// 无模块数量返回错误
	if (m_PortPara.iTerminals <= 0)
	{
		return FALSE;
	}

	// 先发一个消息, 以便初始显示
	::SendMessage(m_pOwner->GetSafeHwnd(), WM_UPDATE_TERMINAL, (WPARAM) 0, (LPARAM) 0);

	//////////////////////////////////////////////////////////////////////////
	// 创建相关线程句柄
	//////////////////////////////////////////////////////////////////////////
	if (m_hSendEvent != NULL)
	{
		ResetEvent(m_hSendEvent);
	}
	else
	{
		m_hSendEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	if (m_hSendWriteEvent != NULL)
	{
		ResetEvent(m_hSendWriteEvent);
	}
	else
	{
		m_hSendWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	
	if (m_hErrEvent != NULL)
	{
		ResetEvent(m_hErrEvent);
	}
	else
	{
		m_hErrEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	
	if (m_hShutdownEvent != NULL)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	// 初始化线程句柄数组
	m_hEventArray[0] = m_hShutdownEvent;	// 最高优先级
	m_hEventArray[1] = m_hErrEvent;
	m_hEventArray[2] = m_hSendEvent;
	m_hEventArray[3] = m_hSendWriteEvent;


	// 初始化临界区变量
	InitializeCriticalSection(&m_csCommunicationSync);

	// 开启线程
	if (!(m_Thread = AfxBeginThread(CommThread, this)))
	{
		return FALSE;
	}
	TRACE("Thread started\n");

	if (m_Port.InitPort(this, m_PortPara.iPortNum, m_PortPara.iBaudrate, m_PortPara.cParity, m_PortPara.iDataBits, 
						m_PortPara.iStopBits, EV_RXCHAR | EV_RXFLAG, m_PortPara.iBufferSize))
	{
		m_Port.StartMonitoring();
		m_bOpen = TRUE;			
	}
	else
	{
		return FALSE;
	}

	SetTimer(0, m_PortPara.lScanInternal, NULL);		// 启动扫描定时器
	return TRUE;
}

void CModBus::DestroyWindows()
{
	// 关闭串口
	if (m_bOpen)
	{
		m_bOpen = FALSE;
		KillTimer(0);			// 释放定时器资源
		m_Port.ClosePort();
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 释放线程句柄
	//////////////////////////////////////////////////////////////////////////
	// 等待线程关闭
// 	while (m_bThreadAlive == TRUE)
// 	{
// 		Sleep(1000);
// 
// 		SetEvent(m_hShutdownEvent);
// 	}

	MSG message;  
    do  
    {  
		SetEvent(m_hShutdownEvent);  
		if(::PeekMessage(&message,m_pOwner->m_hWnd,0,0,PM_REMOVE))  
		{  
			::TranslateMessage(&message);  
			::DispatchMessage(&message);  
		}  
		
	} while (m_bThreadAlive);
	
	if(m_hShutdownEvent != NULL)
	{
		CloseHandle(m_hShutdownEvent);
	}
	
	if(m_hSendEvent != NULL)
	{
		CloseHandle(m_hSendEvent);
	}
	
	if(m_hErrEvent != NULL)
	{
		CloseHandle(m_hErrEvent);
	}
	
	if (m_hSendWriteEvent != NULL)
	{
		CloseHandle(m_hSendWriteEvent);
	}
	
	TRACE("Thread ended\n");

	for (int i = 0; i < m_PortPara.iTerminals; i++)
	{
		if (m_pTemObjectBuff[i].pTemRegBuff != NULL)
		{
			delete [] m_pTemObjectBuff[i].pTemRegBuff;
		}
	}

	if (m_pTemObjectBuff != NULL)
	{
		delete [] m_pTemObjectBuff;
	}
}

/****************************************************************************
函数名称: CRC16Make
函数功能: Modbus主栈读写功能函数 
创建日期: 
创 建 人: 
*Input          
:  unsigned char * pucFrame 待校验的数据区首指针
:  unsigned int usLen待校验的数据区字节长度

*Return :  2字节校验数据	  
***************************************************************************/
unsigned int CModBus::CRC16Make(unsigned char *pucFrame, unsigned int usLen)
{
    unsigned char	uchCRCHi = 0xFF;
    unsigned char	uchCRCLo = 0xFF;
    unsigned long	uIndex;
	
    while(usLen--)
    {
        uIndex = uchCRCLo ^ *pucFrame++;
        uchCRCLo = (uchCRCHi ^ auchCRCHi[uIndex]);
        uchCRCHi = auchCRCLo[uIndex];
    }
	
    return (uchCRCHi << 8 | uchCRCLo);
}

void CModBus::SetWriteTerminalInfo(int iTemIdx, WORD iTemWriteAddr, WORD iTemWriteRegs, WORD *pTemRegBuff)
{
	if (iTemIdx >= m_PortPara.iTerminals)
	{
		return;
	}

	if ((m_pTemObjectBuff[iTemIdx].TemTyp == 1) && (iTemWriteAddr >= m_pTemObjectBuff[iTemIdx].TemAddr))
	{
		// 得到需要写入寄存器的起始地址
		m_pTemObjectBuff[iTemIdx].TemOffsetAddr = iTemWriteAddr - m_pTemObjectBuff[iTemIdx].TemAddr;		
		
		for (int i = 0; i < iTemWriteRegs; i++)
		{
			m_pTemObjectBuff[iTemIdx].pTemRegBuff[m_pTemObjectBuff[iTemIdx].TemOffsetAddr] = pTemRegBuff[i];
		}

		m_pTemObjectBuff[iTemIdx].TemTrig = 1;								// 启动触发
	}
}

// 由定时器调用
void CModBus::Request(void)
{
	if (m_bRecv || !m_bOpen || m_bScan)
	{
		return;
	}

	SetEvent(m_hSendEvent);			// 启动发送线程, 顺序扫描模块
}

// 由线程调用
BOOL CModBus::Request(BYTE iTerminalIndex)
{
	if (!m_bOpen)
	{
		return FALSE;
	}

	assert(iTerminalIndex < m_PortPara.iTerminals);

	if ((m_pTemObjectBuff[iTerminalIndex].TemTyp == 1) 
		&& (m_pTemObjectBuff[iTerminalIndex].TemTrig == FALSE))
	{
		return FALSE;
	}

	ResetComm();
	
	BYTE *pbuff = new BYTE[256];
	int  i;
	WORD CRC;

	switch (m_pTemObjectBuff[iTerminalIndex].TemCmd)
	{
	case MODBUS_INPUT_HREG_CMD:
	case MODBUS_INPUT_REG_CMD:
		assert((m_pTemObjectBuff[iTerminalIndex].TemChn > 0x00) && (m_pTemObjectBuff[iTerminalIndex].TemChn < 0x7d));
		pbuff[0] = m_pTemObjectBuff[iTerminalIndex].TemId;				// 模块ID
		pbuff[1] = m_pTemObjectBuff[iTerminalIndex].TemCmd;				// 命令
		pbuff[2] = (BYTE)(m_pTemObjectBuff[iTerminalIndex].TemAddr >> 8);			
		pbuff[3] = (BYTE)(m_pTemObjectBuff[iTerminalIndex].TemAddr & 0x00ff);
		pbuff[4] = (BYTE)(m_pTemObjectBuff[iTerminalIndex].TemChn >> 8);
		pbuff[5] = (BYTE)(m_pTemObjectBuff[iTerminalIndex].TemChn & 0x00ff);
		
		// CRC排列是先低后高
		CRC      = CRC16Make(pbuff, 6);
		pbuff[6] = (BYTE)(CRC & 0x00ff);
		pbuff[7] = (BYTE)(CRC >> 8);	
		
		m_SendFrameInfo.TmID     = m_pTemObjectBuff[iTerminalIndex].TemId;
		m_SendFrameInfo.OpCmd    = m_pTemObjectBuff[iTerminalIndex].TemCmd;
		m_SendFrameInfo.RegAddr  = m_pTemObjectBuff[iTerminalIndex].TemAddr;
		m_SendFrameInfo.Quantity = m_pTemObjectBuff[iTerminalIndex].TemChn;	
		m_SendFrameInfo.pRegBuff = m_pTemObjectBuff[iTerminalIndex].pTemRegBuff;
		m_Port.WriteToPort(pbuff, 8);
		break;

	case MODBUS_WRITE_REG_CMD:
		pbuff[0] = m_pTemObjectBuff[iTerminalIndex].TemId;				// 模块ID
		pbuff[1] = m_pTemObjectBuff[iTerminalIndex].TemCmd;				// 命令
		pbuff[2] = (BYTE)((m_pTemObjectBuff[iTerminalIndex].TemAddr + m_pTemObjectBuff[iTerminalIndex].TemOffsetAddr) >> 8);			
		pbuff[3] = (BYTE)((m_pTemObjectBuff[iTerminalIndex].TemAddr + m_pTemObjectBuff[iTerminalIndex].TemOffsetAddr) & 0x00ff);
		pbuff[4] = (BYTE)(m_pTemObjectBuff[iTerminalIndex].pTemRegBuff[m_pTemObjectBuff[iTerminalIndex].TemOffsetAddr] >> 8);
		pbuff[5] = (BYTE)(m_pTemObjectBuff[iTerminalIndex].pTemRegBuff[m_pTemObjectBuff[iTerminalIndex].TemOffsetAddr] & 0x00ff);
		
		// CRC排列是先低后高
		CRC      = CRC16Make(pbuff, 6);
		pbuff[6] = (BYTE)(CRC & 0x00ff);
		pbuff[7] = (BYTE)(CRC >> 8);

		m_SendFrameInfo.TmID     = m_pTemObjectBuff[iTerminalIndex].TemId;
		m_SendFrameInfo.OpCmd    = m_pTemObjectBuff[iTerminalIndex].TemCmd;
		m_SendFrameInfo.RegAddr  = m_pTemObjectBuff[iTerminalIndex].TemAddr 
									+ m_pTemObjectBuff[iTerminalIndex].TemOffsetAddr;
		m_SendFrameInfo.Quantity = m_pTemObjectBuff[iTerminalIndex].TemChn;	
		m_SendFrameInfo.pRegBuff = NULL;
		m_Port.WriteToPort(pbuff, 8);
		break;
		
	case MODBUS_WRITE_MREG_CMD:
		assert((m_pTemObjectBuff[iTerminalIndex].TemChn > 0x00) && (m_pTemObjectBuff[iTerminalIndex].TemChn < 0x7b));
		pbuff[0] = m_pTemObjectBuff[iTerminalIndex].TemId;				// 模块ID
		pbuff[1] = m_pTemObjectBuff[iTerminalIndex].TemCmd;				// 命令
		pbuff[2] = (BYTE)(m_pTemObjectBuff[iTerminalIndex].TemAddr >> 8);			
		pbuff[3] = (BYTE)(m_pTemObjectBuff[iTerminalIndex].TemAddr & 0x00ff);
		pbuff[4] = (BYTE)(m_pTemObjectBuff[iTerminalIndex].TemChn >> 8);
		pbuff[5] = (BYTE)(m_pTemObjectBuff[iTerminalIndex].TemChn & 0x00ff);
		pbuff[6] = (BYTE)(m_pTemObjectBuff[iTerminalIndex].TemChn * 2);
		
		for (i = 0; i < m_pTemObjectBuff[iTerminalIndex].TemChn; i++)
		{
			pbuff[7 + i * 2 + 0] = (BYTE)(m_pTemObjectBuff[iTerminalIndex].pTemRegBuff[i] >> 8);
			pbuff[7 + i * 2 + 1] = (BYTE)(m_pTemObjectBuff[iTerminalIndex].pTemRegBuff[i] & 0x00ff);
		}
		
		// CRC排列是先低后高
		CRC = CRC16Make(pbuff, 7 + m_pTemObjectBuff[iTerminalIndex].TemChn * 2);
		pbuff[7 + m_pTemObjectBuff[iTerminalIndex].TemChn * 2 + 0] = (BYTE)(CRC & 0x00ff);
		pbuff[7 + m_pTemObjectBuff[iTerminalIndex].TemChn * 2 + 1] = (BYTE)(CRC >> 8);

		m_SendFrameInfo.TmID     = m_pTemObjectBuff[iTerminalIndex].TemId;
		m_SendFrameInfo.OpCmd    = m_pTemObjectBuff[iTerminalIndex].TemCmd;
		m_SendFrameInfo.RegAddr  = m_pTemObjectBuff[iTerminalIndex].TemAddr;
		m_SendFrameInfo.Quantity = m_pTemObjectBuff[iTerminalIndex].TemChn;	
		m_SendFrameInfo.pRegBuff = NULL;

		m_Port.WriteToPort(pbuff, 7 + m_pTemObjectBuff[iTerminalIndex].TemChn * 2);
		break;

	default:
		break;
	}
	
	delete [] pbuff;
	m_bRecv = TRUE;
	return TRUE;
}

void CModBus::ResetComm(void)
{
	m_bRecv    = FALSE;
	m_RecvProc = RECV_ID;
}

void CModBus::OnTimer(UINT nIDEvent) 
{
	Request();			// 触发扫描
	
	CWnd::OnTimer(nIDEvent);
}

BOOL CModBus::GetRegisterValue(void)
{
	int i, j;
	
	//////////////////////////////////////////////////////////////////////////
	// 装载模块数据
	//////////////////////////////////////////////////////////////////////////
	for (i = 0; i < m_PortPara.iTerminals; i++)
	{
		m_pFatherTemObjectBuff[i].TemSts = m_pTemObjectBuff[i].TemSts;		// 装载模块状态标志

		if (m_pFatherTemObjectBuff[i].TemChn == m_pTemObjectBuff[i].TemChn)
		{
			if (m_pTemObjectBuff[i].TemErr == 0x00)							// 屏蔽或不屏蔽
			{
				// 填充缓冲区
				for (j = 0; j < m_pTemObjectBuff[i].TemChn; j++)
				{
					// 大小端交换+四舍五入处理
					m_pFatherTemObjectBuff[i].pTemRegBuff[j] = (WORD)((double)((short)(WordSwitch(m_pTemObjectBuff[i].pTemRegBuff[j])) + 5) / 10.0);
				}
			}
		}
		else
		{
			return FALSE;
		}
	}

	// 填充缓冲区
// 	for (i = 0; i < m_PortPara.iTotalChannels; i++)
// 	{
// 		// 大小端交换+四舍五入处理
// 		pRegBuff[i] = (WORD)((double)(WordSwitch(m_PortPara.pRegBuff[i]) + 5) / 10.0);
// 	}

	return TRUE;
}

WORD CModBus::WordSwitch(WORD Value)
{
	WORD val = 0;
	
	val = Value & 0x00ff;
	val = val << 8;
	val = val | (Value >> 8);

	return val;
}

LONG CModBus::OnComm(WPARAM ch, LPARAM port)
{
	if (!m_bRecv || !m_bOpen)
	{
		return -1;
	}

	if (port < 1 || port > 30)
	{
		return -1;
	}

	int  i;
	BYTE recv_ch = ch;
	BYTE temp_byte;
	WORD index, offset;

	switch (m_RecvProc)
	{
	case RECV_ID:
		if (m_SendFrameInfo.TmID == recv_ch)					// 地址验证正确
		{
			m_RecvProc = RECV_CMD;
		}
		break;

	case RECV_CMD:
		if (m_SendFrameInfo.OpCmd == recv_ch)					// 命令验证正确
		{
			switch (m_SendFrameInfo.OpCmd)						// 根据不同的命令跳转
			{
			case MODBUS_INPUT_HREG_CMD:
			case MODBUS_INPUT_REG_CMD:
				m_RecvProc = RECV_LEN;
				break;
			case MODBUS_WRITE_MREG_CMD:
				m_SendFrameInfo.RecvPoint = 0;
				m_RecvProc = RECV_M_ADDR;						// 接收寄存器地址+寄存器个数+字节数+数据
				break;
			case MODBUS_WRITE_REG_CMD:
				m_SendFrameInfo.RecvPoint = 0;
				m_RecvProc = RECV_S_ADDR;						// 接收寄存器地址+数据
				break;
			default:
				m_RecvProc = RECV_ID;
				break;
			}
		}
		else if ((m_SendFrameInfo.OpCmd | 0x80) == recv_ch)		// 返回错误
		{
			m_RecvProc = RECV_ERR;
		}
		else													// 非法命令返回接收模块ID
		{
			m_RecvProc = RECV_ID;
		}
		break;

	case RECV_S_ADDR:
		m_TmpComBuffer[m_SendFrameInfo.RecvPoint] = recv_ch;

		m_SendFrameInfo.RecvPoint++;
		if (m_SendFrameInfo.RecvPoint >= 4)
		{
			index =	 ((WORD)(m_TmpComBuffer[0]) << 8) | ((WORD)(m_TmpComBuffer[1]));	// 寄存器地址
			offset = ((WORD)(m_TmpComBuffer[2]) << 8) | ((WORD)(m_TmpComBuffer[3]));	// 寄存器数据
			if (m_SendFrameInfo.RegAddr == index)
			{
				m_SendFrameInfo.RecvPoint = 0;
				m_RecvProc = RECV_CRC;
			}
			else
			{
				m_RecvProc = RECV_ID;
			}
		}
		break;

	case RECV_M_ADDR:			// 为了简化程序, 此接收方式与标准MODBUS不相符
		m_TmpComBuffer[m_SendFrameInfo.RecvPoint] = recv_ch;

		m_SendFrameInfo.RecvPoint++;
		if (m_SendFrameInfo.RecvPoint >= 5)
		{
			index     =	((WORD)(m_TmpComBuffer[0]) << 8) | ((WORD)(m_TmpComBuffer[1]));	// 寄存器地址
			offset    = ((WORD)(m_TmpComBuffer[2]) << 8) | ((WORD)(m_TmpComBuffer[3]));	// 寄存器数量
			temp_byte = m_TmpComBuffer[4];
			if (m_SendFrameInfo.RegAddr == index)
			{
				m_SendFrameInfo.RecvPoint = 0;
				m_RecvProc = RECV_CRC;
			}
			else
			{
				m_RecvProc = RECV_ID;
			}
		}
		break;

	case RECV_ERR:
		// 用终端ID号找到对应的下表
		for (i = 0; i < m_PortPara.iTerminals; i++)
		{
			if (m_pTemObjectBuff[i].TemId == m_SendFrameInfo.TmID)
			{
				m_pTemObjectBuff[i].TemErr = recv_ch;			// 赋值错误代码
				SetEvent(m_hErrEvent);							// 触发线程
				break;
			}
		}		
		break;

	case RECV_LEN:
		if ((m_SendFrameInfo.Quantity * 2) == recv_ch)
		{
			m_SendFrameInfo.RecvPoint = 0;
			m_RecvProc = RECV_DAT;
		}
		else
		{
			m_RecvProc = RECV_ID;
		}
		break;

	case RECV_DAT:		
		if (m_SendFrameInfo.pRegBuff != NULL)
		{
			// 接收数据
			index = m_SendFrameInfo.RecvPoint / 2;			// 计算寄存器的编号
			offset = m_SendFrameInfo.RecvPoint % 2;			// 计算寄存器值的高低位编号

			// 在这里不进行大小端变换, 方便后续校验
			if (offset == 1)
			{
				((BYTE *)(m_SendFrameInfo.pRegBuff))[index * 2 + 1] = recv_ch;
			}
			else
			{
				((BYTE *)(m_SendFrameInfo.pRegBuff))[index * 2 + 0] = recv_ch;
			}
		}
		
		m_SendFrameInfo.RecvPoint++;
		if (m_SendFrameInfo.RecvPoint >= (m_SendFrameInfo.Quantity * 2))
		{
			m_SendFrameInfo.RecvPoint = 0;
			m_RecvProc = RECV_CRC;
		}
		break;

	case RECV_CRC:
		index  = m_SendFrameInfo.RecvPoint / 2;
		offset = m_SendFrameInfo.RecvPoint % 2;

		// 在这里不进行大小端变换, 方便后续校验
		if (offset == 1)
		{
			((BYTE *)(&m_SendFrameInfo.CRC))[index * 2 + 1] = recv_ch;
		}
		else
		{
			((BYTE *)(&m_SendFrameInfo.CRC))[index * 2 + 0] = recv_ch;
		}

		m_SendFrameInfo.RecvPoint++;
		if (m_SendFrameInfo.RecvPoint >= 2)
		{			
			switch (m_SendFrameInfo.OpCmd)						// 根据不同的命令跳转
			{
			case MODBUS_INPUT_HREG_CMD:
			case MODBUS_INPUT_REG_CMD:
				// 校验整个数据帧
				m_TmpComBuffer[0] = m_SendFrameInfo.TmID;
				m_TmpComBuffer[1] = m_SendFrameInfo.OpCmd;
				m_TmpComBuffer[2] = m_SendFrameInfo.Quantity * 2;
				memcpy(&m_TmpComBuffer[3], &((BYTE *)(m_SendFrameInfo.pRegBuff))[0], m_SendFrameInfo.Quantity * 2);
				m_TmpComBuffer[3 + m_SendFrameInfo.Quantity * 2 + 1] = (BYTE)(m_SendFrameInfo.CRC >> 8);
				m_TmpComBuffer[3 + m_SendFrameInfo.Quantity * 2 + 0] = (BYTE)(m_SendFrameInfo.CRC & 0x00ff);
				
				if (0x0000 == CRC16Make(m_TmpComBuffer, 5 + m_SendFrameInfo.Quantity * 2))
				{
					SetEvent(m_hSendEvent);							// 触发线程
				}
				else
				{
					SetEvent(m_hErrEvent);							// 触发线程
				}
				break;
			case MODBUS_WRITE_MREG_CMD:								// 不校验, 默认正确
			case MODBUS_WRITE_REG_CMD:
				SetEvent(m_hSendEvent);								// 触发线程
				break;

			default:
				break;
			}

			ResetComm();
		}
		break;

	default:
		ResetComm();
		break;
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//  线程函数: CommThread
//////////////////////////////////////////////////////////////////////////
UINT CModBus::CommThread(LPVOID pParam)
{
	// Cast the void pointer passed to the thread back to a pointer of CModBus class
	CModBus *pModBus = (CModBus *)pParam;
	
	// Set the status variable in the dialog class to TRUE to indicate the thread is running.
	pModBus->m_bThreadAlive = TRUE;	

	BOOL  Res;
	DWORD Event = 0, Event1 = 0;
	BYTE  ScanPoint = 0;
	int   i;

	for (;;)
	{
		// Main wait function.  This function will normally block the thread
		// until one of nine events occur that require action.
		Event = WaitForMultipleObjects(4, pModBus->m_hEventArray, FALSE, INFINITE);
		
		switch (Event)
		{
		case 0:
		{
			// Shutdown event.  This is event zero so it will be
			// the higest priority and be serviced first.
			pModBus->m_bThreadAlive = FALSE;
			
			// Kill this thread.  break is not needed, but makes me feel better.
			AfxEndThread(100);
			break;
		}

		case 2: // Send Event 发送信号量
			ResetEvent(pModBus->m_hSendEvent);		// 复位信号量

			EnterCriticalSection(&pModBus->m_csCommunicationSync);
			pModBus->m_bScan = TRUE;				// 指示扫描开始
			LeaveCriticalSection(&pModBus->m_csCommunicationSync);

			// 循环扫描每一个模块
			while (ScanPoint < pModBus->m_PortPara.iTerminals)
			{
				Sleep(100);								// 由于没有使用多机方式, 需要发送间隔时间

				EnterCriticalSection(&pModBus->m_csCommunicationSync);
				Res = pModBus->Request(ScanPoint);		/////// 发送数据
				LeaveCriticalSection(&pModBus->m_csCommunicationSync);

				// 如果请求发送失败(有可能是事件型模块), 扫描下一个模块
				if (Res == FALSE)
				{
					ScanPoint++;
					continue;
				}

				// 挂起线程, 等待接收触发, 并设置了超时时间
				Event1 = WaitForMultipleObjects(4, pModBus->m_hEventArray, FALSE, pModBus->m_PortPara.lTimeout);
				if (Event1 == 0)	// shutdown event						
				{
					// Shutdown event.  This is event zero so it will be
					// the higest priority and be serviced first.
					pModBus->m_bThreadAlive = FALSE;
					
					// Kill this thread.  break is not needed, but makes me feel better.
					AfxEndThread(100);
				}
				else if (Event1 == 2)// send event
				{		
					EnterCriticalSection(&pModBus->m_csCommunicationSync);
					pModBus->m_pTemObjectBuff[ScanPoint].TemErr  = 0x00;	// 赋值错误代码: 无错误用0x00代替
					pModBus->m_pTemObjectBuff[ScanPoint].TemSts  = 1;		// 模块状态更新: 在线
					pModBus->m_pTemObjectBuff[ScanPoint].TemTrig = FALSE;	// 清除触发标志
					LeaveCriticalSection(&pModBus->m_csCommunicationSync);

					ResetEvent(pModBus->m_hSendEvent);
				}
				else if (Event1 == 1)// error event
				{
					EnterCriticalSection(&pModBus->m_csCommunicationSync);
					pModBus->m_pTemObjectBuff[ScanPoint].TemErr  = 0xff;	// 赋值错误代码: 校验错误用0xff代替

					// 连续10次错误才算, 如果已经错误就保持
					if (pModBus->m_pTemObjectBuff[ScanPoint].TemSts != 0)
					{
						pModBus->m_pTemObjectBuff[ScanPoint].TemSts  = (pModBus->m_pTemObjectBuff[ScanPoint].TemSts + 1) % 10;
					}					
//					pModBus->m_pTemObjectBuff[ScanPoint].TemSts  = 1;		// 模块状态更新: 在线, 但接收的数据校验错误
//					pModBus->m_pTemObjectBuff[ScanPoint].TemTrig = TRUE;	// 保持触发标志, 下一个循环继续发送

					if (pModBus->m_pTemObjectBuff[ScanPoint].TemSts == 0)
					{
						// 置该模块的缓冲区为非法值
						for (i = 0; i < pModBus->m_pTemObjectBuff[ScanPoint].TemChn; i++)
						{
							if (pModBus->m_SendFrameInfo.pRegBuff != NULL)
							{
								pModBus->m_SendFrameInfo.pRegBuff[i] = pModBus->WordSwitch(32768);
							}
						}
					}
					LeaveCriticalSection(&pModBus->m_csCommunicationSync);

					ResetEvent(pModBus->m_hErrEvent);
				}
				else	// timeout = 258(WAIT_TIMEOUT)
				{
					EnterCriticalSection(&pModBus->m_csCommunicationSync);
					pModBus->m_pTemObjectBuff[ScanPoint].TemErr  = 0xff;	// 赋值错误代码: 校验错误用0xff代替
					// 连续10次错误才算, 如果已经错误就保持
					if (pModBus->m_pTemObjectBuff[ScanPoint].TemSts != 0)
					{
						pModBus->m_pTemObjectBuff[ScanPoint].TemSts  = (pModBus->m_pTemObjectBuff[ScanPoint].TemSts + 1) % 10;
					}	
//					pModBus->m_pTemObjectBuff[ScanPoint].TemSts  = 0;		// 模块状态更新: 离线
//					pModBus->m_pTemObjectBuff[ScanPoint].TemTrig = TRUE;	// 保持触发标志, 下一个循环继续发送
					
					if (pModBus->m_pTemObjectBuff[ScanPoint].TemSts == 0)
					{
						// 置该模块的缓冲区为非法值
						for (i = 0; i < pModBus->m_pTemObjectBuff[ScanPoint].TemChn; i++)
						{
							if (pModBus->m_SendFrameInfo.pRegBuff != NULL)
							{
								pModBus->m_SendFrameInfo.pRegBuff[i] = pModBus->WordSwitch(32768);
							}
						}
					}
					LeaveCriticalSection(&pModBus->m_csCommunicationSync);			
				}
							
				ScanPoint++;				
			}			

			// 一次扫描完成, 发送消息到父类
			::SendMessage(pModBus->m_pOwner->GetSafeHwnd(), WM_UPDATE_TERMINAL, (WPARAM) 0, (LPARAM) 0);

			ScanPoint = 0;

			EnterCriticalSection(&pModBus->m_csCommunicationSync);
			pModBus->ResetComm();
			pModBus->m_bScan = FALSE;
			LeaveCriticalSection(&pModBus->m_csCommunicationSync);
			break;

		case 3:	// SendWrite Event
			ResetEvent(pModBus->m_hSendWriteEvent);		// 复位信号量
			break;

		case 1:	// Error Event
			::SendMessage(pModBus->m_pOwner->GetSafeHwnd(), WM_ERROR_TERMINAL, (WPARAM) 0, (LPARAM) 0);
			ResetEvent(pModBus->m_hErrEvent);
			break;			
		} // end switch
	}

	return 0;
}
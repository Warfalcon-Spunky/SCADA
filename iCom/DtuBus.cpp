// DtuBus.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "DtuBus.h"

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

const BYTE LoginCnt[32] = {'a', 'd', 'm', 'i', 'n'};
const BYTE LoginPwd[32] = {'s', 'p', 'u', 'n', 'k', 'y'};


/////////////////////////////////////////////////////////////////////////////
// CDtuBus

CDtuBus::CDtuBus(CWnd *pOwner, int iPortNum, int iBaudrate, char cParity, int iDataBits, int iStopBits, int iBufferSize,
				 DWORD lLoginInternal, DWORD lCycleInternal, DWORD lDtuTimeout, DWORD LocalID)
{
	RegisterWindowClass();						// 注册窗口, 以便获得消息

	m_pOwner                   = pOwner;
	m_PortPara.iPortNum        = iPortNum;
	m_PortPara.iBaudrate       = iBaudrate;
	m_PortPara.cParity         = cParity;
	m_PortPara.iDataBits       = iDataBits;
	m_PortPara.iStopBits       = iStopBits;
	m_PortPara.iBufferSize     = iBufferSize;
	m_PortPara.lLoginInternal  = lLoginInternal;
	m_PortPara.lCycleInternal  = lCycleInternal;
	m_PortPara.lDtuTimeout     = lDtuTimeout;
	m_PortPara.lLocalID        = LocalID;

	m_bOpen  = FALSE;
	m_bLogin = FALSE;
	m_bRecv  = FALSE;
	m_bEchoPositive = FALSE;

	// 相关变量初始化
	DtuRealTimeFrame.TunnelKilnId = 0;
	DtuRealTimeFrame.TempNum      = 0;
	DtuRealTimeFrame.SideTempNum  = 0;	
	DtuRealTimeFrame.PressNum     = 0;
	DtuRealTimeFrame.HumidityNum  = 0;
	DtuRealTimeFrame.MotorNum     = 0;
	DtuRealTimeFrame.Year         = 0; 
	DtuRealTimeFrame.Month        = 0; 
	DtuRealTimeFrame.Day          = 0; 
	DtuRealTimeFrame.Hour         = 0; 
	DtuRealTimeFrame.Min          = 0; 
	DtuRealTimeFrame.Sec          = 0;

	DtuRecvProc = DTU_RECV_HEAD;
	
	// 相关线程变量初始化
	m_hLoginShutdownEvent = NULL;		
	m_hComShutdownEvent   = NULL;
	m_hLoginEvent         = NULL;
	m_hComEvent           = NULL;
	
	m_bLoginThreadAlive = FALSE;
	m_bComThreadAlive   = FALSE;
}

CDtuBus::~CDtuBus()
{
}


BEGIN_MESSAGE_MAP(CDtuBus, CWnd)
	//{{AFX_MSG_MAP(CDtuBus)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_COMM_RXCHAR, OnComm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDtuBus message handlers

// Register the window class if it has not already been registered.
BOOL CDtuBus::RegisterWindowClass()
{
    WNDCLASS wndcls;
    //HINSTANCE hInst = AfxGetInstanceHandle();
    HINSTANCE hInst = AfxGetResourceHandle();
	
    if (!(::GetClassInfo(hInst, IDTU_CLASSNAME, &wndcls)))
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
        wndcls.lpszClassName    = IDTU_CLASSNAME;
		
        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }
	
    return TRUE;
}

BOOL CDtuBus::Create(CWnd* pParentWnd, UINT nID, DWORD dwStyle)
{
	assert(pParentWnd->GetSafeHwnd());
	
    if (!CWnd::Create(IDTU_CLASSNAME, NULL, dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID))
	{
        return FALSE;
	}
	
	// 初始化和打开串口
	if (m_Port.InitPort(this, m_PortPara.iPortNum, m_PortPara.iBaudrate, m_PortPara.cParity, m_PortPara.iDataBits, 
		m_PortPara.iStopBits, EV_RXCHAR | EV_RXFLAG, m_PortPara.iBufferSize))
	{
		m_Port.StartMonitoring();			
	}
	else
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 创建登录线程句柄
	//////////////////////////////////////////////////////////////////////////
	if (m_hLoginShutdownEvent != NULL)
	{
		ResetEvent(m_hLoginShutdownEvent);
	}
	else
	{
		m_hLoginShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	if (m_hLoginEvent != NULL)
	{
		ResetEvent(m_hLoginEvent);
	}
	else
	{
		m_hLoginEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 创建通讯线程句柄
	//////////////////////////////////////////////////////////////////////////
	if (m_hComShutdownEvent != NULL)
	{
		ResetEvent(m_hComShutdownEvent);
	}
	else
	{
		m_hComShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	
	if (m_hComEvent != NULL)
	{
		ResetEvent(m_hComEvent);
	}
	else
	{
		m_hComEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	
	// 初始化登录线程句柄数组
	m_hLoginEventArray[0] = m_hLoginShutdownEvent;	// 最高优先级
	m_hLoginEventArray[1] = m_hLoginEvent;

	// 开启登录线程
	if (!(m_LoginThread = AfxBeginThread(LoginThread, this)))
	{
		return FALSE;
	}
	TRACE("Login Thread started\n");

	// 初始化通讯线程句柄数组
	m_hComEventArray[0] = m_hComShutdownEvent;	// 最高优先级
	m_hComEventArray[1] = m_hComEvent;

	// 开启通讯线程
	if (!(m_ComThread = AfxBeginThread(ComThread, this)))
	{
		return FALSE;
	}
	TRACE("Com Thread started\n");
	
	// 初始化临界区变量
	InitializeCriticalSection(&m_csLoginCommunicationSync);
	InitializeCriticalSection(&m_csComCommunicationSync);
	
	m_bOpen = TRUE;

	LoginRequest();						// 启动登录操作
	
	return TRUE;
}

void CDtuBus::DestroyWindows()
{
	CWnd::OnDestroy();	
	
	if (m_bOpen)				// 关闭串口
	{
		m_bOpen = FALSE;
		m_Port.ClosePort();
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 释放线程句柄
	//////////////////////////////////////////////////////////////////////////
	MSG login_message;  
    do  
    {  
		SetEvent(m_hLoginShutdownEvent);  
		if(::PeekMessage(&login_message, m_pOwner->m_hWnd, 0, 0, PM_REMOVE))  
		{  
			::TranslateMessage(&login_message);  
			::DispatchMessage(&login_message);  
		}  
		
	} while (m_bLoginThreadAlive);
	
	if(m_hLoginShutdownEvent != NULL)
	{
		CloseHandle(m_hLoginShutdownEvent);
	}
	
	if(m_hLoginEvent != NULL)
	{
		CloseHandle(m_hLoginEvent);
	}
	TRACE("Login Thread ended\n");
	
	MSG com_message;  
    do  
    {  
		SetEvent(m_hComShutdownEvent);  
		if(::PeekMessage(&com_message, m_pOwner->m_hWnd, 0, 0, PM_REMOVE))  
		{  
			::TranslateMessage(&com_message);  
			::DispatchMessage(&com_message);  
		}  
		
	} while (m_bComThreadAlive);
	
	if(m_hComShutdownEvent != NULL)
	{
		CloseHandle(m_hComShutdownEvent);
	}
	
	if(m_hComEvent != NULL)
	{
		CloseHandle(m_hComEvent);
	}
	TRACE("Com Thread ended\n");
}

void CDtuBus::OnDestroy() 
{
	CWnd::OnDestroy();	
	
	if (m_bOpen)				// 关闭串口
	{
		m_bOpen = FALSE;
		m_Port.ClosePort();
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 释放线程句柄
	//////////////////////////////////////////////////////////////////////////
	MSG login_message;  
    do  
    {  
		SetEvent(m_hLoginShutdownEvent);  
		if(::PeekMessage(&login_message, m_pOwner->m_hWnd, 0, 0, PM_REMOVE))  
		{  
			::TranslateMessage(&login_message);  
			::DispatchMessage(&login_message);  
		}  
		
	} while (m_bLoginThreadAlive);
	
	if(m_hLoginShutdownEvent != NULL)
	{
		CloseHandle(m_hLoginShutdownEvent);
	}
	
	if(m_hLoginEvent != NULL)
	{
		CloseHandle(m_hLoginEvent);
	}
	TRACE("Login Thread ended\n");

	MSG com_message;  
    do  
    {  
		SetEvent(m_hComShutdownEvent);  
		if(::PeekMessage(&com_message, m_pOwner->m_hWnd, 0, 0, PM_REMOVE))  
		{  
			::TranslateMessage(&com_message);  
			::DispatchMessage(&com_message);  
		}  
		
	} while (m_bComThreadAlive);
	
	if(m_hComShutdownEvent != NULL)
	{
		CloseHandle(m_hComShutdownEvent);
	}
	
	if(m_hComEvent != NULL)
	{
		CloseHandle(m_hComEvent);
	}
	TRACE("Com Thread ended\n");
}

// 启动登录线程进行登录通讯过程, 登录线程启动后，必须成功登录后才能关闭，否则每3秒钟尝试一次登录过程
void CDtuBus::LoginRequest(void)
{
	if (m_bRecv || !m_bOpen || m_bLogin)
	{
		return;
	}
	
	m_bRecv = TRUE;
	SetEvent(m_hLoginEvent);			// 启动登录通讯线程
}

// 登录传输数据, 用于登录线程调用
void CDtuBus::LoginTransmit(void)
{
	if (!m_bOpen || m_bLogin)
	{
		return;
	}

	WORD point = 0;
	WORD crc;
	BYTE *buff;
	buff = new BYTE[128];
	
	buff[point++] = (BYTE)(THE_FRAME_HEAD >> 8);
	buff[point++] = (BYTE)(THE_FRAME_HEAD & 0x00ff);
	buff[point++] = 0x00;
	buff[point++] = 0x54;						// 19个固定字节+1字节ID+32字节用户名+32字节密码
	buff[point++] = THE_CMD_LOGNIN;
	buff[point++] = 0x00;
	buff[point++] = 0x00;
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 24);
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 16);
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 8);
	buff[point++] = (BYTE)(m_PortPara.lLocalID & 0x000000ff);
	buff[point++] = 0x00;
	buff[point++] = 0x00;
	buff[point++] = 0x00;
	buff[point++] = 0x01;

	buff[point++] = 0x01;			// 设备类型

	memcpy(&buff[point], LoginCnt, 32);
	point = point + 32;

	memcpy(&buff[point], LoginPwd, 32);
	point = point + 32;
	crc = CRC16Make(buff, point);
	buff[point++] = (BYTE)(crc >> 8);
	buff[point++] = (BYTE)(crc & 0x00ff);
	buff[point++] = (BYTE)(THE_FRAME_REAR >> 8);
	buff[point++] = (BYTE)(THE_FRAME_REAR & 0x00ff);

	m_Port.WriteToPort(buff, point);

	delete [] buff;
}

//////////////////////////////////////////////////////////////////////////
//  线程函数: LoginThread
//////////////////////////////////////////////////////////////////////////
UINT CDtuBus::LoginThread(LPVOID pParam)
{
	DWORD Event = 0, Event1 = 0;

	// Cast the void pointer passed to the thread back to a pointer of CModBus class
	CDtuBus *pDtuBus = (CDtuBus *)pParam;
	
	// Set the status variable in the dialog class to TRUE to indicate the thread is running.
	pDtuBus->m_bLoginThreadAlive = TRUE;
	Sleep(3000);								// 等待其他线程启动

	for (;;)
	{
		// Main wait function.  This function will normally block the thread
		// until one of nine events occur that require action.
		Event = WaitForMultipleObjects(2, pDtuBus->m_hLoginEventArray, FALSE, INFINITE);
		
		switch (Event)
		{
		case 0:
		{
			// Shutdown event.  This is event zero so it will be
			// the highest priority and be serviced first.
			pDtuBus->m_bLoginThreadAlive = FALSE;
			
			// Kill this thread.  break is not needed, but makes me feel better.
			AfxEndThread(100);
			break;
		}

		case 1: // 登录通讯信号里
			ResetEvent(pDtuBus->m_hLoginEvent);		// 复位信号量

			EnterCriticalSection(&pDtuBus->m_csLoginCommunicationSync);
			pDtuBus->m_bLogin = FALSE;				// 指示当前未登录
			LeaveCriticalSection(&pDtuBus->m_csLoginCommunicationSync);

			while (pDtuBus->m_bLogin == FALSE)
			{
				pDtuBus->m_bRecv = TRUE;
				pDtuBus->LoginTransmit();				// 发送登录数据
				TRACE("<<<<登录请求>>>>\r\n");			

				// 挂起线程, 等待接收触发, 并设置了超时时间
				Event1 = WaitForMultipleObjects(2, pDtuBus->m_hLoginEventArray, FALSE, pDtuBus->m_PortPara.lDtuTimeout);
				if (Event1 == 0)	// shutdown event						
				{
					// Shutdown event.  This is event zero so it will be
					// the highest priority and be serviced first.
					pDtuBus->m_bLoginThreadAlive = FALSE;
					
					// Kill this thread.  break is not needed, but makes me feel better.
					AfxEndThread(100);
				}
				else if (Event1 == 1)// recv event
				{
					ResetEvent(pDtuBus->m_hLoginEvent);		// 复位信号量

					if (pDtuBus->m_bEchoPositive == TRUE)
					{
						EnterCriticalSection(&pDtuBus->m_csLoginCommunicationSync);
						pDtuBus->m_bLogin = TRUE;			// 指示当前已经登录
						pDtuBus->m_bRecv  = FALSE;
                        LeaveCriticalSection(&pDtuBus->m_csLoginCommunicationSync);

						TRACE("---登录请求成功---\r\n");
					}
					else
					{
						Sleep(pDtuBus->m_PortPara.lLoginInternal);	// 挂起线程5秒钟，再次进行登录
						TRACE("---登录请求失败, 延时5秒继续登录---\r\n");
					}					
				}
				else										// timeout = 258(WAIT_TIMEOUT)
				{
					ResetEvent(pDtuBus->m_hLoginEvent);		// 复位信号量
					TRACE("---登录请求超时---\r\n");
				}
			}
			break;
		default:
			break;
		} // end switch
	}

	return 0;
}

BOOL CDtuBus::ComTransmit(BYTE cmd, void *p_buff)
{
	if ((!m_bLogin) || (!m_bOpen) || (m_bRecv))
	{
		return FALSE;
	}

	switch (cmd)
	{
	case THE_CMD_GET_RTDAT:		// 获取实时数据, DTU主动发送
		DtuRealTimeFrame = *((LPREALTIME_FRAME_INFO)(p_buff));
		DtuSendCmd = THE_CMD_GET_RTDAT;
		m_bRecv = TRUE;
		m_bEchoPositive = FALSE;
		SetEvent(m_hComEvent);	// 启动发送线程
		break;
	case THE_CMD_GET_GROSS:		// 获取产量数据, DTU主动发送
		DtuGrossFrame = *((LPGROSS_FRAME_INFO)(p_buff));
		DtuSendCmd = THE_CMD_GET_GROSS;
		m_bRecv = TRUE;
		m_bEchoPositive = FALSE;
		SetEvent(m_hComEvent);	// 启动发送线程
		break;
	default:
		break;
	}

	return TRUE;
}

// 通讯线程调用, 装载帧并发送
void CDtuBus::ComRealtimeLoad(void)
{
	int i;
	WORD offset;
	WORD point = 0;
	WORD crc, len;

	BYTE *buff;
	buff = new BYTE[256];
	
	buff[point++] = (BYTE)(THE_FRAME_HEAD >> 8);
	buff[point++] = (BYTE)(THE_FRAME_HEAD & 0x00ff);

	len = 19 + 1									// 19个固定字节+1字节窑号+温度*2+边温*2+压力*2+湿度*2+电机*2+7字节时间
		  + 1 + (DtuRealTimeFrame.TempNum * 2)
		  + 1 + (DtuRealTimeFrame.SideTempNum * 2)
		  + 1 + (DtuRealTimeFrame.PressNum * 2)
	      + 1 + (DtuRealTimeFrame.HumidityNum * 2) 
		  + 1 + (DtuRealTimeFrame.MotorNum * 2)
		  + 7;
	buff[point++] = (BYTE)(len >> 8);
	buff[point++] = (BYTE)(len & 0x00ff);

	buff[point++] = THE_CMD_GET_RTDAT;
	buff[point++] = 0x00;
	buff[point++] = 0x00;
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 24);
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 16);
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 8);
	buff[point++] = (BYTE)(m_PortPara.lLocalID & 0x000000ff);
	buff[point++] = 0x00;
	buff[point++] = 0x00;
	buff[point++] = 0x00;
	buff[point++] = 0x01;
	
	buff[point++] = DtuRealTimeFrame.TunnelKilnId + 1;			// 窑号

	buff[point++] = DtuRealTimeFrame.TempNum;				// 温度个数
	for (offset = point, i = 0; i < DtuRealTimeFrame.TempNum; i++, point = point + 2)
	{
		buff[offset + i * 2 + 0] = (BYTE)(DtuRealTimeFrame.TempBuff[i] >> 8);
		buff[offset + i * 2 + 1] = (BYTE)(DtuRealTimeFrame.TempBuff[i] & 0x00ff);
	}

	buff[point++] = DtuRealTimeFrame.SideTempNum;			// 边温个数
	for (offset = point, i = 0; i < DtuRealTimeFrame.SideTempNum; i++, point = point + 2)
	{
		buff[offset + i * 2 + 0] = (BYTE)(DtuRealTimeFrame.SideTempBuff[i] >> 8);
		buff[offset + i * 2 + 1] = (BYTE)(DtuRealTimeFrame.SideTempBuff[i] & 0x00ff);
	}

	buff[point++] = DtuRealTimeFrame.PressNum;				// 压力个数
	for (offset = point, i = 0; i < DtuRealTimeFrame.PressNum; i++, point = point + 2)
	{
		buff[offset + i * 2 + 0] = (BYTE)(DtuRealTimeFrame.PressBuff[i] >> 8);
		buff[offset + i * 2 + 1] = (BYTE)(DtuRealTimeFrame.PressBuff[i] & 0x00ff);
	}

	buff[point++] = DtuRealTimeFrame.HumidityNum;			// 湿度个数
	for (offset = point, i = 0; i < DtuRealTimeFrame.HumidityNum; i++, point = point + 2)
	{
		buff[offset + i * 2 + 0] = (BYTE)(DtuRealTimeFrame.HumidityBuff[i] >> 8);
		buff[offset + i * 2 + 1] = (BYTE)(DtuRealTimeFrame.HumidityBuff[i] & 0x00ff);
	}

	buff[point++] = DtuRealTimeFrame.MotorNum;			// 湿度个数
	for (offset = point, i = 0; i < DtuRealTimeFrame.MotorNum; i++, point = point + 2)
	{
		buff[offset + i * 2 + 0] = (BYTE)(DtuRealTimeFrame.MotorBuff[i] >> 8);
		buff[offset + i * 2 + 1] = (BYTE)(DtuRealTimeFrame.MotorBuff[i] & 0x00ff);
	}

	buff[point++] = (BYTE)(DtuRealTimeFrame.Year >> 8);
	buff[point++] = (BYTE)(DtuRealTimeFrame.Year & 0x00ff);
	buff[point++] = DtuRealTimeFrame.Month;
	buff[point++] = DtuRealTimeFrame.Day;
	buff[point++] = DtuRealTimeFrame.Hour;
	buff[point++] = DtuRealTimeFrame.Min;
	buff[point++] = DtuRealTimeFrame.Sec;
	crc = CRC16Make(buff, point);
	buff[point++] = (BYTE)(crc >> 8);
	buff[point++] = (BYTE)(crc & 0x00ff);
	buff[point++] = (BYTE)(THE_FRAME_REAR >> 8);
	buff[point++] = (BYTE)(THE_FRAME_REAR & 0x00ff);
	
	m_Port.WriteToPort(buff, point);

	delete [] buff;
}

void CDtuBus::ComGrossLoad(void)
{
	WORD point = 0;
	WORD crc;
	BYTE *buff;
	buff = new BYTE[128];
	
	buff[point++] = (BYTE)(THE_FRAME_HEAD >> 8);
	buff[point++] = (BYTE)(THE_FRAME_HEAD & 0x00ff);
	buff[point++] = 0x00;
	buff[point++] = 0x1c;								// 19个固定字节+1个字节窑号+1个字节方式+7个时间
	buff[point++] = THE_CMD_GET_GROSS;
	buff[point++] = 0x00;
	buff[point++] = 0x00;
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 24);
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 16);
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 8);
	buff[point++] = (BYTE)(m_PortPara.lLocalID & 0x000000ff);	// DTU编号
	buff[point++] = 0x00;
	buff[point++] = 0x00;
	buff[point++] = 0x00;
	buff[point++] = 0x01;										// 1代表通讯服务器	
	buff[point++] = DtuGrossFrame.TunnelKilnId + 1;
	buff[point++] = DtuGrossFrame.GrossType;
	buff[point++] = (BYTE)(DtuGrossFrame.Year >> 8);
	buff[point++] = (BYTE)(DtuGrossFrame.Year & 0x00ff);
	buff[point++] = DtuGrossFrame.Month;
	buff[point++] = DtuGrossFrame.Day;
	buff[point++] = DtuGrossFrame.Hour;
	buff[point++] = DtuGrossFrame.Min;
	buff[point++] = DtuGrossFrame.Sec;
	crc = CRC16Make(buff, point);
	buff[point++] = (BYTE)(crc >> 8);
	buff[point++] = (BYTE)(crc & 0x00ff);
	buff[point++] = (BYTE)(THE_FRAME_REAR >> 8);
	buff[point++] = (BYTE)(THE_FRAME_REAR & 0x00ff);

	m_Port.WriteToPort(buff, point);
	delete [] buff;
}

// 正响应回复
void CDtuBus::ComEchoPositive(BYTE cmd, WORD frame_num)
{
	WORD point = 0;
	WORD crc;
	BYTE *buff;
	buff = new BYTE[128];

	buff[point++] = (BYTE)(THE_FRAME_HEAD >> 8);
	buff[point++] = (BYTE)(THE_FRAME_HEAD & 0x00ff);
	buff[point++] = 0x00;
	buff[point++] = 0x14;
	buff[point++] = cmd | 0x80;
	buff[point++] = (BYTE)(frame_num >> 8);
	buff[point++] = (BYTE)(frame_num & 0x00ff);
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 24);
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 16);
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 8);
	buff[point++] = (BYTE)(m_PortPara.lLocalID & 0x000000ff);
	buff[point++] = (BYTE)(DtuSPortNum >> 24);
	buff[point++] = (BYTE)(DtuSPortNum >> 16);
	buff[point++] = (BYTE)(DtuSPortNum >> 8);
	buff[point++] = (BYTE)(DtuSPortNum & 0x000000ff);
	buff[point++] = 0x00;			// 正响应
	crc = CRC16Make(buff, point);
	buff[point++] = (BYTE)(crc >> 8);
	buff[point++] = (BYTE)(crc & 0x00ff);
	buff[point++] = (BYTE)(THE_FRAME_REAR >> 8);
	buff[point++] = (BYTE)(THE_FRAME_REAR & 0x00ff);

	m_Port.WriteToPort(buff, point);

	delete [] buff;
}

// 负响应回复
void CDtuBus::ComEchoNegitive(BYTE cmd, WORD frame_num)
{
	WORD point = 0;
	WORD crc;
	BYTE *buff;
	buff = new BYTE[128];
	
	buff[point++] = (BYTE)(THE_FRAME_HEAD >> 8);
	buff[point++] = (BYTE)(THE_FRAME_HEAD & 0x00ff);
	buff[point++] = 0x00;
	buff[point++] = 0x14;
	buff[point++] = cmd | 0x80;
	buff[point++] = (BYTE)(frame_num >> 8);
	buff[point++] = (BYTE)(frame_num & 0x00ff);
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 24);
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 16);
	buff[point++] = (BYTE)(m_PortPara.lLocalID >> 8);
	buff[point++] = (BYTE)(m_PortPara.lLocalID & 0x000000ff);
	buff[point++] = (BYTE)(DtuSPortNum >> 24);
	buff[point++] = (BYTE)(DtuSPortNum >> 16);
	buff[point++] = (BYTE)(DtuSPortNum >> 8);
	buff[point++] = (BYTE)(DtuSPortNum & 0x000000ff);
	buff[point++] = 0x00;			// 正响应
	crc = CRC16Make(buff, point);
	buff[point++] = (BYTE)(crc >> 8);
	buff[point++] = (BYTE)(crc & 0x00ff);
	buff[point++] = (BYTE)(THE_FRAME_REAR >> 8);
	buff[point++] = (BYTE)(THE_FRAME_REAR & 0x00ff);
	
	m_Port.WriteToPort(buff, point);

	delete [] buff;
}

//////////////////////////////////////////////////////////////////////////
//  线程函数: ComThread
//////////////////////////////////////////////////////////////////////////
UINT CDtuBus::ComThread(LPVOID pParam)
{
	BOOL  Res = FALSE;
	int   RecallCnt = 0;
	DWORD Event = 0, Event1 = 0;
	
	// Cast the void pointer passed to the thread back to a pointer of CModBus class
	CDtuBus *pDtuBus = (CDtuBus *)pParam;
	
	// Set the status variable in the dialog class to TRUE to indicate the thread is running.
	pDtuBus->m_bComThreadAlive = TRUE;	
	Sleep(3000);								// 等待其他线程启动
	
	for (;;)
	{
		// Main wait function.  This function will normally block the thread
		// until one of nine events occur that require action.
		Event = WaitForMultipleObjects(2, pDtuBus->m_hComEventArray, FALSE, INFINITE);
		
		switch (Event)
		{
		case 0:
			{
				// Shutdown event.  This is event zero so it will be
				// the highest priority and be serviced first.
				pDtuBus->m_bComThreadAlive = FALSE;
				
				// Kill this thread.  break is not needed, but makes me feel better.
				AfxEndThread(100);
				break;
			}
			
		case 1: // DTU通讯信号
			ResetEvent(pDtuBus->m_hComEvent);		// 复位信号量
			RecallCnt = THE_MAX_RECALL_CNT;			// 重试3次
			pDtuBus->m_bEchoPositive = FALSE;
			
			while(RecallCnt > 0)
			{
				if (pDtuBus->m_bLogin == FALSE)
				{
					RecallCnt = -3;
					// 先发一个消息通知上层发送的数据服务器已经未反馈或反馈负相应
					::SendMessage(pDtuBus->m_pOwner->GetSafeHwnd(), WM_DTU_TRANSMIT_ECHO, 
							      (WPARAM)pDtuBus->DtuSendCmd, (LPARAM)RecallCnt);
					TRACE("---未登录状态退出---\r\n");
					break;
				}

				if (pDtuBus->m_bRecv == TRUE)				// 当前处于接收状态
				{
					if (pDtuBus->DtuSendCmd == THE_CMD_GET_RTDAT)
					{
						pDtuBus->ComRealtimeLoad();			// 发送实时数据

						EnterCriticalSection(&pDtuBus->m_csComCommunicationSync);
						pDtuBus->m_bEchoPositive = FALSE;
                        LeaveCriticalSection(&pDtuBus->m_csComCommunicationSync);
						Res = TRUE;

						TRACE("---请求实时数据===>数据发出---\r\n");
					}
					else if (pDtuBus->DtuSendCmd == THE_CMD_GET_GROSS)
					{
						pDtuBus->ComGrossLoad();			// 发送产量数据

						EnterCriticalSection(&pDtuBus->m_csComCommunicationSync);
						pDtuBus->m_bEchoPositive = FALSE;
                        LeaveCriticalSection(&pDtuBus->m_csComCommunicationSync);
						Res = TRUE;

						TRACE("---发送产量数据===>数据发出---\r\n");
					}
					else
					{
						RecallCnt = -2;						// 表示命令不支持
						// 先发一个消息通知上层发送的数据服务器已经未反馈或反馈负相应
						::SendMessage(pDtuBus->m_pOwner->GetSafeHwnd(), WM_DTU_TRANSMIT_ECHO, 
							          (WPARAM)pDtuBus->DtuSendCmd, (LPARAM)RecallCnt);

						TRACE("---发送命令不支持退出---\r\n");
						break;
					}
				}
				else
				{
					RecallCnt = -1;							// 表示因为某种原因未发送数据
					// 先发一个消息通知上层发送的数据服务器已经未反馈或反馈负相应
					::SendMessage(pDtuBus->m_pOwner->GetSafeHwnd(), WM_DTU_TRANSMIT_ECHO, 
							      (WPARAM)pDtuBus->DtuSendCmd, (LPARAM)RecallCnt);

					TRACE("---未处于空闲接收状态退出---\r\n");
					break;
				}

				if ((Res == TRUE) && (RecallCnt > 0))
				{
					Event1 = WaitForMultipleObjects(2, pDtuBus->m_hComEventArray, FALSE, pDtuBus->m_PortPara.lDtuTimeout);
					if (Event1 == 0)	// shutdown event						
					{
						// Shutdown event.  This is event zero so it will be
						// the highest priority and be serviced first.
						pDtuBus->m_bComThreadAlive = FALSE;
						
						// Kill this thread.  break is not needed, but makes me feel better.
						AfxEndThread(100);
					}
					else if (Event1 == 1)	// recv event
					{
						ResetEvent(pDtuBus->m_hComEvent);		// 复位信号量
						
						if (pDtuBus->m_bEchoPositive == TRUE)	// 接收到正响应
						{
							break;								// 指示不用重试了
						}
						else
						{
							RecallCnt--;
							TRACE("---接收到负相应......---\r\n");
						}
					}
					else					// recv timeout
					{
						ResetEvent(pDtuBus->m_hComEvent);		// 复位信号量
						RecallCnt--;
						TRACE("---接收数据超时......---\r\n");
					}
				}// end if
			}// end while

			EnterCriticalSection(&pDtuBus->m_csComCommunicationSync);
			pDtuBus->m_bRecv = FALSE;
            LeaveCriticalSection(&pDtuBus->m_csComCommunicationSync);

			if ((pDtuBus->m_bEchoPositive == TRUE) && (RecallCnt > 0))
			{				
				// 先发一个消息通知上层发送的数据服务器已经收到
				::SendMessage(pDtuBus->m_pOwner->GetSafeHwnd(), WM_DTU_TRANSMIT_ECHO, (WPARAM)pDtuBus->DtuSendCmd, (LPARAM) 1);
			}
			else
			{
				EnterCriticalSection(&pDtuBus->m_csComCommunicationSync);
				pDtuBus->m_bLogin = FALSE;								// 重新进行登录操作
				LeaveCriticalSection(&pDtuBus->m_csComCommunicationSync);

				pDtuBus->LoginRequest();								// 重新触发登录线程
				TRACE("---重新发送登录请求---\r\n");

				// 先发一个消息通知上层发送的数据服务器已经未反馈或反馈负相应
				::SendMessage(pDtuBus->m_pOwner->GetSafeHwnd(), WM_DTU_TRANSMIT_ECHO, (WPARAM)pDtuBus->DtuSendCmd, (LPARAM)RecallCnt);
			}
			
			break;
		default:
			break;
		} // end switch
	}
	
	return 0;
}

LONG CDtuBus::OnComm(WPARAM ch, LPARAM port)
{
	if (!m_bRecv || !m_bOpen)
	{
		return -1;
	}

	if (port < 1 || port > 30)
	{
		return -1;
	}

	BYTE recv_ch = ch;

	switch(DtuRecvProc)
	{
	case DTU_RECV_HEAD:
		DtuRecvHR[1] = recv_ch;
		if ((DtuRecvHR[0] == (BYTE)(THE_FRAME_HEAD >> 8)) && (DtuRecvHR[1] == (BYTE)(THE_FRAME_HEAD & 0x00ff)))
		{
			DtuRecvBuffer[0] = DtuRecvHR[0];
			DtuRecvBuffer[1] = DtuRecvHR[1];
			DtuRecvPoint     = 0;
			DtuRecvProc      = DTU_RECV_LEN;
		}
		else
		{
			DtuRecvHR[0] = DtuRecvHR[1];
		}
		break;
	case DTU_RECV_LEN:
		if (DtuRecvPoint == 0)
		{
			DtuRecvBuffer[2] = recv_ch;
			DtuRecvLen       = (WORD)recv_ch;
			DtuRecvLen       = DtuRecvLen << 8;
			DtuRecvPoint++;
		}
		else
		{
			DtuRecvLen |= ((WORD)(recv_ch) & 0x00ff);
			if (DtuRecvLen <= THE_MAX_RECV_BUFF)
			{
				DtuRecvBuffer[3] = recv_ch;
				DtuRecvProc      = DTU_RECV_CMD;
			}
			else
			{
				DtuRecvHR[0] = 0xff;
				DtuRecvHR[1] = 0xff;
				DtuRecvProc = DTU_RECV_HEAD;
			}
		}
		break;
	case DTU_RECV_CMD:
		DtuRecvCmd       = recv_ch & 0x7f;
		DtuRecvBuffer[4] = recv_ch & 0x7f;
		DtuRecvPoint     = 0;
		DtuRecvProc      = DTU_RECV_FNUM;
		break;
	case DTU_RECV_FNUM:
		if (DtuRecvPoint == 0)
		{
			DtuRecvBuffer[5] = recv_ch;
			DtuRecvFNum      = (WORD)recv_ch;
			DtuRecvFNum      = DtuRecvFNum << 8;
			DtuRecvPoint++;
		}
		else
		{
			DtuRecvFNum |= ((WORD)(recv_ch) & 0x00ff);

			DtuRecvBuffer[6] = recv_ch;
			DtuRecvPoint     = 0;
			DtuRecvProc      = DTU_RECV_SPORT;
		}
		break;
	case DTU_RECV_SPORT:
		if (DtuRecvPoint == 0)
		{
			DtuRecvBuffer[7] = recv_ch;
			DtuSPortNum      = ((DWORD)(recv_ch) << 24);
			DtuRecvPoint++;
		}
		else if (DtuRecvPoint == 1)
		{
			DtuRecvBuffer[8] = recv_ch;
			DtuSPortNum     |= ((DWORD)(recv_ch) << 16);
			DtuRecvPoint++;
		}
		else if (DtuRecvPoint == 2)
		{
			DtuRecvBuffer[9] = recv_ch;
			DtuSPortNum     |= ((DWORD)(recv_ch) << 8);
			DtuRecvPoint++;
		}
		else
		{
			DtuSPortNum |= ((DWORD)(recv_ch) & 0x00ff);
			if (DtuSPortNum == 1)				// 只接收服务器的命令
			{
				DtuRecvBuffer[10] = recv_ch;
				DtuRecvPoint      = 0;
				DtuRecvProc       = DTU_RECV_RPORT;
			}
			else if ((DtuSPortNum >= 100) || (DtuSPortNum <= 9999))
			{
				DtuRecvBuffer[10] = recv_ch;
				DtuRecvPoint      = 0;
				DtuRecvProc       = DTU_RECV_RPORT;
			}
			else
			{
				DtuRecvHR[0] = 0xff;
				DtuRecvHR[1] = 0xff;
				DtuRecvProc = DTU_RECV_HEAD;
			}
		}
		break;
	case DTU_RECV_RPORT:
		if (DtuRecvPoint == 0)
		{
			DtuRecvBuffer[11] = recv_ch;
			DtuRPortNum       = ((DWORD)(recv_ch) << 24);
			DtuRecvPoint++;
		}
		else if (DtuRecvPoint == 1)
		{
			DtuRecvBuffer[12] = recv_ch;
			DtuRPortNum      |= ((DWORD)(recv_ch) << 16);
			DtuRecvPoint++;
		}
		else if (DtuRecvPoint == 2)
		{
			DtuRecvBuffer[13] = recv_ch;
			DtuRPortNum      |= ((DWORD)(recv_ch) << 8);
			DtuRecvPoint++;
		}
		else
		{
			DtuRPortNum |= ((DWORD)(recv_ch) & 0x00ff);
			if (DtuRPortNum == m_PortPara.lLocalID)		// 本机ID
			{
				if (DtuRecvLen > 19)
				{
					DtuRecvBuffer[14] = recv_ch;
					DtuRecvPoint      = 0;
					DtuRecvProc       = DTU_RECV_DAT;
				}
				else
				{
					DtuRecvPoint = 0;
					DtuRecvProc  = DTU_RECV_CRC;
				}
			}
			else
			{
				DtuRecvHR[0] = 0xff;
				DtuRecvHR[1] = 0xff;
				DtuRecvProc = DTU_RECV_HEAD;
			}
		}
		break;
	case DTU_RECV_DAT:
		DtuRecvBuffer[15 + DtuRecvPoint] = recv_ch;
		DtuRecvPoint++;
		if (DtuRecvPoint >= (DtuRecvLen - 19))
		{
			DtuRecvPoint = 0;
			DtuRecvProc  = DTU_RECV_CRC;
		}
		break;
	case DTU_RECV_CRC:
		if (DtuRecvPoint == 0)
		{
			DtuCRC = ((WORD)(recv_ch) << 8);
			DtuRecvPoint++;
		}
		else
		{
			DtuCRC |= ((WORD)(recv_ch) & 0x00ff);
			DtuRecvHR[0] = 0xff;
			DtuRecvHR[1] = 0xff;
			DtuRecvPoint = 0;
			DtuRecvProc  = DTU_RECV_REAR;
		}
		break;
	case DTU_RECV_REAR:
		DtuRecvHR[DtuRecvPoint] = recv_ch;
		DtuRecvPoint++;
		if (DtuRecvPoint >= 2)
		{
			if ((DtuRecvHR[0] == (BYTE)(THE_FRAME_REAR >> 8)) && (DtuRecvHR[1] == (BYTE)(THE_FRAME_REAR & 0x00ff)))
			{
				//////////////////////////////////////////////////////////////////////////
				// 校验处理
				//////////////////////////////////////////////////////////////////////////

				switch(DtuRecvCmd)
				{
				case THE_CMD_LOGNIN:
					if (DtuRecvBuffer[15] == 0x00)	// 代表正响应
					{
						m_bEchoPositive = TRUE;
					}
					else
					{
						m_bEchoPositive = FALSE;
					}
					SetEvent(m_hLoginEvent);			// 除服通讯线程, 反馈接收反馈
					break;

				case THE_CMD_GET_RTDAT:
				case THE_CMD_GET_GROSS:
					if (DtuRecvBuffer[15] == 0x00)	// 代表正响应
					{
						m_bEchoPositive = TRUE;
					}
					else
					{
						m_bEchoPositive = FALSE;
					}
					
					SetEvent(m_hComEvent);			// 除服通讯线程, 反馈接收反馈
					break;

				case THE_CMD_SET_DEAD:
					ComEchoPositive(DtuRecvCmd, DtuRecvFNum);		// 发送正响应
					// 先发一个消息通知上层收到服务器数据
					::SendMessage(m_pOwner->GetSafeHwnd(), WM_DTU_TRANSMIT_ECHO, (WPARAM)DtuRecvCmd, (LPARAM) 1);
					break;
				case THE_CMD_SET_LIVE:
					ComEchoPositive(DtuRecvCmd, DtuRecvFNum);		// 发送正响应
					// 先发一个消息通知上层收到服务器数据
					::SendMessage(m_pOwner->GetSafeHwnd(), WM_DTU_TRANSMIT_ECHO, (WPARAM)DtuRecvCmd, (LPARAM) 0);
					break;
				case THE_CMD_SET_PARA:
					ComEchoPositive(DtuRecvCmd, DtuRecvFNum);		// 发送正响应
					// 先发一个消息通知上层收到服务器数据
					::SendMessage(m_pOwner->GetSafeHwnd(), WM_DTU_TRANSMIT_ECHO, (WPARAM)DtuRecvCmd, (LPARAM)(&DtuRecvBuffer[15]));
					break;
				case THE_CMD_SET_ADJ:
					ComEchoPositive(DtuRecvCmd, DtuRecvFNum);		// 发送正响应
					// 先发一个消息通知上层收到服务器数据
					::SendMessage(m_pOwner->GetSafeHwnd(), WM_DTU_TRANSMIT_ECHO, (WPARAM)DtuRecvCmd, (LPARAM)(&DtuRecvBuffer[15]));
					break;
				case THE_CMD_SET_MOTOR:
					ComEchoPositive(DtuRecvCmd, DtuRecvFNum);		// 发送正响应
					// 先发一个消息通知上层收到服务器数据
					::SendMessage(m_pOwner->GetSafeHwnd(), WM_DTU_TRANSMIT_ECHO, (WPARAM)DtuRecvCmd, (LPARAM)(&DtuRecvBuffer[15]));
					break;
				default:
					ComEchoNegitive(DtuRecvCmd, DtuRecvFNum);		// 发送负响应
					break;
				}
			}

			DtuRecvHR[0] = 0xff;
			DtuRecvHR[1] = 0xff;
			DtuRecvProc = DTU_RECV_HEAD;
		}
		break;
	default:
		break;
	}
	
	return 0;
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
unsigned int CDtuBus::CRC16Make(unsigned char *pucFrame, unsigned int usLen)
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

WORD CDtuBus::WordSwitch(WORD Value)
{
	WORD val = 0;
	
	val = Value & 0x00ff;
	val = val << 8;
	val = val | (Value >> 8);
	
	return val;
}

DWORD CDtuBus::DWordSwitch(DWORD Value)
{
	DWORD val = 0;
	
	((BYTE *)(&val))[3] = (BYTE)(Value >> 24);
	((BYTE *)(&val))[2] = (BYTE)(Value >> 16);
	((BYTE *)(&val))[1] = (BYTE)(Value >> 8);
	((BYTE *)(&val))[0] = (BYTE)(Value & 0x000000ff);
	
	return val;
}

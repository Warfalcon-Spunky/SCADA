// iComAPI.cpp: implementation of the CiComAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "iComAPI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CiComAPI::CiComAPI()
{
}

CiComAPI::CiComAPI(CWnd *pOwner,
				   int iComPort, char cPrity, int iBaudrate, int iDataBits, int iStopBits, int iTimeout, int iScanTime, int DevNum,				   
				   int iDtuPort, char cDtuPrity, int iDtuBaudrate, int iDtuDataBits, int iDtuStopBits, DWORD lLoginInternal, DWORD lCycleInternal, DWORD lDtuTimeout, DWORD lLocalID,
				   int iKilnNum)
{
	RegisterWindowClass();

	m_pOwner         = pOwner;
	m_iComPort       = iComPort;
	m_cPrity         = cPrity;
	m_iBaudrate      = iBaudrate;
	m_iDataBits      = iDataBits;
	m_iStopBits      = iStopBits;
	m_iTimeout       = iTimeout;
	m_iScanTime      = iScanTime;

	m_DevNum = DevNum;						// �豸����
	if (m_DevNum <= 0)
	{
		m_pTemObjectBuff = NULL;
	}
	else
	{
		m_pTemObjectBuff = new TemObject[m_DevNum];		// �����豸��Ϣ������
		for (int i = 0; i < m_DevNum; i++)
		{
			m_pTemObjectBuff[i].pTemRegBuff = NULL;
		}
	}

	m_iDtuPort       = iDtuPort;
	m_cDtuPrity      = cDtuPrity; 
	m_iDtuBaudrate   = iDtuBaudrate; 
	m_iDtuDataBits   = iDtuDataBits; 
	m_iDtuStopBits   = iDtuStopBits; 
	m_lLoginInternal = lLoginInternal; 
	m_lCycleInternal = lCycleInternal;
	m_lDtuTimeout    = lDtuTimeout;
	m_lLocalID       = lLocalID;						// DTU�Ķ˿ں�

	m_iKilnNum = iKilnNum;

	m_pModBus = NULL;
	m_pDtuBus = NULL;

	m_hManageShutdownEvent = NULL;
	m_hManageEvent = NULL;

	IsLockDtuTransmint = FALSE;

	m_bManageThreadAlive = FALSE;
}

CiComAPI::~CiComAPI()
{
}

BEGIN_MESSAGE_MAP(CiComAPI, CWnd)
	//{{AFX_MSG_MAP(CModBus)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_MESSAGE(WM_UPDATE_TERMINAL, OnUpdateTerminal)
		ON_MESSAGE(WM_DTU_TRANSMIT_ECHO, OnDtuTransmitEcho)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Register the window class if it has not already been registered.
BOOL CiComAPI::RegisterWindowClass()
{
    WNDCLASS wndcls;
    //HINSTANCE hInst = AfxGetInstanceHandle();
    HINSTANCE hInst = AfxGetResourceHandle();
	
    if (!(::GetClassInfo(hInst, ICOMAPI_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = 0;
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = ICOMAPI_CLASSNAME;
		
        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }
	
    return TRUE;
}

BOOL CiComAPI::ImportDevInfo(int TemIdx, BYTE TemId, BYTE TemCmd, BYTE TemChn, WORD TemAddr, BYTE TemTyp)
{
	if (TemIdx > m_DevNum)
	{
		return FALSE;
	}

	m_pTemObjectBuff[TemIdx].TemId   = TemId;
	m_pTemObjectBuff[TemIdx].TemCmd  = TemCmd;
	m_pTemObjectBuff[TemIdx].TemChn  = TemChn;
	m_pTemObjectBuff[TemIdx].TemAddr = TemAddr;
	m_pTemObjectBuff[TemIdx].TemTyp  = TemTyp;

	return TRUE;
}

BOOL CiComAPI::Create(CWnd* pParentWnd, UINT nID, DWORD dwStyle)
{
	assert(pParentWnd->GetSafeHwnd());
	m_pParentWnd = pParentWnd;				// �洢������ָ��
	
    if (!CWnd::Create(ICOMAPI_CLASSNAME, NULL, dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID))
	{
        return FALSE;
	}

	/* ������ô˺���ǰ, ȷ�ϸ�ֵ�豸��Ϣ */
	for (int i = 0; i < m_DevNum; i++)
	{
		/* �����豸������ */
		if ((m_pTemObjectBuff[i].TemChn > 0) && (m_pTemObjectBuff[i].pTemRegBuff == NULL))
		{
			m_pTemObjectBuff[i].pTemRegBuff = new WORD[m_pTemObjectBuff[i].TemChn];
		}
	}

	m_pModBus = new CModBus(this, 
							m_iComPort,				// �˿ں�
							m_iBaudrate,			// ������
							m_cPrity,				// У�鷽ʽ
							m_iDataBits,			// ����λ��
							m_iStopBits,			// ֹͣλ��
							1024,					// ����������
							m_iTimeout,				// ��ʱʱ��
							m_iScanTime,			// ɨ����ʱ��
						    m_DevNum,				// �ն�����
							m_pTemObjectBuff);		
	
	if (m_pModBus->Create(this, 9900, WS_CHILD) == FALSE)
	{		
		return FALSE;
	}

	m_pDtuBus = new CDtuBus(this, 
							m_iDtuPort,
							m_iDtuBaudrate, 
							m_cDtuPrity, 
							m_iDtuDataBits, 
							m_iDtuStopBits, 
							1024, 
							m_lLoginInternal, 
							m_lCycleInternal,
							m_lDtuTimeout,
							m_lLocalID);

	if (m_pDtuBus->Create(this, 9901, WS_CHILD) == FALSE)
	{		
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// ����ͨѶ�߳̾��
	//////////////////////////////////////////////////////////////////////////
	if (m_hManageShutdownEvent != NULL)
	{
		ResetEvent(m_hManageShutdownEvent);
	}
	else
	{
		m_hManageShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	
	if (m_hManageEvent != NULL)
	{
		ResetEvent(m_hManageEvent);
	}
	else
	{
		m_hManageEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	// ��ʼ��ͨѶ�߳̾������
	m_hManageEventArray[0] = m_hManageShutdownEvent;	// ������ȼ�
	m_hManageEventArray[1] = m_hManageEvent;
	
	// ����ͨѶ�߳�
	if (!(m_ManageThread = AfxBeginThread(ManageThread, this)))
	{
		return FALSE;
	}
	TRACE("Manager Thread started\n");
	
	// ��ʼ���ٽ�������
	InitializeCriticalSection(&m_csCommunicationSync);

	return TRUE;
}

// ��Ϣ����
LONG CiComAPI::OnUpdateTerminal(WPARAM index, LPARAM len)
{
	if (m_pModBus->GetRegisterValue() == TRUE)
	{
		// һ��ɨ�����, ������Ϣ������
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_RX_TERMINAL, (WPARAM) 0, (LPARAM) 0);
		return 1;
	}

	return 0;
}

void CiComAPI::SetWriteTerminalInfo(int iTemIdx, WORD iTemWriteAddr, WORD iTemWriteRegs, WORD *pTemRegBuff)
{
	m_pModBus->SetWriteTerminalInfo(iTemIdx, iTemWriteAddr, iTemWriteRegs, pTemRegBuff);
}

// �ⲿ���û�ȡ��¼״̬
BOOL CiComAPI::GetDtuLoginStatus(void)
{
	if (m_pDtuBus != NULL)
	{
		return m_pDtuBus->m_bLogin;
	}
	else
	{
		return FALSE;
	}
}

// �ⲿ���û�ȡ����״̬
BOOL CiComAPI::GetDtuEchoStatus(void)
{
	return m_pDtuBus->m_bEchoPositive;
}

// DTU�̷߳�����ͨѶ���, �յ�����Ϣ�Ϳ��Խ���߳�������־��IsLockDtuTransmint��
LONG CiComAPI::OnDtuTransmitEcho(WPARAM cmd, LPARAM res)
{
	BYTE echo_cmd = (BYTE)cmd;
	GROSS_FRAME_INFO gross_frame_buffer;

	switch(echo_cmd)
	{
	case THE_CMD_LOGNIN:			// ��¼������DTU�߳��Լ�����, ������ִ�����
		break;

	case THE_CMD_GET_RTDAT:			// ʵʱ����ʧ��, �������κδ���
		IsLockDtuTransmint = FALSE;
		break;

	case THE_CMD_GET_GROSS:			// ��������
		if (res == 1)				// �յ�����Ӧ����Ƴ�һ����������
		{
			vector <GROSS_FRAME_INFO>::iterator it = GrossFrameBuffer.begin();

			gross_frame_buffer.TunnelKilnId = it->TunnelKilnId;
			gross_frame_buffer.GrossType    = it->GrossType;
			gross_frame_buffer.Year  = it->Year;
			gross_frame_buffer.Month = it->Month;
			gross_frame_buffer.Day   = it->Day;
			gross_frame_buffer.Hour  = it->Hour;
			gross_frame_buffer.Min   = it->Min;
			gross_frame_buffer.Sec   = it->Sec;	
			// һ��ɨ�����, ������Ϣ������
			::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_RX_TERMINAL, (WPARAM) 1, (LPARAM)(&gross_frame_buffer));
			
			GrossFrameBuffer.erase(GrossFrameBuffer.begin());
		}
		
		IsLockDtuTransmint = FALSE;
		break;

	case THE_CMD_SET_DEAD:
		// һ��ɨ�����, ������Ϣ������
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_RX_TERMINAL, (WPARAM) 3, (LPARAM)(0));
		break;

	case THE_CMD_SET_LIVE:
		// һ��ɨ�����, ������Ϣ������
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_RX_TERMINAL, (WPARAM) 3, (LPARAM)(1));
		break;

	case THE_CMD_SET_MOTOR:
		// һ��ɨ�����, ������Ϣ������											// ��������ַ��Ҥ�� ���ID ��Ƶ��ֵ
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_RX_TERMINAL, (WPARAM) 2, (LPARAM)(res));
		break;

	case THE_CMD_SET_PARA: // OnRXTerminal()
		// һ��ɨ�����, ������Ϣ������											// ��������ַ��Ҥ�� ����1 ����2....
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_RX_TERMINAL, (WPARAM) 4, (LPARAM)(res));
		break;

	default:
		break;
	}

	return 0;
}

// �ⲿ�����ϴ�ʵʱ����
BOOL CiComAPI::SetRealTimeTransmit(	BYTE TunnelKilnId,
									BYTE TempNum, WORD *p_TempBuff,				// ÿ��Ҥ��������¶ȵ�Ϊ64��
									BYTE SideTempNum, WORD *p_SideTempBuff,		// ÿ��Ҥ�������¶ȵ�Ϊ32��
									BYTE PressNum, WORD *p_PressBuff,			// ÿ��Ҥ�����ѹ����Ϊ16��
									BYTE HumidityNum, WORD *p_HumidityBuff,		// ÿ��Ҥ�����ѹ����Ϊ8��
									BYTE MotorNum, WORD *p_MotorBuff,			// ÿ��Ҥ���������Ϊ8��
									WORD Year, BYTE Month, BYTE Day, BYTE Hour, BYTE Min, BYTE Sec)
{
	if (TunnelKilnId >= 10)			// ���10��Ҥ
	{
		return FALSE;
	}

	int i;

	RealTimeFrameBuffer[TunnelKilnId].TunnelKilnId = TunnelKilnId;

	RealTimeFrameBuffer[TunnelKilnId].TempNum = TempNum;
	for (i = 0; i < TempNum; i++)
	{
		RealTimeFrameBuffer[TunnelKilnId].TempBuff[i] = p_TempBuff[i];
	}

	RealTimeFrameBuffer[TunnelKilnId].SideTempNum = SideTempNum;
	for (i = 0; i < SideTempNum; i++)
	{
		RealTimeFrameBuffer[TunnelKilnId].SideTempBuff[i] = p_SideTempBuff[i];
	}

	RealTimeFrameBuffer[TunnelKilnId].PressNum = PressNum;
	for (i = 0; i < PressNum; i++)
	{
		RealTimeFrameBuffer[TunnelKilnId].PressBuff[i] = p_PressBuff[i];
	}

	RealTimeFrameBuffer[TunnelKilnId].HumidityNum = HumidityNum;
	for (i = 0; i < HumidityNum; i++)
	{
		RealTimeFrameBuffer[TunnelKilnId].HumidityBuff[i] = p_HumidityBuff[i];
	}

	RealTimeFrameBuffer[TunnelKilnId].MotorNum = MotorNum;
	for (i = 0; i < MotorNum; i++)
	{
		RealTimeFrameBuffer[TunnelKilnId].MotorBuff[i] = p_MotorBuff[i];
	}
	
	RealTimeFrameBuffer[TunnelKilnId].Year  = Year;
	RealTimeFrameBuffer[TunnelKilnId].Month = Month;
	RealTimeFrameBuffer[TunnelKilnId].Day   = Day;
	RealTimeFrameBuffer[TunnelKilnId].Hour  = Hour;
	RealTimeFrameBuffer[TunnelKilnId].Min   = Min;
	RealTimeFrameBuffer[TunnelKilnId].Sec   = Sec;
	return TRUE;
}

// �ⲿ�����ϴ���������
BOOL CiComAPI::SetGrossTransmit(BYTE TunnelKilnId,
								BYTE GrossType,
								WORD Year, BYTE Month, BYTE Day, BYTE Hour, BYTE Min, BYTE Sec)
{
	GROSS_FRAME_INFO gross_frame_buffer;
	gross_frame_buffer.TunnelKilnId = TunnelKilnId;
	gross_frame_buffer.GrossType    = GrossType;
	gross_frame_buffer.Year  = Year;
	gross_frame_buffer.Month = Month;
	gross_frame_buffer.Day   = Day;
	gross_frame_buffer.Hour  = Hour;
	gross_frame_buffer.Min   = Min;
	gross_frame_buffer.Sec   = Sec;

	GrossFrameBuffer.push_back(gross_frame_buffer);
	return TRUE;
}

void CiComAPI::DestroyWindows()
{
	m_pModBus->DestroyWindows();

	if (m_pTemObjectBuff != NULL)
	{
		for (int i = 0; i < m_DevNum; i++)
		{
			if (m_pTemObjectBuff[i].pTemRegBuff != NULL)
			{
				delete [] m_pTemObjectBuff[i].pTemRegBuff;
			}
		}

		delete [] m_pTemObjectBuff;
	}
	
	if (m_pModBus != NULL)
	{
		delete m_pModBus;
	}

	m_pDtuBus->DestroyWindows();

	if (m_pDtuBus != NULL)
	{
		delete m_pDtuBus;
	}

	//////////////////////////////////////////////////////////////////////////
	// �ͷ��߳̾��
	//////////////////////////////////////////////////////////////////////////
	MSG manage_message;  
    do  
    {  
		SetEvent(m_hManageShutdownEvent);  
		if(::PeekMessage(&manage_message, m_pOwner->m_hWnd, 0, 0, PM_REMOVE))  
		{  
			::TranslateMessage(&manage_message);  
			::DispatchMessage(&manage_message);  
		}  
		
	} while (m_bManageThreadAlive == TRUE);
	
	if(m_hManageShutdownEvent != NULL)
	{
		CloseHandle(m_hManageShutdownEvent);
	}
	
	if(m_hManageEvent != NULL)
	{
		CloseHandle(m_hManageEvent);
	}
	TRACE("Manage Thread ended\n");
}

//////////////////////////////////////////////////////////////////////////
//  �̺߳���: LoginThread
//////////////////////////////////////////////////////////////////////////
UINT CiComAPI::ManageThread(LPVOID pParam)
{
	DWORD Event = 0;
	DWORD Timeout = THE_CYCLE_INTERNAL;		// Ĭ��5���ʵʱ���ݷ�������
	int   KilnPoint = 0;
	
	// Cast the void pointer passed to the thread back to a pointer of CModBus class
	CiComAPI *pComAPI = (CiComAPI *)pParam;
	
	// Set the status variable in the dialog class to TRUE to indicate the thread is running.
	pComAPI->m_bManageThreadAlive = TRUE;	
	
	for (;;)
	{
		// Main wait function.  This function will normally block the thread
		// until one of nine events occur that require action.
		Event = WaitForMultipleObjects(2, pComAPI->m_hManageEventArray, FALSE, Timeout);

		switch (Event)
		{
		case 0:
			{
				// Shutdown event.  This is event zero so it will be
				// the highest priority and be serviced first.
				pComAPI->m_bManageThreadAlive = FALSE;
				
				// Kill this thread.  break is not needed, but makes me feel better.
				AfxEndThread(100);
				break;
			}
			
		case 1:		// �����߳�
			ResetEvent(pComAPI->m_hManageEvent);		// ��λ�ź���
			break;

		default:	// ��ʱ�����߳�
			ResetEvent(pComAPI->m_hManageEvent);		// ��λ�ź���

			if (pComAPI->m_pDtuBus->m_bLogin == TRUE)
			{
				if (pComAPI->IsLockDtuTransmint == FALSE)	// ��ǰ�����߳̿���
				{
					if (pComAPI->GrossFrameBuffer.empty() == false)	// ���ȷ��Ͳ�������
					{
						// ���ͨѶ�̴߳������ڽ�����, ����ʱ����д��BUFFER�е�
						vector <GROSS_FRAME_INFO>::iterator it = pComAPI->GrossFrameBuffer.begin();
						if (pComAPI->m_pDtuBus->ComTransmit(THE_CMD_GET_GROSS, it) == TRUE)
						{
							EnterCriticalSection(&pComAPI->m_csCommunicationSync);
							pComAPI->IsLockDtuTransmint = TRUE;
                            LeaveCriticalSection(&pComAPI->m_csCommunicationSync);
							TRACE("�Ѿ����Ͳ�������, ��ʱ5S\r\n");
						}
					}
					else
					{
						// ���ͨѶ�̴߳������ڽ�����, ����ʱ����д��BUFFER�е�
						if (pComAPI->m_pDtuBus->ComTransmit(THE_CMD_GET_RTDAT, &pComAPI->RealTimeFrameBuffer[KilnPoint]) == TRUE)
						{
							KilnPoint = (KilnPoint + 1) % pComAPI->m_iKilnNum;
							EnterCriticalSection(&pComAPI->m_csCommunicationSync);
							pComAPI->IsLockDtuTransmint = TRUE;
                            LeaveCriticalSection(&pComAPI->m_csCommunicationSync);
							TRACE("�Ѿ�����ʵʱ����, ��ʱ5S\r\n");
						}
					}
					
					Timeout = THE_CYCLE_INTERNAL;	
				}	
// 				else
// 				{
// 					Timeout = 1000;							// ӦΪ�������޷����͵���������1���м�������
// 					TRACE("�ȴ�������������, ��ʱ1S");
// 				}
			}
			else
			{
				EnterCriticalSection(&pComAPI->m_csCommunicationSync);
				pComAPI->IsLockDtuTransmint = FALSE;         // ���������߳�
                LeaveCriticalSection(&pComAPI->m_csCommunicationSync);	
			}
			break;
		} // end switch
	}
	
	return 0;
}
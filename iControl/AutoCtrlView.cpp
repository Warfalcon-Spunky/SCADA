// AutoCtrlView.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "iControl.h"
#include "MainFrm.h"
#include "AutoCtrlView.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HIGH_TEMP_ALARM						// 是否需要添加高温报警
#define LOW_TEMP_ALARM						// 是否需要添加低温报警

/////////////////////////////////////////////////////////////////////////////
// CAutoCtrlView

IMPLEMENT_DYNCREATE(CAutoCtrlView, CView)

CAutoCtrlView::CAutoCtrlView()
{
	m_pAutoCtrlCntBuff = NULL;
	m_lAutoCtrlLogIdx  = 0;
}

CAutoCtrlView::~CAutoCtrlView()
{
}


BEGIN_MESSAGE_MAP(CAutoCtrlView, CView)
	//{{AFX_MSG_MAP(CAutoCtrlView)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoCtrlView drawing

void CAutoCtrlView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAutoCtrlView diagnostics

#ifdef _DEBUG
void CAutoCtrlView::AssertValid() const
{
	CView::AssertValid();
}

void CAutoCtrlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAutoCtrlView message handlers

void CAutoCtrlView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	OpenFile(pDoc->m_strLogPath);
	m_hFile.SeekToEnd();
	m_hFile.WriteString("**************************************************************************************");
	m_hFile.WriteString("***********************************************************************\r\n");	  

	m_pAutoCtrlCntBuff = new int[pDoc->m_iKilnNum];
	for (int i = 0; i < pDoc->m_iKilnNum; i++)
	{
		m_pAutoCtrlCntBuff[i] = 0;
	}

	m_pAutoCtrlFlagCnt = new int[pDoc->m_iKilnNum];
	m_pFrontDec        = new int[pDoc->m_iKilnNum];
	m_pRearDec         = new int[pDoc->m_iKilnNum];
	m_pWarnTimerCnt    = new int[pDoc->m_iKilnNum];
	for (i = 0; i < pDoc->m_iKilnNum; i++)
	{
		m_pAutoCtrlFlagCnt[i] = 0;
		m_pFrontDec[i]        = 0;
		m_pRearDec[i]         = 0;
		m_pWarnTimerCnt[i]    = 0;
	}

	SetTimer(0, 1000, NULL);			// 打开1秒定时器, 作为控制周期定时器
}

void CAutoCtrlView::OnTimer(UINT nIDEvent) 
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	CString str;

	for (int i = 0; i < pDoc->m_iKilnNum; i++)
	{
		if (m_pWarnTimerCnt[i] > 0)
		{
			m_pWarnTimerCnt[i]--;
		}
	}

	for (i = 0; i < pDoc->m_iKilnNum; i++)
	{
		// 定时推车打铃功能
		if (pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iProcTimerCnt > 0)
		{
			pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iProcTimerCnt--;
		}
		else
		{			
			// 发送响铃命令
			int  tem_idx = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].TemBellIdx;
			int  buf_idx = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].BellIdx;
			WORD tmp     = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].BellValidTime;
			pMainFrame->m_pCom->SetWriteTerminalInfo(tem_idx, (buf_idx + 40312), 1, &tmp);
			
			// 重置最大打铃定时器时间, 在接到产量计数信号后更新为自动打铃间隔时间
			pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iProcTimerCnt = 60 * pDoc->m_iGrossProcMaxTime[i];
		}

		for (int j = 0; j < pDoc->m_pKilnBuff[i].TemperatureNum; j++)
		{
			if ((pDoc->m_pKilnBuff[i].m_pTempObjectBuff[j].Value >= -100) 
				&& (pDoc->m_pKilnBuff[i].m_pTempObjectBuff[j].Value <= 1370))
			{
				if (pDoc->m_pKilnBuff[i].m_pTempObjectBuff[j].Value > pDoc->m_iUpWarn[i])
				{
					if (m_pWarnTimerCnt[i] == 0)
					{
						str.Empty();
						str.Format("%d号窑:\r\n<<<<<高温报警>>>>>\r\n\r\n<<<<<高温报警>>>>>", i + 1);			
						pMainFrame->CreateGrossInfo(str);

						m_pWarnTimerCnt[i] = 300;				// 5分钟后再报警

#ifdef HIGH_TEMP_ALARM
						// 发送响铃命令（高温报警）
						int  tem_idx1 = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].TemBellIdx;
						int  buf_idx1 = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].BellIdx;
						WORD tmp1     = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].BellValidTime;
						// 两条窑的产量报警是DO0和DO1   高温报警是DO1和DO2
						pMainFrame->m_pCom->SetWriteTerminalInfo(tem_idx1, (buf_idx1 + 40312 + 2), 1, &tmp1);
#endif
					}
					break;
				}
			}
		}

		int k = 0;
		for (j = 0; j < pDoc->m_pKilnBuff[i].TemperatureNum; j++)
		{
			if ((pDoc->m_pKilnBuff[i].m_pTempObjectBuff[j].Value >= -100) 
				&& (pDoc->m_pKilnBuff[i].m_pTempObjectBuff[j].Value <= 1370))
			{
				if (pDoc->m_pKilnBuff[i].m_pTempObjectBuff[j].Value >= pDoc->m_iDnWarn[i])
				{
					k = 0;
					break;
				}
				else
				{
					k++;
				}
			}
		}

		if (k > 0)
		{
			if (m_pWarnTimerCnt[i] == 0)
			{
				str.Empty();
				str.Format("%d号窑:\r\n<<<<<低温报警>>>>>\r\n\r\n<<<<<低温报警>>>>>", i + 1);			
				pMainFrame->CreateGrossInfo(str);

				m_pWarnTimerCnt[i] = 300;				// 5分钟后再报警

#ifdef LOW_TEMP_ALARM
				// 发送响铃命令（低温报警）
				int  tem_idx2 = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].TemBellIdx;
				int  buf_idx2 = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].BellIdx;
				WORD tmp2     = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].BellValidTime;
				// 两条窑的产量报警是DO0和DO1   高低温报警是DO2和DO3
				pMainFrame->m_pCom->SetWriteTerminalInfo(tem_idx2, (buf_idx2 + 40312 + 2), 1, &tmp2);
#endif
			}
		}

		if ((pDoc->m_pKilnBuff[i].ControlNum > 0)		// 控制电机数大于0			
			&& (pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].bControl == TRUE))// 控制标志打开
		{
			// 如果推车等待计数器工作, 则需要等待指定时间后再进行自动控制, 输出保持不变
			if (pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iWaitTimerCnt > 0)
			{
				// 以便在已启动自动控制后, 进行一次自动控制
				m_pAutoCtrlCntBuff[i] = (pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iCtrlTime * 60) - 1;

				pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iWaitTimerCnt--;
				continue;
			}

			m_pAutoCtrlCntBuff[i] = (m_pAutoCtrlCntBuff[i] + 1) 
									% (60 * pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iCtrlTime);

			// 控制周期到
			if (m_pAutoCtrlCntBuff[i] == 0)
			{
				int res = AutoCtrlProc(i);
				if (res < 0)
				{
					if (res == -1)
					{
//						pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].bControl = FALSE;
						MessageBox("检测到有热电偶损坏, 请修复后再使用自动控制功能", "警告");
					}
					else if (res == -2)
					{
//						pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].bControl = FALSE;
						MessageBox("检测到最高温度位置不正确, 请人工烧结正常后再使用自动控制功能", "警告");
					}
				}
			}
		}
		else
		{
			// 以便在已启动自动控制后, 进行一次自动控制
			m_pAutoCtrlCntBuff[i] = (pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iCtrlTime * 60) - 1;

			// 推车等待计数器清零
			pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iWaitTimerCnt = 0;

			m_pAutoCtrlFlagCnt[i] = 0;
			m_pFrontDec[i]        = 0;
			m_pRearDec[i]         = 0;
		}
	}
	
	CView::OnTimer(nIDEvent);
}

// 自动控制流程
int CAutoCtrlView::AutoCtrlProc(int iKilnIdx)
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int max_car_loc = 0;
	int max_tmp     = pDoc->m_pKilnBuff[iKilnIdx].m_pTempObjectBuff[0].Value;
	for (int i = 1; i < pDoc->m_pKilnBuff[iKilnIdx].TemperatureNum; i++)
	{
		/* 正温热电偶必须全部正确, 否则不自动 */
		if ((pDoc->m_pKilnBuff[iKilnIdx].m_pTempObjectBuff[i].Value < (-100)) 
			|| (pDoc->m_pKilnBuff[iKilnIdx].m_pTempObjectBuff[i].Value > 1370))
		{
			CTime t = CTime::GetCurrentTime();
			CString str;
			str.Format("[Error] [%d号窑: %04d-%02d-%02d %02d:%02d:%02d 检查到%d号温度传感器错误退出自动控制流程]\r\n", 
														iKilnIdx + 1,
														t.GetYear(), 
														t.GetMonth(), 
														t.GetDay(), 
														t.GetHour(), 
														t.GetMinute(), 
														t.GetSecond(),
														i + 1);
			WriteString(str);
			return -1;
		}

		if (max_tmp < pDoc->m_pKilnBuff[iKilnIdx].m_pTempObjectBuff[i].Value)
		{
			max_tmp     = pDoc->m_pKilnBuff[iKilnIdx].m_pTempObjectBuff[i].Value;
			max_car_loc = i;
		}
	}

	/* 最高温度点位置判断, 不选满足设定的前后点要求, 否则不自动 */
	if ((max_car_loc < pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iFrontPoint) 
		|| ((max_car_loc + pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iRearPoint + 1) > pDoc->m_pKilnBuff[iKilnIdx].TemperatureNum))
	{
		CTime t = CTime::GetCurrentTime();
		CString str;
		str.Format("[Error] [%d号窑: %04d-%02d-%02d %02d:%02d:%02d 检查到最高温度点(%d车位)位置错误退出自动控制流程]\r\n", 
																iKilnIdx + 1,
																t.GetYear(), 
																t.GetMonth(), 
																t.GetDay(), 
																t.GetHour(), 
																t.GetMinute(), 
																t.GetSecond(),
																max_car_loc + 1);
		WriteString(str);
		return -2;
	}

	int front_tmp = 0;
	int front_tmps = 0;
	for (i = 0; i < (pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iFrontPoint + 1); i++)
	{
		front_tmp  += pDoc->m_pKilnBuff[iKilnIdx].m_pTempObjectBuff[max_car_loc - i].Value;
		front_tmps += pDoc->m_pKilnBuff[iKilnIdx].m_pTempObjectBuff[max_car_loc - i].SValue;
	}

	int rear_tmp  = 0;
	int rear_tmps = 0;
	for (i = 0; i < (pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iRearPoint + 1); i++)
	{
		rear_tmp  += pDoc->m_pKilnBuff[iKilnIdx].m_pTempObjectBuff[max_car_loc + i].Value;
		rear_tmps += pDoc->m_pKilnBuff[iKilnIdx].m_pTempObjectBuff[max_car_loc + i].SValue;
	}

	float out_adj;
	int front_dec = front_tmp - front_tmps;
	int rear_dec  = rear_tmp  - rear_tmps;
	bool trim = true;

	// 差值越来越大, 并且相同方向调节超过连续超过2次，则反方向调节
	if ((fabs(front_dec) > fabs(m_pFrontDec[iKilnIdx])) && (fabs(rear_dec) > fabs(m_pRearDec[iKilnIdx])))
	{
		if (fabs(m_pAutoCtrlFlagCnt[iKilnIdx]) > 2)
		{
			out_adj = (pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[0].Value / 10.0)
				+ (pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].IncOutCoeff * (m_pAutoCtrlFlagCnt[iKilnIdx] + 1) * (-1));

			m_pAutoCtrlFlagCnt[iKilnIdx] = 0;								// 下一次周期再回到正常调节
			trim = false;
		}
	}
	
	if (trim == true)
	{
		if (front_dec == 0 && front_dec == 0)					// 没有偏差直接退出
		{
			m_pAutoCtrlFlagCnt[iKilnIdx] = 0;
			return 0;
		}
		else if (front_dec >= 0 && rear_dec >= 0)				// 前后都超过设定温度则加大输出
		{
			m_pAutoCtrlFlagCnt[iKilnIdx]++;
			
			out_adj =(pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[0].Value / 10.0)	// 缓冲区里面的值放大了10倍
				+ pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].IncOutCoeff;	// 当前电机值作为初始值
			
		}
		else if (front_dec <= 0 && rear_dec <= 0)				// 前后都低于设定温度则减小输出
		{
			m_pAutoCtrlFlagCnt[iKilnIdx]--;

			out_adj = (pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[0].Value / 10.0) // 缓冲区里面的值放大了10倍
					  - pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].IncOutCoeff;	// 当前电机值作为初始值
		}
		else if (front_dec >= 0 && rear_dec <= 0)				// 前面超过 后面低于设定温度则减小输出, 并提近闸
		{
			m_pAutoCtrlFlagCnt[iKilnIdx]--;

			out_adj = (pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[0].Value / 10.0)	// 缓冲区里面的值放大了10倍
					  - pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].IncOutCoeff;	// 当前电机值作为初始值
		}
		else if (front_dec <= 0 && rear_dec >= 0)				// 前面低于 后面超过设定温度则差值相加: 正数增加 负数减小
		{
			m_pAutoCtrlFlagCnt[iKilnIdx]++;

			out_adj = (pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[0].Value / 10.0)	// 缓冲区里面的值放大了10倍
					  + pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].IncOutCoeff;
		}
		else													// 非正常情况输出最大值+最小值的1/2
		{
			m_pAutoCtrlFlagCnt[iKilnIdx] = 0;

			// 不累加, 直接输出
			out_adj = (pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].UpLimitOut 
					   - pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].DnLimitOut) / 2.0;
			out_adj += pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].DnLimitOut;
		}
	}

	m_pFrontDec[iKilnIdx] = front_dec;
	m_pRearDec[iKilnIdx]  = rear_dec;

	CTime t = CTime::GetCurrentTime();
	CString str;
	str.Format("[Procs] [%d号窑: %04d-%02d-%02d %02d:%02d:%02d 执行1次自动控制流程, 前和=%d 前标和=%d 后和=%d 后标和=%d, 当前输出=%3.2f 控制输出=%3.2f 上限输出=%3.2f 下限输出=%3.2f]\r\n", 
														iKilnIdx + 1,
														t.GetYear(), 
														t.GetMonth(), 
														t.GetDay(), 
														t.GetHour(), 
														t.GetMinute(), 
														t.GetSecond(),
														front_tmp, front_tmps,
														rear_tmp, rear_tmps,
														pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[0].Value / 10.0,
														out_adj,
														pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].UpLimitOut,
														pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].DnLimitOut);
	WriteString(str);

	// 限幅处理
	if (out_adj > pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].UpLimitOut)
	{
		out_adj = pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].UpLimitOut;
	}
	else if (out_adj < pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].DnLimitOut)
	{
		out_adj = pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].DnLimitOut;
	}

	int  DevIdx  = pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[0].TemOutIdx;
	WORD RegAddr = pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[0].BufOutIdx + 40001;
	WORD OutVal  = (WORD)(out_adj * 10.0);
	pMainFrame->m_pCom->SetWriteTerminalInfo(DevIdx, RegAddr, 1, &OutVal);

	return 0;
} 

void CAutoCtrlView::OpenFile(const char *pFileName)
{
	if (m_hFile.Open(pFileName, CFile::modeReadWrite, NULL) == FALSE)
	{
		MessageBox("LOG文件创建错误", "配置错误", MB_OK);
		m_bFileOpen = FALSE;
		return;
	}
	else
	{
		m_bFileOpen = TRUE;
	}
}

void CAutoCtrlView::CloseFile(void)
{
	if (m_bFileOpen == TRUE)
	{
		m_bFileOpen = FALSE;
		m_hFile.Close();
	}
}

void CAutoCtrlView::WriteString(const char *pString)
{
	if (m_bFileOpen == TRUE)
	{
		CString str;
		str.Format("[%ld] %s", m_lAutoCtrlLogIdx, pString);
		m_hFile.WriteString(str);

		m_lAutoCtrlLogIdx++;
	}
}

void CAutoCtrlView::OnDestroy() 
{
	CView::OnDestroy();

	CloseFile();
	
	KillTimer(0);
	
	delete [] m_pAutoCtrlCntBuff;
	delete [] m_pAutoCtrlFlagCnt;
	delete [] m_pFrontDec;
	delete [] m_pRearDec;
	delete [] m_pWarnTimerCnt;
}

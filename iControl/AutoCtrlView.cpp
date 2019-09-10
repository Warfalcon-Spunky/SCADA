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

#define HIGH_TEMP_ALARM						// �Ƿ���Ҫ��Ӹ��±���
#define LOW_TEMP_ALARM						// �Ƿ���Ҫ��ӵ��±���

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

	SetTimer(0, 1000, NULL);			// ��1�붨ʱ��, ��Ϊ�������ڶ�ʱ��
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
		// ��ʱ�Ƴ����幦��
		if (pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iProcTimerCnt > 0)
		{
			pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iProcTimerCnt--;
		}
		else
		{			
			// ������������
			int  tem_idx = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].TemBellIdx;
			int  buf_idx = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].BellIdx;
			WORD tmp     = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].BellValidTime;
			pMainFrame->m_pCom->SetWriteTerminalInfo(tem_idx, (buf_idx + 40312), 1, &tmp);
			
			// ���������嶨ʱ��ʱ��, �ڽӵ����������źź����Ϊ�Զ�������ʱ��
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
						str.Format("%d��Ҥ:\r\n<<<<<���±���>>>>>\r\n\r\n<<<<<���±���>>>>>", i + 1);			
						pMainFrame->CreateGrossInfo(str);

						m_pWarnTimerCnt[i] = 300;				// 5���Ӻ��ٱ���

#ifdef HIGH_TEMP_ALARM
						// ��������������±�����
						int  tem_idx1 = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].TemBellIdx;
						int  buf_idx1 = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].BellIdx;
						WORD tmp1     = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].BellValidTime;
						// ����Ҥ�Ĳ���������DO0��DO1   ���±�����DO1��DO2
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
				str.Format("%d��Ҥ:\r\n<<<<<���±���>>>>>\r\n\r\n<<<<<���±���>>>>>", i + 1);			
				pMainFrame->CreateGrossInfo(str);

				m_pWarnTimerCnt[i] = 300;				// 5���Ӻ��ٱ���

#ifdef LOW_TEMP_ALARM
				// ��������������±�����
				int  tem_idx2 = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].TemBellIdx;
				int  buf_idx2 = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].BellIdx;
				WORD tmp2     = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[0].BellValidTime;
				// ����Ҥ�Ĳ���������DO0��DO1   �ߵ��±�����DO2��DO3
				pMainFrame->m_pCom->SetWriteTerminalInfo(tem_idx2, (buf_idx2 + 40312 + 2), 1, &tmp2);
#endif
			}
		}

		if ((pDoc->m_pKilnBuff[i].ControlNum > 0)		// ���Ƶ��������0			
			&& (pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].bControl == TRUE))// ���Ʊ�־��
		{
			// ����Ƴ��ȴ�����������, ����Ҫ�ȴ�ָ��ʱ����ٽ����Զ�����, ������ֲ���
			if (pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iWaitTimerCnt > 0)
			{
				// �Ա����������Զ����ƺ�, ����һ���Զ�����
				m_pAutoCtrlCntBuff[i] = (pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iCtrlTime * 60) - 1;

				pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iWaitTimerCnt--;
				continue;
			}

			m_pAutoCtrlCntBuff[i] = (m_pAutoCtrlCntBuff[i] + 1) 
									% (60 * pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iCtrlTime);

			// �������ڵ�
			if (m_pAutoCtrlCntBuff[i] == 0)
			{
				int res = AutoCtrlProc(i);
				if (res < 0)
				{
					if (res == -1)
					{
//						pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].bControl = FALSE;
						MessageBox("��⵽���ȵ�ż��, ���޸�����ʹ���Զ����ƹ���", "����");
					}
					else if (res == -2)
					{
//						pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].bControl = FALSE;
						MessageBox("��⵽����¶�λ�ò���ȷ, ���˹��ս���������ʹ���Զ����ƹ���", "����");
					}
				}
			}
		}
		else
		{
			// �Ա����������Զ����ƺ�, ����һ���Զ�����
			m_pAutoCtrlCntBuff[i] = (pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iCtrlTime * 60) - 1;

			// �Ƴ��ȴ�����������
			pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iWaitTimerCnt = 0;

			m_pAutoCtrlFlagCnt[i] = 0;
			m_pFrontDec[i]        = 0;
			m_pRearDec[i]         = 0;
		}
	}
	
	CView::OnTimer(nIDEvent);
}

// �Զ���������
int CAutoCtrlView::AutoCtrlProc(int iKilnIdx)
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int max_car_loc = 0;
	int max_tmp     = pDoc->m_pKilnBuff[iKilnIdx].m_pTempObjectBuff[0].Value;
	for (int i = 1; i < pDoc->m_pKilnBuff[iKilnIdx].TemperatureNum; i++)
	{
		/* �����ȵ�ż����ȫ����ȷ, �����Զ� */
		if ((pDoc->m_pKilnBuff[iKilnIdx].m_pTempObjectBuff[i].Value < (-100)) 
			|| (pDoc->m_pKilnBuff[iKilnIdx].m_pTempObjectBuff[i].Value > 1370))
		{
			CTime t = CTime::GetCurrentTime();
			CString str;
			str.Format("[Error] [%d��Ҥ: %04d-%02d-%02d %02d:%02d:%02d ��鵽%d���¶ȴ����������˳��Զ���������]\r\n", 
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

	/* ����¶ȵ�λ���ж�, ��ѡ�����趨��ǰ���Ҫ��, �����Զ� */
	if ((max_car_loc < pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iFrontPoint) 
		|| ((max_car_loc + pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iRearPoint + 1) > pDoc->m_pKilnBuff[iKilnIdx].TemperatureNum))
	{
		CTime t = CTime::GetCurrentTime();
		CString str;
		str.Format("[Error] [%d��Ҥ: %04d-%02d-%02d %02d:%02d:%02d ��鵽����¶ȵ�(%d��λ)λ�ô����˳��Զ���������]\r\n", 
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

	// ��ֵԽ��Խ��, ������ͬ������ڳ�����������2�Σ��򷴷������
	if ((fabs(front_dec) > fabs(m_pFrontDec[iKilnIdx])) && (fabs(rear_dec) > fabs(m_pRearDec[iKilnIdx])))
	{
		if (fabs(m_pAutoCtrlFlagCnt[iKilnIdx]) > 2)
		{
			out_adj = (pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[0].Value / 10.0)
				+ (pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].IncOutCoeff * (m_pAutoCtrlFlagCnt[iKilnIdx] + 1) * (-1));

			m_pAutoCtrlFlagCnt[iKilnIdx] = 0;								// ��һ�������ٻص���������
			trim = false;
		}
	}
	
	if (trim == true)
	{
		if (front_dec == 0 && front_dec == 0)					// û��ƫ��ֱ���˳�
		{
			m_pAutoCtrlFlagCnt[iKilnIdx] = 0;
			return 0;
		}
		else if (front_dec >= 0 && rear_dec >= 0)				// ǰ�󶼳����趨�¶���Ӵ����
		{
			m_pAutoCtrlFlagCnt[iKilnIdx]++;
			
			out_adj =(pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[0].Value / 10.0)	// �����������ֵ�Ŵ���10��
				+ pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].IncOutCoeff;	// ��ǰ���ֵ��Ϊ��ʼֵ
			
		}
		else if (front_dec <= 0 && rear_dec <= 0)				// ǰ�󶼵����趨�¶����С���
		{
			m_pAutoCtrlFlagCnt[iKilnIdx]--;

			out_adj = (pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[0].Value / 10.0) // �����������ֵ�Ŵ���10��
					  - pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].IncOutCoeff;	// ��ǰ���ֵ��Ϊ��ʼֵ
		}
		else if (front_dec >= 0 && rear_dec <= 0)				// ǰ�泬�� ��������趨�¶����С���, �����բ
		{
			m_pAutoCtrlFlagCnt[iKilnIdx]--;

			out_adj = (pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[0].Value / 10.0)	// �����������ֵ�Ŵ���10��
					  - pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].IncOutCoeff;	// ��ǰ���ֵ��Ϊ��ʼֵ
		}
		else if (front_dec <= 0 && rear_dec >= 0)				// ǰ����� ���泬���趨�¶����ֵ���: �������� ������С
		{
			m_pAutoCtrlFlagCnt[iKilnIdx]++;

			out_adj = (pDoc->m_pKilnBuff[iKilnIdx].m_pMotorObjectBuff[0].Value / 10.0)	// �����������ֵ�Ŵ���10��
					  + pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].IncOutCoeff;
		}
		else													// ���������������ֵ+��Сֵ��1/2
		{
			m_pAutoCtrlFlagCnt[iKilnIdx] = 0;

			// ���ۼ�, ֱ�����
			out_adj = (pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].UpLimitOut 
					   - pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].DnLimitOut) / 2.0;
			out_adj += pDoc->m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].DnLimitOut;
		}
	}

	m_pFrontDec[iKilnIdx] = front_dec;
	m_pRearDec[iKilnIdx]  = rear_dec;

	CTime t = CTime::GetCurrentTime();
	CString str;
	str.Format("[Procs] [%d��Ҥ: %04d-%02d-%02d %02d:%02d:%02d ִ��1���Զ���������, ǰ��=%d ǰ���=%d ���=%d ����=%d, ��ǰ���=%3.2f �������=%3.2f �������=%3.2f �������=%3.2f]\r\n", 
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

	// �޷�����
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
		MessageBox("LOG�ļ���������", "���ô���", MB_OK);
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

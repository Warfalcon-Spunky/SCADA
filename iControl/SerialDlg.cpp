// SerialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "MainFrm.h"
#include "iControlDoc.h"
#include "SerialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSerialDlg dialog


CSerialDlg::CSerialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerialDlg)
	m_ScanTime = _T("");
	m_Timeout = _T("");
	m_GrossTime = _T("");
	m_BellTime = _T("");
	m_DtuPortID = _T("");
	//}}AFX_DATA_INIT
}


void CSerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialDlg)
	DDX_Control(pDX, IDC_DTU_NUM_COMBO, m_DtuPort);
	DDX_Control(pDX, IDC_DTU_BAUD_COMBO, m_DtuBaudSet);
	DDX_Control(pDX, IDC_STOPBITS_COMBO, m_StopBits);
	DDX_Control(pDX, IDC_PARITY_COMBO, m_Parity);
	DDX_Control(pDX, IDC_NUM_COMBO, m_ComPort);
	DDX_Control(pDX, IDC_DATABITS_COMBO, m_DataBits);
	DDX_Control(pDX, IDC_BAUD_COMBO, m_BaudSet);
	DDX_Text(pDX, IDC_SCANTIME_EDIT, m_ScanTime);
	DDV_MaxChars(pDX, m_ScanTime, 4);
	DDX_Text(pDX, IDC_TIMEOUT_EDIT, m_Timeout);
	DDV_MaxChars(pDX, m_Timeout, 4);
	DDX_Text(pDX, IDC_GROSS_EDIT, m_GrossTime);
	DDV_MaxChars(pDX, m_GrossTime, 4);
	DDX_Text(pDX, IDC_BELL_EDIT, m_BellTime);
	DDV_MaxChars(pDX, m_BellTime, 4);
	DDX_Text(pDX, IDC_DTUPORTID_EDIT, m_DtuPortID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSerialDlg, CDialog)
	//{{AFX_MSG_MAP(CSerialDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialDlg message handlers

BOOL CSerialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	m_ComPort.SetCurSel(pDoc->m_ComCfg.iPortNum - 1);		// 串口号
	m_iComPort = pDoc->m_ComCfg.iPortNum;

	m_iBaudrate = pDoc->m_ComCfg.iBaudrate;
	switch (pDoc->m_ComCfg.iBaudrate)
	{
	case 2400:
		m_BaudSet.SetCurSel(0);
		break;
	case 4800:
		m_BaudSet.SetCurSel(1);
		break;
	case 9600:
		m_BaudSet.SetCurSel(2);
		break;
	case 19200:
		m_BaudSet.SetCurSel(3);
		break;
	case 38400:
		m_BaudSet.SetCurSel(4);
		break;
	case 57600:
		m_BaudSet.SetCurSel(5);
		break;
	case 115200:
		m_BaudSet.SetCurSel(6);
		break;
	default:
		m_BaudSet.SetCurSel(3);
		break;
	}

	m_DtuPort.SetCurSel(pDoc->m_ComCfg.iDtuPortNum - 1);		// DTU串口号
	m_iDtuPort = pDoc->m_ComCfg.iDtuPortNum;

	m_iDtuBaudrate = pDoc->m_ComCfg.iDtuBaudrate;
	switch (pDoc->m_ComCfg.iDtuBaudrate)
	{
	case 2400:
		m_DtuBaudSet.SetCurSel(0);
		break;
	case 4800:
		m_DtuBaudSet.SetCurSel(1);
		break;
	case 9600:
		m_DtuBaudSet.SetCurSel(2);
		break;
	case 19200:
		m_DtuBaudSet.SetCurSel(3);
		break;
	case 38400:
		m_DtuBaudSet.SetCurSel(4);
		break;
	case 57600:
		m_DtuBaudSet.SetCurSel(5);
		break;
	case 115200:
		m_DtuBaudSet.SetCurSel(6);
		break;
	default:
		m_DtuBaudSet.SetCurSel(5);
		break;
	}

	m_cPrity = pDoc->m_ComCfg.cParity;
	switch (pDoc->m_ComCfg.cParity)
	{
	case 'N':
		m_Parity.SetCurSel(0);
		break;
	case 'O':
		m_Parity.SetCurSel(1);
		break;
	case 'E':
		m_Parity.SetCurSel(2);
		break;
	case 'M':
		m_Parity.SetCurSel(3);
		break;
	case 'S':
		m_Parity.SetCurSel(4);
		break;
	default:
		m_Parity.SetCurSel(0);
		break;
	}

	m_iDataBits = pDoc->m_ComCfg.iDataBits;
	switch (pDoc->m_ComCfg.iDataBits)
	{
	case 5:
		m_DataBits.SetCurSel(0);
		break;
	case 6:
		m_DataBits.SetCurSel(1);
		break;
	case 7:
		m_DataBits.SetCurSel(2);
		break;
	case 8:
	default:
		m_DataBits.SetCurSel(3);
		break;
	}

	m_iStopBits = pDoc->m_ComCfg.iStopBits;
	switch (pDoc->m_ComCfg.iStopBits)
	{
	case 1:
		m_StopBits.SetCurSel(0);
		break;
	case 15:
		m_StopBits.SetCurSel(1);
		break;
	case 2:
		m_StopBits.SetCurSel(2);
		break;
	default:
		m_StopBits.SetCurSel(0);
		break;
	}

	m_DtuPortID.Format("%d", pDoc->m_ComCfg.lDtuPort);
	m_lDtuPortID = pDoc->m_ComCfg.lDtuPort;

	m_Timeout.Format("%d", pDoc->m_ComCfg.iTimeout);
	m_iTimeout = pDoc->m_ComCfg.iTimeout;

	m_ScanTime.Format("%d", pDoc->m_ComCfg.iScanTime);	
	m_iScanTime = pDoc->m_ComCfg.iScanTime;

	// 任意1条要的任意1个间隔时间, 因为所有间隔时间一样
	for (int i = 0; i < pDoc->m_iKilnNum; i++)
	{
		for (int j = 0; j < pDoc->m_pKilnBuff[i].GrossNum; j++)
		{
			m_GrossTime.Format("%d", pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[j].TrigInternal);
			m_iGrossTime = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[j].TrigInternal;

			m_BellTime.Format("%d", pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[j].BellValidTime / 1000);
			m_iBellTime = pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[j].BellValidTime / 1000;
		}
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSerialDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_iComPort = m_ComPort.GetCurSel();

	switch (m_BaudSet.GetCurSel())
	{
	case 0:
		m_iBaudrate = 2400;
		break;
	case 1:
		m_iBaudrate = 4800;
		break;
	case 2:
		m_iBaudrate = 9600;
		break;
	case 3:
		m_iBaudrate = 19200;
		break;
	case 4:
		m_iBaudrate = 38400;
		break;
	case 5:
		m_iBaudrate = 57600;
		break;
	case 6:
		m_iBaudrate = 115200;
		break;
	default:
		m_iBaudrate = 19200;
		break;
	}

	m_iDtuPort = m_DtuPort.GetCurSel();

	switch (m_DtuBaudSet.GetCurSel())
	{
	case 0:
		m_iDtuBaudrate = 2400;
		break;
	case 1:
		m_iDtuBaudrate = 4800;
		break;
	case 2:
		m_iDtuBaudrate = 9600;
		break;
	case 3:
		m_iDtuBaudrate = 19200;
		break;
	case 4:
		m_iDtuBaudrate = 38400;
		break;
	case 5:
		m_iDtuBaudrate = 57600;
		break;
	case 6:
		m_iDtuBaudrate = 115200;
		break;
	default:
		m_iDtuBaudrate = 19200;
		break;
	}
	
	switch (m_DataBits.GetCurSel())
	{
	case 0:
		m_iDataBits = 5;
		break;
	case 1:
		m_iDataBits = 6;
		break;
	case 2:
		m_iDataBits = 7;
		break;
	case 3:
		m_iDataBits = 8;
		break;
	default:
		m_iDataBits = 8;
		break;
	}

	switch (m_StopBits.GetCurSel())
	{
	case 0:
		m_iStopBits = 1;
		break;
	case 1:
		m_iStopBits = 15;
		break;
	case 2:
		m_iStopBits = 2;
		break;
	default:
		m_iStopBits = 1;
		break;
	}

	switch (m_Parity.GetCurSel())
	{
	case 0:
		m_cPrity = 'N';
		break;
	case 1:
		m_cPrity = 'O';
		break;
	case 2:
		m_cPrity = 'E';
		break;
	case 3:
		m_cPrity = 'M';
		break;
	case 4:
		m_cPrity = 'S';
		break;
	default:
		m_cPrity = 'N';
		break;
	}

	UpdateData(TRUE);
	m_lDtuPortID     = atoi(m_DtuPortID);
	m_iTimeout       = atoi(m_Timeout);
	m_iScanTime      = atoi(m_ScanTime);
	m_iGrossTime     = atoi(m_GrossTime);
	m_iBellTime      = atoi(m_BellTime);

	if (m_iBellTime > 60)
	{
		m_iBellTime = 60;
	}

	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	pDoc->m_ComCfg.iPortNum    = m_iComPort + 1;
	pDoc->m_ComCfg.iBaudrate   = m_iBaudrate;
	pDoc->m_ComCfg.cParity     = m_cPrity;
	pDoc->m_ComCfg.iDataBits   = m_iDataBits;
	pDoc->m_ComCfg.iStopBits   = m_iStopBits;
	pDoc->m_ComCfg.iTimeout    = m_iTimeout;
	pDoc->m_ComCfg.iScanTime   = m_iScanTime;
	pDoc->m_ComCfg.iDtuBaudrate = m_iDtuBaudrate;
	pDoc->m_ComCfg.lServePort   = 1;
	pDoc->m_ComCfg.iDtuPortNum  = m_iDtuPort + 1;
	pDoc->m_ComCfg.lDtuPort     = m_lDtuPortID;

	for (int i = 0; i < pDoc->m_iKilnNum; i++)
	{
		for (int j = 0; j < pDoc->m_pKilnBuff[i].GrossNum; j++)
		{
			pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[j].TrigInternal  = m_iGrossTime;
			pDoc->m_pKilnBuff[i].m_pGrossObjectBuff[j].BellValidTime = m_iBellTime * 1000;	// 底层传输为毫秒单位
		}
	}

	pDoc->SetComCfgTbl();						// 存入数据库
	
	CDialog::OnOK();
}

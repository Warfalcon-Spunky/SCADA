// AutoCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "iControl.h"
#include "MainFrm.h"
#include "AutoCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoCtrlDlg dialog


CAutoCtrlDlg::CAutoCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoCtrlDlg)
	m_bAutoCtrlEnable = FALSE;
	m_CtrlTime = _T("");
	m_OutDnLimit = _T("");
	m_FrontPts = _T("");
	m_IncCoeff = _T("");
	m_RearPts = _T("");
	m_OutUpLimit = _T("");
	m_GrossWaitTime = _T("");
	m_GrossProcTime = _T("");
	m_GrossProcMaxTime = _T("");
	m_DnWarn = _T("");
	m_UpWarn = _T("");
	//}}AFX_DATA_INIT
}


void CAutoCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoCtrlDlg)
	DDX_Control(pDX, IDC_COMBO_SEL_KILN, m_SelKilnIdx);
	DDX_Check(pDX, IDC_CHECK_BCTRL, m_bAutoCtrlEnable);
	DDX_Text(pDX, IDC_EDIT_CTRL_TIME, m_CtrlTime);
	DDV_MaxChars(pDX, m_CtrlTime, 2);
	DDX_Text(pDX, IDC_EDIT_DN_LIMIT, m_OutDnLimit);
	DDV_MaxChars(pDX, m_OutDnLimit, 5);
	DDX_Text(pDX, IDC_EDIT_FRONT_PTS, m_FrontPts);
	DDV_MaxChars(pDX, m_FrontPts, 1);
	DDX_Text(pDX, IDC_EDIT_INC_COEFF, m_IncCoeff);
	DDV_MaxChars(pDX, m_IncCoeff, 4);
	DDX_Text(pDX, IDC_EDIT_REAR_PTS, m_RearPts);
	DDV_MaxChars(pDX, m_RearPts, 1);
	DDX_Text(pDX, IDC_EDIT_UP_LIMIT, m_OutUpLimit);
	DDV_MaxChars(pDX, m_OutUpLimit, 5);
	DDX_Text(pDX, IDC_EDIT_WAIT_TIME, m_GrossWaitTime);
	DDV_MaxChars(pDX, m_GrossWaitTime, 2);
	DDX_Text(pDX, IDC_EDIT_PROC_TIME, m_GrossProcTime);
	DDX_Text(pDX, IDC_EDIT_PROC_MAX_TIME, m_GrossProcMaxTime);
	DDX_Text(pDX, IDC_EDIT_DN_WARN, m_DnWarn);
	DDX_Text(pDX, IDC_EDIT_UP_WARN, m_UpWarn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoCtrlDlg)
	ON_CBN_SELENDOK(IDC_COMBO_SEL_KILN, OnSelendokComboSelKiln)
	ON_CBN_EDITCHANGE(IDC_COMBO_SEL_KILN, OnEditchangeComboSelKiln)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoCtrlDlg message handlers

BOOL CAutoCtrlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	// ���Ҥ��ѡ����Ŀ
	CString str;
	for (int i = 0; i < pDoc->m_iKilnNum; i++)
	{
		str.Empty();
		str.Format("%02d��Ҥ", i + 1);
		m_SelKilnIdx.AddString(str);
	}

	/************************************************************************/
	/* �õ���һ���Զ�����Ҥ                                                 */
	/************************************************************************/
	for (i = 0; i < pDoc->m_iKilnNum; i++)
	{
		if (pDoc->m_pKilnBuff[i].ControlNum > 0)
		{
			m_iSelKilnIndex = i;
			m_OutUpLimit.Format("%3.2f", pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].UpLimitOut);
			m_OutDnLimit.Format("%3.2f", pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].DnLimitOut);

			m_FrontPts.Format("%d", pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iFrontPoint);
			m_RearPts.Format("%d", pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iRearPoint);

			m_IncCoeff.Format("%3.2f", pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].IncOutCoeff);

			m_CtrlTime.Format("%d", pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iCtrlTime);
			m_GrossWaitTime.Format("%d", pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iGrossWaitTime);
			m_GrossProcTime.Format("%d", pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].iGrossProcTime);
			m_GrossProcMaxTime.Format("%d", pDoc->m_iGrossProcMaxTime[i]);
			m_UpWarn.Format("%d", pDoc->m_iUpWarn[i]);
			m_DnWarn.Format("%d", pDoc->m_iDnWarn[i]);

			m_bAutoCtrlEnable = pDoc->m_pKilnBuff[i].m_pCtrlObjectBuff[0].bControl;
			break;
		}
	}

	m_SelKilnIdx.SetCurSel(m_iSelKilnIndex);
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoCtrlDlg::OnSelendokComboSelKiln() 
{
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	int idx = m_SelKilnIdx.GetCurSel();

	if (pDoc->m_pKilnBuff[idx].ControlNum > 0)
	{
		m_iSelKilnIndex = idx;

		m_OutUpLimit.Format("%3.2f", pDoc->m_pKilnBuff[idx].m_pCtrlObjectBuff[0].UpLimitOut);
		m_OutDnLimit.Format("%3.2f", pDoc->m_pKilnBuff[idx].m_pCtrlObjectBuff[0].DnLimitOut);
		
		m_FrontPts.Format("%d", pDoc->m_pKilnBuff[idx].m_pCtrlObjectBuff[0].iFrontPoint);
		m_RearPts.Format("%d", pDoc->m_pKilnBuff[idx].m_pCtrlObjectBuff[0].iRearPoint);
		
		m_IncCoeff.Format("%3.2f", pDoc->m_pKilnBuff[idx].m_pCtrlObjectBuff[0].IncOutCoeff);
		
		m_CtrlTime.Format("%d", pDoc->m_pKilnBuff[idx].m_pCtrlObjectBuff[0].iCtrlTime);
		m_GrossWaitTime.Format("%d", pDoc->m_pKilnBuff[idx].m_pCtrlObjectBuff[0].iGrossWaitTime);
		m_GrossProcTime.Format("%d", pDoc->m_pKilnBuff[idx].m_pCtrlObjectBuff[0].iGrossProcTime);
		m_GrossProcMaxTime.Format("%d", pDoc->m_iGrossProcMaxTime[idx]);
		m_UpWarn.Format("%d", pDoc->m_iUpWarn[idx]);
		m_DnWarn.Format("%d", pDoc->m_iDnWarn[idx]);
		
		m_bAutoCtrlEnable = pDoc->m_pKilnBuff[idx].m_pCtrlObjectBuff[0].bControl;
	}
	else
	{
		CString str;
		str.Format("%d��Ҥδ�����Զ����ƹ���, ������ѡ��", idx + 1);
		MessageBox(str, "ѡ�����");

		m_SelKilnIdx.SetCurSel(m_iSelKilnIndex);
	}

	UpdateData(FALSE);
}

void CAutoCtrlDlg::OnEditchangeComboSelKiln() 
{
}

void CAutoCtrlDlg::OnOK() 
{
	// ��ȡ�ĵ���ز���
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	CtrlObject ctrl_info;

	UpdateData(TRUE);

	ctrl_info.UpLimitOut = atof(m_OutUpLimit);
	if ((ctrl_info.UpLimitOut < 0.0) || (ctrl_info.UpLimitOut > 50.0))
	{
		MessageBox("��Ƶ�����ֵ���÷�Χ: [0.0 ~ 50.0], ����������", "�������ô���");
		CDialog::EndDialog(-1);			// ��-1�������
		return;
	}

	ctrl_info.DnLimitOut = atof(m_OutDnLimit);
	if ((ctrl_info.DnLimitOut < 0.0) || (ctrl_info.DnLimitOut > 50.0))
	{
		MessageBox("��Ƶ����Сֵ���÷�Χ: [0.0 ~ 50.0], ����������", "�������ô���");
		CDialog::EndDialog(-1);			// ��-1�������
		return;
	}

	if (ctrl_info.UpLimitOut <= ctrl_info.DnLimitOut)
	{
		MessageBox("��Ƶ����Сֵ����Сֵ���ô���, ����������", "�������ô���");
		CDialog::EndDialog(-1);			// ��-1�������
		return;
	}

	ctrl_info.iFrontPoint = atoi(m_FrontPts);
	if ((ctrl_info.iFrontPoint < 1) || (ctrl_info.iFrontPoint > pDoc->m_pKilnBuff[m_iSelKilnIndex].TemperatureNum))
	{
		MessageBox("ǰ��ߵ������ô���", "�������ô���");
		CDialog::EndDialog(-1);			// ��-1�������
		return;
	}

	ctrl_info.iRearPoint = atoi(m_RearPts);
	if ((ctrl_info.iRearPoint < 1) || (ctrl_info.iRearPoint > pDoc->m_pKilnBuff[m_iSelKilnIndex].TemperatureNum))
	{
		MessageBox("����ߵ������ô���", "�������ô���");
		CDialog::EndDialog(-1);			// ��-1�������
		return;
	}

	ctrl_info.IncOutCoeff = atof(m_IncCoeff);
	if ((ctrl_info.IncOutCoeff < 0.0) || (ctrl_info.IncOutCoeff > 50.0))
	{
		MessageBox("����ϵ�����÷�Χ: [0.0 ~ 50.0], ����������", "�������ô���");
		CDialog::EndDialog(-1);			// ��-1�������
		return;
	}

	ctrl_info.iCtrlTime = atoi(m_CtrlTime);
	if ((ctrl_info.iCtrlTime < 1) || (ctrl_info.iCtrlTime > 30))
	{
		MessageBox("�����������÷�Χ: [1 ~ 30], ����������", "�������ô���");
		CDialog::EndDialog(-1);			// ��-1�������
		return;
	}

	ctrl_info.iGrossWaitTime = atoi(m_GrossWaitTime);
	if ((ctrl_info.iGrossWaitTime < 5) || (ctrl_info.iGrossWaitTime > 30))
	{
		MessageBox("�Ƴ���ȴ�ʱ�����÷�Χ: [5 ~ 30], ����������", "�������ô���");
		CDialog::EndDialog(-1);			// ��-1�������
		return;
	}

	ctrl_info.iGrossProcTime = atoi(m_GrossProcTime);
	if ((ctrl_info.iGrossProcTime < 2) || (ctrl_info.iGrossProcTime > 1440))
	{
		MessageBox("�Ƴ����ʱ�����÷�Χ: [2 ~ 1440], ����������", "�������ô���");
		CDialog::EndDialog(-1);			// ��-1�������
		return;
	}

	int GrossProcMaxTime = atoi(m_GrossProcMaxTime);
	if ((GrossProcMaxTime < 2) || (GrossProcMaxTime > 1440))
	{
		MessageBox("�Ƴ����ʱ�����÷�Χ: [2 ~ 1440], ����������", "�������ô���");
		CDialog::EndDialog(-1);			// ��-1�������
		return;
	}

	int UpWarnVal = atoi(m_UpWarn);
	if ((UpWarnVal < 500) || (UpWarnVal > 1370))
	{
		MessageBox("�¶����ޱ���ֵ���÷�Χ: [500 ~ 1370], ����������", "�������ô���");
		CDialog::EndDialog(-1);			// ��-1�������
		return;
	}

	int DnWarnVal = atoi(m_DnWarn);
	if ((DnWarnVal < 0) || (DnWarnVal > 1000))
	{
		MessageBox("�¶����ޱ���ֵ���÷�Χ: [0 ~ 1000], ����������", "�������ô���");
		CDialog::EndDialog(-1);			// ��-1�������
		return;
	}

	if ((ctrl_info.iGrossProcTime != pDoc->m_pKilnBuff[m_iSelKilnIndex].m_pCtrlObjectBuff[0].iGrossProcTime) 
		|| (GrossProcMaxTime != pDoc->m_iGrossProcMaxTime[m_iSelKilnIndex]))
	{
		ctrl_info.iProcTimerCnt = ctrl_info.iGrossProcTime * 60;
	}
	else
	{
		ctrl_info.iProcTimerCnt = pDoc->m_pKilnBuff[m_iSelKilnIndex].m_pCtrlObjectBuff[0].iProcTimerCnt;
	}

	CString strDefaultPath, str_ini;
	char file_path[MAX_PATH];
	GetModuleFileName(NULL, file_path, MAX_PATH);
	strDefaultPath = file_path;
	int nTemp = strDefaultPath.ReverseFind('\\');
	strDefaultPath = strDefaultPath.Left(nTemp);
	str_ini.Format("%s\\setting.ini", strDefaultPath);

	CString str;
	// �������Ⱦ͸���INI�ļ�
	if (GrossProcMaxTime != pDoc->m_iGrossProcMaxTime[m_iSelKilnIndex])
	{	
		str.Empty();
		str.Format("%d", GrossProcMaxTime);
		if (m_iSelKilnIndex == 0)
		{
			WritePrivateProfileString("GrossProcMaxTime", "������ʱ��1", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iGrossProcMaxTime[m_iSelKilnIndex] = GetPrivateProfileInt("GrossProcMaxTime", "������ʱ��1", 150, str_ini.GetBuffer(MAX_PATH));
		}
		else if (m_iSelKilnIndex == 1)
		{
			WritePrivateProfileString("GrossProcMaxTime", "������ʱ��2", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iGrossProcMaxTime[m_iSelKilnIndex] = GetPrivateProfileInt("GrossProcMaxTime", "������ʱ��2", 150, str_ini.GetBuffer(MAX_PATH));
		}
		else if (m_iSelKilnIndex == 2)
		{
			WritePrivateProfileString("GrossProcMaxTime", "������ʱ��3", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iGrossProcMaxTime[m_iSelKilnIndex] = GetPrivateProfileInt("GrossProcMaxTime", "������ʱ��3", 150, str_ini.GetBuffer(MAX_PATH));
		}
		else
		{
			WritePrivateProfileString("GrossProcMaxTime", "������ʱ��4", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iGrossProcMaxTime[m_iSelKilnIndex] = GetPrivateProfileInt("GrossProcMaxTime", "������ʱ��4", 150, str_ini.GetBuffer(MAX_PATH));
		}
	}

	// �������Ⱦ͸���INI�ļ�
	if (UpWarnVal != pDoc->m_iUpWarn[m_iSelKilnIndex])
	{	
		str.Empty();
		str.Format("%d", UpWarnVal);
		if (m_iSelKilnIndex == 0)
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������1", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iUpWarn[m_iSelKilnIndex] = GetPrivateProfileInt("Warn", "�¶ȱ�������1", 1000, str_ini.GetBuffer(MAX_PATH));
		}
		else if (m_iSelKilnIndex == 1)
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������2", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iUpWarn[m_iSelKilnIndex] = GetPrivateProfileInt("Warn", "�¶ȱ�������2", 1000, str_ini.GetBuffer(MAX_PATH));
		}
		else if (m_iSelKilnIndex == 2)
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������3", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iUpWarn[m_iSelKilnIndex] = GetPrivateProfileInt("Warn", "�¶ȱ�������3", 1000, str_ini.GetBuffer(MAX_PATH));
		}
		else
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������4", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iUpWarn[m_iSelKilnIndex] = GetPrivateProfileInt("Warn", "�¶ȱ�������4", 1000, str_ini.GetBuffer(MAX_PATH));
		}
	}

	// �������Ⱦ͸���INI�ļ�
	if (DnWarnVal != pDoc->m_iDnWarn[m_iSelKilnIndex])
	{	
		str.Empty();
		str.Format("%d", DnWarnVal);
		if (m_iSelKilnIndex == 0)
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������1", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iDnWarn[m_iSelKilnIndex] = GetPrivateProfileInt("Warn", "�¶ȱ�������1", 800, str_ini.GetBuffer(MAX_PATH));
		}
		else if (m_iSelKilnIndex == 1)
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������2", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iDnWarn[m_iSelKilnIndex] = GetPrivateProfileInt("Warn", "�¶ȱ�������2", 800, str_ini.GetBuffer(MAX_PATH));
		}
		else if (m_iSelKilnIndex == 2)
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������3", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iDnWarn[m_iSelKilnIndex] = GetPrivateProfileInt("Warn", "�¶ȱ�������3", 800, str_ini.GetBuffer(MAX_PATH));
		}
		else
		{
			WritePrivateProfileString("Warn", "�¶ȱ�������4", str, str_ini.GetBuffer(MAX_PATH));
			pDoc->m_iDnWarn[m_iSelKilnIndex] = GetPrivateProfileInt("Warn", "�¶ȱ�������4", 800, str_ini.GetBuffer(MAX_PATH));
		}
	}

	ctrl_info.bControl = m_bAutoCtrlEnable;

	// �����ǰ�������ֶ�����״̬, �˱������� or ����
	if (pDoc->m_pKilnBuff[m_iSelKilnIndex].m_pCtrlObjectBuff[0].bControl == FALSE)
	{
		ctrl_info.iWaitTimerCnt = 0;

		// ���ֿر�Ϊ�Կ�ʱ, ��¼��ǰ�ı�Ƶ��ֵ, �Ա��ڱ�Ϊ�ֿ�״̬ʱ�ָ���Ƶ����ֵ
		ctrl_info.BakMotorVal = pDoc->m_pKilnBuff[m_iSelKilnIndex].m_pMotorObjectBuff[0].Value;
	}
	else
	{	
		// ���Կر�Ϊ�ֿ�ʱ
		if (ctrl_info.bControl == FALSE)
		{
			ctrl_info.iWaitTimerCnt = 0;
			ctrl_info.BakMotorVal   = pDoc->m_pKilnBuff[m_iSelKilnIndex].m_pCtrlObjectBuff[0].BakMotorVal;

			// �ָ��ϴ��ֿ�״̬��ֵ
			int  DevIdx  = pDoc->m_pKilnBuff[m_iSelKilnIndex].m_pMotorObjectBuff[0].TemOutIdx;
			WORD RegAddr = pDoc->m_pKilnBuff[m_iSelKilnIndex].m_pMotorObjectBuff[0].BufOutIdx + 40001;
			WORD OutVal  = (WORD)(ctrl_info.BakMotorVal * 10.0);
			pMainFrame->m_pCom->SetWriteTerminalInfo(DevIdx, RegAddr, 1, &OutVal);
		}
		else
		{
			ctrl_info.iWaitTimerCnt = pDoc->m_pKilnBuff[m_iSelKilnIndex].m_pCtrlObjectBuff[0].iWaitTimerCnt;
			ctrl_info.BakMotorVal   = pDoc->m_pKilnBuff[m_iSelKilnIndex].m_pCtrlObjectBuff[0].BakMotorVal;
		}
	}
	
	pDoc->m_pKilnBuff[m_iSelKilnIndex].m_pCtrlObjectBuff[0] = ctrl_info;

	pDoc->SaveAutoCtrlInfoRecord(m_iSelKilnIndex);

	CDialog::EndDialog(1);
}

void CAutoCtrlDlg::OnCancel() 
{
	CDialog::EndDialog(-2);			// �˳�
}

void CAutoCtrlDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

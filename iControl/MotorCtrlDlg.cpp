// MotorCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "MotorCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMotorCtrlDlg dialog


CMotorCtrlDlg::CMotorCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMotorCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMotorCtrlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMotorCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotorCtrlDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMotorCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CMotorCtrlDlg)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CTRL, OnCustomdrawSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CTRL, OnReleasedcaptureSlider)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDNEXT, OnNext)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotorCtrlDlg message handlers

BOOL CMotorCtrlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_pAngularGaugeX = NULL;
	m_pSlider        = NULL;
	m_pMotorValue    = NULL;
	m_pToolTips      = NULL;

	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();
	
	m_iTotalMotorNum   = 0;
	m_iMotorIndex      = 0;
	m_iActiveKilnIndex = 0;
	m_iKilnMotorIdx    = 0;
	
	// 获得电机总数量
	for (int i = 0; i < pDoc->m_iKilnNum; i++)
	{
		m_iTotalMotorNum += pDoc->m_pKilnBuff[i].MotorNum;
	}

	if (m_iTotalMotorNum <= 0)
	{
		MessageBox("配置文件中不含任何电机信息");
		CDialog::OnCancel();
	}

	// 根据电机总数量创建缓冲区
	m_pMotorValue = new float[m_iTotalMotorNum];

	int k;
	float tmp_float;
	for (i = 0, k = 0; i < pDoc->m_iKilnNum; i++)
	{
		for (int j = 0; j < pDoc->m_pKilnBuff[i].MotorNum; j++)
		{
			tmp_float = pDoc->m_pKilnBuff[i].m_pMotorObjectBuff[j].Value;
			if (tmp_float > 500.0 || tmp_float < 0.0)
			{
				m_pMotorValue[k++] = 0.0;
			}
			else
			{
				m_pMotorValue[k++] = tmp_float / 10.0;
			}
		}	
	}

	CString str;
	switch (pDoc->m_pKilnBuff[m_iActiveKilnIndex].type)
	{
	case 0:
		str.Format("Frequency Converter Regulation--%d# <Mobile Kiln>--%s", m_iActiveKilnIndex + 1, pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[0].strName);
		break;
	case 1:
		str.Format("Frequency Converter Regulation--%d# <Drying Kiln>--%s", m_iActiveKilnIndex + 1, pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[0].strName);
		break;
	case 2:
		str.Format("Frequency Converter Regulation--%d# <Through Kiln>--%s", m_iActiveKilnIndex + 1, pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[0].strName);
		break;
	case 3:
		str.Format("Frequency Converter Regulation--%d# <Roasting Kiln>--%s", m_iActiveKilnIndex + 1, pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[0].strName);
		break;
	default:
		break;
	}
	SetWindowText(str);
	
	CRect ClientRect, ControlRect;
	GetClientRect(ClientRect);
	ControlRect.left = 0;
	ControlRect.right = ClientRect.Width();
	ControlRect.top = 0;
	ControlRect.bottom = (int)((float)(ClientRect.Height()) * 0.65);

	m_pAngularGaugeX = new CiAngularGaugeX;
	m_pAngularGaugeX->Create("", WS_CHILD | WS_VISIBLE, ControlRect, this, IDC_MOTOR_CTRL);

	m_pAngularGaugeX->SetAutoSize(TRUE);			// 空间自适应大小
	m_pAngularGaugeX->SetPosition(m_pMotorValue[0]);// 初始指针显示位置
	m_pAngularGaugeX->SetArcRangeDegrees(300);		// 圆形仪表的显示度数范围
	m_pAngularGaugeX->SetArcStartDegrees(240);		// 圆形仪表的开始度数
	m_pAngularGaugeX->SetPositionMax(50.0F);		// 仪表的最大显示值
	m_pAngularGaugeX->SetShowHub(TRUE);
	m_pAngularGaugeX->SetShowOuterArcRadius(TRUE);
	m_pAngularGaugeX->SetPointerStyle(3);			// 设置指针样式 : iagpsArrowLine = 0
													//                iagpsArrow = 1
													//                iagpsLine = 2
													//                iagpsTriangle = 3
	m_pAngularGaugeX->SetTickMajorCount(11);		// 刻度标示个数
	m_pAngularGaugeX->SetTickMajorLength(20);		// 大刻度长度
	m_pAngularGaugeX->SetTickLabelPrecision(2);		// 刻度标示小数点位数
	m_pAngularGaugeX->SetTickMinorCount(4);			// 小刻度个数
	m_pAngularGaugeX->SetTickMinorLength(8);			// 小刻度长度
	m_pAngularGaugeX->SetTickMinorAlignment(2);		// 小刻度对齐方式 : itmnaInside = 0
													//                  itmnaCenter = 1
													//                  itmnaOutside = 2
	m_pAngularGaugeX->SetTickLabelMargin(-40);		// 刻度标示的位置
	m_pAngularGaugeX->SetSectionCount(4);			// 颜色标识个数
	m_pAngularGaugeX->SetSectionColor1(0x8080);		// 1号标识颜色 
	m_pAngularGaugeX->SetSectionColor2(0xFF00);		// 2号标识颜色 
	m_pAngularGaugeX->SetSectionColor3(0xFFFF);		// 3号标识颜色 
	m_pAngularGaugeX->SetSectionColor4(0x00FF);		// 4号标识颜色 
	m_pAngularGaugeX->SetSectionEnd1(10);
	m_pAngularGaugeX->SetSectionEnd2(20);
	m_pAngularGaugeX->SetSectionEnd3(30);
	m_pAngularGaugeX->SetSectionEnd4(40);
	m_pAngularGaugeX->SetLabel1Text("");
	m_pAngularGaugeX->SetShowLabel1(FALSE);
	m_pAngularGaugeX->SetLabel1OffsetX(0);
	m_pAngularGaugeX->SetLabel1OffsetY(-30);
	m_pAngularGaugeX->SetLabel2Text("");
	m_pAngularGaugeX->SetShowLabel2(TRUE);
	m_pAngularGaugeX->SetLabel2OffsetX(0);
	m_pAngularGaugeX->SetLabel2OffsetY(23);
	m_pAngularGaugeX->SetMinMaxPointerStyle(1);
	m_pAngularGaugeX->SetMinMaxPointerMargin(15);

	m_pSlider = new CSliderCtrl;
	ControlRect.left = (float)(ClientRect.Width()) * 0.2;
	ControlRect.right = (float)(ClientRect.Width()) * 0.8;
	ControlRect.top   = ControlRect.bottom + 20;
	ControlRect.bottom = ControlRect.top + 40;
	m_pSlider->Create(WS_CHILD | WS_VISIBLE, ControlRect, this, IDC_SLIDER_CTRL);

	m_pSlider->SetRangeMax(5000, TRUE);		// 滑条的最大值, 代表50Hz
	m_pSlider->SetRangeMin(0, TRUE);		// 滑条的最小值, 代表0Hz
	m_pSlider->SetPageSize(1);				// 按PAGE键动作0.01Hz
	m_pSlider->SetPos((int)(m_pMotorValue[0] * 100.0));
	m_pSlider->EnableToolTips(TRUE);

	m_pToolTips = new CToolTipCtrl;
	m_pToolTips->Create(GetDlgItem(IDC_SLIDER_CTRL));
	m_pToolTips->AddTool(GetDlgItem(IDC_SLIDER_CTRL), LPSTR_TEXTCALLBACK);
	m_pToolTips->Activate(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMotorCtrlDlg::OnCustomdrawSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pAngularGaugeX->SetPosition((float)(m_pSlider->GetPos()) / 100.0F);
	*pResult = 0;
}

void CMotorCtrlDlg::OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_pMotorValue[m_iMotorIndex] = (float)(m_pSlider->GetPos()) / 100.0F;
	*pResult = 0;
}

BOOL CMotorCtrlDlg::PreTranslateMessage(MSG* pMsg) 
{
	static cnt = 0;

	if ((WM_KEYFIRST <= pMsg->message) && (pMsg->message <= WM_KEYLAST))
	{
		if (pMsg->wParam == VK_RIGHT)
		{
			if (m_pMotorValue[m_iMotorIndex] < 50.0F)
			{
				m_pMotorValue[m_iMotorIndex] += 0.1F;
			}
			
			m_pAngularGaugeX->SetPosition(m_pMotorValue[m_iMotorIndex]);
			m_pSlider->SetPos(m_pMotorValue[m_iMotorIndex] * 100.0F);
		}
		else if (pMsg->wParam == VK_LEFT)
		{
			if (m_pMotorValue[m_iMotorIndex] > 0.0)
			{
				m_pMotorValue[m_iMotorIndex] -= 0.1F;
			}

			m_pAngularGaugeX->SetPosition(m_pMotorValue[m_iMotorIndex]);
			m_pSlider->SetPos(m_pMotorValue[m_iMotorIndex] * 100.0F);
		}
		else if (pMsg->wParam == VK_DOWN)
		{
			cnt++;
			if ((cnt % 2) != 0)
			{
				OnNext();
			}
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			OnCancel();
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			OnOK();
		}

		GetDlgItem(IDOK)->SetFocus();
		return TRUE;
	}
	else
	{
		m_pToolTips->RelayEvent(pMsg);				// 触发tip的事件
	}

	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CMotorCtrlDlg::OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR; 
	UINT nID =pNMHDR->idFrom; 

	if (pTTT->uFlags & TTF_IDISHWND) 
	{ 
		// idFrom is actually the HWND of the tool 
		nID = ::GetDlgCtrlID((HWND)nID); 
		if(nID == IDC_SLIDER_CTRL) 
		{  
			CString strToolTips; 
			strToolTips.Format("%2.2fHz", (float)(m_pSlider->GetPos()) / 100.0); 
	   
			strcpy(pTTT->lpszText, strToolTips); 
			pTTT->hinst = NULL; 
			return TRUE; 
		} 
	}    
	return FALSE; 
}

void CMotorCtrlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar == (CScrollBar*)GetDlgItem(IDC_SLIDER_CTRL)) 
	{
		m_pToolTips->Update();  //  更新Tooltips，会触发TTN_NEEDTEXT消息
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMotorCtrlDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pAngularGaugeX != NULL)
	{
		delete m_pAngularGaugeX;
	}

	if (m_pSlider != NULL)
	{
		delete m_pSlider;
	}

	if (m_pMotorValue != NULL)
	{
		delete [] m_pMotorValue;
	}

	if (m_pToolTips != NULL)
	{
		delete m_pToolTips;
	}
}

void CMotorCtrlDlg::OnNext() 
{
	m_iMotorIndex = (m_iMotorIndex + 1) % m_iTotalMotorNum;

	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	/************************************************************************/
	/* 得到电机对应的窑号                                                   */
	/************************************************************************/
	int i, tmp;
	for (tmp = 0, i = 0; i < pDoc->m_iKilnNum; i++)
	{
		tmp += pDoc->m_pKilnBuff[i].MotorNum;
		if (m_iMotorIndex < tmp)
		{
			m_iActiveKilnIndex = i;
			break;
		}
	}

	for (tmp = 0, i = 0; i < m_iActiveKilnIndex; i++)
	{
		tmp += pDoc->m_pKilnBuff[i].MotorNum;
	}

	m_iKilnMotorIdx = m_iMotorIndex - tmp;

	CString str;
	switch (pDoc->m_pKilnBuff[m_iActiveKilnIndex].type)
	{
	case 0:
		str.Format("Frequency Converter Regulation--%d# <Mobile Kiln>--%s", m_iActiveKilnIndex + 1, pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[m_iKilnMotorIdx].strName);
		break;
	case 1:
		str.Format("Frequency Converter Regulation--%d# <Drying Kiln>--%s", m_iActiveKilnIndex + 1, pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[m_iKilnMotorIdx].strName);
		break;
	case 2:
		str.Format("Frequency Converter Regulation--%d# <Through Kiln>--%s", m_iActiveKilnIndex + 1, pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[m_iKilnMotorIdx].strName);
		break;
	case 3:
		str.Format("Frequency Converter Regulation--%d# <Roasting Kiln>--%s", m_iActiveKilnIndex + 1, pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[m_iKilnMotorIdx].strName);
		break;
	default:
		break;
	}
	SetWindowText(str);

	m_pSlider->SetPos((int)(m_pMotorValue[m_iMotorIndex] * 100.0));
	m_pAngularGaugeX->SetPosition(m_pMotorValue[m_iMotorIndex]);
}

void CMotorCtrlDlg::OnOK() 
{
	// 获取文档相关参数
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CIControlDoc *pDoc     = (CIControlDoc *)pMainFrame->GetActiveDocument();

	if ((pDoc->m_pKilnBuff[m_iActiveKilnIndex].ControlNum > 0) 
		&& (pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pCtrlObjectBuff[0].bControl == TRUE) 
		&& (m_iKilnMotorIdx == 0))
	{
		CString str;
		switch (pDoc->m_pKilnBuff[m_iActiveKilnIndex].type)
		{
		case 0:
			str.Format("%d# <Mobile Kiln>--%s under automatic status, Please close Automatic function", m_iActiveKilnIndex + 1, pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[m_iKilnMotorIdx].strName);
			break;
		case 1:
			str.Format("%d# <Drying kiln>--%s under automatic status, Please close Automatic function", m_iActiveKilnIndex + 1, pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[m_iKilnMotorIdx].strName);
			break;
		case 2:			
			str.Format("%d# <Through Kiln>--%s under automatic status, Please close Automatic function", m_iActiveKilnIndex + 1, pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[m_iKilnMotorIdx].strName);
			break;
		case 3:			
			str.Format("%d# <Roasting Kiln>--%s under automatic status, Please close Automatic function", m_iActiveKilnIndex + 1, pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[m_iKilnMotorIdx].strName);
			break;
		default:
			break;
		}

		MessageBox(str, "Tips");
		return;
	}

	int  tmp = 0;
	int  DevIdx;
	int  BufIdx;
	WORD RegAddr;
	WORD RegVal;

	for (int i = 0; i < m_iActiveKilnIndex; i++)
	{
		tmp += pDoc->m_pKilnBuff[i].MotorNum;
	}
	BufIdx  = m_iMotorIndex - tmp;
	DevIdx  = pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[BufIdx].TemOutIdx;
	RegAddr = pDoc->m_pKilnBuff[m_iActiveKilnIndex].m_pMotorObjectBuff[BufIdx].BufOutIdx + 40001;
	RegVal  = (WORD)(m_pMotorValue[m_iMotorIndex] * 10.0);

	pMainFrame->m_pCom->SetWriteTerminalInfo(DevIdx, RegAddr, 1, &RegVal);
	
	CDialog::OnOK();
}

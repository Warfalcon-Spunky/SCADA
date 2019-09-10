// RegisterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "RegisterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegisterDlg dialog


CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegisterDlg)
	m_strACode = _T("");
	m_strECode = _T("");
	//}}AFX_DATA_INIT
}


void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegisterDlg)
	DDX_Control(pDX, IDC_DAY_EDIT, m_DayEdit);
	DDX_Control(pDX, IDC_E_EDIT, m_PrivateCodeEdit);
	DDX_Control(pDX, IDC_A_EDIT, m_PublicCodeEdit);
	DDX_Text(pDX, IDC_A_EDIT, m_strACode);
	DDV_MaxChars(pDX, m_strACode, 47);
	DDX_Text(pDX, IDC_E_EDIT, m_strECode);
	DDV_MaxChars(pDX, m_strECode, 47);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialog)
	//{{AFX_MSG_MAP(CRegisterDlg)
	ON_BN_CLICKED(IDOK, OnRegister)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegisterDlg message handlers

BOOL CRegisterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ShowWindow(SW_HIDE);

	m_bIsRegister = FALSE;
	
	m_pStaticAFont = new CFont;
	m_pStaticAFont->CreateFont(24, 0, 0, 0, FW_MEDIUM, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "楷体_GB2312");

	m_pStaticBFont = new CFont;
	m_pStaticBFont->CreateFont(24, 0, 0, 0, FW_MEDIUM, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "楷体_GB2312");
	
	m_pEditAFont = new CFont;
	m_pEditAFont->CreateFont(20, 0, 0, 0, FW_MEDIUM, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Courier New");

	m_pEditBFont = new CFont;
	m_pEditBFont->CreateFont(20, 0, 0, 0, FW_MEDIUM, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Courier New");
	
	m_PublicCodeEdit.SetFont(m_pEditAFont, TRUE);			// 改变控件行高, 以便适应字体大小
	m_PrivateCodeEdit.SetFont(m_pEditBFont, TRUE);			// 改变控件行高, 以便适应字体大小
	m_DayEdit.SetFont(m_pEditBFont, TRUE);					// 改变控件行高, 以便适应字体大小

	m_strACode.Format("");
	m_strECode.Format("");
	UpdateData(FALSE);

	int   i;
	BYTE  m_AccessDaysBuff[4] = {0};
	BYTE  m_RandomBuff[6] = {0};
	BYTE  m_PasswordBuff[16] = {0};
	BYTE  m_DecodeBuff[16] = {0};
	char  m_tmpchar[MAX_PATH];
	CString szRandom, szAccessDays, szPassword;

	m_RegCtrl.CreateRegtrySubKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Spunky\\iControl");

	m_tmpchar[0] = '\0';
	m_RegCtrl.ReadRegistryValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Spunky\\iControl", "Password", m_tmpchar);
	szPassword.Format("%s", m_tmpchar);

	m_tmpchar[0] = '\0';
	m_RegCtrl.ReadRegistryValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Spunky\\iControl", "Random", m_tmpchar); 
	szRandom.Format("%s", m_tmpchar);

	m_tmpchar[0] = '\0';
	m_RegCtrl.ReadRegistryValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Spunky\\iControl", "AccessDays", m_tmpchar);
	szAccessDays.Format("%s", m_tmpchar);

	if ((szRandom.IsEmpty() == FALSE) && (szAccessDays.IsEmpty() == FALSE) && (szPassword.IsEmpty() == FALSE))
	{
		StrToHex(m_AccessDaysBuff, 4, szAccessDays.GetBuffer(szAccessDays.GetLength()));
		StrToHex(m_RandomBuff, 6, szRandom.GetBuffer(szRandom.GetLength()));
		StrToHex(m_PasswordBuff, 16, szPassword.GetBuffer(szPassword.GetLength()));
		
		if (m_RegCtrl.AESDecodeProc(m_DecodeBuff, m_RandomBuff, m_AccessDaysBuff) == TRUE)
		{
			for (i = 0; i < 16; i++)
			{
				if (m_DecodeBuff[i] != m_PasswordBuff[i])
				{
					MessageBox("解码运行错误, 80-参数....");
					break;
				}
			}
			
			if (i >= 16)
			{
				m_iAccessDays = ((DWORD)(m_AccessDaysBuff[0]) << 24) | ((DWORD)(m_AccessDaysBuff[1]) << 16) 
								| ((DWORD)(m_AccessDaysBuff[2]) << 8)  | ((DWORD)(m_AccessDaysBuff[3]) << 0);
				m_bIsRegister = TRUE;

				if (m_bIsModifyResgister == FALSE)
				{
					CDialog::OnOK();
					return TRUE;
				}
			}
		}
		else 
		{
			MessageBox("解码运行错误, 3-参数....");
		}
	}

	ShowWindow(SW_SHOW);
	
	CString str;
	m_RegCtrl.AESGetSeedProc(m_EncodeBuff);
	for (i = 0; i < 11; i++)
	{
		str.Format("%02x ", m_EncodeBuff[i]);
		m_strACode += str;
	}
	str.Format("%02x", m_EncodeBuff[11]);
	m_strACode += str;
	
	m_strECode.Format("");
	UpdateData(FALSE);

	m_DayEdit.SetWindowText("0");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRegisterDlg::StrToHex(BYTE *pHexBuff, int HexCnt, char *pStrBuff)
{
	int ptr = 0, idx = 0, inv = 0;

	while ((pStrBuff[ptr] != '\0') && (idx < HexCnt))
	{
		if((pStrBuff[ptr] >= '0') && (pStrBuff[ptr] <= '9')) 
		{
			if ((inv % 2) != 0)
			{
				pHexBuff[idx] = (pStrBuff[ptr] - 0x30) | pHexBuff[idx];
			}
			else
			{
				pHexBuff[idx] = (pStrBuff[ptr] - 0x30) * 16;
			}
			inv = inv + 1;
		}
		else if ((pStrBuff[ptr] >= 'a') && (pStrBuff[ptr] <= 'f'))
		{
			if ((inv % 2) != 0)
			{
				pHexBuff[idx] = (pStrBuff[ptr] - 0x57) | pHexBuff[idx];
			}
			else
			{
				pHexBuff[idx] = (pStrBuff[ptr] - 0x57) * 16;
			}
			inv = inv + 1;
		}
		else if ((pStrBuff[ptr] >= 'A') && (pStrBuff[ptr] <= 'F'))
		{
			if ((inv % 2) != 0)
			{
				pHexBuff[idx] = (pStrBuff[ptr] - 0x37) | pHexBuff[idx];
			}
			else
			{
				pHexBuff[idx] = (pStrBuff[ptr] - 0x37) * 16;
			}
			inv = inv + 1;
		}
		else
		{
			return FALSE;
		}

		ptr = ptr + 1;

		if ((inv % 2) == 0)
		{
			if (pStrBuff[ptr] == ' ')
			{
				idx = idx + 1;
				ptr = ptr + 1;
			}
			else if (pStrBuff[ptr] == '\0')
			{
				break;
			}
			else
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

void CRegisterDlg::OnRegister() 
{
	int  i;
	CString str, str1;
	BYTE m_PasswordBuff[16];
	BYTE m_DecodeBuff[16];

	m_bIsRegister = FALSE;
	
	m_DayEdit.GetWindowText(str);
	DWORD AccessDays = atoi(str);

	m_EncodeBuff[12] = (BYTE)(AccessDays >> 24);
	m_EncodeBuff[13] = (BYTE)(AccessDays >> 16);
	m_EncodeBuff[14] = (BYTE)(AccessDays >> 8);
	m_EncodeBuff[15] = (BYTE)(AccessDays >> 0);

	UpdateData(TRUE);
	StrToHex(m_PasswordBuff, 16, m_strECode.GetBuffer(m_strECode.GetLength()));
	
	if (m_RegCtrl.AESDecodeProc(m_DecodeBuff, &m_EncodeBuff[0], &m_EncodeBuff[6], &m_EncodeBuff[12]) == TRUE)
	{
		for (i = 0 ; i < 16; i++)
		{
			if (m_DecodeBuff[i] != m_PasswordBuff[i])
			{
				MessageBox("密码错误, 请重新确认输入");
				m_bIsRegister = FALSE;
				return;
			}
		}

		if (i >= 16)
		{
			str.Empty();
			str1.Empty();
			for (i = 0; i < 5; i++)
			{
				str1.Format("%02x ", m_EncodeBuff[6 + i]);
				str = str + str1;
			}
			str1.Format("%02x", m_EncodeBuff[6 + i]);
			str = str + str1;
			m_RegCtrl.WriteRegtryStrValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Spunky\\iControl", "Random", str);

			str.Empty();
			str1.Empty();
			for (i = 0; i < 3; i++)
			{
				str1.Format("%02x ", m_EncodeBuff[12 + i]);
				str = str + str1;
			}
			str1.Format("%02x", m_EncodeBuff[12 + i]);
			str = str + str1;
			m_RegCtrl.WriteRegtryStrValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Spunky\\iControl", "AccessDays", str);

			str.Empty();
			str1.Empty();
			for (i = 0; i < 15; i++)
			{
				str1.Format("%02x ", m_PasswordBuff[i]);
				str = str + str1;
			}
			str1.Format("%02x", m_PasswordBuff[i]);
			str = str + str1;
			m_RegCtrl.WriteRegtryStrValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Spunky\\iControl", "Password", str);

			m_iAccessDays = ((DWORD)(m_EncodeBuff[12]) << 24) | ((DWORD)(m_EncodeBuff[13]) << 16) 
								| ((DWORD)(m_EncodeBuff[14]) << 8)  | ((DWORD)(m_EncodeBuff[15]) << 0);
			m_bIsRegister = TRUE;
		}
	}
	else
	{
		MessageBox("解码运行错误, 4-参数....");
	}

	CDialog::OnOK();
}

int CRegisterDlg::GetAccessDays(void)
{
	return m_iAccessDays;
}

void CRegisterDlg::CleanRegedit(void)
{
	m_RegCtrl.WriteRegtryStrValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Spunky\\iControl", "AccessDays", "");
	m_RegCtrl.WriteRegtryStrValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Spunky\\iControl", "Password", "");
	m_RegCtrl.WriteRegtryStrValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Spunky\\iControl", "Random", "");
}

void CRegisterDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_bIsRegister = FALSE;
	CDialog::OnCancel();
}

HBRUSH CRegisterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	int CtrlID = pWnd->GetDlgCtrlID();
	if (CtrlID == IDC_HEAD_STATIC && nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SelectObject(m_pStaticAFont);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 24, 255));			// 字体颜色
        pDC->SetBkColor(RGB(0, 0, 255));		    // 字体背景色
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	else if (CtrlID == IDC_CODE_STATIC && nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SelectObject(m_pStaticBFont);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 24, 255));			// 字体颜色
        pDC->SetBkColor(RGB(0, 0, 255));		    // 字体背景色
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	else if (CtrlID == IDC_CODE_STATIC2 && nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SelectObject(m_pStaticBFont);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 24, 255));			// 字体颜色
        pDC->SetBkColor(RGB(0, 0, 255));		    // 字体背景色
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	else if (CtrlID == IDC_A_EDIT && nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SelectObject(m_pEditAFont);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(64, 64, 64));			// 字体颜色
        pDC->SetBkColor(RGB(225, 225, 225));		// 字体背景色
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	else if (CtrlID == IDC_E_EDIT && nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SelectObject(m_pEditBFont);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(64, 64, 64));			// 字体颜色
        pDC->SetBkColor(RGB(225, 225, 225));		// 字体背景色
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	else if (CtrlID == IDC_DAY_EDIT && nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SelectObject(m_pEditBFont);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(64, 64, 64));			// 字体颜色
        pDC->SetBkColor(RGB(225, 225, 225));		// 字体背景色
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}
	else if (nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = ::CreateSolidBrush(RGB(225, 225, 225));
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CRegisterDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_pStaticAFont->DeleteObject();
	m_pStaticBFont->DeleteObject();
	m_pEditAFont->DeleteObject();
	m_pEditBFont->DeleteObject();
	delete m_pStaticAFont;
	delete m_pStaticBFont;
	delete m_pEditAFont;
	delete m_pEditBFont;
}

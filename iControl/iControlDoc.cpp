// iControlDoc.cpp : implementation of the CIControlDoc class
//
#include "stdafx.h"
#include "iControl.h"
#include "iControlDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define THE_CONFIG_TABLE_COL			4			// ����Ҥ����, ���Ҥ����, ����, ���绰
#define THE_RESCFG_TABLE_COL			6			// �¶ȸ���, ѹ������, ʪ�ȸ���, �������, �ܳ�λ��, ��ǰʹ�õı�׼����
#define THE_PARACFG_TABLE_COL			12			// �������ñ�����
#define THE_TMCFG_TABLE_COL				5			// �ն����ñ�����
#define THE_CARHEAT_TABLE_COL			2			// ��λ����������
#define THE_GROSS_TABLE_COL				5			// ����������
#define THE_ADJUST_TABLE_COL			3			// У��������

/////////////////////////////////////////////////////////////////////////////
// CIControlDoc

IMPLEMENT_DYNCREATE(CIControlDoc, CDocument)

BEGIN_MESSAGE_MAP(CIControlDoc, CDocument)
	//{{AFX_MSG_MAP(CIControlDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIControlDoc construction/destruction

CIControlDoc::CIControlDoc()
{
	// TODO: add one-time construction code here	
//	_CrtSetBreakAlloc(322);

	m_bDBOpen = FALSE;
	m_pSqlite = NULL;
	m_iDevPower = -1;
	m_strHistoryQuery = "";
	DataBaseInit();
}

CIControlDoc::~CIControlDoc()
{
	if (m_pSqlite != NULL)
	{
		sqlite3_close(m_pSqlite);
	}

	m_pLoginTbl->Close();
	m_pKilnInfoTbl->Close();
	m_pComCfgTbl->Close();
	m_pChannelCfgTbl->Close();
	m_pTerminalCfgTbl->Close();
	m_pGrossTbl->Close();


	delete m_pLoginTbl;
	delete m_pKilnInfoTbl;
	delete m_pComCfgTbl;
	delete m_pChannelCfgTbl;
	delete m_pTerminalCfgTbl;
	delete m_pGrossTbl;
	delete m_pAutoCtrlTbl;

	for (int i = 0; i < m_iKilnNum; i++)
	{	
		m_pStandardTbl[i]->Close();
		m_pHisTbl[i]->Close();

		delete m_pStandardTbl[i];
		delete m_pHisTbl[i];

		delete [] m_pKilnBuff[i].m_pTempObjectBuff;
		delete [] m_pKilnBuff[i].m_pTempAObjectBuff;
		delete [] m_pKilnBuff[i].m_pPressObjectBuff;
		delete [] m_pKilnBuff[i].m_pHumiObjectBuff;
		delete [] m_pKilnBuff[i].m_pMotorObjectBuff;
		delete [] m_pKilnBuff[i].m_pGrossObjectBuff;
		delete [] m_pKilnBuff[i].m_pCtrlObjectBuff;

		delete [] m_pKilnBuffForCurve[i].m_pTempObjectBuff;
		delete [] m_pKilnBuffForCurve[i].m_pTempAObjectBuff;
		delete [] m_pKilnBuffForCurve[i].m_pPressObjectBuff;
		delete [] m_pKilnBuffForCurve[i].m_pHumiObjectBuff;
		delete [] m_pKilnBuffForCurve[i].m_pMotorObjectBuff;
		delete [] m_pKilnBuffForCurve[i].m_pGrossObjectBuff;
		delete [] m_pKilnBuffForCurve[i].m_pCtrlObjectBuff;
	}

	delete [] m_pKilnBuff;	
	delete [] m_pKilnBuffForCurve;	
	delete [] m_pStandardTbl;
	delete [] m_pHisTbl;
	delete [] m_TerminalCfg.m_pTerminalObjectBuff;
}

BOOL CIControlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	m_bScrollBarCreate = FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CIControlDoc serialization

void CIControlDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CIControlDoc diagnostics

#ifdef _DEBUG
void CIControlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIControlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIControlDoc commands

// ֧������·��תUTF8��ʽ, ����SQLITE��׼
CString CIControlDoc::MbcsToUtf8(const char *file)
{
	CString str;
	WCHAR   *pwchar=0;
	CHAR    *pchar=0;
	int len=0;
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	len=MultiByteToWideChar(codepage, 0, file, -1, NULL,0);
	pwchar=new WCHAR[len];
	if(pwchar!=0)
	{
		len = MultiByteToWideChar(codepage, 0, file, -1, pwchar, len);
		if( len!=0 ) 
		{
			len = WideCharToMultiByte(CP_UTF8, 0, pwchar, -1, 0, 0, 0, 0);
			pchar=new CHAR[len];
			if(pchar!=0)
			{
				len = WideCharToMultiByte(CP_UTF8, 0, pwchar, -1, pchar, len,0, 0);
				if(len!=0)                 
				{
					str=pchar;                    
				}
			}
		}
	}

	if (pchar != NULL)
	{
		delete [] pchar;
	}

	if (pwchar != NULL)
	{
		delete [] pwchar;
	}
	return str;
}

BOOL CIControlDoc::ImportDataBase(const char *pFileName)
{	
	CString str;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					"SQL Files (*.sql)|*.sql|All Files (*.*)|*.*||", NULL);
	if (dlg.DoModal() == IDCANCEL)
	{
		AfxGetMainWnd()->MessageBox("ʹ��Ĭ�����ݿ�, ���ڳ����������������ݿ�", "���ݿ����");
		str = "./cfg/InitialCfg.sql";				// �̶���ַ
	}
	else
	{
		str = dlg.GetPathName();
	}
	
	int res;
	char *zErr;
	CStdioFile file;
	if (file.Open(str, CFile::modeReadWrite, NULL) == FALSE)
	{
		return FALSE;
	}
	
	res = sqlite3_open_v2(pFileName, &m_pSqlite, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);
	if (res != SQLITE_OK)
	{
		sqlite3_close(m_pSqlite);
		return FALSE;
	}
	
	while (file.ReadString(str))
	{
		res = sqlite3_exec(m_pSqlite, str.GetBuffer(MAX_PATH), NULL, NULL, &zErr);
		if (res != SQLITE_OK)
		{
			sqlite3_free(zErr);
			sqlite3_close(m_pSqlite);
			file.Close();
			return FALSE;
		}
	}
	
	sqlite3_close(m_pSqlite);
	file.Close();
	return TRUE;
}

BOOL CIControlDoc::DataBaseInit(void)
{
	int i, j, col_index;
	CString str, str_utf8, str_ini;

	//////////////////////////////////////////////////////////////////////////
	// ��õ�ǰ����Ŀ¼��ΪĬ��Ŀ¼
	//////////////////////////////////////////////////////////////////////////
	CString strDefaultPath;
	char file_path[MAX_PATH];
	GetModuleFileName(NULL, file_path, MAX_PATH);
	strDefaultPath = file_path;
	int nTemp = strDefaultPath.ReverseFind('\\');
	strDefaultPath = strDefaultPath.Left(nTemp);
	str_ini.Format("%s\\setting.ini", strDefaultPath);

	GetPrivateProfileString("DBDirPath", "���ݿ������д��·��", 
							"sqlite3.exe", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));	

	m_strShellPath.Format("%s\\db\\%s", strDefaultPath, str);

	GetPrivateProfileString("DBDirPath", "���ݿ���·��", 
							"RunDB.db", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	// ���Ե�ַת����UTF8, �Ա�֧������·��
	m_strDBPath.Format("%s\\db\\%s", strDefaultPath, str);
	str_utf8 = MbcsToUtf8(m_strDBPath.GetBuffer(MAX_PATH));

	GetPrivateProfileString("DBDirPath", "�����ļ����·��", 
							"DataBaseCFG.sql", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	m_strCFGPath.Format("%s\\cfg\\%s", strDefaultPath, str);

	GetPrivateProfileString("DBDirPath", "��־�ļ����·��", 
							"AutoCtrl.log", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));
	
	m_strLogPath.Format("%s\\cfg\\%s", strDefaultPath, str);

	GetPrivateProfileString("DBDirPath", "�������ݿ���·��", 
							"D:\\iControl\\RunDBBak.sql", 
							m_strDBBakPath.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	GetPrivateProfileString("BKPicDirPath", "����ͼƬ���·��", 
							"bk.jpg", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	m_strBKPicPath.Format("%s\\pic\\%s", strDefaultPath, str);

	GetPrivateProfileString("RTPicDirPath", "ʵʱͼƬ���·��", 
							"bk1.jpg", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));
	
	m_strRTPicPath.Format("%s\\pic\\%s", strDefaultPath, str);

	GetPrivateProfileString("RealTimeName", "1��ʵʱ�¶�����", 
							"1�����Ҥʵʱ�¶�", 
							m_strRealTimeName[0].GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	GetPrivateProfileString("RealTimeName", "2��ʵʱ�¶�����", 
							"2�����Ҥʵʱ�¶�", 
							m_strRealTimeName[1].GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	GetPrivateProfileString("RealTimeName", "3��ʵʱ�¶�����", 
							"3�����Ҥʵʱ�¶�", 
							m_strRealTimeName[2].GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	GetPrivateProfileString("RealTimeName", "4��ʵʱ�¶�����", 
							"4�����Ҥʵʱ�¶�", 
							m_strRealTimeName[3].GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	m_iHisSaveInternal = GetPrivateProfileInt("HistoryRecord", "��ʷ���ݴ洢���", 3000, str_ini.GetBuffer(MAX_PATH));

	// �����޸�: ����������������Ϊ����Ҥ
	m_iGrossProcMaxTime[0] = GetPrivateProfileInt("GrossProcMaxTime", "������ʱ��1", 150, str_ini.GetBuffer(MAX_PATH));
	m_iGrossProcMaxTime[1] = GetPrivateProfileInt("GrossProcMaxTime", "������ʱ��2", 150, str_ini.GetBuffer(MAX_PATH));
	m_iGrossProcMaxTime[2] = GetPrivateProfileInt("GrossProcMaxTime", "������ʱ��3", 150, str_ini.GetBuffer(MAX_PATH));
	m_iGrossProcMaxTime[3] = GetPrivateProfileInt("GrossProcMaxTime", "������ʱ��4", 150, str_ini.GetBuffer(MAX_PATH));
	
	m_iUpWarn[0] = GetPrivateProfileInt("Warn", "�¶ȱ�������1", 1000, str_ini.GetBuffer(MAX_PATH));
	m_iDnWarn[0] = GetPrivateProfileInt("Warn", "�¶ȱ�������1", 800,  str_ini.GetBuffer(MAX_PATH));
	
	m_iUpWarn[1] = GetPrivateProfileInt("Warn", "�¶ȱ�������2", 1000, str_ini.GetBuffer(MAX_PATH));
	m_iDnWarn[1] = GetPrivateProfileInt("Warn", "�¶ȱ�������2", 800,  str_ini.GetBuffer(MAX_PATH));
	
	m_iUpWarn[2] = GetPrivateProfileInt("Warn", "�¶ȱ�������3", 1000, str_ini.GetBuffer(MAX_PATH));
	m_iDnWarn[2] = GetPrivateProfileInt("Warn", "�¶ȱ�������3", 800,  str_ini.GetBuffer(MAX_PATH));
	
	m_iUpWarn[3] = GetPrivateProfileInt("Warn", "�¶ȱ�������4", 1000, str_ini.GetBuffer(MAX_PATH));
	m_iDnWarn[3] = GetPrivateProfileInt("Warn", "�¶ȱ�������4", 800,  str_ini.GetBuffer(MAX_PATH));
/////////////////////////////////////
	
	GetPrivateProfileString("SaleInfo", "���۵绰", 
							"13983089826", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	m_strSalePhone.Format("%s", str);
	
	// �ж��ļ��Ƿ����, �����ھʹ���
	CFileFind find;
	if (!find.FindFile(m_strDBPath))
	{
		// �������ɹ�ֱ���˳�, ʹ��UTF8��ʽ·�������ݿ�
		if (!ImportDataBase(str_utf8))
		{			 
			AfxGetMainWnd()->MessageBox("��ʼ�����ݿⲻ�ɹ�", "���ݿ����");
			return FALSE;
		}
		else
		{
			// ��������ڴ������ݿ��, �������ڴ����ݿ���, �������������ȷ
			AfxGetMainWnd()->MessageBox("�������ݿ�ɹ�, ��������������", "���ݿⴴ���ɹ�"); 
			return TRUE;		
		}
	}

	/************************************************************************/
	/* Ҥ�������ñ��ʼ��                                                   */
	/************************************************************************/
	m_pKilnInfoTbl = new CHisTbl(11);							// �������ݿ���	
	m_pKilnInfoTbl->SetTableName("KilnInfo");					// ָ�����ݿ�ı�����
	
	// ָ�����ݿ�ı���ֶ���
	col_index = 0; 
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_id",    MYDB_TYPE_INT);		// Ҥ���
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_type",  MYDB_TYPE_INT);		// Ҥ����
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_total", MYDB_TYPE_INT);		// Ҥ�ܳ��Ȼ�Ҥ�ܳ�λ��
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_temps", MYDB_TYPE_INT);		// Ҥ�¶�����
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_tempas", MYDB_TYPE_INT);		// Ҥ���¶�����
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_press", MYDB_TYPE_INT);		// Ҥѹ������
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_humis", MYDB_TYPE_INT);		// Ҥʪ������
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_motor", MYDB_TYPE_INT);		// Ҥ�������
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_gross", MYDB_TYPE_INT);		// Ҥ�����Ƿ����:0 or 1
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_ctrl",  MYDB_TYPE_INT);		// Ҥ���Զ����Ƶ������
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_std",   MYDB_TYPE_INT);		// Ҥ��׼�¶Ⱥ�

	if (!m_pKilnInfoTbl->Open(str_utf8))
	{	
		return m_bDBOpen;
	}
	
	m_pKilnInfoTbl->SetFilter("");
	m_pKilnInfoTbl->Query();
	m_iKilnNum = m_pKilnInfoTbl->GetCount();			// �õ��ܵ�Ҥ����
	if (m_iKilnNum < 1)
	{
		return m_bDBOpen;
	}

	m_pKilnInfoTbl->SetFilter("[kiln_gross]=1");
	m_pKilnInfoTbl->Query();
	m_iGrossTotalNum = m_pKilnInfoTbl->GetCount();		// �õ���Ҫ����ͳ��Ҥ������;

	m_pKilnInfoTbl->SetFilter("");
	m_pKilnInfoTbl->Query();

	m_pKilnBuff         = new KilnObject[m_iKilnNum];
	m_pKilnBuffForCurve = new KilnObject[m_iKilnNum];
	for (i = 0; i < m_iKilnNum; i++)
	{
		m_pKilnInfoTbl->Move(i + 1);					// �ƶ�����1������

		m_pKilnBuff[i].index           = *((int *)(m_pKilnInfoTbl->m_pColsName[0]));
		m_pKilnBuff[i].type            = *((int *)(m_pKilnInfoTbl->m_pColsName[1]));
		m_pKilnBuff[i].TotalKiln       = *((int *)(m_pKilnInfoTbl->m_pColsName[2]));
		m_pKilnBuff[i].TemperatureNum  = *((int *)(m_pKilnInfoTbl->m_pColsName[3]));
		m_pKilnBuff[i].TemperatureANum = *((int *)(m_pKilnInfoTbl->m_pColsName[4]));
		m_pKilnBuff[i].PressNum        = *((int *)(m_pKilnInfoTbl->m_pColsName[5]));
		m_pKilnBuff[i].HumidityNum     = *((int *)(m_pKilnInfoTbl->m_pColsName[6]));
		m_pKilnBuff[i].MotorNum        = *((int *)(m_pKilnInfoTbl->m_pColsName[7]));
		m_pKilnBuff[i].GrossNum        = *((int *)(m_pKilnInfoTbl->m_pColsName[8]));
		m_pKilnBuff[i].ControlNum      = *((int *)(m_pKilnInfoTbl->m_pColsName[9]));
		m_pKilnBuff[i].StandardIndex   = *((int *)(m_pKilnInfoTbl->m_pColsName[10]));

		m_pKilnBuffForCurve[i].index           = *((int *)(m_pKilnInfoTbl->m_pColsName[0]));
		m_pKilnBuffForCurve[i].type            = *((int *)(m_pKilnInfoTbl->m_pColsName[1]));
		m_pKilnBuffForCurve[i].TotalKiln       = *((int *)(m_pKilnInfoTbl->m_pColsName[2]));
		m_pKilnBuffForCurve[i].TemperatureNum  = *((int *)(m_pKilnInfoTbl->m_pColsName[3]));
		m_pKilnBuffForCurve[i].TemperatureANum = *((int *)(m_pKilnInfoTbl->m_pColsName[4]));
		m_pKilnBuffForCurve[i].PressNum        = *((int *)(m_pKilnInfoTbl->m_pColsName[5]));
		m_pKilnBuffForCurve[i].HumidityNum     = *((int *)(m_pKilnInfoTbl->m_pColsName[6]));
		m_pKilnBuffForCurve[i].MotorNum        = *((int *)(m_pKilnInfoTbl->m_pColsName[7]));
		m_pKilnBuffForCurve[i].GrossNum        = *((int *)(m_pKilnInfoTbl->m_pColsName[8]));
		m_pKilnBuffForCurve[i].ControlNum      = *((int *)(m_pKilnInfoTbl->m_pColsName[9]));
		m_pKilnBuffForCurve[i].StandardIndex   = *((int *)(m_pKilnInfoTbl->m_pColsName[10]));

		// �����¶ȶ���
		m_pKilnBuff[i].m_pTempObjectBuff         = NULL;
		m_pKilnBuffForCurve[i].m_pTempObjectBuff = NULL;
		if (m_pKilnBuff[i].TemperatureNum > 0)
		{
			m_pKilnBuff[i].m_pTempObjectBuff         = new TemperatureObject[m_pKilnBuff[i].TemperatureNum];
			m_pKilnBuffForCurve[i].m_pTempObjectBuff = new TemperatureObject[m_pKilnBuff[i].TemperatureNum];
			for (j = 0; j < m_pKilnBuff[i].TemperatureNum; j++)
			{
				m_pKilnBuff[i].m_pTempObjectBuff[j].TemIdx  = 0;
				m_pKilnBuff[i].m_pTempObjectBuff[j].BufIdx  = j; 
				m_pKilnBuff[i].m_pTempObjectBuff[j].Value   = 10000;
				m_pKilnBuff[i].m_pTempObjectBuff[j].SValue  = 0;
				m_pKilnBuff[i].m_pTempObjectBuff[j].CarIdex = 0;
				m_pKilnBuff[i].m_pTempObjectBuff[j].Gain    = 1.0;
				m_pKilnBuff[i].m_pTempObjectBuff[j].Zero    = 0.0;
				m_pKilnBuff[i].m_pTempObjectBuff[j].strName[0] = '\0';
			}
		}

		// �������¶ȶ���
		m_pKilnBuff[i].m_pTempAObjectBuff         = NULL;
		m_pKilnBuffForCurve[i].m_pTempAObjectBuff = NULL;
		if (m_pKilnBuff[i].TemperatureANum > 0)
		{
			m_pKilnBuff[i].m_pTempAObjectBuff         = new TemperatureObject[m_pKilnBuff[i].TemperatureANum];
			m_pKilnBuffForCurve[i].m_pTempAObjectBuff = new TemperatureObject[m_pKilnBuff[i].TemperatureANum];
			for (j = 0; j < m_pKilnBuff[i].TemperatureANum; j++)
			{
				m_pKilnBuff[i].m_pTempAObjectBuff[j].TemIdx  = 1;
				m_pKilnBuff[i].m_pTempAObjectBuff[j].BufIdx  = j; 
				m_pKilnBuff[i].m_pTempAObjectBuff[j].Value   = 10000;
				m_pKilnBuff[i].m_pTempAObjectBuff[j].SValue  = 0;
				m_pKilnBuff[i].m_pTempAObjectBuff[j].CarIdex = 0;
				m_pKilnBuff[i].m_pTempAObjectBuff[j].Gain    = 1.0;
				m_pKilnBuff[i].m_pTempAObjectBuff[j].Zero    = 0.0;
				m_pKilnBuff[i].m_pTempAObjectBuff[j].strName[0] = '\0';
			}
		}

		// ����ѹ������
		m_pKilnBuff[i].m_pPressObjectBuff         = NULL;
		m_pKilnBuffForCurve[i].m_pPressObjectBuff = NULL;
		if (m_pKilnBuff[i].PressNum > 0)
		{
			m_pKilnBuff[i].m_pPressObjectBuff         = new PressObject[m_pKilnBuff[i].PressNum];
			m_pKilnBuffForCurve[i].m_pPressObjectBuff = new PressObject[m_pKilnBuff[i].PressNum];
			for (j = 0; j < m_pKilnBuff[i].PressNum; j++)
			{
				m_pKilnBuff[i].m_pPressObjectBuff[j].TemIdx  = 2;
				m_pKilnBuff[i].m_pPressObjectBuff[j].BufIdx  = j;
				m_pKilnBuff[i].m_pPressObjectBuff[j].Value   = 100000;
				m_pKilnBuff[i].m_pPressObjectBuff[j].Gain    = 1.0;
				m_pKilnBuff[i].m_pPressObjectBuff[j].Zero    = 0.0;
				m_pKilnBuff[i].m_pPressObjectBuff[j].strName[0] = '\0';

				m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].TemIdx  = 2;
				m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].BufIdx  = j;
				m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].Value   = 100000;
				m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].Gain    = 1.0;
				m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].Zero    = 0.0;
				m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].strName[0] = '\0';
			}
		}

		// ����ʪ�ȶ���
		m_pKilnBuff[i].m_pHumiObjectBuff         = NULL;
		m_pKilnBuffForCurve[i].m_pHumiObjectBuff = NULL;
		if (m_pKilnBuff[i].HumidityNum > 0)
		{
			m_pKilnBuff[i].m_pHumiObjectBuff         = new HumidityObject[m_pKilnBuff[i].HumidityNum];
			m_pKilnBuffForCurve[i].m_pHumiObjectBuff = new HumidityObject[m_pKilnBuff[i].HumidityNum];
			for (j = 0; j < m_pKilnBuff[i].HumidityNum; j++)
			{
				m_pKilnBuff[i].m_pHumiObjectBuff[j].TemIdx  = 3;
				m_pKilnBuff[i].m_pHumiObjectBuff[j].BufIdx  = j;
				m_pKilnBuff[i].m_pHumiObjectBuff[j].Value   = 100000;
				m_pKilnBuff[i].m_pHumiObjectBuff[j].Gain    = 1.0;
				m_pKilnBuff[i].m_pHumiObjectBuff[j].Zero    = 0.0;
				m_pKilnBuff[i].m_pHumiObjectBuff[j].strName[0] = '\0';

				m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].TemIdx  = 3;
				m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].BufIdx  = j;
				m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].Value   = 100000;
				m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].Gain    = 1.0;
				m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].Zero    = 0.0;
				m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].strName[0] = '\0';
			}
		}

		// �����������
		m_pKilnBuff[i].m_pMotorObjectBuff         = NULL;
		m_pKilnBuffForCurve[i].m_pMotorObjectBuff = NULL;
		if (m_pKilnBuff[i].MotorNum > 0)
		{
			m_pKilnBuff[i].m_pMotorObjectBuff         = new MotorObject[m_pKilnBuff[i].MotorNum];
			m_pKilnBuffForCurve[i].m_pMotorObjectBuff = new MotorObject[m_pKilnBuff[i].MotorNum];
			for (j = 0; j < m_pKilnBuff[i].MotorNum; j++)
			{
				m_pKilnBuff[i].m_pMotorObjectBuff[j].TemIdx    = 4;
				m_pKilnBuff[i].m_pMotorObjectBuff[j].BufIdx    = j;
				m_pKilnBuff[i].m_pMotorObjectBuff[j].TemOutIdx = 4;
				m_pKilnBuff[i].m_pMotorObjectBuff[j].BufOutIdx = j;
				m_pKilnBuff[i].m_pMotorObjectBuff[j].Value     = 10000.0;
				m_pKilnBuff[i].m_pMotorObjectBuff[j].bControl  = FALSE;
				m_pKilnBuff[i].m_pMotorObjectBuff[j].Gain      = 1.0;
				m_pKilnBuff[i].m_pMotorObjectBuff[j].Zero      = 0.0;
				m_pKilnBuff[i].m_pMotorObjectBuff[j].strName[0] = '\0';
			}
		}

		// ������������
		m_pKilnBuff[i].m_pGrossObjectBuff         = NULL;
		m_pKilnBuffForCurve[i].m_pGrossObjectBuff = NULL;
		if (m_pKilnBuff[i].GrossNum > 0)
		{
			m_pKilnBuff[i].m_pGrossObjectBuff         = new GrossObject[m_pKilnBuff[i].GrossNum];
			m_pKilnBuffForCurve[i].m_pGrossObjectBuff = new GrossObject[m_pKilnBuff[i].GrossNum];
			for (j = 0; j < m_pKilnBuff[i].GrossNum; j++)
			{
				m_pKilnBuff[i].m_pGrossObjectBuff[j].TemTrigIdx    = 5;
				m_pKilnBuff[i].m_pGrossObjectBuff[j].TrigIdx       = j;
				m_pKilnBuff[i].m_pGrossObjectBuff[j].TrigInternal  = 15;		// ��λ: ��
				m_pKilnBuff[i].m_pGrossObjectBuff[j].TemBellIdx    = 5;
				m_pKilnBuff[i].m_pGrossObjectBuff[j].BellIdx       = j;
				m_pKilnBuff[i].m_pGrossObjectBuff[j].BellValidTime = 15000;		// ��λ: ����
			}
		}

		// �������ƶ���
		m_pKilnBuff[i].m_pCtrlObjectBuff         = NULL;
		m_pKilnBuffForCurve[i].m_pCtrlObjectBuff = NULL;
		if (m_pKilnBuff[i].ControlNum > 0)
		{
			m_pKilnBuff[i].m_pCtrlObjectBuff         = new CtrlObject[m_pKilnBuff[i].ControlNum];
			m_pKilnBuffForCurve[i].m_pCtrlObjectBuff = new CtrlObject[m_pKilnBuff[i].ControlNum];
			for (j = 0; j < m_pKilnBuff[i].ControlNum; j++)
			{
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].UpLimitOut      = 50.0;
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].DnLimitOut      = 30.0;
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iFrontPoint     = 3;			// ����¶�ǰ����¶ȵ��� >= 1
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iRearPoint      = 2;			// ����¶Ⱥ�����¶ȵ��� >= 1
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iCtrlTime       = 10;			// �������� ����
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].IncOutCoeff     = (float)0.1;	// �������ϵ��, ������С����ı���ϵ��
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iGrossWaitTime  = 5;			// ÿ���Ƴ�����Կصȴ�ʱ��
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iGrossProcTime  = 50;			// ��ʱ�Ƴ�ʱ��
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].bControl        = FALSE;		// ��ǰ�Զ�����״̬
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iWaitTimerCnt   = 0;			// �����Զ����Ƽ�ʱ������
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iProcTimerCnt   = 0;			// ��ʱ�Ƴ���ʱ������
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].BakMotorVal     = 0.0;			// ��Ƶ���ݴ��������
			}
		}

		// ����Ҥ���ʹ�������ʵʱ�������ʷ�������
		switch (m_pKilnBuff[i].type)
		{
		case 0:
			str.Empty();
//			str.Format("%02d �� �� �� Ҥ ʵ ʱ �� ��", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCaption, 0, sizeof(m_pKilnBuff[i].strCaption));
			memcpy(m_pKilnBuff[i].strCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCaption));
			memcpy(m_pKilnBuffForCurve[i].strCaption, str, str.GetLength());

			str.Empty();
//			str.Format("%02d �� �� �� Ҥ ʵ ʱ �� ��", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCurveCaption, 0, sizeof(m_pKilnBuff[i].strCurveCaption));
			memcpy(m_pKilnBuff[i].strCurveCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCurveCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCurveCaption));
			memcpy(m_pKilnBuffForCurve[i].strCurveCaption, str, str.GetLength());
			break;
		case 1:
			str.Empty();
//			str.Format("%02d �� �� �� Ҥ ʵ ʱ �� ��", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCaption, 0, sizeof(m_pKilnBuff[i].strCaption));
			memcpy(m_pKilnBuff[i].strCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCaption));
			memcpy(m_pKilnBuffForCurve[i].strCaption, str, str.GetLength());

			str.Empty();
//			str.Format("%02d �� �� �� Ҥ ʵ ʱ �� ��", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCurveCaption, 0, sizeof(m_pKilnBuff[i].strCurveCaption));
			memcpy(m_pKilnBuff[i].strCurveCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCurveCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCurveCaption));
			memcpy(m_pKilnBuffForCurve[i].strCurveCaption, str, str.GetLength());
			break;
		case 2:
			str.Empty();
//			str.Format("%02d �� ֱ ͨ Ҥ ʵ ʱ �� ��", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCaption, 0, sizeof(m_pKilnBuff[i].strCaption));
			memcpy(m_pKilnBuff[i].strCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCaption));
			memcpy(m_pKilnBuffForCurve[i].strCaption, str, str.GetLength());
			
			str.Empty();
//			str.Format("%02d �� ֱ ͨ Ҥ ʵ ʱ �� ��", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCurveCaption, 0, sizeof(m_pKilnBuff[i].strCurveCaption));
			memcpy(m_pKilnBuff[i].strCurveCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCurveCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCurveCaption));
			memcpy(m_pKilnBuffForCurve[i].strCurveCaption, str, str.GetLength());
			break;
		case 3:
			str.Empty();
//			str.Format("%02d �� �� �� ʽ �� �� Ҥ ʵ ʱ �� ��", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCaption, 0, sizeof(m_pKilnBuff[i].strCaption));
			memcpy(m_pKilnBuff[i].strCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCaption));
			memcpy(m_pKilnBuffForCurve[i].strCaption, str, str.GetLength());
			
			str.Empty();
//			str.Format("%02d �� �� �� ʽ �� �� ʵ ʱ �� ��", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCurveCaption, 0, sizeof(m_pKilnBuff[i].strCurveCaption));
			memcpy(m_pKilnBuff[i].strCurveCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCurveCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCurveCaption));
			memcpy(m_pKilnBuffForCurve[i].strCurveCaption, str, str.GetLength());
			break;
		default:
			break;
		}
	}

	/************************************************************************/
	/* ��¼���ʼ��															*/
	/************************************************************************/
	m_pLoginTbl = new CHisTbl(6);
	m_pLoginTbl->SetTableName("LoginInfo");

	// ָ�����ݿ�ı���ֶ���
	col_index = 0;
	m_pLoginTbl->SetItem(col_index++, "id",       MYDB_TYPE_INT);
	m_pLoginTbl->SetItem(col_index++, "name",     MYDB_TYPE_TEXT);
	m_pLoginTbl->SetItem(col_index++, "pwd",      MYDB_TYPE_TEXT);
	m_pLoginTbl->SetItem(col_index++, "pwr",      MYDB_TYPE_INT);
	m_pLoginTbl->SetItem(col_index++, "phone",    MYDB_TYPE_TEXT);
	m_pLoginTbl->SetItem(col_index++, "mail",     MYDB_TYPE_TEXT);

	if (!m_pLoginTbl->Open(str_utf8))
	{
		return m_bDBOpen;
	}
	
	if (m_pLoginTbl->GetCount() <= 0)
	{
		return m_bDBOpen;
	}

	m_pLoginTbl->MoveFirst();

	m_iDevPower = *((int *)(m_pLoginTbl->m_pColsName[3]));

	/************************************************************************/
	/* ͨѶ���ñ��ʼ��                                                     */
	/************************************************************************/
	m_pComCfgTbl = new CHisTbl(13);
	m_pComCfgTbl->SetTableName("ComCfgInfo");

	// ָ�����ݿ�ı���ֶ���
	col_index = 0;
	m_pComCfgTbl->SetItem(col_index++, "port_num",       MYDB_TYPE_INT);
	m_pComCfgTbl->SetItem(col_index++, "baudrate",       MYDB_TYPE_INT);
	m_pComCfgTbl->SetItem(col_index++, "parity",         MYDB_TYPE_TEXT);
	m_pComCfgTbl->SetItem(col_index++, "data_bits",      MYDB_TYPE_INT);
	m_pComCfgTbl->SetItem(col_index++, "stop_bits",      MYDB_TYPE_INT);
	m_pComCfgTbl->SetItem(col_index++, "timeout",        MYDB_TYPE_INT);
	m_pComCfgTbl->SetItem(col_index++, "scan_time",      MYDB_TYPE_INT);
	m_pComCfgTbl->SetItem(col_index++, "m_serve_ip",     MYDB_TYPE_INT);	// ʵ�����壬DTU���ڲ�����
	m_pComCfgTbl->SetItem(col_index++, "m_serve_port",   MYDB_TYPE_INT);	// ʵ�����壬���������=1
	m_pComCfgTbl->SetItem(col_index++, "s_serve_ip",     MYDB_TYPE_INT);	// ʵ�����壬DTU���ں�
	m_pComCfgTbl->SetItem(col_index++, "s_serve_port",   MYDB_TYPE_INT);	// ʵ�����壬DTU���
	m_pComCfgTbl->SetItem(col_index++, "gross_time",     MYDB_TYPE_INT);
	m_pComCfgTbl->SetItem(col_index++, "bell_time",       MYDB_TYPE_INT);

	if (!m_pComCfgTbl->Open(str_utf8))
	{
		return m_bDBOpen;
	}

	if (m_pComCfgTbl->GetCount() <= 0)
	{
		return m_bDBOpen;
	}

	m_pComCfgTbl->MoveFirst();
	
	m_ComCfg.iPortNum  = *((int *)(m_pComCfgTbl->m_pColsName[0]));
	m_ComCfg.iBaudrate = *((int *)(m_pComCfgTbl->m_pColsName[1]));

	str.Format("%s", *((SQLiteString *)(m_pComCfgTbl->m_pColsName[2])));
	m_ComCfg.cParity = ((char *)(str.GetBuffer(MAX_PATH)))[0];

	m_ComCfg.iDataBits      = *((int *)(m_pComCfgTbl->m_pColsName[3]));
	m_ComCfg.iStopBits      = *((int *)(m_pComCfgTbl->m_pColsName[4]));
	m_ComCfg.iTimeout       = *((int *)(m_pComCfgTbl->m_pColsName[5]));
	m_ComCfg.iScanTime      = *((int *)(m_pComCfgTbl->m_pColsName[6]));
	m_ComCfg.iDtuBaudrate   = *((DWORD *)(m_pComCfgTbl->m_pColsName[7]));
	m_ComCfg.lServePort     = *((WORD *)(m_pComCfgTbl->m_pColsName[8]));
	m_ComCfg.iDtuPortNum    = *((DWORD *)(m_pComCfgTbl->m_pColsName[9]));
	m_ComCfg.lDtuPort       = *((WORD *)(m_pComCfgTbl->m_pColsName[10]));

	for (i = 0; i < m_iKilnNum; i++)
	{
		for (j = 0; j < m_pKilnBuff[i].GrossNum; j++)
		{
			m_pKilnBuff[i].m_pGrossObjectBuff[j].TrigInternal  = *((int *)(m_pComCfgTbl->m_pColsName[11]));
			m_pKilnBuff[i].m_pGrossObjectBuff[j].BellValidTime = *((int *)(m_pComCfgTbl->m_pColsName[12]));
		}
	}

	/************************************************************************/
	/* �ն����ñ��ʼ��                                                     */
	/************************************************************************/
	m_pTerminalCfgTbl = new CHisTbl(6);
	m_pTerminalCfgTbl->SetTableName("TerminalCfgInfo");

	// ָ�����ݿ�ı���ֶ���
	col_index = 0;
	m_pTerminalCfgTbl->SetItem(col_index++, "id",       MYDB_TYPE_INT);
	m_pTerminalCfgTbl->SetItem(col_index++, "dev_id",   MYDB_TYPE_INT);
	m_pTerminalCfgTbl->SetItem(col_index++, "dev_cmd",  MYDB_TYPE_INT);
	m_pTerminalCfgTbl->SetItem(col_index++, "dev_chn",  MYDB_TYPE_INT);
	m_pTerminalCfgTbl->SetItem(col_index++, "dev_addr", MYDB_TYPE_INT);
	m_pTerminalCfgTbl->SetItem(col_index++, "dev_type", MYDB_TYPE_INT);

	if (!m_pTerminalCfgTbl->Open(str_utf8))
	{
		return m_bDBOpen;
	}
	
	m_pTerminalCfgTbl->SetFilter("");
	m_pTerminalCfgTbl->SetSort("[id] ASC");
	m_pTerminalCfgTbl->Query();
	m_TerminalCfg.iTerminalNum = m_pTerminalCfgTbl->GetCount();		// �豸����
	if (m_TerminalCfg.iTerminalNum <= 0)
	{
		return m_bDBOpen;
	}

	m_TerminalCfg.m_pTerminalObjectBuff = new TerminalObject[m_TerminalCfg.iTerminalNum];

	for (i = 0; i < m_TerminalCfg.iTerminalNum; i++)
	{
		m_pTerminalCfgTbl->Move(i + 1);				// Move ������1Ϊ��һ����, ������0
		m_TerminalCfg.m_pTerminalObjectBuff[i].DevId   = *((int *)(m_pTerminalCfgTbl->m_pColsName[1]));
		m_TerminalCfg.m_pTerminalObjectBuff[i].DevCmd  = *((int *)(m_pTerminalCfgTbl->m_pColsName[2]));
		m_TerminalCfg.m_pTerminalObjectBuff[i].DevChn  = *((int *)(m_pTerminalCfgTbl->m_pColsName[3]));
		m_TerminalCfg.m_pTerminalObjectBuff[i].DevAddr = *((int *)(m_pTerminalCfgTbl->m_pColsName[4]));
		m_TerminalCfg.m_pTerminalObjectBuff[i].DevType = *((int *)(m_pTerminalCfgTbl->m_pColsName[5]));
	}

	/************************************************************************/
	/* ͨ�����ñ��ʼ��                                                     */
	/************************************************************************/
	m_pChannelCfgTbl = new CHisTbl(9);
	m_pChannelCfgTbl->SetTableName("ChannelCfgInfo");
	
	// ָ�����ݿ�ı���ֶ���
	col_index = 0;
	m_pChannelCfgTbl->SetItem(col_index++, "id",        MYDB_TYPE_INT);			// ������
	m_pChannelCfgTbl->SetItem(col_index++, "tem_idx",   MYDB_TYPE_INT);			// �豸��
	m_pChannelCfgTbl->SetItem(col_index++, "phy_idx",   MYDB_TYPE_INT);			// ����ͨ����
	m_pChannelCfgTbl->SetItem(col_index++, "kiln_idx",  MYDB_TYPE_INT);			// Ҥ��
	m_pChannelCfgTbl->SetItem(col_index++, "chn_type",  MYDB_TYPE_INT);			// ͨ������
	m_pChannelCfgTbl->SetItem(col_index++, "car_loc",   MYDB_TYPE_INT);			// ��λ��
	m_pChannelCfgTbl->SetItem(col_index++, "chn_name",  MYDB_TYPE_TEXT);		// ����
	m_pChannelCfgTbl->SetItem(col_index++, "adj_gain",  MYDB_TYPE_DOUBLE);		// У���Ŵ�ϵ��
	m_pChannelCfgTbl->SetItem(col_index++, "adj_zero",  MYDB_TYPE_DOUBLE);		// У�����ϵ��

	if (!m_pChannelCfgTbl->Open(str_utf8))
	{
		return m_bDBOpen;
	}
	
	if (m_pChannelCfgTbl->GetCount() <= 0)
	{
		return m_bDBOpen;
	}

	for (i = 0; i < m_iKilnNum; i++)
	{
		/************************************************************************/
		/* �¶�ͨ����ʼ��                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=0", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[id] ASC");
		m_pChannelCfgTbl->Query();	

		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuffForCurve[i].TemperatureNum)
		{
			for (j = 0; j < m_pKilnBuffForCurve[i].TemperatureNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuffForCurve[i].m_pTempObjectBuff[j].TemIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1]));
				m_pKilnBuffForCurve[i].m_pTempObjectBuff[j].BufIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
				m_pKilnBuffForCurve[i].m_pTempObjectBuff[j].CarIdex = *((int *)(m_pChannelCfgTbl->m_pColsName[5]));

				str.Empty();
				str.Format("%s", *((SQLiteString *)(m_pChannelCfgTbl->m_pColsName[6])));
				strcpy(m_pKilnBuffForCurve[i].m_pTempObjectBuff[j].strName, str.GetBuffer(21));

				m_pKilnBuffForCurve[i].m_pTempObjectBuff[j].Gain    = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
				m_pKilnBuffForCurve[i].m_pTempObjectBuff[j].Zero    = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
			}
		}

		/************************************************************************/
		/* ���¶�ͨ����ʼ��                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=1", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// ����λ���Ⱥ�����
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuffForCurve[i].TemperatureANum)
		{
			for (j = 0; j < m_pKilnBuffForCurve[i].TemperatureANum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuffForCurve[i].m_pTempAObjectBuff[j].TemIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuffForCurve[i].m_pTempAObjectBuff[j].BufIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
				m_pKilnBuffForCurve[i].m_pTempAObjectBuff[j].CarIdex = *((int *)(m_pChannelCfgTbl->m_pColsName[5]));

				str.Empty();
				str.Format("%s", *((SQLiteString *)(m_pChannelCfgTbl->m_pColsName[6])));
				strcpy(m_pKilnBuffForCurve[i].m_pTempAObjectBuff[j].strName, str.GetBuffer(21));

				m_pKilnBuffForCurve[i].m_pTempAObjectBuff[j].Gain    = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
				m_pKilnBuffForCurve[i].m_pTempAObjectBuff[j].Zero    = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
			}
		}

		/************************************************************************/
		/* ѹ��ͨ����ʼ��                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=2", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// ����λ���Ⱥ�����
		m_pChannelCfgTbl->Query();
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuffForCurve[i].PressNum)
		{
			for (j = 0; j < m_pKilnBuffForCurve[i].PressNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].TemIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].BufIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 

				str.Empty();
				str.Format("%s", *((SQLiteString *)(m_pChannelCfgTbl->m_pColsName[6])));
				strcpy(m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].strName, str.GetBuffer(21));
				
				m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].Gain    = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
				m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].Zero    = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
			}
		}

		/************************************************************************/
		/* ʪ��ͨ����ʼ��                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=3", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// ����λ���Ⱥ�����
		m_pChannelCfgTbl->Query();		
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuffForCurve[i].HumidityNum)
		{
			for (j = 0; j < m_pKilnBuffForCurve[i].HumidityNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].TemIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].BufIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 

				str.Empty();
				str.Format("%s", *((SQLiteString *)(m_pChannelCfgTbl->m_pColsName[6])));
				strcpy(m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].strName, str.GetBuffer(21));
				
				m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].Gain    = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
				m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].Zero    = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
			}
		}

		/************************************************************************/
		/* �������ͨ����ʼ��                                                   */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=4", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// ����λ���Ⱥ�����
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuffForCurve[i].MotorNum)
		{
			for (j = 0; j < m_pKilnBuffForCurve[i].MotorNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].TemIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].BufIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 

				str.Empty();
				str.Format("%s", *((SQLiteString *)(m_pChannelCfgTbl->m_pColsName[6])));
				strcpy(m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].strName, str.GetBuffer(21));

				m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].Gain    = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
				m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].Zero    = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
			}
		}

		/************************************************************************/
		/* ��������ͨ����ʼ��                                                   */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=5", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("");
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuffForCurve[i].GrossNum)
		{
			for (j = 0; j < m_pKilnBuffForCurve[i].GrossNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuffForCurve[i].m_pGrossObjectBuff[j].TemTrigIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuffForCurve[i].m_pGrossObjectBuff[j].TrigIdx     = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
			}
		}

		/************************************************************************/
		/* �������ͨ����ʼ��                                                   */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=6", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("");
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuffForCurve[i].GrossNum)
		{
			for (j = 0; j < m_pKilnBuffForCurve[i].GrossNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuffForCurve[i].m_pGrossObjectBuff[j].TemBellIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuffForCurve[i].m_pGrossObjectBuff[j].BellIdx     = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
			}
		}

		/************************************************************************/
		/* ������ͨ����ʼ��                                                   */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=7", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("");
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuffForCurve[i].MotorNum)
		{
			for (j = 0; j < m_pKilnBuffForCurve[i].MotorNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].TemOutIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].BufOutIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
			}
		}
	} /* for (i = 0; i < m_iKilnNum; i++) */

	for (i = 0; i < m_iKilnNum; i++)
	{
		/************************************************************************/
		/* �¶�ͨ����ʼ��                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=0", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// ����λ���Ⱥ�����
		m_pChannelCfgTbl->Query();	

		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuff[i].TemperatureNum)
		{
			for (j = 0; j < m_pKilnBuff[i].TemperatureNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuff[i].m_pTempObjectBuff[j].TemIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1]));
				m_pKilnBuff[i].m_pTempObjectBuff[j].BufIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
				m_pKilnBuff[i].m_pTempObjectBuff[j].CarIdex = *((int *)(m_pChannelCfgTbl->m_pColsName[5]));

				str.Empty();
				str.Format("%s", *((SQLiteString *)(m_pChannelCfgTbl->m_pColsName[6])));
				strcpy(m_pKilnBuff[i].m_pTempObjectBuff[j].strName, str.GetBuffer(21));

				m_pKilnBuff[i].m_pTempObjectBuff[j].Gain    = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
				m_pKilnBuff[i].m_pTempObjectBuff[j].Zero    = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
			}
		}

		/************************************************************************/
		/* ���¶�ͨ����ʼ��                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=1", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// ����λ���Ⱥ�����
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuff[i].TemperatureANum)
		{
			for (j = 0; j < m_pKilnBuff[i].TemperatureANum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuff[i].m_pTempAObjectBuff[j].TemIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuff[i].m_pTempAObjectBuff[j].BufIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
				m_pKilnBuff[i].m_pTempAObjectBuff[j].CarIdex = *((int *)(m_pChannelCfgTbl->m_pColsName[5]));

				str.Empty();
				str.Format("%s", *((SQLiteString *)(m_pChannelCfgTbl->m_pColsName[6])));
				strcpy(m_pKilnBuff[i].m_pTempAObjectBuff[j].strName, str.GetBuffer(21));

				m_pKilnBuff[i].m_pTempAObjectBuff[j].Gain    = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
				m_pKilnBuff[i].m_pTempAObjectBuff[j].Zero    = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
			}
		}

		/************************************************************************/
		/* ѹ��ͨ����ʼ��                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=2", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// ����λ���Ⱥ�����
		m_pChannelCfgTbl->Query();
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuff[i].PressNum)
		{
			for (j = 0; j < m_pKilnBuff[i].PressNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuff[i].m_pPressObjectBuff[j].TemIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuff[i].m_pPressObjectBuff[j].BufIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 

				str.Empty();
				str.Format("%s", *((SQLiteString *)(m_pChannelCfgTbl->m_pColsName[6])));
				strcpy(m_pKilnBuff[i].m_pPressObjectBuff[j].strName, str.GetBuffer(21));
				
				m_pKilnBuff[i].m_pPressObjectBuff[j].Gain    = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
				m_pKilnBuff[i].m_pPressObjectBuff[j].Zero    = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
			}
		}

		/************************************************************************/
		/* ʪ��ͨ����ʼ��                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=3", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// ����λ���Ⱥ�����
		m_pChannelCfgTbl->Query();		
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuff[i].HumidityNum)
		{
			for (j = 0; j < m_pKilnBuff[i].HumidityNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuff[i].m_pHumiObjectBuff[j].TemIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuff[i].m_pHumiObjectBuff[j].BufIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 

				str.Empty();
				str.Format("%s", *((SQLiteString *)(m_pChannelCfgTbl->m_pColsName[6])));
				strcpy(m_pKilnBuff[i].m_pHumiObjectBuff[j].strName, str.GetBuffer(21));
				
				m_pKilnBuff[i].m_pHumiObjectBuff[j].Gain    = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
				m_pKilnBuff[i].m_pHumiObjectBuff[j].Zero    = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
			}
		}

		/************************************************************************/
		/* �������ͨ����ʼ��                                                   */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=4", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// ����λ���Ⱥ�����
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuff[i].MotorNum)
		{
			for (j = 0; j < m_pKilnBuff[i].MotorNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuff[i].m_pMotorObjectBuff[j].TemIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuff[i].m_pMotorObjectBuff[j].BufIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 

				str.Empty();
				str.Format("%s", *((SQLiteString *)(m_pChannelCfgTbl->m_pColsName[6])));
				strcpy(m_pKilnBuff[i].m_pMotorObjectBuff[j].strName, str.GetBuffer(21));

				m_pKilnBuff[i].m_pMotorObjectBuff[j].Gain    = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
				m_pKilnBuff[i].m_pMotorObjectBuff[j].Zero    = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
			}
		}

		/************************************************************************/
		/* ��������ͨ����ʼ��                                                   */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=5", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("");
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuff[i].GrossNum)
		{
			for (j = 0; j < m_pKilnBuff[i].GrossNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuff[i].m_pGrossObjectBuff[j].TemTrigIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuff[i].m_pGrossObjectBuff[j].TrigIdx     = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
			}
		}

		/************************************************************************/
		/* �������ͨ����ʼ��                                                   */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=6", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("");
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuff[i].GrossNum)
		{
			for (j = 0; j < m_pKilnBuff[i].GrossNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuff[i].m_pGrossObjectBuff[j].TemBellIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuff[i].m_pGrossObjectBuff[j].BellIdx     = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
			}
		}

		/************************************************************************/
		/* ������ͨ����ʼ��                                                   */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=7", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("");
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuff[i].MotorNum)
		{
			for (j = 0; j < m_pKilnBuff[i].MotorNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move ������1Ϊ��һ����, ������0

				m_pKilnBuff[i].m_pMotorObjectBuff[j].TemOutIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuff[i].m_pMotorObjectBuff[j].BufOutIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
			}
		}
	} /* for (i = 0; i < m_iKilnNum; i++) */

	/************************************************************************/
	/* ������׼�¶ȱ�                                                       */
	/************************************************************************/
	m_pStandardTbl = new CHisTbl *[m_iKilnNum];
	for (i = 0; i < m_iKilnNum; i++)
	{
		m_pStandardTbl[i] = new CHisTbl(m_pKilnBuff[i].TemperatureNum + 2);

		str.Empty();
		str.Format("StandardCfgInfo_%d", i + 1);
		m_pStandardTbl[i]->SetTableName(str);
		
		// ָ�����ݿ�ı���ֶ���
		col_index = 0;
		m_pStandardTbl[i]->SetItem(col_index++, "id",       MYDB_TYPE_INT);
		m_pStandardTbl[i]->SetItem(col_index++, "set_time", MYDB_TYPE_TEXT);

		for (j = 0; j < m_pKilnBuff[i].TemperatureNum; j++)
		{
			str.Empty();
			str.Format("temperature_%d", j + 1);
			m_pStandardTbl[i]->SetItem(col_index++, str, MYDB_TYPE_INT);
		}
		
		if (!m_pStandardTbl[i]->Open(str_utf8))
		{
			return m_bDBOpen;
		}
		
		if (m_pStandardTbl[i]->GetCount() <= 0)
		{
			return m_bDBOpen;
		}

		m_pStandardTbl[i]->Move(m_pKilnBuff[i].StandardIndex);

		for (j = 0; j < m_pKilnBuff[i].TemperatureNum; j++)
		{
			m_pKilnBuff[i].m_pTempObjectBuff[j].SValue = *((int *)(m_pStandardTbl[i]->m_pColsName[2 + j]));
		}
	} /* for (i = 0; i < m_iKilnNum; i++) */

	/************************************************************************/
	/* ������ʷ���ݱ�ĸ���, ÿ��Ҥֻ��1����ʷ���ݱ�                        */
	/************************************************************************/
	m_pHisTbl = new CHisTbl *[m_iKilnNum];	
	for (i = 0; i < m_iKilnNum; i++)
	{
		InitHisDBTable(i);
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// �Զ����Ʋ�����
	m_pAutoCtrlTbl = new CHisTbl(9);						// ����Ҥ�Ƿ��Կ�, ÿһ��Ҥ�������Կؼ�¼

	m_pAutoCtrlTbl->SetTableName("AutoCtrlInfo");

	// ָ�����ݿ�ı���ֶ���
	col_index = 0;
	m_pAutoCtrlTbl->SetItem(col_index++, "id",             MYDB_TYPE_INT);
	m_pAutoCtrlTbl->SetItem(col_index++, "out_up",         MYDB_TYPE_DOUBLE);
	m_pAutoCtrlTbl->SetItem(col_index++, "out_dn",         MYDB_TYPE_DOUBLE);
	m_pAutoCtrlTbl->SetItem(col_index++, "front_pts",      MYDB_TYPE_INT);
	m_pAutoCtrlTbl->SetItem(col_index++, "rear_pts",       MYDB_TYPE_INT);
	m_pAutoCtrlTbl->SetItem(col_index++, "ctrl_time",      MYDB_TYPE_INT);
	m_pAutoCtrlTbl->SetItem(col_index++, "inc_coeff",      MYDB_TYPE_DOUBLE);
	m_pAutoCtrlTbl->SetItem(col_index++, "gross_waittime", MYDB_TYPE_INT);
	m_pAutoCtrlTbl->SetItem(col_index++, "gross_proctime", MYDB_TYPE_INT);
	
	if (!m_pAutoCtrlTbl->Open(str_utf8))
	{
		return m_bDBOpen;
	}
	
	if (m_pAutoCtrlTbl->GetCount() <= 0)
	{
		return m_bDBOpen;
	}

	m_pAutoCtrlTbl->SetFilter("");
	m_pAutoCtrlTbl->SetSort("[id] ASC");
	m_pAutoCtrlTbl->Query();
	m_pAutoCtrlTbl->SetSort("");

	for (i = 0; i < m_iKilnNum; i++)
	{
		m_pAutoCtrlTbl->Move(i + 1);

		if (m_pKilnBuff[i].ControlNum > 0)
		{
			m_pKilnBuff[i].m_pCtrlObjectBuff[0].UpLimitOut     = *((double *)(m_pAutoCtrlTbl->m_pColsName[1]));
			m_pKilnBuff[i].m_pCtrlObjectBuff[0].DnLimitOut     = *((double *)(m_pAutoCtrlTbl->m_pColsName[2]));
			m_pKilnBuff[i].m_pCtrlObjectBuff[0].iFrontPoint    = *((int *)(m_pAutoCtrlTbl->m_pColsName[3]));
			m_pKilnBuff[i].m_pCtrlObjectBuff[0].iRearPoint     = *((int *)(m_pAutoCtrlTbl->m_pColsName[4]));
			m_pKilnBuff[i].m_pCtrlObjectBuff[0].iCtrlTime      = *((int *)(m_pAutoCtrlTbl->m_pColsName[5]));
			m_pKilnBuff[i].m_pCtrlObjectBuff[0].IncOutCoeff    = *((double *)(m_pAutoCtrlTbl->m_pColsName[6]));
			m_pKilnBuff[i].m_pCtrlObjectBuff[0].iGrossWaitTime = *((int *)(m_pAutoCtrlTbl->m_pColsName[7]));

			m_pKilnBuff[i].m_pCtrlObjectBuff[0].iGrossProcTime = *((int *)(m_pAutoCtrlTbl->m_pColsName[8]));
			// ��ʼ��, ��ֹ��������
			m_pKilnBuff[i].m_pCtrlObjectBuff[0].iProcTimerCnt  = m_pKilnBuff[i].m_pCtrlObjectBuff[0].iGrossProcTime * 60;
		}
	}

	/************************************************************************/
	/* ������¼���ʼ��                                                     */
	/************************************************************************/
	m_pGrossTbl = new CHisTbl(4);
	m_pGrossTbl->SetTableName("OutputsInfo");
	
	// ָ�����ݿ�ı���ֶ���
	col_index = 0;
	m_pGrossTbl->SetItem(col_index++, "kiln_idx",    MYDB_TYPE_INT);
	m_pGrossTbl->SetItem(col_index++, "out_time",    MYDB_TYPE_TEXT);
	m_pGrossTbl->SetItem(col_index++, "condition",   MYDB_TYPE_INT);
	m_pGrossTbl->SetItem(col_index++, "up_serve",    MYDB_TYPE_INT);
	
	if (!m_pGrossTbl->Open(str_utf8))
	{
		return m_bDBOpen;
	}
	
	m_bDBOpen = TRUE;
	return m_bDBOpen;
}

void CIControlDoc::InitHisDBTable(int iFurnaceNum)
{
	CString str;
	
	// ���ݿ����ò���: �ֶθ��� = id + time + ������Ե�
	m_pHisTbl[iFurnaceNum] = new CHisTbl(m_pKilnBuff[iFurnaceNum].TemperatureNum + 
										 m_pKilnBuff[iFurnaceNum].TemperatureANum + 
										 m_pKilnBuff[iFurnaceNum].PressNum + 
										 m_pKilnBuff[iFurnaceNum].HumidityNum + 
										 m_pKilnBuff[iFurnaceNum].MotorNum + 2);		
	
	// ָ�����ݿ����ʷ������
	str.Format("History_%d", iFurnaceNum + 1);
	m_pHisTbl[iFurnaceNum]->SetTableName(str.GetBuffer(MAX_PATH));
	
	// ָ�����ݿ����ʷ���ݱ���ֶ���
	int col_index = 0;
	m_pHisTbl[iFurnaceNum]->SetItem(col_index++, "id", MYDB_TYPE_INT);
	m_pHisTbl[iFurnaceNum]->SetItem(col_index++, "save_time", MYDB_TYPE_TEXT);
	
	// ָ�����ݿ����ʷ���ݱ���¶��ֶ���
	for (int i = 0; i < m_pKilnBuff[iFurnaceNum].TemperatureNum; i++)
	{
		str.Empty();
		str.Format("temperature_%d", i + 1);
		m_pHisTbl[iFurnaceNum]->SetItem(col_index++, str, MYDB_TYPE_INT);
	}

	// ָ�����ݿ����ʷ���ݱ�ı��¶��ֶ���
	for (i = 0; i < m_pKilnBuff[iFurnaceNum].TemperatureANum; i++)
	{
		str.Empty();
		str.Format("temperaturea_%d", i + 1);
		m_pHisTbl[iFurnaceNum]->SetItem(col_index++, str, MYDB_TYPE_INT);
	}
	
	// ָ�����ݿ����ʷ���ݱ��ѹ���ֶ���
	for (i = 0; i < m_pKilnBuff[iFurnaceNum].PressNum; i++)
	{
		str.Empty();
		str.Format("press_%d", i + 1);
		m_pHisTbl[iFurnaceNum]->SetItem(col_index++, str, MYDB_TYPE_INT);
	}
	
	// ָ�����ݿ����ʷ���ݱ��ʪ���ֶ���
	for (i = 0; i < m_pKilnBuff[iFurnaceNum].HumidityNum; i++)
	{
		str.Empty();
		str.Format("humidity_%d", i + 1);
		m_pHisTbl[iFurnaceNum]->SetItem(col_index++, str, MYDB_TYPE_DOUBLE);
	}
	
	// ָ�����ݿ����ʷ���ݱ�ĵ���ֶ���
	for (i = 0; i < m_pKilnBuff[iFurnaceNum].MotorNum; i++)
	{
		str.Empty();
		str.Format("motor_%d", i + 1);
		m_pHisTbl[iFurnaceNum]->SetItem(col_index++, str, MYDB_TYPE_DOUBLE);
	}
	
	// �����ݿ�, ÿ��1�����ݿⶼ����1�����, ���Բ���ͬһ�����ݿ�
	// �����ݿ��ͬʱ, �Ѿ�ִ����һ�γɹ��Ĳ�ѯ����
	m_pHisTbl[iFurnaceNum]->Open(MbcsToUtf8(m_strDBPath.GetBuffer(MAX_PATH)));
}

BOOL CIControlDoc::UpdateRealTimeValue(void)
{
	assert(pRegBuff != NULL);

	int i, j;
	int tem_idx, buf_idx;
	WORD tmp;
	short tmp_int;

	int no_white_module = 0;
	BOOL res = TRUE;

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	/************************************************************************/
	/* ����������: ���                                                     */
	/************************************************************************/
	for (i = 0; i < m_iKilnNum; i++)
	{
		for (j = 0; j < m_pKilnBuff[i].MotorNum; j++)
		{
			tem_idx = m_pKilnBuff[i].m_pMotorObjectBuff[j].TemIdx;
			buf_idx = m_pKilnBuff[i].m_pMotorObjectBuff[j].BufIdx;
			tmp     = pMainFrame->m_pCom->m_pTemObjectBuff[tem_idx].pTemRegBuff[buf_idx];
			
			m_pKilnBuff[i].m_pMotorObjectBuff[j].Value = ((float)(tmp) 
														* m_pKilnBuff[i].m_pMotorObjectBuff[j].Gain) 
														+ m_pKilnBuff[i].m_pMotorObjectBuff[j].Zero;
			
			tem_idx = m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].TemIdx;
			buf_idx = m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].BufIdx;
			tmp     = pMainFrame->m_pCom->m_pTemObjectBuff[tem_idx].pTemRegBuff[buf_idx];
			
			m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].Value = ((float)(tmp) 
														* m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].Gain) 
														+ m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].Zero;

			if ((m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].Value > 500) || (m_pKilnBuff[i].m_pMotorObjectBuff[j].Value > 500))
			{
				no_white_module++;
			}
		}
	}

	if (no_white_module > 0)
	{
		res = FALSE;
	}

	/************************************************************************/
	/* ����������: �¶�                                                     */
	/************************************************************************/
	for (i = 0; i < m_iKilnNum; i++)
	{
		for (j = 0; j < m_pKilnBuff[i].TemperatureNum; j++)
		{
			tem_idx = m_pKilnBuff[i].m_pTempObjectBuff[j].TemIdx;
			buf_idx = m_pKilnBuff[i].m_pTempObjectBuff[j].BufIdx;
			tmp     = pMainFrame->m_pCom->m_pTemObjectBuff[tem_idx].pTemRegBuff[buf_idx];
			tmp_int = (short)(tmp);

			m_pKilnBuff[i].m_pTempObjectBuff[j].Value = ((float)(tmp_int) 
													  * m_pKilnBuff[i].m_pTempObjectBuff[j].Gain) 
													  + m_pKilnBuff[i].m_pTempObjectBuff[j].Zero;

			tem_idx = m_pKilnBuffForCurve[i].m_pTempObjectBuff[j].TemIdx;
			buf_idx = m_pKilnBuffForCurve[i].m_pTempObjectBuff[j].BufIdx;
			tmp     = pMainFrame->m_pCom->m_pTemObjectBuff[tem_idx].pTemRegBuff[buf_idx];
			tmp_int = (short)(tmp);

			m_pKilnBuffForCurve[i].m_pTempObjectBuff[j].Value = ((float)(tmp_int) 
														* m_pKilnBuffForCurve[i].m_pTempObjectBuff[j].Gain) 
														+ m_pKilnBuffForCurve[i].m_pTempObjectBuff[j].Zero;
		}
	}

	/************************************************************************/
	/* ����������: ���¶�                                                     */
	/************************************************************************/
	for (i = 0; i < m_iKilnNum; i++)
	{
		for (j = 0; j < m_pKilnBuff[i].TemperatureANum; j++)
		{
			tem_idx = m_pKilnBuff[i].m_pTempAObjectBuff[j].TemIdx;
			buf_idx = m_pKilnBuff[i].m_pTempAObjectBuff[j].BufIdx;
			tmp     = pMainFrame->m_pCom->m_pTemObjectBuff[tem_idx].pTemRegBuff[buf_idx];
			tmp_int = (short)(tmp);
			
			m_pKilnBuff[i].m_pTempAObjectBuff[j].Value = ((float)(tmp_int) 
													  * m_pKilnBuff[i].m_pTempAObjectBuff[j].Gain) 
													  + m_pKilnBuff[i].m_pTempAObjectBuff[j].Zero;


			tem_idx = m_pKilnBuffForCurve[i].m_pTempAObjectBuff[j].TemIdx;
			buf_idx = m_pKilnBuffForCurve[i].m_pTempAObjectBuff[j].BufIdx;
			tmp     = pMainFrame->m_pCom->m_pTemObjectBuff[tem_idx].pTemRegBuff[buf_idx];
			tmp_int = (short)(tmp);
			
			m_pKilnBuffForCurve[i].m_pTempAObjectBuff[j].Value = ((float)(tmp_int) 
													* m_pKilnBuffForCurve[i].m_pTempAObjectBuff[j].Gain) 
													+ m_pKilnBuffForCurve[i].m_pTempAObjectBuff[j].Zero;
		}
	}

	/************************************************************************/
	/* ����������: ѹ��                                                     */
	/************************************************************************/
	for (i = 0; i < m_iKilnNum; i++)
	{
		for (j = 0; j < m_pKilnBuff[i].PressNum; j++)
		{
			tem_idx = m_pKilnBuff[i].m_pPressObjectBuff[j].TemIdx;
			buf_idx = m_pKilnBuff[i].m_pPressObjectBuff[j].BufIdx;			
			tmp     = pMainFrame->m_pCom->m_pTemObjectBuff[tem_idx].pTemRegBuff[buf_idx] * 10;
			if (tmp & 0x8000)
			{
				tmp_int = 0 - (short)(tmp - 0x8000);
			}
			else
			{
				tmp_int = (short)tmp;
			}

			m_pKilnBuff[i].m_pPressObjectBuff[j].Value = ((float)(tmp_int) 
														* m_pKilnBuff[i].m_pPressObjectBuff[j].Gain) 
														+ m_pKilnBuff[i].m_pPressObjectBuff[j].Zero;

			tem_idx = m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].TemIdx;
			buf_idx = m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].BufIdx;
			tmp     = pMainFrame->m_pCom->m_pTemObjectBuff[tem_idx].pTemRegBuff[buf_idx] * 10;
			if (tmp & 0x8000)
			{
				tmp_int = 0 - (short)(tmp - 0x8000);
			}
			else
			{
				tmp_int = (short)tmp;
			}
			
			m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].Value = ((float)(tmp_int) 
														* m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].Gain) 
														+ m_pKilnBuffForCurve[i].m_pPressObjectBuff[j].Zero;
		}
	}

	/************************************************************************/
	/* ����������: ʪ��                                                     */
	/************************************************************************/
	for (i = 0; i < m_iKilnNum; i++)
	{
		for (j = 0; j < m_pKilnBuff[i].HumidityNum; j++)
		{
			tem_idx = m_pKilnBuff[i].m_pHumiObjectBuff[j].TemIdx;
			buf_idx = m_pKilnBuff[i].m_pHumiObjectBuff[j].BufIdx;
			tmp     = pMainFrame->m_pCom->m_pTemObjectBuff[tem_idx].pTemRegBuff[buf_idx] * 10;
			if (tmp & 0x8000)
			{
				tmp_int = 0 - (short)(tmp - 0x8000);
			}
			else
			{
				tmp_int = (short)tmp;
			}

			m_pKilnBuff[i].m_pHumiObjectBuff[j].Value = ((float)(tmp_int) 
														* m_pKilnBuff[i].m_pHumiObjectBuff[j].Gain) 
														+ m_pKilnBuff[i].m_pHumiObjectBuff[j].Zero;

			tem_idx = m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].TemIdx;
			buf_idx = m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].BufIdx;
			tmp     = pMainFrame->m_pCom->m_pTemObjectBuff[tem_idx].pTemRegBuff[buf_idx] * 10;
			if (tmp & 0x8000)
			{
				tmp_int = (short)(tmp - 0x8000);
			}
			else
			{
				tmp_int = (short)tmp;
			}
			
			m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].Value = ((float)(tmp_int) 
														* m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].Gain) 
														+ m_pKilnBuffForCurve[i].m_pHumiObjectBuff[j].Zero;
		}
	}

	/************************************************************************/
	/* ����������: ����                                                     */
	/************************************************************************/
	for (i = 0; i < m_iKilnNum; i++)
	{
		for (j = 0; j < m_pKilnBuff[i].GrossNum; j++)
		{
			tem_idx = m_pKilnBuff[i].m_pGrossObjectBuff[j].TemTrigIdx;
			buf_idx = m_pKilnBuff[i].m_pGrossObjectBuff[j].TrigIdx;
			tmp     = pMainFrame->m_pCom->m_pTemObjectBuff[tem_idx].pTemRegBuff[buf_idx];

			if ((tmp > 0) && (tmp < 3277))
			{
				if (AppendGrossRecord(i) == TRUE)
				{
// 					// ������������
// 					tem_idx = m_pKilnBuff[i].m_pGrossObjectBuff[j].TemBellIdx;
// 					buf_idx = m_pKilnBuff[i].m_pGrossObjectBuff[j].BellIdx;
// 					tmp     = m_pKilnBuff[i].m_pGrossObjectBuff[j].BellValidTime;
// 					pMainFrame->m_pCom->SetWriteTerminalInfo(tem_idx, (buf_idx + 40312), 1, &tmp);

					// ���Զ����ƹ�������һ��ʱ��
					if ((m_pKilnBuff[i].ControlNum > 0) 
						&& (m_pKilnBuff[i].m_pCtrlObjectBuff[0].bControl == TRUE))
					{
						m_pKilnBuff[i].m_pCtrlObjectBuff[0].iWaitTimerCnt = 60 * m_pKilnBuff[i].m_pCtrlObjectBuff[0].iGrossWaitTime;
					}

					// �����Ƴ���ʱ��
					m_pKilnBuff[i].m_pCtrlObjectBuff[0].iProcTimerCnt = 60 * m_pKilnBuff[i].m_pCtrlObjectBuff[0].iGrossProcTime;
				}	
			}
		}
	}

	return res;
}

void CIControlDoc::DevPowerOn(void)
{
	m_pLoginTbl->Query();					// ִ��һ�β�ѯ����
	m_pLoginTbl->MoveFirst();
	m_pLoginTbl->Edit();					// ִ�б༭����
	m_iDevPower = 1;
	*((int *)(m_pLoginTbl->m_pColsName[3])) = m_iDevPower;
	m_pLoginTbl->Update();
	m_pLoginTbl->Query();	
}

void CIControlDoc::DevPowerOff(void)
{
	m_pLoginTbl->Query();					// ִ��һ�β�ѯ����
	m_pLoginTbl->MoveFirst();
	m_pLoginTbl->Edit();					// ִ�б༭����
	m_iDevPower = 0;
	*((int *)(m_pLoginTbl->m_pColsName[3])) = m_iDevPower;
	m_pLoginTbl->Update();
	m_pLoginTbl->Query();
}

BOOL CIControlDoc::SetComCfgTbl(void)
{
	CString str;

	m_pComCfgTbl->Query();							// ִ��һ�β�ѯ����
	
	if (m_pComCfgTbl->MoveFirst() == true)			// �ƶ�����һ������					
	{	
		if (m_pComCfgTbl->GetCount() >= 1)
		{
			m_pComCfgTbl->Edit();					// ִ�б༭����

			*((int *)(m_pComCfgTbl->m_pColsName[0])) = m_ComCfg.iPortNum;
			*((int *)(m_pComCfgTbl->m_pColsName[1])) = m_ComCfg.iBaudrate;

			str.Format("%c", m_ComCfg.cParity);
			*((SQLiteString *)(m_pComCfgTbl->m_pColsName[2])) = str;

			*((int *)(m_pComCfgTbl->m_pColsName[3]))    = m_ComCfg.iDataBits;
			*((int *)(m_pComCfgTbl->m_pColsName[4]))    = m_ComCfg.iStopBits;
			*((int *)(m_pComCfgTbl->m_pColsName[5]))    = m_ComCfg.iTimeout;
			*((int *)(m_pComCfgTbl->m_pColsName[6]))    = m_ComCfg.iScanTime;
			*((DWORD *)(m_pComCfgTbl->m_pColsName[7]))  = m_ComCfg.iDtuBaudrate;
			*((WORD *)(m_pComCfgTbl->m_pColsName[8]))   = m_ComCfg.lServePort;
			*((DWORD *)(m_pComCfgTbl->m_pColsName[9]))  = m_ComCfg.iDtuPortNum;
			*((WORD *)(m_pComCfgTbl->m_pColsName[10]))  = m_ComCfg.lDtuPort;

			for (int i = 0; i < m_iKilnNum; i++)
			{
				for (int j = 0; j < m_pKilnBuff[i].GrossNum; j++)
				{
					*((int *)(m_pComCfgTbl->m_pColsName[11])) = m_pKilnBuff[i].m_pGrossObjectBuff[j].TrigInternal;
					*((int *)(m_pComCfgTbl->m_pColsName[12])) = m_pKilnBuff[i].m_pGrossObjectBuff[j].BellValidTime;
				}
			}
			
			m_pComCfgTbl->Update();
			m_pComCfgTbl->Query();			
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		
	}
	else
	{
		return FALSE;
	}
}

// type=0 �¶�
//      1 ����
//      2 ѹ��
//      3 ʪ��
//      4 ���
BOOL CIControlDoc::SetAdjustValue(int iFurnaceNum, int type)
{
	int     i;
	CString str;

	str.Format("[kiln_idx]=%d and [chn_type]=%d", iFurnaceNum + 1, type);
	m_pChannelCfgTbl->SetFilter(str);
	m_pChannelCfgTbl->Query();								// ִ��һ�β�ѯ����, �ҵ��趨�����߱��

	switch (type)
	{
	case 0:
		for (i = 0; i < m_pKilnBuff[iFurnaceNum].TemperatureNum; i++)
		{
			if (m_pChannelCfgTbl->Move(i + 1) == false)		// Move ������1Ϊ��һ����, ������0
			{
				return FALSE;
			}
			
			m_pKilnBuff[iFurnaceNum].m_pTempObjectBuff[i].Gain = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
			m_pKilnBuff[iFurnaceNum].m_pTempObjectBuff[i].Zero = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));

			m_pKilnBuffForCurve[iFurnaceNum].m_pTempObjectBuff[i].Gain = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
			m_pKilnBuffForCurve[iFurnaceNum].m_pTempObjectBuff[i].Zero = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
		}
		break;
	case 1:
		for (i = 0; i < m_pKilnBuff[iFurnaceNum].TemperatureANum; i++)
		{
			if (m_pChannelCfgTbl->Move(i + 1) == false)		// Move ������1Ϊ��һ����, ������0
			{
				return FALSE;
			}
			
			m_pKilnBuff[iFurnaceNum].m_pTempAObjectBuff[i].Gain = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
			m_pKilnBuff[iFurnaceNum].m_pTempAObjectBuff[i].Zero = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));

			m_pKilnBuffForCurve[iFurnaceNum].m_pTempAObjectBuff[i].Gain = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
			m_pKilnBuffForCurve[iFurnaceNum].m_pTempAObjectBuff[i].Zero = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
		}
		break;
	case 2:
		for (i = 0; i < m_pKilnBuff[iFurnaceNum].PressNum; i++)
		{
			if (m_pChannelCfgTbl->Move(i + 1) == false)				// Move ������1Ϊ��һ����, ������0
			{
				return FALSE;
			}
			
			m_pKilnBuff[iFurnaceNum].m_pPressObjectBuff[i].Gain = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
			m_pKilnBuff[iFurnaceNum].m_pPressObjectBuff[i].Zero = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));

			m_pKilnBuffForCurve[iFurnaceNum].m_pPressObjectBuff[i].Gain = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
			m_pKilnBuffForCurve[iFurnaceNum].m_pPressObjectBuff[i].Zero = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
		}
		break;
	case 3:
		for (i = 0; i < m_pKilnBuff[iFurnaceNum].HumidityNum; i++)
		{
			if (m_pChannelCfgTbl->Move(i + 1) == false)				// Move ������1Ϊ��һ����, ������0
			{
				return FALSE;
			}
			
			m_pKilnBuff[iFurnaceNum].m_pHumiObjectBuff[i].Gain = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
			m_pKilnBuff[iFurnaceNum].m_pHumiObjectBuff[i].Zero = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));

			m_pKilnBuffForCurve[iFurnaceNum].m_pHumiObjectBuff[i].Gain = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
			m_pKilnBuffForCurve[iFurnaceNum].m_pHumiObjectBuff[i].Zero = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
		}		
		break;
	case 4:
		for (i = 0; i < m_pKilnBuff[iFurnaceNum].MotorNum; i++)
		{
			if (m_pChannelCfgTbl->Move(i + 1) == false)				// Move ������1Ϊ��һ����, ������0
			{
				return FALSE;
			}
			
			m_pKilnBuff[iFurnaceNum].m_pMotorObjectBuff[i].Gain = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
			m_pKilnBuff[iFurnaceNum].m_pMotorObjectBuff[i].Zero = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));

			m_pKilnBuffForCurve[iFurnaceNum].m_pMotorObjectBuff[i].Gain = *((double *)(m_pChannelCfgTbl->m_pColsName[7]));
			m_pKilnBuffForCurve[iFurnaceNum].m_pMotorObjectBuff[i].Zero = *((double *)(m_pChannelCfgTbl->m_pColsName[8]));
		}
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL CIControlDoc::SetStandardValue(int iFurnaceNum, int iStandardIndex)
{
	int     i;
	CString str;

	str.Format("[id]=%d", iStandardIndex);					// ��ѯָ����׼�¶Ⱥż�¼
	m_pStandardTbl[iFurnaceNum]->SetFilter(str);
	m_pStandardTbl[iFurnaceNum]->Query();					// ִ��һ�β�ѯ����
	m_pStandardTbl[iFurnaceNum]->SetFilter("");				// ��ղ�ѯ����
	if (m_pStandardTbl[iFurnaceNum]->GetCount() > 0)		// �жϱ�׼���ߺ��Ƿ�Ϸ�		
	{
		// �ƶ���ָ��Ҥ�ż�¼, ���±�׼�¶ȺŲ��洢
		m_pKilnInfoTbl->SetFilter("");
		m_pKilnInfoTbl->Query();
		if (m_pKilnInfoTbl->Move(iFurnaceNum + 1) == true)
		{
			m_pKilnBuff[iFurnaceNum].StandardIndex = iStandardIndex;	// ���±�׼�¶Ⱥ�

			// �������ݿ����
			m_pKilnInfoTbl->Edit();	
			*((int *)(m_pKilnInfoTbl->m_pColsName[10])) = m_pKilnBuff[iFurnaceNum].StandardIndex;
			m_pKilnInfoTbl->Update();
			m_pKilnInfoTbl->Query();

			// ���µı�׼�¶����ݸ��Ƶ���������
			for (i = 0; i < m_pKilnBuff[iFurnaceNum].TemperatureNum; i++)
			{
				m_pKilnBuff[iFurnaceNum].m_pTempObjectBuff[i].SValue = *((int *)(m_pStandardTbl[iFurnaceNum]->m_pColsName[i + 2]));
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CIControlDoc::DelStandardValue(int iFurnaceNum, int iStandardIndex)
{
	CString str;

	m_pKilnInfoTbl->SetFilter("");
	m_pKilnInfoTbl->Query();
	if (m_pKilnInfoTbl->Move(iFurnaceNum + 1) == true)			
	{
		// ���ɾ���ı�׼�����ǵ�ǰʹ�õı�׼����, ֱ�ӷ��ش���
		if (*((int *)(m_pKilnInfoTbl->m_pColsName[10])) == iStandardIndex)
		{
			return FALSE;
		}

		str.Empty();
		str.Format("[id]=%d", iStandardIndex);					// ��ѯָ����׼�¶Ⱥż�¼
		m_pStandardTbl[iFurnaceNum]->SetFilter(str);
		m_pStandardTbl[iFurnaceNum]->Query();					// ִ��һ�β�ѯ����
		if (m_pStandardTbl[iFurnaceNum]->GetCount() > 0)		// �жϱ�׼���ߺ��Ƿ�Ϸ�
		{
			m_pStandardTbl[iFurnaceNum]->MoveFirst();
			m_pStandardTbl[iFurnaceNum]->Delete();				// ִ��ɾ������
			m_pStandardTbl[iFurnaceNum]->Update();			
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		
	}
	else
	{
		return FALSE;
	}
}

BOOL CIControlDoc::AppendStandardRecord(int iFurnaceNum, int *pTemperatureBuff, BOOL bSetStandard)
{
	int insert_index;
	
	m_pKilnInfoTbl->SetFilter("");
	m_pKilnInfoTbl->Query();
	if (m_pKilnInfoTbl->Move(iFurnaceNum + 1) == true)				
	{
		m_pStandardTbl[iFurnaceNum]->SetSort("[id] ASC");		// ��id�ŵ���������
		m_pStandardTbl[iFurnaceNum]->Query();
		m_pStandardTbl[iFurnaceNum]->MoveLast();
		insert_index = *((int *)(m_pStandardTbl[iFurnaceNum]->m_pColsName[0])) + 1;	// ������¼�ı�������м�¼id��֮��
		
		m_pStandardTbl[iFurnaceNum]->AddNew();					// ����¼�¼		
		*((int *)(m_pStandardTbl[iFurnaceNum]->m_pColsName[0])) = insert_index;		// ���߱��
		
		CTime t = CTime::GetCurrentTime();	// ��ȡϵͳ����
		CString str;
		str.Format("%04d-%02d-%02d %02d:%02d:%02d", t.GetYear(), 
													t.GetMonth(), 
													t.GetDay(), 
													t.GetHour(), 
													t.GetMinute(), 
													t.GetSecond());
		*((SQLiteString *)(m_pStandardTbl[iFurnaceNum]->m_pColsName[1])) = str;
		
		for (int i = 0; i < m_pKilnBuff[iFurnaceNum].TemperatureNum; i++)
		{
			if (pTemperatureBuff[i] > 1370 || pTemperatureBuff[i] < (-100))
			{
				*((int *)(m_pStandardTbl[iFurnaceNum]->m_pColsName[i + 2])) = 0;
			}
			else
			{
				*((int *)(m_pStandardTbl[iFurnaceNum]->m_pColsName[i + 2])) = pTemperatureBuff[i];
			}			
		}
		
		m_pStandardTbl[iFurnaceNum]->Update();		
		
		if (bSetStandard == TRUE)
		{
			SetStandardValue(iFurnaceNum, insert_index);			// Ӧ���¼�¼
		}
		
		return TRUE;		
	}
	else
	{
		return FALSE;
	}
}

int CIControlDoc::GetHistoryRecordCnt(void)
{
	m_pHisTbl[0]->SetFilter("");
	m_pHisTbl[0]->Query();
	return (m_pHisTbl[0]->GetCount());
}

/* �����ݿ������ʷ��¼��Ϣ */
BOOL CIControlDoc::AppendHistoryRecord(void)
{
	int i, j;
	int insert_index;

	CTime t = CTime::GetCurrentTime();	// ��ȡϵͳ����
	CString str;
	str.Format("%04d-%02d-%02d %02d:%02d:%02d", t.GetYear(), 
												t.GetMonth(), 
												t.GetDay(), 
												t.GetHour(), 
												t.GetMinute(), 
												t.GetSecond());

	for (i = 0; i < m_iKilnNum; i++)
	{
		m_pHisTbl[i]->SetSort("[id] ASC");		// ����ֵID�Ű���������
		m_pHisTbl[i]->Query();
		m_pHisTbl[i]->MoveLast();

		// �����ݻ�ID��Ϊ�Ƿ�ֵ����ʼ��Ϊ1
		if ((m_pHisTbl[i]->GetCount() <= 0) || (*((int *)(m_pHisTbl[i]->m_pColsName[0])) <= 0))
		{
			insert_index = 1;												
		}
		else
		{
			insert_index = *((int *)(m_pHisTbl[i]->m_pColsName[0])) + 1;	// ������¼�ı�������м�¼id��֮��
		}		

		// ����¼�¼
		m_pHisTbl[i]->AddNew();
		*((int *)(m_pHisTbl[i]->m_pColsName[0])) = insert_index;		// ���߱��
		*((SQLiteString *)(m_pHisTbl[i]->m_pColsName[1])) = str;		// �洢ʱ��
		
		insert_index = 2;
		for (j = 0; j < m_pKilnBuff[i].TemperatureNum; j++)
		{
			if ((m_pKilnBuff[i].m_pTempObjectBuff[j].Value >= (-100)) 
				&& (m_pKilnBuff[i].m_pTempObjectBuff[j].Value <= 1370))
			{
				*((int *)(m_pHisTbl[i]->m_pColsName[insert_index++])) = m_pKilnBuff[i].m_pTempObjectBuff[j].Value;
			}
			else
			{
				*((int *)(m_pHisTbl[i]->m_pColsName[insert_index++])) = 10000;
			}
		}

		for (j = 0; j < m_pKilnBuff[i].TemperatureANum; j++)
		{
			if ((m_pKilnBuff[i].m_pTempAObjectBuff[j].Value >= (-100)) 
				&& (m_pKilnBuff[i].m_pTempAObjectBuff[j].Value <= 1370))
			{
				*((int *)(m_pHisTbl[i]->m_pColsName[insert_index++])) = m_pKilnBuff[i].m_pTempAObjectBuff[j].Value;
			}
			else
			{
				*((int *)(m_pHisTbl[i]->m_pColsName[insert_index++])) = 10000;
			}
		}

		for (j = 0; j < m_pKilnBuff[i].PressNum; j++)
		{
			if ((m_pKilnBuff[i].m_pPressObjectBuff[j].Value >= (-500)) 
				&& (m_pKilnBuff[i].m_pPressObjectBuff[j].Value <= 500))
			{
				*((int *)(m_pHisTbl[i]->m_pColsName[insert_index++])) = m_pKilnBuff[i].m_pPressObjectBuff[j].Value;
			}
			else
			{
				*((int *)(m_pHisTbl[i]->m_pColsName[insert_index++])) = 10000;
			}
		}

		for (j = 0; j < m_pKilnBuff[i].HumidityNum; j++)
		{
			if ((m_pKilnBuff[i].m_pHumiObjectBuff[j].Value >= 0.0) 
				&& (m_pKilnBuff[i].m_pHumiObjectBuff[j].Value <= 100.0))
			{
				*((double *)(m_pHisTbl[i]->m_pColsName[insert_index++])) = m_pKilnBuff[i].m_pHumiObjectBuff[j].Value;
			}
			else
			{
				*((double *)(m_pHisTbl[i]->m_pColsName[insert_index++])) = 10000.0;
			}
		}

		for (j = 0; j < m_pKilnBuff[i].MotorNum; j++)
		{
			if ((m_pKilnBuff[i].m_pMotorObjectBuff[j].Value >= 0.0) 
				&& (m_pKilnBuff[i].m_pMotorObjectBuff[j].Value <= 500.0))
			{
				*((double *)(m_pHisTbl[i]->m_pColsName[insert_index++])) = m_pKilnBuff[i].m_pMotorObjectBuff[j].Value / 10.0;
			}
			else
			{
				*((double *)(m_pHisTbl[i]->m_pColsName[insert_index++])) = 10000.0;
			}
		}
		
		m_pHisTbl[i]->Update();		
		m_pHisTbl[i]->Query();
	}

	return TRUE;
}

BOOL CIControlDoc::AppendGrossRecord(int iKilnIdx)
{
	CString t_str;
	CTime last_t;
	CTimeSpan timespan;
	CTime t = CTime::GetCurrentTime();	// ��ȡϵͳ����;
	CString str, str1;
	str.Format("%04d-%02d-%02d %02d:%02d:%02d", t.GetYear(), 
												t.GetMonth(), 
												t.GetDay(), 
												t.GetHour(), 
												t.GetMinute(), 
												t.GetSecond());

	/* ���������, ���Ҷ�ӦשҤ�����1������ */
	str1.Format("[kiln_idx]=%d", iKilnIdx + 1);
	m_pGrossTbl->SetFilter(str1);
	m_pGrossTbl->SetSort("[out_time] DESC");
	m_pGrossTbl->Query();
	m_pGrossTbl->MoveFirst();

	/* ����������м�¼, ��Ҫ��������¼֮���ʱ�� */
	if (m_pGrossTbl->GetCount() > 0)
	{
		t_str  = *((SQLiteString *)(m_pGrossTbl->m_pColsName[1]));		// �õ����һ��������ʱ��
		last_t = StringToDate(t_str);
		if (last_t == NULL)
		{
			return FALSE;
		}

		timespan = t - last_t;
		/* ������¼֮��ļ��ʱ�������n�������� */
		LONG min_span = timespan.GetTotalMinutes();
		if (min_span < m_pKilnBuff[iKilnIdx].m_pGrossObjectBuff[0].TrigInternal)
		{
// 			str.Empty();
// 			str.Format("ÿ���Ƴ������������%d���Ӽ��!", m_ParaCfg.iGrossInternal);
// 
// 			CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
// 			pMainFrame->CreateGrossInfo(str);
			return FALSE;
		}
	}

	/* ��Ӳ�����¼ */
	m_pGrossTbl->AddNew();
	*((int *)(m_pGrossTbl->m_pColsName[0]))          = iKilnIdx + 1;	// Ҥ��
	*((SQLiteString *)(m_pGrossTbl->m_pColsName[1])) = str;				// �洢ʱ��
	*((int *)(m_pGrossTbl->m_pColsName[2]))          = 0;				// ��ʽ
	*((int *)(m_pGrossTbl->m_pColsName[3]))          = 0;				// δ�ϴ�
	m_pGrossTbl->Update();
	m_pGrossTbl->Query();

	str.Empty();
	str.Format("%02d-%02d-%02d %02d:%02d:%02d\r\n�Ƴ������ɹ�!", t.GetYear(), 
																 t.GetMonth(), 
																 t.GetDay(), 
																 t.GetHour(), 
																 t.GetMinute(), 
																 t.GetSecond());
	
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	// ��Զ�˷��������Ͳ�����Ϣ
	pMainFrame->m_pCom->SetGrossTransmit(iKilnIdx, 0, t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	// ���������Ի���
	pMainFrame->CreateGrossInfo(str);
	
	return TRUE;
}

BOOL CIControlDoc::GetGrossNotUpService(void)
{
	CString str;
	int iKilnIdx, GrossType;
	CTime t;

	/* ������: �г�����δ�ϴ��Ĳ������� */
	m_pGrossTbl->SetFilter("[up_serve]=0");
	m_pGrossTbl->SetSort("[out_time] ASC");
	m_pGrossTbl->Query();
	m_pGrossTbl->MoveFirst();

	if (m_pGrossTbl->GetCount() <= 0) // û��δ�ϴ�������
	{
		return FALSE;
	}

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	// ������δ�ϴ��Ĳ�������д�����
	for (int i = 0; i < m_pGrossTbl->GetCount(); i++)
	{
		iKilnIdx  = (*((int *)(m_pGrossTbl->m_pColsName[0]))) - 1;
		GrossType = (*((int *)(m_pGrossTbl->m_pColsName[2])));
		
		str  = *((SQLiteString *)(m_pGrossTbl->m_pColsName[1]));		// �õ���һ��δ�ϴ��Ĳ������ݵ�ʱ��
		t = StringToDate(str);
		if (t == NULL)
		{
			return FALSE;
		}

		pMainFrame->m_pCom->SetGrossTransmit(iKilnIdx, GrossType, 
											 t.GetYear(), t.GetMonth(), t.GetDay(), 
											 t.GetHour(), t.GetMinute(), t.GetSecond());

		m_pGrossTbl->MoveNext();
	}

	return TRUE;
}

BOOL CIControlDoc::SetGrossUpService(BYTE TunnelKilnId, BYTE GrossType, WORD Year, BYTE Month, BYTE Day, BYTE Hour, BYTE Min, BYTE Sec)
{
	CString str, str_t;

	str_t.Format("%02d-%02d-%02d %02d:%02d:%02d", Year, Month, Day, Hour, Min, Sec);
	/* ������: �г�����δ�ϴ��Ĳ������� */
	str.Format("[kiln_idx]=%d and [out_time]=\'%s\' and [condition]=%d and [up_serve]=%d", TunnelKilnId + 1, str_t, GrossType, 0);

	m_pGrossTbl->SetFilter(str);
	m_pGrossTbl->SetSort("[out_time] ASC");
	m_pGrossTbl->Query();
	m_pGrossTbl->MoveFirst();

	if (m_pGrossTbl->GetCount() <= 0) // û��δ�ϴ�������
	{
		return FALSE;
	}

	m_pGrossTbl->Edit();							// ����༭ģʽ
	*((int *)(m_pGrossTbl->m_pColsName[3]))  = 1;	// ���ϴ�
	m_pGrossTbl->Update();

	return TRUE;
}

/************************************************************************/
/* �����ǽ���ǰ�Ƴ�ʱ���¶���֮ǰ�����Ƴ�ʱ���¶���ƽ��, Ȼ������Ϊ��ǰ */
/* ��ǰϵͳ�ı�׼�¶�����.                                              */
/* 1. ����Ҥ���¶ȵ�������������                                        */
/* 2. ����С���ϴ��Ƴ�ʱ����¶���ʷ����                                */
/************************************************************************/
BOOL CIControlDoc::AutoSetStandardRecord(int iKilnIdx)
{
	CString str, str1;
	CTime   t;
	int     i;
	int iSTemperatureNum  = m_pKilnBuff[iKilnIdx].TemperatureNum;	// ��׼�����¶ȸ���
	int *pTemperatureBuff = new int[iSTemperatureNum];				// ����������
	
	str.Format("[kiln_idx]=%d", iKilnIdx + 1);
	m_pGrossTbl->SetFilter(str);
	m_pGrossTbl->SetSort("[out_time] DESC");
	m_pGrossTbl->Query();
	int count = m_pGrossTbl->GetCount();
	if (count <= 0)
	{
		return FALSE;
	}

	int take_times = 0;

	if (count >= 1)							/* ֻ��1����������, ֱ�ӽ���ǰ��������Ϊ��׼���߲�Ӧ�� */
	{
		m_pGrossTbl->Move(1);				/* ��õ�1��������¼��ʱ�� */
		str1 = *((SQLiteString *)(m_pGrossTbl->m_pColsName[1]));
		
		t = StringToDate(str1);
		CTime last_t1(t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute() - 10, t.GetSecond());

		str1.Empty();
		str1.Format("%04d-%02d-%02d %02d:%02d:%02d",	last_t1.GetYear(), 
														last_t1.GetMonth(), 
														last_t1.GetDay(), 
														last_t1.GetHour(), 
														last_t1.GetMinute(), 
														last_t1.GetSecond());
		
		
		/* ����С�ڵڶ���������¼ʱ�����ʷ���� */
		str.Empty();
		str.Format("strftime(\'%%Y-%%m-%%d %%H:%%M:%%S\',save_time)<\'%s\'", str1);
		m_pHisTbl[iKilnIdx]->SetSort("[id] DESC");
		m_pHisTbl[iKilnIdx]->SetFilter(str);
		m_pHisTbl[iKilnIdx]->Query();
		if (m_pHisTbl[iKilnIdx]->MoveFirst() == true)
		{
			take_times++;
			for (i = 0; i < iSTemperatureNum; i++)
			{
				pTemperatureBuff[i] = *((int *)(m_pHisTbl[iKilnIdx]->m_pColsName[i + 2]));
			}
		}
	}

	if (count >= 2)
	{
		/* ��õڶ���������¼��ʱ�� */
		m_pGrossTbl->Move(2);
		str1.Empty();
		str1   = *((SQLiteString *)(m_pGrossTbl->m_pColsName[1]));

		t = StringToDate(str1);
		CTime last_t2(t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute() - 10, t.GetSecond());
		str1.Empty();
		str1.Format("%04d-%02d-%02d %02d:%02d:%02d",	last_t2.GetYear(), 
														last_t2.GetMonth(), 
														last_t2.GetDay(), 
														last_t2.GetHour(), 
														last_t2.GetMinute(), 
														last_t2.GetSecond());
		

		/* ����С�ڵڶ���������¼ʱ�����ʷ���� */
		str.Empty();
		str.Format("strftime(\'%%Y-%%m-%%d %%H:%%M:%%S\',save_time)<\'%s\'", str1);
		m_pHisTbl[iKilnIdx]->SetSort("[id] DESC");
		m_pHisTbl[iKilnIdx]->SetFilter(str);
		m_pHisTbl[iKilnIdx]->Query();
		if (m_pHisTbl[iKilnIdx]->MoveFirst() == true)
		{
			take_times++;
			for (i = 0; i < iSTemperatureNum; i++)
			{
				pTemperatureBuff[i] += *((int *)(m_pHisTbl[iKilnIdx]->m_pColsName[i + 2]));
			}
		}
	}
	
	if (count >= 3)
	{
		m_pGrossTbl->Move(3);							/* ��õ�3��������¼��ʱ�� */
		str1.Empty();
		str1 = *((SQLiteString *)(m_pGrossTbl->m_pColsName[1]));

		t = StringToDate(str1);
		CTime last_t3(t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute() - 10, t.GetSecond());
		str1.Empty();
		str1.Format("%04d-%02d-%02d %02d:%02d:%02d",	last_t3.GetYear(), 
														last_t3.GetMonth(), 
														last_t3.GetDay(), 
														last_t3.GetHour(), 
														last_t3.GetMinute(), 
														last_t3.GetSecond());
		
		/* ����С�ڵڶ���������¼ʱ�����ʷ���� */
		str.Empty();
		str.Format("strftime(\'%%Y-%%m-%%d %%H:%%M:%%S\',save_time)<\'%s\'", str1);
		m_pHisTbl[iKilnIdx]->SetSort("[id] DESC");
		m_pHisTbl[iKilnIdx]->SetFilter(str);
		m_pHisTbl[iKilnIdx]->Query();
		if (m_pHisTbl[iKilnIdx]->MoveFirst() == TRUE)
		{
			take_times++;
			for (i = 0; i < iSTemperatureNum; i++)
			{
				pTemperatureBuff[i] += *((int *)(m_pHisTbl[iKilnIdx]->m_pColsName[i + 2]));
			}
		}
	}

	if (take_times > 0)
	{
		for (i = 0; i < iSTemperatureNum; i++)
		{
			pTemperatureBuff[i] /= take_times;
		}

		AppendStandardRecord(iKilnIdx, pTemperatureBuff, TRUE);

		delete [] pTemperatureBuff;
		return TRUE;
	}

	delete [] pTemperatureBuff;
	return FALSE;
}

BOOL CIControlDoc::SaveAutoCtrlInfoRecord(int iKilnIdx)
{
	if (m_pKilnBuff[iKilnIdx].ControlNum <= 0)
	{
		return FALSE;
	}

	m_pAutoCtrlTbl->SetFilter("");
	m_pAutoCtrlTbl->Query();
	if (m_pAutoCtrlTbl->Move(iKilnIdx + 1) == true)
	{
		m_pAutoCtrlTbl->Edit();
		*((double *)(m_pAutoCtrlTbl->m_pColsName[1])) = m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].UpLimitOut;
		*((double *)(m_pAutoCtrlTbl->m_pColsName[2])) = m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].DnLimitOut;
		*((int *)(m_pAutoCtrlTbl->m_pColsName[3]))    = m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iFrontPoint;
		*((int *)(m_pAutoCtrlTbl->m_pColsName[4]))    = m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iRearPoint;
		*((int *)(m_pAutoCtrlTbl->m_pColsName[5]))    = m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iCtrlTime;
		*((double *)(m_pAutoCtrlTbl->m_pColsName[6])) = m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].IncOutCoeff;
		*((int *)(m_pAutoCtrlTbl->m_pColsName[7]))    = m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iGrossWaitTime;
		*((int *)(m_pAutoCtrlTbl->m_pColsName[8]))    = m_pKilnBuff[iKilnIdx].m_pCtrlObjectBuff[0].iGrossProcTime;

		m_pAutoCtrlTbl->Update();
		return TRUE;
	}

	return FALSE;
}

CTime CIControlDoc::StringToDate(CString strDate)
{
	if (strDate.IsEmpty())
	{
		return NULL;
	}

	if (strDate.GetLength() != 19)
	{
		return NULL;
	}

	strDate.TrimLeft();
	strDate.TrimRight();

	CString date;
	date = strDate.Left(4);
	int year = atoi(date);

	strDate = strDate.Right(14);
	date = strDate.Left(2);
	int mon = atoi(date);

	strDate = strDate.Right(11);
	date = strDate.Left(2);
	int dat = atoi(date);

	strDate = strDate.Right(8);
	date = strDate.Left(2);
	int hour = atoi(date);

	strDate = strDate.Right(5);
	date = strDate.Left(2);
	int min = atoi(date);

	strDate = strDate.Right(2);
	date = strDate.Left(2);
	int sec = atoi(date);

	CTime time(year, mon, dat, hour, min, sec);

	return time;
}
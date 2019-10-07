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

#define THE_CONFIG_TABLE_COL			4			// 焙烧窑条数, 烘干窑条数, 厂名, 厂电话
#define THE_RESCFG_TABLE_COL			6			// 温度个数, 压力个数, 湿度个数, 电机个数, 总车位数, 当前使用的标准曲线
#define THE_PARACFG_TABLE_COL			12			// 参数配置表列数
#define THE_TMCFG_TABLE_COL				5			// 终端配置表列数
#define THE_CARHEAT_TABLE_COL			2			// 车位热量表列数
#define THE_GROSS_TABLE_COL				5			// 产量表列数
#define THE_ADJUST_TABLE_COL			3			// 校正表列数

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

// 支持中文路径转UTF8格式, 符合SQLITE标准
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
		AfxGetMainWnd()->MessageBox("使用默认数据库, 请在程序启动后配置数据库", "数据库操作");
		str = "./cfg/InitialCfg.sql";				// 固定地址
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
	// 获得当前程序目录作为默认目录
	//////////////////////////////////////////////////////////////////////////
	CString strDefaultPath;
	char file_path[MAX_PATH];
	GetModuleFileName(NULL, file_path, MAX_PATH);
	strDefaultPath = file_path;
	int nTemp = strDefaultPath.ReverseFind('\\');
	strDefaultPath = strDefaultPath.Left(nTemp);
	str_ini.Format("%s\\setting.ini", strDefaultPath);

	GetPrivateProfileString("DBDirPath", "数据库命令行存放路径", 
							"sqlite3.exe", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));	

	m_strShellPath.Format("%s\\db\\%s", strDefaultPath, str);

	GetPrivateProfileString("DBDirPath", "数据库存放路径", 
							"RunDB.db", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	// 绝对地址转换成UTF8, 以便支持中文路径
	m_strDBPath.Format("%s\\db\\%s", strDefaultPath, str);
	str_utf8 = MbcsToUtf8(m_strDBPath.GetBuffer(MAX_PATH));

	GetPrivateProfileString("DBDirPath", "配置文件存放路径", 
							"DataBaseCFG.sql", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	m_strCFGPath.Format("%s\\cfg\\%s", strDefaultPath, str);

	GetPrivateProfileString("DBDirPath", "日志文件存放路径", 
							"AutoCtrl.log", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));
	
	m_strLogPath.Format("%s\\cfg\\%s", strDefaultPath, str);

	GetPrivateProfileString("DBDirPath", "备份数据库存放路径", 
							"D:\\iControl\\RunDBBak.sql", 
							m_strDBBakPath.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	GetPrivateProfileString("BKPicDirPath", "背景图片存放路径", 
							"bk.jpg", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	m_strBKPicPath.Format("%s\\pic\\%s", strDefaultPath, str);

	GetPrivateProfileString("RTPicDirPath", "实时图片存放路径", 
							"bk1.jpg", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));
	
	m_strRTPicPath.Format("%s\\pic\\%s", strDefaultPath, str);

	GetPrivateProfileString("RealTimeName", "1号实时温度名称", 
							"1号隧道窑实时温度", 
							m_strRealTimeName[0].GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	GetPrivateProfileString("RealTimeName", "2号实时温度名称", 
							"2号隧道窑实时温度", 
							m_strRealTimeName[1].GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	GetPrivateProfileString("RealTimeName", "3号实时温度名称", 
							"3号隧道窑实时温度", 
							m_strRealTimeName[2].GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	GetPrivateProfileString("RealTimeName", "4号实时温度名称", 
							"4号隧道窑实时温度", 
							m_strRealTimeName[3].GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	m_iHisSaveInternal = GetPrivateProfileInt("HistoryRecord", "历史数据存储间隔", 3000, str_ini.GetBuffer(MAX_PATH));

	// 特殊修改: 将这三个参数，分为两条窑
	m_iGrossProcMaxTime[0] = GetPrivateProfileInt("GrossProcMaxTime", "最大打铃时间1", 150, str_ini.GetBuffer(MAX_PATH));
	m_iGrossProcMaxTime[1] = GetPrivateProfileInt("GrossProcMaxTime", "最大打铃时间2", 150, str_ini.GetBuffer(MAX_PATH));
	m_iGrossProcMaxTime[2] = GetPrivateProfileInt("GrossProcMaxTime", "最大打铃时间3", 150, str_ini.GetBuffer(MAX_PATH));
	m_iGrossProcMaxTime[3] = GetPrivateProfileInt("GrossProcMaxTime", "最大打铃时间4", 150, str_ini.GetBuffer(MAX_PATH));
	
	m_iUpWarn[0] = GetPrivateProfileInt("Warn", "温度报警上限1", 1000, str_ini.GetBuffer(MAX_PATH));
	m_iDnWarn[0] = GetPrivateProfileInt("Warn", "温度报警下限1", 800,  str_ini.GetBuffer(MAX_PATH));
	
	m_iUpWarn[1] = GetPrivateProfileInt("Warn", "温度报警上限2", 1000, str_ini.GetBuffer(MAX_PATH));
	m_iDnWarn[1] = GetPrivateProfileInt("Warn", "温度报警下限2", 800,  str_ini.GetBuffer(MAX_PATH));
	
	m_iUpWarn[2] = GetPrivateProfileInt("Warn", "温度报警上限3", 1000, str_ini.GetBuffer(MAX_PATH));
	m_iDnWarn[2] = GetPrivateProfileInt("Warn", "温度报警下限3", 800,  str_ini.GetBuffer(MAX_PATH));
	
	m_iUpWarn[3] = GetPrivateProfileInt("Warn", "温度报警上限4", 1000, str_ini.GetBuffer(MAX_PATH));
	m_iDnWarn[3] = GetPrivateProfileInt("Warn", "温度报警下限4", 800,  str_ini.GetBuffer(MAX_PATH));
/////////////////////////////////////
	
	GetPrivateProfileString("SaleInfo", "销售电话", 
							"13983089826", 
							str.GetBuffer(MAX_PATH), MAX_PATH, 
							str_ini.GetBuffer(MAX_PATH));

	m_strSalePhone.Format("%s", str);
	
	// 判断文件是否存在, 不存在就创建
	CFileFind find;
	if (!find.FindFile(m_strDBPath))
	{
		// 创建不成功直接退出, 使用UTF8格式路径打开数据库
		if (!ImportDataBase(str_utf8))
		{			 
			AfxGetMainWnd()->MessageBox("初始化数据库不成功", "数据库错误");
			return FALSE;
		}
		else
		{
			// 无论如何在创建数据库后, 都不能在打开数据库了, 重新启动后就正确
			AfxGetMainWnd()->MessageBox("创建数据库成功, 请重新启动程序", "数据库创建成功"); 
			return TRUE;		
		}
	}

	/************************************************************************/
	/* 窑参数配置表初始化                                                   */
	/************************************************************************/
	m_pKilnInfoTbl = new CHisTbl(11);							// 创建数据库句柄	
	m_pKilnInfoTbl->SetTableName("KilnInfo");					// 指定数据库的表名称
	
	// 指定数据库的表的字段名
	col_index = 0; 
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_id",    MYDB_TYPE_INT);		// 窑序号
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_type",  MYDB_TYPE_INT);		// 窑类型
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_total", MYDB_TYPE_INT);		// 窑总长度或窑总车位数
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_temps", MYDB_TYPE_INT);		// 窑温度总数
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_tempas", MYDB_TYPE_INT);		// 窑边温度总数
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_press", MYDB_TYPE_INT);		// 窑压力总数
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_humis", MYDB_TYPE_INT);		// 窑湿度总数
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_motor", MYDB_TYPE_INT);		// 窑电机总数
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_gross", MYDB_TYPE_INT);		// 窑产量是否计数:0 or 1
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_ctrl",  MYDB_TYPE_INT);		// 窑是自动控制电机数量
	m_pKilnInfoTbl->SetItem(col_index++, "kiln_std",   MYDB_TYPE_INT);		// 窑标准温度号

	if (!m_pKilnInfoTbl->Open(str_utf8))
	{	
		return m_bDBOpen;
	}
	
	m_pKilnInfoTbl->SetFilter("");
	m_pKilnInfoTbl->Query();
	m_iKilnNum = m_pKilnInfoTbl->GetCount();			// 得到总的窑数量
	if (m_iKilnNum < 1)
	{
		return m_bDBOpen;
	}

	m_pKilnInfoTbl->SetFilter("[kiln_gross]=1");
	m_pKilnInfoTbl->Query();
	m_iGrossTotalNum = m_pKilnInfoTbl->GetCount();		// 得到需要产量统计窑的数量;

	m_pKilnInfoTbl->SetFilter("");
	m_pKilnInfoTbl->Query();

	m_pKilnBuff         = new KilnObject[m_iKilnNum];
	m_pKilnBuffForCurve = new KilnObject[m_iKilnNum];
	for (i = 0; i < m_iKilnNum; i++)
	{
		m_pKilnInfoTbl->Move(i + 1);					// 移动到第1条数据

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

		// 创建温度对象
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

		// 创建边温度对象
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

		// 创建压力对象
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

		// 创建湿度对象
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

		// 创建电机对象
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

		// 创建产量对象
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
				m_pKilnBuff[i].m_pGrossObjectBuff[j].TrigInternal  = 15;		// 单位: 分
				m_pKilnBuff[i].m_pGrossObjectBuff[j].TemBellIdx    = 5;
				m_pKilnBuff[i].m_pGrossObjectBuff[j].BellIdx       = j;
				m_pKilnBuff[i].m_pGrossObjectBuff[j].BellValidTime = 15000;		// 单位: 毫秒
			}
		}

		// 创建控制对象
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
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iFrontPoint     = 3;			// 最高温度前面的温度点数 >= 1
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iRearPoint      = 2;			// 最高温度后面的温度点数 >= 1
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iCtrlTime       = 10;			// 控制周期 分钟
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].IncOutCoeff     = (float)0.1;	// 电机步进系数, 最大和最小输出的比例系数
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iGrossWaitTime  = 5;			// 每次推车后的自控等待时间
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iGrossProcTime  = 50;			// 定时推车时间
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].bControl        = FALSE;		// 当前自动控制状态
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iWaitTimerCnt   = 0;			// 屏蔽自动控制计时器清零
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].iProcTimerCnt   = 0;			// 定时推车计时器清零
				m_pKilnBuff[i].m_pCtrlObjectBuff[j].BakMotorVal     = 0.0;			// 变频器暂存变量清零
			}
		}

		// 根据窑类型创建设置实时界面和历史界面标题
		switch (m_pKilnBuff[i].type)
		{
		case 0:
			str.Empty();
//			str.Format("%02d 号 焙 烧 窑 实 时 数 据", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCaption, 0, sizeof(m_pKilnBuff[i].strCaption));
			memcpy(m_pKilnBuff[i].strCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCaption));
			memcpy(m_pKilnBuffForCurve[i].strCaption, str, str.GetLength());

			str.Empty();
//			str.Format("%02d 号 焙 烧 窑 实 时 曲 线", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCurveCaption, 0, sizeof(m_pKilnBuff[i].strCurveCaption));
			memcpy(m_pKilnBuff[i].strCurveCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCurveCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCurveCaption));
			memcpy(m_pKilnBuffForCurve[i].strCurveCaption, str, str.GetLength());
			break;
		case 1:
			str.Empty();
//			str.Format("%02d 号 烘 干 窑 实 时 数 据", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCaption, 0, sizeof(m_pKilnBuff[i].strCaption));
			memcpy(m_pKilnBuff[i].strCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCaption));
			memcpy(m_pKilnBuffForCurve[i].strCaption, str, str.GetLength());

			str.Empty();
//			str.Format("%02d 号 烘 干 窑 实 时 曲 线", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCurveCaption, 0, sizeof(m_pKilnBuff[i].strCurveCaption));
			memcpy(m_pKilnBuff[i].strCurveCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCurveCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCurveCaption));
			memcpy(m_pKilnBuffForCurve[i].strCurveCaption, str, str.GetLength());
			break;
		case 2:
			str.Empty();
//			str.Format("%02d 号 直 通 窑 实 时 数 据", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCaption, 0, sizeof(m_pKilnBuff[i].strCaption));
			memcpy(m_pKilnBuff[i].strCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCaption));
			memcpy(m_pKilnBuffForCurve[i].strCaption, str, str.GetLength());
			
			str.Empty();
//			str.Format("%02d 号 直 通 窑 实 时 曲 线", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCurveCaption, 0, sizeof(m_pKilnBuff[i].strCurveCaption));
			memcpy(m_pKilnBuff[i].strCurveCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCurveCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCurveCaption));
			memcpy(m_pKilnBuffForCurve[i].strCurveCaption, str, str.GetLength());
			break;
		case 3:
			str.Empty();
//			str.Format("%02d 号 移 动 式 隧 道 窑 实 时 数 据", m_pKilnBuff[i].index);
			str.Format("%s", m_strRealTimeName[m_pKilnBuff[i].index].GetBuffer(MAX_PATH));
			memset(m_pKilnBuff[i].strCaption, 0, sizeof(m_pKilnBuff[i].strCaption));
			memcpy(m_pKilnBuff[i].strCaption, str, str.GetLength());
			memset(m_pKilnBuffForCurve[i].strCaption, 0, sizeof(m_pKilnBuffForCurve[i].strCaption));
			memcpy(m_pKilnBuffForCurve[i].strCaption, str, str.GetLength());
			
			str.Empty();
//			str.Format("%02d 号 移 动 式 隧 道 实 时 曲 线", m_pKilnBuff[i].index);
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
	/* 登录表初始化															*/
	/************************************************************************/
	m_pLoginTbl = new CHisTbl(6);
	m_pLoginTbl->SetTableName("LoginInfo");

	// 指定数据库的表的字段名
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
	/* 通讯配置表初始化                                                     */
	/************************************************************************/
	m_pComCfgTbl = new CHisTbl(13);
	m_pComCfgTbl->SetTableName("ComCfgInfo");

	// 指定数据库的表的字段名
	col_index = 0;
	m_pComCfgTbl->SetItem(col_index++, "port_num",       MYDB_TYPE_INT);
	m_pComCfgTbl->SetItem(col_index++, "baudrate",       MYDB_TYPE_INT);
	m_pComCfgTbl->SetItem(col_index++, "parity",         MYDB_TYPE_TEXT);
	m_pComCfgTbl->SetItem(col_index++, "data_bits",      MYDB_TYPE_INT);
	m_pComCfgTbl->SetItem(col_index++, "stop_bits",      MYDB_TYPE_INT);
	m_pComCfgTbl->SetItem(col_index++, "timeout",        MYDB_TYPE_INT);
	m_pComCfgTbl->SetItem(col_index++, "scan_time",      MYDB_TYPE_INT);
	m_pComCfgTbl->SetItem(col_index++, "m_serve_ip",     MYDB_TYPE_INT);	// 实际意义，DTU串口波特率
	m_pComCfgTbl->SetItem(col_index++, "m_serve_port",   MYDB_TYPE_INT);	// 实际意义，服务器编号=1
	m_pComCfgTbl->SetItem(col_index++, "s_serve_ip",     MYDB_TYPE_INT);	// 实际意义，DTU串口号
	m_pComCfgTbl->SetItem(col_index++, "s_serve_port",   MYDB_TYPE_INT);	// 实际意义，DTU编号
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
	/* 终端配置表初始化                                                     */
	/************************************************************************/
	m_pTerminalCfgTbl = new CHisTbl(6);
	m_pTerminalCfgTbl->SetTableName("TerminalCfgInfo");

	// 指定数据库的表的字段名
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
	m_TerminalCfg.iTerminalNum = m_pTerminalCfgTbl->GetCount();		// 设备总数
	if (m_TerminalCfg.iTerminalNum <= 0)
	{
		return m_bDBOpen;
	}

	m_TerminalCfg.m_pTerminalObjectBuff = new TerminalObject[m_TerminalCfg.iTerminalNum];

	for (i = 0; i < m_TerminalCfg.iTerminalNum; i++)
	{
		m_pTerminalCfgTbl->Move(i + 1);				// Move 函数以1为第一个数, 不允许0
		m_TerminalCfg.m_pTerminalObjectBuff[i].DevId   = *((int *)(m_pTerminalCfgTbl->m_pColsName[1]));
		m_TerminalCfg.m_pTerminalObjectBuff[i].DevCmd  = *((int *)(m_pTerminalCfgTbl->m_pColsName[2]));
		m_TerminalCfg.m_pTerminalObjectBuff[i].DevChn  = *((int *)(m_pTerminalCfgTbl->m_pColsName[3]));
		m_TerminalCfg.m_pTerminalObjectBuff[i].DevAddr = *((int *)(m_pTerminalCfgTbl->m_pColsName[4]));
		m_TerminalCfg.m_pTerminalObjectBuff[i].DevType = *((int *)(m_pTerminalCfgTbl->m_pColsName[5]));
	}

	/************************************************************************/
	/* 通道配置表初始化                                                     */
	/************************************************************************/
	m_pChannelCfgTbl = new CHisTbl(9);
	m_pChannelCfgTbl->SetTableName("ChannelCfgInfo");
	
	// 指定数据库的表的字段名
	col_index = 0;
	m_pChannelCfgTbl->SetItem(col_index++, "id",        MYDB_TYPE_INT);			// 索引号
	m_pChannelCfgTbl->SetItem(col_index++, "tem_idx",   MYDB_TYPE_INT);			// 设备号
	m_pChannelCfgTbl->SetItem(col_index++, "phy_idx",   MYDB_TYPE_INT);			// 物理通道号
	m_pChannelCfgTbl->SetItem(col_index++, "kiln_idx",  MYDB_TYPE_INT);			// 窑号
	m_pChannelCfgTbl->SetItem(col_index++, "chn_type",  MYDB_TYPE_INT);			// 通道类型
	m_pChannelCfgTbl->SetItem(col_index++, "car_loc",   MYDB_TYPE_INT);			// 车位号
	m_pChannelCfgTbl->SetItem(col_index++, "chn_name",  MYDB_TYPE_TEXT);		// 名称
	m_pChannelCfgTbl->SetItem(col_index++, "adj_gain",  MYDB_TYPE_DOUBLE);		// 校正放大系数
	m_pChannelCfgTbl->SetItem(col_index++, "adj_zero",  MYDB_TYPE_DOUBLE);		// 校正零点系数

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
		/* 温度通道初始化                                                       */
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
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

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
		/* 边温度通道初始化                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=1", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// 按车位号先后排序
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuffForCurve[i].TemperatureANum)
		{
			for (j = 0; j < m_pKilnBuffForCurve[i].TemperatureANum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

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
		/* 压力通道初始化                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=2", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// 按车位号先后排序
		m_pChannelCfgTbl->Query();
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuffForCurve[i].PressNum)
		{
			for (j = 0; j < m_pKilnBuffForCurve[i].PressNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

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
		/* 湿度通道初始化                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=3", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// 按车位号先后排序
		m_pChannelCfgTbl->Query();		
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuffForCurve[i].HumidityNum)
		{
			for (j = 0; j < m_pKilnBuffForCurve[i].HumidityNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

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
		/* 电机输入通道初始化                                                   */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=4", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// 按车位号先后排序
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuffForCurve[i].MotorNum)
		{
			for (j = 0; j < m_pKilnBuffForCurve[i].MotorNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

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
		/* 产量输入通道初始化                                                   */
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
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

				m_pKilnBuffForCurve[i].m_pGrossObjectBuff[j].TemTrigIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuffForCurve[i].m_pGrossObjectBuff[j].TrigIdx     = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
			}
		}

		/************************************************************************/
		/* 产量输出通道初始化                                                   */
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
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

				m_pKilnBuffForCurve[i].m_pGrossObjectBuff[j].TemBellIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuffForCurve[i].m_pGrossObjectBuff[j].BellIdx     = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
			}
		}

		/************************************************************************/
		/* 电机输出通道初始化                                                   */
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
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

				m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].TemOutIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuffForCurve[i].m_pMotorObjectBuff[j].BufOutIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
			}
		}
	} /* for (i = 0; i < m_iKilnNum; i++) */

	for (i = 0; i < m_iKilnNum; i++)
	{
		/************************************************************************/
		/* 温度通道初始化                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=0", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// 按车位号先后排序
		m_pChannelCfgTbl->Query();	

		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuff[i].TemperatureNum)
		{
			for (j = 0; j < m_pKilnBuff[i].TemperatureNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

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
		/* 边温度通道初始化                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=1", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// 按车位号先后排序
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuff[i].TemperatureANum)
		{
			for (j = 0; j < m_pKilnBuff[i].TemperatureANum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

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
		/* 压力通道初始化                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=2", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// 按车位号先后排序
		m_pChannelCfgTbl->Query();
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuff[i].PressNum)
		{
			for (j = 0; j < m_pKilnBuff[i].PressNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

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
		/* 湿度通道初始化                                                       */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=3", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// 按车位号先后排序
		m_pChannelCfgTbl->Query();		
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuff[i].HumidityNum)
		{
			for (j = 0; j < m_pKilnBuff[i].HumidityNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

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
		/* 电机输入通道初始化                                                   */
		/************************************************************************/
		str.Empty();
		str.Format("[kiln_idx]=%d and [chn_type]=4", i + 1);
		m_pChannelCfgTbl->SetFilter(str);
		m_pChannelCfgTbl->SetSort("[car_loc] ASC");				// 按车位号先后排序
		m_pChannelCfgTbl->Query();							
		
		if (m_pChannelCfgTbl->GetCount() == m_pKilnBuff[i].MotorNum)
		{
			for (j = 0; j < m_pKilnBuff[i].MotorNum; j++)
			{
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

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
		/* 产量输入通道初始化                                                   */
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
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

				m_pKilnBuff[i].m_pGrossObjectBuff[j].TemTrigIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuff[i].m_pGrossObjectBuff[j].TrigIdx     = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
			}
		}

		/************************************************************************/
		/* 产量输出通道初始化                                                   */
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
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

				m_pKilnBuff[i].m_pGrossObjectBuff[j].TemBellIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuff[i].m_pGrossObjectBuff[j].BellIdx     = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
			}
		}

		/************************************************************************/
		/* 电机输出通道初始化                                                   */
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
				m_pChannelCfgTbl->Move(j + 1);				// Move 函数以1为第一个数, 不允许0

				m_pKilnBuff[i].m_pMotorObjectBuff[j].TemOutIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[1])); 
				m_pKilnBuff[i].m_pMotorObjectBuff[j].BufOutIdx  = *((int *)(m_pChannelCfgTbl->m_pColsName[2])); 
			}
		}
	} /* for (i = 0; i < m_iKilnNum; i++) */

	/************************************************************************/
	/* 创建标准温度表                                                       */
	/************************************************************************/
	m_pStandardTbl = new CHisTbl *[m_iKilnNum];
	for (i = 0; i < m_iKilnNum; i++)
	{
		m_pStandardTbl[i] = new CHisTbl(m_pKilnBuff[i].TemperatureNum + 2);

		str.Empty();
		str.Format("StandardCfgInfo_%d", i + 1);
		m_pStandardTbl[i]->SetTableName(str);
		
		// 指定数据库的表的字段名
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
	/* 创建历史数据表的个数, 每条窑只有1张历史数据表                        */
	/************************************************************************/
	m_pHisTbl = new CHisTbl *[m_iKilnNum];	
	for (i = 0; i < m_iKilnNum; i++)
	{
		InitHisDBTable(i);
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 自动控制参数表
	m_pAutoCtrlTbl = new CHisTbl(9);						// 不管窑是否自控, 每一条窑都建立自控记录

	m_pAutoCtrlTbl->SetTableName("AutoCtrlInfo");

	// 指定数据库的表的字段名
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
			// 初始化, 防止开机打铃
			m_pKilnBuff[i].m_pCtrlObjectBuff[0].iProcTimerCnt  = m_pKilnBuff[i].m_pCtrlObjectBuff[0].iGrossProcTime * 60;
		}
	}

	/************************************************************************/
	/* 产量记录表初始化                                                     */
	/************************************************************************/
	m_pGrossTbl = new CHisTbl(4);
	m_pGrossTbl->SetTableName("OutputsInfo");
	
	// 指定数据库的表的字段名
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
	
	// 数据库配置操作: 字段个数 = id + time + 各类测试点
	m_pHisTbl[iFurnaceNum] = new CHisTbl(m_pKilnBuff[iFurnaceNum].TemperatureNum + 
										 m_pKilnBuff[iFurnaceNum].TemperatureANum + 
										 m_pKilnBuff[iFurnaceNum].PressNum + 
										 m_pKilnBuff[iFurnaceNum].HumidityNum + 
										 m_pKilnBuff[iFurnaceNum].MotorNum + 2);		
	
	// 指定数据库的历史表名称
	str.Format("History_%d", iFurnaceNum + 1);
	m_pHisTbl[iFurnaceNum]->SetTableName(str.GetBuffer(MAX_PATH));
	
	// 指定数据库的历史数据表的字段名
	int col_index = 0;
	m_pHisTbl[iFurnaceNum]->SetItem(col_index++, "id", MYDB_TYPE_INT);
	m_pHisTbl[iFurnaceNum]->SetItem(col_index++, "save_time", MYDB_TYPE_TEXT);
	
	// 指定数据库的历史数据表的温度字段名
	for (int i = 0; i < m_pKilnBuff[iFurnaceNum].TemperatureNum; i++)
	{
		str.Empty();
		str.Format("temperature_%d", i + 1);
		m_pHisTbl[iFurnaceNum]->SetItem(col_index++, str, MYDB_TYPE_INT);
	}

	// 指定数据库的历史数据表的边温度字段名
	for (i = 0; i < m_pKilnBuff[iFurnaceNum].TemperatureANum; i++)
	{
		str.Empty();
		str.Format("temperaturea_%d", i + 1);
		m_pHisTbl[iFurnaceNum]->SetItem(col_index++, str, MYDB_TYPE_INT);
	}
	
	// 指定数据库的历史数据表的压力字段名
	for (i = 0; i < m_pKilnBuff[iFurnaceNum].PressNum; i++)
	{
		str.Empty();
		str.Format("press_%d", i + 1);
		m_pHisTbl[iFurnaceNum]->SetItem(col_index++, str, MYDB_TYPE_INT);
	}
	
	// 指定数据库的历史数据表的湿度字段名
	for (i = 0; i < m_pKilnBuff[iFurnaceNum].HumidityNum; i++)
	{
		str.Empty();
		str.Format("humidity_%d", i + 1);
		m_pHisTbl[iFurnaceNum]->SetItem(col_index++, str, MYDB_TYPE_DOUBLE);
	}
	
	// 指定数据库的历史数据表的电机字段名
	for (i = 0; i < m_pKilnBuff[iFurnaceNum].MotorNum; i++)
	{
		str.Empty();
		str.Format("motor_%d", i + 1);
		m_pHisTbl[iFurnaceNum]->SetItem(col_index++, str, MYDB_TYPE_DOUBLE);
	}
	
	// 打开数据库, 每打开1个数据库都会获得1个句柄, 可以操作同一个数据库
	// 打开数据库的同时, 已经执行了一次成功的查询操作
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
	/* 传感器类型: 电机                                                     */
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
	/* 传感器类型: 温度                                                     */
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
	/* 传感器类型: 边温度                                                     */
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
	/* 传感器类型: 压力                                                     */
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
	/* 传感器类型: 湿度                                                     */
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
	/* 传感器类型: 产量                                                     */
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
// 					// 发送响铃命令
// 					tem_idx = m_pKilnBuff[i].m_pGrossObjectBuff[j].TemBellIdx;
// 					buf_idx = m_pKilnBuff[i].m_pGrossObjectBuff[j].BellIdx;
// 					tmp     = m_pKilnBuff[i].m_pGrossObjectBuff[j].BellValidTime;
// 					pMainFrame->m_pCom->SetWriteTerminalInfo(tem_idx, (buf_idx + 40312), 1, &tmp);

					// 将自动控制功能屏蔽一段时间
					if ((m_pKilnBuff[i].ControlNum > 0) 
						&& (m_pKilnBuff[i].m_pCtrlObjectBuff[0].bControl == TRUE))
					{
						m_pKilnBuff[i].m_pCtrlObjectBuff[0].iWaitTimerCnt = 60 * m_pKilnBuff[i].m_pCtrlObjectBuff[0].iGrossWaitTime;
					}

					// 重置推车定时器
					m_pKilnBuff[i].m_pCtrlObjectBuff[0].iProcTimerCnt = 60 * m_pKilnBuff[i].m_pCtrlObjectBuff[0].iGrossProcTime;
				}	
			}
		}
	}

	return res;
}

void CIControlDoc::DevPowerOn(void)
{
	m_pLoginTbl->Query();					// 执行一次查询操作
	m_pLoginTbl->MoveFirst();
	m_pLoginTbl->Edit();					// 执行编辑操作
	m_iDevPower = 1;
	*((int *)(m_pLoginTbl->m_pColsName[3])) = m_iDevPower;
	m_pLoginTbl->Update();
	m_pLoginTbl->Query();	
}

void CIControlDoc::DevPowerOff(void)
{
	m_pLoginTbl->Query();					// 执行一次查询操作
	m_pLoginTbl->MoveFirst();
	m_pLoginTbl->Edit();					// 执行编辑操作
	m_iDevPower = 0;
	*((int *)(m_pLoginTbl->m_pColsName[3])) = m_iDevPower;
	m_pLoginTbl->Update();
	m_pLoginTbl->Query();
}

BOOL CIControlDoc::SetComCfgTbl(void)
{
	CString str;

	m_pComCfgTbl->Query();							// 执行一次查询操作
	
	if (m_pComCfgTbl->MoveFirst() == true)			// 移动到第一条数据					
	{	
		if (m_pComCfgTbl->GetCount() >= 1)
		{
			m_pComCfgTbl->Edit();					// 执行编辑操作

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

// type=0 温度
//      1 边温
//      2 压力
//      3 湿度
//      4 电机
BOOL CIControlDoc::SetAdjustValue(int iFurnaceNum, int type)
{
	int     i;
	CString str;

	str.Format("[kiln_idx]=%d and [chn_type]=%d", iFurnaceNum + 1, type);
	m_pChannelCfgTbl->SetFilter(str);
	m_pChannelCfgTbl->Query();								// 执行一次查询操作, 找到设定的曲线编号

	switch (type)
	{
	case 0:
		for (i = 0; i < m_pKilnBuff[iFurnaceNum].TemperatureNum; i++)
		{
			if (m_pChannelCfgTbl->Move(i + 1) == false)		// Move 函数以1为第一个数, 不允许0
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
			if (m_pChannelCfgTbl->Move(i + 1) == false)		// Move 函数以1为第一个数, 不允许0
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
			if (m_pChannelCfgTbl->Move(i + 1) == false)				// Move 函数以1为第一个数, 不允许0
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
			if (m_pChannelCfgTbl->Move(i + 1) == false)				// Move 函数以1为第一个数, 不允许0
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
			if (m_pChannelCfgTbl->Move(i + 1) == false)				// Move 函数以1为第一个数, 不允许0
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

	str.Format("[id]=%d", iStandardIndex);					// 查询指定标准温度号记录
	m_pStandardTbl[iFurnaceNum]->SetFilter(str);
	m_pStandardTbl[iFurnaceNum]->Query();					// 执行一次查询操作
	m_pStandardTbl[iFurnaceNum]->SetFilter("");				// 清空查询条件
	if (m_pStandardTbl[iFurnaceNum]->GetCount() > 0)		// 判断标准曲线好是否合法		
	{
		// 移动到指定窑号记录, 更新标准温度号并存储
		m_pKilnInfoTbl->SetFilter("");
		m_pKilnInfoTbl->Query();
		if (m_pKilnInfoTbl->Move(iFurnaceNum + 1) == true)
		{
			m_pKilnBuff[iFurnaceNum].StandardIndex = iStandardIndex;	// 更新标准温度号

			// 更新数据库操作
			m_pKilnInfoTbl->Edit();	
			*((int *)(m_pKilnInfoTbl->m_pColsName[10])) = m_pKilnBuff[iFurnaceNum].StandardIndex;
			m_pKilnInfoTbl->Update();
			m_pKilnInfoTbl->Query();

			// 将新的标准温度数据复制到缓冲区中
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
		// 如果删除的标准曲线是当前使用的标准曲线, 直接返回错误
		if (*((int *)(m_pKilnInfoTbl->m_pColsName[10])) == iStandardIndex)
		{
			return FALSE;
		}

		str.Empty();
		str.Format("[id]=%d", iStandardIndex);					// 查询指定标准温度号记录
		m_pStandardTbl[iFurnaceNum]->SetFilter(str);
		m_pStandardTbl[iFurnaceNum]->Query();					// 执行一次查询操作
		if (m_pStandardTbl[iFurnaceNum]->GetCount() > 0)		// 判断标准曲线好是否合法
		{
			m_pStandardTbl[iFurnaceNum]->MoveFirst();
			m_pStandardTbl[iFurnaceNum]->Delete();				// 执行删除操作
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
		m_pStandardTbl[iFurnaceNum]->SetSort("[id] ASC");		// 按id号的升序排列
		m_pStandardTbl[iFurnaceNum]->Query();
		m_pStandardTbl[iFurnaceNum]->MoveLast();
		insert_index = *((int *)(m_pStandardTbl[iFurnaceNum]->m_pColsName[0])) + 1;	// 新增记录的编号在所有记录id号之后
		
		m_pStandardTbl[iFurnaceNum]->AddNew();					// 添加新记录		
		*((int *)(m_pStandardTbl[iFurnaceNum]->m_pColsName[0])) = insert_index;		// 曲线编号
		
		CTime t = CTime::GetCurrentTime();	// 获取系统日期
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
			SetStandardValue(iFurnaceNum, insert_index);			// 应用新记录
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

/* 向数据库添加历史记录信息 */
BOOL CIControlDoc::AppendHistoryRecord(void)
{
	int i, j;
	int insert_index;

	CTime t = CTime::GetCurrentTime();	// 获取系统日期
	CString str;
	str.Format("%04d-%02d-%02d %02d:%02d:%02d", t.GetYear(), 
												t.GetMonth(), 
												t.GetDay(), 
												t.GetHour(), 
												t.GetMinute(), 
												t.GetSecond());

	for (i = 0; i < m_iKilnNum; i++)
	{
		m_pHisTbl[i]->SetSort("[id] ASC");		// 主键值ID号按升序排列
		m_pHisTbl[i]->Query();
		m_pHisTbl[i]->MoveLast();

		// 无数据或ID号为非法值，初始化为1
		if ((m_pHisTbl[i]->GetCount() <= 0) || (*((int *)(m_pHisTbl[i]->m_pColsName[0])) <= 0))
		{
			insert_index = 1;												
		}
		else
		{
			insert_index = *((int *)(m_pHisTbl[i]->m_pColsName[0])) + 1;	// 新增记录的编号在所有记录id号之后
		}		

		// 添加新记录
		m_pHisTbl[i]->AddNew();
		*((int *)(m_pHisTbl[i]->m_pColsName[0])) = insert_index;		// 曲线编号
		*((SQLiteString *)(m_pHisTbl[i]->m_pColsName[1])) = str;		// 存储时间
		
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
	CTime t = CTime::GetCurrentTime();	// 获取系统日期;
	CString str, str1;
	str.Format("%04d-%02d-%02d %02d:%02d:%02d", t.GetYear(), 
												t.GetMonth(), 
												t.GetDay(), 
												t.GetHour(), 
												t.GetMinute(), 
												t.GetSecond());

	/* 产量表更新, 查找对应砖窑的最后1条产量 */
	str1.Format("[kiln_idx]=%d", iKilnIdx + 1);
	m_pGrossTbl->SetFilter(str1);
	m_pGrossTbl->SetSort("[out_time] DESC");
	m_pGrossTbl->Query();
	m_pGrossTbl->MoveFirst();

	/* 表中如果含有记录, 需要检查产量记录之间的时间 */
	if (m_pGrossTbl->GetCount() > 0)
	{
		t_str  = *((SQLiteString *)(m_pGrossTbl->m_pColsName[1]));		// 得到最后一条产量的时间
		last_t = StringToDate(t_str);
		if (last_t == NULL)
		{
			return FALSE;
		}

		timespan = t - last_t;
		/* 产量记录之间的间隔时间必须在n分钟以上 */
		LONG min_span = timespan.GetTotalMinutes();
		if (min_span < m_pKilnBuff[iKilnIdx].m_pGrossObjectBuff[0].TrigInternal)
		{
// 			str.Empty();
// 			str.Format("每次推车操作必须大于%d分钟间隔!", m_ParaCfg.iGrossInternal);
// 
// 			CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
// 			pMainFrame->CreateGrossInfo(str);
			return FALSE;
		}
	}

	/* 添加产量记录 */
	m_pGrossTbl->AddNew();
	*((int *)(m_pGrossTbl->m_pColsName[0]))          = iKilnIdx + 1;	// 窑号
	*((SQLiteString *)(m_pGrossTbl->m_pColsName[1])) = str;				// 存储时间
	*((int *)(m_pGrossTbl->m_pColsName[2]))          = 0;				// 方式
	*((int *)(m_pGrossTbl->m_pColsName[3]))          = 0;				// 未上传
	m_pGrossTbl->Update();
	m_pGrossTbl->Query();

	str.Empty();
	str.Format("%02d-%02d-%02d %02d:%02d:%02d\r\n推车操作成功!", t.GetYear(), 
																 t.GetMonth(), 
																 t.GetDay(), 
																 t.GetHour(), 
																 t.GetMinute(), 
																 t.GetSecond());
	
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	// 向远端服务器发送产量信息
	pMainFrame->m_pCom->SetGrossTransmit(iKilnIdx, 0, t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	// 弹出产量对话框
	pMainFrame->CreateGrossInfo(str);
	
	return TRUE;
}

BOOL CIControlDoc::GetGrossNotUpService(void)
{
	CString str;
	int iKilnIdx, GrossType;
	CTime t;

	/* 产量表: 列出所有未上传的产量数据 */
	m_pGrossTbl->SetFilter("[up_serve]=0");
	m_pGrossTbl->SetSort("[out_time] ASC");
	m_pGrossTbl->Query();
	m_pGrossTbl->MoveFirst();

	if (m_pGrossTbl->GetCount() <= 0) // 没有未上传的数据
	{
		return FALSE;
	}

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	// 将所有未上传的产量数据写入队列
	for (int i = 0; i < m_pGrossTbl->GetCount(); i++)
	{
		iKilnIdx  = (*((int *)(m_pGrossTbl->m_pColsName[0]))) - 1;
		GrossType = (*((int *)(m_pGrossTbl->m_pColsName[2])));
		
		str  = *((SQLiteString *)(m_pGrossTbl->m_pColsName[1]));		// 得到第一条未上传的产量数据的时间
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
	/* 产量表: 列出所有未上传的产量数据 */
	str.Format("[kiln_idx]=%d and [out_time]=\'%s\' and [condition]=%d and [up_serve]=%d", TunnelKilnId + 1, str_t, GrossType, 0);

	m_pGrossTbl->SetFilter(str);
	m_pGrossTbl->SetSort("[out_time] ASC");
	m_pGrossTbl->Query();
	m_pGrossTbl->MoveFirst();

	if (m_pGrossTbl->GetCount() <= 0) // 没有未上传的数据
	{
		return FALSE;
	}

	m_pGrossTbl->Edit();							// 进入编辑模式
	*((int *)(m_pGrossTbl->m_pColsName[3]))  = 1;	// 已上传
	m_pGrossTbl->Update();

	return TRUE;
}

/************************************************************************/
/* 下面是将当前推车时的温度与之前两次推车时的温度做平均, 然后设置为当前 */
/* 当前系统的标准温度曲线.                                              */
/* 1. 根据窑的温度点数创建缓冲区                                        */
/* 2. 查找小于上次推车时间的温度历史数据                                */
/************************************************************************/
BOOL CIControlDoc::AutoSetStandardRecord(int iKilnIdx)
{
	CString str, str1;
	CTime   t;
	int     i;
	int iSTemperatureNum  = m_pKilnBuff[iKilnIdx].TemperatureNum;	// 标准曲线温度个数
	int *pTemperatureBuff = new int[iSTemperatureNum];				// 创建缓冲区
	
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

	if (count >= 1)							/* 只有1条产量数据, 直接将当前的曲线作为标准曲线并应用 */
	{
		m_pGrossTbl->Move(1);				/* 获得第1条产量记录的时间 */
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
		
		
		/* 查找小于第二条产量记录时间的历史数据 */
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
		/* 获得第二条产量记录的时间 */
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
		

		/* 查找小于第二条产量记录时间的历史数据 */
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
		m_pGrossTbl->Move(3);							/* 获得第3条产量记录的时间 */
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
		
		/* 查找小于第二条产量记录时间的历史数据 */
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
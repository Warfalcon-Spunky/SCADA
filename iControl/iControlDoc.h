// iControlDoc.h : interface of the CIControlDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICONTROLDOC_H__6C20186E_7711_4F83_AD0A_0DBFCB0F5306__INCLUDED_)
#define AFX_ICONTROLDOC_H__6C20186E_7711_4F83_AD0A_0DBFCB0F5306__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HisTbl.h"

/************************************************************************/
/* 温度对象                                                             */
/************************************************************************/
typedef struct tagTemperatureObject
{
	int    TemIdx;						// 设备号
	int    BufIdx;						// 物理缓冲区位置, 此值应该是唯一的
	int    Value;						// 采样值
	float  Gain;						// 校正参数: 量程参数
	float  Zero;						// 校正参数: 零点参数
	char   strName[21];					// 显示字符串: 实时界面和历史界面

	int    SValue;						// 标准温度
	int    CarIdex;						// 车位或米数, 用于曲线作图
} TemperatureObject, *LPTemperatureObject;

/************************************************************************/
/* 压力对象                                                             */
/************************************************************************/
typedef struct tagPressObject
{
	int    TemIdx;						// 设备号
	int    BufIdx;
	int    Value;
	float  Gain;
	float  Zero;
	char   strName[21];
} PressObject, *LPPressObject;

/************************************************************************/
/* 湿度对象                                                             */
/************************************************************************/
typedef struct tagHumidityObject
{
	int    TemIdx;						// 设备号
	int    BufIdx;
	int    Value;
	float  Gain;
	float  Zero;
	char   strName[21];
} HumidityObject, *LPHumidityObject;

/************************************************************************/
/* 电机对象                                                             */
/************************************************************************/
typedef struct tagMotorObject
{
	int    TemIdx;						// 电机采样设备号
	int    BufIdx;						// 电机采样通道号
	int    TemOutIdx;					// 电机输出设备号
	int    BufOutIdx;					// 电机输出通道号
	float  Value;
	float  Gain;
	float  Zero;
	char   strName[21];
	BOOL   bControl;
} MotorObject, *LPMotorObject;

/************************************************************************/
/* 产量对象                                                             */
/************************************************************************/
typedef struct tagGrossObject
{
	int    TemTrigIdx;
	int    TrigIdx;
	int    TrigInternal;
	int    TemBellIdx;						// 设备号
	int    BellIdx;
	int    BellValidTime;
} GrossObject, *LPGrossObject;

/************************************************************************/
/* 控制对象                                                             */
/************************************************************************/
typedef struct tagCtrlObject
{
	float  UpLimitOut;				// 电机输出上限值
	float  DnLimitOut;				// 电机输出下限值
	int    iFrontPoint;				// 最高温度前面的温度点数 >= 1
	int    iRearPoint;				// 最高温度后面的温度点数 >= 1
	int    iCtrlTime;				// 控制周期 分钟
	float  IncOutCoeff;				// 电机步进值0~50
	int    iGrossWaitTime;			// 每次推车后的自控等待时间
	int    iGrossProcTime;			// 定时推车时间
	BOOL   bControl;				// 当前自动控制状态
	int    iWaitTimerCnt;			// 推车后屏蔽自动控制功能计时器
	int    iProcTimerCnt;			// 定时推车功能定时器
	float  BakMotorVal;				// 在手动变为自动时记录变频器的值
} CtrlObject, *LPCtrlObject;

/************************************************************************/
/* 砖窑对象                                                             */
/************************************************************************/
typedef struct tagKilnObject
{
	char      strCaption[100];
	char      strCurveCaption[100];

	int       index;
	int       type;
	int       TemperatureNum;
	int       TemperatureANum;
	int       PressNum;
	int       HumidityNum;
	int       MotorNum;
	int       TotalKiln;
	int       StandardIndex;
	int       GrossNum;
	int       ControlNum;

	LPTemperatureObject m_pTempObjectBuff;
	LPTemperatureObject m_pTempAObjectBuff;
	LPPressObject       m_pPressObjectBuff;
	LPHumidityObject    m_pHumiObjectBuff;
	LPMotorObject       m_pMotorObjectBuff;
	LPGrossObject       m_pGrossObjectBuff;
	LPCtrlObject        m_pCtrlObjectBuff;
} KilnObject, *LPKilnObject;

typedef struct tagComCfg
{
	int        iPortNum;
	int        iBaudrate;
	char       cParity;
	int        iDataBits;
	int        iStopBits;
	int        iTimeout;
	int        iScanTime;

	int	       iDtuBaudrate;
	DWORD      lServePort;
	int 	   iDtuPortNum;
	DWORD      lDtuPort;
} ComCfg, *LPComCfg;


typedef struct tagTerminalObject
{
	BYTE       DevId;			// 设备地址
	BYTE       DevCmd;			// 设备命令
	BYTE       DevChn;			// 设备通道数
	WORD       DevAddr;			// 设备寄存器起始地址
	BYTE       DevType;			// 设备类型: 0 = 周期型 1 = 事件型
} TerminalObject, *LPTerminalObject;

typedef struct tagTerminalCfg
{
	int               iTerminalNum;
	LPTerminalObject  m_pTerminalObjectBuff;
} TerminalCfg, *LPTerminalCfg;

class CIControlDoc : public CDocument
{
protected: // create from serialization only
	CIControlDoc();
	DECLARE_DYNCREATE(CIControlDoc)

// Attributes
public:
	sqlite3 *m_pSqlite;

	BOOL m_bScrollBarCreate;
	BOOL m_bDBOpen;

	CHisTbl *m_pLoginTbl;
	CHisTbl *m_pKilnInfoTbl;
	CHisTbl *m_pComCfgTbl;
	CHisTbl *m_pTerminalCfgTbl;
	CHisTbl *m_pChannelCfgTbl;
	CHisTbl *m_pGrossTbl;
	CHisTbl *m_pAutoCtrlTbl;

	CHisTbl **m_pHisTbl;
	CHisTbl **m_pStandardTbl;

	int       m_iDevPower;

	int       m_iKilnNum;			// 窑条数
	int       m_iGrossTotalNum;		// 需要产量计数窑的总和
	int       m_iGrossProcMaxTime[4];	// 最大顶车打铃时间
	int	      m_iUpWarn[4];				// 温度上限报警值
	int	      m_iDnWarn[4];				// 温度下限报警值

	CString   m_strFactoryName;
	CString   m_strSalePhone;

	CString   m_strShellPath;		// 数据库命令行路径
	CString   m_strCFGPath;			// 配置文件路径
	CString   m_strLogPath;			// LOG 文件路径
	CString   m_strDBPath;			// 数据库路径
	CString   m_strDBBakPath;		// 数据库备份路径
	CString   m_strBKPicPath;		// 背景图片路径
	CString   m_strRTPicPath;		// 实时图片路径
	CString   m_strRealTimeName[4];	// 实时数据界面标题
	int       m_iHisSaveInternal;	// 历史数据存储间隔(单位秒)

	LPKilnObject m_pKilnBuff;
	LPKilnObject m_pKilnBuffForCurve;

	ComCfg       m_ComCfg;			// 通讯参数对象
	TerminalCfg  m_TerminalCfg;		// 终端设置对象

	CString	   m_strHistoryQuery;	// 历史数据查询条件

// Operations
public:
	CString MbcsToUtf8(const char *file);
	BOOL    ImportDataBase(const char *pFileName);
	BOOL    DataBaseInit(void);
	void    InitHisDBTable(int iFurnaceNum);
	void    DevPowerOn(void);
	void    DevPowerOff(void);
	BOOL    SetComCfgTbl(void);
	BOOL    UpdateRealTimeValue(void);
	BOOL    SetAdjustValue(int iFurnaceNum, int type);
	BOOL    SetStandardValue(int iFurnaceNum, int iStandardIndex);
	BOOL    DelStandardValue(int iFurnaceNum, int iStandardIndex);
	int     GetHistoryRecordCnt(void);
	BOOL    AppendHistoryRecord(void);
	BOOL    AppendStandardRecord(int iFurnaceNum, int *pTemperatureBuff, BOOL bSetStandard);
	BOOL    AppendGrossRecord(int iKilnIdx);
	BOOL    GetGrossNotUpService(void);
	BOOL    SetGrossUpService(BYTE TunnelKilnId, BYTE GrossType, WORD Year, BYTE Month, BYTE Day, BYTE Hour, BYTE Min, BYTE Sec);
	BOOL	AutoSetStandardRecord(int iKilnIdx);
	BOOL    SaveAutoCtrlInfoRecord(int iKilnIdx);
	CTime   StringToDate(CString strDate);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIControlDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIControlDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CIControlDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONTROLDOC_H__6C20186E_7711_4F83_AD0A_0DBFCB0F5306__INCLUDED_)

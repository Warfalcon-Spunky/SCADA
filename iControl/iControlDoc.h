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
/* �¶ȶ���                                                             */
/************************************************************************/
typedef struct tagTemperatureObject
{
	int    TemIdx;						// �豸��
	int    BufIdx;						// ��������λ��, ��ֵӦ����Ψһ��
	int    Value;						// ����ֵ
	float  Gain;						// У������: ���̲���
	float  Zero;						// У������: ������
	char   strName[21];					// ��ʾ�ַ���: ʵʱ�������ʷ����

	int    SValue;						// ��׼�¶�
	int    CarIdex;						// ��λ������, ����������ͼ
} TemperatureObject, *LPTemperatureObject;

/************************************************************************/
/* ѹ������                                                             */
/************************************************************************/
typedef struct tagPressObject
{
	int    TemIdx;						// �豸��
	int    BufIdx;
	int    Value;
	float  Gain;
	float  Zero;
	char   strName[21];
} PressObject, *LPPressObject;

/************************************************************************/
/* ʪ�ȶ���                                                             */
/************************************************************************/
typedef struct tagHumidityObject
{
	int    TemIdx;						// �豸��
	int    BufIdx;
	int    Value;
	float  Gain;
	float  Zero;
	char   strName[21];
} HumidityObject, *LPHumidityObject;

/************************************************************************/
/* �������                                                             */
/************************************************************************/
typedef struct tagMotorObject
{
	int    TemIdx;						// ��������豸��
	int    BufIdx;						// �������ͨ����
	int    TemOutIdx;					// �������豸��
	int    BufOutIdx;					// ������ͨ����
	float  Value;
	float  Gain;
	float  Zero;
	char   strName[21];
	BOOL   bControl;
} MotorObject, *LPMotorObject;

/************************************************************************/
/* ��������                                                             */
/************************************************************************/
typedef struct tagGrossObject
{
	int    TemTrigIdx;
	int    TrigIdx;
	int    TrigInternal;
	int    TemBellIdx;						// �豸��
	int    BellIdx;
	int    BellValidTime;
} GrossObject, *LPGrossObject;

/************************************************************************/
/* ���ƶ���                                                             */
/************************************************************************/
typedef struct tagCtrlObject
{
	float  UpLimitOut;				// ����������ֵ
	float  DnLimitOut;				// ����������ֵ
	int    iFrontPoint;				// ����¶�ǰ����¶ȵ��� >= 1
	int    iRearPoint;				// ����¶Ⱥ�����¶ȵ��� >= 1
	int    iCtrlTime;				// �������� ����
	float  IncOutCoeff;				// �������ֵ0~50
	int    iGrossWaitTime;			// ÿ���Ƴ�����Կصȴ�ʱ��
	int    iGrossProcTime;			// ��ʱ�Ƴ�ʱ��
	BOOL   bControl;				// ��ǰ�Զ�����״̬
	int    iWaitTimerCnt;			// �Ƴ��������Զ����ƹ��ܼ�ʱ��
	int    iProcTimerCnt;			// ��ʱ�Ƴ����ܶ�ʱ��
	float  BakMotorVal;				// ���ֶ���Ϊ�Զ�ʱ��¼��Ƶ����ֵ
} CtrlObject, *LPCtrlObject;

/************************************************************************/
/* שҤ����                                                             */
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
	BYTE       DevId;			// �豸��ַ
	BYTE       DevCmd;			// �豸����
	BYTE       DevChn;			// �豸ͨ����
	WORD       DevAddr;			// �豸�Ĵ�����ʼ��ַ
	BYTE       DevType;			// �豸����: 0 = ������ 1 = �¼���
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

	int       m_iKilnNum;			// Ҥ����
	int       m_iGrossTotalNum;		// ��Ҫ��������Ҥ���ܺ�
	int       m_iGrossProcMaxTime[4];	// ��󶥳�����ʱ��
	int	      m_iUpWarn[4];				// �¶����ޱ���ֵ
	int	      m_iDnWarn[4];				// �¶����ޱ���ֵ

	CString   m_strFactoryName;
	CString   m_strSalePhone;

	CString   m_strShellPath;		// ���ݿ�������·��
	CString   m_strCFGPath;			// �����ļ�·��
	CString   m_strLogPath;			// LOG �ļ�·��
	CString   m_strDBPath;			// ���ݿ�·��
	CString   m_strDBBakPath;		// ���ݿⱸ��·��
	CString   m_strBKPicPath;		// ����ͼƬ·��
	CString   m_strRTPicPath;		// ʵʱͼƬ·��
	CString   m_strRealTimeName[4];	// ʵʱ���ݽ������
	int       m_iHisSaveInternal;	// ��ʷ���ݴ洢���(��λ��)

	LPKilnObject m_pKilnBuff;
	LPKilnObject m_pKilnBuffForCurve;

	ComCfg       m_ComCfg;			// ͨѶ��������
	TerminalCfg  m_TerminalCfg;		// �ն����ö���

	CString	   m_strHistoryQuery;	// ��ʷ���ݲ�ѯ����

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

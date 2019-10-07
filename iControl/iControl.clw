; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=generic CWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "iControl.h"
LastPage=0

ClassCount=29
Class1=CIControlApp
Class2=CIControlDoc
Class3=CIControlView
Class4=CMainFrame
Class9=CHisCurveView

ResourceCount=10
Resource1=IDD_ADD_GROSS
Class5=CAboutDlg
Class6=CRealTimeView
Class7=CCurCurveView
Class8=CHisGridView
Class10=CDataBase
Class11=CSpliteFrame
Class12=CMySplitterWnd
Class13=CHisSGridView
Resource2=IDD_REGISTER_DIALOG
Class14=CLoginDialog
Class15=CStandardView
Resource3=IDD_GROSS_INFO
Class16=CSerialDlg
Class17=CIGridCtrl
Class18=CModBus
Class19=CGrossView
Resource4=IDD_AUTOCTRL
Class20=CAddGrossDialog
Class21=CSetupView
Resource5=IDD_SERIAL_DIALOG
Class22=CRegisterDlg
Resource6=IDD_MOTOR_DIALOG
Class23=CMotorCtrlDlg
Class24=CAdjustView
Resource7=IDD_LOGIN_DIALOG
Class25=CGrossInfo
Resource8=IDR_MAINFRAME
Class26=CAutoCtrlDlg
Class27=CAutoCtrlView
Resource9=IDD_ABOUTBOX
Class28=CTimeQueryDlg
Class29=CDtuBus
Resource10=IDD_QUERY

[CLS:CIControlApp]
Type=0
HeaderFile=iControl.h
ImplementationFile=iControl.cpp
Filter=N
LastObject=CIControlApp

[CLS:CIControlDoc]
Type=0
HeaderFile=iControlDoc.h
ImplementationFile=iControlDoc.cpp
Filter=N
LastObject=CIControlDoc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CIControlView]
Type=0
HeaderFile=iControlView.h
ImplementationFile=iControlView.cpp
Filter=C
LastObject=CIControlView
BaseClass=CView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_APP_ABOUT




[CLS:CAboutDlg]
Type=0
HeaderFile=iControl.cpp
ImplementationFile=iControl.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC_A_CAPTION,static,1342308480
Control2=IDC_STATIC_A_COMPANY,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_EDIT_A_LISENCE,edit,1350631556

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_APP_ABOUT
CommandCount=13

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[CLS:CRealTimeView]
Type=0
HeaderFile=RealTimeView.h
ImplementationFile=RealTimeView.cpp
BaseClass=CView
Filter=C
LastObject=CRealTimeView
VirtualFilter=VWC

[CLS:CCurCurveView]
Type=0
HeaderFile=CurCurveView.h
ImplementationFile=CurCurveView.cpp
BaseClass=CView
Filter=C
VirtualFilter=VWC
LastObject=CCurCurveView

[CLS:CHisGridView]
Type=0
HeaderFile=HisGridView.h
ImplementationFile=HisGridView.cpp
BaseClass=CView
Filter=C
VirtualFilter=VWC
LastObject=CHisGridView

[CLS:CHisCurveView]
Type=0
HeaderFile=HisCurveView.h
ImplementationFile=HisCurveView.cpp
BaseClass=CView
Filter=C
VirtualFilter=VWC
LastObject=CHisCurveView

[CLS:CDataBase]
Type=0
HeaderFile=DataBase.h
ImplementationFile=DataBase.cpp
BaseClass=generic CWnd
Filter=W

[CLS:CSpliteFrame]
Type=0
HeaderFile=SpliteFrame.h
ImplementationFile=SpliteFrame.cpp
BaseClass=CFrameWnd
Filter=T
LastObject=CSpliteFrame
VirtualFilter=fWC

[CLS:CMySplitterWnd]
Type=0
HeaderFile=MySplitterWnd.h
ImplementationFile=MySplitterWnd.cpp
BaseClass=splitter
Filter=T

[CLS:CHisSGridView]
Type=0
HeaderFile=HisSGridView.h
ImplementationFile=HisSGridView.cpp
BaseClass=CView
Filter=C

[DLG:IDD_LOGIN_DIALOG]
Type=1
Class=CLoginDialog
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_COUNT,edit,1350631552
Control4=IDC_EDIT_PWS,edit,1350631584
Control5=IDC_STATIC_COUNT,static,1342308352
Control6=IDC_STATIC_PWD,static,1342308352
Control7=IDC_STATIC_CAPTION,static,1342308353

[CLS:CLoginDialog]
Type=0
HeaderFile=LoginDialog.h
ImplementationFile=LoginDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CLoginDialog
VirtualFilter=dWC

[CLS:CStandardView]
Type=0
HeaderFile=StandardView.h
ImplementationFile=StandardView.cpp
BaseClass=CView
Filter=C
VirtualFilter=VWC
LastObject=CStandardView

[DLG:IDD_SERIAL_DIALOG]
Type=1
Class=CSerialDlg
ControlCount=30
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_NUM_COMBO,combobox,1344339971
Control8=IDC_BAUD_COMBO,combobox,1344339971
Control9=IDC_DATABITS_COMBO,combobox,1344339971
Control10=IDC_PARITY_COMBO,combobox,1344339971
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STOPBITS_COMBO,combobox,1344339971
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_TIMEOUT_EDIT,edit,1350631552
Control16=IDC_SCANTIME_EDIT,edit,1350631552
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_GROSS_EDIT,edit,1350631552
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_BELL_EDIT,edit,1350631552
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_DTU_NUM_COMBO,combobox,1344339971
Control28=IDC_DTU_BAUD_COMBO,combobox,1344339971
Control29=IDC_STATIC,static,1342308352
Control30=IDC_DTUPORTID_EDIT,edit,1350631552

[CLS:CSerialDlg]
Type=0
HeaderFile=SerialDlg.h
ImplementationFile=SerialDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSerialDlg

[CLS:CIGridCtrl]
Type=0
HeaderFile=IGridCtrl.h
ImplementationFile=IGridCtrl.cpp
BaseClass=generic CWnd
Filter=W

[CLS:CModBus]
Type=0
HeaderFile=ModBus.h
ImplementationFile=ModBus.cpp
BaseClass=generic CWnd
Filter=W

[CLS:CGrossView]
Type=0
HeaderFile=GrossView.h
ImplementationFile=GrossView.cpp
BaseClass=CView
Filter=C
VirtualFilter=VWC
LastObject=CGrossView

[DLG:IDD_ADD_GROSS]
Type=1
Class=CAddGrossDialog
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ROAST_COMBO,combobox,1344340226
Control4=IDC_HEAT_EDIT,edit,1350631552
Control5=IDC_ROAST_STATIC,static,1342308352
Control6=IDC_HEAT_STATIC,static,1342308352
Control7=IDC_CAPTION_STATIC,static,1342308353

[CLS:CAddGrossDialog]
Type=0
HeaderFile=AddGrossDialog.h
ImplementationFile=AddGrossDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[CLS:CSetupView]
Type=0
HeaderFile=SetupView.h
ImplementationFile=SetupView.cpp
BaseClass=CView
Filter=C
VirtualFilter=VWC
LastObject=CSetupView

[DLG:IDD_REGISTER_DIALOG]
Type=1
Class=CRegisterDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_HEAD_STATIC,static,1342308352
Control4=IDC_A_EDIT,edit,1350633601
Control5=IDC_CODE_STATIC,static,1342308352
Control6=IDC_E_EDIT,edit,1350631553
Control7=IDC_DAY_EDIT,edit,1350631425
Control8=IDC_CODE_STATIC2,static,1342308352

[CLS:CRegisterDlg]
Type=0
HeaderFile=RegisterDlg.h
ImplementationFile=RegisterDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CRegisterDlg
VirtualFilter=dWC

[DLG:IDD_MOTOR_DIALOG]
Type=1
Class=CMotorCtrlDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDNEXT,button,1342242816

[CLS:CMotorCtrlDlg]
Type=0
HeaderFile=MotorCtrlDlg.h
ImplementationFile=MotorCtrlDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMotorCtrlDlg
VirtualFilter=dWC

[CLS:CAdjustView]
Type=0
HeaderFile=AdjustView.h
ImplementationFile=AdjustView.cpp
BaseClass=CView
Filter=C
LastObject=CAdjustView
VirtualFilter=VWC

[DLG:IDD_GROSS_INFO]
Type=1
Class=CGrossInfo
ControlCount=1
Control1=IDC_INFO_STRING,static,1342308352

[CLS:CGrossInfo]
Type=0
HeaderFile=GrossInfo.h
ImplementationFile=GrossInfo.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CGrossInfo

[DLG:IDD_AUTOCTRL]
Type=1
Class=CAutoCtrlDlg
ControlCount=38
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COMBO_SEL_KILN,combobox,1344340226
Control4=IDC_EDIT_UP_LIMIT,edit,1350631552
Control5=IDC_EDIT_DN_LIMIT,edit,1350631552
Control6=IDC_CHECK_BCTRL,button,1342242819
Control7=IDC_EDIT_FRONT_PTS,edit,1350631552
Control8=IDC_EDIT_REAR_PTS,edit,1350631552
Control9=IDC_EDIT_CTRL_TIME,edit,1350631552
Control10=IDC_EDIT_INC_COEFF,edit,1350631552
Control11=IDC_EDIT_WAIT_TIME,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_EDIT_PROC_TIME,edit,1350631552
Control28=IDC_STATIC,static,1342308352
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_EDIT_PROC_MAX_TIME,edit,1350631552
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352
Control35=IDC_EDIT_UP_WARN,edit,1350631552
Control36=IDC_EDIT_DN_WARN,edit,1350631552
Control37=IDC_STATIC,static,1342308352
Control38=IDC_STATIC,static,1342308352

[CLS:CAutoCtrlDlg]
Type=0
HeaderFile=AutoCtrlDlg.h
ImplementationFile=AutoCtrlDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CAutoCtrlDlg

[CLS:CAutoCtrlView]
Type=0
HeaderFile=AutoCtrlView.h
ImplementationFile=AutoCtrlView.cpp
BaseClass=CView
Filter=C
LastObject=CAutoCtrlView
VirtualFilter=VWC

[CLS:CTimeQueryDlg]
Type=0
HeaderFile=TimeQueryDlg.h
ImplementationFile=TimeQueryDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDCANCEL

[DLG:IDD_QUERY]
Type=1
Class=CTimeQueryDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_DATETIMEPICKER1,SysDateTimePick32,1342242816
Control4=IDC_EDIT_HOUR,edit,1350631553
Control5=IDC_EDIT_MIN,edit,1350631553
Control6=IDC_STATIC,static,1342308352

[CLS:CDtuBus]
Type=0
HeaderFile=\[项目资料]\[砖瓦窑系统]\上位机软件\SCADA\icom\dtubus.h
ImplementationFile=\[项目资料]\[砖瓦窑系统]\上位机软件\SCADA\icom\dtubus.cpp
BaseClass=CWnd
LastObject=CDtuBus
Filter=W
VirtualFilter=WC


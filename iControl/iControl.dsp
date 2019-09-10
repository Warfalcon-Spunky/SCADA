# Microsoft Developer Studio Project File - Name="iControl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=iControl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "iControl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "iControl.mak" CFG="iControl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "iControl - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "iControl - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "iControl - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ../Lib/LKCom.lib lib/sqlite3.lib lib/GdiPlus.lib lib/Register.lib /nologo /subsystem:windows /machine:I386 /out:"../Run/iControl.exe"

!ELSEIF  "$(CFG)" == "iControl - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../Lib_d/LKComD.lib lib/sqlite3.lib lib/GdiPlus.lib lib/Register.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Run_d/iControl.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "iControl - Win32 Release"
# Name "iControl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "GridCtrl Filse"

# PROP Default_Filter ""
# Begin Group "New Cell Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NewCellTypes\GridCellCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellDateTime.cpp
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellNumeric.cpp
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridURLCell.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\GridCtrl_src\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\TitleTip.cpp
# End Source File
# End Group
# Begin Group "SpliteFrame Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HisCurveView.cpp
# End Source File
# Begin Source File

SOURCE=.\HisGridView.cpp
# End Source File
# Begin Source File

SOURCE=.\MySplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SpliteFrame.cpp
# End Source File
# End Group
# Begin Group "RealTime Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CurCurveView.cpp
# End Source File
# Begin Source File

SOURCE=.\Curve.cpp
# End Source File
# Begin Source File

SOURCE=.\RealTimeView.cpp
# End Source File
# End Group
# Begin Group "Setting Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AdjustView.cpp
# End Source File
# Begin Source File

SOURCE=.\IGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StandardView.cpp
# End Source File
# End Group
# Begin Group "Gross Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GrossView.cpp
# End Source File
# End Group
# Begin Group "Dialog Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AutoCtrlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GrossInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MotorCtrlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RegisterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeQueryDlg.cpp
# End Source File
# End Group
# Begin Group "Setup Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SetupView.cpp
# End Source File
# End Group
# Begin Group "ActiveX Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\iangulargaugex.cpp
# End Source File
# Begin Source File

SOURCE=.\iledrectanglex.cpp
# End Source File
# Begin Source File

SOURCE=.\isevensegmentanalogx.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\AutoCtrlView.cpp
# End Source File
# Begin Source File

SOURCE=.\HisTbl.cpp
# End Source File
# Begin Source File

SOURCE=.\iControl.cpp
# End Source File
# Begin Source File

SOURCE=.\iControl.rc
# End Source File
# Begin Source File

SOURCE=.\iControlDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\iControlView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLiteTbl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "GridCtrl Headers Files"

# PROP Default_Filter ""
# Begin Group "New Cell Headers Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NewCellTypes\GridCellCheck.h
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellCombo.h
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellDateTime.h
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellNumeric.h
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridURLCell.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\GridCtrl_src\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCell.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\TitleTip.h
# End Source File
# End Group
# Begin Group "SpliteFrame Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HisCurveView.h
# End Source File
# Begin Source File

SOURCE=.\HisGridView.h
# End Source File
# Begin Source File

SOURCE=.\MySplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\SpliteFrame.h
# End Source File
# End Group
# Begin Group "RealTime Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CurCurveView.h
# End Source File
# Begin Source File

SOURCE=.\Curve.h
# End Source File
# Begin Source File

SOURCE=.\RealTimeView.h
# End Source File
# End Group
# Begin Group "Setting Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AdjustView.h
# End Source File
# Begin Source File

SOURCE=.\IGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StandardView.h
# End Source File
# End Group
# Begin Group "Gross Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GrossView.h
# End Source File
# End Group
# Begin Group "Dialog Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AutoCtrlDlg.h
# End Source File
# Begin Source File

SOURCE=.\GrossInfo.h
# End Source File
# Begin Source File

SOURCE=.\LoginDialog.h
# End Source File
# Begin Source File

SOURCE=.\MotorCtrlDlg.h
# End Source File
# Begin Source File

SOURCE=.\RegisterDlg.h
# End Source File
# Begin Source File

SOURCE=.\SerialDlg.h
# End Source File
# Begin Source File

SOURCE=.\TimeQueryDlg.h
# End Source File
# End Group
# Begin Group "Setup Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SetupView.h
# End Source File
# End Group
# Begin Group "ActiveX Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\iangulargaugex.h
# End Source File
# Begin Source File

SOURCE=.\iledrectanglex.h
# End Source File
# Begin Source File

SOURCE=.\isevensegmentanalogx.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AutoCtrlView.h
# End Source File
# Begin Source File

SOURCE=.\HisTbl.h
# End Source File
# Begin Source File

SOURCE=.\iControl.h
# End Source File
# Begin Source File

SOURCE=.\iControlDoc.h
# End Source File
# Begin Source File

SOURCE=.\iControlView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\sqlite3.h
# End Source File
# Begin Source File

SOURCE=.\sqliteInt.h
# End Source File
# Begin Source File

SOURCE=.\SQLiteTbl.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bookmarks_list_add.ico
# End Source File
# Begin Source File

SOURCE=.\res\db_update.ico
# End Source File
# Begin Source File

SOURCE=.\res\edit_group.ico
# End Source File
# Begin Source File

SOURCE=.\res\exec.ico
# End Source File
# Begin Source File

SOURCE=.\res\find.ico
# End Source File
# Begin Source File

SOURCE=.\res\help.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\iControl.ico
# End Source File
# Begin Source File

SOURCE=.\res\iControl.rc2
# End Source File
# Begin Source File

SOURCE=.\res\iControlDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\kopeteavailable.ico
# End Source File
# Begin Source File

SOURCE=.\res\ksysguard.ico
# End Source File
# Begin Source File

SOURCE=.\res\Login.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Magic_Bus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month.ico
# End Source File
# Begin Source File

SOURCE=.\res\reload.ico
# End Source File
# Begin Source File

SOURCE=.\res\spellcheck.ico
# End Source File
# Begin Source File

SOURCE=.\res\stop.ico
# End Source File
# Begin Source File

SOURCE=.\res\terminal.ico
# End Source File
# Begin Source File

SOURCE=.\res\thumbnail.ico
# End Source File
# Begin Source File

SOURCE=.\res\todo.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\warn1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

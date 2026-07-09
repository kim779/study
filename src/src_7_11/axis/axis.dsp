# Microsoft Developer Studio Project File - Name="axis" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=axis - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axis.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axis.mak" CFG="axis - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axis - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "axis - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/fonix/axis", DRDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axis - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "WIN32" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ../dll/axissm/Release/axissm.lib Imm32.lib ../control/fx_gridEX/Release/fx_gridEX.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../release/axis.exe"
# SUBTRACT LINK32 /incremental:yes /map /nodefaultlib

!ELSEIF  "$(CFG)" == "axis - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /D /Gz"WIN32" /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../dll/axissm/Debug/axissm.lib Imm32.lib ../control/fx_gridEX/Debug/fx_gridEX.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../exe/axis.exe" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no /map /nodefaultlib

!ENDIF 

# Begin Target

# Name "axis - Win32 Release"
# Name "axis - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AccnAdminDLG.cpp
# End Source File
# Begin Source File

SOURCE=.\AccnInterestDLG.cpp
# End Source File
# Begin Source File

SOURCE=.\AccnTreeDLG.cpp
# End Source File
# Begin Source File

SOURCE=.\AccSave.cpp
# End Source File
# Begin Source File

SOURCE=.\Alarm.cpp
# End Source File
# Begin Source File

SOURCE=.\axInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\axis.cpp
# End Source File
# Begin Source File

SOURCE=.\axis.rc
# End Source File
# Begin Source File

SOURCE=.\axisDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\axisView.cpp
# End Source File
# Begin Source File

SOURCE=.\axMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\axScrollView.cpp
# End Source File
# Begin Source File

SOURCE=.\AxTabView.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseMP.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\button.cpp
# End Source File
# Begin Source File

SOURCE=.\Chaser.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ConclusionDLG.cpp
# End Source File
# Begin Source File

SOURCE=.\configDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\connect.cpp
# End Source File
# Begin Source File

SOURCE=.\UnhandledException\ExceptionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\ExitD.cpp
# End Source File
# Begin Source File

SOURCE=.\fname.cpp
# End Source File
# Begin Source File

SOURCE=.\fxGridEx.cpp
# End Source File
# Begin Source File

SOURCE=.\GPop.cpp
# End Source File
# Begin Source File

SOURCE=.\GuideMsgDLG.cpp
# End Source File
# Begin Source File

SOURCE=.\HogaFilterDLG.cpp
# End Source File
# Begin Source File

SOURCE=.\HotKeyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\idispimp.cpp
# End Source File
# Begin Source File

SOURCE=.\InputIP.cpp
# End Source File
# Begin Source File

SOURCE=.\InvalidID.cpp
# End Source File
# Begin Source File

SOURCE=.\Linkbar.cpp
# End Source File
# Begin Source File

SOURCE=.\LockDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ManageInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MPop.cpp
# End Source File
# Begin Source File

SOURCE=.\MTable.cpp
# End Source File
# Begin Source File

SOURCE=.\NClock.cpp
# End Source File
# Begin Source File

SOURCE=.\NDib.cpp
# End Source File
# Begin Source File

SOURCE=.\NExpand.cpp
# End Source File
# Begin Source File

SOURCE=.\NPalette.cpp
# End Source File
# Begin Source File

SOURCE=.\NProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\NTab.cpp
# End Source File
# Begin Source File

SOURCE=.\NTable.cpp
# End Source File
# Begin Source File

SOURCE=.\passdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PinchDLG.cpp
# End Source File
# Begin Source File

SOURCE=.\PrinterSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RSCinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SChild.cpp
# End Source File
# Begin Source File

SOURCE=.\screenDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetBufferDLg.cpp
# End Source File
# Begin Source File

SOURCE=.\Setup.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TTip.cpp
# End Source File
# Begin Source File

SOURCE=.\XCaption.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccnAdminDLG.h
# End Source File
# Begin Source File

SOURCE=.\AccnInterestDLG.h
# End Source File
# Begin Source File

SOURCE=.\AccnTreeDLG.h
# End Source File
# Begin Source File

SOURCE=.\AccSave.h
# End Source File
# Begin Source File

SOURCE=.\Alarm.h
# End Source File
# Begin Source File

SOURCE=.\axInstance.h
# End Source File
# Begin Source File

SOURCE=.\axis.h
# End Source File
# Begin Source File

SOURCE=.\axisDoc.h
# End Source File
# Begin Source File

SOURCE=.\axisView.h
# End Source File
# Begin Source File

SOURCE=.\axMisc.h
# End Source File
# Begin Source File

SOURCE=.\axScrollView.h
# End Source File
# Begin Source File

SOURCE=.\AxTabView.h
# End Source File
# Begin Source File

SOURCE=.\BaseMP.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\button.h
# End Source File
# Begin Source File

SOURCE=.\Chaser.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ConclusionDLG.h
# End Source File
# Begin Source File

SOURCE=.\configDlg.h
# End Source File
# Begin Source File

SOURCE=.\connect.h
# End Source File
# Begin Source File

SOURCE=.\UnhandledException\ExceptionHandler.h
# End Source File
# Begin Source File

SOURCE=.\ExitD.h
# End Source File
# Begin Source File

SOURCE=.\fname.h
# End Source File
# Begin Source File

SOURCE=.\fxGridEx.h
# End Source File
# Begin Source File

SOURCE=.\GPop.h
# End Source File
# Begin Source File

SOURCE=.\GuideMsgDLG.h
# End Source File
# Begin Source File

SOURCE=.\HogaFilterDLG.h
# End Source File
# Begin Source File

SOURCE=.\HotKeyDlg.h
# End Source File
# Begin Source File

SOURCE=.\idispimp.h
# End Source File
# Begin Source File

SOURCE=.\InputIP.h
# End Source File
# Begin Source File

SOURCE=.\InvalidID.h
# End Source File
# Begin Source File

SOURCE=.\Linkbar.h
# End Source File
# Begin Source File

SOURCE=.\LockDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ManageInfo.h
# End Source File
# Begin Source File

SOURCE=.\miscDef.h
# End Source File
# Begin Source File

SOURCE=.\MPop.h
# End Source File
# Begin Source File

SOURCE=.\MTable.h
# End Source File
# Begin Source File

SOURCE=.\MyToolTip.h
# End Source File
# Begin Source File

SOURCE=.\NClock.h
# End Source File
# Begin Source File

SOURCE=.\NDib.h
# End Source File
# Begin Source File

SOURCE=.\NExpand.h
# End Source File
# Begin Source File

SOURCE=.\NPalette.h
# End Source File
# Begin Source File

SOURCE=.\NProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\NSocket.h
# End Source File
# Begin Source File

SOURCE=.\NTab.h
# End Source File
# Begin Source File

SOURCE=.\NTable.h
# End Source File
# Begin Source File

SOURCE=.\passdlg.h
# End Source File
# Begin Source File

SOURCE=.\PinchDLG.h
# End Source File
# Begin Source File

SOURCE=.\PrinterSettings.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RSCinfo.h
# End Source File
# Begin Source File

SOURCE=.\SChild.h
# End Source File
# Begin Source File

SOURCE=.\screenDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetBufferDLg.h
# End Source File
# Begin Source File

SOURCE=.\Setup.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TTip.h
# End Source File
# Begin Source File

SOURCE=.\XCaption.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\1.BMP
# End Source File
# Begin Source File

SOURCE=.\res\1_.BMP
# End Source File
# Begin Source File

SOURCE=.\res\2.BMP
# End Source File
# Begin Source File

SOURCE=.\res\2_.BMP
# End Source File
# Begin Source File

SOURCE=.\res\3.BMP
# End Source File
# Begin Source File

SOURCE=.\res\3_.BMP
# End Source File
# Begin Source File

SOURCE=.\res\4.BMP
# End Source File
# Begin Source File

SOURCE=.\res\4_.BMP
# End Source File
# Begin Source File

SOURCE=.\res\5.BMP
# End Source File
# Begin Source File

SOURCE=.\res\5_.BMP
# End Source File
# Begin Source File

SOURCE=.\res\a.BMP
# End Source File
# Begin Source File

SOURCE=.\res\a_.BMP
# End Source File
# Begin Source File

SOURCE=.\res\alone.bmp
# End Source File
# Begin Source File

SOURCE=.\res\arrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\axis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\axis.ico
# End Source File
# Begin Source File

SOURCE=.\res\axis.rc2
# End Source File
# Begin Source File

SOURCE=.\res\axisDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\checkboxes.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cHelp_off.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cHelp_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ck_off.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ck_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\client.ico
# End Source File
# Begin Source File

SOURCE=.\res\close.bmp
# End Source File
# Begin Source File

SOURCE=.\res\copy.bmp
# End Source File
# Begin Source File

SOURCE=.\res\copy_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\DAETOO.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DPROGRESS.BMP
# End Source File
# Begin Source File

SOURCE=.\res\dticon.ico
# End Source File
# Begin Source File

SOURCE=.\res\dual.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dual_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\font.bmp
# End Source File
# Begin Source File

SOURCE=.\res\font_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\group.bmp
# End Source File
# Begin Source File

SOURCE=.\res\group_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HANA.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HAXIS.ico
# End Source File
# Begin Source File

SOURCE=.\res\HCLIENT.ico
# End Source File
# Begin Source File

SOURCE=.\res\help.bmp
# End Source File
# Begin Source File

SOURCE=.\res\help_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HEXITDLG.BMP
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_axis.ico
# End Source File
# Begin Source File

SOURCE=.\res\INET.bmp
# End Source File
# Begin Source File

SOURCE=.\res\inet_chk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\jumun_off.bmp
# End Source File
# Begin Source File

SOURCE=.\res\jumun_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ListCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login_position.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo_Mask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\m1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\m2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\m3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainfunc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\max.bmp
# End Source File
# Begin Source File

SOURCE=.\res\min.bmp
# End Source File
# Begin Source File

SOURCE=.\res\n.BMP
# End Source File
# Begin Source File

SOURCE=.\res\n_.BMP
# End Source File
# Begin Source File

SOURCE=.\res\newcustomer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\noticeskin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\out_position.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pin_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pro_position.bmp
# End Source File
# Begin Source File

SOURCE=.\res\progress.bmp
# End Source File
# Begin Source File

SOURCE=.\res\register_off.bmp
# End Source File
# Begin Source File

SOURCE=.\res\register_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\restore.BMP
# End Source File
# Begin Source File

SOURCE=.\res\restore_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\setup.ico
# End Source File
# Begin Source File

SOURCE=.\res\setup_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\setup_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\setup_o.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SetupImage.bmp
# End Source File
# Begin Source File

SOURCE=.\res\single.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TCP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tcp_chk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\timer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\top.bmp
# End Source File
# Begin Source File

SOURCE=.\res\top_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treeimag.bmp
# End Source File
# Begin Source File

SOURCE=.\res\vscreen.bmp
# End Source File
# Begin Source File

SOURCE=".\res\로그아웃.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\접속종료.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\종료화면.bmp"
# End Source File
# End Group
# Begin Group "Ctrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\dll\ctrl\MemDC.h
# End Source File
# Begin Source File

SOURCE=..\dll\ctrl\XHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\dll\ctrl\XHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=..\dll\ctrl\XListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\dll\ctrl\XListCtrl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section axis : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
# Section axis : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section

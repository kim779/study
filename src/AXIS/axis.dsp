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
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
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
# ADD CPP /nologo /MD /w /W0 /GX /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "WIN32" /FR /Yu"stdafx.h" /FD /MAPINFO:LINES /MAPINFO:EXPORTS /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ../dll/axiscm/Release/axiscm.lib ../dll/sm/Release/axissm.lib Imm32.lib ../dll/axiscp/Release/axiscp.lib msimg32.lib wininet.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"../../release/axis.exe"
# SUBTRACT LINK32 /pdb:none /incremental:yes

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
# ADD LINK32 ../dll/axiscm/Debug/axiscm.lib ../dll/sm/Debug/axissm.lib Imm32.lib ../dll/axiscp/Debug/axiscp.lib Msimg32.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../exe/axis.exe" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no /map

!ENDIF 

# Begin Target

# Name "axis - Win32 Release"
# Name "axis - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Helper"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GDIHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\MapHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\MDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceHelper.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\AccountConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\AccSave.cpp
# End Source File
# Begin Source File

SOURCE=.\AccTool.cpp
# End Source File
# Begin Source File

SOURCE=.\Alarm.cpp
# End Source File
# Begin Source File

SOURCE=.\APIHookEx.cpp
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

SOURCE=.\BkWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\button.cpp
# End Source File
# Begin Source File

SOURCE=.\CertErrDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CertLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\Chaser.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Conclusion.cpp
# End Source File
# Begin Source File

SOURCE=.\ContGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\CpuUse.cpp
# End Source File
# Begin Source File

SOURCE=.\custsite.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgMisuAlarm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPB.cpp
# End Source File
# Begin Source File

SOURCE=.\Dtconnect.cpp
# End Source File
# Begin Source File

SOURCE=.\dtInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DuplicateLoginConfirmDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EConclusion.cpp
# End Source File
# Begin Source File

SOURCE=.\EmergencyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EmpPassChangeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EmpPassChangeDlg.h
# End Source File
# Begin Source File

SOURCE=.\EmpPassChangeNotifyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EmpPassChangeNotifyDlg.h
# End Source File
# Begin Source File

SOURCE=.\UnhandledException\ExceptionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\ExitD.cpp
# End Source File
# Begin Source File

SOURCE=.\FirstJob.cpp
# End Source File
# Begin Source File

SOURCE=.\fname.cpp
# End Source File
# Begin Source File

SOURCE=.\GPop.cpp
# End Source File
# Begin Source File

SOURCE=.\HelpOK.cpp
# End Source File
# Begin Source File

SOURCE=.\idispimp.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageNotice.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\Info_Interest.cpp
# End Source File
# Begin Source File

SOURCE=.\Infofile.cpp
# End Source File
# Begin Source File

SOURCE=.\kings\KApi2Launcher.cpp
# End Source File
# Begin Source File

SOURCE=.\KobaElwNotify.cpp
# End Source File
# Begin Source File

SOURCE=.\LockDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LockPass.cpp
# End Source File
# Begin Source File

SOURCE=.\LogoDlg.cpp
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

SOURCE=..\appl\DH0000X1\MyToolTip.h
# End Source File
# Begin Source File

SOURCE=.\NClock.cpp
# End Source File
# Begin Source File

SOURCE=.\NDib.cpp
# End Source File
# Begin Source File

SOURCE=.\NewsViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\NExpand.cpp
# End Source File
# Begin Source File

SOURCE=.\NGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\NPalette.cpp
# End Source File
# Begin Source File

SOURCE=.\inca\npenkAppInstall5WIN.cpp
# End Source File
# Begin Source File

SOURCE=.\inca\NpnxMgr.cpp
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

SOURCE=.\PassEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\PhonePad.cpp
# End Source File
# Begin Source File

SOURCE=.\PrinterSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgramDlg.cpp
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

SOURCE=.\SecureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Setup.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupC.cpp
# End Source File
# Begin Source File

SOURCE=.\ShapeButton.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Stoploss.cpp
# End Source File
# Begin Source File

SOURCE=.\StopWarn.cpp
# End Source File
# Begin Source File

SOURCE=.\sysInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\TestInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TOP10Dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TOP10View.cpp
# End Source File
# Begin Source File

SOURCE=.\TTip.cpp
# End Source File
# Begin Source File

SOURCE=.\XCaption.cpp
# End Source File
# Begin Source File

SOURCE=.\XUnzip.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\XZip.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccountConfig.h
# End Source File
# Begin Source File

SOURCE=.\AccSave.h
# End Source File
# Begin Source File

SOURCE=.\AccTool.h
# End Source File
# Begin Source File

SOURCE=.\Alarm.h
# End Source File
# Begin Source File

SOURCE=.\APIHookEx.h
# End Source File
# Begin Source File

SOURCE=..\h\AxColor.h
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

SOURCE=.\BkWnd.h
# End Source File
# Begin Source File

SOURCE=.\BmpButton.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\button.h
# End Source File
# Begin Source File

SOURCE=.\CertErrDialog.h
# End Source File
# Begin Source File

SOURCE=.\CertLogin.h
# End Source File
# Begin Source File

SOURCE=.\Chaser.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\Conclusion.h
# End Source File
# Begin Source File

SOURCE=.\ContGrid.h
# End Source File
# Begin Source File

SOURCE=.\CpuUse.h
# End Source File
# Begin Source File

SOURCE=.\custsite.h
# End Source File
# Begin Source File

SOURCE=.\dlgMisuAlarm.h
# End Source File
# Begin Source File

SOURCE=.\DlgPB.h
# End Source File
# Begin Source File

SOURCE=.\Dtconnect.h
# End Source File
# Begin Source File

SOURCE=.\dtInfo.h
# End Source File
# Begin Source File

SOURCE=.\DupliacateLoginConfirmDlg.h
# End Source File
# Begin Source File

SOURCE=.\EConclusion.h
# End Source File
# Begin Source File

SOURCE=.\EmergencyDlg.h
# End Source File
# Begin Source File

SOURCE=.\Environment.h
# End Source File
# Begin Source File

SOURCE=.\UnhandledException\ExceptionHandler.h
# End Source File
# Begin Source File

SOURCE=.\ExitD.h
# End Source File
# Begin Source File

SOURCE=.\FirstJob.h
# End Source File
# Begin Source File

SOURCE=.\fname.h
# End Source File
# Begin Source File

SOURCE=.\GPop.h
# End Source File
# Begin Source File

SOURCE=.\Helper.h
# End Source File
# Begin Source File

SOURCE=.\HelpOK.h
# End Source File
# Begin Source File

SOURCE=.\idispimp.h
# End Source File
# Begin Source File

SOURCE=.\ImageNotice.h
# End Source File
# Begin Source File

SOURCE=.\ImgBtn.h
# End Source File
# Begin Source File

SOURCE=.\Info_Interest.h
# End Source File
# Begin Source File

SOURCE=.\Infofile.h
# End Source File
# Begin Source File

SOURCE=.\KobaElwNotify.h
# End Source File
# Begin Source File

SOURCE=.\Linkbar.h
# End Source File
# Begin Source File

SOURCE=.\LockDlg.h
# End Source File
# Begin Source File

SOURCE=.\LockPass.h
# End Source File
# Begin Source File

SOURCE=.\LogoDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ManageInfo.h
# End Source File
# Begin Source File

SOURCE=.\MapHelper.h
# End Source File
# Begin Source File

SOURCE=.\miscDef.h
# End Source File
# Begin Source File

SOURCE=.\MPop.h
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

SOURCE=.\NewsViewer.h
# End Source File
# Begin Source File

SOURCE=.\NExpand.h
# End Source File
# Begin Source File

SOURCE=.\NGrid.h
# End Source File
# Begin Source File

SOURCE=.\NPalette.h
# End Source File
# Begin Source File

SOURCE=.\inca\npenkAppInstall5WIN.h
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

SOURCE=.\PassEdit.h
# End Source File
# Begin Source File

SOURCE=.\perCounters.h
# End Source File
# Begin Source File

SOURCE=.\PhonePad.h
# End Source File
# Begin Source File

SOURCE=.\PrinterSettings.h
# End Source File
# Begin Source File

SOURCE=.\ProgramDlg.h
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

SOURCE=.\SecureDlg.h
# End Source File
# Begin Source File

SOURCE=.\Setup.h
# End Source File
# Begin Source File

SOURCE=.\SetupC.h
# End Source File
# Begin Source File

SOURCE=.\ShadeButton.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Stoploss.h
# End Source File
# Begin Source File

SOURCE=..\h\stopmsg.h
# End Source File
# Begin Source File

SOURCE=.\StopWarn.h
# End Source File
# Begin Source File

SOURCE=.\sysInfo.h
# End Source File
# Begin Source File

SOURCE=.\TestInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\TOP10Dialog.h
# End Source File
# Begin Source File

SOURCE=.\TOP10View.h
# End Source File
# Begin Source File

SOURCE=.\TTip.h
# End Source File
# Begin Source File

SOURCE=.\XCaption.h
# End Source File
# Begin Source File

SOURCE=.\XUnzip.h
# End Source File
# Begin Source File

SOURCE=.\XZip.h
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

SOURCE=.\res\_isscreen.bmp
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

SOURCE=.\res\bar1_side_bk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\check_off.bmp
# End Source File
# Begin Source File

SOURCE=.\res\check_on.bmp
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

SOURCE=.\res\CLOCK_AM.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLOCK_BG.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLOCK_BG_MINI.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLOCK_CALENDAR.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLOCK_CALENDAR_OVER.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLOCK_FRI.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLOCK_MON.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLOCK_PM.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLOCK_SAT.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLOCK_SUN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLOCK_THU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLOCK_TUE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLOCK_WED.bmp
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

SOURCE=.\res\DAXIS.BMP
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

SOURCE=.\res\EMPCHANGEPASS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\EMPCHANGEPASS2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\EVENT07.BMP
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

SOURCE=.\res\hot.ico
# End Source File
# Begin Source File

SOURCE=.\res\hott.ico
# End Source File
# Begin Source File

SOURCE=.\res\HTS_ICON.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HTS_ICON_CUS.bmp
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

SOURCE=.\res\isscreen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\isscreen2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\isscreen3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\isscreen4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\isscreen5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\jumun_off.bmp
# End Source File
# Begin Source File

SOURCE=.\res\jumun_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login_position.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo_ico1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo_ico2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LOGOUT_BG.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LOGOUT_BTN_CANCEL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LOGOUT_BTN_OK.bmp
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

SOURCE=.\res\menu_side_bk.bmp
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

SOURCE=.\res\NEW_LOGOUT_BTN_CANCEL.BMP
# End Source File
# Begin Source File

SOURCE=.\res\NEW_LOGOUT_BTN_OK.bmp
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

SOURCE=.\res\setup_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\setup_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\setup_o.bmp
# End Source File
# Begin Source File

SOURCE=.\res\single.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TCP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\test_popup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ticker1_side_bk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ticker2_side_lbk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ticker2_side_rbk.bmp
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

SOURCE=.\res\VERUP.BMP
# End Source File
# Begin Source File

SOURCE=.\res\vscreen.bmp
# End Source File
# Begin Source File

SOURCE=.\xptheme.bin
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
# Begin Group "grid"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\grid\DateTime.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\DateTime.h
# End Source File
# Begin Source File

SOURCE=.\grid\DropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\DropTarget.h
# End Source File
# Begin Source File

SOURCE=.\grid\excel9.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\excel9.h
# End Source File
# Begin Source File

SOURCE=.\grid\fxGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\fxGrid.h
# End Source File
# Begin Source File

SOURCE=.\grid\griditem.h
# End Source File
# Begin Source File

SOURCE=.\grid\InPlaceCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\InPlaceCombo.h
# End Source File
# Begin Source File

SOURCE=.\grid\InPlaceDate.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\InPlaceDate.h
# End Source File
# Begin Source File

SOURCE=.\grid\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\grid\memDc.h
# End Source File
# Begin Source File

SOURCE=.\grid\range.h
# End Source File
# Begin Source File

SOURCE=.\grid\resource.h
# End Source File
# Begin Source File

SOURCE=.\grid\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\TitleTip.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\request_admin.manifest
# End Source File
# Begin Source File

SOURCE=.\Trs5klib.lib
# End Source File
# End Target
# End Project
# Section axis : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
# Section axis : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section

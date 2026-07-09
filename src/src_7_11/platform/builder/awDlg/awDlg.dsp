# Microsoft Developer Studio Project File - Name="awDlg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=awDlg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "awDlg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "awDlg.mak" CFG="awDlg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "awDlg - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "awDlg - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "awDlg - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ../awSock/release/awsock.lib ../awBuild/release/awbuild.lib ../awWcc/release/awwcc.lib ../awCommon/release/awcommon.lib ../../dll/lib/release/axislib.lib ../awObject/release/awObject.lib ../awUser/release/awUser.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../release/awdlg.dll"

!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../awSock/debug/awsock.lib ../awBuild/debug/awbuild.lib ../awWcc/debug/awwcc.lib ../awCommon/debug/awcommon.lib ../../dll/lib/debug/axislib.lib ../awObject/debug/awObject.lib ../awUser/debug/awUser.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../exe/awdlg.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "awDlg - Win32 Release"
# Name "awDlg - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Components"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\BtnCombo.cpp
# End Source File
# Begin Source File

SOURCE=..\Components\EditItem.cpp
# End Source File
# Begin Source File

SOURCE=..\Components\EditList.cpp
# End Source File
# Begin Source File

SOURCE=..\Components\fxColorCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\HBBtn.cpp
# End Source File
# Begin Source File

SOURCE=..\Components\ItemCombo.cpp
# End Source File
# Begin Source File

SOURCE=..\ListGrid.cpp
# End Source File
# Begin Source File

SOURCE=..\NFBtn.cpp
# End Source File
# Begin Source File

SOURCE=..\Components\PopListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SepWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\XFontDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\xTreeCtrl.cpp
# End Source File
# End Group
# Begin Group "OptionsDlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BaseInfoPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DefaultPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptPage.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\addrDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\attrDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\awDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\awDlg.def
# End Source File
# Begin Source File

SOURCE=.\awDlg.rc
# End Source File
# Begin Source File

SOURCE=.\BATCH.CPP
# End Source File
# Begin Source File

SOURCE=.\cbItem.cpp
# End Source File
# Begin Source File

SOURCE=.\cellEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\colorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\coRGB.cpp
# End Source File
# Begin Source File

SOURCE=.\EditOrder.cpp
# End Source File
# Begin Source File

SOURCE=.\folderBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\format.cpp
# End Source File
# Begin Source File

SOURCE=.\GridColumnDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InputKeyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Login.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginSetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\mapOption.cpp
# End Source File
# Begin Source File

SOURCE=.\MGColumnDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Pane.cpp
# End Source File
# Begin Source File

SOURCE=.\Preview.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewRun.cpp
# End Source File
# Begin Source File

SOURCE=.\PublicSetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\saveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SingleProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\upDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\upTmplDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\uTabPage.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Components_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\BtnCombo.h
# End Source File
# Begin Source File

SOURCE=..\Components\ComboItem.h
# End Source File
# Begin Source File

SOURCE=..\Components\EditItem.h
# End Source File
# Begin Source File

SOURCE=..\Components\EditList.h
# End Source File
# Begin Source File

SOURCE=..\Components\fxColorCombo.h
# End Source File
# Begin Source File

SOURCE=.\HBBtn.h
# End Source File
# Begin Source File

SOURCE=..\Components\ItemCombo.h
# End Source File
# Begin Source File

SOURCE=..\ListGrid.h
# End Source File
# Begin Source File

SOURCE=..\NFBtn.h
# End Source File
# Begin Source File

SOURCE=..\Components\PopListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SepWnd.h
# End Source File
# Begin Source File

SOURCE=.\XFontDialog.h
# End Source File
# Begin Source File

SOURCE=.\XFontDialogRes.h
# End Source File
# Begin Source File

SOURCE=.\xTreeCtrl.h
# End Source File
# End Group
# Begin Group "OptionsDlg_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BaseInfoPage.h
# End Source File
# Begin Source File

SOURCE=.\DefaultPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsSheet.h
# End Source File
# Begin Source File

SOURCE=.\ScriptPage.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\addrDlg.h
# End Source File
# Begin Source File

SOURCE=.\attrDlg.h
# End Source File
# Begin Source File

SOURCE=.\BATCH.H
# End Source File
# Begin Source File

SOURCE=.\brushProp.h
# End Source File
# Begin Source File

SOURCE=.\cbItem.h
# End Source File
# Begin Source File

SOURCE=.\cellEdit.h
# End Source File
# Begin Source File

SOURCE=.\colorDlg.h
# End Source File
# Begin Source File

SOURCE=.\ControlDlg.h
# End Source File
# Begin Source File

SOURCE=.\coRGB.h
# End Source File
# Begin Source File

SOURCE=.\EditOrder.h
# End Source File
# Begin Source File

SOURCE=.\folderBrowser.h
# End Source File
# Begin Source File

SOURCE=.\format.h
# End Source File
# Begin Source File

SOURCE=.\GridColumnDlg.h
# End Source File
# Begin Source File

SOURCE=.\InputKeyDlg.h
# End Source File
# Begin Source File

SOURCE=.\LineDlg.h
# End Source File
# Begin Source File

SOURCE=.\lineProp.h
# End Source File
# Begin Source File

SOURCE=.\Login.h
# End Source File
# Begin Source File

SOURCE=.\LoginSetDlg.h
# End Source File
# Begin Source File

SOURCE=.\mapOption.h
# End Source File
# Begin Source File

SOURCE=.\MGColumnDlg.h
# End Source File
# Begin Source File

SOURCE=.\Pane.h
# End Source File
# Begin Source File

SOURCE=.\Preview.h
# End Source File
# Begin Source File

SOURCE=.\PreviewRun.h
# End Source File
# Begin Source File

SOURCE=.\PublicSetDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\saveDlg.h
# End Source File
# Begin Source File

SOURCE=.\SettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SingleProperty.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\upDlg.h
# End Source File
# Begin Source File

SOURCE=.\upTmplDlg.h
# End Source File
# Begin Source File

SOURCE=.\uTabPage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\awDlg.rc2
# End Source File
# Begin Source File

SOURCE=.\RES\axis_builder.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\axis_workstudio.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\cancelBtn_off.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\cancelBtn_on.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\close.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\close_dn.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\close_en.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\connect.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\connect_dn.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\connect_en.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\connectBtn_off.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\connectBtn_on.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\gold_cancel.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\gold_cancel_on.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\gold_connect.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\gold_connect_on.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\gold_setup.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\gold_setup_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graph.ico
# End Source File
# Begin Source File

SOURCE=.\res\listb.ico
# End Source File
# Begin Source File

SOURCE=.\res\normal.ico
# End Source File
# Begin Source File

SOURCE=.\RES\setup.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\setup_dn.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\setup_en.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\setupBtn_off.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\setupBtn_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\split01.ico
# End Source File
# Begin Source File

SOURCE=.\split01.ico
# End Source File
# Begin Source File

SOURCE=.\res\split02.ico
# End Source File
# Begin Source File

SOURCE=.\res\split03.ico
# End Source File
# Begin Source File

SOURCE=.\split03.ico
# End Source File
# Begin Source File

SOURCE=.\res\split04.ico
# End Source File
# Begin Source File

SOURCE=.\split04.ico
# End Source File
# Begin Source File

SOURCE=.\res\split05.ico
# End Source File
# Begin Source File

SOURCE=.\res\split06.ico
# End Source File
# Begin Source File

SOURCE=.\split06.ico
# End Source File
# Begin Source File

SOURCE=.\res\split07.ico
# End Source File
# Begin Source File

SOURCE=.\res\split08.ico
# End Source File
# Begin Source File

SOURCE=.\res\split09.ico
# End Source File
# Begin Source File

SOURCE=.\res\split10.ico
# End Source File
# Begin Source File

SOURCE=.\res\split11.ico
# End Source File
# Begin Source File

SOURCE=.\res\split12.ico
# End Source File
# Begin Source File

SOURCE=.\res\tab.ico
# End Source File
# Begin Source File

SOURCE=.\RES\tfdropcopy.cur
# End Source File
# Begin Source File

SOURCE=.\RES\tfnodropcopy.cur
# End Source File
# Begin Source File

SOURCE=.\RES\treeview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\userob.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

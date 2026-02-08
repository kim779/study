# Microsoft Developer Studio Project File - Name="axiscp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axiscp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axiscp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axiscp.mak" CFG="axiscp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axiscp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axiscp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axiscp - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 Imm32.lib winmm.lib ../sm/Release/axissm.lib ../axiscm/Release/axiscm.lib /nologo /base:"0x00500000" /subsystem:windows /dll /machine:I386 /def:".\axiscp.def" /out:"..\..\..\release\axiscp.dll"
# SUBTRACT LINK32 /pdb:none /map /debug

!ELSEIF  "$(CFG)" == "axiscp - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Imm32.lib ../sm/Debug/axissm.lib ../axiscm/Debug/axiscm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../exe\axiscp.dll" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no /nodefaultlib

!ENDIF 

# Begin Target

# Name "axiscp - Win32 Release"
# Name "axiscp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\1Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\axiscp.cpp
# End Source File
# Begin Source File

SOURCE=.\axiscp.def

!IF  "$(CFG)" == "axiscp - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "axiscp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\axiscp.rc
# End Source File
# Begin Source File

SOURCE=..\..\axis\BmpButton.cpp
# End Source File
# Begin Source File

SOURCE=.\EqualizerSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\esetup.cpp
# End Source File
# Begin Source File

SOURCE=.\Hotkey.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemRename.cpp
# End Source File
# Begin Source File

SOURCE=.\Link.cpp
# End Source File
# Begin Source File

SOURCE=.\MngSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenList.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTickInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ThemePreviewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\TotalSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\tSetup.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\1Picture.h
# End Source File
# Begin Source File

SOURCE=.\axGuide.h
# End Source File
# Begin Source File

SOURCE=.\Basebar0.h
# End Source File
# Begin Source File

SOURCE=.\Cal1.h
# End Source File
# Begin Source File

SOURCE=.\Cal2.h
# End Source File
# Begin Source File

SOURCE=.\Calculator.h
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\Dbar1.h
# End Source File
# Begin Source File

SOURCE=.\Dbar2.h
# End Source File
# Begin Source File

SOURCE=.\EqualizerSetup.h
# End Source File
# Begin Source File

SOURCE=.\ESetup.h
# End Source File
# Begin Source File

SOURCE=.\hBand.h
# End Source File
# Begin Source File

SOURCE=.\Hotkey.h
# End Source File
# Begin Source File

SOURCE=.\ImageSetup.h
# End Source File
# Begin Source File

SOURCE=.\iMenu.h
# End Source File
# Begin Source File

SOURCE=.\ItemRename.h
# End Source File
# Begin Source File

SOURCE=.\Link.h
# End Source File
# Begin Source File

SOURCE=.\MDIClient.h
# End Source File
# Begin Source File

SOURCE=.\MenuSearch.h
# End Source File
# Begin Source File

SOURCE=.\MenuXP.h
# End Source File
# Begin Source File

SOURCE=.\MngSetup.h
# End Source File
# Begin Source File

SOURCE=.\NButton.h
# End Source File
# Begin Source File

SOURCE=.\NPalette.h
# End Source File
# Begin Source File

SOURCE=.\OrderSetup.h
# End Source File
# Begin Source File

SOURCE=.\PalButton.h
# End Source File
# Begin Source File

SOURCE=.\Pane.h
# End Source File
# Begin Source File

SOURCE=.\PaneList.h
# End Source File
# Begin Source File

SOURCE=.\RegCode.h
# End Source File
# Begin Source File

SOURCE=.\RegMap.h
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RunX.h
# End Source File
# Begin Source File

SOURCE=.\ScreenList.h
# End Source File
# Begin Source File

SOURCE=.\SDIBand.h
# End Source File
# Begin Source File

SOURCE=.\Sdibar.h
# End Source File
# Begin Source File

SOURCE=.\SEdit.h
# End Source File
# Begin Source File

SOURCE=.\SetTickInfo.h
# End Source File
# Begin Source File

SOURCE=.\SimpleSplitter.h
# End Source File
# Begin Source File

SOURCE=.\SkinDlg.h
# End Source File
# Begin Source File

SOURCE=.\Smain.h
# End Source File
# Begin Source File

SOURCE=.\smdefine.h
# End Source File
# Begin Source File

SOURCE=.\smheader.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SVband.h
# End Source File
# Begin Source File

SOURCE=.\TInfo1.h
# End Source File
# Begin Source File

SOURCE=.\TInfo2.h
# End Source File
# Begin Source File

SOURCE=.\TMenu.h
# End Source File
# Begin Source File

SOURCE=.\ToolFunc.h
# End Source File
# Begin Source File

SOURCE=.\TotalSetup.h
# End Source File
# Begin Source File

SOURCE=.\TrSearch.h
# End Source File
# Begin Source File

SOURCE=.\TSetup.h
# End Source File
# Begin Source File

SOURCE=.\Xcom.h
# End Source File
# Begin Source File

SOURCE=.\Xcomex.h
# End Source File
# Begin Source File

SOURCE=.\Xedit.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\2019-04-11 16 20 46.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\actskin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\alist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\axiscp.rc2
# End Source File
# Begin Source File

SOURCE=.\res\bar1back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bar2back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bar2button.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cimg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cimg_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\comarrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Darr_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Darr_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\expand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\expand2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HARROW.CUR
# End Source File
# Begin Source File

SOURCE=.\res\infoback.bmp
# End Source File
# Begin Source File

SOURCE=.\res\itemTitle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\menubk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sclose.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sdiexpand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sdivscreen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SetupImage.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sexpand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sicon.ico
# End Source File
# Begin Source File

SOURCE=.\res\skin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\spin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sscreensave.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sswitch.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ticklist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbox.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolImageList.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treeicon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Uarr_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Uarr_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\vitualscreenimg.bmp
# End Source File
# End Group
# Begin Group "Ctrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ctrl\AxScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\AxScrollBar.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\bitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\bitmap.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\colorCombo.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\colorCombo.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExCombo.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExCombo.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExCommaEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExCommaEdit.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExDialog.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExEdit.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExTabCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\fxTab.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\fxTab.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\fxTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\fxTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\listctrlSort.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\listctrlSort.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\listEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\listEdit.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\listHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\listHeader.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\misctype.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\palette.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\palette.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\themeCombo.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\themeCombo.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section axiscp : {38F0EC10-E2B1-4980-AF7D-E240A8335395}
# 	1:8:IDD_CAL2:7166
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:8:Cal2.cpp:Cal2.cpp
# 	2:6:Cal2.h:Cal2.h
# 	2:19:Application Include:axiscp.h
# 	2:8:IDD_CAL2:IDD_CAL2
# 	2:12:CLASS: CCal2:CCal2
# End Section
# Section axiscp : {D5735979-C04D-489D-9774-CE48B56E7890}
# 	1:14:IDD_CALCULATOR:7109
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:14:IDD_CALCULATOR:IDD_CALCULATOR
# 	2:19:Application Include:axiscp.h
# 	2:18:CLASS: CCalculator:CCalculator
# 	2:14:Calculator.cpp:Calculator.cpp
# 	2:12:Calculator.h:Calculator.h
# End Section
# Section axiscp : {DEEDECF2-5894-4BD6-B68B-54D0AAA06DBA}
# 	1:14:IDD_SELECTSKIN:7069
# 	2:16:Resource Include:resource.h
# 	2:11:SkinDlg.cpp:SkinDlg.cpp
# 	2:9:SkinDlg.h:SkinDlg.h
# 	2:10:ENUM: enum:enum
# 	2:14:IDD_SELECTSKIN:IDD_SELECTSKIN
# 	2:15:CLASS: CSkinDlg:CSkinDlg
# 	2:19:Application Include:axiscp.h
# End Section
# Section axiscp : {408B3C61-7B3E-48BE-8C17-1DD66E900188}
# 	1:14:IDD_ITEMRENAME:7068
# 	2:16:Resource Include:resource.h
# 	2:12:ItemRename.h:ItemRename.h
# 	2:10:ENUM: enum:enum
# 	2:14:IDD_ITEMRENAME:IDD_ITEMRENAME
# 	2:14:ItemRename.cpp:ItemRename.cpp
# 	2:19:Application Include:axiscp.h
# 	2:18:CLASS: CItemRename:CItemRename
# End Section
# Section axiscp : {E2177CCA-8F1E-453D-8EC3-15C1CE45D2CE}
# 	1:8:IDD_CAL1:7142
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:8:Cal1.cpp:Cal1.cpp
# 	2:6:Cal1.h:Cal1.h
# 	2:8:IDD_CAL1:IDD_CAL1
# 	2:19:Application Include:axiscp.h
# 	2:12:CLASS: CCal1:CCal1
# End Section

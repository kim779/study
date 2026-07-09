# Microsoft Developer Studio Project File - Name="axissm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axissm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axissm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axissm.mak" CFG="axissm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axissm - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axissm - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/fonix/dll/axissm", QHEAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axissm - Win32 Release"

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
# ADD LINK32 Imm32.lib /nologo /base:"0x00500000" /subsystem:windows /dll /debug /machine:I386 /def:".\axissm.def" /out:"../../../Release/AXISSM.DLL"
# SUBTRACT LINK32 /pdb:none /map

!ELSEIF  "$(CFG)" == "axissm - Win32 Debug"

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
# ADD LINK32 Imm32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../exe/AXISSM.DLL" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no /nodefaultlib

!ENDIF 

# Begin Target

# Name "axissm - Win32 Release"
# Name "axissm - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\axButton.cpp
# End Source File
# Begin Source File

SOURCE=.\axGuide.cpp
# End Source File
# Begin Source File

SOURCE=.\axissm.cpp
# End Source File
# Begin Source File

SOURCE=.\axissm.def

!IF  "$(CFG)" == "axissm - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "axissm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\axissm.rc
# End Source File
# Begin Source File

SOURCE=.\axSB.cpp
# End Source File
# Begin Source File

SOURCE=.\axStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\axTicker.cpp
# End Source File
# Begin Source File

SOURCE=.\Bb.cpp
# End Source File
# Begin Source File

SOURCE=.\BCtrl1.cpp
# End Source File
# Begin Source File

SOURCE=.\BCtrl2.cpp
# End Source File
# Begin Source File

SOURCE=.\BCtrl3.cpp
# End Source File
# Begin Source File

SOURCE=.\Color.CPP
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Dbar0.cpp
# End Source File
# Begin Source File

SOURCE=.\Dbar1.cpp
# End Source File
# Begin Source File

SOURCE=.\Dbar2.cpp
# End Source File
# Begin Source File

SOURCE=.\esetup.cpp
# End Source File
# Begin Source File

SOURCE=.\fxSpin.cpp
# End Source File
# Begin Source File

SOURCE=.\hBand.cpp
# End Source File
# Begin Source File

SOURCE=.\Hotkey.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\iMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemRename.cpp
# End Source File
# Begin Source File

SOURCE=.\Link.cpp
# End Source File
# Begin Source File

SOURCE=.\MDIClient.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuXP.cpp
# End Source File
# Begin Source File

SOURCE=.\MngSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\NButton.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\PalButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Pane.cpp
# End Source File
# Begin Source File

SOURCE=.\PaneList.cpp
# End Source File
# Begin Source File

SOURCE=.\RegCode.cpp
# End Source File
# Begin Source File

SOURCE=.\RegMap.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RunX.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenList.cpp
# End Source File
# Begin Source File

SOURCE=.\SDIBand.cpp
# End Source File
# Begin Source File

SOURCE=.\Sdibar.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTickInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Smain.cpp
# End Source File
# Begin Source File

SOURCE=.\Smcall.cpp
# End Source File
# Begin Source File

SOURCE=.\SmSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SVband.cpp
# End Source File
# Begin Source File

SOURCE=.\TInfo1.cpp
# End Source File
# Begin Source File

SOURCE=.\TInfo2.cpp
# End Source File
# Begin Source File

SOURCE=.\TMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\TotalSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\TrSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\tSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\Xcom.cpp
# End Source File
# Begin Source File

SOURCE=.\Xcomex.cpp
# End Source File
# Begin Source File

SOURCE=.\Xedit.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\axButton.h
# End Source File
# Begin Source File

SOURCE=.\axGuide.h
# End Source File
# Begin Source File

SOURCE=.\axSB.h
# End Source File
# Begin Source File

SOURCE=.\axStatic.h
# End Source File
# Begin Source File

SOURCE=.\axTicker.h
# End Source File
# Begin Source File

SOURCE=.\Bb.h
# End Source File
# Begin Source File

SOURCE=.\BCtrl1.h
# End Source File
# Begin Source File

SOURCE=.\BCtrl2.h
# End Source File
# Begin Source File

SOURCE=.\BCtrl3.h
# End Source File
# Begin Source File

SOURCE=.\color.h
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\Dbar0.h
# End Source File
# Begin Source File

SOURCE=.\Dbar1.h
# End Source File
# Begin Source File

SOURCE=.\Dbar2.h
# End Source File
# Begin Source File

SOURCE=.\ESetup.h
# End Source File
# Begin Source File

SOURCE=.\fxSpin.h
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

SOURCE=.\Smcall.h
# End Source File
# Begin Source File

SOURCE=.\smdefine.h
# End Source File
# Begin Source File

SOURCE=.\smheader.h
# End Source File
# Begin Source File

SOURCE=.\SmSheet.h
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

SOURCE=.\res\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\2_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\3_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\4_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\actskin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\alist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\axissm.rc2
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

SOURCE=.\res\reflesh_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\reflesh_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\reflesh_o.bmp
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
# Begin Source File

SOURCE=".\res\Ã·ºÎÆÄÀÏ.bmp"
# End Source File
# End Group
# Begin Group "Ctrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AxScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\AxScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\bitmap.h
# End Source File
# Begin Source File

SOURCE=.\colorCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\colorCombo.h
# End Source File
# Begin Source File

SOURCE=.\ExCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\ExCombo.h
# End Source File
# Begin Source File

SOURCE=.\ExDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ExDialog.h
# End Source File
# Begin Source File

SOURCE=.\ExListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ExListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ExTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ExTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\fxTab.cpp
# End Source File
# Begin Source File

SOURCE=.\fxTab.h
# End Source File
# Begin Source File

SOURCE=.\listctrlSort.cpp
# End Source File
# Begin Source File

SOURCE=.\listctrlSort.h
# End Source File
# Begin Source File

SOURCE=.\listEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\listEdit.h
# End Source File
# Begin Source File

SOURCE=.\listHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\listHeader.h
# End Source File
# Begin Source File

SOURCE=.\misctype.h
# End Source File
# Begin Source File

SOURCE=.\palette.cpp
# End Source File
# Begin Source File

SOURCE=.\palette.h
# End Source File
# Begin Source File

SOURCE=.\themeCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\themeCombo.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section axissm : {408B3C61-7B3E-48BE-8C17-1DD66E900188}
# 	1:14:IDD_ITEMRENAME:7068
# 	2:16:Resource Include:resource.h
# 	2:12:ItemRename.h:ItemRename.h
# 	2:10:ENUM: enum:enum
# 	2:14:IDD_ITEMRENAME:IDD_ITEMRENAME
# 	2:14:ItemRename.cpp:ItemRename.cpp
# 	2:19:Application Include:axissm.h
# 	2:18:CLASS: CItemRename:CItemRename
# End Section
# Section axissm : {DEEDECF2-5894-4BD6-B68B-54D0AAA06DBA}
# 	1:14:IDD_SELECTSKIN:7069
# 	2:16:Resource Include:resource.h
# 	2:11:SkinDlg.cpp:SkinDlg.cpp
# 	2:9:SkinDlg.h:SkinDlg.h
# 	2:10:ENUM: enum:enum
# 	2:14:IDD_SELECTSKIN:IDD_SELECTSKIN
# 	2:15:CLASS: CSkinDlg:CSkinDlg
# 	2:19:Application Include:axissm.h
# End Section

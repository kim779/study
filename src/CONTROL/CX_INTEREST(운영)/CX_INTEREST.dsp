# Microsoft Developer Studio Project File - Name="CX_INTEREST" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CX_INTEREST - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CX_INTEREST.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CX_INTEREST.mak" CFG="CX_INTEREST - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CX_INTEREST - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CX_INTEREST - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CX_INTEREST - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ../../control/fx_misc/release/fx_misc.lib imm32.lib winmm.lib ../../control/fx_gridex/release/fx_gridex.lib Msimg32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../dev/CX_INTEREST.dll"
# SUBTRACT LINK32 /profile /incremental:yes /map /debug

!ELSEIF  "$(CFG)" == "CX_INTEREST - Win32 Debug"

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
# ADD CPP /nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../../control/fx_misc/debug/fx_misc.lib imm32.lib winmm.lib ../../control/fx_gridex/debug/fx_gridex.lib Msimg32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /include:"__DllMainCRTStartup@12" /out:"../../../dev/CX_INTEREST.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "CX_INTEREST - Win32 Release"
# Name "CX_INTEREST - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Account.cpp
# End Source File
# Begin Source File

SOURCE=.\AlertSoundDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoSaveSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BasketWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnTab.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeDuplicate.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\CX_INTEREST.def
# End Source File
# Begin Source File

SOURCE=.\CX_INTEREST.odl
# End Source File
# Begin Source File

SOURCE=.\CX_INTEREST.rc
# End Source File
# Begin Source File

SOURCE=.\CX_INTERGRID.cpp
# End Source File
# Begin Source File

SOURCE=.\DateTime.cpp
# End Source File
# Begin Source File

SOURCE=.\DropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\excel9.cpp
# End Source File
# Begin Source File

SOURCE=.\GridWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\IHCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\IHCodeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\appl\IhUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceDate.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\InputPswd.cpp
# End Source File
# Begin Source File

SOURCE=.\InterDomino.cpp
# End Source File
# Begin Source File

SOURCE=.\intGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\libctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MainWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MapWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkerSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MarketColorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Memo.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\OptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RealCheckDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RTSGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\RTSWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\setup.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowMsgWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\treewnd.cpp
# End Source File
# Begin Source File

SOURCE=.\UsefulSetupDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Account.h
# End Source File
# Begin Source File

SOURCE=.\AlertSoundDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoSaveSetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\BaseWnd.h
# End Source File
# Begin Source File

SOURCE=.\BasketWnd.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\BtnTab.h
# End Source File
# Begin Source File

SOURCE=.\ButtonStyle.h
# End Source File
# Begin Source File

SOURCE=.\category.h
# End Source File
# Begin Source File

SOURCE=.\CheckBox.h
# End Source File
# Begin Source File

SOURCE=.\CodeDuplicate.h
# End Source File
# Begin Source File

SOURCE=.\ColorBtn.h
# End Source File
# Begin Source File

SOURCE=.\CX_INTERGRID.h
# End Source File
# Begin Source File

SOURCE=.\DateTime.h
# End Source File
# Begin Source File

SOURCE=.\define.h
# End Source File
# Begin Source File

SOURCE=.\DropTarget.h
# End Source File
# Begin Source File

SOURCE=.\excel9.h
# End Source File
# Begin Source File

SOURCE=.\exspin.h
# End Source File
# Begin Source File

SOURCE=.\fxbtn.h
# End Source File
# Begin Source File

SOURCE=.\fxButton.h
# End Source File
# Begin Source File

SOURCE=.\fxGridEx.h
# End Source File
# Begin Source File

SOURCE=.\fxSpin.h
# End Source File
# Begin Source File

SOURCE=.\griditem.h
# End Source File
# Begin Source File

SOURCE=.\GridWnd.h
# End Source File
# Begin Source File

SOURCE=.\GroupWnd.h
# End Source File
# Begin Source File

SOURCE=.\IHCheck.h
# End Source File
# Begin Source File

SOURCE=.\IHCodeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\appl\IhUtils.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceCombo.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceDate.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\InputPswd.h
# End Source File
# Begin Source File

SOURCE=.\InterDomino.h
# End Source File
# Begin Source File

SOURCE=.\intGrid.h
# End Source File
# Begin Source File

SOURCE=.\libctrl.h
# End Source File
# Begin Source File

SOURCE=.\MainWnd.h
# End Source File
# Begin Source File

SOURCE=.\MapWnd.h
# End Source File
# Begin Source File

SOURCE=.\MarkerSetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\MarkerSetupDlg_.h
# End Source File
# Begin Source File

SOURCE=.\MarketColorDlg.h
# End Source File
# Begin Source File

SOURCE=.\memDc.h
# End Source File
# Begin Source File

SOURCE=.\Memo.h
# End Source File
# Begin Source File

SOURCE=.\MoveSetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\MultiCheckListBox.h
# End Source File
# Begin Source File

SOURCE=.\NewMenu.h
# End Source File
# Begin Source File

SOURCE=.\OptDlg.h
# End Source File
# Begin Source File

SOURCE=.\PopupDlg.h
# End Source File
# Begin Source File

SOURCE=.\qsort.h
# End Source File
# Begin Source File

SOURCE=.\range.h
# End Source File
# Begin Source File

SOURCE=.\RealCheckDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RTSGrid.h
# End Source File
# Begin Source File

SOURCE=.\RTSWnd.h
# End Source File
# Begin Source File

SOURCE=.\setup.h
# End Source File
# Begin Source File

SOURCE=.\sharemsg.h
# End Source File
# Begin Source File

SOURCE=.\ShowMsgWnd.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TestDlg.h
# End Source File
# Begin Source File

SOURCE=.\TipDlg.h
# End Source File
# Begin Source File

SOURCE=.\TipWnd.h
# End Source File
# Begin Source File

SOURCE=.\ToolWnd.h
# End Source File
# Begin Source File

SOURCE=.\TreeWnd.h
# End Source File
# Begin Source File

SOURCE=.\UsefulSetupDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CX_INTEREST.rc2
# End Source File
# Begin Source File

SOURCE=.\res\size3_r.cur
# End Source File
# Begin Source File

SOURCE=.\res\size4_r.cur
# End Source File
# Begin Source File

SOURCE=.\res\tree1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree2.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

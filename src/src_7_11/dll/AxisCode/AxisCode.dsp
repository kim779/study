# Microsoft Developer Studio Project File - Name="AxisCode" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=AxisCode - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AxisCode.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AxisCode.mak" CFG="AxisCode - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AxisCode - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AxisCode - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/fonix/dll/AxisCode", GIDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AxisCode - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 imm32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../Release/AXISCODE.DLL"

!ELSEIF  "$(CFG)" == "AxisCode - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 imm32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../exe/AXISCODE.DLL" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "AxisCode - Win32 Release"
# Name "AxisCode - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AxisCode.cpp
# End Source File
# Begin Source File

SOURCE=.\AxisCode.def
# End Source File
# Begin Source File

SOURCE=.\AxisCode.rc
# End Source File
# Begin Source File

SOURCE=.\BondDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\codeInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\EditX.cpp
# End Source File
# Begin Source File

SOURCE=.\ElwDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FundDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FuturesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JongmukDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\K50Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\libdll.cpp
# End Source File
# Begin Source File

SOURCE=.\libSort.cpp
# End Source File
# Begin Source File

SOURCE=.\MyBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SFont.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StockNewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Tool.cpp
# End Source File
# Begin Source File

SOURCE=.\UPCodeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\USCodeDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AxisCode.h
# End Source File
# Begin Source File

SOURCE=.\BondDlg.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\CheckBox.h
# End Source File
# Begin Source File

SOURCE=.\codeInfo.h
# End Source File
# Begin Source File

SOURCE=.\EditX.h
# End Source File
# Begin Source File

SOURCE=.\ElwDlg.h
# End Source File
# Begin Source File

SOURCE=.\FundDlg.h
# End Source File
# Begin Source File

SOURCE=.\FuturesDlg.h
# End Source File
# Begin Source File

SOURCE=.\JongmukDlg.h
# End Source File
# Begin Source File

SOURCE=.\K50Dlg.h
# End Source File
# Begin Source File

SOURCE=.\libdll.h
# End Source File
# Begin Source File

SOURCE=.\libSort.h
# End Source File
# Begin Source File

SOURCE=.\MyBitmap.h
# End Source File
# Begin Source File

SOURCE=.\MyEdit.h
# End Source File
# Begin Source File

SOURCE=.\myuser.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SFont.h
# End Source File
# Begin Source File

SOURCE=.\SkinHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SkinListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StockNewDlg.h
# End Source File
# Begin Source File

SOURCE=.\Tool.h
# End Source File
# Begin Source File

SOURCE=.\UPCodeDlg.h
# End Source File
# Begin Source File

SOURCE=.\USCodeDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AxisCode.rc2
# End Source File
# Begin Source File

SOURCE=.\res\c1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\c2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ColumnHeaderEnd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ColumnHeaderSpan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ColumnHeaderStart.bmp
# End Source File
# Begin Source File

SOURCE=.\res\harrow.cur
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section AxisCode : {9617505D-A92C-4A25-B864-DA1DC457FC14}
# 	2:10:ENUM: enum:enum
# 	2:16:CLASS: CButtonST:CButtonST
# 	2:9:BtnST.cpp:BtnST.cpp
# 	2:19:Application Include:AxisCode.h
# 	2:7:BtnST.h:BtnST.h
# End Section
# Section AxisCode : {F3A59F33-9463-492A-975F-83372FE918AE}
# 	2:14:CLASS: CMyFont:CMyFont
# 	2:10:MyFont.cpp:MyFont.cpp
# 	2:8:MyFont.h:MyFont.h
# 	2:19:Application Include:AxisCode.h
# End Section
# Section AxisCode : {794FD5E2-EE75-4A8C-95E8-1F2F7A03BAB4}
# 	2:16:CLASS: CMyBitmap:CMyBitmap
# 	2:12:MyBitmap.cpp:MyBitmap.cpp
# 	2:10:MyBitmap.h:MyBitmap.h
# 	2:19:Application Include:AxisCode.h
# End Section
# Section AxisCode : {A7F282A2-9489-4E56-AEEA-65D93A6834D1}
# 	1:24:IDD_DIALOG_STOCKCODEINFO:24001
# 	2:17:TYPEDEF: LISTITEM:LISTITEM
# 	2:16:Resource Include:resource.h
# 	2:12:CLASS: favor:favor
# 	2:18:CLASS: CJongmukDlg:CJongmukDlg
# 	2:24:IDD_DIALOG_STOCKCODEINFO:IDD_DIALOG_STOCKCODEINFO
# 	2:14:JongmukDlg.cpp:JongmukDlg.cpp
# 	2:14:TYPEDEF: FLIST:FLIST
# 	2:16:CLASS: _listItem:_listItem
# 	2:10:ENUM: enum:enum
# 	2:12:CLASS: flist:flist
# 	2:12:CLASS: JCode:JCode
# 	2:14:TYPEDEF: FAVOR:FAVOR
# 	2:12:JongmukDlg.h:JongmukDlg.h
# 	2:19:Application Include:AxisCode.h
# End Section
# Section AxisCode : {9C80F249-CC10-44BF-9656-66D7AE840689}
# 	1:18:IDD_DIALOG_FUTURES:24000
# 	2:12:CLASS: PCode:PCode
# 	2:16:Resource Include:resource.h
# 	2:14:TYPEDEF: FCODE:FCODE
# 	2:12:FuturesDlg.h:FuturesDlg.h
# 	2:12:CLASS: FCode:FCode
# 	2:10:ENUM: enum:enum
# 	2:14:TYPEDEF: PCODE:PCODE
# 	2:18:CLASS: CFuturesDlg:CFuturesDlg
# 	2:18:IDD_DIALOG_FUTURES:IDD_DIALOG_FUTURES
# 	2:14:FuturesDlg.cpp:FuturesDlg.cpp
# 	2:19:Application Include:AxisCode.h
# End Section
# Section AxisCode : {A8080709-6AB7-4D79-9032-A807BA0BF696}
# 	2:13:CLASS: BCMenu:BCMenu
# 	2:24:TYPEDEF: BC_MenuDrawMode:BC_MenuDrawMode
# 	2:18:ENUM: BC_Seperator:BC_Seperator
# 	2:18:CLASS: BCMenuMemDC:BCMenuMemDC
# 	2:17:CLASS: BCMenuData:BCMenuData
# 	2:8:BCMenu.h:BCMenu.h
# 	2:21:TYPEDEF: BC_Seperator:BC_Seperator
# 	2:20:CLASS: CMenuItemInfo:CMenuItemInfo
# 	2:21:ENUM: BC_MenuDrawMode:BC_MenuDrawMode
# 	2:19:Application Include:AxisCode.h
# 	2:20:CLASS: BCMenuToolBar:BCMenuToolBar
# 	2:10:BCMenu.cpp:BCMenu.cpp
# End Section
# Section AxisCode : {6D6985CE-DFED-4062-9002-69200595E71E}
# 	2:10:CheckBox.h:CheckBox.h
# 	2:12:CheckBox.cpp:CheckBox.cpp
# 	2:16:CLASS: CCheckBox:CCheckBox
# 	2:13:TYPEDEF: DATA:DATA
# 	2:11:CLASS: Data:Data
# 	2:19:Application Include:AxisCode.h
# End Section
# Section AxisCode : {E6BB1B68-A923-4CC3-83F2-5778E51B1DE6}
# 	2:16:3DTextButton.cpp:3DTextButton.cpp
# 	2:20:CLASS: C3DTextButton:C3DTextButton
# 	2:14:3DTextButton.h:3DTextButton.h
# 	2:19:Application Include:AxisCode.h
# End Section
# Section AxisCode : {E5801201-97EC-451C-A273-AC5976AADA01}
# 	2:12:CLASS: CTool:CTool
# 	2:8:Tool.cpp:Tool.cpp
# 	2:19:Application Include:AxisCode.h
# 	2:6:Tool.h:Tool.h
# End Section

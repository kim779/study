# Microsoft Developer Studio Project File - Name="AxMCtrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=AxMCtrl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AxMCtrl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AxMCtrl.mak" CFG="AxMCtrl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AxMCtrl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AxMCtrl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AxMCtrl - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 msimg32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"D:\IBKS(dev)\HTS\exe\AxMCtrl.dll"

!ELSEIF  "$(CFG)" == "AxMCtrl - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 msimg32.lib /nologo /subsystem:windows /dll /incremental:no /machine:I386 /out:"../../../../../exe/AxMCtrl.dll" /pdbtype:sept
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "AxMCtrl - Win32 Release"
# Name "AxMCtrl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AxButton.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMBarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMBorder.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMButtonCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMCellMergeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMConfigBar.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMCtrl.def
# End Source File
# Begin Source File

SOURCE=.\AxMCtrl.rc
# End Source File
# Begin Source File

SOURCE=.\AxMDib.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMRadioButton.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMShade.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMSpeedButton.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMSpinCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMStaticCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMTableCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMTableMergeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\AxMTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\common.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigBar_Pru.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\IndexSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TargetItemDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AxMCellMergeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\AxMCtrl.h
# End Source File
# Begin Source File

SOURCE=.\AxMDib.h
# End Source File
# Begin Source File

SOURCE=.\AxMEdit.h
# End Source File
# Begin Source File

SOURCE=.\AxMListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\AxMRadioButton.h
# End Source File
# Begin Source File

SOURCE=.\AxMSpeedButton.h
# End Source File
# Begin Source File

SOURCE=.\AxMSpinCtrl.h
# End Source File
# Begin Source File

SOURCE=.\AxMTableMergeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\AxMToolTip.h
# End Source File
# Begin Source File

SOURCE=.\AxMTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ConfigBar_Pru.h
# End Source File
# Begin Source File

SOURCE=.\IndexSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\msgdef.h
# End Source File
# Begin Source File

SOURCE=.\qsort.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AxMCtrl.rc2
# End Source File
# Begin Source File

SOURCE=.\res\folder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tabimg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree_ctrl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree_ctrl2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree_ctrl3.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

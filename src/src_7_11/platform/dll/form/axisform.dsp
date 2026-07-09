# Microsoft Developer Studio Project File - Name="axisform" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axisform - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axisform.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axisform.mak" CFG="axisform - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axisform - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axisform - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axisform - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 msimg32.lib winmm.lib ../lib/release/axislib.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"../../../release/axisform.dll"

!ELSEIF  "$(CFG)" == "axisform - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 msimg32.lib winmm.lib ../lib/debug/axislib.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../Debug/axisform.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "axisform - Win32 Release"
# Name "axisform - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\axArray.cpp
# End Source File
# Begin Source File

SOURCE=.\axform.cpp
# End Source File
# Begin Source File

SOURCE=.\axisform.cpp
# End Source File
# Begin Source File

SOURCE=.\axisform.def
# End Source File
# Begin Source File

SOURCE=.\axisform.odl
# End Source File
# Begin Source File

SOURCE=.\axisform.rc
# End Source File
# Begin Source File

SOURCE=.\browser.cpp
# End Source File
# Begin Source File

SOURCE=.\cbList.cpp
# End Source File
# Begin Source File

SOURCE=.\fmAvi.cpp
# End Source File
# Begin Source File

SOURCE=.\fmBase.cpp
# End Source File
# Begin Source File

SOURCE=.\fmBox.cpp
# End Source File
# Begin Source File

SOURCE=.\fmBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\fmButton.cpp
# End Source File
# Begin Source File

SOURCE=.\fmCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\fmCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\fmCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\fmEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\fmEditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\fmGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\fmGridx.cpp
# End Source File
# Begin Source File

SOURCE=.\fmGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\fmLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\fmMemo.cpp
# End Source File
# Begin Source File

SOURCE=.\fmObject.cpp
# End Source File
# Begin Source File

SOURCE=.\fmOut.cpp
# End Source File
# Begin Source File

SOURCE=.\fmPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\fmRadio.cpp
# End Source File
# Begin Source File

SOURCE=.\fmSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\fmSysm.cpp
# End Source File
# Begin Source File

SOURCE=.\fmTab.cpp
# End Source File
# Begin Source File

SOURCE=.\fmTable.cpp
# End Source File
# Begin Source File

SOURCE=.\fmTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\fmUserTab.cpp
# End Source File
# Begin Source File

SOURCE=.\image.cpp
# End Source File
# Begin Source File

SOURCE=.\memo.cpp
# End Source File
# Begin Source File

SOURCE=.\month.cpp
# End Source File
# Begin Source File

SOURCE=.\monthWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\axArray.h
# End Source File
# Begin Source File

SOURCE=.\axform.h
# End Source File
# Begin Source File

SOURCE=.\browser.h
# End Source File
# Begin Source File

SOURCE=.\cbList.h
# End Source File
# Begin Source File

SOURCE=.\fmAvi.h
# End Source File
# Begin Source File

SOURCE=.\fmBase.h
# End Source File
# Begin Source File

SOURCE=.\fmBox.h
# End Source File
# Begin Source File

SOURCE=.\fmBrowser.h
# End Source File
# Begin Source File

SOURCE=.\fmButton.h
# End Source File
# Begin Source File

SOURCE=.\fmCheck.h
# End Source File
# Begin Source File

SOURCE=.\fmCombo.h
# End Source File
# Begin Source File

SOURCE=.\fmCtrl.h
# End Source File
# Begin Source File

SOURCE=.\fmEdit.h
# End Source File
# Begin Source File

SOURCE=.\fmEditEx.h
# End Source File
# Begin Source File

SOURCE=.\fmGrid.h
# End Source File
# Begin Source File

SOURCE=.\fmGroup.h
# End Source File
# Begin Source File

SOURCE=.\fmLabel.h
# End Source File
# Begin Source File

SOURCE=.\fmMemo.h
# End Source File
# Begin Source File

SOURCE=.\fmObject.h
# End Source File
# Begin Source File

SOURCE=.\fmOut.h
# End Source File
# Begin Source File

SOURCE=.\fmPanel.h
# End Source File
# Begin Source File

SOURCE=.\fmRadio.h
# End Source File
# Begin Source File

SOURCE=.\fmSheet.h
# End Source File
# Begin Source File

SOURCE=.\fmSysm.h
# End Source File
# Begin Source File

SOURCE=.\fmTab.h
# End Source File
# Begin Source File

SOURCE=.\fmTable.h
# End Source File
# Begin Source File

SOURCE=.\fmTreeView.h
# End Source File
# Begin Source File

SOURCE=.\fmUserTab.h
# End Source File
# Begin Source File

SOURCE=.\gif.h
# End Source File
# Begin Source File

SOURCE=.\grid.h
# End Source File
# Begin Source File

SOURCE=.\image.h
# End Source File
# Begin Source File

SOURCE=.\memo.h
# End Source File
# Begin Source File

SOURCE=.\month.h
# End Source File
# Begin Source File

SOURCE=.\monthWnd.h
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

SOURCE=.\res\axisform.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

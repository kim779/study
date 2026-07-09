# Microsoft Developer Studio Project File - Name="awTool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=awTool - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "awTool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "awTool.mak" CFG="awTool - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "awTool - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "awTool - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "awTool - Win32 Release"

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
# ADD LINK32 ../../dll/lib/release/axislib.lib ../awCommon/release/awcommon.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../release/awtool.dll"

!ELSEIF  "$(CFG)" == "awTool - Win32 Debug"

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
# ADD LINK32 ../../dll/lib/debug/axislib.lib ../awCommon/debug/awcommon.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../exe/awtool.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "awTool - Win32 Release"
# Name "awTool - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ArrowWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\awTool.cpp
# End Source File
# Begin Source File

SOURCE=.\awTool.def
# End Source File
# Begin Source File

SOURCE=.\awTool.rc
# End Source File
# Begin Source File

SOURCE=..\ControlTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\pictureImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\SelFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\tlBox.cpp
# End Source File
# Begin Source File

SOURCE=.\tlButton.cpp
# End Source File
# Begin Source File

SOURCE=.\tlImage.cpp
# End Source File
# Begin Source File

SOURCE=.\tlIO.cpp
# End Source File
# Begin Source File

SOURCE=.\tlPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\tlPointer.cpp
# End Source File
# Begin Source File

SOURCE=.\tlRegion.cpp
# End Source File
# Begin Source File

SOURCE=.\tlSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\tlTab.cpp
# End Source File
# Begin Source File

SOURCE=.\tlTable.cpp
# End Source File
# Begin Source File

SOURCE=.\tlText.cpp
# End Source File
# Begin Source File

SOURCE=.\tlTree.cpp
# End Source File
# Begin Source File

SOURCE=.\tool.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ArrowWnd.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\tlBox.h
# End Source File
# Begin Source File

SOURCE=.\tlButton.h
# End Source File
# Begin Source File

SOURCE=.\tlImage.h
# End Source File
# Begin Source File

SOURCE=.\tlIO.h
# End Source File
# Begin Source File

SOURCE=.\tlPanel.h
# End Source File
# Begin Source File

SOURCE=.\tlPointer.h
# End Source File
# Begin Source File

SOURCE=.\tlRegion.h
# End Source File
# Begin Source File

SOURCE=.\tlSheet.h
# End Source File
# Begin Source File

SOURCE=.\tlTab.h
# End Source File
# Begin Source File

SOURCE=.\tlTable.h
# End Source File
# Begin Source File

SOURCE=.\tlText.h
# End Source File
# Begin Source File

SOURCE=.\tlTree.h
# End Source File
# Begin Source File

SOURCE=.\tool.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\awTool.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="cx_codectrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=cx_codectrl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cx_codectrl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cx_codectrl.mak" CFG="cx_codectrl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cx_codectrl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cx_codectrl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cx_codectrl - Win32 Release"

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
# ADD LINK32 imm32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../dev/cx_codectrl.dll"
# SUBTRACT LINK32 /incremental:yes /map /debug

!ELSEIF  "$(CFG)" == "cx_codectrl - Win32 Debug"

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
# ADD LINK32 imm32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../dev/cx_codectrl.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "cx_codectrl - Win32 Release"
# Name "cx_codectrl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AxWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeList.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\cx_codectrl.cpp
# End Source File
# Begin Source File

SOURCE=.\cx_codectrl.def
# End Source File
# Begin Source File

SOURCE=.\cx_codectrl.odl
# End Source File
# Begin Source File

SOURCE=.\cx_codectrl.rc
# End Source File
# Begin Source File

SOURCE=.\DDEDIT.CPP
# End Source File
# Begin Source File

SOURCE=.\fxCodeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\NButton.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SymbolWnd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AxWnd.h
# End Source File
# Begin Source File

SOURCE=.\CodeList.h
# End Source File
# Begin Source File

SOURCE=.\ControlWnd.h
# End Source File
# Begin Source File

SOURCE=.\cx_codectrl.h
# End Source File
# Begin Source File

SOURCE=.\DDEDIT.H
# End Source File
# Begin Source File

SOURCE=.\fxCodeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\InfoPopup.h
# End Source File
# Begin Source File

SOURCE=.\NButton.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SStatic.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SymbolWnd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cx_codectrl.rc2
# End Source File
# End Group
# Begin Group "ctrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AxBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\AxBitmap.h
# End Source File
# Begin Source File

SOURCE=.\ctrls\EditX.cpp
# End Source File
# Begin Source File

SOURCE=.\ctrls\EditX.h
# End Source File
# Begin Source File

SOURCE=.\ctrls\fxImgButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ctrls\fxImgButton.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section cx_codectrl : {58A27B4A-0F3A-48D3-A82A-821D754F38B7}
# 	2:13:fxImgButton.h:fxImgButton.h
# 	2:15:fxImgButton.cpp:fxImgButton.cpp
# 	2:19:CLASS: CfxImgButton:CfxImgButton
# 	2:19:Application Include:cx_codectrl.h
# End Section
# Section cx_codectrl : {33E40E49-6FDB-4E7D-8618-3DF779C419A1}
# 	2:16:CLASS: CAxBitmap:CAxBitmap
# 	2:12:AxBitmap.cpp:AxBitmap.cpp
# 	2:10:AxBitmap.h:AxBitmap.h
# 	2:19:Application Include:cx_codectrl.h
# End Section

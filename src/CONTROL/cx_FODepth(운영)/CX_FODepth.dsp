# Microsoft Developer Studio Project File - Name="CX_FODepth" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CX_FODepth - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CX_FODepth.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CX_FODepth.mak" CFG="CX_FODepth - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CX_FODepth - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CX_FODepth - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CX_FODepth - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /O<none> /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../../../dev/CX_FODepth.dll"
# SUBTRACT LINK32 /incremental:yes /map /debug

!ELSEIF  "$(CFG)" == "CX_FODepth - Win32 Debug"

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
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../dev/CX_FODepth.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CX_FODepth - Win32 Release"
# Name "CX_FODepth - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ContractGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\ContractScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\ContractWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CX_FODepth.cpp
# End Source File
# Begin Source File

SOURCE=.\CX_FODepth.def
# End Source File
# Begin Source File

SOURCE=.\CX_FODepth.odl
# End Source File
# Begin Source File

SOURCE=.\CX_FODepth.rc
# End Source File
# Begin Source File

SOURCE=.\depth.cpp
# End Source File
# Begin Source File

SOURCE=.\dx_opsenscal.cpp
# End Source File
# Begin Source File

SOURCE=.\libCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MBongToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\MBongWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\OpSensCal.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToolTip.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\ContractGrid.h
# End Source File
# Begin Source File

SOURCE=.\ContractScroll.h
# End Source File
# Begin Source File

SOURCE=.\ContractWnd.h
# End Source File
# Begin Source File

SOURCE=.\CX_FODepth.h
# End Source File
# Begin Source File

SOURCE=.\datatype.h
# End Source File
# Begin Source File

SOURCE=.\depth.h
# End Source File
# Begin Source File

SOURCE=.\dx_opsenscal.h
# End Source File
# Begin Source File

SOURCE=.\index.hxx
# End Source File
# Begin Source File

SOURCE=.\keys.hxx
# End Source File
# Begin Source File

SOURCE=.\libCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MBongLib.h
# End Source File
# Begin Source File

SOURCE=.\MBongToolTip.h
# End Source File
# Begin Source File

SOURCE=.\MBongWnd.h
# End Source File
# Begin Source File

SOURCE=.\memDc.h
# End Source File
# Begin Source File

SOURCE=.\OpSensCal.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolTip.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CX_FODepth.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\symbol.h
# End Source File
# End Target
# End Project
# Section CX_FODepth : {3A6DCEA9-8978-4CE7-A7B3-3BBFDCE4DE96}
# 	1:10:IDD_CONFIG:1007
# 	2:16:Resource Include:resource.h
# 	2:13:ConfigDlg.cpp:ConfigDlg.cpp
# 	2:17:CLASS: CConfigDlg:CConfigDlg
# 	2:11:ConfigDlg.h:ConfigDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:CX_FODepth.h
# 	2:10:IDD_CONFIG:IDD_CONFIG
# End Section

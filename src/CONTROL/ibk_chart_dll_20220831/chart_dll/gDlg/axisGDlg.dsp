# Microsoft Developer Studio Project File - Name="axisGDlg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axisGDlg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axisGDlg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axisGDlg.mak" CFG="axisGDlg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axisGDlg - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axisGDlg - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axisGDlg - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ../../control/fx_grid/Release/fx_grid.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../Release/AXISGDLG.DLL"

!ELSEIF  "$(CFG)" == "axisGDlg - Win32 Debug"

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
# ADD LINK32 ../../control/fx_grid/debug/fx_grid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../exe/AXISGDLG.DLL" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "axisGDlg - Win32 Release"
# Name "axisGDlg - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\axisGDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\axisGDlg.def
# End Source File
# Begin Source File

SOURCE=.\axisGDlg.rc
# End Source File
# Begin Source File

SOURCE=.\ControlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls.cpp
# End Source File
# Begin Source File

SOURCE=.\exGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\IndiBase.cpp
# End Source File
# Begin Source File

SOURCE=.\IndiDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\IndiHorizon.cpp
# End Source File
# Begin Source File

SOURCE=.\IndiObj.cpp
# End Source File
# Begin Source File

SOURCE=.\IndiObjMA.cpp
# End Source File
# Begin Source File

SOURCE=.\libcommon.cpp
# End Source File
# Begin Source File

SOURCE=.\libGDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainPreview.cpp
# End Source File
# Begin Source File

SOURCE=.\MainType1.cpp
# End Source File
# Begin Source File

SOURCE=..\MxTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\gCom\PnAidInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\gCom\PnToolInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveFrameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetAidDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetDrawtoolDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupEnvDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupViewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetYAxisDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetYShowDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToolCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\TotalCfg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\axisGDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\h\axisgenv.h
# End Source File
# Begin Source File

SOURCE=..\..\h\axisgwin.h
# End Source File
# Begin Source File

SOURCE=.\ControlEx.h
# End Source File
# Begin Source File

SOURCE=.\Controls.h
# End Source File
# Begin Source File

SOURCE=.\exGrid.h
# End Source File
# Begin Source File

SOURCE=.\IndiBase.h
# End Source File
# Begin Source File

SOURCE=.\IndiDisp.h
# End Source File
# Begin Source File

SOURCE=.\IndiHorizon.h
# End Source File
# Begin Source File

SOURCE=.\indiinfo.h
# End Source File
# Begin Source File

SOURCE=.\IndiObj.h
# End Source File
# Begin Source File

SOURCE=.\IndiObjMA.h
# End Source File
# Begin Source File

SOURCE=.\libcommon.h
# End Source File
# Begin Source File

SOURCE=.\MainPreview.h
# End Source File
# Begin Source File

SOURCE=.\MainType1.h
# End Source File
# Begin Source File

SOURCE=.\ObjConfig.h
# End Source File
# Begin Source File

SOURCE=.\PreviewWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SaveFrameDlg.h
# End Source File
# Begin Source File

SOURCE=.\ScreenCfg.h
# End Source File
# Begin Source File

SOURCE=.\SetAidDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetDrawtoolDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetupEnvDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetupViewDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetYAxisDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetYShowDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolCfg.h
# End Source File
# Begin Source File

SOURCE=.\TotalCfg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\axisGDlg.rc2
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\justtest.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

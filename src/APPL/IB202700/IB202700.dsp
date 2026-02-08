# Microsoft Developer Studio Project File - Name="IB202700" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=IB202700 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IB202700.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB202700.mak" CFG="IB202700 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB202700 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB202700 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IB202700 - Win32 Release"

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
# ADD LINK32 ../../control/fx_misc/release/fx_misc.lib imm32.lib winmm.lib ../../control/fx_gridex/release/fx_gridex.lib Msimg32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../release/IB202700.dll"
# SUBTRACT LINK32 /incremental:yes /map /debug

!ELSEIF  "$(CFG)" == "IB202700 - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../../control/fx_misc/release/fx_misc.lib imm32.lib winmm.lib ../../control/fx_gridex/release/fx_gridex.lib Msimg32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../dev/IB202700.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "IB202700 - Win32 Release"
# Name "IB202700 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AxisExt.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeDuplicate.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrlExt.cpp
# End Source File
# Begin Source File

SOURCE=.\GridWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\IB202700.cpp
# End Source File
# Begin Source File

SOURCE=.\IB202700.def
# End Source File
# Begin Source File

SOURCE=.\IB202700.odl
# End Source File
# Begin Source File

SOURCE=.\IB202700.rc
# End Source File
# Begin Source File

SOURCE=..\IhUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\MainWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AxisExt.h
# End Source File
# Begin Source File

SOURCE=.\CodeDuplicate.h
# End Source File
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrlExt.h
# End Source File
# Begin Source File

SOURCE=.\GridWnd.h
# End Source File
# Begin Source File

SOURCE=.\GroupWnd.h
# End Source File
# Begin Source File

SOURCE=.\IB202700.h
# End Source File
# Begin Source File

SOURCE=.\ioformat.h
# End Source File
# Begin Source File

SOURCE=.\MainWnd.h
# End Source File
# Begin Source File

SOURCE=.\MoveSetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\PopupDlg.h
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

SOURCE=.\res\IB202700.rc2
# End Source File
# End Group
# Begin Group "grid"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\grid\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\grid\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\GridCell.h
# End Source File
# Begin Source File

SOURCE=.\grid\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=.\grid\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\grid\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\grid\griditem.h
# End Source File
# Begin Source File

SOURCE=.\grid\IHCodeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\IHCodeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\grid\InPlaceCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\InPlaceCombo.h
# End Source File
# Begin Source File

SOURCE=.\grid\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\grid\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\grid\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\grid\TitleTip.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

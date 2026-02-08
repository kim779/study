# Microsoft Developer Studio Project File - Name="IB191000" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=IB191000 - Win32 IB300700 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IB191000.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB191000.mak" CFG="IB191000 - Win32 IB300700 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB191000 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB191000 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB191000 - Win32 IB300600 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB191000 - Win32 IB300600 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB191000 - Win32 IB300700 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB191000 - Win32 IB300700 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IB191000 - Win32 Release"

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
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 msimg32.lib ../../control/fx_misc/Release/fx_misc.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../dev/IB191000.DLL"

!ELSEIF  "$(CFG)" == "IB191000 - Win32 Debug"

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
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 msimg32.lib ../../control/fx_misc/Debug/fx_misc.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../dev/IB191000.DLL" /pdbtype:sept

!ELSEIF  "$(CFG)" == "IB191000 - Win32 IB300600 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "IB191000___Win32_IB300600_Release"
# PROP BASE Intermediate_Dir "IB191000___Win32_IB300600_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "IB300600_Release"
# PROP Intermediate_Dir "IB300600_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /D "_DEF_IB300600_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 msimg32.lib ../../control/fx_misc/Release/fx_misc.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../dev/IB191000.DLL"
# ADD LINK32 msimg32.lib ../../control/fx_misc/Release/fx_misc.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"../../../dev/IB300600.DLL"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "IB191000 - Win32 IB300600 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "IB191000___Win32_IB300600_Debug"
# PROP BASE Intermediate_Dir "IB191000___Win32_IB300600_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "IB300600_Debug"
# PROP Intermediate_Dir "IB300600_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEF_IB300600_" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 msimg32.lib ../../control/fx_misc/Debug/fx_misc.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../dev/IB191000.DLL" /pdbtype:sept
# ADD LINK32 msimg32.lib ../../control/fx_misc/Debug/fx_misc.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"../../../dev/IB300600.DLL" /pdbtype:sept

!ELSEIF  "$(CFG)" == "IB191000 - Win32 IB300700 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "IB191000___Win32_IB300700_Debug"
# PROP BASE Intermediate_Dir "IB191000___Win32_IB300700_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "IB300700_Debug"
# PROP Intermediate_Dir "IB300700_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 msimg32.lib ../../control/fx_misc/Debug/fx_misc.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../dev/IB191000.DLL" /pdbtype:sept
# ADD LINK32 msimg32.lib ../../control/fx_misc/Debug/fx_misc.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../dev\IB300700.DLL" /pdbtype:sept

!ELSEIF  "$(CFG)" == "IB191000 - Win32 IB300700 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "IB191000___Win32_IB300700_Release"
# PROP BASE Intermediate_Dir "IB191000___Win32_IB300700_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "IB300700_Release"
# PROP Intermediate_Dir "IB300700_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 msimg32.lib ../../control/fx_misc/Release/fx_misc.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../dev/IB191000.DLL"
# ADD LINK32 msimg32.lib ../../control/fx_misc/Release/fx_misc.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../dev/IB300700.DLL"
# SUBTRACT LINK32 /incremental:yes /map /debug

!ENDIF 

# Begin Target

# Name "IB191000 - Win32 Release"
# Name "IB191000 - Win32 Debug"
# Name "IB191000 - Win32 IB300600 Release"
# Name "IB191000 - Win32 IB300600 Debug"
# Name "IB191000 - Win32 IB300700 Debug"
# Name "IB191000 - Win32 IB300700 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Account.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountConfirmWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxisExt.cpp
# End Source File
# Begin Source File

SOURCE=.\BottomGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfirmDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Control2Wnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FOGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FOHogaWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrlExt.cpp
# End Source File
# Begin Source File

SOURCE=.\IB191000.cpp
# End Source File
# Begin Source File

SOURCE=.\IB191000.def
# End Source File
# Begin Source File

SOURCE=.\IB191000.rc
# End Source File
# Begin Source File

SOURCE=.\JngoTicker.cpp
# End Source File
# Begin Source File

SOURCE=.\JumunCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\MapWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MarketConfirm.cpp
# End Source File
# Begin Source File

SOURCE=.\NotifyCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SiseWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SpEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Account.h
# End Source File
# Begin Source File

SOURCE=.\AccountConfirmWnd.h
# End Source File
# Begin Source File

SOURCE=.\AccountCtrl.h
# End Source File
# Begin Source File

SOURCE=.\AxisExt.h
# End Source File
# Begin Source File

SOURCE=.\AxWnd.h
# End Source File
# Begin Source File

SOURCE=.\BottomGrid.h
# End Source File
# Begin Source File

SOURCE=.\CodeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\ConfirmDlg.h
# End Source File
# Begin Source File

SOURCE=.\Control2Wnd.h
# End Source File
# Begin Source File

SOURCE=.\ControlWnd.h
# End Source File
# Begin Source File

SOURCE=.\dataio.h
# End Source File
# Begin Source File

SOURCE=.\FOGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FOHogaWnd.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrlExt.h
# End Source File
# Begin Source File

SOURCE=.\IB191000.h
# End Source File
# Begin Source File

SOURCE=.\JngoTicker.h
# End Source File
# Begin Source File

SOURCE=.\JumunCheck.h
# End Source File
# Begin Source File

SOURCE=.\MapWnd.h
# End Source File
# Begin Source File

SOURCE=.\MarketConfirm.h
# End Source File
# Begin Source File

SOURCE=.\memDc.h
# End Source File
# Begin Source File

SOURCE=.\NotifyCtrl.h
# End Source File
# Begin Source File

SOURCE=.\OptionEx.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScreenWnd.h
# End Source File
# Begin Source File

SOURCE=.\SiseWnd.h
# End Source File
# Begin Source File

SOURCE=.\SpEdit.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\IB191000.rc2
# End Source File
# Begin Source File

SOURCE=.\IB191000.rgs
# End Source File
# End Group
# Begin Group "Grid"

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
# End Target
# End Project
# Section IB191000 : {00550055-0C72-0000-1000-402035006500}
# 	1:12:IDR_IB191000:1008
# End Section

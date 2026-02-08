# Microsoft Developer Studio Project File - Name="IB885200" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=IB885200 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IB885200.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB885200.mak" CFG="IB885200 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB885200 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB885200 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IB885200 - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ../../control/fx_misc/Release/fx_misc.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"../../../dev/IB885200.DLL"

!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"

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
# ADD LINK32 ../../control/fx_misc/Debug/fx_misc.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"C:\Program Files\IBK_DEV\HTS\dev\IB885200.DLL" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "IB885200 - Win32 Release"
# Name "IB885200 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3DEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeReg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ContainerWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\cx_notify.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogConfirm.cpp
# End Source File
# Begin Source File

SOURCE=.\HogaWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\IB885200.cpp
# End Source File
# Begin Source File

SOURCE=.\IB885200.def
# End Source File
# Begin Source File

SOURCE=.\IB885200.rc
# End Source File
# Begin Source File

SOURCE=.\JumunCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\MapWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiJumunDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RectWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SelJumunDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SendOrder.cpp
# End Source File
# Begin Source File

SOURCE=.\SpinCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Tool.cpp
# End Source File
# Begin Source File

SOURCE=.\Warning.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\3DEdit.h
# End Source File
# Begin Source File

SOURCE=.\AccountCtrl.h
# End Source File
# Begin Source File

SOURCE=.\AxScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\CodeReg.h
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\ContainerWnd.h
# End Source File
# Begin Source File

SOURCE=.\ControlWnd.h
# End Source File
# Begin Source File

SOURCE=.\cx_notify.h
# End Source File
# Begin Source File

SOURCE=.\DialogConfirm.h
# End Source File
# Begin Source File

SOURCE=.\HogaWnd.h
# End Source File
# Begin Source File

SOURCE=.\IB885200.h
# End Source File
# Begin Source File

SOURCE=..\Share\index_com.h
# End Source File
# Begin Source File

SOURCE=.\JumunCheck.h
# End Source File
# Begin Source File

SOURCE=.\MapWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\h\MistakeStopOrder.h
# End Source File
# Begin Source File

SOURCE=.\MultiJumunDlg.h
# End Source File
# Begin Source File

SOURCE=.\RectWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelJumunDlg.h
# End Source File
# Begin Source File

SOURCE=.\SendOrder.h
# End Source File
# Begin Source File

SOURCE=.\SpinCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextWnd.h
# End Source File
# Begin Source File

SOURCE=.\Tool.h
# End Source File
# Begin Source File

SOURCE=.\UserDefine.h
# End Source File
# Begin Source File

SOURCE=.\Warning.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\cursor_h.cur
# End Source File
# Begin Source File

SOURCE=.\res\IB885200.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

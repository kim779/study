# Microsoft Developer Studio Project File - Name="IB300200" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=IB300200 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IB300200.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB300200.mak" CFG="IB300200 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB300200 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB300200 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IB300200 - Win32 Release"

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
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ../../control/fx_misc/Release/fx_misc.lib ../../control/fx_GridEx/Release/fx_GridEx.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../dev/IB300200.dll" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none /incremental:yes /map /debug

!ELSEIF  "$(CFG)" == "IB300200 - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../../control/fx_misc/Debug/fx_misc.lib ../../control/fx_GridEx/Debug/fx_GridEx.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../dev/IB300200.dll" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no /map

!ENDIF 

# Begin Target

# Name "IB300200 - Win32 Release"
# Name "IB300200 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3DEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxCommaEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\AxScrollBar.cpp
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

SOURCE=.\exButton.cpp
# End Source File
# Begin Source File

SOURCE=.\HogaWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\IB300200.cpp
# End Source File
# Begin Source File

SOURCE=.\IB300200.def
# End Source File
# Begin Source File

SOURCE=.\IB300200.rc
# End Source File
# Begin Source File

SOURCE=.\JangoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\JumunCheck.cpp
# End Source File
# Begin Source File

SOURCE="..\..\appl\IB0000A1 - 계좌비밀번호저장\MapForm.cpp"
# End Source File
# Begin Source File

SOURCE=.\MapWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MarketConfirm.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiJumunDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionEx.cpp
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
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccountCtrl.h
# End Source File
# Begin Source File

SOURCE=.\AxCommaEdit.h
# End Source File
# Begin Source File

SOURCE=.\AxScrollBar.h
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

SOURCE=.\dataio.h
# End Source File
# Begin Source File

SOURCE=.\DialogConfirm.h
# End Source File
# Begin Source File

SOURCE=.\exButton.h
# End Source File
# Begin Source File

SOURCE=.\HogaWnd.h
# End Source File
# Begin Source File

SOURCE=.\IB300200.h
# End Source File
# Begin Source File

SOURCE=.\JangoWnd.h
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

SOURCE=..\..\h\MistakeStopOrder.h
# End Source File
# Begin Source File

SOURCE=.\MultiJumunDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptionEx.h
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

SOURCE=.\symbol_rts.h
# End Source File
# Begin Source File

SOURCE=.\TextWnd.h
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

SOURCE=.\res\IB300200.rc2
# End Source File
# End Group
# End Target
# End Project
# Section IB300200 : {5798B7A2-0FFD-48B2-8E92-762C96D8D129}
# 	1:15:IDD_SELJUMUNDLG:1008
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:15:SelJumunDlg.cpp:SelJumunDlg.cpp
# 	2:13:SelJumunDlg.h:SelJumunDlg.h
# 	2:19:CLASS: CSelJumunDlg:CSelJumunDlg
# 	2:19:Application Include:IB300200.h
# 	2:15:IDD_SELJUMUNDLG:IDD_SELJUMUNDLG
# End Section
# Section IB300200 : {F4E6349D-D6ED-47ED-A8A5-2B6C4AF4BCE9}
# 	1:14:IDD_MULTIJUMUN:1007
# 	2:16:Resource Include:resource.h
# 	2:15:MultiJumunDlg.h:MultiJumunDlg.h
# 	2:10:ENUM: enum:enum
# 	2:17:MultiJumunDlg.cpp:MultiJumunDlg.cpp
# 	2:14:IDD_MULTIJUMUN:IDD_MULTIJUMUN
# 	2:19:Application Include:IB300200.h
# 	2:21:CLASS: CMultiJumunDlg:CMultiJumunDlg
# End Section
# Section IB300200 : {BFA8E2F2-A6EC-4703-8CEC-F05F0E2EC2B6}
# 	1:18:IDD_DIALOG_CONFIRM:1009
# 	2:16:Resource Include:resource.h
# 	2:17:DialogConfirm.cpp:DialogConfirm.cpp
# 	2:15:DialogConfirm.h:DialogConfirm.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:IB300200.h
# 	2:21:CLASS: CDialogConfirm:CDialogConfirm
# 	2:18:IDD_DIALOG_CONFIRM:IDD_DIALOG_CONFIRM
# End Section

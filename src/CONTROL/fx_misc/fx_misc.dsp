# Microsoft Developer Studio Project File - Name="fx_misc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fx_misc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fx_misc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fx_misc.mak" CFG="fx_misc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fx_misc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fx_misc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fx_misc - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 imm32.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"../../../dev/FX_MISC.DLL"

!ELSEIF  "$(CFG)" == "fx_misc - Win32 Debug"

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
# SUBTRACT CPP /Z<none>
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 imm32.lib /nologo /subsystem:windows /dll /incremental:no /machine:I386 /out:"../../../dev/fx_misc.dll" /pdbtype:sept
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "fx_misc - Win32 Release"
# Name "fx_misc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\colorCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\fontNameCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\fontSizeCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\fx_misc.cpp
# End Source File
# Begin Source File

SOURCE=.\fx_misc.def
# End Source File
# Begin Source File

SOURCE=.\fx_misc.rc
# End Source File
# Begin Source File

SOURCE=.\fxButton.cpp
# End Source File
# Begin Source File

SOURCE=.\fxCodeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\fxColorPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\fxEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\fxFontDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\fxImgButton.cpp
# End Source File
# Begin Source File

SOURCE=.\fxLineStyleCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\fxSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\fxSpin.cpp
# End Source File
# Begin Source File

SOURCE=.\fxStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\fxTab.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\colorCombo.h
# End Source File
# Begin Source File

SOURCE=.\fontNameCombo.h
# End Source File
# Begin Source File

SOURCE=.\fontSizeCombo.h
# End Source File
# Begin Source File

SOURCE=.\fxButton.h
# End Source File
# Begin Source File

SOURCE=.\fxCodeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\fxColorPopup.h
# End Source File
# Begin Source File

SOURCE=.\fxEdit.h
# End Source File
# Begin Source File

SOURCE=.\fxFontDialog.h
# End Source File
# Begin Source File

SOURCE=.\fxImgButton.h
# End Source File
# Begin Source File

SOURCE=.\fxLineStyleCombo.h
# End Source File
# Begin Source File

SOURCE=.\fxSheet.h
# End Source File
# Begin Source File

SOURCE=.\fxSpin.h
# End Source File
# Begin Source File

SOURCE=.\fxStatic.h
# End Source File
# Begin Source File

SOURCE=.\fxTab.h
# End Source File
# Begin Source File

SOURCE=.\memDc.h
# End Source File
# Begin Source File

SOURCE=.\misctype.h
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

SOURCE=.\res\fx_misc.rc2
# End Source File
# Begin Source File

SOURCE=.\res\sheetimg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treeimg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\truetype.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

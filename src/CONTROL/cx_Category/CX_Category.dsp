# Microsoft Developer Studio Project File - Name="CX_Category" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CX_Category - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CX_Category.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CX_Category.mak" CFG="CX_Category - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CX_Category - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CX_Category - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CX_Category - Win32 Release"

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
# ADD LINK32 /nologo /base:"0x65100000" /subsystem:windows /dll /machine:I386 /def:".\CX_Category.def" /out:"../../../dev/CX_CATEGORY.dll" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none /map /debug

!ELSEIF  "$(CFG)" == "CX_Category - Win32 Debug"

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
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../dev/CX_Category.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CX_Category - Win32 Release"
# Name "CX_Category - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CategoryPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CX_Category.cpp
# End Source File
# Begin Source File

SOURCE=.\CX_Category.def

!IF  "$(CFG)" == "CX_Category - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CX_Category - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CX_Category.odl
# End Source File
# Begin Source File

SOURCE=.\CX_Category.rc
# End Source File
# Begin Source File

SOURCE=.\libctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\SStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CategoryPopup.h
# End Source File
# Begin Source File

SOURCE=.\ControlWnd.h
# End Source File
# Begin Source File

SOURCE=.\CX_Category.h
# End Source File
# Begin Source File

SOURCE=.\libctrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SComboBox.h
# End Source File
# Begin Source File

SOURCE=.\SStatic.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CX_Category.rc2
# End Source File
# End Group
# Begin Group "CTRL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ExListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\listctrlSort.cpp
# End Source File
# Begin Source File

SOURCE=.\listctrlSort.h
# End Source File
# Begin Source File

SOURCE=.\listEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\listEdit.h
# End Source File
# Begin Source File

SOURCE=.\listHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\listHeader.h
# End Source File
# Begin Source File

SOURCE=.\XPButton.cpp
# End Source File
# Begin Source File

SOURCE=.\XPButton.h
# End Source File
# Begin Source File

SOURCE=.\xSkinButton.cpp
# End Source File
# Begin Source File

SOURCE=.\xSkinButton.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\h\jmcode.h
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="IB0000A2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=IB0000A2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IB0000A2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB0000A2.mak" CFG="IB0000A2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB0000A2 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB0000A2 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IB0000A2 - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ../../control/fx_grid/release/fx_grid.lib ../../dll/axiscm/Debug/axiscm.lib ../../control/fx_misc/Release/fx_misc.lib /nologo /subsystem:windows /dll /map /machine:I386 /def:".\IB0000A2.def" /out:"../../../dev/IB0000A2.dll" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "IB0000A2 - Win32 Debug"

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
# ADD LINK32 ../../control/fx_grid/Debug/fx_grid.lib ../../dll/axiscm/Debug/axiscm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../Dev/IB0000A2.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "IB0000A2 - Win32 Release"
# Name "IB0000A2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AccEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\AxMapForm\AxMapForm.cpp
# End Source File
# Begin Source File

SOURCE=.\AxStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgDirect.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgGrNew.cpp
# End Source File
# Begin Source File

SOURCE=.\exTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\IB0000A2.cpp
# End Source File
# Begin Source File

SOURCE=.\IB0000A2.def

!IF  "$(CFG)" == "IB0000A2 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "IB0000A2 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IB0000A2.rc
# End Source File
# Begin Source File

SOURCE=.\ImgBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\libmap.cpp
# End Source File
# Begin Source File

SOURCE=.\mapForm.cpp
# End Source File
# Begin Source File

SOURCE=..\AxCommon\MDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\XXGrid.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccEdit.h
# End Source File
# Begin Source File

SOURCE=.\AxMapForm.h
# End Source File
# Begin Source File

SOURCE=.\AxStatic.h
# End Source File
# Begin Source File

SOURCE=.\dlgDirect.h
# End Source File
# Begin Source File

SOURCE=.\dlgGrNew.h
# End Source File
# Begin Source File

SOURCE=.\exTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\IB0000A2.h
# End Source File
# Begin Source File

SOURCE=.\ImgBtn.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\libDefine.h
# End Source File
# Begin Source File

SOURCE=.\libmap.h
# End Source File
# Begin Source File

SOURCE=.\mapForm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\XXGrid.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IB0000A2.rc2
# End Source File
# Begin Source File

SOURCE=.\res\treeimg.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

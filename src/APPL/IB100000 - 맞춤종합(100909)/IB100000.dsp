# Microsoft Developer Studio Project File - Name="IB100000" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=IB100000 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IB100000.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB100000.mak" CFG="IB100000 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB100000 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB100000 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IB100000 - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../../../dev/IB100000.dll"

!ELSEIF  "$(CFG)" == "IB100000 - Win32 Debug"

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
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../dev\IB100000.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile /map

!ENDIF 

# Begin Target

# Name "IB100000 - Win32 Release"
# Name "IB100000 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\base.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigBar.cpp
# End Source File
# Begin Source File

SOURCE=.\IB100000.cpp
# End Source File
# Begin Source File

SOURCE=.\IB100000.def
# End Source File
# Begin Source File

SOURCE=.\IB100000.rc
# End Source File
# Begin Source File

SOURCE=.\libx.cpp
# End Source File
# Begin Source File

SOURCE=.\Pane.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\USetup.cpp
# End Source File
# Begin Source File

SOURCE=.\XTab.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\base.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\ConfigBar.h
# End Source File
# Begin Source File

SOURCE=.\IB100000.h
# End Source File
# Begin Source File

SOURCE=.\libx.h
# End Source File
# Begin Source File

SOURCE=.\Pane.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Screen.h
# End Source File
# Begin Source File

SOURCE=.\Sm.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\USetup.h
# End Source File
# Begin Source File

SOURCE=.\XTab.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\IB100000.rc2
# End Source File
# Begin Source File

SOURCE=.\res\menu1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\menu2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\setup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sOFF.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sON.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type1_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type2_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type3_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type4_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type5_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type6_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type7_.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="fx_GridEx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fx_GridEx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fx_GridEx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fx_GridEx.mak" CFG="fx_GridEx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fx_GridEx - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fx_GridEx - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fx_GridEx - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_BUILD_UG_INTO_EXTDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../../../dev/fx_GridEx.dll"

!ELSEIF  "$(CFG)" == "fx_GridEx - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_BUILD_UG_INTO_EXTDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /u
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../dev/fx_GridEx.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "fx_GridEx - Win32 Release"
# Name "fx_GridEx - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\fx_GridEx.cpp
# End Source File
# Begin Source File

SOURCE=.\fx_GridEx.def
# End Source File
# Begin Source File

SOURCE=.\fx_GridEx.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UGCBType.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCell.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCelTyp.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCnrBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTafnt.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTarrw.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTAutoSize.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTbutn.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTDropGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTdtp.cpp
# End Source File
# Begin Source File

SOURCE=.\ugctelps.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTLabeled.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTMarquee.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTmfnt.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTNote.cpp
# End Source File
# Begin Source File

SOURCE=.\ugctnotewnd.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTpie.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTpro1.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTprog.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTRado.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTsarw.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTsldr.cpp
# End Source File
# Begin Source File

SOURCE=.\UGCTSpin.cpp
# End Source File
# Begin Source File

SOURCE=.\UGDLType.cpp
# End Source File
# Begin Source File

SOURCE=.\UGDrgDrp.cpp
# End Source File
# Begin Source File

SOURCE=.\UGDrwHnt.cpp
# End Source File
# Begin Source File

SOURCE=.\UGDtaSrc.cpp
# End Source File
# Begin Source File

SOURCE=.\UGEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\UGEditBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ugformat.cpp
# End Source File
# Begin Source File

SOURCE=.\uggdinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\UGGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\UGHint.cpp
# End Source File
# Begin Source File

SOURCE=.\ughscrol.cpp
# End Source File
# Begin Source File

SOURCE=.\ugLstBox.cpp
# End Source File
# Begin Source File

SOURCE=.\UGMEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\UGMemMan.cpp
# End Source File
# Begin Source File

SOURCE=.\UGMultiS.cpp
# End Source File
# Begin Source File

SOURCE=.\ugprint.cpp
# End Source File
# Begin Source File

SOURCE=.\ugptrlst.cpp
# End Source File
# Begin Source File

SOURCE=.\ugsidehd.cpp
# End Source File
# Begin Source File

SOURCE=.\ugtab.cpp
# End Source File
# Begin Source File

SOURCE=.\UGTopHdg.cpp
# End Source File
# Begin Source File

SOURCE=.\ugvscrol.cpp
# End Source File
# Begin Source File

SOURCE=.\UTEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\UTMaskedEdit.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UGCBType.h
# End Source File
# Begin Source File

SOURCE=.\UGCell.h
# End Source File
# Begin Source File

SOURCE=.\UGCelTyp.h
# End Source File
# Begin Source File

SOURCE=.\UGCnrBtn.h
# End Source File
# Begin Source File

SOURCE=.\UGCTafnt.h
# End Source File
# Begin Source File

SOURCE=.\UGCTarrw.h
# End Source File
# Begin Source File

SOURCE=.\UGCTAutoSize.h
# End Source File
# Begin Source File

SOURCE=.\UGCTbutn.h
# End Source File
# Begin Source File

SOURCE=.\UGCTDropGrid.h
# End Source File
# Begin Source File

SOURCE=.\UGCTdtp.h
# End Source File
# Begin Source File

SOURCE=.\ugctelps.h
# End Source File
# Begin Source File

SOURCE=.\UGCTLabeled.h
# End Source File
# Begin Source File

SOURCE=.\UGCTMarquee.h
# End Source File
# Begin Source File

SOURCE=.\UGCTmfnt.h
# End Source File
# Begin Source File

SOURCE=.\UGCTNote.h
# End Source File
# Begin Source File

SOURCE=.\ugctnotewnd.h
# End Source File
# Begin Source File

SOURCE=.\UGCTpie.h
# End Source File
# Begin Source File

SOURCE=.\UGCTpro1.h
# End Source File
# Begin Source File

SOURCE=.\UGCTprog.h
# End Source File
# Begin Source File

SOURCE=.\UGCTRado.h
# End Source File
# Begin Source File

SOURCE=.\UGCtrl.h
# End Source File
# Begin Source File

SOURCE=.\UGCTsarw.h
# End Source File
# Begin Source File

SOURCE=.\UGCTsldr.h
# End Source File
# Begin Source File

SOURCE=.\UGCTSpin.h
# End Source File
# Begin Source File

SOURCE=.\ugdefine.h
# End Source File
# Begin Source File

SOURCE=.\UGDLType.h
# End Source File
# Begin Source File

SOURCE=.\UGDrgDrp.h
# End Source File
# Begin Source File

SOURCE=.\UGDrwHnt.h
# End Source File
# Begin Source File

SOURCE=.\UGDtaSrc.h
# End Source File
# Begin Source File

SOURCE=.\UGEdit.h
# End Source File
# Begin Source File

SOURCE=.\UGEditBase.h
# End Source File
# Begin Source File

SOURCE=.\ugformat.h
# End Source File
# Begin Source File

SOURCE=.\uggdinfo.h
# End Source File
# Begin Source File

SOURCE=.\UGGrid.h
# End Source File
# Begin Source File

SOURCE=.\UGHint.h
# End Source File
# Begin Source File

SOURCE=.\ughscrol.h
# End Source File
# Begin Source File

SOURCE=.\Uglibsel.h
# End Source File
# Begin Source File

SOURCE=.\uglstbox.h
# End Source File
# Begin Source File

SOURCE=.\UGMEdit.h
# End Source File
# Begin Source File

SOURCE=.\UGMemMan.h
# End Source File
# Begin Source File

SOURCE=.\UGMultiS.h
# End Source File
# Begin Source File

SOURCE=.\ugprint.h
# End Source File
# Begin Source File

SOURCE=.\ugptrlst.h
# End Source File
# Begin Source File

SOURCE=.\ugsidehd.h
# End Source File
# Begin Source File

SOURCE=.\ugstruct.h
# End Source File
# Begin Source File

SOURCE=.\ugtab.h
# End Source File
# Begin Source File

SOURCE=.\UGTopHdg.h
# End Source File
# Begin Source File

SOURCE=.\ugvscrol.h
# End Source File
# Begin Source File

SOURCE=.\UTEdit.h
# End Source File
# Begin Source File

SOURCE=.\UTMaskedEdit.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res\fx_GridEx.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

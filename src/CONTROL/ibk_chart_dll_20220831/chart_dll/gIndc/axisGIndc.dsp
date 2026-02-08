# Microsoft Developer Studio Project File - Name="axisGIndc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axisGIndc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axisGIndc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axisGIndc.mak" CFG="axisGIndc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axisGIndc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axisGIndc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axisGIndc - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ../gData/Release/axisGData.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../Release/AXISGINDC.DLL"

!ELSEIF  "$(CFG)" == "axisGIndc - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../gData/debug/axisGData.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../exe/AXISGINDC.DLL" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "axisGIndc - Win32 Release"
# Name "axisGIndc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "BongType_CPP"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\BAmt.cpp
# End Source File
# Begin Source File

SOURCE=.\BAVol.cpp
# End Source File
# Begin Source File

SOURCE=.\BBalance.cpp
# End Source File
# Begin Source File

SOURCE=.\BBollinger.cpp
# End Source File
# Begin Source File

SOURCE=.\BDemark.cpp
# End Source File
# Begin Source File

SOURCE=.\BEnvelope.cpp
# End Source File
# Begin Source File

SOURCE=.\BJugaChart.cpp
# End Source File
# Begin Source File

SOURCE=.\BMA.cpp
# End Source File
# Begin Source File

SOURCE=.\BParabolic.cpp
# End Source File
# Begin Source File

SOURCE=.\BPivot.cpp
# End Source File
# Begin Source File

SOURCE=.\BVolume.cpp
# End Source File
# Begin Source File

SOURCE=.\LRSIWeight.cpp
# End Source File
# End Group
# Begin Group "VarType_CPP"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\VGukki.cpp
# End Source File
# Begin Source File

SOURCE=.\VKagi.cpp
# End Source File
# Begin Source File

SOURCE=.\VPCV.cpp
# End Source File
# Begin Source File

SOURCE=.\VPNF.cpp
# End Source File
# Begin Source File

SOURCE=.\VRClock.cpp
# End Source File
# Begin Source File

SOURCE=.\VRenko.cpp
# End Source File
# Begin Source File

SOURCE=.\VSamsun.cpp
# End Source File
# Begin Source File

SOURCE=.\VSwing.cpp
# End Source File
# End Group
# Begin Group "LineType_CPP"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\LABRatio.cpp
# End Source File
# Begin Source File

SOURCE=.\LADLine.cpp
# End Source File
# Begin Source File

SOURCE=.\LADR.cpp
# End Source File
# Begin Source File

SOURCE=.\LADX.cpp
# End Source File
# Begin Source File

SOURCE=.\LBandWidth.cpp
# End Source File
# Begin Source File

SOURCE=.\LCCI.cpp
# End Source File
# Begin Source File

SOURCE=.\LCO.cpp
# End Source File
# Begin Source File

SOURCE=.\LCV.cpp
# End Source File
# Begin Source File

SOURCE=.\LDisparity.cpp
# End Source File
# Begin Source File

SOURCE=.\LDMI.cpp
# End Source File
# Begin Source File

SOURCE=.\LEMV.cpp
# End Source File
# Begin Source File

SOURCE=.\LFastStoch.cpp
# End Source File
# Begin Source File

SOURCE=.\LForeignMM.cpp
# End Source File
# Begin Source File

SOURCE=.\LForeignRate.cpp
# End Source File
# Begin Source File

SOURCE=.\LLFI.cpp
# End Source File
# Begin Source File

SOURCE=.\LMACD.cpp
# End Source File
# Begin Source File

SOURCE=.\LMAO.cpp
# End Source File
# Begin Source File

SOURCE=.\LMFI.cpp
# End Source File
# Begin Source File

SOURCE=.\LMgjy.cpp
# End Source File
# Begin Source File

SOURCE=.\LMomentom.cpp
# End Source File
# Begin Source File

SOURCE=.\LNCO.cpp
# End Source File
# Begin Source File

SOURCE=.\LNewPsy.cpp
# End Source File
# Begin Source File

SOURCE=.\LOBV.cpp
# End Source File
# Begin Source File

SOURCE=.\LPsy.cpp
# End Source File
# Begin Source File

SOURCE=.\LRate.cpp
# End Source File
# Begin Source File

SOURCE=.\LROC.cpp
# End Source File
# Begin Source File

SOURCE=.\LRSI.cpp
# End Source File
# Begin Source File

SOURCE=.\LSigma.cpp
# End Source File
# Begin Source File

SOURCE=.\LSlowStoch.cpp
# End Source File
# Begin Source File

SOURCE=.\LSona.cpp
# End Source File
# Begin Source File

SOURCE=.\LSonaM.cpp
# End Source File
# Begin Source File

SOURCE=.\LTrix.cpp
# End Source File
# Begin Source File

SOURCE=.\LVR.cpp
# End Source File
# Begin Source File

SOURCE=.\LWilliamR.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\axisGIndc.cpp
# End Source File
# Begin Source File

SOURCE=.\axisGIndc.def
# End Source File
# Begin Source File

SOURCE=.\axisGIndc.rc
# End Source File
# Begin Source File

SOURCE=.\BongBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\IndcBase.cpp
# End Source File
# Begin Source File

SOURCE=.\LineBase.cpp
# End Source File
# Begin Source File

SOURCE=..\MxTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VarsBase.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "BongType_H"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\BAmt.h
# End Source File
# Begin Source File

SOURCE=.\BAVol.h
# End Source File
# Begin Source File

SOURCE=.\BBalance.h
# End Source File
# Begin Source File

SOURCE=.\BBollinger.h
# End Source File
# Begin Source File

SOURCE=.\BDemark.h
# End Source File
# Begin Source File

SOURCE=.\BEnvelope.h
# End Source File
# Begin Source File

SOURCE=.\BJugaChart.h
# End Source File
# Begin Source File

SOURCE=.\BMA.h
# End Source File
# Begin Source File

SOURCE=.\BParabolic.h
# End Source File
# Begin Source File

SOURCE=.\BPivot.h
# End Source File
# Begin Source File

SOURCE=.\BVolume.h
# End Source File
# Begin Source File

SOURCE=.\LRSIWeight.h
# End Source File
# End Group
# Begin Group "VarType_H"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\VGukki.h
# End Source File
# Begin Source File

SOURCE=.\VKagi.h
# End Source File
# Begin Source File

SOURCE=.\VPCV.h
# End Source File
# Begin Source File

SOURCE=.\VPNF.h
# End Source File
# Begin Source File

SOURCE=.\VRClock.h
# End Source File
# Begin Source File

SOURCE=.\VRenko.h
# End Source File
# Begin Source File

SOURCE=.\VSamsun.h
# End Source File
# Begin Source File

SOURCE=.\VSwing.h
# End Source File
# End Group
# Begin Group "LineType_H"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\LABRatio.h
# End Source File
# Begin Source File

SOURCE=.\LADLine.h
# End Source File
# Begin Source File

SOURCE=.\LADR.h
# End Source File
# Begin Source File

SOURCE=.\LADX.h
# End Source File
# Begin Source File

SOURCE=.\LBandWidth.h
# End Source File
# Begin Source File

SOURCE=.\LCCI.h
# End Source File
# Begin Source File

SOURCE=.\LCO.h
# End Source File
# Begin Source File

SOURCE=.\LCV.h
# End Source File
# Begin Source File

SOURCE=.\LDisparity.h
# End Source File
# Begin Source File

SOURCE=.\LDMI.h
# End Source File
# Begin Source File

SOURCE=.\LEMV.h
# End Source File
# Begin Source File

SOURCE=.\LFastStoch.h
# End Source File
# Begin Source File

SOURCE=.\LForeignMM.h
# End Source File
# Begin Source File

SOURCE=.\LForeignRate.h
# End Source File
# Begin Source File

SOURCE=.\LLFI.h
# End Source File
# Begin Source File

SOURCE=.\LMACD.h
# End Source File
# Begin Source File

SOURCE=.\LMAO.h
# End Source File
# Begin Source File

SOURCE=.\LMFI.h
# End Source File
# Begin Source File

SOURCE=.\LMgjy.h
# End Source File
# Begin Source File

SOURCE=.\LMomentom.h
# End Source File
# Begin Source File

SOURCE=.\LNCO.h
# End Source File
# Begin Source File

SOURCE=.\LNewPsy.h
# End Source File
# Begin Source File

SOURCE=.\LOBV.h
# End Source File
# Begin Source File

SOURCE=.\LPsy.h
# End Source File
# Begin Source File

SOURCE=.\LRate.h
# End Source File
# Begin Source File

SOURCE=.\LROC.h
# End Source File
# Begin Source File

SOURCE=.\LRSI.h
# End Source File
# Begin Source File

SOURCE=.\LSigma.h
# End Source File
# Begin Source File

SOURCE=.\LSlowStoch.h
# End Source File
# Begin Source File

SOURCE=.\LSona.h
# End Source File
# Begin Source File

SOURCE=.\LSonaM.h
# End Source File
# Begin Source File

SOURCE=.\LTrix.h
# End Source File
# Begin Source File

SOURCE=.\LVR.h
# End Source File
# Begin Source File

SOURCE=.\LWilliamR.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BongBase.h
# End Source File
# Begin Source File

SOURCE=.\GraphMgr.h
# End Source File
# Begin Source File

SOURCE=.\IndcBase.h
# End Source File
# Begin Source File

SOURCE=.\LineBase.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VarsBase.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\axisGIndc.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

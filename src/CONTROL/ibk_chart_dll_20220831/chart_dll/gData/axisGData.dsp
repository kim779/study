# Microsoft Developer Studio Project File - Name="axisGData" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axisGData - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axisGData.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axisGData.mak" CFG="axisGData - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axisGData - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axisGData - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axisGData - Win32 Release"

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
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../../../Release/AXISGDATA.DLL"

!ELSEIF  "$(CFG)" == "axisGData - Win32 Debug"

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
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../exe/AXISGDATA.DLL" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "axisGData - Win32 Release"
# Name "axisGData - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AppABRatio.cpp
# End Source File
# Begin Source File

SOURCE=.\AppADLine.cpp
# End Source File
# Begin Source File

SOURCE=.\AppADR.cpp
# End Source File
# Begin Source File

SOURCE=.\AppAmt.cpp
# End Source File
# Begin Source File

SOURCE=.\AppBandWidth.cpp
# End Source File
# Begin Source File

SOURCE=.\AppBol.cpp
# End Source File
# Begin Source File

SOURCE=.\AppCCI.cpp
# End Source File
# Begin Source File

SOURCE=.\AppCO.cpp
# End Source File
# Begin Source File

SOURCE=.\AppCV.cpp
# End Source File
# Begin Source File

SOURCE=.\AppData.cpp
# End Source File
# Begin Source File

SOURCE=.\AppDemark.cpp
# End Source File
# Begin Source File

SOURCE=.\AppDisparity.cpp
# End Source File
# Begin Source File

SOURCE=.\AppDMI.cpp
# End Source File
# Begin Source File

SOURCE=.\AppEMV.cpp
# End Source File
# Begin Source File

SOURCE=.\AppEnv.cpp
# End Source File
# Begin Source File

SOURCE=.\AppForeign.cpp
# End Source File
# Begin Source File

SOURCE=.\AppLFI.cpp
# End Source File
# Begin Source File

SOURCE=.\AppMA.cpp
# End Source File
# Begin Source File

SOURCE=.\AppMACD.cpp
# End Source File
# Begin Source File

SOURCE=.\AppMAO.cpp
# End Source File
# Begin Source File

SOURCE=.\AppMFI.cpp
# End Source File
# Begin Source File

SOURCE=.\AppMgjy.cpp
# End Source File
# Begin Source File

SOURCE=.\AppMmt.cpp
# End Source File
# Begin Source File

SOURCE=.\AppNCO.cpp
# End Source File
# Begin Source File

SOURCE=.\AppNet.cpp
# End Source File
# Begin Source File

SOURCE=.\AppNPsy.cpp
# End Source File
# Begin Source File

SOURCE=.\AppOBV.cpp
# End Source File
# Begin Source File

SOURCE=.\AppParabolic.cpp
# End Source File
# Begin Source File

SOURCE=.\AppPivot.cpp
# End Source File
# Begin Source File

SOURCE=.\AppPsy.cpp
# End Source File
# Begin Source File

SOURCE=.\AppRate.cpp
# End Source File
# Begin Source File

SOURCE=.\AppROC.cpp
# End Source File
# Begin Source File

SOURCE=.\AppRSI.cpp
# End Source File
# Begin Source File

SOURCE=.\AppRSIWeight.cpp
# End Source File
# Begin Source File

SOURCE=.\AppSigma.cpp
# End Source File
# Begin Source File

SOURCE=.\AppSona.cpp
# End Source File
# Begin Source File

SOURCE=.\AppSonaM.cpp
# End Source File
# Begin Source File

SOURCE=.\AppStoch.cpp
# End Source File
# Begin Source File

SOURCE=.\AppTrix.cpp
# End Source File
# Begin Source File

SOURCE=.\AppVR.cpp
# End Source File
# Begin Source File

SOURCE=.\AppWillR.cpp
# End Source File
# Begin Source File

SOURCE=.\axisGData.cpp
# End Source File
# Begin Source File

SOURCE=.\axisGData.def
# End Source File
# Begin Source File

SOURCE=.\axisGData.rc
# End Source File
# Begin Source File

SOURCE=.\DataMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\GrpBasic.cpp
# End Source File
# Begin Source File

SOURCE=.\LineData.cpp
# End Source File
# Begin Source File

SOURCE=..\MxTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\OrgData.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AppABRatio.h
# End Source File
# Begin Source File

SOURCE=.\AppADLine.h
# End Source File
# Begin Source File

SOURCE=.\AppADR.h
# End Source File
# Begin Source File

SOURCE=.\AppAmt.h
# End Source File
# Begin Source File

SOURCE=.\AppBandWidth.h
# End Source File
# Begin Source File

SOURCE=.\AppBol.h
# End Source File
# Begin Source File

SOURCE=.\AppCCI.h
# End Source File
# Begin Source File

SOURCE=.\AppCO.h
# End Source File
# Begin Source File

SOURCE=.\AppCV.h
# End Source File
# Begin Source File

SOURCE=.\AppData.h
# End Source File
# Begin Source File

SOURCE=.\AppDemark.h
# End Source File
# Begin Source File

SOURCE=.\AppDisparity.h
# End Source File
# Begin Source File

SOURCE=.\AppDMI.h
# End Source File
# Begin Source File

SOURCE=.\AppEMV.h
# End Source File
# Begin Source File

SOURCE=.\AppEnv.h
# End Source File
# Begin Source File

SOURCE=.\AppForeign.h
# End Source File
# Begin Source File

SOURCE=.\AppLFI.h
# End Source File
# Begin Source File

SOURCE=.\AppMA.h
# End Source File
# Begin Source File

SOURCE=.\AppMACD.h
# End Source File
# Begin Source File

SOURCE=.\AppMAO.h
# End Source File
# Begin Source File

SOURCE=.\AppMFI.h
# End Source File
# Begin Source File

SOURCE=.\AppMgjy.h
# End Source File
# Begin Source File

SOURCE=.\AppMmt.h
# End Source File
# Begin Source File

SOURCE=.\AppNCO.h
# End Source File
# Begin Source File

SOURCE=.\AppNet.h
# End Source File
# Begin Source File

SOURCE=.\AppNPsy.h
# End Source File
# Begin Source File

SOURCE=.\AppOBV.h
# End Source File
# Begin Source File

SOURCE=.\AppParabolic.h
# End Source File
# Begin Source File

SOURCE=.\AppPivot.h
# End Source File
# Begin Source File

SOURCE=.\AppPsy.h
# End Source File
# Begin Source File

SOURCE=.\AppRate.h
# End Source File
# Begin Source File

SOURCE=.\AppROC.h
# End Source File
# Begin Source File

SOURCE=.\AppRSI.h
# End Source File
# Begin Source File

SOURCE=.\AppRSIWeight.h
# End Source File
# Begin Source File

SOURCE=.\AppSigma.h
# End Source File
# Begin Source File

SOURCE=.\AppSona.h
# End Source File
# Begin Source File

SOURCE=.\AppSonaM.h
# End Source File
# Begin Source File

SOURCE=.\AppStoch.h
# End Source File
# Begin Source File

SOURCE=.\AppTrix.h
# End Source File
# Begin Source File

SOURCE=.\AppVR.h
# End Source File
# Begin Source File

SOURCE=.\AppWillR.h
# End Source File
# Begin Source File

SOURCE=.\DataMgr.h
# End Source File
# Begin Source File

SOURCE=.\GrpBasic.h
# End Source File
# Begin Source File

SOURCE=.\LineData.h
# End Source File
# Begin Source File

SOURCE=..\MxTrace.h
# End Source File
# Begin Source File

SOURCE=.\OrgData.h
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

SOURCE=.\res\axisGData.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

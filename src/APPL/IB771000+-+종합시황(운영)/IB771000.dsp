# Microsoft Developer Studio Project File - Name="IB771000" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=IB771000 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IB771000.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB771000.mak" CFG="IB771000 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB771000 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB771000 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IB771000 - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /Zi /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 msimg32.lib ../../control/fx_misc/release/fx_misc.lib winmm.lib ../../dll/sm/Release/axissm.lib Msimg32.lib /nologo /subsystem:windows /dll /incremental:yes /map /debug /machine:I386 /out:"../../../release/IB771000.dll" /MAPINFO:EXPORTS /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 msimg32.lib ../../control/fx_misc/debug/fx_misc.lib imm32.lib winmm.lib ../../dll/sm/Debug/axissm.lib Msimg32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../dev/IB771000.DLL" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no /map /nodefaultlib

!ENDIF 

# Begin Target

# Name "IB771000 - Win32 Release"
# Name "IB771000 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BkUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ColoredListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\datap.cpp
# End Source File
# Begin Source File

SOURCE=.\excel.cpp
# End Source File
# Begin Source File

SOURCE=.\fxTab.cpp
# End Source File
# Begin Source File

SOURCE=.\GubnConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IB771000.cpp
# End Source File
# Begin Source File

SOURCE=.\IB771000.def
# End Source File
# Begin Source File

SOURCE=.\IB771000.rc
# End Source File
# Begin Source File

SOURCE=.\ImageTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MapWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MarketDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Markup.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\STimeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeOptionsCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Util.cpp
# End Source File
# Begin Source File

SOURCE=.\WxEdit.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BkUtil.h
# End Source File
# Begin Source File

SOURCE=.\ColorButton.h
# End Source File
# Begin Source File

SOURCE=.\ColoredListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ColourPopup.h
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\datap.h
# End Source File
# Begin Source File

SOURCE=.\excel.h
# End Source File
# Begin Source File

SOURCE=.\fxTab.h
# End Source File
# Begin Source File

SOURCE=.\GubnConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\IB771000.h
# End Source File
# Begin Source File

SOURCE=.\ImageTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MapWnd.h
# End Source File
# Begin Source File

SOURCE=.\MarketDlg.h
# End Source File
# Begin Source File

SOURCE=.\Markup.h
# End Source File
# Begin Source File

SOURCE=.\OptionWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\shareMsg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\STimeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TipDlg.h
# End Source File
# Begin Source File

SOURCE=.\tr_data.h
# End Source File
# Begin Source File

SOURCE=.\TreeOptionsCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Util.h
# End Source File
# Begin Source File

SOURCE=.\WxEdit.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\7710_bg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\border.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bullet_disk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\IB771000.rc2
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\medal_gold_1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\medal_gold_2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\medal_gold_3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tREE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree5.bmp
# End Source File
# Begin Source File

SOURCE=.\tree_ctr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree_ctrl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treectl.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Ω√»≤_TREE.bmp"
# End Source File
# End Group
# Begin Group "CTRL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AxBrower.cpp
# End Source File
# Begin Source File

SOURCE=.\AxBrower.h
# End Source File
# Begin Source File

SOURCE=.\DropBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\DropBtn.h
# End Source File
# Begin Source File

SOURCE=.\SButton.cpp
# End Source File
# Begin Source File

SOURCE=.\SButton.h
# End Source File
# Begin Source File

SOURCE=.\SComboBox1.cpp
# End Source File
# Begin Source File

SOURCE=.\SComboBox1.h
# End Source File
# Begin Source File

SOURCE=.\SEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SEdit.h
# End Source File
# Begin Source File

SOURCE=.\SHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SListCtrl.h
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
# Begin Group "DLG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\KeywordConDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KeywordConDlg.h
# End Source File
# Begin Source File

SOURCE=.\MapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MapDlg.h
# End Source File
# Begin Source File

SOURCE=.\SearchWordSetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchWordSetDlg.h
# End Source File
# End Group
# Begin Group "PRINT"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

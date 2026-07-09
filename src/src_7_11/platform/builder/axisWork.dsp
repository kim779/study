# Microsoft Developer Studio Project File - Name="axisWork" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=axisWork - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axisWork.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axisWork.mak" CFG="axisWork - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axisWork - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "axisWork - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axisWork - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ../dll/lib/release/axislib.lib awWcc/release/awWcc.lib awSock/release/awSock.lib awTool/release/awTool.lib awDlg/release/awDlg.lib awBuild/release/awbuild.lib awObject/release/awObject.lib awUser/release/awUser.lib awCommon/release/awcommon.lib Imm32.lib htmlhelp.lib Ws2_32.lib Msimg32.lib /nologo /subsystem:windows /map /machine:I386 /out:"../../release/AxisBuilder.exe"

!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../dll/lib/debug/axislib.lib awWcc/debug/awWcc.lib awSock/debug/awSock.lib awTool/debug/awTool.lib awDlg/debug/awDlg.lib awBuild/debug/awbuild.lib awObject/debug/awObject.lib awUser/debug/awuser.lib awCommon/debug/awcommon.lib Imm32.lib htmlhelp.lib Ws2_32.lib Msimg32.lib /nologo /stack:0x400 /subsystem:windows /debug /machine:I386 /out:"../../exe/AxisBuilder.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "axisWork - Win32 Release"
# Name "axisWork - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Components"

# PROP Default_Filter ""
# Begin Group "CrystalEdit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Components\crysedit_src\CCrystalEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\crysedit_src\CCrystalTextBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\crysedit_src\CCrystalTextView.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\crysedit_src\CCrystalTextView2.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\crysedit_src\CEditReplaceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\crysedit_src\CFindTextDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\crysedit_src\VBScriptEdit.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\AutoCmplDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\CheckComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\ComboBoxBold.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\FileTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Group.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\ListBoxST.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadThread.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\MDITabs.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\MultiSelTree.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\NFBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\ResultEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\scbarg.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\ServerTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\sizecbar.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\symboltreectrl.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Components\Tips.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\ToolTipListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\TrueColorToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\TWScriptEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\WEdit.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\axisWork.cpp
# End Source File
# Begin Source File

SOURCE=.\axisWork.rc
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PopFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\PreViewBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertiesBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptBar.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectionBar.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TraceBar.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkSpaceBar.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Components_head"

# PROP Default_Filter ""
# Begin Group "CrystalEdit_head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Components\crysedit_src\CCrystalEditView.h
# End Source File
# Begin Source File

SOURCE=.\Components\crysedit_src\CCrystalTextBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Components\crysedit_src\CCrystalTextView.h
# End Source File
# Begin Source File

SOURCE=.\Components\crysedit_src\CEditReplaceDlg.h
# End Source File
# Begin Source File

SOURCE=.\Components\crysedit_src\CFindTextDlg.h
# End Source File
# Begin Source File

SOURCE=.\Components\crysedit_src\VBScriptEdit.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AutoCmplDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoToolTip.h
# End Source File
# Begin Source File

SOURCE=.\Components\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\BtnCombo.h
# End Source File
# Begin Source File

SOURCE=.\Components\CheckComboBox.h
# End Source File
# Begin Source File

SOURCE=.\Components\ComboBoxBold.h
# End Source File
# Begin Source File

SOURCE=.\ControlTracker.h
# End Source File
# Begin Source File

SOURCE=.\Components\FileTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Group.h
# End Source File
# Begin Source File

SOURCE=.\Components\ListBoxST.h
# End Source File
# Begin Source File

SOURCE=.\LoadThread.h
# End Source File
# Begin Source File

SOURCE=.\Components\MDITabs.h
# End Source File
# Begin Source File

SOURCE=.\Components\MultiSelTree.h
# End Source File
# Begin Source File

SOURCE=.\NFBtn.h
# End Source File
# Begin Source File

SOURCE=.\Components\ResultEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Components\scbarg.h
# End Source File
# Begin Source File

SOURCE=.\Components\ServerTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Components\sizecbar.h
# End Source File
# Begin Source File

SOURCE=.\Components\Splash.h
# End Source File
# Begin Source File

SOURCE=.\Components\symboltreectrl.h
# End Source File
# Begin Source File

SOURCE=.\Components\Tips.h
# End Source File
# Begin Source File

SOURCE=.\Components\ToolTipListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Components\TrueColorToolBar.h
# End Source File
# Begin Source File

SOURCE=.\Components\TWScriptEdit.h
# End Source File
# Begin Source File

SOURCE=.\Components\WEdit.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\axisWork.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ChildView.h
# End Source File
# Begin Source File

SOURCE=.\keywords.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\mapvar.h
# End Source File
# Begin Source File

SOURCE=.\PopFrame.h
# End Source File
# Begin Source File

SOURCE=.\PreViewBar.h
# End Source File
# Begin Source File

SOURCE=.\PreviewWnd.h
# End Source File
# Begin Source File

SOURCE=.\PropertiesBar.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScriptBar.h
# End Source File
# Begin Source File

SOURCE=.\SelectionBar.h
# End Source File
# Begin Source File

SOURCE=.\split.hxx
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TraceBar.h
# End Source File
# Begin Source File

SOURCE=.\WorkSpaceBar.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\alignbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ALIGNBAR_disable.BMP
# End Source File
# Begin Source File

SOURCE=.\res\axis_builder_32.ico
# End Source File
# Begin Source File

SOURCE=.\res\axis_workstudio_32.ico
# End Source File
# Begin Source File

SOURCE=.\res\AXISUPDATE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\axisWork.ico
# End Source File
# Begin Source File

SOURCE=.\res\axisWork.rc2
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00016.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Controls.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\drawbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\drawbar_disable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\EXBAR_disable.BMP
# End Source File
# Begin Source File

SOURCE=.\res\ftnetwork.bmp
# End Source File
# Begin Source File

SOURCE=.\res\list.bmp
# End Source File
# Begin Source File

SOURCE=.\res\map.ico
# End Source File
# Begin Source File

SOURCE=.\res\map2.ico
# End Source File
# Begin Source File

SOURCE=.\res\maponly.bmp
# End Source File
# Begin Source File

SOURCE=.\mg_cur.cur
# End Source File
# Begin Source File

SOURCE=.\mg_icons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\procedures.ico
# End Source File
# Begin Source File

SOURCE=".\res\Template-1.ico"
# End Source File
# Begin Source File

SOURCE=.\res\Template.ico
# End Source File
# Begin Source File

SOURCE=.\res\tfdropcopy.cur
# End Source File
# Begin Source File

SOURCE=.\res\tfnodropcopy.cur
# End Source File
# Begin Source File

SOURCE=.\res\tfnodropmove.cur
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar_disable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree2depth.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treeview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\workshop_s1.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

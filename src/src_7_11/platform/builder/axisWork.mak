# Microsoft Developer Studio Generated NMAKE File, Based on axisWork.dsp
!IF "$(CFG)" == ""
CFG=axisWork - Win32 Debug
!MESSAGE No configuration specified. Defaulting to axisWork - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "axisWork - Win32 Release" && "$(CFG)" != "axisWork - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axisWork - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\release\AxisBuilder.exe"


CLEAN :
	-@erase "$(INTDIR)\AutoCmplDlg.obj"
	-@erase "$(INTDIR)\AutoToolTip.obj"
	-@erase "$(INTDIR)\axisWork.obj"
	-@erase "$(INTDIR)\axisWork.pch"
	-@erase "$(INTDIR)\axisWork.res"
	-@erase "$(INTDIR)\BCMenu.obj"
	-@erase "$(INTDIR)\BtnCombo.obj"
	-@erase "$(INTDIR)\CheckComboBox.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\ChildView.obj"
	-@erase "$(INTDIR)\ComboBoxBold.obj"
	-@erase "$(INTDIR)\ControlTracker.obj"
	-@erase "$(INTDIR)\FileTreeCtrl.obj"
	-@erase "$(INTDIR)\Group.obj"
	-@erase "$(INTDIR)\ListBoxST.obj"
	-@erase "$(INTDIR)\LoadThread.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MDITabs.obj"
	-@erase "$(INTDIR)\MyFindDlg.obj"
	-@erase "$(INTDIR)\NFBtn.obj"
	-@erase "$(INTDIR)\PreViewBar.obj"
	-@erase "$(INTDIR)\PreviewWnd.obj"
	-@erase "$(INTDIR)\PropertiesBar.obj"
	-@erase "$(INTDIR)\ResultEditCtrl.obj"
	-@erase "$(INTDIR)\scbarg.obj"
	-@erase "$(INTDIR)\ScriptBar.obj"
	-@erase "$(INTDIR)\ServerTreeCtrl.obj"
	-@erase "$(INTDIR)\sizecbar.obj"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\ToolTipListCtrl.obj"
	-@erase "$(INTDIR)\TraceBar.obj"
	-@erase "$(INTDIR)\TrueColorToolBar.obj"
	-@erase "$(INTDIR)\TWScriptEdit.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WEdit.obj"
	-@erase "$(INTDIR)\WorkSpaceBar.obj"
	-@erase "$(OUTDIR)\AxisBuilder.map"
	-@erase "..\..\release\AxisBuilder.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\axisWork.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\axisWork.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\axisWork.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../dll/lib/release/axislib.lib awWcc/release/awWcc.lib awSock/release/awSock.lib awTool/release/awTool.lib awDlg/release/awDlg.lib awBuild/release/awbuild.lib awObject/release/awObject.lib awUser/release/awUser.lib awCommon/release/awcommon.lib Imm32.lib htmlhelp.lib awCnv/release/awCnv.lib Ws2_32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\AxisBuilder.pdb" /map:"$(INTDIR)\AxisBuilder.map" /machine:I386 /out:"../../release/AxisBuilder.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AutoCmplDlg.obj" \
	"$(INTDIR)\AutoToolTip.obj" \
	"$(INTDIR)\BCMenu.obj" \
	"$(INTDIR)\BtnCombo.obj" \
	"$(INTDIR)\CheckComboBox.obj" \
	"$(INTDIR)\ComboBoxBold.obj" \
	"$(INTDIR)\ControlTracker.obj" \
	"$(INTDIR)\FileTreeCtrl.obj" \
	"$(INTDIR)\Group.obj" \
	"$(INTDIR)\ListBoxST.obj" \
	"$(INTDIR)\LoadThread.obj" \
	"$(INTDIR)\MDITabs.obj" \
	"$(INTDIR)\NFBtn.obj" \
	"$(INTDIR)\ResultEditCtrl.obj" \
	"$(INTDIR)\scbarg.obj" \
	"$(INTDIR)\ServerTreeCtrl.obj" \
	"$(INTDIR)\sizecbar.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\ToolTipListCtrl.obj" \
	"$(INTDIR)\TrueColorToolBar.obj" \
	"$(INTDIR)\TWScriptEdit.obj" \
	"$(INTDIR)\WEdit.obj" \
	"$(INTDIR)\axisWork.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\ChildView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MyFindDlg.obj" \
	"$(INTDIR)\PreViewBar.obj" \
	"$(INTDIR)\PreviewWnd.obj" \
	"$(INTDIR)\PropertiesBar.obj" \
	"$(INTDIR)\ScriptBar.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TraceBar.obj" \
	"$(INTDIR)\WorkSpaceBar.obj" \
	"$(INTDIR)\axisWork.res"

"..\..\release\AxisBuilder.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\debug\axiswork.exe" "$(OUTDIR)\axisWork.bsc"


CLEAN :
	-@erase "$(INTDIR)\AutoCmplDlg.obj"
	-@erase "$(INTDIR)\AutoCmplDlg.sbr"
	-@erase "$(INTDIR)\AutoToolTip.obj"
	-@erase "$(INTDIR)\AutoToolTip.sbr"
	-@erase "$(INTDIR)\axisWork.obj"
	-@erase "$(INTDIR)\axisWork.pch"
	-@erase "$(INTDIR)\axisWork.res"
	-@erase "$(INTDIR)\axisWork.sbr"
	-@erase "$(INTDIR)\BCMenu.obj"
	-@erase "$(INTDIR)\BCMenu.sbr"
	-@erase "$(INTDIR)\BtnCombo.obj"
	-@erase "$(INTDIR)\BtnCombo.sbr"
	-@erase "$(INTDIR)\CheckComboBox.obj"
	-@erase "$(INTDIR)\CheckComboBox.sbr"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\ChildFrm.sbr"
	-@erase "$(INTDIR)\ChildView.obj"
	-@erase "$(INTDIR)\ChildView.sbr"
	-@erase "$(INTDIR)\ComboBoxBold.obj"
	-@erase "$(INTDIR)\ComboBoxBold.sbr"
	-@erase "$(INTDIR)\ControlTracker.obj"
	-@erase "$(INTDIR)\ControlTracker.sbr"
	-@erase "$(INTDIR)\FileTreeCtrl.obj"
	-@erase "$(INTDIR)\FileTreeCtrl.sbr"
	-@erase "$(INTDIR)\Group.obj"
	-@erase "$(INTDIR)\Group.sbr"
	-@erase "$(INTDIR)\ListBoxST.obj"
	-@erase "$(INTDIR)\ListBoxST.sbr"
	-@erase "$(INTDIR)\LoadThread.obj"
	-@erase "$(INTDIR)\LoadThread.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MDITabs.obj"
	-@erase "$(INTDIR)\MDITabs.sbr"
	-@erase "$(INTDIR)\MyFindDlg.obj"
	-@erase "$(INTDIR)\MyFindDlg.sbr"
	-@erase "$(INTDIR)\NFBtn.obj"
	-@erase "$(INTDIR)\NFBtn.sbr"
	-@erase "$(INTDIR)\PreViewBar.obj"
	-@erase "$(INTDIR)\PreViewBar.sbr"
	-@erase "$(INTDIR)\PreviewWnd.obj"
	-@erase "$(INTDIR)\PreviewWnd.sbr"
	-@erase "$(INTDIR)\PropertiesBar.obj"
	-@erase "$(INTDIR)\PropertiesBar.sbr"
	-@erase "$(INTDIR)\ResultEditCtrl.obj"
	-@erase "$(INTDIR)\ResultEditCtrl.sbr"
	-@erase "$(INTDIR)\scbarg.obj"
	-@erase "$(INTDIR)\scbarg.sbr"
	-@erase "$(INTDIR)\ScriptBar.obj"
	-@erase "$(INTDIR)\ScriptBar.sbr"
	-@erase "$(INTDIR)\ServerTreeCtrl.obj"
	-@erase "$(INTDIR)\ServerTreeCtrl.sbr"
	-@erase "$(INTDIR)\sizecbar.obj"
	-@erase "$(INTDIR)\sizecbar.sbr"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\Splash.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\ToolTipListCtrl.obj"
	-@erase "$(INTDIR)\ToolTipListCtrl.sbr"
	-@erase "$(INTDIR)\TraceBar.obj"
	-@erase "$(INTDIR)\TraceBar.sbr"
	-@erase "$(INTDIR)\TrueColorToolBar.obj"
	-@erase "$(INTDIR)\TrueColorToolBar.sbr"
	-@erase "$(INTDIR)\TWScriptEdit.obj"
	-@erase "$(INTDIR)\TWScriptEdit.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WEdit.obj"
	-@erase "$(INTDIR)\WEdit.sbr"
	-@erase "$(INTDIR)\WorkSpaceBar.obj"
	-@erase "$(INTDIR)\WorkSpaceBar.sbr"
	-@erase "$(OUTDIR)\axisWork.bsc"
	-@erase "$(OUTDIR)\axiswork.pdb"
	-@erase "..\..\debug\axiswork.exe"
	-@erase "..\..\debug\axiswork.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\axisWork.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\axisWork.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\axisWork.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AutoCmplDlg.sbr" \
	"$(INTDIR)\AutoToolTip.sbr" \
	"$(INTDIR)\BCMenu.sbr" \
	"$(INTDIR)\BtnCombo.sbr" \
	"$(INTDIR)\CheckComboBox.sbr" \
	"$(INTDIR)\ComboBoxBold.sbr" \
	"$(INTDIR)\ControlTracker.sbr" \
	"$(INTDIR)\FileTreeCtrl.sbr" \
	"$(INTDIR)\Group.sbr" \
	"$(INTDIR)\ListBoxST.sbr" \
	"$(INTDIR)\LoadThread.sbr" \
	"$(INTDIR)\MDITabs.sbr" \
	"$(INTDIR)\NFBtn.sbr" \
	"$(INTDIR)\ResultEditCtrl.sbr" \
	"$(INTDIR)\scbarg.sbr" \
	"$(INTDIR)\ServerTreeCtrl.sbr" \
	"$(INTDIR)\sizecbar.sbr" \
	"$(INTDIR)\Splash.sbr" \
	"$(INTDIR)\ToolTipListCtrl.sbr" \
	"$(INTDIR)\TrueColorToolBar.sbr" \
	"$(INTDIR)\TWScriptEdit.sbr" \
	"$(INTDIR)\WEdit.sbr" \
	"$(INTDIR)\axisWork.sbr" \
	"$(INTDIR)\ChildFrm.sbr" \
	"$(INTDIR)\ChildView.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\MyFindDlg.sbr" \
	"$(INTDIR)\PreViewBar.sbr" \
	"$(INTDIR)\PreviewWnd.sbr" \
	"$(INTDIR)\PropertiesBar.sbr" \
	"$(INTDIR)\ScriptBar.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TraceBar.sbr" \
	"$(INTDIR)\WorkSpaceBar.sbr"

"$(OUTDIR)\axisWork.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=../dll/lib/debug/axislib.lib awWcc/debug/awWcc.lib awSock/debug/awSock.lib awTool/debug/awTool.lib awDlg/debug/awDlg.lib awBuild/debug/awbuild.lib awObject/debug/awObject.lib awUser/debug/awuser.lib awCommon/debug/awcommon.lib Imm32.lib htmlhelp.lib awCnv/debug/awCnv.lib Ws2_32.lib /nologo /stack:0x400 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\axiswork.pdb" /debug /machine:I386 /out:"../../debug/axiswork.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AutoCmplDlg.obj" \
	"$(INTDIR)\AutoToolTip.obj" \
	"$(INTDIR)\BCMenu.obj" \
	"$(INTDIR)\BtnCombo.obj" \
	"$(INTDIR)\CheckComboBox.obj" \
	"$(INTDIR)\ComboBoxBold.obj" \
	"$(INTDIR)\ControlTracker.obj" \
	"$(INTDIR)\FileTreeCtrl.obj" \
	"$(INTDIR)\Group.obj" \
	"$(INTDIR)\ListBoxST.obj" \
	"$(INTDIR)\LoadThread.obj" \
	"$(INTDIR)\MDITabs.obj" \
	"$(INTDIR)\NFBtn.obj" \
	"$(INTDIR)\ResultEditCtrl.obj" \
	"$(INTDIR)\scbarg.obj" \
	"$(INTDIR)\ServerTreeCtrl.obj" \
	"$(INTDIR)\sizecbar.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\ToolTipListCtrl.obj" \
	"$(INTDIR)\TrueColorToolBar.obj" \
	"$(INTDIR)\TWScriptEdit.obj" \
	"$(INTDIR)\WEdit.obj" \
	"$(INTDIR)\axisWork.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\ChildView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MyFindDlg.obj" \
	"$(INTDIR)\PreViewBar.obj" \
	"$(INTDIR)\PreviewWnd.obj" \
	"$(INTDIR)\PropertiesBar.obj" \
	"$(INTDIR)\ScriptBar.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TraceBar.obj" \
	"$(INTDIR)\WorkSpaceBar.obj" \
	"$(INTDIR)\axisWork.res"

"..\..\debug\axiswork.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("axisWork.dep")
!INCLUDE "axisWork.dep"
!ELSE 
!MESSAGE Warning: cannot find "axisWork.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "axisWork - Win32 Release" || "$(CFG)" == "axisWork - Win32 Debug"
SOURCE=.\AutoCmplDlg.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\AutoCmplDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\AutoCmplDlg.obj"	"$(INTDIR)\AutoCmplDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\AutoToolTip.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\AutoToolTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\AutoToolTip.obj"	"$(INTDIR)\AutoToolTip.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\Components\BCMenu.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\BCMenu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\BCMenu.obj"	"$(INTDIR)\BCMenu.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\BtnCombo.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\BtnCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\BtnCombo.obj"	"$(INTDIR)\BtnCombo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\Components\CheckComboBox.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\CheckComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\CheckComboBox.obj"	"$(INTDIR)\CheckComboBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Components\ComboBoxBold.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\ComboBoxBold.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\ComboBoxBold.obj"	"$(INTDIR)\ComboBoxBold.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\ControlTracker.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\ControlTracker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\ControlTracker.obj"	"$(INTDIR)\ControlTracker.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\Components\FileTreeCtrl.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\FileTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\FileTreeCtrl.obj"	"$(INTDIR)\FileTreeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Group.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\Group.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\Group.obj"	"$(INTDIR)\Group.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\Components\ListBoxST.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\ListBoxST.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\ListBoxST.obj"	"$(INTDIR)\ListBoxST.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\LoadThread.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\LoadThread.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\LoadThread.obj"	"$(INTDIR)\LoadThread.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\Components\MDITabs.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\MDITabs.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\MDITabs.obj"	"$(INTDIR)\MDITabs.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\NFBtn.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\NFBtn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\NFBtn.obj"	"$(INTDIR)\NFBtn.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\Components\ResultEditCtrl.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\ResultEditCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\ResultEditCtrl.obj"	"$(INTDIR)\ResultEditCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Components\scbarg.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\scbarg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\scbarg.obj"	"$(INTDIR)\scbarg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Components\ServerTreeCtrl.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\ServerTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\ServerTreeCtrl.obj"	"$(INTDIR)\ServerTreeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Components\sizecbar.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\sizecbar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\sizecbar.obj"	"$(INTDIR)\sizecbar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Components\Splash.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\Splash.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\Splash.obj"	"$(INTDIR)\Splash.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Components\ToolTipListCtrl.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\ToolTipListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\ToolTipListCtrl.obj"	"$(INTDIR)\ToolTipListCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Components\TrueColorToolBar.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\TrueColorToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\TrueColorToolBar.obj"	"$(INTDIR)\TrueColorToolBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Components\TWScriptEdit.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\TWScriptEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\TWScriptEdit.obj"	"$(INTDIR)\TWScriptEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Components\WEdit.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\WEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\WEdit.obj"	"$(INTDIR)\WEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\axisWork.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\axisWork.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\axisWork.obj"	"$(INTDIR)\axisWork.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\axisWork.rc

"$(INTDIR)\axisWork.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ChildFrm.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\ChildFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\ChildFrm.obj"	"$(INTDIR)\ChildFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\ChildView.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\ChildView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\ChildView.obj"	"$(INTDIR)\ChildView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\MyFindDlg.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\MyFindDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\MyFindDlg.obj"	"$(INTDIR)\MyFindDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\PreViewBar.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\PreViewBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\PreViewBar.obj"	"$(INTDIR)\PreViewBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\PreviewWnd.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\PreviewWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\PreviewWnd.obj"	"$(INTDIR)\PreviewWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\PropertiesBar.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\PropertiesBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\PropertiesBar.obj"	"$(INTDIR)\PropertiesBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\ScriptBar.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\ScriptBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\ScriptBar.obj"	"$(INTDIR)\ScriptBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\axisWork.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\axisWork.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\axisWork.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\axisWork.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TraceBar.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\TraceBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\TraceBar.obj"	"$(INTDIR)\TraceBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 

SOURCE=.\WorkSpaceBar.cpp

!IF  "$(CFG)" == "axisWork - Win32 Release"


"$(INTDIR)\WorkSpaceBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ELSEIF  "$(CFG)" == "axisWork - Win32 Debug"


"$(INTDIR)\WorkSpaceBar.obj"	"$(INTDIR)\WorkSpaceBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axisWork.pch"


!ENDIF 


!ENDIF 


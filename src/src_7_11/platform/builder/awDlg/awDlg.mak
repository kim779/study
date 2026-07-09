# Microsoft Developer Studio Generated NMAKE File, Based on awDlg.dsp
!IF "$(CFG)" == ""
CFG=awDlg - Win32 Debug
!MESSAGE No configuration specified. Defaulting to awDlg - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "awDlg - Win32 Release" && "$(CFG)" != "awDlg - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "awDlg.mak" CFG="awDlg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "awDlg - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "awDlg - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "awDlg - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\release\awdlg.dll"


CLEAN :
	-@erase "$(INTDIR)\addrDlg.obj"
	-@erase "$(INTDIR)\attrDlg.obj"
	-@erase "$(INTDIR)\awDlg.obj"
	-@erase "$(INTDIR)\awDlg.pch"
	-@erase "$(INTDIR)\awDlg.res"
	-@erase "$(INTDIR)\BATCH.OBJ"
	-@erase "$(INTDIR)\BtnCombo.obj"
	-@erase "$(INTDIR)\cbItem.obj"
	-@erase "$(INTDIR)\cellEdit.obj"
	-@erase "$(INTDIR)\colorDlg.obj"
	-@erase "$(INTDIR)\ControlDlg.obj"
	-@erase "$(INTDIR)\coRGB.obj"
	-@erase "$(INTDIR)\EditItem.obj"
	-@erase "$(INTDIR)\EditList.obj"
	-@erase "$(INTDIR)\EditOrder.obj"
	-@erase "$(INTDIR)\folderBrowser.obj"
	-@erase "$(INTDIR)\format.obj"
	-@erase "$(INTDIR)\GridColumnDlg.obj"
	-@erase "$(INTDIR)\HBBtn.obj"
	-@erase "$(INTDIR)\InputKeyDlg.obj"
	-@erase "$(INTDIR)\ItemCombo.obj"
	-@erase "$(INTDIR)\LineDlg.obj"
	-@erase "$(INTDIR)\ListGrid.obj"
	-@erase "$(INTDIR)\Login.obj"
	-@erase "$(INTDIR)\LoginSetDlg.obj"
	-@erase "$(INTDIR)\mapOption.obj"
	-@erase "$(INTDIR)\MGColumnDlg.obj"
	-@erase "$(INTDIR)\NFBtn.obj"
	-@erase "$(INTDIR)\Pane.obj"
	-@erase "$(INTDIR)\PopListCtrl.obj"
	-@erase "$(INTDIR)\Preview.obj"
	-@erase "$(INTDIR)\PreviewRun.obj"
	-@erase "$(INTDIR)\PublicSetDlg.obj"
	-@erase "$(INTDIR)\saveDlg.obj"
	-@erase "$(INTDIR)\SepWnd.obj"
	-@erase "$(INTDIR)\SettingDlg.obj"
	-@erase "$(INTDIR)\SingleProperty.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\upDlg.obj"
	-@erase "$(INTDIR)\upTmplDlg.obj"
	-@erase "$(INTDIR)\uTabPage.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\XFontDialog.obj"
	-@erase "$(INTDIR)\xTreeCtrl.obj"
	-@erase "$(OUTDIR)\awdlg.exp"
	-@erase "$(OUTDIR)\awdlg.lib"
	-@erase "..\..\..\release\awdlg.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\awDlg.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\awDlg.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\awDlg.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../awSock/release/awsock.lib ../awBuild/release/awbuild.lib ../awWcc/release/awwcc.lib ../awCommon/release/awcommon.lib ../../dll/lib/release/axislib.lib ../awObject/release/awObject.lib ../awUser/release/awUser.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\awdlg.pdb" /machine:I386 /def:".\awDlg.def" /out:"../../../release/awdlg.dll" /implib:"$(OUTDIR)\awdlg.lib" 
DEF_FILE= \
	".\awDlg.def"
LINK32_OBJS= \
	"$(INTDIR)\BtnCombo.obj" \
	"$(INTDIR)\EditItem.obj" \
	"$(INTDIR)\EditList.obj" \
	"$(INTDIR)\ItemCombo.obj" \
	"$(INTDIR)\ListGrid.obj" \
	"$(INTDIR)\NFBtn.obj" \
	"$(INTDIR)\PopListCtrl.obj" \
	"$(INTDIR)\SepWnd.obj" \
	"$(INTDIR)\XFontDialog.obj" \
	"$(INTDIR)\xTreeCtrl.obj" \
	"$(INTDIR)\addrDlg.obj" \
	"$(INTDIR)\attrDlg.obj" \
	"$(INTDIR)\awDlg.obj" \
	"$(INTDIR)\BATCH.OBJ" \
	"$(INTDIR)\cbItem.obj" \
	"$(INTDIR)\cellEdit.obj" \
	"$(INTDIR)\colorDlg.obj" \
	"$(INTDIR)\ControlDlg.obj" \
	"$(INTDIR)\coRGB.obj" \
	"$(INTDIR)\EditOrder.obj" \
	"$(INTDIR)\folderBrowser.obj" \
	"$(INTDIR)\format.obj" \
	"$(INTDIR)\GridColumnDlg.obj" \
	"$(INTDIR)\InputKeyDlg.obj" \
	"$(INTDIR)\LineDlg.obj" \
	"$(INTDIR)\Login.obj" \
	"$(INTDIR)\LoginSetDlg.obj" \
	"$(INTDIR)\mapOption.obj" \
	"$(INTDIR)\MGColumnDlg.obj" \
	"$(INTDIR)\Pane.obj" \
	"$(INTDIR)\Preview.obj" \
	"$(INTDIR)\PreviewRun.obj" \
	"$(INTDIR)\PublicSetDlg.obj" \
	"$(INTDIR)\saveDlg.obj" \
	"$(INTDIR)\SettingDlg.obj" \
	"$(INTDIR)\SingleProperty.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\upDlg.obj" \
	"$(INTDIR)\upTmplDlg.obj" \
	"$(INTDIR)\uTabPage.obj" \
	"$(INTDIR)\awDlg.res" \
	"$(INTDIR)\HBBtn.obj"

"..\..\..\release\awdlg.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\debug\awdlg.dll" "$(OUTDIR)\awDlg.bsc"


CLEAN :
	-@erase "$(INTDIR)\addrDlg.obj"
	-@erase "$(INTDIR)\addrDlg.sbr"
	-@erase "$(INTDIR)\attrDlg.obj"
	-@erase "$(INTDIR)\attrDlg.sbr"
	-@erase "$(INTDIR)\awDlg.obj"
	-@erase "$(INTDIR)\awDlg.pch"
	-@erase "$(INTDIR)\awDlg.res"
	-@erase "$(INTDIR)\awDlg.sbr"
	-@erase "$(INTDIR)\BATCH.OBJ"
	-@erase "$(INTDIR)\BATCH.SBR"
	-@erase "$(INTDIR)\BtnCombo.obj"
	-@erase "$(INTDIR)\BtnCombo.sbr"
	-@erase "$(INTDIR)\cbItem.obj"
	-@erase "$(INTDIR)\cbItem.sbr"
	-@erase "$(INTDIR)\cellEdit.obj"
	-@erase "$(INTDIR)\cellEdit.sbr"
	-@erase "$(INTDIR)\colorDlg.obj"
	-@erase "$(INTDIR)\colorDlg.sbr"
	-@erase "$(INTDIR)\ControlDlg.obj"
	-@erase "$(INTDIR)\ControlDlg.sbr"
	-@erase "$(INTDIR)\coRGB.obj"
	-@erase "$(INTDIR)\coRGB.sbr"
	-@erase "$(INTDIR)\EditItem.obj"
	-@erase "$(INTDIR)\EditItem.sbr"
	-@erase "$(INTDIR)\EditList.obj"
	-@erase "$(INTDIR)\EditList.sbr"
	-@erase "$(INTDIR)\EditOrder.obj"
	-@erase "$(INTDIR)\EditOrder.sbr"
	-@erase "$(INTDIR)\folderBrowser.obj"
	-@erase "$(INTDIR)\folderBrowser.sbr"
	-@erase "$(INTDIR)\format.obj"
	-@erase "$(INTDIR)\format.sbr"
	-@erase "$(INTDIR)\GridColumnDlg.obj"
	-@erase "$(INTDIR)\GridColumnDlg.sbr"
	-@erase "$(INTDIR)\HBBtn.obj"
	-@erase "$(INTDIR)\HBBtn.sbr"
	-@erase "$(INTDIR)\InputKeyDlg.obj"
	-@erase "$(INTDIR)\InputKeyDlg.sbr"
	-@erase "$(INTDIR)\ItemCombo.obj"
	-@erase "$(INTDIR)\ItemCombo.sbr"
	-@erase "$(INTDIR)\LineDlg.obj"
	-@erase "$(INTDIR)\LineDlg.sbr"
	-@erase "$(INTDIR)\ListGrid.obj"
	-@erase "$(INTDIR)\ListGrid.sbr"
	-@erase "$(INTDIR)\Login.obj"
	-@erase "$(INTDIR)\Login.sbr"
	-@erase "$(INTDIR)\LoginSetDlg.obj"
	-@erase "$(INTDIR)\LoginSetDlg.sbr"
	-@erase "$(INTDIR)\mapOption.obj"
	-@erase "$(INTDIR)\mapOption.sbr"
	-@erase "$(INTDIR)\MGColumnDlg.obj"
	-@erase "$(INTDIR)\MGColumnDlg.sbr"
	-@erase "$(INTDIR)\NFBtn.obj"
	-@erase "$(INTDIR)\NFBtn.sbr"
	-@erase "$(INTDIR)\Pane.obj"
	-@erase "$(INTDIR)\Pane.sbr"
	-@erase "$(INTDIR)\PopListCtrl.obj"
	-@erase "$(INTDIR)\PopListCtrl.sbr"
	-@erase "$(INTDIR)\Preview.obj"
	-@erase "$(INTDIR)\Preview.sbr"
	-@erase "$(INTDIR)\PreviewRun.obj"
	-@erase "$(INTDIR)\PreviewRun.sbr"
	-@erase "$(INTDIR)\PublicSetDlg.obj"
	-@erase "$(INTDIR)\PublicSetDlg.sbr"
	-@erase "$(INTDIR)\saveDlg.obj"
	-@erase "$(INTDIR)\saveDlg.sbr"
	-@erase "$(INTDIR)\SepWnd.obj"
	-@erase "$(INTDIR)\SepWnd.sbr"
	-@erase "$(INTDIR)\SettingDlg.obj"
	-@erase "$(INTDIR)\SettingDlg.sbr"
	-@erase "$(INTDIR)\SingleProperty.obj"
	-@erase "$(INTDIR)\SingleProperty.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\upDlg.obj"
	-@erase "$(INTDIR)\upDlg.sbr"
	-@erase "$(INTDIR)\upTmplDlg.obj"
	-@erase "$(INTDIR)\upTmplDlg.sbr"
	-@erase "$(INTDIR)\uTabPage.obj"
	-@erase "$(INTDIR)\uTabPage.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XFontDialog.obj"
	-@erase "$(INTDIR)\XFontDialog.sbr"
	-@erase "$(INTDIR)\xTreeCtrl.obj"
	-@erase "$(INTDIR)\xTreeCtrl.sbr"
	-@erase "$(OUTDIR)\awDlg.bsc"
	-@erase "$(OUTDIR)\awdlg.exp"
	-@erase "$(OUTDIR)\awdlg.lib"
	-@erase "$(OUTDIR)\awdlg.pdb"
	-@erase "..\..\..\debug\awdlg.dll"
	-@erase "..\..\..\debug\awdlg.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\awDlg.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\awDlg.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\awDlg.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BtnCombo.sbr" \
	"$(INTDIR)\EditItem.sbr" \
	"$(INTDIR)\EditList.sbr" \
	"$(INTDIR)\ItemCombo.sbr" \
	"$(INTDIR)\ListGrid.sbr" \
	"$(INTDIR)\NFBtn.sbr" \
	"$(INTDIR)\PopListCtrl.sbr" \
	"$(INTDIR)\SepWnd.sbr" \
	"$(INTDIR)\XFontDialog.sbr" \
	"$(INTDIR)\xTreeCtrl.sbr" \
	"$(INTDIR)\addrDlg.sbr" \
	"$(INTDIR)\attrDlg.sbr" \
	"$(INTDIR)\awDlg.sbr" \
	"$(INTDIR)\BATCH.SBR" \
	"$(INTDIR)\cbItem.sbr" \
	"$(INTDIR)\cellEdit.sbr" \
	"$(INTDIR)\colorDlg.sbr" \
	"$(INTDIR)\ControlDlg.sbr" \
	"$(INTDIR)\coRGB.sbr" \
	"$(INTDIR)\EditOrder.sbr" \
	"$(INTDIR)\folderBrowser.sbr" \
	"$(INTDIR)\format.sbr" \
	"$(INTDIR)\GridColumnDlg.sbr" \
	"$(INTDIR)\InputKeyDlg.sbr" \
	"$(INTDIR)\LineDlg.sbr" \
	"$(INTDIR)\Login.sbr" \
	"$(INTDIR)\LoginSetDlg.sbr" \
	"$(INTDIR)\mapOption.sbr" \
	"$(INTDIR)\MGColumnDlg.sbr" \
	"$(INTDIR)\Pane.sbr" \
	"$(INTDIR)\Preview.sbr" \
	"$(INTDIR)\PreviewRun.sbr" \
	"$(INTDIR)\PublicSetDlg.sbr" \
	"$(INTDIR)\saveDlg.sbr" \
	"$(INTDIR)\SettingDlg.sbr" \
	"$(INTDIR)\SingleProperty.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\upDlg.sbr" \
	"$(INTDIR)\upTmplDlg.sbr" \
	"$(INTDIR)\uTabPage.sbr" \
	"$(INTDIR)\HBBtn.sbr"

"$(OUTDIR)\awDlg.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=../awSock/debug/awsock.lib ../awBuild/debug/awbuild.lib ../awWcc/debug/awwcc.lib ../awCommon/debug/awcommon.lib ../../dll/lib/debug/axislib.lib ../awObject/debug/awObject.lib ../awUser/debug/awUser.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\awdlg.pdb" /debug /machine:I386 /def:".\awDlg.def" /out:"../../../debug/awdlg.dll" /implib:"$(OUTDIR)\awdlg.lib" /pdbtype:sept 
DEF_FILE= \
	".\awDlg.def"
LINK32_OBJS= \
	"$(INTDIR)\BtnCombo.obj" \
	"$(INTDIR)\EditItem.obj" \
	"$(INTDIR)\EditList.obj" \
	"$(INTDIR)\ItemCombo.obj" \
	"$(INTDIR)\ListGrid.obj" \
	"$(INTDIR)\NFBtn.obj" \
	"$(INTDIR)\PopListCtrl.obj" \
	"$(INTDIR)\SepWnd.obj" \
	"$(INTDIR)\XFontDialog.obj" \
	"$(INTDIR)\xTreeCtrl.obj" \
	"$(INTDIR)\addrDlg.obj" \
	"$(INTDIR)\attrDlg.obj" \
	"$(INTDIR)\awDlg.obj" \
	"$(INTDIR)\BATCH.OBJ" \
	"$(INTDIR)\cbItem.obj" \
	"$(INTDIR)\cellEdit.obj" \
	"$(INTDIR)\colorDlg.obj" \
	"$(INTDIR)\ControlDlg.obj" \
	"$(INTDIR)\coRGB.obj" \
	"$(INTDIR)\EditOrder.obj" \
	"$(INTDIR)\folderBrowser.obj" \
	"$(INTDIR)\format.obj" \
	"$(INTDIR)\GridColumnDlg.obj" \
	"$(INTDIR)\InputKeyDlg.obj" \
	"$(INTDIR)\LineDlg.obj" \
	"$(INTDIR)\Login.obj" \
	"$(INTDIR)\LoginSetDlg.obj" \
	"$(INTDIR)\mapOption.obj" \
	"$(INTDIR)\MGColumnDlg.obj" \
	"$(INTDIR)\Pane.obj" \
	"$(INTDIR)\Preview.obj" \
	"$(INTDIR)\PreviewRun.obj" \
	"$(INTDIR)\PublicSetDlg.obj" \
	"$(INTDIR)\saveDlg.obj" \
	"$(INTDIR)\SettingDlg.obj" \
	"$(INTDIR)\SingleProperty.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\upDlg.obj" \
	"$(INTDIR)\upTmplDlg.obj" \
	"$(INTDIR)\uTabPage.obj" \
	"$(INTDIR)\awDlg.res" \
	"$(INTDIR)\HBBtn.obj"

"..\..\..\debug\awdlg.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("awDlg.dep")
!INCLUDE "awDlg.dep"
!ELSE 
!MESSAGE Warning: cannot find "awDlg.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "awDlg - Win32 Release" || "$(CFG)" == "awDlg - Win32 Debug"
SOURCE=..\BtnCombo.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\BtnCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\BtnCombo.obj"	"$(INTDIR)\BtnCombo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Components\EditItem.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\EditItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\EditItem.obj"	"$(INTDIR)\EditItem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Components\EditList.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\EditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\EditList.obj"	"$(INTDIR)\EditList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\HBBtn.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\HBBtn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\HBBtn.obj"	"$(INTDIR)\HBBtn.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=..\Components\ItemCombo.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\ItemCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\ItemCombo.obj"	"$(INTDIR)\ItemCombo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ListGrid.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\ListGrid.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\ListGrid.obj"	"$(INTDIR)\ListGrid.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\NFBtn.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\NFBtn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\NFBtn.obj"	"$(INTDIR)\NFBtn.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Components\PopListCtrl.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\PopListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\PopListCtrl.obj"	"$(INTDIR)\PopListCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SepWnd.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\SepWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\SepWnd.obj"	"$(INTDIR)\SepWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\XFontDialog.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\XFontDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\XFontDialog.obj"	"$(INTDIR)\XFontDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\xTreeCtrl.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\xTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\xTreeCtrl.obj"	"$(INTDIR)\xTreeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\addrDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\addrDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\addrDlg.obj"	"$(INTDIR)\addrDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\attrDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\attrDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\attrDlg.obj"	"$(INTDIR)\attrDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\awDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\awDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\awDlg.obj"	"$(INTDIR)\awDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\awDlg.rc

"$(INTDIR)\awDlg.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\BATCH.CPP

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\BATCH.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\BATCH.OBJ"	"$(INTDIR)\BATCH.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\cbItem.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\cbItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\cbItem.obj"	"$(INTDIR)\cbItem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\cellEdit.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\cellEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\cellEdit.obj"	"$(INTDIR)\cellEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\colorDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\colorDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\colorDlg.obj"	"$(INTDIR)\colorDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\ControlDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\ControlDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\ControlDlg.obj"	"$(INTDIR)\ControlDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\coRGB.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\coRGB.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\coRGB.obj"	"$(INTDIR)\coRGB.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\EditOrder.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\EditOrder.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\EditOrder.obj"	"$(INTDIR)\EditOrder.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\folderBrowser.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\folderBrowser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\folderBrowser.obj"	"$(INTDIR)\folderBrowser.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\format.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\format.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\format.obj"	"$(INTDIR)\format.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\GridColumnDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\GridColumnDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\GridColumnDlg.obj"	"$(INTDIR)\GridColumnDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\InputKeyDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\InputKeyDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\InputKeyDlg.obj"	"$(INTDIR)\InputKeyDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\LineDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\LineDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\LineDlg.obj"	"$(INTDIR)\LineDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\Login.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\Login.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\Login.obj"	"$(INTDIR)\Login.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\LoginSetDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\LoginSetDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\LoginSetDlg.obj"	"$(INTDIR)\LoginSetDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\mapOption.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\mapOption.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\mapOption.obj"	"$(INTDIR)\mapOption.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\MGColumnDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\MGColumnDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\MGColumnDlg.obj"	"$(INTDIR)\MGColumnDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\Pane.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\Pane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\Pane.obj"	"$(INTDIR)\Pane.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\Preview.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\Preview.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\Preview.obj"	"$(INTDIR)\Preview.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\PreviewRun.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\PreviewRun.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\PreviewRun.obj"	"$(INTDIR)\PreviewRun.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\PublicSetDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\PublicSetDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\PublicSetDlg.obj"	"$(INTDIR)\PublicSetDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\saveDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\saveDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\saveDlg.obj"	"$(INTDIR)\saveDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\SettingDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\SettingDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\SettingDlg.obj"	"$(INTDIR)\SettingDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\SingleProperty.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\SingleProperty.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\SingleProperty.obj"	"$(INTDIR)\SingleProperty.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\awDlg.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\awDlg.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\awDlg.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\awDlg.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\upDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\upDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\upDlg.obj"	"$(INTDIR)\upDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\upTmplDlg.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\upTmplDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\upTmplDlg.obj"	"$(INTDIR)\upTmplDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 

SOURCE=.\uTabPage.cpp

!IF  "$(CFG)" == "awDlg - Win32 Release"


"$(INTDIR)\uTabPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ELSEIF  "$(CFG)" == "awDlg - Win32 Debug"


"$(INTDIR)\uTabPage.obj"	"$(INTDIR)\uTabPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\awDlg.pch"


!ENDIF 


!ENDIF 


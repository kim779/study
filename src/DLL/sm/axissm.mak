# Microsoft Developer Studio Generated NMAKE File, Based on axissm.dsp
!IF "$(CFG)" == ""
CFG=axissm - Win32 Debug
!MESSAGE No configuration specified. Defaulting to axissm - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "axissm - Win32 Release" && "$(CFG)" != "axissm - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axissm.mak" CFG="axissm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axissm - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axissm - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "axissm - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\Release\axissm.dll"


CLEAN :
	-@erase "$(INTDIR)\axGuide.obj"
	-@erase "$(INTDIR)\axissm.obj"
	-@erase "$(INTDIR)\axissm.pch"
	-@erase "$(INTDIR)\axissm.res"
	-@erase "$(INTDIR)\Basebar0.obj"
	-@erase "$(INTDIR)\bitmap.obj"
	-@erase "$(INTDIR)\CoolDialogBar.obj"
	-@erase "$(INTDIR)\Dbar1.obj"
	-@erase "$(INTDIR)\Dbar2.obj"
	-@erase "$(INTDIR)\esetup.obj"
	-@erase "$(INTDIR)\eticker.obj"
	-@erase "$(INTDIR)\ExCombo.obj"
	-@erase "$(INTDIR)\ExDialog.obj"
	-@erase "$(INTDIR)\ExListCtrl.obj"
	-@erase "$(INTDIR)\ExTreeCtrl.obj"
	-@erase "$(INTDIR)\hBand.obj"
	-@erase "$(INTDIR)\Hotkey.obj"
	-@erase "$(INTDIR)\iMenu.obj"
	-@erase "$(INTDIR)\Link.obj"
	-@erase "$(INTDIR)\listctrlSort.obj"
	-@erase "$(INTDIR)\listEdit.obj"
	-@erase "$(INTDIR)\listHeader.obj"
	-@erase "$(INTDIR)\MDIClient.obj"
	-@erase "$(INTDIR)\MenuXP.obj"
	-@erase "$(INTDIR)\MsgSetup.obj"
	-@erase "$(INTDIR)\NButton.obj"
	-@erase "$(INTDIR)\NewsHistory.obj"
	-@erase "$(INTDIR)\OrderSetup.obj"
	-@erase "$(INTDIR)\palette.obj"
	-@erase "$(INTDIR)\Pane.obj"
	-@erase "$(INTDIR)\Rband.obj"
	-@erase "$(INTDIR)\RegCode.obj"
	-@erase "$(INTDIR)\RegMap.obj"
	-@erase "$(INTDIR)\ReportCtrl.obj"
	-@erase "$(INTDIR)\RunX.obj"
	-@erase "$(INTDIR)\ScreenList.obj"
	-@erase "$(INTDIR)\SDIBand.obj"
	-@erase "$(INTDIR)\Sdibar.obj"
	-@erase "$(INTDIR)\SEdit.obj"
	-@erase "$(INTDIR)\SetTickInfo.obj"
	-@erase "$(INTDIR)\SimpleSplitter.obj"
	-@erase "$(INTDIR)\Smain.obj"
	-@erase "$(INTDIR)\Smcall.obj"
	-@erase "$(INTDIR)\SmSheet.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SVband.obj"
	-@erase "$(INTDIR)\TInfo1.obj"
	-@erase "$(INTDIR)\TInfo2.obj"
	-@erase "$(INTDIR)\TInfo3.obj"
	-@erase "$(INTDIR)\TMenu.obj"
	-@erase "$(INTDIR)\TrSearch.obj"
	-@erase "$(INTDIR)\tSetup.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Xcom.obj"
	-@erase "$(INTDIR)\Xcomex.obj"
	-@erase "$(INTDIR)\Xedit.obj"
	-@erase "$(OUTDIR)\axissm.exp"
	-@erase "$(OUTDIR)\axissm.lib"
	-@erase "$(OUTDIR)\axissm.map"
	-@erase "$(OUTDIR)\axissm.pdb"
	-@erase "..\..\..\Release\axissm.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\axissm.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\axissm.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\axissm.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Imm32.lib /nologo /base:"0x00500000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\axissm.pdb" /map:"$(INTDIR)\axissm.map" /debug /machine:I386 /def:".\axissm.def" /out:"../../../Release/axissm.dll" /implib:"$(OUTDIR)\axissm.lib" 
LINK32_OBJS= \
	"$(INTDIR)\axGuide.obj" \
	"$(INTDIR)\axissm.obj" \
	"$(INTDIR)\Basebar0.obj" \
	"$(INTDIR)\CoolDialogBar.obj" \
	"$(INTDIR)\Dbar1.obj" \
	"$(INTDIR)\Dbar2.obj" \
	"$(INTDIR)\esetup.obj" \
	"$(INTDIR)\eticker.obj" \
	"$(INTDIR)\hBand.obj" \
	"$(INTDIR)\Hotkey.obj" \
	"$(INTDIR)\iMenu.obj" \
	"$(INTDIR)\Link.obj" \
	"$(INTDIR)\MDIClient.obj" \
	"$(INTDIR)\MenuXP.obj" \
	"$(INTDIR)\MsgSetup.obj" \
	"$(INTDIR)\NButton.obj" \
	"$(INTDIR)\NewsHistory.obj" \
	"$(INTDIR)\OrderSetup.obj" \
	"$(INTDIR)\Pane.obj" \
	"$(INTDIR)\Rband.obj" \
	"$(INTDIR)\RegCode.obj" \
	"$(INTDIR)\RegMap.obj" \
	"$(INTDIR)\ReportCtrl.obj" \
	"$(INTDIR)\RunX.obj" \
	"$(INTDIR)\ScreenList.obj" \
	"$(INTDIR)\SDIBand.obj" \
	"$(INTDIR)\Sdibar.obj" \
	"$(INTDIR)\SEdit.obj" \
	"$(INTDIR)\SetTickInfo.obj" \
	"$(INTDIR)\SimpleSplitter.obj" \
	"$(INTDIR)\Smain.obj" \
	"$(INTDIR)\Smcall.obj" \
	"$(INTDIR)\SmSheet.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SVband.obj" \
	"$(INTDIR)\TInfo1.obj" \
	"$(INTDIR)\TInfo2.obj" \
	"$(INTDIR)\TInfo3.obj" \
	"$(INTDIR)\TMenu.obj" \
	"$(INTDIR)\TrSearch.obj" \
	"$(INTDIR)\tSetup.obj" \
	"$(INTDIR)\Xcom.obj" \
	"$(INTDIR)\Xcomex.obj" \
	"$(INTDIR)\Xedit.obj" \
	"$(INTDIR)\bitmap.obj" \
	"$(INTDIR)\ExCombo.obj" \
	"$(INTDIR)\ExDialog.obj" \
	"$(INTDIR)\ExListCtrl.obj" \
	"$(INTDIR)\ExTreeCtrl.obj" \
	"$(INTDIR)\listctrlSort.obj" \
	"$(INTDIR)\listEdit.obj" \
	"$(INTDIR)\listHeader.obj" \
	"$(INTDIR)\palette.obj" \
	"$(INTDIR)\axissm.res"

"..\..\..\Release\axissm.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "axissm - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\Debug\axissm.dll"


CLEAN :
	-@erase "$(INTDIR)\axGuide.obj"
	-@erase "$(INTDIR)\axissm.obj"
	-@erase "$(INTDIR)\axissm.pch"
	-@erase "$(INTDIR)\axissm.res"
	-@erase "$(INTDIR)\Basebar0.obj"
	-@erase "$(INTDIR)\bitmap.obj"
	-@erase "$(INTDIR)\CoolDialogBar.obj"
	-@erase "$(INTDIR)\Dbar1.obj"
	-@erase "$(INTDIR)\Dbar2.obj"
	-@erase "$(INTDIR)\esetup.obj"
	-@erase "$(INTDIR)\eticker.obj"
	-@erase "$(INTDIR)\ExCombo.obj"
	-@erase "$(INTDIR)\ExDialog.obj"
	-@erase "$(INTDIR)\ExListCtrl.obj"
	-@erase "$(INTDIR)\ExTreeCtrl.obj"
	-@erase "$(INTDIR)\hBand.obj"
	-@erase "$(INTDIR)\Hotkey.obj"
	-@erase "$(INTDIR)\iMenu.obj"
	-@erase "$(INTDIR)\Link.obj"
	-@erase "$(INTDIR)\listctrlSort.obj"
	-@erase "$(INTDIR)\listEdit.obj"
	-@erase "$(INTDIR)\listHeader.obj"
	-@erase "$(INTDIR)\MDIClient.obj"
	-@erase "$(INTDIR)\MenuXP.obj"
	-@erase "$(INTDIR)\MsgSetup.obj"
	-@erase "$(INTDIR)\NButton.obj"
	-@erase "$(INTDIR)\NewsHistory.obj"
	-@erase "$(INTDIR)\OrderSetup.obj"
	-@erase "$(INTDIR)\palette.obj"
	-@erase "$(INTDIR)\Pane.obj"
	-@erase "$(INTDIR)\Rband.obj"
	-@erase "$(INTDIR)\RegCode.obj"
	-@erase "$(INTDIR)\RegMap.obj"
	-@erase "$(INTDIR)\ReportCtrl.obj"
	-@erase "$(INTDIR)\RunX.obj"
	-@erase "$(INTDIR)\ScreenList.obj"
	-@erase "$(INTDIR)\SDIBand.obj"
	-@erase "$(INTDIR)\Sdibar.obj"
	-@erase "$(INTDIR)\SEdit.obj"
	-@erase "$(INTDIR)\SetTickInfo.obj"
	-@erase "$(INTDIR)\SimpleSplitter.obj"
	-@erase "$(INTDIR)\Smain.obj"
	-@erase "$(INTDIR)\Smcall.obj"
	-@erase "$(INTDIR)\SmSheet.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SVband.obj"
	-@erase "$(INTDIR)\TInfo1.obj"
	-@erase "$(INTDIR)\TInfo2.obj"
	-@erase "$(INTDIR)\TInfo3.obj"
	-@erase "$(INTDIR)\TMenu.obj"
	-@erase "$(INTDIR)\TrSearch.obj"
	-@erase "$(INTDIR)\tSetup.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Xcom.obj"
	-@erase "$(INTDIR)\Xcomex.obj"
	-@erase "$(INTDIR)\Xedit.obj"
	-@erase "$(OUTDIR)\axissm.exp"
	-@erase "$(OUTDIR)\axissm.lib"
	-@erase "$(OUTDIR)\axissm.pdb"
	-@erase "..\..\..\Debug\axissm.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\axissm.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\axissm.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\axissm.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Imm32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\axissm.pdb" /debug /machine:I386 /def:".\axissm.def" /out:"../../../Debug/axissm.dll" /implib:"$(OUTDIR)\axissm.lib" /pdbtype:sept 
DEF_FILE= \
	".\axissm.def"
LINK32_OBJS= \
	"$(INTDIR)\axGuide.obj" \
	"$(INTDIR)\axissm.obj" \
	"$(INTDIR)\Basebar0.obj" \
	"$(INTDIR)\CoolDialogBar.obj" \
	"$(INTDIR)\Dbar1.obj" \
	"$(INTDIR)\Dbar2.obj" \
	"$(INTDIR)\esetup.obj" \
	"$(INTDIR)\eticker.obj" \
	"$(INTDIR)\hBand.obj" \
	"$(INTDIR)\Hotkey.obj" \
	"$(INTDIR)\iMenu.obj" \
	"$(INTDIR)\Link.obj" \
	"$(INTDIR)\MDIClient.obj" \
	"$(INTDIR)\MenuXP.obj" \
	"$(INTDIR)\MsgSetup.obj" \
	"$(INTDIR)\NButton.obj" \
	"$(INTDIR)\NewsHistory.obj" \
	"$(INTDIR)\OrderSetup.obj" \
	"$(INTDIR)\Pane.obj" \
	"$(INTDIR)\Rband.obj" \
	"$(INTDIR)\RegCode.obj" \
	"$(INTDIR)\RegMap.obj" \
	"$(INTDIR)\ReportCtrl.obj" \
	"$(INTDIR)\RunX.obj" \
	"$(INTDIR)\ScreenList.obj" \
	"$(INTDIR)\SDIBand.obj" \
	"$(INTDIR)\Sdibar.obj" \
	"$(INTDIR)\SEdit.obj" \
	"$(INTDIR)\SetTickInfo.obj" \
	"$(INTDIR)\SimpleSplitter.obj" \
	"$(INTDIR)\Smain.obj" \
	"$(INTDIR)\Smcall.obj" \
	"$(INTDIR)\SmSheet.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SVband.obj" \
	"$(INTDIR)\TInfo1.obj" \
	"$(INTDIR)\TInfo2.obj" \
	"$(INTDIR)\TInfo3.obj" \
	"$(INTDIR)\TMenu.obj" \
	"$(INTDIR)\TrSearch.obj" \
	"$(INTDIR)\tSetup.obj" \
	"$(INTDIR)\Xcom.obj" \
	"$(INTDIR)\Xcomex.obj" \
	"$(INTDIR)\Xedit.obj" \
	"$(INTDIR)\bitmap.obj" \
	"$(INTDIR)\ExCombo.obj" \
	"$(INTDIR)\ExDialog.obj" \
	"$(INTDIR)\ExListCtrl.obj" \
	"$(INTDIR)\ExTreeCtrl.obj" \
	"$(INTDIR)\listctrlSort.obj" \
	"$(INTDIR)\listEdit.obj" \
	"$(INTDIR)\listHeader.obj" \
	"$(INTDIR)\palette.obj" \
	"$(INTDIR)\axissm.res"

"..\..\..\Debug\axissm.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("axissm.dep")
!INCLUDE "axissm.dep"
!ELSE 
!MESSAGE Warning: cannot find "axissm.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "axissm - Win32 Release" || "$(CFG)" == "axissm - Win32 Debug"
SOURCE=.\axGuide.cpp

"$(INTDIR)\axGuide.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\axissm.cpp

"$(INTDIR)\axissm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\axissm.rc

"$(INTDIR)\axissm.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Basebar0.cpp

"$(INTDIR)\Basebar0.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\CoolDialogBar.cpp

"$(INTDIR)\CoolDialogBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\Dbar1.cpp

"$(INTDIR)\Dbar1.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\Dbar2.cpp

"$(INTDIR)\Dbar2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\esetup.cpp

"$(INTDIR)\esetup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\eticker.cpp

"$(INTDIR)\eticker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\hBand.cpp

"$(INTDIR)\hBand.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\Hotkey.cpp

"$(INTDIR)\Hotkey.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\iMenu.cpp

"$(INTDIR)\iMenu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\Link.cpp

"$(INTDIR)\Link.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\MDIClient.cpp

"$(INTDIR)\MDIClient.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\MenuXP.cpp

"$(INTDIR)\MenuXP.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\MsgSetup.cpp

"$(INTDIR)\MsgSetup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\NButton.cpp

"$(INTDIR)\NButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\NewsHistory.cpp

"$(INTDIR)\NewsHistory.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\OrderSetup.cpp

"$(INTDIR)\OrderSetup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\Pane.cpp

"$(INTDIR)\Pane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\Rband.cpp

"$(INTDIR)\Rband.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\RegCode.cpp

"$(INTDIR)\RegCode.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\RegMap.cpp

"$(INTDIR)\RegMap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\ReportCtrl.cpp

"$(INTDIR)\ReportCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\RunX.cpp

"$(INTDIR)\RunX.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\ScreenList.cpp

"$(INTDIR)\ScreenList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\SDIBand.cpp

"$(INTDIR)\SDIBand.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\Sdibar.cpp

"$(INTDIR)\Sdibar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\SEdit.cpp

"$(INTDIR)\SEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\SetTickInfo.cpp

"$(INTDIR)\SetTickInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\SimpleSplitter.cpp

"$(INTDIR)\SimpleSplitter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\Smain.cpp

"$(INTDIR)\Smain.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\Smcall.cpp

"$(INTDIR)\Smcall.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\SmSheet.cpp

"$(INTDIR)\SmSheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "axissm - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\axissm.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\axissm.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "axissm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\axissm.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\axissm.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\SVband.cpp

"$(INTDIR)\SVband.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\TInfo1.cpp

"$(INTDIR)\TInfo1.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\TInfo2.cpp

"$(INTDIR)\TInfo2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\TInfo3.cpp

"$(INTDIR)\TInfo3.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\TMenu.cpp

"$(INTDIR)\TMenu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\TrSearch.cpp

"$(INTDIR)\TrSearch.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\tSetup.cpp

"$(INTDIR)\tSetup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\Xcom.cpp

"$(INTDIR)\Xcom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\Xcomex.cpp

"$(INTDIR)\Xcomex.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=.\Xedit.cpp

"$(INTDIR)\Xedit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"


SOURCE=..\ctrl\bitmap.cpp

"$(INTDIR)\bitmap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ctrl\ExCombo.cpp

"$(INTDIR)\ExCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ctrl\ExDialog.cpp

"$(INTDIR)\ExDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ctrl\ExListCtrl.cpp

"$(INTDIR)\ExListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ctrl\ExTreeCtrl.cpp

"$(INTDIR)\ExTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ctrl\listctrlSort.cpp

"$(INTDIR)\listctrlSort.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ctrl\listEdit.cpp

"$(INTDIR)\listEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ctrl\listHeader.cpp

"$(INTDIR)\listHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ctrl\palette.cpp

"$(INTDIR)\palette.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axissm.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 


# Microsoft Developer Studio Generated NMAKE File, Based on IBXXXX08.dsp
!IF "$(CFG)" == ""
CFG=IBXXXX08 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to IBXXXX08 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IBXXXX08 - Win32 Release" && "$(CFG)" != "IBXXXX08 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IBXXXX08.mak" CFG="IBXXXX08 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IBXXXX08 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IBXXXX08 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
nullptr=
!ELSE 
nullptr=nul
!ENDIF 

!IF  "$(CFG)" == "IBXXXX08 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\Release\IBXXXX08.dll"


CLEAN :
	-@erase "$(INTDIR)\AxMapForm.obj"
	-@erase "$(INTDIR)\AxStatic.obj"
	-@erase "$(INTDIR)\catchWnd.obj"
	-@erase "$(INTDIR)\cxPrice.obj"
	-@erase "$(INTDIR)\IBXXXX08.obj"
	-@erase "$(INTDIR)\IBXXXX08.pch"
	-@erase "$(INTDIR)\IBXXXX08.res"
	-@erase "$(INTDIR)\dlgHistory.obj"
	-@erase "$(INTDIR)\exEdit.obj"
	-@erase "$(INTDIR)\exGrid.obj"
	-@erase "$(INTDIR)\exListCtrl.obj"
	-@erase "$(INTDIR)\exSpin.obj"
	-@erase "$(INTDIR)\ImgBtn.obj"
	-@erase "$(INTDIR)\libFunction.obj"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\mapForm.obj"
	-@erase "$(INTDIR)\setupDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\IBXXXX08.exp"
	-@erase "$(OUTDIR)\IBXXXX08.lib"
	-@erase "..\..\..\Release\IBXXXX08.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(nullptr)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IBXXXX08.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IBXXXX08.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IBXXXX08.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../../control/fx_grid/release/fx_grid.lib winmm.lib Imm32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\IBXXXX08.pdb" /machine:I386 /def:".\IBXXXX08.def" /out:"../../../Release/IBXXXX08.dll" /implib:"$(OUTDIR)\IBXXXX08.lib" 
DEF_FILE= \
	".\IBXXXX08.def"
LINK32_OBJS= \
	"$(INTDIR)\AxMapForm.obj" \
	"$(INTDIR)\AxStatic.obj" \
	"$(INTDIR)\catchWnd.obj" \
	"$(INTDIR)\cxPrice.obj" \
	"$(INTDIR)\IBXXXX08.obj" \
	"$(INTDIR)\dlgHistory.obj" \
	"$(INTDIR)\exEdit.obj" \
	"$(INTDIR)\exGrid.obj" \
	"$(INTDIR)\exListCtrl.obj" \
	"$(INTDIR)\exSpin.obj" \
	"$(INTDIR)\ImgBtn.obj" \
	"$(INTDIR)\libFunction.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\mapForm.obj" \
	"$(INTDIR)\setupDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IBXXXX08.res"

"..\..\..\Release\IBXXXX08.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IBXXXX08 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\dev\IBXXXX08.dll"


CLEAN :
	-@erase "$(INTDIR)\AxMapForm.obj"
	-@erase "$(INTDIR)\AxStatic.obj"
	-@erase "$(INTDIR)\catchWnd.obj"
	-@erase "$(INTDIR)\cxPrice.obj"
	-@erase "$(INTDIR)\IBXXXX08.obj"
	-@erase "$(INTDIR)\IBXXXX08.pch"
	-@erase "$(INTDIR)\IBXXXX08.res"
	-@erase "$(INTDIR)\dlgHistory.obj"
	-@erase "$(INTDIR)\exEdit.obj"
	-@erase "$(INTDIR)\exGrid.obj"
	-@erase "$(INTDIR)\exListCtrl.obj"
	-@erase "$(INTDIR)\exSpin.obj"
	-@erase "$(INTDIR)\ImgBtn.obj"
	-@erase "$(INTDIR)\libFunction.obj"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\mapForm.obj"
	-@erase "$(INTDIR)\setupDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\IBXXXX08.exp"
	-@erase "$(OUTDIR)\IBXXXX08.lib"
	-@erase "$(OUTDIR)\IBXXXX08.pdb"
	-@erase "..\..\..\dev\IBXXXX08.dll"
	-@erase "..\..\..\dev\IBXXXX08.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(nullptr)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IBXXXX08.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IBXXXX08.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IBXXXX08.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../../control/fx_grid/Debug/fx_grid.lib winmm.lib Imm32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\IBXXXX08.pdb" /debug /machine:I386 /def:".\IBXXXX08.def" /out:"../../../dev/IBXXXX08.dll" /implib:"$(OUTDIR)\IBXXXX08.lib" /pdbtype:sept 
DEF_FILE= \
	".\IBXXXX08.def"
LINK32_OBJS= \
	"$(INTDIR)\AxMapForm.obj" \
	"$(INTDIR)\AxStatic.obj" \
	"$(INTDIR)\catchWnd.obj" \
	"$(INTDIR)\cxPrice.obj" \
	"$(INTDIR)\IBXXXX08.obj" \
	"$(INTDIR)\dlgHistory.obj" \
	"$(INTDIR)\exEdit.obj" \
	"$(INTDIR)\exGrid.obj" \
	"$(INTDIR)\exListCtrl.obj" \
	"$(INTDIR)\exSpin.obj" \
	"$(INTDIR)\ImgBtn.obj" \
	"$(INTDIR)\libFunction.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\mapForm.obj" \
	"$(INTDIR)\setupDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IBXXXX08.res"

"..\..\..\dev\IBXXXX08.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("IBXXXX08.dep")
!INCLUDE "IBXXXX08.dep"
!ELSE 
!MESSAGE Warning: cannot find "IBXXXX08.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IBXXXX08 - Win32 Release" || "$(CFG)" == "IBXXXX08 - Win32 Debug"
SOURCE=.\AxMapForm.cpp

"$(INTDIR)\AxMapForm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\AxStatic.cpp

"$(INTDIR)\AxStatic.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\catchWnd.cpp

"$(INTDIR)\catchWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\cxPrice.cpp

"$(INTDIR)\cxPrice.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\IBXXXX08.cpp

"$(INTDIR)\IBXXXX08.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\IBXXXX08.rc

"$(INTDIR)\IBXXXX08.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\dlgHistory.cpp

"$(INTDIR)\dlgHistory.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\exEdit.cpp

"$(INTDIR)\exEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\exGrid.cpp

"$(INTDIR)\exGrid.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\exListCtrl.cpp

"$(INTDIR)\exListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\exSpin.cpp

"$(INTDIR)\exSpin.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\ImgBtn.cpp

"$(INTDIR)\ImgBtn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\libFunction.cpp

"$(INTDIR)\libFunction.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\libmap.cpp

"$(INTDIR)\libmap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\mapForm.cpp

"$(INTDIR)\mapForm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\setupDlg.cpp

"$(INTDIR)\setupDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX08.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "IBXXXX08 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IBXXXX08.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IBXXXX08.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "IBXXXX08 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IBXXXX08.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IBXXXX08.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


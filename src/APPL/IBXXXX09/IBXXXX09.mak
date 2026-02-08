# Microsoft Developer Studio Generated NMAKE File, Based on IBXXXX09.dsp
!IF "$(CFG)" == ""
CFG=IBXXXX09 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to IBXXXX09 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IBXXXX09 - Win32 Release" && "$(CFG)" != "IBXXXX09 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IBXXXX09.mak" CFG="IBXXXX09 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IBXXXX09 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IBXXXX09 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
nullptr=
!ELSE 
nullptr=nul
!ENDIF 

!IF  "$(CFG)" == "IBXXXX09 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\Release\IBXXXX09.dll"


CLEAN :
	-@erase "$(INTDIR)\captureWnd.obj"
	-@erase "$(INTDIR)\IBXXXX09.obj"
	-@erase "$(INTDIR)\IBXXXX09.pch"
	-@erase "$(INTDIR)\IBXXXX09.res"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\mapWnd.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\IBXXXX09.exp"
	-@erase "$(OUTDIR)\IBXXXX09.lib"
	-@erase "..\..\..\Release\IBXXXX09.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(nullptr)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IBXXXX09.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IBXXXX09.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IBXXXX09.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../../control/fx_misc/release/fx_misc.lib ../../control/fx_grid/release/fx_grid.lib winmm.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\IBXXXX09.pdb" /machine:I386 /def:".\IBXXXX09.def" /out:"../../../Release/IBXXXX09.dll" /implib:"$(OUTDIR)\IBXXXX09.lib" 
DEF_FILE= \
	".\IBXXXX09.def"
LINK32_OBJS= \
	"$(INTDIR)\captureWnd.obj" \
	"$(INTDIR)\IBXXXX09.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\mapWnd.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IBXXXX09.res"

"..\..\..\Release\IBXXXX09.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IBXXXX09 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\dev\IBXXXX09.dll"


CLEAN :
	-@erase "$(INTDIR)\captureWnd.obj"
	-@erase "$(INTDIR)\IBXXXX09.obj"
	-@erase "$(INTDIR)\IBXXXX09.pch"
	-@erase "$(INTDIR)\IBXXXX09.res"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\mapWnd.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\IBXXXX09.exp"
	-@erase "$(OUTDIR)\IBXXXX09.lib"
	-@erase "$(OUTDIR)\IBXXXX09.pdb"
	-@erase "..\..\..\dev\IBXXXX09.dll"
	-@erase "..\..\..\dev\IBXXXX09.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(nullptr)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IBXXXX09.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IBXXXX09.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IBXXXX09.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../../control/fx_misc/debug/fx_misc.lib ../../control/fx_grid/debug/fx_grid.lib winmm.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\IBXXXX09.pdb" /debug /machine:I386 /def:".\IBXXXX09.def" /out:"../../../dev/IBXXXX09.dll" /implib:"$(OUTDIR)\IBXXXX09.lib" /pdbtype:sept 
DEF_FILE= \
	".\IBXXXX09.def"
LINK32_OBJS= \
	"$(INTDIR)\captureWnd.obj" \
	"$(INTDIR)\IBXXXX09.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\mapWnd.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IBXXXX09.res"

"..\..\..\dev\IBXXXX09.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("IBXXXX09.dep")
!INCLUDE "IBXXXX09.dep"
!ELSE 
!MESSAGE Warning: cannot find "IBXXXX09.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IBXXXX09 - Win32 Release" || "$(CFG)" == "IBXXXX09 - Win32 Debug"
SOURCE=.\captureWnd.cpp

"$(INTDIR)\captureWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX09.pch"


SOURCE=.\IBXXXX09.cpp

"$(INTDIR)\IBXXXX09.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX09.pch"


SOURCE=.\IBXXXX09.rc

"$(INTDIR)\IBXXXX09.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\libmap.cpp

"$(INTDIR)\libmap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX09.pch"


SOURCE=.\mapWnd.cpp

"$(INTDIR)\mapWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX09.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "IBXXXX09 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IBXXXX09.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IBXXXX09.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "IBXXXX09 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IBXXXX09.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IBXXXX09.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


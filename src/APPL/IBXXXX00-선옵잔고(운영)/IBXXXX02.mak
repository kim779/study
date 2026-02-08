# Microsoft Developer Studio Generated NMAKE File, Based on IBXXXX02.dsp
!IF "$(CFG)" == ""
CFG=IBXXXX02 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to IBXXXX02 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IBXXXX02 - Win32 Release" && "$(CFG)" != "IBXXXX02 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IBXXXX02.mak" CFG="IBXXXX02 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IBXXXX02 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IBXXXX02 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "IBXXXX02 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "..\..\..\Release\IBXXXX02.dll" "$(OUTDIR)\IBXXXX02.bsc"


CLEAN :
	-@erase "$(INTDIR)\IBXXXX02.obj"
	-@erase "$(INTDIR)\IBXXXX02.pch"
	-@erase "$(INTDIR)\IBXXXX02.res"
	-@erase "$(INTDIR)\IBXXXX02.sbr"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\libmap.sbr"
	-@erase "$(INTDIR)\MapWnd.obj"
	-@erase "$(INTDIR)\MapWnd.sbr"
	-@erase "$(INTDIR)\Remain.obj"
	-@erase "$(INTDIR)\Remain.sbr"
	-@erase "$(INTDIR)\RemainFuture.obj"
	-@erase "$(INTDIR)\RemainFuture.sbr"
	-@erase "$(INTDIR)\ShMemory.obj"
	-@erase "$(INTDIR)\ShMemory.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\IBXXXX02.bsc"
	-@erase "$(OUTDIR)\IBXXXX02.exp"
	-@erase "$(OUTDIR)\IBXXXX02.lib"
	-@erase "$(OUTDIR)\IBXXXX02.map"
	-@erase "$(OUTDIR)\IBXXXX02.pdb"
	-@erase "..\..\..\Release\IBXXXX02.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IBXXXX02.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IBXXXX02.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IBXXXX02.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\IBXXXX02.sbr" \
	"$(INTDIR)\libmap.sbr" \
	"$(INTDIR)\MapWnd.sbr" \
	"$(INTDIR)\Remain.sbr" \
	"$(INTDIR)\RemainFuture.sbr" \
	"$(INTDIR)\ShMemory.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\IBXXXX02.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\IBXXXX02.pdb" /map:"$(INTDIR)\IBXXXX02.map" /debug /machine:I386 /def:".\IBXXXX02.def" /out:"../../../Release/IBXXXX02.dll" /implib:"$(OUTDIR)\IBXXXX02.lib" 
DEF_FILE= \
	".\IBXXXX02.def"
LINK32_OBJS= \
	"$(INTDIR)\IBXXXX02.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\Remain.obj" \
	"$(INTDIR)\RemainFuture.obj" \
	"$(INTDIR)\ShMemory.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IBXXXX02.res"

"..\..\..\Release\IBXXXX02.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IBXXXX02 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\dev\IBXXXX02.dll" "$(OUTDIR)\IBXXXX02.bsc"


CLEAN :
	-@erase "$(INTDIR)\IBXXXX02.obj"
	-@erase "$(INTDIR)\IBXXXX02.pch"
	-@erase "$(INTDIR)\IBXXXX02.res"
	-@erase "$(INTDIR)\IBXXXX02.sbr"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\libmap.sbr"
	-@erase "$(INTDIR)\MapWnd.obj"
	-@erase "$(INTDIR)\MapWnd.sbr"
	-@erase "$(INTDIR)\Remain.obj"
	-@erase "$(INTDIR)\Remain.sbr"
	-@erase "$(INTDIR)\RemainFuture.obj"
	-@erase "$(INTDIR)\RemainFuture.sbr"
	-@erase "$(INTDIR)\ShMemory.obj"
	-@erase "$(INTDIR)\ShMemory.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\IBXXXX02.bsc"
	-@erase "$(OUTDIR)\IBXXXX02.exp"
	-@erase "$(OUTDIR)\IBXXXX02.lib"
	-@erase "$(OUTDIR)\IBXXXX02.pdb"
	-@erase "..\..\..\dev\IBXXXX02.dll"
	-@erase "..\..\..\dev\IBXXXX02.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IBXXXX02.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IBXXXX02.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IBXXXX02.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\IBXXXX02.sbr" \
	"$(INTDIR)\libmap.sbr" \
	"$(INTDIR)\MapWnd.sbr" \
	"$(INTDIR)\Remain.sbr" \
	"$(INTDIR)\RemainFuture.sbr" \
	"$(INTDIR)\ShMemory.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\IBXXXX02.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\IBXXXX02.pdb" /debug /machine:I386 /def:".\IBXXXX02.def" /out:"../../../dev/IBXXXX02.dll" /implib:"$(OUTDIR)\IBXXXX02.lib" /pdbtype:sept 
DEF_FILE= \
	".\IBXXXX02.def"
LINK32_OBJS= \
	"$(INTDIR)\IBXXXX02.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\Remain.obj" \
	"$(INTDIR)\RemainFuture.obj" \
	"$(INTDIR)\ShMemory.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IBXXXX02.res"

"..\..\..\dev\IBXXXX02.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("IBXXXX02.dep")
!INCLUDE "IBXXXX02.dep"
!ELSE 
!MESSAGE Warning: cannot find "IBXXXX02.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IBXXXX02 - Win32 Release" || "$(CFG)" == "IBXXXX02 - Win32 Debug"
SOURCE=.\IBXXXX02.cpp

"$(INTDIR)\IBXXXX02.obj"	"$(INTDIR)\IBXXXX02.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX02.pch"


SOURCE=.\IBXXXX02.rc

"$(INTDIR)\IBXXXX02.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\libmap.cpp

"$(INTDIR)\libmap.obj"	"$(INTDIR)\libmap.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX02.pch"


SOURCE=.\MapWnd.cpp

"$(INTDIR)\MapWnd.obj"	"$(INTDIR)\MapWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX02.pch"


SOURCE=.\Remain.cpp

"$(INTDIR)\Remain.obj"	"$(INTDIR)\Remain.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX02.pch"


SOURCE=.\RemainFuture.cpp

"$(INTDIR)\RemainFuture.obj"	"$(INTDIR)\RemainFuture.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX02.pch"


SOURCE=.\ShMemory.cpp

"$(INTDIR)\ShMemory.obj"	"$(INTDIR)\ShMemory.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX02.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "IBXXXX02 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IBXXXX02.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\IBXXXX02.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "IBXXXX02 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IBXXXX02.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\IBXXXX02.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


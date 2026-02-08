# Microsoft Developer Studio Generated NMAKE File, Based on IBXXXX01.dsp
!IF "$(CFG)" == ""
CFG=IBXXXX01 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to IBXXXX01 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IBXXXX01 - Win32 Release" && "$(CFG)" != "IBXXXX01 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IBXXXX01.mak" CFG="IBXXXX01 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IBXXXX01 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IBXXXX01 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "IBXXXX01 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "..\..\..\Release\IBXXXX01.dll" "$(OUTDIR)\IBXXXX01.bsc"


CLEAN :
	-@erase "$(INTDIR)\IBXXXX01.obj"
	-@erase "$(INTDIR)\IBXXXX01.pch"
	-@erase "$(INTDIR)\IBXXXX01.res"
	-@erase "$(INTDIR)\IBXXXX01.sbr"
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
	-@erase "$(OUTDIR)\IBXXXX01.bsc"
	-@erase "$(OUTDIR)\IBXXXX01.exp"
	-@erase "$(OUTDIR)\IBXXXX01.lib"
	-@erase "$(OUTDIR)\IBXXXX01.map"
	-@erase "$(OUTDIR)\IBXXXX01.pdb"
	-@erase "..\..\..\Release\IBXXXX01.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IBXXXX01.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IBXXXX01.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IBXXXX01.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\IBXXXX01.sbr" \
	"$(INTDIR)\libmap.sbr" \
	"$(INTDIR)\MapWnd.sbr" \
	"$(INTDIR)\Remain.sbr" \
	"$(INTDIR)\RemainFuture.sbr" \
	"$(INTDIR)\ShMemory.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\IBXXXX01.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\IBXXXX01.pdb" /map:"$(INTDIR)\IBXXXX01.map" /debug /machine:I386 /def:".\IBXXXX01.def" /out:"../../../Release/IBXXXX01.dll" /implib:"$(OUTDIR)\IBXXXX01.lib" 
DEF_FILE= \
	".\IBXXXX01.def"
LINK32_OBJS= \
	"$(INTDIR)\IBXXXX01.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\Remain.obj" \
	"$(INTDIR)\RemainFuture.obj" \
	"$(INTDIR)\ShMemory.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IBXXXX01.res"

"..\..\..\Release\IBXXXX01.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IBXXXX01 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\dev\IBXXXX01.dll" "$(OUTDIR)\IBXXXX01.bsc"


CLEAN :
	-@erase "$(INTDIR)\IBXXXX01.obj"
	-@erase "$(INTDIR)\IBXXXX01.pch"
	-@erase "$(INTDIR)\IBXXXX01.res"
	-@erase "$(INTDIR)\IBXXXX01.sbr"
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
	-@erase "$(OUTDIR)\IBXXXX01.bsc"
	-@erase "$(OUTDIR)\IBXXXX01.exp"
	-@erase "$(OUTDIR)\IBXXXX01.lib"
	-@erase "$(OUTDIR)\IBXXXX01.pdb"
	-@erase "..\..\..\dev\IBXXXX01.dll"
	-@erase "..\..\..\dev\IBXXXX01.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IBXXXX01.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IBXXXX01.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IBXXXX01.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\IBXXXX01.sbr" \
	"$(INTDIR)\libmap.sbr" \
	"$(INTDIR)\MapWnd.sbr" \
	"$(INTDIR)\Remain.sbr" \
	"$(INTDIR)\RemainFuture.sbr" \
	"$(INTDIR)\ShMemory.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\IBXXXX01.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\IBXXXX01.pdb" /debug /machine:I386 /def:".\IBXXXX01.def" /out:"../../../dev/IBXXXX01.dll" /implib:"$(OUTDIR)\IBXXXX01.lib" /pdbtype:sept 
DEF_FILE= \
	".\IBXXXX01.def"
LINK32_OBJS= \
	"$(INTDIR)\IBXXXX01.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\Remain.obj" \
	"$(INTDIR)\RemainFuture.obj" \
	"$(INTDIR)\ShMemory.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IBXXXX01.res"

"..\..\..\dev\IBXXXX01.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("IBXXXX01.dep")
!INCLUDE "IBXXXX01.dep"
!ELSE 
!MESSAGE Warning: cannot find "IBXXXX01.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IBXXXX01 - Win32 Release" || "$(CFG)" == "IBXXXX01 - Win32 Debug"
SOURCE=.\IBXXXX01.cpp

"$(INTDIR)\IBXXXX01.obj"	"$(INTDIR)\IBXXXX01.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX01.pch"


SOURCE=.\IBXXXX01.rc

"$(INTDIR)\IBXXXX01.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\libmap.cpp

"$(INTDIR)\libmap.obj"	"$(INTDIR)\libmap.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX01.pch"


SOURCE=.\MapWnd.cpp

"$(INTDIR)\MapWnd.obj"	"$(INTDIR)\MapWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX01.pch"


SOURCE=.\Remain.cpp

"$(INTDIR)\Remain.obj"	"$(INTDIR)\Remain.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX01.pch"


SOURCE=.\RemainFuture.cpp

"$(INTDIR)\RemainFuture.obj"	"$(INTDIR)\RemainFuture.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX01.pch"


SOURCE=.\ShMemory.cpp

"$(INTDIR)\ShMemory.obj"	"$(INTDIR)\ShMemory.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IBXXXX01.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "IBXXXX01 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IBXXXX01.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\IBXXXX01.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "IBXXXX01 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IBXXXX01.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\IBXXXX01.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


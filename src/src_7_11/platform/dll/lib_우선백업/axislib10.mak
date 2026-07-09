# Microsoft Developer Studio Generated NMAKE File, Based on axislib10.dsp
!IF "$(CFG)" == ""
CFG=axislib10 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to axislib10 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "axislib10 - Win32 Release" && "$(CFG)" != "axislib10 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axislib10.mak" CFG="axislib10 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axislib10 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axislib10 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "axislib10 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\Release\axislib10.dll"


CLEAN :
	-@erase "$(INTDIR)\axislib10.obj"
	-@erase "$(INTDIR)\axislib10.pch"
	-@erase "$(INTDIR)\axislib10.res"
	-@erase "$(INTDIR)\Draw.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\Palette.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\axislib10.exp"
	-@erase "$(OUTDIR)\axislib10.lib"
	-@erase "$(OUTDIR)\axislib10.map"
	-@erase "..\..\..\Release\axislib10.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\axislib10.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\axislib10.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\axislib10.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\axislib10.pdb" /map:"$(INTDIR)\axislib10.map" /machine:I386 /def:".\axislib10.def" /out:"../../../Release/axislib10.dll" /implib:"$(OUTDIR)\axislib10.lib" 
DEF_FILE= \
	".\axislib10.def"
LINK32_OBJS= \
	"$(INTDIR)\axislib10.obj" \
	"$(INTDIR)\Draw.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\Palette.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\axislib10.res"

"..\..\..\Release\axislib10.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "axislib10 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\Debug\axislib10.dll"


CLEAN :
	-@erase "$(INTDIR)\axislib10.obj"
	-@erase "$(INTDIR)\axislib10.pch"
	-@erase "$(INTDIR)\axislib10.res"
	-@erase "$(INTDIR)\Draw.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\Palette.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\axislib10.exp"
	-@erase "$(OUTDIR)\axislib10.lib"
	-@erase "$(OUTDIR)\axislib10.pdb"
	-@erase "..\..\..\Debug\axislib10.dll"
	-@erase "..\..\..\Debug\axislib10.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\axislib10.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\axislib10.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\axislib10.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\axislib10.pdb" /debug /machine:I386 /def:".\axislib10.def" /out:"../../../Debug/axislib10.dll" /implib:"$(OUTDIR)\axislib10.lib" /pdbtype:sept 
DEF_FILE= \
	".\axislib10.def"
LINK32_OBJS= \
	"$(INTDIR)\axislib10.obj" \
	"$(INTDIR)\Draw.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\Palette.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\axislib10.res"

"..\..\..\Debug\axislib10.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("axislib10.dep")
!INCLUDE "axislib10.dep"
!ELSE 
!MESSAGE Warning: cannot find "axislib10.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "axislib10 - Win32 Release" || "$(CFG)" == "axislib10 - Win32 Debug"
SOURCE=.\axislib10.cpp

"$(INTDIR)\axislib10.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axislib10.pch"


SOURCE=.\axislib10.rc

"$(INTDIR)\axislib10.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Draw.cpp

"$(INTDIR)\Draw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axislib10.pch"


SOURCE=.\log.cpp

"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axislib10.pch"


SOURCE=.\Palette.cpp

"$(INTDIR)\Palette.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axislib10.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "axislib10 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\axislib10.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\axislib10.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "axislib10 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\axislib10.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\axislib10.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


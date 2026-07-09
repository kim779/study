# Microsoft Developer Studio Generated NMAKE File, Based on axislib.dsp
!IF "$(CFG)" == ""
CFG=axislib - Win32 Debug
!MESSAGE No configuration specified. Defaulting to axislib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "axislib - Win32 Release" && "$(CFG)" != "axislib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axislib.mak" CFG="axislib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axislib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axislib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "axislib - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\Release\axislib.dll"


CLEAN :
	-@erase "$(INTDIR)\axislib.obj"
	-@erase "$(INTDIR)\axislib.pch"
	-@erase "$(INTDIR)\axislib.res"
	-@erase "$(INTDIR)\Draw.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\Palette.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\axislib.exp"
	-@erase "$(OUTDIR)\axislib.lib"
	-@erase "$(OUTDIR)\axislib.map"
	-@erase "..\..\..\Release\axislib.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\axislib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\axislib.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\axislib.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\axislib.pdb" /map:"$(INTDIR)\axislib.map" /machine:I386 /def:".\axislib.def" /out:"../../../Release/axislib.dll" /implib:"$(OUTDIR)\axislib.lib" 
DEF_FILE= \
	".\axislib.def"
LINK32_OBJS= \
	"$(INTDIR)\axislib.obj" \
	"$(INTDIR)\Draw.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\Palette.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\axislib.res"

"..\..\..\Release\axislib.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "axislib - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\Debug\axislib.dll"


CLEAN :
	-@erase "$(INTDIR)\axislib.obj"
	-@erase "$(INTDIR)\axislib.pch"
	-@erase "$(INTDIR)\axislib.res"
	-@erase "$(INTDIR)\Draw.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\Palette.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\axislib.exp"
	-@erase "$(OUTDIR)\axislib.lib"
	-@erase "$(OUTDIR)\axislib.pdb"
	-@erase "..\..\..\Debug\axislib.dll"
	-@erase "..\..\..\Debug\axislib.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\axislib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\axislib.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\axislib.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\axislib.pdb" /debug /machine:I386 /def:".\axislib.def" /out:"../../../Debug/axislib.dll" /implib:"$(OUTDIR)\axislib.lib" /pdbtype:sept 
DEF_FILE= \
	".\axislib.def"
LINK32_OBJS= \
	"$(INTDIR)\axislib.obj" \
	"$(INTDIR)\Draw.obj" \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\Palette.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\axislib.res"

"..\..\..\Debug\axislib.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("axislib.dep")
!INCLUDE "axislib.dep"
!ELSE 
!MESSAGE Warning: cannot find "axislib.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "axislib - Win32 Release" || "$(CFG)" == "axislib - Win32 Debug"
SOURCE=.\axislib.cpp

"$(INTDIR)\axislib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axislib.pch"


SOURCE=.\axislib.rc

"$(INTDIR)\axislib.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Draw.cpp

"$(INTDIR)\Draw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axislib.pch"


SOURCE=.\log.cpp

"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axislib.pch"


SOURCE=.\Palette.cpp

"$(INTDIR)\Palette.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axislib.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "axislib - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\axislib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\axislib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "axislib - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\axislib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\axislib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


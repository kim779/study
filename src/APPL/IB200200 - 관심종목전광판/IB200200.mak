# Microsoft Developer Studio Generated NMAKE File, Based on IB200200.dsp
!IF "$(CFG)" == ""
CFG=IB200200 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to IB200200 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IB200200 - Win32 Release" && "$(CFG)" != "IB200200 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB200200.mak" CFG="IB200200 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB200200 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB200200 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "IB200200 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\release\IB200200.dll"


CLEAN :
	-@erase "$(INTDIR)\category.obj"
	-@erase "$(INTDIR)\IB200200.obj"
	-@erase "$(INTDIR)\IB200200.pch"
	-@erase "$(INTDIR)\IB200200.res"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\MapDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\IB200200.exp"
	-@erase "$(OUTDIR)\IB200200.lib"
	-@erase "$(OUTDIR)\IB200200.map"
	-@erase "$(OUTDIR)\IB200200.pdb"
	-@erase "..\..\..\release\IB200200.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB200200.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB200200.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB200200.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../../control/fx_grid/release/fx_grid.lib ../../control/fx_misc/release/fx_misc.lib /nologo /base:"0x61000000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\IB200200.pdb" /map:"$(INTDIR)\IB200200.map" /debug /machine:I386 /def:".\IB200200.def" /out:"../../../release/IB200200.dll" /implib:"$(OUTDIR)\IB200200.lib" 
DEF_FILE= \
	".\IB200200.def"
LINK32_OBJS= \
	"$(INTDIR)\category.obj" \
	"$(INTDIR)\IB200200.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\MapDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IB200200.res"

"..\..\..\release\IB200200.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IB200200 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\dev\IB200200.dll"


CLEAN :
	-@erase "$(INTDIR)\category.obj"
	-@erase "$(INTDIR)\IB200200.obj"
	-@erase "$(INTDIR)\IB200200.pch"
	-@erase "$(INTDIR)\IB200200.res"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\MapDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\IB200200.exp"
	-@erase "$(OUTDIR)\IB200200.lib"
	-@erase "$(OUTDIR)\IB200200.pdb"
	-@erase "..\..\..\dev\IB200200.dll"
	-@erase "..\..\..\dev\IB200200.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB200200.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB200200.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB200200.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../../control/fx_grid/debug/fx_grid.lib ../../control/fx_misc/debug/fx_misc.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\IB200200.pdb" /debug /machine:I386 /def:".\IB200200.def" /out:"../../../dev/IB200200.dll" /implib:"$(OUTDIR)\IB200200.lib" /pdbtype:sept 
DEF_FILE= \
	".\IB200200.def"
LINK32_OBJS= \
	"$(INTDIR)\category.obj" \
	"$(INTDIR)\IB200200.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\MapDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IB200200.res"

"..\..\..\dev\IB200200.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("IB200200.dep")
!INCLUDE "IB200200.dep"
!ELSE 
!MESSAGE Warning: cannot find "IB200200.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IB200200 - Win32 Release" || "$(CFG)" == "IB200200 - Win32 Debug"
SOURCE=.\category.cpp

"$(INTDIR)\category.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB200200.pch"


SOURCE=.\IB200200.cpp

"$(INTDIR)\IB200200.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB200200.pch"


SOURCE=.\IB200200.rc

"$(INTDIR)\IB200200.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\libmap.cpp

"$(INTDIR)\libmap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB200200.pch"


SOURCE=.\MapDlg.cpp

"$(INTDIR)\MapDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB200200.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "IB200200 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB200200.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IB200200.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "IB200200 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB200200.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IB200200.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


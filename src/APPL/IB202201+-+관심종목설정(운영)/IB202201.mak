# Microsoft Developer Studio Generated NMAKE File, Based on IB202201.dsp
!IF "$(CFG)" == ""
CFG=IB202201 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to IB202201 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IB202201 - Win32 Release" && "$(CFG)" != "IB202201 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB202201.mak" CFG="IB202201 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB202201 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB202201 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
nullptr=
!ELSE 
nullptr=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IB202201 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\release\IB202201.dll"


CLEAN :
	-@erase "$(INTDIR)\ColorBtn.obj"
	-@erase "$(INTDIR)\IB202201.obj"
	-@erase "$(INTDIR)\IB202201.pch"
	-@erase "$(INTDIR)\IB202201.res"
	-@erase "$(INTDIR)\EditGroup.obj"
	-@erase "$(INTDIR)\EditX.obj"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\MapWnd.obj"
	-@erase "$(INTDIR)\NewGroup.obj"
	-@erase "$(INTDIR)\Page1.obj"
	-@erase "$(INTDIR)\Page2.obj"
	-@erase "$(INTDIR)\Page3.obj"
	-@erase "$(INTDIR)\Page4.obj"
	-@erase "$(INTDIR)\Sheet.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\IB202201.exp"
	-@erase "$(OUTDIR)\IB202201.lib"
	-@erase "$(OUTDIR)\IB202201.map"
	-@erase "$(OUTDIR)\IB202201.pdb"
	-@erase "..\..\..\release\IB202201.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(nullptr)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB202201.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB202201.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB202201.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../../control/fx_grid/release/fx_grid.lib ../../control/fx_misc/release/fx_misc.lib winmm.lib Imm32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\IB202201.pdb" /map:"$(INTDIR)\IB202201.map" /debug /machine:I386 /def:".\IB202201.def" /out:"../../../release/IB202201.dll" /implib:"$(OUTDIR)\IB202201.lib" 
DEF_FILE= \
	".\IB202201.def"
LINK32_OBJS= \
	"$(INTDIR)\ColorBtn.obj" \
	"$(INTDIR)\IB202201.obj" \
	"$(INTDIR)\EditGroup.obj" \
	"$(INTDIR)\EditX.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\NewGroup.obj" \
	"$(INTDIR)\Page1.obj" \
	"$(INTDIR)\Page2.obj" \
	"$(INTDIR)\Page3.obj" \
	"$(INTDIR)\Page4.obj" \
	"$(INTDIR)\Sheet.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IB202201.res"

"..\..\..\release\IB202201.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IB202201 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\dev\IB202201.dll"


CLEAN :
	-@erase "$(INTDIR)\ColorBtn.obj"
	-@erase "$(INTDIR)\IB202201.obj"
	-@erase "$(INTDIR)\IB202201.pch"
	-@erase "$(INTDIR)\IB202201.res"
	-@erase "$(INTDIR)\EditGroup.obj"
	-@erase "$(INTDIR)\EditX.obj"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\MapWnd.obj"
	-@erase "$(INTDIR)\NewGroup.obj"
	-@erase "$(INTDIR)\Page1.obj"
	-@erase "$(INTDIR)\Page2.obj"
	-@erase "$(INTDIR)\Page3.obj"
	-@erase "$(INTDIR)\Page4.obj"
	-@erase "$(INTDIR)\Sheet.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\IB202201.exp"
	-@erase "$(OUTDIR)\IB202201.lib"
	-@erase "$(OUTDIR)\IB202201.pdb"
	-@erase "..\..\..\dev\IB202201.dll"
	-@erase "..\..\..\dev\IB202201.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(nullptr)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB202201.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB202201.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB202201.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../../control/fx_grid/debug/fx_grid.lib ../../control/fx_misc/debug/fx_misc.lib winmm.lib Imm32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\IB202201.pdb" /debug /machine:I386 /def:".\IB202201.def" /out:"../../../dev/IB202201.dll" /implib:"$(OUTDIR)\IB202201.lib" /pdbtype:sept 
DEF_FILE= \
	".\IB202201.def"
LINK32_OBJS= \
	"$(INTDIR)\ColorBtn.obj" \
	"$(INTDIR)\IB202201.obj" \
	"$(INTDIR)\EditGroup.obj" \
	"$(INTDIR)\EditX.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\NewGroup.obj" \
	"$(INTDIR)\Page1.obj" \
	"$(INTDIR)\Page2.obj" \
	"$(INTDIR)\Page3.obj" \
	"$(INTDIR)\Page4.obj" \
	"$(INTDIR)\Sheet.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IB202201.res"

"..\..\..\dev\IB202201.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("IB202201.dep")
!INCLUDE "IB202201.dep"
!ELSE 
!MESSAGE Warning: cannot find "IB202201.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IB202201 - Win32 Release" || "$(CFG)" == "IB202201 - Win32 Debug"
SOURCE=.\ColorBtn.cpp

"$(INTDIR)\ColorBtn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB202201.pch"


SOURCE=.\IB202201.cpp

"$(INTDIR)\IB202201.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB202201.pch"


SOURCE=.\IB202201.rc

"$(INTDIR)\IB202201.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\EditGroup.cpp

"$(INTDIR)\EditGroup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB202201.pch"


SOURCE=.\EditX.cpp

"$(INTDIR)\EditX.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB202201.pch"


SOURCE=.\libmap.cpp

"$(INTDIR)\libmap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB202201.pch"


SOURCE=.\MapWnd.cpp

"$(INTDIR)\MapWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB202201.pch"


SOURCE=.\NewGroup.cpp

"$(INTDIR)\NewGroup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB202201.pch"


SOURCE=.\Page1.cpp

"$(INTDIR)\Page1.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB202201.pch"


SOURCE=.\Page2.cpp

"$(INTDIR)\Page2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB202201.pch"


SOURCE=.\Page3.cpp

"$(INTDIR)\Page3.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB202201.pch"


SOURCE=.\Page4.cpp

"$(INTDIR)\Page4.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB202201.pch"


SOURCE=.\Sheet.cpp

"$(INTDIR)\Sheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB202201.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "IB202201 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB202201.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IB202201.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "IB202201 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB202201.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IB202201.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


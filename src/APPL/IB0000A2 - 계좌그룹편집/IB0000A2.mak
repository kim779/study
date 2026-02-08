# Microsoft Developer Studio Generated NMAKE File, Based on IB0000A2.dsp
!IF "$(CFG)" == ""
CFG=IB0000A2 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to IB0000A2 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IB0000A2 - Win32 Release" && "$(CFG)" != "IB0000A2 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB0000A2.mak" CFG="IB0000A2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB0000A2 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB0000A2 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "IB0000A2 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\Release\IB0000A2.dll"


CLEAN :
	-@erase "$(INTDIR)\AccEdit.obj"
	-@erase "$(INTDIR)\AxMapForm.obj"
	-@erase "$(INTDIR)\AxStatic.obj"
	-@erase "$(INTDIR)\IB0000A2.obj"
	-@erase "$(INTDIR)\IB0000A2.pch"
	-@erase "$(INTDIR)\IB0000A2.res"
	-@erase "$(INTDIR)\dlgDirect.obj"
	-@erase "$(INTDIR)\dlgGrNew.obj"
	-@erase "$(INTDIR)\exTreeCtrl.obj"
	-@erase "$(INTDIR)\ImgBtn.obj"
	-@erase "$(INTDIR)\InPlaceEdit.obj"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\mapForm.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\XXGrid.obj"
	-@erase "$(OUTDIR)\IB0000A2.exp"
	-@erase "$(OUTDIR)\IB0000A2.lib"
	-@erase "..\..\..\Release\IB0000A2.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB0000A2.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB0000A2.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB0000A2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../../control/fx_grid/release/fx_grid.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\IB0000A2.pdb" /machine:I386 /def:".\IB0000A2.def" /out:"../../../Release/IB0000A2.dll" /implib:"$(OUTDIR)\IB0000A2.lib" 
DEF_FILE= \
	".\IB0000A2.def"
LINK32_OBJS= \
	"$(INTDIR)\AccEdit.obj" \
	"$(INTDIR)\AxMapForm.obj" \
	"$(INTDIR)\AxStatic.obj" \
	"$(INTDIR)\IB0000A2.obj" \
	"$(INTDIR)\dlgDirect.obj" \
	"$(INTDIR)\dlgGrNew.obj" \
	"$(INTDIR)\exTreeCtrl.obj" \
	"$(INTDIR)\ImgBtn.obj" \
	"$(INTDIR)\InPlaceEdit.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\mapForm.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\XXGrid.obj" \
	"$(INTDIR)\IB0000A2.res"

"..\..\..\Release\IB0000A2.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IB0000A2 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\Dev\IB0000A2.dll"


CLEAN :
	-@erase "$(INTDIR)\AccEdit.obj"
	-@erase "$(INTDIR)\AxMapForm.obj"
	-@erase "$(INTDIR)\AxStatic.obj"
	-@erase "$(INTDIR)\IB0000A2.obj"
	-@erase "$(INTDIR)\IB0000A2.pch"
	-@erase "$(INTDIR)\IB0000A2.res"
	-@erase "$(INTDIR)\dlgDirect.obj"
	-@erase "$(INTDIR)\dlgGrNew.obj"
	-@erase "$(INTDIR)\exTreeCtrl.obj"
	-@erase "$(INTDIR)\ImgBtn.obj"
	-@erase "$(INTDIR)\InPlaceEdit.obj"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\mapForm.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XXGrid.obj"
	-@erase "$(OUTDIR)\IB0000A2.exp"
	-@erase "$(OUTDIR)\IB0000A2.lib"
	-@erase "$(OUTDIR)\IB0000A2.pdb"
	-@erase "..\..\..\Dev\IB0000A2.dll"
	-@erase "..\..\..\Dev\IB0000A2.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB0000A2.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB0000A2.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB0000A2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../../control/fx_grid/Debug/fx_grid.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\IB0000A2.pdb" /debug /machine:I386 /def:".\IB0000A2.def" /out:"../../../Dev/IB0000A2.dll" /implib:"$(OUTDIR)\IB0000A2.lib" /pdbtype:sept 
DEF_FILE= \
	".\IB0000A2.def"
LINK32_OBJS= \
	"$(INTDIR)\AccEdit.obj" \
	"$(INTDIR)\AxMapForm.obj" \
	"$(INTDIR)\AxStatic.obj" \
	"$(INTDIR)\IB0000A2.obj" \
	"$(INTDIR)\dlgDirect.obj" \
	"$(INTDIR)\dlgGrNew.obj" \
	"$(INTDIR)\exTreeCtrl.obj" \
	"$(INTDIR)\ImgBtn.obj" \
	"$(INTDIR)\InPlaceEdit.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\mapForm.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\XXGrid.obj" \
	"$(INTDIR)\IB0000A2.res"

"..\..\..\Dev\IB0000A2.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("IB0000A2.dep")
!INCLUDE "IB0000A2.dep"
!ELSE 
!MESSAGE Warning: cannot find "IB0000A2.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IB0000A2 - Win32 Release" || "$(CFG)" == "IB0000A2 - Win32 Debug"
SOURCE=.\AccEdit.cpp

"$(INTDIR)\AccEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A2.pch"


SOURCE=.\AxMapForm.cpp

"$(INTDIR)\AxMapForm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A2.pch"


SOURCE=.\AxStatic.cpp

"$(INTDIR)\AxStatic.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A2.pch"


SOURCE=.\IB0000A2.cpp

"$(INTDIR)\IB0000A2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A2.pch"


SOURCE=.\IB0000A2.rc

"$(INTDIR)\IB0000A2.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\dlgDirect.cpp

"$(INTDIR)\dlgDirect.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A2.pch"


SOURCE=.\dlgGrNew.cpp

"$(INTDIR)\dlgGrNew.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A2.pch"


SOURCE=.\exTreeCtrl.cpp

"$(INTDIR)\exTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A2.pch"


SOURCE=.\ImgBtn.cpp

"$(INTDIR)\ImgBtn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A2.pch"


SOURCE=..\..\control\fx_grid\InPlaceEdit.cpp

"$(INTDIR)\InPlaceEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A2.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\libmap.cpp

"$(INTDIR)\libmap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A2.pch"


SOURCE=.\mapForm.cpp

"$(INTDIR)\mapForm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A2.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "IB0000A2 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB0000A2.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IB0000A2.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "IB0000A2 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB0000A2.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IB0000A2.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\XXGrid.cpp

"$(INTDIR)\XXGrid.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A2.pch"



!ENDIF 


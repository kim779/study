# Microsoft Developer Studio Generated NMAKE File, Based on CX_Notify.dsp
!IF "$(CFG)" == ""
CFG=CX_Notify - Win32 Debug
!MESSAGE No configuration specified. Defaulting to CX_Notify - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CX_Notify - Win32 Release" && "$(CFG)" != "CX_Notify - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CX_Notify.mak" CFG="CX_Notify - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CX_Notify - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CX_Notify - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "CX_Notify - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "..\..\..\Release\CX_Notify.dll" "$(OUTDIR)\CX_Notify.tlb"


CLEAN :
	-@erase "$(INTDIR)\ControlWnd.obj"
	-@erase "$(INTDIR)\CX_Notify.obj"
	-@erase "$(INTDIR)\CX_Notify.pch"
	-@erase "$(INTDIR)\CX_Notify.res"
	-@erase "$(INTDIR)\CX_Notify.tlb"
	-@erase "$(INTDIR)\libctrl.obj"
	-@erase "$(INTDIR)\SetDlg.obj"
	-@erase "$(INTDIR)\ShMemory.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\CX_Notify.exp"
	-@erase "$(OUTDIR)\CX_Notify.lib"
	-@erase "$(OUTDIR)\CX_Notify.map"
	-@erase "$(OUTDIR)\CX_Notify.pdb"
	-@erase "..\..\..\Release\CX_Notify.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\CX_Notify.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\CX_Notify.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CX_Notify.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\CX_Notify.pdb" /map:"$(INTDIR)\CX_Notify.map" /debug /machine:I386 /def:".\CX_Notify.def" /out:"../../../Release/CX_Notify.dll" /implib:"$(OUTDIR)\CX_Notify.lib" 
DEF_FILE= \
	".\CX_Notify.def"
LINK32_OBJS= \
	"$(INTDIR)\ControlWnd.obj" \
	"$(INTDIR)\CX_Notify.obj" \
	"$(INTDIR)\libctrl.obj" \
	"$(INTDIR)\SetDlg.obj" \
	"$(INTDIR)\ShMemory.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\CX_Notify.res"

"..\..\..\Release\CX_Notify.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CX_Notify - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\dev\CX_Notify.dll" "$(OUTDIR)\CX_Notify.bsc"


CLEAN :
	-@erase "$(INTDIR)\ControlWnd.obj"
	-@erase "$(INTDIR)\ControlWnd.sbr"
	-@erase "$(INTDIR)\CX_Notify.obj"
	-@erase "$(INTDIR)\CX_Notify.pch"
	-@erase "$(INTDIR)\CX_Notify.res"
	-@erase "$(INTDIR)\CX_Notify.sbr"
	-@erase "$(INTDIR)\CX_Notify.tlb"
	-@erase "$(INTDIR)\libctrl.obj"
	-@erase "$(INTDIR)\libctrl.sbr"
	-@erase "$(INTDIR)\SetDlg.obj"
	-@erase "$(INTDIR)\SetDlg.sbr"
	-@erase "$(INTDIR)\ShMemory.obj"
	-@erase "$(INTDIR)\ShMemory.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\CX_Notify.bsc"
	-@erase "$(OUTDIR)\CX_Notify.exp"
	-@erase "$(OUTDIR)\CX_Notify.lib"
	-@erase "$(OUTDIR)\CX_Notify.pdb"
	-@erase "..\..\..\dev\CX_Notify.dll"
	-@erase "..\..\..\dev\CX_Notify.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CX_Notify.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\CX_Notify.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CX_Notify.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ControlWnd.sbr" \
	"$(INTDIR)\CX_Notify.sbr" \
	"$(INTDIR)\libctrl.sbr" \
	"$(INTDIR)\SetDlg.sbr" \
	"$(INTDIR)\ShMemory.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\CX_Notify.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\CX_Notify.pdb" /debug /machine:I386 /def:".\CX_Notify.def" /out:"../../../dev/CX_Notify.dll" /implib:"$(OUTDIR)\CX_Notify.lib" /pdbtype:sept 
DEF_FILE= \
	".\CX_Notify.def"
LINK32_OBJS= \
	"$(INTDIR)\ControlWnd.obj" \
	"$(INTDIR)\CX_Notify.obj" \
	"$(INTDIR)\libctrl.obj" \
	"$(INTDIR)\SetDlg.obj" \
	"$(INTDIR)\ShMemory.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\CX_Notify.res"

"..\..\..\dev\CX_Notify.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("CX_Notify.dep")
!INCLUDE "CX_Notify.dep"
!ELSE 
!MESSAGE Warning: cannot find "CX_Notify.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CX_Notify - Win32 Release" || "$(CFG)" == "CX_Notify - Win32 Debug"
SOURCE=.\ControlWnd.cpp

!IF  "$(CFG)" == "CX_Notify - Win32 Release"


"$(INTDIR)\ControlWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CX_Notify.pch"


!ELSEIF  "$(CFG)" == "CX_Notify - Win32 Debug"


"$(INTDIR)\ControlWnd.obj"	"$(INTDIR)\ControlWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CX_Notify.pch"


!ENDIF 

SOURCE=.\CX_Notify.cpp

!IF  "$(CFG)" == "CX_Notify - Win32 Release"


"$(INTDIR)\CX_Notify.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CX_Notify.pch"


!ELSEIF  "$(CFG)" == "CX_Notify - Win32 Debug"


"$(INTDIR)\CX_Notify.obj"	"$(INTDIR)\CX_Notify.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CX_Notify.pch"


!ENDIF 

SOURCE=.\CX_Notify.odl

!IF  "$(CFG)" == "CX_Notify - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\CX_Notify.tlb" /mktyplib203 /win32 

"$(OUTDIR)\CX_Notify.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CX_Notify - Win32 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\CX_Notify.tlb" /mktyplib203 /win32 

"$(OUTDIR)\CX_Notify.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\CX_Notify.rc

!IF  "$(CFG)" == "CX_Notify - Win32 Release"


"$(INTDIR)\CX_Notify.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x412 /fo"$(INTDIR)\CX_Notify.res" /i "Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "CX_Notify - Win32 Debug"


"$(INTDIR)\CX_Notify.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CX_Notify.tlb"
	$(RSC) /l 0x412 /fo"$(INTDIR)\CX_Notify.res" /i "Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=.\libctrl.cpp

!IF  "$(CFG)" == "CX_Notify - Win32 Release"


"$(INTDIR)\libctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CX_Notify.pch"


!ELSEIF  "$(CFG)" == "CX_Notify - Win32 Debug"


"$(INTDIR)\libctrl.obj"	"$(INTDIR)\libctrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CX_Notify.pch"


!ENDIF 

SOURCE=.\SetDlg.cpp

!IF  "$(CFG)" == "CX_Notify - Win32 Release"


"$(INTDIR)\SetDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CX_Notify.pch"


!ELSEIF  "$(CFG)" == "CX_Notify - Win32 Debug"


"$(INTDIR)\SetDlg.obj"	"$(INTDIR)\SetDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CX_Notify.pch"


!ENDIF 

SOURCE=.\ShMemory.cpp

!IF  "$(CFG)" == "CX_Notify - Win32 Release"


"$(INTDIR)\ShMemory.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CX_Notify.pch"


!ELSEIF  "$(CFG)" == "CX_Notify - Win32 Debug"


"$(INTDIR)\ShMemory.obj"	"$(INTDIR)\ShMemory.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CX_Notify.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "CX_Notify - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\CX_Notify.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\CX_Notify.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CX_Notify - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CX_Notify.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\CX_Notify.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


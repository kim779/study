# Microsoft Developer Studio Generated NMAKE File, Based on Cx_NotifyEx.dsp
!IF "$(CFG)" == ""
CFG=Cx_NotifyEx - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Cx_NotifyEx - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Cx_NotifyEx - Win32 Release" && "$(CFG)" != "Cx_NotifyEx - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Cx_NotifyEx.mak" CFG="Cx_NotifyEx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Cx_NotifyEx - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Cx_NotifyEx - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Cx_NotifyEx - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "..\..\..\Release\Cx_NotifyEx.dll" "$(OUTDIR)\Cx_NotifyEx.tlb"


CLEAN :
	-@erase "$(INTDIR)\controlWnd.obj"
	-@erase "$(INTDIR)\Cx_NotifyEx.obj"
	-@erase "$(INTDIR)\Cx_NotifyEx.pch"
	-@erase "$(INTDIR)\Cx_NotifyEx.res"
	-@erase "$(INTDIR)\Cx_NotifyEx.tlb"
	-@erase "$(INTDIR)\dxopsenscal.obj"
	-@erase "$(INTDIR)\info.obj"
	-@erase "$(INTDIR)\libctrl.obj"
	-@erase "$(INTDIR)\libDefine.obj"
	-@erase "$(INTDIR)\opSensCal.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Cx_NotifyEx.exp"
	-@erase "$(OUTDIR)\Cx_NotifyEx.lib"
	-@erase "..\..\..\Release\Cx_NotifyEx.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\Cx_NotifyEx.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Cx_NotifyEx.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Cx_NotifyEx.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\Cx_NotifyEx.pdb" /machine:I386 /def:".\Cx_NotifyEx.def" /out:"../../../Release/Cx_NotifyEx.dll" /implib:"$(OUTDIR)\Cx_NotifyEx.lib" 
DEF_FILE= \
	".\Cx_NotifyEx.def"
LINK32_OBJS= \
	"$(INTDIR)\controlWnd.obj" \
	"$(INTDIR)\Cx_NotifyEx.obj" \
	"$(INTDIR)\dxopsenscal.obj" \
	"$(INTDIR)\info.obj" \
	"$(INTDIR)\libctrl.obj" \
	"$(INTDIR)\libDefine.obj" \
	"$(INTDIR)\opSensCal.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Cx_NotifyEx.res"

"..\..\..\Release\Cx_NotifyEx.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Cx_NotifyEx - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\dev\Cx_NotifyEx.dll"


CLEAN :
	-@erase "$(INTDIR)\controlWnd.obj"
	-@erase "$(INTDIR)\Cx_NotifyEx.obj"
	-@erase "$(INTDIR)\Cx_NotifyEx.pch"
	-@erase "$(INTDIR)\Cx_NotifyEx.res"
	-@erase "$(INTDIR)\Cx_NotifyEx.tlb"
	-@erase "$(INTDIR)\dxopsenscal.obj"
	-@erase "$(INTDIR)\info.obj"
	-@erase "$(INTDIR)\libctrl.obj"
	-@erase "$(INTDIR)\libDefine.obj"
	-@erase "$(INTDIR)\opSensCal.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Cx_NotifyEx.exp"
	-@erase "$(OUTDIR)\Cx_NotifyEx.lib"
	-@erase "$(OUTDIR)\Cx_NotifyEx.pdb"
	-@erase "..\..\..\dev\Cx_NotifyEx.dll"
	-@erase "..\..\..\dev\Cx_NotifyEx.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\Cx_NotifyEx.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Cx_NotifyEx.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Cx_NotifyEx.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\Cx_NotifyEx.pdb" /debug /machine:I386 /def:".\Cx_NotifyEx.def" /out:"../../../dev/Cx_NotifyEx.dll" /implib:"$(OUTDIR)\Cx_NotifyEx.lib" /pdbtype:sept 
DEF_FILE= \
	".\Cx_NotifyEx.def"
LINK32_OBJS= \
	"$(INTDIR)\controlWnd.obj" \
	"$(INTDIR)\Cx_NotifyEx.obj" \
	"$(INTDIR)\dxopsenscal.obj" \
	"$(INTDIR)\info.obj" \
	"$(INTDIR)\libctrl.obj" \
	"$(INTDIR)\libDefine.obj" \
	"$(INTDIR)\opSensCal.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Cx_NotifyEx.res"

"..\..\..\dev\Cx_NotifyEx.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Cx_NotifyEx.dep")
!INCLUDE "Cx_NotifyEx.dep"
!ELSE 
!MESSAGE Warning: cannot find "Cx_NotifyEx.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Cx_NotifyEx - Win32 Release" || "$(CFG)" == "Cx_NotifyEx - Win32 Debug"
SOURCE=.\controlWnd.cpp

"$(INTDIR)\controlWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Cx_NotifyEx.pch"


SOURCE=.\Cx_NotifyEx.cpp

"$(INTDIR)\Cx_NotifyEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Cx_NotifyEx.pch"


SOURCE=.\Cx_NotifyEx.odl

!IF  "$(CFG)" == "Cx_NotifyEx - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\Cx_NotifyEx.tlb" /mktyplib203 /win32 

"$(OUTDIR)\Cx_NotifyEx.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Cx_NotifyEx - Win32 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\Cx_NotifyEx.tlb" /mktyplib203 /win32 

"$(OUTDIR)\Cx_NotifyEx.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Cx_NotifyEx.rc

!IF  "$(CFG)" == "Cx_NotifyEx - Win32 Release"


"$(INTDIR)\Cx_NotifyEx.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x412 /fo"$(INTDIR)\Cx_NotifyEx.res" /i "Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "Cx_NotifyEx - Win32 Debug"


"$(INTDIR)\Cx_NotifyEx.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Cx_NotifyEx.tlb"
	$(RSC) /l 0x412 /fo"$(INTDIR)\Cx_NotifyEx.res" /i "Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=.\dxopsenscal.cpp

"$(INTDIR)\dxopsenscal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Cx_NotifyEx.pch"


SOURCE=.\info.cpp

"$(INTDIR)\info.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Cx_NotifyEx.pch"


SOURCE=.\libctrl.cpp

"$(INTDIR)\libctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Cx_NotifyEx.pch"


SOURCE=.\libDefine.cpp

"$(INTDIR)\libDefine.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Cx_NotifyEx.pch"


SOURCE=.\opSensCal.cpp

"$(INTDIR)\opSensCal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Cx_NotifyEx.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Cx_NotifyEx - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\Cx_NotifyEx.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Cx_NotifyEx.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Cx_NotifyEx - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\Cx_NotifyEx.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Cx_NotifyEx.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


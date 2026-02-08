# Microsoft Developer Studio Generated NMAKE File, Based on fx_misc.dsp
!IF "$(CFG)" == ""
CFG=fx_misc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to fx_misc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "fx_misc - Win32 Release" && "$(CFG)" != "fx_misc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fx_misc.mak" CFG="fx_misc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fx_misc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fx_misc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "fx_misc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\release\fx_misc.dll"


CLEAN :
	-@erase "$(INTDIR)\colorCombo.obj"
	-@erase "$(INTDIR)\fontNameCombo.obj"
	-@erase "$(INTDIR)\fontSizeCombo.obj"
	-@erase "$(INTDIR)\fx_misc.obj"
	-@erase "$(INTDIR)\fx_misc.pch"
	-@erase "$(INTDIR)\fx_misc.res"
	-@erase "$(INTDIR)\fxButton.obj"
	-@erase "$(INTDIR)\fxCodeCtrl.obj"
	-@erase "$(INTDIR)\fxColorPopup.obj"
	-@erase "$(INTDIR)\fxEdit.obj"
	-@erase "$(INTDIR)\fxFontDialog.obj"
	-@erase "$(INTDIR)\fxImgButton.obj"
	-@erase "$(INTDIR)\fxLineStyleCombo.obj"
	-@erase "$(INTDIR)\fxSheet.obj"
	-@erase "$(INTDIR)\fxSpin.obj"
	-@erase "$(INTDIR)\fxStatic.obj"
	-@erase "$(INTDIR)\fxTab.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\fx_misc.exp"
	-@erase "$(OUTDIR)\fx_misc.lib"
	-@erase "..\..\..\release\fx_misc.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\fx_misc.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\fx_misc.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fx_misc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=imm32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\fx_misc.pdb" /machine:I386 /def:".\fx_misc.def" /out:"../../../release/fx_misc.dll" /implib:"$(OUTDIR)\fx_misc.lib" 
DEF_FILE= \
	".\fx_misc.def"
LINK32_OBJS= \
	"$(INTDIR)\colorCombo.obj" \
	"$(INTDIR)\fontNameCombo.obj" \
	"$(INTDIR)\fontSizeCombo.obj" \
	"$(INTDIR)\fx_misc.obj" \
	"$(INTDIR)\fxButton.obj" \
	"$(INTDIR)\fxCodeCtrl.obj" \
	"$(INTDIR)\fxColorPopup.obj" \
	"$(INTDIR)\fxEdit.obj" \
	"$(INTDIR)\fxFontDialog.obj" \
	"$(INTDIR)\fxImgButton.obj" \
	"$(INTDIR)\fxLineStyleCombo.obj" \
	"$(INTDIR)\fxSheet.obj" \
	"$(INTDIR)\fxSpin.obj" \
	"$(INTDIR)\fxStatic.obj" \
	"$(INTDIR)\fxTab.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\fx_misc.res"

"..\..\..\release\fx_misc.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "fx_misc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\dev\fx_misc.dll"


CLEAN :
	-@erase "$(INTDIR)\colorCombo.obj"
	-@erase "$(INTDIR)\fontNameCombo.obj"
	-@erase "$(INTDIR)\fontSizeCombo.obj"
	-@erase "$(INTDIR)\fx_misc.obj"
	-@erase "$(INTDIR)\fx_misc.pch"
	-@erase "$(INTDIR)\fx_misc.res"
	-@erase "$(INTDIR)\fxButton.obj"
	-@erase "$(INTDIR)\fxCodeCtrl.obj"
	-@erase "$(INTDIR)\fxColorPopup.obj"
	-@erase "$(INTDIR)\fxEdit.obj"
	-@erase "$(INTDIR)\fxFontDialog.obj"
	-@erase "$(INTDIR)\fxImgButton.obj"
	-@erase "$(INTDIR)\fxLineStyleCombo.obj"
	-@erase "$(INTDIR)\fxSheet.obj"
	-@erase "$(INTDIR)\fxSpin.obj"
	-@erase "$(INTDIR)\fxStatic.obj"
	-@erase "$(INTDIR)\fxTab.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\fx_misc.exp"
	-@erase "$(OUTDIR)\fx_misc.lib"
	-@erase "$(OUTDIR)\fx_misc.pdb"
	-@erase "..\..\..\dev\fx_misc.dll"
	-@erase "..\..\..\dev\fx_misc.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\fx_misc.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\fx_misc.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fx_misc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=imm32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\fx_misc.pdb" /debug /machine:I386 /def:".\fx_misc.def" /out:"../../../dev/fx_misc.dll" /implib:"$(OUTDIR)\fx_misc.lib" /pdbtype:sept 
DEF_FILE= \
	".\fx_misc.def"
LINK32_OBJS= \
	"$(INTDIR)\colorCombo.obj" \
	"$(INTDIR)\fontNameCombo.obj" \
	"$(INTDIR)\fontSizeCombo.obj" \
	"$(INTDIR)\fx_misc.obj" \
	"$(INTDIR)\fxButton.obj" \
	"$(INTDIR)\fxCodeCtrl.obj" \
	"$(INTDIR)\fxColorPopup.obj" \
	"$(INTDIR)\fxEdit.obj" \
	"$(INTDIR)\fxFontDialog.obj" \
	"$(INTDIR)\fxImgButton.obj" \
	"$(INTDIR)\fxLineStyleCombo.obj" \
	"$(INTDIR)\fxSheet.obj" \
	"$(INTDIR)\fxSpin.obj" \
	"$(INTDIR)\fxStatic.obj" \
	"$(INTDIR)\fxTab.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\fx_misc.res"

"..\..\..\dev\fx_misc.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("fx_misc.dep")
!INCLUDE "fx_misc.dep"
!ELSE 
!MESSAGE Warning: cannot find "fx_misc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "fx_misc - Win32 Release" || "$(CFG)" == "fx_misc - Win32 Debug"
SOURCE=.\colorCombo.cpp

"$(INTDIR)\colorCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fontNameCombo.cpp

"$(INTDIR)\fontNameCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fontSizeCombo.cpp

"$(INTDIR)\fontSizeCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fx_misc.cpp

"$(INTDIR)\fx_misc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fx_misc.rc

"$(INTDIR)\fx_misc.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\fxButton.cpp

"$(INTDIR)\fxButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fxCodeCtrl.cpp

"$(INTDIR)\fxCodeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fxColorPopup.cpp

"$(INTDIR)\fxColorPopup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fxEdit.cpp

"$(INTDIR)\fxEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fxFontDialog.cpp

"$(INTDIR)\fxFontDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fxImgButton.cpp

"$(INTDIR)\fxImgButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fxLineStyleCombo.cpp

"$(INTDIR)\fxLineStyleCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fxSheet.cpp

"$(INTDIR)\fxSheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fxSpin.cpp

"$(INTDIR)\fxSpin.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fxStatic.cpp

"$(INTDIR)\fxStatic.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\fxTab.cpp

"$(INTDIR)\fxTab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\fx_misc.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "fx_misc - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\fx_misc.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\fx_misc.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "fx_misc - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\fx_misc.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\fx_misc.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


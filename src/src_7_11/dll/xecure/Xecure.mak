# Microsoft Developer Studio Generated NMAKE File, Based on Xecure.dsp
!IF "$(CFG)" == ""
CFG=Xecure - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Xecure - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Xecure - Win32 Release" && "$(CFG)" != "Xecure - Win32 Debug" && "$(CFG)" != "Xecure - Win32 Unicode Debug" && "$(CFG)" != "Xecure - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Xecure.mak" CFG="Xecure - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Xecure - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Xecure - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Xecure - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Xecure - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Xecure - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\Release\axXecure.ocx" ".\Release\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\Certify.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Xecure.obj"
	-@erase "$(INTDIR)\Xecure.pch"
	-@erase "$(INTDIR)\Xecure.res"
	-@erase "$(INTDIR)\Xecure.tlb"
	-@erase "$(INTDIR)\XecureCtl.obj"
	-@erase "$(INTDIR)\XecurePpg.obj"
	-@erase "$(OUTDIR)\axXecure.exp"
	-@erase "$(OUTDIR)\axXecure.lib"
	-@erase "..\..\Release\axXecure.ocx"
	-@erase ".\Release\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\Xecure.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Xecure.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Xecure.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\axXecure.pdb" /machine:I386 /def:".\Xecure.def" /out:"../../Release/axXecure.ocx" /implib:"$(OUTDIR)\axXecure.lib" 
DEF_FILE= \
	".\Xecure.def"
LINK32_OBJS= \
	"$(INTDIR)\Certify.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Xecure.obj" \
	"$(INTDIR)\XecureCtl.obj" \
	"$(INTDIR)\XecurePpg.obj" \
	"$(INTDIR)\Xecure.res"

"..\..\Release\axXecure.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Release
TargetPath=\axisEx\Release\axXecure.ocx
InputPath=\axisEx\Release\axXecure.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "Xecure - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\Debug\axXecure.ocx" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\Certify.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Xecure.obj"
	-@erase "$(INTDIR)\Xecure.pch"
	-@erase "$(INTDIR)\Xecure.res"
	-@erase "$(INTDIR)\Xecure.tlb"
	-@erase "$(INTDIR)\XecureCtl.obj"
	-@erase "$(INTDIR)\XecurePpg.obj"
	-@erase "$(OUTDIR)\axXecure.exp"
	-@erase "$(OUTDIR)\axXecure.lib"
	-@erase "$(OUTDIR)\axXecure.pdb"
	-@erase "..\..\Debug\axXecure.ilk"
	-@erase "..\..\Debug\axXecure.ocx"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\Xecure.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Xecure.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Xecure.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\axXecure.pdb" /debug /machine:I386 /def:".\Xecure.def" /out:"../../Debug/axXecure.ocx" /implib:"$(OUTDIR)\axXecure.lib" /pdbtype:sept 
DEF_FILE= \
	".\Xecure.def"
LINK32_OBJS= \
	"$(INTDIR)\Certify.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Xecure.obj" \
	"$(INTDIR)\XecureCtl.obj" \
	"$(INTDIR)\XecurePpg.obj" \
	"$(INTDIR)\Xecure.res"

"..\..\Debug\axXecure.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=\axisEx\Debug\axXecure.ocx
InputPath=\axisEx\Debug\axXecure.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "Xecure - Win32 Unicode Debug"

OUTDIR=.\DebugU
INTDIR=.\DebugU
# Begin Custom Macros
OutDir=.\DebugU
# End Custom Macros

ALL : "..\..\DebugU\axXecure.ocx" "$(OUTDIR)\Xecure.tlb" ".\DebugU\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\Certify.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Xecure.obj"
	-@erase "$(INTDIR)\Xecure.pch"
	-@erase "$(INTDIR)\Xecure.res"
	-@erase "$(INTDIR)\Xecure.tlb"
	-@erase "$(INTDIR)\XecureCtl.obj"
	-@erase "$(INTDIR)\XecurePpg.obj"
	-@erase "$(OUTDIR)\axXecure.exp"
	-@erase "$(OUTDIR)\axXecure.lib"
	-@erase "$(OUTDIR)\axXecure.pdb"
	-@erase "..\..\DebugU\axXecure.ilk"
	-@erase "..\..\DebugU\axXecure.ocx"
	-@erase ".\DebugU\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\Xecure.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Xecure.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Xecure.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\axXecure.pdb" /debug /machine:I386 /def:".\Xecure.def" /out:"../../DebugU/axXecure.ocx" /implib:"$(OUTDIR)\axXecure.lib" /pdbtype:sept 
DEF_FILE= \
	".\Xecure.def"
LINK32_OBJS= \
	"$(INTDIR)\Certify.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Xecure.obj" \
	"$(INTDIR)\XecureCtl.obj" \
	"$(INTDIR)\XecurePpg.obj" \
	"$(INTDIR)\Xecure.res"

"..\..\DebugU\axXecure.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\DebugU
TargetPath=\axisEx\DebugU\axXecure.ocx
InputPath=\axisEx\DebugU\axXecure.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "Xecure - Win32 Unicode Release"

OUTDIR=.\ReleaseU
INTDIR=.\ReleaseU
# Begin Custom Macros
OutDir=.\ReleaseU
# End Custom Macros

ALL : "..\..\ReleaseU\axXecure.ocx" "$(OUTDIR)\Xecure.tlb" ".\ReleaseU\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\Certify.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Xecure.obj"
	-@erase "$(INTDIR)\Xecure.pch"
	-@erase "$(INTDIR)\Xecure.res"
	-@erase "$(INTDIR)\Xecure.tlb"
	-@erase "$(INTDIR)\XecureCtl.obj"
	-@erase "$(INTDIR)\XecurePpg.obj"
	-@erase "$(OUTDIR)\axXecure.exp"
	-@erase "$(OUTDIR)\axXecure.lib"
	-@erase "..\..\ReleaseU\axXecure.ocx"
	-@erase ".\ReleaseU\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\Xecure.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Xecure.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Xecure.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\axXecure.pdb" /machine:I386 /def:".\Xecure.def" /out:"../../ReleaseU/axXecure.ocx" /implib:"$(OUTDIR)\axXecure.lib" 
DEF_FILE= \
	".\Xecure.def"
LINK32_OBJS= \
	"$(INTDIR)\Certify.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Xecure.obj" \
	"$(INTDIR)\XecureCtl.obj" \
	"$(INTDIR)\XecurePpg.obj" \
	"$(INTDIR)\Xecure.res"

"..\..\ReleaseU\axXecure.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\ReleaseU
TargetPath=\axisEx\ReleaseU\axXecure.ocx
InputPath=\axisEx\ReleaseU\axXecure.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
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
!IF EXISTS("Xecure.dep")
!INCLUDE "Xecure.dep"
!ELSE 
!MESSAGE Warning: cannot find "Xecure.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Xecure - Win32 Release" || "$(CFG)" == "Xecure - Win32 Debug" || "$(CFG)" == "Xecure - Win32 Unicode Debug" || "$(CFG)" == "Xecure - Win32 Unicode Release"
SOURCE=.\Certify.cpp

"$(INTDIR)\Certify.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Xecure.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Xecure - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\Xecure.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Xecure.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Xecure - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\Xecure.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Xecure.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Xecure - Win32 Unicode Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\Xecure.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Xecure.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Xecure - Win32 Unicode Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\Xecure.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Xecure.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Xecure.cpp

"$(INTDIR)\Xecure.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Xecure.pch"


SOURCE=.\Xecure.odl

!IF  "$(CFG)" == "Xecure - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\Xecure.tlb" /mktyplib203 /win32 

"$(OUTDIR)\Xecure.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Xecure - Win32 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\Xecure.tlb" /mktyplib203 /win32 

"$(OUTDIR)\Xecure.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Xecure - Win32 Unicode Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\Xecure.tlb" /mktyplib203 /win32 

"$(OUTDIR)\Xecure.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Xecure - Win32 Unicode Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\Xecure.tlb" /mktyplib203 /win32 

"$(OUTDIR)\Xecure.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Xecure.rc

!IF  "$(CFG)" == "Xecure - Win32 Release"


"$(INTDIR)\Xecure.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Xecure.tlb"
	$(RSC) /l 0x412 /fo"$(INTDIR)\Xecure.res" /i "Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "Xecure - Win32 Debug"


"$(INTDIR)\Xecure.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Xecure.tlb"
	$(RSC) /l 0x412 /fo"$(INTDIR)\Xecure.res" /i "Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "Xecure - Win32 Unicode Debug"


"$(INTDIR)\Xecure.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x412 /fo"$(INTDIR)\Xecure.res" /i "DebugU" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "Xecure - Win32 Unicode Release"


"$(INTDIR)\Xecure.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x412 /fo"$(INTDIR)\Xecure.res" /i "ReleaseU" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=.\XecureCtl.cpp

"$(INTDIR)\XecureCtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Xecure.pch"


SOURCE=.\XecurePpg.cpp

"$(INTDIR)\XecurePpg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Xecure.pch"



!ENDIF 


# Microsoft Developer Studio Generated NMAKE File, Based on SystemTradingParser_YES.dsp
!IF "$(CFG)" == ""
CFG=SystemTradingParser_YES - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SystemTradingParser_YES - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SystemTradingParser_YES - Win32 Release" && "$(CFG)" != "SystemTradingParser_YES - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SystemTradingParser_YES.mak" CFG="SystemTradingParser_YES - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SystemTradingParser_YES - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SystemTradingParser_YES - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "SystemTradingParser_YES - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SystemTradingParser_YES.dll"

!ELSE 

ALL : "SysTrading - Win32 Release" "$(OUTDIR)\SystemTradingParser.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"SysTrading - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SystemTradingParser.obj"
	-@erase "$(INTDIR)\SystemTradingParser.pch"
	-@erase "$(INTDIR)\SystemTradingParser.res"
	-@erase "$(INTDIR)\SysTrading.l.obj"
	-@erase "$(INTDIR)\SysTrading.obj"
	-@erase "$(INTDIR)\SysTradingFunctions.obj"
	-@erase "$(INTDIR)\SysTradingParser.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SystemTradingParser.dll"
	-@erase "$(OUTDIR)\SystemTradingParser.exp"
	-@erase "$(OUTDIR)\SystemTradingParser.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\SystemTradingParser.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SystemTradingParser.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SystemTradingParser.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\SystemTradingParser.pdb" /machine:I386 /def:".\SystemTradingParser.def" /out:"$(OUTDIR)\SystemTradingParser.dll" /implib:"$(OUTDIR)\SystemTradingParser.lib" 
DEF_FILE= \
	".\SystemTradingParser.def"
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SystemTradingParser.obj" \
	"$(INTDIR)\SysTrading.obj" \
	"$(INTDIR)\SysTrading.l.obj" \
	"$(INTDIR)\SysTradingParser.obj" \
	"$(INTDIR)\SystemTradingParser.res" \
	"$(INTDIR)\SysTradingFunctions.obj"

"$(OUTDIR)\SystemTradingParser.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SystemTradingParser_YES - Win32 Debug"

OUTDIR=.\../SystemTradingParser_YES/Debug
INTDIR=.\../SystemTradingParser_YES/Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\..\SystemTrading\BinD\SystemTradingParser.dll"

!ELSE 

ALL : "SysTrading - Win32 Debug" "..\..\..\..\..\SystemTrading\BinD\SystemTradingParser.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"SysTrading - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SystemTradingParser.obj"
	-@erase "$(INTDIR)\SystemTradingParser.pch"
	-@erase "$(INTDIR)\SystemTradingParser.res"
	-@erase "$(INTDIR)\SysTrading.l.obj"
	-@erase "$(INTDIR)\SysTrading.obj"
	-@erase "$(INTDIR)\SysTradingFunctions.obj"
	-@erase "$(INTDIR)\SysTradingParser.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SystemTradingParser.exp"
	-@erase "$(OUTDIR)\SystemTradingParser.lib"
	-@erase "$(OUTDIR)\SystemTradingParser.pdb"
	-@erase "..\..\..\..\..\SystemTrading\BinD\SystemTradingParser.dll"
	-@erase "..\..\..\..\..\SystemTrading\BinD\SystemTradingParser.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\SystemTradingParser.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SystemTradingParser.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SystemTradingParser.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\SystemTradingParser.pdb" /debug /machine:I386 /def:".\SystemTradingParser.def" /out:"d:\SystemTrading\BinD\SystemTradingParser.dll" /implib:"$(OUTDIR)\SystemTradingParser.lib" /pdbtype:sept 
DEF_FILE= \
	".\SystemTradingParser.def"
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SystemTradingParser.obj" \
	"$(INTDIR)\SysTrading.obj" \
	"$(INTDIR)\SysTrading.l.obj" \
	"$(INTDIR)\SysTradingParser.obj" \
	"$(INTDIR)\SystemTradingParser.res" \
	"$(INTDIR)\SysTradingFunctions.obj"

"..\..\..\..\..\SystemTrading\BinD\SystemTradingParser.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("SystemTradingParser.dep")
!INCLUDE "SystemTradingParser.dep"
!ELSE 
!MESSAGE Warning: cannot find "SystemTradingParser.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SystemTradingParser_YES - Win32 Release" || "$(CFG)" == "SystemTradingParser_YES - Win32 Debug"
SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "SystemTradingParser_YES - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\SystemTradingParser.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SystemTradingParser.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SystemTradingParser_YES - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\SystemTradingParser.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SystemTradingParser.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\SystemTradingParser.cpp

"$(INTDIR)\SystemTradingParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SystemTradingParser.pch"


SOURCE=.\SystemTradingParser.rc

"$(INTDIR)\SystemTradingParser.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\SysTrading_YES\SysTrading.cpp

"$(INTDIR)\SysTrading.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SystemTradingParser.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\SysTrading_YES\SysTrading.l.cpp

"$(INTDIR)\SysTrading.l.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SystemTradingParser.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\SysTrading_YES\SysTradingFunctions.cpp

"$(INTDIR)\SysTradingFunctions.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SystemTradingParser.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\SysTrading_YES\SysTradingParser_YES.cpp

"$(INTDIR)\SysTradingParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SystemTradingParser.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "SystemTradingParser_YES - Win32 Release"

"SysTrading - Win32 Release" : 
   cd ".\SysTrading_YES"
   $(MAKE) /$(MAKEFLAGS) /F .\SysTrading_YES.mak CFG="SysTrading_YES - Win32 Release" 
   cd ".."

"SysTrading - Win32 ReleaseCLEAN" : 
   cd ".\SysTrading_YES"
   $(MAKE) /$(MAKEFLAGS) /F .\SysTrading_YES.mak CFG="SysTrading_YES - Win32 Release" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "SystemTradingParser_YES - Win32 Debug"

"SysTrading - Win32 Debug" : 
   cd ".\SysTrading_YES"
   $(MAKE) /$(MAKEFLAGS) /F .\SysTrading_YES.mak CFG="SysTrading_YES - Win32 Debug" 
   cd ".."

"SysTrading - Win32 DebugCLEAN" : 
   cd ".\SysTrading_YES"
   $(MAKE) /$(MAKEFLAGS) /F .\SysTrading_YES.mak CFG="SysTrading_YES - Win32 Debug" RECURSE=1 CLEAN 
   cd ".."

!ENDIF 


!ENDIF 


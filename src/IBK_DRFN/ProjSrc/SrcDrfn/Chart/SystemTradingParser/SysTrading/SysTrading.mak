# Microsoft Developer Studio Generated NMAKE File, Based on SysTrading.dsp
!IF "$(CFG)" == ""
CFG=SysTrading - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SysTrading - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SysTrading - Win32 Release" && "$(CFG)" != "SysTrading - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SysTrading.mak" CFG="SysTrading - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SysTrading - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SysTrading - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "SysTrading - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\SysTrading.exe"


CLEAN :
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SysTrading.l.obj"
	-@erase "$(INTDIR)\SysTrading.obj"
	-@erase "$(INTDIR)\SysTradingFunctions.obj"
	-@erase "$(INTDIR)\SysTradingParser.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SysTrading.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "YY_NEVER_INTERACTIVE" /Fp"$(INTDIR)\SysTrading.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SysTrading.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=libflex.lib /nologo /incremental:no /pdb:"$(OUTDIR)\SysTrading.pdb" /machine:I386 /out:"$(OUTDIR)\SysTrading.exe" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SysTrading.obj" \
	"$(INTDIR)\SysTrading.l.obj" \
	"$(INTDIR)\SysTradingParser.obj" \
	"$(INTDIR)\SysTradingFunctions.obj"

"$(OUTDIR)\SysTrading.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SysTrading - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\SysTrading.exe"


CLEAN :
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SysTrading.l.obj"
	-@erase "$(INTDIR)\SysTrading.obj"
	-@erase "$(INTDIR)\SysTradingFunctions.obj"
	-@erase "$(INTDIR)\SysTradingParser.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SysTrading.exe"
	-@erase "$(OUTDIR)\SysTrading.ilk"
	-@erase "$(OUTDIR)\SysTrading.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "YY_NEVER_INTERACTIVE" /Fp"$(INTDIR)\SysTrading.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SysTrading.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=libflex.lib /nologo /incremental:yes /pdb:"$(OUTDIR)\SysTrading.pdb" /debug /machine:I386 /out:"$(OUTDIR)\SysTrading.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SysTrading.obj" \
	"$(INTDIR)\SysTrading.l.obj" \
	"$(INTDIR)\SysTradingParser.obj" \
	"$(INTDIR)\SysTradingFunctions.obj"

"$(OUTDIR)\SysTrading.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("SysTrading.dep")
!INCLUDE "SysTrading.dep"
!ELSE 
!MESSAGE Warning: cannot find "SysTrading.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SysTrading - Win32 Release" || "$(CFG)" == "SysTrading - Win32 Debug"
SOURCE=.\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\StdAfx.cpp

"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SysTrading.cpp

"$(INTDIR)\SysTrading.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SysTrading.l.cpp

"$(INTDIR)\SysTrading.l.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SysTradingFunctions.cpp

"$(INTDIR)\SysTradingFunctions.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SysTradingParser.cpp

"$(INTDIR)\SysTradingParser.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SysTrading.l

!IF  "$(CFG)" == "SysTrading - Win32 Release"

InputPath=.\SysTrading.l
InputName=SysTrading

".\SysTrading.l.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	flex.exe -PSysTrading -t $(InputName).l > $(InputName).l.cpp
<< 
	

!ELSEIF  "$(CFG)" == "SysTrading - Win32 Debug"

InputPath=.\SysTrading.l
InputName=SysTrading

".\SysTrading.l.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	flex.exe -PSysTrading -t $(InputName).l > $(InputName).l.cpp
<< 
	

!ENDIF 

SOURCE=.\SysTrading.y

!IF  "$(CFG)" == "SysTrading - Win32 Release"

InputPath=.\SysTrading.y
InputName=SysTrading

".\SysTrading.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	bison.exe -pSysTrading -o SysTrading.cpp -d $(InputName).y
<< 
	

!ELSEIF  "$(CFG)" == "SysTrading - Win32 Debug"

InputPath=.\SysTrading.y
InputName=SysTrading

".\SysTrading.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	bison.exe -pSysTrading -o SysTrading.cpp -d $(InputName).y
<< 
	

!ENDIF 


!ENDIF 


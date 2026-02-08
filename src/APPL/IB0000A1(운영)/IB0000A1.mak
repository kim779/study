# Microsoft Developer Studio Generated NMAKE File, Based on IB0000A1.dsp
!IF "$(CFG)" == ""
CFG=IB0000A1 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to IB0000A1 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IB0000A1 - Win32 Release" && "$(CFG)" != "IB0000A1 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB0000A1.mak" CFG="IB0000A1 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB0000A1 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB0000A1 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "IB0000A1 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\Release\IB0000A1.dll"


CLEAN :
	-@erase "$(INTDIR)\IB0000A1.obj"
	-@erase "$(INTDIR)\IB0000A1.pch"
	-@erase "$(INTDIR)\IB0000A1.res"
	-@erase "$(INTDIR)\MapForm.obj"
	-@erase "$(INTDIR)\MapWnd.obj"
	-@erase "$(INTDIR)\MyBitmap.obj"
	-@erase "$(INTDIR)\MyButton.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TooltipDlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XHeaderCtrl.obj"
	-@erase "$(INTDIR)\XListCtrl.obj"
	-@erase "$(OUTDIR)\IB0000A1.exp"
	-@erase "$(OUTDIR)\IB0000A1.lib"
	-@erase "$(OUTDIR)\IB0000A1.map"
	-@erase "$(OUTDIR)\IB0000A1.pdb"
	-@erase "..\..\..\Release\IB0000A1.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB0000A1.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB0000A1.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB0000A1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /base:"0x66100000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\IB0000A1.pdb" /map:"$(INTDIR)\IB0000A1.map" /debug /machine:I386 /def:".\IB0000A1.def" /out:"../../../Release/IB0000A1.dll" /implib:"$(OUTDIR)\IB0000A1.lib" 
LINK32_OBJS= \
	"$(INTDIR)\IB0000A1.obj" \
	"$(INTDIR)\MapForm.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\MyBitmap.obj" \
	"$(INTDIR)\MyButton.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TooltipDlg.obj" \
	"$(INTDIR)\XHeaderCtrl.obj" \
	"$(INTDIR)\XListCtrl.obj" \
	"$(INTDIR)\IB0000A1.res"

"..\..\..\Release\IB0000A1.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IB0000A1 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\dev\IB0000A1.dll"


CLEAN :
	-@erase "$(INTDIR)\IB0000A1.obj"
	-@erase "$(INTDIR)\IB0000A1.pch"
	-@erase "$(INTDIR)\IB0000A1.res"
	-@erase "$(INTDIR)\MapForm.obj"
	-@erase "$(INTDIR)\MapWnd.obj"
	-@erase "$(INTDIR)\MyBitmap.obj"
	-@erase "$(INTDIR)\MyButton.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TooltipDlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XHeaderCtrl.obj"
	-@erase "$(INTDIR)\XListCtrl.obj"
	-@erase "$(OUTDIR)\IB0000A1.exp"
	-@erase "$(OUTDIR)\IB0000A1.lib"
	-@erase "$(OUTDIR)\IB0000A1.pdb"
	-@erase "..\..\..\dev\IB0000A1.dll"
	-@erase "..\..\..\dev\IB0000A1.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB0000A1.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB0000A1.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB0000A1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\IB0000A1.pdb" /debug /machine:I386 /def:".\IB0000A1.def" /out:"../../../dev/IB0000A1.dll" /implib:"$(OUTDIR)\IB0000A1.lib" /pdbtype:sept 
DEF_FILE= \
	".\IB0000A1.def"
LINK32_OBJS= \
	"$(INTDIR)\IB0000A1.obj" \
	"$(INTDIR)\MapForm.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\MyBitmap.obj" \
	"$(INTDIR)\MyButton.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TooltipDlg.obj" \
	"$(INTDIR)\XHeaderCtrl.obj" \
	"$(INTDIR)\XListCtrl.obj" \
	"$(INTDIR)\IB0000A1.res"

"..\..\..\dev\IB0000A1.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("IB0000A1.dep")
!INCLUDE "IB0000A1.dep"
!ELSE 
!MESSAGE Warning: cannot find "IB0000A1.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IB0000A1 - Win32 Release" || "$(CFG)" == "IB0000A1 - Win32 Debug"
SOURCE=.\IB0000A1.cpp

"$(INTDIR)\IB0000A1.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A1.pch"


SOURCE=.\IB0000A1.rc

"$(INTDIR)\IB0000A1.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\MapForm.cpp

"$(INTDIR)\MapForm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A1.pch"


SOURCE=.\MapWnd.cpp

"$(INTDIR)\MapWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A1.pch"


SOURCE=.\MyBitmap.cpp

"$(INTDIR)\MyBitmap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A1.pch"


SOURCE=.\MyButton.cpp

"$(INTDIR)\MyButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A1.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "IB0000A1 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB0000A1.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IB0000A1.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "IB0000A1 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB0000A1.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IB0000A1.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TooltipDlg.cpp

"$(INTDIR)\TooltipDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A1.pch"


SOURCE=..\..\dll\ctrl\XHeaderCtrl.cpp

"$(INTDIR)\XHeaderCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A1.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\dll\ctrl\XListCtrl.cpp

"$(INTDIR)\XListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB0000A1.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 


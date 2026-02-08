# Microsoft Developer Studio Generated NMAKE File, Based on IB410100.dsp
!IF "$(CFG)" == ""
CFG=IB410100 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to IB410100 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IB410100 - Win32 Release" && "$(CFG)" != "IB410100 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB410100.mak" CFG="IB410100 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB410100 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB410100 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "IB410100 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "..\..\..\release\IB410100.dll" "$(OUTDIR)\IB410100.bsc"


CLEAN :
	-@erase "$(INTDIR)\Account.obj"
	-@erase "$(INTDIR)\Account.sbr"
	-@erase "$(INTDIR)\Cfg.obj"
	-@erase "$(INTDIR)\Cfg.sbr"
	-@erase "$(INTDIR)\CrossLine.obj"
	-@erase "$(INTDIR)\CrossLine.sbr"
	-@erase "$(INTDIR)\cxcfg.obj"
	-@erase "$(INTDIR)\cxcfg.sbr"
	-@erase "$(INTDIR)\IB410100.obj"
	-@erase "$(INTDIR)\IB410100.pch"
	-@erase "$(INTDIR)\IB410100.res"
	-@erase "$(INTDIR)\IB410100.sbr"
	-@erase "$(INTDIR)\DlgSave.obj"
	-@erase "$(INTDIR)\DlgSave.sbr"
	-@erase "$(INTDIR)\dxopsenscal.obj"
	-@erase "$(INTDIR)\dxopsenscal.sbr"
	-@erase "$(INTDIR)\excel9.obj"
	-@erase "$(INTDIR)\excel9.sbr"
	-@erase "$(INTDIR)\GridWnd.obj"
	-@erase "$(INTDIR)\GridWnd.sbr"
	-@erase "$(INTDIR)\GrpWnd.obj"
	-@erase "$(INTDIR)\GrpWnd.sbr"
	-@erase "$(INTDIR)\HedgePage.obj"
	-@erase "$(INTDIR)\HedgePage.sbr"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\libmap.sbr"
	-@erase "$(INTDIR)\MapWnd.obj"
	-@erase "$(INTDIR)\MapWnd.sbr"
	-@erase "$(INTDIR)\MGrpWnd.obj"
	-@erase "$(INTDIR)\MGrpWnd.sbr"
	-@erase "$(INTDIR)\MyStgPage.obj"
	-@erase "$(INTDIR)\MyStgPage.sbr"
	-@erase "$(INTDIR)\Notify.obj"
	-@erase "$(INTDIR)\Notify.sbr"
	-@erase "$(INTDIR)\OpSensCal.obj"
	-@erase "$(INTDIR)\OpSensCal.sbr"
	-@erase "$(INTDIR)\PopupWnd.obj"
	-@erase "$(INTDIR)\PopupWnd.sbr"
	-@erase "$(INTDIR)\Price.obj"
	-@erase "$(INTDIR)\Price.sbr"
	-@erase "$(INTDIR)\SendOrder.obj"
	-@erase "$(INTDIR)\SendOrder.sbr"
	-@erase "$(INTDIR)\Sheet1.obj"
	-@erase "$(INTDIR)\Sheet1.sbr"
	-@erase "$(INTDIR)\SonikPage.obj"
	-@erase "$(INTDIR)\SonikPage.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\StgBoard.obj"
	-@erase "$(INTDIR)\StgBoard.sbr"
	-@erase "$(INTDIR)\StgInfo.obj"
	-@erase "$(INTDIR)\StgInfo.sbr"
	-@erase "$(INTDIR)\StgPage.obj"
	-@erase "$(INTDIR)\StgPage.sbr"
	-@erase "$(INTDIR)\ToolTip.obj"
	-@erase "$(INTDIR)\ToolTip.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\IB410100.bsc"
	-@erase "$(OUTDIR)\IB410100.exp"
	-@erase "$(OUTDIR)\IB410100.lib"
	-@erase "..\..\..\release\IB410100.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IB410100.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB410100.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB410100.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Account.sbr" \
	"$(INTDIR)\Cfg.sbr" \
	"$(INTDIR)\CrossLine.sbr" \
	"$(INTDIR)\cxcfg.sbr" \
	"$(INTDIR)\IB410100.sbr" \
	"$(INTDIR)\DlgSave.sbr" \
	"$(INTDIR)\dxopsenscal.sbr" \
	"$(INTDIR)\excel9.sbr" \
	"$(INTDIR)\GridWnd.sbr" \
	"$(INTDIR)\GrpWnd.sbr" \
	"$(INTDIR)\HedgePage.sbr" \
	"$(INTDIR)\libmap.sbr" \
	"$(INTDIR)\MapWnd.sbr" \
	"$(INTDIR)\MGrpWnd.sbr" \
	"$(INTDIR)\MyStgPage.sbr" \
	"$(INTDIR)\Notify.sbr" \
	"$(INTDIR)\OpSensCal.sbr" \
	"$(INTDIR)\PopupWnd.sbr" \
	"$(INTDIR)\Price.sbr" \
	"$(INTDIR)\SendOrder.sbr" \
	"$(INTDIR)\Sheet1.sbr" \
	"$(INTDIR)\SonikPage.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\StgBoard.sbr" \
	"$(INTDIR)\StgInfo.sbr" \
	"$(INTDIR)\StgPage.sbr" \
	"$(INTDIR)\ToolTip.sbr"

"$(OUTDIR)\IB410100.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=../../control/fx_grid/release/fx_grid.lib ../../control/fx_misc/release/fx_misc.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\IB410100.pdb" /machine:I386 /def:".\IB410100.def" /out:"../../../release/IB410100.dll" /implib:"$(OUTDIR)\IB410100.lib" 
DEF_FILE= \
	".\IB410100.def"
LINK32_OBJS= \
	"$(INTDIR)\Account.obj" \
	"$(INTDIR)\Cfg.obj" \
	"$(INTDIR)\CrossLine.obj" \
	"$(INTDIR)\cxcfg.obj" \
	"$(INTDIR)\IB410100.obj" \
	"$(INTDIR)\DlgSave.obj" \
	"$(INTDIR)\dxopsenscal.obj" \
	"$(INTDIR)\excel9.obj" \
	"$(INTDIR)\GridWnd.obj" \
	"$(INTDIR)\GrpWnd.obj" \
	"$(INTDIR)\HedgePage.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\MGrpWnd.obj" \
	"$(INTDIR)\MyStgPage.obj" \
	"$(INTDIR)\Notify.obj" \
	"$(INTDIR)\OpSensCal.obj" \
	"$(INTDIR)\PopupWnd.obj" \
	"$(INTDIR)\Price.obj" \
	"$(INTDIR)\SendOrder.obj" \
	"$(INTDIR)\Sheet1.obj" \
	"$(INTDIR)\SonikPage.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StgBoard.obj" \
	"$(INTDIR)\StgInfo.obj" \
	"$(INTDIR)\StgPage.obj" \
	"$(INTDIR)\ToolTip.obj" \
	"$(INTDIR)\IB410100.res"

"..\..\..\release\IB410100.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IB410100 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\dev\IB410100.dll" "$(OUTDIR)\IB410100.bsc"


CLEAN :
	-@erase "$(INTDIR)\Account.obj"
	-@erase "$(INTDIR)\Account.sbr"
	-@erase "$(INTDIR)\Cfg.obj"
	-@erase "$(INTDIR)\Cfg.sbr"
	-@erase "$(INTDIR)\CrossLine.obj"
	-@erase "$(INTDIR)\CrossLine.sbr"
	-@erase "$(INTDIR)\cxcfg.obj"
	-@erase "$(INTDIR)\cxcfg.sbr"
	-@erase "$(INTDIR)\IB410100.obj"
	-@erase "$(INTDIR)\IB410100.pch"
	-@erase "$(INTDIR)\IB410100.res"
	-@erase "$(INTDIR)\IB410100.sbr"
	-@erase "$(INTDIR)\DlgSave.obj"
	-@erase "$(INTDIR)\DlgSave.sbr"
	-@erase "$(INTDIR)\dxopsenscal.obj"
	-@erase "$(INTDIR)\dxopsenscal.sbr"
	-@erase "$(INTDIR)\excel9.obj"
	-@erase "$(INTDIR)\excel9.sbr"
	-@erase "$(INTDIR)\GridWnd.obj"
	-@erase "$(INTDIR)\GridWnd.sbr"
	-@erase "$(INTDIR)\GrpWnd.obj"
	-@erase "$(INTDIR)\GrpWnd.sbr"
	-@erase "$(INTDIR)\HedgePage.obj"
	-@erase "$(INTDIR)\HedgePage.sbr"
	-@erase "$(INTDIR)\libmap.obj"
	-@erase "$(INTDIR)\libmap.sbr"
	-@erase "$(INTDIR)\MapWnd.obj"
	-@erase "$(INTDIR)\MapWnd.sbr"
	-@erase "$(INTDIR)\MGrpWnd.obj"
	-@erase "$(INTDIR)\MGrpWnd.sbr"
	-@erase "$(INTDIR)\MyStgPage.obj"
	-@erase "$(INTDIR)\MyStgPage.sbr"
	-@erase "$(INTDIR)\Notify.obj"
	-@erase "$(INTDIR)\Notify.sbr"
	-@erase "$(INTDIR)\OpSensCal.obj"
	-@erase "$(INTDIR)\OpSensCal.sbr"
	-@erase "$(INTDIR)\PopupWnd.obj"
	-@erase "$(INTDIR)\PopupWnd.sbr"
	-@erase "$(INTDIR)\Price.obj"
	-@erase "$(INTDIR)\Price.sbr"
	-@erase "$(INTDIR)\SendOrder.obj"
	-@erase "$(INTDIR)\SendOrder.sbr"
	-@erase "$(INTDIR)\Sheet1.obj"
	-@erase "$(INTDIR)\Sheet1.sbr"
	-@erase "$(INTDIR)\SonikPage.obj"
	-@erase "$(INTDIR)\SonikPage.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\StgBoard.obj"
	-@erase "$(INTDIR)\StgBoard.sbr"
	-@erase "$(INTDIR)\StgInfo.obj"
	-@erase "$(INTDIR)\StgInfo.sbr"
	-@erase "$(INTDIR)\StgPage.obj"
	-@erase "$(INTDIR)\StgPage.sbr"
	-@erase "$(INTDIR)\ToolTip.obj"
	-@erase "$(INTDIR)\ToolTip.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\IB410100.bsc"
	-@erase "$(OUTDIR)\IB410100.exp"
	-@erase "$(OUTDIR)\IB410100.lib"
	-@erase "$(OUTDIR)\IB410100.pdb"
	-@erase "..\..\..\dev\IB410100.dll"
	-@erase "..\..\..\dev\IB410100.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IB410100.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB410100.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB410100.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Account.sbr" \
	"$(INTDIR)\Cfg.sbr" \
	"$(INTDIR)\CrossLine.sbr" \
	"$(INTDIR)\cxcfg.sbr" \
	"$(INTDIR)\IB410100.sbr" \
	"$(INTDIR)\DlgSave.sbr" \
	"$(INTDIR)\dxopsenscal.sbr" \
	"$(INTDIR)\excel9.sbr" \
	"$(INTDIR)\GridWnd.sbr" \
	"$(INTDIR)\GrpWnd.sbr" \
	"$(INTDIR)\HedgePage.sbr" \
	"$(INTDIR)\libmap.sbr" \
	"$(INTDIR)\MapWnd.sbr" \
	"$(INTDIR)\MGrpWnd.sbr" \
	"$(INTDIR)\MyStgPage.sbr" \
	"$(INTDIR)\Notify.sbr" \
	"$(INTDIR)\OpSensCal.sbr" \
	"$(INTDIR)\PopupWnd.sbr" \
	"$(INTDIR)\Price.sbr" \
	"$(INTDIR)\SendOrder.sbr" \
	"$(INTDIR)\Sheet1.sbr" \
	"$(INTDIR)\SonikPage.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\StgBoard.sbr" \
	"$(INTDIR)\StgInfo.sbr" \
	"$(INTDIR)\StgPage.sbr" \
	"$(INTDIR)\ToolTip.sbr"

"$(OUTDIR)\IB410100.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=../../control/fx_grid/debug/fx_grid.lib ../../control/fx_misc/debug/fx_misc.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\IB410100.pdb" /debug /machine:I386 /def:".\IB410100.def" /out:"../../../dev/IB410100.dll" /implib:"$(OUTDIR)\IB410100.lib" /pdbtype:sept 
DEF_FILE= \
	".\IB410100.def"
LINK32_OBJS= \
	"$(INTDIR)\Account.obj" \
	"$(INTDIR)\Cfg.obj" \
	"$(INTDIR)\CrossLine.obj" \
	"$(INTDIR)\cxcfg.obj" \
	"$(INTDIR)\IB410100.obj" \
	"$(INTDIR)\DlgSave.obj" \
	"$(INTDIR)\dxopsenscal.obj" \
	"$(INTDIR)\excel9.obj" \
	"$(INTDIR)\GridWnd.obj" \
	"$(INTDIR)\GrpWnd.obj" \
	"$(INTDIR)\HedgePage.obj" \
	"$(INTDIR)\libmap.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\MGrpWnd.obj" \
	"$(INTDIR)\MyStgPage.obj" \
	"$(INTDIR)\Notify.obj" \
	"$(INTDIR)\OpSensCal.obj" \
	"$(INTDIR)\PopupWnd.obj" \
	"$(INTDIR)\Price.obj" \
	"$(INTDIR)\SendOrder.obj" \
	"$(INTDIR)\Sheet1.obj" \
	"$(INTDIR)\SonikPage.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StgBoard.obj" \
	"$(INTDIR)\StgInfo.obj" \
	"$(INTDIR)\StgPage.obj" \
	"$(INTDIR)\ToolTip.obj" \
	"$(INTDIR)\IB410100.res"

"..\..\..\dev\IB410100.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("IB410100.dep")
!INCLUDE "IB410100.dep"
!ELSE 
!MESSAGE Warning: cannot find "IB410100.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IB410100 - Win32 Release" || "$(CFG)" == "IB410100 - Win32 Debug"
SOURCE=.\Account.cpp

"$(INTDIR)\Account.obj"	"$(INTDIR)\Account.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\Cfg.cpp

"$(INTDIR)\Cfg.obj"	"$(INTDIR)\Cfg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\CrossLine.cpp

"$(INTDIR)\CrossLine.obj"	"$(INTDIR)\CrossLine.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\cxcfg.cpp

"$(INTDIR)\cxcfg.obj"	"$(INTDIR)\cxcfg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\IB410100.cpp

"$(INTDIR)\IB410100.obj"	"$(INTDIR)\IB410100.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\IB410100.rc

"$(INTDIR)\IB410100.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\DlgSave.cpp

"$(INTDIR)\DlgSave.obj"	"$(INTDIR)\DlgSave.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\dxopsenscal.cpp

"$(INTDIR)\dxopsenscal.obj"	"$(INTDIR)\dxopsenscal.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\excel9.cpp

"$(INTDIR)\excel9.obj"	"$(INTDIR)\excel9.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\GridWnd.cpp

"$(INTDIR)\GridWnd.obj"	"$(INTDIR)\GridWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\GrpWnd.cpp

"$(INTDIR)\GrpWnd.obj"	"$(INTDIR)\GrpWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\HedgePage.cpp

"$(INTDIR)\HedgePage.obj"	"$(INTDIR)\HedgePage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\libmap.cpp

"$(INTDIR)\libmap.obj"	"$(INTDIR)\libmap.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\MapWnd.cpp

"$(INTDIR)\MapWnd.obj"	"$(INTDIR)\MapWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\MGrpWnd.cpp

"$(INTDIR)\MGrpWnd.obj"	"$(INTDIR)\MGrpWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\MyStgPage.cpp

"$(INTDIR)\MyStgPage.obj"	"$(INTDIR)\MyStgPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\Notify.cpp

"$(INTDIR)\Notify.obj"	"$(INTDIR)\Notify.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\OpSensCal.cpp

"$(INTDIR)\OpSensCal.obj"	"$(INTDIR)\OpSensCal.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\PopupWnd.cpp

"$(INTDIR)\PopupWnd.obj"	"$(INTDIR)\PopupWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\Price.cpp

"$(INTDIR)\Price.obj"	"$(INTDIR)\Price.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\SendOrder.cpp

"$(INTDIR)\SendOrder.obj"	"$(INTDIR)\SendOrder.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\Sheet1.cpp

"$(INTDIR)\Sheet1.obj"	"$(INTDIR)\Sheet1.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\SonikPage.cpp

"$(INTDIR)\SonikPage.obj"	"$(INTDIR)\SonikPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "IB410100 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IB410100.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\IB410100.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "IB410100 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IB410100.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\IB410100.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StgBoard.cpp

"$(INTDIR)\StgBoard.obj"	"$(INTDIR)\StgBoard.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\StgInfo.cpp

"$(INTDIR)\StgInfo.obj"	"$(INTDIR)\StgInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\StgPage.cpp

"$(INTDIR)\StgPage.obj"	"$(INTDIR)\StgPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"


SOURCE=.\ToolTip.cpp

"$(INTDIR)\ToolTip.obj"	"$(INTDIR)\ToolTip.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB410100.pch"



!ENDIF 


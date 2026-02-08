# Microsoft Developer Studio Generated NMAKE File, Based on IB885200.dsp
!IF "$(CFG)" == ""
CFG=IB885200 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to IB885200 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IB885200 - Win32 Release" && "$(CFG)" != "IB885200 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB885200.mak" CFG="IB885200 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB885200 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB885200 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "IB885200 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\Release\IB885200.DLL"


CLEAN :
	-@erase "$(INTDIR)\3DEdit.obj"
	-@erase "$(INTDIR)\AccountCtrl.obj"
	-@erase "$(INTDIR)\AxScrollBar.obj"
	-@erase "$(INTDIR)\CodeReg.obj"
	-@erase "$(INTDIR)\ConfigDlg.obj"
	-@erase "$(INTDIR)\ContainerWnd.obj"
	-@erase "$(INTDIR)\ControlWnd.obj"
	-@erase "$(INTDIR)\cx_notify.obj"
	-@erase "$(INTDIR)\IB885200.obj"
	-@erase "$(INTDIR)\IB885200.pch"
	-@erase "$(INTDIR)\IB885200.res"
	-@erase "$(INTDIR)\DialogConfirm.obj"
	-@erase "$(INTDIR)\HogaWnd.obj"
	-@erase "$(INTDIR)\MapWnd.obj"
	-@erase "$(INTDIR)\MultiJumunDlg.obj"
	-@erase "$(INTDIR)\RectWnd.obj"
	-@erase "$(INTDIR)\SelJumunDlg.obj"
	-@erase "$(INTDIR)\SendOrder.obj"
	-@erase "$(INTDIR)\SpinCtrl.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TextWnd.obj"
	-@erase "$(INTDIR)\Tool.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Warning.obj"
	-@erase "$(OUTDIR)\IB885200.exp"
	-@erase "$(OUTDIR)\IB885200.lib"
	-@erase "$(OUTDIR)\IB885200.map"
	-@erase "$(OUTDIR)\IB885200.pdb"
	-@erase "..\..\..\Release\IB885200.DLL"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB885200.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB885200.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB885200.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../../control/fx_misc/Release/fx_misc.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\IB885200.pdb" /map:"$(INTDIR)\IB885200.map" /debug /machine:I386 /def:".\IB885200.def" /out:"../../../Release/IB885200.DLL" /implib:"$(OUTDIR)\IB885200.lib" 
DEF_FILE= \
	".\IB885200.def"
LINK32_OBJS= \
	"$(INTDIR)\3DEdit.obj" \
	"$(INTDIR)\AccountCtrl.obj" \
	"$(INTDIR)\AxScrollBar.obj" \
	"$(INTDIR)\CodeReg.obj" \
	"$(INTDIR)\ConfigDlg.obj" \
	"$(INTDIR)\ContainerWnd.obj" \
	"$(INTDIR)\ControlWnd.obj" \
	"$(INTDIR)\cx_notify.obj" \
	"$(INTDIR)\IB885200.obj" \
	"$(INTDIR)\DialogConfirm.obj" \
	"$(INTDIR)\HogaWnd.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\MultiJumunDlg.obj" \
	"$(INTDIR)\RectWnd.obj" \
	"$(INTDIR)\SelJumunDlg.obj" \
	"$(INTDIR)\SendOrder.obj" \
	"$(INTDIR)\SpinCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TextWnd.obj" \
	"$(INTDIR)\Tool.obj" \
	"$(INTDIR)\Warning.obj" \
	"$(INTDIR)\IB885200.res"

"..\..\..\Release\IB885200.DLL" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\dev\IB885200.DLL" "$(OUTDIR)\IB885200.bsc"


CLEAN :
	-@erase "$(INTDIR)\3DEdit.obj"
	-@erase "$(INTDIR)\3DEdit.sbr"
	-@erase "$(INTDIR)\AccountCtrl.obj"
	-@erase "$(INTDIR)\AccountCtrl.sbr"
	-@erase "$(INTDIR)\AxScrollBar.obj"
	-@erase "$(INTDIR)\AxScrollBar.sbr"
	-@erase "$(INTDIR)\CodeReg.obj"
	-@erase "$(INTDIR)\CodeReg.sbr"
	-@erase "$(INTDIR)\ConfigDlg.obj"
	-@erase "$(INTDIR)\ConfigDlg.sbr"
	-@erase "$(INTDIR)\ContainerWnd.obj"
	-@erase "$(INTDIR)\ContainerWnd.sbr"
	-@erase "$(INTDIR)\ControlWnd.obj"
	-@erase "$(INTDIR)\ControlWnd.sbr"
	-@erase "$(INTDIR)\cx_notify.obj"
	-@erase "$(INTDIR)\cx_notify.sbr"
	-@erase "$(INTDIR)\IB885200.obj"
	-@erase "$(INTDIR)\IB885200.pch"
	-@erase "$(INTDIR)\IB885200.res"
	-@erase "$(INTDIR)\IB885200.sbr"
	-@erase "$(INTDIR)\DialogConfirm.obj"
	-@erase "$(INTDIR)\DialogConfirm.sbr"
	-@erase "$(INTDIR)\HogaWnd.obj"
	-@erase "$(INTDIR)\HogaWnd.sbr"
	-@erase "$(INTDIR)\MapWnd.obj"
	-@erase "$(INTDIR)\MapWnd.sbr"
	-@erase "$(INTDIR)\MultiJumunDlg.obj"
	-@erase "$(INTDIR)\MultiJumunDlg.sbr"
	-@erase "$(INTDIR)\RectWnd.obj"
	-@erase "$(INTDIR)\RectWnd.sbr"
	-@erase "$(INTDIR)\SelJumunDlg.obj"
	-@erase "$(INTDIR)\SelJumunDlg.sbr"
	-@erase "$(INTDIR)\SendOrder.obj"
	-@erase "$(INTDIR)\SendOrder.sbr"
	-@erase "$(INTDIR)\SpinCtrl.obj"
	-@erase "$(INTDIR)\SpinCtrl.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TextWnd.obj"
	-@erase "$(INTDIR)\TextWnd.sbr"
	-@erase "$(INTDIR)\Tool.obj"
	-@erase "$(INTDIR)\Tool.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Warning.obj"
	-@erase "$(INTDIR)\Warning.sbr"
	-@erase "$(OUTDIR)\IB885200.bsc"
	-@erase "$(OUTDIR)\IB885200.exp"
	-@erase "$(OUTDIR)\IB885200.lib"
	-@erase "$(OUTDIR)\IB885200.pdb"
	-@erase "..\..\..\dev\IB885200.DLL"
	-@erase "..\..\..\dev\IB885200.ILK"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IB885200.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB885200.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB885200.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\3DEdit.sbr" \
	"$(INTDIR)\AccountCtrl.sbr" \
	"$(INTDIR)\AxScrollBar.sbr" \
	"$(INTDIR)\CodeReg.sbr" \
	"$(INTDIR)\ConfigDlg.sbr" \
	"$(INTDIR)\ContainerWnd.sbr" \
	"$(INTDIR)\ControlWnd.sbr" \
	"$(INTDIR)\cx_notify.sbr" \
	"$(INTDIR)\IB885200.sbr" \
	"$(INTDIR)\DialogConfirm.sbr" \
	"$(INTDIR)\HogaWnd.sbr" \
	"$(INTDIR)\MapWnd.sbr" \
	"$(INTDIR)\MultiJumunDlg.sbr" \
	"$(INTDIR)\RectWnd.sbr" \
	"$(INTDIR)\SelJumunDlg.sbr" \
	"$(INTDIR)\SendOrder.sbr" \
	"$(INTDIR)\SpinCtrl.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TextWnd.sbr" \
	"$(INTDIR)\Tool.sbr" \
	"$(INTDIR)\Warning.sbr"

"$(OUTDIR)\IB885200.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=../../control/fx_misc/Debug/fx_misc.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\IB885200.pdb" /debug /machine:I386 /def:".\IB885200.def" /out:"../../../dev/IB885200.DLL" /implib:"$(OUTDIR)\IB885200.lib" /pdbtype:sept 
DEF_FILE= \
	".\IB885200.def"
LINK32_OBJS= \
	"$(INTDIR)\3DEdit.obj" \
	"$(INTDIR)\AccountCtrl.obj" \
	"$(INTDIR)\AxScrollBar.obj" \
	"$(INTDIR)\CodeReg.obj" \
	"$(INTDIR)\ConfigDlg.obj" \
	"$(INTDIR)\ContainerWnd.obj" \
	"$(INTDIR)\ControlWnd.obj" \
	"$(INTDIR)\cx_notify.obj" \
	"$(INTDIR)\IB885200.obj" \
	"$(INTDIR)\DialogConfirm.obj" \
	"$(INTDIR)\HogaWnd.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\MultiJumunDlg.obj" \
	"$(INTDIR)\RectWnd.obj" \
	"$(INTDIR)\SelJumunDlg.obj" \
	"$(INTDIR)\SendOrder.obj" \
	"$(INTDIR)\SpinCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TextWnd.obj" \
	"$(INTDIR)\Tool.obj" \
	"$(INTDIR)\Warning.obj" \
	"$(INTDIR)\IB885200.res"

"..\..\..\dev\IB885200.DLL" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("IB885200.dep")
!INCLUDE "IB885200.dep"
!ELSE 
!MESSAGE Warning: cannot find "IB885200.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IB885200 - Win32 Release" || "$(CFG)" == "IB885200 - Win32 Debug"
SOURCE=.\3DEdit.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\3DEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\3DEdit.obj"	"$(INTDIR)\3DEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\AccountCtrl.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\AccountCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\AccountCtrl.obj"	"$(INTDIR)\AccountCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\AxScrollBar.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\AxScrollBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\AxScrollBar.obj"	"$(INTDIR)\AxScrollBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\CodeReg.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\CodeReg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\CodeReg.obj"	"$(INTDIR)\CodeReg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\ConfigDlg.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\ConfigDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\ConfigDlg.obj"	"$(INTDIR)\ConfigDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\ContainerWnd.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\ContainerWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\ContainerWnd.obj"	"$(INTDIR)\ContainerWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\ControlWnd.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\ControlWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\ControlWnd.obj"	"$(INTDIR)\ControlWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\cx_notify.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\cx_notify.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\cx_notify.obj"	"$(INTDIR)\cx_notify.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\IB885200.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\IB885200.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\IB885200.obj"	"$(INTDIR)\IB885200.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\IB885200.rc

"$(INTDIR)\IB885200.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\DialogConfirm.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\DialogConfirm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\DialogConfirm.obj"	"$(INTDIR)\DialogConfirm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\HogaWnd.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\HogaWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\HogaWnd.obj"	"$(INTDIR)\HogaWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\MapWnd.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\MapWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\MapWnd.obj"	"$(INTDIR)\MapWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\MultiJumunDlg.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\MultiJumunDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\MultiJumunDlg.obj"	"$(INTDIR)\MultiJumunDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\RectWnd.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\RectWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\RectWnd.obj"	"$(INTDIR)\RectWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\SelJumunDlg.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\SelJumunDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\SelJumunDlg.obj"	"$(INTDIR)\SelJumunDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\SendOrder.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\SendOrder.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\SendOrder.obj"	"$(INTDIR)\SendOrder.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\SpinCtrl.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\SpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\SpinCtrl.obj"	"$(INTDIR)\SpinCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB885200.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IB885200.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IB885200.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\IB885200.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TextWnd.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\TextWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\TextWnd.obj"	"$(INTDIR)\TextWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\Tool.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\Tool.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\Tool.obj"	"$(INTDIR)\Tool.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 

SOURCE=.\Warning.cpp

!IF  "$(CFG)" == "IB885200 - Win32 Release"


"$(INTDIR)\Warning.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ELSEIF  "$(CFG)" == "IB885200 - Win32 Debug"


"$(INTDIR)\Warning.obj"	"$(INTDIR)\Warning.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB885200.pch"


!ENDIF 


!ENDIF 


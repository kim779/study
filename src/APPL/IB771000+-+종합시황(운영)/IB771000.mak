# Microsoft Developer Studio Generated NMAKE File, Based on IB771000.dsp
!IF "$(CFG)" == ""
CFG=IB771000 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to IB771000 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IB771000 - Win32 Release" && "$(CFG)" != "IB771000 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IB771000.mak" CFG="IB771000 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IB771000 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IB771000 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "IB771000 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\Release\IB771000.dll"


CLEAN :
	-@erase "$(INTDIR)\AxBrower.obj"
	-@erase "$(INTDIR)\BkUtil.obj"
	-@erase "$(INTDIR)\DetailSetDlg.obj"
	-@erase "$(INTDIR)\IB771000.obj"
	-@erase "$(INTDIR)\IB771000.pch"
	-@erase "$(INTDIR)\IB771000.res"
	-@erase "$(INTDIR)\DropBtn.obj"
	-@erase "$(INTDIR)\MapDlg.obj"
	-@erase "$(INTDIR)\MapWnd.obj"
	-@erase "$(INTDIR)\SButton.obj"
	-@erase "$(INTDIR)\SComboBox1.obj"
	-@erase "$(INTDIR)\SearchWordSetDlg.obj"
	-@erase "$(INTDIR)\SEdit.obj"
	-@erase "$(INTDIR)\SHeaderCtrl.obj"
	-@erase "$(INTDIR)\SListCtrl.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\STimeCtrl.obj"
	-@erase "$(INTDIR)\Util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XPButton.obj"
	-@erase "$(INTDIR)\xSkinButton.obj"
	-@erase "$(OUTDIR)\IB771000.exp"
	-@erase "$(OUTDIR)\IB771000.lib"
	-@erase "$(OUTDIR)\IB771000.map"
	-@erase "$(OUTDIR)\IB771000.pdb"
	-@erase "..\..\..\Release\IB771000.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB771000.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB771000.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB771000.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../../control/fx_misc/release/fx_misc.lib /nologo /base:"0x63000000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\IB771000.pdb" /map:"$(INTDIR)\IB771000.map" /debug /machine:I386 /def:".\IB771000.def" /out:"../../../Release/IB771000.dll" /implib:"$(OUTDIR)\IB771000.lib" 
DEF_FILE= \
	".\IB771000.def"
LINK32_OBJS= \
	"$(INTDIR)\BkUtil.obj" \
	"$(INTDIR)\IB771000.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\STimeCtrl.obj" \
	"$(INTDIR)\Util.obj" \
	"$(INTDIR)\AxBrower.obj" \
	"$(INTDIR)\DropBtn.obj" \
	"$(INTDIR)\SButton.obj" \
	"$(INTDIR)\SComboBox1.obj" \
	"$(INTDIR)\SEdit.obj" \
	"$(INTDIR)\SHeaderCtrl.obj" \
	"$(INTDIR)\SListCtrl.obj" \
	"$(INTDIR)\XPButton.obj" \
	"$(INTDIR)\xSkinButton.obj" \
	"$(INTDIR)\DetailSetDlg.obj" \
	"$(INTDIR)\MapDlg.obj" \
	"$(INTDIR)\SearchWordSetDlg.obj" \
	"$(INTDIR)\IB771000.res"

"..\..\..\Release\IB771000.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\Dev\IB771000.dll" "$(OUTDIR)\IB771000.bsc"


CLEAN :
	-@erase "$(INTDIR)\AxBrower.obj"
	-@erase "$(INTDIR)\AxBrower.sbr"
	-@erase "$(INTDIR)\BkUtil.obj"
	-@erase "$(INTDIR)\BkUtil.sbr"
	-@erase "$(INTDIR)\DetailSetDlg.obj"
	-@erase "$(INTDIR)\DetailSetDlg.sbr"
	-@erase "$(INTDIR)\IB771000.obj"
	-@erase "$(INTDIR)\IB771000.pch"
	-@erase "$(INTDIR)\IB771000.res"
	-@erase "$(INTDIR)\IB771000.sbr"
	-@erase "$(INTDIR)\DropBtn.obj"
	-@erase "$(INTDIR)\DropBtn.sbr"
	-@erase "$(INTDIR)\MapDlg.obj"
	-@erase "$(INTDIR)\MapDlg.sbr"
	-@erase "$(INTDIR)\MapWnd.obj"
	-@erase "$(INTDIR)\MapWnd.sbr"
	-@erase "$(INTDIR)\SButton.obj"
	-@erase "$(INTDIR)\SButton.sbr"
	-@erase "$(INTDIR)\SComboBox1.obj"
	-@erase "$(INTDIR)\SComboBox1.sbr"
	-@erase "$(INTDIR)\SearchWordSetDlg.obj"
	-@erase "$(INTDIR)\SearchWordSetDlg.sbr"
	-@erase "$(INTDIR)\SEdit.obj"
	-@erase "$(INTDIR)\SEdit.sbr"
	-@erase "$(INTDIR)\SHeaderCtrl.obj"
	-@erase "$(INTDIR)\SHeaderCtrl.sbr"
	-@erase "$(INTDIR)\SListCtrl.obj"
	-@erase "$(INTDIR)\SListCtrl.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\STimeCtrl.obj"
	-@erase "$(INTDIR)\STimeCtrl.sbr"
	-@erase "$(INTDIR)\Util.obj"
	-@erase "$(INTDIR)\Util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XPButton.obj"
	-@erase "$(INTDIR)\XPButton.sbr"
	-@erase "$(INTDIR)\xSkinButton.obj"
	-@erase "$(INTDIR)\xSkinButton.sbr"
	-@erase "$(OUTDIR)\IB771000.bsc"
	-@erase "$(OUTDIR)\IB771000.exp"
	-@erase "$(OUTDIR)\IB771000.lib"
	-@erase "$(OUTDIR)\IB771000.pdb"
	-@erase "..\..\..\Dev\IB771000.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IB771000.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IB771000.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IB771000.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BkUtil.sbr" \
	"$(INTDIR)\IB771000.sbr" \
	"$(INTDIR)\MapWnd.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\STimeCtrl.sbr" \
	"$(INTDIR)\Util.sbr" \
	"$(INTDIR)\AxBrower.sbr" \
	"$(INTDIR)\DropBtn.sbr" \
	"$(INTDIR)\SButton.sbr" \
	"$(INTDIR)\SComboBox1.sbr" \
	"$(INTDIR)\SEdit.sbr" \
	"$(INTDIR)\SHeaderCtrl.sbr" \
	"$(INTDIR)\SListCtrl.sbr" \
	"$(INTDIR)\XPButton.sbr" \
	"$(INTDIR)\xSkinButton.sbr" \
	"$(INTDIR)\DetailSetDlg.sbr" \
	"$(INTDIR)\MapDlg.sbr" \
	"$(INTDIR)\SearchWordSetDlg.sbr"

"$(OUTDIR)\IB771000.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=../../control/fx_misc/debug/fx_misc.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\IB771000.pdb" /debug /machine:I386 /def:".\IB771000.def" /out:"../../../Dev/IB771000.dll" /implib:"$(OUTDIR)\IB771000.lib" /pdbtype:sept 
DEF_FILE= \
	".\IB771000.def"
LINK32_OBJS= \
	"$(INTDIR)\BkUtil.obj" \
	"$(INTDIR)\IB771000.obj" \
	"$(INTDIR)\MapWnd.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\STimeCtrl.obj" \
	"$(INTDIR)\Util.obj" \
	"$(INTDIR)\AxBrower.obj" \
	"$(INTDIR)\DropBtn.obj" \
	"$(INTDIR)\SButton.obj" \
	"$(INTDIR)\SComboBox1.obj" \
	"$(INTDIR)\SEdit.obj" \
	"$(INTDIR)\SHeaderCtrl.obj" \
	"$(INTDIR)\SListCtrl.obj" \
	"$(INTDIR)\XPButton.obj" \
	"$(INTDIR)\xSkinButton.obj" \
	"$(INTDIR)\DetailSetDlg.obj" \
	"$(INTDIR)\MapDlg.obj" \
	"$(INTDIR)\SearchWordSetDlg.obj" \
	"$(INTDIR)\IB771000.res"

"..\..\..\Dev\IB771000.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("IB771000.dep")
!INCLUDE "IB771000.dep"
!ELSE 
!MESSAGE Warning: cannot find "IB771000.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IB771000 - Win32 Release" || "$(CFG)" == "IB771000 - Win32 Debug"
SOURCE=.\BkUtil.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\BkUtil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\BkUtil.obj"	"$(INTDIR)\BkUtil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\IB771000.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\IB771000.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\IB771000.obj"	"$(INTDIR)\IB771000.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\IB771000.rc

"$(INTDIR)\IB771000.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\MapWnd.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\MapWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\MapWnd.obj"	"$(INTDIR)\MapWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\IB771000.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IB771000.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\IB771000.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\IB771000.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\STimeCtrl.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\STimeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\STimeCtrl.obj"	"$(INTDIR)\STimeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\Util.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\Util.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\Util.obj"	"$(INTDIR)\Util.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\AxBrower.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\AxBrower.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\AxBrower.obj"	"$(INTDIR)\AxBrower.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\DropBtn.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\DropBtn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\DropBtn.obj"	"$(INTDIR)\DropBtn.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\SButton.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\SButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\SButton.obj"	"$(INTDIR)\SButton.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\SComboBox1.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\SComboBox1.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\SComboBox1.obj"	"$(INTDIR)\SComboBox1.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\SEdit.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\SEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\SEdit.obj"	"$(INTDIR)\SEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\SHeaderCtrl.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\SHeaderCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\SHeaderCtrl.obj"	"$(INTDIR)\SHeaderCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\SListCtrl.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\SListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\SListCtrl.obj"	"$(INTDIR)\SListCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\XPButton.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\XPButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\XPButton.obj"	"$(INTDIR)\XPButton.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\xSkinButton.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\xSkinButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\xSkinButton.obj"	"$(INTDIR)\xSkinButton.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\DetailSetDlg.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\DetailSetDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\DetailSetDlg.obj"	"$(INTDIR)\DetailSetDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\MapDlg.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\MapDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\MapDlg.obj"	"$(INTDIR)\MapDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 

SOURCE=.\SearchWordSetDlg.cpp

!IF  "$(CFG)" == "IB771000 - Win32 Release"


"$(INTDIR)\SearchWordSetDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ELSEIF  "$(CFG)" == "IB771000 - Win32 Debug"


"$(INTDIR)\SearchWordSetDlg.obj"	"$(INTDIR)\SearchWordSetDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IB771000.pch"


!ENDIF 


!ENDIF 


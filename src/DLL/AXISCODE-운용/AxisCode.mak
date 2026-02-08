# Microsoft Developer Studio Generated NMAKE File, Based on AxisCode.dsp
!IF "$(CFG)" == ""
CFG=AxisCode - Win32 Debug
!MESSAGE No configuration specified. Defaulting to AxisCode - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "AxisCode - Win32 Release" && "$(CFG)" != "AxisCode - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AxisCode.mak" CFG="AxisCode - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AxisCode - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AxisCode - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "AxisCode - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "..\..\..\release\axiscode.dll" "$(OUTDIR)\AxisCode.bsc"


CLEAN :
	-@erase "$(INTDIR)\AxisCode.obj"
	-@erase "$(INTDIR)\AxisCode.pch"
	-@erase "$(INTDIR)\AxisCode.res"
	-@erase "$(INTDIR)\AxisCode.sbr"
	-@erase "$(INTDIR)\BondDlg.obj"
	-@erase "$(INTDIR)\BondDlg.sbr"
	-@erase "$(INTDIR)\BtnST.obj"
	-@erase "$(INTDIR)\BtnST.sbr"
	-@erase "$(INTDIR)\CheckBox.obj"
	-@erase "$(INTDIR)\CheckBox.sbr"
	-@erase "$(INTDIR)\CustomCheckBox.obj"
	-@erase "$(INTDIR)\CustomCheckBox.sbr"
	-@erase "$(INTDIR)\CustomFuturesDlg.obj"
	-@erase "$(INTDIR)\CustomFuturesDlg.sbr"
	-@erase "$(INTDIR)\ElwDlg.obj"
	-@erase "$(INTDIR)\ElwDlg.sbr"
	-@erase "$(INTDIR)\FuturesDlg.obj"
	-@erase "$(INTDIR)\FuturesDlg.sbr"
	-@erase "$(INTDIR)\JongmukDlg.obj"
	-@erase "$(INTDIR)\JongmukDlg.sbr"
	-@erase "$(INTDIR)\libdll.obj"
	-@erase "$(INTDIR)\libdll.sbr"
	-@erase "$(INTDIR)\MyBitmap.obj"
	-@erase "$(INTDIR)\MyBitmap.sbr"
	-@erase "$(INTDIR)\MyEdit.obj"
	-@erase "$(INTDIR)\MyEdit.sbr"
	-@erase "$(INTDIR)\MyFont.obj"
	-@erase "$(INTDIR)\MyFont.sbr"
	-@erase "$(INTDIR)\SFont.obj"
	-@erase "$(INTDIR)\SFont.sbr"
	-@erase "$(INTDIR)\SkinHeaderCtrl.obj"
	-@erase "$(INTDIR)\SkinHeaderCtrl.sbr"
	-@erase "$(INTDIR)\SkinListCtrl.obj"
	-@erase "$(INTDIR)\SkinListCtrl.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\StockNewDlg.obj"
	-@erase "$(INTDIR)\StockNewDlg.sbr"
	-@erase "$(INTDIR)\Tool.obj"
	-@erase "$(INTDIR)\Tool.sbr"
	-@erase "$(INTDIR)\UPCodeDlg.obj"
	-@erase "$(INTDIR)\UPCodeDlg.sbr"
	-@erase "$(INTDIR)\USCodeDlg.obj"
	-@erase "$(INTDIR)\USCodeDlg.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\AxisCode.bsc"
	-@erase "$(OUTDIR)\axiscode.exp"
	-@erase "$(OUTDIR)\axiscode.lib"
	-@erase "..\..\..\release\axiscode.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\AxisCode.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\AxisCode.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AxisCode.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AxisCode.sbr" \
	"$(INTDIR)\BondDlg.sbr" \
	"$(INTDIR)\BtnST.sbr" \
	"$(INTDIR)\CheckBox.sbr" \
	"$(INTDIR)\CustomCheckBox.sbr" \
	"$(INTDIR)\CustomFuturesDlg.sbr" \
	"$(INTDIR)\ElwDlg.sbr" \
	"$(INTDIR)\FuturesDlg.sbr" \
	"$(INTDIR)\JongmukDlg.sbr" \
	"$(INTDIR)\libdll.sbr" \
	"$(INTDIR)\MyBitmap.sbr" \
	"$(INTDIR)\MyEdit.sbr" \
	"$(INTDIR)\MyFont.sbr" \
	"$(INTDIR)\SFont.sbr" \
	"$(INTDIR)\SkinHeaderCtrl.sbr" \
	"$(INTDIR)\SkinListCtrl.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\StockNewDlg.sbr" \
	"$(INTDIR)\Tool.sbr" \
	"$(INTDIR)\UPCodeDlg.sbr" \
	"$(INTDIR)\USCodeDlg.sbr"

"$(OUTDIR)\AxisCode.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=imm32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\axiscode.pdb" /machine:I386 /def:".\AxisCode.def" /out:"../../../release/axiscode.dll" /implib:"$(OUTDIR)\axiscode.lib" 
DEF_FILE= \
	".\AxisCode.def"
LINK32_OBJS= \
	"$(INTDIR)\AxisCode.obj" \
	"$(INTDIR)\BondDlg.obj" \
	"$(INTDIR)\BtnST.obj" \
	"$(INTDIR)\CheckBox.obj" \
	"$(INTDIR)\CustomCheckBox.obj" \
	"$(INTDIR)\CustomFuturesDlg.obj" \
	"$(INTDIR)\ElwDlg.obj" \
	"$(INTDIR)\FuturesDlg.obj" \
	"$(INTDIR)\JongmukDlg.obj" \
	"$(INTDIR)\libdll.obj" \
	"$(INTDIR)\MyBitmap.obj" \
	"$(INTDIR)\MyEdit.obj" \
	"$(INTDIR)\MyFont.obj" \
	"$(INTDIR)\SFont.obj" \
	"$(INTDIR)\SkinHeaderCtrl.obj" \
	"$(INTDIR)\SkinListCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StockNewDlg.obj" \
	"$(INTDIR)\Tool.obj" \
	"$(INTDIR)\UPCodeDlg.obj" \
	"$(INTDIR)\USCodeDlg.obj" \
	"$(INTDIR)\AxisCode.res"

"..\..\..\release\axiscode.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "AxisCode - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\exe\axiscode.dll" "$(OUTDIR)\AxisCode.bsc"


CLEAN :
	-@erase "$(INTDIR)\AxisCode.obj"
	-@erase "$(INTDIR)\AxisCode.pch"
	-@erase "$(INTDIR)\AxisCode.res"
	-@erase "$(INTDIR)\AxisCode.sbr"
	-@erase "$(INTDIR)\BondDlg.obj"
	-@erase "$(INTDIR)\BondDlg.sbr"
	-@erase "$(INTDIR)\BtnST.obj"
	-@erase "$(INTDIR)\BtnST.sbr"
	-@erase "$(INTDIR)\CheckBox.obj"
	-@erase "$(INTDIR)\CheckBox.sbr"
	-@erase "$(INTDIR)\CustomCheckBox.obj"
	-@erase "$(INTDIR)\CustomCheckBox.sbr"
	-@erase "$(INTDIR)\CustomFuturesDlg.obj"
	-@erase "$(INTDIR)\CustomFuturesDlg.sbr"
	-@erase "$(INTDIR)\ElwDlg.obj"
	-@erase "$(INTDIR)\ElwDlg.sbr"
	-@erase "$(INTDIR)\FuturesDlg.obj"
	-@erase "$(INTDIR)\FuturesDlg.sbr"
	-@erase "$(INTDIR)\JongmukDlg.obj"
	-@erase "$(INTDIR)\JongmukDlg.sbr"
	-@erase "$(INTDIR)\libdll.obj"
	-@erase "$(INTDIR)\libdll.sbr"
	-@erase "$(INTDIR)\MyBitmap.obj"
	-@erase "$(INTDIR)\MyBitmap.sbr"
	-@erase "$(INTDIR)\MyEdit.obj"
	-@erase "$(INTDIR)\MyEdit.sbr"
	-@erase "$(INTDIR)\MyFont.obj"
	-@erase "$(INTDIR)\MyFont.sbr"
	-@erase "$(INTDIR)\SFont.obj"
	-@erase "$(INTDIR)\SFont.sbr"
	-@erase "$(INTDIR)\SkinHeaderCtrl.obj"
	-@erase "$(INTDIR)\SkinHeaderCtrl.sbr"
	-@erase "$(INTDIR)\SkinListCtrl.obj"
	-@erase "$(INTDIR)\SkinListCtrl.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\StockNewDlg.obj"
	-@erase "$(INTDIR)\StockNewDlg.sbr"
	-@erase "$(INTDIR)\Tool.obj"
	-@erase "$(INTDIR)\Tool.sbr"
	-@erase "$(INTDIR)\UPCodeDlg.obj"
	-@erase "$(INTDIR)\UPCodeDlg.sbr"
	-@erase "$(INTDIR)\USCodeDlg.obj"
	-@erase "$(INTDIR)\USCodeDlg.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\AxisCode.bsc"
	-@erase "$(OUTDIR)\axiscode.exp"
	-@erase "$(OUTDIR)\axiscode.lib"
	-@erase "$(OUTDIR)\axiscode.pdb"
	-@erase "..\..\..\exe\axiscode.dll"
	-@erase "..\..\..\exe\axiscode.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\AxisCode.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\AxisCode.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AxisCode.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AxisCode.sbr" \
	"$(INTDIR)\BondDlg.sbr" \
	"$(INTDIR)\BtnST.sbr" \
	"$(INTDIR)\CheckBox.sbr" \
	"$(INTDIR)\CustomCheckBox.sbr" \
	"$(INTDIR)\CustomFuturesDlg.sbr" \
	"$(INTDIR)\ElwDlg.sbr" \
	"$(INTDIR)\FuturesDlg.sbr" \
	"$(INTDIR)\JongmukDlg.sbr" \
	"$(INTDIR)\libdll.sbr" \
	"$(INTDIR)\MyBitmap.sbr" \
	"$(INTDIR)\MyEdit.sbr" \
	"$(INTDIR)\MyFont.sbr" \
	"$(INTDIR)\SFont.sbr" \
	"$(INTDIR)\SkinHeaderCtrl.sbr" \
	"$(INTDIR)\SkinListCtrl.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\StockNewDlg.sbr" \
	"$(INTDIR)\Tool.sbr" \
	"$(INTDIR)\UPCodeDlg.sbr" \
	"$(INTDIR)\USCodeDlg.sbr"

"$(OUTDIR)\AxisCode.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=imm32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\axiscode.pdb" /debug /machine:I386 /def:".\AxisCode.def" /out:"../../../exe/axiscode.dll" /implib:"$(OUTDIR)\axiscode.lib" /pdbtype:sept 
DEF_FILE= \
	".\AxisCode.def"
LINK32_OBJS= \
	"$(INTDIR)\AxisCode.obj" \
	"$(INTDIR)\BondDlg.obj" \
	"$(INTDIR)\BtnST.obj" \
	"$(INTDIR)\CheckBox.obj" \
	"$(INTDIR)\CustomCheckBox.obj" \
	"$(INTDIR)\CustomFuturesDlg.obj" \
	"$(INTDIR)\ElwDlg.obj" \
	"$(INTDIR)\FuturesDlg.obj" \
	"$(INTDIR)\JongmukDlg.obj" \
	"$(INTDIR)\libdll.obj" \
	"$(INTDIR)\MyBitmap.obj" \
	"$(INTDIR)\MyEdit.obj" \
	"$(INTDIR)\MyFont.obj" \
	"$(INTDIR)\SFont.obj" \
	"$(INTDIR)\SkinHeaderCtrl.obj" \
	"$(INTDIR)\SkinListCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StockNewDlg.obj" \
	"$(INTDIR)\Tool.obj" \
	"$(INTDIR)\UPCodeDlg.obj" \
	"$(INTDIR)\USCodeDlg.obj" \
	"$(INTDIR)\AxisCode.res"

"..\..\..\exe\axiscode.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("AxisCode.dep")
!INCLUDE "AxisCode.dep"
!ELSE 
!MESSAGE Warning: cannot find "AxisCode.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "AxisCode - Win32 Release" || "$(CFG)" == "AxisCode - Win32 Debug"
SOURCE=.\AxisCode.cpp

"$(INTDIR)\AxisCode.obj"	"$(INTDIR)\AxisCode.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\AxisCode.rc

"$(INTDIR)\AxisCode.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\BondDlg.cpp

"$(INTDIR)\BondDlg.obj"	"$(INTDIR)\BondDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\BtnST.cpp

"$(INTDIR)\BtnST.obj"	"$(INTDIR)\BtnST.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\CheckBox.cpp

"$(INTDIR)\CheckBox.obj"	"$(INTDIR)\CheckBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\CustomCheckBox.cpp

"$(INTDIR)\CustomCheckBox.obj"	"$(INTDIR)\CustomCheckBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\CustomFuturesDlg.cpp

"$(INTDIR)\CustomFuturesDlg.obj"	"$(INTDIR)\CustomFuturesDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\ElwDlg.cpp

"$(INTDIR)\ElwDlg.obj"	"$(INTDIR)\ElwDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\FuturesDlg.cpp

"$(INTDIR)\FuturesDlg.obj"	"$(INTDIR)\FuturesDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\JongmukDlg.cpp

"$(INTDIR)\JongmukDlg.obj"	"$(INTDIR)\JongmukDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\libdll.cpp

"$(INTDIR)\libdll.obj"	"$(INTDIR)\libdll.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\MyBitmap.cpp

"$(INTDIR)\MyBitmap.obj"	"$(INTDIR)\MyBitmap.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\MyEdit.cpp

"$(INTDIR)\MyEdit.obj"	"$(INTDIR)\MyEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\MyFont.cpp

"$(INTDIR)\MyFont.obj"	"$(INTDIR)\MyFont.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\SFont.cpp

"$(INTDIR)\SFont.obj"	"$(INTDIR)\SFont.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\SkinHeaderCtrl.cpp

"$(INTDIR)\SkinHeaderCtrl.obj"	"$(INTDIR)\SkinHeaderCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\SkinListCtrl.cpp

"$(INTDIR)\SkinListCtrl.obj"	"$(INTDIR)\SkinListCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "AxisCode - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\AxisCode.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\AxisCode.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "AxisCode - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\AxisCode.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\AxisCode.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StockNewDlg.cpp

"$(INTDIR)\StockNewDlg.obj"	"$(INTDIR)\StockNewDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\Tool.cpp

"$(INTDIR)\Tool.obj"	"$(INTDIR)\Tool.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\UPCodeDlg.cpp

"$(INTDIR)\UPCodeDlg.obj"	"$(INTDIR)\UPCodeDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"


SOURCE=.\USCodeDlg.cpp

"$(INTDIR)\USCodeDlg.obj"	"$(INTDIR)\USCodeDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AxisCode.pch"



!ENDIF 


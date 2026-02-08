# Microsoft Developer Studio Generated NMAKE File, Based on axis.dsp
!IF "$(CFG)" == ""
CFG=axis - Win32 Debug
!MESSAGE No configuration specified. Defaulting to axis - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "axis - Win32 Release" && "$(CFG)" != "axis - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axis.mak" CFG="axis - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axis - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "axis - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "axis - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\release\axis.exe"


CLEAN :
	-@erase "$(INTDIR)\AccSave.obj"
	-@erase "$(INTDIR)\AccTool.obj"
	-@erase "$(INTDIR)\Alarm.obj"
	-@erase "$(INTDIR)\axInstance.obj"
	-@erase "$(INTDIR)\axis.obj"
	-@erase "$(INTDIR)\axis.pch"
	-@erase "$(INTDIR)\axis.res"
	-@erase "$(INTDIR)\axisDoc.obj"
	-@erase "$(INTDIR)\axisView.obj"
	-@erase "$(INTDIR)\axMisc.obj"
	-@erase "$(INTDIR)\axScrollView.obj"
	-@erase "$(INTDIR)\AxTabView.obj"
	-@erase "$(INTDIR)\BaseMP.obj"
	-@erase "$(INTDIR)\BtnST.obj"
	-@erase "$(INTDIR)\button.obj"
	-@erase "$(INTDIR)\Chaser.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Conclusion.obj"
	-@erase "$(INTDIR)\CpuUse.obj"
	-@erase "$(INTDIR)\custsite.obj"
	-@erase "$(INTDIR)\dlgMisuAlarm.obj"
	-@erase "$(INTDIR)\Dtconnect.obj"
	-@erase "$(INTDIR)\dtInfo.obj"
	-@erase "$(INTDIR)\EConclusion.obj"
	-@erase "$(INTDIR)\EmergencyDlg.obj"
	-@erase "$(INTDIR)\ExceptionHandler.obj"
	-@erase "$(INTDIR)\ExitD.obj"
	-@erase "$(INTDIR)\fname.obj"
	-@erase "$(INTDIR)\GPop.obj"
	-@erase "$(INTDIR)\HelpOK.obj"
	-@erase "$(INTDIR)\idispimp.obj"
	-@erase "$(INTDIR)\Infofile.obj"
	-@erase "$(INTDIR)\Linkbar.obj"
	-@erase "$(INTDIR)\LockDlg.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\ManageInfo.obj"
	-@erase "$(INTDIR)\MPop.obj"
	-@erase "$(INTDIR)\NClock.obj"
	-@erase "$(INTDIR)\NDib.obj"
	-@erase "$(INTDIR)\NewsViewer.obj"
	-@erase "$(INTDIR)\NExpand.obj"
	-@erase "$(INTDIR)\NGrid.obj"
	-@erase "$(INTDIR)\NPalette.obj"
	-@erase "$(INTDIR)\NProgressCtrl.obj"
	-@erase "$(INTDIR)\NSocket.obj"
	-@erase "$(INTDIR)\NTab.obj"
	-@erase "$(INTDIR)\NTable.obj"
	-@erase "$(INTDIR)\passdlg.obj"
	-@erase "$(INTDIR)\PassEdit.obj"
	-@erase "$(INTDIR)\PrinterSettings.obj"
	-@erase "$(INTDIR)\ProgramDlg.obj"
	-@erase "$(INTDIR)\ProgressDlg.obj"
	-@erase "$(INTDIR)\RSCinfo.obj"
	-@erase "$(INTDIR)\SChild.obj"
	-@erase "$(INTDIR)\screenDlg.obj"
	-@erase "$(INTDIR)\Setup.obj"
	-@erase "$(INTDIR)\SetupC.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Stoploss.obj"
	-@erase "$(INTDIR)\StopWarn.obj"
	-@erase "$(INTDIR)\sysInfo.obj"
	-@erase "$(INTDIR)\TestInfoDlg.obj"
	-@erase "$(INTDIR)\TTip.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XCaption.obj"
	-@erase "$(INTDIR)\XHeaderCtrl.obj"
	-@erase "$(INTDIR)\XListCtrl.obj"
	-@erase "$(INTDIR)\XUnzip.obj"
	-@erase "$(INTDIR)\XZip.obj"
	-@erase "$(OUTDIR)\axis.map"
	-@erase "$(OUTDIR)\axis.pdb"
	-@erase "..\..\release\axis.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /Od /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "WIN32" /Fp"$(INTDIR)\axis.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\axis.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\axis.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../dll/sm/Release/axissm.lib Imm32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\axis.pdb" /map:"$(INTDIR)\axis.map" /debug /machine:I386 /out:"../../release/axis.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AccSave.obj" \
	"$(INTDIR)\AccTool.obj" \
	"$(INTDIR)\Alarm.obj" \
	"$(INTDIR)\axInstance.obj" \
	"$(INTDIR)\axis.obj" \
	"$(INTDIR)\axisDoc.obj" \
	"$(INTDIR)\axisView.obj" \
	"$(INTDIR)\axMisc.obj" \
	"$(INTDIR)\axScrollView.obj" \
	"$(INTDIR)\AxTabView.obj" \
	"$(INTDIR)\BaseMP.obj" \
	"$(INTDIR)\BtnST.obj" \
	"$(INTDIR)\button.obj" \
	"$(INTDIR)\Chaser.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Conclusion.obj" \
	"$(INTDIR)\CpuUse.obj" \
	"$(INTDIR)\custsite.obj" \
	"$(INTDIR)\dlgMisuAlarm.obj" \
	"$(INTDIR)\Dtconnect.obj" \
	"$(INTDIR)\dtInfo.obj" \
	"$(INTDIR)\EConclusion.obj" \
	"$(INTDIR)\EmergencyDlg.obj" \
	"$(INTDIR)\ExceptionHandler.obj" \
	"$(INTDIR)\ExitD.obj" \
	"$(INTDIR)\fname.obj" \
	"$(INTDIR)\GPop.obj" \
	"$(INTDIR)\HelpOK.obj" \
	"$(INTDIR)\idispimp.obj" \
	"$(INTDIR)\Infofile.obj" \
	"$(INTDIR)\Linkbar.obj" \
	"$(INTDIR)\LockDlg.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\ManageInfo.obj" \
	"$(INTDIR)\MPop.obj" \
	"$(INTDIR)\NClock.obj" \
	"$(INTDIR)\NDib.obj" \
	"$(INTDIR)\NewsViewer.obj" \
	"$(INTDIR)\NExpand.obj" \
	"$(INTDIR)\NGrid.obj" \
	"$(INTDIR)\NPalette.obj" \
	"$(INTDIR)\NProgressCtrl.obj" \
	"$(INTDIR)\NSocket.obj" \
	"$(INTDIR)\NTab.obj" \
	"$(INTDIR)\NTable.obj" \
	"$(INTDIR)\passdlg.obj" \
	"$(INTDIR)\PassEdit.obj" \
	"$(INTDIR)\PrinterSettings.obj" \
	"$(INTDIR)\ProgramDlg.obj" \
	"$(INTDIR)\ProgressDlg.obj" \
	"$(INTDIR)\RSCinfo.obj" \
	"$(INTDIR)\SChild.obj" \
	"$(INTDIR)\screenDlg.obj" \
	"$(INTDIR)\Setup.obj" \
	"$(INTDIR)\SetupC.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Stoploss.obj" \
	"$(INTDIR)\StopWarn.obj" \
	"$(INTDIR)\sysInfo.obj" \
	"$(INTDIR)\TestInfoDlg.obj" \
	"$(INTDIR)\TTip.obj" \
	"$(INTDIR)\XCaption.obj" \
	"$(INTDIR)\XUnzip.obj" \
	"$(INTDIR)\XZip.obj" \
	"$(INTDIR)\XHeaderCtrl.obj" \
	"$(INTDIR)\XListCtrl.obj" \
	"$(INTDIR)\axis.res"

"..\..\release\axis.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "axis - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\Debug\axis.exe"


CLEAN :
	-@erase "$(INTDIR)\AccSave.obj"
	-@erase "$(INTDIR)\AccTool.obj"
	-@erase "$(INTDIR)\Alarm.obj"
	-@erase "$(INTDIR)\axInstance.obj"
	-@erase "$(INTDIR)\axis.obj"
	-@erase "$(INTDIR)\axis.pch"
	-@erase "$(INTDIR)\axis.res"
	-@erase "$(INTDIR)\axisDoc.obj"
	-@erase "$(INTDIR)\axisView.obj"
	-@erase "$(INTDIR)\axMisc.obj"
	-@erase "$(INTDIR)\axScrollView.obj"
	-@erase "$(INTDIR)\AxTabView.obj"
	-@erase "$(INTDIR)\BaseMP.obj"
	-@erase "$(INTDIR)\BtnST.obj"
	-@erase "$(INTDIR)\button.obj"
	-@erase "$(INTDIR)\Chaser.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Conclusion.obj"
	-@erase "$(INTDIR)\CpuUse.obj"
	-@erase "$(INTDIR)\custsite.obj"
	-@erase "$(INTDIR)\dlgMisuAlarm.obj"
	-@erase "$(INTDIR)\Dtconnect.obj"
	-@erase "$(INTDIR)\dtInfo.obj"
	-@erase "$(INTDIR)\EConclusion.obj"
	-@erase "$(INTDIR)\EmergencyDlg.obj"
	-@erase "$(INTDIR)\ExceptionHandler.obj"
	-@erase "$(INTDIR)\ExitD.obj"
	-@erase "$(INTDIR)\fname.obj"
	-@erase "$(INTDIR)\GPop.obj"
	-@erase "$(INTDIR)\HelpOK.obj"
	-@erase "$(INTDIR)\idispimp.obj"
	-@erase "$(INTDIR)\Infofile.obj"
	-@erase "$(INTDIR)\Linkbar.obj"
	-@erase "$(INTDIR)\LockDlg.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\ManageInfo.obj"
	-@erase "$(INTDIR)\MPop.obj"
	-@erase "$(INTDIR)\NClock.obj"
	-@erase "$(INTDIR)\NDib.obj"
	-@erase "$(INTDIR)\NewsViewer.obj"
	-@erase "$(INTDIR)\NExpand.obj"
	-@erase "$(INTDIR)\NGrid.obj"
	-@erase "$(INTDIR)\NPalette.obj"
	-@erase "$(INTDIR)\NProgressCtrl.obj"
	-@erase "$(INTDIR)\NSocket.obj"
	-@erase "$(INTDIR)\NTab.obj"
	-@erase "$(INTDIR)\NTable.obj"
	-@erase "$(INTDIR)\passdlg.obj"
	-@erase "$(INTDIR)\PassEdit.obj"
	-@erase "$(INTDIR)\PrinterSettings.obj"
	-@erase "$(INTDIR)\ProgramDlg.obj"
	-@erase "$(INTDIR)\ProgressDlg.obj"
	-@erase "$(INTDIR)\RSCinfo.obj"
	-@erase "$(INTDIR)\SChild.obj"
	-@erase "$(INTDIR)\screenDlg.obj"
	-@erase "$(INTDIR)\Setup.obj"
	-@erase "$(INTDIR)\SetupC.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Stoploss.obj"
	-@erase "$(INTDIR)\StopWarn.obj"
	-@erase "$(INTDIR)\sysInfo.obj"
	-@erase "$(INTDIR)\TestInfoDlg.obj"
	-@erase "$(INTDIR)\TTip.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XCaption.obj"
	-@erase "$(INTDIR)\XHeaderCtrl.obj"
	-@erase "$(INTDIR)\XListCtrl.obj"
	-@erase "$(INTDIR)\XUnzip.obj"
	-@erase "$(INTDIR)\XZip.obj"
	-@erase "$(OUTDIR)\axis.pdb"
	-@erase "..\..\Debug\axis.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\axis.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /D /Gz"WIN32" /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\axis.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\axis.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../dll/sm/Debug/axissm.lib Imm32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\axis.pdb" /debug /machine:I386 /out:"../../Debug/axis.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AccSave.obj" \
	"$(INTDIR)\AccTool.obj" \
	"$(INTDIR)\Alarm.obj" \
	"$(INTDIR)\axInstance.obj" \
	"$(INTDIR)\axis.obj" \
	"$(INTDIR)\axisDoc.obj" \
	"$(INTDIR)\axisView.obj" \
	"$(INTDIR)\axMisc.obj" \
	"$(INTDIR)\axScrollView.obj" \
	"$(INTDIR)\AxTabView.obj" \
	"$(INTDIR)\BaseMP.obj" \
	"$(INTDIR)\BtnST.obj" \
	"$(INTDIR)\button.obj" \
	"$(INTDIR)\Chaser.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Conclusion.obj" \
	"$(INTDIR)\CpuUse.obj" \
	"$(INTDIR)\custsite.obj" \
	"$(INTDIR)\dlgMisuAlarm.obj" \
	"$(INTDIR)\Dtconnect.obj" \
	"$(INTDIR)\dtInfo.obj" \
	"$(INTDIR)\EConclusion.obj" \
	"$(INTDIR)\EmergencyDlg.obj" \
	"$(INTDIR)\ExceptionHandler.obj" \
	"$(INTDIR)\ExitD.obj" \
	"$(INTDIR)\fname.obj" \
	"$(INTDIR)\GPop.obj" \
	"$(INTDIR)\HelpOK.obj" \
	"$(INTDIR)\idispimp.obj" \
	"$(INTDIR)\Infofile.obj" \
	"$(INTDIR)\Linkbar.obj" \
	"$(INTDIR)\LockDlg.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\ManageInfo.obj" \
	"$(INTDIR)\MPop.obj" \
	"$(INTDIR)\NClock.obj" \
	"$(INTDIR)\NDib.obj" \
	"$(INTDIR)\NewsViewer.obj" \
	"$(INTDIR)\NExpand.obj" \
	"$(INTDIR)\NGrid.obj" \
	"$(INTDIR)\NPalette.obj" \
	"$(INTDIR)\NProgressCtrl.obj" \
	"$(INTDIR)\NSocket.obj" \
	"$(INTDIR)\NTab.obj" \
	"$(INTDIR)\NTable.obj" \
	"$(INTDIR)\passdlg.obj" \
	"$(INTDIR)\PassEdit.obj" \
	"$(INTDIR)\PrinterSettings.obj" \
	"$(INTDIR)\ProgramDlg.obj" \
	"$(INTDIR)\ProgressDlg.obj" \
	"$(INTDIR)\RSCinfo.obj" \
	"$(INTDIR)\SChild.obj" \
	"$(INTDIR)\screenDlg.obj" \
	"$(INTDIR)\Setup.obj" \
	"$(INTDIR)\SetupC.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Stoploss.obj" \
	"$(INTDIR)\StopWarn.obj" \
	"$(INTDIR)\sysInfo.obj" \
	"$(INTDIR)\TestInfoDlg.obj" \
	"$(INTDIR)\TTip.obj" \
	"$(INTDIR)\XCaption.obj" \
	"$(INTDIR)\XUnzip.obj" \
	"$(INTDIR)\XZip.obj" \
	"$(INTDIR)\XHeaderCtrl.obj" \
	"$(INTDIR)\XListCtrl.obj" \
	"$(INTDIR)\axis.res"

"..\..\Debug\axis.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("axis.dep")
!INCLUDE "axis.dep"
!ELSE 
!MESSAGE Warning: cannot find "axis.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "axis - Win32 Release" || "$(CFG)" == "axis - Win32 Debug"
SOURCE=.\AccSave.cpp

"$(INTDIR)\AccSave.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\AccTool.cpp

"$(INTDIR)\AccTool.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\Alarm.cpp

"$(INTDIR)\Alarm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\axInstance.cpp

"$(INTDIR)\axInstance.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\axis.cpp

"$(INTDIR)\axis.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\axis.rc

"$(INTDIR)\axis.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\axisDoc.cpp

"$(INTDIR)\axisDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\axisView.cpp

"$(INTDIR)\axisView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\axMisc.cpp

"$(INTDIR)\axMisc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\axScrollView.cpp

"$(INTDIR)\axScrollView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\AxTabView.cpp

"$(INTDIR)\AxTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\BaseMP.cpp

"$(INTDIR)\BaseMP.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\BtnST.cpp

"$(INTDIR)\BtnST.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\button.cpp

"$(INTDIR)\button.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\Chaser.cpp

"$(INTDIR)\Chaser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\ChildFrm.cpp

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\Conclusion.cpp

"$(INTDIR)\Conclusion.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\CpuUse.cpp

"$(INTDIR)\CpuUse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\custsite.cpp

"$(INTDIR)\custsite.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\dlgMisuAlarm.cpp

"$(INTDIR)\dlgMisuAlarm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\Dtconnect.cpp

"$(INTDIR)\Dtconnect.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\dtInfo.cpp

"$(INTDIR)\dtInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\EConclusion.cpp

"$(INTDIR)\EConclusion.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\EmergencyDlg.cpp

"$(INTDIR)\EmergencyDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\UnhandledException\ExceptionHandler.cpp

"$(INTDIR)\ExceptionHandler.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ExitD.cpp

"$(INTDIR)\ExitD.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\fname.cpp

"$(INTDIR)\fname.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\GPop.cpp

"$(INTDIR)\GPop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\HelpOK.cpp

"$(INTDIR)\HelpOK.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\idispimp.cpp

"$(INTDIR)\idispimp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\Infofile.cpp

"$(INTDIR)\Infofile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\Linkbar.cpp

"$(INTDIR)\Linkbar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\LockDlg.cpp

"$(INTDIR)\LockDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\ManageInfo.cpp

"$(INTDIR)\ManageInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\MPop.cpp

"$(INTDIR)\MPop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\NClock.cpp

"$(INTDIR)\NClock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\NDib.cpp

"$(INTDIR)\NDib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\NewsViewer.cpp

"$(INTDIR)\NewsViewer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\NExpand.cpp

"$(INTDIR)\NExpand.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\NGrid.cpp

"$(INTDIR)\NGrid.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\NPalette.cpp

"$(INTDIR)\NPalette.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\NProgressCtrl.cpp

"$(INTDIR)\NProgressCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\NSocket.cpp

"$(INTDIR)\NSocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\NTab.cpp

"$(INTDIR)\NTab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\NTable.cpp

"$(INTDIR)\NTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\passdlg.cpp

"$(INTDIR)\passdlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\PassEdit.cpp

"$(INTDIR)\PassEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\PrinterSettings.cpp

"$(INTDIR)\PrinterSettings.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\ProgramDlg.cpp

"$(INTDIR)\ProgramDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\ProgressDlg.cpp

"$(INTDIR)\ProgressDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\RSCinfo.cpp

"$(INTDIR)\RSCinfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\SChild.cpp

"$(INTDIR)\SChild.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\screenDlg.cpp

"$(INTDIR)\screenDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\Setup.cpp

"$(INTDIR)\Setup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\SetupC.cpp

"$(INTDIR)\SetupC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "axis - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /Od /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "WIN32" /Fp"$(INTDIR)\axis.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\axis.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "axis - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\axis.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /D /Gz"WIN32" /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\axis.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Stoploss.cpp

"$(INTDIR)\Stoploss.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\StopWarn.cpp

"$(INTDIR)\StopWarn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\sysInfo.cpp

"$(INTDIR)\sysInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\TestInfoDlg.cpp

"$(INTDIR)\TestInfoDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\TTip.cpp

"$(INTDIR)\TTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\XCaption.cpp

"$(INTDIR)\XCaption.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"


SOURCE=.\XUnzip.cpp

!IF  "$(CFG)" == "axis - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /Od /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\XUnzip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "axis - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /D /Gz"WIN32" /GZ /c 

"$(INTDIR)\XUnzip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\XZip.cpp

!IF  "$(CFG)" == "axis - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /Od /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\XZip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "axis - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /D /Gz"WIN32" /GZ /c 

"$(INTDIR)\XZip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\dll\ctrl\XHeaderCtrl.cpp

"$(INTDIR)\XHeaderCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\dll\ctrl\XListCtrl.cpp

"$(INTDIR)\XListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\axis.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 


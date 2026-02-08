; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgSavePattern
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "patternsearch.h"
LastPage=0

ClassCount=9
Class1=CDrawPattenCtrl
Class2=CExEditCtrl
Class3=ShowGrid
Class4=CILButton
Class5=CListImageWnd
Class6=CDlgSavePattern
Class7=CFieldEditDlg
Class8=CScreenWnd
Class9=CMainFrmWnd

ResourceCount=5
Resource1=IDD_PATTERNVIEW_DLG
Resource2=IDD_FIELDEDIT_DLG
Resource3=IDD_DLG_SAVEPATTERN
Resource4=IDD_STD_DIALOG
Resource5=IDD_DLG_CHART

[CLS:CDrawPattenCtrl]
Type=0
BaseClass=CWnd
HeaderFile=AddControls\DrawPattenCtrl.h
ImplementationFile=AddControls\DrawPattenCtrl.cpp
Filter=W
VirtualFilter=WC
LastObject=CDrawPattenCtrl

[CLS:CExEditCtrl]
Type=0
BaseClass=CEdit
HeaderFile=AddControls\ExEditCtrl.h
ImplementationFile=AddControls\ExEditCtrl.cpp

[CLS:ShowGrid]
Type=0
HeaderFile=AddControls\Grid_Comm.h
ImplementationFile=AddControls\Grid_Comm.cpp
LastObject=ShowGrid

[CLS:CILButton]
Type=0
BaseClass=CButton
HeaderFile=AddControls\ILButton.h
ImplementationFile=AddControls\ILButton.cpp
LastObject=CILButton

[CLS:CListImageWnd]
Type=0
BaseClass=CWnd
HeaderFile=AddControls\ListImageWnd.h
ImplementationFile=AddControls\ListImageWnd.cpp

[CLS:CDlgSavePattern]
Type=0
BaseClass=CDialog
HeaderFile=DlgSavePattern.h
ImplementationFile=DlgSavePattern.cpp
LastObject=CDlgSavePattern
Filter=D
VirtualFilter=dWC

[CLS:CFieldEditDlg]
Type=0
BaseClass=CDialog
HeaderFile=FieldEditDlg.h
ImplementationFile=FieldEditDlg.cpp
LastObject=CFieldEditDlg

[CLS:CScreenWnd]
Type=0
BaseClass=CWnd
HeaderFile=ScreenWnd.h
ImplementationFile=FnSetScreenWnd.cpp
LastObject=CScreenWnd

[CLS:CMainFrmWnd]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrmWnd.h
ImplementationFile=MainFrmWnd.cpp
LastObject=CMainFrmWnd

[DLG:IDD_DLG_SAVEPATTERN]
Type=1
Class=CDlgSavePattern
ControlCount=4
Control1=IDC_ED_PATTERNNAME,edit,1350631552
Control2=IDOK,button,1073807361
Control3=IDCANCEL,button,1073807360
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_FIELDEDIT_DLG]
Type=1
Class=CFieldEditDlg
ControlCount=0

[DLG:IDD_STD_DIALOG]
Type=1
Class=?
ControlCount=0

[DLG:IDD_PATTERNVIEW_DLG]
Type=1
Class=?
ControlCount=0

[DLG:IDD_DLG_CHART]
Type=1
Class=?
ControlCount=0


; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=DlgHelp
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "conditionsearch.h"
LastPage=0

ClassCount=14
Class1=CConditionListView
Class2=CConditionMakeWnd
Class3=CConditionResultWnd
Class4=CConditionView
Class5=CFieldEditDlg
Class6=CFolderEditDlg
Class7=CMainFrmWnd
Class8=CMatchValEditDlg
Class9=CPortPolioDelDlg
Class10=CQuickMakeDlg
Class11=CSaveDlg
Class12=CScreenWnd

ResourceCount=17
Resource1="IDD_FILEDOWN_DLG"
Resource2=IDD_FOLDEREDIT_DLG
Resource3=IDD_PORTDEL_DLG
Resource4="IDD_FINDER_MAIN_DLG"
Resource5=IDD_DIALOGPW
Resource6=IDD_EDITSUBJECT
Resource7=IDD_STD_DIALOG
Resource8=IDD_DLGREQUEST
Resource9=IDD_FIELDEDIT_DLG
Resource10=IDD_SEARCHRNG
Resource11=IDD_SAVE_DLG
Resource12=IDD_QUICKMAKE_DLG
Resource13=IDD_TARGETEDIT
Class13=CConditionSearch
Resource14=IDD_MATCHVALEDIT_DLG
Resource15=IDD_LOGICHELP
Resource16=IDD_NO_ITEM
Class14=DlgHelp
Resource17=IDD_HELP

[CLS:CConditionListView]
Type=0
BaseClass=CView
HeaderFile=ConditionListView.h
ImplementationFile=ConditionListView.cpp
LastObject=CConditionListView

[CLS:CConditionMakeWnd]
Type=0
BaseClass=CWnd
HeaderFile=ConditionMakeWnd.h
ImplementationFile=conditionmakewnd.cpp

[CLS:CConditionResultWnd]
Type=0
BaseClass=CWnd
HeaderFile=ConditionResultWnd.h
ImplementationFile=conditionresultwnd.cpp
Filter=W
LastObject=CConditionResultWnd

[CLS:CConditionView]
Type=0
BaseClass=CView
HeaderFile=ConditionView.h
ImplementationFile=ConditionView.cpp

[CLS:CFieldEditDlg]
Type=0
BaseClass=CDialog
HeaderFile=FieldEditDlg.h
ImplementationFile=FieldEditDlg.cpp

[CLS:CFolderEditDlg]
Type=0
BaseClass=CDialog
HeaderFile=FolderEditDlg.h
ImplementationFile=FolderEditDlg.cpp

[CLS:CMainFrmWnd]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrmWnd.h
ImplementationFile=MainFrmWnd.cpp
Filter=T
VirtualFilter=fWC
LastObject=CMainFrmWnd

[CLS:CMatchValEditDlg]
Type=0
BaseClass=CDialog
HeaderFile=MatchValEditDlg.h
ImplementationFile=MatchValEditDlg.cpp

[CLS:CPortPolioDelDlg]
Type=0
BaseClass=CDialog
HeaderFile=PortPolioDelDlg.h
ImplementationFile=PortPolioDelDlg.cpp

[CLS:CQuickMakeDlg]
Type=0
BaseClass=CDialog
HeaderFile=QuickMakeDlg.h
ImplementationFile=QuickMakeDlg.cpp

[CLS:CSaveDlg]
Type=0
BaseClass=CDialog
HeaderFile=SaveDlg.h
ImplementationFile=SaveDlg.cpp
Filter=D
LastObject=CSaveDlg

[CLS:CScreenWnd]
Type=0
BaseClass=CWnd
HeaderFile=ScreenWnd.h
ImplementationFile=ScreenWnd.cpp

[DLG:IDD_FIELDEDIT_DLG]
Type=1
Class=CFieldEditDlg
ControlCount=0

[DLG:IDD_FOLDEREDIT_DLG]
Type=1
Class=CFolderEditDlg
ControlCount=0

[DLG:IDD_MATCHVALEDIT_DLG]
Type=1
Class=CMatchValEditDlg
ControlCount=0

[DLG:IDD_PORTDEL_DLG]
Type=1
Class=CPortPolioDelDlg
ControlCount=0

[DLG:IDD_QUICKMAKE_DLG]
Type=1
Class=CQuickMakeDlg
ControlCount=0

[DLG:IDD_SAVE_DLG]
Type=1
Class=CSaveDlg
ControlCount=0

[DLG:IDD_STD_DIALOG]
Type=1
Class=?
ControlCount=0

[DLG:"IDD_FINDER_MAIN_DLG"]
Type=1
Class=?
ControlCount=0

[DLG:"IDD_FILEDOWN_DLG"]
Type=1
Class=?
ControlCount=0

[DLG:IDD_LOGICHELP]
Type=1
Class=?
ControlCount=1
Control1=IDOK,button,1073807361

[DLG:IDD_SEARCHRNG]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1073807360
Control2=IDCANCEL,button,1073807360

[DLG:IDD_EDITSUBJECT]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360

[DLG:IDD_NO_ITEM]
Type=1
Class=?
ControlCount=0

[CLS:CConditionSearch]
Type=0
HeaderFile=ConditionSearch.h
ImplementationFile=ConditionSearch.cpp
BaseClass=generic CWnd
Filter=W
LastObject=CConditionSearch

[DLG:IDD_TARGETEDIT]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_DIALOGPW]
Type=1
Class=?
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC_ACCT,static,1342308352
Control5=IDC_STATIC_PSWD,static,1342308352
Control6=IDC_STATIC_ACCT2,static,1342308352
Control7=1177,edit,1350631584

[DLG:IDD_DLGREQUEST]
Type=1
Class=?
ControlCount=1
Control1=IDC_STMESSAGE,static,1342308353

[DLG:IDD_HELP]
Type=1
Class=DlgHelp
ControlCount=2
Control1=IDOK,button,1073807361
Control2=IDC_EDIT1,edit,1350567940

[CLS:DlgHelp]
Type=0
HeaderFile=DlgHelp.h
ImplementationFile=DlgHelp.cpp
BaseClass=CDialog
Filter=D
LastObject=DlgHelp
VirtualFilter=dWC


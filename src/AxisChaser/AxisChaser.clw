; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChildView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "AxisChaser.h"
LastPage=0

ClassCount=7
Class1=CAxisChaserApp
Class3=CMainFrame
Class4=CAboutDlg

ResourceCount=6
Resource1=IDR_MAINFRAME
Class2=CChildView
Resource2=IDD_TEXTCOLORSETTING (English (U.S.))
Class5=CTextColorDlg
Resource3=IDD_ABOUTBOX (English (U.S.))
Class6=CFontSetDlg
Resource4=IDD_FONTSETTING (English (U.S.))
Resource5=IDR_MAINFRAME (English (U.S.))
Class7=COptions
Resource6=IDD_OPTIONS

[CLS:CAxisChaserApp]
Type=0
HeaderFile=AxisChaser.h
ImplementationFile=AxisChaser.cpp
Filter=N

[CLS:CChildView]
Type=0
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp
Filter=W
BaseClass=CWnd 
VirtualFilter=WC
LastObject=ID_SAVEAS

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=AxisChaser.cpp
ImplementationFile=AxisChaser.cpp
Filter=D
LastObject=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_SETBKCOLOR
Command2=ID_SETTEXTCOLOR
Command3=ID_SETFONT
Command4=ID_APP_ABOUT
Command5=ID_APP_EXIT
Command6=ID_EDIT_COPY
Command7=ID_STRING_FIND
Command8=ID_NEXT_FIND
Command9=ID_PREV_FIND
Command10=ID_VIEW_TOOLBAR
Command11=ID_MODE_BINARY
Command12=ID_MODE_TEXT
Command13=ID_SNDRCV
Command14=ID_RTM
Command15=ID_SYSTEMTRACE
Command16=ID_CLEAR
Command17=ID_NOSCROLL
Command18=ID_STAYONTOP
CommandCount=18

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_FIND
Command6=ID_STRING_FIND
Command7=ID_PREV_FIND
Command8=ID_NEXT_PANE
Command9=ID_PREV_PANE
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_EDIT_CUT
Command13=ID_EDIT_UNDO
CommandCount=13

[CLS:CTextColorDlg]
Type=0
HeaderFile=TextColorDlg.h
ImplementationFile=TextColorDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_SETRPTCOLOR

[CLS:CFontSetDlg]
Type=0
HeaderFile=FontSetDlg.h
ImplementationFile=FontSetDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CFontSetDlg

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_SAVE
Command2=ID_SAVEAS
Command3=ID_SETBKCOLOR
Command4=ID_SETTEXTCOLOR
Command5=ID_SETFONT
Command6=ID_OPTIONS
Command7=ID_APP_ABOUT
Command8=ID_APP_EXIT
Command9=ID_EDIT_COPY
Command10=ID_STRING_FIND
Command11=ID_NEXT_FIND
Command12=ID_PREV_FIND
Command13=ID_VIEW_TOOLBAR
Command14=ID_MODE_BINARY
Command15=ID_MODE_TEXT
Command16=ID_SNDRCV
Command17=ID_RTM
Command18=ID_SYSTEMTRACE
Command19=ID_REPORTFORMAT
Command20=ID_CLEAR
Command21=ID_NOSCROLL
Command22=ID_STAYONTOP
CommandCount=22

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=3
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352

[DLG:IDD_FONTSETTING (English (U.S.))]
Type=1
Class=CFontSetDlg
ControlCount=20
Control1=IDC_ALLFONTSETTING,button,1342341129
Control2=IDC_EACHFONTSETTING,button,1342210057
Control3=IDC_SETALLFONT,button,1342242816
Control4=IDC_SETSNRFONT,button,1342242816
Control5=IDC_SETRTMFONT,button,1342242816
Control6=IDC_SETSYSFONT,button,1342242816
Control7=IDCANCEL,button,1342242816
Control8=IDOK,button,1342242817
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308864
Control11=IDC_STATIC,static,1342308864
Control12=IDC_STATIC,static,1342308864
Control13=IDC_STATIC,static,1342308864
Control14=IDC_ALLFONT,static,1342312960
Control15=IDC_SNRFONT,static,1342312960
Control16=IDC_RTMFONT,static,1342312960
Control17=IDC_SYSFONT,static,1342312960
Control18=IDC_SETRPTFONT,button,1342242816
Control19=IDC_STATIC,static,1342308864
Control20=IDC_RPTFONT,static,1342312960

[DLG:IDD_TEXTCOLORSETTING (English (U.S.))]
Type=1
Class=CTextColorDlg
ControlCount=20
Control1=IDC_ALLTEXTCOLOR,button,1342341129
Control2=IDC_EACHTEXTCOLOR,button,1342210057
Control3=IDC_SETALLCOLOR,button,1342242816
Control4=IDC_SETSNRCOLOR,button,1342242816
Control5=IDC_SETRTMCOLOR,button,1342242816
Control6=IDC_SETSYSCOLOR,button,1342242816
Control7=IDC_SETRPTCOLOR,button,1342242816
Control8=IDOK,button,1342242817
Control9=IDCANCEL,button,1342242816
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342308864
Control12=IDC_STATIC,static,1342308864
Control13=IDC_STATIC,static,1342308864
Control14=IDC_STATIC,static,1342308864
Control15=IDC_ALLCOLOR,static,1342312448
Control16=IDC_SNRCOLOR,static,1342312448
Control17=IDC_RTMCOLOR,static,1342312448
Control18=IDC_SYSCOLOR,static,1342312448
Control19=IDC_STATIC,static,1342308864
Control20=IDC_RPTCOLOR,static,1342312448

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_SETBKCOLOR
Command2=ID_SETTEXTCOLOR
Command3=ID_SETFONT
Command4=ID_MODE_BINARY
Command5=ID_MODE_TEXT
Command6=ID_SNDRCV
Command7=ID_RTM
Command8=ID_SYSTEMTRACE
Command9=ID_REPORTFORMAT
Command10=ID_CLEAR
Command11=ID_NOSCROLL
Command12=ID_STAYONTOP
CommandCount=12

[DLG:IDD_OPTIONS]
Type=1
Class=COptions
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SEND,button,1342242819
Control4=IDC_RECEIVE,button,1342242819
Control5=IDC_HEADER,button,1342242819
Control6=IDC_DATA,button,1342242819
Control7=IDC_ALLCODE,button,1342177289
Control8=IDC_FILTERINGCODE,button,1342177289
Control9=IDC_EDIT,edit,1350631552
Control10=IDC_ADD,button,1342242816
Control11=IDC_REMOVE,button,1342242816
Control12=IDC_LIST,SysListView32,1350631433
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,button,1342177287

[CLS:COptions]
Type=0
HeaderFile=Options.h
ImplementationFile=Options.cpp
BaseClass=CDialog
Filter=D
LastObject=COptions
VirtualFilter=dWC


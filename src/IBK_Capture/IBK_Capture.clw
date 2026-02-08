; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CIBK_CaptureDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "IBK_Capture.h"

ClassCount=3
Class1=CIBK_CaptureApp
Class2=CIBK_CaptureDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_IBK_CAPTURE_DIALOG

[CLS:CIBK_CaptureApp]
Type=0
HeaderFile=IBK_Capture.h
ImplementationFile=IBK_Capture.cpp
Filter=N

[CLS:CIBK_CaptureDlg]
Type=0
HeaderFile=IBK_CaptureDlg.h
ImplementationFile=IBK_CaptureDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CIBK_CaptureDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=IBK_CaptureDlg.h
ImplementationFile=IBK_CaptureDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_IBK_CAPTURE_DIALOG]
Type=1
Class=CIBK_CaptureDlg
ControlCount=9
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_BTN_START,button,1342242816
Control4=IDC_EDIT_PATH,edit,1484849280
Control5=IDC_BTN_FIND,button,1342242816
Control6=IDC_CB_TIME,combobox,1344339970
Control7=IDC_STATIC,static,1342308352
Control8=IDC_BTN_STOP,button,1342242816
Control9=IDC_BTN_JPEG,button,1073807360


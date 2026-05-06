#include "stdafx.h"
#include "EAPISample.h"
#include "EAPISampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CEAPISampleApp, CWinApp)
END_MESSAGE_MAP()

CEAPISampleApp::CEAPISampleApp()
{
}

CEAPISampleApp theApp;

BOOL CEAPISampleApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("EAPISample_IBK"));

	CEAPISampleDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}

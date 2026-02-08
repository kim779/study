// IBKS_Sample.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "IBKS_Sample.h"
#include "IBKS_SampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIBKS_SampleApp

BEGIN_MESSAGE_MAP(CIBKS_SampleApp, CWinApp)
	//{{AFX_MSG_MAP(CIBKS_SampleApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIBKS_SampleApp construction

CIBKS_SampleApp::CIBKS_SampleApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIBKS_SampleApp object

CIBKS_SampleApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CIBKS_SampleApp initialization

BOOL CIBKS_SampleApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CIBKS_SampleDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

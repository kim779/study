// DrChartSet.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DrChartSet.h"
#include "DrChartSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrChartSetApp

BEGIN_MESSAGE_MAP(CDrChartSetApp, CWinApp)
	//{{AFX_MSG_MAP(CDrChartSetApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrChartSetApp construction

CDrChartSetApp::CDrChartSetApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDrChartSetApp object

CDrChartSetApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDrChartSetApp initialization

BOOL CDrChartSetApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CDrChartSetDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CDrChartSetApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			BOOL bControl = FALSE, bShift = FALSE;
			if(GetKeyState(VK_CONTROL) & 0xf0)	{ bControl = TRUE; }
			if(GetKeyState(VK_SHIFT) & 0xf0)	{ bShift = TRUE; }
			
			//Ctrl+Shift+5이면 리스트 보여줌.
			WPARAM vk = (WPARAM)pMsg->wParam;
			if(bControl && bShift)
			{
				//if(vk >= 48 && vk <= 57)		// 0 ~ 9
				if(vk == 53)
				{
					CDrChartSetDlg* pDlgMain =  (CDrChartSetDlg*)m_pMainWnd;
					pDlgMain->DoShowVip();
				}
			}
		}
	}
	
	return CWinApp::PreTranslateMessage(pMsg);
}


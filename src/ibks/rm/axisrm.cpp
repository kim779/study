// axisrm.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "axisrm.h"
#include "resDlg.h"
#include "aboutdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxisrmApp

BEGIN_MESSAGE_MAP(CAxisrmApp, CWinApp)
	//{{AFX_MSG_MAP(CAxisrmApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisrmApp construction

CAxisrmApp::CAxisrmApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_mode = rsNONE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAxisrmApp object

CAxisrmApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAxisrmApp initialization

BOOL CAxisrmApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	parsingCommandStatus();

	if (m_mode == rsNONE)
	{
		CAboutDlg aboutDlg;
		aboutDlg.DoModal();
	}
	else
	{
		CResDlg	resDlg(m_mode);
		resDlg.DoModal();
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CAxisrmApp::parsingCommandStatus()
{
	m_mode = rsNONE;

	CCommLine cmdInfo;
	ParseCommandLine(cmdInfo);
}


/////////////////////////////////////////////////////////////////////////////
//
//	CCommLine
//

CCommLine::CCommLine()
{
}

CCommLine::~CCommLine()
{
}

void CCommLine::ParseParam(LPCSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	CAxisrmApp* app = (CAxisrmApp *)AfxGetApp();
	CString tmps    = lpszParam;

	if (bFlag)
	{
		if (!tmps.CompareNoCase("admin"))
			app->m_mode = rsADMIN;
		else if (!tmps.CompareNoCase("axis"))
			app->m_mode = rsAXIS;
		else if (!tmps.CompareNoCase("rop"))
			app->m_mode = rsROP;
		else if (!tmps.CompareNoCase("help"))
			app->m_mode = rsHELP;
		else if (!tmps.CompareNoCase("mtbl"))
			app->m_mode = rsMTBL;
		else if (!tmps.CompareNoCase("image"))
			app->m_mode = rsIMAGE;
		else if (!tmps.CompareNoCase("tab"))
			app->m_mode = rsTAB;
		else if (!tmps.CompareNoCase("dev"))
			app->m_mode = rsDEV;
		else if (!tmps.CompareNoCase("map"))
			app->m_mode = rsMAP;
		else if (!tmps.CompareNoCase("exe"))
			app->m_mode = rsEXE;
		else if (!tmps.CompareNoCase("menu"))
			app->m_mode = rsMENU;
		else if (!tmps.CompareNoCase("gex"))
			app->m_mode = rsGEX;
	}
}

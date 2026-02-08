// PctrWinix.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PctrWinix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CPctrWinixApp

BEGIN_MESSAGE_MAP(CPctrWinixApp, CWinApp)
	//{{AFX_MSG_MAP(CPctrWinixApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPctrWinixApp construction

CPctrWinixApp::CPctrWinixApp()
{
	for(int i=0; i<MAX_Partner_INTERFACE; i++) m_uaInterface[i].pInterface=NULL;

	m_uaInterface[0].pInterface = (LONG)&m_GateMng;
	m_uaInterface[1].pInterface = (LONG)&m_GwanMng;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPctrWinixApp object

BOOL CPctrWinixApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_CodeMasterWnd.OpenWindow();

	return CWinApp::InitInstance();
}

int CPctrWinixApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_CodeMasterWnd.DestroyWindow();
	
	return CWinApp::ExitInstance();
}

CPctrWinixApp theApp;

/////////////////////////////////////////////////////////////////////////////
// Export Function

LONG WINAPI GetInferface(int nID, long dwData)
{
	return theApp.m_GateMng.GetInterface(nID, dwData);
}

LONG WINAPI SetInferface(int nID, long dwData)
{
	//	int nID = (int)wp;
	//	LONG dwInterface = dwData
	
	return NULL;
}

void WINAPI SetPcTrInterface(int nID, long dwData)
{
}

HWND WINAPI GetCodeMaster()
{
	return theApp.m_CodeMasterWnd.GetSafeHwnd();
}

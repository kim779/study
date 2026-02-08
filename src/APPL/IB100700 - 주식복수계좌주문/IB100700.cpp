// IB100700.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "IB100700.h"
#include "MapWnd.h"

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
// CIB100700App

BEGIN_MESSAGE_MAP(CIB100700App, CWinApp)
	//{{AFX_MSG_MAP(CIB100700App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIB100700App construction

CIB100700App::CIB100700App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIB100700App object

CIB100700App theApp;

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	std::unique_ptr<CMapWnd> pMapWnd = std::make_unique<CMapWnd>(pParent);

	if (!pMapWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 
		CRect(0, 0, SZ_MAP_WIDTH, SZ_MAP_HEIGHT), pParent, 0))
	{
		pMapWnd = nullptr;
		return NULL;
	}
	
	pMapWnd->Init();

	return pMapWnd.release();
}
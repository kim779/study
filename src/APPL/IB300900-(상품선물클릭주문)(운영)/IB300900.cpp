// IB191000.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "IB300900.h"
#include "MapWnd.h"
#include <initguid.h>
#include "IB300900_i.c"

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
// CIB191000App

BEGIN_MESSAGE_MAP(CIB191000App, CWinApp)
	//{{AFX_MSG_MAP(CIB191000App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIB191000App construction

CIB191000App::CIB191000App()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIB191000App object

CIB191000App theApp;

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CMapWnd *pWnd = new CMapWnd(pParent);
	
	if (!pWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, CRect(0, 0, 684, 600), pParent, 0))
	{
		delete pWnd;
		pWnd = NULL;
	}
	
	pWnd->Init();
	
	return pWnd;
}

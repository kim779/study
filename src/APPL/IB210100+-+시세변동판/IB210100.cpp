// IB210100.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "IB210100.h"

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
// CIB210100App

BEGIN_MESSAGE_MAP(CIB210100App, CWinApp)
	//{{AFX_MSG_MAP(CIB210100App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIB210100App construction

CIB210100App::CIB210100App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIB210100App object

CIB210100App theApp;


void CIB210100App::SaveBuffer(CString sKey, CString sData)
{
	m_mapSaveList.SetAt(sKey, sData); 
}

CString CIB210100App::GetBuffer(CString sKey)
{
	CString sResult = _T("");
	m_mapSaveList.Lookup(sKey, sResult); 

	return sResult;
}
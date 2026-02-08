// IB300200.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "IB300200.h"
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
// CIB300200App

BEGIN_MESSAGE_MAP(CIB300200App, CWinApp)
	//{{AFX_MSG_MAP(CIB300200App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIB300200App construction

CIB300200App::CIB300200App()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIB300200App object

CIB300200App theApp;


BOOL CIB300200App::InitInstance() 
{
	AfxEnableControlContainer();
	m_SaveMapList.RemoveAll();
	return CWinApp::InitInstance();
}
void CIB300200App::SaveBuffer(CString sKey, CString sData)
{
	m_SaveMapList.SetAt(sKey, sData); 
}

CString CIB300200App::GetBuffer(CString sKey)
{
	CString sResult = _T("");
	m_SaveMapList.Lookup(sKey, sResult); 

	return sResult;
}
void CIB300200App::DeleteKey(CString sKey)
{
	m_SaveMapList.RemoveKey(sKey); 
}


#define CX	565
#define CY	467

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	std::unique_ptr<CMapWnd>pMapWnd = std::make_unique<CMapWnd>(parent);

	if (!pMapWnd->Create(NULL, NULL, WS_CHILD | WS_CLIPCHILDREN, 
		CRect(0, 0, MAP_WIDTH, MAP_HEIGHT), parent, 0, NULL))
	{
		pMapWnd = nullptr;
		return NULL;
	}
	pMapWnd->Init();

	return pMapWnd.release();
}


int CIB300200App::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_SaveMapList.RemoveAll();	
	return CWinApp::ExitInstance();
}

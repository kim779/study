#include "stdafx.h"
#include <initguid.h>
#include "IBNEWSXX.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CIBNEWSXXApp, CWinApp)
	//{{AFX_MSG_MAP(CIBNEWSXXApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CIBNEWSXXApp::CIBNEWSXXApp()
{
}

CIBNEWSXXApp theApp;

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	std::unique_ptr<CMapWnd> pWnd = std::make_unique<CMapWnd>(parent);

	if (!pWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, CRect(0,0,600,480), parent, -1))
	{
	}

	return pWnd.release();
}

BOOL CIBNEWSXXApp::InitInstance() 
{
	return CWinApp::InitInstance();
}

int CIBNEWSXXApp::ExitInstance() 
{
	return CWinApp::ExitInstance();
}
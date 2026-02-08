// IB401300.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "IB401300.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CIB401300App, CWinApp)
	//{{AFX_MSG_MAP(CIB401300App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CIB401300App::CIB401300App()
{
}

CIB401300App theApp;

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	std::unique_ptr<CMapWnd> pWnd = std::make_unique<CMapWnd>(pParent);
	
	if (!pWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, CRect(0, 0, 310, 650), pParent, 0))
	{
		pWnd = nullptr;
	}
	
	return pWnd.release();
}

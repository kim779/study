#include "stdafx.h"
#include "IB650001.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CIB650001App, CWinApp)
	//{{AFX_MSG_MAP(CIB650001App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CIB650001App::CIB650001App()
{
}

CIB650001App theApp;

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CMapWnd *pWnd = new CMapWnd(pParent);
	
	if (!pWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, CRect(0, 0, 775, 390), pParent, 0))
	{
		delete pWnd;
		pWnd = NULL;
	}
	
	return pWnd;
}

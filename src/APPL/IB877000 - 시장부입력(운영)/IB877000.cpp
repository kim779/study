#include "stdafx.h"
#include "IB877000.h"

#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CIB877000App, CWinApp)
	//{{AFX_MSG_MAP(CIB877000App)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CIB877000App::CIB877000App()
{
}

CIB877000App theApp;

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	std::unique_ptr<CMapWnd> pMapWnd = std::make_unique < CMapWnd >(pParent);
	
	if (!pMapWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, CRect(0, 0, 800, 600), pParent, 0))
	{
		return NULL;
	}
	
	return pMapWnd.release();
}

BOOL CIB877000App::InitInstance() 
{
	AfxInitRichEdit();
	
	return CWinApp::InitInstance();
}

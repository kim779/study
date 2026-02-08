#include "stdafx.h"
#include "CX_IB220001.h"
#include "ControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CCX_IB220001App, CWinApp)
	//{{AFX_MSG_MAP(CCX_IB220001App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CCX_IB220001App::CCX_IB220001App()
{
}

CCX_IB220001App theApp;

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* pParent, _param* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CControlWnd *pCtrl = new CControlWnd(pParent, pParam);
	if (!pCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, pParam->rect, pParent, -1))
	{
		TRACE("CX_IB220001 Create Failed\n");
		return 0;
	}
	
	return pCtrl;
	
}

__declspec(dllexport) CWnd* WINAPI axCreateDLL(CWnd* pParent, _param* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CControlWnd* pCtrl = new CControlWnd(pParent, pParam);
	if (!pCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, pParam->rect, pParent, -1))
	{
		TRACE("CX_IB220001 Create Failed\n");
		return 0;
	}

	return pCtrl;
}

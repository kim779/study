#include "stdafx.h"
#include "libctr.h"
#include "MainWnd.h"
#include "CX_PCIdentity.h"
#include "../../h/axisfire.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//AfxEnableControlContainer();
	//AfxInitRichEdit( );
	CMainWnd* pDlg = NULL;

	CRect rect(0,0,0,0);
		
	pDlg = new CMainWnd(parent);
	if(!pDlg->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rect, parent, 100))
	{
		OutputDebugString("Failed to load Identity Control.\n");
		return 0;
	}
	
	return pDlg;
}

__declspec(dllexport) char* WINAPI axGetMac()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sMac;
	PC_INFO_CONTEXT	pContext{};
	const int ret = GetPCIdentity(&pContext, 13);

	if (ret == P_PARAMETER_IS_NULL)
		return nullptr;

	if (ret == P_OK)
	{
		sMac = pContext.szPhysicalMAC;
		return (LPSTR)(LPCTSTR)sMac;
	}
	return nullptr;
}
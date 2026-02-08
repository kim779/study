#include "stdafx.h"
#include "GraphWnd.h"
#include "C_MBong.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* pParent, void* cInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CC_MBongApp*)AfxGetApp())->SetView(pParent);

	struct _param*	param = (struct _param*)cInfo;
	CGraphWnd* mapWnd = new CGraphWnd(pParent, param);	
	
	mapWnd->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, param->rect, pParent, 0);

	return (CWnd*)mapWnd;
}

__declspec(dllexport) CWnd* WINAPI axCreateEx(CWnd* pParent, void* cInfo, CWnd* pSend)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CC_MBongApp*)AfxGetApp())->SetView(pSend);

	struct _param*	param = (struct _param*)cInfo;
	CGraphWnd* mapWnd = new CGraphWnd(pSend, param);	
	
	mapWnd->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, param->rect, pParent, 0);

	return (CWnd*)mapWnd;
}
#include "stdafx.h"
#include "C_VolLine1.h"
#include "MainWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* pParent, void* cInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	struct _param*	param = (struct _param*)cInfo;
	CMainWnd* pMainWnd = new CMainWnd(pParent, param);
	
	pMainWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, param->rect, pParent, 0);
	SetWindowPos(pMainWnd->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

	return (CWnd*)pMainWnd;
}


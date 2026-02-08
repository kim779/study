#include "stdafx.h"
#include "C_Avol.h"
#include "MainWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* pParent, void* cInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	struct _param*	param = (struct _param*)cInfo;
	std::unique_ptr<CMainWnd> pMainWnd = std::make_unique<CMainWnd>(pParent, param);
	
	pMainWnd->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, param->rect, pParent, 0);
	SetWindowPos(pMainWnd->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	return pMainWnd.release();
}


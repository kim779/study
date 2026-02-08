#include "stdafx.h"
#include "Cx_MiniChart.h"
#include "MainWnd.h"


__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* pParent, _param* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	auto pMainWnd = std::make_unique<CMainWnd>(pParent, pParam);
	
	pMainWnd->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, pParam->rect, pParent, 100);
	return pMainWnd.release();
}

__declspec(dllexport) CWnd* WINAPI axCreateDLL(CWnd* pParent, _param* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	auto pMainWnd = std::make_unique<CMainWnd>(pParent, pParam);
	
	pMainWnd->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE, pParam->rect, pParent, 100);
	return pMainWnd.release();
}



#include "stdafx.h"
#include "C_TickEx.h"
#include "MainWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* pParent, void* cInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	struct _param*	param = (struct _param*)cInfo;
	auto pMainWnd = std::make_unique<CMainWnd>(pParent, param);
	
	pMainWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, param->rect, pParent, 0);
	SetWindowPos(pMainWnd->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	return (CWnd*)pMainWnd.release();
}

/*
	data		- _cdTick2
	graph		- line chart(1ºÐºÀ, Æ½)
	parameter	- /d/r/e/u
	d : display count - 0:all
	r : request count
	e : data expand(true-ºÐ, false-Æ½)
	u : data unit - 1:Á¾¸ñ, 2:¾÷Á¾, 3:¼±¹°, 4:¾÷Á¾, 5:ECN
	t : draw tick - 0: no 1: left, 2: right
	i : data index - 1:Æ½, 2:ºÐ
*/
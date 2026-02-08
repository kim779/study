#include "stdafx.h"
#include "C_Tick.h"
#include "GrpWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* pParent, void* cInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	struct _param*	param = (struct _param*)cInfo;
	auto pGrpWnd = std::make_unique<CGrpWnd>(pParent, param);
	
	pGrpWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, param->rect, pParent, 0);
	SetWindowPos(pGrpWnd->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

	return (CWnd*)pGrpWnd.release();
}

/*
	data		- _cdTick
	graph		- line chart(1ºÐºÀ, Æ½)
	parameter	- /d/r/e/u
	d : display count - 0:all
	r : request count
	e : data expand(true-ºÐ, false-Æ½)
	u : data unit - 1:Á¾¸ñ, 2:¾÷Á¾, 3:¼±¹°, 4:¾÷Á¾, 5:ECN
	t : draw tick - 0: no 1: left, 2: right
	i : data index - 1:Æ½, 2:ºÐ
*/

__declspec(dllexport) CWnd* WINAPI axCreate2(CWnd* pParent, void* cInfo, CWnd* pwndWizard)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	struct _param*	param = (struct _param*)cInfo;
	auto pGrpWnd = std::make_unique<CGrpWnd>(pwndWizard, param);
	
	pGrpWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, param->rect, pParent, 0);
	SetWindowPos(pGrpWnd->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

	return (CWnd*)pGrpWnd.release();
}
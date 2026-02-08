#include "stdafx.h"
#include "libctrl.h"
#include "ControlWnd.h"


__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, _param* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CControlWnd *control = new CControlWnd(parent, pParam);

	if (!control->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, pParam->rect, parent, (UINT) control))
	{
		delete control;
		control = nullptr;
	}

	if (control && !control->Initialize(FALSE))
	{
		delete control;
		control = nullptr;
	}

	return control;
}

__declspec(dllexport) CWnd* WINAPI axCreateDll(CWnd *parent, _param* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CControlWnd *control = new CControlWnd(parent, pParam);

	if (!control->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, pParam->rect, parent, (UINT) control))
	{
		delete control;
		control = nullptr;
	}

	if (control && !control->Initialize(TRUE))
	{
		delete control;
		control = nullptr;
	}

	return control;
}
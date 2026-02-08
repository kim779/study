#include "stdafx.h"
#include "libctrl.h"
#include "ControlWnd.h"


__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CControlWnd *control = new CControlWnd();
	control->m_pParent = parent;

	control->SetParam((struct _param*)pParam);
	if (!control->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, control->m_Param.rect, parent, 100))
	{
		delete control;
		control = NULL;
	}
	return control;
}

__declspec(dllexport) CWnd* WINAPI axCreateEx(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CControlWnd *control = new CControlWnd(TRUE);
	control->m_pParent = parent;

	control->SetParam((struct _param*)pParam);
	if (!control->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, control->m_Param.rect, parent, (UINT) control))
	{
		delete control;
		control = NULL;
	}
	return control;
}
#include "stdafx.h"
#include "libctrl.h"
#include "controlWnd.h"
#include "../../h/axisfire.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* param)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CControlWnd* controlWnd = new CControlWnd();
	controlWnd->m_parent = parent;
	controlWnd->SetParam((_param *) param);
	controlWnd->Create(NULL, NULL, WS_CHILD|WS_CLIPSIBLINGS, controlWnd->m_param.rect, parent, 100);

	return controlWnd;
}
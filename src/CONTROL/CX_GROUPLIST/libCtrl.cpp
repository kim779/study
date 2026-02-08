#include "stdafx.h"
#include "libctrl.h"
#include "ControlWnd.h"

__declspec(dllexport) CWnd* APIENTRY axCreate(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	struct _param* param = (struct _param*)pParam;
	CControlWnd* pWnd = new CControlWnd(param, parent);
	if (!pWnd->Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, param->rect, parent, 100))
		return NULL;	

	return pWnd;
}

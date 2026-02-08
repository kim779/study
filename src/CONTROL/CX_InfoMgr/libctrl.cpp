#include "stdafx.h"
#include "ControlWnd.h"
#include "../../h/axisfire.h"

__declspec(dllexport) CWnd* APIENTRY axCreate(CWnd *pParent, struct _param *ptr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CControlWnd* pCtrlWnd = new CControlWnd;
	pCtrlWnd->CreateCtrl(pParent, ptr);

	return pCtrlWnd;
}

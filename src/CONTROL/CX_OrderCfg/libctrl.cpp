#include "stdafx.h"
#include "libctrl.h"
#include "ControlWnd.h"
#include "../../h/axisfire.h"

__declspec(dllexport) CWnd* APIENTRY axCreate(CWnd *parent, struct _param *ptr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CControlWnd* ctrl = new CControlWnd;
	ctrl->CreateCtrl(parent, ptr);
	return ctrl;
}

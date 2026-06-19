#include "pch.h"
#include "libctrl.h"
#include "ControlWnd.h"
#include "../../../H/axisfire.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_param* p = static_cast<_param*>(pParam);
	ControlWnd* ctrl = new ControlWnd();
	ctrl->SetParam(p);
	if (!ctrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		p->rect, parent, (UINT)(UINT_PTR)ctrl))
	{
		delete ctrl;
		return nullptr;
	}
	return ctrl;
}

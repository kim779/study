#include "stdafx.h"
#include "libctrl.h"
#include "depth.h"

__declspec(dllexport) CWnd* APIENTRY axCreate(CWnd *parent, void* ptr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	Cdepth* depth = new Cdepth;
	depth->Create(parent, ptr);
	return depth;
}

#include "stdafx.h"
#include "libcom.h"
#include "Component.h"

__declspec(dllexport) CCmdTarget* APIENTRY axCreateEx(void* root)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComponent* File = new CComponent;
	return File;
}

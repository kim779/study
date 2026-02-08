#include "stdafx.h"
#include "libFile.h"
#include "Filex.h"

__declspec(dllexport) CCmdTarget* WINAPI axCreateEx(void* root)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CFilex* File = new CFilex;
	return File;
}

#include "stdafx.h"
#include "libcom.h"
#include "Execute.h"
#include "../../h/axisfire.h"

__declspec(dllexport) CCmdTarget* APIENTRY axCreateEx(void* root)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CExecute *m_pComponent = new CExecute();

	return m_pComponent;
}
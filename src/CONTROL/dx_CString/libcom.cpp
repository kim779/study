#include "stdafx.h"
#include "libcom.h"
#include "Component.h"
#include "../../h/axisfire.h"

__declspec(dllexport) CCmdTarget* WINAPI axCreateEx(void* root)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComponent *m_pComponent = new CComponent();

	return m_pComponent;
}
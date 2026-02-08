#include "stdafx.h"
#include "libcom.h"
#include "Component.h"

__declspec(dllexport) CCmdTarget* WINAPI axCreateEx(void* root)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComponent *m_pComponent = new CComponent();

	m_pComponent->m_sRoot = CString((char*)root);
	return m_pComponent;
}
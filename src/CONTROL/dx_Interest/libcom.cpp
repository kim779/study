#include "stdafx.h"
#include "libcom.h"
#include "Component.h"
#include "../../h/axisfire.h"

__declspec(dllexport) CCmdTarget* WINAPI axCreateEx(void* root)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	auto m_pComponent = std::make_unique<CComponent>();
	m_pComponent->m_sRoot = CString((char*)root);
	return m_pComponent.release();
}
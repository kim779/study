#include "stdafx.h"
#include "libmap.h"
#include "MapWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	std::unique_ptr<CMapWnd> m_pMapWnd = std::make_unique<CMapWnd>();
	m_pMapWnd->m_pParent = parent;
	m_pMapWnd->CreateMap(parent);

	return m_pMapWnd.release();
}

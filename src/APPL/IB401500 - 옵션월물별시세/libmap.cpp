#include "stdafx.h"
#include "MapWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	std::unique_ptr< CMapWnd > m_pMapWnd = std::make_unique< CMapWnd>();
	m_pMapWnd->CreateMap(pParent);

	return m_pMapWnd.release();

//	CMapWnd *m_pMapWnd = new CMapWnd();
//	m_pMapWnd->CreateMap(pParent);

//	return m_pMapWnd;
}


#include "stdafx.h"
#include "libmap.h"
#include "MapWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMapWnd *m_pMapWnd = new CMapWnd();
	m_pMapWnd->m_pParent = parent;
	m_pMapWnd->CreateMap(parent);

	return m_pMapWnd;
}

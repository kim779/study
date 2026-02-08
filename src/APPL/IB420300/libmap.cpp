#include "stdafx.h"
#include "libmap.h"
#include "MapWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	std::unique_ptr<CMapWnd> m_pMapWnd = std::make_unique<CMapWnd>();
	m_pMapWnd->m_pParent = parent;
	m_pMapWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, MAPWND_RECT, parent, 0);
	return m_pMapWnd.release();
}

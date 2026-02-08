#include "stdafx.h"
#include "libmap.h"
#include "mapWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	auto mapWnd = std::make_unique<CMapWnd>(parent);
	mapWnd->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(0, 0, 840, 397), parent, 0);
	return mapWnd.release();
}

#include "stdafx.h"
#include "libmap.h"
#include "mapWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	std::unique_ptr<CMapWnd> mapWnd = std::make_unique<CMapWnd>(parent);
	mapWnd->CreateMap(parent);

	return mapWnd.release();
}

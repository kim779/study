#include "stdafx.h"
#include "libmap.h"
#include "mapWnd.h"

__declspec(dllexport) CWnd* APIENTRY axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	std::unique_ptr<CMapWnd> mapWnd = std::make_unique<CMapWnd>(parent);
	mapWnd->CreateMap();
	return mapWnd.release();
}
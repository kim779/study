#include "stdafx.h"
#include "IBXXXX09.h"
#include "libmap.h"
#include "mapWnd.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	auto m_pMapWnd = std::make_unique<CMapWnd>();
	m_pMapWnd->CreateMap(parent);
	
	return m_pMapWnd.release();
}


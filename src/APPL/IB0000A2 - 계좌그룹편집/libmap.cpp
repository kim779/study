#include "stdafx.h"
#include "IB0000A2.h"
#include "libmap.h"
#include "mapForm.h"
#include "libDefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMapForm *pWnd = new CMapForm();
	pWnd->m_pParent = parent;
	pWnd->CreateMap(parent, CSize(SCRWIDTH, SCRHEIGHT));
	
	return (CWnd*)pWnd;
}

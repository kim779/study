// libx.cpp: implementation of the libEX000101 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IB100000.h"
#include "libx.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CIB100000App *pApp = (CIB100000App *) AfxGetApp();
	ASSERT_VALID(pApp);
	return pApp->loadMap(parent);
}

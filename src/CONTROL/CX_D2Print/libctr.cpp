#include "stdafx.h"
#include "libctr.h"
#include "MainWnd.h"
#include "CX_D2Print.h"
#include "../../h/axisfire.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CMainWnd* pDlg = new CMainWnd(parent);
	if (!pDlg->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(0,0,0,0), parent, 100))
	{
		TRACE("D2Print Create Failed\n");
		return 0;
	}
	
	return pDlg;
}
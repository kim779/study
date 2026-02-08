#include "stdafx.h"
#include "libctr.h"
#include "MainWnd.h"
#include "CX_STOPLOSS.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CRect	rect;
	CMainWnd* pDlg = NULL;

	rect.SetRectEmpty();
	pDlg = new CMainWnd(parent,(struct _param*)pParam);
	if (!pDlg->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rect, parent, 100))
	{
		delete pDlg;
		return 0;
	}
	
	return pDlg;
}
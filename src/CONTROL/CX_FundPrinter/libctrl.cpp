#include "stdafx.h"
#include "libctrl.h"
#include "MainWnd.h"
#include "CX_FundPrinter.h"
#include "../../h/axisfire.h"

/*
__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* pParent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CMainWnd *pMainWnd = new CMainWnd(pParent);
	
	pMainWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, ((_param*)pParam)->rect, pParent, 100);
	return pMainWnd;
}
*/

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//AfxEnableControlContainer();
	//AfxInitRichEdit( );
	CMainWnd* pDlg = NULL;

	CRect rect(0,0,0,0);
		
	pDlg = new CMainWnd(parent);
    pDlg->SetParam ((struct _param*)pParam);
	if(!pDlg->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, pDlg->m_Param.rect, parent, 100))
	{
		TRACE("AccountCtrl Create Failed\n");
		return 0;
	}

// 	try{
// 		if (!pDlg->Create(IDD_DIALOG1, NULL))
// 		{
// 			delete pDlg;
// 			pDlg = NULL;
// 		}
// // 		else
// // 			pDlg->ShowWindow(SW_SHOW);
// // 		}
// 	}
// 	catch(CException* e)
// 	{
// 		char* s;
// 
// 		e->GetErrorMessage(s,2,NULL);
// 	}
	
	return pDlg;
}

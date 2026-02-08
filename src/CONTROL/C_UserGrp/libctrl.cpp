#include "stdafx.h"
#include "C_UserGrp.h"
#include "MainWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *pParent, void* pInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	auto pMainWnd = std::make_unique<CMainWnd>(pParent, pInfo);
	pMainWnd->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, ((_param *)pInfo)->rect, pParent, 100);
	return pMainWnd.release();
}

__declspec(dllexport) char* WINAPI axPropDlg(char* pStr, char* pMtblPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	static CString retVal;
	CC_UserGrpApp	*pApp = (CC_UserGrpApp *)AfxGetApp();

	if (!pApp->m_hDlg)
		pApp->m_hDlg = LoadLibrary("C_UserGrpDlg.dll");

	if (pApp->m_hDlg)
	{
		char*	(APIENTRY *axPropDlg)(char *, char *);

		axPropDlg = (char* (APIENTRY *)(char *, char *))GetProcAddress(pApp->m_hDlg, "axPropDlg");
		if (axPropDlg)
			retVal = axPropDlg(pStr, pMtblPath);
	}

	return (char *)retVal.GetString(); 
}

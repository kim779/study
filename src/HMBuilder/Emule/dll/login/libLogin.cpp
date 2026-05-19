#include "stdafx.h"
#include "liblogin.h"
#include "axislogin.h"
#include "ledgerx.h"


__declspec(dllexport) void* APIENTRY axLogin(void* data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();
	if (data == NULL)
	{
		CLogin* login = new CLogin;
		app->m_login = login;
		return app->m_login;
	}
	app->m_login->SetLogin((char *)data);
	return NULL;
}

__declspec(dllexport) void  APIENTRY axLoginLedger(void* ledger)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();
	struct _ledgerH *ledgerH = (struct _ledgerH *)ledger;

	FillMemory(ledgerH, L_ledgerH, ' ');
//	CopyMemory(ledgerH->ledgerH.flag, "0000", 4);
//	CopyMemory(ledgerH->ledgerH.svid, "01", sizeof(ledgerH->ledgerH.svid));
	
	CopyMemory(ledgerH->usid, app->m_user, min(app->m_user.GetLength(), sizeof(ledgerH->usid)));
	CopyMemory(ledgerH->dept, app->m_dept, min(app->m_dept.GetLength(), sizeof(ledgerH->dept)));
	CopyMemory(ledgerH->usnm, app->m_name, min(app->m_name.GetLength(), sizeof(ledgerH->usnm)));
	CopyMemory(ledgerH->term, app->m_term,    min(app->m_term.GetLength(), sizeof(ledgerH->term)));
}

__declspec(dllexport) void* APIENTRY axLedger()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();
	CLedger* ledger = new CLedger;
	return ledger;
}
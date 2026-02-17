#include "stdafx.h"
#include "liblogin.h"
#include "axislogin.h"
#include "ledgerx.h"

__declspec(dllexport) void* APIENTRY axLogin(void* data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString slog;
	//slog.Format("[ledger] axLogin [%s]\r\n", (char*)data);
	//OutputDebugString(slog);

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();
	if (data == NULL)
	{
		CLogin* login = new CLogin;
		app->m_login = login;
		return login;
	}
	else
		return (void *)(app->m_login->SetLogin((char *)data).operator LPCTSTR());

	return NULL;
}

__declspec(dllexport) void APIENTRY axLoginLedger(void* ledger)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();
	struct	_ledgerH* ledgerH = (struct _ledgerH *)ledger;
	CopyMemory(ledgerH->usid, app->m_user, min(app->m_user.GetLength(), sizeof(ledgerH->usid)));
	CopyMemory(ledgerH->pcip, app->m_host, min(app->m_host.GetLength(), sizeof(ledgerH->pcip)));
	CopyMemory(ledgerH->brno, app->m_brno, min(app->m_brno.GetLength(), sizeof(ledgerH->brno)));
	CopyMemory(ledgerH->cogb, app->m_cogb, min(app->m_cogb.GetLength(), sizeof(ledgerH->cogb)));
	CopyMemory(ledgerH->rcnt, "0000", sizeof(ledgerH->rcnt));

	//CString slog;
	//slog.Format("[ledger] axLoginLedger [%s][%s][%s][%s]\r\n", app->m_user, app->m_host, app->m_brno, app->m_cogb);
	//OutputDebugString(slog);
}

__declspec(dllexport) void* APIENTRY axLedger(void* data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString slog;
	//slog.Format("[ledger] axLedger");
	//OutputDebugString(slog);

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();
	CLedger* ledger = new CLedger(data);
	return ledger;
}

__declspec(dllexport) void APIENTRY axLedgerEx(void* ledger, void* data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString slog;
	//slog.Format("[ledger] axLedgerEx [%s]",(char*)data);
	//OutputDebugString(slog);

	((CLedger *)ledger)->LedgerEx(data);
}

__declspec(dllexport) void* APIENTRY axGetLedger(void* ledger, int pos, int length)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString slog;
	//slog.Format("[ledger] axGetLedger length=[%d]", length);
	//OutputDebugString(slog);

	return (void *)((CLedger *)ledger)->GetLedger(pos, length).operator LPCTSTR();
}

__declspec(dllexport) void APIENTRY axSetLedger(void* ledger, int pos, int length, void* data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	
	((CLedger *)ledger)->SetLedger(pos, length, data);
}

__declspec(dllexport) void* APIENTRY axGetLedgerEx(void* ledger, int id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString slog;
	//slog.Format("[ledger] axGetLedgerEx id=[%d] ", id);
	//OutputDebugString(slog);


	return (void *)((CLedger *)ledger)->GetLedgerEx(id, NULL).operator LPCTSTR();
}

__declspec(dllexport) void* APIENTRY axGetLedgerData(void* ledger, void* data, int id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString slog;
	//slog.Format("[ledger] axGetLedgerData id=[%d] ", id);
	//OutputDebugString(slog);

	return (void *)((CLedger *)ledger)->GetLedgerEx(id, data).operator LPCTSTR();
}

__declspec(dllexport) void* APIENTRY axLoginData(int id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//CString slog;
	//slog.Format("[ledger] axLoginData id=[%d] ", id);
	//OutputDebugString(slog);

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();
	return (void *)(app->m_login->GetLogin(id).operator LPCTSTR());
}

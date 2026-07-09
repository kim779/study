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
		return login;
	}
	else
		return (void *)(app->m_login->SetLogin((char *)data).operator LPCTSTR());

	return NULL;
}

__declspec(dllexport) void  APIENTRY axLoginLedger(void* ledger)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();
	struct _ledgerLoginH *ledgerLoginH = (struct _ledgerLoginH *)ledger;

	FillMemory(ledgerLoginH, L_ledgerLoginH, ' ');
	/*
	CopyMemory(ledgerLoginH->ledgerH.flag, "0000", 4);
	CopyMemory(ledgerLoginH->ledgerH.fnam, "01", sizeof(ledgerLoginH->ledgerH.svid));
	
	CString	tmps;
	if (atoi(app->m_whoi) == 0)		// 직원
	{
		CopyMemory(ledgerLoginH->ledgerH.usid, app->m_user, min(app->m_user.GetLength(), sizeof(ledgerLoginH->ledgerH.usid)));
		CopyMemory(ledgerLoginH->ledgerH.dept, app->m_dept, min(app->m_dept.GetLength(), sizeof(ledgerLoginH->ledgerH.dept)));
		tmps = app->m_name.Mid(1);
		CopyMemory(ledgerLoginH->ledgerH.usnm, tmps, min(tmps.GetLength(), sizeof(ledgerLoginH->ledgerH.usnm)));
	}
	else
	{
		if (app->m_wrapwhoi == "W")
		{			
			CopyMemory(ledgerLoginH->ledgerH.usid, app->m_user, min(app->m_user.GetLength(), sizeof(ledgerLoginH->ledgerH.usid)));
			CopyMemory(ledgerLoginH->ledgerH.dept, "Y90", strlen("Y90"));
			CopyMemory(ledgerLoginH->ledgerH.usnm, app->m_name, min(app->m_name.GetLength(), sizeof(ledgerLoginH->ledgerH.usnm)));
		}
		else
		{
			CopyMemory(ledgerLoginH->ledgerH.usid, "HTS002", strlen("HTS002"));
			CopyMemory(ledgerLoginH->ledgerH.dept, "F07",    strlen("F07"));
			CopyMemory(ledgerLoginH->ledgerH.usnm, "HTS002", strlen("HTS002"));
		}
	}
	CopyMemory(ledgerLoginH->ledgerH.term, app->m_term,    min(app->m_term.GetLength(), sizeof(ledgerLoginH->ledgerH.term)));
	ledgerLoginH->ledgerH.uflg[0] = 'Y';
	CopyMemory(ledgerLoginH->ledgerH.user, app->m_user,    min(app->m_user.GetLength(), sizeof(ledgerLoginH->ledgerH.user)));
	CopyMemory(ledgerLoginH->svno, app->m_login->m_svno, min(app->m_login->m_svno.GetLength(), sizeof(ledgerLoginH->svno))); // 서버번호
	CopyMemory(ledgerLoginH->usid, app->m_login->m_user, min(app->m_login->m_user.GetLength(), sizeof(ledgerLoginH->usid))); // 주문자사번
	CopyMemory(ledgerLoginH->term, app->m_login->m_term, min(app->m_login->m_term.GetLength(), sizeof(ledgerLoginH->term))); // 단말번호
	CopyMemory(ledgerLoginH->inputGB, app->m_login->m_inputGb, min(app->m_login->m_inputGb.GetLength(), sizeof(ledgerLoginH->inputGB))); //입력매체구분
	CopyMemory(ledgerLoginH->comGB, app->m_login->m_comGb, min(app->m_login->m_comGb.GetLength(), sizeof(ledgerLoginH->comGB))); // 통신매체구분
	*/
}

__declspec(dllexport) void* APIENTRY axLedger(void* data)
{//맵에서 ledger를 사용하면 첫번째호출
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();
	CLedger* ledger = new CLedger(data);
	return ledger;
}

__declspec(dllexport) void* APIENTRY axGetLedger(void* ledger, int pos, int length)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//m_iInputlen = inputhlen;
	return (void *)((CLedger *)ledger)->GetLedger(pos, length).operator LPCTSTR();
}

__declspec(dllexport) void  APIENTRY axSetLedger(void* ledger, int pos, int length, void* data)
{//화면에 열리면세번째호출
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CLedger *)ledger)->SetLedger(pos, length, data);
}

__declspec(dllexport) void* APIENTRY axGetLedgerEx(void* ledger, int id)
{//화면에 열리면 두번째
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (void *)((CLedger *)ledger)->GetLedgerEx(id, NULL).operator LPCTSTR();
}

__declspec(dllexport) void* APIENTRY axGetLedgerData(void* ledger, void* data, int id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (void *)((CLedger *)ledger)->GetLedgerEx(id, data).operator LPCTSTR();
}

__declspec(dllexport) void* APIENTRY axLoginData(int id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();
	return (void *)(app->m_login->GetLogin(id).operator LPCTSTR());
}

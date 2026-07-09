#include "stdafx.h"
#include "axiscodx.h"
#include "libcodx.h"

__declspec(dllexport) char* APIENTRY axGetMaster(char* root, int* ncode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAxiscodxApp* app = (CAxiscodxApp *)AfxGetApp();
	if (app->m_code->GetMaster(root, ncode))
		return (char *)app->m_code->m_master.operator LPCTSTR();

	return NULL;
}

__declspec(dllexport) bool APIENTRY axGetCategory(char* root, int kind, int index, char* code, bool prev)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAxiscodxApp* app = (CAxiscodxApp *)AfxGetApp();
	return app->m_code->GetCategory(root, kind, index, code, prev);
}

__declspec(dllexport) int APIENTRY axGetSpinData(DWORD type, int nfloat, int xfloat, char* data, double value, bool prev)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAxiscodxApp* app = (CAxiscodxApp *)AfxGetApp();
	return app->m_code->GetSpinData(type, nfloat, xfloat, data, value, prev);
}

__declspec(dllexport) double APIENTRY axGetDelta(DWORD type, double value, bool prev)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAxiscodxApp* app = (CAxiscodxApp *)AfxGetApp();
	return app->m_code->GetDelta(type, value, prev);
}
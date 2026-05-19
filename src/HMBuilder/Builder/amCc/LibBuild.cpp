// LibBuild.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "amCc.h"
#include "LibBuild.h"

// CLibBuild

__declspec(dllexport) void APIENTRY setRoot(CString root)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	((CamCcApp*)AfxGetApp())->m_root = root;
}

__declspec(dllexport) int APIENTRY generateSource(bool alarm, struct _mapH* mapH, bool getsource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CamCcApp* app = (CamCcApp*)AfxGetApp();
	app->LoadControl();
	return app->m_build.GenerateSource(alarm, mapH, getsource);
}

__declspec(dllexport) CString APIENTRY getSource(bool bNotResource, struct _mapH* mapH)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return ((CamCcApp*)AfxGetApp())->m_build.GetSource(bNotResource, mapH);
}

__declspec(dllexport) void APIENTRY wccSetTempPath(CString& dir, char* fileN, bool source)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CamCcApp *)AfxGetApp())->SetTempPath(dir, fileN, source);
}

__declspec(dllexport) void APIENTRY wccSetCurrentDirectory(CString curdir)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CamCcApp *)AfxGetApp())->setCurrentDirectory(curdir);
}

__declspec(dllexport) void APIENTRY wccTrLayOut(int kind, CString tr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CamCcApp *)AfxGetApp())->setTrLayOut(kind, tr);
}

__declspec(dllexport) CString APIENTRY getTrLayOut()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return ((CamCcApp*)AfxGetApp())->GetTrLayOut();
}

__declspec(dllexport) void APIENTRY wccRTSSymbol(int kind, CString tr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CamCcApp *)AfxGetApp())->setRtsSymbol(kind, tr);
}

__declspec(dllexport) CString APIENTRY getRTSSymbol()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return ((CamCcApp*)AfxGetApp())->GetRtsSymbol();
}

__declspec(dllexport) void APIENTRY SetPath(CString& dir, char* fileN, bool source)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CamCcApp *)AfxGetApp())->SetMapFilePath(dir, fileN, source);
}

__declspec(dllexport) void APIENTRY wccGetTempName(char* fileN, char* prefix)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CamCcApp *)AfxGetApp())->GetTempName(fileN, prefix);
}

__declspec(dllexport) void APIENTRY load(struct _mapH* mapH, CString curdir)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CamCcApp* app = (CamCcApp*)AfxGetApp();
	app->LoadControl();
	app->m_load.Load(mapH, curdir);
}

__declspec(dllexport) int APIENTRY getformcount()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return ((CamCcApp*)AfxGetApp())->m_load.formcount();
}
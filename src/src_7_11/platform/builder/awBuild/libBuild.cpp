#include <stdafx.h>
#include "libBuild.h"
#include "awBuild.h"
#include "build.h"
#include "mapload.h"

__declspec(dllexport) int APIENTRY generateSource(bool alarm, struct _mapH* mapH)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ((CAwBuildApp *)AfxGetApp())->m_build.GenerateSource(alarm, mapH);
}

__declspec(dllexport) CString APIENTRY getSource(bool bNotResource, struct _mapH* mapH)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ((CAwBuildApp *)AfxGetApp())->m_build.GetSource(bNotResource, mapH);
}

__declspec(dllexport) void APIENTRY load(struct _mapH* mapH, CString curdir)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CAwBuildApp *)AfxGetApp())->m_load.Load(mapH, curdir);
}

__declspec(dllexport) void APIENTRY buildHeader(CMapToken* token, int count)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CAwBuildApp *)AfxGetApp())->m_load.BuildHeader(token, count);
}

__declspec(dllexport) void APIENTRY buildForm(CMapToken* token, int count, int& fN)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CAwBuildApp *)AfxGetApp())->m_load.BuildForm(token, count, fN);
}
	
__declspec(dllexport) void APIENTRY buildResource(char* text, int line, int formN)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CAwBuildApp *)AfxGetApp())->m_load.BuildResource(text, line, formN);
}

__declspec(dllexport) void APIENTRY buildGraph(CMapToken* token, int count, int formN, int& grN)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//((CAwBuildApp *)AfxGetApp())->m_load->BuildGraph(token, count, formN, grN);
}

__declspec(dllexport) void APIENTRY buildTab(int idx)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	((CAwBuildApp *)AfxGetApp())->m_load.BuildTab(idx);
}
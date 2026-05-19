
#include "stdafx.h"
#include "libBuild.h"
#include "awBuild.h"

__declspec(dllexport) int APIENTRY generateSource(bool alarm, class mapForm* mapForm)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ((CawBuildApp *)AfxGetApp())->m_build.GenerateSource(alarm, mapForm);
}

__declspec(dllexport) CString APIENTRY getSource(bool bResource, class mapForm* mapForm)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ((CawBuildApp *)AfxGetApp())->m_build.GetSource(bResource, mapForm);
}

__declspec(dllexport) void APIENTRY load(class mapForm* mapForm, CString curdir)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CawBuildApp *)AfxGetApp())->m_load.Load(mapForm, curdir);
}

__declspec(dllexport) void APIENTRY buildTab(int idx)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	((CawBuildApp *)AfxGetApp())->m_load.BuildTab(idx);
}

__declspec(dllexport) void APIENTRY buildXmlHeader(CNode* pElementNode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CawBuildApp *)AfxGetApp())->m_load.BuildXmlHeader(pElementNode);
}

__declspec(dllexport) void APIENTRY buildXmlForm(CNode* pElementNode, CNode* pCellPropNode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CawBuildApp *)AfxGetApp())->m_load.BuildXmlForm(pElementNode, pCellPropNode);
}

__declspec(dllexport) void APIENTRY buildXmlResource(CNode* pElementNode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CawBuildApp *)AfxGetApp())->m_load.BuildXmlResource(pElementNode);
}

__declspec(dllexport) void APIENTRY BuildXmlTRlayout(CNode* pElementNode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CawBuildApp *)AfxGetApp())->m_load.BuildXmlTRlayout(pElementNode);
}

__declspec(dllexport) void APIENTRY BuildXmlLayoutEdit(CNode* pElementNode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CawBuildApp *)AfxGetApp())->m_load.BuildXmlLayoutEdit(pElementNode);
}

__declspec(dllexport) void APIENTRY BuildXmlRTS(CNode* pElementNode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CawBuildApp *)AfxGetApp())->m_load.BuildXmlRTS(pElementNode);
}
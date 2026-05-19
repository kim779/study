
#include "stdafx.h"
#include "libWcc.h"
#include "awWcc.h"

__declspec(dllexport) int APIENTRY wccLoadSource(CString path, CWnd* hwnd, CString srcData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ((CawWccApp *)AfxGetApp())->m_compile.LoadSource((char *)path.operator LPCTSTR(), hwnd, srcData);
}

__declspec(dllexport) bool APIENTRY wccmakeBinaryFile(CString strTmp, mapForm* mapH)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ((CawWccApp *)AfxGetApp())->m_compile.makeBinaryFile(strTmp, mapH);
}

__declspec(dllexport) void APIENTRY wccSetDominoForm()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CawWccApp *)AfxGetApp())->m_compile.SetDominoForm();
}

__declspec(dllexport) void APIENTRY wccSetCurrentDirectory(CString curdir)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CawWccApp *)AfxGetApp())->m_compile.SetCurrentDirectory(curdir);
}

__declspec(dllexport) void APIENTRY wccSetTempPath(CString& dir, char* fileN, bool source)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CawWccApp *)AfxGetApp())->m_compile.SetTempPath(dir, fileN, source);
}

__declspec(dllexport) void APIENTRY wccGetTempName(char* fileN, char* prefix)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CawWccApp *)AfxGetApp())->m_compile.GetTempName(fileN, prefix);
}

__declspec(dllexport) void APIENTRY wccGetXmlNode(CNode &pElementNode, CString ElementKind)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CawWccApp *)AfxGetApp())->m_compile.GetXmlNode(pElementNode, ElementKind);
}
__declspec(dllexport) int APIENTRY wccGetFormNode(CNode &pElementNode, int* idx)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ((CawWccApp *)AfxGetApp())->m_compile.GetFormNode(pElementNode, idx);
}

__declspec(dllexport) int APIENTRY wccGetCellNode(CNode &pElementNode, int* idx)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ((CawWccApp *)AfxGetApp())->m_compile.GetCellNode(pElementNode, idx);
}
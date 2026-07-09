
#include <stdafx.h>
#include "awwcc.h"

__declspec(dllexport) int APIENTRY wccAllocateMemory(int totL)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	OutputDebugString("[AWWCC][DEBUG] wccAllocateMemory called - awWcc.dll build 2026-07-07-1 (with scriptR/strR diagnostics)\n");

	return ((CAwWccApp *)AfxGetApp())->m_compile.mapMemoryAllocation(totL);
}

__declspec(dllexport) int APIENTRY wccParseToken(CMapToken *token, int line, WORD *type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ((CAwWccApp *)AfxGetApp())->m_compile.mapTokenParsing(token, line, type);
}

__declspec(dllexport) int APIENTRY wccBuildToken(CMapToken *token, int line, WORD *type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ((CAwWccApp *)AfxGetApp())->m_compile.mapTokenParsingBuilding(token, line, type);
}

__declspec(dllexport) void APIENTRY wccFreeMemory()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CAwWccApp *)AfxGetApp())->m_compile.mapMemoryFree();
}

__declspec(dllexport) int APIENTRY wccLoadSource(CString path, CWnd* hwnd, CString srcData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ((CAwWccApp *)AfxGetApp())->m_compile.mapSourceFileLoad((char *)path.operator LPCTSTR(), hwnd, srcData);
}

__declspec(dllexport) int APIENTRY wccBuildObject(CString path, CString nickN)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	OutputDebugString("[AWWCC][DEBUG] wccBuildObject called - awWcc.dll build 2026-07-07-1 (with scriptR/strR diagnostics)\n");

	return ((CAwWccApp *)AfxGetApp())->m_compile.mapBuildObjectFile((char *)path.operator LPCTSTR(), nickN);
}

__declspec(dllexport) void APIENTRY wccSetPath(CString& dir, char* fileN, bool source)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CAwWccApp *)AfxGetApp())->m_compile.SetMapFilePath(dir, fileN, source);
}

__declspec(dllexport) void APIENTRY wccSetTempPath(CString& dir, char* fileN, bool source)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CAwWccApp *)AfxGetApp())->m_compile.SetTempPath(dir, fileN, source);
}

__declspec(dllexport) int APIENTRY wccAnalyzeColumn(int tLine)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ((CAwWccApp *)AfxGetApp())->m_compile.mapColumnAnalyze(tLine);
}

__declspec(dllexport) char* APIENTRY wccParseResource(int cLine, WORD *Type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ((CAwWccApp *)AfxGetApp())->m_compile.mapResParsing(cLine, Type);
}

__declspec(dllexport) void APIENTRY wccSetDominoForm()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CAwWccApp *)AfxGetApp())->m_compile.setDominoForm();
}

__declspec(dllexport) void APIENTRY wccLastCompileWork()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CAwWccApp *)AfxGetApp())->m_compile.lastCompileWork();
}

__declspec(dllexport) void APIENTRY wccSetCurrentDirectory(CString curdir)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CAwWccApp *)AfxGetApp())->m_compile.setCurrentDirectory(curdir);
}

__declspec(dllexport) void APIENTRY wccToBUFFER(char* buf, int size, int value)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	char	format[16], edit[128];

	sprintf_s(format, "%%0%dd", size);
	sprintf_s(edit, format, value);
	CopyMemory(buf, edit, size);
}

__declspec(dllexport) int APIENTRY wccToNUMBER(char* buf, int size)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	tmps;

	tmps = CString(buf, size);
	return (atoi(tmps));
}

__declspec(dllexport) void APIENTRY wccGetTempName(char* fileN, char* prefix)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	((CAwWccApp *)AfxGetApp())->m_compile.getTempName(fileN, prefix);
}

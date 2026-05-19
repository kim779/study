
#pragma once

// CLibBuild 명령 대상입니다.

__declspec(dllexport) void APIENTRY setRoot(CString root);
__declspec(dllexport) int  APIENTRY generateSource(bool alarm, struct _mapH* mapH, bool getsource = false);
__declspec(dllexport) CString APIENTRY getSource(bool bNotResource, struct _mapH* mapH);
__declspec(dllexport) void APIENTRY load(struct _mapH* mapH, CString curdir);
__declspec(dllexport) void APIENTRY wccSetTempPath(CString& dir, char* fileN, bool source = true);
__declspec(dllexport) void APIENTRY SetPath(CString& dir, char* fileN, bool source = true);
__declspec(dllexport) void APIENTRY wccSetCurrentDirectory(CString curdir);
__declspec(dllexport) void APIENTRY wccGetTempName(char* fileN, char* prefix);
__declspec(dllexport) int APIENTRY getformcount();
__declspec(dllexport) void APIENTRY wccTrLayOut(int kind, CString tr = _T(""));
__declspec(dllexport) CString APIENTRY getTrLayOut();
__declspec(dllexport) void APIENTRY wccRTSSymbol(int kind, CString tr);
__declspec(dllexport) CString APIENTRY getRTSSymbol();
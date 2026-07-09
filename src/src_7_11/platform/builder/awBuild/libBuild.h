#ifndef	_LIBBUILD_H
#define	_LIBBUILD_H
#include "../awWcc/libWcc.h"

__declspec(dllexport) int  APIENTRY generateSource(bool alarm, struct _mapH* mapH);
__declspec(dllexport) CString  APIENTRY getSource(bool bResource, struct _mapH* mapH);
__declspec(dllexport) void APIENTRY load(struct _mapH* mapH, CString curdir);
__declspec(dllexport) void APIENTRY buildHeader(CMapToken* token, int count);
__declspec(dllexport) void APIENTRY buildForm(CMapToken* token, int count, int& fN);
__declspec(dllexport) void APIENTRY buildResource(char* text, int line, int formN);
__declspec(dllexport) void APIENTRY buildGraph(CMapToken* token, int count, int formN, int& grN);
__declspec(dllexport) void APIENTRY buildTab(int idx = 0);

#endif
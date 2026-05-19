
#pragma once

#include "../awWcc/libWcc.h"
#include "../h/mapxml.H"

__declspec(dllexport) int     APIENTRY generateSource(bool alarm, class mapForm* mapForm);
__declspec(dllexport) CString APIENTRY getSource(bool bResource, class mapForm* mapForm);
__declspec(dllexport) void    APIENTRY load(class mapForm* mapForm, CString curdir);
__declspec(dllexport) void    APIENTRY buildTab(int idx = 0);

__declspec(dllexport) void    APIENTRY buildXmlHeader(CNode* pElementNode);
__declspec(dllexport) void    APIENTRY buildXmlForm(CNode* pElementNode, CNode* pCellPropNode);
__declspec(dllexport) void    APIENTRY buildXmlResource(CNode* pElementNode);
__declspec(dllexport) void    APIENTRY BuildXmlTRlayout(CNode* pElementNode);
__declspec(dllexport) void    APIENTRY BuildXmlLayoutEdit(CNode* pElementNode);
__declspec(dllexport) void    APIENTRY BuildXmlRTS(CNode* pElementNode);
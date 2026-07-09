#ifndef	_LIBDLL_H
#define	_LIBDLL_H
#include "UnBond.h"
#if _MSC_VER > 1000
#pragma once
#endif

//__declspec(dllexport) bool WINAPI axGetCode(int kind, CString root, char* code, int type);
//__declspec(dllexport) bool WINAPI axGetName(int kind, CString root, CString user, CString code, char* name, int* type);

#endif	// _LIBDLL_H

CWnd* m_pwnd;
CUnBond* m_pdlg;

//------------------------------------------------------------------------------
// File    : MyTrace.h 
// Version : 1.0
// Date    : Mai 2002
// Author  : Bruno Podetti
// Email   : Podetti@gmx.net
//
// Systems : VC6.0 and VC7.0
// 
// Tracing windowsmessages. Copied and modified from the MFC part.
//------------------------------------------------------------------------------

#pragma once


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
#endif

void MyTraceMsg(LPCTSTR lpszPrefix, const MSG* pMsg);
void MyTraceMsg(LPCTSTR lpszPrefix, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


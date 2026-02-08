// cx_THtmlEx.h : main header file for the cx_THtmlEx DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"

/////////////////////////////////////////////////////////////////////////////
// Ccx_THtmlExApp
// See cx_THtmlEx.cpp for the implementation of this class
//

class Ccx_THtmlExApp : public CWinApp
{
public:
	Ccx_THtmlExApp();

	CFont*	GetFont(CWnd* pView, int point, CString name, int bold = 0, bool italic = false);
	CPen*	GetPen(CWnd* pView, int style, int width, COLORREF clr);
	CBrush*	GetBrush(CWnd* pView, COLORREF rColor);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Ccx_THtmlExApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(Ccx_THtmlExApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
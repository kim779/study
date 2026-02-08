// CX_FODepth.h : main header file for the CX_FODepth DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"

/////////////////////////////////////////////////////////////////////////////
// CCX_FODepthApp
// See CX_FODepth.cpp for the implementation of this class
//

class CCX_FODepthApp : public CWinApp
{
public:
	CCX_FODepthApp();

	COLORREF	GetColor(CWnd *pWizard, int color);
	CPen*		GetPen(CWnd *pWizard, int color, int width = 1, int style = PS_SOLID);
	CBrush*		GetBrush(CWnd* pWizard, COLORREF rColor);
	CFont*		GetFont(CWnd *pWizard, int point, CString name, int bold = 0, bool italic = false);
	CString		Parser(CString &srcstr, CString substr);
	void		VariableComma(CString &dispTxt, double dVal, int point, int maxDigit = 12);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_FODepthApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_FODepthApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
// CX_ContractX.h : main header file for the CX_CONTRACTX DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define	WM_SCRL		WM_USER+100	// scroll message
#define	SCRL_VCHANGE	1		// change vscroll
/////////////////////////////////////////////////////////////////////////////
// CCX_ContractXApp
// See CX_ContractX.cpp for the implementation of this class
//

class CCX_ContractXApp : public CWinApp
{
public:
	CCX_ContractXApp();

	COLORREF	GetColor(CWnd *pWizard, int color);
	CPen*		GetPen(CWnd *pWizard, int color, int width);
	CFont*		GetFont(CWnd *pWizard, int point, CString name, int bold, bool italic);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_ContractXApp)

public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_ContractXApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

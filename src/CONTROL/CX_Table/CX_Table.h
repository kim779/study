// CX_Table.h : main header file for the CX_TABLE DLL
//

#if !defined(AFX_CX_TABLE_H__D85CABD3_E7E0_4CFE_A73C_2A3B2C294C1B__INCLUDED_)
#define AFX_CX_TABLE_H__D85CABD3_E7E0_4CFE_A73C_2A3B2C294C1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_TableApp
// See CX_Table.cpp for the implementation of this class
//

class CCX_TableApp : public CWinApp
{
public:
	CCX_TableApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_TableApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_TableApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_TABLE_H__D85CABD3_E7E0_4CFE_A73C_2A3B2C294C1B__INCLUDED_)

// cx_codectrl.h : main header file for the cx_codectrl DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main cx_codectrls

/////////////////////////////////////////////////////////////////////////////
// Ccx_codectrlApp
// See cx_codectrl.cpp for the implementation of this class
//

class Ccx_codectrlApp : public CWinApp
{
public:
	Ccx_codectrlApp();
//	HINSTANCE m_hInstace;


private:
	void	loadDefaultResource();
	void	freeDefaultResource();

	HINSTANCE m_hInst;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Ccx_codectrlApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(Ccx_codectrlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
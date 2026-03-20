// cx_comboList.h : main header file for the cx_comboList DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main cx_comboLists

/////////////////////////////////////////////////////////////////////////////
// Ccx_comboListApp
// See cx_comboList.cpp for the implementation of this class
//

class Ccx_comboListApp : public CWinApp
{
public:
	Ccx_comboListApp();
//	HINSTANCE m_hInstace;


private:
	void	loadDefaultResource();
	void	freeDefaultResource();

	HINSTANCE m_hInst;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Ccx_comboListApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(Ccx_comboListApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
// IB100600.h : main header file for the IB100600 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB100600App
// See IB100600.cpp for the implementation of this class
//

class CIB100600App : public CWinApp
{
public:
	CIB100600App();

	BOOL			m_bChangeFont = FALSE;
	CMapStringToString	m_SaveMapList;
	void			SaveBuffer(CString sKey, CString sData);
	CString			GetBuffer(CString sKey);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB100600App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB100600App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



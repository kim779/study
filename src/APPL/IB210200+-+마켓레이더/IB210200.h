// IB210200.h : main header file for the IB210200 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB210200App
// See IB210200.cpp for the implementation of this class
//

class CIB210200App : public CWinApp
{
public:
	CIB210200App();
	
	void				SaveBuffer(CString sKey, CString sData);
	CString				GetBuffer(CString sKey);
	CMapStringToString	m_SaveMapList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB210200App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB210200App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



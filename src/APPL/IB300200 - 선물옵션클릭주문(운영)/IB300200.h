// IB300200.h : main header file for the IB300200 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB300200App
// See IB300200.cpp for the implementation of this class
//

class CIB300200App : public CWinApp
{
public:
	CIB300200App();
	CMapStringToString	m_SaveMapList;
	void			SaveBuffer(CString sKey, CString sData);
	CString			GetBuffer(CString sKey);
	void			DeleteKey(CString sKey);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB300200App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB300200App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



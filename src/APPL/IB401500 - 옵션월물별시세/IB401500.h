// IB401500.h : main header file for the IB401500 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB401500App
// See IB401500.cpp for the implementation of this class
//

class CIB401500App : public CWinApp
{
public:
	CIB401500App();
	CMapStringToString	m_SaveMapList;
	void			SaveBuffer(CString sKey, CString sData);
	CString			GetBuffer(CString sKey);
	void			DeleteKey(CString sKey);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB401500App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB401500App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



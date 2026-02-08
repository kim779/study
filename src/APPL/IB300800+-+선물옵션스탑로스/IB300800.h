// IB100400.h : main header file for the IB100400 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB100400App
// See IB100400.cpp for the implementation of this class
//

class CIB100400App : public CWinApp
{
public:
	CIB100400App();
	void DebugLog(CString strText) ;
	CMapStringToString	m_SaveMapList;
	void			SaveBuffer(CString sKey, CString sData);
	CString			GetBuffer(CString sKey);
	void			DeleteBufferKey(CString sKey);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB100400App)
	public:
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB100400App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



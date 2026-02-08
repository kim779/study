// IB210100.h : main header file for the IB210100 DLL
//


#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB210100App
// See IB210100.cpp for the implementation of this class
//

class CIB210100App : public CWinApp
{
public:
	CMapStringToString m_mapSaveList;
	CIB210100App();
	
	CString GetBuffer(CString sKey);
	void SaveBuffer(CString sKey, CString sData);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB210100App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB210100App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////


// SystemTradingZipper.h : main header file for the SYSTEMTRADINGZIPPER DLL
//

#if !defined(AFX_SYSTEMTRADINGZIPPER_H__A5137024_6535_4BBD_B6AD_6179FAFA0651__INCLUDED_)
#define AFX_SYSTEMTRADINGZIPPER_H__A5137024_6535_4BBD_B6AD_6179FAFA0651__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ZipLib/ZipArchive.h"

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingZipperApp
// See SystemTradingZipper.cpp for the implementation of this class
//

class CSystemTradingZipperApp : public CWinApp
{
public:
	CSystemTradingZipperApp();

	CZipArchive m_zip;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemTradingZipperApp)
	public:
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSystemTradingZipperApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTRADINGZIPPER_H__A5137024_6535_4BBD_B6AD_6179FAFA0651__INCLUDED_)

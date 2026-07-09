// axiscore.h : main header file for the AXISCORE application
//

#if !defined(AFX_AXISCORE_H__B77A1479_189F_4D03_878E_8A3D4ED695B2__INCLUDED_)
#define AFX_AXISCORE_H__B77A1479_189F_4D03_878E_8A3D4ED695B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#pragma comment (lib, "version.lib")

/////////////////////////////////////////////////////////////////////////////
// CAxiscoreApp:
// See axiscore.cpp for the implementation of this class
//

class CAxiscoreApp : public CWinApp
{
public:
	CAxiscoreApp();

protected:
	CString GetFileVersion(CString strPath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxiscoreApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
protected:
	CString	m_regKey;
// Implementation
	void	write_Regkey();
	//{{AFX_MSG(CAxiscoreApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISCORE_H__B77A1479_189F_4D03_878E_8A3D4ED695B2__INCLUDED_)

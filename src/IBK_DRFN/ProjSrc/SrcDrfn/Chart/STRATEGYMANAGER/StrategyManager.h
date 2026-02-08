// StrategyManager.h : main header file for the STRATEGYMANAGER DLL
//

#if !defined(AFX_STRATEGYMANAGER_H__13E7A578_D856_48CA_AB51_CC1A6900BA33__INCLUDED_)
#define AFX_STRATEGYMANAGER_H__13E7A578_D856_48CA_AB51_CC1A6900BA33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CStrategyManagerApp
// See StrategyManager.cpp for the implementation of this class
//

class CStrategyManagerApp : public CWinApp
{
public:
	CStrategyManagerApp();
	long GetStrategyItem();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStrategyManagerApp)
	public:
	virtual int ExitInstance();
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CStrategyManagerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRATEGYMANAGER_H__13E7A578_D856_48CA_AB51_CC1A6900BA33__INCLUDED_)

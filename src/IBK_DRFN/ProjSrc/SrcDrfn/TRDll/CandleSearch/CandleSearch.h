// CandleSearch.h : main header file for the CANDLESEARCH DLL
//

#if !defined(AFX_CANDLESEARCH_H__EE30B413_D320_43f7_A6C5_E16A5BB0E4E4__INCLUDED_)
#define AFX_CANDLESEARCH_H__EE30B413_D320_43f7_A6C5_E16A5BB0E4E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCandleSearchApp
// See CandleSearch.cpp for the implementation of this class
//

class CCandleSearchApp : public CWinApp
{
public:
	CCandleSearchApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCandleSearchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCandleSearchApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANDLESEARCH_H__EE30B413_D320_43f7_A6C5_E16A5BB0E4E4__INCLUDED_)

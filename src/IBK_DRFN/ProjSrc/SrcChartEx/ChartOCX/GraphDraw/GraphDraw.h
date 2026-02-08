// GraphDraw.h : main header file for the GRAPHDRAW DLL
//

#if !defined(AFX_GRAPHDRAW_H__8B870559_E077_47F0_8CBA_F94499985AF2__INCLUDED_)
#define AFX_GRAPHDRAW_H__8B870559_E077_47F0_8CBA_F94499985AF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGraphDrawApp
// See GraphDraw.cpp for the implementation of this class
//

class CGraphDrawApp : public CWinApp
{
public:
	CGraphDrawApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphDrawApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGraphDrawApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHDRAW_H__8B870559_E077_47F0_8CBA_F94499985AF2__INCLUDED_)

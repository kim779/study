// CX_MarketPicker.h : main header file for the CX_MARKETPICKER DLL
//

#if !defined(AFX_CX_MARKETPICKER_H__7205BB6C_5C24_4901_BF6D_A342822E9DB1__INCLUDED_)
#define AFX_CX_MARKETPICKER_H__7205BB6C_5C24_4901_BF6D_A342822E9DB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_MarketPickerApp
// See CX_MarketPicker.cpp for the implementation of this class
//

class CCX_MarketPickerApp : public CWinApp
{
public:
	CCX_MarketPickerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_MarketPickerApp)
	public:
	virtual BOOL InitInstance();
	CWnd* Get_MainWnd() {
		return m_pMainWnd;
	}
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_MarketPickerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_MARKETPICKER_H__7205BB6C_5C24_4901_BF6D_A342822E9DB1__INCLUDED_)

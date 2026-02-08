// CX_FundPrinter.h : main header file for the CX_FUNDPRINTER DLL
//

#if !defined(AFX_CX_FUNDPRINTER_H__14E2EB56_D695_485D_A90A_9DF5E75B8095__INCLUDED_)
#define AFX_CX_FUNDPRINTER_H__14E2EB56_D695_485D_A90A_9DF5E75B8095__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"
#include "../../h/axiscgrp.h"


/////////////////////////////////////////////////////////////////////////////
// CCX_FundPrinterApp
// See CX_FundPrinter.cpp for the implementation of this class
//

class CCX_FundPrinterApp : public CWinApp
{
public:
	CCX_FundPrinterApp();

private:
	void	loadDefaultResource();
	void	freeDefaultResource();

	HINSTANCE m_hInst;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_FundPrinterApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_FundPrinterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//ULONG_PTR m_gdiplusToken;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_FUNDPRINTER_H__14E2EB56_D695_485D_A90A_9DF5E75B8095__INCLUDED_)

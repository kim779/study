// SecCalc.h : main header file for the SECCALC application
//

#if !defined(AFX_SECCALC_H__32C6EFE4_EE33_4D1F_B7F2_E602DB5C26F4__INCLUDED_)
#define AFX_SECCALC_H__32C6EFE4_EE33_4D1F_B7F2_E602DB5C26F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <math.h>
#include "SecCalcDlg.h"


#define	Fee_Stock	_T("0.1")
#define	Fee_Future	_T("0.01")
#define	Fee_Option	_T("0.5")
#define	Fee_Elw		_T("0.1")
//#define	Fee_Tax		_T("0.003") //test
//#define	Fee_Tax		_T("0.0018")  //20240102
//#define	Fee_Tax		_T("0.0015")  //20241226
#define	Fee_Tax		_T("0.002")  //20251226

enum { ok_None = -1, ok_Buy, ok_Sell};
enum { mk_None = -1, mk_Stock, mk_Future, mk_Option, mk_Elw};
enum { ck_None = 0, ck_Char, ck_Plus, ck_Minus, ck_Mul, ck_Div};

#define	SN_FeeRate	_T("수수료율")

/////////////////////////////////////////////////////////////////////////////
// CSecCalcApp:
// See SecCalc.cpp for the implementation of this class
//

class CSecCalcApp : public CWinApp
{
public:
	CSecCalcApp();
	void	LoadInfo();
	void	SaveInfo();
	CString	editComma(double dVal, bool bInt = true);
	double	CalcBuyCharge(int market, double dCnt, double dPrice, double &dFee, double &dTax);
	double	CalcSellCharge(int market, double dCnt, double dPrice, double &dFee, double &dTax);
	void	CorrectString(CWnd *pWnd, int market = mk_Stock);

	CString	m_pFeeRate[4];
	CString	m_pCharFee[4];
	CString	m_sKey[4];
	CString	m_sPath;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSecCalcApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSecCalcApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECCALC_H__32C6EFE4_EE33_4D1F_B7F2_E602DB5C26F4__INCLUDED_)

// ChartDesignPropertyAddIn.h : main header file for the CHARTDESIGNPROPERTYADDIN DLL
//

#if !defined(AFX_CHARTDESIGNPROPERTYADDIN_H__14B088ED_8D6A_447A_8866_6288CD617FB2__INCLUDED_)
#define AFX_CHARTDESIGNPROPERTYADDIN_H__14B088ED_8D6A_447A_8866_6288CD617FB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// (2004.10.07, 배승원) AddIn DLL의 Interface를 정의하기 위한 Export Type을 정의한다.
#ifdef ADDIN_EXPORTS
#define ADDIN_API extern "C" __declspec(dllexport)
#else
#define ADDIN_API extern "C" __declspec(dllimport)
#endif

#include "resource.h"		// main symbols
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable

static const int GRAPHTYPE_ITEM = 8;
static IMetaTable::E_METACODE pGraphTypeItem[ GRAPHTYPE_ITEM] =
{
	IMetaTable::C5_CANDLE_TYPE,
	IMetaTable::C5_LINE_TYPE,
	IMetaTable::C5_BAR_TYPE,
	IMetaTable::C5_PIE_TYPE,
	IMetaTable::C5_SPECIAL_TYPE,
	IMetaTable::C5_FUND_TYPE,
	IMetaTable::C5_DATA_EXPRESSION_TYPE,
	IMetaTable::C5_FUTURE_OPTION_TYPE,
};

/////////////////////////////////////////////////////////////////////////////
// CChartDesignPropertyAddInApp
// See ChartDesignPropertyAddIn.cpp for the implementation of this class
//

class CChartDesignPropertyAddInApp : public CWinApp
{
public:
	CChartDesignPropertyAddInApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartDesignPropertyAddInApp)
	public:
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChartDesignPropertyAddInApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTDESIGNPROPERTYADDIN_H__14B088ED_8D6A_447A_8866_6288CD617FB2__INCLUDED_)

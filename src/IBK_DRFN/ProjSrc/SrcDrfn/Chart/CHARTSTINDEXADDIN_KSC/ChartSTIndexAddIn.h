// ChartSTIndexAddIn.h : main header file for the CHARTSTINDEXADDIN DLL
//

#if !defined(AFX_CHARTSTINDEXADDIN_H__FA12ABC7_F7E7_46DE_A991_A13EDF82A8AE__INCLUDED_)
#define AFX_CHARTSTINDEXADDIN_H__FA12ABC7_F7E7_46DE_A991_A13EDF82A8AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CChartSTIndexAddInApp
// See ChartSTIndexAddIn.cpp for the implementation of this class
//

class CChartSTIndexAddInApp : public CWinApp
{
public:
	CChartSTIndexAddInApp();
	BOOL IsInitedMapIndex();
	BOOL InitMapIndex(LPCTSTR lpMainPath);
	CString GetIndexFile(LPCTSTR lpIndexName, BOOL &rBHasPriceInput, CString& rStrType, CString& rStrScaleType);
	void GetCodeName(UINT nMarket, LPCTSTR lpszCode, LPSTR lpszName, size_t nSize);

private:
	CMapStringToString m_mapIndex;
	CMapStringToString m_mapIndexNoPriceInput;
	CMapStringToString m_mapIndexType;
	CMapStringToString m_mapIndexScaleType;
	BOOL m_bIsInitedMapIndex;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartSTIndexAddInApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChartSTIndexAddInApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTSTINDEXADDIN_H__FA12ABC7_F7E7_46DE_A991_A13EDF82A8AE__INCLUDED_)

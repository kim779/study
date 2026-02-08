// ChartObject.h : main header file for the CHARTOBJECT DLL
//

#if !defined(AFX_CHARTOBJECT_H__4A3175BA_24AE_48EE_A506_0E9CFC805ECB__INCLUDED_)
#define AFX_CHARTOBJECT_H__4A3175BA_24AE_48EE_A506_0E9CFC805ECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

// (2004.10.07, 배승원) AddIn DLL의 Interface를 정의하기 위한 Export Type을 정의한다.
#ifdef CHART_OBJECT_EXPORT
#define CHART_OBJECT_API extern "C" __declspec(dllexport)
#else
#define CHART_OBJECT_API extern "C" __declspec(dllimport)
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartObjectApp
// See ChartObject.cpp for the implementation of this class
//

class CChartObjectApp : public CWinApp
{
public:
	CChartObjectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartObjectApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChartObjectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTOBJECT_H__4A3175BA_24AE_48EE_A506_0E9CFC805ECB__INCLUDED_)

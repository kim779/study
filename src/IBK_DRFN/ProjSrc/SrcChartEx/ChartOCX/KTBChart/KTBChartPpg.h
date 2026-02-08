#if !defined(AFX_KTBCHARTPPG_H__E0E39FED_766F_4A17_9AE5_7C4B57E6EC16__INCLUDED_)
#define AFX_KTBCHARTPPG_H__E0E39FED_766F_4A17_9AE5_7C4B57E6EC16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// KTBChartPpg.h : Declaration of the CKTBChartPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CKTBChartPropPage : See KTBChartPpg.cpp.cpp for implementation.

class CKTBChartPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CKTBChartPropPage)
	DECLARE_OLECREATE_EX(CKTBChartPropPage)

// Constructor
public:
	CKTBChartPropPage();

// Dialog Data
	//{{AFX_DATA(CKTBChartPropPage)
	enum { IDD = IDD_PROPPAGE_KTBCHART };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CKTBChartPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KTBCHARTPPG_H__E0E39FED_766F_4A17_9AE5_7C4B57E6EC16__INCLUDED)

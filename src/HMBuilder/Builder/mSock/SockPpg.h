#if !defined(AFX_SOCKPPG_H__AE718E7D_83BA_4BFB_8BE7_2AFE0CC835B9__INCLUDED_)
#define AFX_SOCKPPG_H__AE718E7D_83BA_4BFB_8BE7_2AFE0CC835B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SockPpg.h : Declaration of the CSockPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSockPropPage : See SockPpg.cpp.cpp for implementation.

class CSockPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSockPropPage)
	DECLARE_OLECREATE_EX(CSockPropPage)

// Constructor
public:
	CSockPropPage();

// Dialog Data
	//{{AFX_DATA(CSockPropPage)
	enum { IDD = IDD_PROPPAGE_SOCK };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSockPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKPPG_H__AE718E7D_83BA_4BFB_8BE7_2AFE0CC835B9__INCLUDED)

#if !defined(AFX_XECUREPPG_H__ED7BB28A_5FC1_4FA0_891D_AEED6DCD4F15__INCLUDED_)
#define AFX_XECUREPPG_H__ED7BB28A_5FC1_4FA0_891D_AEED6DCD4F15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// XecurePpg.h : Declaration of the CXecurePropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CXecurePropPage : See XecurePpg.cpp.cpp for implementation.

class CXecurePropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CXecurePropPage)
	DECLARE_OLECREATE_EX(CXecurePropPage)

// Constructor
public:
	CXecurePropPage();

// Dialog Data
	//{{AFX_DATA(CXecurePropPage)
	enum { IDD = IDD_PROPPAGE_XECURE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CXecurePropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XECUREPPG_H__ED7BB28A_5FC1_4FA0_891D_AEED6DCD4F15__INCLUDED)

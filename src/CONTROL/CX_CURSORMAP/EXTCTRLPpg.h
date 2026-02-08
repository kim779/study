#pragma once

// EXTCTRLPpg.h : Declaration of the CEXTCTRLPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CEXTCTRLPropPage : See EXTCTRLPpg.cpp.cpp for implementation.

class CEXTCTRLPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CEXTCTRLPropPage)
	DECLARE_OLECREATE_EX(CEXTCTRLPropPage)

// Constructor
public:
	CEXTCTRLPropPage();

// Dialog Data
	//{{AFX_DATA(CEXTCTRLPropPage)
	enum { IDD = IDD_PROPPAGE_EXTCTRL };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CEXTCTRLPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
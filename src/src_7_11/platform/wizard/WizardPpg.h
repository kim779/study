#if !defined(AFX_WIZARDPPG_H__39515BE8_52ED_4C87_8E7F_D452037D6DF9__INCLUDED_)
#define AFX_WIZARDPPG_H__39515BE8_52ED_4C87_8E7F_D452037D6DF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// WizardPpg.h : Declaration of the CWizardPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CWizardPropPage : See WizardPpg.cpp.cpp for implementation.

class CWizardPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CWizardPropPage)
	DECLARE_OLECREATE_EX(CWizardPropPage)

// Constructor
public:
	CWizardPropPage();

// Dialog Data
	//{{AFX_DATA(CWizardPropPage)
	enum { IDD = IDD_PROPPAGE_WIZARD };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CWizardPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDPPG_H__39515BE8_52ED_4C87_8E7F_D452037D6DF9__INCLUDED)

#if !defined(AFX_IBKSCONNECTORPPG_H__9F31C0E0_B0C2_43F7_A74F_D4BF6B5BBCE3__INCLUDED_)
#define AFX_IBKSCONNECTORPPG_H__9F31C0E0_B0C2_43F7_A74F_D4BF6B5BBCE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// IBKSConnectorPpg.h : Declaration of the CIBKSConnectorPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CIBKSConnectorPropPage : See IBKSConnectorPpg.cpp.cpp for implementation.

class CIBKSConnectorPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CIBKSConnectorPropPage)
	DECLARE_OLECREATE_EX(CIBKSConnectorPropPage)

// Constructor
public:
	CIBKSConnectorPropPage();

// Dialog Data
	//{{AFX_DATA(CIBKSConnectorPropPage)
	enum { IDD = IDD_PROPPAGE_IBKSCONNECTOR };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CIBKSConnectorPropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IBKSCONNECTORPPG_H__9F31C0E0_B0C2_43F7_A74F_D4BF6B5BBCE3__INCLUDED)

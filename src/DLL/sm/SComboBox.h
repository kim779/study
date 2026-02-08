#if !defined(AFX_SCOMBOBOX_H__9E6F0AD1_7FBA_43C0_849C_12A80D75581E__INCLUDED_)
#define AFX_SCOMBOBOX_H__9E6F0AD1_7FBA_43C0_849C_12A80D75581E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSComboBox window

class CSComboBox : public CComboBox
{
// Construction
public:
	CSComboBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSComboBox)
	afx_msg void OnDropdown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCOMBOBOX_H__9E6F0AD1_7FBA_43C0_849C_12A80D75581E__INCLUDED_)

#pragma once

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
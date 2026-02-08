#pragma once
// fontSizeCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CfontSizeCombo window

class CfontSizeCombo : public CComboBox
{
// Construction
public:
	CfontSizeCombo();

// Attributes
public:

// Operations
public:
	int	AddSize(int point, LONG height);
	LONG	GetHeight(int sel = -1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfontSizeCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfontSizeCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfontSizeCombo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


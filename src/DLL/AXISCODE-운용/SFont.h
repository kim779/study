#pragma once
// SFont.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CSFont command target

class CSFont : public CFont
{
// Attributes
public:

// Operations
public:
	CSFont();
	virtual ~CSFont();

// Overrides
public:
	BOOL CreateFont( int nHeight, LPCTSTR lpszFacename , int nWeight = 0);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSFont)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSFont)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};


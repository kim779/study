#if !defined(AFX_SFONT_H__E74DA51D_9B49_49BB_B46F_DB98DD3F366B__INCLUDED_)
#define AFX_SFONT_H__E74DA51D_9B49_49BB_B46F_DB98DD3F366B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SFONT_H__E74DA51D_9B49_49BB_B46F_DB98DD3F366B__INCLUDED_)

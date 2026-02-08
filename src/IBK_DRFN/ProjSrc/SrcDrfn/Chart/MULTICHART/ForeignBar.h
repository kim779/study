#if !defined(AFX_FOREIGNBAR_H__6A06184A_5E04_4EE7_A665_B865EC826CE5__INCLUDED_)
#define AFX_FOREIGNBAR_H__6A06184A_5E04_4EE7_A665_B865EC826CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ForeignBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CForeignBar dialog

class CForeignBar : public CDialog
{
// Construction
public:
	CForeignBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CForeignBar)
	enum { IDD = IDD_FOREIGNBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CForeignBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CForeignBar)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOREIGNBAR_H__6A06184A_5E04_4EE7_A665_B865EC826CE5__INCLUDED_)

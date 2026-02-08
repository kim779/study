#if !defined(AFX_TOTALACCOUNT_H__AAF6B192_116D_4E3E_8660_C08C5085BC4A__INCLUDED_)
#define AFX_TOTALACCOUNT_H__AAF6B192_116D_4E3E_8660_C08C5085BC4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TotalAccount.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTotalAccount dialog

class CTotalAccount : public CDialog
{
// Construction
public:
	CTotalAccount(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTotalAccount)
	enum { IDD = IDC_INFOLIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTotalAccount)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTotalAccount)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOTALACCOUNT_H__AAF6B192_116D_4E3E_8660_C08C5085BC4A__INCLUDED_)

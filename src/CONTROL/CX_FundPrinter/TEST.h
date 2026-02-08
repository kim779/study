#if !defined(AFX_TEST_H__7AD0AB5F_5CBE_4B3C_AA08_1A11186125C0__INCLUDED_)
#define AFX_TEST_H__7AD0AB5F_5CBE_4B3C_AA08_1A11186125C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TEST.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TEST dialog

class TEST : public CDialog
{
// Construction
public:
	TEST(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TEST)
	enum { IDD = IDD_DIALOG3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TEST)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TEST)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_H__7AD0AB5F_5CBE_4B3C_AA08_1A11186125C0__INCLUDED_)

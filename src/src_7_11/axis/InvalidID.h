#if !defined(AFX_INVALIDID_H__19251EC7_22F4_474A_B2E1_6948CC771B57__INCLUDED_)
#define AFX_INVALIDID_H__19251EC7_22F4_474A_B2E1_6948CC771B57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InvalidID.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInvalidID dialog

class CInvalidID : public CDialog
{
// Construction
public:
	CInvalidID(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInvalidID)
	enum { IDD = IDD_INVALIDID };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInvalidID)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInvalidID)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INVALIDID_H__19251EC7_22F4_474A_B2E1_6948CC771B57__INCLUDED_)

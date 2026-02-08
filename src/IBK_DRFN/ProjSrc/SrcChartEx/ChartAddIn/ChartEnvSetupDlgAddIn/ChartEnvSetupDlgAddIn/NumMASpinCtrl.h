#if !defined(AFX_NUMMASPINCTRL_H__DCC97D02_75F6_4D1C_8845_0C3068E2050F__INCLUDED_)
#define AFX_NUMMASPINCTRL_H__DCC97D02_75F6_4D1C_8845_0C3068E2050F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NumMASpinCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNumMASpinCtrl window

class CNumMASpinCtrl : public CSpinButtonCtrl
{
// Construction
public:
	CNumMASpinCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumMASpinCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNumMASpinCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNumMASpinCtrl)
	afx_msg void OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMMASPINCTRL_H__DCC97D02_75F6_4D1C_8845_0C3068E2050F__INCLUDED_)

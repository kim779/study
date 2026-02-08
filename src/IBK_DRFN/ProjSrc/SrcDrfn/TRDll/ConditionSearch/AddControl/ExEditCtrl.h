#if !defined(AFX_EXEDITCTRL_H__4E21DEF9_3F7E_4F4D_A59B_E351A0A321C5__INCLUDED_)
#define AFX_EXEDITCTRL_H__4E21DEF9_3F7E_4F4D_A59B_E351A0A321C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExEditCtrl window

class CExEditCtrl : public CEdit
{
// Construction
public:
	CExEditCtrl();
	virtual ~CExEditCtrl();

// Attributes
public:

	int m_nMaxVal;
	CSpinButtonCtrl		*m_pbtnSpin;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExEditCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetEditVal(UINT nChar);
	

	// Generated message map functions
protected:
	//{{AFX_MSG(CExEditCtrl)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillfocus();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXEDITCTRL_H__4E21DEF9_3F7E_4F4D_A59B_E351A0A321C5__INCLUDED_)

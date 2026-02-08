#if !defined(AFX_INDICATORGROUPBUTTON_H__5C25FBDD_0881_427A_AF85_E9F28CCE5997__INCLUDED_)
#define AFX_INDICATORGROUPBUTTON_H__5C25FBDD_0881_427A_AF85_E9F28CCE5997__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndicatorGroupButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIndicatorGroupButton window

class CGroupButtonCtrl;
class CIndicatorGroupButton : public CButton
{
// Construction
public:
	CIndicatorGroupButton();
	CIndicatorGroupButton( CGroupButtonCtrl *pGroupButtonCtrl, int nIndex );
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndicatorGroupButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIndicatorGroupButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIndicatorGroupButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CGroupButtonCtrl* m_pGroupButtonCtrl;
	int m_nIndex;

public:
	int GetIndex(){ return m_nIndex; };
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDICATORGROUPBUTTON_H__5C25FBDD_0881_427A_AF85_E9F28CCE5997__INCLUDED_)

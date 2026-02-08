#if !defined(AFX_BMPBUTTON_H__8968392E_8FC5_4A8F_AE0F_4846394D367E__INCLUDED_)
#define AFX_BMPBUTTON_H__8968392E_8FC5_4A8F_AE0F_4846394D367E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBmpButton window

class CBmpButton : public CWnd
{
// Construction
public:
	CBmpButton(CWnd *pView);

// Attributes
public:
	CWnd	*m_pView;
	bool	m_bLBDN;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBmpButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBmpButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPBUTTON_H__8968392E_8FC5_4A8F_AE0F_4846394D367E__INCLUDED_)

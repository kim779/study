#pragma once
// EditX.h : header file
//

#define	WM_EDITX	(WM_USER + 100)

#define	wpBLANK		1
#define	wpENTER		2
#define	wpSEARCH1	3
#define	wpSEARCH2	4
#define wpDown		10
#define	wpBTNREDRAW		7  //0422
#define	wpBTNCLEAR	8  //0422

/////////////////////////////////////////////////////////////////////////////
// CEditX window

class CEditX : public CEdit
{
// Construction
public:
	CEditX();
	bool	m_bKeyDown;
	bool m_bTracking{};
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditX)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditX();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditX)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


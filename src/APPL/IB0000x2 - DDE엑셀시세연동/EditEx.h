#pragma once
// EditEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditEx window
#define	WM_EDITEX	(WM_USER + 100)

#define	wpBLANK		1
#define	wpENTER		2
#define	wpSEARCH1	3
#define	wpSEARCH2	4
#define wpDown		10

class CEditEx : public CEdit
{
// Construction
public:
	CEditEx();
	bool	m_bKeyDown;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditEx)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


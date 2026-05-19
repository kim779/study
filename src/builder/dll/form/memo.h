#if !defined(AFX_MEMO_H__FACD855A_DE5B_4CC8_B556_49C061FDFBE4__INCLUDED_)
#define AFX_MEMO_H__FACD855A_DE5B_4CC8_B556_49C061FDFBE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// memo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Cmemo window

class Cmemo : public CEdit
{
// Construction
public:
	Cmemo(CWnd* view, class CfmEdit* edit);
	virtual	~Cmemo();

// Attributes
public:

protected:
	CWnd*	m_view;
	CFont*	m_font;
	bool	m_readOnly;
	CfmEdit* m_edit;

	COLORREF m_pRGB;
	COLORREF m_tRGB;
	CBrush	m_brBACK;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cmemo)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	SetFont(const LOGFONT* logFont = NULL);
	void	SetColor(COLORREF pRGB = PALETTERGB(255,255,255), COLORREF tRGB = PALETTERGB(0,0,0));
	void	SetReadOnly(bool readOnly = true);

	// Generated message map functions
protected:
	//{{AFX_MSG(Cmemo)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMO_H__FACD855A_DE5B_4CC8_B556_49C061FDFBE4__INCLUDED_)

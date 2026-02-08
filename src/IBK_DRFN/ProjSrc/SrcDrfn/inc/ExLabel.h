// ExLabel.h: interface for the CExLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXLABEL_H__6E97B59C_FAF9_4abf_8B63_311BC65161E2__INCLUDED_)
#define AFX_EXLABEL_H__6E97B59C_FAF9_4abf_8B63_311BC65161E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoaderLabel;
class CExLabel
{
// Construction
public:
	CExLabel(BOOL bUseDefSkin = FALSE);

// Attributes
protected:
	CLoaderLabel*		m_pLoader;
	CDialog*			m_pParent;

// Operations
public:
	void	MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	void	MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
	BOOL	SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExLabel)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExLabel)
	afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_EXLABEL_H__6E97B59C_FAF9_4abf_8B63_311BC65161E2__INCLUDED_)

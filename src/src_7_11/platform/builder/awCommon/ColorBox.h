#if !defined(AFX_COLORBOX_H__D3AE34CE_A57D_4ED1_9FE5_B2A4BAA3B2CF__INCLUDED_)
#define AFX_COLORBOX_H__D3AE34CE_A57D_4ED1_9FE5_B2A4BAA3B2CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBox.h : header file
//

#define	userCOLOR	0x02000000

/////////////////////////////////////////////////////////////////////////////
// CColorBox window

class AFX_EXT_CLASS CColorBox : public CWnd
{
// Construction
public:
	CColorBox(CWnd* pParent, DWORD color, bool user =true, int offsetClr = 0);

// Attributes
public:
	DWORD	m_color;
	//class	CAxisDraw*	m_draw;
	//class	CAxisPalette*	m_palette;

protected:
	CString		m_path;
	bool		m_user;
	int		m_offsetClr;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorBox();

	// Generated message map functions
protected:
	void saveUserColor(COLORREF* ccolor);
	void loadUserColor(COLORREF* ccolor);
	void onUser();
	CRect makeRect(int row = -1, int col = -1);
	//{{AFX_MSG(CColorBox)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBOX_H__D3AE34CE_A57D_4ED1_9FE5_B2A4BAA3B2CF__INCLUDED_)

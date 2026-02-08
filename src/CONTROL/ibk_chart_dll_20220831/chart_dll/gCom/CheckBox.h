#if !defined(AFX_CHECKBOX_H__F0DBFB70_8D5A_4C05_9E9C_D3F6B286FECE__INCLUDED_)
#define AFX_CHECKBOX_H__F0DBFB70_8D5A_4C05_9E9C_D3F6B286FECE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckBox window

class CCheckBox : public CButton
{
// Construction
public:
	CCheckBox();

// Attributes
public:

protected:
	bool		m_bCreateBrush;
	bool		m_bCreateFont;
	CBrush*		m_pbrush;
	CFont*		m_pfont;

// Operations
public:
	void	SetColorBG(COLORREF rgbBG, bool bRefresh = true);
	void	SetFont(int iPoint, CString strFontName, bool bRefresh = true);

protected:
	int	Init();


public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckBox)
	public:
	virtual BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CFont* pFont = NULL, CBrush* pbrush = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCheckBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCheckBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKBOX_H__F0DBFB70_8D5A_4C05_9E9C_D3F6B286FECE__INCLUDED_)

/*
	Class CCTButton Version History

	v1.002		2005.12.01		Change CCheckBox()
						Change Create() 
	v1.001		2005.11.28		Change CCheckBox()
						Change Create()
	v1.000		2005.7.6		Class Defined
*/
#if !defined(AFX_COLORBTN_H__B0782653_8734_48FD_B873_0A396B949F1A__INCLUDED_)
#define AFX_COLORBTN_H__B0782653_8734_48FD_B873_0A396B949F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorBtn window

class CColorBtn : public CStatic
{
// Construction
public:
	CColorBtn();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorBtn)
	//}}AFX_VIRTUAL

public:
	COLORREF	GetColor();
	void		SetColor(COLORREF Color);

public:
	COLORREF	m_bColor;
	COLORREF	m_bDef;
	//CButton*	m_btn;
	std::unique_ptr <CButton> m_btn;
	
// Implementation
public:
	virtual ~CColorBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorBtn)
	afx_msg void OnPaint();
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBTN_H__B0782653_8734_48FD_B873_0A396B949F1A__INCLUDED_)

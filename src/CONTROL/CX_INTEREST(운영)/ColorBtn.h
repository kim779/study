#pragma once
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
	std::unique_ptr<CButton>	m_btn;
	
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
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
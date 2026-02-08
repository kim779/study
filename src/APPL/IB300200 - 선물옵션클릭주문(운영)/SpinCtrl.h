#pragma once
// SpinCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpinCtrl window

class CSpinCtrl : public CSpinButtonCtrl
{
// Construction
public:
	CSpinCtrl();

// Attributes
public:
	
public:
	void	SetAllAccel(int iAccel);
	void	SetFontFromParent(CFont* pfont);

protected:
	gsl::owner<CFont*>	m_pfont;
	BOOL	m_bCreateFont;

protected:
	int	Initialize();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpinCtrl)
	public:
	virtual BOOL Create(	DWORD dwStyle,
				const RECT& rect,
				CWnd* pParentWnd,
				UINT nID,
				CFont* pfont = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpinCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSpinCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


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
	BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CFont* pfont = NULL);
	void	SetAllAccel(int iAccel);
	void	SetFontFromParent(CFont* pfont);
	void	SetFloatType(CEdit* peditBuddy, double dMin, double dMax, double dDelta);
	void	SetFutureOptionType(CEdit* peditBuddy);
	void	SetNaturalType(CEdit* peditBuddy, int iMin, int iMax, int iDelta = 1);

	void	SetDelta(int iDelta);
	void	SetDelta(double dDelta);

protected:
	CFont*	m_pfont;
	BOOL	m_bCreateFont;

	CWnd*	m_peditBuddy;
	double	m_dMin;
	double	m_dMax;
	double	m_dDelta;
	int	m_iMin;
	int	m_iMax;
	int	m_iDelta;

	enum	enumType {	T_NORMAL,
				T_NATURAL,
				T_FLOAT,
				T_FO		// Future, Option
	}	m_enumType;

protected:
	int	Initialize();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpinCtrl)
	public:
//	virtual BOOL Create(	DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CFont* pfont = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpinCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSpinCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/*
	Class CSpinCtrl Version History

	v1.001		2006.02.10		Add	SetDelta();
	v1.000		2005.07.08		Class Define

*/
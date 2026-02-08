#pragma once
// ColorButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorButton window

class CColorButton : public CButton
{
// Construction
public:
	CColorButton();

// Attributes
public:

private:
	COLORREF	m_fg{};
	COLORREF	m_bg{};
	COLORREF	m_disabled{};
	COLORREF	m_gradient{};
	UINT		m_bevel{}; //버튼의 두께 :default 2
	bool		m_bDrawBong{};
	bool		m_bGradient{};
	bool		m_bFill{};

// Operations
public:
	void	SetNewColor(COLORREF bg = PALETTERGB(192, 192, 192),
		COLORREF fg = PALETTERGB(1, 1, 1), 
		COLORREF DisabledColor = PALETTERGB(128, 128, 128));
	BOOL	Attach	(const UINT nID, CWnd* pParent, 
			const COLORREF BGColor = PALETTERGB(192, 192, 192),		// gray button
			const COLORREF FGColor = PALETTERGB(1, 1, 1),				// black text 
			const COLORREF DisabledColor = PALETTERGB(128, 128, 128),	// dark gray disabled text
			const UINT nBevel = 2
			);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorButton)
	//}}AFX_VIRTUAL

protected:
	COLORREF	GetFGColor() { return m_fg; }	
	COLORREF	GetBGColor() { return m_bg; }
	COLORREF	GetDisabledColor() { return m_disabled; }
	UINT		GetBevel() { return m_bevel; }
	void		DrawFrame(CDC *DC, CRect R, int Inset);
	void		DrawFilledRect(CDC *DC, CRect R, COLORREF color);
	void		DrawLine(CDC *DC, CRect EndPoints, COLORREF color);
	void		DrawLine(CDC *DC, long left, long top,
			long right, long bottom, COLORREF color);
	void		DrawButtonText(CDC *DC, CRect R, 
			const char *Buf, COLORREF TextColor);
	
// Implementation
public:
	virtual ~CColorButton();

	// Generated message map functions
protected:
	virtual void	DrawItem(LPDRAWITEMSTRUCT lpDIS); //override function
	//{{AFX_MSG(CColorButton)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
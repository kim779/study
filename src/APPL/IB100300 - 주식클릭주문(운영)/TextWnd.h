#pragma once
// TextWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextWnd window

class CTextWnd : public CWnd
{
// Construction
public: 
	CTextWnd();

// Attributes
public:

protected:
//	// text
	CString		m_strText;

	// color
	COLORREF	m_rgbBG{};
	COLORREF	m_rgbFG{};
	COLORREF	m_rgbBorder{};

	// font
	std::unique_ptr<CFont>	m_pfontLocal = NULL;
	CFont *		m_pfont = NULL;
	BOOL		m_bCreateFont;

	// attribute
	DWORD		m_dwAlign;

	// image
	std::unique_ptr<CBitmap>	m_pbmpBG;
	CRect		m_rectI;

public:
	BOOL	SetFont(int iPoint, CString strFontName);
	void	SetFontFromParent(CFont* pfont);

	void	SetColor(COLORREF rgbBG, COLORREF rgbFG, COLORREF rgbBorder);
	void	SetColorBG(COLORREF rgbBG);
	void	SetColorFG(COLORREF rgbFG);

	void	SetText(CString strText);
	char*	GetText();

	void	SetAlign(DWORD dwAlign);

	void	SetImagePath(CString strPath, CRect rectBmp);


protected:
	// Init
	int	Initialize();

	// Draw
	void	Draw(CDC* dc);

	// Mouse
	void	LButtonDown(CPoint point);




// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextWnd)
	public:
	virtual BOOL Create(	LPCTSTR lpszWindowName,
				DWORD dwStyle,
				const RECT& rect,
				CWnd* pParentWnd,
				UINT nID,
				CFont* pfont = NULL,
				COLORREF rgbBG = RGB(255, 255, 255),
				COLORREF rgbFG = RGB(0, 0, 0),
				COLORREF rgbBorder = RGB(128, 128, 128));
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTextWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////



/*

	Class CTextWnd Version History

	v1.004		2006.02.01		Modify Positon of CFont* at Create(), SetColorBG()
	v1.003		2005.12.22		Add SetColorFG(), SetColorBG()
	v1.002		2005.12.01		Change Create()
	v1.001		2005.10.21		Initialize() -> _Initialize()
						Draw() -> _Draw()
	v1.000		2004			Class Defined
*/
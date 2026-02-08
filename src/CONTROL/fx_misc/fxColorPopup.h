#pragma once
// fxColorPopup.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CColorBtn CObject

class CColorBtn : public CObject
{
//	DECLARE_DYNCREATE(CColorBtn)
public:
	CColorBtn();           // protected constructor used by dynamic creation
	virtual ~CColorBtn();
// Attributes
public:
	CRect	m_winRC;
	COLORREF m_color;

// Operations
public:
	CRect getRect()		{ return m_winRC; };
	void  setRect(CRect rc) { m_winRC = rc; }
	COLORREF getColor()	{ return m_color; }
	void  setColor(COLORREF col) { m_color = col; }

	BOOL HitTest(CPoint point);

	void drawButton(CDC* dc);
	void drawSelectButton(CDC* dc);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorBtn)
	//}}AFX_VIRTUAL

// Implementation

	

	// Generated message map functions
	//{{AFX_MSG(CColorBtn)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

//	DECLARE_MESSAGE_MAP()
};


const	int nROW = 8;
const	int nCOL = 6;

/////////////////////////////////////////////////////////////////////////////
// CfxColorPopup window

class AFX_EXT_CLASS CfxColorPopup : public CWnd
{
// Construction
public:
	CfxColorPopup(CFont* pFont = nullptr);

// Attributes
public:
	CRect	m_winRC;

	CRect	m_popRC;
	CRect	m_closeRC;
	CRect	m_drawRC;
	CRect	m_sepRC;
	CRect	m_customRC;
	CRect	m_customRCx;

	bool	m_custom;
	bool	m_customSEL;
	CFont*	m_pFont;

	COLORREF  m_colCUR;		// current select color
	CColorBtn m_colBTN[nROW*nCOL];

	WPARAM	m_wparam;

	COLORREF m_titleBkCOL;
	COLORREF m_titleFgCOL;
	COLORREF m_winBkCOL;

// Operations
public:
	BOOL	CreateColorPopup(CWnd* parent, COLORREF defaultColor = RGB(0, 0, 0), bool custom = true);
	void	DisplayColorPopup(CPoint point, bool show, WPARAM wp = -1);
	void	SetColorPopup(COLORREF color);

private:
	void	initialPopup();

	void	drawCaption(CDC* dc);
	void	drawCustom(CDC* dc, bool hasColor);
	void	drawCustomSelect(CDC* dc);
	CColorBtn* HitTest(CPoint point);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxColorPopup)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfxColorPopup();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxColorPopup)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


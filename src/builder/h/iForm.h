#pragma once

// contrast 
#define	ct_uLimit	4
#define	ct_Rise		3
#define	ct_Fall		2
#define	ct_lLimit	1
#define	ct_Flat		0

class AFX_EXT_CLASS CiForm
{
public:
	CiForm(CString root = _T(""));
	virtual ~CiForm(void);

public:
	CString	m_root;
	CString	m_userID;
	CString	m_userName;
	CString	m_theme;

	bool	m_editMode;		// builder mode

private:
	class	CAxDraw*	m_draw;
	class	CAxPalette*	m_palette;

	WORD		m_langID;	// language identifier
	CStringArray	m_contrast;	// contrast string

public:
	// palette
	void	SetTheme(CString theme);
	COLORREF GetRGB(int index);
	int	GetPaletteIndex(COLORREF rgb);
	BOOL	Lookup(int index, COLORREF& rgb);

	// GDI object
	CFont*	SetFont(CString fontN, int point, bool italic, int bold);
	CPen*	SetPen(COLORREF rgb, int width = 1, int style = PS_SOLID);
	CBrush*	SetBrush(COLORREF rgb);
	void*	SetImage(CString fileN);		// CBitmap* or Gdiplus::Image*

	// contrast
	bool	IsContrast(int& value);
	int	GetContrast(int key, CString& text);
	CString	TrimSign(CString& data);
	int	GetDouble(int value);
	int	GetSign(CString str, int& offs);

	// code spin
};


#pragma once

class CAxPalette
{
public:
	CAxPalette(void);
	CAxPalette(CString fileN, CString theme = _T(""));
	virtual ~CAxPalette(void);

	COLORREF GetPaletteRGB(int index);
	int	GetIndexRGB(COLORREF rgb);
	void	SetPalette(CString theme);
	BOOL	Lookup(int index, COLORREF& rgb);

private:
	CString	m_paletteini;
	CMap	< int, int, COLORREF, COLORREF& > m_rgbs;
};


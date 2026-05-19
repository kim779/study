
#pragma once

class AFX_EXT_CLASS CAmPalette
{
public:
	CAmPalette();
	CAmPalette(char *path, char *theme);
	virtual ~CAmPalette();

public:
	ARGB GetPaletteARGB(int index);
	void	Setpalette(char *theme);
	BOOL	Lookup(int index, ARGB &argb);
	int	GetIndexARGB(ARGB argb);

protected:
	CString	m_palette;
	CMap	< int, int, ARGB, ARGB& > m_argbs;
};



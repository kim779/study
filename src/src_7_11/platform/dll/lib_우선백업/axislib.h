// axslib.h: interface for the CAxisDraw class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <afxtempl.h>

class Cbitmap
{
public:
	time_t	m_mtime;
	CBitmap* m_bitmap;
};

class AFX_EXT_CLASS CAxisDraw  
{
public:
	CAxisDraw();
	virtual ~CAxisDraw();

public:
	CFont*	SetFont(CDC* pDC, char* fontN, int point, bool italic, int bold);
	void	SetPen(CDC* pDC, COLORREF crRGB, int width = 1, int style = PS_SOLID);
	CBrush*	SetBrush(CDC* pDC, COLORREF crRGB);
	CBitmap* SetBitmap(char* fileN);

	CFont*	SetFont(struct _fontR* fontR);
	CPen*	SetPen(struct _penR* penR);
	CBrush*	SetBrush(COLORREF crRGB);

private:
	void	RemoveAllFont();
	void	RemoveAllPen();
	void	RemoveAllBrush();
	void	RemoveAllBitmap();

protected:
	CMapStringToOb	m_penObs;
	CMap	< COLORREF, COLORREF, CBrush *, CBrush * > m_brushObs;
	CMapStringToOb	m_pointFontObs;
	CMapStringToPtr	m_bmpObs;
};


class AFX_EXT_CLASS CAxisPalette  
{
public:
	CAxisPalette();
	CAxisPalette(char *rgbN);
	virtual ~CAxisPalette();

public:
	COLORREF GetPaletteRGB(int index);
	int	GetIndexRGB(COLORREF rgb);
	void	SetPalette(char *rgbN, bool force = false);
	BOOL	Lookup(int index, COLORREF& rgb);

protected:
	CMap	< int, int, COLORREF, COLORREF& > m_rgbs;
};


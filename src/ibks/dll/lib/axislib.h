// axslib.h: interface for the CAxisDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXISLIB_H__F56D12CC_AD2F_11D2_A459_00C00C02F5D7__INCLUDED_)
#define AFX_AXISLIB_H__F56D12CC_AD2F_11D2_A459_00C00C02F5D7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>

#define HIMETRIC_INCH	2540			// 1inch -> 1/100 mm

class Cbitmap
{
public:
	Cbitmap();
	virtual	~Cbitmap();

public:
	time_t	m_mtime;
	CBitmap* m_bitmap;

protected:
	IPicture*	m_picture;

public:
	bool	Load(LPCTSTR fileN);

protected:
	bool	getHandle(HBITMAP& bitmap);
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
// updateXXX_20220725
	void	ModifyFontsize(CString fName, long& height);

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
	void	SetPalette(char *rgbN);
	BOOL	Lookup(int index, COLORREF& rgb);

protected:
	CMap	< int, int, COLORREF, COLORREF& > m_rgbs;
};

#endif // !defined(AFX_AXISLIB_H__F56D12CC_AD2F_11D2_A459_00C00C02F5D7__INCLUDED_)

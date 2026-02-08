#ifndef BITMAP_H
#define BITMAP_H

class CExBitmap
{
	friend class CExPalette;

	CExPalette*	m_pPal;
	BITMAPINFO*	m_pInfo;
	BYTE*		m_pPixels;
	BOOL		m_bIsPadded;

public:	
	CExBitmap();
	virtual ~CExBitmap();

private:
	CExBitmap( const CExBitmap& dbmp );

public:	
	BITMAPINFO*	GetHeaderPtr() const;
	BYTE*		GetPixelPtr() const;
	RGBQUAD*	GetColorTablePtr() const;
	int		GetWidth() const;
	int		GetHeight() const;
	void		GetSize(int& width, int& height);
	CExPalette*	GetPalette() { return m_pPal; }
		
public:	
	BOOL		CreatePalette();
	void		ClearPalette();
	BOOL		CreateFromBitmap(CDC *, CBitmap *);
	void		DestroyBitmap();
	BOOL		DeleteObject() { DestroyBitmap(); return TRUE; }
	
	virtual	BOOL	Load(CFile* pFile);
	virtual	BOOL	Load(const CString &);
	virtual	BOOL	Load(HBITMAP& hBitmap);
	
	virtual void	DrawDIB(CDC *pDC, int x=0, int y=0);
	virtual void	DrawDIB(CDC *pDC, int x, int y, int width, int height);

protected:
	int		GetPalEntries() const;
	int		GetPalEntries(const BITMAPINFOHEADER& infoHeader) const;
private:

	
};

#include "palette.h"

#endif	BITMAP_H
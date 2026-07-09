// picture.h: interface for the Cpicture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTURE_H__8695D734_5BD9_460C_85D1_89D8BA98D6ED__INCLUDED_)
#define AFX_PICTURE_H__8695D734_5BD9_460C_85D1_89D8BA98D6ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CpictureImpl;
class AFX_EXT_CLASS Cpicture  
{
public:
	Cpicture(CString fileN);
	virtual ~Cpicture();

protected:
	CpictureImpl*	m_picImpl;
	bool	m_valid;
	enum	{FX_NONE, FX_BMP, FX_GIF} m_ext;

public:
	void	DrawPicture(CDC* dc, CRect rect, int style);
	CSize	getImageSize();

protected:
	void	GetFileType(CString fileN);
};


class AFX_EXT_CLASS CpictureImpl  
{
public:
	CpictureImpl();
	virtual ~CpictureImpl();

protected:
	bool	m_available;

public:
	virtual	void	Draw(CDC* pDC, CRect rect, int style) {}
	virtual CSize	getImageSize(){return CSize(0, 0);}
};


class AFX_EXT_CLASS CpictureBMP : public CpictureImpl
{
public:
	CpictureBMP()	{}
	CpictureBMP(CString fileN);
	virtual ~CpictureBMP();

//Attributes
protected :
	BITMAPINFOHEADER	*bmInfoHp;
	BITMAPINFOHEADER	bmInfoH;
	HANDLE			hBitmapDes;
	int			palSize;
	int			nColors;

public:
	void	Draw(CDC* dc, CRect rect, int style);
	CSize	getImageSize();

protected:
	int	GetNumOfColors(void* pv);
};


#include "gif.h"
class AFX_EXT_CLASS CpictureGIF : public CpictureImpl
{
public:
	CpictureGIF()	{}
	CpictureGIF(CString fileN);
	virtual ~CpictureGIF();

protected :
	int	Width, Height, Depth;
	long	EffWidth;

	LPBITMAPINFOHEADER	lpbi;
	HPALETTE		hPalette;
	BYTE	*RawImage;
	BYTE	*IterImage;

	int	bgindex;
	COLORREF bgCR;

	int	ibf;
	BYTE	imb[MaxGIFB+1];

	int	interlaced, iheight, istep, iypos, ipass;

	int	Itx, Ity;
	int	Stepx, Stepy;

	FILE	*fp;
	struct	dscgif	dscgif;
	struct	gifgce	gifgce;
	struct	tabcol	tabcol;
	struct	image	image;

	short	curr_size;		// The current code size
	short	clear;			// Value for a clear code
	short	ending;			// Value for a ending code
	short	newcodes;		// First available code
	short	top_slot;		// Highest code for current size
	short	slot;			// Last read code

	short	navail_bytes;		// # bytes left in block
	short	nbits_left;		// # bits left in current byte
	BYTE	b1;			// Current byte
	BYTE	byte_buff[257];		// Current block
	BYTE	*pbytes;		// Pointer to next byte in block

public:
	void	Draw(CDC* dc, CRect rect, int style);
	CSize	getImageSize();

protected:
	int	getbyte();
	int	outline(BYTE* pixels, int linelen);
	void	initexp(int size);
	WORD	getnextcode();
	short	parseCode(int linewidth);
	void	createImage(int width, int height, int depth);
	void	dibCreate(int bits, int dx, int dy);
	void	setPalette(int n, BYTE* r, BYTE* g, BYTE* b);
	void	drawImage(CDC* dc, int x, int y, int cx, int cy, int xSrc, int ySrc, UINT sScan, UINT nScan, void* buf);
	void	drawImage(CDC* dc, int x, int y, int cx, int cy, int cxSrc, int cySrc);
};

#endif // !defined(AFX_PICTURE_H__8695D734_5BD9_460C_85D1_89D8BA98D6ED__INCLUDED_)

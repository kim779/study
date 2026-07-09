// pictureImpl.cpp: implementation of the CpictureImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "picture.h"
#include "../form_w.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CpictureImpl::CpictureImpl()
{
}

CpictureImpl::~CpictureImpl()
{
}

//------------------------------------------------------
//	class CpictureBMP
//

#define	WIDTHBYTES(w)	((w + 31) / 32 * 4)	//

static long code_mask[13] =
{
	0,
	0x0001, 0x0003,
	0x0007, 0x000f,
	0x001f, 0x003f,
	0x007f, 0x00ff,
	0x01ff, 0x03ff,
	0x07ff, 0x0fff,
};

CpictureBMP::CpictureBMP(CString fileN)
{
	HANDLE	hBitmapFile;
	BOOL	fBool;
	DWORD	Offsets, NumOfBytesRead;

	BITMAPFILEHEADER bmFileH;
	BITMAPCOREHEADER *bmCoreH;

	RGBQUAD	rgb;
	RGBQUAD *pRgb;
	int	biSize, ii;

	m_available = false;
	hBitmapFile = CreateFile((char *)fileN.operator LPCTSTR(), GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hBitmapFile == INVALID_HANDLE_VALUE)
		return;

	Offsets = SetFilePointer(hBitmapFile, 0, NULL, FILE_CURRENT);
	fBool = ReadFile(hBitmapFile, &bmFileH, sizeof(BITMAPFILEHEADER), &NumOfBytesRead, NULL);

	if (!fBool || NumOfBytesRead != sizeof(BITMAPFILEHEADER))
	{
		CloseHandle(hBitmapFile);
		return;
	}

	if (bmFileH.bfType != 0x4d42)
	{
		bmFileH.bfOffBits = 0L;
		SetFilePointer(hBitmapFile, Offsets, NULL, FILE_BEGIN);
	}

	fBool = ReadFile(hBitmapFile, &bmInfoH, sizeof(BITMAPINFOHEADER), &NumOfBytesRead, NULL);
	if (!fBool || NumOfBytesRead != sizeof(BITMAPINFOHEADER))
	{
		CloseHandle(hBitmapFile);
		return;
	}

	nColors = GetNumOfColors(&bmInfoH);

	biSize = (int)bmInfoH.biSize;
	switch (biSize)
	{
	case sizeof(BITMAPINFOHEADER):
		palSize = nColors * sizeof(RGBQUAD);
		break;
	case sizeof(BITMAPCOREHEADER):
		bmCoreH = (BITMAPCOREHEADER *) &bmInfoH;

		bmInfoH.biSize = sizeof(BITMAPINFOHEADER);
		bmInfoH.biWidth = bmCoreH->bcWidth;
		bmInfoH.biHeight = bmCoreH->bcHeight;
		bmInfoH.biPlanes = bmCoreH->bcPlanes;
		bmInfoH.biBitCount = bmCoreH->bcBitCount;
		bmInfoH.biCompression = BI_RGB;
		bmInfoH.biSizeImage = 0;
		bmInfoH.biXPelsPerMeter = 0;
		bmInfoH.biYPelsPerMeter = 0;
		bmInfoH.biClrUsed = nColors;
		bmInfoH.biClrImportant = nColors;
		SetFilePointer(hBitmapFile, sizeof(BITMAPCOREHEADER) - sizeof(BITMAPINFOHEADER),
				NULL, FILE_CURRENT);
		palSize = nColors * sizeof(RGBTRIPLE);
		break;
	default:
		CloseHandle(hBitmapFile);
		return;
	}

	if (bmInfoH.biSizeImage == 0)
	{
		bmInfoH.biSizeImage
			= WIDTHBYTES((DWORD)bmInfoH.biWidth * bmInfoH.biBitCount)* bmInfoH.biHeight;
	}

	if (bmInfoH.biClrUsed == 0)
		bmInfoH.biClrUsed = nColors;

	hBitmapDes = GlobalAlloc(GHND, bmInfoH.biSize + palSize + bmInfoH.biSizeImage);

	if (hBitmapDes == NULL)
	{
		CloseHandle(hBitmapFile);
		return;
	}

	bmInfoHp  = (BITMAPINFOHEADER *) GlobalLock(hBitmapDes);
	*bmInfoHp = bmInfoH;

	pRgb = (RGBQUAD *)((LPSTR)bmInfoHp+bmInfoH.biSize);
	if (nColors)
	{
		if (biSize == sizeof(BITMAPCOREHEADER))
		{
			ReadFile(hBitmapFile, pRgb, nColors * sizeof(RGBTRIPLE), &NumOfBytesRead, NULL);
			for (ii = nColors - 1; ii >= 0; ii--)
			{
				rgb.rgbRed      = ((RGBTRIPLE *)pRgb)[ii].rgbtRed;
				rgb.rgbBlue     = ((RGBTRIPLE *)pRgb)[ii].rgbtBlue;
				rgb.rgbGreen    = ((RGBTRIPLE *)pRgb)[ii].rgbtGreen;
				rgb.rgbReserved = PC_RESERVED;
				pRgb[ii] = rgb;
			}
		}
		else
			ReadFile(hBitmapFile, pRgb, nColors * sizeof(RGBQUAD), &NumOfBytesRead, NULL);
	}

	if (bmFileH.bfOffBits != 0L)
		SetFilePointer(hBitmapFile, Offsets + bmFileH.bfOffBits, NULL, FILE_BEGIN);

	ReadFile(hBitmapFile, (LPSTR)bmInfoHp + (WORD)bmInfoHp->biSize + palSize, bmInfoH.biSizeImage, &NumOfBytesRead, NULL);
	CloseHandle(hBitmapFile);
	m_available = true;
}

CpictureBMP::~CpictureBMP()
{
	GlobalUnlock(hBitmapDes);
	GlobalFree(hBitmapDes);
}

void CpictureBMP::Draw(CDC* pDC, CRect rect, int bStyle)
{
	void	*pBuf;
	int	xb, yb;
	int	cxb, cyb, ybl;
	int	tcxb, tcyb;

	if (!m_available)
		return;

	pDC->SetBkMode(TRANSPARENT);
	pBuf = (LPSTR)bmInfoHp + (WORD)bmInfoHp->biSize + palSize;

	xb = rect.left;
	yb = rect.top;
	cxb = rect.right - rect.left;
	cyb = rect.bottom - rect.top;

	switch (bStyle)
	{
	case AL_TILE:
		tcyb = cyb;

		for (;;)
		{
			tcxb = cxb;
			for (;;)
			{
				if (bmInfoH.biWidth < cxb)
					cxb = bmInfoH.biWidth;

				if (cyb > bmInfoH.biHeight)
				{
					ybl = 0;
					cyb = bmInfoH.biHeight;
				}
				else
				{
					ybl = bmInfoH.biHeight - cyb;
				}
				::SetDIBitsToDevice(pDC->m_hDC, xb, yb, cxb, cyb, 0, ybl, 0, bmInfoH.biHeight,
						pBuf, (LPBITMAPINFO)bmInfoHp, DIB_RGB_COLORS);
				tcxb -= cxb;
				if (tcxb <= 0)
					break;

				xb += cxb;
				cxb = tcxb;
			}
			tcyb -= cyb;
			if (tcyb <= 0)
				break;

			xb = rect.left;
			cxb = rect.right - rect.left;
			yb += cyb;
			cyb = tcyb;
		}
		break;

	case AL_STRETCH:
		::StretchDIBits(pDC->m_hDC, xb, yb, cxb, cyb, 0, 0, bmInfoH.biWidth, bmInfoH.biHeight,
				pBuf, (LPBITMAPINFO)bmInfoHp, DIB_RGB_COLORS, SRCCOPY);
		break;

	case AL_CENTER:
	default:
		if (bmInfoH.biWidth < cxb)
		{
			xb += (cxb - bmInfoH.biWidth) / 2;
			cxb = bmInfoH.biWidth;
		}

		if (cyb > bmInfoH.biHeight)
		{
			ybl = 0;
			yb += (cyb - bmInfoH.biHeight) / 2;
			cyb = bmInfoH.biHeight;
		}
		else
			ybl = bmInfoH.biHeight - cyb;

		::SetDIBitsToDevice(pDC->m_hDC, xb, yb, cxb, cyb, 0, ybl, 0, bmInfoH.biHeight,
				pBuf, (LPBITMAPINFO)bmInfoHp, DIB_RGB_COLORS);
		break;
	}

	pDC->SetBkMode(OPAQUE);
}

int CpictureBMP::GetNumOfColors(void* pv)
{
	LPBITMAPINFOHEADER	lpbi;
	LPBITMAPCOREHEADER	lpbc;
	int	bits;

	lpbi = (LPBITMAPINFOHEADER)pv;
	lpbc = (LPBITMAPCOREHEADER)pv;

	if (lpbi->biSize != sizeof(BITMAPCOREHEADER))
	{
		if (lpbi->biClrUsed != 0)
			return((WORD)(lpbi->biClrUsed));

		bits = lpbi->biBitCount;
	}
	else
		bits = lpbc->bcBitCount;

	switch (bits)
	{
	case 1:
		return(2);
	case 4:
		return(16);
	case 8:
		return(256);
	default:
		return(0);
	}
}

CSize CpictureBMP::getImageSize()
{
	return CSize(bmInfoHp->biWidth, bmInfoHp->biHeight);
}

//------------------------------------------------------
//	class CpictureGIF
//

CpictureGIF::CpictureGIF(CString fileN)
{
	char	ch;
	BYTE	fc, count;

	lpbi     = NULL;
	bgindex  = -1;
	ibf      = MaxGIFB;
	hPalette = NULL;

	m_available = false;

	FILE*	fp = nullptr;
	errno_t	err = 0;

	err = fopen_s(&fp, (char *)fileN.operator LPCTSTR(), "rb");
	if (err != 0)
		return;

	fread((char *)&dscgif, sizeof(struct dscgif), 1, fp);
	if (strncmp(dscgif.header, "GIF8", 4) != 0)
	{
		fclose(fp);
		return;
	}

	tabcol.sogct = 1 << ((dscgif.pflds & 0x07) + 1);
	tabcol.colres = ((int)(dscgif.pflds & 0x70) >> 3) + 1;

	if (dscgif.pflds & 0x80)
	{
		fread((char *)tabcol.rgbp,
			sizeof(struct rgb) * tabcol.sogct, 1, fp);
	}

	for (;;)
	{
		if (fread(&ch, 1, 1, fp) <= 0)
			break;

		if (ch == '!')
		{
			if (fread(&fc, 1, 1, fp) <=0)
				break;

			if (fc == 0xf9)
			{
				if (fread(&count, 1, 1, fp) <= 0)
					break;
				if (fread(&gifgce, 1, 4, fp) != count)
					break;
				if (gifgce.transpcolflag)
					bgindex = gifgce.transpcolindex;
			}

			while (fread(&count, 1, 1,fp) && count)
			{
				fseek(fp, count, SEEK_CUR);
			}
			continue;
		}

		if (ch != ',')
			break;

		fread((char *)&image, sizeof(struct image), 1, fp);
		if (image.pf & 0x80)
		{
			tabcol.sogct = 1 << ((image.pf & 0x07) + 1);
			fread((char *)tabcol.rgbp,
				sizeof(struct rgb) * tabcol.sogct, 1, fp);
		}

		createImage(image.w, image.h, 8);

		if (image.pf & 0x80 || dscgif.pflds & 0x80)
		{
			BYTE	r[256], g[256], b[256];
			int	ii, has_white = 0;

			for (ii = 0; ii < tabcol.sogct; ii++)
			{
				r[ii] = tabcol.rgbp[ii].r;
				g[ii] = tabcol.rgbp[ii].g;
				b[ii] = tabcol.rgbp[ii].b;

				if (RGB(r[ii], g[ii], b[ii]) == 0xffffff)
					has_white = 1;
			}

			if (bgindex != -1)
			{
				bgCR = RGB(r[bgindex], g[bgindex], b[bgindex]);
			//	r[bgindex] = g[bgindex] = b[bgindex] = 255;
/*
				for (ii = 0; ii < 256; ii++)
				{
					has_white = 0;
					r[ii] = g[ii] = b[ii] = 0;
				}
*/
			}

			if (bgindex != -1 && !has_white)
				r[255] = g[255] = b[255] = 255;
			setPalette(bgindex != -1 ? 255 : tabcol.sogct, r, g, b);
		}

		Itx = Ity = 0;
		Stepx = Stepy = 0;
		ibf = MaxGIFB;

		interlaced = image.pf & 0x40;
		iheight = image.h;
		istep = 8;
		iypos = 0;
		ipass = 0;

		Itx = 0;
		Ity = Height - 1;
		IterImage = RawImage + EffWidth * (Height - 1);

		parseCode(Width);
	}

	fclose(fp);
	m_available = true;
}

CpictureGIF::~CpictureGIF()
{
	if (lpbi)
		delete[] lpbi;
	if (hPalette)
		DeleteObject(hPalette);
}

void CpictureGIF::Draw(CDC* pDC, CRect rect, int bStyle)
{
	HPALETTE hpalT;
	void*	pBuf;
	int	xb, yb;
	int	cxb, cyb, ybl;
	int	tcxb, tcyb;
	int	nDC;

	if (!m_available)
		return;
	nDC = pDC->SaveDC();

	pDC->SetBkMode(TRANSPARENT);
	hpalT = ::SelectPalette(pDC->m_hDC, hPalette, FALSE);
	pDC->RealizePalette();

	pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize + lpbi->biClrUsed * sizeof(RGBQUAD);

	xb = rect.left;
	yb = rect.top;
	cxb = rect.right - rect.left;
	cyb = rect.bottom - rect.top;

	switch (bStyle)
	{
	case AL_TILE:
		tcyb = cyb;

		for (;;)
		{
			tcxb = cxb;
			for (;;)
			{
				if (lpbi->biWidth < cxb)
					cxb = lpbi->biWidth;

				if (cyb > lpbi->biHeight)
				{
					ybl = 0;
					cyb = lpbi->biHeight;
				}
				else
					ybl = lpbi->biHeight - cyb;

				if (bgindex != -1)
					ybl = 0;
				drawImage(pDC, xb, yb, cxb, cyb, 0, ybl,
						0, lpbi->biHeight, pBuf);
				tcxb -= cxb;
				if (tcxb <= 0)
					break;

				xb += cxb;
				cxb = tcxb;
			}
			tcyb -= cyb;
			if (tcyb <= 0)
				break;

			xb = rect.left;
			cxb = rect.right - rect.left;
			yb += cyb;
			cyb = tcyb;
		}
		break;

	case AL_STRETCH:
		::StretchDIBits(pDC->m_hDC, xb, yb, cxb, cyb, 0, 0, lpbi->biWidth, lpbi->biHeight,
					pBuf, (LPBITMAPINFO)lpbi, DIB_PAL_COLORS, SRCCOPY);
		break;
	//	drawImage(pDC, xb, yb, cxb, cyb, lpbi->biWidth, lpbi->biHeight);
	//	break;

	case AL_CENTER:
	default:
		if (lpbi->biWidth < cxb)
		{
			xb += (cxb - lpbi->biWidth) / 2;
			cxb = lpbi->biWidth;
		}

		if (cyb > lpbi->biHeight)
		{
			ybl = 0;
			yb += (cyb - lpbi->biHeight) / 2;
			cyb = lpbi->biHeight;
		}
		else
		{
			ybl = lpbi->biHeight - cyb;
		}
		drawImage(pDC, xb, yb, cxb, cyb, 0, ybl, 0, lpbi->biHeight, pBuf);
		break;
	}

	pDC->RestoreDC(nDC);

	::SelectPalette(pDC->m_hDC, hpalT, FALSE);
	pDC->SetBkMode(OPAQUE);

}

void CpictureGIF::drawImage(CDC* dc, int x, int y, int cx, int cy, int xSrc,
			      int ySrc, UINT sScan, UINT nScan, void* buf)
{
	if (bgindex == -1)
	{
		::SetDIBitsToDevice(dc->m_hDC, x, y, cx, cy, xSrc, ySrc,
				sScan, nScan, buf,
				(LPBITMAPINFO)lpbi, DIB_PAL_COLORS);
		return;
	}

	COLORREF crOldBk = dc->SetBkColor(RGB(255, 255, 255));
	COLORREF crOldFg = dc->SetTextColor(RGB(0,   0,   0));

	CDC	dcImage,  dcTrans;
	dcImage.CreateCompatibleDC(dc);
	dcTrans.CreateCompatibleDC(dc);

	HBITMAP hbitmap = CreateDIBitmap(dc->m_hDC, lpbi,
			CBM_INIT, (void *)RawImage, (LPBITMAPINFO)lpbi,
			DIB_PAL_COLORS);

	CBitmap* orgBmp = CBitmap::FromHandle(hbitmap);
	CBitmap* oldBmp = dcImage.SelectObject(orgBmp);
	CBitmap	transBmp;
	transBmp.CreateBitmap(cx, cy, 1, 1, NULL);

	CBitmap* oldTrans = dcTrans.SelectObject(&transBmp);
	dcImage.SetBkColor(bgCR);
	dcTrans.BitBlt(0, 0, cx, cy, &dcImage, xSrc, ySrc, SRCCOPY);
	dc->BitBlt(x, y, cx, cy, &dcImage, xSrc, ySrc, SRCINVERT);
	dc->BitBlt(x, y, cx, cy, &dcTrans, xSrc, ySrc, SRCAND);
	dc->BitBlt(x, y, cx, cy, &dcImage, xSrc, ySrc, SRCINVERT);

	dcImage.SelectObject(oldBmp);
	dcTrans.SelectObject(oldTrans);
	dc->SetBkColor(crOldBk);
	dc->SetTextColor(crOldFg);
	transBmp.DeleteObject();
	DeleteObject(hbitmap);
}

void CpictureGIF::drawImage(CDC* dc, int x, int y, int cx, int cy, int cxSrc, int cySrc)
{
	COLORREF crOldBk = dc->SetBkColor(RGB(255, 255, 255));
	COLORREF crOldFg = dc->SetTextColor(RGB(0,   0,   0));

	CDC	dcImage,  dcTrans;
	dcImage.CreateCompatibleDC(dc);
	dcTrans.CreateCompatibleDC(dc);

	HBITMAP hbitmap = CreateDIBitmap(dc->m_hDC, lpbi,
			CBM_INIT, (void *)RawImage, (LPBITMAPINFO)lpbi,
			DIB_PAL_COLORS);

	CBitmap* orgBmp = CBitmap::FromHandle(hbitmap);
	CBitmap* oldBmp = dcImage.SelectObject(orgBmp);
	dc->StretchBlt(x, y, cx, cy, &dcImage, 0, 0, cxSrc, cySrc, SRCCOPY);
/*
	CBitmap	transBmp;
	transBmp.CreateBitmap(cx, cy, 1, 1, NULL);

	CBitmap* oldTrans = dcTrans.SelectObject(&transBmp);
	dcImage.SetBkColor(bgCR);
	dcTrans.StretchBlt(0, 0, cx, cy, &dcImage, 0, 0, cxSrc, cySrc, SRCCOPY);
	dc->StretchBlt(x, y, cx, cy, &dcImage, 0, 0, cxSrc, cySrc, SRCINVERT);
	dc->StretchBlt(x, y, cx, cy, &dcTrans, 0, 0, cxSrc, cySrc, SRCAND);
	dc->StretchBlt(x, y, cx, cy, &dcImage, 0, 0, cxSrc, cySrc, SRCINVERT);
*/
	dcImage.SelectObject(oldBmp);
//	dcTrans.SelectObject(oldTrans);
	dc->SetBkColor(crOldBk);
	dc->SetTextColor(crOldFg);
	DeleteObject(hbitmap);
}

int CpictureGIF::getbyte()
{
	if (ibf >= MaxGIFB)
	{
		fread(imb, MaxGIFB, 1, fp);
		ibf = 0;
	}
	return(imb[ibf++]);
}

int CpictureGIF::outline(BYTE* pixels, int linelen)
{
	int	y;

	if (interlaced)
	{
		y = iheight-iypos-1;
		if (y >= 0 && y <= Height)
		{
			Ity = y;
			IterImage = RawImage + EffWidth * y;
		}

		if (linelen < 0)
			linelen = Width;
		CopyMemory(IterImage, pixels, linelen);

		if ((iypos += istep) >= iheight)
		{
			do
			{
				if (ipass++ > 0)
					istep /= 2;

				iypos = istep / 2;
			}
			while (iypos > iheight);
		}

		return 0;
	}

	if (Itx >= 0 && Itx < Width && Ity >= 0 && Ity < Height)
	{
		if (linelen < 0)
			linelen = Width;
		CopyMemory(IterImage, pixels, linelen);

		if (--Ity >= 0)
			IterImage -= EffWidth;
		return 0;
	}
	return -1;
}

void CpictureGIF::initexp(int size)
{
	curr_size = size + 1;
	top_slot = 1 << curr_size;
	clear = 1 << size;
	ending = clear + 1;
	slot = newcodes = ending + 1;
	navail_bytes = nbits_left = 0;
}

WORD CpictureGIF::getnextcode()
{
	short	ii, x;
	WORD	ret;

	if (nbits_left == 0)
	{
		if (navail_bytes <= 0)
		{
		// Out of bytes in current block, so read next block
			pbytes = byte_buff;
			if ((navail_bytes = getbyte()) < 0)
				return(navail_bytes);
			else if (navail_bytes)
			{
				for (ii = 0; ii < navail_bytes; ++ii)
				{
					if ((x = getbyte()) < 0)
						return(x);
					byte_buff[ii] = (BYTE) x;
				}
			}
		}
		b1 = *pbytes++;
		nbits_left = 8;
		--navail_bytes;
	}

	ret = b1 >> (8 - nbits_left);
	while (curr_size > nbits_left)
	{
		if (navail_bytes <= 0)
		{
		// Out of bytes in current block, so read next block
			pbytes = byte_buff;
			if ((navail_bytes = getbyte()) < 0)
				return(navail_bytes);
			else if (navail_bytes)
			{
				for (ii = 0; ii < navail_bytes; ++ii)
				{
					if ((x = getbyte()) < 0)
						return(x);
					byte_buff[ii] = (BYTE) x;
				}
			}
		}
		b1 = *pbytes++;
		ret |= b1 << nbits_left;
		nbits_left += 8;
		--navail_bytes;
	}
	nbits_left -= curr_size;
	ret &= code_mask[curr_size];
	return(ret);
}

short CpictureGIF::parseCode(int linewidth)
{
	static BYTE stack[MaxCODES];		// Stack for storing pixels
	static BYTE suffix[MaxCODES];		// Suffix table
	static WORD prefix[MaxCODES];		// Prefix linked list

	register BYTE	*sp, *bufptr;
	BYTE	*buf;
	register short	code, fc, oc, bufcnt;
	short	c, size, ret;

	if ((size = getbyte()) < 0)
		return(0);

	if (size < 2 || size > 9)
		return(0);

	initexp(size);

	// Initialize in case they forgot to put in a clear code.
	// (This shouldn't happen, but we'll try and decode it anyway...)
	oc = fc = 0;

	// Allocate space for the decode buffer
	if ((buf = (BYTE*)new char[linewidth + 1]) == NULL)
	      return(0);

	// Set up the stack pointer and decode buffer pointer
	sp = stack;
	bufptr = buf;
	bufcnt = linewidth;

	// This is the main loop.  For each code we get we pass through the
	// linked list of prefix codes, pushing the corresponding "character"
	// for each code onto the stack.  When the list reaches a single
	// "character" we push that on the stack too, and then start unstacking
	// each character for output in the correct order.  Special handling is
	// included for the clear code, and the whole thing ends when we get
	// an ending code.
	while ((c = getnextcode()) != ending)
	{
	// If we had a file error, return without completing the decode
		if (c < 0)
		{
			delete[] buf;
			return(0);
		}

	// If the code is a clear code, reinitialize all necessary items.
		if (c == clear)
		{
			curr_size = size + 1;
			slot = newcodes;
			top_slot = 1 << curr_size;

	// Continue reading codes until we get a non-clear code
	// (Another unlikely, but possible case...)
			while ((c = getnextcode()) == clear)
				;

	// If we get an ending code immediately after a clear code
	// (Yet another unlikely case), then break out of the loop.
			if (c == ending)
				break;

	// Finally, if the code is beyond the range of already set codes,
	// (This one had better NOT happen...  I have no idea what will
	// result from this, but I doubt it will look good...) then set it
	// to color zero.
			if (c >= slot)
				c = 0;

			oc = fc = c;

	// And let us not forget to put the char into the buffer... And
	// if, on the off chance, we were exactly one pixel from the end
	// of the line, we have to send the buffer to the outline()
	// routine...
			*bufptr++ = (BYTE) c;
			if (--bufcnt == 0)
			{
				if ((ret = outline(buf, linewidth)) < 0)
				{
					delete[] buf;
					return(ret);
				}
				bufptr = buf;
				bufcnt = linewidth;
			}
		}
		else
		{

	// In this case, it's not a clear code or an ending code, so
	// it must be a code code...  So we can now decode the code into
	// a stack of character codes. (Clear as mud, right?)
			code = c;

	// Here we go again with one of those off chances...  If, on the
	// off chance, the code we got is beyond the range of those already
	// set up (Another thing which had better NOT happen...) we trick
	// the decoder into thinking it actually got the last code read.
	// (Hmmn... I'm not sure why this works...  But it does...)
			if (code >= slot)
			{
				code = oc;
				*sp++ = (BYTE) fc;
			}

	// Here we scan back along the linked list of prefixes, pushing
	// helpless characters (ie. suffixes) onto the stack as we do so.
			while (code >= newcodes)
			{
				*sp++ = suffix[code];
				code = prefix[code];
			}

	// Push the last character on the stack, and set up the new
	// prefix and suffix, and if the required slot number is greater
	// than that allowed by the current bit size, increase the bit
	// size.  (NOTE - If we are all full, we *don't* save the new
	// suffix and prefix...  I'm not certain if this is correct...
	// it might be more proper to overwrite the last code...
			*sp++ = (BYTE) code;
			if (slot < top_slot)
			{
				suffix[slot] = (BYTE) code;
				fc = code;
				prefix[slot++] = oc;
				oc = c;
			}
			if (slot >= top_slot)
				if (curr_size < 12)
				{
					top_slot <<= 1;
					++curr_size;
				} 

	// Now that we've pushed the decoded string (in reverse order)
	// onto the stack, lets pop it off and put it into our decode
	// buffer...  And when the decode buffer is full, write another
	// line...
			while (sp > stack)
			{
				*bufptr++ = *(--sp);
				if (--bufcnt == 0)
				{
					if ((ret = outline(buf, linewidth)) < 0)
					{
						delete[] buf;
						return(ret);
					}
					bufptr = buf;
					bufcnt = linewidth;
				}
			}
		}
	}
	ret = 0;
	if (bufcnt != linewidth)
		ret = outline(buf, (linewidth - bufcnt));
	delete[] buf;
	return(ret);
}

void CpictureGIF::createImage(int width, int height, int depth)
{
	Width  = width;
	Height = height;
	Depth  = depth;

	if (lpbi)
		delete[] lpbi;

	dibCreate(Depth, Width, Height);
}

void CpictureGIF::dibCreate(int bits, int dx, int dy)
{
	DWORD	dwSizeImage;
	int	ii;
	WORD	*pw;

	dwSizeImage = dy * (DWORD)((dx*bits/8+3)&~3);
	lpbi = (BITMAPINFOHEADER*) new char[sizeof(BITMAPINFOHEADER)+dwSizeImage+1024];
	if (lpbi == NULL)
		return;

	lpbi->biSize            = sizeof(BITMAPINFOHEADER);
	lpbi->biWidth           = dx;
	lpbi->biHeight          = dy;
	lpbi->biPlanes          = 1;
	lpbi->biBitCount        = bits;
	lpbi->biCompression     = BI_RGB;
	lpbi->biSizeImage       = dwSizeImage;
	lpbi->biXPelsPerMeter   = 0;
	lpbi->biYPelsPerMeter   = 0;
	lpbi->biClrUsed         = 0;
	lpbi->biClrImportant    = 0;

	if (bits == 4)
		lpbi->biClrUsed = 16;
	else if (bits == 8)
		lpbi->biClrUsed = 256;

	pw = (WORD *)((LPBYTE)lpbi+(int)lpbi->biSize);

	for (ii = 0; ii < (int)lpbi->biClrUsed; ii++)
	{
		*pw++ = ii;
	}

	RawImage = (BYTE*)(lpbi->biCompression == BI_BITFIELDS ?
			(LPVOID)((RGBQUAD *)((LPBYTE)lpbi + lpbi->biSize) + 3) :
			(LPVOID)((RGBQUAD *)((LPBYTE)lpbi + lpbi->biSize) +
					(UINT)lpbi->biClrUsed));
	EffWidth = (long)(((long)Width*Depth + 31) / 32) * 4;
}

void CpictureGIF::setPalette(int n, BYTE* r, BYTE* g, BYTE* b)
{
	NPLOGPALETTE	npPal;
	int	ii;

	if (!g)	g = r;
	if (!b)	b = g;

	npPal = (LOGPALETTE*)LocalAlloc(LMEM_FIXED,
		sizeof(LOGPALETTE) + n * sizeof(PALETTEENTRY));
	if (!npPal)
		return;

	npPal->palVersion = 0x300;
	npPal->palNumEntries = n;

	for (ii = 0; ii < n; ii++)
	{
		npPal->palPalEntry[ii].peRed   = r[ii];
		npPal->palPalEntry[ii].peGreen = g[ii];
		npPal->palPalEntry[ii].peBlue  = b[ii];
		npPal->palPalEntry[ii].peFlags = 0;
	}

	hPalette = CreatePalette(npPal);
	LocalFree(npPal);
}

CSize CpictureGIF::getImageSize()
{
	return CSize(lpbi->biWidth, lpbi->biHeight);
}
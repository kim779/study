// image.cpp: implementation of the Cimage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "image.h"

#include "../lib/axislib.h"
#include "../../h/form.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	SUFFIX_ENTER	"_EN"
#define	SUFFIX_DOWN	"_DN"
#define	SUFFIX_OFF	"_OFF"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cimage::Cimage(CAxisDraw* draw, CString fileN, bool only)
{
	m_draw = draw;

	m_images.RemoveAll();
	m_valids.RemoveAll();

	int	offs;
	CString	tmps;
	CBitmap* bitmap;

	if (only)
	{
		bitmap = m_draw->SetBitmap((char *)fileN.operator LPCTSTR());
		m_images.Add(bitmap);
		m_valids.Add(bitmap ? 1 : 0);
	}
	else
	{
		for (int ii = 0; ii < 4; ii++)
		{
			tmps = fileN;
			offs = fileN.ReverseFind('.');
			if (offs != -1)
			{
				tmps = fileN.Left(offs);
				switch (ii)
				{
				case PI_ENTER:
					tmps += SUFFIX_ENTER;	break;
				case PI_DOWN:
					tmps += SUFFIX_DOWN;	break;
				case PI_OFF:
					tmps += SUFFIX_OFF;	break;
				case PI_DEFAULT:
					break;
				}
				tmps += fileN.Mid(offs);
			}

			bitmap = m_draw->SetBitmap((char *)tmps.operator LPCTSTR());
			m_images.Add(bitmap);
			m_valids.Add(bitmap ? 1 : 0);
		}
	}
}

Cimage::~Cimage()
{
	m_images.RemoveAll();
	m_valids.RemoveAll();
}

void Cimage::DrawPicture(CDC* dc, CRect rect, int style, bool oldVers, int index)
{
	BYTE	valid;
	CBitmap* bitmap;
	
	if (!(valid = m_valids.GetAt(index)))
	{
		index = 0;
		valid = m_valids.GetAt(index);
	}

	bitmap = (CBitmap *)m_images.GetAt(index);
	if (valid && bitmap)
	{
		BITMAP	bm;
		CDC	memDC;
		CBitmap* o_bitmap;
		int	ii, kk;
		int	x, y, cx, cy;

		bitmap->GetBitmap(&bm);
		x  = rect.left;
		y  = rect.top;
		cx = bm.bmWidth;
		cy = bm.bmHeight;

		memDC.CreateCompatibleDC(dc);

		switch (style)
		{
		case AL_TILE:
			o_bitmap = (CBitmap *)memDC.SelectObject(bitmap);
			for (ii = 0 ;; ii++, y += bm.bmHeight)
			{
				x  = rect.left;
				cx = bm.bmWidth;
				if (y + bm.bmHeight > rect.bottom)
					cy = rect.bottom - y;
				for (kk = 0 ;; kk++, x += bm.bmWidth)
				{
					if (x + bm.bmWidth > rect.right)
						cx = rect.right - x;
					dc->BitBlt(x, y, cx, cy, &memDC, 0, 0, SRCCOPY);
					if (cx < bm.bmWidth)
						break;
				}
				if (cy < bm.bmHeight)
					break;
			}
			memDC.SelectObject(o_bitmap);
			break;

		case AL_STRETCH:
		//	dc->StretchBlt(x, y, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			if (oldVers)
				transparentBlt(dc, bitmap, x, y, rect.Width(), rect.Height());
			else
			{
				o_bitmap = (CBitmap *)memDC.SelectObject(bitmap);
				::TransparentBlt(dc->m_hDC, x, y, rect.Width(), rect.Height(), memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
				memDC.SelectObject(o_bitmap);
			}
			break;

		case AL_CENTER:
		default:
			if (cx >= rect.Width())
			{
				ii = (cx - rect.Width())/2;
				cx = rect.Width();
			}
			else
			{
				ii = 0;
				x += (rect.Width() - cx)/2;
			}

			if (cy >= rect.Height())
			{
				kk = (cy - rect.Height())/2;
				cy = rect.Height();
			}
			else
			{
				kk = 0;
				y += (rect.Height() - cy)/2;
			}
		//	dc->BitBlt(x, y, cx, cy, &memDC, ii, kk, SRCCOPY);
			if (oldVers)
				transparentBlt(dc, bitmap, x, y, cx, cy);
			else
			{
				o_bitmap = (CBitmap *)memDC.SelectObject(bitmap);
				::TransparentBlt(dc->m_hDC, x, y, cx, cy, memDC.m_hDC, ii, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
				memDC.SelectObject(o_bitmap);
			}
			break;
		}

		memDC.DeleteDC();
	}
}

bool Cimage::GetValid(int index)
{
	if (m_valids.GetSize() <= 0)
		return false;

	return m_valids.GetAt(index) ? true : false;
}

bool Cimage::GetHandle(CBitmap*& bitmap)
{
	if (m_valids.GetSize() > 0 && m_valids.GetAt(0) && m_images.GetSize() > 0)
	{
		bitmap = (CBitmap *)m_images.GetAt(0);
		return true;
	}

	return false;
}

bool Cimage::GetHandle(int index, CBitmap*& bitmap)
{
	if (m_valids.GetSize() > index && m_valids.GetAt(index) && m_images.GetSize() > index)
	{
		bitmap = (CBitmap *)m_images.GetAt(index);
		if (bitmap)
			return true;
	}

	return false;
}

#define SRCMASK			0x00220326    // mask
void Cimage::transparentBlt(CDC *pDC, CBitmap *pBitmap, int x, int y, int cx, int cy)
{
	if (!pBitmap || !pBitmap->GetSafeHandle())
		return;

	CRect	bRc = CRect(x, y, x+cx, y+cy);
	BITMAP		bm;
	pBitmap->GetBitmap(&bm);
	int bx = bm.bmWidth, by = bm.bmHeight;
	CDC		buffDC, maskDC, memoryDC, copyDC;
	CBitmap		buffBitmap, maskBitmap, copyBitmap;
	CBitmap		*oldbuffBitmap, *oldmaskBitmap, *oldmemoryBitmap, *oldcopyBitmap;

	buffDC.CreateCompatibleDC(pDC);
	buffBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldbuffBitmap = buffDC.SelectObject(&buffBitmap);

	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(bx, by, 1, 1, NULL);
	oldmaskBitmap = maskDC.SelectObject(&maskBitmap);

	CRect		maskRc( 0, 0, bx, by);
	buffDC.FillSolidRect(&maskRc, RGB(255,0,255));

	memoryDC.CreateCompatibleDC(pDC);
	oldmemoryBitmap = memoryDC.SelectObject(pBitmap);
	buffDC.BitBlt( 0, 0, bx, by, &memoryDC, 0, 0, SRCCOPY);
	maskDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCCOPY);

	copyDC.CreateCompatibleDC(pDC);
	copyBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldcopyBitmap = copyDC.SelectObject(&copyBitmap);

	copyDC.BitBlt( 0, 0, bx, by, pDC, bRc.left, bRc.top, SRCCOPY);
	copyDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCAND);
	buffDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCMASK);
	copyDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCPAINT);
	pDC->StretchBlt(bRc.left, bRc.top, bRc.Width(), bRc.Height(), &copyDC, 0, 0, bx, by, SRCCOPY);

	copyDC.SelectObject(oldcopyBitmap);
	memoryDC.SelectObject(oldmemoryBitmap);
	maskDC.SelectObject(oldmaskBitmap);
	buffDC.SelectObject(oldbuffBitmap);

	buffDC.DeleteDC();
	maskDC.DeleteDC();
	memoryDC.DeleteDC();
	copyDC.DeleteDC();

	buffBitmap.DeleteObject();
	maskBitmap.DeleteObject();
	copyBitmap.DeleteObject();
}

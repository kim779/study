// tlImage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tlImage.h"
#include "../amlib/amlib.h"
#include "../h/form_w.h"

// CTlImage

CTlImage::CTlImage(class CAmDraw* draw, CString fileN)
{
	m_draw = draw;

	m_image = m_draw->SetBitmap((char *)fileN.operator LPCTSTR());
	m_valid = m_image ? true : false;
}

CTlImage::~CTlImage()
{
}

void CTlImage::DrawPicture(CDC* dc, CRect rect, int style)
{
	if (!m_valid)
		return;

	BITMAP	bm;
	CDC	memDC;
	CBitmap* o_bitmap;
	int	ii, kk;
	int	x, y, cx, cy;

	m_image->GetBitmap(&bm);
	x  = rect.left;
	y  = rect.top;
	cx = bm.bmWidth;
	cy = bm.bmHeight;

	memDC.CreateCompatibleDC(dc);

	switch (style)
	{
	case IA_TILE:
		o_bitmap = (CBitmap *)memDC.SelectObject(m_image);
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

	case IA_STRETCH:
		o_bitmap = (CBitmap *)memDC.SelectObject(m_image);
		::TransparentBlt(dc->m_hDC, x, y, rect.Width(), rect.Height(), memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
		memDC.SelectObject(o_bitmap);
		break;

	case IA_CENTER:
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
		o_bitmap = (CBitmap *)memDC.SelectObject(m_image);
		::TransparentBlt(dc->m_hDC, x, y, cx, cy, memDC.m_hDC, ii, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
		memDC.SelectObject(o_bitmap);
		break;
	}

	memDC.DeleteDC();
}

bool CTlImage::GetHandle(CBitmap*& bitmap)
{
	if (m_valid)
		bitmap = m_image;

	return m_valid;
}

CSize CTlImage::GetSize()
{
	if (m_valid)
	{
		BITMAP	bm;

		m_image->GetBitmap(&bm);
		return CSize(bm.bmWidth, bm.bmHeight);
	}

	return CSize(0, 0);
}
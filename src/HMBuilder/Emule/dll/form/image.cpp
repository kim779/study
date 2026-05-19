// image.cpp: implementation of the Cimage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "image.h"

//#include "../lib/axislib.h"
#include "../../h/form_w.h"
#include "../amLib/AmDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define	SUFFIX_DISABLE	"_ds"
//#define	SUFFIX_DOWN		"_ds"
#define	SUFFIX_SELECT	"_sel"
#define SUFFIX_LONG		"_lt"
//#define	SUFFIX_ENTER	"_EN"
#define	SUFFIX_DOWN		"_dn"
#//define	SUFFIX_OFF		"_XN"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cimage::Cimage(CAmDraw* draw, CString fileN, bool only)
{
	m_draw = draw;

	m_images.RemoveAll();
	m_valids.RemoveAll();

	int	offs;
	CString	tmps;
	Image* pImage;
	if (only)
	{		
		pImage = m_draw->SetImage((char *)fileN.operator LPCTSTR());
		
		m_images.Add(pImage);
		m_valids.Add(pImage ? 1 : 0);
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
					tmps += SUFFIX_SELECT;	break;
				case PI_DOWN:
					tmps += SUFFIX_DOWN;	break;
				case PI_OFF:
					tmps += SUFFIX_DISABLE;	break;
				case PI_LONGTAB:
					tmps += SUFFIX_LONG;	break;
					break;
				case PI_DEFAULT:
					break;
				}
				tmps += fileN.Mid(offs);
			}

			pImage = m_draw->SetImage((char *)tmps.operator LPCTSTR());
			m_images.Add(pImage);
			m_valids.Add(pImage ? 1 : 0);
		}
	}
}

Cimage::~Cimage()
{
	m_images.RemoveAll();
	m_valids.RemoveAll();
}

void Cimage::DrawPicture(CDC *dc, CRect rect, int style, int index)
{
	Graphics g(dc->GetSafeHdc());
	DrawPicture(g, rect, style, index);
}

void Cimage::DrawPicture(Graphics& g, CRect rect, int style, int index)
{
/*
#ifndef _last	// 후에 삭제...
	switch (style)
	{
	case 0:
		style = 1;
		break;
	case 5:
		style = 2;
		break;
	case 6:
		style = 0;
		break;
	default:
		break;
	}
#endif
*/

	Gdiplus::ColorMatrix clrMatrix = { 
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	Gdiplus::ImageAttributes imgAttr;
	imgAttr.SetColorMatrix(&clrMatrix);
	

	
	BYTE	valid;
	Image* pImage;

	if (!(valid = m_valids.GetAt(index)))
	{
		index = 0;
		valid = m_valids.GetAt(index);
	}

	pImage = (Image *)m_images.GetAt(index);
	if (valid && pImage)
	{
		int ii, kk;
		int	x, y, cx, cy;
		x  = rect.left;
		y  = rect.top;
		cx = pImage->GetWidth();
		cy = pImage->GetHeight();

		switch (style)
		{
		case IA_TILE:
			{
#ifdef _DEBUG
				TextureBrush *pBr = ::new TextureBrush(pImage, WrapModeTile);	
#else
				TextureBrush *pBr = new TextureBrush(pImage, WrapModeTile);	
#endif
				Matrix matrix(1, 0, 0, 1, (REAL)rect.left, (REAL)rect.top);  
				pBr->SetTransform(&matrix);
				g.FillRectangle(pBr, RECTG(rect));

#ifdef _DEBUG
				::delete pBr;
#else
				delete pBr;
#endif
			}
			break;
		case IA_STRETCH:			
			g.DrawImage(pImage, RECTF(rect));			
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
			g.DrawImage(pImage, Rect(x, y, cx, cy), ii, 0, pImage->GetWidth(), pImage->GetHeight(), UnitPixel);
		
			break;
		}
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

bool Cimage::GetSize(int& width, int&height)
{
	if (m_valids.GetSize() > 0 && m_valids.GetAt(0) && m_images.GetSize() > 0)
	{
		//bitmap = (CBitmap *)m_images.GetAt(0);
		Image* pImage = m_images.GetAt(0);

		width  = pImage->GetWidth();
		height = pImage->GetHeight();
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

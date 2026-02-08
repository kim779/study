// fxButton.cpp : implementation file
//

#include "stdafx.h"
#include "DxButton.h"
#include "misctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDxButton

CDxButton::CDxButton(CFont* pFont/*=NULL*/)
{
	m_style   = bsTEXT|bsCOLOR;
	m_text    = _T("");
	m_bPress = false;
	m_bitmap  = (CBitmap *)0;
	m_clrBACK = GetSysColor(COLOR_3DFACE);
	m_clrFORE = GetSysColor(COLOR_BTNTEXT);
	m_clrDISBACK = GetSysColor(COLOR_3DFACE);
	m_clrDISFORE = GetSysColor(COLOR_GRAYTEXT);
	m_clrTOGGLE = m_clrBACK;
	m_clrEdge = RGB(11, 94, 149);
	m_clrLight = RGB(235, 243, 255);
	m_clrDark = RGB(140, 162, 194);
	m_clrLight_dn = RGB(242, 248, 255);
	m_clrDark_dn = RGB(38, 132, 255);

	m_bPushed  = false;
	m_parent  = (CWnd *)0;

	m_pFont = pFont;
}

CDxButton::~CDxButton()
{
	if (m_bitmap)
		m_bitmap->DeleteObject();
}

BEGIN_MESSAGE_MAP(CDxButton, CButton)
	//{{AFX_MSG_MAP(CDxButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDxButton message handlers

BOOL CDxButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	dwStyle |= BS_OWNERDRAW; // Enforce
	m_parent = pParentWnd;

	return CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);
}

void CDxButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	if (!IsWindowVisible())
		return;

	CDC	dc;
	CRect	rect, textRc, focusRc;
	CPen	penLight, penDark, penLight_dn, penDark_dn, *oPen;
	
	penLight.CreatePen(PS_SOLID, 2, m_clrLight);
	penDark.CreatePen(PS_SOLID, 1, m_clrDark);
	penLight_dn.CreatePen(PS_SOLID, 1, m_clrLight_dn);
	penDark_dn.CreatePen(PS_SOLID, 2, m_clrDark_dn);

	dc.Attach(lpDIS->hDC);
	rect.CopyRect(&lpDIS->rcItem);
	textRc = rect;
	
	if (m_style & bsCOLOR)
	{
		if ((m_style & bsTOGGLE) && m_bPress)
			dc.FillSolidRect(rect, m_clrTOGGLE);
		else
		{
			if (m_style & bsGRAD)
				drawGradRect(&dc, rect, m_clrBACK, m_clrTOGGLE);
			else	// ÀÏ¹Ý
			{
				if (ODS_DISABLED & lpDIS->itemState)
					dc.FillSolidRect(rect, m_clrDISBACK);
				else
					dc.FillSolidRect(rect, m_clrBACK);
			}
				
		}
	}
	
	
	if (m_style & bsTOGGLE)
	{
		if (m_style & bsGRAD)
		{
			if (m_bPress)
				drawGradRect(&dc, rect, m_clrTOGGLE, m_clrBACK);
			else
				drawGradRect(&dc, rect, m_clrBACK, m_clrTOGGLE);
			dc.Draw3dRect(rect.left, rect.top, rect.right, rect.bottom, RGB(95, 107, 125), RGB(95, 107, 125));
		}
		else
		{
			if (m_bPress)
			{
				oPen = dc.SelectObject(&penDark_dn);
				dc.MoveTo(rect.left+2, rect.bottom-2);
				dc.LineTo(rect.left+2, rect.top+1);
				dc.MoveTo(rect.left+2, rect.top+2);
				dc.LineTo(rect.right-1, rect.top+2);

				oPen = dc.SelectObject(&penLight_dn);
				dc.MoveTo(rect.left+1, rect.bottom-2);
				dc.LineTo(rect.right-1, rect.bottom-2);
				dc.MoveTo(rect.left+2, rect.bottom-3);
				dc.LineTo(rect.right-1, rect.bottom-3);

				dc.MoveTo(rect.right-2, rect.top+2);
				dc.LineTo(rect.right-2, rect.bottom-2);
				dc.MoveTo(rect.right-3, rect.top+3);
				dc.LineTo(rect.right-3, rect.bottom-2);
			}
			else
			{
				oPen = dc.SelectObject(&penLight);
				dc.MoveTo(rect.left+2, rect.bottom-2);
				dc.LineTo(rect.left+2, rect.top+1);
				dc.MoveTo(rect.left+2, rect.top+2);
				dc.LineTo(rect.right-1, rect.top+2);

				oPen = dc.SelectObject(&penDark);
				dc.MoveTo(rect.left+1, rect.bottom-2);
				dc.LineTo(rect.right-1, rect.bottom-2);
				dc.MoveTo(rect.left+2, rect.bottom-3);
				dc.LineTo(rect.right-1, rect.bottom-3);

				dc.MoveTo(rect.right-2, rect.top+2);
				dc.LineTo(rect.right-2, rect.bottom-2);
				dc.MoveTo(rect.right-3, rect.top+3);
				dc.LineTo(rect.right-3, rect.bottom-2);
			}
			dc.Draw3dRect(rect.left, rect.top, rect.right, rect.bottom, m_clrEdge, m_clrEdge);
			dc.SelectObject(oPen);
		}
	}
	else if (m_style & bsPUSH)
	{
		if (m_style & bsGRAD)
		{
			if (m_bPushed)
				drawGradRect(&dc, rect, m_clrTOGGLE, m_clrBACK);
			else
				drawGradRect(&dc, rect, m_clrBACK, m_clrTOGGLE);
			dc.Draw3dRect(rect.left, rect.top, rect.right, rect.bottom, RGB(95, 107, 125), RGB(95, 107, 125));
		}
		else
		{
			if (m_bPushed)
			{
				oPen = dc.SelectObject(&penDark_dn);
				dc.MoveTo(rect.left+2, rect.bottom-2);
				dc.LineTo(rect.left+2, rect.top+1);
				dc.MoveTo(rect.left+2, rect.top+2);
				dc.LineTo(rect.right-1, rect.top+2);

				oPen = dc.SelectObject(&penLight_dn);
				dc.MoveTo(rect.left+1, rect.bottom-2);
				dc.LineTo(rect.right-1, rect.bottom-2);
				dc.MoveTo(rect.left+2, rect.bottom-3);
				dc.LineTo(rect.right-1, rect.bottom-3);

				dc.MoveTo(rect.right-2, rect.top+2);
				dc.LineTo(rect.right-2,
					rect.bottom-2);
				dc.MoveTo(rect.right-3, rect.top+3);
				dc.LineTo(rect.right-3, rect.bottom-2);
			}
			else
			{
				oPen = dc.SelectObject(&penLight);
				dc.MoveTo(rect.left+2, rect.bottom-2);
				dc.LineTo(rect.left+2, rect.top+1);
				dc.MoveTo(rect.left+2, rect.top+2);
				dc.LineTo(rect.right-1, rect.top+2);

				oPen = dc.SelectObject(&penDark);
				dc.MoveTo(rect.left+1, rect.bottom-2);
				dc.LineTo(rect.right-1, rect.bottom-2);
				dc.MoveTo(rect.left+2, rect.bottom-3);
				dc.LineTo(rect.right-1, rect.bottom-3);

				dc.MoveTo(rect.right-2, rect.top+2);
				dc.LineTo(rect.right-2, rect.bottom-2);
				dc.MoveTo(rect.right-3, rect.top+3);
				dc.LineTo(rect.right-3, rect.bottom-2);
			}
			dc.Draw3dRect(rect.left, rect.top, rect.right, rect.bottom, m_clrEdge, m_clrEdge);
			dc.SelectObject(oPen);
		}
	}
	else if (m_style & bsGENERAL)
	{
		if (m_bPushed)
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		else
			dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
	}
	else if (m_style & bsRECT)
	{
		dc.DrawEdge(&rect, EDGE_BUMP, BF_RECT);
	}
	else	// bsFLATE/DEFAULT
	{
		if (m_bPushed)
			dc.FrameRect(&lpDIS->rcItem, CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
		else
			dc.DrawEdge(&rect, EDGE_BUMP, BF_RECT);
	}

	if (m_style & bsIMAGE)
	{
		if (m_bitmap)
		{
			BITMAP	bm;
			UINT	flag = DST_BITMAP;
			CPoint	pt   = CPoint(2, 2);
			m_bitmap->GetBitmap(&bm);
			flag |= DSS_NORMAL;
			dc.DrawState(pt, CSize(bm.bmWidth, bm.bmHeight), m_bitmap, flag);
			textRc.left += (pt.x + bm.bmWidth + 2);
		}
	}

	//m_bPushed && 
	if ((lpDIS->itemAction & ODA_FOCUS || lpDIS->itemState & ODS_FOCUS)) 
	{
		focusRc.CopyRect(rect);
		focusRc.DeflateRect(3, 3);

		if (!(m_style & bsTOGGLE))
			dc.DrawFocusRect(&focusRc);
	}

	if (!m_text.IsEmpty())
	{
		if ((m_style & bsTEXT) || ((m_style & bsIMAGE) && !m_bitmap))
		{
			COLORREF clr;
			int	mode = dc.SetBkMode(TRANSPARENT);

			CGdiObject* font;
			if (m_pFont != NULL) font = dc.SelectObject(m_pFont);
			else font = dc.SelectStockObject(SYSTEM_FONT);
			
			textRc.OffsetRect(0, 1);
			if (m_style & bsTOGGLE)
			{
				if (m_bPress)
					textRc.OffsetRect(1, 1);
			}

			if (ODS_DISABLED & lpDIS->itemState)
			{
				clr = dc.SetTextColor(m_clrDISFORE);
				dc.DrawText(m_text, textRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			else
			{
				clr = dc.SetTextColor(m_clrFORE);
				dc.DrawText(m_text, textRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			dc.SetTextColor(clr);
			dc.SetBkMode(mode);
			dc.SelectObject(font);

		}
		
	}

	dc.Detach();
}

void CDxButton::SetLineColor(COLORREF clrEdge, COLORREF clrLight, COLORREF clrDark, COLORREF clrLight_dn, COLORREF clrDark_dn)
{
	m_clrEdge = clrEdge;
	m_clrLight = clrLight;
	m_clrDark = clrDark;
	m_clrLight_dn = clrLight_dn;
	m_clrDark_dn = clrDark_dn;
}

void CDxButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_style & bsTOGGLE)
		m_bPress = !m_bPress;
	else if (!(m_style & bsRECT))
		m_bPushed = true;
	
	Invalidate(); CButton::OnLButtonDown(nFlags, point);
}

void CDxButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bPushed)
		m_bPushed = false;
	m_bPress = !m_bPress;
	Invalidate();
	CButton::OnLButtonUp(nFlags, point);
}

void CDxButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bPushed)
	{
		ClientToScreen(&point);
		if (WindowFromPoint(point) != this)
		{
			m_bPushed = false;
			Invalidate();
		}
	}
	CButton::OnMouseMove(nFlags, point);
}


void CDxButton::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	Invalidate();	
}

void CDxButton::Init(DWORD style, CString text, CString images, 
		     COLORREF clrBACK, COLORREF clrFORE, COLORREF clrTOGGLE)
{
	bool	redraw = false;

	if (m_style != style)
	{
		m_style = style;
		redraw  = true;
	}
	if (m_style & bsTEXT)
	{
		m_text = text;
		redraw = true;
	}
	if (m_style & bsIMAGE)
	{
		SetBitmap(images, false);
		m_text = text;
		redraw = true;
		ModifyStyle(0, BS_OWNERDRAW);
	}
	if (m_style & bsCOLOR)
	{
		m_clrBACK = clrBACK;
		m_clrFORE = clrFORE;
		m_clrDISBACK = clrBACK;
		m_clrTOGGLE = clrTOGGLE;
		redraw    = true;
		ModifyStyle(0, BS_OWNERDRAW);
	}

	if (redraw)
		Invalidate();
}

void CDxButton::SetStyle(DWORD style, bool redraw)
{
	if  (m_style == style)
		return;

	m_style = style;
	if (redraw)
		Invalidate();
}

void CDxButton::SetText(CString text, bool redraw)
{
	if (!m_text.CompareNoCase(text))
		return;

	SetWindowText(text);
	m_text = text;
	if (redraw)
		Invalidate();
}

void CDxButton::SetBitmap(CString path, bool redraw)
{
	if (m_bitmap)
	{
		m_bitmap->DeleteObject();
		m_bitmap = (CBitmap *) 0;
	}

	HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbitmap)
		m_bitmap = CBitmap::FromHandle(hbitmap);

	if (redraw)
		Invalidate();
}

void CDxButton::SetBkColor(COLORREF clrBACK, bool redraw)
{
	if (m_clrBACK == clrBACK)	return;
	m_clrBACK = clrBACK;
	if (redraw)	Invalidate();
}

void CDxButton::SetFgColor(COLORREF clrFORE, bool redraw)
{
	if (m_clrFORE == clrFORE)	return;
	m_clrFORE = clrFORE;
	if (redraw)	Invalidate();
}

void CDxButton::SetDisableBkColor(COLORREF clrDISBACK, bool redraw)
{
	if (m_clrDISBACK == clrDISBACK)	return;
	m_clrDISBACK = clrDISBACK;
	if (redraw)	Invalidate();
}

void CDxButton::SetDisableFgColor(COLORREF clrDISFORE, bool redraw)
{
	if (m_clrDISFORE == clrDISFORE)	return;
	m_clrDISFORE = clrDISFORE;
	if (redraw)	Invalidate();
}

void CDxButton::SetToggleColor(COLORREF clrTOGGLE, bool redraw)
{
	if (m_clrTOGGLE == clrTOGGLE)
		return;
	m_clrTOGGLE = clrTOGGLE;
	if (redraw)
		Invalidate();
}

void CDxButton::SetToggleStatus(bool bPress, bool redraw)
{
	if (m_bPress == bPress)
		return;
	m_bPress = bPress;
	if (redraw)
		Invalidate();
}

void CDxButton::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;
	if (redraw) Invalidate();
}

void CDxButton::ModifyBtnStyle(DWORD remove, DWORD add, bool redraw)
{
	m_style &= ~remove;
	m_style |= add;
	if (redraw)
		Invalidate();
}

void CDxButton::drawGradRect(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor)
{
	CRect	rect;
	int	nWidth, nHeight;

	drawRC.right += 1;
	drawRC.bottom += 1;
	nWidth = drawRC.Width(); 
	nHeight = drawRC.Height();

	int rs,gs,bs;
	rs = GetRValue(sColor);
	gs = GetGValue(sColor);
	bs = GetBValue(sColor);

	int re,ge,be;
	re = GetRValue(eColor);
	ge = GetGValue(eColor);
	be = GetBValue(eColor);

	float	rStep, gStep, bStep;
	CPen	*sPen;

	if (!nHeight)
	{
		CPen	cPen(PS_SOLID, 1, sColor);
		sPen = pDC->SelectObject(&cPen);
		
		pDC->MoveTo(drawRC.left, drawRC.top);
		pDC->LineTo(drawRC.right, drawRC.bottom);
		pDC->SelectObject(sPen);
		cPen.DeleteObject();
		return;
	}


	rStep = float(rs - re)/float(nHeight);
	gStep = float(gs - ge)/float(nHeight);
	bStep = float(bs - be)/float(nHeight);

	if (!nWidth)
	{
		for (int ii = 0; ii < nHeight; ++ii)
		{
			CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
			sPen = pDC->SelectObject(&cPen);

			pDC->MoveTo(drawRC.left, drawRC.top + ii);
			pDC->LineTo(drawRC.left, drawRC.top + ii + 1);
			pDC->SelectObject(sPen);
			cPen.DeleteObject();
		}
	}
	else
	{
		for (int ii = 0; ii < nHeight; ++ii)
		{
			rect.SetRect(drawRC.left, drawRC.top + ii, drawRC.right, drawRC.top + ii + 1);
			
			CBrush brush;
			brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
			pDC->FillRect(&rect, &brush);
			brush.DeleteObject();
		}
	}
}

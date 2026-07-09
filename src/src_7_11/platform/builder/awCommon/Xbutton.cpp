// xbutton.cpp : implementation file
//

#include "stdafx.h"
#include "xbutton.h"

#include "../../dll/lib/axislib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////]//////////////////////////////////////////////////////
// CXButton

CXButton::CXButton()
{
}

CXButton::~CXButton()
{
}


BEGIN_MESSAGE_MAP(CXButton, CButton)
	//{{AFX_MSG_MAP(CXButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXButton message handlers

BOOL CXButton::Attach(const UINT nID, CWnd* pParent, CAxisPalette* pal, int index, int pRGB, int tRGB)
{
	if (!SubclassDlgItem(nID, pParent))
		return FALSE;

	m_focus   = false;
	m_palette = pal;
	m_pRGB    = pRGB;
	m_tRGB    = tRGB;
	m_text.Format("%d", index);

	return TRUE;
}

void CXButton::SetFocus(bool focus)
{
	m_focus = focus;
	Invalidate();
}

void CXButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CRect	rc;
	CFont	font;
	CBrush	brush;
	CDC*	dc = CDC::FromHandle(lpDIS->hDC);

	font.CreateFont(10, 5, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, "");
	brush.CreateSolidBrush((m_pRGB & 0x02000000) ? m_pRGB : m_palette->GetPaletteRGB(m_pRGB));
	
	CFont*	prevFont  = dc->SelectObject(&font);
	CBrush* prevBrush = dc->SelectObject(&brush);

	rc.CopyRect(&lpDIS->rcItem);
	dc->FillRect(&rc, &brush);
	dc->SetTextColor(m_palette->GetPaletteRGB(m_tRGB));
	dc->SetBkMode(TRANSPARENT);
	dc->DrawText(m_text, rc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	dc->DrawEdge(rc, EDGE_SUNKEN, BF_RECT);
	if (m_focus)
	{
		dc->SelectStockObject(BLACK_PEN);
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(rc);
		rc.DeflateRect(1, 1);
		dc->Rectangle(rc);
	}
	dc->SelectObject(prevFont);
	dc->SelectObject(prevBrush);

	font.DeleteObject();
	brush.DeleteObject();
}
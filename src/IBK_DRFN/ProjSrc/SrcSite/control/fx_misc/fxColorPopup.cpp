// fxColorPopup.cpp : implementation file
//

#include "stdafx.h"
#include "fxColorPopup.h"
#include "memdc.h"
#include "misctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const	int xBTN = 14;
const	int yBTN = 10;

const	int szGAP2 = 2;
const	int szGAP4 = 4;
const	int szPOPUP = 14;
const	int szCLOSE = 10;
const	int szCUSTOM = 20;

static COLORREF _color[nROW][nCOL]  = 
{
	{ RGB(255,255,255),  RGB(224, 224, 224), RGB(192, 192, 192), RGB(128, 128, 128), RGB(64,64,64), RGB(0, 0, 0)},
	{ RGB(255,192,192),  RGB(255, 128, 128), RGB(255, 0, 0),     RGB(192, 0, 0),     RGB(128,0, 0), RGB(64, 0, 0)},
	{ RGB(255,224, 192), RGB(255, 224, 128), RGB(255, 192, 0),   RGB(255, 128, 0),   RGB(128, 64, 0), RGB(128, 64, 64)},
	{ RGB(255,255, 192), RGB(255, 255, 128), RGB(255, 255, 0),   RGB(192, 192, 0),   RGB(128,128, 0), RGB(64, 64, 0)},
	{ RGB(192,255, 192), RGB(128, 255, 128), RGB(0, 255, 0),     RGB(0, 192, 0),     RGB(0, 128, 0), RGB(0, 64, 0)},
	{ RGB(192,255,255),  RGB(128, 255, 255), RGB(0, 255, 255),   RGB(0, 192, 192),   RGB(0,128,128), RGB(0, 64, 64)},
	{ RGB(192,192,255),  RGB(128, 128, 255), RGB(0, 0, 255),     RGB(0, 0, 192),     RGB(0,0,128), RGB(0, 0, 64)},
	{ RGB(255,192,255),  RGB(255, 128, 255), RGB(255, 0, 255),   RGB(192, 0, 192),   RGB(128,0,128), RGB(64, 0, 64)}
};

const int szBORDER = 1;
/////////////////////////////////////////////////////////////////////////////
// CColorBtn

//IMPLEMENT_DYNCREATE(CColorBtn, CObject)

CColorBtn::CColorBtn()
{
	m_winRC = CRect(0, 0, 0, 0);
	m_color = RGB(255, 255, 255);
}

CColorBtn::~CColorBtn()
{
}

/////////////////////////////////////////////////////////////////////////////
// CColorBtn message handlers
void CColorBtn::drawButton(CDC* dc)
{
	dc->SaveDC();

	CBrush brush; brush.CreateSolidBrush(m_color);
	dc->SelectObject(&brush);
	dc->SetBkMode(TRANSPARENT);
	dc->SelectStockObject(NULL_PEN);

	CRect	rect = m_winRC;

	dc->FillSolidRect(rect, GetSysColor(COLOR_BTNFACE));
	rect.DeflateRect(szBORDER, szBORDER, szBORDER, szBORDER);

	dc->Rectangle(rect);
	dc->Draw3dRect(rect, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));

	dc->RestoreDC(-1);
}

void CColorBtn::drawSelectButton(CDC* dc)
{
	CBrush* white = CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH));
	CBrush* black = CBrush::FromHandle((HBRUSH)::GetStockObject(BLACK_BRUSH));

	CRgn rgnOne, rgnTwo, rgnThree;
	
	CRect rect;
	rect = m_winRC; rect.DeflateRect(1,1,1,1);
	
	rgnTwo.CreateRectRgnIndirect(&rect);
	dc->FrameRgn(&rgnTwo, white, 2, 2);
	rect.InflateRect(2, 2, 2, 2);
	
	rgnThree.CreateRectRgnIndirect(&rect);
	dc->FrameRgn(&rgnThree, black, 1, 1);
}

BOOL CColorBtn::HitTest(CPoint point)
{
	CRgn rgn; rgn.CreateRectRgn(m_winRC.left, m_winRC.top, m_winRC.right, m_winRC.bottom);
	return rgn.PtInRegion(point);
}

/////////////////////////////////////////////////////////////////////////////
// CfxColorPopup

CfxColorPopup::CfxColorPopup(CFont* pFont/*=NULL*/)
{
	m_pFont = pFont;
	m_winRC.SetRect(0, 0, 0, 0);

	m_popRC.SetRect(0, 0, 0, 0);
	m_drawRC.SetRect(0, 0, 0, 0);
	m_sepRC.SetRect(0, 0, 0, 0);
	m_customRC.SetRect(0, 0, 0, 0);
	m_customRCx.SetRect(0, 0, 0, 0);

	m_custom = true;
	m_customSEL = false;
	m_colCUR = RGB(0, 0, 0);

	m_wparam = -1;

	m_titleBkCOL = GetSysColor(COLOR_HIGHLIGHT);
	m_titleFgCOL = GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_winBkCOL   = GetSysColor(COLOR_BTNFACE);
}

CfxColorPopup::~CfxColorPopup()
{
}


BEGIN_MESSAGE_MAP(CfxColorPopup, CWnd)
	//{{AFX_MSG_MAP(CfxColorPopup)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfxColorPopup message handlers
BOOL CfxColorPopup::CreateColorPopup(CWnd* parent, COLORREF defaultColor, bool custom)
{
	int width = xBTN * nCOL + szGAP2*5 + szGAP4*2 +1;
	int height = szPOPUP + yBTN * nROW  + szGAP2*7 + szGAP4*2 +1;
	if (custom) height += (szGAP2*2 + szCUSTOM);

	m_winRC.SetRect(0, 0, width, height);

	if (!CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)),
		_T("Color popup"), WS_POPUP|WS_BORDER,
			0, 0, width, height, parent->GetSafeHwnd(), (HMENU) 0, NULL))
	{
		TRACE("Error create color popup...\n");
		return FALSE;
	}

	m_colCUR = defaultColor;
	m_custom = custom;

	initialPopup();

	return TRUE;
}

void CfxColorPopup::DisplayColorPopup(CPoint point, bool show, WPARAM wp)
{
	if (!IsWindow(this->m_hWnd)) return;

	if (show)
		SetWindowPos(&wndTop, point.x, point.y, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE);
	else
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE);

	if (show) SetFocus();

	m_wparam = wp;
}

void CfxColorPopup::SetColorPopup(COLORREF color)
{
	CColorBtn* oldcurBtn = NULL, *curBtn = NULL;
	bool	oldcur = false, cur = false;

	for (int ii = 0; ii < nCOL * nROW; ii++)
	{
		if (m_colBTN[ii].getColor() == m_colCUR)
		{
			oldcur = true; oldcurBtn = &m_colBTN[ii];
		}
		if (m_colBTN[ii].getColor() == color)
		{
			cur = true; curBtn = &m_colBTN[ii];
			if (oldcur && cur) break;
		}
		if (oldcur && cur) break;
	}

	if (curBtn != NULL)
	{
		m_colCUR = curBtn->getColor();
		CRect	rect = curBtn->getRect();

		rect.DeflateRect(1, 1, 1, 1);
		InvalidateRect(&rect);

		if (oldcurBtn != NULL)
		{
			rect = oldcurBtn->getRect();
			rect.DeflateRect(1, 1, 1, 1);
			InvalidateRect(&rect);
		}
	}
}

void CfxColorPopup::initialPopup()
{
	CRect	rect = m_winRC;

	m_popRC = rect;
	m_popRC.bottom = m_popRC.top + szPOPUP;
	rect.top += szPOPUP;

	m_closeRC.left  = m_popRC.right - (szCLOSE+4);
	m_closeRC.top   = m_popRC.top + 2;
	m_closeRC.right = m_closeRC.left + szCLOSE;
	m_closeRC.bottom= m_closeRC.top + szCLOSE;


	if (m_custom)
	{
		m_sepRC = rect;
		m_sepRC.top = rect.bottom - (szCUSTOM+szGAP2*2);
		m_sepRC.bottom = m_sepRC.top + szGAP2;
		m_sepRC.left += szGAP2;
		m_sepRC.right -= szGAP2;
		
		m_customRC = rect;
		m_customRC.top = m_sepRC.bottom;
		m_customRC.left += szGAP2;
		m_customRC.right -= szGAP2;
		m_customRC.bottom = m_customRC.top + szCUSTOM;

		m_customRCx = m_customRC;

		rect.bottom = m_sepRC.top;
	}

	int x = rect.left + szGAP4;
	int y = rect.top  + szGAP4;

	for (int ii = 0; ii < nROW; ii++)
	{
		for (int jj = 0; jj < nCOL; jj++)
		{
			m_colBTN[ii*nCOL+jj].setRect(CRect(x, y, x+xBTN, y+yBTN));
			m_colBTN[ii*nCOL+jj].setColor(_color[ii][jj]);
			x += (xBTN + szGAP2);
		}

		x  = rect.left + szGAP4;
		y += yBTN + szGAP2;
	}
}

void CfxColorPopup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC memDC(&dc);

	memDC.FillSolidRect(m_winRC, m_winBkCOL);
	memDC.FillSolidRect(m_popRC, m_titleBkCOL);

	drawCaption(memDC);

	CRect	rect;
	CColorBtn* button = NULL;
	for (int ii = 0; ii < nROW * nCOL; ii++)
	{
		m_colBTN[ii].drawButton(&memDC);
		if (m_colBTN[ii].getColor() == m_colCUR)
			button = &m_colBTN[ii];
	}

	if (button != NULL)
	{
		button->drawSelectButton(&memDC);

		if (m_custom)
		{
			memDC.Draw3dRect(m_sepRC, ::GetSysColor(COLOR_BTNSHADOW), RGB(255, 255, 255));
			drawCustom(&memDC, false);
			if (m_customSEL) drawCustomSelect(&memDC);
		}
	}
	else
	{
		if (m_custom)
		{
			memDC.Draw3dRect(m_sepRC, ::GetSysColor(COLOR_BTNSHADOW), RGB(255, 255, 255));
			drawCustom(&memDC, true);
			if (m_customSEL) drawCustomSelect(&memDC);
		}
	}
}

void CfxColorPopup::drawCaption(CDC* dc)
{
	CRect rc;
	CPen Pen, *pOldPen;

	CGdiObject* sFont;
	if (m_pFont != NULL) sFont = dc->SelectObject(m_pFont);
	else sFont = dc->SelectStockObject(SYSTEM_FONT);

	Pen.CreatePen(PS_SOLID, 0, GetSysColor(COLOR_HIGHLIGHTTEXT));
	pOldPen = (CPen*)dc->SelectObject(&Pen);

	rc = m_popRC;
	rc.DeflateRect(2, 0, 16, 0);
	dc->SetBkMode(TRANSPARENT);
	dc->SetTextColor(m_titleFgCOL);
	dc->DrawText("COLOR", rc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	// close button
	rc = m_closeRC;
	dc->Rectangle(rc); rc.DeflateRect(1, 1);

	dc->FillSolidRect(rc, m_titleBkCOL);
	rc.InflateRect(1, 1);
	dc->MoveTo(rc.left, rc.top);
	dc->LineTo(rc.right - 1, rc.bottom - 1);
	dc->MoveTo(rc.right - 1, rc.top);
	dc->LineTo(rc.left, rc.bottom - 1);

	dc->SelectObject(pOldPen);
	Pen.DeleteObject();
	dc->SelectObject(sFont);
}

void CfxColorPopup::drawCustom(CDC* dc, bool hasColor)
{
	ASSERT_VALID(dc);
	dc->FillSolidRect(m_customRCx, m_winBkCOL);

	m_customRC = m_customRCx;
	CRect rect = m_customRC, rcColor = m_customRC;
	
	rcColor.left = rect.right - xBTN;
	rcColor.top += szGAP2; rcColor.bottom -= szGAP2;

	rect.right = rcColor.left;
	m_customRC = rect;

	rect.DeflateRect(szGAP2, szGAP2, szGAP2, szGAP2);

	CPen pen(PS_SOLID, 1, m_winBkCOL);
	CPen* oldpen = dc->SelectObject(&pen);

	CGdiObject* oldFont;
	if (m_pFont != NULL) oldFont = dc->SelectObject(m_pFont);
	else oldFont = dc->SelectStockObject(SYSTEM_FONT);

	dc->SetBkMode(TRANSPARENT);
	dc->SetTextCharacterExtra(0);
	dc->DrawText("User Color", rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);

	dc->SelectObject(oldFont);
	dc->SelectObject(oldpen);

	if (hasColor)
	{
		dc->Draw3dRect(rcColor, ::GetSysColor(COLOR_BTNSHADOW), RGB(255, 255, 255));
		rcColor.DeflateRect(szGAP2, szGAP2, szGAP2, szGAP2);
		dc->FillSolidRect(rcColor, m_colCUR);
		dc->Draw3dRect(rcColor, ::GetSysColor(COLOR_BTNSHADOW), RGB(255, 255, 255));
	}
}

void CfxColorPopup::drawCustomSelect(CDC* dc)
{
	ASSERT_VALID(dc);

	CBrush* white = CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH));
	CBrush* black = CBrush::FromHandle((HBRUSH)::GetStockObject(BLACK_BRUSH));
	
	CRgn rgnOne, rgnTwo, rgnThree;
	
	CRect rect;
	rect = m_customRC;
	rect.DeflateRect(1,1,1,1);
	
	rgnTwo.CreateRectRgnIndirect(&rect);
	dc->FrameRgn(&rgnTwo, white, szGAP2, szGAP2);
	
//	rect.InflateRect(szGAP2, szGAP2, szGAP2, szGAP2);
	
	rgnThree.CreateRectRgnIndirect(&rect);
	dc->FrameRgn(&rgnThree, black, 1, 1);
}

void CfxColorPopup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);
	if (m_closeRC.PtInRect(point))
	{
		GetParent()->SendMessage(WM_COLORPOPUP, m_wparam, (LPARAM)0);
		ShowWindow(SW_HIDE); return;
	}

	if (m_popRC.PtInRect(point))
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	}
}

void CfxColorPopup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_customSEL)
	{
		m_customSEL = false;

		CRect rc = m_customRC;
		rc.DeflateRect(szGAP2, szGAP2, szGAP2, szGAP2);
		InvalidateRect(&rc);
	}

	for (int ii = 0; ii < nROW * nCOL; ii++)
	{
		if (m_colBTN[ii].HitTest(point))
		{
			m_colCUR = m_colBTN[ii].getColor();
			GetParent()->SendMessage(WM_COLORPOPUP, m_wparam, (LPARAM)m_colCUR);
			ShowWindow(SW_HIDE);
			return;
		}
	}
	
	if (m_custom && m_customRC.PtInRect(point))
	{
		ShowWindow(SW_HIDE);

		CColorDialog dlg;
		
		dlg.m_cc.Flags |= CC_RGBINIT|CC_FULLOPEN;
		dlg.m_cc.rgbResult = RGB(GetRValue(m_colCUR), GetGValue(m_colCUR), GetBValue(m_colCUR));

		if (dlg.DoModal() == IDOK)
		{
			m_colCUR = dlg.GetColor();
			GetParent()->SendMessage(WM_COLORPOPUP, m_wparam, (LPARAM)m_colCUR);
		}
		else
		{
			ShowWindow(SW_SHOW); return;
		}
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CfxColorPopup::OnMouseMove(UINT nFlags, CPoint point) 
{
	CColorBtn* selBtn = HitTest(point);
	if (selBtn == NULL && m_customRC.PtInRect(point))
	{
		m_customSEL = true;
		CRect rect = m_customRC;
		rect.DeflateRect(szGAP2, szGAP2, szGAP2, szGAP2);
		InvalidateRect(&rect);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CfxColorPopup::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int curCOL = 0, newCOL = 0;
	for (int ii = 0; ii < nROW*nCOL; ii++)
	{
		if (m_colBTN[ii].getColor() == m_colCUR)
		{
			curCOL = ii;
			break;
		}
	}

	newCOL = curCOL;

	switch (nChar)
	{
	case VK_LEFT:
		if (curCOL == 0)
			newCOL = nROW*nCOL -1;
		else if (curCOL >= 0)
			newCOL -= 1;

		SetColorPopup(m_colBTN[newCOL].getColor());
		break;
	case VK_RIGHT:
		if (curCOL >= nROW*nCOL-1)
			newCOL = 0;
		else if (curCOL < nROW*nCOL-1)
			newCOL += 1;
		SetColorPopup(m_colBTN[newCOL].getColor());
		break;
	case VK_DOWN:
		if (curCOL == nROW*nCOL-1)
			newCOL = 0;
		else if (curCOL < nROW*nCOL-1)
			newCOL += nCOL;
		if (newCOL > nROW*nCOL-1)
			newCOL -= (nROW*nCOL);
		SetColorPopup(m_colBTN[newCOL].getColor());
		break;
	case VK_UP:
		if (curCOL == 0)
			newCOL = nROW*nCOL-1;
		else if (curCOL>= 0)
			newCOL -= nCOL;

		if (newCOL < 0)
			newCOL += (nROW*nCOL);
		SetColorPopup(m_colBTN[newCOL].getColor());
		break;
	case VK_RETURN:
		GetParent()->SendMessage(WM_COLORPOPUP, m_wparam, (LPARAM)m_colCUR);
	case VK_ESCAPE:
		ShowWindow(SW_HIDE);
		GetParent()->SendMessage(WM_COLORPOPUP, m_wparam, (LPARAM)0);
		break;
	default:
		break;
	}
}

CColorBtn* CfxColorPopup::HitTest(CPoint point)
{
	CColorBtn *selBtn = NULL, *hitBtn = NULL;
	bool	bsel = false, bhit = false;

	for (int ii = 0; ii < nROW*nCOL; ii++)
	{
		if (m_colBTN[ii].getColor() == m_colCUR)
		{
			bsel = true; selBtn = &m_colBTN[ii];
		}
		if (m_colBTN[ii].HitTest(point))
		{
			bhit = true; hitBtn = &m_colBTN[ii];
		}
		if (bsel && bhit) break;
	}

	if (hitBtn != NULL)
	{
		m_colCUR = hitBtn->getColor();
		CRect rc = hitBtn->getRect();
		rc.DeflateRect(szGAP2, szGAP2, szGAP2, szGAP2);
		InvalidateRect(&rc);
	}

	if (selBtn != NULL)
	{
		CRect rc = selBtn->getRect();
		rc.DeflateRect(szGAP2, szGAP2, szGAP2, szGAP2);
		InvalidateRect(&rc);
	}
	return hitBtn;
}
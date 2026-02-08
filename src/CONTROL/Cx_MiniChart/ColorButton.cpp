// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "Cx_MiniChart.h"
#include "ColorButton.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorButton

CColorButton::CColorButton()
{
	m_bDrawBong = false;
	m_bGradient = false;
	m_bFill = true;
}

CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorButton message handlers
BOOL CColorButton::Attach(const UINT nID, CWnd* pParent, const COLORREF BGColor, const COLORREF FGColor, const COLORREF DisabledColor, const UINT nBevel)
{
	//dynamic attach!!
	if (!SubclassDlgItem(nID, pParent))	return FALSE;

	m_fg	= FGColor;
	m_bg	= BGColor; 
	m_bevel = nBevel;
	m_disabled	= DisabledColor;

	return TRUE;
} 

void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	const UINT state = lpDIS->itemState; 
	CRect focusRect, btnRect;
	focusRect.CopyRect(&lpDIS->rcItem); 
	btnRect.CopyRect(&lpDIS->rcItem); 
	// Set the focus rectangle to just past the border decoration
	focusRect.left += 4;
	focusRect.right -= 4;
	focusRect.top += 4;
	focusRect.bottom -= 4;
	// Retrieve the button's caption
	const int bufSize = 512;
	TCHAR buffer[bufSize];
	GetWindowText(buffer, bufSize);
	// Draw and label the button using draw methods 
	DrawFilledRect(pDC, btnRect, GetBGColor()); 
	DrawFrame(pDC, btnRect, m_bevel);
  	DrawButtonText(pDC, btnRect, buffer, GetFGColor());

	// Now, depending upon the state, redraw the button (down image) if it is selected,
	// place a focus rectangle on it, or redisplay the caption if it is disabled

	if (state & ODS_FOCUS) 
	{
		DrawFocusRect(lpDIS->hDC, (LPRECT)&focusRect);
		if (state & ODS_SELECTED)
		{ 
    			DrawFilledRect(pDC, btnRect, GetBGColor()); 
    			DrawFrame(pDC, btnRect, -1);
			DrawButtonText(pDC, btnRect, buffer, GetFGColor());
			DrawFocusRect(lpDIS->hDC, (LPRECT)&focusRect);
		}
	}
	else if (state & ODS_DISABLED) 
	{
  		DrawButtonText(pDC, btnRect, buffer, GetDisabledColor());
	}
} 

void CColorButton::DrawFrame(CDC *DC, CRect R, int Inset)
{ 
	COLORREF	dark{},light{},tlColor{},brColor{};
	int		i{}, m{}, width{};

	width = (Inset < 0)? -Inset : Inset;
	for (i = 0; i < width; i += 1)
	{
		m = 255 / (i + 2);
		dark = PALETTERGB(m, m, m);
		m = 192 + (63 / (i + 1));
		light = PALETTERGB(m, m, m);
		  
	  	if ( width == 1 )
		{
			light = PALETTERGB(255, 255, 255);
			dark = PALETTERGB(128, 128, 128);
		}
		
		if ( Inset < 0 )
		{
			tlColor = dark;
			brColor = light;
		}
		else
		{
			tlColor = light;
			brColor = dark;
		}
		
		DrawLine(DC, R.left, R.top, R.right, R.top, tlColor);							// Across top
		DrawLine(DC, R.left, R.top, R.left, R.bottom, tlColor);							// Down left
	  
		if ( (Inset < 0) && (i == width - 1) && (width > 1) ) 
		{
			DrawLine(DC, R.left + 1, R.bottom - 1, R.right, R.bottom - 1, PALETTERGB(1, 1, 1));// Across bottom
			DrawLine(DC, R.right - 1, R.top + 1, R.right - 1, R.bottom, PALETTERGB(1, 1, 1));	// Down right
		}
	  	else 
		{
			DrawLine(DC, R.left + 1, R.bottom - 1, R.right, R.bottom - 1, brColor);		// Across bottom
			DrawLine(DC, R.right - 1, R.top + 1, R.right - 1, R.bottom, brColor);		// Down right
		}
	  	InflateRect(R, -1, -1);
	}
}

void CColorButton::DrawFilledRect(CDC *DC, CRect R, COLORREF color)
{ 
	CBrush B;
	B.CreateSolidBrush(color);
	DC->FillRect(R, &B);
}

void CColorButton::DrawLine(CDC *DC, CRect EndPoints, COLORREF color)
{ 
	CPen newPen;
	newPen.CreatePen(PS_SOLID, 1, color);
	CPen *oldPen = DC->SelectObject(&newPen);
	DC->MoveTo(EndPoints.left, EndPoints.top);
	DC->LineTo(EndPoints.right, EndPoints.bottom);
	DC->SelectObject(oldPen);
	newPen.DeleteObject();
}

void CColorButton::DrawLine(CDC *DC, long left, long top, long right, long bottom, COLORREF color)
{ 
	CPen newPen;
	newPen.CreatePen(PS_SOLID, 1, color);
	CPen *oldPen = DC->SelectObject(&newPen);
	DC->MoveTo(left, top);
	DC->LineTo(right, bottom);
	DC->SelectObject(oldPen);
	newPen.DeleteObject();
}

void CColorButton::DrawButtonText(CDC *DC, CRect R, const char *Buf, COLORREF TextColor)
{
	if (!m_bDrawBong)
	{
		const COLORREF prevColor = DC->SetTextColor(TextColor);
		DC->SetBkMode(TRANSPARENT);
		DC->DrawText(Buf, strlen(Buf), R, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		DC->SetTextColor(prevColor);

		return;
	}

	CRect	rect = R;
	const int	gap = 3;

	rect.DeflateRect(1, R.Height() / 4);
	CPen	cpen(PS_SOLID, 1, m_gradient);
	CPen	*sPen = DC->SelectObject(&cpen);
	if (m_bFill)
	{
		DC->MoveTo(R.left + R.Width()/2, R.top + gap);
		DC->LineTo(R.left + R.Width()/2, R.bottom - gap);
	}
	else
	{
		CPoint	pPt[10];
		pPt[0] = CPoint(R.left + R.Width()/2, R.top + gap);
		pPt[1] = CPoint(R.left + R.Width()/2, rect.top);
		pPt[2] = CPoint(rect.left, rect.top);
		pPt[3] = CPoint(rect.left, rect.bottom);
		pPt[4] = CPoint(R.left + R.Width()/2, rect.bottom);
		pPt[5] = CPoint(R.left + R.Width()/2, R.bottom - gap);
		pPt[6] = pPt[4];
		pPt[7] = CPoint(rect.right, rect.bottom);
		pPt[8] = CPoint(rect.right, rect.top);
		pPt[9] = pPt[1];

		DC->Polyline(pPt, 10);
	}
	cpen.DeleteObject();
	DC->SelectObject(sPen);
}

void CColorButton::SetNewColor(COLORREF bg,COLORREF fg, COLORREF DisabledColor)
{
	m_fg = fg;
	m_bg = bg; 
	m_disabled = DisabledColor;
	Invalidate();
}


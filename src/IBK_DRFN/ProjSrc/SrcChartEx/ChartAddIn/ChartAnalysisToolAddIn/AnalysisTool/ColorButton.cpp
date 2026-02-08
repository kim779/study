
// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "ColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CColorButton

CColorButton::CColorButton():
	m_color(RGB(0, 0, 0))
{
}

CColorButton::CColorButton(const COLORREF& color):
	m_color(color)
{
}

BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CColorButton message handlers

void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	UINT uStyle = DFCS_BUTTONPUSH;

	// This code only works with buttons.
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

	// If drawing selected, add the pushed style to DrawFrameControl.
	if(lpDrawItemStruct->itemState & ODS_SELECTED)
		uStyle |= DFCS_PUSHED;
	
	// Draw the button frame.
	::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, DFC_BUTTON, uStyle);

	DrawRectangle(CDC::FromHandle(lpDrawItemStruct->hDC));
}

///////////////////////////////////////////////////////////////////////////////
// public =====================================================================
void CColorButton::SetColor(const COLORREF &color)
{
	m_color = color;
}

COLORREF CColorButton::GetColor() const
{
	return m_color;
}

// private ====================================================================
COLORREF CColorButton::GetComplementaryColor(const COLORREF &color) const
{
	return RGB(GetComplementaryColor(GetRValue(color)), GetComplementaryColor(GetGValue(color)), GetComplementaryColor(GetBValue(color)));
}

int CColorButton::GetComplementaryColor(const int nColor) const
{
	return (255 - nColor);
}

// ----------------------------------------------------------------------------
void CColorButton::DrawRectangle(CDC* pDC)
{
	CBrush brush(m_color);
	CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brush);
	pDC->SelectStockObject(NULL_PEN);
	CRect region(0, 0, 0, 0);
	GetClientRect(region);
	pDC->Rectangle(&region);
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
}

void CColorButton::DrawText(CDC* pDC, const CString& strText)
{
	COLORREF oldBkColor = pDC->SetBkColor(m_color);
	COLORREF oldTextColor = pDC->SetTextColor(GetComplementaryColor(m_color));
	CRect region(0, 0, 0, 0);
	GetClientRect(region);
	pDC->DrawText(strText, strText.GetLength(), region, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	pDC->SetBkColor(oldBkColor);
	pDC->SetTextColor(oldTextColor);
}
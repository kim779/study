// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "ColorButton.h"
// 2008.02.20 by LYH >>
#include "../resource.h"
// 2008.02.20 by LYH <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorButton

CColorButton::CColorButton()
{
// 2008.01.25 by LYH >>
	//m_strBtnText = _T("»ö ¼³Á¤");
	m_strBtnText.LoadString(IDS_COLOR_SET);
// 2008.01.25 by LYH <<
	m_bEnable = TRUE;
}

CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorButton message handlers

void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
   UINT uStyle = DFCS_BUTTONPUSH;

   // This code only works with buttons.
   ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

   // If drawing selected, add the pushed style to DrawFrameControl.
   if (lpDrawItemStruct->itemState & ODS_SELECTED)
      uStyle |= DFCS_PUSHED;

   // Draw the button frame.
   ::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, 
      DFC_BUTTON, uStyle);

    // Draw the button text using the text color red.
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CBrush brush(m_color);
    CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brush);
	pDC->SelectStockObject(NULL_PEN);
    pDC->Rectangle(&lpDrawItemStruct->rcItem);
    pDC->SelectObject(pOldBrush);

	//	
	COLORREF crOldBkColor = ::SetBkColor(lpDrawItemStruct->hDC, m_color);
	//	COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, GetComplementaryColor(m_color));

	if(m_bEnable)
	{
		COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, RGB(255, 255, 255));
		::DrawText(lpDrawItemStruct->hDC, m_strBtnText, m_strBtnText.GetLength(), 
			&lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		::SetTextColor(lpDrawItemStruct->hDC, crOldColor); 
		::SetBkColor(lpDrawItemStruct->hDC, crOldBkColor); 
	}
}

void CColorButton::SetColor(const COLORREF &color)
{
	m_color = color;
}

BOOL CColorButton::EnableWindow(BOOL bEnable /*= TRUE*/)
{
	m_bEnable = bEnable;
	return CButton::EnableWindow(bEnable);
}


void CColorButton::SetBtnText(const char* p_pBtnText)
{
	m_strBtnText.Format("%s", p_pBtnText);
}

CString CColorButton::GetBtnText()
{
	return m_strBtnText;
}

// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "ColorButton.h"
// 2008.01.25 by LYH >>
#include "resource.h"
// 2008.01.25 by LYH <<
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorButton

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
CColorButton::CColorButton()
{
// 2008.01.25 by LYH >>
//	m_strText = "색 설정";
	m_strText.LoadString(IDS_COLOR_SET);
// 2008.01.25 by LYH <<
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Return void : 
// Param  LPDRAWITEMSTRUCT lpDrawItemStruct : 
// Comments		: 
//-----------------------------------------------------------------------------
void CColorButton::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct ) 
{
   UINT uStyle = DFCS_BUTTONPUSH;

   // This code only works with buttons.
   ASSERT( lpDrawItemStruct->CtlType == ODT_BUTTON );

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

	// text
	COLORREF crOldBkColor = ::SetBkColor( lpDrawItemStruct->hDC, m_color );
	COLORREF crOldColor = ::SetTextColor( lpDrawItemStruct->hDC, RGB( 255, 255, 255 ) );
    
	::DrawText( lpDrawItemStruct->hDC, m_strText, m_strText.GetLength(), &lpDrawItemStruct->rcItem,
		DT_SINGLELINE | DT_VCENTER | DT_CENTER );
    
	::SetTextColor( lpDrawItemStruct->hDC, crOldColor ); 
    ::SetBkColor( lpDrawItemStruct->hDC, crOldBkColor ); 
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Return void : 
// Param  const COLORREF &color : 
// Comments		: 
//-----------------------------------------------------------------------------
void CColorButton::SetColor( const COLORREF &color )
{
	m_color = color;
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 10 / 11
// Return  void : 
// Param  CString szBtnText   : 
// Explain		: 색상 버튼에 표시될 택스트를 설정한다.
//-----------------------------------------------------------------------------
void CColorButton::SetText( CString szBtnText )
{
	m_strText = szBtnText;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Return COLORREF : 
// Param  COLORREF &color : 
// Comments		: 
//-----------------------------------------------------------------------------
COLORREF CColorButton::GetComplementaryColor( COLORREF &color )
{
	int rValue = GetRValue(color);
	int gValue = GetGValue(color);
	int bValue = GetBValue(color);
	
	return RGB(255 - rValue, 255 - gValue, 255- bValue);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Return COLORREF : 
// Param  void : 
// Comments		: 
//-----------------------------------------------------------------------------
COLORREF CColorButton::GetColor( void )
{
	return m_color;
}

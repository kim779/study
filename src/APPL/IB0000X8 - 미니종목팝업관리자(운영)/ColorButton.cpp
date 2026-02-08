// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000X1.h"
#include "ColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorButton


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
	
	// Get the button's text.
	CString strText;
	GetWindowText(strText);
	
	CRect rc = lpDrawItemStruct->rcItem;
	rc.DeflateRect(2, 2);
	::SetBkColor(lpDrawItemStruct->hDC, m_crBk);
	::ExtTextOut(lpDrawItemStruct->hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);

	// Draw the button text using the text color red.
	const COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, RGB(0,0,0));
	::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), 
		&lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
   ::SetTextColor(lpDrawItemStruct->hDC, crOldColor);
}

BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorButton message handlers

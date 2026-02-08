#include "stdafx.h"
#include "BtnST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonST

// Mask for control's type
 #define XBS_TYPEMASK SS_TYPEMASK

#ifndef	TTM_SETTITLE
#define TTM_SETTITLEA           (WM_USER + 32)  // wParam = TTI_*, lParam = char* szTitle
#define TTM_SETTITLEW           (WM_USER + 33)  // wParam = TTI_*, lParam = wchar* szTitle
#ifdef	UNICODE
#define TTM_SETTITLE            TTM_SETTITLEW
#else
#define TTM_SETTITLE            TTM_SETTITLEA
#endif
#endif

#ifndef	TTS_BALLOON
#define	TTS_BALLOON		0x40
#endif

CButtonST::CButtonST()
{
	m_bIsPressed		= FALSE;
	m_bIsFocused		= FALSE;
	m_bIsDisabled		= FALSE;
	m_bMouseOnButton	= FALSE;
	m_bSelected		= FALSE;
	m_bNoFocus		= FALSE;

	// Default type is "flat" button
	m_bIsFlat = TRUE;
  
	// By default use usual pressed style
	SetPressedStyle(BTNST_PRESSED_LEFTRIGHT, FALSE);
  
	// By default the button is not the default button
	m_bIsDefault = FALSE;
	// Invalid value, since type still unknown
	m_nTypeStyle = XBS_TYPEMASK;

	// By default the button is not a checkbox
	m_bIsCheckBox = FALSE;
	m_nCheck = 0;

	// Set default colors
	SetDefaultColors(FALSE);

	m_Font.CreatePointFont(9*10, "돋움");
	m_boldFont.CreateFont(12, 0, 0, 0, FW_SEMIBOLD,
				FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
				OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
				"돋움" );
} // End of CButtonST

CButtonST::~CButtonST()
{
	m_Font.DeleteObject();
	m_boldFont.DeleteObject();
	
} // End of ~CButtonST

BEGIN_MESSAGE_MAP(CButtonST, CButton)
    //{{AFX_MSG_MAP(CButtonST)
	ON_WM_KILLFOCUS()
	ON_WM_SYSCOLORCHANGE()
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_WM_ACTIVATE()
	ON_WM_ENABLE()
	ON_WM_CANCELMODE()
	ON_WM_GETDLGCODE()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BM_SETSTYLE, OnSetStyle)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(BM_SETCHECK, OnSetCheck)
	ON_MESSAGE(BM_GETCHECK, OnGetCheck)
END_MESSAGE_MAP()

void CButtonST::PreSubclassWindow() 
{
	const	UINT	nBS = GetButtonStyle();

	// Set initial control type
	m_nTypeStyle = nBS & XBS_TYPEMASK;

	// Check if this is a checkbox
	if (nBS & BS_CHECKBOX) m_bIsCheckBox = TRUE;

	// Set initial default state flag
	if (m_nTypeStyle == BS_DEFPUSHBUTTON)
	{
		// Set default state for a default button
		m_bIsDefault = TRUE;

		// Adjust style for default button
		m_nTypeStyle = BS_PUSHBUTTON;
	}

	// You should not set the Owner Draw before this call
	// (don't use the resource editor "Owner Draw" or
	// ModifyStyle(0, BS_OWNERDRAW) before calling PreSubclassWindow() )
	ASSERT(m_nTypeStyle != BS_OWNERDRAW);

	// Switch to owner-draw
	ModifyStyle(XBS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);
	
	CButton::PreSubclassWindow();
} // End of PreSubclassWindow

UINT CButtonST::OnGetDlgCode() 
{
	UINT nCode = CButton::OnGetDlgCode();

	// Tell the system if we want default state handling
	// (losing default state always allowed)
	nCode |= (m_bIsDefault ? DLGC_DEFPUSHBUTTON : DLGC_UNDEFPUSHBUTTON);

	return nCode;
} // End of OnGetDlgCode

HBRUSH CButtonST::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
} // End of CtlColor

void CButtonST::OnSysColorChange() 
{
	CButton::OnSysColorChange();

	SetDefaultColors();
} // End of OnSysColorChange

LRESULT CButtonST::OnSetStyle(WPARAM wParam, LPARAM lParam)
{
	const	UINT	nNewType = (wParam & XBS_TYPEMASK);

	// Update default state flag
	if (nNewType == BS_DEFPUSHBUTTON)
	{
		m_bIsDefault = TRUE;
	}
	else if (nNewType == BS_PUSHBUTTON)
	{
		// Losing default state always allowed
		m_bIsDefault = FALSE;
	}

	// Can't change control type after owner-draw is set.
	// Let the system process changes to other style bits
	// and redrawing, while keeping owner-draw style
	return DefWindowProc(BM_SETSTYLE, (wParam & ~XBS_TYPEMASK) | BS_OWNERDRAW, lParam);
} // End of OnSetStyle

LRESULT CButtonST::OnSetCheck(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_bIsCheckBox);

	switch (wParam)
	{
	case BST_CHECKED:
	case BST_INDETERMINATE:	// Indeterminate state is handled like checked state
		SetCheck(1);
		break;
	default:
		SetCheck(0);
		break;
	} // switch

	return 0;
} // End of OnSetCheck

LRESULT CButtonST::OnGetCheck(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_bIsCheckBox);
	
	return GetCheck();
} // End of OnGetCheck

void CButtonST::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
	if (bEnable == FALSE)	
	{
		CWnd*	pWnd = GetParent()->GetNextDlgTabItem(this);
		if (pWnd)
			pWnd->SetFocus();
		else
			GetParent()->SetFocus();

		CancelHover();
	} // if
} // End of OnEnable

void CButtonST::OnKillFocus(CWnd * pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	CancelHover();
	Invalidate();
} // End of OnKillFocus

void CButtonST::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CButton::OnActivate(nState, pWndOther, bMinimized);
	if (nState == WA_INACTIVE)	CancelHover();
} // End of OnActivate

void CButtonST::OnCancelMode() 
{
	CButton::OnCancelMode();
	CancelHover();
} // End of OnCancelMode

void CButtonST::CancelHover()
{
	// Only for flat buttons
	if (m_bIsFlat)
	{
		if (m_bMouseOnButton)
		{
			m_bMouseOnButton = FALSE;
			Invalidate();
		}
	}
} // End of CancelHover

void CButtonST::OnMouseMove(UINT nFlags, CPoint point)
{
	const	CWnd* wndActive = this;
	CWnd*	wndUnderMouse = nullptr;
	TRACKMOUSEEVENT	csTME{};
	CRect	rect;

	CButton::OnMouseMove(nFlags, point);
	
	GetClientRect(&rect);

	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);

	// If the mouse enter the button with the left button pressed then do nothing
	if (nFlags & MK_LBUTTON && m_bMouseOnButton == FALSE) return;

	// If our button is not flat then do nothing
	if (m_bIsFlat == FALSE) return;

	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_bMouseOnButton)
		{
			m_bMouseOnButton = TRUE;

			Invalidate();

			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
			
		}
	}
	else	CancelHover();
} // End of OnMouseMove


// Handler for WM_MOUSELEAVE
LRESULT CButtonST::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	CancelHover();

	return 0;
} // End of OnMouseLeave

BOOL CButtonST::OnClicked() 
{	
	SetFocus();
	if (m_bIsCheckBox)
	{
		m_nCheck = !m_nCheck;
		Invalidate();
	}

	return FALSE;
} // End of OnClicked

void CButtonST::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC*	pDC = CDC::FromHandle(lpDIS->hDC);
	CFont	*poldfont = (CFont*)pDC->SelectObject(&m_Font);
	
	// Checkbox...?
	if (m_bIsCheckBox)
	{
		m_bIsPressed  =  (lpDIS->itemState & ODS_SELECTED) || (m_nCheck != 0);
	}
	else	// Normal button OR other button style ...
	{
		m_bIsPressed = (lpDIS->itemState & ODS_SELECTED);
		
	}
	
	if (!m_bNoFocus)
		m_bIsFocused  = (lpDIS->itemState & ODS_FOCUS);
	m_bIsDisabled = (lpDIS->itemState & ODS_DISABLED);

	CRect	itemRect = lpDIS->rcItem;

	//itemRect.DeflateRect(2,2);

	pDC->SetBkMode(TRANSPARENT);

	// Prepare draw... paint button background

	OnDrawBackground(pDC, &itemRect);

	//.......... Read the button's title
	CString sTitle;
	GetWindowText(sTitle);

	//............ Write the button title (if any)
	if (!sTitle.IsEmpty())
	{
		CRect captionRect = lpDIS->rcItem;
		DrawTheText(pDC, sTitle.GetString(), &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
	}

	pDC->SelectObject(poldfont);
} // End of DrawItem

//
// Parameters:
//		[IN]	bHasTitle
//				TRUE if the button has a text
//		[IN]	rpItem
//				A pointer to a RECT structure indicating the allowed paint area
//		[IN/OUT]rpTitle
//				A pointer to a CRect object indicating the paint area reserved for the
//				text. This structure will be modified if necessary.
//		[IN]	bIsPressed
//				TRUE if the button is currently pressed
//		[IN]	dwWidth
//				Width of the image (icon or bitmap)
//		[IN]	dwHeight
//				Height of the image (icon or bitmap)
//		[OUT]	rpImage
//				A pointer to a CRect object that will receive the area available to the image
//


void CButtonST::DrawTheText(CDC* pDC, LPCTSTR lpszText, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled)
{
	CFont	*poldfont = nullptr;

	if (m_bIsPressed && m_bIsCheckBox == FALSE)
		rpCaption->OffsetRect(m_ptPressedOffset.x, m_ptPressedOffset.y);

	// Center text
	const	CRect	centerRect = rpCaption;
	pDC->DrawText(lpszText, -1, rpCaption, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
	rpCaption->OffsetRect((centerRect.Width() - rpCaption->Width())/2, (centerRect.Height() - rpCaption->Height())/2);

	pDC->SetBkMode(TRANSPARENT);
	if (m_bIsDisabled)
	{
		rpCaption->OffsetRect(1, 1);
		pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
		pDC->DrawText(lpszText, -1, rpCaption, DT_WORDBREAK | DT_CENTER);
		rpCaption->OffsetRect(-1, -1);
		pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
		pDC->DrawText(lpszText, -1, rpCaption, DT_WORDBREAK | DT_CENTER);
	}
	else
	{	
		if (m_bMouseOnButton || m_bIsPressed) 
		{
			pDC->SetTextColor(m_crColors[BTNST_COLOR_FG_IN]);
			pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_IN]);
		}
		else 
		{	// Focus...
			if (m_bIsFocused)
			{ 
/////////////////////////////////////////// 글자를 굵게...///////////////////////////////////////
				rpCaption->InflateRect(3,0);
				poldfont = (CFont*)pDC->SelectObject(&m_boldFont);
				
				pDC->SetTextColor(m_crColors[BTNST_COLOR_FG_FOCUS]); 
				pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_FOCUS]); 
			}
			else 
			{	
				if (m_bSelected) 
					poldfont = (CFont*)pDC->SelectObject(&m_boldFont);
				else 
					poldfont = (CFont*)pDC->SelectObject(&m_Font);

				pDC->SetTextColor(m_crColors[BTNST_COLOR_FG_OUT]); 
				pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_OUT]); 

			}
		}
		rpCaption->InflateRect(10, 0);	//2015.07.06 KSJ mini글자가 안보여 수정함.
		pDC->DrawText(lpszText, -1, rpCaption, 
			DT_SINGLELINE|DT_WORDBREAK | DT_CENTER|DT_VCENTER);

	}

	if (poldfont)
		pDC->SelectObject(poldfont);
} // End of DrawTheText

// This functions sets the button to have a standard or flat style.
//
// Parameters:
//		[IN]	bFlat
//				If TRUE the button will have a flat style, else
//				will have a standard style.
//				By default, CButtonST buttons are flat.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
// Return value:
//		BTNST_OK
//			Function executed successfully.
//
DWORD CButtonST::SetFlat(BOOL bFlat, BOOL bRepaint)
{
	m_bIsFlat = bFlat;
	if (bRepaint)	Invalidate();

	return BTNST_OK;
} // End of SetFlat


// This function sets the pressed style.
//
// Parameters:
//		[IN]	byStyle
//				Pressed style. Can be one of the following values:
//				BTNST_PRESSED_LEFTRIGHT		Pressed style from left to right (as usual)
//				BTNST_PRESSED_TOPBOTTOM		Pressed style from top to bottom
//				By default, CButtonST buttons have BTNST_PRESSED_LEFTRIGHT style.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
// Return value:
//		BTNST_OK
//			Function executed successfully.
//		BTNST_INVALIDPRESSEDSTYLE
//			Pressed style not supported.
//


DWORD CButtonST::SetPressedStyle(BYTE byStyle, BOOL bRepaint)
{
	switch (byStyle)
	{
		case BTNST_PRESSED_LEFTRIGHT:
			m_ptPressedOffset.x = 1;
			m_ptPressedOffset.y = 1;
			break;
		case BTNST_PRESSED_TOPBOTTOM:
			m_ptPressedOffset.x = 0;
			m_ptPressedOffset.y = 2;
			break;
		default:
			return BTNST_INVALIDPRESSEDSTYLE;
	} // switch

	if (bRepaint)	Invalidate();

	return BTNST_OK;
} // End of SetPressedStyle

// This function sets all colors to a default value.
//
// Parameters:
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
// Return value:
//		BTNST_OK
//			Function executed successfully.
//

DWORD CButtonST::SetDefaultColors(BOOL bRepaint)
{
	m_crColors[BTNST_COLOR_BK_IN]		= ::GetSysColor(COLOR_BTNFACE);
	m_crColors[BTNST_COLOR_FG_IN]		= ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[BTNST_COLOR_BK_OUT]		= ::GetSysColor(COLOR_BTNFACE);
	m_crColors[BTNST_COLOR_FG_OUT]		= ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[BTNST_COLOR_BK_FOCUS]	= ::GetSysColor(COLOR_BTNFACE);
	m_crColors[BTNST_COLOR_FG_FOCUS]	= ::GetSysColor(COLOR_BTNTEXT);

	if (bRepaint)	Invalidate();

	return BTNST_OK;
} // End of SetDefaultColors



// This function sets the color to use for a particular state.
//
// Parameters:
//		[IN]	byColorIndex
//				Index of the color to set. Can be one of the following values:
//				BTNST_COLOR_BK_IN		Background color when mouse is over the button
//				BTNST_COLOR_FG_IN		Text color when mouse is over the button
//				BTNST_COLOR_BK_OUT		Background color when mouse is outside the button
//				BTNST_COLOR_FG_OUT		Text color when mouse is outside the button
//				BTNST_COLOR_BK_FOCUS	Background color when the button is focused
//				BTNST_COLOR_FG_FOCUS	Text color when the button is focused
//		[IN]	crColor
//				New color.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
// Return value:
//		BTNST_OK
//			Function executed successfully.
//		BTNST_INVALIDINDEX
//			Invalid color index.
//
DWORD CButtonST::SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint)
{
	if (byColorIndex >= BTNST_MAX_COLORS)	return BTNST_INVALIDINDEX;

	// Set new color
	m_crColors[byColorIndex] = crColor;

	if (bRepaint)	Invalidate();

	return BTNST_OK;
} // End of SetColor

// This function is called every time the button background needs to be painted.
// If the button is in transparent mode this function will NOT be called.
// This is a virtual function that can be rewritten in CButtonST-derived classes
// to produce a whole range of buttons not available by default.
//
// Parameters:
//		[IN]	pDC
//				Pointer to a CDC object that indicates the device context.
//		[IN]	pRect
//				Pointer to a CRect object that indicates the bounds of the
//				area to be painted.
//
// Return value:
//		BTNST_OK
//			Function executed successfully.
//
DWORD CButtonST::OnDrawBackground(CDC* pDC, CRect* pRect)
{
	COLORREF crColor = 0;
	const	CPoint pt(2, 2);

	pRect->DeflateRect(2, 2);

	if (m_bMouseOnButton || m_bIsPressed)
	{
		crColor = m_crColors[BTNST_COLOR_BK_FOCUS];
		CBrush	brBackground(crColor);
		pRect->left = pRect->left - 1;
		pRect->top = pRect->top - 1;
		
		CBrush*	poldbrush = (CBrush*)pDC->SelectObject(&brBackground);
		pDC->RoundRect(pRect, pt);
		pRect->DeflateRect(2, 2);
		pDC->Draw3dRect(pRect, RGB(163, 167, 179), RGB(237, 241, 250));

		pRect->InflateRect(2, 2);

		pDC->SelectObject(poldbrush);
		brBackground.DeleteObject();
	}
	else
	{
		if (m_bIsFocused)
		{	
			crColor = m_crColors[BTNST_COLOR_BK_FOCUS];
			CBrush		brBackground(crColor);
			pRect->left = pRect->left - 1;
			pRect->top = pRect->top - 1;

			CBrush*	poldbrush = (CBrush*)pDC->SelectObject(&brBackground);
			pDC->RoundRect(pRect, pt);
			pRect->DeflateRect(2, 2);
			pDC->Draw3dRect(pRect, RGB(163, 167, 179), RGB(237, 241, 250));

			pDC->SelectObject(poldbrush);
			brBackground.DeleteObject();

			pRect->InflateRect(2, 2);
		}
		else
		{
			crColor = m_crColors[BTNST_COLOR_BK_OUT];
			CBrush	brBackground(crColor);
			pRect->left = pRect->left - 1;
			pRect->top = pRect->top - 1;
			
			CBrush*	poldbrush = (CBrush*)pDC->SelectObject(&brBackground);
			pDC->RoundRect(pRect, pt);
	
			if (!m_bIsDisabled)
			{
				if (m_bSelected)
				{
					pRect->DeflateRect(2, 2);
					pDC->Draw3dRect(pRect, RGB(163, 167, 179), RGB(237, 241, 250));
					pRect->InflateRect(2, 2);
				}
				else
				{
					pRect->DeflateRect(1, 1);
					pDC->Draw3dRect(pRect, RGB(255, 255, 255), RGB(231, 227, 231));
					pRect->DeflateRect(1, 1);
					pDC->Draw3dRect(pRect, RGB(255, 255, 255), RGB(231, 227, 231));
					pRect->InflateRect(2, 2);
				}
			}

			pDC->SelectObject(poldbrush);
			brBackground.DeleteObject();
		}
	}

	return BTNST_OK;
} // End of OnDrawBackground

#undef XBS_TYPEMASK
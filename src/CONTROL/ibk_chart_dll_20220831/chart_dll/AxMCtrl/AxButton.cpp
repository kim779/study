#include "stdafx.h"
#include "AxButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
// CAxButton

CAxButton::CAxButton()
{
	m_pressed		= FALSE;
	m_focused		= FALSE;
	m_disabled		= FALSE;
	m_mouseOnButton	= FALSE;

	FreeResources(FALSE);

	// Default type is "flat" button
	m_flat = TRUE; 
  
	// By default draw border in "flat" button 
	m_bDrawBorder = TRUE; 
  
	// By default icon is aligned horizontally
	m_nAlign = AX_ALIGN_HORIZ; 
  
	// By default, for "flat" button, don't draw the focus rect
	m_bDrawFlatFocus = FALSE;

	// By default the button is not the default button
	m_default = FALSE;

	// By default the button is not a checkbox
	m_checkBox = FALSE;
	m_nCheck = 0;

	// Set default colors
	SetDefaultColors(FALSE);

	// No tooltip created
	m_ToolTip.m_hWnd = NULL;

	// Do not draw as a transparent button
	m_bDrawTransparent = FALSE;
	m_pbmpOldBk = NULL;

	// No URL defined
	::ZeroMemory(&m_szURL, sizeof(m_szURL));

	// No cursor defined
	m_hCursor = NULL;	// Fix by kawasaki@us.dnttm.ro

	// No autorepeat
	m_bAutoRepeat = FALSE;
	m_hWndAutoRepeat = NULL;
	m_nMsgAutoRepeat = WM_APP;
	m_dwPeriodAutoRepeat = 100;

	m_textType = TT_NOEFFECT;
} // End of CAxButton


CAxButton::~CAxButton()
{
	// Restore old bitmap (if any)
	if (m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
	{
		m_dcBk.SelectObject(m_pbmpOldBk);
	}

	FreeResources();

	// Destroy the cursor (if any)
	if (m_hCursor != NULL) ::DestroyCursor(m_hCursor);
} // End of ~CAxButton


BEGIN_MESSAGE_MAP(CAxButton, CButton)
    //{{AFX_MSG_MAP(CAxButton)
	ON_WM_CAPTURECHANGED()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_SYSCOLORCHANGE()
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_WM_ACTIVATE()
	ON_WM_ENABLE()
	ON_WM_CANCELMODE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


DWORD CAxButton::SetIcon(int nIconInId, int nIconOutId)
{
	HICON		hIconIn;
	HICON		hIconOut;
	HINSTANCE	hInstResource;

	// Find correct resource handle
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nIconInId), RT_GROUP_ICON);

	// Set icon when the mouse is IN the button
	hIconIn = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(nIconInId), IMAGE_ICON, 0, 0, 0);
  	// Set icon when the mouse is OUT the button
	hIconOut = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(nIconOutId), IMAGE_ICON, 0, 0, 0);

	return SetIcon(hIconIn, hIconOut);
} // End of SetIcon

DWORD CAxButton::SetIcon(HICON hIconIn, HICON hIconOut)
{
	BOOL		bRetValue;
	ICONINFO	ii;

	// Free any loaded resource
	FreeResources();

	if (hIconIn != NULL)
	{
		m_csIcons[0].hIcon = hIconIn;

		// Get icon dimension
		ZeroMemory(&ii, sizeof(ICONINFO));
		bRetValue = ::GetIconInfo(hIconIn, &ii);
		if (bRetValue == FALSE)
		{
			FreeResources();
			return AXBTN_INVALIDRESOURCE;
		} // if

		m_csIcons[0].dwWidth	= (DWORD)(ii.xHotspot * 2);
		m_csIcons[0].dwHeight	= (DWORD)(ii.yHotspot * 2);
		::DeleteObject(ii.hbmMask);
		::DeleteObject(ii.hbmColor);

		if (hIconOut != NULL)
		{
			m_csIcons[1].hIcon = hIconOut;

			// Get icon dimension
			ZeroMemory(&ii, sizeof(ICONINFO));
			bRetValue = ::GetIconInfo(hIconOut, &ii);
			if (bRetValue == FALSE)
			{
				FreeResources();
				return AXBTN_INVALIDRESOURCE;
			} // if

			m_csIcons[1].dwWidth	= (DWORD)(ii.xHotspot * 2);
			m_csIcons[1].dwHeight	= (DWORD)(ii.yHotspot * 2);
			::DeleteObject(ii.hbmMask);
			::DeleteObject(ii.hbmColor);
		} // if
	} // if

	RedrawWindow();

	return AXBTN_OK;
} // End of SetIcon

BOOL CAxButton::SetBtnCursor(int nCursorId)
{
	HINSTANCE hInstResource;
	// Destroy any previous cursor
	if (m_hCursor != NULL) ::DestroyCursor(m_hCursor);
	m_hCursor = NULL;

	// If we want a cursor
	if (nCursorId != NULL)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nCursorId),
											RT_GROUP_CURSOR);
		// Load icon resource
		m_hCursor = (HCURSOR)::LoadImage(hInstResource/*AfxGetApp()->m_hInstance*/, MAKEINTRESOURCE(nCursorId), IMAGE_CURSOR, 0, 0, 0);
		// If something wrong then return FALSE
		if (m_hCursor == NULL) return FALSE;
	}

	return TRUE;
} // End of SetBtnCursor

void CAxButton::SetTextType(UINT tt)
{
	if (m_textType != tt) {
		m_textType = tt;
		Invalidate();
	}
}

UINT CAxButton::GetTextType()
{
	return m_textType;
}

void CAxButton::SetFlat(BOOL bState)
{
	m_flat = bState;
	Invalidate();
} // End of SetFlat


BOOL CAxButton::GetFlat()
{
	return m_flat;
} // End of GetFlat


void CAxButton::SetAlign(int nAlign)
{
	switch (nAlign)
	{    
		case AX_ALIGN_HORIZ:
		case AX_ALIGN_HORIZ_RIGHT:
		case AX_ALIGN_VERT:
			m_nAlign = nAlign;
			break;
	}
	Invalidate();
} // End of SetAlign


int CAxButton::GetAlign()
{
	return m_nAlign;
} // End of GetAlign


void CAxButton::DrawBorder(BOOL bEnable)
{
	m_bDrawBorder = bEnable;
} // End of DrawBorder

void CAxButton::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd*	pWnd;		// Active window
	CWnd*	pParent;	// Window that owns the button

	CButton::OnMouseMove(nFlags, point);

	// If the mouse enter the button with the left button pressed then do nothing
	if (nFlags & MK_LBUTTON && m_mouseOnButton == FALSE) return;

	// If our button is not flat then do nothing
	if (m_flat == FALSE) return;

	pWnd = GetActiveWindow();
	pParent = GetOwner();

	if ((GetCapture() != this) && 
		(
#ifndef AX_LIKEIE
		pWnd != NULL && 
#endif
		pParent != NULL)) 
	{
		m_mouseOnButton = TRUE;
		//SetFocus();	// Thanks Ralph!
		SetCapture();
		Invalidate();
	} // if
	else
	{
		/*
		CRect rc;
		GetClientRect(&rc);
		if (!rc.PtInRect(point))
		{
		*/

		POINT p2 = point;
		ClientToScreen(&p2);
		CWnd* wndUnderMouse = WindowFromPoint(p2);
//		if (wndUnderMouse != this)
		if (wndUnderMouse && wndUnderMouse->m_hWnd != this->m_hWnd)
		{
			// Redraw only if mouse goes out
			if (m_mouseOnButton == TRUE)
			{
				m_mouseOnButton = FALSE;
				Invalidate();
			} // if
			// If user is NOT pressing left button then release capture!
			if (!(nFlags & MK_LBUTTON)) ReleaseCapture();
		} // if
	} // else
} // End of OnMouseMove

void CAxButton::OnKillFocus(CWnd * pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	CancelHover();
} // End of OnKillFocus

void CAxButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CButton::OnLButtonDown(nFlags, point);

	if (m_bAutoRepeat == TRUE)
	{
		MSG		csMsg;
		int		nButtonID;
		HWND	hWndParent;
		BOOL	bInitialState = TRUE;

		nButtonID = GetDlgCtrlID();
		hWndParent = GetParent()->GetSafeHwnd();
		SetCapture();
		while (PeekMessage(&csMsg, m_hWnd, WM_LBUTTONUP, WM_LBUTTONUP, PM_REMOVE) == FALSE)
		{
			::SendMessage(hWndParent, WM_COMMAND, MAKEWPARAM((WORD)nButtonID, BN_CLICKED), (LPARAM)m_hWnd);
			::Sleep(m_dwPeriodAutoRepeat);
			bInitialState = !bInitialState;
		} // while

		if (!bInitialState)
		{
			::SendMessage(hWndParent, WM_COMMAND, MAKEWPARAM((WORD)nButtonID, BN_CLICKED), (LPARAM)m_hWnd);
		} // if
		ReleaseCapture();
		SendMessage(WM_LBUTTONUP);
		CPoint	ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);
		SendMessage(WM_MOUSEMOVE, 0, MAKELPARAM(ptCursor.x, ptCursor.y));
	} // if
} // End of OnLButtonDown

void CAxButton::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CButton::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState == WA_INACTIVE)	CancelHover();
} // End of OnActivate

void CAxButton::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
	if (bEnable == FALSE)	CancelHover();
} // End of OnEnable

void CAxButton::OnCancelMode() 
{
	CButton::OnCancelMode();
		
	CancelHover();
} // End of OnCancelMode

void CAxButton::OnCaptureChanged(CWnd *pWnd) 
{
	if (m_mouseOnButton == TRUE)
	{
		ReleaseCapture();
		Invalidate();
	} // if

	CButton::OnCaptureChanged(pWnd);
} // End of OnCaptureChanged

void CAxButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC*	pDC = CDC::FromHandle(lpDIS->hDC);

	CPen	*pOldPen;

	// Checkbox or Radiobutton style ?
	if (m_checkBox == TRUE)
	{
		m_pressed  =  (lpDIS->itemState & ODS_SELECTED)
						|| (m_nCheck != 0);
		//m_pressed = TRUE;
	}
	// Normal button OR other button style ...
	else
	{
		m_pressed = (lpDIS->itemState & ODS_SELECTED);
	}

	m_focused  = (lpDIS->itemState & ODS_FOCUS);
	m_disabled = (lpDIS->itemState & ODS_DISABLED);

	CRect itemRect = lpDIS->rcItem;

	pDC->SetBkMode(TRANSPARENT);

	if (m_flat == FALSE)
	{
		if (m_focused || (GetDefault() == TRUE))
		{
			CBrush br(RGB(0,0,0));  
			pDC->FrameRect(&itemRect, &br);
			itemRect.DeflateRect(1, 1);
		} // if
	} // if

	// Prepare draw... paint button background

	// Draw transparent?
	if (m_bDrawTransparent == TRUE)
		PaintBk(pDC);
	else
		OnDrawBackground(pDC, &itemRect);

	// Draw pressed button
	if (m_pressed)
	{
		if (m_flat == TRUE)
		{
			if (m_bDrawBorder)
				OnDrawBorder(pDC, &itemRect);
		}
		else    
		{
			CBrush brBtnShadow(GetSysColor(COLOR_BTNSHADOW));
			pDC->FrameRect(&itemRect, &brBtnShadow);
		}
	}
	else // ...else draw non pressed button
	{
		CPen penBtnHiLight(PS_SOLID, 0, GetSysColor(COLOR_BTNHILIGHT)); // White
		CPen pen3DLight(PS_SOLID, 0, GetSysColor(COLOR_3DLIGHT));       // Light gray
		CPen penBtnShadow(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));   // Dark gray
		CPen pen3DDKShadow(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW)); // Black

		if (m_flat == TRUE)
		{
			if (m_mouseOnButton && m_bDrawBorder)
				OnDrawBorder(pDC, &itemRect);
		}
		else
		{
			// Draw top-left borders
			// White line
			pOldPen = pDC->SelectObject(&penBtnHiLight);
			pDC->MoveTo(itemRect.left, itemRect.bottom-1);
			pDC->LineTo(itemRect.left, itemRect.top);
			pDC->LineTo(itemRect.right, itemRect.top);
			// Light gray line
			pDC->SelectObject(pen3DLight);
			pDC->MoveTo(itemRect.left+1, itemRect.bottom-1);
			pDC->LineTo(itemRect.left+1, itemRect.top+1);
			pDC->LineTo(itemRect.right, itemRect.top+1);
			// Draw bottom-right borders
			// Black line
			pDC->SelectObject(pen3DDKShadow);
			pDC->MoveTo(itemRect.left, itemRect.bottom-1);
			pDC->LineTo(itemRect.right-1, itemRect.bottom-1);
			pDC->LineTo(itemRect.right-1, itemRect.top-1);
			// Dark gray line
			pDC->SelectObject(penBtnShadow);
			pDC->MoveTo(itemRect.left+1, itemRect.bottom-2);
			pDC->LineTo(itemRect.right-2, itemRect.bottom-2);
			pDC->LineTo(itemRect.right-2, itemRect.top);
			//
			pDC->SelectObject(pOldPen);
		}
	}

	// Read the button's title
	CString sTitle;
	GetWindowText(sTitle);

	CRect captionRect = lpDIS->rcItem;

	// Draw the icon
	if (m_csIcons[0].hIcon != NULL)
	{
		DrawTheIcon(pDC, !sTitle.IsEmpty(), &lpDIS->rcItem, &captionRect, m_pressed, m_disabled);
	}

	if (m_csBitmaps[0].hBitmap != NULL)
	{
		pDC->SetBkColor(RGB(255,255,255));
		DrawTheBitmap(pDC, !sTitle.IsEmpty(), &lpDIS->rcItem, &captionRect, m_pressed, m_disabled);
	} // if

	// Write the button title (if any)
	if (sTitle.IsEmpty() == FALSE)
	{
		// Draw the button's title
		// If button is pressed then "press" title also
		if (m_pressed && m_checkBox == FALSE)
			captionRect.OffsetRect(1, 1);
    
		// ONLY FOR DEBUG 
		//CBrush brBtnShadow(RGB(255, 0, 0));
		//pDC->FrameRect(&captionRect, &brBtnShadow);

		/*
		if ((m_mouseOnButton == TRUE) || (bIsPressed)) 
		{
			pDC->SetTextColor(GetActiveFgColor());
			pDC->SetBkColor(GetActiveBgColor());
		} 
		else 
		{
			pDC->SetTextColor(GetInactiveFgColor());
			pDC->SetBkColor(GetInactiveBgColor());
		}
		*/
		// Center text
		CRect centerRect = captionRect;
		pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);

		captionRect.OffsetRect((centerRect.Width() - captionRect.Width())/2, (centerRect.Height() - captionRect.Height())/2);
		/* RFU
		captionRect.OffsetRect(0, (centerRect.Height() - captionRect.Height())/2);
		captionRect.OffsetRect((centerRect.Width() - captionRect.Width())-4, (centerRect.Height() - captionRect.Height())/2);
		*/

		pDC->SetBkMode(TRANSPARENT);
		/*
		pDC->DrawState(captionRect.TopLeft(), captionRect.Size(), (LPCTSTR)sTitle, (bIsDisabled ? DSS_DISABLED : DSS_NORMAL), 
						TRUE, 0, (CBrush*)NULL);
		*/
		if (m_disabled)
		{
			captionRect.OffsetRect(1, 1);
			pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
			pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
			captionRect.OffsetRect(-1, -1);
			pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
			pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
		} // if
		else
		{
			if (m_mouseOnButton || m_pressed) 
			{
				pDC->SetTextColor(m_crColors[AXBTN_COLOR_FG_IN]);
				pDC->SetBkColor(m_crColors[AXBTN_COLOR_BK_IN]);
			} // if
			else 
			{
				pDC->SetTextColor(m_crColors[AXBTN_COLOR_FG_OUT]);
				pDC->SetBkColor(m_crColors[AXBTN_COLOR_BK_OUT]);
			} // else

			COLORREF oldColor;
			if (m_textType != TT_NOEFFECT) {
				oldColor = pDC->SetTextColor(clWhite);
			}

			switch (m_textType) {
				case TT_SUNKEN:
					captionRect.OffsetRect(-1, -1);
					pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
					captionRect.OffsetRect(1, 1);
					break;
				case TT_RAISED:
					captionRect.OffsetRect(1, 1);
					pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
					captionRect.OffsetRect(-1, -1);
					break;
			}

			if (m_textType != TT_NOEFFECT) {
				pDC->SetTextColor(oldColor);
			}

			pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
		} // if
	} // if

	if (m_flat == FALSE || (m_flat == TRUE && m_bDrawFlatFocus == TRUE))
	{
		// Draw the focus rect
		if (m_focused)
		{
			CRect focusRect = itemRect;
			focusRect.DeflateRect(3, 3);
			pDC->DrawFocusRect(&focusRect);
		} // if
	} // if
} // End of DrawItem

void CAxButton::DrawTheIcon(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, BOOL bIsPressed, BOOL bIsDisabled)
{
	BYTE		byIndex		= 0;

	// Select the icon to use
	if (m_checkBox == TRUE)
	{
		if (bIsPressed == TRUE)
			byIndex = 0;
		else
			byIndex = (m_csIcons[1].hIcon == NULL ? 0 : 1);
	} // if
	else
	{
		if (m_mouseOnButton == TRUE || bIsPressed == TRUE)
			byIndex = 0;
		else
			byIndex = (m_csIcons[1].hIcon == NULL ? 0 : 1);
	} // else

	CRect	rImage;
	PrepareImageRect(bHasTitle, rpItem, rpTitle, bIsPressed, m_csIcons[byIndex].dwWidth, m_csIcons[byIndex].dwHeight, &rImage);

	// Ole'!
	pDC->DrawState(	rImage.TopLeft(),
					rImage.Size(), 
					m_csIcons[byIndex].hIcon,
					(bIsDisabled ? DSS_DISABLED : DSS_NORMAL), 
					(CBrush*)NULL);
} // End of DrawTheIcon

void CAxButton::PreSubclassWindow() 
{
	UINT nBS;

	nBS = GetButtonStyle();

	// Check if this is the default button
	if (nBS & BS_DEFPUSHBUTTON) m_default = TRUE;

	// Check if this is a checkbox
	if (nBS & BS_CHECKBOX) m_checkBox = TRUE;

	// Add BS_OWNERDRAW style
	SetButtonStyle(nBS | BS_OWNERDRAW);

	CButton::PreSubclassWindow();
} // End of PreSubclassWindow

BOOL CAxButton::PreTranslateMessage(MSG* pMsg) 
{
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);
	
	return CButton::PreTranslateMessage(pMsg);
} // End of PreTranslateMessage

LRESULT CAxButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_LBUTTONDBLCLK)
	{
		message = WM_LBUTTONDOWN;
	} // if
	return CButton::DefWindowProc(message, wParam, lParam);
} // End of DefWindowProc

void CAxButton::SetFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint)
{
	m_bDrawFlatFocus = bDrawFlatFocus;
	
	// Repaint the button
	if (bRepaint == TRUE) Invalidate();
} // End of SetFlatFocus

BOOL CAxButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// If a cursor was specified then use it!
	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	} // if

	return CButton::OnSetCursor(pWnd, nHitTest, message);
} // End of OnSetCursor

void CAxButton::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (lpszText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, lpszText, rectBtn, 1);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText(lpszText, this, 1);
	m_ToolTip.Activate(bActivate);
} // End of SetTooltipText


void CAxButton::SetTooltipText(int nId, BOOL bActivate)
{
	CString sText;

	// load string resource
	sText.LoadString(nId);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE) SetTooltipText((LPCTSTR)sText, bActivate);
} // End of SetTooltipText


void CAxButton::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
} // End of EnableTooltip


BOOL CAxButton::GetDefault()
{
	return m_default;
} // End of GetDefault


void CAxButton::DrawTransparent(BOOL bRepaint)
{
	m_bDrawTransparent = TRUE;

	// Restore old bitmap (if any)
	if (m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
	{
		m_dcBk.SelectObject(m_pbmpOldBk);
	} // if

	m_bmpBk.DeleteObject();
	m_dcBk.DeleteDC();

	// Repaint the button
	if (bRepaint == TRUE) Invalidate();
} // End of DrawTransparent


void CAxButton::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this);
		// Create inactive
		m_ToolTip.Activate(FALSE);
		// Enable multiline
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
	} // if
} // End of InitToolTip


void CAxButton::PaintBk(CDC* pDC)
{
	CClientDC clDC(GetParent());
	CRect rect;
	CRect rect1;

	GetClientRect(rect);

	GetWindowRect(rect1);
	GetParent()->ScreenToClient(rect1);

	if (m_dcBk.m_hDC == NULL)
	{
		m_dcBk.CreateCompatibleDC(&clDC);
		m_bmpBk.CreateCompatibleBitmap(&clDC, rect.Width(), rect.Height());
		m_pbmpOldBk = m_dcBk.SelectObject(&m_bmpBk);
		m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), &clDC, rect1.left, rect1.top, SRCCOPY);
	} // if

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
} // End of PaintBk


HBRUSH CAxButton::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
} // End of CtlColor


void CAxButton::OnSysColorChange() 
{
	CButton::OnSysColorChange();

	m_dcBk.DeleteDC();
	m_bmpBk.DeleteObject();	
} // End of OnSysColorChange

BOOL CAxButton::OnClicked() 
{	
	if (m_checkBox == TRUE)
	{
		m_nCheck = !m_nCheck;
		Invalidate();
	} // if
	else
	{
		// Handle the URL (if any)
		if (::lstrlen(m_szURL) > 0)
			::ShellExecute(NULL, _T("open"), m_szURL, NULL,NULL, SW_SHOWMAXIMIZED);
	} // else

	return FALSE;
} // End of OnClicked

void CAxButton::SetCheck(int nCheck, BOOL bRepaint)
{
	if (m_checkBox == TRUE)
	{
		if (nCheck == 0) m_nCheck = 0;
		else m_nCheck = 1;

		if (bRepaint == TRUE) Invalidate();
	} // if
} // End of SetCheck

int CAxButton::GetCheck()
{
	return m_nCheck;
} // End of GetCheck

void CAxButton::FreeResources(BOOL bCheckForNULL)
{
	if (bCheckForNULL == TRUE)
	{
		// Destroy icons
		// Note: the following two lines MUST be here! even if
		// BoundChecker says they are unnecessary!
		if (m_csIcons[0].hIcon != NULL)	::DeleteObject(m_csIcons[0].hIcon);
		if (m_csIcons[1].hIcon != NULL)	::DeleteObject(m_csIcons[1].hIcon);

		// Destroy bitmaps
		if (m_csBitmaps[0].hBitmap != NULL)	::DeleteObject(m_csBitmaps[0].hBitmap);
		if (m_csBitmaps[1].hBitmap != NULL)	::DeleteObject(m_csBitmaps[1].hBitmap);

		// Destroy mask bitmaps
		if (m_csBitmaps[0].hMask != NULL)	::DeleteObject(m_csBitmaps[0].hMask);
		if (m_csBitmaps[1].hMask != NULL)	::DeleteObject(m_csBitmaps[1].hMask);
	} // if

	::ZeroMemory(&m_csIcons, sizeof(m_csIcons));
	::ZeroMemory(&m_csBitmaps, sizeof(m_csBitmaps));
} // End of FreeResources

DWORD CAxButton::SetBitmaps(HBITMAP hBitmapIn, COLORREF crTransColorIn, HBITMAP hBitmapOut, COLORREF crTransColorOut)
{
	int		nRetValue;
	BITMAP	csBitmapSize;

	// Free any loaded resource
	FreeResources();

	if (hBitmapIn != NULL)
	{
		m_csBitmaps[0].hBitmap = hBitmapIn;
		m_csBitmaps[0].crTransparent = crTransColorIn;
		// Get bitmap size
		nRetValue = ::GetObject(hBitmapIn, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			FreeResources();
			return AXBTN_INVALIDRESOURCE;
		} // if
		m_csBitmaps[0].dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_csBitmaps[0].dwHeight = (DWORD)csBitmapSize.bmHeight;

		// Create mask for bitmap In
		m_csBitmaps[0].hMask = CreateBitmapMask(hBitmapIn, m_csBitmaps[0].dwWidth, m_csBitmaps[0].dwHeight, crTransColorIn);
		if (m_csBitmaps[0].hMask == NULL)
		{
			FreeResources();
			return AXBTN_FAILEDMASK;
		} // if

		if (hBitmapOut != NULL)
		{
			m_csBitmaps[1].hBitmap = hBitmapOut;
			m_csBitmaps[1].crTransparent = crTransColorOut;
			// Get bitmap size
			nRetValue = ::GetObject(hBitmapOut, sizeof(csBitmapSize), &csBitmapSize);
			if (nRetValue == 0)
			{
				FreeResources();
				return AXBTN_INVALIDRESOURCE;
			} // if
			m_csBitmaps[1].dwWidth = (DWORD)csBitmapSize.bmWidth;
			m_csBitmaps[1].dwHeight = (DWORD)csBitmapSize.bmHeight;

			// Create mask for bitmap Out
			m_csBitmaps[1].hMask = CreateBitmapMask(hBitmapOut, m_csBitmaps[1].dwWidth, m_csBitmaps[1].dwHeight, crTransColorOut);
			if (m_csBitmaps[1].hMask == NULL)
			{
				FreeResources();
				return AXBTN_FAILEDMASK;
			} // if
		} // if
	} // if

	RedrawWindow();

	return AXBTN_OK;
} // End of SetBitmaps

DWORD CAxButton::SetBitmaps(int nBitmapIn, COLORREF crTransColorIn, int nBitmapOut, COLORREF crTransColorOut)
{
	HBITMAP		hBitmapIn	= NULL;
	HBITMAP		hBitmapOut	= NULL;
	HINSTANCE hInstResource	= NULL;
	
	// Find correct resource handle
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapIn), RT_BITMAP);

	// Load bitmap In
	hBitmapIn = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapIn), IMAGE_BITMAP, 0, 0, 0);

	// Load bitmap Out
	hBitmapOut = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapOut), IMAGE_BITMAP, 0, 0, 0);

	return SetBitmaps(hBitmapIn, crTransColorIn, hBitmapOut, crTransColorOut);
} // End of SetBitmaps

void CAxButton::DrawTheBitmap(CDC* pDC, BOOL bHasTitle, RECT* rItem, CRect *rCaption, BOOL bIsPressed, BOOL bIsDisabled)
{
	HDC			hdcBmpMem	= NULL;
	HBITMAP		hbmOldBmp	= NULL;
	HDC			hdcMem		= NULL;
	HBITMAP		hbmT		= NULL;

	BYTE		byIndex		= 0;

	// Select the bitmap to use
	if (m_checkBox == TRUE)
	{
		if (bIsPressed == TRUE)
			byIndex = 0;
		else
			byIndex = (m_csBitmaps[1].hBitmap == NULL ? 0 : 1);
	} // if
	else
	{
		if (m_mouseOnButton == TRUE || bIsPressed == TRUE)
			byIndex = 0;
		else
			byIndex = (m_csBitmaps[1].hBitmap == NULL ? 0 : 1);
	} // else

	CRect	rImage;
	PrepareImageRect(bHasTitle, rItem, rCaption, bIsPressed, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, &rImage);

	hdcBmpMem = ::CreateCompatibleDC(pDC->m_hDC);

	hbmOldBmp = (HBITMAP)::SelectObject(hdcBmpMem, m_csBitmaps[byIndex].hBitmap);

	hdcMem = ::CreateCompatibleDC(NULL);

	hbmT = (HBITMAP)::SelectObject(hdcMem, m_csBitmaps[byIndex].hMask);

	::BitBlt(pDC->m_hDC, rImage.left, rImage.top, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, hdcMem, 0, 0, SRCAND);

	::BitBlt(pDC->m_hDC, rImage.left, rImage.top, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, hdcBmpMem, 0, 0, SRCPAINT);

	::SelectObject(hdcMem, hbmT);
	::DeleteDC(hdcMem);

	::SelectObject(hdcBmpMem, hbmOldBmp);
	::DeleteDC(hdcBmpMem);
} // End of DrawTheBitmap

HBITMAP CAxButton::CreateBitmapMask(HBITMAP hSourceBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTransColor)
{
	HBITMAP		hMask		= NULL;
	HDC			hdcSrc		= NULL;
	HDC			hdcDest		= NULL;
	HBITMAP		hbmSrcT		= NULL;
	HBITMAP		hbmDestT	= NULL;
	COLORREF	crSaveBk;
	COLORREF	crSaveDestText;

	hMask = ::CreateBitmap(dwWidth, dwHeight, 1, 1, NULL);
	if (hMask == NULL)	return NULL;

	hdcSrc	= ::CreateCompatibleDC(NULL);
	hdcDest	= ::CreateCompatibleDC(NULL);

	hbmSrcT = (HBITMAP)::SelectObject(hdcSrc, hSourceBitmap);
	hbmDestT = (HBITMAP)::SelectObject(hdcDest, hMask);

	crSaveBk = ::SetBkColor(hdcSrc, crTransColor);

	::BitBlt(hdcDest, 0, 0, dwWidth, dwHeight, hdcSrc, 0, 0, SRCCOPY);

	crSaveDestText = ::SetTextColor(hdcSrc, RGB(255, 255, 255));
	::SetBkColor(hdcSrc,RGB(0, 0, 0));

	::BitBlt(hdcSrc, 0, 0, dwWidth, dwHeight, hdcDest, 0, 0, SRCAND);

	SetTextColor(hdcDest, crSaveDestText);

	::SetBkColor(hdcSrc, crSaveBk);
	::SelectObject(hdcSrc, hbmSrcT);
	::SelectObject(hdcDest, hbmDestT);

	::DeleteDC(hdcSrc);
	::DeleteDC(hdcDest);

	return hMask;
} // End of CreateBitmapMask

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
void CAxButton::PrepareImageRect(BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, BOOL bIsPressed, DWORD dwWidth, DWORD dwHeight, CRect* rpImage)
{
	CRect rBtn;

	rpImage->CopyRect(rpItem);

	switch (m_nAlign)
	{
		case AX_ALIGN_HORIZ:
			if (bHasTitle == FALSE /*spTitle->IsEmpty()*/)
			{
				// Center image horizontally
				rpImage->left += ((rpImage->Width() - dwWidth)/2);
			}
			else
			{
				// Image must be placed just inside the focus rect
				rpImage->left += 3;  
				rpTitle->left += dwWidth + 3;
			}
			// Center image vertically
			rpImage->top += ((rpImage->Height() - dwHeight)/2);
			break;

		case AX_ALIGN_HORIZ_RIGHT:
			GetClientRect(&rBtn);
			if (bHasTitle == FALSE /*spTitle->IsEmpty()*/)
			{
				// Center image horizontally
				rpImage->left += ((rpImage->Width() - dwWidth)/2);
			}
			else
			{
				// Image must be placed just inside the focus rect
				rpTitle->right = rpTitle->Width() - dwWidth - 3;
				rpTitle->left = 3;
				rpImage->left = rBtn.right - dwWidth - 3;
				// Center image vertically
				rpImage->top += ((rpImage->Height() - dwHeight)/2);
			}
			break;
		
		case AX_ALIGN_VERT:
			// Center image horizontally
			rpImage->left += ((rpImage->Width() - dwWidth)/2);
			if (bHasTitle == FALSE /*spTitle->IsEmpty()*/)
			{
				// Center image vertically
				rpImage->top += ((rpImage->Height() - dwHeight)/2);           
			}
			else
			{
				rpImage->top = 3;
				rpTitle->top += dwHeight;
			}
			break;
	}
    
	// If button is pressed then press image also
	if (bIsPressed == TRUE && m_checkBox == FALSE)
		rpImage->OffsetRect(1, 1);
} // End of PrepareImageRect

//
// Parameters:
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
// Return value:
//		AXBTN_OK
//			Function executed successfully.
//
DWORD CAxButton::SetDefaultColors(BOOL bRepaint)
{
	m_crColors[AXBTN_COLOR_BK_IN]	= ::GetSysColor(COLOR_BTNFACE);
	m_crColors[AXBTN_COLOR_FG_IN]	= ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[AXBTN_COLOR_BK_OUT]	= ::GetSysColor(COLOR_BTNFACE);
	m_crColors[AXBTN_COLOR_FG_OUT]	= ::GetSysColor(COLOR_BTNTEXT);

	if (bRepaint == TRUE)	Invalidate();

	return AXBTN_OK;
} // End of SetDefaultColors

//
// Parameters:
//		[IN]	byColorIndex
//				Index of the color to set. This index is zero-based.
//		[IN]	crColor
//				New color.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
// Return value:
//		AXBTN_OK
//			Function executed successfully.
//		AXBTN_INVALIDINDEX
//			Invalid color index.
//
DWORD CAxButton::SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint)
{
	if (byColorIndex >= AXBTN_MAX_COLORS)	return AXBTN_INVALIDINDEX;

	// Set new color
	m_crColors[byColorIndex] = crColor;

	if (bRepaint == TRUE)	Invalidate();

	return AXBTN_OK;
} // End of SetColor

//
// Parameters:
//		[IN]	byColorIndex
//				Index of the color to get. This index is zero-based.
//		[OUT]	crpColor
//				A pointer to a COLORREF that will receive the color.
//
// Return value:
//		AXBTN_OK
//			Function executed successfully.
//		AXBTN_INVALIDINDEX
//			Invalid color index.
//
DWORD CAxButton::GetColor(BYTE byColorIndex, COLORREF* crpColor)
{
	if (byColorIndex >= AXBTN_MAX_COLORS)	return AXBTN_INVALIDINDEX;

	// Get color
	*crpColor = m_crColors[byColorIndex];

	return AXBTN_OK;
} // End of GetColor

//
// Parameters:
//		[IN]	lpszURL
//				Pointer to a null-terminated string that contains the URL.
//
// Return value:
//		AXBTN_OK
//			Function executed successfully.
//
DWORD CAxButton::SetURL(LPCTSTR lpszURL)
{
	if (lpszURL != NULL)
	{
		// Store the URL
		::lstrcpyn(m_szURL, lpszURL, _MAX_PATH);
	} // if
	else
	{
		// Remove any existing URL
		::ZeroMemory(&m_szURL, sizeof(m_szURL));
	} // else

	return AXBTN_OK;
} // End of SetURL

void CAxButton::CancelHover()
{
	// If our button is not flat then do nothing
	if (m_flat == FALSE) return;

	if (m_mouseOnButton == TRUE)
	{
		m_mouseOnButton = FALSE;
		Invalidate();
	} // if
} // End of CancelHover

// This function enable or disable the autorepeat feature.
//
// Parameters:
//		[IN]	bSet
//				TRUE to enable autorepeat. FALSE to disable.
//		[IN]	dwMilliseconds
//				Time (in milliseconds) between each button click.
//				If bSet is FALSE this parameter is ignored.
//
// Return value:
//		AXBTN_OK
//			Function executed successfully.
//
DWORD CAxButton::SetAutoRepeat(BOOL bSet, DWORD dwMilliseconds)
{
	m_bAutoRepeat = bSet;
	m_dwPeriodAutoRepeat = dwMilliseconds;

	return AXBTN_OK;
} // End of SetAutoRepeat

// This function is called every time the button background needs to be painted.
// If the button is in transparent mode this function will NOT be called.
//
// Parameters:
//		[IN]	pDC
//				Pointer to a CDC object that indicates the device context.
//		[IN]	pRect
//				Pointer to a CRect object that indicates the bounds of the
//				area to be painted.
//
// Return value:
//		AXBTN_OK
//			Function executed successfully.
//
DWORD CAxButton::OnDrawBackground(CDC* pDC, LPCRECT pRect)
{
	COLORREF	crColor;

	if (m_mouseOnButton || m_pressed)
		crColor = m_crColors[AXBTN_COLOR_BK_IN];
	else
		crColor = m_crColors[AXBTN_COLOR_BK_OUT];

	CBrush		brBackground(crColor);

	pDC->FillRect(pRect, &brBackground);

	return AXBTN_OK;
} // End of OnDrawBackground

// This function is called every time the button border needs to be painted.
// If the button is in standard (not flat) mode this function will NOT be called.
//
// Parameters:
//		[IN]	pDC
//				Pointer to a CDC object that indicates the device context.
//		[IN]	pRect
//				Pointer to a CRect object that indicates the bounds of the
//				area to be painted.
//
// Return value:
//		AXBTN_OK
//			Function executed successfully.
//
DWORD CAxButton::OnDrawBorder(CDC* pDC, LPCRECT pRect)
{
	if (m_pressed)
		pDC->Draw3dRect(pRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
	else
		pDC->Draw3dRect(pRect, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));

	return AXBTN_OK;
} // End of OnDrawBorder

#undef AX_LIKEIE









//
// CAxShadeButton
//
CAxShadeButton::CAxShadeButton()
{
}

CAxShadeButton::~CAxShadeButton()
{
}

void CAxShadeButton::SetShade(UINT shadeID, BYTE granularity, BYTE highlight, BYTE coloring, COLORREF color)
{
	long	sXSize,sYSize,bytes,j,i,k,h;
	BYTE	*iDst ,*posDst;
	
	RECT rect;
	GetWindowRect(&rect);
	sYSize=rect.bottom-rect.top;
	sXSize=rect.right-rect.left ;

	m_dh.Create(max(1,sXSize /*-2*m_FocusRectMargin-1*/ ),1,8);	//create the horizontal focus bitmap
	m_dv.Create(1,max(1,sYSize /*-2*m_FocusRectMargin*/),8);	//create the vertical focus bitmap

	m_dNormal.Create(sXSize,sYSize,8);					//create the default bitmap

	COLORREF hicr=GetSysColor(COLOR_BTNHIGHLIGHT);		//get the button base colors
	COLORREF midcr=GetSysColor(COLOR_BTNFACE);
	COLORREF locr=GetSysColor(COLOR_BTNSHADOW);
	long r,g,b;											//build the shaded palette
	for(i=0;i<129;i++){
		r=((128-i)*GetRValue(locr)+i*GetRValue(midcr))/128;
		g=((128-i)*GetGValue(locr)+i*GetGValue(midcr))/128;
		b=((128-i)*GetBValue(locr)+i*GetBValue(midcr))/128;
		m_dNormal.SetPaletteIndex((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
		m_dh.SetPaletteIndex((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
		m_dv.SetPaletteIndex((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
	}
	for(i=1;i<129;i++){
		r=((128-i)*GetRValue(midcr)+i*GetRValue(hicr))/128;
		g=((128-i)*GetGValue(midcr)+i*GetGValue(hicr))/128;
		b=((128-i)*GetBValue(midcr)+i*GetBValue(hicr))/128;
		m_dNormal.SetPaletteIndex((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
		m_dh.SetPaletteIndex((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
		m_dv.SetPaletteIndex((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
	}

	m_dNormal.BlendPalette(color,coloring);	//color the palette

	iDst=m_dh.GetBits();		//build the horiz. dotted focus bitmap
	j=(long)m_dh.GetWidth();
	for(i=0;i<j;i++){
//		iDst[i]=64+127*(i%2);	//soft
		iDst[i]=255*(i%2);		//hard
	}

	iDst=m_dv.GetBits();		//build the vert. dotted focus bitmap
	j=(long)m_dv.GetHeight();
	for(i=0;i<j;i++){
//		*iDst=64+127*(i%2);		//soft
		*iDst=255*(i%2);		//hard
		iDst+=4;
	}

	bytes = m_dNormal.GetLineWidth();
	iDst = m_dNormal.GetBits();
	posDst =iDst;
	long a,x,y,d,xs,idxmax,idxmin;

	int grainx2=RAND_MAX/(max(1,2*granularity));
	idxmax=255-granularity;
	idxmin=granularity;

	switch(shadeID){
//----------------------------------------------------
	case 8:	//SHS_METAL
		m_dNormal.Clear();
		// create the strokes
		k=40;	//stroke granularity
		for(a=0;a<200;a++){
			x=rand()/(RAND_MAX/sXSize); //stroke postion
			y=rand()/(RAND_MAX/sYSize);	//stroke position
			xs=rand()/(RAND_MAX/min(sXSize,sYSize))/2; //stroke lenght
			d=rand()/(RAND_MAX/k);	//stroke color
			for(i=0;i<xs;i++){
				if (((x-i)>0)&&((y+i)<sYSize))
					m_dNormal.SetPixelIndex(x-i,y+i,(BYTE)d);
				if (((x+i)<sXSize)&&((y-i)>0))
					m_dNormal.SetPixelIndex(sXSize-x+i,y-i,(BYTE)d);
			}
		}
		//blend strokes with SHS_DIAGONAL
		posDst =iDst;
		a=(idxmax-idxmin-k)/2;
		for(i = 0; i < sYSize; i++) {
			for(j = 0; j < sXSize; j++) {
				d=posDst[j]+((a*i)/sYSize+(a*(sXSize-j))/sXSize);
				posDst[j]=(BYTE)d;
				posDst[j]+=rand()/grainx2;
			}
			posDst+=bytes;
		}

		break;
//----------------------------------------------------
	case 7:	// SHS_HARDBUMP
		//set horizontal bump
		for(i = 0; i < sYSize; i++) {
			k=(255*i/sYSize)-127;
			k=(k*(k*k)/128)/128;
			k=(k*(128-granularity*2))/128+128;
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		//set vertical bump
		d=min(16,sXSize/6);	//max edge=16
		a=sYSize*sYSize/4;
		posDst =iDst;
		for(i = 0; i < sYSize; i++) {
			y=i-sYSize/2;
			for(j = 0; j < sXSize; j++) {
				x=j-sXSize/2;
				xs=sXSize/2-d+(y*y*d)/a;
				if (x>xs) posDst[j]=idxmin+(BYTE)(((sXSize-j)*128)/d);
				if ((x+xs)<0) posDst[j]=idxmax-(BYTE)((j*128)/d);
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 6: //SHS_SOFTBUMP
		for(i = 0; i < sYSize; i++) {
			h=(255*i/sYSize)-127;
			for(j = 0; j < sXSize; j++) {
				k=(255*(sXSize-j)/sXSize)-127;
				k=(h*(h*h)/128)/128+(k*(k*k)/128)/128;
				k=k*(128-granularity)/128+128;
				if (k<idxmin) k=idxmin;
				if (k>idxmax) k=idxmax;
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 5: // SHS_VBUMP
		for(j = 0; j < sXSize; j++) {
			k=(255*(sXSize-j)/sXSize)-127;
			k=(k*(k*k)/128)/128;
			k=(k*(128-granularity))/128+128;
			for(i = 0; i < sYSize; i++) {
				posDst[j+i*bytes]=(BYTE)k;
				posDst[j+i*bytes]+=rand()/grainx2-granularity;
			}
		}
		break;
//----------------------------------------------------
	case 4: //SHS_HBUMP
		for(i = 0; i < sYSize; i++) {
			k=(255*i/sYSize)-127;
			k=(k*(k*k)/128)/128;
			k=(k*(128-granularity))/128+128;
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 1:	//SHS_DIAGSHADE
		a=(idxmax-idxmin)/2;
		for(i = 0; i < sYSize; i++) {
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)(idxmin+a*i/sYSize+a*(sXSize-j)/sXSize);
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 2:	//SHS_HSHADE
		a=idxmax-idxmin;
		for(i = 0; i < sYSize; i++) {
			k=a*i/sYSize+idxmin;
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 3:	//SHS_VSHADE:
		a=idxmax-idxmin;
		for(j = 0; j < sXSize; j++) {
			k=a*(sXSize-j)/sXSize+idxmin;
			for(i = 0; i < sYSize; i++) {
				posDst[j+i*bytes]=(BYTE)k;
				posDst[j+i*bytes]+=rand()/grainx2-granularity;
			}
		}
		break;
//----------------------------------------------------
	default:	//SHS_NOISE
		for(i = 0; i < sYSize; i++) {
			for(j = 0; j < sXSize; j++) {
				posDst[j]=128+rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
	}
//----------------------------------------------------
	m_dDisabled.Clone(&m_dNormal);	//build the other bitmaps
	m_dOver.Clone(&m_dNormal);
	m_dOver.BlendPalette(hicr,highlight);
	m_dDown.Clone(&m_dOver);
} // End of SetShade

DWORD CAxShadeButton::OnDrawBackground(CDC *pDC, LPCRECT pRect)
{
	if (m_mouseOnButton)
	{
		if (m_pressed)
			m_dOver.Draw(pDC->GetSafeHdc(),1,1);
		else
			m_dOver.Draw(pDC->GetSafeHdc(),0,0);
	}
	else
		m_dNormal.Draw(pDC->GetSafeHdc(),0,0);

	return AXBTN_OK;
} // End of OnDrawBackground

DWORD CAxShadeButton::OnDrawBorder(CDC* pDC, LPCRECT pRect)
{
	CAxButton::OnDrawBorder(pDC, pRect);

	return AXBTN_OK;
} // End of OnDrawBorder

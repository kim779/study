// DrBtnCtrl.cpp : implementation file
//

/************************************************************************/
/*							DR Botton Control						    */
/************************************************************************/

#include "stdafx.h"
#include "DrBtnCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define USELESS_COLOR   RGB(0, 0, 0)

#define BTN_STATUS_NORMAL	  0
#define BTN_STATUS_FOCUS	  1
#define BTN_STATUS_DOWN		  2
#define BTN_STATUS_DISABLE	3

#define	NORMAL_BTN_OUTLINE1			RGB(149,179,149)

#define	NORMAL_BTN_BACK				RGB(255, 255, 255)
#define BTN_GRADIENT_BLUE_S	RGB(227, 239, 251)
#define BTN_GRADIENT_BLUE_E	RGB(182, 212, 241)

#define BTN_GRADIENT_GRAY_S	RGB(255, 255, 255)
#define BTN_GRADIENT_GRAY_E	RGB(222, 221, 221)
#define BTN_GRAY_LINE		RGB(147, 164, 181)


/////////////////////////////////////////////////////////////////////////////
// CDrBtnCtrl

CDrBtnCtrl::CDrBtnCtrl()
{
	m_bSetImgBtn = FALSE;

	m_MouseOnButton = FALSE;

	m_hIconIn = NULL;
	m_hIconOut = NULL;
	m_cxIcon = 0;
	m_cyIcon = 0;
	m_hCursor = NULL;

	// Default type is "flat" button
	m_bIsFlat = FALSE; 

	// By default draw border in "flat" button 
	m_bDrawBorder = FALSE; 

	// By default icon is aligned horizontally
	m_nAlign = ST_ALIGN_HORIZ; 

	// By default show the text button
	m_bShowText = TRUE; 

	// By default, for "flat" button, don't draw the focus rect
	m_bDrawFlatFocus = FALSE;

	// By default the button is not the default button
	m_bIsDefault = TRUE;

	SetDefaultInactiveBgColor();
	SetDefaultInactiveFgColor();
	SetDefaultActiveBgColor();
	SetDefaultActiveFgColor();

	SetBtnColor((int)STYLE_BASE);

	// No tooltip created
	m_ToolTip.m_hWnd = NULL;

	// Do not draw as a transparent button
	m_bDrawTransparent = FALSE;
	m_pbmpOldBk = NULL;

	m_bIsToggleBtn = FALSE;
	m_bToggle = FALSE;

	m_nBitmapID	= 0;
	m_nMOverOpt = OPT_NONE;
	m_bMouseover = FALSE;
	m_nType = 0;

	LOGFONT	  logFont;

	logFont.lfHeight = 14;                           
	logFont.lfWidth = 0;                                  
	logFont.lfEscapement = 0;                             
	logFont.lfOrientation = 0;                            
	logFont.lfWeight = 600;                           
	logFont.lfItalic = 0;                                 
	logFont.lfUnderline	=	0;                            
	logFont.lfStrikeOut	=	0;                            
	logFont.lfCharSet	=	129;                          
	logFont.lfOutPrecision	=	3;                        
	logFont.lfClipPrecision	=	2;                        
	logFont.lfQuality	=	1;                            
	logFont.lfPitchAndFamily=	49;                           
	strcpy(logFont.lfFaceName, (const char*)_T("굴림체"));  
	m_fontType.CreateFontIndirect(&logFont);



	//MouseOver

} // End of CDrBtnCtrl


CDrBtnCtrl::~CDrBtnCtrl()
{
	// Restore old bitmap (if any)
	if (m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
	{
		m_dcBk.SelectObject(m_pbmpOldBk);
	}

	// Destroy the icons (if any)
	// Note: the following two lines MUST be here! even if
	// BoundChecker says they are unnecessary!
	if (m_hIconIn != NULL) ::DestroyIcon(m_hIconIn);
	if (m_hIconOut != NULL) ::DestroyIcon(m_hIconOut);
	// Destroy the cursor (if any)
	if (m_hCursor != NULL) ::DestroyCursor(m_hCursor);

	if (m_bSetImgBtn == TRUE)
		m_imglistTree.DeleteImageList();

	m_fontType.DeleteObject();

} // End of ~CDrBtnCtrl


BEGIN_MESSAGE_MAP(CDrBtnCtrl, CBitmapButton)
	//{{AFX_MSG_MAP(CDrBtnCtrl)
	ON_WM_CAPTURECHANGED()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


void CDrBtnCtrl::SetIcon(int nIconInId, int nIconOutId)
{
	HICON hIconIn;
	HICON hIconOut;

	// Set icon when the mouse is IN the button
	hIconIn = (HICON)::LoadImage(NULL, MAKEINTRESOURCE(nIconInId), IMAGE_ICON, 0, 0, 0);

	// Set icon when the mouse is OUT the button
	hIconOut = (nIconOutId == NULL) ? NULL : (HICON)::LoadImage(NULL, MAKEINTRESOURCE(nIconOutId), IMAGE_ICON, 0, 0, 0);

	SetIcon(hIconIn, hIconOut);
	// Note: the following two lines MUST be here! even if
	// BoundChecker says they are unnecessary!
	if (m_hIconIn != NULL) ::DestroyIcon(m_hIconIn);
	if (m_hIconOut != NULL) ::DestroyIcon(m_hIconOut);

	// Set icon when the mouse is IN the button
	m_hIconIn = (HICON)::LoadImage(NULL, MAKEINTRESOURCE(nIconInId), IMAGE_ICON, 0, 0, 0);

	// Set icon when the mouse is OUT the button
	m_hIconOut = (nIconOutId == NULL) ? m_hIconIn : (HICON)::LoadImage(NULL, MAKEINTRESOURCE(nIconOutId), IMAGE_ICON, 0, 0, 0);

	ICONINFO ii;
	// Get icon dimension
	ZeroMemory(&ii, sizeof(ICONINFO));
	::GetIconInfo(m_hIconIn, &ii);

	m_cxIcon = (BYTE)(ii.xHotspot * 2);
	m_cyIcon = (BYTE)(ii.yHotspot * 2);
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);

	RedrawWindow();

} // End of SetIcon

void CDrBtnCtrl::SetIcon(CString strIconInId, CString strIconOutId)
{
	HICON hIconIn;
	HICON hIconOut;

	// Set icon when the mouse is IN the button
	hIconIn = (HICON)::LoadImage(NULL, strIconInId, IMAGE_ICON, 0, 0, 0);

	// Set icon when the mouse is OUT the button
	hIconOut = (strIconOutId == "") ? NULL : (HICON)::LoadImage(NULL, strIconOutId, IMAGE_ICON, 0, 0, 0);

	SetIcon(hIconIn, hIconOut);
	// Note: the following two lines MUST be here! even if
	// BoundChecker says they are unnecessary!
	if (m_hIconIn != NULL) ::DestroyIcon(m_hIconIn);
	if (m_hIconOut != NULL) ::DestroyIcon(m_hIconOut);

	// Set icon when the mouse is IN the button
	m_hIconIn = (HICON)::LoadImage(NULL, strIconInId, IMAGE_ICON, 0, 0, 0);

	// Set icon when the mouse is OUT the button
	m_hIconOut = (strIconOutId == "") ? m_hIconIn : (HICON)::LoadImage(NULL, strIconOutId, IMAGE_ICON, 0, 0, 0);

	ICONINFO ii;
	// Get icon dimension
	ZeroMemory(&ii, sizeof(ICONINFO));
	::GetIconInfo(m_hIconIn, &ii);

	m_cxIcon = (BYTE)(ii.xHotspot * 2);
	m_cyIcon = (BYTE)(ii.yHotspot * 2);
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);

	RedrawWindow();

} // End of SetIcon

BOOL CDrBtnCtrl::SetLoadBitmaps(LPCTSTR lpszBitmapResource, LPCTSTR lpszBitmapResourceSel, LPCTSTR lpszBitmapResourceDisabled)
{
	BOOL bResult = FALSE;

	bResult = LoadBitmaps(lpszBitmapResource, lpszBitmapResourceSel, NULL, lpszBitmapResourceDisabled);

	return bResult;
}

void CDrBtnCtrl::SetIcon(HICON hIconIn, HICON hIconOut)
{
	// Note: the following two lines MUST be here! even if
	// BoundChecker says they are unnecessary!
	if (m_hIconIn != NULL) ::DestroyIcon(m_hIconIn);
	if (m_hIconOut != NULL) ::DestroyIcon(m_hIconOut);

	// Set icon when the mouse is IN the button
	m_hIconIn = hIconIn;
	// Set icon when the mouse is OUT the button
	m_hIconOut = (hIconOut == NULL) ? m_hIconIn : hIconOut;

	ICONINFO ii;

	// Get icon dimension
	ZeroMemory(&ii, sizeof(ICONINFO));
	::GetIconInfo(m_hIconIn, &ii);

	m_cxIcon = (BYTE)(ii.xHotspot * 2);
	m_cyIcon = (BYTE)(ii.yHotspot * 2);
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);

	RedrawWindow();
} // End of SetIcon

BOOL CDrBtnCtrl::SetBtnCursor(int nCursorId)
{
	// Destroy any previous cursor
	if (m_hCursor != NULL) ::DestroyCursor(m_hCursor);
	m_hCursor = NULL;

	// If we want a cursor
	if (nCursorId != -1)
	{
		// Load icon resource
		m_hCursor = (HCURSOR)::LoadImage(NULL, MAKEINTRESOURCE(nCursorId), IMAGE_CURSOR, 0, 0, 0);

		// If something wrong then return FALSE
		if (m_hCursor == NULL) return FALSE;
	}

	return TRUE;
} // End of SetBtnCursor


void CDrBtnCtrl::SetFlat(BOOL bState)
{
	m_bIsFlat = bState;
	Invalidate();
} // End of SetFlat


BOOL CDrBtnCtrl::GetFlat()
{
	return m_bIsFlat;
} // End of GetFlat


void CDrBtnCtrl::SetAlign(int nAlign)
{
	switch (nAlign)
	{    
	case ST_ALIGN_HORIZ:
		m_nAlign = ST_ALIGN_HORIZ;
		break;
	case ST_ALIGN_HORIZ_RIGHT:
		m_nAlign = ST_ALIGN_HORIZ_RIGHT;
		break;
	case ST_ALIGN_VERT:
		m_nAlign = ST_ALIGN_VERT;
		break;
	}
	Invalidate();
} // End of SetAlign


int CDrBtnCtrl::GetAlign()
{
	return m_nAlign;
} // End of GetAlign


void CDrBtnCtrl::DrawBorder(BOOL bEnable)
{
	m_bDrawBorder = bEnable;
} // End of DrawBorder


const char* CDrBtnCtrl::GetVersionC()
{
	return "3.0";
} // End of GetVersionC


const short CDrBtnCtrl::GetVersionI()
{
	return 30; // Divide by 10 to get actual version
} // End of GetVersionI


void CDrBtnCtrl::SetShowText(BOOL bShow)
{
	m_bShowText = bShow;
	Invalidate();
} // End of SetShowText


BOOL CDrBtnCtrl::GetShowText()
{
	return m_bShowText;
} // End of GetShowText

// yiilyoul(2002/03/08) - start 
void CDrBtnCtrl::SetColor(COLORREF clrInActiveBg, COLORREF clrInActiveFg,
						  COLORREF clrActiveBg, COLORREF clrActiveFg)
{
	m_crInactiveBg  = clrInActiveBg;
	m_crInactiveFg  = clrInActiveFg;
	m_crActiveBg    = clrActiveBg;
	m_crActiveFg    = clrActiveFg;
}

void CDrBtnCtrl::SetColor(COLORREF clrBtnBg)
{
	m_crInactiveBg  = clrBtnBg;
	m_crActiveBg    = clrBtnBg;
}

void CDrBtnCtrl::SetToggleBtnColor(COLORREF clrBgToggleNormal, COLORREF clrBgToggleSelect,
								   COLORREF clrTextToggleNormal, COLORREF clrTextToggleSelect)
{
	m_clrBgToggleNormal = clrBgToggleNormal;
	m_clrBgToggleSelect = clrBgToggleSelect;
	m_clrTextToggleNormal = clrTextToggleNormal;
	m_clrTextToggleSelect = clrTextToggleSelect;
}
// yiilyoul(2002/03/08) - end

//20090710 SJ_KIM MouseOverEvent 관련 Func:SetUseMouseOverEvent
//Start ->
LRESULT CDrBtnCtrl::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if(m_bMouseover) return TRUE;

//	if(m_bSetImgBtn)
	{
		m_bMouseover = TRUE;
	}
// 	else
// 	{
// 		if(m_nMOverOpt == OPT_GREEN)
// 			SetBtnColor(3);
// 		else if(m_nMOverOpt == OPT_WHITE)
// 			SetBtnColor(2);			
// 	}
	Invalidate();
	return TRUE;
}

LRESULT CDrBtnCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
//	if(m_bSetImgBtn)
	{
		m_bMouseover = FALSE;
	}
// 	{
// 		if(m_nMOverOpt == OPT_GREEN)
// 			SetBtnColor(2);
// 		else if(m_nMOverOpt == OPT_WHITE)
// 			SetBtnColor(4);		
// 	}
	Invalidate();


	return TRUE;
}

void CDrBtnCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	//20090710 SJ_KIM MouseOver

	TRACKMOUSEEVENT trackMouseEvent;

	trackMouseEvent.cbSize = sizeof( trackMouseEvent );

	trackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
	trackMouseEvent.hwndTrack = GetSafeHwnd();

	trackMouseEvent.dwHoverTime = 0x00000001;
	_TrackMouseEvent( &trackMouseEvent );	

	CButton::OnMouseMove(nFlags, point);

	return;

	CWnd* pWnd;  // Finestra attiva
	CWnd* pParent; // Finestra che contiene il bottone

	CBitmapButton::OnMouseMove(nFlags, point);

	// If the mouse enter the button with the left button pressed
	// then do nothing
	if (nFlags & MK_LBUTTON && m_MouseOnButton == FALSE) return;

	// If our button is not flat then do nothing
	if (m_bIsFlat == FALSE) return;

	pWnd = GetActiveWindow();
	pParent = GetOwner();

	if ((GetCapture() != this) && 
		(
#ifndef ST_LIKEIE
		pWnd != NULL && 
#endif
		pParent != NULL)) 
	{
		m_MouseOnButton = TRUE;
		//SetFocus();	// Thanks Ralph!
		SetCapture();
		Invalidate();
	}
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
			if (m_MouseOnButton == TRUE)
			{
				m_MouseOnButton = FALSE;
				Invalidate();
			}
			// If user is NOT pressing left button then release capture!
			if (!(nFlags & MK_LBUTTON)) ReleaseCapture();
		}
	}
} // End of OnMouseMove


void CDrBtnCtrl::OnKillFocus(CWnd * pNewWnd)
{
	CBitmapButton::OnKillFocus(pNewWnd);

	// If our button is not flat then do nothing
	if (m_bIsFlat == FALSE) return;

	if (m_MouseOnButton == TRUE)
	{
		m_MouseOnButton = FALSE;
		Invalidate();
	}
} // End of OnKillFocus


void CDrBtnCtrl::OnCaptureChanged(CWnd *pWnd) 
{
	if (m_MouseOnButton == TRUE)
	{
		ReleaseCapture();
		Invalidate();
	}

	// Call base message handler
	CBitmapButton::OnCaptureChanged(pWnd);
} // End of OnCaptureChanged


void CDrBtnCtrl::DrawItem_DrewBtn(LPDRAWITEMSTRUCT lpDIS)
{
#ifdef ST_USE_MEMDC
	CDC  *pdrawDC = CDC::FromHandle(lpDIS->hDC);
	CMemDC memDC(pdrawDC);
	CDC  *pDC = &memDC;
#else	
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
#endif

	BOOL bIsPressed  = (lpDIS->itemState & ODS_SELECTED);
	BOOL bIsFocused  = (lpDIS->itemState & ODS_FOCUS);
	BOOL bIsDisabled = (lpDIS->itemState & ODS_DISABLED);

	CRect itemRect = lpDIS->rcItem;

	int nStatus = 0;
	nStatus = (int)BTN_STATUS_NORMAL;
	if (bIsPressed == TRUE)
		nStatus = (int)BTN_STATUS_DOWN;
	else if (bIsFocused == TRUE)
		nStatus = (int)BTN_STATUS_FOCUS;
	else
		//if (bIsDisabled == TRUE)
	{
		if (IsWindowEnabled() == FALSE)
			nStatus = (int)BTN_STATUS_DISABLE;
	}

	CBitmap bmpBtn;
	if (m_bSetImgBtn == TRUE)
	{				
		if(bIsPressed == FALSE)
		{
			if(bIsDisabled && m_imglistTree.GetImageCount() >= 4)
				GetBitmapSelected(pDC, 3, &bmpBtn);
			else if(m_bMouseover)
				GetBitmapSelected(pDC, 1, &bmpBtn);
			else
				GetBitmapSelected(pDC, 0, &bmpBtn);		
		}	
		else
			GetBitmapSelected(pDC, 2, &bmpBtn);


		pDC->DrawState(itemRect.TopLeft(), itemRect.Size(), 
			//(bIsPressed == FALSE) ? m_imglistTree.ExtractIcon(0) : m_imglistTree.ExtractIcon(1), 
			&bmpBtn,
			(bIsDisabled ? DSS_DISABLED : DSS_NORMAL), (CBrush*)NULL);
	}
	else
	{
		OnDrawBtn(pDC, itemRect, nStatus);
		OnDrawText(pDC, itemRect, nStatus);
	}
}

void CDrBtnCtrl::DrawItem_FlatBtn(LPDRAWITEMSTRUCT lpDIS)
{
#ifdef ST_USE_MEMDC
	CDC  *pdrawDC = CDC::FromHandle(lpDIS->hDC);
	CMemDC memDC(pdrawDC);
	CDC  *pDC = &memDC;
#else	
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
#endif

	CPen *pOldPen;
	BOOL bIsPressed  = (lpDIS->itemState & ODS_SELECTED);
	BOOL bIsFocused  = (lpDIS->itemState & ODS_FOCUS);
	BOOL bIsDisabled = (lpDIS->itemState & ODS_DISABLED);

	CRect itemRect = lpDIS->rcItem;

	// Toggle버튼일경우에 처리해 줍니다.
	if(m_bIsToggleBtn == TRUE) 
		bIsPressed = m_bToggle;

	pDC->SetBkMode(TRANSPARENT);
	if (m_bIsFlat == FALSE)
	{
		if (bIsFocused || (GetDefault() == TRUE))
		{
			CBrush br(RGB(0,0,0));  
			pDC->FrameRect(&itemRect, &br);
			itemRect.DeflateRect(1, 1);
		}
	}

	// Prepare draw... paint button's area with background color
	COLORREF bgColor;
	if ((m_MouseOnButton == TRUE) || (bIsPressed))
		bgColor = GetActiveBgColor();
	else
		bgColor = GetInactiveBgColor();

	// Toggle버튼일경우에 처리해 줍니다.
	if(m_bIsToggleBtn == TRUE)
	{
		if( m_bToggle == TRUE ) bgColor = m_clrBgToggleSelect;
		else bgColor = m_clrBgToggleNormal;
	}

	CBrush br(bgColor);
	// Draw transparent?
	if (m_bDrawTransparent == TRUE)
	{
		PaintBk(pDC);
	}
	else
	{
		pDC->FillRect(&itemRect, &br);
	}

	// Disegno lo sfondo del bottone
	//CBrush br(GetSysColor(COLOR_BTNFACE));  
	//pDC->FillRect(&itemRect, &br);

	// Draw pressed button
	if (bIsPressed)
	{
		if (m_bIsFlat == TRUE)
		{
			if (m_bDrawBorder == TRUE)
			{
				pDC->Draw3dRect(itemRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
			}
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

		if (m_bIsFlat == TRUE)
		{
			if (m_MouseOnButton == TRUE && m_bDrawBorder == TRUE)
			{
				pDC->Draw3dRect(itemRect, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));
			}
		}
		else
		{
			// Disegno i bordi a sinistra e in alto
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
			// Disegno i bordi a destra e in basso
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

	// If we don't want the title displayed
	if (m_bShowText == FALSE) sTitle.Empty();

	CRect captionRect = lpDIS->rcItem;

	// Draw the icon
	if (m_hIconIn != NULL)
	{
		DrawTheIcon(pDC, &sTitle, &lpDIS->rcItem, &captionRect, bIsPressed, bIsDisabled);
	}

	BOOL bBtnDownFlag = FALSE;

	// Write the button title (if any)
	if (sTitle.IsEmpty() == FALSE)
	{
		// Disegno la caption del bottone
		// Se il bottone e' premuto muovo la captionRect di conseguenza
		if (bIsPressed)
		{
			captionRect.OffsetRect(1, 1);
			bBtnDownFlag = TRUE;
		}

		// ONLY FOR DEBUG 
		// Evidenzia il rettangolo in cui verra' centrata la caption 
		//CBrush brBtnShadow(RGB(255, 0, 0));
		//pDC->FrameRect(&captionRect, &brBtnShadow);

#ifdef ST_USE_MEMDC
		// Get dialog's font
		CFont *pCurrentFont = GetFont(); 
		CFont *pOldFont = pDC->SelectObject(pCurrentFont);
#endif
		if ((m_MouseOnButton == TRUE) || (bIsPressed)) 
		{
			pDC->SetTextColor(GetActiveFgColor());
			pDC->SetBkColor(GetActiveBgColor());
		} 
		else 
		{
			pDC->SetTextColor(GetInactiveFgColor());
			pDC->SetBkColor(GetInactiveBgColor());
		}

		// Toggle버튼일경우에 처리해 줍니다.
		if(m_bIsToggleBtn == TRUE)
		{
			COLORREF clrBg;
			COLORREF clrText;

			if( m_bToggle == TRUE ) 
			{
				clrBg   = m_clrBgToggleSelect;
				clrText = m_clrTextToggleSelect;
			}
			else
			{
				clrBg   = m_clrBgToggleNormal;
				clrText = m_clrTextToggleNormal;
			}

			pDC->SetBkColor(clrBg);
			pDC->SetTextColor(clrText);
		}

		// Center text
		CRect centerRect = captionRect;
		pDC->DrawText(sTitle, -1, captionRect, DT_SINGLELINE|DT_CALCRECT);
		captionRect.OffsetRect((centerRect.Width() - captionRect.Width())/2, (centerRect.Height() - captionRect.Height())/2);
		/* RFU
		captionRect.OffsetRect(0, (centerRect.Height() - captionRect.Height())/2);
		captionRect.OffsetRect((centerRect.Width() - captionRect.Width())-4, (centerRect.Height() - captionRect.Height())/2);
		*/

		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawState(captionRect.TopLeft(), captionRect.Size(), (LPCTSTR)sTitle, (bIsDisabled ? DSS_DISABLED : DSS_NORMAL), 
			TRUE, 0, (CBrush*)NULL);
#ifdef ST_USE_MEMDC
		pDC->SelectObject(pOldFont);
#endif
	}

	if (m_bIsFlat == FALSE || (m_bIsFlat == TRUE && m_bDrawFlatFocus == TRUE))
	{
		// Draw the focus rect
		if (bIsFocused)
		{
			CRect focusRect = itemRect;
			focusRect.DeflateRect(3, 3);
			pDC->DrawFocusRect(&focusRect);
		}
	}

	pDC->Draw3dRect(itemRect, RGB(102, 102, 102), RGB(102, 102, 102));

	CRect focusRect = itemRect;
	focusRect.DeflateRect(1, 1);

	if (bBtnDownFlag == TRUE)
		pDC->Draw3dRect(focusRect, RGB(192, 192, 192), RGB(255, 255, 255));
	else
		pDC->Draw3dRect(focusRect, RGB(255, 255, 255), RGB(192, 192, 192));
}

void CDrBtnCtrl::DrawBaseBtn(CDC* pDC, CRect& rc, int nBtnStyle, int nStatus,int nToggleState)
{
	CString strKey;

// 	if(nBtnStyle == 11 || nBtnStyle == 12)
// 		nBtnStyle = 4;

// 	strKey.Format("%d%d",  nBtnStyle, nStatus);
// 	CBaseBtnColorValue* pColor = NULL; 
// 	m_mapBtn.Lookup(strKey, (void*&)pColor);

//	if(pColor)
	{			
	//	clrText = m_stBtnClr[nIndex].clrFont;
		COLORREF clrText = RGB(0,0,0);

		rc.DeflateRect(1, 1);
		GXPatB(pDC, rc, NORMAL_BTN_BACK);

		rc.InflateRect(1, 1);

		// 일반 버튼일경우는 Default로 그려준다.
		if ( nToggleState == -1 )
			DrawGradientRect(pDC, rc, BTN_GRADIENT_BLUE_S, BTN_GRADIENT_BLUE_E);
		else
		{
			//	토글버튼은 상태에 따라 그려준다.
			if ( nToggleState == 0 )
				DrawGradientRect(pDC, rc, BTN_GRADIENT_BLUE_S, BTN_GRADIENT_BLUE_E);
			else
				DrawGradientRect(pDC, rc, BTN_GRADIENT_BLUE_E, BTN_GRADIENT_BLUE_S);
		}


		//외곽 라인 그린다.
		//1. Left
		GXPatB(pDC, rc.left, rc.top, 1, rc.Height(), NORMAL_BTN_OUTLINE1);
		GXPatB(pDC, rc.left+1, rc.top+1, 1, rc.Height()-1, NORMAL_BTN_BACK /* NORMAL_BTN_OUTLINE3 */);

		//2. Top
		GXPatB(pDC, rc.left, rc.top, rc.Width(), 1, NORMAL_BTN_OUTLINE1);
		GXPatB(pDC, rc.left+1, rc.top+1, rc.Width()-1, 1, NORMAL_BTN_BACK /* NORMAL_BTN_OUTLINE3 */);

		//3. right
		GXPatB(pDC, rc.right-1, rc.top, 1, rc.Height(),  NORMAL_BTN_OUTLINE1);
		GXPatB(pDC, rc.right-2, rc.top+1, 1, rc.Height()-1,  NORMAL_BTN_BACK /* NORMAL_BTN_OUTLINE2 */);

		//4. bottom
		GXPatB(pDC, rc.left, rc.bottom-1, rc.Width(), 1, NORMAL_BTN_OUTLINE1);
		GXPatB(pDC, rc.left+1, rc.bottom-2, rc.Width()-2, 1, NORMAL_BTN_BACK /* NORMAL_BTN_OUTLINE2 */);
	}

}

void CDrBtnCtrl::DrawGrayBtn(CDC* pDC, CRect& rc, int nBtnStyle, int nStatus, int nToggleState)
{
	CString strKey;

// 	if(nBtnStyle == 11 || nBtnStyle == 12)
// 		nBtnStyle = 4;
// 
// 	strKey.Format("%d%d",  nBtnStyle, nStatus);
// 	CBaseBtnColorValue* pColor = NULL; 
// 	m_mapBtn.Lookup(strKey, (void*&)pColor);
// 
// 	if(pColor)
	{
		COLORREF clrOutLine = BTN_GRAY_LINE;

		rc.DeflateRect(1, 1);
		GXPatB(pDC, rc, NORMAL_BTN_BACK);

		rc.InflateRect(1, 1);

		if(m_nType == 1)
		{
			pDC->FillSolidRect(rc,  RGB(108, 152, 223));
			pDC->FillSolidRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1,  RGB(0, 63, 166));

		}
		else if(m_nType == 2)
		{
			pDC->FillSolidRect(rc,  RGB(244, 117 ,108));
			pDC->FillSolidRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1,  RGB(228,11,0));
		}
		else if(m_nType == 3)
		{
			pDC->FillSolidRect(rc,  RGB(126, 205, 128));
			pDC->FillSolidRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1,  RGB(66, 140, 91));
		}
		else
		{
			// --> [Edit]  강지원 2010/08/18
			// 일반 버튼일경우는 Default로 그려준다.
			if ( nToggleState == -1 )
			{
				if(nStatus == BTN_STATUS_DISABLE)
				{
					DrawGradientRect(pDC, rc, BTN_GRADIENT_GRAY_S, BTN_GRADIENT_GRAY_E);
					clrOutLine = RGB(188,198,208);
				}
				else if(nStatus == BTN_STATUS_DOWN)
				{
					DrawGradientRect(pDC, rc, BTN_GRADIENT_GRAY_S, RGB(206,206,206));
					clrOutLine = RGB(101,113,125);
				}
				else if(m_bMouseover)
				{
					DrawGradientRect(pDC, rc, BTN_GRADIENT_GRAY_S, RGB(185,185,185));
					clrOutLine = RGB(106,149,191);
				}
				else
					DrawGradientRect(pDC, rc, BTN_GRADIENT_GRAY_S, BTN_GRADIENT_GRAY_E);

			}
			else
			{
				//	토글버튼은 상태에 따라 그려준다.
				if ( nToggleState == 0 )
					DrawGradientRect(pDC, rc, BTN_GRADIENT_GRAY_S, BTN_GRADIENT_GRAY_E);
				else
					DrawGradientRect(pDC, rc, BTN_GRADIENT_GRAY_E, BTN_GRADIENT_GRAY_S);
			}
			// <-- [Edit]  강지원 2010/08/18


			//외곽 라인 그린다.
			//1. Left
			GXPatB(pDC, rc.left, rc.top, 1, rc.Height(), clrOutLine);
			if(nStatus == BTN_STATUS_DOWN)
			{
				GXPatB(pDC, rc.left+1, rc.top+1, 1, rc.Height()-1, RGB(165,165,165));
				GXPatB(pDC, rc.left+2, rc.top+2, 1, rc.Height()-2, RGB(219,219,219));
			}
			else
				GXPatB(pDC, rc.left+1, rc.top+1, 1, rc.Height()-1, NORMAL_BTN_BACK);

			//2. Top
			GXPatB(pDC, rc.left, rc.top, rc.Width(), 1, clrOutLine);
			if(nStatus == BTN_STATUS_DOWN)
			{
				GXPatB(pDC, rc.left+1, rc.top+1, rc.Width()-1, 1, RGB(165,165,165));
				GXPatB(pDC, rc.left+2, rc.top+2, rc.Width()-2, 1, RGB(219,219,219));
			}
			else
				GXPatB(pDC, rc.left+1, rc.top+1, rc.Width()-1, 1, NORMAL_BTN_BACK);

			//3. right
			GXPatB(pDC, rc.right-1, rc.top, 1, rc.Height(),  clrOutLine);
			GXPatB(pDC, rc.right-2, rc.top+2, 1, rc.Height()-1,  NORMAL_BTN_BACK);

			//4. bottom
			GXPatB(pDC, rc.left, rc.bottom-1, rc.Width(), 1, clrOutLine);
			GXPatB(pDC, rc.left+2, rc.bottom-2, rc.Width()-3, 1, NORMAL_BTN_BACK);
		}		
	}

}

void CDrBtnCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if(m_imglistTree.m_hImageList &&  m_imglistTree.GetImageCount())
	{
		DrawItem_DrewBtn(lpDIS);
	}
	else
// 	if (m_bIsFlat == TRUE)
	{
			//DrawItem_FlatBtn(lpDIS);
		CDC* pDC = CDC::FromHandle(lpDIS->hDC);
		BOOL bIsPressed  = (lpDIS->itemState & ODS_SELECTED);
		BOOL bIsFocused  = (lpDIS->itemState & ODS_FOCUS);
		BOOL bIsDisabled = (lpDIS->itemState & ODS_DISABLED);
		CRect itemRect = lpDIS->rcItem;

		int nStatus = BTN_STATUS_NORMAL;
		if(bIsPressed) nStatus = BTN_STATUS_DOWN;
		if(bIsDisabled) nStatus = BTN_STATUS_DISABLE;

		if(!m_bIsDefault)
			DrawBaseBtn(pDC, itemRect, 0, 1, bIsPressed);
		else 
			DrawGrayBtn(pDC, itemRect, 0, nStatus, -1);

		// Read the button's title
		CString sTitle;
		GetWindowText(sTitle);

		// If we don't want the title displayed
		if (m_bShowText == FALSE) sTitle.Empty();

		CRect captionRect = lpDIS->rcItem;
	// 	BOOL bBtnDownFlag = FALSE;

		if (sTitle.IsEmpty() == FALSE)
		{
			// Disegno la caption del bottone
			// Se il bottone e' premuto muovo la captionRect di conseguenza
			if (bIsPressed)
			{
				captionRect.OffsetRect(0, 1);
			}
			
			COLORREF clrText = RGB(82,115,145);
			if(bIsDisabled) clrText = RGB(166,176,208);
			else if(bIsPressed) clrText = RGB(116,132,149);
			else if(m_bMouseover) clrText = RGB(30,93,165);

			COLORREF clrTextOld = pDC->SetTextColor(clrText);
			CFont* pOldFont;
			if(m_nType != 0)
			{				
				pOldFont = pDC->SelectObject(&m_fontType);
				clrTextOld = pDC->SetTextColor(RGB(255,255,255));
			}
			
			// Center text
			CRect centerRect = captionRect;
			pDC->DrawText(sTitle, -1, captionRect, DT_SINGLELINE|DT_CALCRECT);
			captionRect.OffsetRect((centerRect.Width() - captionRect.Width())/2, (centerRect.Height() - captionRect.Height())/2);
			pDC->SetBkMode(TRANSPARENT);
			pDC->DrawState(captionRect.TopLeft(), captionRect.Size(), (LPCTSTR)sTitle, DSS_NORMAL, 
				TRUE, 0, (CBrush*)NULL);
			pDC->SetTextColor(clrTextOld);

			if(m_nType != 0)
				pDC->SelectObject(pOldFont);
		}
	}
// 	else
// 		DrawItem_DrewBtn(lpDIS);
}

void CDrBtnCtrl::DrawTheIcon(CDC* pDC, CString* title, RECT* rcItem, CRect* captionRect, BOOL IsPressed, BOOL IsDisabled)
{
	CRect iconRect = rcItem;
	CRect btnRect;

	switch (m_nAlign)
	{
	case ST_ALIGN_HORIZ:
		if (title->IsEmpty())
		{
			// Center the icon horizontally
			iconRect.left += ((iconRect.Width() - m_cxIcon)/2);
		}
		else
		{
			// L'icona deve vedersi subito dentro il focus rect
			iconRect.left += 3;  
			captionRect->left += m_cxIcon + 3;
		}
		// Center the icon vertically
		iconRect.top += ((iconRect.Height() - m_cyIcon)/2);
		break;
	case ST_ALIGN_HORIZ_RIGHT:
		GetClientRect(&btnRect);
		if (title->IsEmpty())
		{
			// Center the icon horizontally
			iconRect.left += ((iconRect.Width() - m_cxIcon)/2);
		}
		else
		{
			// L'icona deve vedersi subito dentro il focus rect
			captionRect->right = captionRect->Width() - m_cxIcon - 3;
			captionRect->left = 3;
			iconRect.left = btnRect.right - m_cxIcon - 3;
			// Center the icon vertically
			iconRect.top += ((iconRect.Height() - m_cyIcon)/2);
		}
		break;
	case ST_ALIGN_VERT:
		// Center the icon horizontally
		iconRect.left += ((iconRect.Width() - m_cxIcon)/2);
		if (title->IsEmpty())
		{
			// Center the icon vertically
			iconRect.top += ((iconRect.Height() - m_cyIcon)/2);           
		}
		else
		{
			captionRect->top += m_cyIcon;
		}
		break;
	}

	if (IsPressed) iconRect.OffsetRect(1, 1);

	m_hIconIn = m_imglistTree.ExtractIcon(0);
	m_hIconOut = m_imglistTree.ExtractIcon(1);

	pDC->DrawState(	iconRect.TopLeft(),
		iconRect.Size(), 
		(m_MouseOnButton == TRUE || IsPressed) ? m_hIconIn : m_hIconOut, 
		(IsDisabled ? DSS_DISABLED : DSS_NORMAL), 
		(CBrush*)NULL);
} // End of DrawTheIcon


void CDrBtnCtrl::PreSubclassWindow() 
{
	UINT nBS;

	nBS = GetButtonStyle();

	// Check if this is the default button
//	if (nBS & BS_DEFPUSHBUTTON) m_bIsDefault = TRUE;

	// Add BS_OWNERDRAW style
	SetButtonStyle(nBS | BS_OWNERDRAW);

	CBitmapButton::PreSubclassWindow();
} // End of PreSubclassWindow


BOOL CDrBtnCtrl::PreTranslateMessage(MSG* pMsg) 
{
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);

	return CBitmapButton::PreTranslateMessage(pMsg);
} // End of PreTranslateMessage


LRESULT CDrBtnCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_LBUTTONDBLCLK)
	{
		message = WM_LBUTTONDOWN;
	}
	return CBitmapButton::DefWindowProc(message, wParam, lParam);
} // End of DefWindowProc


void CDrBtnCtrl::SetDefaultInactiveBgColor(BOOL bRepaint)
{
	m_crInactiveBg = ::GetSysColor(COLOR_BTNFACE); 
	if (bRepaint == TRUE) Invalidate();
} // End of SetDefaultInactiveBgColor


void CDrBtnCtrl::SetInactiveBgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crInactiveBg = crNew; 
	if (bRepaint == TRUE) Invalidate();
} // End of SetInactiveBgColor


const COLORREF CDrBtnCtrl::GetInactiveBgColor()
{
	return m_crInactiveBg;
} // End of GetInactiveBgColor


void CDrBtnCtrl::SetDefaultInactiveFgColor(BOOL bRepaint)
{
	m_crInactiveFg = ::GetSysColor(COLOR_BTNTEXT); 
	if (bRepaint == TRUE) Invalidate();
} // End of SetDefaultInactiveFgColor


void CDrBtnCtrl::SetInactiveFgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crInactiveFg = crNew; 
	if (bRepaint == TRUE) Invalidate();
} // End of SetInactiveFgColor


const COLORREF CDrBtnCtrl::GetInactiveFgColor()
{
	return m_crInactiveFg;
} // End of GetInactiveFgColor


void CDrBtnCtrl::SetDefaultActiveBgColor(BOOL bRepaint)
{
	m_crActiveBg = ::GetSysColor(COLOR_BTNFACE); 
	if (bRepaint == TRUE) Invalidate();
} // End of SetDefaultActiveBgColor


void CDrBtnCtrl::SetActiveBgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crActiveBg = crNew; 
	if (bRepaint == TRUE) Invalidate();
} // End of SetActiveBgColor


const COLORREF CDrBtnCtrl::GetActiveBgColor()
{
	return m_crActiveBg;
} // End of GetActiveBgColor


void CDrBtnCtrl::SetDefaultActiveFgColor(BOOL bRepaint)
{
	m_crActiveFg = ::GetSysColor(COLOR_BTNTEXT); 
	if (bRepaint == TRUE) Invalidate();
} // End of SetDefaultActiveFgColor


void CDrBtnCtrl::SetActiveFgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crActiveFg = crNew; 
	if (bRepaint == TRUE) Invalidate();
} // End of SetActiveFgColor


const COLORREF CDrBtnCtrl::GetActiveFgColor()
{
	return m_crActiveFg;
} // End of GetActiveFgColor


void CDrBtnCtrl::SetFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint)
{
	m_bDrawFlatFocus = bDrawFlatFocus;

	// Repaint the button
	if (bRepaint == TRUE) Invalidate();
} // End of SetFlatFocus


BOOL CDrBtnCtrl::GetFlatFocus()
{
	return m_bDrawFlatFocus;
} // End of GetFlatFocus


BOOL CDrBtnCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// If a cursor was specified then use it!
	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CBitmapButton::OnSetCursor(pWnd, nHitTest, message);
} // End of OnSetCursor


void CDrBtnCtrl::SetTooltipText(CString& sText, BOOL bActivate)
{
	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		BOOL bFalg = m_ToolTip.AddTool(this, (LPCTSTR)sText, rectBtn, 1);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText((LPCTSTR)sText, this, 1);
	m_ToolTip.Activate(bActivate);
} // End of SetTooltipText


void CDrBtnCtrl::SetTooltipText(int nId, BOOL bActivate)
{
	CString sText;

	// load string resource
	sText.LoadString(nId);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE) SetTooltipText(sText, bActivate);
} // End of SetTooltipText


void CDrBtnCtrl::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
} // End of EnableTooltip


BOOL CDrBtnCtrl::GetDefault()
{
	return m_bIsDefault;
} // End of GetDefault


void CDrBtnCtrl::DrawTransparent(BOOL bRepaint)
{
	m_bDrawTransparent = TRUE;

	// Restore old bitmap (if any)
	if (m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
	{
		m_dcBk.SelectObject(m_pbmpOldBk);
	}

	m_bmpBk.Detach();
	m_dcBk.Detach();

	// Repaint the button
	if (bRepaint == TRUE) Invalidate();
} // End of DrawTransparent


void CDrBtnCtrl::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this);
		// Create inactive
		m_ToolTip.Activate(FALSE);
	}
} // End of InitToolTip


void CDrBtnCtrl::PaintBk(CDC * pDC)
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
	}

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
} // End of PaintBk


HBRUSH CDrBtnCtrl::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
} // End of CtlColor


void CDrBtnCtrl::OnSysColorChange() 
{
	CBitmapButton::OnSysColorChange();

	m_dcBk.DeleteDC();
	m_bmpBk.DeleteObject();	
} // End of OnSysColorChange


void CDrBtnCtrl::OnDrawBtn(CDC* pDC, CRect& rc, int nStatus)
{
	rc.DeflateRect(1, 1);
	GXPatB(pDC, rc, m_stBtnClr[nStatus].clrBtnBkRnd);

	rc.InflateRect(1, 1);
	//외곽 라인 그린다.
	//1. Left
	/*GXPatB(pDC, rc.left, rc.top+1, 1, rc.Height() - 2, m_stBtnClr[nStatus].clrBorder);*/
	GXPatB(pDC, rc.left, rc.top+1, 1, rc.Height() - 2, RGB(255, 0, 0));
	//2. Top
	GXPatB(pDC, rc.left+1, rc.top, rc.Width() - 2, 1, m_stBtnClr[nStatus].clrBorder);
	//3. right
	GXPatB(pDC, rc.right-1, rc.top+1, 1, rc.Height() - 2,  m_stBtnClr[nStatus].clrBorder);
	//4. bottom
	GXPatB(pDC, rc.left+1, rc.bottom-1, rc.Width() - 2, 1, m_stBtnClr[nStatus].clrBorder);

	//상단 1단계 
	//1. Left
	GXPatB(pDC, rc.left+1, rc.top+1, 1, rc.Height() - 3, m_stBtnClr[nStatus].clr1DgrLTop);
	//2. Top
	GXPatB(pDC, rc.left+1, rc.top+1, rc.Width() - 2, 1, m_stBtnClr[nStatus].clr1DgrLTop);

	//상단 2단계
	//1. Left
	GXPatB(pDC, rc.left+2, rc.top+2, 1, rc.Height() - 5, m_stBtnClr[nStatus].clr2DgrLTop);
	//2. Top
	GXPatB(pDC, rc.left+2, rc.top+2, rc.Width() - 5, 1, m_stBtnClr[nStatus].clr2DgrLTop);

	//상단 3단계
	//1. Left
	GXPatB(pDC, rc.left+3, rc.top+3, 1, rc.Height() - 7, m_stBtnClr[nStatus].clr3DgrLTop);
	//2. Top
	GXPatB(pDC, rc.left+3, rc.top+3, rc.Width() - 5, 1, m_stBtnClr[nStatus].clr3DgrLTop);

	//하단 1단계
	//3. right
	GXPatB(pDC, rc.right-2, rc.top+2, 1, rc.Height() - 3,  m_stBtnClr[nStatus].clr1DgrRBttm);
	//4. bottom
	GXPatB(pDC, rc.left+1, rc.bottom-2, rc.Width() - 2, 1, m_stBtnClr[nStatus].clr1DgrRBttm);

	//하단 2단계
	//3. right
	GXPatB(pDC, rc.right-3, rc.top+2, 1, rc.Height() - 5,  m_stBtnClr[nStatus].clr2DgrRBttm);
	//4. bottom
	GXPatB(pDC, rc.left+2, rc.bottom-3, rc.Width() - 4, 1, m_stBtnClr[nStatus].clr2DgrRBttm);

	//하단 3단계
	//3. right
	GXPatB(pDC, rc.right-4, rc.top+4, 1, rc.Height() - 7,  m_stBtnClr[nStatus].clr3DgrRBttm);
	//4. bottom
	GXPatB(pDC, rc.left+3, rc.bottom-4, rc.Width() - 6, 1, m_stBtnClr[nStatus].clr3DgrRBttm);			
}


void CDrBtnCtrl::OnDrawText(CDC* pDC, CRect& rc, int nStatus)
{
	BOOL bBtnDownFlag = FALSE;

	CString strBtnText;
	GetWindowText(strBtnText);
	if (strBtnText.IsEmpty() == FALSE)
	{
		if (nStatus == (int)BTN_STATUS_DOWN)
		{
			rc.OffsetRect(1, 1);
			bBtnDownFlag = TRUE;
		}

#ifdef ST_USE_MEMDC
		// Get dialog's font
		CFont *pCurrentFont = GetFont(); 
		CFont *pOldFont = pDC->SelectObject(pCurrentFont);
#endif
		if ((m_MouseOnButton == TRUE) || (nStatus == (int)BTN_STATUS_DOWN)) 
		{
			pDC->SetTextColor(m_stBtnClr[nStatus].clrFont);
			pDC->SetBkColor(m_stBtnClr[nStatus].clrBorder);
		} 
		else 
		{
			pDC->SetTextColor(m_stBtnClr[nStatus].clrFont);
			pDC->SetBkColor(m_stBtnClr[nStatus].clrBorder);
		}

		if (m_bIsToggleBtn == TRUE)
		{
			COLORREF clrBg;
			COLORREF clrText;
			if (m_bToggle == TRUE) 
			{
				clrBg   = m_clrBgToggleSelect;
				clrText = m_clrTextToggleSelect;
			}
			else
			{
				clrBg   = m_clrBgToggleNormal;
				clrText = m_clrTextToggleNormal;
			}

			pDC->SetBkColor(clrBg);
			pDC->SetTextColor(clrText);
		}

		// Center text
		CRect reCenter = rc;
		pDC->DrawText(strBtnText, -1, rc, DT_SINGLELINE | DT_CALCRECT);
		rc.OffsetRect((reCenter.Width() - rc.Width()) / 2, 
			(reCenter.Height() - rc.Height())/2);

		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawState(rc.TopLeft(), rc.Size(), (LPCTSTR)strBtnText, 
			(nStatus == (int)BTN_STATUS_DISABLE) ? DSS_DISABLED : DSS_NORMAL, TRUE, 0, (CBrush*)NULL);
#ifdef ST_USE_MEMDC
		pDC->SelectObject(pOldFont);
#endif
	}
}

void CDrBtnCtrl::GXPatB(CDC* pDC, int x, int y, int dx, int dy, COLORREF rgb)
{
	// Fill rectangle with color
	RECT    rc;

	rc.left   = x;
	rc.top    = y;
	rc.right  = x + dx;
	rc.bottom = (y + dy);

	COLORREF rgbOld = pDC->SetBkColor(rgb);
	pDC->ExtTextOut(0,0,ETO_OPAQUE,&rc,_T(""),0,NULL);
	pDC->SetBkColor(rgbOld);
}

void CDrBtnCtrl::GXPatB(CDC* pDC, const CRect& rc, COLORREF rgb)
{
	// Fill rectangle with color
	COLORREF rgbOld = pDC->SetBkColor(rgb);
	pDC->ExtTextOut(0,0,ETO_OPAQUE,&rc,_T(""),0,NULL);
	pDC->SetBkColor(rgbOld);
}

void CDrBtnCtrl::SetBtnColor(int nType)
{
	int nIndex;

	nIndex = (int)BTN_STATUS_NORMAL;
	if (nType == 0)
	{
		m_stBtnClr[nIndex].clrFont		  = RGB(0,0,0);
		m_stBtnClr[nIndex].clrBorder		= USELESS_COLOR;
		m_stBtnClr[nIndex].clrBtnBkRnd	= RGB(235,233,221);

		m_stBtnClr[nIndex].clr1DgrLTop	= RGB(255,255,255);
		m_stBtnClr[nIndex].clr2DgrLTop	= RGB(255,255,255);
		m_stBtnClr[nIndex].clr3DgrLTop	= USELESS_COLOR;
		m_stBtnClr[nIndex].clr1DgrRBttm	= RGB(120,118,103);
		m_stBtnClr[nIndex].clr2DgrRBttm	= RGB(120,118,103);
		m_stBtnClr[nIndex].clr3DgrRBttm	= USELESS_COLOR;
	}
	else if (nType == 1)
	{
		m_stBtnClr[nIndex].clrFont		  = RGB(255,255,255);
		m_stBtnClr[nIndex].clrBorder		= RGB(16,112,186);
		m_stBtnClr[nIndex].clrBtnBkRnd	= RGB(28,157,223);

		m_stBtnClr[nIndex].clr1DgrLTop	= RGB(57,191,244);
		m_stBtnClr[nIndex].clr2DgrLTop	= RGB(40,170,230);
		m_stBtnClr[nIndex].clr3DgrLTop	= RGB(31,163,227);
		m_stBtnClr[nIndex].clr1DgrRBttm	= RGB(18,128,203);
		m_stBtnClr[nIndex].clr2DgrRBttm	= RGB(24,147,215);
		m_stBtnClr[nIndex].clr3DgrRBttm	= RGB(27,152,219);
	}
	else if (nType == 2)
	{
		m_stBtnClr[nIndex].clrFont		  = RGB(0, 0, 0);
		m_stBtnClr[nIndex].clrBorder		= RGB(111, 111, 111);
		m_stBtnClr[nIndex].clrBtnBkRnd	= RGB(255, 255, 255);

		m_stBtnClr[nIndex].clr1DgrLTop	= RGB(225, 225, 225);
		m_stBtnClr[nIndex].clr2DgrLTop	= RGB(244, 244, 244);
		m_stBtnClr[nIndex].clr3DgrLTop	= RGB(250, 250, 250);
		m_stBtnClr[nIndex].clr1DgrRBttm	= RGB(181, 181, 181);
		m_stBtnClr[nIndex].clr2DgrRBttm	= RGB(225, 225, 225);
		m_stBtnClr[nIndex].clr3DgrRBttm	= RGB(250, 250, 250);
	}

	nIndex = (int)BTN_STATUS_FOCUS;
	if (nType == 0)
	{
		m_stBtnClr[nIndex].clrFont		  = RGB(0,0,0);
		m_stBtnClr[nIndex].clrBorder		= USELESS_COLOR;
		m_stBtnClr[nIndex].clrBtnBkRnd	= RGB(253,233,221);

		m_stBtnClr[nIndex].clr1DgrLTop	= RGB(255,255,255);
		m_stBtnClr[nIndex].clr2DgrLTop	= RGB(255,255,255);
		m_stBtnClr[nIndex].clr3DgrLTop	= USELESS_COLOR;
		m_stBtnClr[nIndex].clr1DgrRBttm	= RGB(120,118,103);
		m_stBtnClr[nIndex].clr2DgrRBttm	= RGB(120,118,103);
		m_stBtnClr[nIndex].clr3DgrRBttm	= USELESS_COLOR;
	}
	else if (nType == 1)
	{
		m_stBtnClr[nIndex].clrFont		= RGB(255,255,255);
		m_stBtnClr[nIndex].clrBorder	= RGB(16,112,186);
		m_stBtnClr[nIndex].clrBtnBkRnd	= RGB(28,157,223);

		m_stBtnClr[nIndex].clr1DgrLTop	= RGB(57,191,244);
		m_stBtnClr[nIndex].clr2DgrLTop	= RGB(40,170,230);
		m_stBtnClr[nIndex].clr3DgrLTop	= RGB(31,163,227);
		m_stBtnClr[nIndex].clr1DgrRBttm	= RGB(57,191,244);
		m_stBtnClr[nIndex].clr2DgrRBttm	= RGB(24,147,215);
		m_stBtnClr[nIndex].clr3DgrRBttm	= RGB(27,152,219);
	}

	nIndex = (int)BTN_STATUS_DOWN;
	if (nType == 0)
	{
		m_stBtnClr[nIndex].clrFont		= RGB(0,0,0);
		m_stBtnClr[nIndex].clrBorder	= USELESS_COLOR;
		m_stBtnClr[nIndex].clrBtnBkRnd	= RGB(195,195,195);

		m_stBtnClr[nIndex].clr1DgrLTop	= RGB(121,119,104);
		m_stBtnClr[nIndex].clr2DgrLTop	= RGB(121,119,104);
		m_stBtnClr[nIndex].clr3DgrLTop	= USELESS_COLOR;
		m_stBtnClr[nIndex].clr1DgrRBttm	= RGB(255,255,255);
		m_stBtnClr[nIndex].clr2DgrRBttm	= RGB(255,255,255);
		m_stBtnClr[nIndex].clr3DgrRBttm	= USELESS_COLOR;
	}
	else if (nType == 1)
	{
		m_stBtnClr[nIndex].clrFont		= RGB(255,255,255);
		m_stBtnClr[nIndex].clrBorder	= RGB(16,112,186);
		m_stBtnClr[nIndex].clrBtnBkRnd	= RGB(28,157,223);

		m_stBtnClr[nIndex].clr1DgrLTop	= RGB(18,128,203);
		m_stBtnClr[nIndex].clr2DgrLTop	= RGB(24,147,215);
		m_stBtnClr[nIndex].clr3DgrLTop	= RGB(27,152,219);
		m_stBtnClr[nIndex].clr1DgrRBttm	= RGB(57,191,244);
		m_stBtnClr[nIndex].clr2DgrRBttm	= RGB(40,170,230);
		m_stBtnClr[nIndex].clr3DgrRBttm	= RGB(31,163,227);
	}
	else if (nType == 2)
	{
		m_stBtnClr[nIndex].clrFont		  = RGB(0, 0, 0);
		m_stBtnClr[nIndex].clrBorder		= RGB(111, 111, 111);
		m_stBtnClr[nIndex].clrBtnBkRnd	= RGB(197, 197, 197);

		m_stBtnClr[nIndex].clr1DgrLTop	= RGB(66, 66, 66);
		m_stBtnClr[nIndex].clr2DgrLTop	= RGB(91, 91, 91);
		m_stBtnClr[nIndex].clr3DgrLTop	= RGB(197, 197, 197);
		m_stBtnClr[nIndex].clr1DgrRBttm	= RGB(225, 225, 225);
		m_stBtnClr[nIndex].clr2DgrRBttm	= RGB(222, 222, 222);
		m_stBtnClr[nIndex].clr3DgrRBttm	= RGB(197, 197, 197);
	}

	nIndex = (int)BTN_STATUS_DISABLE;
	if (nType == 0)
	{
		m_stBtnClr[nIndex].clrFont		  = RGB(128,128,128);
		m_stBtnClr[nIndex].clrBorder		= USELESS_COLOR;
		m_stBtnClr[nIndex].clrBtnBkRnd	= RGB(167,167,167);

		m_stBtnClr[nIndex].clr1DgrLTop	= RGB(255,255,255);
		m_stBtnClr[nIndex].clr2DgrLTop	= RGB(255,255,255);
		m_stBtnClr[nIndex].clr3DgrLTop	= USELESS_COLOR;
		m_stBtnClr[nIndex].clr1DgrRBttm	= RGB(110,110,110);
		m_stBtnClr[nIndex].clr2DgrRBttm	= RGB(110,110,110);
		m_stBtnClr[nIndex].clr3DgrRBttm	= USELESS_COLOR;
	}
	else if (nType == 1)
	{
		m_stBtnClr[nIndex].clrFont		  = RGB(242,242,242);
		m_stBtnClr[nIndex].clrBorder		= RGB(111,111,111);
		m_stBtnClr[nIndex].clrBtnBkRnd	= RGB(163,163,163);

		m_stBtnClr[nIndex].clr1DgrLTop	= RGB(228,228,228);
		m_stBtnClr[nIndex].clr2DgrLTop	= RGB(187,187,187);
		m_stBtnClr[nIndex].clr3DgrLTop	= RGB(167,167,167);
		m_stBtnClr[nIndex].clr1DgrRBttm	= RGB(126,126,126);
		m_stBtnClr[nIndex].clr2DgrRBttm	= RGB(147,147,147);
		m_stBtnClr[nIndex].clr3DgrRBttm	= RGB(167,167,167);
	}
	else if (nType == 2)
	{
		m_stBtnClr[nIndex].clrFont		  = RGB(189, 189, 189);
		m_stBtnClr[nIndex].clrBorder		= RGB(111, 111, 111);
		m_stBtnClr[nIndex].clrBtnBkRnd	= RGB(247, 247, 247);

		m_stBtnClr[nIndex].clr1DgrLTop	= RGB(255, 255, 255);
		m_stBtnClr[nIndex].clr2DgrLTop	= RGB(237, 237, 237);
		m_stBtnClr[nIndex].clr3DgrLTop	= RGB(247, 247, 247);
		m_stBtnClr[nIndex].clr1DgrRBttm	= RGB(181, 181, 181);
		m_stBtnClr[nIndex].clr2DgrRBttm	= RGB(215, 215, 215);
		m_stBtnClr[nIndex].clr3DgrRBttm	= RGB(247, 247, 247);
	}
}

void CDrBtnCtrl::SetLoadImage(int nImgID, int nImgWidth)
{
	m_nBitmapID = nImgID;
	m_bSetImgBtn = TRUE;
	m_imglistTree.DeleteImageList();
	//	m_imglistTree.Create(nImgID, nImgWidth, 4, RGB(255, 0, 255));
	/*m_imglistTree.Create(20, 20 ,ILC_COLOR24 | ILC_MASK, 0, 4);*/
	
	if(nImgWidth == 0)
		m_imglistTree.Create(20, 20, ILC_COLOR24 | ILC_MASK, 0, 4);
	else
		m_imglistTree.Create(nImgWidth, 20, ILC_COLOR24 | ILC_MASK, 0, 4);
	
	CBitmap bm;	
	bm.LoadBitmap(nImgID);
	m_imglistTree.Add(&bm, RGB(255, 0, 255));


	//	m_imglistTree.Add(AfxGetApp()->LoadIcon(nImgID));
}

BOOL CDrBtnCtrl::GetBitmapSelected(CDC* pDC, int nIndex, CBitmap* pSrc)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bmpWhole;
	int nResult = bmpWhole.LoadBitmap(m_nBitmapID );

	BITMAP bmpInfo;
	nResult = bmpWhole.GetBitmap(&bmpInfo);

	int nWidth = bmpInfo.bmWidth / 4;
	int nHeight= bmpInfo.bmHeight;

	//	CBitmap* pOldBitmap = memDC.SelectObject(&bmpWhole);	

	m_imglistTree.Draw(pDC, nIndex, CPoint(0, 0), ILD_TRANSPARENT );

	//	pDC->BitBlt(0, 0, nWidth, nHeight, &memDC, nWidth*nIndex, 0, SRCCOPY);
	pSrc = pDC->GetCurrentBitmap();


	//	memDC.SelectObject(pOldBitmap);
	bmpWhole.DeleteObject();
	memDC.DeleteDC();

	return TRUE;
}

//20090710 SJ_KIM MouseOver
void CDrBtnCtrl::SetMOverEventOpt(int nFlag)
{
	m_nMOverOpt = nFlag;
}

#undef ST_USE_MEMDC
#undef ST_LIKEIE

void CDrBtnCtrl::DrawGradientRect(CDC* pDC, CRect& rc, COLORREF colorStart, COLORREF colorEnd)
{
	TRIVERTEX verTex[2];
	GRADIENT_RECT gradentRect;

	memset(verTex, 0x00, sizeof(TRIVERTEX) *2);
	memset(&gradentRect, 0x00, sizeof(GRADIENT_RECT));

	verTex[0].x = rc.left;
	verTex[0].y = rc.top;

	verTex[0].Alpha		= 0;
	verTex[0].Red	= GetRValue(colorStart) << 8;
	verTex[0].Green	= GetGValue(colorStart) << 8;
	verTex[0].Blue	= GetBValue(colorStart) << 8;

	verTex[1].x = rc.right;
	verTex[1].y = rc.bottom;

	verTex[1].Alpha		= 0x0000;
	verTex[1].Red	= GetRValue(colorEnd) << 8;
	verTex[1].Green	= GetGValue(colorEnd) << 8;
	verTex[1].Blue	= GetBValue(colorEnd) << 8;

	gradentRect.UpperLeft = 0;
	gradentRect.LowerRight = 1;

//	pDC->GradientFill(verTex, 2, &gradentRect, 1, GRADIENT_FILL_RECT_V);

}

void CDrBtnCtrl::SetBtnType(int nType)
{
	m_nType = nType;
}

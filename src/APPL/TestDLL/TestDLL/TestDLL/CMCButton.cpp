#include "pch.h"
#include "CMCButton.h"

#define LOG(fmt, ...) do { \
    char buffer[512]; \
    snprintf(buffer, sizeof(buffer), fmt, __VA_ARGS__); \
    OutputDebugStringA(buffer); \
} while(0)

#ifdef	BTNST_USE_SOUND
#pragma comment(lib, "winmm.lib")
#include <Mmsystem.h>
#endif

#define BS_TYPEMASK SS_TYPEMASK// .net 일 경우 주석처리

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

#pragma warning (disable : 6387)

#define DF_JUST_COMMENT

CMCButton::CMCButton()
{
	m_bIsPressed = FALSE;
	m_bIsFocused = FALSE;
	m_bIsDisabled = FALSE;
	m_bMouseOnButton = FALSE;

	FreeResources(FALSE);

	// Default type is "flat" button
	m_bIsFlat = TRUE;
	// Button will be tracked also if when the window is inactive (like Internet Explorer)
	m_bAlwaysTrack = TRUE;

	// By default draw border in "flat" button 
	m_bDrawBorder = TRUE;

	// By default icon is aligned horizontally
	m_byAlign = ST_ALIGN_HORIZ;

	// By default use usual pressed style
	SetPressedStyle(BTNST_PRESSED_LEFTRIGHT, FALSE);

	// By default, for "flat" button, don't draw the focus rect
	m_bDrawFlatFocus = FALSE;

	// By default the button is not the default button
	m_bIsDefault = FALSE;
	// Invalid value, since type still unknown
	m_nTypeStyle = BS_TYPEMASK;

	// By default the button is not a checkbox
	m_bIsCheckBox = FALSE;
	m_nCheck = 0;

	// Set default colors
	SetDefaultColors(FALSE);

	// No tooltip created
	m_ToolTip.m_hWnd = NULL;
	m_dwToolTipStyle = 0;

	// Do not draw as a transparent button
	m_bDrawTransparent = FALSE;
	m_pbmpOldBk = NULL;

	// No URL defined
	SetURL(NULL);

	// No cursor defined
	m_hCursor = NULL;

	// No associated menu
/*#ifndef	BTNST_USE_BCMENU
	m_hMenu = NULL;
#endif*/
	m_hParentWndMenu = NULL;
	m_bMenuDisplayed = FALSE;

	m_bShowDisabledBitmap = TRUE;

	m_ptImageOrg.x = 3;
	m_ptImageOrg.y = 3;

	// No defined callbacks
	::ZeroMemory(&m_csCallbacks, sizeof(m_csCallbacks));

#ifdef	BTNST_USE_SOUND
	// No defined sounds
	::ZeroMemory(&m_csSounds, sizeof(m_csSounds));
#endif
	CString	fontName;
	fontName = _T("굴림체");
	m_Bfont.CreateFont(12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,	           // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,           // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		fontName);                   // lpszFacename

}

void CMCButton::AdjustFontSize(CWnd* pWnd, CFont& font, LPCTSTR fontName, int weight)
{
	LOG("[DrawItem][%x][%s]", pWnd, fontName);
	if (!pWnd) 
		return;

	// 윈도우 크기 가져오기
	CRect rect;
	pWnd->GetClientRect(&rect);
	int windowHeight = rect.Height();

	// 폰트 크기 계산 (윈도우 높이의 5%로 설정)
	int fontSize = windowHeight / 1.5;  // 비율 조정 (원하는 값으로 변경 가능)

	// 최소/최대 폰트 크기 설정
	fontSize = max(10, min(fontSize, 30));  // 최소 10, 최대 30
	LOG("[DrawItem] fontSize= [%d] windowHeight=[%d]", fontSize, windowHeight);
	// 기존 폰트 삭제 후 새 폰트 생성
	font.DeleteObject();
	font.CreateFont(
		fontSize,                // nHeight (윈도우 크기 기반)
		0,                       // nWidth
		0,                       // nEscapement
		0,                       // nOrientation
		weight,                  // nWeight (FW_BOLD 등)
		FALSE,                   // bItalic
		FALSE,                   // bUnderline
		0,                       // cStrikeOut
		DEFAULT_CHARSET,         // nCharSet
		OUT_DEFAULT_PRECIS,      // nOutPrecision
		CLIP_DEFAULT_PRECIS,     // nClipPrecision
		DEFAULT_QUALITY,         // nQuality
		DEFAULT_PITCH | FF_SWISS,// nPitchAndFamily
		fontName);               // lpszFacename
}

BOOL CMCButton::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//cs.style |= BS_OWNERDRAW;
	return CButton::PreCreateWindow(cs);
}
BEGIN_MESSAGE_MAP(CMCButton, CButton)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT(BN_CLICKED, &CMCButton::OnBnClicked)

	ON_WM_ACTIVATE()
	ON_WM_ENABLE()
	ON_WM_CANCELMODE()
	ON_WM_GETDLGCODE()
	ON_WM_CTLCOLOR()
	ON_WM_CTLCOLOR_REFLECT()


	ON_MESSAGE(BM_SETSTYLE, OnSetStyle)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(BM_SETCHECK, OnSetCheck)
	ON_MESSAGE(BM_GETCHECK, OnGetCheck)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CMCButton::FreeResources(BOOL bCheckForNULL)
{
	if (bCheckForNULL)
	{
		// Destroy icons
		// Note: the following two lines MUST be here! even if
		// BoundChecker says they are unnecessary!
		if (m_csIcons[0].hIcon)	::DestroyIcon(m_csIcons[0].hIcon);
		if (m_csIcons[1].hIcon)	::DestroyIcon(m_csIcons[1].hIcon);

		// Destroy bitmaps
		if (m_csBitmaps[0].hBitmap)	::DeleteObject(m_csBitmaps[0].hBitmap);
		if (m_csBitmaps[1].hBitmap)	::DeleteObject(m_csBitmaps[1].hBitmap);

		// Destroy mask bitmaps
		if (m_csBitmaps[0].hMask)	::DeleteObject(m_csBitmaps[0].hMask);
		if (m_csBitmaps[1].hMask)	::DeleteObject(m_csBitmaps[1].hMask);
	} // if

	::ZeroMemory(&m_csIcons, sizeof(m_csIcons));
	::ZeroMemory(&m_csBitmaps, sizeof(m_csBitmaps));
}

void CMCButton::PreSubclassWindow()
{
	UINT nBS;

	nBS = GetButtonStyle();

	// Set initial control type
	m_nTypeStyle = nBS & BS_TYPEMASK;

	// Check if this is a checkbox
	if (nBS & BS_CHECKBOX) 
		m_bIsCheckBox = TRUE;

	// Set initial default state flag
	if (m_nTypeStyle == BS_DEFPUSHBUTTON)
	{
		// Set default state for a default button
		m_bIsDefault = TRUE;

		// Adjust style for default button
		m_nTypeStyle = BS_PUSHBUTTON;
	} // If

	// You should not set the Owner Draw before this call
	// (don't use the resource editor "Owner Draw" or
	// ModifyStyle(0, BS_OWNERDRAW) before calling PreSubclassWindow() )
	ASSERT(m_nTypeStyle != BS_OWNERDRAW);

	// Switch to owner-draw
	ModifyStyle(BS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}

UINT CMCButton::OnGetDlgCode()
{
	UINT nCode = CButton::OnGetDlgCode();

#ifndef DF_JUST_COMMENT
	// Tell the system if we want default state handling 
	// (losing default state always allowed)
	   nCode |= (m_bIsDefault ? DLGC_DEFPUSHBUTTON : DLGC_UNDEFPUSHBUTTON);
#endif

	return nCode;
} // End of OnGetDlgCode

BOOL CMCButton::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);

	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	return CButton::PreTranslateMessage(pMsg);
}

HBRUSH CMCButton::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	// TODO:  여기서 DC의 특성을 변경합니다.
	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	// TODO:  부모 처리기가 호출되지 않을 경우 Null이 아닌 브러시를 반환합니다.
}

void CMCButton::OnSysColorChange()
{
	CButton::OnSysColorChange();

	m_dcBk.DeleteDC();
	m_bmpBk.DeleteObject();
//	SetDefaultColors();
} // End of OnSysColorChange

LRESULT CMCButton::OnSetStyle(WPARAM wParam, LPARAM lParam)
{
	const UINT nNewType = (wParam & BS_TYPEMASK);

	// Update default state flag
	if (nNewType == BS_DEFPUSHBUTTON)
	{
		m_bIsDefault = TRUE;
	} // if
	else if (nNewType == BS_PUSHBUTTON)
	{
		// Losing default state always allowed
		m_bIsDefault = FALSE;
	} // if

	// Can't change control type after owner-draw is set.
	// Let the system process changes to other style bits
	// and redrawing, while keeping owner-draw style
	return DefWindowProc(BM_SETSTYLE,
		(wParam & ~BS_TYPEMASK) | BS_OWNERDRAW, lParam);
} // End of OnSetStyle

LRESULT CMCButton::OnSetCheck(WPARAM wParam, LPARAM lParam)
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

LRESULT CMCButton::OnGetCheck(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_bIsCheckBox);
	return GetCheck();
} // End of OnGetCheck

#ifdef	BTNST_USE_BCMENU
LRESULT CMCButton::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu)
{
	LRESULT lResult;
	if (BCMenu::IsMenu(pMenu))
		lResult = BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	else
		lResult = CButton::OnMenuChar(nChar, nFlags, pMenu);
	return lResult;
} // End of OnMenuChar
#endif

#ifdef	BTNST_USE_BCMENU
void CMCButton::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	BOOL bSetFlag = FALSE;
	if (lpMeasureItemStruct->CtlType == ODT_MENU)
	{
		if (IsMenu((HMENU)lpMeasureItemStruct->itemID) && BCMenu::IsMenu((HMENU)lpMeasureItemStruct->itemID))
		{
			m_menuPopup.MeasureItem(lpMeasureItemStruct);
			bSetFlag = TRUE;
		} // if
	} // if
	if (!bSetFlag) CButton::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
} // End of OnMeasureItem
#endif

void CMCButton::OnEnable(BOOL bEnable)
{
	CButton::OnEnable(bEnable);

	if (bEnable == FALSE)
	{
		CWnd* pWnd = GetParent()->GetNextDlgTabItem(this);
		if (pWnd)
			pWnd->SetFocus();
		else
			GetParent()->SetFocus();

		CancelHover();
	} // if
} // End of OnEnable

void CMCButton::CancelHover()
{
	// Only for flat buttons
	if (m_bIsFlat)
	{
		if (m_bMouseOnButton)
		{
			m_bMouseOnButton = FALSE;
			Invalidate();
		} // if
	} // if
} // End of CancelHover

void CMCButton::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd* wndUnderMouse = NULL;
	CWnd* wndActive = this;
	TRACKMOUSEEVENT		csTME{};

	CButton::OnMouseMove(nFlags, point);

	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);

	// If the mouse enter the button with the left button pressed then do nothing
	if (nFlags & MK_LBUTTON && m_bMouseOnButton == FALSE) 
		return;

	// If our button is not flat then do nothing
	if (m_bIsFlat == FALSE) 
		return;

	if (m_bAlwaysTrack == FALSE)	
		wndActive = GetActiveWindow();

	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_bMouseOnButton)
		{
			
			m_bMouseOnButton = TRUE;
			LOG("[DrawItem][OnMouseMove] m_bMouseOnButton  = [%d]", m_bMouseOnButton);
			Invalidate();

#ifdef	BTNST_USE_SOUND
			// Play sound ?
			if (m_csSounds[0].lpszSound)
				::PlaySound(m_csSounds[0].lpszSound, m_csSounds[0].hMod, m_csSounds[0].dwFlags);
#endif

			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		} // if
	}
	else 
		CancelHover();
}

LRESULT CMCButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	LOG("[DrawItem][OnMouseLeave] m_bMouseOnButton  = [%d]", m_bMouseOnButton);
	CancelHover();
	return 0;
} // End of OnMouseLeave

void CMCButton::OnBnClicked()
{
	SetFocus();

	if (m_bIsCheckBox)
	{
		m_nCheck = !m_nCheck;
		Invalidate();
	}
}

void CMCButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC{};
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);


#ifndef DF_JUST_COMMENT1
	// Checkbox?
	if (m_bIsCheckBox)
	{
		m_bIsPressed = (lpDrawItemStruct->itemState & ODS_SELECTED) || (m_nCheck != 0);
	} // if
	else	// Normal button OR other button style ...
	{
		m_bIsPressed = (lpDrawItemStruct->itemState & ODS_SELECTED);

		// If there is a menu and it's displayed, draw the button as pressed
/*		if (
#ifdef	BTNST_USE_BCMENU
			m_menuPopup.m_hMenu
#else
			m_hMenu
#endif
			&& m_bMenuDisplayed)	m_bIsPressed = TRUE;
*/
	} // else

	m_bIsFocused = (lpDrawItemStruct->itemState & ODS_FOCUS);
	m_bIsDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED);

	CRect itemRect = lpDrawItemStruct->rcItem;

	pDC->SetBkMode(TRANSPARENT);

	// Prepare draw... paint button background

	// Draw transparent?
	if (m_bDrawTransparent)
		PaintBk(pDC);
	else
		OnDrawBackground(pDC, &itemRect);

	CString sTitle;
	GetWindowText(sTitle);

	CRect captionRect = lpDrawItemStruct->rcItem;

	// Draw the icon
	if (m_csIcons[0].hIcon)
	{
		DrawTheIcon(pDC, !sTitle.IsEmpty(), &lpDrawItemStruct->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
	} // if

	if (m_csBitmaps[0].hBitmap)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
		DrawTheBitmap(pDC, !sTitle.IsEmpty(), &lpDrawItemStruct->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
	} // if

	// Write the button title (if any)
	if (sTitle.IsEmpty() == FALSE)
	{
		captionRect.left += 15;
		DrawTheText(pDC, (LPCTSTR)sTitle, &lpDrawItemStruct->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
	} // if

	if (m_bIsCheckBox)
	{
		CRect checkBoxRect = lpDrawItemStruct->rcItem;
		checkBoxRect.right = checkBoxRect.left + 15; // 체크박스 크기
		pDC->DrawFrameControl(&checkBoxRect, DFC_BUTTON, DFCS_BUTTONCHECK | (m_nCheck ? DFCS_CHECKED : 0));
	}

	if (m_bIsFlat == FALSE || (m_bIsFlat && m_bDrawFlatFocus))
	{
		// Draw the focus rect
		if (m_bIsFocused)
		{
			CRect focusRect = itemRect;
			focusRect.DeflateRect(3, 3);
			pDC->DrawFocusRect(&focusRect);
		} // if
	} // if

#else

	if (!pDC)
		return;

	CRect rc = lpDrawItemStruct->rcItem;

	//부모 윈도우 배경 복사 (투명 효과)
	//CWnd* pParent = GetParent();
	//if (pParent)
	//{
	//	CRect parentRect;
	//	pParent->GetClientRect(&parentRect);
	//	pParent->ScreenToClient(&rc); // 위치 변환
	//	pParent->SendMessage(WM_ERASEBKGND, (WPARAM)lpDrawItemStruct->hDC);
	//}

	// 배경을 투명하게 설정
	pDC->SetBkMode(TRANSPARENT);

	// 체크 상태 확인
	BOOL isChecked = m_nCheck;

	m_slog.Format("[onerdraw] [ CMCButton::DrawItem()]  isChecked =[ %d] ", GetCheck());
	OutputDebugString(m_slog);

	//체크박스 그리기
	CRect checkBoxRect = rc;
	checkBoxRect.right = checkBoxRect.left + 15; // 체크박스 크기
	pDC->DrawFrameControl(&checkBoxRect, DFC_BUTTON, DFCS_BUTTONCHECK | (isChecked ? DFCS_CHECKED : 0));

	//텍스트 그리기 (투명 배경)
	CString strText;
	GetWindowText(strText);
	rc.left += 20; // 체크박스 오른쪽에 여백 추가
	pDC->DrawText(strText, rc, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
#endif
}

void CMCButton::PaintBk(CDC* pDC)
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
} 

HBITMAP CMCButton::CreateBitmapMask(HBITMAP hSourceBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTransColor)
{
	//비트맵을 전달받고 특정색깔을 투명처리한다.
	HBITMAP		hMask = NULL;
	HDC			hdcSrc = NULL;
	HDC			hdcDest = NULL;
	HBITMAP		hbmSrcT = NULL;
	HBITMAP		hbmDestT = NULL;
	COLORREF	crSaveBk{};
	COLORREF	crSaveDestText{};

	hMask = ::CreateBitmap(dwWidth, dwHeight, 1, 1, NULL);
	if (hMask == NULL)	return NULL;

	hdcSrc = ::CreateCompatibleDC(NULL);
	hdcDest = ::CreateCompatibleDC(NULL);

	hbmSrcT = (HBITMAP)::SelectObject(hdcSrc, hSourceBitmap);
	hbmDestT = (HBITMAP)::SelectObject(hdcDest, hMask);

	crSaveBk = ::SetBkColor(hdcSrc, crTransColor);

	::BitBlt(hdcDest, 0, 0, dwWidth, dwHeight, hdcSrc, 0, 0, SRCCOPY);

	crSaveDestText = ::SetTextColor(hdcSrc, RGB(255, 255, 255));
	::SetBkColor(hdcSrc, RGB(0, 0, 0));

	::BitBlt(hdcSrc, 0, 0, dwWidth, dwHeight, hdcDest, 0, 0, SRCAND);

	SetTextColor(hdcDest, crSaveDestText);

	::SetBkColor(hdcSrc, crSaveBk);
	::SelectObject(hdcSrc, hbmSrcT);
	::SelectObject(hdcDest, hbmDestT);

	::DeleteDC(hdcSrc);
	::DeleteDC(hdcDest);

	return hMask;
} // End of CreateBitmapMask

void CMCButton::PrepareImageRect(BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, BOOL bIsPressed, DWORD dwWidth, DWORD dwHeight, CRect* rpImage)
{
	CRect rBtn;

	rpImage->CopyRect(rpItem);

	switch (m_byAlign)
	{
	case ST_ALIGN_HORIZ:
		if (bHasTitle == FALSE)
		{
			// Center image horizontally
			rpImage->left += ((rpImage->Width() - (long)dwWidth) / 2);
		}
		else
		{
			// Image must be placed just inside the focus rect
			rpImage->left += m_ptImageOrg.x;
			rpTitle->left += dwWidth + m_ptImageOrg.x;
		}
		// Center image vertically
		rpImage->top += ((rpImage->Height() - (long)dwHeight) / 2);
		break;

	case ST_ALIGN_HORIZ_RIGHT:
		GetClientRect(&rBtn);
		if (bHasTitle == FALSE)
		{
			// Center image horizontally
			rpImage->left += ((rpImage->Width() - (long)dwWidth) / 2);
		}
		else
		{
			// Image must be placed just inside the focus rect
			rpTitle->right = rpTitle->Width() - dwWidth - m_ptImageOrg.x;
			rpTitle->left = m_ptImageOrg.x;
			rpImage->left = rBtn.right - dwWidth - m_ptImageOrg.x;
			// Center image vertically
			rpImage->top += ((rpImage->Height() - (long)dwHeight) / 2);
		}
		break;

	case ST_ALIGN_VERT:
		// Center image horizontally
		rpImage->left += ((rpImage->Width() - (long)dwWidth) / 2);
		if (bHasTitle == FALSE)
		{
			// Center image vertically
			rpImage->top += ((rpImage->Height() - (long)dwHeight) / 2);
		}
		else
		{
			rpImage->top = m_ptImageOrg.y;
			rpTitle->top += dwHeight;
		}
		break;

	case ST_ALIGN_OVERLAP:
		break;
	} // switch

	// If button is pressed then press image also
	if (bIsPressed && m_bIsCheckBox == FALSE)
		rpImage->OffsetRect(m_ptPressedOffset.x, m_ptPressedOffset.y);
} // End of PrepareImageRect

void CMCButton::DrawTheIcon(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled)
{
	BYTE		byIndex = 0;

	// Select the icon to use
	if ((m_bIsCheckBox && bIsPressed) || (!m_bIsCheckBox && (bIsPressed || m_bMouseOnButton)))
		byIndex = 0;
	else
		byIndex = (m_csIcons[1].hIcon == NULL ? 0 : 1);

	CRect	rImage;
	PrepareImageRect(bHasTitle, rpItem, rpCaption, bIsPressed, m_csIcons[byIndex].dwWidth, m_csIcons[byIndex].dwHeight, &rImage);

	// Ole'!
	pDC->DrawState(rImage.TopLeft(),
		rImage.Size(),
		m_csIcons[byIndex].hIcon,
		(bIsDisabled ? DSS_DISABLED : DSS_NORMAL),
		(CBrush*)NULL);
}

void CMCButton::DrawTheBitmap(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled)
{
	HDC			hdcBmpMem = NULL;
	HBITMAP		hbmOldBmp = NULL;
	HDC			hdcMem = NULL;
	HBITMAP		hbmT = NULL;

	BYTE		byIndex = 0;

	// Select the bitmap to use
	if ((m_bIsCheckBox && bIsPressed) || (!m_bIsCheckBox && (bIsPressed || m_bMouseOnButton)))
		byIndex = 0;
	else
		byIndex = (m_csBitmaps[1].hBitmap == NULL ? 0 : 1);

	CRect	rImage;
	int bmpWidth{}, bmpHeight{};
	//PrepareImageRect(bHasTitle, rpItem, rpCaption, bIsPressed, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, &rImage);
	BITMAP bmp;
	if (GetObject(m_csBitmaps[1].hBitmap, sizeof(BITMAP), &bmp)) {
		bmpWidth = bmp.bmWidth;
		bmpHeight = bmp.bmHeight;
	}

	hdcBmpMem = ::CreateCompatibleDC(pDC->m_hDC);

	hbmOldBmp = (HBITMAP)::SelectObject(hdcBmpMem, m_csBitmaps[byIndex].hBitmap);

	hdcMem = ::CreateCompatibleDC(NULL);

	hbmT = (HBITMAP)::SelectObject(hdcMem, m_csBitmaps[byIndex].hMask);

	if (bIsDisabled && m_bShowDisabledBitmap)
	{
		if (!m_bEnableRGBColor)  //testlogin
		{
			HDC		hDC = NULL;
			HBITMAP	hBitmap = NULL;

			hDC = ::CreateCompatibleDC(pDC->m_hDC);
			hBitmap = ::CreateCompatibleBitmap(pDC->m_hDC, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight);
			HBITMAP	hOldBmp2 = (HBITMAP)::SelectObject(hDC, hBitmap);

			RECT	rRect;
			rRect.left = 0;
			rRect.top = 0;
			rRect.right = rImage.right + 1;
			rRect.bottom = rImage.bottom + 1;
			::FillRect(hDC, &rRect, (HBRUSH)RGB(255, 255, 255));

			const COLORREF crOldColor = ::SetBkColor(hDC, RGB(255, 255, 255));

			if (0)
			{
				::BitBlt(hDC, 0, 0, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, hdcMem, 0, 0, SRCAND);
				::BitBlt(hDC, 0, 0, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, hdcBmpMem, 0, 0, SRCPAINT);
			}
			else
			{
				::StretchBlt(hDC, 0, 0, rRect.right, rRect.bottom,
					hdcMem, 0, 0, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, SRCAND);
				::StretchBlt(hDC, 0, 0, rRect.right, rRect.bottom,
					hdcBmpMem, 0, 0, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, SRCPAINT);
			}
			

			::SetBkColor(hDC, crOldColor);
			::SelectObject(hDC, hOldBmp2);
			::DeleteDC(hDC);

			pDC->DrawState(CPoint(rImage.left/*+1*/, rImage.top),
				CSize(m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight),
				hBitmap, DST_BITMAP | DSS_DISABLED);

			::DeleteObject(hBitmap);
		}
		else
		{
			CRect rec;
			rec.SetRect(0, 0, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight);
			rec.OffsetRect(3, 1);
			pDC->FillSolidRect(rec, m_clrEnableCheck);
		}
	} // if
	else
	{
		if (0)
		{
			::BitBlt(pDC->m_hDC, rImage.left, rImage.top, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, hdcMem, 0, 0, SRCAND);
			::BitBlt(pDC->m_hDC, rImage.left, rImage.top, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, hdcBmpMem, 0, 0, SRCPAINT);
		}
		else
		{
			CRect rec;
			GetClientRect(rec);

			m_slog.Format("[DrawItem] byIndex = [%d]", byIndex);
			
			LOG(m_slog);

			::StretchBlt(pDC->m_hDC, 0, 0, rec.Width(), rec.Height(),
				hdcMem, 0, 0, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, SRCAND);

			::StretchBlt(pDC->m_hDC, 0, 0, rec.Width(), rec.Height(),
				hdcBmpMem, 0, 0, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, SRCPAINT);
		}
	} // else

	::SelectObject(hdcMem, hbmT);
	::DeleteDC(hdcMem);

	::SelectObject(hdcBmpMem, hbmOldBmp);
	::DeleteDC(hdcBmpMem);
}

void CMCButton::DrawTheText(CDC* pDC, LPCTSTR lpszText, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled)
{
	CFont* font = NULL;
#if 1	// bold text............
	if (m_bIsPressed || m_bMouseOnButton)
		font = pDC->SelectObject(&m_Bfont);
#endif

	// Draw the button's title
	// If button is pressed then "press" title also
	if (m_bIsPressed && m_bIsCheckBox == FALSE)
		rpCaption->OffsetRect(m_ptPressedOffset.x, m_ptPressedOffset.y);

	// ONLY FOR DEBUG 
	//CBrush brBtnShadow(RGB(255, 0, 0));
	//pDC->FrameRect(rCaption, &brBtnShadow);



	const CRect centerRect = rpCaption;
#if 0	// Center text
	pDC->DrawText(lpszText, -1, rpCaption, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
	rpCaption->OffsetRect((centerRect.Width() - rpCaption->Width()) / 2, (centerRect.Height() - rpCaption->Height()) / 2);
#else	// left text
	rpCaption->OffsetRect(5, 0);
#endif
	/* RFU
	rpCaption->OffsetRect(0, (centerRect.Height() - rpCaption->Height())/2);
	rpCaption->OffsetRect((centerRect.Width() - rpCaption->Width())-4, (centerRect.Height() - rpCaption->Height())/2);
	*/

	pDC->SetBkMode(TRANSPARENT);
	/*
	pDC->DrawState(rCaption->TopLeft(), rCaption->Size(), (LPCTSTR)sTitle, (bIsDisabled ? DSS_DISABLED : DSS_NORMAL),
					TRUE, 0, (CBrush*)NULL);
	*/
	if (m_bIsDisabled)
	{
		rpCaption->OffsetRect(1, 1);
		pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
		pDC->DrawText(lpszText, -1, rpCaption, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		rpCaption->OffsetRect(-1, -1);
		pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
		pDC->DrawText(lpszText, -1, rpCaption, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	} // if
	else
	{
		if (m_bMouseOnButton || m_bIsPressed)
		{
			pDC->SetTextColor(m_crColors[BTNST_COLOR_FG_IN]);
			pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_IN]);
		} // if
		else
		{
			if (m_bIsFocused)
			{
				pDC->SetTextColor(m_crColors[BTNST_COLOR_FG_FOCUS]);
				pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_FOCUS]);
			} // if
			else
			{
				pDC->SetTextColor(m_crColors[BTNST_COLOR_FG_OUT]);
				pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_OUT]);
			} // else
		} // else
		//pDC->DrawText(lpszText, -1, rpCaption, DT_WORDBREAK | DT_CENTER);
		pDC->DrawText(lpszText, -1, rpCaption, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	} // if

	if (font)	
		pDC->SelectObject(font);
}

HBITMAP CMCButton::CreateGrayscaleBitmap(HBITMAP hBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTrans)
{
	HBITMAP		hGrayBitmap = NULL;
	HDC			hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;

	hMainDC = ::GetDC(NULL);
	if (hMainDC == NULL)	return NULL;
	hMemDC1 = ::CreateCompatibleDC(hMainDC);
	if (hMemDC1 == NULL)
	{
		::ReleaseDC(NULL, hMainDC);
		return NULL;
	} // if
	hMemDC2 = ::CreateCompatibleDC(hMainDC);
	if (hMemDC2 == NULL)
	{
		::DeleteDC(hMemDC1);
		::ReleaseDC(NULL, hMainDC);
		return NULL;
	} // if

	hGrayBitmap = ::CreateCompatibleBitmap(hMainDC, dwWidth, dwHeight);
	if (hGrayBitmap)
	{
		hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, hGrayBitmap);
		hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, hBitmap);

		//::BitBlt(hMemDC1, 0, 0, dwWidth, dwHeight, hMemDC2, 0, 0, SRCCOPY);

		DWORD		dwLoopY = 0, dwLoopX = 0;
		COLORREF	crPixel = 0;
		BYTE		byNewPixel = 0;

		for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
		{
			for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
			{
				crPixel = ::GetPixel(hMemDC2, dwLoopX, dwLoopY);
				byNewPixel = (BYTE)((GetRValue(crPixel) * 0.299) + (GetGValue(crPixel) * 0.587) + (GetBValue(crPixel) * 0.114));

				if (crPixel != crTrans)
					::SetPixel(hMemDC1, dwLoopX, dwLoopY, RGB(byNewPixel, byNewPixel, byNewPixel));
				else
					::SetPixel(hMemDC1, dwLoopX, dwLoopY, crPixel);
			} // for
		} // for

		::SelectObject(hMemDC1, hOldBmp1);
		::SelectObject(hMemDC2, hOldBmp2);
	} // if

	::DeleteDC(hMemDC1);
	::DeleteDC(hMemDC2);
	::ReleaseDC(NULL, hMainDC);

	return hGrayBitmap;
} // End of CreateGrayscaleBitmap

HBITMAP CMCButton::CreateDarkerBitmap(HBITMAP hBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTrans)
{
	HBITMAP		hGrayBitmap = NULL;
	HDC			hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;

	hMainDC = ::GetDC(NULL);
	if (hMainDC == NULL)	return NULL;
	hMemDC1 = ::CreateCompatibleDC(hMainDC);
	if (hMemDC1 == NULL)
	{
		::ReleaseDC(NULL, hMainDC);
		return NULL;
	} // if
	hMemDC2 = ::CreateCompatibleDC(hMainDC);
	if (hMemDC2 == NULL)
	{
		::DeleteDC(hMemDC1);
		::ReleaseDC(NULL, hMainDC);
		return NULL;
	} // if

	hGrayBitmap = ::CreateCompatibleBitmap(hMainDC, dwWidth, dwHeight);
	if (hGrayBitmap)
	{
		hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, hGrayBitmap);
		hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, hBitmap);

		//::BitBlt(hMemDC1, 0, 0, dwWidth, dwHeight, hMemDC2, 0, 0, SRCCOPY);

		DWORD		dwLoopY = 0, dwLoopX = 0;
		COLORREF	crPixel = 0;

		for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
		{
			for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
			{
				crPixel = ::GetPixel(hMemDC2, dwLoopX, dwLoopY);

				if (crPixel != crTrans)
					::SetPixel(hMemDC1, dwLoopX, dwLoopY, DarkenColor(crPixel, 0.25));
				else
					::SetPixel(hMemDC1, dwLoopX, dwLoopY, crPixel);
			} // for
		} // for

		::SelectObject(hMemDC1, hOldBmp1);
		::SelectObject(hMemDC2, hOldBmp2);
	} // if

	::DeleteDC(hMemDC1);
	::DeleteDC(hMemDC2);
	::ReleaseDC(NULL, hMainDC);

	return hGrayBitmap;
} // End of CreateDarkerBitmap

HICON CMCButton::CreateGrayscaleIcon(HICON hIcon)
{
	HICON		hGrayIcon = NULL;
	HDC			hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	BITMAP		bmp{};
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;
	ICONINFO	csII{}, csGrayII{};
	BOOL		bRetValue = FALSE;

	bRetValue = ::GetIconInfo(hIcon, &csII);
	if (bRetValue == FALSE)	return NULL;

	hMainDC = ::GetDC(NULL);
	hMemDC1 = ::CreateCompatibleDC(hMainDC);
	hMemDC2 = ::CreateCompatibleDC(hMainDC);
	if (hMainDC == NULL || hMemDC1 == NULL || hMemDC2 == NULL)	return NULL;

	if (::GetObject(csII.hbmColor, sizeof(BITMAP), &bmp))
	{
		const DWORD	dwWidth = csII.xHotspot * 2;
		const DWORD	dwHeight = csII.yHotspot * 2;

		csGrayII.hbmColor = ::CreateBitmap(dwWidth, dwHeight, bmp.bmPlanes, bmp.bmBitsPixel, NULL);
		if (csGrayII.hbmColor)
		{
			hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, csII.hbmColor);
			hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, csGrayII.hbmColor);

			//::BitBlt(hMemDC2, 0, 0, dwWidth, dwHeight, hMemDC1, 0, 0, SRCCOPY);

			DWORD		dwLoopY = 0, dwLoopX = 0;
			COLORREF	crPixel = 0;
			BYTE		byNewPixel = 0;

			for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
			{
				for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
				{
					crPixel = ::GetPixel(hMemDC1, dwLoopX, dwLoopY);
					byNewPixel = (BYTE)((GetRValue(crPixel) * 0.299) + (GetGValue(crPixel) * 0.587) + (GetBValue(crPixel) * 0.114));

					if (crPixel)
						::SetPixel(hMemDC2, dwLoopX, dwLoopY, RGB(byNewPixel, byNewPixel, byNewPixel));
					else
						::SetPixel(hMemDC2, dwLoopX, dwLoopY, crPixel);
				} // for
			} // for

			::SelectObject(hMemDC1, hOldBmp1);
			::SelectObject(hMemDC2, hOldBmp2);

			csGrayII.hbmMask = csII.hbmMask;

			csGrayII.fIcon = TRUE;
			hGrayIcon = ::CreateIconIndirect(&csGrayII);
		} // if

		::DeleteObject(csGrayII.hbmColor);
		//::DeleteObject(csGrayII.hbmMask);
	} // if

	::DeleteObject(csII.hbmColor);
	::DeleteObject(csII.hbmMask);
	::DeleteDC(hMemDC1);
	::DeleteDC(hMemDC2);
	::ReleaseDC(NULL, hMainDC);

	return hGrayIcon;
} // End of CreateGrayscaleIcon

HICON CMCButton::CreateDarkerIcon(HICON hIcon)
{
	HICON		hGrayIcon = NULL;
	HDC			hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	BITMAP		bmp{};
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;
	ICONINFO	csII{}, csGrayII{};
	BOOL		bRetValue = FALSE;

	bRetValue = ::GetIconInfo(hIcon, &csII);
	if (bRetValue == FALSE)	return NULL;

	hMainDC = ::GetDC(NULL);
	hMemDC1 = ::CreateCompatibleDC(hMainDC);
	hMemDC2 = ::CreateCompatibleDC(hMainDC);
	if (hMainDC == NULL || hMemDC1 == NULL || hMemDC2 == NULL)	return NULL;

	if (::GetObject(csII.hbmColor, sizeof(BITMAP), &bmp))
	{
		const DWORD	dwWidth = csII.xHotspot * 2;
		const DWORD	dwHeight = csII.yHotspot * 2;

		csGrayII.hbmColor = ::CreateBitmap(dwWidth, dwHeight, bmp.bmPlanes, bmp.bmBitsPixel, NULL);
		if (csGrayII.hbmColor)
		{
			hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, csII.hbmColor);
			hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, csGrayII.hbmColor);

			//::BitBlt(hMemDC2, 0, 0, dwWidth, dwHeight, hMemDC1, 0, 0, SRCCOPY);

			DWORD		dwLoopY = 0, dwLoopX = 0;
			COLORREF	crPixel = 0;

			for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
			{
				for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
				{
					crPixel = ::GetPixel(hMemDC1, dwLoopX, dwLoopY);

					if (crPixel)
						::SetPixel(hMemDC2, dwLoopX, dwLoopY, DarkenColor(crPixel, 0.25));
					else
						::SetPixel(hMemDC2, dwLoopX, dwLoopY, crPixel);
				} // for
			} // for

			::SelectObject(hMemDC1, hOldBmp1);
			::SelectObject(hMemDC2, hOldBmp2);

			csGrayII.hbmMask = csII.hbmMask;

			csGrayII.fIcon = TRUE;
			hGrayIcon = ::CreateIconIndirect(&csGrayII);
		} // if

		::DeleteObject(csGrayII.hbmColor);
		//::DeleteObject(csGrayII.hbmMask);
	} // if

	::DeleteObject(csII.hbmColor);
	::DeleteObject(csII.hbmMask);
	::DeleteDC(hMemDC1);
	::DeleteDC(hMemDC2);
	::ReleaseDC(NULL, hMainDC);

	return hGrayIcon;
} // End of CreateDarkerIcon

COLORREF CMCButton::DarkenColor(COLORREF crColor, double dFactor)
{
	if (dFactor > 0.0 && dFactor <= 1.0)
	{
		BYTE red{}, green{}, blue{}, lightred{}, lightgreen{}, lightblue{};
		red = GetRValue(crColor);
		green = GetGValue(crColor);
		blue = GetBValue(crColor);
		lightred = (BYTE)(red - (dFactor * red));
		lightgreen = (BYTE)(green - (dFactor * green));
		lightblue = (BYTE)(blue - (dFactor * blue));
		crColor = RGB(lightred, lightgreen, lightblue);
	} // if

	return crColor;
} // End of DarkenColor

DWORD CMCButton::SetIcon(int nIconIn, int nCxDesiredIn, int nCyDesiredIn, int nIconOut, int nCxDesiredOut, int nCyDesiredOut)
{
	HICON		hIconIn = NULL;
	HICON		hIconOut = NULL;
	HINSTANCE	hInstResource = NULL;

	// Find correct resource handle
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nIconIn), RT_GROUP_ICON);

	// Set icon when the mouse is IN the button
	hIconIn = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(nIconIn), IMAGE_ICON, nCxDesiredIn, nCyDesiredIn, 0);

	// Set icon when the mouse is OUT the button
	switch (nIconOut)
	{
	case NULL:
		break;
	case (int)BTNST_AUTO_GRAY:
		hIconOut = BTNST_AUTO_GRAY;
		break;
	case (int)BTNST_AUTO_DARKER:
		hIconOut = BTNST_AUTO_DARKER;
		break;
	default:
		hIconOut = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(nIconOut), IMAGE_ICON, nCxDesiredOut, nCyDesiredOut, 0);
		break;
	} // switch

	return SetIcon(hIconIn, hIconOut);
} // End of SetIcon

DWORD CMCButton::SetIcon(int nIconIn, int nIconOut)
{
	return SetIcon(nIconIn, 0, 0, nIconOut, 0, 0);
} // End of SetIcon

DWORD CMCButton::SetIcon(HICON hIconIn, HICON hIconOut)
{
	BOOL		bRetValue{};
	ICONINFO	ii{};

	// Free any loaded resource
	FreeResources();

	if (hIconIn)
	{
		// Icon when mouse over button?
		m_csIcons[0].hIcon = hIconIn;
		// Get icon dimension
		::ZeroMemory(&ii, sizeof(ICONINFO));
		bRetValue = ::GetIconInfo(hIconIn, &ii);
		if (bRetValue == FALSE)
		{
			FreeResources();
			return BTNST_INVALIDRESOURCE;
		} // if

		m_csIcons[0].dwWidth = (DWORD)(ii.xHotspot * 2);
		m_csIcons[0].dwHeight = (DWORD)(ii.yHotspot * 2);
		::DeleteObject(ii.hbmMask);
		::DeleteObject(ii.hbmColor);

		// Icon when mouse outside button?
		if (hIconOut)
		{
			switch ((int)hIconOut)
			{
			case (int)BTNST_AUTO_GRAY:
				hIconOut = CreateGrayscaleIcon(hIconIn);
				break;
			case (int)BTNST_AUTO_DARKER:
				hIconOut = CreateDarkerIcon(hIconIn);
				break;
			} // switch

			m_csIcons[1].hIcon = hIconOut;
			// Get icon dimension
			::ZeroMemory(&ii, sizeof(ICONINFO));
			bRetValue = ::GetIconInfo(hIconOut, &ii);
			if (bRetValue == FALSE)
			{
				FreeResources();
				return BTNST_INVALIDRESOURCE;
			} // if

			m_csIcons[1].dwWidth = (DWORD)(ii.xHotspot * 2);
			m_csIcons[1].dwHeight = (DWORD)(ii.yHotspot * 2);
			::DeleteObject(ii.hbmMask);
			::DeleteObject(ii.hbmColor);
		} // if
	} // if

	Invalidate();

	return BTNST_OK;
} // End of SetIcon

DWORD CMCButton::SetBitmaps(int nBitmapIn, COLORREF crTransColorIn, int nBitmapOut, COLORREF crTransColorOut)
{
	HBITMAP		hBitmapIn = NULL;
	HBITMAP		hBitmapOut = NULL;
	HINSTANCE	hInstResource = NULL;

	// Find correct resource handle
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapIn), RT_BITMAP);

	// Load bitmap In
	hBitmapIn = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapIn), IMAGE_BITMAP, 0, 0, 0);

	// Load bitmap Out
	switch (nBitmapOut)
	{
	case NULL:
		break;
	case (int)BTNST_AUTO_GRAY:
		hBitmapOut = (HBITMAP)BTNST_AUTO_GRAY;
		break;
	case (int)BTNST_AUTO_DARKER:
		hBitmapOut = (HBITMAP)BTNST_AUTO_DARKER;
		break;
	default:
		hBitmapOut = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapOut), IMAGE_BITMAP, 0, 0, 0);
		break;
	} // if

	return SetBitmaps(hBitmapIn, crTransColorIn, hBitmapOut, crTransColorOut);
}

DWORD CMCButton::SetBitmaps(CString sBitmapIn, COLORREF crTransColorIn, CString sBitmapOut, COLORREF crTransColorOut)
{
	int		nRetValue = 0;
	BITMAP	csBitmapSize{};

	HBITMAP		hBitmapIn = NULL;
	HBITMAP		hBitmapOut = NULL;
	HINSTANCE	hInstResource = NULL;

	hBitmapIn = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sBitmapIn,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	hBitmapOut = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sBitmapOut,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	// Free any loaded resource
	FreeResources();

	if (hBitmapIn)
	{
		m_csBitmaps[0].hBitmap = hBitmapIn;
		m_csBitmaps[0].crTransparent = crTransColorIn;
		// Get bitmap size
		nRetValue = ::GetObject(hBitmapIn, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			FreeResources();
			return BTNST_INVALIDRESOURCE;
		} // if
		m_csBitmaps[0].dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_csBitmaps[0].dwHeight = (DWORD)csBitmapSize.bmHeight;

		// Create grayscale/darker bitmap BEFORE mask (of hBitmapIn)
		switch ((int)hBitmapOut)
		{
		case (int)BTNST_AUTO_GRAY:
			hBitmapOut = CreateGrayscaleBitmap(hBitmapIn, m_csBitmaps[0].dwWidth, m_csBitmaps[0].dwHeight, crTransColorIn);
			m_csBitmaps[1].hBitmap = hBitmapOut;
			crTransColorOut = crTransColorIn;
			break;
		case (int)BTNST_AUTO_DARKER:
			hBitmapOut = CreateDarkerBitmap(hBitmapIn, m_csBitmaps[0].dwWidth, m_csBitmaps[0].dwHeight, crTransColorIn);
			m_csBitmaps[1].hBitmap = hBitmapOut;
			crTransColorOut = crTransColorIn;
			break;
		} // switch

		// Create mask for bitmap In
		m_csBitmaps[0].hMask = CreateBitmapMask(hBitmapIn, m_csBitmaps[0].dwWidth, m_csBitmaps[0].dwHeight, crTransColorIn);
		if (m_csBitmaps[0].hMask == NULL)
		{
			FreeResources();
			return BTNST_FAILEDMASK;
		} // if

		if (hBitmapOut)
		{
			m_csBitmaps[1].hBitmap = hBitmapOut;
			m_csBitmaps[1].crTransparent = crTransColorOut;
			// Get bitmap size
			nRetValue = ::GetObject(hBitmapOut, sizeof(csBitmapSize), &csBitmapSize);
			if (nRetValue == 0)
			{
				FreeResources();
				return BTNST_INVALIDRESOURCE;
			} // if
			m_csBitmaps[1].dwWidth = (DWORD)csBitmapSize.bmWidth;
			m_csBitmaps[1].dwHeight = (DWORD)csBitmapSize.bmHeight;

			// Create mask for bitmap Out
			m_csBitmaps[1].hMask = CreateBitmapMask(hBitmapOut, m_csBitmaps[1].dwWidth, m_csBitmaps[1].dwHeight, crTransColorOut);
			if (m_csBitmaps[1].hMask == NULL)
			{
				FreeResources();
				return BTNST_FAILEDMASK;
			} // if
		} // if
	} // if

	Invalidate();

	return BTNST_OK;
	return BTNST_OK;
}

DWORD CMCButton::SetBitmaps(HBITMAP hBitmapIn, COLORREF crTransColorIn, HBITMAP hBitmapOut, COLORREF crTransColorOut)
{
	int		nRetValue = 0;
	BITMAP	csBitmapSize{};

	// Free any loaded resource
	FreeResources();

	if (hBitmapIn)
	{
		m_csBitmaps[0].hBitmap = hBitmapIn;
		m_csBitmaps[0].crTransparent = crTransColorIn;
		// Get bitmap size
		nRetValue = ::GetObject(hBitmapIn, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			FreeResources();
			return BTNST_INVALIDRESOURCE;
		} // if
		m_csBitmaps[0].dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_csBitmaps[0].dwHeight = (DWORD)csBitmapSize.bmHeight;

		// Create grayscale/darker bitmap BEFORE mask (of hBitmapIn)
		switch ((int)hBitmapOut)
		{
		case (int)BTNST_AUTO_GRAY:
			hBitmapOut = CreateGrayscaleBitmap(hBitmapIn, m_csBitmaps[0].dwWidth, m_csBitmaps[0].dwHeight, crTransColorIn);
			m_csBitmaps[1].hBitmap = hBitmapOut;
			crTransColorOut = crTransColorIn;
			break;
		case (int)BTNST_AUTO_DARKER:
			hBitmapOut = CreateDarkerBitmap(hBitmapIn, m_csBitmaps[0].dwWidth, m_csBitmaps[0].dwHeight, crTransColorIn);
			m_csBitmaps[1].hBitmap = hBitmapOut;
			crTransColorOut = crTransColorIn;
			break;
		} // switch

		// Create mask for bitmap In
		m_csBitmaps[0].hMask = CreateBitmapMask(hBitmapIn, m_csBitmaps[0].dwWidth, m_csBitmaps[0].dwHeight, crTransColorIn);
		if (m_csBitmaps[0].hMask == NULL)
		{
			FreeResources();
			return BTNST_FAILEDMASK;
		} // if

		if (hBitmapOut)
		{
			m_csBitmaps[1].hBitmap = hBitmapOut;
			m_csBitmaps[1].crTransparent = crTransColorOut;
			// Get bitmap size
			nRetValue = ::GetObject(hBitmapOut, sizeof(csBitmapSize), &csBitmapSize);
			if (nRetValue == 0)
			{
				FreeResources();
				return BTNST_INVALIDRESOURCE;
			} // if
			m_csBitmaps[1].dwWidth = (DWORD)csBitmapSize.bmWidth;
			m_csBitmaps[1].dwHeight = (DWORD)csBitmapSize.bmHeight;

			// Create mask for bitmap Out
			m_csBitmaps[1].hMask = CreateBitmapMask(hBitmapOut, m_csBitmaps[1].dwWidth, m_csBitmaps[1].dwHeight, crTransColorOut);
			if (m_csBitmaps[1].hMask == NULL)
			{
				FreeResources();
				return BTNST_FAILEDMASK;
			} // if
		} // if
	} // if

	Invalidate();

	return BTNST_OK;
} // End of SetBitmaps

DWORD CMCButton::SetFlat(BOOL bFlat, BOOL bRepaint)
{
	m_bIsFlat = bFlat;
	if (bRepaint)	Invalidate();

	return BTNST_OK;
} // End of SetFlat

DWORD CMCButton::SetAlign(BYTE byAlign, BOOL bRepaint)
{
	switch (byAlign)
	{
	case ST_ALIGN_HORIZ:
	case ST_ALIGN_HORIZ_RIGHT:
	case ST_ALIGN_VERT:
	case ST_ALIGN_OVERLAP:
		m_byAlign = byAlign;
		if (bRepaint)	Invalidate();
		return BTNST_OK;
		break;
	} // switch

	return BTNST_INVALIDALIGN;
} // End of SetAlign

DWORD CMCButton::SetPressedStyle(BYTE byStyle, BOOL bRepaint)
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

DWORD CMCButton::SetCheck(int nCheck, BOOL bRepaint)
{
	if (m_bIsCheckBox)
	{
		if (nCheck == 0) m_nCheck = 0;
		else m_nCheck = 1;

		if (bRepaint) Invalidate();
	} // if

	return BTNST_OK;
} // End of SetCheck

int CMCButton::GetCheck()
{
	return m_nCheck;
}

DWORD CMCButton::SetDefaultColors(BOOL bRepaint)
{
	m_crColors[BTNST_COLOR_BK_IN] = ::GetSysColor(COLOR_BTNFACE);
	m_crColors[BTNST_COLOR_FG_IN] = ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[BTNST_COLOR_BK_OUT] = ::GetSysColor(COLOR_BTNFACE);
	m_crColors[BTNST_COLOR_FG_OUT] = ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[BTNST_COLOR_BK_FOCUS] = ::GetSysColor(COLOR_BTNFACE);
	m_crColors[BTNST_COLOR_FG_FOCUS] = ::GetSysColor(COLOR_BTNTEXT);

	if (bRepaint)	Invalidate();

	return BTNST_OK;
} // End of SetDefaultColors

DWORD CMCButton::SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint)
{
	if (byColorIndex >= BTNST_MAX_COLORS)	return BTNST_INVALIDINDEX;

	// Set new color
	m_crColors[byColorIndex] = crColor;

	if (bRepaint)	Invalidate();

	return BTNST_OK;
} // End of SetColor

DWORD CMCButton::GetColor(BYTE byColorIndex, COLORREF* crpColor)
{
	if (byColorIndex >= BTNST_MAX_COLORS)	return BTNST_INVALIDINDEX;

	// Get color
	*crpColor = m_crColors[byColorIndex];

	return BTNST_OK;
} // End of GetColor

DWORD CMCButton::OffsetColor(BYTE byColorIndex, short shOffset, BOOL bRepaint)
{
	BYTE	byRed = 0;
	BYTE	byGreen = 0;
	BYTE	byBlue = 0;
	short	shOffsetR = shOffset;
	short	shOffsetG = shOffset;
	short	shOffsetB = shOffset;

	if (byColorIndex >= BTNST_MAX_COLORS)	return BTNST_INVALIDINDEX;
	if (shOffset < -255 || shOffset > 255)	return BTNST_BADPARAM;

	// Get RGB components of specified color
	byRed = GetRValue(m_crColors[byColorIndex]);
	byGreen = GetGValue(m_crColors[byColorIndex]);
	byBlue = GetBValue(m_crColors[byColorIndex]);

	// Calculate max. allowed real offset
	if (shOffset > 0)
	{
		if (byRed + shOffset > 255)		shOffsetR = 255 - byRed;
		if (byGreen + shOffset > 255)	shOffsetG = 255 - byGreen;
		if (byBlue + shOffset > 255)	shOffsetB = 255 - byBlue;

		shOffset = min(min(shOffsetR, shOffsetG), shOffsetB);
	} // if
	else
	{
		if (byRed + shOffset < 0)		shOffsetR = -byRed;
		if (byGreen + shOffset < 0)		shOffsetG = -byGreen;
		if (byBlue + shOffset < 0)		shOffsetB = -byBlue;

		shOffset = max(max(shOffsetR, shOffsetG), shOffsetB);
	} // else

	// Set new color
	m_crColors[byColorIndex] = RGB(byRed + shOffset, byGreen + shOffset, byBlue + shOffset);

	if (bRepaint)	Invalidate();

	return BTNST_OK;
} // End of OffsetColor

DWORD CMCButton::SetAlwaysTrack(BOOL bAlwaysTrack)
{
	m_bAlwaysTrack = bAlwaysTrack;
	return BTNST_OK;
} // End of SetAlwaysTrack

DWORD CMCButton::SetBtnCursor(int nCursorId, BOOL bRepaint)
{
	HINSTANCE	hInstResource = NULL;
	// Destroy any previous cursor
	if (m_hCursor)
	{
		::DestroyCursor(m_hCursor);
		m_hCursor = NULL;
	} // if

	// Load cursor
	if (nCursorId)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nCursorId), RT_GROUP_CURSOR);
		// Load cursor resource
		m_hCursor = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(nCursorId), IMAGE_CURSOR, 0, 0, 0);
		// Repaint the button
		if (bRepaint) Invalidate();
		// If something wrong
		if (m_hCursor == NULL) return BTNST_INVALIDRESOURCE;
	} // if

	return BTNST_OK;
} // End of SetBtnCursor

DWORD CMCButton::DrawBorder(BOOL bDrawBorder, BOOL bRepaint)
{
	m_bDrawBorder = bDrawBorder;
	// Repaint the button
	if (bRepaint) Invalidate();

	return BTNST_OK;
} // End of DrawBorder

DWORD CMCButton::DrawFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint)
{
	m_bDrawFlatFocus = bDrawFlatFocus;
	// Repaint the button
	if (bRepaint) Invalidate();

	return BTNST_OK;
} // End of DrawFlatFocus

void CMCButton::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this, m_dwToolTipStyle);
		// Create inactive
		m_ToolTip.Activate(FALSE);
		// Enable multiline
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
		//m_ToolTip.SendMessage(TTM_SETTITLE, TTI_INFO, (LPARAM)_T("Title"));
	} // if
}

void CMCButton::SetTooltipText(int nText, BOOL bActivate)
{
	CString sText;

	// Load string resource
	sText.LoadString(nText);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE) SetTooltipText((LPCTSTR)sText, bActivate);
} // End of SetTooltipText

void CMCButton::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
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
	} // if

	// Set text for tooltip
	m_ToolTip.UpdateTipText(lpszText, this, 1);
	m_ToolTip.Activate(bActivate);
} // End of SetTooltipText

void CMCButton::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
} // End of EnableTooltip

DWORD CMCButton::EnableBalloonTooltip()
{
	m_dwToolTipStyle |= TTS_BALLOON;
	return BTNST_OK;
} // End of EnableBalloonTooltip

BOOL CMCButton::GetDefault()
{
	return m_bIsDefault;
} // End of GetDefault

void CMCButton::DrawTransparent(BOOL bRepaint)
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
	if (bRepaint) Invalidate();
} // End of DrawTransparent

DWORD CMCButton::SetBk(CDC* pDC)
{
	if (m_bDrawTransparent && pDC)
	{
		// Restore old bitmap (if any)
		if (m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
		{
			m_dcBk.SelectObject(m_pbmpOldBk);
		} // if

		m_bmpBk.DeleteObject();
		m_dcBk.DeleteDC();

		CRect rect;
		CRect rect1;

		GetClientRect(rect);

		GetWindowRect(rect1);
		GetParent()->ScreenToClient(rect1);

		m_dcBk.CreateCompatibleDC(pDC);
		m_bmpBk.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		m_pbmpOldBk = m_dcBk.SelectObject(&m_bmpBk);
		m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, rect1.left, rect1.top, SRCCOPY);

		return BTNST_OK;
	} // if

	return BTNST_BADPARAM;
} // End of SetBk

DWORD CMCButton::SetURL(LPCTSTR lpszURL)
{
	// Remove any existing URL
	memset(m_szURL, 0, sizeof(m_szURL));

	if (lpszURL)
	{
		// Store the URL
		_tcsncpy_s(m_szURL, 260, lpszURL, _MAX_PATH);
	} // if

	return BTNST_OK;
} // End of SetURL

void CMCButton::SizeToContent()
{
	if (m_csIcons[0].hIcon)
	{
		m_ptImageOrg.x = 0;
		m_ptImageOrg.y = 0;
		SetWindowPos(NULL, -1, -1, m_csIcons[0].dwWidth, m_csIcons[0].dwHeight,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE);
	} // if
	else
		if (m_csBitmaps[0].hBitmap)
		{
			m_ptImageOrg.x = 0;
			m_ptImageOrg.y = 0;
			SetWindowPos(NULL, -1, -1, m_csBitmaps[0].dwWidth, m_csBitmaps[0].dwHeight,
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE);
		} // if
} // End of SizeToContent

DWORD CMCButton::OnDrawBackground(CDC* pDC, CRect* pRect)
{
	COLORREF	crColor{};

	if (m_bIsFlat == FALSE)
	{
		if (m_bIsFocused || m_bIsDefault)
		{
			CBrush br(RGB(0, 0, 0));
			pDC->FrameRect(pRect, &br);
			pRect->DeflateRect(1, 1);
		} // if
	} // if

	if (m_bMouseOnButton || m_bIsPressed)
		crColor = m_crColors[BTNST_COLOR_BK_IN];
	else
	{
		if (m_bIsFocused)
			crColor = m_crColors[BTNST_COLOR_BK_FOCUS];
		else
			crColor = m_crColors[BTNST_COLOR_BK_OUT];
	} // else

	CBrush		brBackground(crColor);

	pDC->FillRect(pRect, &brBackground);

	return BTNST_OK;
} // End of OnDrawBackground

BOOL CMCButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

void CMCButton::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);
	CancelHover();
}

void CMCButton::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CButton::OnActivate(nState, pWndOther, bMinimized);
	if (nState == WA_INACTIVE)	CancelHover();
}

void CMCButton::OnCancelMode()
{
	CButton::OnCancelMode();
	CancelHover();
}

HBRUSH CMCButton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CButton::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

DWORD CMCButton::OnDrawBorder(CDC* pDC, CRect* pRect)
{
	// Draw pressed button
	if (m_bIsPressed)
	{
		if (m_bIsFlat)
		{
			if (m_bDrawBorder)
				pDC->Draw3dRect(pRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
		}
		else
		{
			CBrush brBtnShadow(GetSysColor(COLOR_BTNSHADOW));
			pDC->FrameRect(pRect, &brBtnShadow);
		}
	}
	else // ...else draw non pressed button
	{
		CPen penBtnHiLight(PS_SOLID, 0, GetSysColor(COLOR_BTNHILIGHT)); // White
		CPen pen3DLight(PS_SOLID, 0, GetSysColor(COLOR_3DLIGHT));       // Light gray
		CPen penBtnShadow(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));   // Dark gray
		CPen pen3DDKShadow(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW)); // Black

		if (m_bIsFlat)
		{
			if (m_bMouseOnButton && m_bDrawBorder)
				pDC->Draw3dRect(pRect, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));
		}
		else
		{
			// Draw top-left borders
			// White line
			CPen* pOldPen = pDC->SelectObject(&penBtnHiLight);
			pDC->MoveTo(pRect->left, pRect->bottom - 1);
			pDC->LineTo(pRect->left, pRect->top);
			pDC->LineTo(pRect->right, pRect->top);
			// Light gray line
			pDC->SelectObject(pen3DLight);
			pDC->MoveTo(pRect->left + 1, pRect->bottom - 1);
			pDC->LineTo(pRect->left + 1, pRect->top + 1);
			pDC->LineTo(pRect->right, pRect->top + 1);
			// Draw bottom-right borders
			// Black line
			pDC->SelectObject(pen3DDKShadow);
			pDC->MoveTo(pRect->left, pRect->bottom - 1);
			pDC->LineTo(pRect->right - 1, pRect->bottom - 1);
			pDC->LineTo(pRect->right - 1, pRect->top - 1);
			// Dark gray line
			pDC->SelectObject(penBtnShadow);
			pDC->MoveTo(pRect->left + 1, pRect->bottom - 2);
			pDC->LineTo(pRect->right - 2, pRect->bottom - 2);
			pDC->LineTo(pRect->right - 2, pRect->top);
			//
			pDC->SelectObject(pOldPen);
		} // else
	} // else

	return BTNST_OK;
} // End of OnDrawBorder




void CMCButton::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);

	AdjustFontSize(this, m_Bfont, _T("Arial"));
	AdjustFontSize(this, m_sfont, _T("Arial"), FW_LIGHT);

	// 컨트롤에 적용
	SetFont(&m_sfont);
}

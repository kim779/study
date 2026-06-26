#include "pch.h"
#include "CHMenu.h"

int const CXGAP = 0;		// num pixels between button and text
int const CXTEXTMARGIN = 2;		// num pixels after hilite to start text
int const CXBUTTONMARGIN = 3;/*2*/	// num pixels wider button is than bitmap
int const CYBUTTONMARGIN = 3;/*2*/	// ditto for height

// DrawText flags
int const DT_MYSTANDARD = DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX;

#define BACK_COLOR			RGB(237, 243, 249)
#define LINE_COLOR			RGB( 89, 100, 114)
#define SELECTBAR_COLOR		RGB(169, 205, 246)
#define DISABLEBAR_COLOR	RGB(235, 241, 247)
#define SELBORDER_COLOR		RGB( 27, 117, 216)
#define CHECK_COLOR			RGB(237, 243, 249)
#define SELECTCHECK_COLOR	RGB(237, 243, 249)
#define ICONAREA_COLOR		RGB(213, 220, 229)
#define SEPLINE1_COLOR		RGB(213, 220, 229)
#define SEPLINE2_COLOR		RGB(255, 255, 255)

#define BORDER_COLOR	SELBORDER_COLOR

IMPLEMENT_DYNAMIC(CHMenu, CMenu)
CHMenu::CHMenu()
{
	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
	VERIFY(m_fontMenu.CreateFontIndirect(&info.lfMenuFont));

	m_clrBackGround = BACK_COLOR;
	m_clrSelectedBar = SELECTBAR_COLOR;
	m_clrSelectedText = ::GetSysColor(COLOR_MENUTEXT);
	m_clrText = ::GetSysColor(COLOR_MENUTEXT);
	m_clrDisabledText = ::GetSysColor(COLOR_GRAYTEXT);
	m_clrIconArea = ICONAREA_COLOR;
	m_clrBorder = BORDER_COLOR;

	//initialize sidebar colors
	m_clrSideBarStart = RGB(0, 0, 192);
	m_clrSideBarEnd = RGB(0, 0, 0);

	//the default sytle is office style
	m_Style = STYLE_XP;

	m_bBreak = FALSE;
	m_bBreakBar = FALSE;
	m_hBitmap = NULL;

	m_penBorder.CreatePen(PS_SOLID, 0, BORDER_COLOR);
}

CHMenu::~CHMenu()
{
	m_fontMenu.DeleteObject();
	m_penBorder.DeleteObject();
	//ref 우선주석
	//Clear();  
}

void CHMenu::MeasureItem(LPMEASUREITEMSTRUCT lpms)
{
	if (lpms->CtlType != ODT_MENU)	return;
	CHMenuItem* pItem = (CHMenuItem*)lpms->itemData;

	if (!pItem || !pItem->IsMyData())return;

	if (pItem->m_bSideBar)
	{
		lpms->itemWidth = pItem->m_nSize;
		lpms->itemHeight = 0;
	}
	else if (pItem->m_bSeparator)
	{
		lpms->itemHeight = 3;
		lpms->itemWidth = 0;
	}
	else
	{
		CWindowDC dc(NULL);	// screen DC--I won't actually draw on it
		CRect rcText(0, 0, 0, 0);
		CFont* pOldFont{};

		CFont	fontBold;
		LOGFONT	logFont;
		m_fontMenu.GetLogFont(&logFont);
		logFont.lfWeight = FW_BOLD;
		fontBold.CreateFontIndirect(&logFont);

		pOldFont = dc.SelectObject(&fontBold);
		//		pOldFont= dc.SelectObject(&m_fontMenu);
		dc.DrawText(pItem->m_strText, rcText, DT_MYSTANDARD | DT_CALCRECT);
		dc.SelectObject(pOldFont);

		// the height of the item should be the maximun of the text and the button
		lpms->itemHeight = max(rcText.Height(), pItem->m_nSize + (CYBUTTONMARGIN << 1));

		if (pItem->m_bButtonOnly)
		{	//for button only style, we set the item's width to be the same as its height
			lpms->itemWidth = lpms->itemHeight;
		}
		else
		{
			// width is width of text plus a bunch of stuff
			int cx = rcText.Width();	// text width 
			cx += CXTEXTMARGIN << 1;		// L/R margin for readability
			cx += CXGAP;					// space between button and menu text
			cx += (pItem->m_nSize + CYBUTTONMARGIN * 2) << 1;		// button width (L=button; R=empty margin)

			lpms->itemWidth = cx;		// done deal
		}
	}
	lpms->itemWidth -= GetSystemMetrics(SM_CXMENUCHECK) - 1;
}

void CHMenu::DrawItem(LPDRAWITEMSTRUCT lpds)
{
	ASSERT(lpds);
	if (lpds->CtlType != ODT_MENU)
		return; // not handled by me
	CHMenuItem* pItem = (CHMenuItem*)lpds->itemData;
	if (!pItem)
		return;

	ASSERT(lpds->itemAction != ODA_FOCUS);
	ASSERT(lpds->hDC);
	CDC dc;
	dc.Attach(lpds->hDC);

	//get the drawing area
	CRect rcItem = lpds->rcItem;

	if (pItem->m_bSideBar)
	{
		CRect rcClipBox;
		dc.GetClipBox(rcClipBox);
	
		//draw the side bar
		CRect rc = rcItem;
		rc.top = rcClipBox.top;
		rc.bottom = rcClipBox.bottom;
		//ref 주석 DrawSideBar
		//DrawSideBar(&dc, rc, pItem->m_hIcon, pItem->m_strText);
	}
	else if (pItem->m_bSeparator)
	{
		//draw background first
		DrawBackGround(&dc, rcItem, FALSE, FALSE);
		// draw the background
		CRect rc = rcItem;								// copy rect
		rc.top += rc.Height() >> 1;						// vertical center

		DrawSeparator(&dc, rc);

		// in XP mode, fill the icon area with the iconarea color
		if (m_Style == STYLE_XP)
		{
			const CRect rcArea(rcItem.TopLeft(),
				CSize(pItem->m_nSize + (CYBUTTONMARGIN << 1),
					pItem->m_nSize + (CYBUTTONMARGIN << 1)));
			DrawIconArea(&dc, rcArea, FALSE, FALSE, FALSE);
		}
	}
	else
	{
		const BOOL bDisabled = lpds->itemState & ODS_GRAYED;
		const BOOL bSelected = lpds->itemState & ODS_SELECTED;
		const BOOL bChecked = lpds->itemState & ODS_CHECKED;

		//draw the background first
		DrawBackGround(&dc, rcItem, bSelected, bDisabled);

		//Draw the icon area for XP style
		if (m_Style == STYLE_XP)
		{
			const CRect rcArea(rcItem.TopLeft(), CSize(rcItem.Height(), rcItem.Height()));
			DrawIconArea(&dc, rcArea, bSelected, bDisabled, bChecked);
		}

		//draw the button, not the icon
		CRect rcButton(rcItem.TopLeft(), CSize(rcItem.Height(), rcItem.Height()));
		if (pItem->m_bButtonOnly)
			rcButton = rcItem;
		if (pItem->m_hIcon || bChecked)
		{
			DrawButton(&dc, rcButton, bSelected, bDisabled, bChecked);
		}

		//draw the icon actually
		if (pItem->m_hIcon)
		{
			CRect	rcIcon = rcButton;
			rcIcon.DeflateRect(2, 2);
			DrawIcon(&dc, rcIcon, pItem->m_hIcon, bSelected, bDisabled, bChecked);
		}
		else if (bChecked)
		{
			//draw the check mark
			CRect	rcCheck = rcButton;
			rcCheck.DeflateRect(2, 2);
			DrawCheckMark(&dc, rcCheck, bSelected);
		}

		//draw text finally
		if (!pItem->m_bButtonOnly)
		{
			CRect rcText = rcItem;				 // start w/whole item
			rcText.left += rcButton.Width() + CXGAP + CXTEXTMARGIN; // left margin
			rcText.right -= pItem->m_nSize;				 // right margin
			DrawText(&dc, rcText, pItem->m_strText, bSelected, bDisabled, lpds->itemState & ODS_DEFAULT ? 1 : 0);
		}
	}
	dc.Detach();
}

void CHMenu::DrawBackGround(CDC* pDC, CRect rect, BOOL bSelected, BOOL bDisabled)
{
	if (m_hBitmap && (!bSelected || bDisabled))
	{
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &m_memDC,
			0, rect.top, SRCCOPY);
	}
	else if (bSelected)
	{
		FillRect(pDC, rect, bDisabled ? DISABLEBAR_COLOR : m_clrSelectedBar);
	}
	else
	{
		FillRect(pDC, rect, m_clrBackGround);
	}

	if (bSelected)
	{
		CGdiObject* pOldBrush = pDC->SelectStockObject(HOLLOW_BRUSH);
		CPen pn; pn.CreatePen(PS_SOLID, 1, m_clrBorder);
		CPen* pPen = pDC->SelectObject(&pn);

		pDC->Rectangle(rect);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pPen);
		pn.DeleteObject();
	}
}

void CHMenu::DrawButton(CDC* pDC, CRect rect, BOOL bSelected, BOOL bDisabled, BOOL bChecked)
{
	rect.DeflateRect(1, 1);
	if (bChecked)// && !bSelected)
	{
		if (bSelected)
			FillRect(pDC, rect, SELECTCHECK_COLOR);//GetSysColor(COLOR_3DHILIGHT));
		else	
			FillRect(pDC, rect, CHECK_COLOR);
	}

	if (!bDisabled && bChecked)
		pDC->Draw3dRect(rect, BORDER_COLOR, BORDER_COLOR);
}

void CHMenu::DrawIconArea(CDC* pDC, CRect rect, BOOL bSelected, BOOL bDisabled, BOOL bChecked)
{
	if (m_Style != STYLE_XP)
		return;

	if (!bSelected)
		FillRect(pDC, rect, m_clrIconArea);
}

void CHMenu::DrawIcon(CDC* pDC, CRect rect, HICON hIcon, BOOL bSelected, BOOL bDisabled, BOOL bChecked)
{
	if (bDisabled)
	{
		DrawEmbossed(pDC, hIcon, rect);
	}
	else
	{
		::DrawIconEx(pDC->m_hDC, rect.left, rect.top, hIcon,
			rect.Width(), rect.Height(), 0, NULL,
			DI_IMAGE);//DI_NORMAL);
	}
}

void CHMenu::DrawSideBar(CDC* pDC, CRect rect, HICON hIcon, CString strText)
{
	rect.right += 3;	//fill the gap produced by the menubreak

	HBITMAP	bmpBar = CreateGradientBMP(
		pDC->m_hDC, m_clrSideBarStart, m_clrSideBarEnd,
		rect.Width(), rect.Height(),
		0, 256);

	if (bmpBar)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(memDC.m_hDC, bmpBar);
		pDC->BitBlt(rect.left, rect.top,
			rect.Width(), rect.Height(),
			&memDC, 0, 0, SRCCOPY);
		::SelectObject(memDC, hOldBmp);
		::DeleteObject(bmpBar);
	}
	//Draw Sidebar text
	CFont	vertFont;
	vertFont.CreateFont(16, 0, 900, 900, FW_BOLD,
		0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, "Arial");
	CFont* pOldFont = pDC->SelectObject(&vertFont);
	const COLORREF oldColor = pDC->GetTextColor();
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(rect.left + 2, rect.bottom - 4, strText);
	pDC->SetTextColor(oldColor);
	pDC->SelectObject(pOldFont);
	vertFont.DeleteObject();
}

void CHMenu::DrawCheckMark(CDC* pDC, CRect rect, BOOL bSelected)
{
	const int nCheckDots = 8;
	CPoint pt1, pt2, pt3;	//3 point of the checkmark
	pt1.x = 0;	// 5/18 of the rect width
	pt1.y = 3;
	pt2.x = 2;
	pt2.y = 5;
	pt3.x = 7;
	pt3.y = 0;

	const int xOff = (rect.Width() - nCheckDots) / 2 + rect.left + 1;
	const int yOff = (rect.Height() - nCheckDots) / 2 + rect.top;
	pt1.Offset(xOff, yOff);
	pt2.Offset(xOff, yOff);
	pt3.Offset(xOff, yOff);

	CPen	pen(PS_SOLID, 0, RGB(0, 0, 0));
	CGdiObject* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(pt1);
	pDC->LineTo(pt2);
	pDC->LineTo(pt3);
	pt1.Offset(0, 1);
	pt2.Offset(0, 1);
	pt3.Offset(0, 1);
	pDC->MoveTo(pt1);
	pDC->LineTo(pt2);
	pDC->LineTo(pt3);

	pDC->SelectObject(pOldPen);
}

void CHMenu::DrawText(CDC* pDC, CRect rect, CString strText, BOOL bSelected, BOOL bDisabled, BOOL bBold)
{
	CFont* pOldFont{};
	CFont	fontBold;

	if (bBold)
	{
		LOGFONT	logFont;
		m_fontMenu.GetLogFont(&logFont);
		logFont.lfWeight = FW_BOLD;
		fontBold.CreateFontIndirect(&logFont);

		pOldFont = pDC->SelectObject(&fontBold);
	}
	else
	{
		pOldFont = pDC->SelectObject(&m_fontMenu);
	}

	pDC->SetBkMode(TRANSPARENT);
	if (bDisabled)
	{
		DrawMenuText(*pDC, rect, strText, m_clrDisabledText);
	}
	else
	{
		DrawMenuText(*pDC, rect, strText, bSelected ? m_clrSelectedText : m_clrText);
	}

	pDC->SelectObject(pOldFont);

	if (bBold)	
		fontBold.DeleteObject();
}

void CHMenu::DrawMenuText(CDC& dc, CRect rc, CString text,
	COLORREF color)
{
	CString left = text;
	CString right;
	const int iTabPos = left.Find('\t');

	if (iTabPos >= 0) {
		right = left.Right(left.GetLength() - iTabPos - 1);
		left = left.Left(iTabPos);
	}
	dc.SetTextColor(color);
	dc.DrawText(left, &rc, DT_MYSTANDARD);
	if (iTabPos > 0)
		dc.DrawText(right, &rc, DT_MYSTANDARD | DT_RIGHT);

}

void CHMenu::DrawSeparator(CDC* pDC, CRect rc)
{
	CPen pen1, pen2;
	pen1.CreatePen(PS_SOLID, 1, SEPLINE1_COLOR);
	pen2.CreatePen(PS_SOLID, 1, SEPLINE2_COLOR);

	CPen* oldPen = pDC->SelectObject(&pen1);
	pDC->MoveTo(rc.left, rc.top);
	pDC->LineTo(rc.right, rc.top);

	pDC->SelectObject(&pen2);
	pDC->MoveTo(rc.left, rc.top + 1);
	pDC->LineTo(rc.right, rc.top + 1);

	pDC->SelectObject(oldPen);
}

//draw embossed icon for the disabled item
const DWORD		MAGICROP = 0xb8074a;
const COLORREF CWHITE = RGB(255, 255, 255);

void CHMenu::DrawEmbossed(CDC* pDC, HICON hIcon, CRect rect, BOOL bColor, BOOL bShadow)
{
	CDC	memdc;
	memdc.CreateCompatibleDC(pDC);
	const int cx = rect.Width();
	const int cy = rect.Height();

	// create mono or color bitmap
	CBitmap bm;
	if (bColor)
		bm.CreateCompatibleBitmap(pDC, cx, cy);
	else
		bm.CreateBitmap(cx, cy, 1, 1, NULL);

	// draw image into memory DC--fill BG white first
	CBitmap* pOldBitmap = memdc.SelectObject(&bm);
	//FillRect(&memdc, CRect(0, 0, cx, cy), m_clrBackGround);
	memdc.PatBlt(0, 0, cx, cy, WHITENESS);
	::DrawIconEx(memdc.m_hDC, 0, 0, hIcon, cx, cy, 1, NULL, DI_NORMAL);

	// This seems to be required. Why, I don't know. ???
	const COLORREF colorOldBG = pDC->SetBkColor(CWHITE);

	// Draw using hilite offset by (1,1), then shadow
	CBrush brShadow(GetSysColor(COLOR_3DSHADOW));
	CBrush brHilite(GetSysColor(COLOR_3DHIGHLIGHT));
	CBrush* pOldBrush = pDC->SelectObject(bShadow ? &brShadow : &brHilite);
	pDC->BitBlt(rect.left + 1, rect.top + 1, cx, cy, &memdc, 0, 0, MAGICROP);
	pDC->SelectObject(&brShadow);
	pDC->BitBlt(rect.left, rect.top, cx, cy, &memdc, 0, 0, MAGICROP);
	pDC->SelectObject(pOldBrush);
	pDC->SetBkColor(colorOldBG);				 // restore
	memdc.SelectObject(pOldBitmap);		 // ...
	bm.DeleteObject();
	brShadow.DeleteObject();
	brHilite.DeleteObject();
}

void CHMenu::FillRect(CDC* pDC, const CRect& rc, COLORREF color)
{
	CBrush brush(color);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
}

HBITMAP CHMenu::CreateGradientBMP(HDC hDC, COLORREF cl1, COLORREF cl2, int nWidth, int nHeight, int nDir, int nNumColors)
{
	if (nNumColors > 256)
		nNumColors = 256;

	COLORREF		PalVal[256];
	memset(PalVal, 0, sizeof(COLORREF) * 256);

	int nIndex{};
	BYTE peRed = 0, peGreen = 0, peBlue = 0;

	const int r1 = GetRValue(cl1);
	const int r2 = GetRValue(cl2);
	const int g1 = GetGValue(cl1);
	const int g2 = GetGValue(cl2);
	const int b1 = GetBValue(cl1);
	const int b2 = GetBValue(cl2);

	for (nIndex = 0; nIndex < nNumColors; nIndex++)
	{
		peRed = (BYTE)(r1 + MulDiv((r2 - r1), nIndex, nNumColors - 1));
		peGreen = (BYTE)(g1 + MulDiv((g2 - g1), nIndex, nNumColors - 1));
		peBlue = (BYTE)(b1 + MulDiv((b2 - b1), nIndex, nNumColors - 1));

		PalVal[nIndex] = (peRed << 16) | (peGreen << 8) | (peBlue);
	}

	int x{}, y{}, w{}, h{};
	w = nWidth;
	h = nHeight;

	LPDWORD			pGradBits{};
	BITMAPINFO		GradBitInfo{};

	pGradBits = (DWORD*)malloc(w * h * sizeof(DWORD));
	memset(&GradBitInfo, 0, sizeof(BITMAPINFO));

	GradBitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	GradBitInfo.bmiHeader.biWidth = w;
	GradBitInfo.bmiHeader.biHeight = h;
	GradBitInfo.bmiHeader.biPlanes = 1;
	GradBitInfo.bmiHeader.biBitCount = 32;
	GradBitInfo.bmiHeader.biCompression = BI_RGB;

	if (nDir == 0)
	{
		for (y = 0; y < h; y++)
		{
			for (x = 0; x < w; x++)
			{
				*(pGradBits + (y * w) + x) = PalVal[MulDiv(nNumColors, y, h)];
			}
		}
	}
	else if (nDir == 1)
	{
		for (y = 0; y < h; y++)
		{
			int l{}, r{};
			l = MulDiv((nNumColors / 2), y, h);
			r = l + (nNumColors / 2) - 1;
			for (x = 0; x < w; x++)
			{
				*(pGradBits + (y * w) + x) = PalVal[l + MulDiv((r - l), x, w)];
			}
		}
	}
	else if (nDir == 2)
	{
		for (x = 0; x < w; x++)
		{
			for (y = 0; y < h; y++)
			{
				*(pGradBits + (y * w) + x) = PalVal[MulDiv(nNumColors, x, w)];
			}
		}
	}
	else if (nDir == 3)
	{
		for (y = 0; y < h; y++)
		{
			int l{}, r{};
			r = MulDiv((nNumColors / 2), y, h);
			l = r + (nNumColors / 2) - 1;
			for (x = 0; x < w; x++)
			{
				*(pGradBits + (y * w) + x) = PalVal[l + MulDiv((r - l), x, w)];
			}
		}
	}

	HBITMAP hBmp = CreateDIBitmap(hDC, &GradBitInfo.bmiHeader, CBM_INIT,
		pGradBits, &GradBitInfo, DIB_RGB_COLORS);

	free(pGradBits);

	return hBmp;
}

LRESULT CHMenu::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu)
{
	UINT iCurrentItem = (UINT)-1; // guaranteed higher than any command ID
	CUIntArray arItemsMatched;		// items that match the character typed

	const UINT nItem = pMenu->GetMenuItemCount();
	for (UINT i = 0; i < nItem; i++)
	{
		MENUITEMINFO	info;
		memset(&info, 0, sizeof(info));
		info.cbSize = sizeof(info);
		info.fMask = MIIM_DATA | MIIM_TYPE | MIIM_STATE;
		::GetMenuItemInfo(*pMenu, i, TRUE, &info);

		CHMenuItem* pData = (CHMenuItem*)info.dwItemData;
		if ((info.fType & MFT_OWNERDRAW) && pData && pData->IsMyData())
		{
			CString	text = pData->m_strText;
			const int iAmpersand = text.Find('&');
			if (iAmpersand >= 0 && toupper(nChar) == toupper(text[iAmpersand + 1]))
				arItemsMatched.Add(i);
		}
		if (info.fState & MFS_HILITE)
			iCurrentItem = i; // note index of current item
	}

	const UINT nFound = arItemsMatched.GetSize();
	if (nFound == 0)
		return 0;

	else if (nFound == 1)
		return MAKELONG(arItemsMatched[0], MNC_EXECUTE);

	// more than one found--return 1st one past current selected item;
	UINT iSelect = 0;
	for (int i = 0; i < (int)nFound; i++) {
		if (arItemsMatched[i] > iCurrentItem) {
			iSelect = i;
			break;
		}
	}
	return MAKELONG(arItemsMatched[iSelect], MNC_SELECT);
}

CHMenu* CHMenu::FindSubMenuFromID(DWORD dwID)
{
	CHMenu* pSubMenu{};
	CHMenu* pResult{};

	for (int i = 0; i < GetMenuItemCount(); i++)
	{
		if (GetMenuItemID(i) == dwID)
			return this;
	}

	for (int i = 0; i < GetMenuItemCount(); i++)
	{
		pSubMenu = (CHMenu*)GetSubMenu(i);
		if (pSubMenu)
		{
			pResult = pSubMenu->FindSubMenuFromID(dwID);
			if (pResult)
				return pResult;
		}
	}

	return nullptr;
}

BOOL CHMenu::AddSideBar(CHMenuSideBar* pItem)
{
	ASSERT(pItem);

	m_bBreak = TRUE;
	m_bBreakBar = FALSE;

	return AppendMenu(MF_OWNERDRAW | MF_SEPARATOR, pItem->m_dwID, (LPCTSTR)pItem);
}

BOOL CHMenu::AppendODMenu(UINT nFlags, CHMenuItem* pItem, ACCEL* pAccel)
{
	ASSERT(pItem);

	nFlags |= MF_OWNERDRAW;
	if (m_bBreak) 	nFlags |= MF_MENUBREAK;
	if (m_bBreakBar)nFlags |= MF_MENUBARBREAK;
	m_bBreak = m_bBreakBar = FALSE;

	return AppendMenu(nFlags, pItem->m_dwID, (LPCTSTR)pItem);
}

BOOL CHMenu::InsertMenuX(UINT nPosition, UINT nFlags, UINT nIDNewItem, LPCTSTR lpszNewItem, HICON hIcon)
{
	if (nFlags & MF_SEPARATOR)	return AppendSeparator();
	const CHMenuItem* item = new CHMenuText(nIDNewItem, lpszNewItem, hIcon);
	ASSERT(item);

	nFlags |= MF_OWNERDRAW;
	/* ===>> ???????????????????? */
	if (m_bBreak) 	nFlags |= MF_MENUBREAK;
	if (m_bBreakBar)nFlags |= MF_MENUBARBREAK;
	m_bBreak = m_bBreakBar = FALSE;
	/* ???????????????????? <<=== */
	return InsertMenu(nPosition, nFlags, item->m_dwID, (LPCTSTR)item);
}

BOOL CHMenu::AppendMenuX(UINT nFlags, UINT nIDNewItem, LPCTSTR lpszNewItem, HICON hIcon)
{
	if (nFlags & MF_SEPARATOR)	return AppendSeparator();
	const CHMenuItem* item = new CHMenuText(nIDNewItem, lpszNewItem, hIcon);
	ASSERT(item);

	nFlags |= MF_OWNERDRAW;
	/* ===>> ???????????????????? */
	if (m_bBreak) 	nFlags |= MF_MENUBREAK;
	if (m_bBreakBar)nFlags |= MF_MENUBARBREAK;
	m_bBreak = m_bBreakBar = FALSE;
	/* ???????????????????? <<=== */
	return AppendMenu(nFlags, item->m_dwID, (LPCTSTR)item);
}

BOOL CHMenu::AppendSeparator(void)
{
	m_bBreak = m_bBreakBar = FALSE;

	const  CHMenuSeparator* pItem = new  CHMenuSeparator;
	return AppendMenu(MF_OWNERDRAW | MF_SEPARATOR, 0, (LPCTSTR)pItem);
}

BOOL CHMenu::AppendODPopup(UINT nFlags, CHMenu* pPopup, CHMenuItem* pItem)
{
	ASSERT(pPopup);
	ASSERT(pItem);

	nFlags |= MF_OWNERDRAW;
	nFlags |= MF_POPUP;
	if (m_bBreak)
		nFlags |= MF_MENUBREAK;
	if (m_bBreakBar)
		nFlags |= MF_MENUBARBREAK;
	m_bBreak = m_bBreakBar = FALSE;

	return AppendMenu(nFlags, (UINT)pPopup->m_hMenu, (LPCTSTR)pItem);
}

void CHMenu::Break(void)
{
	m_bBreak = TRUE;
}

void CHMenu::BreakBar(void)
{
	m_bBreakBar = TRUE;
}

void CHMenu::SetBackBitmap(HBITMAP hBmp)
{
	if (hBmp == NULL && m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
		m_memDC.DeleteDC();
		return;
	}
	m_hBitmap = hBmp;
	if (!m_memDC.m_hDC)
	{
		CWindowDC	dc(NULL);
		m_memDC.CreateCompatibleDC(&dc);
	}

	ASSERT(m_memDC.m_hDC);

	::SelectObject(m_memDC.m_hDC, m_hBitmap);
}

BOOL CHMenu::TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect)
{
	if (!m_hMenu) return FALSE;

	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	HWND hWndOld = pThreadState->m_hTrackingWindow;
	HMENU hMenuOld = pThreadState->m_hTrackingMenu;
	pThreadState->m_hTrackingWindow = pWnd->GetSafeHwnd();
	pThreadState->m_hTrackingMenu = m_hMenu;
	pWnd->SetForegroundWindow();
	const BOOL bOK = ::TrackPopupMenu(m_hMenu, nFlags, x, y, 0, pThreadState->m_hTrackingWindow, lpRect);
	pWnd->PostMessage(WM_NULL);
	pThreadState->m_hTrackingWindow = hWndOld;
	pThreadState->m_hTrackingMenu = hMenuOld;
	return bOK;
}
// scListBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "scListBox.h"


#define	MASK_DWDATA		0x01	// dwItemData is valid
#define	MASK_LPDATA		0x02	// pData is valid
#define	MASK_NIMAGE		0x04	// nImage is valid
#define	MASK_DWFLAGS		0x08	// dwFlags is valid
#define MASK_ALL		0xff	// All fields are valid
#define TEST_BIT0		0x00000001
#define	CX_BORDER		3
#define	CY_BORDER		0

// CScListBox

IMPLEMENT_DYNAMIC(CScListBox, CListBox)

CScListBox::CScListBox()
{
	LOGFONT	lf;

	m_pImageList = NULL;
	::ZeroMemory(&m_szImage, sizeof(m_szImage));

	setRowSelect(ST_FULLTEXTSELECT, FALSE);
	setFont(lf, 9);
	m_font.CreatePointFontIndirect(&lf);
}

CScListBox::~CScListBox()
{
}


BEGIN_MESSAGE_MAP(CScListBox, CListBox)
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT_EX(LBN_DBLCLK, OnReflectedDblclk)
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CScListBox 메시지 처리기입니다.

void CScListBox::OnDestroy()
{
	freeResources();
	CListBox::OnDestroy();
}

BOOL CScListBox::OnReflectedDblclk()
{
	UINT	nIndex = LB_ERR;
	BOOL	bOutside = FALSE;
	DWORD	dwPos = ::GetMessagePos();
	CPoint	Point(((int)(short)LOWORD(dwPos)), ((int)(short)HIWORD(dwPos)));

	ScreenToClient(&Point);
	nIndex = ItemFromPoint(Point, bOutside);
	if (!bOutside)
		return !isItemEnabled(nIndex);
	return FALSE;
}

int CScListBox::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return MA_NOACTIVATE;	
//	return CListBox::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CScListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CListBox::OnLButtonDown(nFlags, point);
}

void CScListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	GetParent()->PostMessage(WM_USER, VK_RETURN, 0);
}

void CScListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	int	cy = 0;
	for (int ii = 0; ii < GetCount(); ii++)
	{
		cy += GetItemHeight(ii);
		if (cy - GetItemHeight(ii) <= point.y && cy > point.y)
		{
			SetCurSel(ii + GetTopIndex());
			break;
		}
	}
	//CListBox::OnLButtonUp(nFlags, point);
}

int CScListBox::AddString(LPCTSTR lpszItem, int nImage)
{
	int	nIndex;

	nIndex = CListBox::AddString(lpszItem);
	if (nIndex != LB_ERR && nIndex != LB_ERRSPACE)
		replaceItemData(nIndex, 0, NULL, nImage, 0, MASK_ALL);

	return nIndex;
}

void CScListBox::ResetContent()
{
	freeResources();
	CListBox::ResetContent();
}

void CScListBox::SetImageList(CImageList* pImageList)
{
	m_pImageList = pImageList;
	if (m_pImageList)
		ImageList_GetIconSize(*m_pImageList, (LPINT)&m_szImage.cx, (LPINT)&m_szImage.cy);
	else
		ZeroMemory(&m_szImage, sizeof(m_szImage));

	Invalidate();
}

void CScListBox::DrawItem(LPDRAWITEMSTRUCT lpDS)
{
	CDC*	pDC = CDC::FromHandle(lpDS->hDC);
	BOOL	bIsSelected, bIsFocused, bIsDisabled;
	COLORREF crNormal = GetSysColor(COLOR_WINDOW);
	COLORREF crSelected = GetSysColor(COLOR_HIGHLIGHT);
	COLORREF crText = GetSysColor(COLOR_WINDOWTEXT);
	COLORREF crColor = RGB(0, 0, 0);

	CString	sText;
	STRUCT_LBDATA*	lpLBData = NULL;
	CFont* pOldFont = pDC->SelectObject(&m_font);

	lpLBData = (STRUCT_LBDATA*)CListBox::GetItemDataPtr(lpDS->itemID);
	if (lpLBData == NULL || lpLBData == (LPVOID)-1L)
		return;

	bIsSelected = (lpDS->itemState & ODS_SELECTED);
	bIsFocused  = (lpDS->itemState & ODS_FOCUS);
	bIsDisabled = ((lpDS->itemState & ODS_DISABLED) || ((lpLBData->dwFlags & TEST_BIT0) == TEST_BIT0));

	CRect	rcItem = lpDS->rcItem;
	CRect	rcIcon = lpDS->rcItem;
	CRect	rcText = lpDS->rcItem;
	CRect	rcCenteredText = lpDS->rcItem;

	pDC->SetBkMode(TRANSPARENT);

	// Calculate rcIcon
	if (m_pImageList)
	{
		rcIcon.right = rcIcon.left + m_szImage.cx + CX_BORDER*2;
		rcIcon.bottom = rcIcon.top + m_szImage.cy + CY_BORDER*2;
	}
	else
		rcIcon.SetRect(0, 0, 0, 0);

	// Calculate rcText
	rcText.left = rcIcon.right;

	// Calculate rcCenteredText
	// Get list box item text
	CListBox::GetText(lpDS->itemID, sText);
	rcCenteredText = rcText;
	pDC->DrawText(sText, rcCenteredText, DT_WORDBREAK | DT_EXPANDTABS| DT_CALCRECT | lpLBData->nFormat);
	rcCenteredText.OffsetRect(0, (rcText.Height() - rcCenteredText.Height())/2);

	// Draw rcIcon background
	if (m_pImageList)
	{
		if (bIsSelected && (m_byRowSelect == ST_FULLROWSELECT) && !bIsDisabled)
			crColor = crSelected;
		else
			crColor = crNormal;
		OnDrawIconBackground(lpDS->itemID, pDC, &rcItem, &rcIcon, bIsDisabled, bIsSelected, crColor);
	}

	// Draw rcText/rcCenteredText background
	if (bIsDisabled)
	{
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		crColor = crNormal;
	}
	else
	{
		if (bIsSelected)
		{
			pDC->SetTextColor(0x00FFFFFF & ~crText);
			crColor = crSelected;
		}
		else
		{
			pDC->SetTextColor(crText);
			crColor = crNormal;
		}
	}

	if (m_byRowSelect == ST_TEXTSELECT)
		OnDrawTextBackground(lpDS->itemID, pDC, &rcItem, &rcCenteredText, bIsDisabled, bIsSelected, crColor);
	else
		OnDrawTextBackground(lpDS->itemID, pDC, &rcItem, &rcText, bIsDisabled, bIsSelected, crColor);

	// Draw the icon (if any)
	if (m_pImageList)
		OnDrawIcon(lpDS->itemID, pDC, &rcItem, &rcIcon, lpLBData->nImage, bIsDisabled, bIsSelected);

	// Draw text
	pDC->DrawText(sText, rcCenteredText, DT_WORDBREAK | DT_EXPANDTABS | lpLBData->nFormat);

	// Draw focus rectangle
	if (bIsFocused && !bIsDisabled)
	{
		switch (m_byRowSelect)
		{
		case ST_FULLROWSELECT:
			pDC->DrawFocusRect(&rcItem);
			break;
		case ST_FULLTEXTSELECT:
			pDC->DrawFocusRect(&rcText);
			break;
		case ST_TEXTSELECT:
		default:
			pDC->DrawFocusRect(&rcCenteredText);
			break;
		}
	}
	pDC->SelectObject(pOldFont);
}

void CScListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMS)
{
	ASSERT(lpMS->CtlType == ODT_LISTBOX);

	CDC*	pDC = GetDC();
	int	nHeight;
	CString	sText;
	CRect	csRect(0, 0, lpMS->itemWidth, lpMS->itemHeight);

	CListBox::GetText(lpMS->itemID, sText);

	nHeight = pDC->DrawText(sText, csRect, DT_WORDBREAK | DT_EXPANDTABS | DT_CALCRECT);

	if (m_pImageList)
		lpMS->itemHeight = max(nHeight, m_szImage.cy + CY_BORDER*2);
	else
		lpMS->itemHeight  = nHeight;

	lpMS->itemHeight += CY_BORDER*2;
}

DWORD CScListBox::OnDrawTextBackground(int nIndex, CDC* pDC, CRect* prcItem, CRect* prcText, BOOL bIsDisabled, BOOL bIsSelected, COLORREF crSuggestedColor)
{
	pDC->SetBkColor(crSuggestedColor);
	pDC->FillSolidRect(prcText, crSuggestedColor);

	return 0;
}

DWORD CScListBox::OnDrawIconBackground(int nIndex, CDC* pDC, CRect* prcItem, CRect* prcIcon, BOOL bIsDisabled, BOOL bIsSelected, COLORREF crSuggestedColor)
{
	pDC->SetBkColor(crSuggestedColor);
	pDC->FillSolidRect(prcIcon->left, prcIcon->top, prcIcon->Width(), prcItem->Height(), crSuggestedColor);

	return 0;
}

DWORD CScListBox::OnDrawIcon(int nIndex, CDC* pDC, CRect* prcItem, CRect* prcIcon, int nImage, BOOL bIsDisabled, BOOL bIsSelected)
{
	HICON	hIcon;

	hIcon = m_pImageList->ExtractIcon(nImage);
	if (hIcon)
	{
		CPoint	Point(prcIcon->left + CX_BORDER, prcIcon->top + CY_BORDER);
		CSize	Size(m_szImage);

		pDC->DrawState(Point, Size, hIcon, (bIsDisabled ? DSS_DISABLED : DSS_NORMAL), (CBrush*)NULL);
		::DestroyIcon(hIcon);
	}
	return 0;
}

void CScListBox::setFont(LOGFONT& lf, int fsize, CString fname, BOOL bBold, BOOL bItalic)
{
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight         = fsize * 10; 
	lf.lfCharSet        = DEFAULT_CHARSET;
	lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	lf.lfWeight         = (bBold?FW_BLACK:FW_NORMAL);
	lf.lfItalic         = bItalic;
}

void CScListBox::enableItem(int nIndex, BOOL bEnable, BOOL bRepaint)
{
	STRUCT_LBDATA*	lpLBData = NULL;

	lpLBData = (STRUCT_LBDATA*)CListBox::GetItemDataPtr(nIndex);
	if (lpLBData != NULL && lpLBData != (LPVOID)-1L)
	{
		if (bEnable)
			replaceItemData(nIndex, 0, NULL, 0, (lpLBData->dwFlags & ~TEST_BIT0), MASK_DWFLAGS);
		else
			replaceItemData(nIndex, 0, NULL, 0, (lpLBData->dwFlags | TEST_BIT0), MASK_DWFLAGS);

		if (bRepaint)
			Invalidate();
	}
}

BOOL CScListBox::isItemEnabled(int nIndex)
{
	STRUCT_LBDATA*	lpLBData = NULL;

	lpLBData = (STRUCT_LBDATA*)CListBox::GetItemDataPtr(nIndex);
	if (lpLBData != NULL && lpLBData != (LPVOID)-1L)
		return !((lpLBData->dwFlags & TEST_BIT0) == TEST_BIT0);
	return TRUE;
}

void  CScListBox::setRowSelect(BYTE byRowSelect, BOOL bRepaint)
{
	switch (byRowSelect)
	{
	case ST_FULLROWSELECT:
	case ST_FULLTEXTSELECT:
	case ST_TEXTSELECT:
		m_byRowSelect = byRowSelect;
		if (bRepaint)
			Invalidate();
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void  CScListBox::freeResources()
{
	int	nCount;

	nCount = GetCount();
	if (nCount != LB_ERR)
	{
		for ( ;nCount > 0; nCount--)
			deleteItemData(nCount-1);
	}
}

int  CScListBox::replaceItemData(int nIndex, DWORD dwItemData, LPVOID pData, int nImage, DWORD dwFlags, BYTE byMask)
{
	STRUCT_LBDATA*	lpLBData = NULL;
	int	nRetValue = LB_ERR;

	lpLBData = (STRUCT_LBDATA*)CListBox::GetItemDataPtr(nIndex);
	if (lpLBData == NULL)
	{
		lpLBData = new STRUCT_LBDATA;
		if (lpLBData)	::ZeroMemory(lpLBData, sizeof(STRUCT_LBDATA));
	}

	if (lpLBData)
	{
		if ((byMask & MASK_DWDATA) == MASK_DWDATA)
			lpLBData->dwItemData = dwItemData;
		if ((byMask & MASK_LPDATA) == MASK_LPDATA)
			lpLBData->pData = pData;
		if ((byMask & MASK_NIMAGE) == MASK_NIMAGE)
			lpLBData->nImage = nImage;
		if ((byMask & MASK_DWFLAGS) == MASK_DWFLAGS)
			lpLBData->dwFlags = dwFlags;

		nRetValue = CListBox::SetItemDataPtr(nIndex, lpLBData);
	}
	return nRetValue;
}

void  CScListBox::deleteItemData(int nIndex)
{
	STRUCT_LBDATA*	lpLBData = NULL;

	lpLBData = (STRUCT_LBDATA*)CListBox::GetItemDataPtr(nIndex);
	if (lpLBData != (LPVOID)-1L)
		delete lpLBData;

	CListBox::SetItemDataPtr(nIndex, NULL);
}

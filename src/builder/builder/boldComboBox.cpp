// boldComboBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisBuilder.h"
#include "boldComboBox.h"


// CBoldComboBox

IMPLEMENT_DYNAMIC(CBoldComboBox, CComboBox)

CBoldComboBox::CBoldComboBox()
{
	m_pImageList = NULL;
	m_bflag = false;
}

CBoldComboBox::~CBoldComboBox()
{
	if (m_pImageList)
		delete m_pImageList;
}


BEGIN_MESSAGE_MAP(CBoldComboBox, CComboBox)
END_MESSAGE_MAP()

// CBoldComboBox 메시지 처리기입니다.


void CBoldComboBox::DrawItem(LPDRAWITEMSTRUCT lpDS)
{
	if (GetCount() == 0)
		return;

	if ((int)lpDS->itemID < 0 || GetCount() <= (int)lpDS->itemID)
		return;

	CString str;
	CDC	dc;
	int	iImage = -1;
	BOOL	bold = FALSE;
	ITEMDATA iData;
	
	dc.Attach(lpDS->hDC);
	GetLBText(lpDS->itemID, str);
	
	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();
	
	// If this item is selected, set the background color 
	// and the text color to appropriate values. Erase
	// the rect by filling it with the background color.
	if ((lpDS->itemAction | ODA_SELECT) && (lpDS->itemState  & ODS_SELECTED))
	{
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDS->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
	}
	else
	{
		if (m_bflag)
			dc.SetTextColor(GetItemData(lpDS->itemID));
		dc.FillSolidRect(&lpDS->rcItem, crOldBkColor);
	}
	
	CRect rect(lpDS->rcItem);
	rect.DeflateRect(1,0);
	
	if (m_items.Lookup(lpDS->itemID, iData))
	{
		iImage = iData.image;
		bold = iData.bold;
	}
	
	if (m_pImageList && iImage >= 0)
	{
		DrawIconEx(dc.GetSafeHdc(), rect.left, rect.top, m_pImageList->ExtractIcon(iImage), 0, 0, 0, NULL, DI_NORMAL);
		
		IMAGEINFO sImageInfo;
		m_pImageList->GetImageInfo(0, &sImageInfo);
		rect.left += sImageInfo.rcImage.right;
	}
	
	
	CFont	boldFont,*oldFont;
	LOGFONT lf;
	CFont*	curFont = dc.GetCurrentFont();
	curFont->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	boldFont.CreateFontIndirect(&lf);
	if (bold)
		oldFont = dc.SelectObject(&boldFont);
	
	// Draw the text.
	dc.DrawText(str, &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	
	if (bold)
		dc.SelectObject(oldFont);
	boldFont.DeleteObject();
	// Reset the background color and the text color back to their
	// original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
	dc.Detach();
}


void CBoldComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMS)
{

}

bool CBoldComboBox::IsItemBold(int iItem)
{
	ITEMDATA iData;

	if (m_items.Lookup(GetItemData(iItem), iData))
		return (iData.bold ? true : false);
	return false;
}

void CBoldComboBox::ResetItemInfo()
{
	m_items.RemoveAll();
}

void CBoldComboBox::SetItemImage(int iItem, int iImage)
{
	ITEMDATA iData;

	if (m_items.Lookup(iItem, iData))
		iData.image = iImage - 10;
	else
	{
		iData.bold = FALSE;
		iData.image = iImage - 10;
	}
	m_items.SetAt(iItem, iData);
	Invalidate();
}

void CBoldComboBox::SetItemImage2(int iItem, int iImage)
{
	ITEMDATA iData;

	if (m_items.Lookup(iItem, iData))
		iData.image = iImage;
	else
	{
		iData.bold = FALSE;
		iData.image = iImage;
	}
	m_items.SetAt(iItem, iData);
	Invalidate();
}

void CBoldComboBox::SetItemBold(int iItem,bool bold)
{
	ITEMDATA iData;

	if (m_items.Lookup(iItem, iData))
		iData.bold = bold;
	else
	{
		iData.bold = bold;
		iData.image = NULL;
	}
	m_items.SetAt(iItem, iData);
	Invalidate();
}

int CBoldComboBox::GetItemImage(int iItem)
{
	ITEMDATA iData;
	if (m_items.Lookup(iItem, iData))
	{
		return iData.image;
	}
	return -1;
}
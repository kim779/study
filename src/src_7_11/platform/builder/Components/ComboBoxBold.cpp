// ComboBoxBold.cpp : implementation file
//

#include "stdafx.h"
#include "ComboBoxBold.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboBoxBold

CComboBoxBold::CComboBoxBold()
{
	m_pImageList = NULL;
}

CComboBoxBold::~CComboBoxBold()
{
	if (m_pImageList)
		delete m_pImageList;
}


BEGIN_MESSAGE_MAP(CComboBoxBold, CComboBox)
	//{{AFX_MSG_MAP(CComboBoxBold)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboBoxBold message handlers
void CComboBoxBold::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (GetCount() == 0)
		return;
	if ((int)lpDrawItemStruct->itemID < 0 || GetCount() <= (int)lpDrawItemStruct->itemID)
		return;
	CString str;
	GetLBText(lpDrawItemStruct->itemID, str);
	CDC dc;
	
	dc.Attach(lpDrawItemStruct->hDC);
	
	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();
	
	// If this item is selected, set the background color 
	// and the text color to appropriate values. Erase
	// the rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState  & ODS_SELECTED))
	{
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
	}
	else
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);
	
	CRect rect(lpDrawItemStruct->rcItem);
	rect.DeflateRect(1,0);
	
	int iImage = -1;
	BOOL bold = FALSE;
	ITEMDATA iData;
	if (myMap.Lookup(lpDrawItemStruct->itemData, iData))
	{
		iImage = iData.image;
		bold = iData.bold;
	}
	
	if (m_pImageList && iImage >= 0)
	{
		DrawIconEx(dc.GetSafeHdc(), rect.left, rect.top, 
			m_pImageList->ExtractIcon(iImage), 0, 0, 0, NULL, DI_NORMAL);
		
		IMAGEINFO sImageInfo;
		m_pImageList->GetImageInfo(0, &sImageInfo);
		rect.left += sImageInfo.rcImage.right;
	}
	
	
	CFont *curFont = dc.GetCurrentFont();
	CFont boldFont,*oldFont;
	LOGFONT lf;
	curFont->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	boldFont.CreateFontIndirect(&lf);
	if (bold)
		oldFont = dc.SelectObject(&boldFont);
	
	// Draw the text.
	dc.DrawText(
		str,
		-1,
		&rect,
		DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	
	if (bold)
		dc.SelectObject(oldFont);
	boldFont.DeleteObject();
	// Reset the background color and the text color back to their
	// original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
	
	dc.Detach();
	
}

void CComboBoxBold::SetItemBold(int iItem, bool bold)
{
	ITEMDATA iData;
	if (myMap.Lookup(GetItemData(iItem), iData))
		iData.bold = bold;
	else
	{
		iData.bold = bold;
		iData.image = NULL;
	}
	myMap.SetAt(GetItemData(iItem), iData);
	Invalidate();
}
void CComboBoxBold::SetItemImage(int iItem, int iImage)
{
	ITEMDATA iData;
	if (myMap.Lookup(GetItemData(iItem), iData))
		iData.image = iImage;
	else
	{
		iData.bold = FALSE;
		iData.image = iImage;
	}
	myMap.SetAt(GetItemData(iItem), iData);
	Invalidate();
}

void CComboBoxBold::resetItemInfo()
{
	myMap.RemoveAll();
}

bool CComboBoxBold::isItemBold(int iItem)
{
	ITEMDATA iData;
	if (myMap.Lookup(GetItemData(iItem), iData))
		return (iData.bold?true:false);
	return false;
}

void CComboBoxBold::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	
}

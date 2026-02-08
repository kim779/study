// BitmapItemComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "BitmapItemComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitmapItemComboBox
CBitmapItemComboBox::CBitmapItemComboBox()
{
}

CBitmapItemComboBox::~CBitmapItemComboBox()
{
}


BEGIN_MESSAGE_MAP(CBitmapItemComboBox, CComboBox)
	//{{AFX_MSG_MAP(CBitmapItemComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapItemComboBox message handlers

#define LINE_ITEM_HEIGHT   15

void CBitmapItemComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
	{
		if ((long int)lpDrawItemStruct->itemData == -1) return;
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap* pOld = memDC.SelectObject(&m_bitmap);

		CRect rect;
		rect.CopyRect(&lpDrawItemStruct->rcItem);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), LINE_ITEM_HEIGHT,
			&memDC, 0, LINE_ITEM_HEIGHT*lpDrawItemStruct->itemID, SRCCOPY);
		memDC.SelectObject(pOld);
	}

	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		// item has been selected - hilite frame
		CBrush br(RGB(192,0,192));
		pDC->FrameRect(&lpDrawItemStruct->rcItem, &br);
	}

	if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove frame
		CBrush br(RGB(255,255,255));
		pDC->FrameRect(&lpDrawItemStruct->rcItem, &br);
	}
}

void CBitmapItemComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = LINE_ITEM_HEIGHT;	
}
// PopupMenu.cpp : implementation file
//

#include "stdafx.h"
#include "PopupMenu.h"
//#include "../CommonLib/grpdib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopUpMenu

CPopUpMenu::CPopUpMenu()
{
	m_bBitMap = FALSE;
	m_CoolBmp.RemoveAll();
	m_HotBmp.RemoveAll();
}

CPopUpMenu::~CPopUpMenu()
{
}

/////////////////////////////////////////////////////////////////////////////
// CPopUpMenu message handlers
void CPopUpMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	// all items are of fixed size
	lpMIS->itemWidth	= 18;
	lpMIS->itemHeight	= 26;

	if (m_bBitMap)	{
		lpMIS->itemWidth	= 16;
		lpMIS->itemHeight	= 20;
	}
}

void CPopUpMenu::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	COLORREF cr = RGB(192,192,192);//(COLORREF)lpDIS->itemData; // RGB in item data
	COLORREF crHilite = RGB(255-GetRValue(cr),
				  255-GetGValue(cr), 255-GetBValue(cr));
	CBrush br, brAction, *pOldBrush;
	br.CreateSolidBrush(cr);
	brAction.CreateSolidBrush(crHilite);
	pOldBrush = pDC->SelectObject(&br);

	if (lpDIS->itemAction & ODA_DRAWENTIRE)
	{
      // Paint the color item in the color requested
		pDC->FillRect(&lpDIS->rcItem, &br);
	}

	if ((lpDIS->itemState & ODS_SELECTED) &&
	  (lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
	  // item has been selected - hilite frame
		pDC->FrameRect(&lpDIS->rcItem, &brAction);
	}

	if (!(lpDIS->itemState & ODS_SELECTED) &&
	  (lpDIS->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove frame
		pDC->FrameRect(&lpDIS->rcItem, &br);
	}

	if(m_bBitMap)
	{
		for (int i = 0; i < m_CoolBmp.GetSize(); i++)	{
			CBitmap bitMap, *pOldBitmap;
			CDC mDC;
			if (mDC.CreateCompatibleDC(pDC))	{
				bitMap.LoadBitmap(m_CoolBmp.GetAt(i));
				pOldBitmap = mDC.SelectObject(&bitMap);
				pDC->BitBlt(1, i*20, 27, 18, &mDC, 0, 0, SRCCOPY);
				mDC.SelectObject( pOldBitmap );
			}
			mDC.DeleteDC();
		}
	}

	pDC->SelectObject(pOldBrush);

	br.DeleteObject();			//shc 20021113
	brAction.DeleteObject();	//shc 20021113
}

void CPopUpMenu::AddBmpCool(int nRes)
{
	m_CoolBmp.Add(nRes);
}
void CPopUpMenu::AddBmpHot(int nRes)
{
	m_HotBmp.Add(nRes);
}

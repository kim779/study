// SkinHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SkinHeaderCtrl.h"

#include "memDC.h"
#include "myfont.h"


#include "resource.h"
#include "mybitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinHeaderCtrl

CSkinHeaderCtrl::CSkinHeaderCtrl()
{
	m_Font.CreateFont(12, "±¼¸²");
}

CSkinHeaderCtrl::~CSkinHeaderCtrl()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSkinHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CSkinHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinHeaderCtrl message handlers

void CSkinHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	TCHAR buf[256]{};
	HD_ITEM hditem;
	
	hditem.mask = HDI_TEXT | HDI_FORMAT;
	hditem.pszText = buf;
	hditem.cchTextMax = 255;

	GetItem( lpDrawItemStruct->itemID, &hditem );

}

void CSkinHeaderCtrl::OnPaint() 
{
	// ¸®½ºÆ® ÄÁÆ®·Ñ Çì´õ¸¦ ±×·ÁÁÜ...
	CPaintDC dc(this); // device context for painting
	
	CRect rect, rc;
	GetClientRect(&rect);
	CMemDC memDC(&dc);
		
	memDC.FillSolidRect(&rect, RGB(74,82,107));
	
	CMyBitmap bitmapSpan;

	bitmapSpan.LoadBitmap(IDB_COLUMNHEADER_SPAN);
	
	for(int ii = 0; ii < rect.Width(); ii++)
	{
		const CRect rc((rect.left+2)+(ii*1), 0, 1, rect.bottom);
		bitmapSpan.Draw(memDC, rc);
	}
	
	const int nItems = GetItemCount();	// ÅÇÀÇ °¹¼ö ...
	for(int ii = 0; ii < nItems; ii++)
	{

		TCHAR buf[256]{};
		HD_ITEM hditem1;
		
		hditem1.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem1.pszText = buf;
		hditem1.cchTextMax = 255;
		GetItem(ii, &hditem1 );
		
		GetItemRect(ii, &rect);
		
		CMyBitmap bitmap;
		CMyBitmap bitmap2;
		CMyBitmap bitmap3;

		bitmap.LoadBitmap(IDB_COLUMNHEADER_START);
		bitmap2.LoadBitmap(IDB_COLUMNHEADER_SPAN);
		bitmap3.LoadBitmap(IDB_COLUMNHEADER_END);
		
		if(hditem1.iOrder==0)
		{
			rc.SetRect(rect.left, rect.top, 2 , rect.bottom);
			bitmap.Draw(memDC, rc);
		}
		else
		{
			rc.SetRect(rect.left ,rect.top, rect.left + 2, rect.bottom);
			bitmap.Draw(memDC, rc);
			
			rc.SetRect(rect.right, rect.top, rect.right + 1, rect.bottom);
			bitmap.Draw(memDC, rc);
		}

		const UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS ;

		CString szText(buf);

//		CMyFont font;

//		font.SetMyFont(12, "±¼¸²");

//		memDC.SelectObject(font);
		memDC.SelectObject(m_Font);

		memDC.SetBkMode(TRANSPARENT);

		memDC.DrawText(buf, rect, uFormat);

//		DeleteObject(font);
		DeleteObject(bitmap);
		DeleteObject(bitmap3);
		DeleteObject(bitmap2);
	}

	DeleteObject(bitmapSpan);
}

BOOL CSkinHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return false;
	//return CHeaderCtrl::OnEraseBkgnd(pDC);
}

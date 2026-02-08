// ImgHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ImgHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/************************************************************************/
/* CMemDC                                                               */
/************************************************************************/
class CMemDC : public CDC 
{
private:
	std::unique_ptr<CBitmap>	m_bitmap;
	CBitmap*	m_oldBitmap;
	CDC*		m_pDC;
	CRect		m_rcBounds;
public:
	CMemDC(CDC* pDC, const CRect& rcBounds) : CDC()
	{
		CreateCompatibleDC(pDC);
		m_bitmap = std::make_unique<CBitmap>();
		m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
		m_oldBitmap = SelectObject(m_bitmap.get());
		m_pDC = pDC;
		m_rcBounds = rcBounds;
	}
	~CMemDC() 
	{
		m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), 
					this, m_rcBounds.left, m_rcBounds.top, SRCCOPY);
		SelectObject(m_oldBitmap);
	}
	CMemDC* operator->() 
	{
		return this;
	}
};


/////////////////////////////////////////////////////////////////////////////
// CImgHeaderCtrl

CImgHeaderCtrl::CImgHeaderCtrl()
{
	m_bmpStart = m_bmpSpan = m_bmpEnd = NULL;
}

CImgHeaderCtrl::~CImgHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CImgHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CImgHeaderCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgHeaderCtrl message handlers
void CImgHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT di)
{
}	

void CImgHeaderCtrl::SetImgBitmap(CBitmap* bmpStart, CBitmap* bmpSpan, CBitmap* bmpEnd)
{
	m_bmpStart = bmpStart;
	m_bmpSpan = bmpSpan;
	m_bmpEnd = bmpEnd;
}

void CImgHeaderCtrl::OnPaint() 
{
	ASSERT(m_bmpStart && m_bmpSpan && m_bmpEnd);
	
	CPaintDC dc(this); // device context for painting

	const int HDHEIGHT = 18;

	CRect rect, rectItem, clientRect;
	GetClientRect(&rect);
	GetClientRect(&clientRect);
	CMemDC memDC(&dc, rect);
	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(&dc);
	
	memDC.FillSolidRect(&rect, RGB(76,85,118));

	CBitmap* pOldBitmapSpan = bitmapDC.SelectObject(m_bmpSpan);

	memDC.StretchBlt(rect.left+2, 0, rect.Width(), HDHEIGHT, &bitmapDC, 0, 0, 1, HDHEIGHT, SRCCOPY);

	bitmapDC.SelectObject(pOldBitmapSpan);
	
	const int nItems = GetItemCount();

	for(int i = 0; i <nItems; i++)
	{
		
		TCHAR buf1[256]{};
		HD_ITEM hditem1;
		
		hditem1.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem1.pszText = buf1;
		hditem1.cchTextMax = 255;
		GetItem( i, &hditem1 );
		
		GetItemRect(i, &rect);
		
		CBitmap* pOldBitmap = NULL;
		
		//make sure we draw the start piece
		//on the first item so it has a left border

		//For the following items we will just use the
		//right border of the previous items as the left
		//border
		if(hditem1.iOrder==0)
		{
			pOldBitmap = bitmapDC.SelectObject(m_bmpStart);
			memDC.BitBlt(rect.left,rect.top,2,HDHEIGHT,&bitmapDC,0,0,SRCCOPY);
		}
		else
		{
			memDC.BitBlt(rect.left-1,rect.top,2,HDHEIGHT,&bitmapDC,0,0,SRCCOPY);
			pOldBitmap = bitmapDC.SelectObject(m_bmpSpan);
			memDC.BitBlt(rect.left+1,rect.top,1,HDHEIGHT,&bitmapDC,0,0,SRCCOPY);
		}

		bitmapDC.SelectObject(pOldBitmap);
		
		//span the bitmap for the width of the column header item
		const int nWidth = rect.Width() - 4;
		
		CBitmap* pOldBitmap2 = bitmapDC.SelectObject(m_bmpSpan);
		
		memDC.StretchBlt(rect.left+2, 0, nWidth, HDHEIGHT, &bitmapDC, 0,0, 1, HDHEIGHT, SRCCOPY);

		bitmapDC.SelectObject(pOldBitmap2);
		
		
		//draw the end piece of the column header
		CBitmap* pOldBitmap3 = bitmapDC.SelectObject(m_bmpEnd);
		memDC.BitBlt((rect.right-2), 0, 2, HDHEIGHT, &bitmapDC,0,0,SRCCOPY);
		bitmapDC.SelectObject(pOldBitmap3);
		
		//Get all the info for the current
		//item so we can draw the text to it
		//in the desired font and style
		DRAWITEMSTRUCT	DrawItemStruct;
		GetItemRect(i, &rectItem);
		
		DrawItemStruct.CtlType		= 100;
		DrawItemStruct.hDC			= dc.GetSafeHdc();
		DrawItemStruct.itemAction	= ODA_DRAWENTIRE; 
		DrawItemStruct.hwndItem 	= GetSafeHwnd(); 
		DrawItemStruct.rcItem	= rectItem;
		DrawItemStruct.itemID	= i;
		DrawItem(&DrawItemStruct);
		
		const UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER |DT_CENTER | DT_END_ELLIPSIS ;
		
		
		CFont font;
		font.CreatePointFont(90, "±¼¸²Ã¼");

		CFont* def_font = memDC.SelectObject(&font);
		
		memDC.SetBkMode(TRANSPARENT);
		//rectItem.DeflateRect(2,2,2,2);
		
		TCHAR buf[256]{};
		HD_ITEM hditem;
		
		hditem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem.pszText = buf;
		hditem.cchTextMax = 255;
		GetItem( DrawItemStruct.itemID, &hditem );

		memDC.DrawText(buf, &rectItem, uFormat);
		memDC.SelectObject(def_font);
		font.DeleteObject();
	}		
}

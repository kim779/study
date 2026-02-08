// XHeaderCtrl.cpp
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This code is based on "Outlook 98-Style FlatHeader Control" 
// by Maarten Hoeben.
//
// See http://www.codeguru.com/listview/FlatHeader.shtml
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXHeaderCtrl

IMPLEMENT_DYNCREATE(CXHeaderCtrl, CHeaderCtrl)

BEGIN_MESSAGE_MAP(CXHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CXHeaderCtrl)
	ON_MESSAGE(HDM_INSERTITEMA, OnInsertItem)
	ON_MESSAGE(HDM_INSERTITEMW, OnInsertItem)
	ON_MESSAGE(HDM_DELETEITEM, OnDeleteItem)
	ON_MESSAGE(HDM_SETIMAGELIST, OnSetImageList)
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_WM_PAINT()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CXHeaderCtrl::CXHeaderCtrl()
{
	m_bDoubleBuffer = TRUE;
	m_iSpacing = 6;
	m_sizeArrow.cx = 8;
	m_sizeArrow.cy = 8;
	m_sizeImage.cx = 0;
	m_sizeImage.cy = 0;
	m_bStaticBorder = FALSE;
	m_nDontDropCursor = 0;
	m_bResizing = FALSE;
	m_nClickFlags = 0;

//	m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
//	m_cr3DHighLight = RGB(255,255,255);
	m_cr3DHighLight = RGB(204,204,204);
//	m_cr3DHighLight = RGB(218,218,218);
//	m_cr3DShadow    = ::GetSysColor(COLOR_3DSHADOW);
//	m_cr3DShadow = RGB(103,119,138);
	m_cr3DShadow	= RGB(240,240,240); //RGB(64,144,222);
//	m_cr3DFace      = ::GetSysColor(COLOR_3DFACE);
//	m_cr3DFace		= RGB(239,242,249);
	m_cr3DFace		= RGB(240,240,240);
//	m_crBtnText     = ::GetSysColor(COLOR_BTNTEXT);
	m_crBtnText		= CLR_FONT_DEFAULT;

	m_nSortCol = -1;
}

///////////////////////////////////////////////////////////////////////////////
// dtor
CXHeaderCtrl::~CXHeaderCtrl()
{
}

///////////////////////////////////////////////////////////////////////////////
// ModifyProperty
BOOL CXHeaderCtrl::ModifyProperty(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case FH_PROPERTY_SPACING:
			m_iSpacing = (int)lParam;
			break;

		case FH_PROPERTY_ARROW:
			m_sizeArrow.cx = LOWORD(lParam);
			m_sizeArrow.cy = HIWORD(lParam);
			break;

		case FH_PROPERTY_STATICBORDER:
			m_bStaticBorder = (BOOL)lParam;
			break;

		case FH_PROPERTY_DONTDROPCURSOR:
			m_nDontDropCursor = (UINT)lParam;
			break;

		default:
			return FALSE;
	}

	Invalidate();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// DrawCtrl
void CXHeaderCtrl::DrawCtrl(CDC* pDC)
{
	CRect rectClip;
	if (pDC->GetClipBox(&rectClip) == ERROR)
		return;

	CRect rectClient, rectItem;
	GetClientRect(&rectClient);

    pDC->FillSolidRect(rectClip, m_cr3DFace);

	int iItems = GetItemCount();
	ASSERT(iItems >= 0);

	CPen penHighLight(PS_SOLID, 1, m_cr3DHighLight);
	CPen penShadow(PS_SOLID, 1, m_cr3DShadow);
	CPen* pPen = pDC->GetCurrentPen();

	CFont* pFont = pDC->SelectObject(GetFont());

	pDC->SetBkColor(m_cr3DFace);
	pDC->SetTextColor(m_crBtnText);

	int iWidth = 0;

	for (int i = 0; i < iItems; i++)
	{
		int iItem = OrderToIndex(i);

		TCHAR szText[FLATHEADER_TEXT_MAX];

		HDITEM hditem;
		hditem.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT|HDI_IMAGE|HDI_BITMAP;
		hditem.pszText = szText;
		hditem.cchTextMax = sizeof(szText);
		VERIFY(GetItem(iItem, &hditem));

		VERIFY(GetItemRect(iItem, rectItem));

		if (rectItem.right >= rectClip.left || rectItem.left <= rectClip.right)
		{
			if (hditem.fmt & HDF_OWNERDRAW)
			{
				DRAWITEMSTRUCT disItem;
				disItem.CtlType = ODT_BUTTON;
				disItem.CtlID = GetDlgCtrlID();
				disItem.itemID = iItem;
				disItem.itemAction = ODA_DRAWENTIRE;
				disItem.itemState = 0;
				disItem.hwndItem = m_hWnd;
				disItem.hDC = pDC->m_hDC;
				disItem.rcItem = rectItem;
				disItem.itemData = 0;

				DrawItem(&disItem);
			}
			else
			{
				rectItem.DeflateRect(m_iSpacing, 0);
				DrawItem(pDC, rectItem, &hditem);
				

				//int offset = pDC->GetTextExtent(_T(" "), 1 ).cx*2/2;
				int offset = 8;
				int offsetY = 6;
				int nShiftX = 16;
				// Draw the Sort arrow
				if( iItem == m_nSortCol )
				{
					CRect rcIcon(rectItem);

					// Set up pens to use for drawing the triangle
					CPen penLight(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
					CPen penShadow(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
					CPen *pOldPen = pDC->SelectObject( &penLight );

					if(m_bSortAsc)
					{
						// Draw triangle pointing upwards
						pDC->MoveTo( rcIcon.right - 2*offset + offset*2, offsetY-1);
						pDC->LineTo( rcIcon.right - 3*offset/2 + offset*2, rcIcon.bottom - offsetY );
						pDC->LineTo( rcIcon.right - 5*offset/2-2 + offset*2, rcIcon.bottom - offsetY );
						pDC->MoveTo( rcIcon.right - 5*offset/2-1 + offset*2, rcIcon.bottom - offsetY-1 );

						pDC->SelectObject( &penShadow );
						pDC->LineTo( rcIcon.right - 2*offset + offset*2, offsetY-2);
					}
					else
					{
						// Draw triangle pointing downwords
						pDC->MoveTo( rcIcon.right - 3*offset/2 + offset*2, offsetY-1);
						pDC->LineTo( rcIcon.right - 2*offset-1 + offset*2, rcIcon.bottom - offsetY + 1 );
						pDC->MoveTo( rcIcon.right - 2*offset-1 + offset*2, rcIcon.bottom - offsetY );

						pDC->SelectObject( &penShadow );
						pDC->LineTo( rcIcon.right - 5*offset/2-1 + offset*2, offsetY -1 );
						pDC->LineTo( rcIcon.right - 3*offset/2 + offset*2, offsetY -1);
					}

					// Restore the pen
					pDC->SelectObject( pOldPen );
				}
				rectItem.InflateRect(m_iSpacing, 0);

				//if (m_nClickFlags & MK_LBUTTON && m_iHotIndex == iItem && m_hdhtiHotItem.flags & HHT_ONHEADER)
				//	pDC->InvertRect(rectItem);
			}

//			if (i < iItems-1)
			{
				pDC->SelectObject(&penHighLight);
				pDC->MoveTo(rectItem.right-1, rectItem.top+1);
				pDC->LineTo(rectItem.right-1, rectItem.bottom-1);

				//pDC->SelectObject(&penHighLight);
				//pDC->MoveTo(rectItem.right, rectItem.top);
				//pDC->LineTo(rectItem.right, rectItem.bottom-1);
			}
		}

		iWidth += hditem.cxy;
	}

	if (iWidth > 0)
	{
		//Rect
		CRect rcRest = rectClient;
		rectClient.right = rectClient.left + iWidth;
		//pDC->Draw3dRect(rectClient, m_cr3DHighLight, m_cr3DShadow);
		//pDC->Draw3dRect(rectClient, m_cr3DShadow, m_cr3DShadow);
//		pDC->SelectObject(&penShadow);
		pDC->MoveTo(rectClient.left, rectClient.top);
		pDC->LineTo(rectClient.right-1, rectClient.top);
		pDC->LineTo(rectClient.right-1, rectClient.bottom-1);
		pDC->LineTo(rectClient.left, rectClient.bottom-1);
		pDC->LineTo(rectClient.left, rectClient.top);
//		pDC->MoveTo(rectClient.right, rectClient.top);
//		pDC->LineTo(rcRest.right-1, rcRest.top);
		pDC->MoveTo(rcRest.right-1, rcRest.top);
		pDC->LineTo(rectClient.right, rectClient.top);
		pDC->MoveTo(rcRest.right-1, rcRest.bottom-1);
		pDC->LineTo(rectClient.right, rectClient.bottom-1);
	}

	pDC->SelectObject(pFont);
	pDC->SelectObject(pPen);

	penHighLight.DeleteObject();
	penShadow.DeleteObject();
}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CXHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT)
{
	ASSERT(FALSE);  // must override for self draw header controls
}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CXHeaderCtrl::DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	ASSERT(lphdi->mask & HDI_FORMAT);

	int iWidth = 0;

	CBitmap* pBitmap = NULL;
	BITMAP BitmapInfo;

	if (lphdi->fmt & HDF_BITMAP)
	{
		ASSERT(lphdi->mask & HDI_BITMAP);
		ASSERT(lphdi->hbm);

		pBitmap = CBitmap::FromHandle(lphdi->hbm);
		if (pBitmap)
			VERIFY(pBitmap->GetObject(sizeof(BITMAP), &BitmapInfo));
	}

	rect.left += ((iWidth = DrawImage(pDC, rect, lphdi, FALSE)) != 0) ? iWidth + m_iSpacing : 0;
	rect.right -= ((iWidth = DrawBitmap(pDC, rect, lphdi, pBitmap, &BitmapInfo, TRUE)) != 0) ? 
		iWidth + m_iSpacing : 0;
	
	DrawText(pDC, rect, lphdi);
	
}

///////////////////////////////////////////////////////////////////////////////
// DrawImage
int CXHeaderCtrl::DrawImage(CDC* pDC, CRect rect, LPHDITEM lphdi, BOOL bRight)
{
	CImageList* pImageList = GetImageList();
	int iWidth = 0;

	if (lphdi->iImage != XHEADERCTRL_NO_IMAGE)
	{
		if (pImageList)
		{
			if (rect.Width() > 0)
			{
				POINT point;

				point.y = rect.CenterPoint().y - (m_sizeImage.cy >> 1);

				if (bRight)
					point.x = rect.right - m_sizeImage.cx;
				else
					point.x = rect.left;

				SIZE size;
				size.cx = rect.Width()<m_sizeImage.cx ? rect.Width():m_sizeImage.cx;
				size.cy = m_sizeImage.cy;

				// save image list background color
				COLORREF rgb = pImageList->GetBkColor();

				// set image list background color to same as header control
				pImageList->SetBkColor(pDC->GetBkColor());
				pImageList->DrawIndirect(pDC, lphdi->iImage, point, size, CPoint(0, 0));
				pImageList->SetBkColor(rgb);

				iWidth = m_sizeImage.cx;
			}
		}
	}

	return iWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawBitmap
int CXHeaderCtrl::DrawBitmap(CDC* pDC, 
								CRect rect, 
								LPHDITEM lphdi, 
								CBitmap* pBitmap, 
								BITMAP* pBitmapInfo, 
								BOOL bRight)
{
	UNUSED_ALWAYS(lphdi);

	int iWidth = 0;

	if (pBitmap)
	{
		iWidth = pBitmapInfo->bmWidth;
		if (iWidth <= rect.Width() && rect.Width() > 0)
		{
			POINT point;

			point.y = rect.CenterPoint().y - (pBitmapInfo->bmHeight >> 1);

			if (bRight)
				point.x = rect.right - iWidth;
			else
				point.x = rect.left;

			CDC dc;
			if (dc.CreateCompatibleDC(pDC) == TRUE) 
			{
				VERIFY(dc.SelectObject(pBitmap));
				iWidth = pDC->BitBlt(
					point.x, point.y, 
					pBitmapInfo->bmWidth, pBitmapInfo->bmHeight, 
					&dc, 
					0, 0, 
					SRCCOPY
				) ? iWidth:0;
			}
			else 
				iWidth = 0;
		}
		else
			iWidth = 0;
	}

	return iWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawText
int CXHeaderCtrl::DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	CSize size;

	pDC->SetTextColor(m_crBtnText);//header text
	pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));

	if (rect.Width() > 0 && lphdi->mask & HDI_TEXT && lphdi->fmt & HDF_STRING)
	{
		size = pDC->GetTextExtent(lphdi->pszText);

		// always center column headers
		pDC->DrawText(lphdi->pszText, -1, rect, 
			DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);

	}

	size.cx = rect.Width()>size.cx ? size.cx:rect.Width();
	return size.cx>0 ? size.cx:0;
}

///////////////////////////////////////////////////////////////////////////////
// OnInsertItem
LRESULT CXHeaderCtrl::OnInsertItem(WPARAM, LPARAM)
{
	return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnDeleteItem
LRESULT CXHeaderCtrl::OnDeleteItem(WPARAM, LPARAM)
{
	return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnSetImageList
LRESULT CXHeaderCtrl::OnSetImageList(WPARAM, LPARAM lParam)
{
	CImageList* pImageList;
	pImageList = CImageList::FromHandle((HIMAGELIST)lParam);

	IMAGEINFO info;
	if (pImageList->GetImageInfo(0, &info))
	{
		m_sizeImage.cx = info.rcImage.right - info.rcImage.left;
		m_sizeImage.cy = info.rcImage.bottom - info.rcImage.top;
	}

	return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnLayout
LRESULT CXHeaderCtrl::OnLayout(WPARAM, LPARAM lParam)
{
	LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;

	if (m_bStaticBorder)
		lphdlayout->prc->right += GetSystemMetrics(SM_CXBORDER)*2;

	return CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
}

///////////////////////////////////////////////////////////////////////////////
// OnSysColorChange
void CXHeaderCtrl::OnSysColorChange() 
{
	TRACE(_T("in CXHeaderCtrl::OnSysColorChange\n"));

	CHeaderCtrl::OnSysColorChange();
	
//	m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DHighLight = RGB(239,242,249);
//	m_cr3DShadow    = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DShadow	= RGB(103,119,138);
//	m_cr3DFace      = ::GetSysColor(COLOR_3DFACE);
	m_cr3DFace		= RGB(239,242,249);
//	m_crBtnText     = ::GetSysColor(COLOR_BTNTEXT);
	m_crBtnText		= CLR_FONT_DEFAULT;
}

///////////////////////////////////////////////////////////////////////////////
// OnEraseBkgnd
BOOL CXHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// OnPaint
void CXHeaderCtrl::OnPaint() 
{
    CPaintDC dc(this);

    if (m_bDoubleBuffer)
    {
        CMemDC MemDC(&dc);
        DrawCtrl(&MemDC);
    }
    else
        DrawCtrl(&dc);
}

void CXHeaderCtrl::SetSortImage(int nCol,BOOL bAscending )
{
	m_nSortCol = nCol;
	m_bSortAsc = bAscending;
}

void CXHeaderCtrl::SetHeaderColor(COLORREF clrBkgrnd, COLORREF clrOutLine)
{
	m_cr3DShadow = clrOutLine;
	m_cr3DFace = clrBkgrnd;
}


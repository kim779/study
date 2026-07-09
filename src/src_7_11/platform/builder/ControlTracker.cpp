// ControlTracker.cpp: implementation of the CControlTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axiswork.h"
#include "ControlTracker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// various GDI objects we need to draw
AFX_STATIC_DATA HCURSOR _afxCursors[10] = { 0, };
AFX_STATIC_DATA HBRUSH _afxHatchBrush = 0;
AFX_STATIC_DATA HPEN _afxBlackDottedPen = 0;
AFX_STATIC_DATA int _afxHandleSize = 0;

// the struct below is used to determine the qualities of a particular handle
struct AFX_HANDLEINFO
{
	size_t nOffsetX;    // offset within RECT for X coordinate
	size_t nOffsetY;    // offset within RECT for Y coordinate
	int nCenterX;       // adjust X by Width()/2 * this number
	int nCenterY;       // adjust Y by Height()/2 * this number
	int nHandleX;       // adjust X by handle size * this number
	int nHandleY;       // adjust Y by handle size * this number
	int nInvertX;       // handle converts to this when X inverted
	int nInvertY;       // handle converts to this when Y inverted
};

// this array describes all 8 handles (clock-wise)
AFX_STATIC_DATA const AFX_HANDLEINFO _afxHandleInfo[] =
{
	// corner handles (top-left, top-right, bottom-right, bottom-left
	{ offsetof(RECT, left), offsetof(RECT, top),        0, 0,  0,  0, 1, 3 },
	{ offsetof(RECT, right), offsetof(RECT, top),       0, 0, -1,  0, 0, 2 },
	{ offsetof(RECT, right), offsetof(RECT, bottom),    0, 0, -1, -1, 3, 1 },
	{ offsetof(RECT, left), offsetof(RECT, bottom),     0, 0,  0, -1, 2, 0 },

	// side handles (top, right, bottom, left)
	{ offsetof(RECT, left), offsetof(RECT, top),        1, 0,  0,  0, 4, 6 },
	{ offsetof(RECT, right), offsetof(RECT, top),       0, 1, -1,  0, 7, 5 },
	{ offsetof(RECT, left), offsetof(RECT, bottom),     1, 0,  0, -1, 6, 4 },
	{ offsetof(RECT, left), offsetof(RECT, top),        0, 1,  0,  0, 5, 7 }
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlTracker::CControlTracker(LPCRECT lpSrcRect, UINT nStyle)
{
	ASSERT(AfxIsValidAddress(lpSrcRect, sizeof(RECT), FALSE));

	Construct();
	m_rect.CopyRect(lpSrcRect);
	m_nStyle = nStyle;
}

CControlTracker::~CControlTracker()
{

}

void CControlTracker::Draw(CDC* pDC)
{
	// set initial DC state
	VERIFY(pDC->SaveDC() != 0);
	pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportOrg(0, 0);
	pDC->SetWindowOrg(0, 0);

	// get normalized rectangle
	CRect rect = m_rect;
	rect.NormalizeRect();

	CPen* pOldPen = NULL;
	CBrush* pOldBrush = NULL;
	CGdiObject* pTemp;
	int nOldROP;

	// draw lines
	if ((m_nStyle & (dottedLine|solidLine)) != 0)
	{
		if (m_nStyle & dottedLine)
			pOldPen = pDC->SelectObject(CPen::FromHandle(_afxBlackDottedPen));
		else
			pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		nOldROP = pDC->SetROP2(R2_COPYPEN);
		rect.InflateRect(+1, +1);   // borders are one pixel outside
		pDC->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// if hatchBrush is going to be used, need to unrealize it
	if ((m_nStyle & (hatchInside|hatchedBorder)) != 0)
		UnrealizeObject(_afxHatchBrush);

	// hatch inside
	if ((m_nStyle & hatchInside) != 0)
	{
		pTemp = pDC->SelectStockObject(NULL_PEN);
		if (pOldPen == NULL)
			pOldPen = (CPen*)pTemp;
		pTemp = pDC->SelectObject(CBrush::FromHandle(_afxHatchBrush));
		if (pOldBrush == NULL)
			pOldBrush = (CBrush*)pTemp;
		pDC->SetBkMode(TRANSPARENT);
		nOldROP = pDC->SetROP2(R2_MASKNOTPEN);
		pDC->Rectangle(rect.left+1, rect.top+1, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// draw hatched border
	if ((m_nStyle & hatchedBorder) != 0)
	{
		bool bIn = false;
		if (!(m_nStyle & resizeInside))
			bIn = true;
		
		m_nStyle |= resizeInside;
		UINT mask = GetHandleMask();
		for (int i = 0; i < 8; ++i)
		{
			if (mask & (1<<i))
			{
				GetHandleRect((TrackerHit)i, &rect);
				pDC->FillSolidRect(rect, RGB(0, 0, 100));
				rect.DeflateRect(1, 1);
				pDC->FillSolidRect(rect, RGB(255, 255, 255));
			}
		}
		if (bIn)
			m_nStyle &= ~resizeInside;
	}
	if ((m_nStyle & (resizeInside|resizeOutside)) != 0)
	{
		UINT mask = GetHandleMask();
		for (int i = 0; i < 8; ++i)
		{
			if (mask & (1<<i))
			{
				GetHandleRect((TrackerHit)i, &rect);
				pDC->FillSolidRect(rect, RGB(255, 255, 255));
				rect.DeflateRect(1, 1);
				pDC->FillSolidRect(rect, RGB(0, 0, 100));
			}
		}
	}

	// cleanup pDC state
	if (pOldPen != NULL)
		pDC->SelectObject(pOldPen);
	if (pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);
	VERIFY(pDC->RestoreDC(-1));
}

void CControlTracker::GetHandleRect(int nHandle, CRect* pHandleRect)
{
	ASSERT(nHandle < 8);

	// get normalized rectangle of the tracker
	CRect rectT = m_rect;
	rectT.NormalizeRect();
	if ((m_nStyle & (solidLine|dottedLine)) != 0)
		rectT.InflateRect(+1, +1);

	// since the rectangle itself was normalized, we also have to invert the
	//  resize handles.
	nHandle = NormalizeHit(nHandle);

	// handle case of resize handles outside the tracker
	int size = GetHandleSize() + 2;
	if (m_nStyle & resizeOutside)
		rectT.InflateRect(size-1, size-1);

	// calculate position of the resize handle
	int nWidth = rectT.Width();
	int nHeight = rectT.Height();
	CRect rect;
	const AFX_HANDLEINFO* pHandleInfo = &_afxHandleInfo[nHandle];
	rect.left = *(int*)((BYTE*)&rectT + pHandleInfo->nOffsetX);
	rect.top = *(int*)((BYTE*)&rectT + pHandleInfo->nOffsetY);
	rect.left += size * pHandleInfo->nHandleX;
	rect.top += size * pHandleInfo->nHandleY;
	rect.left += pHandleInfo->nCenterX * (nWidth - size) / 2;
	rect.top += pHandleInfo->nCenterY * (nHeight - size) / 2;
	rect.right = rect.left + size;
	rect.bottom = rect.top + size;

	*pHandleRect = rect;
}

int CControlTracker::HitTestHandles(CPoint point)
{
	CRect rect;
	UINT mask = GetHandleMask();

	// see if hit anywhere inside the tracker
	GetTrueRect(&rect);
	if (!rect.PtInRect(point))
		return hitNothing;  // totally missed

	// see if we hit a handle
	for (int i = 0; i < 8; ++i)
	{
		if (mask & (1<<i))
		{
			GetHandleRect((TrackerHit)i, &rect);
			if (rect.PtInRect(point))
				return (TrackerHit)i;
		}
	}

	// last of all, check for non-hit outside of object, between resize handles
	if ((m_nStyle & hatchedBorder) == 0)
	{
		CRect rect = m_rect;
		rect.NormalizeRect();
		if ((m_nStyle & dottedLine|solidLine) != 0)
			rect.InflateRect(+1, +1);
		if (!rect.PtInRect(point))
			return hitNothing;  // must have been between resize handles
	}
	return hitMiddle;   // no handle hit, but hit object (or object border)
}

int CControlTracker::HitTest(CPoint point)
{
	TrackerHit hitResult = hitNothing;

	CRect rectTrue;
	GetTrueRect(&rectTrue);
	ASSERT(rectTrue.left <= rectTrue.right);
	ASSERT(rectTrue.top <= rectTrue.bottom);
	if (rectTrue.PtInRect(point))
	{
		if ((m_nStyle & (resizeInside|resizeOutside)) != 0)
			hitResult = (TrackerHit)HitTestHandles(point);
		else
			hitResult = hitMiddle;
	}
	return hitResult;
}

void CControlTracker::GetTrueRect(LPRECT lpTrueRect)
{
	ASSERT(AfxIsValidAddress(lpTrueRect, sizeof(RECT)));

	CRect rect = m_rect;
	rect.NormalizeRect();
	int nInflateBy = 0;
	if ((m_nStyle & (resizeOutside|hatchedBorder)) != 0)
		nInflateBy += GetHandleSize() + 1;
	if ((m_nStyle & (solidLine|dottedLine)) != 0)
		++nInflateBy;
	rect.InflateRect(nInflateBy, nInflateBy);
	*lpTrueRect = rect;
}

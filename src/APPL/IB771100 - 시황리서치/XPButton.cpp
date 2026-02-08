// XPButton.cpp : implementation file
//

#include "stdafx.h"
#include "XPButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXPButton
#include "BkUtil.h"

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CXPButton, CButton)
//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1999, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//                   Added print support. - KR
//
//           11/3/99 Fixed most common complaint. Added
//                   background color fill. - KR
//
//           11/3/99 Added support for mapping modes other than
//                   MM_TEXT as suggested by Lee Sang Hun. - KR
//
// This class implements a memory Device Context which allows
// flicker free drawing.

CXPButton::CXPButton()
{
	pBoundryPen.CreatePen(PS_INSIDEFRAME|PS_SOLID,1,RGB(0,0,0));
	pInsideBoundryPenLeft.CreatePen(PS_INSIDEFRAME|PS_SOLID,1,RGB(250,196,88)); 
	pInsideBoundryPenRight.CreatePen(PS_INSIDEFRAME|PS_SOLID,1,RGB(251,202,106)); 
	pInsideBoundryPenTop.CreatePen(PS_INSIDEFRAME|PS_SOLID,1,RGB(252,210,121)); 
	pInsideBoundryPenBottom.CreatePen(PS_INSIDEFRAME|PS_SOLID,1,RGB(229,151,0)); 
	pFillActive.CreateSolidBrush(RGB(222,223,236));
	pFillInactive.CreateSolidBrush(RGB(222,223,236));
	
	pInsideBoundryPenLeftSel.CreatePen(PS_INSIDEFRAME|PS_SOLID,1,RGB(153,198,252)); 
	pInsideBoundryPenTopSel.CreatePen(PS_INSIDEFRAME|PS_SOLID,1,RGB(162,201,255));
	pInsideBoundryPenRightSel.CreatePen(PS_INSIDEFRAME|PS_SOLID,1,RGB(162,189,252));
	pInsideBoundryPenBottomSel.CreatePen(PS_INSIDEFRAME|PS_SOLID,1,RGB(162,201,255));

	m_bOverControl = m_bTracking = m_bSelected = Focus = 0;
}

CXPButton::~CXPButton()
{
	pBoundryPen.DeleteObject();
	
	pFillActive.DeleteObject();
	pFillInactive.DeleteObject();
	
	pInsideBoundryPenLeft.DeleteObject();
	pInsideBoundryPenRight.DeleteObject();
	pInsideBoundryPenBottom.DeleteObject();
	pInsideBoundryPenTop.DeleteObject();
	
	pInsideBoundryPenLeftSel.DeleteObject();
	pInsideBoundryPenTopSel.DeleteObject();
	pInsideBoundryPenRightSel.DeleteObject();
	pInsideBoundryPenBottomSel.DeleteObject();	
}


BEGIN_MESSAGE_MAP(CXPButton, CButton)
//{{AFX_MSG_MAP(CXPButton)
//}}AFX_MSG_MAP
ON_WM_MOUSEMOVE()
ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()

void CXPButton::DoGradientFill(CDC *pDC, CRect rect)
{
	CBrush pBrush[64];
	const int nWidth = (rect.right) - (rect.left);
	int nHeight = (rect.bottom) - (rect.top);
	CRect rct;
	for (int i=0; i<64; i++)
	{
		if(m_bOverControl)
		{
			if(Focus)
			{
				pBrush[i].CreateSolidBrush(RGB(255-(i/4), 255-(i/4), 255-(i/3)));
			}
			else
			{
				pBrush[i].CreateSolidBrush(RGB(255-(i/4), 255-(i/4), 255-(i/5)));			
			}
		}
		else
		{
			if(Focus)
			{
				pBrush[i].CreateSolidBrush(RGB(255-(i/3), 255-(i/3), 255-(i/4)));
			}
			else
			{
				pBrush[i].CreateSolidBrush(RGB(255-(i/3), 255-(i/3), 255-(i/5)));
			}
		}
	}

	
	for (int i=rect.top; i<nHeight+2; i++) 
	{
		rct.SetRect (rect.left, i, nWidth+2, i + 1);
		pDC->FillRect (&rct, &pBrush[((i * 63) / nHeight)]);
	}
	
	for (int i=0; i<64; i++)
	{
		pBrush[i].DeleteObject();
	}
	
}

void CXPButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* ppDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CMemDC pDC(ppDC);
	CRect rect = lpDrawItemStruct->rcItem;
	const UINT state = lpDrawItemStruct->itemState;
	CPoint pt;
	CString strText;
//	CString &m_title = strText;
	CString m_title;

// escdream check!!
//	GetWindowText(m_strText);
	GetWindowText(m_title);

	pt.x = 0;
	pt.y = 0;
	
	pOldPen = pDC->SelectObject(&pBoundryPen);
	
	pDC->RoundRect(rect,pt);
	if (state & ODS_FOCUS)
	{
		Focus = TRUE;
		m_bSelected = TRUE;
	}
	else
	{
		m_bSelected = FALSE;
		Focus = FALSE;
	}
	
	if (state & ODS_SELECTED || state & ODS_DEFAULT)
	{
   		Focus = TRUE;
	}

	pDC->SelectObject(pOldPen);
	rect.DeflateRect( 1,1,2,2);
	
	if (m_bOverControl)
	{
		pOldBrush = pDC->SelectObject(&pFillActive);   
		DoGradientFill(pDC,rect);		
	}
	else
	{
		pOldBrush = pDC->SelectObject(&pFillInactive);   
		DoGradientFill(pDC,rect);
	}
	if (m_bSelected || m_bOverControl)
	{
		DrawInsideBorder(pDC,rect);
	}
	pDC->SelectObject(pOldBrush);
    
	if (!m_title.IsEmpty())
	{
		const CSize Extent = pDC->GetTextExtent(m_title/*strText*/);
		CPoint pt( rect.CenterPoint().x - Extent.cx/2, 
				rect.CenterPoint().y - Extent.cy/2 );
		if (state & ODS_SELECTED) 
			pt.Offset(1,1);
		
		const int nMode = pDC->SetBkMode(TRANSPARENT);
		
		CFont* pFont = GetFont();
		CFont* pOld = NULL;
		if (pFont)
			pOld = pDC->SelectObject(pFont);
		rect.InflateRect( 1,1,2,2);
		
		if (state & ODS_DISABLED)
			pDC->DrawState(pt, Extent, m_title/*strCaption*/, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		else
			pDC->DrawText(m_title,rect,  DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		if (pOld)
			pDC->SelectObject(pOld);

		pDC->SetBkMode(nMode);
	}
	return;
	
}

void CXPButton::PreSubclassWindow() 
{
	ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void CXPButton::DrawInsideBorder(CDC *pDC, CRect rect)
{
	CPen* left=NULL, * right=NULL, * top=NULL, * bottom=NULL;
	if (m_bSelected && !m_bOverControl)
	{
		left = &pInsideBoundryPenLeftSel;
		top = &pInsideBoundryPenTopSel;
		right = &pInsideBoundryPenRightSel;
		bottom = &pInsideBoundryPenBottomSel;
	}
	else
	{
		left = &pInsideBoundryPenLeft;
		top = &pInsideBoundryPenTop;
		right = &pInsideBoundryPenRight;
		bottom = &pInsideBoundryPenBottom;
	}

	const CPoint oldpoint = pDC->MoveTo(rect.left,rect.bottom);
	pOldPen = pDC->SelectObject(left);
	pDC->LineTo(rect.left,rect.top);
	pDC->SelectObject(right);
	pDC->MoveTo(rect.right,rect.bottom);
	pDC->LineTo(rect.right,rect.top);
	pDC->SelectObject(top);
	pDC->MoveTo(rect.left,rect.top);
	pDC->LineTo(rect.right+1,rect.top);
	pDC->SelectObject(bottom);
	pDC->MoveTo(rect.left,rect.bottom);
	pDC->LineTo(rect.right+1,rect.bottom);
	pDC->SelectObject(pOldPen);	
	pDC->MoveTo(oldpoint);
}

void CXPButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);		
	}	
	CButton::OnMouseMove(nFlags, point);
	
}

LRESULT CXPButton::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bOverControl=TRUE;
	Invalidate();
	return 1;
}

LRESULT CXPButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bOverControl = FALSE;
	Invalidate(FALSE);
	return 0;
}

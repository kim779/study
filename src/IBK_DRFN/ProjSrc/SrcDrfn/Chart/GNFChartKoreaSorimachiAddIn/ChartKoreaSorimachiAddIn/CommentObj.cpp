// CommentObj.cpp: implementation of the CCommentItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommentObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommentItem::CCommentItem():m_rc(0,0,COMMENT_WIDTH,COMMENT_HEIGHT)
{
}

CCommentItem::~CCommentItem()
{
}

void CCommentItem::Draw(CDC *pDC)
{
	CRect rect;
	rect = m_rc;
	pDC->SetBkMode(TRANSPARENT);

//	if (m_lSumOrdCnt <= 0)
//		return;

//	CString strCnt;
//	strCnt.Format("%d", (int)m_lSumOrdCnt);
/*
	if( drawingString != "☞" &&
		drawingString != "☜" &&
		drawingString != "◆" &&
		drawingString != "★" &&
		drawingString != "●" &&
		drawingString != "■")
		♠♣
*/
	CString strDrawingMark = "★";

	if( m_strComment.Left(4) == "진입" )
		pDC->SetTextColor(RGB(0, 208, 208));
	else
		pDC->SetTextColor(RGB(0, 208, 0));

	if (m_bBuy)		// 저가위치
	{
		rect += CPoint(0, rect.Height()/2);
		rect.bottom = rect.top + rect.Height()/ 2;

		pDC->DrawText(strDrawingMark, rect, DT_CENTER | DT_TOP);
		rect += CPoint(0, rect.Height());
		pDC->SetTextColor(RGB(208, 45, 45));
		pDC->DrawText(m_strComment, rect, DT_CENTER | DT_TOP);
	}
	else			// 고가위치
	{
		rect.bottom = rect.top + rect.Height() / 2;

		pDC->DrawText(strDrawingMark, rect, DT_CENTER | DT_BOTTOM);
		rect -= CPoint(0, rect.Height());
		pDC->SetTextColor(RGB(45, 45, 208));
		pDC->DrawText(m_strComment, rect, DT_CENTER | DT_BOTTOM);
	}
}

void CCommentItem::SetDate(long lDate)
{
	int a, b, c;	// 시간 데이터를 분리

	// 시작일
	a = lDate/10000;
	lDate = lDate%10000;
	b = lDate/100;
	lDate = lDate%100;
	c = lDate;

	m_strDate.Format("%d/%d/%d", a,b,c);
}

void CCommentItem::SetPosition(CDC* pDC, CPoint *pPoint)
{
	CRect rect;
	pDC->DrawText(m_strComment, &rect, DT_CALCRECT);

	m_rc.left = pPoint->x - rect.Width()/2;
	m_rc.top = pPoint->y - rect.Height();
	m_rc.right = pPoint->x + rect.Width()/2;
	m_rc.bottom = pPoint->y + rect.Height();
}

BOOL CCommentItem::IsInRegion(CPoint pt)  
{
	if(m_rc.PtInRect(pt))
		return TRUE;
	else 
		return FALSE;
}

CString CCommentItem::GetToolTipText()
{
	return m_strComment;
}

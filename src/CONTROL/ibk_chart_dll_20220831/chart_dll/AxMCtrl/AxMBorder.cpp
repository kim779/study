// AxMBorder.cpp : implementation of the Debug
//

#include "stdafx.h"
#include "AxMBorder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAxMBorder::CAxMBorder()
{
}

CAxMBorder::~CAxMBorder()
{
}

void CAxMBorder::DrawEdge(CDC* dc, CRect rc, UINT edge, UINT flags)
{
	dc->DrawEdge(rc, edge, flags);
}

void CAxMBorder::DrawBorder(CDC* dc, CRect rc, COLORREF borderColor)
{
	CPen pen, *oldPen;
	pen.CreatePen(PS_SOLID, 1, borderColor);

	oldPen = dc->SelectObject(&pen);

	dc->MoveTo(rc.left, rc.top);
	dc->LineTo(rc.right-1, rc.top);
	dc->LineTo(rc.right-1, rc.bottom-1);
	dc->LineTo(rc.left, rc.bottom-1);
	dc->LineTo(rc.left, rc.top);

	dc->SelectObject(oldPen);
}


// STPropertyGridItemLineFormat.cpp: implementation of the CSTPropertyGridItemLineFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "systemtrading.h"
#include "STPropertyGridItemLineFormat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSTPropertyGridItemLineFormat::CSTPropertyGridItemLineFormat(CString strCaption, int nIndex)
	: CXTPPropertyGridItem(strCaption)
{
	m_nIndex = nIndex;
	m_nFlags = 0;
}

CSTPropertyGridItemLineFormat::~CSTPropertyGridItemLineFormat()
{

}

BOOL CSTPropertyGridItemLineFormat::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	COLORREF clr = dc.GetTextColor();

	CPen pen;
	pen.CreatePen(GetPenStyle(), 1, clr);
	dc.SelectObject(pen);

	int nY = rcValue.top + (rcValue.Height() / 2);
	dc.MoveTo(rcValue.left, nY);
	dc.LineTo(rcValue.right - 5, nY);

	pen.DeleteObject();
	return TRUE;
}

int CSTPropertyGridItemLineFormat::GetPenStyle()
{
	switch(m_nIndex)
	{
	case 0:
		return PS_SOLID;
	case 1:
		return PS_DOT;
	case 2:
		return PS_DASH;
	case 3:
		return PS_DASHDOT;
	case 4:
		return PS_DASHDOTDOT;
	}

	return PS_SOLID;
}

void CSTPropertyGridItemLineFormat::SetPenStyle(int nPenStyle)
{
	m_nIndex = nPenStyle;
}


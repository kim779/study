// STPropertyGridItemLineWidth.cpp: implementation of the CSTPropertyGridItemLineWidth class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "systemtrading.h"
#include "STPropertyGridItemLineWidth.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSTPropertyGridItemLineWidth::CSTPropertyGridItemLineWidth(CString strCaption, int nWidth)
	: CXTPPropertyGridItem(strCaption)
{
	m_nWidth = nWidth;
	m_nFlags = 0;
}

CSTPropertyGridItemLineWidth::~CSTPropertyGridItemLineWidth()
{

}

BOOL CSTPropertyGridItemLineWidth::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	COLORREF clr = dc.GetTextColor();

	CRect rcText(rcValue);
	rcText.right = rcText.left + 25;

	CString strWidth;
	strWidth.Format("%dpt", m_nWidth);
	dc.DrawText( strWidth, rcText,  DT_SINGLELINE|DT_VCENTER);

	CPen pen;
	pen.CreatePen(PS_SOLID, m_nWidth, clr);
	dc.SelectObject(pen);

	int nY = rcValue.top + (rcValue.Height() / 2);
	dc.MoveTo(rcValue.left + 25, nY);
	dc.LineTo(rcValue.right - 5, nY);

	pen.DeleteObject();

	return TRUE;
}

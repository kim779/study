#include "stdafx.h"
#include "GridCtrlExt.h"

#include <math.h>
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGridCtrlExt::CGridCtrlExt()
: CGridCtrl() 
{
	m_crArrow = RGB(0x90,0x90,0x90);
}

CGridCtrlExt::~CGridCtrlExt()
{
}


BEGIN_MESSAGE_MAP(CGridCtrlExt, CGridCtrl)
	//{{AFX_MSG_MAP(CGridCtrlExt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CGridCtrlExt::OnDraw( CDC* pDC )
{
	CGridCtrl::OnDraw(pDC);

	if (m_Arrows.empty()) return;

	CPoint poly[3];
	CPen pen(PS_SOLID, 1, m_crArrow);
	CBrush brush(m_crArrow);
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);

	const CCellID cid = GetTopleftNonFixedCell(FALSE);
	const int offset_y = (cid.row-1) * GRID_CELL_HEIGHT;

	vector<CArrow>::iterator st = m_Arrows.begin();
	const vector<CArrow>::iterator ed = m_Arrows.end();
	for(;st!=ed; ++st)
	{
		CPoint s_pt(st->m_st.x, st->m_st.y-offset_y);
		const CPoint e_pt(st->m_ed.x, st->m_ed.y-offset_y);

		for(int n=0; n<3; ++n)
		{
			poly[n].x = st->m_poly[n].x + e_pt.x;
			poly[n].y = st->m_poly[n].y + e_pt.y;
		}
		
		pDC->MoveTo(s_pt);
		pDC->LineTo(e_pt);
		s_pt.Offset(-ARROW_BOX_WIDTH/2, -ARROW_BOX_WIDTH/2);
		pDC->FillSolidRect(s_pt.x, s_pt.y, ARROW_BOX_WIDTH, ARROW_BOX_WIDTH, m_crArrow);
		pDC->Polygon(poly, 3);
	}
	pDC->SelectStockObject(NULL_PEN);
}

BOOL CGridCtrlExt::AddArrow( CArrow &ar )
{
	const vector<CArrow>::iterator pos = find(m_Arrows.begin(), m_Arrows.end(), ar);
	if (pos==m_Arrows.end())
	{
		double dx=0, dy=0, degree=0;

		// get degree
		// (90.0*3.14159265358979/180.0) => 1.570796326794895
		degree = atan2(ar.m_ed.x-ar.m_st.x, ar.m_st.y-ar.m_ed.y) - 1.570796326794895;

		// origine point {-3,-1}, {0,4}, {3,-1}
		dx = (-2.0 * cos(degree)) - ( 3.0 * sin(degree)); ar.m_poly[0].x = (LONG)((dx>0) ? dx+0.5 : dx-0.5);
		dy = (-2.0 * sin(degree)) + ( 3.0 * cos(degree)); ar.m_poly[0].y = (LONG)((dy>0) ? dy+0.5 : dy-0.5);
		dx = ( 4.0 * cos(degree)) - ( 0.0 * sin(degree)); ar.m_poly[1].x = (LONG)((dx>0) ? dx+0.5 : dx-0.5);
		dy = ( 4.0 * sin(degree)) + ( 0.0 * cos(degree)); ar.m_poly[1].y = (LONG)((dy>0) ? dy+0.5 : dy-0.5);
		dx = (-2.0 * cos(degree)) - (-3.0 * sin(degree)); ar.m_poly[2].x = (LONG)((dx>0) ? dx+0.5 : dx-0.5);
		dy = (-2.0 * sin(degree)) + (-3.0 * cos(degree)); ar.m_poly[2].y = (LONG)((dy>0) ? dy+0.5 : dy-0.5);
		
		m_Arrows.push_back(ar);
		
		DrawArrow(ar);

		return TRUE;
	}
	return FALSE;
}

BOOL CGridCtrlExt::DelArrow(const CArrow &ar )
{
	const vector<CArrow>::iterator pos = find(m_Arrows.begin(), m_Arrows.end(), ar);
	if (pos!=m_Arrows.end())
	{
		m_Arrows.erase(pos);
		return TRUE;
	}
	return FALSE;
}

BOOL CGridCtrlExt::GetAbsCellRect(const  CCellID &id , CRect &rc)
{
	if (!id.IsValid()) return FALSE;

	// Fixed Cell
	if (id.col<m_nFixedCols || id.row<m_nFixedRows)
	{
		GetCellRect(id, rc);
		return TRUE;
	}

	// 가상으로 계산한다.
	int n=0;
	//CCellID org = GetTopleftNonFixedCell(FALSE);
	const CCellID org( 0, 0 );

	rc.SetRect(0, 0, GetColumnWidth(org.col), GetRowHeight(org.row));
	if (id.col < org.col)
	{
		for(n=id.col; n<org.col; ++n)
		{
			rc.left -= GetColumnWidth(n);
			rc.right -= GetColumnWidth(n);
		}
	}
	else
	{
		for(n=org.col; n<id.col; ++n)
		{
			rc.left += GetColumnWidth(n);
			rc.right += GetColumnWidth(n);
		}	
	}
	if (id.row < org.row)
	{
		for(n=id.row; n<org.row; ++n)
		{
			rc.top -= GetRowHeight(n);
			rc.bottom -= GetRowHeight(n);
		}
	}
	else
	{
		for(n=org.row; n<id.row; ++n)
		{
			rc.top += GetRowHeight(n);
			rc.bottom += GetRowHeight(n);
		}
	}

	TRACE("x[%3d] y[%3d] left[%4d] top[%4d] right[%4d] bottom[%4d] width[%4d] height[%4d] SPos[%4d/%4d]\n", 
		id.col, id.row, rc.left, rc.top, rc.right, rc.bottom, rc.Width(), rc.Height(), GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));

	return TRUE;
}

void CGridCtrlExt::DrawArrow(const CArrow &ar, CDC *pDC/*=NULL*/ )
{
	const BOOL bMustRelease = (pDC==NULL);
	if (!pDC)
		pDC = GetDC();

	CPoint poly[3];
	CPen pen(PS_SOLID, 1, m_crArrow);
	CBrush brush(m_crArrow);
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);
	
	const CCellID cid = GetTopleftNonFixedCell(FALSE);
	const int offset_y = (cid.row-1) * GRID_CELL_HEIGHT;

	CPoint s_pt(ar.m_st.x, ar.m_st.y-offset_y);
	const CPoint e_pt(ar.m_ed.x, ar.m_ed.y-offset_y);
	
	for(int n=0; n<3; ++n)
	{
		poly[n].x = ar.m_poly[n].x + e_pt.x;
		poly[n].y = ar.m_poly[n].y + e_pt.y;
	}
	
	pDC->MoveTo(s_pt);
	pDC->LineTo(e_pt);
	s_pt.Offset(-ARROW_BOX_WIDTH/2, -ARROW_BOX_WIDTH/2);
	pDC->FillSolidRect(s_pt.x, s_pt.y, ARROW_BOX_WIDTH, ARROW_BOX_WIDTH, m_crArrow);
	pDC->Polygon(poly, 3);

	pDC->SelectStockObject(NULL_PEN);

	if (bMustRelease) ReleaseDC(pDC);
}

BOOL CGridCtrlExt::RedrawCell( int nRow, int nCol, CDC* pDC /*= NULL*/ )
{
	const BOOL bMustRelease = (pDC==NULL);
	if (!pDC) pDC = GetDC();

	const BOOL bRet = CGridCtrl::RedrawCell(nRow, nCol, pDC);

	if (m_Arrows.empty()) 
	{
		if (bMustRelease) ReleaseDC(pDC);
		return bRet;
	}

	CPoint poly[3];
	CPen pen(PS_SOLID, 1, m_crArrow);
	CBrush brush(m_crArrow);
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);
	
	const CCellID cid = GetTopleftNonFixedCell(FALSE);
	const int offset_y = (cid.row-1) * GRID_CELL_HEIGHT;
	
	vector<CArrow>::iterator st = m_Arrows.begin();
	const vector<CArrow>::iterator ed = m_Arrows.end();
	for(;st!=ed; ++st)
	{
		CPoint s_pt(st->m_st.x, st->m_st.y-offset_y);
		const CPoint e_pt(st->m_ed.x, st->m_ed.y-offset_y);
		
		for(int n=0; n<3; ++n)
		{
			poly[n].x = st->m_poly[n].x + e_pt.x;
			poly[n].y = st->m_poly[n].y + e_pt.y;
		}
		
		pDC->MoveTo(s_pt);
		pDC->LineTo(e_pt);
		s_pt.Offset(-ARROW_BOX_WIDTH/2, -ARROW_BOX_WIDTH/2);
		pDC->FillSolidRect(s_pt.x, s_pt.y, ARROW_BOX_WIDTH, ARROW_BOX_WIDTH, m_crArrow);
		pDC->Polygon(poly, 3);
	}
	pDC->SelectStockObject(NULL_PEN);

	if (bMustRelease) ReleaseDC(pDC);
	return bRet;
}

BOOL CGridCtrlExt::RedrawCell( const CCellID& cell, CDC* pDC /*= NULL*/ )
{
	return RedrawCell(cell.row, cell.col, pDC);
}

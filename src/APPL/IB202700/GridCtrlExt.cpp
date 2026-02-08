#include "stdafx.h"
#include "IB202700.h"
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

BOOL CGridCtrlExt::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CGridCtrlExt::OnDraw( CDC* pDC )
{
	CGridCtrl::OnDraw(pDC);

	//if (m_Arrows.empty()) return;
//	OutputDebugString("GRID CTRL ONDRAW\n");
// #ifdef _DEF_IB300600_
// 	if(m_arrAvr.m_st.x > -1)
// 	{
// 		DrawAverage(m_arrAvr,pDC);
// 	}
// #endif

	if (m_Arrows.empty())
	{
		//pDC->SelectStockObject(NULL_PEN);

		return;
	}

	CPoint poly[3];
	CPen pen(PS_SOLID, 1, m_crArrow);
	CBrush brush(m_crArrow);
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);

	const CCellID cid = GetTopleftNonFixedCell(FALSE);
	const int offset_y = (cid.row-1) * GRID_CELL_HEIGHT;

	for (const auto& st : m_Arrows)
	{
		if(st.m_bmarket)
		{
			CArrow ar;

			ar.m_st = st.m_st;
			ar.m_sid = st.m_sid;
			ar.m_eid.row = st.m_eid.row;
			ar.m_eid.col = st.m_eid.col;
			ar.m_ed = st.m_ed;
			ar.m_bmarket = st.m_bmarket;

			//DrawStar(ar);
			const CCellID cid = GetTopleftNonFixedCell(FALSE);
			const int offset_y = (cid.row-1) * GRID_CELL_HEIGHT;
			
			const CPoint s_pt(ar.m_st.x, ar.m_st.y-offset_y);
			
			CRect rc;
			rc.SetRect(0,0,10,10);
			
			if(ar.m_st.x > ar.m_ed.x)
			{
				rc.OffsetRect(s_pt.x,s_pt.y-5);	
			}
			else
			{
				rc.OffsetRect(s_pt.x-10,s_pt.y-5);
			}
			
			pDC->FillSolidRect(rc,m_crArrow);
		}
		else
		{
			CPoint s_pt(st.m_st.x, st.m_st.y-offset_y);
			const CPoint e_pt(st.m_ed.x, st.m_ed.y-offset_y);

			for(int n=0; n<3; ++n)
			{
				poly[n].x = st.m_poly[n].x + e_pt.x;
				poly[n].y = st.m_poly[n].y + e_pt.y;
			}
			
			pDC->MoveTo(s_pt);
			pDC->LineTo(e_pt);
			s_pt.Offset(-ARROW_BOX_WIDTH/2, -ARROW_BOX_WIDTH/2);
			pDC->FillSolidRect(s_pt.x, s_pt.y, ARROW_BOX_WIDTH, ARROW_BOX_WIDTH, m_crArrow);
			pDC->Polygon(poly, 3);
		}
	}
	pDC->SelectStockObject(NULL_PEN);
}

BOOL CGridCtrlExt::AddArrow( CArrow &ar )
{
	const vector<CArrow>::iterator pos = find(m_Arrows.begin(), m_Arrows.end(), ar);
	if (pos==m_Arrows.end())
	{
		double dx{}, dy{}, degree{};

		degree = atan2(ar.m_ed.x-ar.m_st.x, ar.m_st.y-ar.m_ed.y) - 1.570796326794895;

		// origine point {-3,-1}, {0,4}, {3,-1}
		dx = (-2.0 * cos(degree)) - ( 3.0 * sin(degree)); ar.m_poly[0].x = gsl::narrow_cast<LONG>((dx > 0) ? dx + 0.5 : dx - 0.5);
		dy = (-2.0 * sin(degree)) + ( 3.0 * cos(degree)); ar.m_poly[0].y = gsl::narrow_cast<LONG>((dy > 0) ? dy + 0.5 : dy - 0.5);
		dx = ( 4.0 * cos(degree)) - ( 0.0 * sin(degree)); ar.m_poly[1].x = gsl::narrow_cast<LONG>((dx > 0) ? dx + 0.5 : dx - 0.5);
		dy = ( 4.0 * sin(degree)) + ( 0.0 * cos(degree)); ar.m_poly[1].y = gsl::narrow_cast<LONG>((dy > 0) ? dy + 0.5 : dy - 0.5);
		dx = (-2.0 * cos(degree)) - (-3.0 * sin(degree)); ar.m_poly[2].x = gsl::narrow_cast<LONG>((dx > 0) ? dx + 0.5 : dx - 0.5);
		dy = (-2.0 * sin(degree)) + (-3.0 * cos(degree)); ar.m_poly[2].y = gsl::narrow_cast<LONG>((dy > 0) ? dy + 0.5 : dy - 0.5);
		
		m_Arrows.push_back(ar);

		
		if(ar.m_bmarket)
		{
			DrawStar(ar);
		}
		else
		{
			DrawArrow(ar);
		}

		return TRUE;
	}
	return FALSE;
}

void CGridCtrlExt::SetAverage(CArrow ar)
{
	double dx{}, dy{}, degree;
	
	degree = atan2(ar.m_ed.x-ar.m_st.x, ar.m_st.y-ar.m_ed.y) - 1.570796326794895;
	
	// origine point {-3,-1}, {0,4}, {3,-1}
	dx = (-2.0 * cos(degree)) - ( 3.0 * sin(degree)); ar.m_poly[0].x = gsl::narrow_cast<LONG>((dx > 0) ? dx + 0.5 : dx - 0.5);
	dy = (-2.0 * sin(degree)) + ( 3.0 * cos(degree)); ar.m_poly[0].y = gsl::narrow_cast<LONG>((dy > 0) ? dy + 0.5 : dy - 0.5);
	dx = ( 4.0 * cos(degree)) - ( 0.0 * sin(degree)); ar.m_poly[1].x = gsl::narrow_cast<LONG>((dx > 0) ? dx + 0.5 : dx - 0.5);
	dy = ( 4.0 * sin(degree)) + ( 0.0 * cos(degree)); ar.m_poly[1].y = gsl::narrow_cast<LONG>((dy > 0) ? dy + 0.5 : dy - 0.5);
	dx = (-2.0 * cos(degree)) - (-3.0 * sin(degree)); ar.m_poly[2].x = gsl::narrow_cast<LONG>((dx > 0) ? dx + 0.5 : dx - 0.5);
	dy = (-2.0 * sin(degree)) + (-3.0 * cos(degree)); ar.m_poly[2].y = gsl::narrow_cast<LONG>((dy > 0) ? dy + 0.5 : dy - 0.5);
	
	m_arrAvr = ar;
	
	DrawAverage(ar);
}

BOOL CGridCtrlExt::DelArrow(const CArrow &ar)
{
	const auto ft = find(m_Arrows.begin(), m_Arrows.end(), ar);
	if (ft != m_Arrows.end())
	{
		m_Arrows.erase(ft);
		return TRUE;
	}
	return FALSE;
}

BOOL CGridCtrlExt::GetAbsCellRect(const CCellID &id , CRect &rc)
{
	if (!id.IsValid()) return FALSE;

	CString s;
// 	s.Format("FIXED COLS : [%d]		FIXED ROWS : [%d] ID.COL : [%d]\n",m_nFixedCols,m_nFixedRows,id.col);
// 	OutputDebugString(s);
	// Fixed Cell
	if (id.col<m_nFixedCols || id.row<m_nFixedRows)
	{
		GetCellRect(id, rc);
		return TRUE;
	}

	// 가상으로 계산한다.
	int n{};
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

	//TRACE("x[%3d] y[%3d] left[%4d] top[%4d] right[%4d] bottom[%4d] width[%4d] height[%4d] SPos[%4d/%4d]\n", 
	//	id.col, id.row, rc.left, rc.top, rc.right, rc.bottom, rc.Width(), rc.Height(), GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));

	return TRUE;
}

void CGridCtrlExt::DrawArrow(const CArrow &ar, CDC *pDC/*=nullptr*/ )
{
	if(ar.m_bmarket)
		return;

	//OutputDebugString("DRAW ARROW\n");
	const BOOL bMustRelease = (pDC==nullptr);
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

void CGridCtrlExt::DrawStar(const CArrow &ar, CDC *pDC)
{
	const BOOL bMustRelease = (pDC==nullptr);
	if (!pDC)
		pDC = GetDC();
	
	CPoint poly[3]{};
	CPen pen(PS_SOLID, 1, m_crArrow);
	CBrush brush(m_crArrow);
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);
	
	const CCellID cid = GetTopleftNonFixedCell(FALSE);
	const int offset_y = (cid.row-1) * GRID_CELL_HEIGHT;
	
	const CPoint s_pt(ar.m_st.x, ar.m_st.y-offset_y);

	CRect rc;
	rc.SetRect(0,0,10,10);

	if(ar.m_st.x > ar.m_ed.x)
	{
		rc.OffsetRect(s_pt.x,s_pt.y-5);	
	}
	else
	{
		rc.OffsetRect(s_pt.x-10,s_pt.y-5);
	}

	pDC->FillSolidRect(rc,m_crArrow);
	pDC->SelectStockObject(NULL_PEN);

	if (bMustRelease) ReleaseDC(pDC);
}

void CGridCtrlExt::DrawAverage(const CArrow &ar, CDC *pDC)
{
	const BOOL bMustRelease = (pDC==nullptr);

	if(ar.m_st.x < 0 && ar.m_st.y < 0)
	{
		return;
	}

	if (!pDC)
		pDC = GetDC();
	
	CPoint poly[3]{};
	CPen pen(PS_SOLID, 1, m_crArrow);

	CRect rc;
	rc.SetRect(0,0,3,GRID_CELL_HEIGHT);

	COLORREF crTmp{};

	const CCellID cid = GetTopleftNonFixedCell(FALSE);
	const int offset_y = (cid.row-1) * GRID_CELL_HEIGHT;

// 	CString s;
// 	s.Format("ARROW M_ST.X : [%d]	M_ST.Y : [%d]\n",ar.m_st.x,ar.m_st.y);
// 	OutputDebugString(s);

	const CPoint s_pt(ar.m_st.x, ar.m_st.y-offset_y);
	
	if(ar.m_st.x > ar.m_ed.x)
	{
		rc.OffsetRect(s_pt.x,s_pt.y - 9);	
		crTmp = RGB(0,51,255);
	}
	else
	{
		rc.OffsetRect(s_pt.x,s_pt.y - 9);
		crTmp = RGB(255,0,0);
	}

	if(rc.top <= 0)
	{
		return;
	}

	CBrush brush(crTmp);
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);
	
	pDC->FillSolidRect(rc,crTmp);

	
	pDC->SelectStockObject(NULL_PEN);
	
	if (bMustRelease) ReleaseDC(pDC);
}

void CGridCtrlExt::ClearAverage()
{
	m_arrAvr.m_st.x = -1;
	m_arrAvr.m_st.y = -1;
}

BOOL CGridCtrlExt::RedrawCell( int nRow, int nCol, CDC* pDC /*= nullptr*/ )
{
	const BOOL bMustRelease = (pDC==nullptr);
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
		
	for (const auto& st : m_Arrows)
	{
		if(st.m_bmarket)
		{
			CArrow ar;
			
			ar.m_st = st.m_st;
			ar.m_sid = st.m_sid;
			ar.m_eid.row = st.m_eid.row;
			ar.m_eid.col = st.m_eid.col;
			ar.m_ed = ar.m_eid.col = st.m_eid.col;
			ar.m_bmarket = st.m_bmarket;
			
			DrawStar(ar);
		}
		else
		{
			CPoint s_pt(st.m_st.x, st.m_st.y-offset_y);
			const CPoint e_pt(st.m_ed.x, st.m_ed.y-offset_y);
			
			for(int n=0; n<3; ++n)
			{
				poly[n].x = st.m_poly[n].x + e_pt.x;
				poly[n].y = st.m_poly[n].y + e_pt.y;
			}
			
			pDC->MoveTo(s_pt);
			pDC->LineTo(e_pt);
			s_pt.Offset(-ARROW_BOX_WIDTH/2, -ARROW_BOX_WIDTH/2);
			pDC->FillSolidRect(s_pt.x, s_pt.y, ARROW_BOX_WIDTH, ARROW_BOX_WIDTH, m_crArrow);
			pDC->Polygon(poly, 3);
		}
	}
	pDC->SelectStockObject(NULL_PEN);

	if (bMustRelease) ReleaseDC(pDC);
	return bRet;
}

BOOL CGridCtrlExt::RedrawCell( const CCellID& cell, CDC* pDC /*= nullptr*/ )
{
	return RedrawCell(cell.row, cell.col, pDC);
}

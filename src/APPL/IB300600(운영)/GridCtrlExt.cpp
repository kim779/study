#include "stdafx.h"
#include "IB300600.h"
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
	m_iRowHeight = GRID_CELL_HEIGHT;
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

	CCellID cid = GetTopleftNonFixedCell(FALSE);
	int offset_y = (cid.row-1) * m_iRowHeight;  //test height  GRID_CELL_HEIGHT

	vector<CArrow>::iterator st = m_Arrows.begin();
	vector<CArrow>::iterator ed = m_Arrows.end();

	for(;st!=ed; ++st)
	{
		if(st->m_bmarket)
		{
			CArrow ar;

			ar.m_st = st->m_st;
			ar.m_sid = st->m_sid;
			ar.m_eid.row = st->m_eid.row;
			ar.m_eid.col = st->m_eid.col;
			ar.m_ed = st->m_ed;
			ar.m_bmarket = st->m_bmarket;

			//DrawStar(ar);
			CCellID cid = GetTopleftNonFixedCell(FALSE);
			int offset_y = (cid.row-1) * m_iRowHeight;  //test height
			
			CPoint s_pt(ar.m_st.x, ar.m_st.y-offset_y);
			
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
			CPoint s_pt(st->m_st.x, st->m_st.y-offset_y);
			CPoint e_pt(st->m_ed.x, st->m_ed.y-offset_y);

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
	}

	pDC->SelectStockObject(NULL_PEN);
}

BOOL CGridCtrlExt::AddArrow( CArrow &ar )
{
// 	CString s;
// 	OutputDebugString("GRIDCTRL ADDARROW\n");
	vector<CArrow>::iterator pos = find(m_Arrows.begin(), m_Arrows.end(), ar);
	if (pos==m_Arrows.end())
	{
		CRect src, erc;
		double dx, dy, degree;

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

// 		s.Format("MARKET FLAG : [%d]\n",ar.m_bmarket);
// 		OutputDebugString(s);
		
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
	CRect src, erc;
	double dx, dy, degree;
	
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
	
	m_arrAvr = ar;
	
	DrawAverage(ar);
}

BOOL CGridCtrlExt::DelArrow( CArrow &ar )
{
	//OutputDebugString("GRIDEXT DELARROW\n");
	vector<CArrow>::iterator pos = find(m_Arrows.begin(), m_Arrows.end(), ar);
	CString s;
// 	s.Format("1.m_st : [%d]	m_sid : [%d]\n",111ar.m_st,ar.m_sid);
// 	OutputDebugString(s);

// 	s.Format("2.m_st : [%d]	m_sid : [%d]\n",pos->m_st,pos->m_sid);
// 	OutputDebugString(s);
	if (pos!=m_Arrows.end())
	{
		//OutputDebugString("GRIDEXT REAL DELARROW\n");
		m_Arrows.erase(pos);
		return TRUE;
	}

// 	s.Format("3.m_st x : [%d]	m_st y : [%d]	m_sid row : [%d]\n",ar.m_st.x,ar.m_st.y,ar.m_sid.row);
// 	OutputDebugString(s);
	vector<CArrow>::iterator st = m_Arrows.begin();
	vector<CArrow>::iterator ed = m_Arrows.end();
	for(;st!=ed; ++st)
	{
// 		s.Format("4.m_st x : [%d]	m_st y : [%d]	m_sid row : [%d]\n",st->m_st.x,st->m_st.y,st->m_sid.row);
// 		OutputDebugString(s);
		if(st->m_sid.row == ar.m_sid.row && st->m_sid.col == ar.m_sid.col)
		{
			//OutputDebugString("GRIDEXT REAL ERASEW\n");
			m_Arrows.erase(st);

			Invalidate();

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CGridCtrlExt::GetAbsCellRect( CCellID &id , CRect &rc)
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
	int n;
	//CCellID org = GetTopleftNonFixedCell(FALSE);
	CCellID org( 0, 0 );

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

void CGridCtrlExt::DrawArrow( CArrow &ar, CDC *pDC/*=NULL*/ )
{
	if(ar.m_bmarket)
		return;

	//OutputDebugString("DRAW ARROW\n");
	BOOL bMustRelease = (pDC==NULL);
	if (!pDC)
		pDC = GetDC();

	CPoint poly[3];
	CPen pen(PS_SOLID, 1, m_crArrow);
	CBrush brush(m_crArrow);
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);
	
	CCellID cid = GetTopleftNonFixedCell(FALSE);
	int offset_y = (cid.row-1) * m_iRowHeight;  //test height

	CPoint s_pt(ar.m_st.x, ar.m_st.y-offset_y);
	CPoint e_pt(ar.m_ed.x, ar.m_ed.y-offset_y);
	
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

void CGridCtrlExt::DrawStar(CArrow &ar, CDC *pDC)
{
	BOOL bMustRelease = (pDC==NULL);
	if (!pDC)
		pDC = GetDC();
	
	CPoint poly[3];
	CPen pen(PS_SOLID, 1, m_crArrow);
	CBrush brush(m_crArrow);
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);
	
	CCellID cid = GetTopleftNonFixedCell(FALSE);
	int offset_y = (cid.row-1) * m_iRowHeight;  //test height
	
	CPoint s_pt(ar.m_st.x, ar.m_st.y-offset_y);

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

// 	s_pt.Offset(ARROW_BOX_WIDTH, -ARROW_BOX_WIDTH/2);
// 	
// 	// 다각형의모양을가지는점들의집합을준비합니다.
// 	
// 	// 여기는별만그리지만삼각형도똑같습니다.
// 	
// 	// 그리는모양은마음대로바꾸세요.
// 	
// 	CPoint points[11];
// 	
// 	// 72' 씩회전된꼭지점으로이루어진점생성
// 	
// 	// 여기서알아서모양을만드세요.
// 	
// 	double Pi = 3.14159265359;
// 	
// 	for ( int i = 0; i < 10; i += 2 )	
// 	{	
// 		// 별의바깥쪽꼭지점(큰반지름으로만듬)
// 		
// 		points[i].x = (long)(8*cos((double)(i*72*Pi)/360.0));	
// 		points[i].y = (long)(8*sin((double)(i*72*Pi)/360.0));	
// 		
// 		// 별의안쪽꼭지점(작은반지름으로만듬)
// 		
// 		points[i+1].x = (long)(4*cos((double)((i+1)*72*Pi)/360.0));	
// 		points[i+1].y = (long)(4*sin((double)((i+1)*72*Pi)/360.0));      	
// 	}
// 	
// 	// 끝점은첫점과같게맞춤니다.
// 	
// 	points[10] = points[0];
// 	
// 	CRgn rgnStar;
// 	
// 	rgnStar.CreatePolygonRgn(points, 11, WINDING);
// 	
// 	// 클라이언트영역을가져옵니다.
// 	
// 	CRect r;
// 	
// 	GetClientRect(&r);
// 	
// 	// 그림그릴브러시를초기화합니다.
// 	
// 	//brush.CreateSolidBrush(RGB(0,0,0));
// 	
// 	// 영역을초기화합니다.
// 	
// 	CRgn rgn;
// 	
// 	rgn.CreateRectRgn(0, 0, 0, 0);
// 	rgn.CopyRgn(&rgnStar);
// 	rgn.OffsetRgn(s_pt.x, s_pt.y);
// 	
// 	// 별을그립니다.
// 	
// 	pDC->FillRgn(&rgn, &brush);

	pDC->SelectStockObject(NULL_PEN);

	if (bMustRelease) ReleaseDC(pDC);
}

void CGridCtrlExt::DrawAverage(CArrow &ar, CDC *pDC)
{
	BOOL bMustRelease = (pDC==NULL);

	if(ar.m_st.x < 0 && ar.m_st.y < 0)
	{
		return;
	}

	if (!pDC)
		pDC = GetDC();
	
	CPoint poly[3];
	CPen pen(PS_SOLID, 1, m_crArrow);

	CRect rc;
	rc.SetRect(0,0,3,m_iRowHeight);  //test height

	COLORREF crTmp;

	CCellID cid = GetTopleftNonFixedCell(FALSE);
	int offset_y = (cid.row-1) * m_iRowHeight;  //test height

// 	CString s;
// 	s.Format("ARROW M_ST.X : [%d]	M_ST.Y : [%d]\n",ar.m_st.x,ar.m_st.y);
// 	OutputDebugString(s);

	CPoint s_pt(ar.m_st.x, ar.m_st.y-offset_y);
	
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

// 	s.Format("DRAW AVR : [%d] [%d] [%d] [%d]\n",rc.top,rc.left,rc.bottom,rc.right);
// 	OutputDebugString(s);
	
	pDC->FillSolidRect(rc,crTmp);
	
	// 	s_pt.Offset(ARROW_BOX_WIDTH, -ARROW_BOX_WIDTH/2);
	// 	
	// 	// 다각형의모양을가지는점들의집합을준비합니다.
	// 	
	// 	// 여기는별만그리지만삼각형도똑같습니다.
	// 	
	// 	// 그리는모양은마음대로바꾸세요.
	// 	
	// 	CPoint points[11];
	// 	
	// 	// 72' 씩회전된꼭지점으로이루어진점생성
	// 	
	// 	// 여기서알아서모양을만드세요.
	// 	
	// 	double Pi = 3.14159265359;
	// 	
	// 	for ( int i = 0; i < 10; i += 2 )	
	// 	{	
	// 		// 별의바깥쪽꼭지점(큰반지름으로만듬)
	// 		
	// 		points[i].x = (long)(8*cos((double)(i*72*Pi)/360.0));	
	// 		points[i].y = (long)(8*sin((double)(i*72*Pi)/360.0));	
	// 		
	// 		// 별의안쪽꼭지점(작은반지름으로만듬)
	// 		
	// 		points[i+1].x = (long)(4*cos((double)((i+1)*72*Pi)/360.0));	
	// 		points[i+1].y = (long)(4*sin((double)((i+1)*72*Pi)/360.0));      	
	// 	}
	// 	
	// 	// 끝점은첫점과같게맞춤니다.
	// 	
	// 	points[10] = points[0];
	// 	
	// 	CRgn rgnStar;
	// 	
	// 	rgnStar.CreatePolygonRgn(points, 11, WINDING);
	// 	
	// 	// 클라이언트영역을가져옵니다.
	// 	
	// 	CRect r;
	// 	
	// 	GetClientRect(&r);
	// 	
	// 	// 그림그릴브러시를초기화합니다.
	// 	
	// 	//brush.CreateSolidBrush(RGB(0,0,0));
	// 	
	// 	// 영역을초기화합니다.
	// 	
	// 	CRgn rgn;
	// 	
	// 	rgn.CreateRectRgn(0, 0, 0, 0);
	// 	rgn.CopyRgn(&rgnStar);
	// 	rgn.OffsetRgn(s_pt.x, s_pt.y);
	// 	
	// 	// 별을그립니다.
	// 	
	// 	pDC->FillRgn(&rgn, &brush);
	
	pDC->SelectStockObject(NULL_PEN);
	
	if (bMustRelease) ReleaseDC(pDC);
}

void CGridCtrlExt::ClearAverage()
{
	m_arrAvr.m_st.x = -1;
	m_arrAvr.m_st.y = -1;
}

BOOL CGridCtrlExt::RedrawCell( int nRow, int nCol, CDC* pDC /*= NULL*/ )
{
	BOOL bMustRelease = (pDC==NULL);
	if (!pDC) pDC = GetDC();
	
	BOOL bRet = CGridCtrl::RedrawCell(nRow, nCol, pDC);

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
	
	CCellID cid = GetTopleftNonFixedCell(FALSE);
	int offset_y = (cid.row-1) * m_iRowHeight;  //test height
	//OutputDebugString("REDRAWCELL\n");
// #ifdef _DEF_IB300600_
// 	DrawAverage(m_arrAvr);
// #endif
	
	vector<CArrow>::iterator st = m_Arrows.begin();
	vector<CArrow>::iterator ed = m_Arrows.end();
	for(;st!=ed; ++st)
	{
		if(st->m_bmarket)
		{
			CArrow ar;
			
			ar.m_st = st->m_st;
			ar.m_sid = st->m_sid;
			ar.m_eid.row = st->m_eid.row;
			ar.m_eid.col = st->m_eid.col;
			ar.m_ed = st->m_ed;
			ar.m_bmarket = st->m_bmarket;
			
			DrawStar(ar);
		}
		else
		{
			CPoint s_pt(st->m_st.x, st->m_st.y-offset_y);
			CPoint e_pt(st->m_ed.x, st->m_ed.y-offset_y);
			
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
	}
	pDC->SelectStockObject(NULL_PEN);

	if (bMustRelease) ReleaseDC(pDC);
	return bRet;
}

BOOL CGridCtrlExt::RedrawCell( const CCellID& cell, CDC* pDC /*= NULL*/ )
{
	return RedrawCell(cell.row, cell.col, pDC);
}

void CGridCtrlExt::DelAllArrow()
{
	vector<CArrow>::iterator st = m_Arrows.begin();
	vector<CArrow>::iterator ed = m_Arrows.end();

	int isize = m_Arrows.size();

	m_Arrows.clear();
	isize = m_Arrows.size();

	Invalidate();	
}
// GannFan.cpp: implementation of the CGannFan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GannFan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

double	gannFAN[] = {8.0, 4.0, 3.0, 2.0};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGannFan::CGannFan(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{
	m_basePOINT.x = m_basePOINT.y = 0;
	m_id = tkname;
}

CGannFan::~CGannFan()
{

}

void CGannFan::DrawTool(CDC* pDC)
{
	if (m_pointStart.x == m_pointEnd.x && m_pointStart.y == m_pointEnd.y)
		return;

	CPen	*cPen, *yellowPen;
	CPoint sp, ep;
	if (!m_bMove)
	{
		m_pIndcBase->GetValueToPoint(sp, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetValueToPoint(ep, m_dPosEnd, m_dValueEnd);
		if(m_pIndcBase->IsNoTime())//kangmc추세선변경
		{
			sp.x += m_iSXGab;
			ep.x += m_iEXGab;
		}
		m_pointStart = sp;
		m_pointEnd = ep;
		
		cPen = GetPen(m_pwndView, m_btLStyle, m_btLWidth, m_rgbText);
		yellowPen = GetPen(m_pwndView, PS_SOLID, 1, _YELLOW ^ m_rgbBG);
	}
	else
	{
		sp = m_pointStart;
		ep = m_pointEnd;
		m_pIndcBase->GetPtToPosValue(sp, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetPtToPosValue(ep, m_dPosEnd, m_dValueEnd);
		if(m_pIndcBase->IsNoTime())//kangmc추세선변경
		{
			m_pIndcBase->GetValueToPoint(sp, m_dPosStart, m_dValueStart);
			m_pIndcBase->GetValueToPoint(ep, m_dPosEnd, m_dValueEnd);
			m_iSXGab = m_pointStart.x - sp.x;
			m_iEXGab = m_pointEnd.x - ep.x;
		}
//TRACE("===>>>S[%d:%d] [%.f][%.f]  E[%d:%d] [%.f][%.f]\n",
//	sp.x, sp.y,	m_dPosStart, m_dValueStart, 
//	ep.x, ep.y,	m_dPosEnd, m_dValueEnd);
		
		cPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
		yellowPen = GetPen(m_pwndView, PS_DOT, 1, _YELLOW);

	}

	if (m_bClipRegion)
		pDC->IntersectClipRect(&m_rectGrp);

	CPen	*sPen = pDC->SelectObject(yellowPen);
	int	sROP = pDC->SetROP2(R2_XORPEN);
	int	bkMode = pDC->SetBkMode(TRANSPARENT);

	if (CalculateDrawingPosition())
	{
		pDC->MoveTo(m_pointStart);
		pDC->LineTo(m_gannEXT);

		pDC->SelectObject(cPen);
		for (int ii = 0; ii < maxGANNFAN; ii++)
		{
			pDC->MoveTo(m_pointStart);
			pDC->LineTo(m_XgannEXT[ii]);

			pDC->MoveTo(m_pointStart);
			pDC->LineTo(m_YgannEXT[ii]);
		}
	}
	pDC->SetROP2(sROP);
	pDC->SelectObject(sPen);
	pDC->SetBkMode(bkMode);

	if (m_bClipRegion)	pDC->SelectClipRgn(NULL);
	
	CToolBase::InflateHolder();
	if (m_bShowHolder)
		CToolBase::DrawHolder(pDC, true);
}

void CGannFan::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CGannFan::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointStart = pt;
	m_pointEnd = pt;

	int Width = m_rectGrp.Width() / 10;
	if (Width > 10)	Width = 10;

	if (m_id == (int) enumTK::tkGANNFAN)
		m_pointEnd.Offset(Width, -Width);
	else
		m_pointEnd.Offset(Width, Width);

	bMouseCapture = TRUE;
	m_bMove = TRUE;
	m_bClipRegion = FALSE;

	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);

	CToolBase::InflateHolder();
}

void CGannFan::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);

	m_pointStart = pt;
	m_pointEnd = pt;
	int Width = m_rectGrp.Width() / 10;
	if (Width > 10)	Width = 10;

	if (m_id == (int) enumTK::tkGANNFAN)
		m_pointEnd.Offset(Width, -Width);
	else
		m_pointEnd.Offset(Width, Width);

	CToolBase::InflateHolder();
	DrawTool(&ClientDC);
}

void CGannFan::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	CToolBase::DrawHolder(&ClientDC, true);
	DrawTool(&ClientDC);

	bMouseCapture = FALSE;

	m_bMove = FALSE;
	m_bClipRegion = TRUE;

	DrawTool(&ClientDC);
}

void CGannFan::ChangeToolLBDN(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	CToolBase::DrawHolder(&ClientDC, false);

	DrawTool(&ClientDC);
	m_bClipRegion = FALSE;
	DrawTool(&ClientDC);

	if (m_rectDownHolder.PtInRect(pt))
	{
		m_eChgUpDn = enumUD::chgDN;
		m_pointStart = pt;
	}
	else if (m_rectUpHolder.PtInRect(pt))
	{
		m_eChgUpDn = enumUD::chgUP;
		m_pointEnd = pt;
	}

	m_bMove = TRUE;
	DrawTool(&ClientDC);

	::SetCapture(m_pwndParent->m_hWnd);
	bMouseCapture = TRUE;

	CToolBase::InflateHolder();
}

void CGannFan::ChangeToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	if (m_bShowHolder)
		CToolBase::DrawHolder(&ClientDC, false);

	DrawTool(&ClientDC);
	switch (m_eChgUpDn)
	{
	case enumUD::chgDN:
		m_pointStart = pt;
		break;
	case enumUD::chgUP:
		m_pointEnd = pt;
		break;
	case enumUD::chgNO:
	default:
		return;
	}
	DrawTool(&ClientDC);
	CToolBase::InflateHolder();
}

void CGannFan::ChangeToolLBUP(CPoint pt, bool &bMouseCapture)
{
	MoveToolLBUP(pt, bMouseCapture);
}

void CGannFan::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointCur = pt;

	::SetCapture(m_pwndParent->m_hWnd);
	bMouseCapture = TRUE;

	CClientDC ClientDC(m_pwndParent);
	if (!m_bShowHolder)
		CToolBase::DrawHolder(&ClientDC, true);
	m_bShowHolder = false;
	DrawTool(&ClientDC);
	m_bClipRegion = FALSE;
	DrawTool(&ClientDC);

	m_bMove = TRUE;
	DrawTool(&ClientDC);
	m_bShowHolder = true;
}

void CGannFan::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		CToolBase::DrawHolder(&ClientDC, false);
	DrawTool(&ClientDC);

	m_pointStart.x += pt.x - m_pointCur.x;
	m_pointStart.y += pt.y - m_pointCur.y;
	m_pointEnd.x += pt.x - m_pointCur.x;
	m_pointEnd.y += pt.y - m_pointCur.y;
	DrawTool(&ClientDC);

	m_pointCur = pt;
	CToolBase::InflateHolder();
}

void CGannFan::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();
}

bool CGannFan::PtInToolRegion(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	return false;

	const	int arrayCOUNT = maxGANNFAN * 2 +1;
	int	aIDX = 0;

	SetPointArray(aIDX++, m_pointStart, m_gannEXT);
	for (int ii = 0; ii < maxGANNFAN; ii++)
	{
		SetPointArray(aIDX++, m_pointStart, m_XgannEXT[ii]);
		SetPointArray(aIDX++, m_pointStart, m_YgannEXT[ii]);
	}

	int count[arrayCOUNT]{ 0, };
	for (int ii = 0; ii < arrayCOUNT; ii++)
		count[ii] = 5;

	CRgn FiboRGN;
	if (!FiboRGN.CreatePolyPolygonRgn(&m_RgnPoint[0][0], count, arrayCOUNT, WINDING))
		return false;

	BOOL bInRgn = FiboRGN.PtInRegion(pt); 		
	FiboRGN.DeleteObject();	

	return (bInRgn ? true : false);
}

/////////////////////////////////////////////////////////////////////////////
// private member function

bool CGannFan::CalculateDrawingPosition()
{
	enum whichAREA {noneAREA = 0, firstAREA, secondAREA, thirdAREA, fourthAREA};

	whichAREA which = noneAREA;
	if (m_pointStart.x <= m_pointEnd.x)
	{
		if (m_pointStart.y > m_pointEnd.y)
		{
			m_basePOINT.x = m_rectChart.right;
			m_basePOINT.y = m_rectChart.top;
			which = firstAREA;
		}
		else if (m_pointStart.y < m_pointEnd.y) 
		{
			m_basePOINT.x = m_rectChart.right;
			m_basePOINT.y = m_rectChart.bottom;
			which = fourthAREA;
		}
	}
	else
	{
		if (m_pointStart.y > m_pointEnd.y)
		{
			m_basePOINT.x = m_rectChart.left;
			m_basePOINT.y = m_rectChart.top;
			which = secondAREA;
		}
		else if (m_pointStart.y < m_pointEnd.y)
		{
			m_basePOINT.x = m_rectChart.left;
			m_basePOINT.y = m_rectChart.bottom;
			which = thirdAREA;
		}
	}

	if (which == noneAREA)
		return false;

	int	gannWidth = abs(m_pointStart.x - m_pointEnd.x);
	int	gannHeight = abs(m_pointStart.y - m_pointEnd.y);
	if (gannWidth < 1)	gannWidth = 1;
	double	gannRATE = double(double(gannHeight) / double(gannWidth));

	if (gannRATE == 0)	gannRATE = 1;
	for (int ii = 0; ii < maxGANNFAN; ii++)
	{
		switch (which)
		{
		case firstAREA:
			// 세로축
			m_Ygann[ii].x = m_pointEnd.x;
			m_Ygann[ii].y = m_pointStart.y - int((gannWidth * gannRATE) / gannFAN[ii]);
			// 가로축
			m_Xgann[ii].x = m_pointStart.x + int(gannHeight / (gannRATE * gannFAN[ii]));
			m_Xgann[ii].y = m_pointEnd.y;
			break;
		case secondAREA:
			// 세로축
			m_Ygann[ii].x = m_pointEnd.x;
			m_Ygann[ii].y = m_pointStart.y - int((gannWidth * gannRATE) / gannFAN[ii]);
			// 가로축
			m_Xgann[ii].x = m_pointStart.x - int(gannHeight / (gannRATE * gannFAN[ii]));
			m_Xgann[ii].y = m_pointEnd.y;
			break;
		case thirdAREA:
			// 세로축
			m_Ygann[ii].x = m_pointEnd.x;
			m_Ygann[ii].y = m_pointStart.y + int((gannWidth * gannRATE) / gannFAN[ii]);
			// 가로축
			m_Xgann[ii].x = m_pointStart.x - int(gannHeight / (gannRATE * gannFAN[ii]));
			m_Xgann[ii].y = m_pointEnd.y;
			break;
		case fourthAREA:
			// 세로축
			m_Ygann[ii].x = m_pointEnd.x;
			m_Ygann[ii].y = m_pointStart.y + int((gannWidth * gannRATE) / gannFAN[ii]);
			// 가로축
			m_Xgann[ii].x = m_pointStart.x + int(gannHeight / (gannRATE * gannFAN[ii]));
			m_Xgann[ii].y = m_pointEnd.y;
			break;
		}
	}

	int Width  = abs(m_basePOINT.x - m_pointStart.x);
	int Height = abs(m_basePOINT.y - m_pointStart.y);
	if (Width < 1)	Width = 1;
	double baseRATE = double(Height) / double(Width);

	switch (which)				// gann line ext
	{
	case firstAREA:
		if (gannRATE > baseRATE)
		{
			Height = abs(m_pointStart.y - m_rectChart.top);
			m_gannEXT.x = m_pointStart.x + int(Height / gannRATE);
			m_gannEXT.y = m_rectChart.top;
		}
		else
		{
			Width = abs(m_pointStart.x - m_rectChart.right);
			m_gannEXT.x = m_rectChart.right;
			m_gannEXT.y = m_pointStart.y - int(Width * gannRATE);
		}
		break;
	case secondAREA:
		if (gannRATE > baseRATE)
		{
			Height = abs(m_pointStart.y - m_rectChart.top);
			m_gannEXT.x = m_pointStart.x - int(Height / gannRATE);
			m_gannEXT.y = m_rectChart.top;
		}
		else
		{
			Width = abs(m_pointStart.x - m_rectChart.left);
			m_gannEXT.x = m_rectChart.left;
			m_gannEXT.y = m_pointStart.y - int(Width * gannRATE);

		}
		break;
	case thirdAREA:
		if (gannRATE > baseRATE)
		{
			Height = abs(m_pointStart.y - m_rectChart.bottom);
			m_gannEXT.x = m_pointStart.x - int(Height / gannRATE);
			m_gannEXT.y = m_rectChart.bottom;
		}
		else
		{
			Width = abs(m_pointStart.x - m_rectChart.left);
			m_gannEXT.x = m_rectChart.left;
			m_gannEXT.y = m_pointStart.y + int(Width * gannRATE);

		}
		break;
	case fourthAREA:
		if (gannRATE > baseRATE)
		{
			Height = abs(m_pointStart.y - m_rectChart.bottom);
			m_gannEXT.x = m_pointStart.x + int(Height / gannRATE);
			m_gannEXT.y = m_rectChart.bottom;
		}
		else
		{
			Width = abs(m_pointStart.x - m_rectChart.right);
			m_gannEXT.x = m_rectChart.right;
			m_gannEXT.y = m_pointStart.y + int(Width * gannRATE);
		}
		break;
	}

	double xChkRATE, yChkRATE;
	for (int ii = 0; ii < maxGANNFAN; ii++)
	{
		Width  = abs(m_pointStart.x - m_Xgann[ii].x);
		Height = abs(m_pointStart.y - m_Xgann[ii].y);
		if (Width < 1)	Width = 1;
		xChkRATE = double(Height) / double(Width);

		Width  = abs(m_pointStart.x - m_Ygann[ii].x);
		Height = abs(m_pointStart.y - m_Ygann[ii].y);
		if (Width < 1)	Width = 1;
		yChkRATE = double(Height) / double(Width);

		if (yChkRATE == 0)	yChkRATE = 1;
		switch (which)
		{
		case firstAREA:
			if (xChkRATE > baseRATE)
			{
				Height = abs(m_pointStart.y - m_rectChart.top);
				m_XgannEXT[ii].x = m_pointStart.x + int(Height / xChkRATE);
				m_XgannEXT[ii].y = m_rectChart.top;
			}
			else
			{
				Width = abs(m_pointStart.x - m_rectChart.right);
				m_XgannEXT[ii].x = m_rectChart.right;
				m_XgannEXT[ii].y = m_pointStart.y - int(Width * xChkRATE);
			}
			if (yChkRATE <= baseRATE)
			{
				Width = abs(m_pointStart.x - m_rectChart.right);
				m_YgannEXT[ii].x = m_rectChart.right;
				m_YgannEXT[ii].y = m_pointStart.y - int(Width * yChkRATE);
			}
			else
			{
				Height = abs(m_pointStart.y - m_rectChart.top);
				m_YgannEXT[ii].x = m_pointStart.x + int(Height / yChkRATE);
				m_YgannEXT[ii].y = m_rectChart.top;
			}
			break;
		case secondAREA:
			if (xChkRATE > baseRATE)
			{
				Height = abs(m_pointStart.y - m_rectChart.top);
				m_XgannEXT[ii].x = m_pointStart.x - int(Height / xChkRATE);
				m_XgannEXT[ii].y = m_rectChart.top;
			}
			else
			{
				Width = abs(m_pointStart.x - m_rectChart.left);
				m_XgannEXT[ii].x = m_rectChart.left;
				m_XgannEXT[ii].y = m_pointStart.y - int(Width * xChkRATE);

			}
			if (yChkRATE <= baseRATE)
			{
				Width = abs(m_pointStart.x - m_rectChart.left);
				m_YgannEXT[ii].x = m_rectChart.left;
				m_YgannEXT[ii].y = m_pointStart.y - int(Width * yChkRATE);
			}
			else
			{
				Height = abs(m_pointStart.y - m_rectChart.top);
				m_YgannEXT[ii].x = m_pointStart.x -  int(Height / yChkRATE);
				m_YgannEXT[ii].y = m_rectChart.top;
			}
			break;
		case thirdAREA:
			if (xChkRATE > baseRATE)
			{
				Height = abs(m_pointStart.y - m_rectChart.bottom);
				m_XgannEXT[ii].x = m_pointStart.x - int(Height / xChkRATE);
				m_XgannEXT[ii].y = m_rectChart.bottom;
			}
			else
			{
				Width = abs(m_pointStart.x - m_rectChart.left);
				m_XgannEXT[ii].x = m_rectChart.left;
				m_XgannEXT[ii].y = m_pointStart.y + int(Width * xChkRATE);

			}
			if (yChkRATE > baseRATE)
			{
				Height = abs(m_pointStart.y - m_rectChart.bottom);
				m_YgannEXT[ii].x = m_pointStart.x -  int(Height / yChkRATE);
				m_YgannEXT[ii].y = m_rectChart.bottom;
			}
			else
			{
				Width = abs(m_pointStart.x - m_rectChart.left);
				m_YgannEXT[ii].x = m_rectChart.left;
				m_YgannEXT[ii].y = m_pointStart.y + int(Width * yChkRATE);
			}
			break;
		case fourthAREA:
			if (xChkRATE > baseRATE)
			{
				Height = abs(m_pointStart.y - m_rectChart.bottom);
				m_XgannEXT[ii].x = m_pointStart.x + int(Height / xChkRATE);
				m_XgannEXT[ii].y = m_rectChart.bottom;
			}
			else
			{
				Width = abs(m_pointStart.x - m_rectChart.right);
				m_XgannEXT[ii].x = m_rectChart.right;
				m_XgannEXT[ii].y = m_pointStart.y + int(Width * xChkRATE);
			}

			if (yChkRATE > baseRATE)
			{
				Height = abs(m_pointStart.y - m_rectChart.bottom);
				m_YgannEXT[ii].x = m_pointStart.x + int(Height / yChkRATE);
				m_YgannEXT[ii].y = m_rectChart.bottom;
			}
			else
			{
				Width = abs(m_pointStart.x - m_rectChart.right);
				m_YgannEXT[ii].x = m_rectChart.right;
				m_YgannEXT[ii].y = m_pointStart.y + int(Width * yChkRATE);
			}
			break;
		}
	}

	return true;
}

void CGannFan::SetPointArray(int idx, CPoint pt1, CPoint pt2)
{
	if (pt1.x == pt2.x)
	{
		m_RgnPoint[idx][0].x = pt1.x - szREGION;
		m_RgnPoint[idx][0].y = pt1.y;
		m_RgnPoint[idx][1].x = pt1.x + szREGION;
		m_RgnPoint[idx][1].y = pt1.y;
		m_RgnPoint[idx][2].x = pt2.x + szREGION;
		m_RgnPoint[idx][2].y = pt2.y;
		m_RgnPoint[idx][3].x = pt2.x - szREGION;
		m_RgnPoint[idx][3].y = pt2.y;
		m_RgnPoint[idx][4].x = pt1.x - szREGION;
		m_RgnPoint[idx][4].y = pt1.y;
	}
	else
	{
		m_RgnPoint[idx][0].x = pt1.x;
		m_RgnPoint[idx][0].y = pt1.y - szREGION;
		m_RgnPoint[idx][1].x = pt2.x;
		m_RgnPoint[idx][1].y = pt2.y - szREGION;
		m_RgnPoint[idx][2].x = pt2.x;
		m_RgnPoint[idx][2].y = pt2.y + szREGION;
		m_RgnPoint[idx][3].x = pt1.x;
		m_RgnPoint[idx][3].y = pt1.y + szREGION;
		m_RgnPoint[idx][4].x = pt1.x;
		m_RgnPoint[idx][4].y = pt1.y - szREGION;
	}
}

// FiboFan2.cpp: implementation of the CFiboFan2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FiboFan2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
double	fiboFAN2[] = {23.6, 38.2, 50.0, 61.8};

CFiboFan2::CFiboFan2(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{
	m_basePOINT.x = m_basePOINT.y = 0;
}

CFiboFan2::~CFiboFan2()
{

}

void CFiboFan2::DrawTool(CDC* pDC)
{
	if (m_pointStart.x == m_pointEnd.x && m_pointStart.y == m_pointEnd.y)
		return;

	CPen	*cPen, *yellowPen;
	CPoint sp, ep;
	if (!m_bMove)
	{
		m_pIndcBase->GetValueToPoint(sp, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetValueToPoint(ep, m_dPosEnd, m_dValueEnd);
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
		
		cPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
		yellowPen = GetPen(m_pwndView, PS_DOT, 1, _YELLOW);
	}	
	
	if (m_bClipRegion)
		pDC->IntersectClipRect(&m_rectGrp);

	CPen*	sPen = pDC->SelectObject(yellowPen);
	int	sROP = pDC->SetROP2(R2_XORPEN);
	int	bkMode = pDC->SetBkMode(TRANSPARENT);

	if (CalculateDrawingPosition())
	{
		pDC->MoveTo(m_pointStart);
		pDC->LineTo(m_pointEnd);

		pDC->SelectObject(cPen);
		pDC->Polyline(m_BOXfibo, 5);
	
		for (int ii = 0; ii < maxFIBOFAN2; ii++)
		{
			pDC->MoveTo(m_Yfibo[ii]);
			pDC->LineTo(m_pointStart.x, m_Yfibo[ii].y);

			pDC->MoveTo(m_Xfibo[ii]);
			pDC->LineTo(m_Xfibo[ii].x, m_pointStart.y);

			pDC->MoveTo(m_pointStart);
			pDC->LineTo(m_XfiboEXT[ii]);

			pDC->MoveTo(m_pointStart);
			pDC->LineTo(m_YfiboEXT[ii]);
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

void CFiboFan2::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CFiboFan2::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointStart = pt;
	m_pointEnd = pt;
	m_orgSPoint = pt;

	bMouseCapture = TRUE;

	m_bMove = TRUE;
	m_bClipRegion = FALSE;
	CToolBase::InflateHolder();
}

void CFiboFan2::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	DrawTool(&ClientDC);
	if (pt.x < m_orgSPoint.x)
	{
		m_pointStart = pt;
		m_pointEnd = m_orgSPoint;
	}
	else
	{
		m_pointStart = m_orgSPoint;
		m_pointEnd = pt;
	}

	CToolBase::InflateHolder();
	DrawTool(&ClientDC);
}

void CFiboFan2::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
//	CToolBase::DrawHolder(&ClientDC, true);
	DrawTool(&ClientDC);

	bMouseCapture = FALSE;

	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;

	if (pt.x < m_orgSPoint.x)
	{
		m_pointStart = pt;
		m_pointEnd = m_orgSPoint;
	}
	else
	{
		m_pointStart = m_orgSPoint;
		m_pointEnd = pt;
	}
	DrawTool(&ClientDC);

	m_pIndcBase->GetPtToPosValue(m_pointEnd, m_dPosEnd, m_dValueEnd);
}

void CFiboFan2::ChangeToolLBDN(CPoint pt, bool &bMouseCapture)
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

void CFiboFan2::ChangeToolLBMV(CPoint pt, bool &bMouseCapture)
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

void CFiboFan2::ChangeToolLBUP(CPoint pt, bool &bMouseCapture)
{
	MoveToolLBUP(pt, bMouseCapture);
}

void CFiboFan2::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	m_pointCur = pt;

	::SetCapture(m_pwndParent->m_hWnd);
	bMouseCapture = TRUE;

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

void CFiboFan2::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
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

void CFiboFan2::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();

}

bool CFiboFan2::PtInToolRegion(CPoint pt)
{
	CPoint	pt1, pt2;
	const	int arrayCOUNT = maxFIBOFAN2 * 5 +1;
	int	aIDX = 0;

	SetPointArray(aIDX++, m_pointStart, m_pointEnd);
	for (int ii = 0; ii < maxFIBOFAN2; ii++)
	{
		SetPointArray(aIDX, m_BOXfibo[ii], m_BOXfibo[ii+1]);
		aIDX++;
	}

	for (int ii = 0; ii < maxFIBOFAN2; ii++)
	{
		pt1 = m_Yfibo[ii];
		pt2.x = m_pointStart.x;
		pt2.y = m_Yfibo[ii].y;
		SetPointArray(aIDX, pt1, pt2);
		aIDX++;

		pt1 = m_Xfibo[ii];
		pt2.x = m_Xfibo[ii].x;
		pt2.y = m_pointStart.y;
		SetPointArray(aIDX, pt1, pt2);
		aIDX++;

		SetPointArray(aIDX, m_pointStart, m_YfiboEXT[ii]);
		aIDX++;
		SetPointArray(aIDX, m_pointStart, m_XfiboEXT[ii]);
		aIDX++;
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

bool CFiboFan2::CalculateDrawingPosition()
{
	enum whichAREA {noneAREA = 0, firstAREA, secondAREA, thirdAREA, fourthAREA};

	whichAREA which = noneAREA;
	if (m_pointStart.x <= m_pointEnd.x)
	{
		if (m_pointStart.y > m_pointEnd.y)
		{
			m_basePOINT.x = m_rectGrp.right;
			m_basePOINT.y = m_rectGrp.top;
			which = firstAREA;
		}
		else if (m_pointStart.y < m_pointEnd.y) 
		{
			m_basePOINT.x = m_rectGrp.right;
			m_basePOINT.y = m_rectGrp.bottom;
			which = fourthAREA;
		}
	}
	else
	{
		if (m_pointStart.y > m_pointEnd.y)
		{
			m_basePOINT.x = m_rectGrp.left;
			m_basePOINT.y = m_rectGrp.top;
			which = secondAREA;
		}
		else if (m_pointStart.y < m_pointEnd.y)
		{
			m_basePOINT.x = m_rectGrp.left;
			m_basePOINT.y = m_rectGrp.bottom;
			which = thirdAREA;
		}
	}

	if (which == noneAREA)
		return false;

	m_BOXfibo[0].x = m_pointStart.x; m_BOXfibo[0].y = m_pointStart.y;
	m_BOXfibo[1].x = m_pointEnd.x;   m_BOXfibo[1].y = m_pointStart.y;
	m_BOXfibo[2].x = m_pointEnd.x;   m_BOXfibo[2].y = m_pointEnd.y;
	m_BOXfibo[3].x = m_pointStart.x; m_BOXfibo[3].y = m_pointEnd.y;
	m_BOXfibo[4].x = m_pointStart.x; m_BOXfibo[4].y = m_pointStart.y;

	int	Width = abs(m_pointStart.x - m_pointEnd.x);
	int	Height = abs(m_pointStart.y - m_pointEnd.y);
	for (int ii = 0; ii < maxFIBOFAN2; ii++)
	{
		switch (which)
		{
		case firstAREA:
			// 세로축
			m_Yfibo[ii].x = m_pointEnd.x;
			m_Yfibo[ii].y = m_pointStart.y - int(Height * fiboFAN2[ii] / 100.);
			// 가로축
			m_Xfibo[ii].x = m_pointStart.x + int(Width * fiboFAN2[ii] / 100.);
			m_Xfibo[ii].y = m_pointEnd.y;
			break;
		case secondAREA:
			// 세로축
			m_Yfibo[ii].x = m_pointEnd.x;
			m_Yfibo[ii].y = m_pointStart.y - int(Height * fiboFAN2[ii] / 100.);
			// 가로축
			m_Xfibo[ii].x = m_pointStart.x - int(Width * fiboFAN2[ii] / 100.);
			m_Xfibo[ii].y = m_pointEnd.y;
			break;
		case thirdAREA:
			// 세로축
			m_Yfibo[ii].x = m_pointEnd.x;
			m_Yfibo[ii].y = m_pointStart.y + int(Height * fiboFAN2[ii] / 100.);
			// 가로축
			m_Xfibo[ii].x = m_pointStart.x - int(Width * fiboFAN2[ii] / 100.);
			m_Xfibo[ii].y = m_pointEnd.y;
			break;
		case fourthAREA:
			// 세로축
			m_Yfibo[ii].x = m_pointEnd.x;
			m_Yfibo[ii].y = m_pointStart.y + int(Height * fiboFAN2[ii] / 100.);
			// 가로축
			m_Xfibo[ii].x = m_pointStart.x + int(Width * fiboFAN2[ii] / 100.);
			m_Xfibo[ii].y = m_pointEnd.y;
			break;
		}
	}

	double	baseRATE, xChkRATE, yChkRATE;
	Width  = abs(m_basePOINT.x - m_pointStart.x);
	Height = abs(m_basePOINT.y - m_pointStart.y);
	if (Width < 0)	Width = 1;
	baseRATE = double(Height) / double(Width);

	for (int ii = 0; ii < maxFIBOFAN2; ii++)
	{
		Width  = abs(m_pointStart.x - m_Xfibo[ii].x);
		Height = abs(m_pointStart.y - m_Xfibo[ii].y);
		if (Width < 1)	Width = 1;
		xChkRATE = double(Height) / double(Width);
		if (xChkRATE == 0)	xChkRATE = 1;

		Width  = abs(m_pointStart.x - m_Yfibo[ii].x);
		Height = abs(m_pointStart.y - m_Yfibo[ii].y);
		if (Width < 1)	Width = 1;
		yChkRATE = double(Height) / double(Width);
		if (yChkRATE == 0)	yChkRATE = 1;

		switch (which)
		{
		case firstAREA:
			if (xChkRATE > baseRATE)
			{
				Height = abs(m_pointStart.y - m_rectGrp.top);
				m_XfiboEXT[ii].x = m_pointStart.x + int(Height / xChkRATE);
				m_XfiboEXT[ii].y = m_rectGrp.top;
			}
			else
			{
				Width = abs(m_pointStart.x - m_rectGrp.right);
				m_XfiboEXT[ii].x = m_rectGrp.right;
				m_XfiboEXT[ii].y = m_pointStart.y - int(Width * xChkRATE);
			}
			if (yChkRATE <= baseRATE)
			{
				Width = abs(m_pointStart.x - m_rectGrp.right);
				m_YfiboEXT[ii].x = m_rectGrp.right;
				m_YfiboEXT[ii].y = m_pointStart.y - int(Width * yChkRATE);
			}
			else
			{
				Height = abs(m_pointStart.y - m_rectGrp.top);
				m_YfiboEXT[ii].x = m_pointStart.x + int(Height / yChkRATE);
				m_YfiboEXT[ii].y = m_rectGrp.top;
			}
			break;
		case secondAREA:
			if (xChkRATE > baseRATE)
			{
				Height = abs(m_pointStart.y - m_rectGrp.top);
				m_XfiboEXT[ii].x = m_pointStart.x - int(Height / xChkRATE);
				m_XfiboEXT[ii].y = m_rectGrp.top;
			}
			else
			{
				Width = abs(m_pointStart.x - m_rectGrp.left);
				m_XfiboEXT[ii].x = m_rectGrp.left;
				m_XfiboEXT[ii].y = m_pointStart.y - int(Width * xChkRATE);

			}
			if (yChkRATE <= baseRATE)
			{
				Width = abs(m_pointStart.x - m_rectGrp.left);
				m_YfiboEXT[ii].x = m_rectGrp.left;
				m_YfiboEXT[ii].y = m_pointStart.y - int(Width * yChkRATE);
			}
			else
			{
				Height = abs(m_pointStart.y - m_rectGrp.top);
				m_YfiboEXT[ii].x = m_pointStart.x -  int(Height / yChkRATE);
				m_YfiboEXT[ii].y = m_rectGrp.top;
			}
			break;
		case thirdAREA:
			if (xChkRATE > baseRATE)
			{
				Height = abs(m_pointStart.y - m_rectGrp.bottom);
				m_XfiboEXT[ii].x = m_pointStart.x - int(Height / xChkRATE);
				m_XfiboEXT[ii].y = m_rectGrp.bottom;
			}
			else
			{
				Width = abs(m_pointStart.x - m_rectGrp.left);
				m_XfiboEXT[ii].x = m_rectGrp.left;
				m_XfiboEXT[ii].y = m_pointStart.y + int(Width * xChkRATE);

			}
			if (yChkRATE > baseRATE)
			{
				Height = abs(m_pointStart.y - m_rectGrp.bottom);
				m_YfiboEXT[ii].x = m_pointStart.x -  int(Height / yChkRATE);
				m_YfiboEXT[ii].y = m_rectGrp.bottom;
			}
			else
			{
				Width = abs(m_pointStart.x - m_rectGrp.left);
				m_YfiboEXT[ii].x = m_rectGrp.left;
				m_YfiboEXT[ii].y = m_pointStart.y + int(Width * yChkRATE);
			}
			break;
		case fourthAREA:
			if (xChkRATE > baseRATE)
			{
				Height = abs(m_pointStart.y - m_rectGrp.bottom);
				m_XfiboEXT[ii].x = m_pointStart.x + int(Height / xChkRATE);
				m_XfiboEXT[ii].y = m_rectGrp.bottom;
			}
			else
			{
				Width = abs(m_pointStart.x - m_rectGrp.right);
				m_XfiboEXT[ii].x = m_rectGrp.right;
				m_XfiboEXT[ii].y = m_pointStart.y + int(Width * xChkRATE);
			}

			if (yChkRATE > baseRATE)
			{
				Height = abs(m_pointStart.y - m_rectGrp.bottom);
				m_YfiboEXT[ii].x = m_pointStart.x + int(Height / yChkRATE);
				m_YfiboEXT[ii].y = m_rectGrp.bottom;
			}
			else
			{
				Width = abs(m_pointStart.x - m_rectGrp.right);
				m_YfiboEXT[ii].x = m_rectGrp.right;
				m_YfiboEXT[ii].y = m_pointStart.y + int(Width * yChkRATE);
			}
			break;
		}
	}

	return true;
}

void CFiboFan2::SetPointArray(int idx, CPoint pt1, CPoint pt2)
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

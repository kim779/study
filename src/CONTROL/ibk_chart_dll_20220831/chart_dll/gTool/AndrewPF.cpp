// AndrewPF.cpp: implementation of the CAndrewPF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AndrewPF.h"
#include "../../h/axisgwin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAndrewPF::CAndrewPF(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{
	m_dwToolOption = TO_SHOW_POSI;
	m_PointCount = 0;

	m_FPoint = 0;
	m_SPoint = 0;
	m_TPoint = 0;

	m_IndexPoint1 = 0;
	m_IndexPoint2 = 0;
	m_IndexPoint3 = 0;

	m_CenterPoint = 0;

	m_OffSetPointUp = 0;
	m_OffSetPointDown = 0;

	m_DrawLine = false;

	m_SelectPoint = 0;

	m_RgnEndPoint = 0;
	m_RgnStartPoint = 0;
	m_RgnEndPoint1 = 0;
	m_RgnStartPoint1 = 0;
	m_RgnEndPoint2 = 0;
	m_RgnStartPoint2 = 0;

	m_MaxPoint = 0;
	m_MiddlePoint = 0;
	m_MinPoint = 0;

	m_Maxvalue = 0;
	m_Middlevalue = 0;
	m_Minvalue = 0;
	m_Centervalue = 0;
	m_Upvalue = 0;
	m_Downvalue = 0;

	m_Maxpos = 0;
	m_Middlepos = 0;
	m_Minpos = 0;
	m_Centerpos = 0;
	m_uppos = 0;
	m_Downpos = 0;

	m_Firstvalue = 0;
	m_Secundvalue = 0;
	m_Thirdvalue = 0;

	m_Firstpos = 0;
	m_Secundpos = 0;
	m_Thirdpos = 0;

	m_cntRcHolder.SetRectEmpty();
	m_cntRcHolder1.SetRectEmpty();
	m_cntRcHolder2.SetRectEmpty();
}

CAndrewPF::~CAndrewPF()
{

}

void CAndrewPF::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CAndrewPF::DrawHolder(CDC *pDC, bool showHolder)
{
	CBrush	*cBrush = GetBrush(m_pwndView, m_rgbHolder);
	CBrush	*sBrush = pDC->SelectObject(cBrush);
	int	sROP;

	if (showHolder)
	{
		m_bShowHolder = true;
	}
	else
	{
		m_bShowHolder = false;
	}
	sROP = pDC->SetROP2(R2_XORPEN);

	if (m_cntRcHolder.right < m_rectGrp.right)
		pDC->Rectangle(&m_cntRcHolder);

	if (m_cntRcHolder1.right < m_rectGrp.right)
		pDC->Rectangle(&m_cntRcHolder1);

	if (m_cntRcHolder2.right < m_rectGrp.right)
		pDC->Rectangle(&m_cntRcHolder2);

	pDC->SetROP2(sROP);
	pDC->SelectObject(sBrush);
}

void CAndrewPF::HideHolder()
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);
}

void CAndrewPF::InflateHolder()
{
	CPoint holder;
	CPoint	dn;
	
	switch (m_PointCount)
	{
	case 1:		
		holder.x = m_IndexPoint1.x;
		holder.y = m_IndexPoint1.y;

		m_cntRcHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
		m_cntRcHolder.InflateRect(szHOLDER, szHOLDER);
		break;
	case 2:
		holder.x = m_IndexPoint1.x;
		holder.y = m_IndexPoint1.y;
		m_cntRcHolder.SetRect(holder.x, holder.y, holder.x, holder.y);

		holder.x = m_IndexPoint2.x;
		holder.y = m_IndexPoint2.y;
		m_cntRcHolder1.SetRect(holder.x, holder.y, holder.x, holder.y);

		m_cntRcHolder.InflateRect(szHOLDER, szHOLDER);
		m_cntRcHolder1.InflateRect(szHOLDER, szHOLDER);
		break;
	case 0:
		holder.x = m_IndexPoint1.x;
		holder.y = m_IndexPoint1.y;
		m_cntRcHolder.SetRect(holder.x, holder.y, holder.x, holder.y);

		holder.x = m_IndexPoint2.x;
		holder.y = m_IndexPoint2.y;
		m_cntRcHolder1.SetRect(holder.x, holder.y, holder.x, holder.y);

		holder.x = m_IndexPoint3.x;
		holder.y = m_IndexPoint3.y;
		m_cntRcHolder2.SetRect(holder.x, holder.y, holder.x, holder.y);

		m_cntRcHolder.InflateRect(szHOLDER, szHOLDER);
		m_cntRcHolder1.InflateRect(szHOLDER, szHOLDER);
		m_cntRcHolder2.InflateRect(szHOLDER, szHOLDER);
		break;
	}

	CToolBase::InflateHolder();
}

void CAndrewPF::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	InitPoint(pt);

	m_bMove = TRUE;
	bMouseCapture = TRUE;
	m_bClipRegion = FALSE;

	DrawTool(&ClientDC);
	InflateHolder();
}

void CAndrewPF::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	return;
	if (pt.x < m_rectGrp.left)
		return;
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);
	InflateHolder();
}

void CAndrewPF::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	DrawHolder(&ClientDC, true);
	DrawTool(&ClientDC);

	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);
}

void CAndrewPF::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointCur = pt;
	
	::SetCapture(m_pwndParent->m_hWnd);
	bMouseCapture = TRUE;

	CClientDC ClientDC(m_pwndParent);

	if (!m_bShowHolder)
		DrawHolder(&ClientDC, false);

	m_bShowHolder = false;
	m_bClipRegion = FALSE;
	m_bMove = TRUE;

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);
	m_bShowHolder = true;
}

void CAndrewPF::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	CPoint	MVPoint;

	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);

	DrawTool(&ClientDC);
		
	m_IndexPoint1.x += pt.x - m_pointCur.x;
	m_IndexPoint1.y += pt.y - m_pointCur.y;

	m_IndexPoint2.x += pt.x - m_pointCur.x;
	m_IndexPoint2.y += pt.y - m_pointCur.y;
	
	m_IndexPoint3.x += pt.x - m_pointCur.x;
	m_IndexPoint3.y += pt.y - m_pointCur.y;
	
	//m_CenterPoint.x += pt.x - m_pointCur.x;
	//m_CenterPoint.y += pt.y - m_pointCur.y;
	
	//m_OffSetPointUp.x += pt.x - m_pointCur.x;
	//m_OffSetPointUp.y += pt.y - m_pointCur.y;
	
	//m_OffSetPointDown.x += pt.x - m_pointCur.x;
	//m_OffSetPointDown.y += pt.y - m_pointCur.y;
	
	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);

	DrawTool(&ClientDC);
	m_pointCur = pt;
	InflateHolder();
}

void CAndrewPF::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();
}

bool CAndrewPF::PtInToolHolderRect(CPoint pt)
{
	if (m_cntRcHolder.PtInRect(pt) || m_cntRcHolder1.PtInRect(pt) || m_cntRcHolder2.PtInRect(pt))
		return true;
	
	return false;
}

bool CAndrewPF::PtInToolRegion(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))
		return false;
	
	CPoint	point[maxPFLINE][5];
	CPoint	drawUP, drawDN;
	
	drawUP.x = m_RgnEndPoint2.x;
	drawUP.y = m_RgnEndPoint2.y;
	drawDN.x = m_RgnStartPoint2.x;
	drawDN.y = m_RgnStartPoint2.y;
	
	point[0][0].x = drawUP.x;
	point[0][0].y = drawUP.y - szREGION;
	point[0][1].x = drawUP.x;
	point[0][1].y = drawUP.y + szREGION;
	point[0][2].x = drawDN.x;
	point[0][2].y = drawDN.y + szREGION;
	point[0][3].x = drawDN.x;
	point[0][3].y = drawDN.y - szREGION;
	point[0][4].x = drawUP.x;
	point[0][4].y = drawUP.y - szREGION;

	drawUP.x = m_RgnEndPoint.x;
	drawUP.y = m_RgnEndPoint.y;

	drawDN.x = m_RgnStartPoint.x;
	drawDN.y = m_RgnStartPoint.y;

	point[1][0].x = drawUP.x;
	point[1][0].y = drawUP.y - szREGION;
	point[1][1].x = drawUP.x;
	point[1][1].y = drawUP.y + szREGION;
	point[1][2].x = drawDN.x;
	point[1][2].y = drawDN.y + szREGION;
	point[1][3].x = drawDN.x;
	point[1][3].y = drawDN.y - szREGION;
	point[1][4].x = drawUP.x;
	point[1][4].y = drawUP.y - szREGION;

	drawUP.x = m_RgnEndPoint1.x;
	drawUP.y = m_RgnEndPoint1.y;

	drawDN.x = m_RgnStartPoint1.x;
	drawDN.y = m_RgnStartPoint1.y;
	
	point[2][0].x = drawUP.x;
	point[2][0].y = drawUP.y - szREGION;
	point[2][1].x = drawUP.x;
	point[2][1].y = drawUP.y + szREGION;
	point[2][2].x = drawDN.x;
	point[2][2].y = drawDN.y + szREGION;
	point[2][3].x = drawDN.x;
	point[2][3].y = drawDN.y - szREGION;
	point[2][4].x = drawUP.x;
	point[2][4].y = drawUP.y - szREGION;

	int count[maxPFLINE];
	for (int ii = 0; ii < maxPFLINE; ii++)
		count[ii] = 5;

	CRgn HLineRGN;
	if (!HLineRGN.CreatePolyPolygonRgn(&point[0][0], count, maxPFLINE, WINDING))
		return false;

	BOOL bInRgn = HLineRGN.PtInRegion(pt); 		
	HLineRGN.DeleteObject();	

	return (bInRgn ? true : false);
}

void CAndrewPF::DrawTool(CDC* pDC)
{
	CPen* ppenCur;
	CPoint pointStart;
	
	if (!m_bMove)
	{
		//m_pIndcBase->GetValueToPoint(m_FPoint, m_Firstpos, m_Firstvalue);
		//m_pIndcBase->GetValueToPoint(m_SPoint, m_Secundpos, m_Secundvalue);
		//m_pIndcBase->GetValueToPoint(m_TPoint, m_Thirdpos, m_Thirdvalue);

		ppenCur = GetPen(m_pwndView, m_btLStyle, m_btLWidth, m_rgbText);
	}
	else
	{
		//m_pIndcBase->GetPtToPosValue(m_FPoint, m_Firstpos, m_Firstvalue);
		//m_pIndcBase->GetPtToPosValue(m_SPoint, m_Secundpos, m_Secundvalue);
		//m_pIndcBase->GetPtToPosValue(m_TPoint, m_Thirdpos, m_Thirdvalue);

		ppenCur = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
	}

	int iOldMode = pDC->SetBkMode(TRANSPARENT);
	CPen* ppenOld = pDC->SelectObject(ppenCur);
	int iOldRop = pDC->SetROP2(R2_XORPEN);

	if (m_bClipRegion)
		pDC->IntersectClipRect(&m_rectGrp);

	CPoint HCross1, VCross1;
	CPoint HCross2, VCross2;
	CPoint tmpPoint;
	CString	str;

	if (!m_DrawLine)
	{
		for (int ii = 0; ii < m_PointCount; ii++)
		{
			if (ii == 0)
			{
				tmpPoint = m_FPoint;
				str      = _T("1");
			}
			else if (ii == 1)
			{
				tmpPoint = m_SPoint;
				str      = _T("2");
			}
			else if (ii == 2)
			{
				tmpPoint = m_TPoint;
				str      = _T("3");
			}
			
			HCross1.x = tmpPoint.x - 5;
			HCross1.y = tmpPoint.y;
			
			HCross2.x = tmpPoint.x + 5;
			HCross2.y = tmpPoint.y;
			
			VCross1.x = tmpPoint.x;
			VCross1.y = tmpPoint.y + 5;
			
			VCross2.x = tmpPoint.x;
			VCross2.y = tmpPoint.y - 5;
			
			pDC->MoveTo(HCross1);
			pDC->LineTo(HCross2);
			
			pDC->MoveTo(VCross1);
			pDC->LineTo(VCross2);

			CFont* pfontCur = GetFont(m_pwndView, 9, "굴림체");
			CFont* pfontOld = pDC->SelectObject(pfontCur);

			CRect rc;
			rc.SetRect(VCross2.x + 5, VCross2.y + 5, VCross2.x + 20, VCross2.y + 20);
			pDC->DrawText(str, &rc, DT_SINGLELINE|DT_VCENTER|DT_LEFT);

			pDC->SelectObject(pfontOld);
		}
	}
	else
	{
		CPoint	pointStart = 0;
		CPoint	pointEnd = 0;
		CPoint	PointGap = 0;

		CPoint	MaxPoint;
		CPoint	MiddlePoint;
		CPoint	MinPoint;
		CPoint	CenterPoint;
		CPoint	OffSetPointUp;
		CPoint	OffSetPointDown;
		
		CalMinMaxPoint();
		//OffsetCal();

		if (!m_bMove)
		{
			m_pIndcBase->GetValueToPoint(MaxPoint, m_Maxpos, m_Maxvalue);
			//m_pIndcBase->GetValueToPoint(CenterPoint, m_Centerpos, m_Centervalue);

			m_pIndcBase->GetValueToPoint(MiddlePoint, m_Middlepos, m_Middlevalue);
			//m_pIndcBase->GetValueToPoint(OffSetPointUp, m_uppos, m_Upvalue);

			m_pIndcBase->GetValueToPoint(MinPoint, m_Minpos, m_Minvalue);
			//m_pIndcBase->GetValueToPoint(OffSetPointDown, m_Downpos, m_Downvalue);

			m_MaxPoint        = MaxPoint;
			m_MiddlePoint     = MiddlePoint;
			m_MinPoint        = MinPoint;
			//m_CenterPoint     = CenterPoint;
			//m_OffSetPointUp   = OffSetPointUp;
			//m_OffSetPointDown = OffSetPointDown;
			
			m_IndexPoint1 = m_MaxPoint;
			m_IndexPoint2 = m_MiddlePoint;
			m_IndexPoint3 = m_MinPoint;
		}
		else
		{
			MaxPoint        = m_MaxPoint;
			MiddlePoint     = m_MiddlePoint;
			MinPoint        = m_MinPoint;
			//CenterPoint     = m_CenterPoint;
			//OffSetPointUp   = m_OffSetPointUp;
			//OffSetPointDown = m_OffSetPointDown;

			m_pIndcBase->GetPtToPosValue(MaxPoint, m_Maxpos, m_Maxvalue);
			//m_pIndcBase->GetPtToPosValue(CenterPoint, m_Centerpos, m_Centervalue);

			m_pIndcBase->GetPtToPosValue(MiddlePoint, m_Middlepos, m_Middlevalue);
			//m_pIndcBase->GetPtToPosValue(OffSetPointUp, m_uppos, m_Upvalue);

			m_pIndcBase->GetPtToPosValue(MinPoint, m_Minpos, m_Minvalue);
			//m_pIndcBase->GetPtToPosValue(OffSetPointDown, m_Downpos, m_Downvalue);
		}

		CenterPoint.x = (MaxPoint.x + MiddlePoint.x) / 2;
		CenterPoint.y = (MaxPoint.y + MiddlePoint.y) / 2;

		pointStart = MinPoint;
		pointEnd   = CenterPoint;

		if (!m_rectChart.PtInRect(MinPoint) && !m_rectChart.PtInRect(CenterPoint))
		{
			// 양쪽이 왼쪽에
			if (m_rectChart.left > MinPoint.x && m_rectChart.left > CenterPoint.x)
			{
				if (MinPoint.x > CenterPoint.x)
					CalculateDrawingPositionStart(pointStart, MinPoint, CenterPoint);
				else
					CalculateDrawingPositionEnd(pointEnd, MinPoint, CenterPoint);
			}
			// 양쪽이 오른쪽에
			else if (m_rectChart.right < MinPoint.x && m_rectChart.right < CenterPoint.x)
			{
				if (MinPoint.x < CenterPoint.x)
					CalculateDrawingPositionStart(pointStart, MinPoint, CenterPoint);
				else
					CalculateDrawingPositionEnd(pointEnd, MinPoint, CenterPoint);
			}
		}
		else
		{
			CalculateDrawingPositionStart(pointStart, MinPoint, CenterPoint);
			CalculateDrawingPositionEnd(pointEnd, MinPoint, CenterPoint);
		}

		m_RgnStartPoint2 =  pointStart;
		m_RgnEndPoint2   =  pointEnd;
		
		pDC->MoveTo(pointStart.x, pointStart.y);
		pDC->LineTo(pointEnd.x, pointEnd.y);

		OffSetPointUp.x = MinPoint.x - (CenterPoint.x - MiddlePoint.x);
		OffSetPointUp.y = MinPoint.y - (CenterPoint.y - MiddlePoint.y);

		OffSetPointDown.x = MinPoint.x + (MaxPoint.x - CenterPoint.x);
		OffSetPointDown.y = MinPoint.y + (MaxPoint.y - CenterPoint.y);

		pointStart = OffSetPointDown;
		pointEnd   = MaxPoint;
		
		if (!m_rectChart.PtInRect(OffSetPointDown) && !m_rectChart.PtInRect(MaxPoint))
		{
			// 양쪽이 왼쪽에
			if (m_rectChart.left > OffSetPointDown.x && m_rectChart.left > MaxPoint.x)
			{
				if (OffSetPointDown.x > MaxPoint.x)
					CalculateDrawingPositionStart(pointStart, OffSetPointDown, MaxPoint);
				else
					CalculateDrawingPositionEnd(pointEnd, OffSetPointDown, MaxPoint);
			}
			// 양쪽이 오른쪽에
			else if (m_rectChart.right < OffSetPointDown.x && m_rectChart.right < MaxPoint.x)
			{
				if (OffSetPointDown.x < MaxPoint.x)
					CalculateDrawingPositionStart(pointStart, OffSetPointDown, MaxPoint);
				else
					CalculateDrawingPositionEnd(pointEnd, OffSetPointDown, MaxPoint);
			}
		}
		else
		{
			CalculateDrawingPositionStart(pointStart, OffSetPointDown, MaxPoint);
			CalculateDrawingPositionEnd(pointEnd, OffSetPointDown, MaxPoint);
		}
		
		m_RgnStartPoint =  pointStart;
		m_RgnEndPoint   =  pointEnd;
		
		pDC->MoveTo(pointStart.x, pointStart.y);
		pDC->LineTo(pointEnd.x, pointEnd.y);
		
		pointStart = OffSetPointUp;
		pointEnd   = MiddlePoint;
		
		if (!m_rectChart.PtInRect(OffSetPointUp) && !m_rectChart.PtInRect(MiddlePoint))
		{
			// 양쪽이 왼쪽에
			if (m_rectChart.left > OffSetPointUp.x && m_rectChart.left > MiddlePoint.x)
			{
				if (OffSetPointUp.x > MiddlePoint.x)
					CalculateDrawingPositionStart(pointStart, OffSetPointUp, MiddlePoint);
				else
					CalculateDrawingPositionEnd(pointEnd, OffSetPointUp, MiddlePoint);
			}
			// 양쪽이 오른쪽에
			else if (m_rectChart.right < OffSetPointUp.x && m_rectChart.right < MiddlePoint.x)
			{
				if (OffSetPointUp.x < MiddlePoint.x)
					CalculateDrawingPositionStart(pointStart, OffSetPointUp, MiddlePoint);
				else
					CalculateDrawingPositionEnd(pointEnd, OffSetPointUp, MiddlePoint);
			}
		}
		else
		{
			CalculateDrawingPositionStart(pointStart, OffSetPointUp, MiddlePoint);
			CalculateDrawingPositionEnd(pointEnd, OffSetPointUp, MiddlePoint);
		}
		
		m_RgnStartPoint1 =  pointStart;
		m_RgnEndPoint1   =  pointEnd;
		
		pDC->MoveTo(pointStart.x, pointStart.y);
		pDC->LineTo(pointEnd.x, pointEnd.y);
	}

	pDC->SelectObject(ppenOld);
	pDC->SetROP2(iOldRop);
	pDC->SetBkMode(iOldMode);

	InflateHolder();

	if (m_bShowHolder)
		DrawHolder(pDC, true);

	if (m_bClipRegion)
		pDC->SelectClipRgn(NULL);
}

/*int  CAndrewPF::XYPointCount()
{
	return m_PointCount;
}*/

void CAndrewPF::InitPoint(CPoint pt)
{
	if (pt.x > m_rectGrp.left && pt.x < m_rectGrp.right)
	{
		if (pt.y > m_rectGrp.top && pt.y < m_rectGrp.bottom)
		{
			m_PointCount++;
			
			switch (m_PointCount)
			{
			case 1:
				m_FPoint = pt;
				m_IndexPoint1 = pt;
				
				m_DrawLine = false;
				break;
			case 2:
				m_SPoint = pt;
				
				if (m_IndexPoint1.x > pt.x)
					m_IndexPoint2 = pt;
				else
				{
					m_IndexPoint2 = m_IndexPoint1;
					m_IndexPoint1 = pt;
				}
				
				m_DrawLine = false;
				break;
			case 3:
				m_TPoint = pt;
				
				if (m_IndexPoint1.x > pt.x)
				{
					if (m_IndexPoint2.x > pt.x)
						m_IndexPoint3 = pt;
					else
					{
						m_IndexPoint3 = m_IndexPoint2;
						m_IndexPoint2 = pt;
					}
				}
				else
				{
					m_IndexPoint3 = m_IndexPoint2;
					m_IndexPoint2 = m_IndexPoint1;
					m_IndexPoint1 = pt;
				}

				m_PointCount = 0;
				m_DrawLine = true;
				break;
			}
		}
	}
}

bool CAndrewPF::CalculateDrawingPositionStart(CPoint& rpointStart, CPoint ipointStart, CPoint ipointEnd)
{
	enum enumWhichArea {noneAREA = 0, firstAREA, secondAREA, thirdAREA, fourthAREA};

	CPoint pointBase;
	enumWhichArea eWhichArea = noneAREA;

	if (ipointStart.x < ipointEnd.x)
	{
		if (ipointStart.y > ipointEnd.y)
		{
			pointBase.x = m_rectChart.left;
			pointBase.y = m_rectChart.bottom;
			eWhichArea = thirdAREA;
		}
		else if (ipointStart.y < ipointEnd.y) 
		{
			pointBase.x = m_rectChart.left;
			pointBase.y = m_rectChart.top;
			eWhichArea = secondAREA;
		}
		else
		{
			rpointStart.x = m_rectChart.left;
			rpointStart.y = ipointStart.y;
			//m_pointExtStart = rpointStart;
			return true;
		}
	}
	else if (ipointStart.x > ipointEnd.x)
	{
		if (ipointStart.y > ipointEnd.y)
		{
			pointBase.x = m_rectChart.right;
			pointBase.y = m_rectChart.bottom;
			eWhichArea = fourthAREA;
		}
		else if (ipointStart.y < ipointEnd.y)
		{
			pointBase.x = m_rectChart.right;
			pointBase.y = m_rectChart.top;
			eWhichArea = firstAREA;
		}
		else
		{
			rpointStart.x = m_rectChart.right;
			rpointStart.y = ipointStart.y;
			//m_pointExtStart = rpointStart;
			return true;
		}
	}
	else
	{
		rpointStart.x = ipointStart.x;
		if (ipointStart.y > ipointEnd.y)
		{
			rpointStart.y = m_rectChart.bottom;
		}
		else if (ipointStart.y < ipointEnd.y)
		{
			rpointStart.y = m_rectChart.top;
		}
		else
		{
			rpointStart.y = ipointStart.y;
		}
		//m_pointExtStart = rpointStart;
		return true;
	}

	if (eWhichArea == noneAREA)
		return false;

	int iWidth = abs(ipointStart.x - ipointEnd.x);
	int iHeight = abs(ipointStart.y - ipointEnd.y);
	if (iWidth < 1)
		iWidth = 1;

	double dGannRate = double(double(iHeight) / double(iWidth));

	iWidth  = abs(pointBase.x - ipointEnd.x);
	iHeight = abs(pointBase.y - ipointEnd.y);
	if (iWidth < 1)
		iWidth = 1;

	double dBaseRate = double(iHeight) / double(iWidth);
//TRACE("Start : Gan = %f, \tBase = %f\n", dGannRate, dBaseRate);
	if (dGannRate == 0)
		dGannRate = 1;

	switch (eWhichArea)
	{
	case firstAREA:
		if (dGannRate > dBaseRate)
		{
			rpointStart.x = ipointEnd.x + int(iHeight / dGannRate);
			rpointStart.y = m_rectChart.top;
		}
		else
		{
			rpointStart.x = m_rectChart.right;
			rpointStart.y = ipointEnd.y - int(iWidth * dGannRate);
		}
		break;
	case secondAREA:
		if (dGannRate > dBaseRate)
		{
			rpointStart.x = ipointEnd.x - int(iHeight / dGannRate);
			rpointStart.y = m_rectChart.top;
		}
		else
		{
			rpointStart.x = m_rectChart.left;
			rpointStart.y = ipointEnd.y - int(iWidth * dGannRate);

		}
		break;
	case thirdAREA:
		if (dGannRate > dBaseRate)
		{
			rpointStart.x = ipointEnd.x - int(iHeight / dGannRate);
			rpointStart.y = m_rectChart.bottom;
		}
		else
		{
			rpointStart.x = m_rectChart.left;
			rpointStart.y = ipointEnd.y + int(iWidth * dGannRate);

		}
		break;
	case fourthAREA:
		if (dGannRate > dBaseRate)
		{
			rpointStart.x = ipointEnd.x + int(iHeight / dGannRate);
			rpointStart.y = m_rectChart.bottom;
		}
		else
		{
			iWidth = abs(ipointEnd.x - m_rectChart.right);
			rpointStart.x = m_rectChart.right;
			rpointStart.y = ipointEnd.y + int(iWidth * dGannRate);
		}
		break;
	}

	//m_pointExtStart = rpointStart;

	return true;
}

bool CAndrewPF::CalculateDrawingPositionEnd(CPoint& rpointEnd, CPoint ipointStart, CPoint ipointEnd)
{
	enum enumWhichArea {noneAREA = 0, firstAREA, secondAREA, thirdAREA, fourthAREA};

	CPoint pointBase;
	enumWhichArea eWhichArea = noneAREA;

	if (ipointStart.x < ipointEnd.x)
	{
		if (ipointStart.y > ipointEnd.y)
		{
			pointBase.x = m_rectChart.right;
			pointBase.y = m_rectChart.top;
			eWhichArea = firstAREA;
		}
		else if (ipointStart.y < ipointEnd.y) 
		{
			pointBase.x = m_rectChart.right;
			pointBase.y = m_rectChart.bottom;
			eWhichArea = fourthAREA;
		}
		else
		{
			rpointEnd.x = m_rectChart.right;
			rpointEnd.y = ipointEnd.y;
			//m_pointExtEnd = rpointEnd;
			return true;
		}
	}
	else if (ipointStart.x > ipointEnd.x)
	{
		if (ipointStart.y > ipointEnd.y)
		{
			pointBase.x = m_rectChart.left;
			pointBase.y = m_rectChart.top;
			eWhichArea = secondAREA;
		}
		else if (ipointStart.y < ipointEnd.y)
		{
			pointBase.x = m_rectChart.left;
			pointBase.y = m_rectChart.bottom;
			eWhichArea = thirdAREA;
		}
		else
		{
			rpointEnd.x = m_rectChart.left;
			rpointEnd.y = ipointEnd.y;
			//m_pointExtEnd = rpointEnd;
			return true;
		}
	}
	else
	{
		rpointEnd.x = ipointEnd.x;
		if (ipointStart.y > ipointEnd.y)
		{
			rpointEnd.y = m_rectChart.top;
		}
		else if (ipointStart.y < ipointEnd.y)
		{
			rpointEnd.y = m_rectChart.bottom;
		}
		else
		{
			rpointEnd.y = ipointEnd.y;
		}
		//m_pointExtEnd = rpointEnd;
		return true;
	}

	if (eWhichArea == noneAREA)
		return false;

	int iWidth = abs(ipointStart.x - ipointEnd.x);
	int iHeight = abs(ipointStart.y - ipointEnd.y);
	if (iWidth < 1)
		iWidth = 1;

	double dGannRate = double(double(iHeight) / double(iWidth));

	iWidth  = abs(pointBase.x - ipointStart.x);
	iHeight = abs(pointBase.y - ipointStart.y);
	if (iWidth < 1)
		iWidth = 1;

	double dBaseRate = double(iHeight) / double(iWidth);
	if (dGannRate == 0)
		dGannRate = 1;

	switch (eWhichArea)
	{
	case firstAREA:
		if (dGannRate > dBaseRate)
		{
			rpointEnd.x = ipointStart.x + int(iHeight / dGannRate);
			rpointEnd.y = m_rectChart.top;
		}
		else
		{
			rpointEnd.x = m_rectChart.right;
			rpointEnd.y = ipointStart.y - int(iWidth * dGannRate);
		}
		break;
	case secondAREA:
		if (dGannRate > dBaseRate)
		{
			rpointEnd.x = ipointStart.x - int(iHeight / dGannRate);
			rpointEnd.y = m_rectChart.top;
		}
		else
		{
			rpointEnd.x = m_rectChart.left;
			rpointEnd.y = ipointStart.y - int(iWidth * dGannRate);

		}
		break;
	case thirdAREA:
		if (dGannRate > dBaseRate)
		{
			rpointEnd.x = ipointStart.x - int(iHeight / dGannRate);
			rpointEnd.y = m_rectChart.bottom;
		}
		else
		{
			rpointEnd.x = m_rectChart.left;
			rpointEnd.y = ipointStart.y + int(iWidth * dGannRate);

		}
		break;
	case fourthAREA:
		if (dGannRate > dBaseRate)
		{
			rpointEnd.x = ipointStart.x + int(iHeight / dGannRate);
			rpointEnd.y = m_rectChart.bottom;
		}
		else
		{
			rpointEnd.x = m_rectChart.right;
			rpointEnd.y = ipointStart.y + int(iWidth * dGannRate);
		}
		break;
	}

	//m_pointExtEnd = rpointEnd;

	return true;
}

void CAndrewPF::ChangeToolLBDN(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	CToolBase::DrawHolder(&ClientDC, false);

	m_pointCur = pt;

	if (m_cntRcHolder.PtInRect(pt))
		m_SelectPoint = 1;
	else if (m_cntRcHolder1.PtInRect(pt))
		m_SelectPoint = 2;
	else if (m_cntRcHolder2.PtInRect(pt))
		m_SelectPoint = 3;

	DrawTool(&ClientDC);
	m_bClipRegion = FALSE;
	DrawTool(&ClientDC);

	::SetCapture(m_pwndParent->m_hWnd);
	bMouseCapture = TRUE;

	m_bMove = TRUE;
	m_bClipRegion = FALSE;

	DrawTool(&ClientDC);
}

void CAndrewPF::ChangeToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	if (m_bShowHolder)
		CToolBase::DrawHolder(&ClientDC, false);

	DrawTool(&ClientDC);

	if (m_SelectPoint == 1)
	{
		m_IndexPoint1.x += pt.x - m_pointCur.x;
		m_IndexPoint1.y += pt.y - m_pointCur.y;
	}
	else if (m_SelectPoint == 2)
	{
		m_IndexPoint2.x += pt.x - m_pointCur.x;
		m_IndexPoint2.y += pt.y - m_pointCur.y;
	}
	else if (m_SelectPoint == 3)
	{
		m_IndexPoint3.x += pt.x - m_pointCur.x;
		m_IndexPoint3.y += pt.y - m_pointCur.y;
	}
	
	DrawTool(&ClientDC);
	m_pointCur = pt;
	CToolBase::InflateHolder();
}

void CAndrewPF::ChangeToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	CToolBase::InflateHolder();
	m_pwndParent->Invalidate();
}

void CAndrewPF::CalMinMaxPoint()
{	
	if (m_IndexPoint1.x > m_IndexPoint2.x)
	{
		if (m_IndexPoint1.x > m_IndexPoint3.x)
		{
			m_MaxPoint = m_IndexPoint1;
			
			if (m_IndexPoint2.x > m_IndexPoint3.x)
			{
				m_MiddlePoint = m_IndexPoint2;
				m_MinPoint    = m_IndexPoint3;
			}
			else
			{
				m_MiddlePoint = m_IndexPoint3;
				m_MinPoint    = m_IndexPoint2;
			}
		}
	}
	else
	{
		if (m_IndexPoint2.x > m_IndexPoint3.x)
		{
			m_MaxPoint = m_IndexPoint2;
			
			if (m_IndexPoint1.x > m_IndexPoint3.x)
			{
				m_MiddlePoint = m_IndexPoint1;
				m_MinPoint    = m_IndexPoint3;
			}
			else
			{
				m_MiddlePoint = m_IndexPoint3;
				m_MinPoint    = m_IndexPoint1;
			}
		}
	}
	
	if (m_IndexPoint1.x > m_IndexPoint3.x)
	{
		if (m_IndexPoint1.x > m_IndexPoint2.x)
		{
			m_MaxPoint = m_IndexPoint1;
			
			if (m_IndexPoint2.x > m_IndexPoint3.x)
			{
				m_MiddlePoint = m_IndexPoint2;
				m_MinPoint    = m_IndexPoint3;
			}
			else
			{
				m_MiddlePoint = m_IndexPoint3;
				m_MinPoint    = m_IndexPoint2;
			}
		}
	}
	else
	{
		if (m_IndexPoint3.x > m_IndexPoint2.x)
		{
			m_MaxPoint = m_IndexPoint3;
			
			if (m_IndexPoint1.x > m_IndexPoint2.x)
			{
				m_MiddlePoint = m_IndexPoint1;
				m_MinPoint    = m_IndexPoint2;
			}
			else
			{
				m_MiddlePoint = m_IndexPoint2;
				m_MinPoint    = m_IndexPoint1;
			}
		}
	}
}

void CAndrewPF::OffsetCal()
{
	CPoint	PointGap;
	
	PointGap.x = abs((m_MaxPoint.x - m_MiddlePoint.x)) / 2;
	PointGap.y = abs((m_MaxPoint.y - m_MiddlePoint.y)) / 2;
	
	m_CenterPoint.x = (m_MaxPoint.x + m_MiddlePoint.x) / 2;
	m_CenterPoint.y = (m_MaxPoint.y + m_MiddlePoint.y) / 2;

	m_OffSetPointDown.x = m_MinPoint.x + PointGap.x;
	m_OffSetPointDown.y = m_MinPoint.y + PointGap.y;
	
	m_OffSetPointUp.x = m_MinPoint.x - PointGap.x;
	m_OffSetPointUp.y = m_MinPoint.y - PointGap.y;
}
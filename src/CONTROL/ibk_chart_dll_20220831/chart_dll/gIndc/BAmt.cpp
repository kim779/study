#include "stdafx.h"
#include "BAmt.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBAmt::CBAmt(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
: CBongBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	class COrgData *pOrgData;
	if (m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
	{
		m_iTtlDtCnt = pOrgData->GetArraySize();
		if (dispDay == 0)
			m_iDispDtCnt = m_iTtlDtCnt;
		else if (m_iTtlDtCnt < dispDay)
			m_iDispDtCnt = m_iTtlDtCnt;
		else
			m_iDispDtCnt = dispDay;

		EditMemberValue();
	}
	m_iValidDigit = 0;
}

CBAmt::~CBAmt()
{

}

void CBAmt::DrawGraph(CDC* pDC)
{
	m_RgnInfo.iCount = 0;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return;
	}

	if (pOrgData->GetPattern() != dpBASIC)
		return;

	if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_AMT)
	{
		m_dYMax = m_pCoDraw->AMax;
		m_dYMin = m_pCoDraw->AMin;
	}

	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
		return;

//	CBongBase::DrawTick(pDC);
	DrawChart(pDC);
}

void CBAmt::DrawChart(CDC* pDC)
{
	int	ii = 0, rgncnt = 0;
	CPoint	*pRgnPoint;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return;

	CGrpBasic* gBasic;
	for (ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
		if (gBasic->m_dTrdMoney != GD_UNUSED)
			break;
	}
	int nPoint = m_iDispEPos - m_iDispSPos - ii;
	if (nPoint <= 0)
		return;
	pRgnPoint = new CPoint[nPoint * 2];

	COLORREF	VolColor = m_pgraph->cInfo.aline[0].rgbLColor;
	CPen	*cPen = GetPen(PS_SOLID, 1, VolColor ^ m_crBG);
	CPen	*sPen = pDC->SelectObject(cPen);

	CBrush	*cBrush = GetBrush(VolColor ^ m_crBG);
	CBrush	*sBrush = pDC->SelectObject(cBrush);
	int	sROP = pDC->SetROP2(R2_XORPEN);

	COLORREF upColor = m_pgraph->cInfo.aline[1].rgbLColor ^ m_crBG;
	COLORREF dnColor = m_pgraph->cInfo.aline[2].rgbLColor ^ m_crBG;
	CPen	*upPen = GetPen(PS_SOLID, 1, upColor);
	CPen	*dnPen = GetPen(PS_SOLID, 1, dnColor);
	CBrush	*upBrush = GetBrush(upColor);
	CBrush	*dnBrush = GetBrush(dnColor);

	int DrawHeight = m_rectGrp.Height();
	int DrawWidth  = m_rectGrp.Width();

	double ValueHeight = m_dYMax - m_dYMin;
	if (ValueHeight == 0) 
		ValueHeight = 1;

	double	amt;
	double	dValue;
	CRect	rectTRN;

	bool	bFill = (m_pgraph->dwDraOption & GO_UNFILLVOLUME)? false:true;
	double	preVal = 0;
	if (m_pgraph->wCalOption & GC_VOLPRC)
	{
		for (; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);

			amt = gBasic->m_dTrdMoney;
			if (gBasic->m_iClose >= preVal)
			{
				pDC->SelectObject(upPen);
				pDC->SelectObject(upBrush);
			}
			else
			{
				pDC->SelectObject(dnPen);
				pDC->SelectObject(dnBrush);
			}
			preVal = gBasic->m_iClose;

			rectTRN.left = m_rectGrp.left;
			rectTRN.left += int((DrawWidth * ii) / m_iDispDtCnt);
			rectTRN.right = m_rectGrp.left;
			rectTRN.right += int((DrawWidth * (ii +1)) / m_iDispDtCnt);
			rectTRN.right -= DISTANCE_GAP;
			rectTRN.top = m_rectGrp.top;
			dValue = double(DrawHeight) * double(m_dYMax - amt);
			rectTRN.top += int(dValue / ValueHeight);
			rectTRN.bottom = m_rectGrp.bottom;

			rectTRN.right -= DISTANCE_GAP;
			if (rectTRN.Width() > 0)
			{
				CPoint	pt[5];

				pt[0].x = rectTRN.left; pt[0].y = rectTRN.top;
				pt[1].x = rectTRN.right; pt[1].y = rectTRN.top;
				pt[2].x = rectTRN.right; pt[2].y = rectTRN.bottom;
				pt[3].x = rectTRN.left; pt[3].y = rectTRN.bottom;
				pt[4].x = rectTRN.left; pt[4].y = rectTRN.top;
				if (bFill)
				{
					pDC->Polygon(pt, 5);
					if (rectTRN.top != rectTRN.bottom)
					{
						pDC->MoveTo(rectTRN.right-1, rectTRN.top);
						pDC->LineTo(rectTRN.left, rectTRN.top);
						pDC->LineTo(rectTRN.left, rectTRN.bottom);
					}
				}
				else
				{
					pDC->Polyline(pt, 5);
				}
			}
			else
			{
				pDC->MoveTo(rectTRN.left, rectTRN.top);
				pDC->LineTo(rectTRN.left, rectTRN.bottom);
			}

			pRgnPoint[rgncnt] = 
				CPoint((rectTRN.left + rectTRN.right)/2, (rectTRN.top + rectTRN.bottom)/2);
				
			rgncnt++;
		}
	}
	else if (m_pgraph->wCalOption & GC_VOLVOL)
	{
		for (; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);

			amt = gBasic->m_dTrdMoney;
			if (amt >= preVal)
			{
				pDC->SelectObject(upPen);
				pDC->SelectObject(upBrush);
			}
			else
			{
				pDC->SelectObject(dnPen);
				pDC->SelectObject(dnBrush);
			}
			preVal = amt;

			rectTRN.left = m_rectGrp.left;
			rectTRN.left += int((DrawWidth * ii) / m_iDispDtCnt);
			rectTRN.right = m_rectGrp.left;
			rectTRN.right += int((DrawWidth * (ii +1)) / m_iDispDtCnt);
			rectTRN.right -= DISTANCE_GAP;
			rectTRN.top = m_rectGrp.top;
			dValue = double(DrawHeight) * double(m_dYMax - amt);
			rectTRN.top += int(dValue / ValueHeight);
			rectTRN.bottom = m_rectGrp.bottom;

			rectTRN.right -= DISTANCE_GAP;
			if (rectTRN.Width() > 0)
			{
				CPoint	pt[5];

				pt[0].x = rectTRN.left; pt[0].y = rectTRN.top;
				pt[1].x = rectTRN.right; pt[1].y = rectTRN.top;
				pt[2].x = rectTRN.right; pt[2].y = rectTRN.bottom;
				pt[3].x = rectTRN.left; pt[3].y = rectTRN.bottom;
				pt[4].x = rectTRN.left; pt[4].y = rectTRN.top;
				if (bFill)
				{
					pDC->Polygon(pt, 5);
					if (rectTRN.top != rectTRN.bottom)
					{
						pDC->MoveTo(rectTRN.right-1, rectTRN.top);
						pDC->LineTo(rectTRN.left, rectTRN.top);
						pDC->LineTo(rectTRN.left, rectTRN.bottom);
					}
				}
				else
				{
					pDC->Polyline(pt, 5);
				}
			}
			else
			{
				pDC->MoveTo(rectTRN.left, rectTRN.top);
				pDC->LineTo(rectTRN.left, rectTRN.bottom);
			}

			pRgnPoint[rgncnt] = 
				CPoint((rectTRN.left + rectTRN.right)/2, (rectTRN.top + rectTRN.bottom)/2);
				
			rgncnt++;
		}
	}
	else if (m_pgraph->wCalOption & GC_VOLFPR)
	{
		for (; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);

			amt = gBasic->m_dTrdMoney;
			if (gBasic->m_iOpen < gBasic->m_iClose)
			{
				pDC->SelectObject(upPen);
				pDC->SelectObject(upBrush);
			}
			else if (gBasic->m_iOpen > gBasic->m_iClose)
			{
				pDC->SelectObject(dnPen);
				pDC->SelectObject(dnBrush);
			}
			else
			{
				int	epr = gBasic->m_iClose;
				bool	bUp = true;
				CGrpBasic* rBasic;
				for (int rr = ii+m_iDispSPos; rr >= m_iDispSPos; rr--)
				{
					rBasic = pOrgData->GetGraphData(rr);
					if (rBasic->m_iClose == epr)
						continue;
					if (rBasic->m_iClose > epr)
						bUp = false;
					else
						bUp = true;
					break;
				}
				if (bUp)
				{
					pDC->SelectObject(upPen);
					pDC->SelectObject(upBrush);
				}
				else
				{
					pDC->SelectObject(dnPen);
					pDC->SelectObject(dnBrush);
				}
			}

			rectTRN.left = m_rectGrp.left;
			rectTRN.left += int((DrawWidth * ii) / m_iDispDtCnt);
			rectTRN.right = m_rectGrp.left;
			rectTRN.right += int((DrawWidth * (ii +1)) / m_iDispDtCnt);
			rectTRN.right -= DISTANCE_GAP;
			rectTRN.top = m_rectGrp.top;
			dValue = double(DrawHeight) * double(m_dYMax - amt);
			rectTRN.top += int(dValue / ValueHeight);
			rectTRN.bottom = m_rectGrp.bottom;

			rectTRN.right -= DISTANCE_GAP;
			if (rectTRN.Width() > 0)
			{
				CPoint	pt[5];

				pt[0].x = rectTRN.left; pt[0].y = rectTRN.top;
				pt[1].x = rectTRN.right; pt[1].y = rectTRN.top;
				pt[2].x = rectTRN.right; pt[2].y = rectTRN.bottom;
				pt[3].x = rectTRN.left; pt[3].y = rectTRN.bottom;
				pt[4].x = rectTRN.left; pt[4].y = rectTRN.top;
				if (bFill)
				{
					pDC->Polygon(pt, 5);
					if (rectTRN.top != rectTRN.bottom)
					{
						pDC->MoveTo(rectTRN.right-1, rectTRN.top);
						pDC->LineTo(rectTRN.left, rectTRN.top);
						pDC->LineTo(rectTRN.left, rectTRN.bottom);
					}
				}
				else
				{
					pDC->Polyline(pt, 5);
				}
			}
			else
			{
				pDC->MoveTo(rectTRN.left, rectTRN.top);
				pDC->LineTo(rectTRN.left, rectTRN.bottom);
			}

			pRgnPoint[rgncnt] = 
				CPoint((rectTRN.left + rectTRN.right)/2, (rectTRN.top + rectTRN.bottom)/2);
				
			rgncnt++;
		}
	}
	else
	{
		for (; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);

			amt = gBasic->m_dTrdMoney;

			rectTRN.left = m_rectGrp.left;
			rectTRN.left += int((DrawWidth * ii) / m_iDispDtCnt);
			rectTRN.right = m_rectGrp.left;
			rectTRN.right += int((DrawWidth * (ii +1)) / m_iDispDtCnt);
			rectTRN.right -= DISTANCE_GAP;
			rectTRN.top = m_rectGrp.top;
			dValue = double(DrawHeight) * double(m_dYMax - amt);
			rectTRN.top += int(dValue / ValueHeight);
			rectTRN.bottom = m_rectGrp.bottom;

			rectTRN.right -= DISTANCE_GAP;
			if (rectTRN.Width() > 0)
			{
				CPoint	pt[5];

				pt[0].x = rectTRN.left; pt[0].y = rectTRN.top;
				pt[1].x = rectTRN.right; pt[1].y = rectTRN.top;
				pt[2].x = rectTRN.right; pt[2].y = rectTRN.bottom;
				pt[3].x = rectTRN.left; pt[3].y = rectTRN.bottom;
				pt[4].x = rectTRN.left; pt[4].y = rectTRN.top;
				if (bFill)
				{
					pDC->Polygon(pt, 5);
					if (rectTRN.top != rectTRN.bottom)
					{
						pDC->MoveTo(rectTRN.right-1, rectTRN.top);
						pDC->LineTo(rectTRN.left, rectTRN.top);
						pDC->LineTo(rectTRN.left, rectTRN.bottom);
					}
				}
				else
				{
					pDC->Polyline(pt, 5);
				}
			}
			else
			{
				pDC->MoveTo(rectTRN.left, rectTRN.top);
				pDC->LineTo(rectTRN.left, rectTRN.bottom);
			}

			pRgnPoint[rgncnt] = 
				CPoint((rectTRN.left + rectTRN.right)/2, (rectTRN.top + rectTRN.bottom)/2);
				
			rgncnt++;
		}
	}

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
	pDC->SetROP2(sROP);

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	if (rgncnt == 0)
	{
		pRgnPoint[0] = CPoint(0,0);
		pRgnPoint[1] = CPoint(1,1);
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, 2, WINDING);
	}
	else
	{
		for (int jj = 0; jj < rgncnt; jj++)
		{
			pRgnPoint[rgncnt*2 - jj - 1] = pRgnPoint[jj];
			pRgnPoint[jj].y += rgnLINEGAP;
			pRgnPoint[rgncnt*2 - jj - 1].y -= rgnLINEGAP;
		}
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, rgncnt * 2, WINDING);
	}

	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		CPoint	pointEPR;
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);
		for (int ii = 0; ii < 4; ii++)
		{
			int idx = (rgncnt) * (ii+1)/5;

			pointEPR.x = pRgnPoint[idx].x;
			pointEPR.y = pRgnPoint[idx].y - rgnMARKGAP;

			CRect	rc;
			rc.SetRect(pointEPR.x - rgnMARKGAP, pointEPR.y - rgnMARKGAP, 
				pointEPR.x + rgnMARKGAP, pointEPR.y + rgnMARKGAP);
			pDC->Rectangle(&rc);
		}
		pDC->SetROP2(sROP);
		pDC->SelectObject(srgnBrush);
	}

//	DrawTickIndex(pDC, "거래대금", VolColor, true);
	m_RgnInfo.iCount++;

	delete[] pRgnPoint;
	pRgnPoint = NULL;
}

bool CBAmt::CalculateMinMax(bool bForce)
{
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return false;

	bool	bRet = false;
	m_dYMin = DBL_MAX;	m_dYMax = DBL_MIN;
	if (m_dwDspOpt & DO_SCREEN)
		bRet = pOrgData->CalculateMinMax(m_iDispSPos, m_iDispEPos, &m_dYMin, &m_dYMax, 
		m_pgraph->wCalOption, cAMT, bForce);
	else
		bRet = pOrgData->CalculateMinMax(0, m_iTtlDtCnt, &m_dYMin, &m_dYMax,
		m_pgraph->wCalOption, cAMT, bForce);

	ReviseMinMax(m_dYMax, m_dYMin);
	return bRet;
}

bool CBAmt::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
{
	double	svMax = m_dYMax;
	double  svMin = m_dYMin;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return false;

	pOrgData->CalculateLastIndex(m_pgraph->wCalOption, cAMT);

	if (!CalculateMinMax(false))
			return false;

	if ((svMax != m_dYMax) || (svMin != m_dYMin))
		return true;

	return false;
}

CString CBAmt::GetDisplayPosData(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");
	
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData) || pOrgData->GetArraySize() <= 0)
		return "";

	int DrawWidth = m_rectGrp.Width();
	int xPosition = pt.x - m_rectGrp.left;

	int drawShift = 0;

	double szOneDay = double(DrawWidth) / double(m_iDispDtCnt);
	int dataPos = int(double(xPosition) / szOneDay);
	dataPos += m_iDispSPos;
	if (dataPos < 0)
		return _T("");
	if (dataPos >= m_iTtlDtCnt)
		dataPos = m_iTtlDtCnt-1;

	CString	unitStr = _T("");
	CGrpBasic* gBasic = pOrgData->GetGraphData(dataPos);

	CString	sAmt;
	double amt = gBasic->m_dTrdMoney;
	if (amt == GD_UNUSED)
		return unitStr;
	else
	{
		FormatVariableComma(sAmt, amt, 1);
		if (unitStr.IsEmpty())
		{
			if (bGrid)
				//unitStr.Format("$$거래대금:%sM", sAmt);
				unitStr.Format("$$거래대금:%s백만", sAmt);
			else
				//unitStr.Format("거래대금\t%sM", sAmt);
				unitStr.Format("거래대금\t%s백만", sAmt);
		}
	}

	return unitStr;
}

CString CBAmt::ExcelData(int idx)
{
	CString	dispStr = _T("");
	CString	name = _T("거래대금");

	CString	tmpS;
	if (idx < 0)
	{
			dispStr += name;
	}
	else
	{
		class COrgData *pOrgData;
		if (!m_pDataMgr->GetData(m_iDataKey, pOrgData) || pOrgData->GetArraySize() <= 0)
			return dispStr;

		CGrpBasic* gBasic = pOrgData->GetGraphData(idx);
		if (gBasic->m_dTrdMoney == GD_UNUSED)
			dispStr = _T("0");
		else
			dispStr.Format("%.fM", gBasic->m_dTrdMoney);
	}

	return dispStr;
}

void CBAmt::DrawLegend(CDC* pDC)
{
	m_RgnInfo.iCount = 0;
	DrawTickIndex(pDC, "거래대금", m_pgraph->cInfo.aline[0].rgbLColor, true);
	m_RgnInfo.iCount++;
}

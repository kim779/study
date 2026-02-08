#include "stdafx.h"
#include "BVolume.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBVolume::CBVolume(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
: CBongBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_iValidDigit = 0;
}

CBVolume::~CBVolume()
{

}

void CBVolume::DrawGraph(CDC* pDC)
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

	if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_VOL)
	{
		m_dYMax = m_pCoDraw->VMax;
		m_dYMin = m_pCoDraw->VMin;
	}

	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
		return;

//	CBongBase::DrawTick(pDC);
	DrawChart(pDC);
}

void CBVolume::DrawChart(CDC* pDC)
{
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return;

	int	ii = 0;
	CGrpBasic* pgbData = nullptr;
	for (ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		pgbData = pOrgData->GetGraphData(ii + m_iDispSPos);
		if (pgbData->m_dVolume != GD_UNUSED)
			break;
	}

	int iPointCount = m_iDispEPos - m_iDispSPos - ii;
	if (iPointCount <= 0)
		return;

	if (m_bEqui && !m_piVol)
		return;

	CPoint* ppointRgn = new CPoint[iPointCount * 2];

	COLORREF rgbVol = m_pgraph->cInfo.aline[0].rgbLColor;
	//CPen* ppenVol = GetPen(PS_SOLID, 1, rgbVol ^ m_crBG);
	CPen* ppenVol = GetPen(PS_SOLID, 1, rgbVol);
	CPen* ppenOld = pDC->SelectObject(ppenVol);

	//CBrush* pbrushVol = GetBrush(rgbVol ^ m_crBG);
	CBrush* pbrushVol = GetBrush(rgbVol);
	CBrush* pbrushOld = pDC->SelectObject(pbrushVol);
//	int iRopOld = pDC->SetROP2(R2_XORPEN);

	//COLORREF rgbUp = m_pgraph->cInfo.aline[1].rgbLColor ^ m_crBG;
	COLORREF rgbUp = m_pgraph->cInfo.aline[1].rgbLColor;
	//COLORREF rgbDown = m_pgraph->cInfo.aline[2].rgbLColor ^ m_crBG;
	COLORREF rgbDown = m_pgraph->cInfo.aline[2].rgbLColor;
	CPen* ppenUp = GetPen(PS_SOLID, 1, rgbUp);
	CPen* ppenDown = GetPen(PS_SOLID, 1, rgbDown);
	CBrush* pbrushUp = GetBrush(rgbUp);
	CBrush* pbrushDown = GetBrush(rgbDown);

	int iDrawHeight = m_rectGrp.Height();
	int iDrawWidth  = m_rectGrp.Width();

	double dValueHeight = m_dYMax - m_dYMin;
	if (dValueHeight == 0) 
		dValueHeight = 1;

	double	dEquiWidth = 0.0;
	if (m_dTotal)
		dEquiWidth = (double)iDrawWidth / (double)m_dTotal;

	bool bFill = (m_pgraph->dwDraOption & GO_UNFILLVOLUME)? false:true;
	int iCnt = 0;
	int iRgnCnt = 0;
	double dTrn;
	double dValue;
	double dLeft = m_rectGrp.left;
	double dRight = 0;
	double dPreVal = 0;
	CRect rectTrn;
	if (m_pgraph->wCalOption & GC_VOLPRC)
	{
		for (; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			pgbData = pOrgData->GetGraphData(ii+m_iDispSPos);
			dTrn = pgbData->m_dVolume;
			if (pgbData->m_iClose >= dPreVal)
			{
				pDC->SelectObject(ppenUp);
				pDC->SelectObject(pbrushUp);
			}
			else
			{
				pDC->SelectObject(ppenDown);
				pDC->SelectObject(pbrushDown);
			}
			dPreVal = pgbData->m_iClose;

			if (!m_bEqui)
			{
				rectTrn.left = m_rectGrp.left;
				rectTrn.left += int((iDrawWidth * ii) / m_iDispDtCnt);
				rectTrn.right = m_rectGrp.left;
				rectTrn.right += int((iDrawWidth * (ii +1)) / m_iDispDtCnt);
				rectTrn.right -= DISTANCE_GAP;
			}
			else
			{
				dRight = dLeft;
				dRight += m_piVol[ii] * dEquiWidth;
				int iWidth = int(dRight - dLeft);
				rectTrn.left = int(dLeft);
				if (iWidth <= 1)
					rectTrn.right = int(dRight);
				else if (iWidth > 1)
					rectTrn.right = int(dRight) - DISTANCE_GAP;
				iCnt += m_piVol[ii];
				dLeft = dRight;
			}

			rectTrn.top = m_rectGrp.top;
			dValue = double(iDrawHeight) * double(m_dYMax - dTrn);
			rectTrn.top += int(dValue / dValueHeight);
			rectTrn.bottom = m_rectGrp.bottom;

			if (!m_bEqui)
				rectTrn.right -= DISTANCE_GAP;
			else
			{
				if (rectTrn.right - rectTrn.left > DISTANCE_GAP)
					rectTrn.right -= DISTANCE_GAP;
			}

			if (rectTrn.Width() > 0)
			{
				CPoint apointVol[5];

				apointVol[0].x = rectTrn.left; apointVol[0].y = rectTrn.top;
				apointVol[1].x = rectTrn.right; apointVol[1].y = rectTrn.top;
				apointVol[2].x = rectTrn.right; apointVol[2].y = rectTrn.bottom;
				apointVol[3].x = rectTrn.left; apointVol[3].y = rectTrn.bottom;
				apointVol[4].x = rectTrn.left; apointVol[4].y = rectTrn.top;
				if (bFill)
				{
					pDC->Polygon(apointVol, 5);
					if (rectTrn.top != rectTrn.bottom)
					{
						pDC->MoveTo(rectTrn.right-1, rectTrn.top);
						pDC->LineTo(rectTrn.left, rectTrn.top);
						pDC->LineTo(rectTrn.left, rectTrn.bottom);
					}
				}
				else
				{
					pDC->Polyline(apointVol, 5);
				}
			}
			else
			{
				pDC->MoveTo(rectTrn.left, rectTrn.top);
				pDC->LineTo(rectTrn.left, rectTrn.bottom);
			}

			ppointRgn[iRgnCnt] = 
				CPoint((rectTrn.left + rectTrn.right) / 2, (rectTrn.top + rectTrn.bottom) / 2);
				
			iRgnCnt++;
		}
	}
	else if (m_pgraph->wCalOption & GC_VOLVOL)
	{
		for (; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			pgbData = pOrgData->GetGraphData(ii+m_iDispSPos);

			dTrn = pgbData->m_dVolume;
			if (dTrn >= dPreVal)
			{
				pDC->SelectObject(ppenUp);
				pDC->SelectObject(pbrushUp);
			}
			else
			{
				pDC->SelectObject(ppenDown);
				pDC->SelectObject(pbrushDown);
			}
			dPreVal = dTrn;

			if (!m_bEqui)
			{
				rectTrn.left = m_rectGrp.left;
				rectTrn.left += int((iDrawWidth * ii) / m_iDispDtCnt);
				rectTrn.right = m_rectGrp.left;
				rectTrn.right += int((iDrawWidth * (ii + 1)) / m_iDispDtCnt);
				rectTrn.right -= DISTANCE_GAP;
			}
			else
			{
				dRight = dLeft;
				dRight += m_piVol[ii] * dEquiWidth;
				int iWidth = int(dRight - dLeft);
				rectTrn.left = int(dLeft);
				if (iWidth <= 1)
					rectTrn.right = int(dRight);
				else if (iWidth > 1)
					rectTrn.right = int(dRight) - DISTANCE_GAP;
				iCnt += m_piVol[ii];
				dLeft = dRight;
			}

			rectTrn.top = m_rectGrp.top;
			dValue = double(iDrawHeight) * double(m_dYMax - dTrn);
			rectTrn.top += int(dValue / dValueHeight);
			rectTrn.bottom = m_rectGrp.bottom;

			if (!m_bEqui)
				rectTrn.right -= DISTANCE_GAP;
			else
			{
				if (rectTrn.right - rectTrn.left > DISTANCE_GAP)
					rectTrn.right -= DISTANCE_GAP;
			}

			if (rectTrn.Width() > 0)
			{
				CPoint	apointVol[5];

				apointVol[0].x = rectTrn.left; apointVol[0].y = rectTrn.top;
				apointVol[1].x = rectTrn.right; apointVol[1].y = rectTrn.top;
				apointVol[2].x = rectTrn.right; apointVol[2].y = rectTrn.bottom;
				apointVol[3].x = rectTrn.left; apointVol[3].y = rectTrn.bottom;
				apointVol[4].x = rectTrn.left; apointVol[4].y = rectTrn.top;
				if (bFill)
				{
					pDC->Polygon(apointVol, 5);
					if (rectTrn.top != rectTrn.bottom)
					{
						pDC->MoveTo(rectTrn.right-1, rectTrn.top);
						pDC->LineTo(rectTrn.left, rectTrn.top);
						pDC->LineTo(rectTrn.left, rectTrn.bottom);
					}
				}
				else
				{
					pDC->Polyline(apointVol, 5);
				}
			}
			else
			{
				pDC->MoveTo(rectTrn.left, rectTrn.top);
				pDC->LineTo(rectTrn.left, rectTrn.bottom);
			}

			ppointRgn[iRgnCnt] = 
				CPoint((rectTrn.left + rectTrn.right)/2, (rectTrn.top + rectTrn.bottom)/2);
				
			iRgnCnt++;
		}
	}
	else if (m_pgraph->wCalOption & GC_VOLFPR)
	{
		for (; ii < (m_iDispEPos - m_iDispSPos); ii++)
		{
			pgbData = pOrgData->GetGraphData(ii+m_iDispSPos);

			dTrn = pgbData->m_dVolume;
			if (pgbData->m_iOpen < pgbData->m_iClose)
			{
				pDC->SelectObject(ppenUp);
				pDC->SelectObject(pbrushUp);
			}
			else if (pgbData->m_iOpen > pgbData->m_iClose)
			{
				pDC->SelectObject(ppenDown);
				pDC->SelectObject(pbrushDown);
			}
			else
			{
				int iClose = pgbData->m_iClose;
				bool bUp = true;
				CGrpBasic* pgbR;
				for (int rr = ii + m_iDispSPos; rr >= m_iDispSPos; rr--)
				{
					pgbR = pOrgData->GetGraphData(rr);
					if (pgbR->m_iClose == iClose)
						continue;
					if (pgbR->m_iClose > iClose)
						bUp = false;
					else
						bUp = true;
					break;
				}

				if (bUp)
				{
					pDC->SelectObject(ppenUp);
					pDC->SelectObject(pbrushUp);
				}
				else
				{
					pDC->SelectObject(ppenDown);
					pDC->SelectObject(pbrushDown);
				}
			}

			if (!m_bEqui)
			{
				rectTrn.left = m_rectGrp.left;
				rectTrn.left += int((iDrawWidth * ii) / m_iDispDtCnt);
				rectTrn.right = m_rectGrp.left;
				rectTrn.right += int((iDrawWidth * (ii +1)) / m_iDispDtCnt);
				rectTrn.right -= DISTANCE_GAP;
			}
			else
			{
				dRight = dLeft;
				dRight += m_piVol[ii] * dEquiWidth;
				int iWidth = int(dRight - dLeft);
				rectTrn.left = int(dLeft);
				if (iWidth <= 1)
					rectTrn.right = int(dRight);
				else if (iWidth > 1)
					rectTrn.right = int(dRight) - DISTANCE_GAP;
				iCnt += m_piVol[ii];
				dLeft = dRight;
			}

			rectTrn.top = m_rectGrp.top;
			dValue = double(iDrawHeight) * double(m_dYMax - dTrn);
			rectTrn.top += int(dValue / dValueHeight);
			rectTrn.bottom = m_rectGrp.bottom;

			if (!m_bEqui)
				rectTrn.right -= DISTANCE_GAP;
			else
			{
				if (rectTrn.right - rectTrn.left > DISTANCE_GAP)
					rectTrn.right -= DISTANCE_GAP;
			}

			if (rectTrn.Width() > 0)
			{
				CPoint apointVol[5];

				apointVol[0].x = rectTrn.left; apointVol[0].y = rectTrn.top;
				apointVol[1].x = rectTrn.right; apointVol[1].y = rectTrn.top;
				apointVol[2].x = rectTrn.right; apointVol[2].y = rectTrn.bottom;
				apointVol[3].x = rectTrn.left; apointVol[3].y = rectTrn.bottom;
				apointVol[4].x = rectTrn.left; apointVol[4].y = rectTrn.top;
				if (bFill)
				{
					pDC->Polygon(apointVol, 5);
					if (rectTrn.top != rectTrn.bottom)
					{
						pDC->MoveTo(rectTrn.right-1, rectTrn.top);
						pDC->LineTo(rectTrn.left, rectTrn.top);
						pDC->LineTo(rectTrn.left, rectTrn.bottom);
					}
				}
				else
				{
					pDC->Polyline(apointVol, 5);
				}
			}
			else
			{
				pDC->MoveTo(rectTrn.left, rectTrn.top);
				pDC->LineTo(rectTrn.left, rectTrn.bottom);
			}

			ppointRgn[iRgnCnt] = 
				CPoint((rectTrn.left + rectTrn.right) / 2, (rectTrn.top + rectTrn.bottom) / 2);
				
			iRgnCnt++;
		}
	}
	else
	{
		for (; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			pgbData = pOrgData->GetGraphData(ii+m_iDispSPos);

			dTrn = pgbData->m_dVolume;

			if (!m_bEqui)
			{
				rectTrn.left = m_rectGrp.left;
				rectTrn.left += int((iDrawWidth * ii) / m_iDispDtCnt);
				rectTrn.right = m_rectGrp.left;
				rectTrn.right += int((iDrawWidth * (ii +1)) / m_iDispDtCnt);
				rectTrn.right -= DISTANCE_GAP;
			}
			else
			{
				dRight = dLeft;
				dRight += m_piVol[ii] * dEquiWidth;
				int iWidth = int(dRight - dLeft);
				rectTrn.left = int(dLeft);
				if (iWidth <= 1)
					rectTrn.right = int(dRight);
				else if (iWidth > 1)
					rectTrn.right = int(dRight) - DISTANCE_GAP;
				iCnt += m_piVol[ii];
				dLeft = dRight;
			}

			rectTrn.top = m_rectGrp.top;
			dValue = double(iDrawHeight) * double(m_dYMax - dTrn);
			rectTrn.top += int(dValue / dValueHeight);
			rectTrn.bottom = m_rectGrp.bottom;

			if (!m_bEqui)
				rectTrn.right -= DISTANCE_GAP;
			else
			{
				if (rectTrn.right - rectTrn.left > DISTANCE_GAP)
					rectTrn.right -= DISTANCE_GAP;
			}

			if (rectTrn.Width() > 0)
			{
				CPoint apointVol[5];

				apointVol[0].x = rectTrn.left; apointVol[0].y = rectTrn.top;
				apointVol[1].x = rectTrn.right; apointVol[1].y = rectTrn.top;
				apointVol[2].x = rectTrn.right; apointVol[2].y = rectTrn.bottom;
				apointVol[3].x = rectTrn.left; apointVol[3].y = rectTrn.bottom;
				apointVol[4].x = rectTrn.left; apointVol[4].y = rectTrn.top;
				if (bFill)
				{
					pDC->Polygon(apointVol, 5);
					if (rectTrn.top != rectTrn.bottom)
					{
						pDC->MoveTo(rectTrn.right-1, rectTrn.top);
						pDC->LineTo(rectTrn.left, rectTrn.top);
						pDC->LineTo(rectTrn.left, rectTrn.bottom);
					}
				}
				else
				{
					pDC->Polyline(apointVol, 5);
				}
			}
			else
			{
				pDC->MoveTo(rectTrn.left, rectTrn.top);
				pDC->LineTo(rectTrn.left, rectTrn.bottom);
			}

			ppointRgn[iRgnCnt] = 
				CPoint((rectTrn.left + rectTrn.right)/2, (rectTrn.top + rectTrn.bottom)/2);
				
			iRgnCnt++;
		}
	}

	pDC->SelectObject(ppenOld);
	pDC->SelectObject(pbrushOld);
//	pDC->SetROP2(iRopOld);

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	if (iRgnCnt == 0)
	{
		ppointRgn[0] = CPoint(0,0);
		ppointRgn[1] = CPoint(1,1);
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(ppointRgn, 2, WINDING);
	}
	else
	{
		for (int jj = 0; jj < iRgnCnt; jj++)
		{
			ppointRgn[iRgnCnt*2 - jj - 1] = ppointRgn[jj];
			ppointRgn[jj].y += rgnLINEGAP;
			ppointRgn[iRgnCnt*2 - jj - 1].y -= rgnLINEGAP;
		}
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(ppointRgn, iRgnCnt * 2, WINDING);
	}
	
	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		CPoint	pointEPR;
//		int iRopOld = pDC->SetROP2(R2_XORPEN);
		CBrush* pbrushRgn = GetBrush(_L_WHITE);
		CBrush* pbrushOld = pDC->SelectObject(pbrushRgn);
		for (int ii = 0; ii < 4; ii++)
		{
			int idx = (iRgnCnt) * (ii+1)/5;

			pointEPR.x = ppointRgn[idx].x;
			pointEPR.y = ppointRgn[idx].y - rgnMARKGAP;

			CRect rect;
			rect.SetRect(pointEPR.x - rgnMARKGAP, pointEPR.y - rgnMARKGAP, 
				pointEPR.x + rgnMARKGAP, pointEPR.y + rgnMARKGAP);
			pDC->Rectangle(&rect);
		}
//		pDC->SetROP2(iRopOld);
		pDC->SelectObject(pbrushOld);
	}

//	DrawTickIndex(pDC, "거래량", rgbVol, true);
	m_RgnInfo.iCount++;

	delete[] ppointRgn;
	ppointRgn = NULL;
}

bool CBVolume::CalculateMinMax(bool bForce)
{
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return false;

	bool	bRet = false;
	m_dYMin = DBL_MAX;
	m_dYMax = DBL_MIN;
	if (m_dwDspOpt & DO_SCREEN)
		bRet = pOrgData->CalculateMinMax(m_iDispSPos, m_iDispEPos, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cVOL, bForce);
	else
		bRet = pOrgData->CalculateMinMax(0, m_iTtlDtCnt, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cVOL, bForce);

	if (m_bEqui)
	{
		if (!CalculateVolMinMax())
			return false;

		CalculateVol();
	}

	ReviseMinMax(m_dYMax, m_dYMin);
	return bRet;
}

bool CBVolume::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
{
	double	svMax = m_dYMax;
	double  svMin = m_dYMin;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return false;

	pOrgData->CalculateLastIndex(m_pgraph->wCalOption, cVOL);

	if (!CalculateMinMax(false))
		return false;

	if ((svMax != m_dYMax) || (svMin != m_dYMin))
		return true;

	return false;
}

CString CBVolume::GetDisplayPosData(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");
	
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData) || pOrgData->GetArraySize() <= 0)
		return "";

	if (!m_bEqui)
	{
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

		CString	sTrn;
		double trn = gBasic->m_dVolume;
		if (trn == GD_UNUSED)
			return unitStr;
		else
		{
			FormatVariableComma(sTrn, trn, 1, 14);
			if (unitStr.IsEmpty())
			{
				if (m_iUnit == GU_INDEX)
				{
					if (bGrid)
						//unitStr.Format("$$거래량:%sK", sTrn);
						return "";

					else
						unitStr.Format("거래량\t%s천", sTrn);
				}
				else
				{
					if (bGrid)
						//unitStr.Format("$$거래량:%s", sTrn);
						return "";
					else
						unitStr.Format("거래량\t%s", sTrn);
				}
			}
		}

		return unitStr;
	}
	else
	{
		if (!m_piVol)
			return _T("");

		int DrawWidth  = m_rectGrp.Width();
		if (m_dTotal == 0.0)
			return _T("");

		double VolWidth = (double)DrawWidth / (double)m_dTotal;

		double	trn;
		
		CString	sTrn;
		CRect	rc;

		double	right = m_rectGrp.left;
		CString	unitStr = _T("");
		for (int ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			CGrpBasic* gBasic = pOrgData->GetGraphData(ii + m_iDispSPos);

			rc.left = int(right);
			right += m_piVol[ii] * VolWidth;
			rc.right = int(right) - DISTANCE_GAP;

			if (pt.x < rc.left || pt.x > rc.right)
				continue;

			trn = gBasic->m_dVolume;
			if (trn == GD_UNUSED)
				return unitStr;
			else
			{
				FormatVariableComma(sTrn, trn, 1, 14);
				if (m_iUnit == GU_INDEX)
				{
					if (bGrid)
						//unitStr.Format("$$거래량:%sK", sTrn);
						return "";
					else
						unitStr.Format("거래량\t%s천", sTrn);
				}
				else
				{
					if (bGrid)
						//unitStr.Format("$$거래량:%s", sTrn);
						return "";
					else
						unitStr.Format("거래량\t%s", sTrn);
				}
			}

			return unitStr;
		}
	}
	return _T("");
}

void CBVolume::DrawLegend(CDC* pDC)
{
	m_RgnInfo.iCount = 0;
	DrawTickIndex(pDC, "거래량", m_pgraph->cInfo.aline[0].rgbLColor, true);
	m_RgnInfo.iCount++;
}
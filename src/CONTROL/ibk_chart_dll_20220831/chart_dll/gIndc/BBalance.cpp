#include "stdafx.h"
#include "BBalance.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBBalance::CBBalance(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
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

	m_Switch = NULL;
	m_Pivot = NULL;
	m_BSpan = NULL;
	m_PSpan1 = NULL;
	m_PSpan2 = NULL;

	m_UnitSize1 = 9;
	m_UnitSize2 = 26;
	m_UnitSize3 = 26;
	m_UnitSize4 = 26;
	m_UnitSize5 = 52;
	m_bCalculated = false;
	SetMember();
}

CBBalance::~CBBalance()
{
	RemoveData();
}

void CBBalance::RemoveData()
{
	if (m_Switch)
	{
		delete[] m_Switch;	m_Switch = NULL;
	}
	if (m_Pivot)
	{
		delete[] m_Pivot;	m_Pivot = NULL;
	}
	if (m_BSpan)
	{
		delete[] m_BSpan;	m_BSpan = NULL;
	}
	if (m_PSpan1)
	{
		delete[] m_PSpan1;	m_PSpan1 = NULL;
	}
	if (m_PSpan2)
	{
		delete[] m_PSpan2;	m_PSpan2 = NULL;
	}
}

void CBBalance::DrawGraph(CDC* pDC)
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

	if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_PRC)
	{
		m_dYMax = m_pCoDraw->PMax;
		m_dYMin = m_pCoDraw->PMin;
	}
	
	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
		return;

//	CBongBase::DrawTick(pDC);
	DrawChart(pDC);
}

int CBBalance::GetYPos(double val)
{
	double	fMinMax = m_dYMax - m_dYMin;
	double	fHeight = m_rectGrp.Height();
	double	yPos = 0;
	double	fGab = m_dYMax - val;

	if (fMinMax <= 0.0)
		return m_rectGrp.bottom;

	yPos = fHeight * fGab / fMinMax;
	return (int)((double)m_rectGrp.top + yPos);
}

void CBBalance::SetMember()
{
	m_UnitSize1 = (int)m_pgraph->cInfo.awValue[0];
	m_UnitSize2 = (int)m_pgraph->cInfo.awValue[1];
	m_UnitSize3 = (int)m_pgraph->cInfo.awValue[2];
	m_UnitSize4 = (int)m_pgraph->cInfo.awValue[3];
	m_UnitSize5 = (int)m_pgraph->cInfo.awValue[4];

	if (m_UnitSize1 <= 0)	m_UnitSize1 = 9;
	if (m_UnitSize2 <= 0)	m_UnitSize2 = 26;
	if (m_UnitSize3 <= 0)	m_UnitSize3 = 26;
	if (m_UnitSize4 <= 0)	m_UnitSize4 = 26;
	if (m_UnitSize5 <= 0)	m_UnitSize5 = 52;

	memcpy(m_line, m_pgraph->cInfo.aline, sizeof(m_line));
}

bool CBBalance::CreateData()
{
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return false;
	}

	CGrpBasic* gBasic = NULL;

	if (m_Switch)
	{
		delete [] m_Switch;
		m_Switch = NULL;
	}

	m_Switch = new double[m_iTtlDtCnt + m_UnitSize4];
	for (int ii = 0; ii < m_iTtlDtCnt + m_UnitSize4; ii++)
		m_Switch[ii] = GD_UNUSED;
	
	if (m_Pivot)
	{
		delete [] m_Pivot;
		m_Pivot = NULL;
	}
	
	m_Pivot = new double[m_iTtlDtCnt + m_UnitSize4];

	for (int ii = 0; ii < m_iTtlDtCnt + m_UnitSize4; ii++)
		m_Pivot[ii] = GD_UNUSED;	

	if (m_BSpan)
	{
		delete [] m_BSpan;
		m_BSpan = NULL;
	}
	
	m_BSpan = new double[m_iTtlDtCnt + m_UnitSize4];
	for (int ii = 0; ii < m_iTtlDtCnt + m_UnitSize4; ii++)
		m_BSpan[ii] = GD_UNUSED;
	
	if (m_PSpan1)
	{
		delete [] m_PSpan1;
		m_PSpan1 = NULL;
	}

	m_PSpan1 = new double[m_iTtlDtCnt + m_UnitSize4];
	for (int ii = 0; ii < m_iTtlDtCnt + m_UnitSize4; ii++)
		m_PSpan1[ii] = GD_UNUSED;
	
	if (m_PSpan2)
	{
		delete [] m_PSpan2;
		m_PSpan2 = NULL;
	}

	m_PSpan2 = new double[m_iTtlDtCnt + m_UnitSize4];
	for (int ii = 0; ii < m_iTtlDtCnt + m_UnitSize4; ii++)
		m_PSpan2[ii] = GD_UNUSED;	

	int	StartInx = 0;

	// 전환선
	int	xMin, xMax;
	int	jj = 0;

	for (int ii = StartInx + m_UnitSize1 - 1; ii < m_iTtlDtCnt; ii++)
	{
		xMin = INT_MAX;
		xMax = INT_MIN;

		for (jj = ii; jj > ii - m_UnitSize1; jj--)
		{
			gBasic = pOrgData->GetGraphData(jj);

			if (gBasic->m_iHigh > xMax)
				xMax = gBasic->m_iHigh;
			if (gBasic->m_iLow < xMin)
				xMin = gBasic->m_iLow;
		}

		if (xMax == GD_UNUSED || xMin == GD_UNUSED)
			continue;
		else
			m_Switch[ii] = (xMax + xMin) / 2;
	}

	// 기준선
	for (int ii = StartInx + m_UnitSize2 - 1; ii < m_iTtlDtCnt; ii++)
	{
		xMin = INT_MAX;
		xMax = INT_MIN;

		for (jj = ii; jj > ii - m_UnitSize2; jj--)
		{
			gBasic = pOrgData->GetGraphData(jj);

			if (gBasic->m_iHigh > xMax)
				xMax = gBasic->m_iHigh;
			if (gBasic->m_iLow < xMin)
				xMin = gBasic->m_iLow;
		}

		if (xMax == GD_UNUSED || xMin == GD_UNUSED)
		{
			continue;
		}
		else
		{
			m_Pivot[ii] = (xMax + xMin) / 2;		
		}
	}

	// 후행스팬
	for (int ii = StartInx + m_UnitSize3 - 1; ii < m_iTtlDtCnt; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii);

		if (gBasic->m_iClose == GD_UNUSED)
			continue;
		else
			m_BSpan[ii - m_UnitSize3 + 1] = gBasic->m_iClose;
	}

	// 선행스팬1
	for (int ii = StartInx + m_UnitSize4 - 1 ; ii < m_iTtlDtCnt; ii++)
	{
		if (m_Pivot[ii] == GD_UNUSED || m_Switch[ii] == GD_UNUSED)
			continue;
		m_PSpan1[ii + m_UnitSize4 - 1] = (m_Switch[ii] + m_Pivot[ii]) / 2;
	}

	// 선행스팬2
	for (int ii = StartInx + m_UnitSize5 - 1; ii < m_iTtlDtCnt; ii++)
	{
		
		xMin = INT_MAX;
		xMax = INT_MIN;
		for (jj = ii; jj > ii - m_UnitSize5; jj--)
		{
			gBasic = pOrgData->GetGraphData(jj);

			if (gBasic->m_iHigh > xMax)	xMax = gBasic->m_iHigh;
			if (gBasic->m_iLow < xMin)	xMin = gBasic->m_iLow;
		}

		if (xMin == GD_UNUSED)
			continue;
		else
			m_PSpan2[ii + m_UnitSize4 - 1] = (xMax + xMin) / 2;
	}
	return true;
}

bool CBBalance::CalculateLast(bool bShift)
{
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return false;
	}
	
	int	ii = 0, jj = 0;

	if (bShift)
	{
		int	totSize = sizeof(double) * (m_iTtlDtCnt + m_UnitSize4);
		double*	pTemp = new double[m_iTtlDtCnt + m_UnitSize4];
		double*	ptr[] = {m_Switch, m_Pivot, m_BSpan, m_PSpan1, m_PSpan2};

		int	maxVal = (m_UnitSize1 > m_UnitSize2)? m_UnitSize1:m_UnitSize2;
		int	nLimit[] = {m_UnitSize1, m_UnitSize2, 0, (maxVal + m_UnitSize4 - 1), (m_UnitSize4 + m_UnitSize5 - 2)};
		int	nCount = sizeof(nLimit) / sizeof(int);

		for ( ii = 0 ; ii < nCount ; ii++ )
		{
			memset(pTemp, 0x00, totSize);
			memcpy(pTemp, &ptr[ii][1], totSize - sizeof(double));
			memcpy(ptr[ii], pTemp, totSize);

			for ( jj = 0 ; jj < nLimit[ii] ; jj++ )
			{
				ptr[ii][jj] = GD_UNUSED;
			}

			ptr[ii][m_iTtlDtCnt + m_UnitSize4 - 1] = GD_UNUSED;
		}
	}

	CGrpBasic* gBasic = NULL;
	int	StartInx = 0;

	// 전환선
	int	xMin = 0, xMax = 0;

	StartInx = m_iTtlDtCnt - m_UnitSize1;

	for (ii = StartInx + m_UnitSize1 - 1; ii < m_iTtlDtCnt; ii++)
	{
		xMin = INT_MAX;
		xMax = INT_MIN;

		for (jj = ii; jj > ii - m_UnitSize1; jj--)
		{
			gBasic = pOrgData->GetGraphData(jj);

			if (gBasic->m_iHigh > xMax)	xMax = gBasic->m_iHigh;
			if (gBasic->m_iLow < xMin)	xMin = gBasic->m_iLow;
		}

		if (xMax == GD_UNUSED || xMin == GD_UNUSED)
			continue;
		else
			m_Switch[ii] = (xMax + xMin) / 2;
	}

	// 기준선
	StartInx = m_iTtlDtCnt - m_UnitSize2;

	for (ii = StartInx + m_UnitSize2 - 1; ii < m_iTtlDtCnt; ii++)
	{
		xMin = INT_MAX;
		xMax = INT_MIN;

		for (jj = ii; jj > ii - m_UnitSize2; jj--)
		{
			gBasic = pOrgData->GetGraphData(jj);

			if (gBasic->m_iHigh > xMax)	xMax = gBasic->m_iHigh;
			if (gBasic->m_iLow < xMin)	xMin = gBasic->m_iLow;
		}

		if (xMax == GD_UNUSED || xMin == GD_UNUSED)
		{
			continue;
		}
		else
		{
			m_Pivot[ii] = (xMax + xMin) / 2;		
		}
	}

	// 후행스팬
	gBasic = pOrgData->GetGraphData(m_iTtlDtCnt - 1);
	if (gBasic->m_iClose != GD_UNUSED)
		m_BSpan[m_iTtlDtCnt - m_UnitSize3] = gBasic->m_iClose;

	// 선행스팬1
	if (m_Pivot[m_iTtlDtCnt - 1] != GD_UNUSED && m_Switch[m_iTtlDtCnt - 1] != GD_UNUSED)
		m_PSpan1[m_iTtlDtCnt + m_UnitSize4 - 2] = 
			(m_Switch[m_iTtlDtCnt - 1] + m_Pivot[m_iTtlDtCnt - 1]) / 2;

	// 선행스팬2
	xMin = INT_MAX;
	xMax = INT_MIN;
	for (jj = m_iTtlDtCnt - 1; jj > m_iTtlDtCnt - 1 - m_UnitSize5; jj--)
	{
		gBasic = pOrgData->GetGraphData(jj);

		if (gBasic->m_iHigh > xMax)	xMax = gBasic->m_iHigh;
		if (gBasic->m_iLow < xMin)	xMin = gBasic->m_iLow;
	}
	if (xMin != GD_UNUSED)
		m_PSpan2[m_iTtlDtCnt - 1 + m_UnitSize4 - 1] = (xMax + xMin) / 2;

	return true;
}

void CBBalance::DrawChart(CDC* pDC)
{
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return;
	}

	CPen	*cPen[5];
	CBrush	*cBrush[5];

	for ( int ii = 0 ; ii < 5 ; ii++ )
	{
		cPen[ii] = GetPen(m_line[ii].btLStyle, m_line[ii].btLWidth, m_line[ii].rgbLColor);
		cBrush[ii] = GetBrush(m_line[ii].rgbLColor);
	}
	 
	int DrawHeight = m_rectGrp.Height();
	int DrawWidth  = m_rectGrp.Width();

	double	Max = m_dYMax;
	double	Min = m_dYMin;
	if (IsLogChart())
	{
		Max = GetLogData(Max);
		Min = GetLogData(Min);
	}

	int ValueHeight = int(Max - Min);

	if (ValueHeight == 0) 
		ValueHeight = 1;

	double drawShift = 0;

	drawShift = (double)DrawWidth / (double)(m_iDispDtCnt + m_UnitSize4);

	double	xValue = double(DrawWidth) / double(m_iDispDtCnt + m_UnitSize4);
	double	yValue = double(DrawHeight) / double(ValueHeight);
	CPoint	pPoint(0,0);
	CPoint	cPoint(0,0);

	// 전환선
	CPen	*sPen = pDC->SelectObject(cPen[0]);
	CBrush	*sBrush = pDC->SelectObject(cBrush[0]);

	int	jj = 0, cnt = 0;
	double	dVal;

	CPoint	*pRgnPoint;
	const int titleL = 24;
	char	title[5][titleL];
	sprintf_s(title[0], titleL, "일목균형표 전환선[%d]", m_UnitSize1);
	sprintf_s(title[1], titleL, "기준선[%d]", m_UnitSize2);
	sprintf_s(title[2], titleL, "후행스팬[%d]", m_UnitSize3);
	sprintf_s(title[3], titleL, "선행스팬1[%d]", m_UnitSize4);
	sprintf_s(title[4], titleL, "선행스팬2[%d]", m_UnitSize5);
	double*	pValue[5] = {m_Switch, m_Pivot, m_BSpan, m_PSpan1, m_PSpan2};	

	int	drawEnd[5];
	drawEnd[0] = drawEnd[1] = m_iDispEPos - m_iDispSPos;
	drawEnd[2] = m_iDispEPos - m_iDispSPos - m_UnitSize3 + 1;
	drawEnd[3] = m_iDispEPos - m_iDispSPos + m_UnitSize4;
	drawEnd[4] = m_iDispEPos - m_iDispSPos + m_UnitSize4;
	for ( int ii = 0 ; ii < 5 ; ii++ )
	{
		cnt = 0;

		pPoint = cPoint = CPoint(0, 0);

		pRgnPoint = new CPoint[(m_iDispEPos - m_iDispSPos + m_UnitSize4) * 2];

		for ( jj = 0 ; jj < (m_iDispEPos - m_iDispSPos + m_UnitSize4) * 2 ; jj++ )
		{
			pRgnPoint[jj] = CPoint(0, 0);
		}

		pDC->SelectObject(cPen[ii]);
		pDC->SelectObject(cBrush[ii]);

		for ( jj = 0 ; jj < drawEnd[ii] ; jj++ )
		{
			if (pValue[ii][jj + m_iDispSPos] == GD_UNUSED)	continue;

			dVal = pValue[ii][jj + m_iDispSPos];

			if (IsLogChart())
				dVal = GetLogData(dVal);

			cPoint.x = (int)((double)m_rectGrp.left + drawShift / 2.0);
			cPoint.x += int(drawShift * (double)jj);
			cPoint.y = m_rectGrp.bottom;
			cPoint.y -= int(yValue * double(dVal - Min));
			pRgnPoint[cnt] = cPoint;
			pRgnPoint[cnt++].y = cPoint.y + rgnLINEGAP;
		
			if (pPoint == CPoint(0, 0))
			{
				pPoint = cPoint;
				pDC->MoveTo(pPoint);
				continue;
			}
			
			pDC->LineTo(cPoint);			
		}

		if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
		{
			int	sROP = pDC->SetROP2(R2_XORPEN);
			CBrush	*rgnBrush = GetBrush(_L_WHITE);
			CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);
			for ( jj = 0 ; jj < 4 ; jj++)
			{
				int idx = (m_iDispEPos - m_iDispSPos) * (jj + 1)/5;
				CGrpBasic* gBasic = pOrgData->GetGraphData(idx + m_iDispSPos);
				if (gBasic->m_iClose == GD_UNUSED)
					continue;

				cPoint = pRgnPoint[idx];

				CRect	rc;
				rc.SetRect(cPoint.x - rgnMARKGAP, cPoint.y - rgnMARKGAP, 
					cPoint.x + rgnMARKGAP, cPoint.y + rgnMARKGAP);
				rc.IntersectRect(&rc, &m_rectGrp);
				pDC->Rectangle(&rc);
			}

			pDC->SetROP2(sROP);
			pDC->SelectObject(srgnBrush);
		}

		if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
		{
			m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
			delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
			m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
		}

		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;

		if (cnt == 0)
		{
			pRgnPoint[0] = CPoint(0,0);
			pRgnPoint[1] = CPoint(1,1);
			m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, 2, WINDING);
		}
		else
		{
			for (jj = 0 ; jj < cnt ; jj++)
			{
				pRgnPoint[cnt*2 - jj - 1] = pRgnPoint[jj];
				pRgnPoint[cnt*2 - jj - 1].y -= rgnLINEGAP * 2;
			}

			m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, cnt * 2, WINDING);			
		}

		DrawTickIndex(pDC, title[ii], m_line[ii].rgbLColor, true);	
		delete[] pRgnPoint;	
		pRgnPoint = NULL;
		m_RgnInfo.iCount++;		
	}
	
	// 구름층

	CPen	*upPen = GetPen(m_line[5].btLStyle, m_line[5].btLWidth, m_line[5].rgbLColor);
	CPen	*dnPen = GetPen(m_line[6].btLStyle, m_line[6].btLWidth, m_line[6].rgbLColor);
	CBrush	*upBrush = GetBrush(m_line[5].rgbLColor);
	CBrush	*dnBrush = GetBrush(m_line[6].rgbLColor);
	for (int ii = 0; ii < m_iDispEPos - m_iDispSPos + m_UnitSize4 - 1; ii++)
	{
		if (m_PSpan1[ii + m_iDispSPos] == GD_UNUSED || 
			m_PSpan2[ii + m_iDispSPos] == GD_UNUSED)
			continue;

		dVal = m_PSpan1[ii + m_iDispSPos];
		if (IsLogChart())
			dVal = GetLogData(dVal);

		pPoint.x = (int)((double)m_rectGrp.left + (double)drawShift / 2.0);
		pPoint.x += int(drawShift * (double)ii);
		pPoint.y = m_rectGrp.bottom;
		pPoint.y -= int(yValue * double(dVal - Min));

		dVal = m_PSpan2[ii + m_iDispSPos];
		if (IsLogChart())
			dVal = GetLogData(dVal);

		cPoint.x = pPoint.x;
		cPoint.y = m_rectGrp.bottom;
		cPoint.y -= int(yValue * double(dVal - Min));

		if (pPoint.y <= cPoint.y)
		{
			pDC->SelectObject(upPen);
			pDC->SelectObject(upBrush);
		}
		else
		{
			pDC->SelectObject(dnPen);
			pDC->SelectObject(dnBrush);
		}

		pDC->MoveTo(pPoint);
		pDC->LineTo(cPoint);
	}

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
}

bool CBBalance::CalculateMinMax(bool bForce)
{
	SetMember();

	if (!m_bCalculated || bForce)
	{
		if (!CreateData())
			return false;

		m_bCalculated = true;
	}

	FindMinMax();
	ReviseMinMax(m_dYMax, m_dYMin);
	return true;
}

void CBBalance::FindMinMax()
{
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return;
	}

	double	tmpMax, tmpMin;

	// calc min/max
	m_dYMax = INT_MIN;
	m_dYMin = INT_MAX;

	int	start = m_iDispSPos;
	int	end = m_iDispEPos;

	if (m_dwDspOpt & DO_SCREEN)
	{
		start = m_iDispSPos;
		end = m_iDispEPos;
	}
	else
	{
		start = 0;
		end = m_iTtlDtCnt;
	}

	CGrpBasic* gBasic = NULL;
	
	for ( int ii = start ; ii < end ; ii++ )
	{
		gBasic = pOrgData->GetGraphData(ii);

		tmpMax = gBasic->m_iHigh;
		tmpMin = gBasic->m_iLow;

		if (tmpMax != GD_UNUSED && double(tmpMax) > m_dYMax)
			m_dYMax = double(tmpMax);
		if (tmpMin != GD_UNUSED && double(tmpMin) < m_dYMin)
			m_dYMin = double(tmpMin);

		if (m_Switch[ii] != GD_UNUSED && double(m_Switch[ii]) > m_dYMax)
			m_dYMax = double(m_Switch[ii]);
		if (m_Switch[ii] != GD_UNUSED && double(m_Switch[ii]) < m_dYMin)
			m_dYMin = double(m_Switch[ii]);

		if (m_Pivot[ii] != GD_UNUSED && double(m_Pivot[ii]) > m_dYMax)
			m_dYMax = double(m_Pivot[ii]);
		if (m_Pivot[ii] != GD_UNUSED && double(m_Pivot[ii]) < m_dYMin)
			m_dYMin = double(m_Pivot[ii]);

		if (m_BSpan[ii] != GD_UNUSED && double(m_BSpan[ii]) > m_dYMax)
			m_dYMax = double(m_BSpan[ii]);
		if (m_BSpan[ii] != GD_UNUSED && double(m_BSpan[ii]) < m_dYMin)
			m_dYMin = double(m_BSpan[ii]);

		if (m_PSpan1[ii] != GD_UNUSED && double(m_PSpan1[ii]) > m_dYMax)
			m_dYMax = double(m_PSpan1[ii]);
		if (m_PSpan1[ii] != GD_UNUSED && double(m_PSpan1[ii]) < m_dYMin)
			m_dYMin = double(m_PSpan1[ii]);

		if (m_PSpan2[ii] != GD_UNUSED && double(m_PSpan2[ii]) > m_dYMax)
			m_dYMax = double(m_PSpan2[ii]);
		if (m_PSpan2[ii] != GD_UNUSED && double(m_PSpan2[ii]) < m_dYMin)
			m_dYMin = double(m_PSpan2[ii]);
	}

	for (int ii = end; ii < end + m_UnitSize4; ii++)
	{
		if (m_PSpan1[ii] != GD_UNUSED && double(m_PSpan1[ii]) > m_dYMax)
			m_dYMax = double(m_PSpan1[ii]);
		if (m_PSpan1[ii] != GD_UNUSED && double(m_PSpan1[ii]) < m_dYMin)
			m_dYMin = double(m_PSpan1[ii]);

		if (m_PSpan2[ii] != GD_UNUSED && double(m_PSpan2[ii]) > m_dYMax)
			m_dYMax = double(m_PSpan2[ii]);
		if (m_PSpan2[ii] != GD_UNUSED && double(m_PSpan2[ii]) < m_dYMin)
			m_dYMin = double(m_PSpan2[ii]);
	}
}

bool CBBalance::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
{
	double	svMax = m_dYMax;
	double  svMin = m_dYMin;

	CalculateLast(bShift);

	if (!CalculateMinMax(false))
		return false;

	if ((svMax != m_dYMax) || (svMin != m_dYMin))
		return true;

	return false;
}

CString CBBalance::GetDisplayPosData(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");
	
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData) || pOrgData->GetArraySize() <= 0)
		return "";

	int DrawWidth = m_rectGrp.Width();
	int xPosition = pt.x - m_rectGrp.left;

	int drawShift = 0;

	double szOneDay = double(DrawWidth) / double(m_iDispDtCnt + m_UnitSize4);
	int dataPos = int(double(xPosition) / szOneDay);
	dataPos += m_iDispSPos;
	if (dataPos < 0)
		return _T("");
	if (dataPos >= (m_iTtlDtCnt + m_UnitSize4))
		dataPos = (m_iTtlDtCnt + m_UnitSize4)-1;

	const int titleL = 24;
	char	title[5][titleL];

	sprintf_s(title[0], titleL, "전환선[%d]", m_UnitSize1);
	sprintf_s(title[1], titleL, "기준선[%d]", m_UnitSize2);
	sprintf_s(title[2], titleL, "후행스팬[%d]", m_UnitSize3);
	sprintf_s(title[3], titleL, "선행스팬1[%d]", m_UnitSize4);
	sprintf_s(title[4], titleL, "선행스팬2[%d]", m_UnitSize5);

	CString	unitStr = _T("");	
	CString tmpS = _T(""), sVal = _T("");
	double val[5];

	val[0] = m_Switch[dataPos];
	val[1] = m_Pivot[dataPos];
	val[2] = m_BSpan[dataPos];
	val[3] = m_PSpan1[dataPos];
	val[4] = m_PSpan2[dataPos];

	if (dataPos >= m_iDispEPos - m_UnitSize3 + 1)
		val[2] = GD_UNUSED;

	char	split = ':';
	if (!bGrid) split = '\t';

	int ii = 0;
	for ( ii = 0 ; ii < 2 ; ii++ )	// 전환선,기준선
	{
		if (val[ii] == GD_UNUSED || dataPos >= m_iDispEPos)
			continue;		

		FormatVariableComma(sVal, val[ii], 100);
		tmpS.Format("%s%c%s", title[ii], split, sVal);

		if (!unitStr.IsEmpty())
			unitStr += "\t" + tmpS;
		else
			unitStr = tmpS;
	}

	for (; ii < 5 ; ii++ )
	{
		if (val[ii] == GD_UNUSED)
			continue;		

		FormatVariableComma(sVal, val[ii], 100);
		tmpS.Format("%s%c%s", title[ii], split, sVal);

		if (!unitStr.IsEmpty())
			unitStr += "\t" + tmpS;
		else
			unitStr = tmpS;
	}

	return unitStr;
}

CString CBBalance::ExcelData(int idx)
{

	CString	dispStr = _T(""), tmpS = _T("");
	const	int titleL = 24;
	char	title[5][titleL];

	sprintf_s(title[0], titleL, "전환선[%d]",	m_UnitSize1);
	sprintf_s(title[1], titleL, "기준선[%d]",	m_UnitSize2);
	sprintf_s(title[2], titleL, "후행스팬[%d]",	m_UnitSize3);
	sprintf_s(title[3], titleL, "선행스팬1[%d]",	m_UnitSize4);
	sprintf_s(title[4], titleL, "선행스팬2[%d]",	m_UnitSize5);

	double*	value[] = {m_Switch, m_Pivot, m_BSpan, m_PSpan1, m_PSpan2};

	if (idx < 0)
	{
		for ( int ii = 0 ; ii < 5 ; ii++ )
		{
			if (ii != 0)
				dispStr += _T("\t");
			
			dispStr += title[ii];			
		}		
	}
	else
	{	
		for ( int ii = 0 ; ii < 5 ; ii++ )
		{
			if (ii != 0)
				dispStr += _T("\t");

			double	val = value[ii][idx];

			if (val == GD_UNUSED)
				tmpS = _T("0");
			else
				FormatVariableComma(tmpS, val, 0, 0);

			dispStr += tmpS;
		}
	}
	return dispStr;	
}

CString CBBalance::GetXposValue(CPoint pt)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData) || pOrgData->GetArraySize() <= 0)
		return "";

	int DrawWidth = m_rectGrp.Width();
	int xPosition = pt.x - m_rectGrp.left;

	int drawShift = 0;

	double szOneDay = double(DrawWidth) / double(m_iDispDtCnt + m_UnitSize4);
	int dataPos = int(double(xPosition) / szOneDay);
	dataPos += m_iDispSPos;
	if (dataPos < 0)
		return _T("");
	if (dataPos >= (m_iTtlDtCnt + m_UnitSize4))
		dataPos = (m_iTtlDtCnt + m_UnitSize4)-1;

	CString	unitStr = _T("");
	if (m_Pivot[dataPos] == GD_UNUSED)
		return unitStr;

	CGrpBasic* gBasic = pOrgData->GetGraphData(dataPos);
	if (gBasic->m_iClose == GD_UNUSED)	return _T("");

	switch (GetDataIndex())
	{
	case GI_MINUTE:
	case GI_TICK:
		unitStr.Format("%02d/%02d %02d:%02d:%02d", 
			gBasic->m_index.date.mm, gBasic->m_index.date.dd,
			gBasic->m_index.time.hh, gBasic->m_index.time.mm, gBasic->m_index.time.ss);
		break;
	default:
		unitStr.Format("%04d/%02d/%02d", 
			gBasic->m_index.date.yy, gBasic->m_index.date.mm, gBasic->m_index.date.dd);
		break;
	}

	return unitStr;
}

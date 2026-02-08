// Samsun.cpp: implementation of the CSamsun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VSamsun.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVSamsun::CVSamsun(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
: CVarsBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_pLocal	= NULL;
	m_pSamsunData	= NULL;
	m_pUpDnInfo	= NULL;
	m_pXpos = NULL;
	m_JWValue = 0.0;
	m_nShow = 50;
	m_fUnit = 0.0;
	m_bCalculated = false;
}

CVSamsun::~CVSamsun()
{
	if (m_pLocal != NULL)
	{
		delete[] m_pLocal;	m_pLocal = NULL;
	}
	if (m_pSamsunData != NULL)
	{
		delete[] m_pSamsunData;	m_pSamsunData = NULL;
	}
	if (m_pUpDnInfo != NULL)
	{
		delete[] m_pUpDnInfo;	m_pUpDnInfo = NULL;
	}

	if (m_pXpos)
	{
		delete[] m_pXpos;	m_pXpos = NULL;
	}
}

#include <math.h>
void CVSamsun::GetValueToPoint(CPoint& pt, double dXValue, double dYValue)
{
	pt = CPoint(-1, -1);

	// 2006.12.27 schbang 데이터 없을시 죽는걸 막기위해
	if (m_CurveN < 1)
	{
		return;
	}
	
	double dMax = m_dYMax;
	double dMin = m_dYMin;

	if (IsLogChart())
	{
		dMax = GetLogData(dMax);
		dMin = GetLogData(dMin);
		dYValue = GetLogData(dYValue);
	}
	
	CRect rectDraw = m_rectGrp;
	if (m_iGrpKind == GK_BALANCE)
	{
		int iBalanceGap = GetValue();
		if (iBalanceGap > 0)
//			rectDraw.right -= int(castDVAL(m_rectGrp.Width()*iBalanceGap) / castDVAL(m_iDispDtCnt + iBalanceGap) );
			rectDraw.right -= castIVAL( castDVAL(m_rectGrp.Width()) * castDVAL(iBalanceGap) / (castDVAL(m_iDispDtCnt) + castDVAL(iBalanceGap)) );
	}
	
	double dGap = castDVAL(m_iDispEPos) - castDVAL(m_iDispSPos);

	if (dGap == 0)
		dGap = 1;
	
//	double dX = (dXValue - m_iDispSPos) * rectDraw.Width() / dGap;
	struct	_local_data*	pObject = NULL;
	int iShow = m_CurveN;
	double dX = 0;
	double	dMValue = dXValue;

	m_fUnit = (double)m_rectGrp.Width() / iShow;
	for ( int ii = m_iViweSidx; ii <= m_iViweEidx ; ii++ )
	{
		pObject = &m_pLocal[ii];
		if(ii < m_iTtlDtCnt && pObject->dSDay <= dMValue && dMValue < pObject->dEDay)
		{
			dX = (castDVAL(ii) - castDVAL(m_iViweSidx)) * m_fUnit + (m_fUnit / 2);
			break;
		}
	}

	dGap = dMax - dMin;
	if (dGap == 0)
		dGap = 1;	
	
	double dY;
	if (IsReverseChart())
		dY = rectDraw.top + (dYValue - dMin) * rectDraw.Height() / dGap;
	else
		dY = rectDraw.bottom - (dYValue - dMin) * rectDraw.Height() / dGap;		
	
	pt.x = int(dX + rectDraw.left);
	pt.y = int(dY);
}

void CVSamsun::GetPtToPosValue(CPoint pt, double& dPos, double& dValue)
{
	double dMax = m_dYMax;
	double dMin = m_dYMin;

	// 2006.12.27 schbang 데이터 없을시 죽는걸 막기위해
	if (m_CurveN < 1)
	{
		dPos = 0;
		dValue = 0;
		return;
	}


	int iDispCount = GetDisplayEnd() - GetDisplayPos();
	CRect rectDraw = m_rectGrp;
	if (m_iGrpKind == GK_BALANCE)
	{
		int iBalanceGap = GetValue();
		if (iBalanceGap > 0)
			rectDraw.right -= int( (castDVAL(m_rectGrp.Width()) * castDVAL(iBalanceGap) ) / ( castDVAL(m_iDispDtCnt) + castDVAL(iBalanceGap) ) );
	}
	
	int		ixPos;
	ixPos = (int)( (castDVAL(pt.x) - castDVAL(rectDraw.left) ) / m_fUnit); //* iDispCount) / double(rectDraw.Width());
	ixPos += GetDisplayPos();
	
	struct	_local_data*	pObject = NULL;
	if(ixPos >= m_CurveN)
		ixPos = m_CurveN -1;
	if(ixPos < 0)
		ixPos = 0;

	if(ixPos < m_iTtlDtCnt )
	{
		pObject = &m_pLocal[ixPos];
		dPos = pObject->dMDay;
	}

	int iDrawHeight = rectDraw.Height();
	
	if (IsLogChart())
	{
		dMax = GetLogData(dMax);
		dMin = GetLogData(dMin);
	}
	
	double dValueHeight = dMax - dMin;
	double dPosValue = double(dValueHeight) / double(iDrawHeight);
	dPosValue *= double(rectDraw.bottom - double(pt.y));
	
	if (IsReverseChart())
		dPosValue = dMax - dPosValue;
	else
		dPosValue = dMin + dPosValue;
	
	if (IsLogChart())
		dPosValue = exp(dPosValue / 1000.);
	
	dValue = dPosValue;
}

void CVSamsun::GetPtToEndvalPt(CPoint pt, CPoint &ept)
{
	double dPos, dValue;
	GetPtToPosValue(pt, dPos, dValue);
	int	 iXIdx = GetValueToXIdx(dPos);
	ept = pt;
	struct	_local_data*	pObject = NULL;

	if(iXIdx < m_iTtlDtCnt)
	{
		pObject = &m_pLocal[iXIdx];
		ept.y = GetYPos(pObject->hpr);
	}
	
}

CString CVSamsun::GetIndexStrTool(int index, int &fpr, int &hpr, int &lpr, int &epr)
{
	if (index < 0)
		return _T("");
	

	if (index >= m_CurveN)
		index = m_CurveN-1;
	
	struct	_local_data*	pObject = NULL;

	if(index > m_iTtlDtCnt)
		return _T("");
	pObject = &m_pLocal[index];
	
	fpr = (int)pObject->lpr;
	hpr = (int)pObject->hpr;
	lpr = (int)pObject->lpr;
	epr = (int)pObject->hpr;
	
	return _T("");
}

double CVSamsun::GetOneDataSize()
{
	
	return ((double)m_rectGrp.Width() / m_CurveN);
	
}

int	CVSamsun::GetValueToXIdx(double dXValue)
{
	struct	_local_data*	pObject = NULL;
	int m_iShow = m_CurveN;
	double dX = 0;
	double	dMValue = dXValue;
	
	m_fUnit = (double)m_rectGrp.Width() / m_iShow;
	for ( int ii = m_iViweSidx; ii <= m_iViweEidx ; ii++ )
	{
		pObject = &m_pLocal[ii];
		if(ii < m_iTtlDtCnt && pObject->dSDay <= dMValue && dMValue < pObject->dEDay)
		{
			return (ii);
		}
	}
	return 0;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void CVSamsun::DrawGraph(CDC* pDC)
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

	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
		return;

//	CVarsBase::DrawTick(pDC);

	if ((m_pgraph->dwDraOption & GO_UNION))
	{
		DrawUnionChart(pDC);
		DrawBottomStick(pDC);
	}
	else
		DrawChart(pDC);
}

void CVSamsun::DrawChart(CDC* pDC)
{
	if (!m_pSamsunData)
		return;

	CPen	*upPen = GetPen(PS_SOLID, 1, m_UpperColor);
	CPen	*dnPen = GetPen(PS_SOLID, 1, m_LowerColor);
	CPen	*sPen = pDC->SelectObject(upPen);

	CBrush	*upBrush = GetBrush(m_UpperColor);
	CBrush	*dnBrush = GetBrush(m_LowerColor);
	CBrush	*sBrush = pDC->SelectObject(upBrush);

	COLORREF Ycolor = RGB(196, 196, 0);
	COLORREF Gcolor = RGB(192,  0, 192);

	int	CurState = m_pUpDnInfo[0];
	int	Year = 0;
	int	ii = m_StartInx;
	int	vHeight = m_pSamsunData[ii*2];

	CPoint	pt;
	CRect	DrawRect;
	CString	dispText;
	CArray <SAMSUNTICK,SAMSUNTICK>	Stick;
	int	nPoint = m_CurveN - m_StartInx;

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	CPoint	*pRgnPoint = new CPoint[nPoint*2];
	CPoint	ptTmp[5];

	double	nXpos = (double)m_rectGrp.left;
	
	for (int jj = 0; ii < m_CurveN; ii++, jj++)
	{
//		DrawRect = CRect(m_pXpos[jj], GetYPos(m_pSamsunData[ii*2]),	m_pXpos[jj + 1], GetYPos(m_pSamsunData[ii*2 + 1]));
		DrawRect = CRect((int)nXpos, GetYPos(m_pSamsunData[ii*2]),	
			(int)(nXpos + m_fUnit ), GetYPos(m_pSamsunData[ii*2 + 1]));
		
		
		DrawRect.NormalizeRect();

		// Save date
		if (m_pUpDnInfo[ii] != CurState)
		{
			CurState = m_pUpDnInfo[ii];

			if (ii != m_StartInx)
			{
				if (Year != m_pLocal[ii].date.date.yy)
				{
					Year = m_pLocal[ii].date.date.yy;
					dispText.Format("%d.%d.%d", Year,m_pLocal[ii].date.date.mm, 
						m_pLocal[ii].date.date.dd);
				}
				else
				{
					dispText.Format("%d.%d", m_pLocal[ii].date.date.mm, 
						m_pLocal[ii].date.date.dd);
				}
			}
			else
				dispText = _T("");

			SAMSUNTICK tick;
			int cHeight;

			if (CurState == INC)
			{
				pt.x = (long)(nXpos - m_fUnit + 6);
				pt.y = GetYPos(m_pSamsunData[ ii*2 - 1]);
				tick.PenColor = Ycolor;
			}
			else
			{
				pt.x = (long)(nXpos - m_fUnit + 6);
				pt.y = GetYPos(m_pSamsunData[ ii*2 - 1]) - 15;
				tick.PenColor = Gcolor;
			}

			cHeight = GetYPos(m_pSamsunData[ ii*2 - 1]);

			if ( m_TkInfo.tkHeight[TICK_BOTTOM] * 3 < abs(vHeight - cHeight) )
			{
				tick.Context = dispText;
				tick.xyPoint = pt;
				Stick.Add(tick);
			}
			vHeight = cHeight;
		}
		
		ptTmp[4] = ptTmp[0] = CPoint(DrawRect.left, DrawRect.bottom);
		ptTmp[1] = CPoint(DrawRect.right, ptTmp[0].y);
		ptTmp[2] = CPoint(ptTmp[1].x, DrawRect.top);
		ptTmp[3] = CPoint(ptTmp[0].x, ptTmp[2].y);

		// draw region
		if (m_pUpDnInfo[ii] == INC)
		{
			pDC->SelectObject(upPen);
			pDC->SelectObject(upBrush);

			
			if (m_pgraph->dwDraOption & GO_FIILUPBONG)
				pDC->Polygon(ptTmp, 5);					
			else					
				pDC->Polyline(ptTmp, 5);			
		}
		else
		{
			pDC->SelectObject(dnPen);
			pDC->SelectObject(dnBrush);

			if (m_pgraph->dwDraOption & GO_FIILDNBONG)					
				pDC->Polygon(ptTmp, 5);					
			else					
				pDC->Polyline(ptTmp, 5);			
		}

		DrawRect.NormalizeRect();
		pt = CPoint((DrawRect.left+DrawRect.right)/2, DrawRect.top);
		pRgnPoint[jj] = pt;
		pt = CPoint((DrawRect.left+DrawRect.right)/2, DrawRect.bottom);
		pRgnPoint[nPoint*2 - jj - 1] = pt;
		nXpos += m_fUnit;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, nPoint * 2, WINDING);

	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);
		for (int ii = 0; ii < 5; ii++)
		{
			int idx = (nPoint) * (ii+1)/6;
			CPoint	pt = pRgnPoint[idx];

			CRect	rc;
			rc.SetRect(pt.x - rgnMARKGAP, pt.y - rgnMARKGAP, 
				pt.x + rgnMARKGAP, pt.y + rgnMARKGAP);
			rc.IntersectRect(&rc, &m_rectGrp);
			pDC->Rectangle(&rc);
		}
		pDC->SetROP2(sROP);
		pDC->SelectObject(srgnBrush);
	}
/*
	// draw date
	if (GetDataIndex() == GI_DAY)
	{
		for (ii = 0; ii < Stick.GetSize(); ii++)
		{
			SAMSUNTICK tick = Stick.GetAt(ii);
			DrawTickIndex(pDC, tick.Context, tick.PenColor, false, true, tick.xyPoint);
		}
	}
*/
	CString	str;
	FormatVariableComma(str, m_JWValue, m_dYMax);
	dispText.Format("삼선전환도 전환가격[%s]", str.GetString());
	DrawTickIndex(pDC, dispText, m_pgraph->rgbLabel, true);

	m_RgnInfo.iCount++;

	delete[] pRgnPoint;	

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
}

void CVSamsun::DrawUnionChart(CDC* pDC)
{
	if (!m_pSamsunData)
		return;

	CheckMinMax();
	
	if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_PRC)
	{
		m_dYMax = m_pCoDraw->PMax;
		m_dYMin = m_pCoDraw->PMin;
	}
	
//	int iPointCnt = m_iViweOrgDataCnt;
	int iPointCnt = m_iDispEPos - m_iDispSPos;
	int	nSize  = m_nShow;
	int	nIndex = 0;
	int	nWidth = m_rectGrp.Width();
	m_fUnit = (double)m_rectGrp.Width() / iPointCnt;

	CPen	*upPen = GetPen(PS_SOLID, 2, m_UpperColor);
	CPen	*dnPen = GetPen(PS_SOLID, 2, m_LowerColor);
	CPen	*sPen = pDC->SelectObject(upPen);

	CBrush	*upBrush = GetBrush(m_UpperColor);
	CBrush	*dnBrush = GetBrush(m_LowerColor);
	CBrush	*sBrush = pDC->SelectObject(upBrush);

	COLORREF Ycolor = RGB(196, 196, 0);
	COLORREF Gcolor = RGB(192,  0, 192);

	int	CurState = m_pUpDnInfo[0];
	int	Year = 0;
	int	ii = m_iViweSidx, jj;
	int	vHeight = m_pSamsunData[ii*2];

	CPoint	pt;
	CRect	DrawRect;
	CString	dispText;
	CArray <SAMSUNTICK,SAMSUNTICK>	Stick;
	int	nPoint = m_CurveN - m_StartInx;

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	CPoint	*pRgnPoint = new CPoint[nPoint*2];
	CPoint	ptTmp[5];

	int	iViewOrg_Sidx, iViewOrg_Eidx, iGap;
	
	iViewOrg_Sidx = m_OrgInDataStart;
	iViewOrg_Eidx = m_iTtlDtCnt;
	if(iViewOrg_Sidx < m_iDispSPos)
		iViewOrg_Sidx = m_iDispSPos;
	if(iViewOrg_Eidx > m_iDispEPos)
		iViewOrg_Eidx = m_iDispEPos;
	
	double	nXpos = (double)m_rectGrp.left;
	double	ixval,  old_ixval , old_nXpos;

	if(m_OrgInDataStart > 0 && m_iDispSPos < m_OrgInDataStart)
	{
		nXpos += (m_fUnit * (castDVAL(m_OrgInDataStart) - castDVAL(m_iDispSPos)));
	}
	old_nXpos = nXpos;
	ixval = old_ixval = 0;
	m_arPointX.Add((int)nXpos);

	for (ii = m_iViweSidx, jj = 0; ii <= m_iViweEidx; ii++, jj++)
	{
//m_pLocal[m_CurveN].iGap
		iGap = m_pLocal[ii].iGap;		
		if( ii == m_iViweSidx && m_pLocal[ii].iGap > 0 &&
			m_pLocal[ii].iSOrgidx <= iViewOrg_Sidx && iViewOrg_Sidx <= m_pLocal[ii].iEOrgidx )
			iGap = m_pLocal[ii].iEOrgidx -  iViewOrg_Sidx + 1;
		if(ii == m_iViweEidx && m_pLocal[ii].iGap > 0 && 
			m_pLocal[ii].iSOrgidx <= iViewOrg_Eidx && iViewOrg_Eidx <= m_pLocal[ii].iEOrgidx )
			iGap = iViewOrg_Eidx - m_pLocal[ii].iSOrgidx + 1;
		ixval = m_fUnit * iGap;

		//		if(m_pLocal[ii].lpr < m_dYMin)
		//			continue;
		//		if(m_pLocal[ii].hpr > m_dYMax)
		//			continue;
		
		if(nWidth <= nXpos)
			continue;
		if(nWidth < (int)(nXpos + ixval))
			ixval = nWidth - nXpos;

		
		DrawRect = CRect((int)nXpos, GetYPos(m_pSamsunData[ii*2]), (int)(nXpos + ixval ), GetYPos(m_pSamsunData[ii*2 + 1]));
		DrawRect.NormalizeRect();

		// Save date
		if (m_pUpDnInfo[ii] != CurState)
		{
			CurState = m_pUpDnInfo[ii];

			SAMSUNTICK tick;
			int cHeight;

			if (CurState == INC)
			{
				pt.x = (long)(nXpos - m_fUnit + 6);
				pt.y = GetYPos(m_pSamsunData[ ii*2 - 1]);
				tick.PenColor = Ycolor;
			}
			else
			{
				pt.x = (long)(nXpos - m_fUnit + 6);
				pt.y = GetYPos(m_pSamsunData[ ii*2 - 1]) - 15;
				tick.PenColor = Gcolor;
			}

			cHeight = GetYPos(m_pSamsunData[ ii*2 - 1]);

			if ( m_TkInfo.tkHeight[TICK_BOTTOM] * 3 < abs(vHeight - cHeight) )
			{
				tick.Context = dispText;
				tick.xyPoint = pt;
				Stick.Add(tick);
			}
			vHeight = cHeight;
		}
		
		ptTmp[4] = ptTmp[0] = CPoint(DrawRect.left, DrawRect.bottom);
		ptTmp[1] = CPoint(DrawRect.right, ptTmp[0].y);
		ptTmp[2] = CPoint(ptTmp[1].x, DrawRect.top);
		ptTmp[3] = CPoint(ptTmp[0].x, ptTmp[2].y);

		// draw region
		if (m_pUpDnInfo[ii] == INC)
		{
			pDC->SelectObject(upPen);
			pDC->SelectObject(upBrush);
			m_arColorX.Add(m_pgraph->cInfo.aline[0].rgbLColor);
		}
		else
		{
			pDC->SelectObject(dnPen);
			pDC->SelectObject(dnBrush);
			m_arColorX.Add(m_pgraph->cInfo.aline[1].rgbLColor);
		}

		if(iGap != m_pLocal[ii].iGap)
		{
			pDC->MoveTo(ptTmp[3].x,ptTmp[3].y);
			if(ii == m_iViweSidx)
				pDC->MoveTo(ptTmp[0].x,ptTmp[0].y);
			else
				pDC->LineTo(ptTmp[0].x,ptTmp[0].y);
			pDC->LineTo(ptTmp[1].x,ptTmp[1].y);
			if(ii == m_iViweEidx)
				pDC->MoveTo(ptTmp[2].x,ptTmp[2].y);
			else
				pDC->LineTo(ptTmp[2].x,ptTmp[2].y);
			pDC->LineTo(ptTmp[3].x,ptTmp[3].y);	
		}
		else		
			pDC->Polyline(ptTmp, 5);
		
		DrawRect.NormalizeRect();
		pt = CPoint((DrawRect.left+DrawRect.right)/2, DrawRect.top);
		pRgnPoint[jj] = pt;
		pt = CPoint((DrawRect.left+DrawRect.right)/2, DrawRect.bottom);
		pRgnPoint[nPoint*2 - jj - 1] = pt;
		nXpos += ixval;
		m_arPointX.Add((int)nXpos);
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, nPoint * 2, WINDING);

	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);
		for (int ii = 0; ii < 5; ii++)
		{
			int idx = (nPoint) * (ii+1)/6;
			CPoint	pt = pRgnPoint[idx];

			CRect	rc;
			rc.SetRect(pt.x - rgnMARKGAP, pt.y - rgnMARKGAP, 
				pt.x + rgnMARKGAP, pt.y + rgnMARKGAP);
			rc.IntersectRect(&rc, &m_rectGrp);
			pDC->Rectangle(&rc);
		}
		pDC->SetROP2(sROP);
		pDC->SelectObject(srgnBrush);
	}

//	CString	str;
//	FormatVariableComma(str, m_JWValue, m_dYMax);
//	dispText.Format("삼선전환도 전환가격[%s]", str);
//	DrawTickIndex(pDC, dispText, m_pgraph->rgbLabel, true);

	m_RgnInfo.iCount++;

	delete[] pRgnPoint;	

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
}

bool CVSamsun::CreateData()
{
	CGrpBasic* pObject = NULL;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return false;
	}

	if (!(m_pgraph->dwDraOption & GO_UNION))
	{
		m_iTtlDtCnt = pOrgData->GetArraySize();//TEST KANG
		m_iDispEPos = m_iTtlDtCnt;
	}
	

	int Start = 0;
	for (Start = 0; Start < m_iTtlDtCnt; Start++)
	{
		pObject = pOrgData->GetGraphData(Start);

		if (pObject->m_iClose != GD_UNUSED)
			break;
	}
	
	if (Start > m_iTtlDtCnt - 2)
		return false;
	m_OrgInDataStart = Start;

	if (m_pSamsunData)
	{
		delete [] m_pSamsunData;
		m_pSamsunData = NULL;
	}

	m_pSamsunData	= new int[m_iTtlDtCnt*2];
	
	if (m_pLocal)
	{
		delete [] m_pLocal;
		m_pLocal = NULL;
	}
	m_pLocal	= new _local_data[m_iTtlDtCnt];

	if (m_pUpDnInfo)
	{
		delete [] m_pUpDnInfo;
		m_pUpDnInfo = NULL;
	}

	m_pUpDnInfo	= new int[m_iTtlDtCnt];

	for ( int ii = 0 ; ii < m_iTtlDtCnt ; ii++ )
	{
		m_pSamsunData[ii] = -1;
		m_pUpDnInfo[ii] = -1;
	}

	int	IncLvl = 0, DecLvl = 0;
	int	DnBuf[3],UpBuf[3];	// 상승,하락의 삼선값
	int	PreVal = 0;		// 유효한 이전값
	int	CurVal = 0;		// 현재값
	int	LastVal = 0;	// 추세 전환의 마지막값,반전의 시작값
	int	CurState = EMPTY;	// 현재 흐름(EMPTY,상승,하락)
	int	UpSwitch, DnSwitch;
	int	iGap =0;
	int	iSOrgidx;
	double		dSDay, dEDay, dMDay;
	CGrpBasic*	pBas = NULL;

	pObject	= pOrgData->GetGraphData(Start);
	PreVal	= pObject->m_iClose;
	m_CurveN = 0;

	ZeroMemory(DnBuf, sizeof(DnBuf));
	ZeroMemory(UpBuf, sizeof(UpBuf));

	for ( int ii = Start + 1 ; ii < m_iTtlDtCnt ; ii++ )
	{
		pObject = pOrgData->GetGraphData(ii);
		CurVal = pObject->m_iClose;

		if(iGap == 0)
		{
				iSOrgidx = (ii >= 1) ? ii -1 : ii;
		}
		iGap++;
		switch (GetState(PreVal, CurVal))
		{
		case INC:
			switch (CurState)
			{
			case DEC:	// 상승세로 전환

				if (DecLvl == 3)
					UpSwitch = DnBuf[0];
				else
					UpSwitch = GetSwitchVal(true);

				if (UpSwitch < CurVal) //삼선상승
				{
					IncLvl = 1;
					m_pSamsunData[m_CurveN*2] = LastVal;
					m_pSamsunData[m_CurveN*2 + 1] = CurVal;
					memcpy(&m_pLocal[m_CurveN].date, &pObject->m_index, sz_gIndex);
					m_pLocal[m_CurveN].fpr = (double)pObject->m_iOpen;
					m_pLocal[m_CurveN].hpr = (double)pObject->m_iHigh;
					m_pLocal[m_CurveN].lpr = (double)pObject->m_iLow;
					m_pLocal[m_CurveN].epr = (double)pObject->m_iClose;
					m_pLocal[m_CurveN].iGap = iGap; iGap = 0;
					m_pLocal[m_CurveN].iSOrgidx = iSOrgidx;
					m_pLocal[m_CurveN].iEOrgidx = ii;//(ii >= 1) ? ii -1 : ii;
//kangmc추세선변경
					pBas = pOrgData->GetGraphData(iSOrgidx);
					dSDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
					dSDay = (dSDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
					pBas = pOrgData->GetGraphData(ii);
					dEDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
					dEDay = (dEDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
					pBas = pOrgData->GetGraphData((iSOrgidx + ii) / 2 );
					dMDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
					dMDay = (dMDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
					
					m_pLocal[m_CurveN].dSDay = dSDay;
					m_pLocal[m_CurveN].dEDay = dEDay;
					m_pLocal[m_CurveN].dMDay = dMDay;
					
					m_pUpDnInfo[m_CurveN++] = INC;
					UpBuf[0] = LastVal;
					PreVal = CurVal;
					CurState = INC;
				}
				break;
			case INC:	// 상승세
			case EMPTY:	// 시작시 발생
				if (IncLvl != 3)
				{
					UpBuf[IncLvl] = LastVal = PreVal;
					IncLvl++;
				}
				else
				{
					UpBuf[0] = UpBuf[1];
					UpBuf[1] = UpBuf[2];
					UpBuf[2] = LastVal = PreVal;
				}

				memcpy(&m_pLocal[m_CurveN].date, &pObject->m_index, sz_gIndex);
				m_pLocal[m_CurveN].fpr = (double)pObject->m_iOpen;
				m_pLocal[m_CurveN].hpr = (double)pObject->m_iHigh;
				m_pLocal[m_CurveN].lpr = (double)pObject->m_iLow;
				m_pLocal[m_CurveN].epr = (double)pObject->m_iClose;
				m_pLocal[m_CurveN].iGap = iGap; iGap = 0;
				m_pLocal[m_CurveN].iSOrgidx = iSOrgidx;
				m_pLocal[m_CurveN].iEOrgidx = ii; //(ii >= 1) ? ii -1 : ii;
				pBas = pOrgData->GetGraphData(iSOrgidx);
				dSDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
				dSDay = (dSDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
				pBas = pOrgData->GetGraphData(ii);
				dEDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
				dEDay = (dEDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
				pBas = pOrgData->GetGraphData((iSOrgidx + ii) / 2 );
				dMDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
				dMDay = (dMDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
				m_pLocal[m_CurveN].dSDay = dSDay;
				m_pLocal[m_CurveN].dEDay = dEDay;
				m_pLocal[m_CurveN].dMDay = dMDay;
				
				m_pSamsunData[m_CurveN*2] = PreVal;
				m_pSamsunData[m_CurveN*2 + 1] = CurVal;
				m_pUpDnInfo[m_CurveN++] = INC;
				PreVal = CurVal;
				CurState = INC;
				break;
			}
			break;	
		case DEC:
			switch (CurState)
			{
			case INC:	// 하락세로 전환
				if (IncLvl == 3)
					DnSwitch = UpBuf[0];
				else
					DnSwitch = GetSwitchVal(false);

				if (DnSwitch > CurVal) //삼선하락
				{
					DecLvl = 1;
					m_pSamsunData[m_CurveN*2] = LastVal;
					m_pSamsunData[m_CurveN*2 + 1] = CurVal;
					memcpy(&m_pLocal[m_CurveN].date, &pObject->m_index, sz_gIndex);
					m_pLocal[m_CurveN].fpr = (double)pObject->m_iOpen;
					m_pLocal[m_CurveN].hpr = (double)pObject->m_iHigh;
					m_pLocal[m_CurveN].lpr = (double)pObject->m_iLow;
					m_pLocal[m_CurveN].epr = (double)pObject->m_iClose;
					m_pLocal[m_CurveN].iGap = iGap; iGap = 0;
					m_pLocal[m_CurveN].iSOrgidx = iSOrgidx;
					m_pLocal[m_CurveN].iEOrgidx = ii; //(ii >= 1) ? ii -1 : ii;
					pBas = pOrgData->GetGraphData(iSOrgidx);
					dSDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
					dSDay = (dSDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
					pBas = pOrgData->GetGraphData(ii);
					dEDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
					dEDay = (dEDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
					pBas = pOrgData->GetGraphData((iSOrgidx + ii) / 2 );
					dMDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
					dMDay = (dMDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
					m_pLocal[m_CurveN].dSDay = dSDay;
					m_pLocal[m_CurveN].dEDay = dEDay;
					m_pLocal[m_CurveN].dMDay = dMDay;
					
					m_pUpDnInfo[m_CurveN++] = DEC;
					DnBuf[0] = LastVal;
					PreVal = CurVal;
					CurState = DEC;
				}
				break;
			case DEC:	// 하락세
			case EMPTY:	// 시작시 발생
				if (DecLvl != 3)
				{
					DnBuf[DecLvl] = LastVal = PreVal;
					DecLvl++;				
				}
				else
				{
					DnBuf[0] = DnBuf[1];
					DnBuf[1] = DnBuf[2];
					DnBuf[2] = LastVal = PreVal;
				}

				memcpy(&m_pLocal[m_CurveN].date, &pObject->m_index, sz_gIndex);
				m_pLocal[m_CurveN].fpr = (double)pObject->m_iOpen;
				m_pLocal[m_CurveN].hpr = (double)pObject->m_iHigh;
				m_pLocal[m_CurveN].lpr = (double)pObject->m_iLow;
				m_pLocal[m_CurveN].epr = (double)pObject->m_iClose;
				m_pLocal[m_CurveN].iGap = iGap; iGap = 0;
				m_pLocal[m_CurveN].iSOrgidx = iSOrgidx;
				m_pLocal[m_CurveN].iEOrgidx = ii; //(ii >= 1) ? ii -1 : ii;
				pBas = pOrgData->GetGraphData(iSOrgidx);
				dSDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
				dSDay = (dSDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
				pBas = pOrgData->GetGraphData(ii);
				dEDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
				dEDay = (dEDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
				pBas = pOrgData->GetGraphData((iSOrgidx + ii) / 2 );
				dMDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
				dMDay = (dMDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
				m_pLocal[m_CurveN].dSDay = dSDay;
				m_pLocal[m_CurveN].dEDay = dEDay;
				m_pLocal[m_CurveN].dMDay = dMDay;
				
				m_pSamsunData[m_CurveN*2] = PreVal;
				m_pSamsunData[m_CurveN*2 + 1] = CurVal;
				m_pUpDnInfo[m_CurveN++] = DEC;
				PreVal = CurVal;
				CurState = DEC;
				break;
			}
			break;
		default:
			PreVal = CurVal;
			break;	
		}	
	}
	
	return true;
}

bool CVSamsun::CalculateMinMax(bool bForce)
{
	SetMember();

	if (!m_bCalculated || bForce)
	{
		if (!CreateData())
			return false;
		m_bCalculated = true;
	}
	CheckMinMax();
	int nCount = m_CurveN;

	if (nCount <= 0)
	{
		m_fUnit = 0.0;
		return false;
	}

	int iPointCnt = m_iDispEPos - m_iDispSPos;

	int	nWidth = m_rectGrp.Width();
	m_nShow = nCount;
	m_fUnit = (double)nWidth / (m_nShow);

	// X좌표 계산
	if (m_pXpos)
	{
		delete [] m_pXpos;
		m_pXpos = NULL;
	}	m_pXpos = new int[nCount + 1];
	for (int ii = 0 ; ii < nCount + 1 ; ii++ )
	{
		m_pXpos[ii] = m_rectGrp.left + (int)(ii*m_fUnit) + 1;
	}

//	if ( (m_CurveN - nCount) >= 0 )
//		m_StartInx = m_CurveN - nCount;
//	else 
		m_StartInx = m_iViweSidx;


	if (m_pUpDnInfo[m_CurveN - 1] == DEC)
		m_JWValue = GetSwitchVal(true);
	else
		m_JWValue = GetSwitchVal(false);


	FindMinMax(&m_pSamsunData[m_StartInx*2], (m_iViweDataCnt)*2);
	ReviseMinMax(m_dYMax, m_dYMin);		
	return true;
}


bool CVSamsun::CheckMinMax()
{
	int	ii, nSize;
	
	nSize = m_CurveN;
	
//	if (m_OrgInDataStart > nSize - 2)
//		return false;

	
	int	iViewOrg_Sidx, iViewOrg_Eidx;
	
	iViewOrg_Sidx = m_OrgInDataStart;
	iViewOrg_Eidx = m_iTtlDtCnt;
	if(iViewOrg_Sidx < m_iDispSPos)
		iViewOrg_Sidx = m_iDispSPos;
	if(iViewOrg_Eidx > m_iDispEPos)
		iViewOrg_Eidx = m_iDispEPos;
	
	m_iViweOrgDataCnt = iViewOrg_Eidx - iViewOrg_Sidx;
	
	m_iViweDataCnt = 0;
	m_iViweSidx = 0;
	m_iViweEidx = 0;
	
	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		if(m_iViweDataCnt == 0 && m_pLocal[ii].iEOrgidx > iViewOrg_Sidx)
		{
			m_iViweSidx = ii;
			m_iViweDataCnt++;
			continue;
		}
		if(m_iViweDataCnt != 0 && m_pLocal[ii].iSOrgidx < iViewOrg_Eidx)
		{
			m_iViweEidx = ii;
			m_iViweDataCnt++;
			continue;
		}
	}
	
	return true;
}

int CVSamsun::GetSwitchVal(bool bUp)
{
	int Inx = m_CurveN * 2 - 1;
	int ret;

	if (bUp)
	{
		ret = INT_MIN;
		
		while ( Inx != -1 && Inx >= m_CurveN * 2 - 6)
		{
			if (ret < m_pSamsunData[Inx])
				ret = m_pSamsunData[Inx];
			Inx--;
		}
	}
	else
	{
		ret = INT_MAX;

		while ( Inx != -1 && Inx >= m_CurveN * 2 - 6)
		{
			if (ret > m_pSamsunData[Inx])
				ret = m_pSamsunData[Inx];
			Inx--;
		}
	}

	return ret;
}


int CVSamsun::GetYPos(double val)
{
	double	fMinMax = m_dYMax - m_dYMin;
	double	fHeight = m_rectGrp.Height();
	double	yPos = 0;
	double	fGab = m_dYMax - val;
	double	Max = m_dYMax;
	double	Min = m_dYMin;
	double	posValue = val;
	
	if (fMinMax <= 0.0)
		return m_rectGrp.bottom;

	if (IsLogChart())
	{
		Max = GetLogData(Max);
		Min = GetLogData(Min);
		posValue = GetLogData(val);
	}
	
	yPos = fHeight * (Max - posValue) / (Max - Min);
	
	if (IsReverseChart())
		posValue = (double)m_rectGrp.bottom - yPos;
	else
		posValue =  (double)m_rectGrp.top + yPos;
	
	return (int) posValue;
}

void CVSamsun::FindMinMax(int *pArray, int Size)
{
	m_dYMin = castDVAL(INT_MAX);
	m_dYMax = castDVAL(INT_MIN);

	for (int ii = 0; ii < Size; ii++)
	{
		if (m_dYMax < pArray[ii])
			m_dYMax = pArray[ii];

		if (m_dYMin > pArray[ii])
			m_dYMin = pArray[ii];
	}
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return ;
	
	if (m_dwDspOpt & DO_SCREEN && m_pgraph->dwDraOption & GO_UNION)
		pOrgData->CalculateMinMax(m_iDispSPos, m_iDispEPos, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cPRC, TRUE);
	else
		pOrgData->CalculateMinMax(0, m_iTtlDtCnt, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cPRC, TRUE);
	
}

int CVSamsun::GetState(int PreVal, int CurVal)
{
	if (PreVal > CurVal)
		return DEC;
	else if (PreVal < CurVal)
		return INC;
	else
		return EMPTY;
}

bool CVSamsun::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
{
	double	svMax = m_dYMax;
	double  svMin = m_dYMin;

	CreateData();

	if (!CalculateMinMax(false))
		return false;

	if ((svMax != m_dYMax) || (svMin != m_dYMin))
		return true;

	return false;
}

CString CVSamsun::GetDisplayPosHeader(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)	return _T("");

	if (m_CurveN <= 0)	return _T("");

	struct _local_data* pObject = NULL;
	int	nIndex = -1;

	int	nGab = (pt.x - m_rectGrp.left);
	
	if (nGab < 0)	return _T("");
	
	nIndex = (int)((double)nGab / m_fUnit);
	if (m_CurveN > m_nShow)	nIndex += m_CurveN - m_nShow;
	if (nIndex < 0 || nIndex >= m_CurveN)	return _T("");
	

//	for ( int ii = 1 + m_StartInx; ii < m_CurveN + 1; ii++ )
//	{
//		if (pt.x >= m_pXpos[ii - 1 - m_StartInx] && pt.x < m_pXpos[ii - m_StartInx])
//		{
//			nIndex = ii - 1;
//			break;
//		}
//	}

	
	if (nIndex < 0)	return _T("");
	
	CString	unitStr1 = _T("");
	pObject = &m_pLocal[nIndex];
	int	Start, End;

	Start = m_pSamsunData[nIndex*2];
	End = m_pSamsunData[nIndex*2 + 1];

	if (bGrid)
	{
		switch (GetDataIndex())
		{
		case GI_MINUTE:
		case GI_TICK:
			if (pObject->date.date.dd == 0)
				unitStr1.Format("~~시간:%02d:%02d:%02d\t", pObject->date.time.hh,
								pObject->date.time.mm, pObject->date.time.ss);
			else
				unitStr1.Format("~~시간:%02d/%02d-%02d:%02d:%02d\t", 
					pObject->date.date.mm, pObject->date.date.dd,
					pObject->date.time.hh, pObject->date.time.mm, pObject->date.time.ss);
			break;
		default:
			unitStr1.Format("~~일자:%04d/%02d/%02d\t", pObject->date.date.yy,
				pObject->date.date.mm, pObject->date.date.dd);
			break;
		}
	}
	else
	{
		switch (GetDataIndex())
		{
		case GI_MINUTE:
		case GI_TICK:
			unitStr1.Format("시간\t%02d:%02d:%02d\t", pObject->date.time.hh,
					pObject->date.time.mm, pObject->date.time.ss);
			break;
		default:
			unitStr1.Format("일자\t%04d/%02d/%02d\t", pObject->date.date.yy,
				pObject->date.date.mm, pObject->date.date.dd);
			break;
		}
	}

	return unitStr1;
}

CString CVSamsun::GetDisplayPosData(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	if (m_CurveN <= 0)
		return _T("");

	struct _local_data* pObject = NULL;
	int	nIndex = -1;

	int	nGab = (pt.x - m_rectGrp.left);
	
	if (nGab < 0)	return _T("");
	
	nIndex = (int)((double)nGab / m_fUnit);
	if (m_CurveN > m_nShow)	nIndex += m_CurveN - m_nShow;
	if (nIndex < 0 || nIndex >= m_CurveN)	return _T("");
	
//	for ( int ii = 1 + m_StartInx; ii < m_CurveN + 1; ii++ )
//	{
//		if (pt.x >= m_pXpos[ii - 1 - m_StartInx] && pt.x < m_pXpos[ii - m_StartInx])
//		{
//			nIndex = ii - 1;
//			break;
//		}
//	}
	
	if (nIndex < 0)
		return _T("");
	
	CString	unitStr1 = _T("");
	pObject = &m_pLocal[nIndex];
	int	Start, End;

	Start = m_pSamsunData[nIndex * 2];
	End = m_pSamsunData[nIndex * 2 + 1];

	CString	sFpr, sHpr, sLpr, sEpr, sStart, sEnd;
	if (pObject->epr == GD_UNUSED)
		unitStr1 = _T("");
	else
	{
		// added by schbang 2006.03.17, calculate percent
		CString strOpenP;
		CString strHighP;
		CString strLowP;
		CString strCloseP;
		if (nIndex - 1 >= 0)
		{
			struct _local_data* pldPrev = &m_pLocal[nIndex - 1];
			if (pldPrev->epr != GD_UNUSED)
			{
				double dPcnt = (double)(pObject->fpr - pldPrev->epr) / (double)pldPrev->epr * 100;
				dPcnt < 0 ? strOpenP.Format("  (%0.2f%%)", dPcnt) : strOpenP.Format("  (+%0.2f%%)", dPcnt);

				dPcnt = (double)(pObject->hpr - pldPrev->epr) / (double)pldPrev->epr * 100;
				dPcnt < 0 ? strHighP.Format("  (%0.2f%%)", dPcnt) : strHighP.Format("  (+%0.2f%%)", dPcnt);

				dPcnt = (double)(pObject->lpr - pldPrev->epr) / (double)pldPrev->epr * 100;
				dPcnt < 0 ? strLowP.Format("  (%0.2f%%)", dPcnt) : strLowP.Format("  (+%0.2f%%)", dPcnt);

				dPcnt = (double)(pObject->epr - pldPrev->epr) / (double)pldPrev->epr * 100;
				dPcnt < 0 ? strCloseP.Format("  (%0.2f%%)", dPcnt) : strCloseP.Format("  (+%0.2f%%)", dPcnt);
			}
		}
		

		FormatVariableComma(sFpr, pObject->fpr, pObject->fpr);
		FormatVariableComma(sHpr, pObject->hpr, pObject->hpr);
		FormatVariableComma(sLpr, pObject->lpr, pObject->lpr);
		FormatVariableComma(sEpr, pObject->epr, pObject->epr);
		FormatVariableComma(sStart, Start, Start);
		FormatVariableComma(sEnd, End, End);
		if (bGrid)
			unitStr1.Format("시가:%s\t고가:%s\t저가:%s\t$$종가:%s\t전환시가:%s\t전환종가:%s",
				LPCTSTR(sFpr + strOpenP), LPCTSTR(sHpr + strHighP), LPCTSTR(sLpr + strLowP), LPCTSTR(sEpr + strCloseP), LPCTSTR(sStart), LPCTSTR(sEnd));
		else
			unitStr1.Format("시가\t%s\t고가\t%s\t저가\t%s\t종가\t%s\t전환시가\t%s\t전환종가\t%s",
				LPCTSTR(sFpr + strOpenP), LPCTSTR(sHpr + strHighP), LPCTSTR(sLpr + strLowP), LPCTSTR(sEpr + strCloseP), LPCTSTR(sStart), LPCTSTR(sEnd));
	}

	return unitStr1;
}

void CVSamsun::SetMember()
{
	m_UpperColor = m_pgraph->cInfo.aline[0].rgbLColor;
	m_LowerColor = m_pgraph->cInfo.aline[1].rgbLColor;

	m_iDrawHeight	= m_rectGrp.Height();
	m_iDrawWidth	= m_rectGrp.Width();	
}

CString CVSamsun::GetXposValue(CPoint pt)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)	return _T("");

	if (m_CurveN <= 0)	return _T("");

	struct _local_data* pObject = NULL;
	int	nIndex = -1;

	int	nGab = (pt.x - m_rectGrp.left);
	
	if (nGab < 0)	return _T("");
	
	nIndex = (int)((double)nGab / m_fUnit);
	if (m_CurveN > m_nShow)	nIndex += m_CurveN - m_nShow;
	if (nIndex < 0 || nIndex >= m_CurveN)	return _T("");
	
//	for ( int ii = 1 + m_StartInx; ii < m_CurveN + 1; ii++ )
//	{
//		if (pt.x >= m_pXpos[ii - 1 - m_StartInx] && pt.x < m_pXpos[ii - m_StartInx])
//		{
//			nIndex = ii - 1;
//			break;
//		}
//	}
	
	if (nIndex < 0)	return _T("");
	
	CString	unitStr1 = _T("");
	pObject = &m_pLocal[nIndex];
	int	Start, End;

	Start = m_pSamsunData[nIndex*2];
	End = m_pSamsunData[nIndex*2 + 1];

	switch (GetDataIndex())
	{
	case GI_MINUTE:
	case GI_TICK:
		unitStr1.Format("%02d/%02d %02d:%02d:%02d", 
			pObject->date.date.mm, pObject->date.date.dd,
			pObject->date.time.hh, pObject->date.time.mm, pObject->date.time.ss);
		break;
	default:
		unitStr1.Format("%04d/%02d/%02d", 
			pObject->date.date.yy, pObject->date.date.mm, pObject->date.date.dd);
		break;
	}

	return unitStr1;
}
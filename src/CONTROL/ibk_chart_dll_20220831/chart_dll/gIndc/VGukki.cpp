// VGukki.cpp : implementation file
//

#include "stdafx.h"
#include "VGukki.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVGukki

CVGukki::CVGukki(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
: CVarsBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_bCheck = true;
	m_bCalculated = false;

	m_piGukkiData = NULL;
	m_piXpos = NULL;
	m_pLocal = NULL;
	m_iUserJWPnt = 0;
	m_iOnePrice = 0;
	m_iStartInx = 0;

	_envInfo *envinfo = (_envInfo*)pInfo;
	m_btnindex = envinfo->datainfo.btIndex;

	m_union = false;
	m_BlkDataCnt= 0;
	m_jjumData.RemoveAll();
	m_gjumData.RemoveAll();
}

CVGukki::~CVGukki()
{
	if (m_piGukkiData != NULL)
	{
		delete[] m_piGukkiData;	m_piGukkiData = NULL;
	}

	if (m_pLocal != NULL)
	{
		delete[] m_pLocal;	m_pLocal = NULL;
	}

	if (m_piXpos)
	{
		delete[] m_piXpos;	m_piXpos = NULL;
	}

	m_jjumData.RemoveAll();
	m_gjumData.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// CVGukki message handlers

void CVGukki::DrawGraph(CDC* pDC)
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
	
	if (m_pgraph->dwDraOption & GO_UNION)
	{
		m_union = true;
		CalculateMinMax(false);

		if (m_piXpos)
		{
			DrawUnionChart(pDC);
			
			DrawBottomStick(pDC);
		}
	}
}

void CVGukki::DrawUnionChart(CDC* pDC)
{
	if (!m_piGukkiData)
		return;

	CPen	*UpPen = GetPen(PS_SOLID, 2, m_UpperColor);
	CPen	*DnPen = GetPen(PS_SOLID, 2, m_LowerColor);

	CPen	*sPen = pDC->SelectObject(DnPen);
	COLORREF Ycolor = RGB(196, 196, 0);
	COLORREF Gcolor = RGB(192,  0, 192);

	CRgn rgnDraw;
	rgnDraw.CreateRectRgnIndirect(m_rectGrp);
	pDC->SelectClipRgn(&rgnDraw);
	
	if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_PRC)
	{
		m_dYMax = m_pCoDraw->PMax;
		m_dYMin = m_pCoDraw->PMin;
	}

	CPoint	dispPt;
	CRect	DrawR;
	CString	dispText;
	CArray <PFTICK,PFTICK>	Stick;

	int	kk = m_iStartInx;
	int	Year = 0;
	int	vHeight = m_piGukkiData[kk * 2];

	double	sPoint, realStartPoint, dPixel;
	realStartPoint = 0.0;
	dPixel = double(m_iDrawHeight) / double(m_dYMax - m_dYMin);

	int	nPoint = m_iCurveN - m_iStartInx;

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}
	
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreateRectRgn(0, 0, 0, 0);
	CRgn	tmpRGN;
	CRect	tmpRECT;
	CPoint	*pRgnPoint = new CPoint[nPoint * 2];

	double  oldval;
	double  gojum = 0, jujum = 0;
	CPoint	tpoint, OldPt, xPt;
	bool	bInc = false;
	double	xv;
	double	yv;
	double	Start, PerYval;

	m_jjumData.RemoveAll();
	m_gjumData.RemoveAll();

	oldval = 0;

	for (int ii = 0; kk <= m_iCurveN; ii++, kk++)
	{ 
		Start   = m_piGukkiData[ii];
		sPoint  = GetYPos(Start);

		tpoint.x = int(m_piXpos[ii]);
		tpoint.y = int(sPoint);

		if (ii == 0)
		{
			oldval = Start;
			OldPt  = tpoint;
		}

		if (!bInc)
		{
			pDC->SelectObject(DnPen);
			m_arColorX.Add(m_LowerColor);
		}
		else
		{
			pDC->SelectObject(UpPen);
			m_arColorX.Add(m_UpperColor);
		}

		m_arPointX.Add(tpoint.x);

		if (ii == 0)
			pDC->MoveTo(tpoint);
		else
			pDC->LineTo(tpoint);			
		
		if (jujum && bInc && jujum > Start)
		{
			bInc = !bInc;
			xv = (oldval - jujum) * static_cast<double>(tpoint.x) - static_cast<double>(OldPt.x) / (oldval - Start) + OldPt.x;

			if (ii > 2)
			{
				PerYval = m_piGukkiData[ii - 2];
				yv = GetYPos(PerYval);
			}

			xPt.x = (int)xv;
			xPt.y = (int)yv;

			if (!bInc)
			{
				pDC->SelectObject(DnPen);
				m_arColorX.Add(m_LowerColor);
			}
			else
			{
				pDC->SelectObject(UpPen);
				m_arColorX.Add(m_UpperColor);
			}

			m_arPointX.Add(xPt.x);
			pDC->MoveTo(xPt);
			pDC->LineTo(tpoint);
		}

		if (gojum && !bInc && gojum < Start)
		{
			bInc = !bInc;

			xv = (gojum - oldval) * castDVAL(tpoint.x) - castDVAL(OldPt.x) / (Start - oldval) + OldPt.x;
			
			if (ii > 2)
			{
				PerYval = m_piGukkiData[ii - 2];
				yv = GetYPos(PerYval);
			}

			xPt.x = (int)xv;
			xPt.y = (int)yv;

			if (!bInc)
			{
				pDC->SelectObject(DnPen);
				m_arColorX.Add(m_LowerColor);
			}
			else
			{
				pDC->SelectObject(UpPen);
				m_arColorX.Add(m_UpperColor);
			}

			m_arPointX.Add(xPt.x);
			pDC->MoveTo(xPt);
			pDC->LineTo(tpoint);
		}

		if (Start > oldval)
			gojum = Start;
		else 
			jujum = Start;

		oldval = Start;
		OldPt  = tpoint;
	}

	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		int	sROP = pDC->SetROP2(R2_XORPEN);

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
	}

	delete[] pRgnPoint;
	
	pDC->SelectObject(sPen);
	pDC->SelectClipRgn(NULL);

	m_RgnInfo.iCount++;
}

int CVGukki::GetOnePrice()
{
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return 0;
	}

	CGrpBasic* gBasic = pOrgData->GetGraphData(m_iTtlDtCnt - 1);
	int	epr = gBasic->m_iClose;

	int	Price = 0;
	if (epr < 50)			Price = 1;
	else if (epr < 100)		Price = 2;
	else if (epr < 250)		Price = 5;
	else if (epr < 500)		Price = 10;
	else if (epr < 1000)		Price = 20;
	else if (epr < 2500)		Price = 50;
	else if (epr < 5000)		Price = 100;
	else if (epr < 10000)		Price = 200;
	else if (epr < 25000)		Price = 500;
	else if (epr < 50000)		Price = 1000;
	else if (epr < 100000)		Price = 2000;
	else if (epr < 250000)		Price = 5000;
	else if (epr < 500000)		Price = 10000;
	else if (epr < 1000000)		Price = 20000;
	else if (epr < 2500000)		Price = 50000;
	else if (epr < 5000000)		Price = 100000;
	else if (epr < 10000000)	Price = 200000;
	else if (epr < 25000000)	Price = 500000;
	else if (epr < 50000000)	Price = 1000000;
	else if (epr < 100000000)	Price = 2000000;
	else if (epr < 250000000)	Price = 5000000;
	else				Price = 10000000;

	int mok = int ((m_dYMax - m_dYMin) / (double)Price);

	if (mok == 0)
		Price = (int)((m_dYMax - m_dYMin) / 50.0);
	else if (mok < 5)
		Price /= 10;
	else if (mok < 10)
		Price /= 5;
	else if (mok < 20)
		Price /= 2;
	else if (mok > 100)
	{
		mok = (int(double(mok) / 100.0 + 0.5) * 2);
		Price *= mok;
	}

	return Price;
}

void CVGukki::SetMember()
{
	// 2006.12.27 schbang : 색깔맞춤
	//m_UpperColor = m_pgraph->cInfo.aline[0].rgbLColor;
	m_UpperColor = RGB(220, 2, 2);
	//m_LowerColor = m_pgraph->cInfo.aline[1].rgbLColor;
	m_LowerColor = RGB(0, 1, 200);
	
	m_iDrawHeight	= m_rectGrp.Height();
	m_iDrawWidth	= m_rectGrp.Width();

	// m_iJWPnt = m_pgraph->cInfo.awValue[0];
}

bool CVGukki::CalculateMinMax(bool bForce)
{
	SetMember();

	if (!m_bCalculated || bForce)
	{
		if (!CreateData())
			return false;
		m_bCalculated = true;
	}

	int WidthCnt = m_iCurveN;
	if (WidthCnt <= 0)	
		return false;

	if ((m_iCurveN - WidthCnt) < 0)	
		m_iStartInx = 0;
	else				
		m_iStartInx = m_iCurveN - WidthCnt;
		
	if (m_piXpos)
	{
		delete [] m_piXpos;
		m_piXpos = NULL;
	}

	m_piXpos = new double[WidthCnt + 1];

	double	dWidth;
	int	iPointCnt = m_iDispEPos - m_iDispSPos;
	int	wWidth = m_rectGrp.left;
	int	wRight = m_rectGrp.right;

	// x좌표 설정
	
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return false;
	
	if (m_dwDspOpt & DO_SCREEN && m_pgraph->dwDraOption & GO_UNION)
		pOrgData->CalculateMinMax(m_iDispSPos, m_iDispEPos, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cPRC, bForce);
	else
		pOrgData->CalculateMinMax(0, m_iTtlDtCnt, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cPRC, bForce);

	for (int ii = 0; ii < WidthCnt + 1; ii++)
	{
		if (m_union)
		{
			dWidth = (double)m_rectGrp.Width() / (double)iPointCnt;

			if (ii == 0)
				m_piXpos[ii] = (wWidth - (dWidth * m_iDispSPos)) + dWidth * m_pLocal[ii].orgindex + (dWidth / 2);
			else if (ii == m_iCurveN)
				m_piXpos[ii] = m_piXpos[ii - 1] + (wRight - m_piXpos[ii - 1]) - (dWidth / 2);
			else
				m_piXpos[ii] = m_piXpos[ii - 1] + dWidth * castDVAL(castDVAL(m_pLocal[ii].orgindex) - castDVAL(m_pLocal[ii - 1].orgindex));
		}
	}

	return true;
}
bool CVGukki::CreateData()
{
	if (m_iTtlDtCnt <= 0)
		return false;

	int	*pGukki;

	class COrgData *pOrgData;

	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return false;
	}

	CGrpBasic* gBasic = nullptr;

	pGukki = new int[m_iTtlDtCnt] {0, };

	int Start = -1;

	m_BlkDataCnt = 0;

	for (int ii = 0 ; ii < m_iTtlDtCnt; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii);
		pGukki[ii] = gBasic->m_iClose;

		if (gBasic->m_iClose != GD_UNUSED && Start < 0)
			Start = ii;
		if (gBasic->m_iClose == GD_UNUSED)
			m_BlkDataCnt++;
	}

	if (Start > m_iTtlDtCnt - 2)
	{
		delete [] pGukki;	pGukki = NULL;
		return false;
	}

	FindMinMax(&pGukki[Start], m_iTtlDtCnt - Start);
	ReviseMinMax(m_dYMax, m_dYMin);

	if (m_dYMax - m_dYMin < 10)
	{
		delete[] pGukki;	pGukki = NULL;
		return false;
	}

	if (m_bCheck)
	{
		if (m_iOnePrice <= 0 || m_iOnePrice >= (m_dYMax - m_dYMin)/10)
			m_iOnePrice = GetOnePrice();
		if (m_iOnePrice < 1)
			m_iOnePrice = 1;
	}

	if (m_bCheck && GetDataIndex() == GI_MINUTE)
	{
		gBasic = pOrgData->GetGraphData(m_iTtlDtCnt-1);

		switch (GetDataUnit())
		{
		case GU_FUTURE:
			m_iOnePrice = 5;
			break;
		case GU_OPTION:
			if (gBasic->m_iClose < 300)
				m_iOnePrice = 1;
			else
				m_iOnePrice = 5;
			break;
		}
	}
	
	double	JWPrice;				// 전환가격
	
	switch (m_btnindex)
	{
		case 5:	
			JWPrice = 0.1;		// tick
			break;
		case 4:	
			JWPrice = 1.0;		// min
			break;
		default:	
			JWPrice = 5.0;		// etc
			break;
	}
	
	if (m_dYMax - m_dYMin <= JWPrice)
	{
		delete[] pGukki;	pGukki = NULL;
		return false;
	}

	if (m_piGukkiData)
	{
		delete [] m_piGukkiData;
		m_piGukkiData = NULL;
	}

	m_piGukkiData = new int[m_iTtlDtCnt * 2];

	if (m_pLocal)
	{
		delete [] m_pLocal;
		m_pLocal = NULL;
	}
	
	m_pLocal = new struct _local_data[m_iTtlDtCnt];

	int PreVal = pGukki[Start], Cnt = 0;
	int Pivot = PreVal;
	int CurState = EMPTY;

	int	CurVal;
	int	igap = 0;
	int	oindex = 0;
	double	div;

	m_iCurveN = 0;

	for (int ii = Start + 1; ii < m_iTtlDtCnt; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii);
		CurVal = pGukki[ii];

		switch (GetState(PreVal, CurVal))
		{
		case INC:	// 상승세로 전환
			//igap++;
			switch (CurState)
			{
			case DEC:
//				div = ((CurVal - PreVal) * 100.) / PreVal;
				div = (castDVAL(CurVal - PreVal) * 100.) / PreVal;
				
				if (div >= JWPrice)
				{	// 전환선만큼 상승
					m_piGukkiData[Cnt++] = PreVal;
					
					memcpy(&m_pLocal[m_iCurveN].date, &gBasic->m_index, sz_gIndex);
					m_pLocal[m_iCurveN].fpr = (double)gBasic->m_iOpen;
					m_pLocal[m_iCurveN].hpr = (double)gBasic->m_iHigh;
					m_pLocal[m_iCurveN].lpr = (double)gBasic->m_iLow;
					m_pLocal[m_iCurveN].epr = (double)gBasic->m_iClose;
					m_pLocal[m_iCurveN].orgindex = oindex;
					igap = 0;

					PreVal = CurVal;
					oindex = ii;

					CurState = INC;
					m_iCurveN++;
				}
				break;
			case INC:
			case EMPTY:
				if (CurVal <= PreVal)
					break;

				PreVal = CurVal;
				oindex = ii;

				CurState = INC;
				break;
			}
			break;
		case DEC:	// 하락세로 전환
			//igap++;
			switch (CurState)
			{
			case INC:
				div = (static_cast<double>(PreVal) - static_cast<double>(CurVal) * 100.) / static_cast<double>(PreVal);
				
				if (div >= JWPrice)
				{	// 전환선만큼 하락
					m_piGukkiData[Cnt++] = PreVal;
					
					memcpy(&m_pLocal[m_iCurveN].date, &gBasic->m_index, sz_gIndex);
					m_pLocal[m_iCurveN].fpr = (double)gBasic->m_iOpen;
					m_pLocal[m_iCurveN].hpr = (double)gBasic->m_iHigh;
					m_pLocal[m_iCurveN].lpr = (double)gBasic->m_iLow;
					m_pLocal[m_iCurveN].epr = (double)gBasic->m_iClose;
					m_pLocal[m_iCurveN].orgindex = oindex;
					igap = 0;

					PreVal = CurVal;
					oindex = ii;

					CurState = DEC;
					m_iCurveN++;
				}
				break;
			case DEC:
			case EMPTY:
				if (CurVal >= PreVal)
					break;

				PreVal = CurVal;
				oindex = ii;

				CurState = DEC;
				break;
			}
			break;	
		default:
			break;
		}
	}

	m_piGukkiData[m_iCurveN] = gBasic->m_iClose;	// 마지막값

	delete[] pGukki;	pGukki = NULL;

	if (m_iCurveN == 1)
	{
		return true;
		//if (abs(((m_piGukkiData[0] - m_piGukkiData[1]) * 100.) / m_piGukkiData[0]) < m_iJWPnt)
		//	return false;
	}

	return true;
}

bool CVGukki::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
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

CString CVGukki::GetDisplayPosHeader(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)	return _T("");

	if (m_iCurveN <= 0)	return _T("");

	struct _local_data* pObject = NULL;
	int	nIndex = -1;

	for ( int ii = 1 + m_iStartInx ; ii < m_iCurveN + 1 ; ii++ )
	{
		if (pt.x >= m_piXpos[ii - 1 - m_iStartInx] && pt.x < m_piXpos[ii - m_iStartInx])
			nIndex = ii - 1;		
	}
	
	if (nIndex < 0)	return _T("");
	
	CString	unitStr1 = _T("");
	pObject = &m_pLocal[nIndex];
	int	Start, End;

	Start = m_piGukkiData[nIndex * 2];
	End = m_piGukkiData[nIndex * 2 + 1];

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

CString CVGukki::GetDisplayPosData(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	if (m_iCurveN <= 0)
		return _T("");

	struct _local_data* pObject = NULL;
	int	nIndex = -1;

	for ( int ii = 1 + m_iStartInx ; ii < m_iCurveN + 1 ; ii++ )
	{
		if (pt.x >= m_piXpos[ii - 1 - m_iStartInx] && pt.x < m_piXpos[ii - m_iStartInx])
			nIndex = ii - 1;		
	}
	
	if (nIndex < 0)
		return _T("");
	
	CString	unitStr1 = _T("");
	pObject = &m_pLocal[nIndex];
	int	Start, End;

	Start = m_piGukkiData[nIndex * 2];
	End = m_piGukkiData[nIndex * 2 + 1];

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

CString CVGukki::GetXposValue(CPoint pt)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)	return _T("");

	if (m_iCurveN <= 0)	return _T("");

	struct _local_data* pObject = NULL;
	int	nIndex = -1;

	for ( int ii = 1 + m_iStartInx ; ii < m_iCurveN + 1 ; ii++ )
	{
		if (pt.x >= m_piXpos[ii - 1 - m_iStartInx] && pt.x < m_piXpos[ii - m_iStartInx])
			nIndex = ii - 1;		
	}
	
	if (nIndex < 0)	return _T("");
	
	CString	unitStr1 = _T("");
	pObject = &m_pLocal[nIndex];
	int	Start, End;

	Start = m_piGukkiData[nIndex * 2];
	End = m_piGukkiData[nIndex * 2 + 1];

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

int CVGukki::GetState(int PreVal, int CurVal)
{
	if (PreVal > CurVal)
		return DEC;
	else if(PreVal < CurVal)
		return INC;
	else
		return EMPTY;
}

void CVGukki::FindMinMax(int *pArray, int Size)
{
	m_dYMin = DBL_MAX;
	m_dYMax = DBL_MIN;

	for (int ii = 0; ii < Size; ii ++)
	{
		if (m_dYMax < pArray[ii])
			m_dYMax = pArray[ii];

		if (m_dYMin > pArray[ii])
			m_dYMin = pArray[ii];
	}
}

void CVGukki::SetUserDefined(CString sVal)
{
	if (sVal.IsEmpty())
		return;

	m_bCheck = false;	

	switch (GetDataUnit())
	{
	case GU_CODE:	case GU_COFUT:	case GU_COOPT:
		m_iOnePrice = atoi(sVal);
		break;
	default:
		m_iOnePrice = int(atof(sVal)*100);
		break;
	}
}

void CVGukki::GetUsedValue(CString &sVal)
{
	switch (GetDataUnit())
	{
	case GU_CODE:	case GU_COFUT:	case GU_COOPT:
		sVal.Format("%d", m_iOnePrice);
		break;
	default:
		sVal.Format("%f", m_iOnePrice/100.);
		break;
	}
}

int CVGukki::GetYPos(double val)
{
	double fMinMax = m_dYMax - m_dYMin;
	double fHeight = m_rectGrp.Height();
	double yPos = 0;
	double fGab = m_dYMax - val;
	double Max  = m_dYMax;
	double Min  = m_dYMin;
	double posValue = val;
	
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
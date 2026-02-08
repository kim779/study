#include "stdafx.h"
#include "VSwing.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CVSwing::CVSwing(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
: CVarsBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_bCheck = true;
	m_bCalculated = false;

	m_piPnFData = NULL;
	m_piXpos = NULL;
	m_pLocal = NULL;
	m_iJWPnt = 1;
	m_iUserJWPnt = 0;
	m_iOnePrice = 0;
}


CVSwing::~CVSwing()
{
	if (m_piPnFData != NULL)
	{
		delete[] m_piPnFData;	m_piPnFData = NULL;
	}

	if (m_pLocal != NULL)
	{
		delete[] m_pLocal;	m_pLocal = NULL;
	}

	if (m_piXpos)
	{
		delete[] m_piXpos;	m_piXpos = NULL;
	}
}


void CVSwing::DrawGraph(CDC* pDC)
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
		DrawUnionChart(pDC);
}

//#define TITLE_SWING 1
void CVSwing::DrawUnionChart(CDC* pDC)
{
	if (!m_piPnFData)
		return;

	CheckMinMax();
	
	if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_PRC)
	{
		m_dYMax = m_pCoDraw->PMax;
		m_dYMin = m_pCoDraw->PMin;
	}
	
	int iPointCnt = m_iDispEPos - m_iDispSPos;
//	int iPointCnt = m_iViweOrgDataCnt;
	int	nSize  = m_iCurveN;
	int	nIndex = 0;
	int	nWidth = m_rectGrp.Width();
	m_fUnit = (double)m_rectGrp.Width() / iPointCnt;
	
	CPen	*UpPen = GetPen(PS_SOLID, 2,RGB(255, 0, 0));
	CPen	*DnPen = GetPen(PS_SOLID, 2,m_LowerColor);
	CBrush	*UpBrush = GetBrush(m_pgraph->cInfo.aline[0].rgbLColor);
	CBrush	*DnBrush = GetBrush(m_LowerColor);

	CPen	*sPen = pDC->SelectObject(DnPen);
	CBrush	*sBrush = pDC->SelectObject(DnBrush);
//	COLORREF Ycolor = RGB(196, 196, 0);
//	COLORREF Gcolor = RGB(192, 0, 192);

	bool bInc;
	if (m_piPnFData[m_iStartInx * 2] > m_piPnFData[m_iStartInx * 2 + 1])
		bInc = false;
	else if (m_piPnFData[m_iStartInx * 2] < m_piPnFData[m_iStartInx * 2 + 1])
		bInc = true;
	else 
	{
		m_iStartInx++;

		if (m_piPnFData[m_iStartInx * 2] > m_piPnFData[m_iStartInx * 2 + 1])
			bInc = false;
		else if (m_piPnFData[m_iStartInx * 2] < m_piPnFData[m_iStartInx * 2 + 1])
			bInc = true;
	}

	CPoint	tpoint, dispPt;
	CRect	DrawR;
	CString	dispText;
	CArray <PFTICK,PFTICK>	Stick;

	int	Start, End;
	int	kk = m_iStartInx;
	int	Year = 0;
	int	vHeight = m_piPnFData[kk * 2];

	int	sPoint, ePoint;
	

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
	
	int	iViewOrg_Sidx, iViewOrg_Eidx, iGap;
	int	Y1,Y2;
	int	MaxYPoint;

	if (IsReverseChart())
		MaxYPoint = GetYPos(m_dYMax);
	else
		MaxYPoint = GetYPos(m_dYMin);


	iViewOrg_Sidx = m_OrgInDataStart;
	iViewOrg_Eidx = m_iTtlDtCnt;
	if(iViewOrg_Sidx < m_iDispSPos)
		iViewOrg_Sidx = m_iDispSPos;
	if(iViewOrg_Eidx > m_iDispEPos)
		iViewOrg_Eidx = m_iDispEPos;

	double	nXpos = (double)m_rectGrp.left + (m_fUnit /2);
	double	ixval,  old_ixval , old_nXpos;
	if(m_OrgInDataStart > 0 && m_iDispSPos < m_OrgInDataStart)
	{
		nXpos += (m_fUnit * (castDVAL(m_OrgInDataStart) - castDVAL(m_iDispSPos)));
	}

	old_nXpos = nXpos;
	ixval = old_ixval = 0;
	
	
	for (int ii = m_iViweSidx; ii <= m_iViweEidx; ii++, kk++ )	
	{
#if   1
		iGap = m_pLocal[ii].iGap;		
		if(ii == m_iViweSidx && m_pLocal[ii].iGap > 1 &&
			m_pLocal[ii].iSOrgidx <= iViewOrg_Sidx && iViewOrg_Sidx <= m_pLocal[ii].iEOrgidx )
			iGap = m_pLocal[ii].iEOrgidx -  iViewOrg_Sidx;
		if(ii == m_iViweEidx && m_pLocal[ii].iGap > 0 && 
			m_pLocal[ii].iSOrgidx <= iViewOrg_Eidx && iViewOrg_Eidx <= m_pLocal[ii].iEOrgidx )
			iGap = iViewOrg_Eidx - m_pLocal[ii].iSOrgidx + 1;
		ixval = m_fUnit * iGap;
#else
		iGap = m_pLocal[ii].iGap;		
		if(m_iViweSidx != 0 && ii == m_iViweSidx && m_pLocal[ii].iGap > 1)
			iGap = m_pLocal[ii].iEOrgidx -  iViewOrg_Sidx + 1;
		if(ii == m_iViweEidx && m_pLocal[ii].iGap > 0)
			iGap = iViewOrg_Eidx - m_pLocal[ii].iSOrgidx + 1;
		ixval = m_fUnit * iGap;
#endif


#if    0
		if(ii >= 1)
			Start = m_piPnFData[ii * 2 - 1];
		else
			Start = m_piPnFData[0];
		End = m_piPnFData[ii * 2 + 1];
#else
		if(ii >= 1)
			Start = (int)m_pLocal[ii-1].sval;
		else
			Start = m_piPnFData[0];
		End = (int)m_pLocal[ii].sval;

#endif
		

		sPoint = GetYPos( Start);
		ePoint = GetYPos(End);

#if ( TITLE_SWING == 1)
		// save date
		dispPt.x = (long)nXpos;

		PFTICK	tick;
		if (Year != m_pLocal[ii].date.date.yy)
		{
			dispText.Format("%d.%d.%d", m_pLocal[ii].date.date.yy,
				m_pLocal[ii].date.date.mm, m_pLocal[ii].date.date.dd);
		}
		else
		{
			dispText.Format("%d.%d", m_pLocal[ii].date.date.mm, 
				m_pLocal[ii].date.date.dd);
		}
#endif

		pDC->SelectObject(UpPen);
		pDC->SelectObject(UpBrush);

		Y1 = (sPoint < MaxYPoint) ? sPoint : MaxYPoint;
		Y2 = (ePoint < MaxYPoint) ? ePoint : MaxYPoint;
		if(ii == 0)
		{
			pDC->MoveTo((int)0 ,Y1);
			pDC->LineTo((int)nXpos,Y1);
		}
		else
			pDC->MoveTo((int)nXpos,Y1);

		if(ii == m_iViweSidx && ii != 0)
			pDC->MoveTo((int)nXpos,Y2);
		else
			pDC->LineTo((int)nXpos,Y2);

		if(ePoint <= MaxYPoint)
		{
			if(ii != (m_iCurveN -1))
				pDC->LineTo((int)(nXpos +ixval),(int)ePoint);
		}
		nXpos += ixval;
	}

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

#if ( TITLE_SWING == 1)
	
	
	if (m_rectGrp.Height() > int(m_TkInfo.tkHeight[TICK_BOTTOM]))
	{
		CString	str;
		FormatVariableComma(str, int(m_iOnePrice), m_dYMax);
		dispText.Format ("Swing차트 전환포인트 : %d, 한칸의 값 : %s", m_iJWPnt, str);
		DrawTickIndex(pDC, dispText, m_pgraph->rgbLabel, true);
	}
#endif

	delete[] pRgnPoint;
	
	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);

	m_RgnInfo.iCount++;
}




int CVSwing::GetYPos(double val)
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

#if 1
int CVSwing::GetOnePrice()
{
	COrgData* pOrgData = NULL;
	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return 0;
	
	double	dMinValue = DBL_MAX;
	double	dMaxValue = DBL_MIN;
	
	int iSize = pOrgData->GetArraySize();
	CGrpBasic* pBasic = NULL;

	int	iViewOrg_Sidx, iViewOrg_Eidx;
	
	iViewOrg_Sidx = m_OrgInDataStart;
	iViewOrg_Eidx = m_iTtlDtCnt;
	if(iViewOrg_Sidx < m_iDispSPos)
		iViewOrg_Sidx = m_iDispSPos;
	if(iViewOrg_Eidx > m_iDispEPos)
		iViewOrg_Eidx = m_iDispEPos;

//	for (int  ii = iViewOrg_Sidx; ii < iViewOrg_Eidx ; ii++ )
	for (int  ii = 0; ii < iSize ; ii++ )
	{
		pBasic = pOrgData->GetGraphData(ii);
		
		if (pBasic->m_iClose == GD_UNUSED)
			continue;
		
		if (pBasic->m_iHigh > dMaxValue)
			dMaxValue = pBasic->m_iHigh;
		
		if (pBasic->m_iLow < dMinValue)
			dMinValue = pBasic->m_iLow;
	}
//	return (int)((dMaxValue - dMinValue) * 0.0158);
	return (int)((dMaxValue - dMinValue) * 0.02);
//	return 542;
}
#else
int CVSwing::GetOnePrice()
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

#endif
void CVSwing::SetMember()
{
	m_UpperColor = m_pgraph->cInfo.aline[0].rgbLColor;
	m_LowerColor = m_pgraph->cInfo.aline[1].rgbLColor;
	
	m_iDrawHeight	= m_rectGrp.Height();
	m_iDrawWidth	= m_rectGrp.Width();

//	m_iJWPnt = m_pgraph->cInfo.awValue[0];
}

bool CVSwing::CalculateMinMax(bool bForce)
{
	SetMember();

	if (!m_bCalculated || bForce)
	{
		if (!CreateData())
			return false;
		m_bCalculated = true;
	}
	CheckMinMax();
	m_fUnit = (double)m_iDrawHeight / (m_iCurveN);

	m_dOnePoint = double(m_iDrawHeight) / (double(m_dYMax - m_dYMin) / double(m_iOnePrice));

	if (m_dOnePoint == 0)
		return false;

	int WidthCnt = int( double(m_iDrawWidth) / m_dOnePoint);
	if (WidthCnt <= 0)	return false;

	if ((m_iCurveN - WidthCnt) < 0)	
		m_iStartInx = 0;
	else				
		m_iStartInx = m_iCurveN - WidthCnt;

	if (m_iStartInx == 0)
	{
		if (abs(m_piPnFData[0] - m_piPnFData[1]) < m_iOnePrice * m_iJWPnt)
			m_iStartInx++;
	}
	
	
	if (m_piXpos)
	{
		delete [] m_piXpos;
		m_piXpos = NULL;
	}

	m_piXpos = new int[WidthCnt + 1];

	// x좌표 설정
	for ( int ii = 0; ii < WidthCnt + 1; ii++)
		m_piXpos[ii] = m_rectGrp.left + int(m_dOnePoint * ii) + 1;

	FindMinMax();	
	ReviseMinMax(m_dYMax, m_dYMin);
	return true;
}

bool CVSwing::CheckMinMax()
{
	int	ii, nSize;
	
	nSize = m_iCurveN;
	
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

bool CVSwing::CreateData()
{
	if (m_iTtlDtCnt <= 0)
		return false;

	int	*pPnF;

	class COrgData *pOrgData;

	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return false;
	}

	CGrpBasic* gBasic = nullptr;

	pPnF = new int[m_iTtlDtCnt];	

	int	Start = -1;

	for (int ii = 0 ; ii < m_iTtlDtCnt; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii);
		pPnF[ii] = gBasic->m_iClose;

		if (gBasic->m_iClose != GD_UNUSED && Start < 0)
		{
			Start = ii;
		}
	}

	if (Start > m_iTtlDtCnt - 2)
	{
		delete[] pPnF;	pPnF = NULL;
		return false;
	}
	m_OrgInDataStart = Start;
	
	FindMinMax(&pPnF[Start], m_iTtlDtCnt - Start);
	ReviseMinMax(m_dYMax, m_dYMin);

	if (m_dYMax - m_dYMin < 10)
	{
		delete[] pPnF;	pPnF = NULL;
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

	int	JWPrice = m_iOnePrice * (m_iJWPnt);	// 전환가격	
	m_dYMin -= JWPrice;
	
	if (m_iOnePrice <= 0 || m_dYMax - m_dYMin <= JWPrice)
	{
		delete[] pPnF;	pPnF = NULL;
		return false;
	}

	if (m_piPnFData)
	{
		delete [] m_piPnFData;
		m_piPnFData = NULL;
	}

	m_piPnFData = new int[ m_iTtlDtCnt*2 ];

	if (m_pLocal)
	{
		delete [] m_pLocal;
		m_pLocal = NULL;
	}
	
	m_pLocal = new struct _local_data[m_iTtlDtCnt];

	int PreVal = pPnF[Start], Cnt = 1;
	int Pivot = PreVal;
	int CurState = EMPTY;

	int	ii = 0;
	int	CurVal;
	int	mok, mod;
	int	iGap =0;
	int	iSOrgidx;

	m_iCurveN = 0;
	m_piPnFData[0] = pPnF[Start];	// 시작값

	for (ii = Start + 1; ii < m_iTtlDtCnt; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii);
		CurVal = pPnF[ii];

		if(iGap == 0)
		{
			iSOrgidx = (ii >= 1) ? ii -1 : ii;
		}
		iGap++;
		
		switch (GetState(PreVal, CurVal))
		{
		case INC:	// 상승세로 전환
			switch (CurState)
			{
			case DEC:
				if (CurVal - PreVal >= JWPrice)
				{	// 전환선만큼 상승
					m_piPnFData[Cnt++] = Pivot;
					m_piPnFData[Cnt++] = Pivot + m_iOnePrice;

					memcpy(&m_pLocal[m_iCurveN].date, &gBasic->m_index, sz_gIndex);
					m_pLocal[m_iCurveN].fpr = (double)gBasic->m_iOpen;
					m_pLocal[m_iCurveN].hpr = (double)gBasic->m_iHigh;
					m_pLocal[m_iCurveN].lpr = (double)gBasic->m_iLow;
					m_pLocal[m_iCurveN].epr = (double)gBasic->m_iClose;
					m_pLocal[m_iCurveN].iGap = iGap; iGap = 0;
					m_pLocal[m_iCurveN].iSOrgidx = iSOrgidx;
					m_pLocal[m_iCurveN].iEOrgidx = ii;//(ii >= 1) ? ii -1 : ii;
					m_pLocal[m_iCurveN].UpDn = DEC;
					m_pLocal[m_iCurveN].sval = PreVal;
					
					mok = (CurVal - Pivot) / m_iOnePrice;
					mod = (CurVal - Pivot) % m_iOnePrice;
					if (mod)
						Pivot = Pivot + m_iOnePrice * (mok + 1);
					else
						Pivot = Pivot + m_iOnePrice * mok;

					PreVal = CurVal;
					CurState = INC;
					m_iCurveN++;
				}
				break;
			case INC:
			case EMPTY:
				if (CurVal <= PreVal)
					break;

//				if (CurState == EMPTY)
//				{
//					memcpy(&m_pLocal[m_iCurveN].date, &gBasic->m_index, sz_gIndex);
//					m_pLocal[m_iCurveN].fpr = (double)gBasic->m_iOpen;
//					m_pLocal[m_iCurveN].hpr = (double)gBasic->m_iHigh;
//					m_pLocal[m_iCurveN].lpr = (double)gBasic->m_iLow;
//					m_pLocal[m_iCurveN].epr = (double)gBasic->m_iClose;
//					m_pLocal[m_iCurveN].iGap += iGap; iGap = 0;
//					m_pLocal[m_iCurveN].iSOrgidx = iSOrgidx;
//					m_pLocal[m_iCurveN].iEOrgidx = ii;
//					
//					m_iCurveN++;
//				}


				mok = (CurVal - m_piPnFData[Cnt-1]) / m_iOnePrice;
				mod = (CurVal - m_piPnFData[Cnt-1]) % m_iOnePrice;
				if (mod)
					Pivot = m_piPnFData[Cnt-1] + m_iOnePrice * (mok + 1);
				else
					Pivot = m_piPnFData[Cnt-1] + m_iOnePrice * mok;
				PreVal = CurVal;
				CurState = INC;
				break;
			}
			break;
		case DEC:	// 하락세로 전환
			switch (CurState)
			{
			case INC:
				if (PreVal - CurVal >= JWPrice)
				{	// 전환선만큼 하락
					m_piPnFData[Cnt++] = Pivot;
					m_piPnFData[Cnt++] = Pivot - m_iOnePrice;
					memcpy(&m_pLocal[m_iCurveN].date, &gBasic->m_index, sz_gIndex);
					m_pLocal[m_iCurveN].fpr = (double)gBasic->m_iOpen;
					m_pLocal[m_iCurveN].hpr = (double)gBasic->m_iHigh;
					m_pLocal[m_iCurveN].lpr = (double)gBasic->m_iLow;
					m_pLocal[m_iCurveN].epr = (double)gBasic->m_iClose;
					m_pLocal[m_iCurveN].iGap = iGap; iGap = 0;
					m_pLocal[m_iCurveN].iSOrgidx = iSOrgidx;
					m_pLocal[m_iCurveN].iEOrgidx = ii;//(ii >= 1) ? ii -1 : ii;
					m_pLocal[m_iCurveN].UpDn = INC;
					m_pLocal[m_iCurveN].sval = PreVal;
					
					mok = (Pivot - CurVal) / m_iOnePrice;

					Pivot = Pivot - m_iOnePrice * (mok + 1);
					PreVal = CurVal;

					CurState = DEC;
					m_iCurveN++;
				}
				break;
			case DEC:
			case EMPTY:
				if (CurVal >= PreVal)
					break;

//				if (CurState == EMPTY)
//				{
//					memcpy(&m_pLocal[m_iCurveN].date, &gBasic->m_index, sz_gIndex);
//					m_pLocal[m_iCurveN].fpr = (double)gBasic->m_iOpen;
//					m_pLocal[m_iCurveN].hpr = (double)gBasic->m_iHigh;
//					m_pLocal[m_iCurveN].lpr = (double)gBasic->m_iLow;
//					m_pLocal[m_iCurveN].epr = (double)gBasic->m_iClose;
//					m_pLocal[m_iCurveN].iGap = iGap; iGap = 0;
//					m_pLocal[m_iCurveN].iSOrgidx = iSOrgidx;
//					m_pLocal[m_iCurveN].iEOrgidx = ii;
//					
//					m_iCurveN++;
//				}


				mok = (m_piPnFData[Cnt-1] - CurVal) / m_iOnePrice;
				Pivot = m_piPnFData[Cnt-1] - m_iOnePrice * (mok + 1);
				PreVal = CurVal;
				CurState = DEC;
				break;
			}
			break;	
		default:
			break;
		}
	}

	m_piPnFData[Cnt++] = Pivot;	// 마지막값

	if (iGap > 0 && gBasic != nullptr)
	{
		memcpy(&m_pLocal[m_iCurveN].date, &gBasic->m_index, sz_gIndex);
		m_pLocal[m_iCurveN].fpr = (double)gBasic->m_iOpen;
		m_pLocal[m_iCurveN].hpr = (double)gBasic->m_iHigh;
		m_pLocal[m_iCurveN].lpr = (double)gBasic->m_iLow;
		m_pLocal[m_iCurveN].epr = (double)gBasic->m_iClose;
		m_pLocal[m_iCurveN].iGap = iGap; iGap = 0;
		m_pLocal[m_iCurveN].iSOrgidx = iSOrgidx;
		m_pLocal[m_iCurveN].iEOrgidx = ii;//(ii >= 1) ? ii -1 : ii;
		m_pLocal[m_iCurveN].UpDn = INC;
		m_pLocal[m_iCurveN].sval = PreVal;
		
		m_iCurveN++;
	}

	delete[] pPnF;	pPnF = NULL;

	if (m_iCurveN == 1)
	{
		if (abs(m_piPnFData[0] - m_piPnFData[1]) < m_iOnePrice * m_iJWPnt)
			return false;
	}

	return true;
}

bool CVSwing::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
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

CString CVSwing::GetDisplayPosHeader(CPoint pt, bool bGrid)
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

	Start = m_piPnFData[nIndex * 2];
	End = m_piPnFData[nIndex * 2 + 1];

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

CString CVSwing::GetDisplayPosData(CPoint pt, bool bGrid)
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

	Start = m_piPnFData[nIndex * 2];
	End = m_piPnFData[nIndex * 2 + 1];

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

CString CVSwing::GetXposValue(CPoint pt)
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

	Start = m_piPnFData[nIndex * 2];
	End = m_piPnFData[nIndex * 2 + 1];

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

int CVSwing::GetState(int PreVal, int CurVal)
{
	if (PreVal > CurVal)
		return DEC;
	else if(PreVal < CurVal)
		return INC;
	else
		return EMPTY;
}

void CVSwing::FindMinMax(int *pArray, int Size)
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

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return ;
	
	if (m_dwDspOpt & DO_SCREEN && m_pgraph->dwDraOption & GO_UNION)
		pOrgData->CalculateMinMax(m_iDispSPos, m_iDispEPos, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cPRC, TRUE);
	else
		pOrgData->CalculateMinMax(0, m_iTtlDtCnt, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cPRC, TRUE);
	
}

void CVSwing::SetUserDefined(CString sVal)
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

void CVSwing::GetUsedValue(CString &sVal)
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



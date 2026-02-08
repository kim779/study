// VRenko.cpp: implementation of the CVRenko class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VRenko.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVRenko::CVRenko(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
: CVarsBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_nUnitSize = 0;
	m_bCheck = true;
	m_JWValue = 0;
	m_UpperColor = 0;
	m_LowerColor = 0;
	m_iDrawHeight = 0;
	m_iDrawWidth = 0;
	m_fUnit = -1.0;
	m_nShow = -1;
	m_arData.RemoveAll();
	m_bCalculated = false;
	m_dFirsrDefVal = 0;
}

CVRenko::~CVRenko()
{
	RemoveData();
}

void CVRenko::DrawGraph(CDC* pDC)
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

void CVRenko::SetMember()
{
	if (m_bCheck)
		m_JWValue = GetDefVal();	
}

void CVRenko::SetUserDefined(CString sVal)
{
	if (sVal.IsEmpty())
		return;

	m_bCheck = false;

	switch (GetDataUnit())
	{
	case GU_CODE:	case GU_COFUT:	case GU_COOPT:
		m_JWValue = atof(sVal);
		break;
	default:
		m_JWValue = int(atof(sVal) * 100.0);
		break;
	}	
}

void CVRenko::GetUsedValue(CString &sVal)
{
	switch (GetDataUnit())
	{
	case GU_CODE:	case GU_COFUT:	case GU_COOPT:
		sVal.Format("%d", (int)m_JWValue);
		break;
	default:
		sVal.Format("%f", m_JWValue/100.0);
		break;
	}
}

#include <math.h>
void CVRenko::GetValueToPoint(CPoint& pt, double dXValue, double dYValue)
{
	pt = CPoint(-1, -1);

	// 2006.12.27 schbang 데이터 없을시 죽는걸 막기위해
	if (m_arData.GetSize() < 1)
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
//			rectDraw.right -= int(double(m_rectGrp.Width()*iBalanceGap) / double(m_iDispDtCnt + iBalanceGap));
			rectDraw.right -= int( (castDVAL(m_rectGrp.Width()) * castDVAL(iBalanceGap)) / (castDVAL(m_iDispDtCnt) + castDVAL(iBalanceGap)) );
	}
	
	double dGap = castDVAL(m_iDispEPos) - castDVAL(m_iDispSPos);

	if (dGap == 0) dGap = 1;
	
//	double dX = (dXValue - m_iDispSPos) * rectDraw.Width() / dGap;
	struct	_local_data*	pObject = NULL;
	int m_iShow = m_arData.GetSize();
	double dX = 0, dMValue = 0;
	int	iNam = 0;


	char Buff[32];
	sprintf_s(Buff, sizeof(Buff), "%.2f",dXValue / 100);
	int len = strlen(Buff);

	for (int jj=0; jj < len; jj++)
	{
		if(Buff[jj] == '.')
		{
			Buff[jj] = NULL;
			dMValue = atof(Buff);
			iNam = atoi(&Buff[jj+1]);
			break;
		}
	}
//	dMValue = dXValue / 100;
//	iNam = (int)(dXValue % 100);
	

	m_fUnit = (double)m_rectGrp.Width() / m_iShow;
	for ( int ii = m_iViweSidx; ii <= m_iViweEidx ; ii++ )
	{
		pObject = m_arData.GetAt(ii);
		if (pObject != NULL && pObject->dSDay <= dMValue && dMValue < pObject->dEDay)
		{
			int dummy = ii + iNam - m_iViweSidx;
			dX = castDVAL(dummy) * m_fUnit + (m_fUnit / 2);
			break;
		}
	}
//	if(dX == 0)

	dGap = dMax - dMin;
	if (dGap == 0) dGap = 1;	
	
	double dY;
	if (IsReverseChart())
		dY = rectDraw.top + (dYValue - dMin) * rectDraw.Height() / dGap;
	else
		dY = rectDraw.bottom - (dYValue - dMin) * rectDraw.Height() / dGap;		
	
	pt.x = int(dX + rectDraw.left);
	pt.y = int(dY);
}

void CVRenko::GetPtToPosValue(CPoint pt, double& dPos, double& dValue)
{
	double dMax = m_dYMax;
	double dMin = m_dYMin;

	// 2006.12.27 schbang 데이터 없을시 죽는걸 막기위해
	if (m_arData.GetSize() < 1)
	{
		dPos = dValue = 0;
		return;
	}

	int iDispCount = GetDisplayEnd() - GetDisplayPos();
	CRect rectDraw = m_rectGrp;
	if (m_iGrpKind == GK_BALANCE)
	{
		int iBalanceGap = GetValue();
		if (iBalanceGap > 0)
			rectDraw.right -= int( (castDVAL(m_rectGrp.Width())* castDVAL(iBalanceGap)) / (castDVAL(m_iDispDtCnt) + castDVAL(iBalanceGap)) );
	}
	
	int	ixPos;
	ixPos = (int)(castDVAL(pt.x) - castDVAL(rectDraw.left) / m_fUnit); //* iDispCount) / double(rectDraw.Width());
	ixPos += GetDisplayPos();
	
	struct	_local_data*	pObject = NULL;
	int	iCnt = 0;
	if(ixPos >= m_arData.GetSize())
		ixPos = m_arData.GetSize() -1;
	if(ixPos < 0)
		ixPos = 0;

	pObject = m_arData.GetAt((int)ixPos);
	if(pObject)
		dPos = pObject->dSDay;
	for (int ii = ixPos; ii >= m_iViweSidx; ii--)
	{
		pObject = m_arData.GetAt(ii);
		if(pObject != NULL && pObject->iGap != 0)
		{
			dPos = pObject->dMDay;
			break;
		}
		iCnt++;
	}
	dPos = dPos * 100 + iCnt;
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

void CVRenko::GetPtToEndvalPt(CPoint pt, CPoint &ept)
{
	double dPos, dValue;
	GetPtToPosValue(pt, dPos, dValue);
	int	 iXIdx = GetValueToXIdx(dPos);
	ept = pt;
	struct	_local_data*	pObject = NULL;
	pObject = m_arData.GetAt(iXIdx);
	if(pObject)
		ept.y = GetYPos(pObject->hpr);
}

CString CVRenko::GetIndexStrTool(int index, int &fpr, int &hpr, int &lpr, int &epr)
{
	if (index < 0)
		return _T("");
	

	if (index >= m_arData.GetSize())
		index = m_arData.GetSize()-1;
	
	struct	_local_data*	pObject = NULL;
	pObject = m_arData.GetAt(index);
	if(!pObject)
		return _T("");
	
	fpr = (int)pObject->lpr;
	hpr = (int)pObject->hpr;
	lpr = (int)pObject->lpr;
	epr = (int)pObject->hpr;
	
	return _T("");
}

double CVRenko::GetOneDataSize()
{
	
	return ((double)m_rectGrp.Width() / m_arData.GetSize());
	
}

int	CVRenko::GetValueToXIdx(double dXValue)
{
	struct	_local_data*	pObject = NULL;
	int m_iShow = m_arData.GetSize();
	double dX = 0;
	double	dMValue = 0.;
	int	iNam = 0;
	
	char	Buff[32];
	int	len;
	sprintf_s(Buff, sizeof(Buff), "%.2f", dXValue/100);
	len = strlen(Buff);
	for (int jj = 0; jj < len; jj++)
	{
		if (Buff[jj] == '.')
		{
			Buff[jj] = NULL;
			dMValue = atof(Buff);
			iNam = atoi(&Buff[jj+1]);
			break;
		}
	}
	
	m_fUnit = (double)m_rectGrp.Width() / m_iShow;
	for ( int ii = m_iViweSidx; ii <= m_iViweEidx ; ii++ )
	{
		pObject = m_arData.GetAt(ii);
		if(pObject != NULL && pObject->dSDay <= dMValue && dMValue < pObject->dEDay)
		{
			return (ii  + iNam);
		}
	}
	return 0;
}


void CVRenko::DrawChart(CDC* pDC)
{
	if (m_arData.GetSize() < 0)
		return;

	int m_iShow = m_arData.GetSize();
	m_fUnit = (double)m_rectGrp.Width() / m_iShow;

	CPen	*UpPen = GetPen(m_pgraph->cInfo.aline[0].btLStyle, m_pgraph->cInfo.aline[0].btLWidth, m_pgraph->cInfo.aline[0].rgbLColor);
	CPen	*DnPen = GetPen(m_pgraph->cInfo.aline[1].btLStyle, m_pgraph->cInfo.aline[1].btLWidth, m_pgraph->cInfo.aline[1].rgbLColor);
	CPen	*sPen = pDC->SelectObject(UpPen);
	
	CBrush	*DnBrush = GetBrush(m_pgraph->cInfo.aline[1].rgbLColor);
	CBrush	*UpBrush = GetBrush(m_pgraph->cInfo.aline[0].rgbLColor);
	CBrush	*sBrush = pDC->SelectObject(UpBrush);

	CPoint	pt;
	CPoint	ptAr[5];
	CString	dispText;
	int	nSize = m_arData.GetSize();
	int	nIndex = 0;

//	if (m_iShow < nSize)
//		nIndex = nSize - m_iShow;
	
	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;

	double	nXpos = (double)m_rectGrp.left;
	struct	_local_data*	pObject = NULL;
	CRgn	tmpRGN;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreateRectRgn(0, 0, 0, 0);
	
	for ( int ii = nIndex, jj = 0; ii < nSize ; ii++, jj++ )
	{
		pObject = m_arData.GetAt(ii);
		if(!pObject)
			break;
		ptAr[1].x = ptAr[0].x = (int)nXpos;
		ptAr[3].y = ptAr[0].y = GetYPos(pObject->lpr);

		ptAr[1].x = ptAr[0].x;
		ptAr[2].y = ptAr[1].y = GetYPos(pObject->hpr);
		
		ptAr[3].x = ptAr[2].x = (int)(nXpos + m_fUnit);

		ptAr[4] = ptAr[0];
		nXpos += m_fUnit;

		if (pObject->bUp)
		{
			pDC->SelectObject(UpPen);
			pDC->SelectObject(UpBrush);

			if (m_pgraph->dwDraOption & GO_FIILUPBONG)
				pDC->Polygon(ptAr, 5);
			else
				pDC->Polyline(ptAr, 5);
		}
		else
		{
			pDC->SelectObject(DnPen);
			pDC->SelectObject(DnBrush);

			if (m_pgraph->dwDraOption & GO_FIILDNBONG)
				pDC->Polygon(ptAr, 5);
			else
				pDC->Polyline(ptAr, 5);
		}		

		tmpRGN.DeleteObject();
		tmpRGN.CreatePolygonRgn(ptAr, 5, WINDING);
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CombineRgn(m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn, &tmpRGN, RGN_OR);		
	}

	tmpRGN.DeleteObject();
	
	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		int	nGab = nSize;
		int	nRet = 0;
		nGab = nGab - nIndex;
		nGab = nGab / 5;

		if (nGab == 0)
			nGab = 1;

		
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);

		if (m_arData.GetSize() > 0)
		{
			for (int ii = 0 ; ii < 4 ; ii++ )
			{
				int idx = nGab * (ii+1);
				pt.x = (int)((double)m_rectGrp.left + (castDVAL(idx) + 1) * m_fUnit + m_fUnit / 2.0);

				if (idx + nIndex >= nSize)
					continue;

				pObject = m_arData.GetAt(idx + nIndex);
//				pt.y = (int)(( GetYPos(pObject->hpr) + GetYPos(pObject->lpr) ) / 2.0);
				pt.y = castIVAL( ( castDVAL(GetYPos(pObject->hpr)) + castDVAL(GetYPos(pObject->lpr)) ) / 2.0);


				CRect	rc;	
				rc.SetRect(pt.x - rgnMARKGAP, pt.y - rgnMARKGAP, 
					pt.x + rgnMARKGAP, pt.y + rgnMARKGAP);
				rc.IntersectRect(&rc, &m_rectGrp);
				pDC->Rectangle(&rc);
			}
		}
		pDC->SetROP2(sROP);
		pDC->SelectObject(srgnBrush);
	}

	CString	str;
	FormatVariableComma(str, m_JWValue, m_dYMax);
	dispText.Format(_T("Renko 최소변화폭:%s"), str.GetString());
	DrawTickIndex(pDC, dispText, m_pgraph->rgbLabel, true);

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);

	m_RgnInfo.iCount++;
}

void CVRenko::DrawUnionChart(CDC* pDC)
{
	if (m_arData.GetSize() < 0)
		return;
	
	CheckMinMax();

	if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_PRC)
	{
			m_dYMax = m_pCoDraw->PMax;
			m_dYMin = m_pCoDraw->PMin;
	}

//	int iPointCnt = m_iViweOrgDataCnt;
	int iPointCnt = m_iDispEPos - m_iDispSPos;
	int m_iShow = m_arData.GetSize();
	int	nSize = m_arData.GetSize();
	int	nIndex = 0;
	int	nWidth = m_rectGrp.Width();
	m_fUnit = (double)m_rectGrp.Width() / iPointCnt;

	CPen	*UpPen = GetPen(m_pgraph->cInfo.aline[0].btLStyle, 2/*m_pgraph->cInfo.aline[0].btLWidth*/, m_pgraph->cInfo.aline[0].rgbLColor);
	CPen	*DnPen = GetPen(m_pgraph->cInfo.aline[1].btLStyle, 2/*m_pgraph->cInfo.aline[1].btLWidth*/, m_pgraph->cInfo.aline[1].rgbLColor);
	CPen	*sPen = pDC->SelectObject(UpPen);
	
	CBrush	*DnBrush = GetBrush(m_pgraph->cInfo.aline[1].rgbLColor);
	CBrush	*UpBrush = GetBrush(m_pgraph->cInfo.aline[0].rgbLColor);
	CBrush	*sBrush = pDC->SelectObject(UpBrush);

	CPoint	pt;
	CPoint	ptAr[5];
	CString	dispText;
	
	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;

	struct	_local_data*	pObject = NULL;
	CRgn	tmpRGN;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreateRectRgn(0, 0, 0, 0);

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
		nXpos += ( m_fUnit * castDVAL(m_OrgInDataStart) - castDVAL(m_iDispSPos) );
	}
	old_nXpos = nXpos;
	ixval = old_ixval = 0;
	m_arPointX.Add((int)nXpos);
	for ( int ii = m_iViweSidx, jj = 0; ii <= m_iViweEidx ; ii++, jj++ )
	{
		pObject = m_arData.GetAt(ii);
		if(!pObject)
			break;
		iGap = pObject->iGap;		
		if( ii == m_iViweSidx && pObject->iGap > 0 &&
			pObject->iSOrgidx <= iViewOrg_Sidx && iViewOrg_Sidx <= pObject->iEOrgidx )
		{
			iGap = pObject->iEOrgidx -  iViewOrg_Sidx + 1;
		}
		if(ii == m_iViweEidx && pObject->iGap > 0 && 
			pObject->iSOrgidx <= iViewOrg_Eidx && iViewOrg_Eidx <= pObject->iEOrgidx )
			iGap = iViewOrg_Eidx - pObject->iSOrgidx + 1;
		ixval = m_fUnit * iGap;

//		if(pObject->lpr < m_dYMin)
//			continue;
//		if(pObject->hpr > m_dYMax)
//			continue;

		if(nWidth <= nXpos)
			continue;
		if(nWidth < (int)(nXpos + ixval))
			ixval = nWidth - nXpos;

		ptAr[1].x = ptAr[0].x = (int)nXpos;
		ptAr[3].y = ptAr[0].y = GetYPos(pObject->lpr);

		ptAr[1].x = ptAr[0].x;
		ptAr[2].y = ptAr[1].y = GetYPos(pObject->hpr);
		
		ptAr[3].x = ptAr[2].x = (int)(nXpos + ixval);

		ptAr[4] = ptAr[0];
		old_nXpos = nXpos;
		old_ixval = ixval;
		nXpos += ixval;
		
		m_arPointX.Add((int)nXpos);
		if (pObject->bUp)
		{
			pDC->SelectObject(UpPen);
			pDC->SelectObject(UpBrush);
			m_arColorX.Add(m_pgraph->cInfo.aline[0].rgbLColor);
		}
		else
		{
			pDC->SelectObject(DnPen);
			pDC->SelectObject(DnBrush);
			m_arColorX.Add(m_pgraph->cInfo.aline[1].rgbLColor);
		}		
		if(iGap != pObject->iGap)
		{
			pDC->MoveTo(ptAr[1].x,ptAr[1].y);
			if(ii == m_iViweSidx)
				pDC->MoveTo(ptAr[0].x,ptAr[0].y);
			else
				pDC->LineTo(ptAr[0].x,ptAr[0].y);
			pDC->LineTo(ptAr[3].x,ptAr[3].y);
			if(ii == m_iViweEidx)
				pDC->MoveTo(ptAr[2].x,ptAr[2].y);
			else
				pDC->LineTo(ptAr[2].x,ptAr[2].y);
			pDC->LineTo(ptAr[1].x,ptAr[1].y);			
		}
		else
			pDC->Polyline(ptAr, 5);
		
		tmpRGN.DeleteObject();
		tmpRGN.CreatePolygonRgn(ptAr, 5, WINDING);
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CombineRgn(m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn, &tmpRGN, RGN_OR);		
	}

	tmpRGN.DeleteObject();
	
	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		int	nGab = nSize;
		int	nRet = 0;
		nGab = nGab - nIndex;
		nGab = nGab / 5;

		if (nGab == 0)
			nGab = 1;

		
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);

		if (m_arData.GetSize() > 0)
		{
			for (int ii = 0 ; ii < 4 ; ii++ )
			{
				int idx = nGab * (ii+1);
				pt.x = (int)(castDVAL(m_rectGrp.left) + ( (castDVAL(idx) + 1) * ixval + ixval / 2.0));

				if (idx + nIndex >= nSize)
					continue;

				pObject = m_arData.GetAt(idx + nIndex);
				pt.y = (int)( (castDVAL(GetYPos(pObject->hpr)) + castDVAL(GetYPos(pObject->lpr))) / 2.0 );

				CRect	rc;	
				rc.SetRect(pt.x - rgnMARKGAP, pt.y - rgnMARKGAP, pt.x + rgnMARKGAP, pt.y + rgnMARKGAP);
				rc.IntersectRect(&rc, &m_rectGrp);
				pDC->Rectangle(&rc);
			}
		}
		pDC->SetROP2(sROP);
		pDC->SelectObject(srgnBrush);
	}

//	CString	str;
//	FormatVariableComma(str, m_JWValue, m_dYMax);
//	dispText.Format("Renko 최소변화폭:%s", str);
//	DrawTickIndex(pDC, dispText, m_pgraph->rgbLabel, true);


	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);

	m_RgnInfo.iCount++;
}

//void CVRenko::l_RectLine(CDC *pDC, CRect Rct, int Fg)// 1:left ,2:right, 0:all
//{
//	pDC->MoveTo(Rct.left,Rct.top);
//	if(Fg == 1)
//		pDC->MoveTo(Rct.left,Rct.bottom);
//	else
//		pDC->LineTo(Rct.left,Rct.bottom);
//	pDC->LineTo(Rct.right,Rct.bottom);
//	if(Fg == 2)
//		pDC->MoveTo(Rct.right,Rct.top);
//	else
//		pDC->LineTo(Rct.right,Rct.top);
//	pDC->LineTo(Rct.left,Rct.top);


bool CVRenko::CalculateMinMax(bool bForce)
{
	SetMember();

	if (!m_bCalculated || bForce)
	{
		if (!CreateData())
			return false;
		m_bCalculated = true;
	}
	CheckMinMax();
	
	int	nCount = m_arData.GetSize();

	if (nCount == 0)
	{
		m_fUnit = 0.0;
		return false;
	}
	int iPointCnt = m_iDispEPos - m_iDispSPos ;

	int	nWidth = m_rectGrp.Width();
//	int	nWidthCount = nWidth / 7;
	int	nWidthCount = nCount;
	
	if (nWidthCount <= 0)
		return false;

	m_nShow = nWidthCount;
	m_fUnit = (double)nWidth / (m_nShow);

	FindMinMax();	
	ReviseMinMax(m_dYMax, m_dYMin);
	return true;
}

void CVRenko::FindMinMax()
{
	m_dYMin = castDVAL(INT_MAX);
	m_dYMax = castDVAL(INT_MIN);
	
	struct _local_data*	pObject = NULL;
	int	nSize = m_arData.GetSize();

	int	nStart = 0;

	if (m_nShow < nSize)
		nStart = nSize - m_nShow;
	
	for ( int ii = m_iViweSidx ; ii < m_iViweEidx ; ii++ )
	{
		pObject = m_arData.GetAt(ii);
		if(!pObject)
			break;
		if (m_dYMax < pObject->hpr)
			m_dYMax = (int)pObject->hpr;
		
		if (m_dYMin > pObject->lpr)
			m_dYMin = (int)pObject->lpr;
	}

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return ;
	
	if (m_dwDspOpt & DO_SCREEN && m_pgraph->dwDraOption & GO_UNION)
		pOrgData->CalculateMinMax(m_iDispSPos, m_iDispEPos, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cPRC, TRUE);
	else
		pOrgData->CalculateMinMax(0, m_iTtlDtCnt, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cPRC, TRUE);
	
}



void CVRenko::RemoveData()
{
	int	ii = 0, nSize = m_arData.GetSize();
	struct	_local_data*	pObject = NULL;

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pObject = m_arData.GetAt(ii);
		delete pObject;		
	}

	m_arData.RemoveAll();
}

#if 0
double CVRenko::GetDefVal()
{
	double	minValue = DBL_MAX;
	double	maxValue = DBL_MIN;
	double	val = 0.0;
	COrgData*	pOrgData = NULL;

	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return 0.0;

	int	nSize = pOrgData->GetArraySize();
	int	ii = 0;
	CGrpBasic*	pBasic = NULL;

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pBasic = pOrgData->GetGraphData(ii);

		if (pBasic->m_iClose == GD_UNUSED)
			continue;

		if (pBasic->m_iHigh > maxValue)
			maxValue = pBasic->m_iHigh;

		if (pBasic->m_iLow < minValue)
			minValue = pBasic->m_iLow;
	}

	double	gap = maxValue - minValue;
	double	div;
	if (gap < 5)			div = 1;
	else if (gap < 50)		div = 5;
	else if (gap < 100)		div = 10;
	else if (gap < 200)		div = 20;
	else if (gap < 500)		div = 50;
	else if (gap < 1000)		div = 100;
	else if (gap < 2000)		div = 200;
	else if (gap < 5000)		div = 500;
	else if (gap < 10000)		div = 1000;
	else if (gap < 20000)		div = 2000;
	else if (gap < 50000)		div = 5000;
	else if (gap < 100000)		div = 10000;
	else if (gap < 200000)		div = 20000;
	else if (gap < 500000)		div = 50000;
	else if (gap < 1000000)		div = 100000;
	else if (gap < 2000000)		div = 200000;
	else if (gap < 5000000)		div = 500000;
	else if (gap < 10000000)	div = 1000000;
	else if (gap < 50000000)	div = 5000000;
	else if (gap < 100000000)	div = 10000000;
	else if (gap < 500000000)	div = 50000000;
	else if (gap < 1000000000)	div = 100000000;
	else				div = 500000000;

	if (div >= 5)
		div /= 4.0;

	return div;
}
#else
double CVRenko::GetDefVal()
{
	COrgData* pOrgData = NULL;
	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return 0.0;
	
	double	dMinValue = DBL_MAX;
	double	dMaxValue = DBL_MIN;
	
	int iSize = pOrgData->GetArraySize();
	CGrpBasic* pBasic = NULL;
	for (int  ii = 0 ; ii < iSize ; ii++ )
	{
		pBasic = pOrgData->GetGraphData(ii);
		
		if (pBasic->m_iClose == GD_UNUSED)
			continue;
		
		if (pBasic->m_iHigh > dMaxValue)
			dMaxValue = pBasic->m_iHigh;
		
		if (pBasic->m_iLow < dMinValue)
			dMinValue = pBasic->m_iLow;
	}

//TEST
	if(m_dFirsrDefVal == 0 )
	{
		m_dFirsrDefVal = (dMaxValue - dMinValue) * 0.02;
	}
	if(m_dFirsrDefVal != 0 && !(m_pgraph->dwDraOption & GO_UNION))
		return m_dFirsrDefVal;

	return (dMaxValue - dMinValue) * 0.02;
//	return 542.0;
	
}
#endif

#include <math.h>
int CVRenko::GetYPos(double val)
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


#define __LIKE_DAEWOO__	0

bool CVRenko::CreateData()
{
	COrgData*	pOrgData = NULL;

	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return false;

	if (!(m_pgraph->dwDraOption & GO_UNION))
	{
		m_iTtlDtCnt = pOrgData->GetArraySize();//TEST KANG
		m_iDispEPos = m_iTtlDtCnt;
	}


//int jk = sizeof(class CGrpBasic);

	int	nSize = m_iTtlDtCnt;
	int	ii = 0;
	CGrpBasic*	pBasic = NULL;

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pBasic = pOrgData->GetGraphData(ii);

		if (pBasic->m_iClose != GD_UNUSED)
			break;
	}


	if (ii > nSize - 2)
		return false;

	if (m_bCheck)	m_JWValue = GetDefVal();//125

	if (m_JWValue == 0)
		return false;

	RemoveData();
	m_OrgInDataStart = ii;
	int	nPoint = ii;
	double	pHpr = 0, pLpr = 0;
	double	val = 0, diff1 = 0, diff2 = 0;
	bool	bUp = false;
	int		nBox = 0, iGap = 0, iViewidx = 0;
	int		iSOrgidx, iEOrgidx;
	struct _local_data*	pObject = NULL;

	double		dSDay, dEDay, dMDay;
	CGrpBasic*	pBas = NULL;

	for ( ii = nPoint ; ii < nSize ; ii++ )
	{
		pBasic	= pOrgData->GetGraphData(ii);		
		val = (double)pBasic->m_iClose;

		if (val == GD_UNUSED)
			break;

		if(iGap == 0)
		{
			iSOrgidx = (ii >= 1) ? ii -1 : ii;
		}
		iGap++;
		
		if (ii == nPoint)
		{
			pHpr = pLpr = (double)pBasic->m_iClose;
			continue;
		}

		diff1 = val - pHpr;
		diff2 = pLpr - val;
		if (diff1 > 0)
			bUp = true;
		else if (diff2 > 0)
		{
			diff1 = diff2;
			bUp = false;
		}
		else 
			continue;

		nBox = (int)(diff1 / m_JWValue);
#if __LIKE_DAEWOO__
		if (bUp)
		{
		if (((int)diff1 % (int)m_JWValue) == 0)
			nBox--;
		}
#endif

		if (!nBox)
			continue;

//		iEOrgidx = (ii >= 1) ? ii -1 : ii;
		iEOrgidx = ii;
		
		for ( int jj = 0 ; jj < nBox ; jj++ )
		{
			pObject = new struct _local_data;
			m_arData.Add(pObject);
			
			memcpy(&pObject->info.date, &pBasic->m_index, sz_gIndex);
			pObject->info.fpr = (double)pBasic->m_iOpen;
			pObject->info.hpr = (double)pBasic->m_iHigh;
			pObject->info.lpr = (double)pBasic->m_iLow;
			pObject->info.epr = (double)pBasic->m_iClose;
			pObject->bUp = bUp;
			pObject->iGap = iGap;

			pBas = pOrgData->GetGraphData(iSOrgidx);
			dSDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
			dSDay = (dSDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
			pBas = pOrgData->GetGraphData(iEOrgidx);
			dEDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
			dEDay = (dEDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
			pBas = pOrgData->GetGraphData((iSOrgidx + iEOrgidx)/2);
			dMDay = (castDVAL(pBas->m_index.date.yy) * 10000 ) + (castDVAL(pBas->m_index.date.mm) * 100 ) + pBas->m_index.date.dd;
			dMDay = (dMDay * 1000000) + (castDVAL(pBas->m_index.time.hh) * 10000) + (castDVAL(pBas->m_index.time.mm) * 100) + pBas->m_index.time.ss;
			
			if(iGap > 0)
			{
				pObject->iSOrgidx = iSOrgidx;
				pObject->iEOrgidx = iEOrgidx;
				pObject->dSDay = dSDay;
				pObject->dEDay = dEDay;
				pObject->dMDay = dMDay;
			}
			else
			{
				pObject->iSOrgidx = iEOrgidx;
				pObject->iEOrgidx = iEOrgidx;
				pObject->dSDay = dEDay;
				pObject->dEDay = dEDay;
				pObject->dMDay = dEDay;
			}

			if (bUp)
			{
				pObject->hpr = pHpr + m_JWValue;
				pObject->lpr = pHpr;					
			}
			else
			{
				pObject->hpr = pLpr;
				pObject->lpr = pLpr - m_JWValue;
			}	

			pHpr = pObject->hpr;
			pLpr = pObject->lpr;
			iGap = 0;
		}
		int ss = 0;
	}
	return true;
}

bool CVRenko::CheckMinMax()
{
	COrgData*	pOrgData = NULL;
	struct _local_data*	pObject = NULL;
	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return false;
	
	
	int	ii, nSize;
	CGrpBasic*	pBasic = NULL;

	nSize = m_arData.GetSize();
	
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
		pObject = m_arData.GetAt(ii);
		if(!pObject)
			break;
		if(m_iViweDataCnt == 0 && pObject->iEOrgidx > iViewOrg_Sidx)
		{
			m_iViweSidx = ii;
			m_iViweDataCnt++;
			continue;
		}
		if(m_iViweDataCnt != 0 && pObject->iSOrgidx < iViewOrg_Eidx)
		{
			m_iViweEidx = ii;
			m_iViweDataCnt++;
			continue;
		}
	}

	return true;
}


bool CVRenko::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
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

CString CVRenko::GetDisplayPosHeader(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)	return _T("");

	int	nCount = m_arData.GetSize();
	if (nCount == 0 || m_nShow == 0)	return _T("");

	struct _local_data* pObject = NULL;
	int	nIndex = 0;

	int	nGab = (pt.x - m_rectGrp.left);

	if (nGab < 0)	return _T("");

	nIndex = (int)((double)nGab / m_fUnit);
	if (nCount > m_nShow)	nIndex += nCount - m_nShow;
	if (nIndex < 0 || nIndex >= nCount)	return _T("");

	CString	unitStr1 = _T("");
	pObject = m_arData.GetAt(nIndex);

	if (bGrid)
	{
		switch (GetDataIndex())
		{
		case GI_MINUTE:
		case GI_TICK:
			if (pObject->info.date.date.dd == 0)
				unitStr1.Format("~~시간:%02d:%02d:%02d\t", pObject->info.date.time.hh,
								pObject->info.date.time.mm, pObject->info.date.time.ss);
			else
				unitStr1.Format("~~시간:%02d/%02d-%02d:%02d:%02d\t", 
					pObject->info.date.date.mm, pObject->info.date.date.dd,
					pObject->info.date.time.hh, pObject->info.date.time.mm, pObject->info.date.time.ss);
			break;
		default:
			unitStr1.Format("~~일자:%04d/%02d/%02d\t", pObject->info.date.date.yy,
				pObject->info.date.date.mm, pObject->info.date.date.dd);
			break;
		}
	}
	else
	{
		switch (GetDataIndex())
		{
		case GI_MINUTE:
		case GI_TICK:
			unitStr1.Format("시간\t%02d:%02d:%02d\t", pObject->info.date.time.hh,
				pObject->info.date.time.mm, pObject->info.date.time.ss);
			break;
		default:
			unitStr1.Format("일자\t%04d/%02d/%02d\t", pObject->info.date.date.yy,
				pObject->info.date.date.mm, pObject->info.date.date.dd);
			break;
		}
	}

	return unitStr1;
}

CString CVRenko::GetDisplayPosData(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	int	nCount = m_arData.GetSize();
	if (nCount == 0 || m_nShow == 0)
		return _T("");

	struct _local_data* pObject = NULL;
	int	nIndex = 0;

	int	nGab = (pt.x - m_rectGrp.left);

	if (nGab < 0)
		return _T("");

	nIndex = (int)((double)nGab / m_fUnit);
	if (nCount > m_nShow)
		nIndex += nCount - m_nShow;
	if (nIndex < 0 || nIndex >= nCount)
		return _T("");

	CString	unitStr1 = _T("");
	pObject = m_arData.GetAt(nIndex);

	CString	sFpr, sHpr, sLpr, sEpr, sUp, sDown;
	if (pObject->info.epr == GD_UNUSED)
	{
		unitStr1 = _T("");
	}
	else
	{
		// added by schbang 2006.03.17, calculate percent
		CString strOpenP;
		CString strHighP;
		CString strLowP;
		CString strCloseP;
		if (nIndex - 1 >= 0)
		{
			struct _local_data* pldPrev = m_arData.GetAt(nIndex - 1);
			if (pldPrev->info.epr != GD_UNUSED)
			{
				double dPcnt = (double)(pObject->info.fpr - pldPrev->info.epr) / (double)pldPrev->info.epr * 100;
				dPcnt < 0 ? strOpenP.Format("  (%0.2f%%)", dPcnt) : strOpenP.Format("  (+%0.2f%%)", dPcnt);

				dPcnt = (double)(pObject->info.hpr - pldPrev->info.epr) / (double)pldPrev->info.epr * 100;
				dPcnt < 0 ? strHighP.Format("  (%0.2f%%)", dPcnt) : strHighP.Format("  (+%0.2f%%)", dPcnt);

				dPcnt = (double)(pObject->info.lpr - pldPrev->info.epr) / (double)pldPrev->info.epr * 100;
				dPcnt < 0 ? strLowP.Format("  (%0.2f%%)", dPcnt) : strLowP.Format("  (+%0.2f%%)", dPcnt);

				dPcnt = (double)(pObject->info.epr - pldPrev->info.epr) / (double)pldPrev->info.epr * 100;
				dPcnt < 0 ? strCloseP.Format("  (%0.2f%%)", dPcnt) : strCloseP.Format("  (+%0.2f%%)", dPcnt);
			}
		}

		CIndcBase::FormatVariableComma(sFpr, pObject->info.fpr, pObject->info.fpr);
		CIndcBase::FormatVariableComma(sHpr, pObject->info.hpr, pObject->info.hpr);
		CIndcBase::FormatVariableComma(sLpr, pObject->info.lpr, pObject->info.lpr);
		CIndcBase::FormatVariableComma(sEpr, pObject->info.epr, pObject->info.epr);
		CIndcBase::FormatVariableComma(sUp, pObject->hpr, pObject->hpr);
		CIndcBase::FormatVariableComma(sDown, pObject->lpr, pObject->lpr);
		if (bGrid)
			unitStr1.Format("시가:%s\t고가:%s\t저가:%s\t$$종가:%s\t상단:%s\t하단:%s", 
				LPCTSTR(sFpr + strOpenP), LPCTSTR(sHpr + strHighP), LPCTSTR(sLpr + strLowP), LPCTSTR(sEpr + strCloseP), LPCTSTR(sUp), LPCTSTR(sDown));
		else
			unitStr1.Format("시가\t%s\t고가\t%s\t저가\t%s\t종가\t%s\t상단\t%s\t하단\t%s",
				LPCTSTR(sFpr + strOpenP), LPCTSTR(sHpr + strHighP), LPCTSTR(sLpr + strLowP), LPCTSTR(sEpr + strCloseP), LPCTSTR(sUp), LPCTSTR(sDown));
	}

	return unitStr1;
}

CString CVRenko::GetXposValue(CPoint pt)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)	return _T("");

	int	nCount = m_arData.GetSize();
	if (nCount == 0 || m_nShow == 0)	return _T("");

	struct _local_data* pObject = NULL;
	int	nIndex = 0;

	int	nGab = (pt.x - m_rectGrp.left);

	if (nGab < 0)	return _T("");

	nIndex = (int)((double)nGab / m_fUnit);
	if (nCount > m_nShow)	nIndex += nCount - m_nShow;
	if (nIndex < 0 || nIndex >= nCount)	return _T("");

	CString	unitStr1 = _T("");
	pObject = m_arData.GetAt(nIndex);

	switch (GetDataIndex())
	{
	case GI_MINUTE:
	case GI_TICK:
		unitStr1.Format("%02d/%02d %02d:%02d:%02d", 
			pObject->info.date.date.mm, pObject->info.date.date.dd, 
			pObject->info.date.time.hh, pObject->info.date.time.mm, pObject->info.date.time.ss);
		break;
	default:
		unitStr1.Format("%04d/%02d/%02d", 
			pObject->info.date.date.yy, pObject->info.date.date.mm, pObject->info.date.date.dd);
		break;
	}

	return unitStr1;
}
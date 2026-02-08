#include "stdafx.h"
#include "VKagi.h"
#include "../gData/DataMgr.h"
#include <math.h>
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CVKagi::CVKagi(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
: CVarsBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_bCalculated	= false;
	m_bCheck	= true;;
	m_iValue	= 0;
	m_iShow		= 0;
	m_dUnit		= 0;

	m_arData.RemoveAll();
}

CVKagi::~CVKagi()
{
	RemoveData();
}

void CVKagi::DrawGraph(CDC* pDC)
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
		DrawUnionChart(pDC);
		DrawBottomStick(pDC);
	}
	else
		DrawChart(pDC);
}

void CVKagi::SetMember()
{
//	int nCount = m_arData.GetSize();

	if (m_bCheck)
		m_iValue = (int)GetDefVal();
}

void CVKagi::SetUserDefined(CString sVal)
{
	if (sVal.IsEmpty())
		return;

	m_bCheck = false;

	switch (GetDataUnit())
	{
	case GU_CODE:	case GU_COFUT:	case GU_COOPT:
		m_iValue = atoi(sVal);
		break;
	default:
		m_iValue = int(atof(sVal) * 100.0);
		break;
	}	
}

void CVKagi::GetUsedValue(CString &sVal)
{
	switch (GetDataUnit())
	{
	case GU_CODE:	case GU_COFUT:	case GU_COOPT:
		sVal.Format("%d", m_iValue);
		break;
	default:
		sVal.Format("%f", m_iValue/100.0);
		break;
	}
}


void CVKagi::DrawChart(CDC* pDC)
{
	if (m_arData.GetSize() < 0)
		return;

	// 전제 다그리기
	m_iShow = m_arData.GetSize();
	m_dUnit = (double)m_rectGrp.Width() / m_iShow;

int iPointCnt = m_iDispEPos - m_iDispSPos ;


	CPen* ppenUp = GetPen(m_pgraph->cInfo.aline[0].btLStyle, m_pgraph->cInfo.aline[0].btLWidth, m_pgraph->cInfo.aline[0].rgbLColor);
	CPen* ppenDown = GetPen(m_pgraph->cInfo.aline[1].btLStyle, m_pgraph->cInfo.aline[1].btLWidth, m_pgraph->cInfo.aline[1].rgbLColor);
	CPen* ppenOld = pDC->SelectObject(ppenUp);

	CPoint point;
	CPoint*	ppoint = NULL;
	CString	strDisp;
	int iDataSize = m_arData.GetSize();
	int iIndex = 0;

	if (m_iShow < iDataSize)
		iIndex = iDataSize - m_iShow;

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	
	bool bThick = false;
	bool bUp = bThick;	// true : red false : blue
	double dXpos = (double)m_rectGrp.left;
	double dMidVal = 0.0;
	double dStandard = 0.0;
	int iMid = 0;
	int iPt = 0;
	int iColor = 0;
	struct _local_data* pldCur = NULL;
	struct _local_data* pldPre = NULL;
	CRgn rgnTemp;
	CRect rect;
	CArray<CPoint, CPoint> arPt;
	CMap<int, int, int, int> mapColor;
	
	

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreateRectRgn(0, 0, 0, 0);	

	for ( int ii = 0 ; ii < iDataSize ; ii++ )
	{
		pldCur = m_arData.GetAt(ii);

		if (ii)
			pldPre = m_arData.GetAt(ii - 1);

		
		if (pldCur->adVal[1] > pldCur->adVal[0])
			bUp = true;
		else
			bUp = false;

		dMidVal = (pldCur->adVal[1] + pldCur->adVal[0]) / 2.0;

		if (!pldPre)
		{
			if (bUp)
				bThick = true;
			else
				bThick = false;
			
			if (bThick)
				pDC->SelectObject(ppenUp);
			else
				pDC->SelectObject(ppenDown);

			arPt.RemoveAll();

			iMid = (int)(dXpos + m_dUnit / 2.0);
			point.x = (int)dXpos;
			point.y = GetYPos(pldCur->adVal[0]);
			arPt.Add(point);
			
			point.x = iMid;
			arPt.Add(point);

			point.y = GetYPos(dMidVal);
			arPt.Add(point);

			point.x = (int)(dXpos + m_dUnit);
			arPt.Add(point);

			point.x = iMid;
			arPt.Add(point);

			point.y = GetYPos(pldCur->adVal[1]);
			arPt.Add(point);

			point.x = (int)(dXpos + m_dUnit);
			arPt.Add(point);

			iPt = arPt.GetSize();

			if (ii < iIndex)
				continue;	

			for ( int jj = 0 ; jj < iPt ; jj++ )
			{
				point = arPt.GetAt(jj);

				if (jj == 0)
					pDC->MoveTo(point);
				else
				{
					if (ii == (iDataSize - 1))
					{
						if (jj != (iPt - 1))
							pDC->LineTo(point);
					}
					else
						pDC->LineTo(point);
				}
			}
		}
		else
		{	
			arPt.RemoveAll();
			mapColor.RemoveAll();
			dStandard = pldPre->adVal[0];
			iMid = (int)(dXpos + m_dUnit / 2.0);
			
			point.x = (int)dXpos;
			point.y = GetYPos(pldCur->adVal[0]);
			arPt.Add(point);
			
			point.x = iMid;
			arPt.Add(point);

			if (bThick)
				iColor = 1;
			else
				iColor = 0;
				
			mapColor.SetAt(arPt.GetSize() - 1, iColor);

			if (bUp)
			{				
				if (!bThick && pldCur->adVal[1] > dStandard)
				{
					if (dMidVal > dStandard)
					{
						point.y = GetYPos(dStandard);
						arPt.Add(point);					
						mapColor.SetAt(arPt.GetSize() - 1, iColor);						
						
						point.y = GetYPos(dMidVal);
						arPt.Add(point);

						bThick = !bThick;

						if (bThick)
							iColor = 1;						
						else
							iColor = 0;

						mapColor.SetAt(arPt.GetSize() - 1, iColor);
						
						point.x = (int)(dXpos + m_dUnit);
						arPt.Add(point);
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.x = iMid;
						arPt.Add(point);
						mapColor.SetAt(arPt.GetSize() - 1, iColor);
					}
					else
					{
						point.y = GetYPos(dMidVal);
						arPt.Add(point);						
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.x = (int)(dXpos + m_dUnit);
						arPt.Add(point);						
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.x = iMid;
						arPt.Add(point);				
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.y = GetYPos(dStandard);
						arPt.Add(point);					
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						bThick = !bThick;

						if (bThick)
							iColor = 1;						
						else
							iColor = 0;
					}

					
				}
				else
				{
					point.y = GetYPos(dMidVal);
					arPt.Add(point);						
					mapColor.SetAt(arPt.GetSize() - 1, iColor);

					point.x = (int)(dXpos + m_dUnit);
					arPt.Add(point);						
					mapColor.SetAt(arPt.GetSize() - 1, iColor);

					point.x = iMid;
					arPt.Add(point);				
					mapColor.SetAt(arPt.GetSize() - 1, iColor);									
				}
			}
			else
			{
				if (bThick && pldCur->adVal[1] < dStandard)
				{
					if (dMidVal < dStandard)
					{
						point.y = GetYPos(dStandard);
						arPt.Add(point);					
						mapColor.SetAt(arPt.GetSize() - 1, iColor);						
						
						point.y = GetYPos(dMidVal);
						arPt.Add(point);

						bThick = !bThick;

						if (bThick)
							iColor = 1;						
						else
							iColor = 0;

						mapColor.SetAt(arPt.GetSize() - 1, iColor);
						
						point.x = (int)(dXpos + m_dUnit);
						arPt.Add(point);
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.x = iMid;
						arPt.Add(point);
						mapColor.SetAt(arPt.GetSize() - 1, iColor);
					}
					else
					{
						point.y = GetYPos(dMidVal);
						arPt.Add(point);						
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.x = (int)(dXpos + m_dUnit);
						arPt.Add(point);						
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.x = iMid;
						arPt.Add(point);				
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.y = GetYPos(dStandard);
						arPt.Add(point);					
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						bThick = !bThick;

						if (bThick)
							iColor = 1;						
						else
							iColor = 0;
					}					
				}
				else
				{
					point.y = GetYPos(dMidVal);
					arPt.Add(point);						
					mapColor.SetAt(arPt.GetSize() - 1, iColor);

					point.x = (int)(dXpos + m_dUnit);
					arPt.Add(point);						
					mapColor.SetAt(arPt.GetSize() - 1, iColor);

					point.x = iMid;
					arPt.Add(point);				
					mapColor.SetAt(arPt.GetSize() - 1, iColor);									
				}
			}

			point.y = GetYPos(pldCur->adVal[1]);
			arPt.Add(point);
			mapColor.SetAt(arPt.GetSize() - 1, iColor);

			point.x = (int)(dXpos + m_dUnit);
			arPt.Add(point);
			mapColor.SetAt(arPt.GetSize() - 1, iColor);

			iPt = arPt.GetSize();
			int	nPrev = -1;

			if (ii < iIndex)
				continue;

			for ( int jj = 0 ; jj < iPt ; jj++ )
			{
				point = arPt.GetAt(jj);

				if (jj == 0)
					pDC->MoveTo(point);
				else
				{
					mapColor.Lookup(jj, iColor);

					if (nPrev != iColor)
					{
						if (iColor)
							pDC->SelectObject(ppenUp);
						else
							pDC->SelectObject(ppenDown);
					}
					
					nPrev = iColor;

					if (ii == (iDataSize - 1))
					{
						if (jj != (iPt - 1))
							pDC->LineTo(point);
					}
					else
						pDC->LineTo(point);
				}
			}		
		}

		CPoint	ptTmp[2];

		ptTmp[0] = arPt.GetAt(0);
		ptTmp[1] = arPt.GetAt(iPt - 1);
		
		if (bUp)
			rect.SetRect(CPoint(ptTmp[0].x, ptTmp[1].y), CPoint(ptTmp[1].x, ptTmp[0].y));
		else
			rect.SetRect(ptTmp[0], ptTmp[1]);
		
		rgnTemp.DeleteObject();
		rgnTemp.CreateRectRgnIndirect(rect);
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CombineRgn(m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn, &rgnTemp, RGN_OR);
		dXpos += m_dUnit;
	}

	rgnTemp.DeleteObject();

	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		int	nGab = iDataSize;
		int	nRet = 0;
		nGab = nGab - iIndex;
		nGab = nGab / 5;

		if (nGab == 0)
			nGab = 1;
		
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);
		for (int ii = 0; ii < 5; ii++)
		{
			int idx = nGab * (ii+1);
			point.x = (int)((double)m_rectGrp.left + ((double)(idx + 1) * m_dUnit));	

			if (idx + iIndex >= iDataSize)
				continue;

			pldCur = m_arData.GetAt(idx + iIndex);
			point.y = (int)((GetYPos(pldCur->adVal[0]) + GetYPos(pldCur->adVal[1])) / 2.0);


			CRect	rc;	
			rc.SetRect(point.x - rgnMARKGAP, point.y - rgnMARKGAP, 
				point.x + rgnMARKGAP, point.y + rgnMARKGAP);
			rc.IntersectRect(&rc, &m_rectGrp);
			pDC->Rectangle(&rc);
		}
		pDC->SetROP2(sROP);
		pDC->SelectObject(srgnBrush);
	}

	CString	str;
	FormatVariableComma(str, m_iValue, m_dYMax);
	strDisp.Format("Kagi 최소반전폭:%s", str);
	DrawTickIndex(pDC, strDisp, m_pgraph->rgbLabel, true);

	m_RgnInfo.iCount++;
	
	pDC->SelectObject(ppenOld);
}

void CVKagi::DrawUnionChart(CDC* pDC)
{
	if (m_arData.GetSize() < 0)
		return;

	if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_PRC)
	{
		m_dYMax = m_pCoDraw->PMax;
		m_dYMin = m_pCoDraw->PMin;
	}

	int iDrawHeight = m_rectGrp.Height();
	int iDispCnt = m_iDispEPos - m_iDispSPos ;

	// 전제 다그리기
	m_dUnit = (double)m_rectGrp.Width() / iDispCnt;

	CPen* ppenUp = GetPen(m_pgraph->cInfo.aline[0].btLStyle, m_pgraph->cInfo.aline[0].btLWidth, m_pgraph->cInfo.aline[0].rgbLColor);
	CPen* ppenDown = GetPen(m_pgraph->cInfo.aline[1].btLStyle, m_pgraph->cInfo.aline[1].btLWidth, m_pgraph->cInfo.aline[1].rgbLColor);
	CPen* ppenOld = pDC->SelectObject(ppenUp);

	CPoint point;
	CPoint*	ppoint = NULL;
	CString	strDisp;
	int iDataSize = m_arData.GetSize();
	int iIndex = 0;


	CRgn rgnDraw;
	rgnDraw.CreateRectRgnIndirect(m_rectGrp);
	pDC->SelectClipRgn(&rgnDraw);

	// 라인 영역 초기화

	bool bThick = false;
	bool bUp = bThick;	// true : red false : blue
	//double dXpos = (double)m_rectGrp.left;
	double dXpos = (double)m_rectGrp.left - m_dUnit * m_iDispSPos + m_dUnit * m_iBlankGap;
	
	double dMidVal = 0.0;
	double dStandard = 0.0;
	int iMid = 0;
	int iPt = 0;
	int iColor = 0;
	struct _local_data* pldCur = NULL;
	struct _local_data* pldPre = NULL;

	CRect rect;
	CArray<CPoint, CPoint> arPt;
	CMap<int, int, int, int> mapColor;
	bool bReverse = IsReverseChart();

	for (int ii = 0 ; ii < iDataSize ; ii++)
	{
		pldCur = m_arData.GetAt(ii);

		if (ii)
			pldPre = m_arData.GetAt(ii - 1);

		if (pldCur->adVal[1] > pldCur->adVal[0])
			bUp = true;
		else
			bUp = false;

		dMidVal = (pldCur->adVal[1] + pldCur->adVal[0]) / 2.0;

		if (!pldPre)	// 처음
		{
			if (bUp)
				bThick = true;
			else
				bThick = false;
			
			if (bThick)
			{
				pDC->SelectObject(ppenUp);
				m_arColorX.Add(m_pgraph->cInfo.aline[0].rgbLColor);
			}
			else
			{
				pDC->SelectObject(ppenDown);
				m_arColorX.Add(m_pgraph->cInfo.aline[1].rgbLColor);
			}

			arPt.RemoveAll();

			iMid = (int)(dXpos + m_dUnit / 2.0);
			point.x = (int)dXpos;
			point.y = GetYPos(pldCur->adVal[0]);
			arPt.Add(point);

			m_arPointX.Add(point.x);
			
			point.x = iMid;
			arPt.Add(point);

			point.y = GetYPos(dMidVal);
			arPt.Add(point);

			point.x = (int)(dXpos + m_dUnit / 2);
			arPt.Add(point);

			point.x = iMid;
			arPt.Add(point);

			point.y = GetYPos(pldCur->adVal[1]);
			arPt.Add(point);

			point.x = (int)(dXpos + m_dUnit * pldCur->iGap);
			arPt.Add(point);

			iPt = arPt.GetSize();

			for ( int jj = 0 ; jj < iPt ; jj++ )
			{
				point = arPt.GetAt(jj);

				if (jj == 0)
					pDC->MoveTo(point);
				else
				{
					if (ii == (iDataSize - 1))
					{
						if (jj != (iPt - 1))
							pDC->LineTo(point);
					}
					else
						pDC->LineTo(point);
				}
			}
		}
		else	// 2번째 이상
		{	
			arPt.RemoveAll();
			mapColor.RemoveAll();
			dStandard = pldPre->adVal[0];
			iMid = (int)(dXpos + m_dUnit / 2.0);
			
			point.x = (int)dXpos;
			point.y = GetYPos(pldCur->adVal[0]);
			arPt.Add(point);
			
			point.x = iMid;
			arPt.Add(point);

			m_arPointX.Add(point.x);

			if (bThick)
			{
				iColor = 1;
				m_arColorX.Add(m_pgraph->cInfo.aline[0].rgbLColor);

			}
			else
			{
				iColor = 0;
				m_arColorX.Add(m_pgraph->cInfo.aline[1].rgbLColor);
			}
				
			mapColor.SetAt(arPt.GetSize() - 1, iColor);

			if (bUp)
			{				
				if (!bThick && pldCur->adVal[1] > dStandard)
				{
					if (dMidVal > dStandard)
					{
						point.y = GetYPos(dStandard);
						arPt.Add(point);
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.y = GetYPos(dMidVal);
						arPt.Add(point);
						m_arPointX.Add(point.x);

						bThick = !bThick;

						if (bThick)
						{
							iColor = 1;
							m_arColorX.Add(m_pgraph->cInfo.aline[0].rgbLColor);
						}
						else
						{
							iColor = 0;
							m_arColorX.Add(m_pgraph->cInfo.aline[1].rgbLColor);
						}

						mapColor.SetAt(arPt.GetSize() - 1, iColor);
						
						point.x = (int)(dXpos + m_dUnit);
						arPt.Add(point);
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.x = iMid;
						arPt.Add(point);
						mapColor.SetAt(arPt.GetSize() - 1, iColor);
					}
					else
					{
						point.y = GetYPos(dMidVal);
						arPt.Add(point);
						m_arPointX.Add(point.x);
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.x = (int)(dXpos + m_dUnit);
						arPt.Add(point);						
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.x = iMid;
						arPt.Add(point);				
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.y = GetYPos(dStandard);
						arPt.Add(point);					
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						bThick = !bThick;

						if (bThick)
						{
							iColor = 1;
							m_arColorX.Add(m_pgraph->cInfo.aline[0].rgbLColor);
						}
						else
						{
							iColor = 0;
							m_arColorX.Add(m_pgraph->cInfo.aline[1].rgbLColor);
						}
					}
				}
				else
				{
					point.y = GetYPos(dMidVal);
					arPt.Add(point);						
					mapColor.SetAt(arPt.GetSize() - 1, iColor);

					point.x = (int)(dXpos + m_dUnit);

					arPt.Add(point);						
					mapColor.SetAt(arPt.GetSize() - 1, iColor);

					point.x = iMid;
					arPt.Add(point);				
					mapColor.SetAt(arPt.GetSize() - 1, iColor);									
				}
			}
			else
			{
				if (bThick && pldCur->adVal[1] < dStandard)
				{
					if (dMidVal < dStandard)
					{
						point.y = GetYPos(dStandard);
						arPt.Add(point);					
						mapColor.SetAt(arPt.GetSize() - 1, iColor);						
						
						point.y = GetYPos(dMidVal);
						arPt.Add(point);
						m_arPointX.Add(point.x);

						bThick = !bThick;

						if (bThick)
						{
							iColor = 1;
							m_arColorX.Add(m_pgraph->cInfo.aline[0].rgbLColor);
						}
						else
						{
							iColor = 0;
							m_arColorX.Add(m_pgraph->cInfo.aline[1].rgbLColor);
						}

						mapColor.SetAt(arPt.GetSize() - 1, iColor);
						
						point.x = (int)(dXpos + m_dUnit);
						arPt.Add(point);
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.x = iMid;
						arPt.Add(point);
						mapColor.SetAt(arPt.GetSize() - 1, iColor);
					}
					else
					{
						point.y = GetYPos(dMidVal);
						arPt.Add(point);
						m_arPointX.Add(point.x);
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.x = (int)(dXpos + m_dUnit);
						arPt.Add(point);						
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.x = iMid;
						arPt.Add(point);				
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						point.y = GetYPos(dStandard);
						arPt.Add(point);					
						mapColor.SetAt(arPt.GetSize() - 1, iColor);

						bThick = !bThick;

						if (bThick)
						{
							iColor = 1;
							m_arColorX.Add(m_pgraph->cInfo.aline[0].rgbLColor);
						}
						else
						{
							iColor = 0;
							m_arColorX.Add(m_pgraph->cInfo.aline[1].rgbLColor);
						}
					}					
				}
				else
				{
					point.y = GetYPos(dMidVal);
					arPt.Add(point);						
					mapColor.SetAt(arPt.GetSize() - 1, iColor);

					point.x = (int)(dXpos + m_dUnit);
					arPt.Add(point);						
					mapColor.SetAt(arPt.GetSize() - 1, iColor);

					point.x = iMid;
					arPt.Add(point);				
					mapColor.SetAt(arPt.GetSize() - 1, iColor);									
				}
			}

			point.y = GetYPos(pldCur->adVal[1]);
			arPt.Add(point);
			mapColor.SetAt(arPt.GetSize() - 1, iColor);

			point.x = (int)(dXpos + m_dUnit * pldCur->iGap);
			arPt.Add(point);
			mapColor.SetAt(arPt.GetSize() - 1, iColor);

			iPt = arPt.GetSize();
			int	nPrev = -1;

			if (ii < iIndex)
				continue;

			for ( int jj = 0 ; jj < iPt ; jj++ )
			{
				point = arPt.GetAt(jj);

				if (jj == 0)
					pDC->MoveTo(point);
				else
				{
					mapColor.Lookup(jj, iColor);

					if (nPrev != iColor)
					{
						if (iColor)
							pDC->SelectObject(ppenUp);
						else
							pDC->SelectObject(ppenDown);
					}
					
					nPrev = iColor;

					if (ii == (iDataSize - 1))
					{
						if (jj != (iPt - 1))
							pDC->LineTo(point);
					}
					else
						pDC->LineTo(point);
				}
			}
		}

		dXpos += m_dUnit * pldCur->iGap;
	}

/*
	CString	str;
	FormatVariableComma(str, m_iValue, m_dYMax);
	strDisp.Format("Kagi 최소반전폭:%s", str);
	DrawTickIndex(pDC, strDisp, m_pgraph->rgbLabel, true);
*/

	pDC->SelectClipRgn(NULL);
	rgnDraw.DeleteObject();
	
	pDC->SelectObject(ppenOld);
}

bool CVKagi::CalculateMinMax(bool bForce)
{
	SetMember();

	if (!m_bCalculated || bForce)
	{
		if (!CreateData())
			return false;
		m_bCalculated = true;
	}

	// find minmax;
	int	nSize = m_arData.GetSize();
	struct _local_data*	pObject = NULL;
	int	nSubSize = sizeof(pObject->adVal) / sizeof(double);
	double	minValue = DBL_MAX;
	double	maxValue = DBL_MIN;

	for ( int ii = 0 ; ii < nSize ; ii++ )
	{
		pObject = m_arData.GetAt(ii);

		for ( int jj = 0 ; jj < nSubSize ; jj++ )
		{
			if (pObject->adVal[jj] > maxValue)
				maxValue = pObject->adVal[jj];
			if (pObject->adVal[jj] < minValue)
				minValue = pObject->adVal[jj];
		}
	}

	m_dYMax = maxValue;
	m_dYMin = minValue;
// 추가
//*
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return false;

	if (m_dwDspOpt & DO_SCREEN && m_pgraph->dwDraOption & GO_UNION)
		pOrgData->CalculateMinMax(m_iDispSPos, m_iDispEPos, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cPRC, bForce);
	else
		pOrgData->CalculateMinMax(0, m_iTtlDtCnt, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cPRC, bForce);
//*/

//
	
	ReviseMinMax(m_dYMax, m_dYMin);

	if (nSize == 0)
	{
		m_dUnit = 0.0;
		return false;
	}

	int	nWidth = m_rectGrp.Width();
	int	nScroll = GetSystemMetrics(SM_CXVSCROLL);
	int	nWidthCount = nWidth / nScroll;

	if (nWidthCount <= 0)
		return false;

	m_iShow = nWidthCount;
	m_dUnit = (double)nWidth / m_iShow;

	return true;
}

void CVKagi::RemoveData()
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

/*
double CVKagi::GetDefVal()
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
*/
double CVKagi::GetDefVal()
{
	COrgData* pOrgData = NULL;
	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return 0.0;

	double dMinValue = DBL_MAX;
	double dMaxValue = DBL_MIN;

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

	if (IsLogChart())
		return (GetLogData(dMaxValue) - GetLogData(dMinValue)) * 0.02;

	return (dMaxValue - dMinValue) * 0.02;
}

/*
int CVKagi::GetYPos(double val)
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
*/
int CVKagi::GetYPos(double dVal)
{
	bool bLog = IsLogChart();

	//double dDiff = m_dYMax - m_dYMin;
	double dDiff = bLog ? (GetLogData(m_dYMax) -  GetLogData(m_dYMin)) : (m_dYMax - m_dYMin);
	double dRectHeight = m_rectGrp.Height();
	double dYPos = 0;
	double dGab =  bLog ? (GetLogData(m_dYMax) -  GetLogData(dVal) ) : m_dYMax - dVal;

	if (dDiff <= 0.0)
		return m_rectGrp.bottom;

	dYPos = dRectHeight * dGab / dDiff;

	if (IsReverseChart())
		return (int)((double)m_rectGrp.bottom - dYPos);

	return (int)((double)m_rectGrp.top + dYPos);
}

/*
bool CVKagi::CreateData()
{
	COrgData* pOrgData = NULL;

	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return false;

	int iSize = m_iTtlDtCnt;
	int ii = 0;
	CGrpBasic* pBasic = NULL;

	for ( ii = 0 ; ii < iSize ; ii++ )
	{
		pBasic = pOrgData->GetGraphData(ii);

		if (pBasic->m_iClose != GD_UNUSED)
			break;
	}

	RemoveData();

	if (ii > iSize - 2)
		return false;

	int iPoint = ii;
	double dPreVal = 0;
	double dCurVal = 0;
	double dVal = 0;
	double dDiff = 0;
	bool bUp = false;
	bool bPrev = false;
	bool bTurn = true;
	bool bStart = false;
//	bool bPaning = false;	
	
	struct _local_data* pldCur = NULL;
	
	bPrev = bUp;

	for ( ii = iPoint ; ii < iSize ; ii++ )
	{
		pBasic = pOrgData->GetGraphData(ii);
		
		dCurVal = (double)pBasic->m_iClose;

		if (dCurVal == GD_UNUSED)
			break;

		if (ii == iPoint)
		{
			dPreVal = dCurVal;
			continue;
		}

		if (dCurVal > dPreVal)
			bUp = true;
		else if (dCurVal < dPreVal)
			bUp = false;
		else
			continue;

		if (bStart && bPrev != bUp)
		{
			dDiff = fabs(dPreVal - dCurVal);

			if (dDiff >= m_iValue)
				bTurn = true;
			else
			{
				bUp = bPrev;
				continue;
			}
		}
		
		bStart = true;
		
		if (bTurn)
		{
			if (pldCur)
				dPreVal = pldCur->adVal[1];
			
			pldCur = new _local_data;
			pldCur->adVal[0] = dPreVal;
			pldCur->adVal[1] = dCurVal;

			memcpy(&pldCur->gIndex, &pBasic->m_index, sz_gIndex);
			pldCur->dOpen = (double)pBasic->m_iOpen;
			pldCur->dHigh = (double)pBasic->m_iHigh;
			pldCur->dLow = (double)pBasic->m_iLow;
			pldCur->dClose = (double)pBasic->m_iClose;

			bTurn = false;	
			m_arData.Add(pldCur);
		}
		else
		{
			if (bUp)
			{
				if ( dCurVal > pldCur->adVal[1] )
				{
					pldCur->adVal[1] = dCurVal;
					memcpy(&pldCur->gIndex, &pBasic->m_index, sz_gIndex);
					pldCur->dOpen = (double)pBasic->m_iOpen;
					pldCur->dHigh = (double)pBasic->m_iHigh;
					pldCur->dLow = (double)pBasic->m_iLow;
					pldCur->dClose = (double)pBasic->m_iClose;									
				}
			}
			else
			{
				if ( dCurVal < pldCur->adVal[1] )
				{
					pldCur->adVal[1] = dCurVal;
					memcpy(&pldCur->gIndex, &pBasic->m_index, sz_gIndex);
					pldCur->dOpen = (double)pBasic->m_iOpen;
					pldCur->dHigh = (double)pBasic->m_iHigh;
					pldCur->dLow = (double)pBasic->m_iLow;
					pldCur->dClose = (double)pBasic->m_iClose;									
				}
			}
		}

		bPrev = bUp;
		dPreVal = dCurVal;
	}	

	return true;
}
*/
bool CVKagi::CreateData()
{
//TRACE("CreateData()\n");
	COrgData* pOrgData = NULL;
	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return false;

	if (!(m_pgraph->dwDraOption & GO_UNION))
	{
		m_iTtlDtCnt = pOrgData->GetArraySize();
	}

	int iSize = m_iTtlDtCnt;
	int ii = 0;
	CGrpBasic* pBasic = NULL;

	m_iBlankGap = 0;
	for ( ii = 0 ; ii < iSize ; ii++ )
	{
		pBasic = pOrgData->GetGraphData(ii);

		if (pBasic->m_iClose != GD_UNUSED)
			break;
		m_iBlankGap++;
	}

	RemoveData();

	if (ii > iSize - 2)
		return false;

	int iPoint = ii;
	double dPreVal = 0;
	double dCurVal = 0;
	double dVal = 0;
	double dDiff = 0;
	bool bUp = false;
	bool bPrev = false;
	bool bTurn = true;
	bool bStart = false;
//	bool bPaning = false;
//	bool bLog = IsLogChart();
	
	struct _local_data* pldCur = NULL;
	
	bPrev = bUp;

	for ( ii = iPoint ; ii < iSize ; ii++ )
	{
		pBasic = pOrgData->GetGraphData(ii);
		if (pBasic->m_iClose == GD_UNUSED)
			break;

		double dOpen = pBasic->m_iOpen;
		double dHigh = pBasic->m_iHigh;
		double dLow = pBasic->m_iLow;
		double dClose = pBasic->m_iClose;

		dCurVal = dClose;

		if (ii == iPoint)
		{
			dPreVal = dCurVal;
			continue;
		}

		if (dCurVal > dPreVal)
			bUp = true;
		else if (dCurVal < dPreVal)
			bUp = false;
		else
		{
			if (pldCur)
				pldCur->iGap++;
			continue;
		}

		if (bStart && bPrev != bUp)	// 종가가 오르거나 내린경우
		{
			dDiff = fabs(dPreVal - dCurVal);

			if (dDiff >= m_iValue)
				bTurn = true;
			else
			{
				bUp = bPrev;
				if (pldCur)
					pldCur->iGap++;
				continue;
			}
		}
		
		bStart = true;
		
		if (bTurn)	// 꺾임
		{
			if (pldCur)
				dPreVal = pldCur->adVal[1];
			
			pldCur = new _local_data;
			pldCur->adVal[0] = dPreVal;
			pldCur->adVal[1] = dCurVal;

			memcpy(&pldCur->gIndex, &pBasic->m_index, sz_gIndex);
			pldCur->dOpen = dOpen;
			pldCur->dHigh = dHigh;
			pldCur->dLow = dLow;
			pldCur->dClose = dClose;
			pldCur->iGap = 1;

			pldCur->dSDay = pldCur->gIndex.date.yy * 10000 + pldCur->gIndex.date.mm * 100 + pldCur->gIndex.date.dd;
			pldCur->dSDay = 1000000 * pldCur->dSDay + pldCur->gIndex.time.hh * 10000 + pldCur->gIndex.time.mm * 100 + pldCur->gIndex.time.ss;

			pldCur->dEDay = pldCur->dSDay;

			bTurn = false;	
			m_arData.Add(pldCur);
		}
		else
		{
			if (bUp)
			{
				if ( dCurVal > pldCur->adVal[1] )
				{
					pldCur->adVal[1] = dCurVal;
					memcpy(&pldCur->gIndex, &pBasic->m_index, sz_gIndex);
					pldCur->dOpen = dOpen;
					pldCur->dHigh = dHigh;
					pldCur->dLow = dLow;
					pldCur->dClose = dClose;

					pldCur->dEDay = pldCur->gIndex.date.yy * 10000 + pldCur->gIndex.date.mm * 100 + pldCur->gIndex.date.dd;
					pldCur->dEDay = 1000000 * pldCur->dEDay + pldCur->gIndex.time.hh * 10000 + pldCur->gIndex.time.mm * 100 + pldCur->gIndex.time.ss;

				}
			}
			else
			{
				if ( dCurVal < pldCur->adVal[1] )
				{
					pldCur->adVal[1] = dCurVal;
					memcpy(&pldCur->gIndex, &pBasic->m_index, sz_gIndex);
					pldCur->dOpen = dOpen;
					pldCur->dHigh = dHigh;
					pldCur->dLow = dLow;
					pldCur->dClose = dClose;

					pldCur->dEDay = pldCur->gIndex.date.yy * 10000 + pldCur->gIndex.date.mm * 100 + pldCur->gIndex.date.dd;
					pldCur->dEDay = 1000000 * pldCur->dEDay + pldCur->gIndex.time.hh * 10000 + pldCur->gIndex.time.mm * 100 + pldCur->gIndex.time.ss;
				}
			}

			if (pldCur)
				pldCur->iGap++;
		}

		bPrev = bUp;
		dPreVal = dCurVal;
	}	

	return true;
}

bool CVKagi::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
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

CString CVKagi::GetDisplayPosHeader(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	int	nCount = m_arData.GetSize();
	if (nCount == 0 || m_iShow == 0)
		return _T("");

	struct _local_data* pObject = NULL;
	int	nIndex = 0;

	int	nGab = (pt.x - m_rectGrp.left);

	if (nGab < 0)	return _T("");

	nIndex = (int)((double)nGab / m_dUnit);
	if (nCount > m_iShow)
		nIndex += nCount - m_iShow;
	if (nIndex < 0 || nIndex >= nCount)
		return _T("");

	CString	unitStr1 = _T("");
	pObject = m_arData.GetAt(nIndex);

	if (bGrid)
	{
		switch (GetDataIndex())
		{
		case GI_MINUTE:
		case GI_TICK:
			if (pObject->gIndex.date.dd == 0)
				unitStr1.Format("~~시간:%02d:%02d:%02d\t", pObject->gIndex.time.hh,
								pObject->gIndex.time.mm, pObject->gIndex.time.ss);
			else
				unitStr1.Format("~~시간:%02d/%02d-%02d:%02d:%02d\t", 
					pObject->gIndex.date.mm, pObject->gIndex.date.dd,
					pObject->gIndex.time.hh, pObject->gIndex.time.mm, pObject->gIndex.time.ss);
			break;
		default:
			unitStr1.Format("~~일자:%04d/%02d/%02d\t", pObject->gIndex.date.yy,
				pObject->gIndex.date.mm, pObject->gIndex.date.dd);
			break;
		}
	}
	else
	{
		switch (GetDataIndex())
		{
		case GI_MINUTE:
		case GI_TICK:
			unitStr1.Format("시간\t%02d:%02d:%02d\t", pObject->gIndex.time.hh,
				pObject->gIndex.time.mm, pObject->gIndex.time.ss);
			break;
		default:
			unitStr1.Format("일자\t%04d/%02d/%02d\t", pObject->gIndex.date.yy,
				pObject->gIndex.date.mm, pObject->gIndex.date.dd);
			break;
		}
	}
	
	return unitStr1;
}

CString CVKagi::GetDisplayPosData(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	int	nCount = m_arData.GetSize();
	if (nCount == 0 || m_iShow == 0)
		return _T("");

	struct _local_data* pObject = NULL;
	int	nIndex = 0;

	int	nGab = (pt.x - m_rectGrp.left);

	if (nGab < 0)
		return _T("");

	nIndex = (int)((double)nGab / m_dUnit);
	if (nCount > m_iShow)
		nIndex += nCount - m_iShow;
	if (nIndex < 0 || nIndex >= nCount)
		return _T("");

	CString	unitStr1 = _T("");
	pObject = m_arData.GetAt(nIndex);

	CString	sFpr, sHpr, sLpr, sEpr, sPivot, sVal;
	if (pObject->dClose == GD_UNUSED)
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
			if (pldPrev->dClose != GD_UNUSED)
			{
				double dPcnt = (double)(pObject->dOpen - pldPrev->dClose) / (double)pldPrev->dClose * 100;
				dPcnt < 0 ? strOpenP.Format("  (%0.2f%%)", dPcnt) : strOpenP.Format("  (+%0.2f%%)", dPcnt);

				dPcnt = (double)(pObject->dHigh - pldPrev->dClose) / (double)pldPrev->dClose * 100;
				dPcnt < 0 ? strHighP.Format("  (%0.2f%%)", dPcnt) : strHighP.Format("  (+%0.2f%%)", dPcnt);

				dPcnt = (double)(pObject->dLow - pldPrev->dClose) / (double)pldPrev->dClose * 100;
				dPcnt < 0 ? strLowP.Format("  (%0.2f%%)", dPcnt) : strLowP.Format("  (+%0.2f%%)", dPcnt);

				dPcnt = (double)(pObject->dClose - pldPrev->dClose) / (double)pldPrev->dClose * 100;
				dPcnt < 0 ? strCloseP.Format("  (%0.2f%%)", dPcnt) : strCloseP.Format("  (+%0.2f%%)", dPcnt);
			}
		}

		FormatVariableComma(sFpr, pObject->dOpen, pObject->dOpen);
		FormatVariableComma(sHpr, pObject->dHigh, pObject->dHigh);
		FormatVariableComma(sLpr, pObject->dLow, pObject->dLow);
		FormatVariableComma(sEpr, pObject->dClose, pObject->dClose);
		FormatVariableComma(sPivot, pObject->adVal[0], pObject->adVal[0]);
		FormatVariableComma(sVal, pObject->adVal[1], pObject->adVal[1]);
		if (bGrid)
			unitStr1.Format("시가:%s\t고가:%s\t저가:%s\t$$종가:%s\t기준값:%s\t전환값:%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, sPivot, sVal);
		else
			unitStr1.Format("시가\t%s\t고가\t%s\t저가\t%s\t종가\t%s\t기준값\t%s\t전환값\t%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, sPivot, sVal);
	}

	return unitStr1;
}

CString CVKagi::GetXposValue(CPoint pt)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	int	nCount = m_arData.GetSize();
	if (nCount == 0 || m_iShow == 0)
		return _T("");

	struct _local_data* pObject = NULL;
	int	nIndex = 0;

	int	nGab = (pt.x - m_rectGrp.left);

	if (nGab < 0)
		return _T("");

	nIndex = (int)((double)nGab / m_dUnit);
	if (nCount > m_iShow)
		nIndex += nCount - m_iShow;

	if (nIndex < 0 || nIndex >= nCount)
		return _T("");

	CString	unitStr1 = _T("");
	pObject = m_arData.GetAt(nIndex);

	switch (GetDataIndex())
	{
	case GI_MINUTE:
	case GI_TICK:
		unitStr1.Format("%02d/%02d %02d:%02d:%02d", 
			pObject->gIndex.date.mm, pObject->gIndex.date.dd, 
			pObject->gIndex.time.hh, pObject->gIndex.time.mm, pObject->gIndex.time.ss);
		break;
	default:
		unitStr1.Format("%04d/%02d/%02d", 
			pObject->gIndex.date.yy, pObject->gIndex.date.mm, pObject->gIndex.date.dd);
		break;
	}
	
	return unitStr1;
}

#include <math.h>
void CVKagi::GetValueToPoint(CPoint& pt, double dXValue, double dYValue)
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
			rectDraw.right -= int(double(m_rectGrp.Width()*iBalanceGap) / double(m_iDispDtCnt + iBalanceGap));
	}
	
	double dGap = m_iDispEPos - m_iDispSPos;

	if (dGap == 0)
		dGap = 1;
	

	struct	_local_data* pObject = NULL;
	double dX = 0;
	double	dMValue = dXValue;

//	bool bFind = false;
	m_dUnit = (double)m_rectGrp.Width() / m_iShow;
	for (int ii = 0; ii < m_arData.GetSize(); ii++)
	{
		pObject = m_arData.GetAt(ii);

		if(ii < m_iTtlDtCnt && pObject->dSDay <= dMValue && dMValue <= pObject->dEDay)
		{
			dX = ii * m_dUnit + (m_dUnit / 2);
//			bFind = true;
			break;
		}
	}

//	if (!bFind)
//		dX = pt.x - m_dUnit - rectDraw.left;

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

void CVKagi::GetPtToPosValue(CPoint pt, double& dPos, double& dValue)
{
	double dMax = m_dYMax;
	double dMin = m_dYMin;

	// 2006.12.27 schbang 데이터 없을시 죽는걸 막기위해
	if (m_arData.GetSize() < 1)
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
			rectDraw.right -= int(double(m_rectGrp.Width()*iBalanceGap) / double(m_iDispDtCnt + iBalanceGap));
	}
	
	int ixPos;
	ixPos = (int)(double(pt.x - rectDraw.left) / m_dUnit); //* iDispCount) / double(rectDraw.Width());
	ixPos += GetDisplayPos();
	
	struct	_local_data*	pObject = NULL;
	//if(ixPos >= m_CurveN)
	if(ixPos >= m_arData.GetSize())
		//ixPos = m_CurveN -1;
		ixPos = m_arData.GetSize() - 1;
	if(ixPos < 0)
		ixPos = 0;

	if(ixPos < m_iTtlDtCnt )
	{
		//pObject = &m_pLocal[ixPos];
		pObject = m_arData.GetAt(ixPos);
		dPos = pObject->dSDay;
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

void CVKagi::GetPtToEndvalPt(CPoint pt, CPoint &ept)
{
	double dPos, dValue;
	GetPtToPosValue(pt, dPos, dValue);
	int iXIdx = GetValueToXIdx(dPos);
	ept = pt;
	struct	_local_data*	pObject = NULL;

	if(iXIdx < m_iTtlDtCnt)
	{
		//pObject = &m_pLocal[iXIdx];
		pObject = m_arData.GetAt(iXIdx);
		ept.y = GetYPos(pObject->dHigh);
	}
	
}

CString CVKagi::GetIndexStrTool(int index, int &fpr, int &hpr, int &lpr, int &epr)
{
	if (index < 0)
		return _T("");
	

	//if (index >= m_CurveN)
	if (index >= m_arData.GetSize())
		//index = m_CurveN-1;
		index = m_arData.GetSize() - 1 ;
	
	struct	_local_data*	pObject = NULL;

	if(index > m_iTtlDtCnt)
		return _T("");

	//pObject = &m_pLocal[index];
	pObject = m_arData.GetAt(index);

/*
	fpr = (int)pObject->lpr;
	hpr = (int)pObject->hpr;
	lpr = (int)pObject->lpr;
	epr = (int)pObject->hpr;
*/
	fpr = (int)pObject->dOpen;
	hpr = (int)pObject->dHigh;
	lpr = (int)pObject->dLow;
	epr = (int)pObject->dClose;
	
	return _T("");
}

double CVKagi::GetOneDataSize()
{
	
	//return ((double)m_rectGrp.Width() / m_CurveN);
	return ((double)m_rectGrp.Width() / m_arData.GetSize());
	
}

int CVKagi::GetValueToXIdx(double dXValue)
{
	struct	_local_data*	pObject = NULL;
//	int m_iShow = m_CurveN;
	double dX = 0;
	double	dMValue = dXValue;
	
	m_dUnit = (double)m_rectGrp.Width() / m_iShow;
	//for ( int ii = m_iViweSidx; ii <= m_iViweEidx ; ii++ )
	for (int ii = 0; ii < m_arData.GetSize(); ii++)
	{
		//pObject = &m_pLocal[ii];
		pObject = m_arData.GetAt(ii);
		if(ii < m_iTtlDtCnt && pObject->dSDay <= dMValue && dMValue < pObject->dEDay)
		{
			return (ii);
		}
	}
	return 0;
}
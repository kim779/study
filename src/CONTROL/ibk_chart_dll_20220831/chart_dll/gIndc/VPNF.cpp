#include "stdafx.h"
#include "VPNF.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CVPNF::CVPNF(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
: CVarsBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_bCheck = true;
	m_bCalculated = false;

	m_piPnFData = NULL;
	m_piXpos = NULL;
	m_pLocal = NULL;
	m_iJWPnt = 3;
	m_iUserJWPnt = 0;
	m_iOnePrice = 0;

	m_union = false;
	m_BlkDataCnt= 0;
}

CVPNF::~CVPNF()
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

void CVPNF::DrawGraph(CDC* pDC)
{
	m_RgnInfo.iCount = 0;
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return;
	}

	if (pOrgData->GetPattern() != dpBASIC) {
		return;
	}

	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX) {
		return;
	}
	
//	CVarsBase::DrawTick(pDC);	
	
	if (m_pgraph->dwDraOption & GO_UNION)
	{
		m_union = true;
		//CalXpos();
		//CalculateMinMax(false);

		bool bReverse = IsReverseChart();

		if (bReverse)
			DrawReversChart(pDC);
		else
			DrawUnionChart(pDC);

		DrawBottomStick(pDC);
	}
	else
	{
		m_union = false;
		//CalculateMinMax(false);
		DrawChart(pDC);
	}
}

void CVPNF::DrawReversChart(CDC* pDC)
{
	if (!m_piPnFData)
		return;

	CPen	*UpPen = GetPen(PS_SOLID, 1,m_UpperColor);
	CPen	*DnPen = GetPen(PS_SOLID, 1,m_LowerColor);

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

	CalXpos();
	
	bool bInc;

	if (m_piPnFData[m_iStartInx * 2] > m_piPnFData[m_iStartInx * 2 + 1])
		bInc = false;
	else if (m_piPnFData[m_iStartInx * 2] < m_piPnFData[m_iStartInx * 2 + 1])
		bInc = true;

	CPoint	tpoint, dispPt;
	CRect	DrawR;
	CString	dispText;
	CArray <PFTICK,PFTICK>	Stick;

	double	Start, End;
	int	kk = m_iStartInx;
	int	Year = 0;
	int	vHeight = m_piPnFData[kk * 2];

	double	sPoint, ePoint, pnt, realStartPoint, dPixel;
	realStartPoint = 0.0;

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

	bool	bLog = IsLogChart();
	
	for (int ii = 0; kk < m_iCurveN; ii++, kk++ )	
	{
		Start = m_piPnFData[kk * 2];
		End   = m_piPnFData[kk * 2 + 1];
		
		dPixel = double(m_iDrawHeight) / double(m_dYMax - m_dYMin);		
		sPoint = double(m_rectGrp.bottom) - (m_dYMax - End) * dPixel;
		ePoint = double(m_rectGrp.bottom) - (m_dYMax - Start) * dPixel;
		
		if (bLog)
		{			
			sPoint = GetYPos(End);
			ePoint = GetYPos(Start);
		}

		if (realStartPoint != 0.0)
			sPoint = realStartPoint;

		// save date
		dispPt.x = int(m_piXpos[ii]);

		PFTICK	tick;
		if (Year != m_pLocal[ii].date.date.yy)
		{
			dispText.Format("%d.%d.%d", m_pLocal[kk].date.date.yy,
				m_pLocal[kk].date.date.mm, m_pLocal[kk].date.date.dd);
		}
		else
		{
			dispText.Format("%d.%d", m_pLocal[kk].date.date.mm, 
				m_pLocal[kk].date.date.dd);
		}

		// draw PnF
		if (bInc)	// 상승
		{
			pDC->SelectObject(UpPen);
			tpoint.x = int(m_piXpos[ii]);// + 1;
			tpoint.y = int(sPoint);
			pDC->MoveTo(tpoint);

			m_arColorX.Add(m_pgraph->cInfo.aline[0].rgbLColor);
			m_arPointX.Add(int(m_piXpos[ii]));

			tmpRECT.left = tpoint.x;
			tmpRECT.bottom = tpoint.y;

			pRgnPoint[nPoint*2 - ii - 1] = tpoint;

			bool bDir = true;
			pnt = sPoint + m_dOnePoint;

			for (; int(pnt) > int(ePoint);)
			{
				if (bDir)
				{
					tpoint.x = int(m_piXpos[ii + 1]);// - 1;
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				else
				{
					tpoint.x = int(m_piXpos[ii]);// + 1;
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				bDir = !bDir;
				pnt -= m_dOnePoint;
			}

			tmpRECT.top = tpoint.y;

			tpoint.x = int(m_piXpos[ii + 1]);// - 1;
			tpoint.y = int(sPoint);
			pDC->MoveTo(tpoint);

			tmpRECT.right = tpoint.x;			

			bDir = true;
			pnt = sPoint + m_dOnePoint;
			for (; int(pnt) > int(ePoint);)
			{
				if (bDir)
				{
					tpoint.x = int(m_piXpos[ii]);// + 1;
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				else
				{
					tpoint.x = int(m_piXpos[ii + 1]);// - 1;
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				bDir = !bDir;
				pnt -= m_dOnePoint;
			}

			dispPt.y = int(realStartPoint + m_dOnePoint + m_TkInfo.tkHeight[TICK_BOTTOM] / 5);
			tick.PenColor = Gcolor;
			tick.Context = dispText;
			tick.xyPoint = dispPt;
			Stick.Add(tick);
			Year = m_pLocal[kk].date.date.yy;

			pRgnPoint[ii] = dispPt;

			tmpRGN.DeleteObject();
			tmpRGN.CreateRectRgnIndirect(&tmpRECT);
			m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CombineRgn(m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn, &tmpRGN, RGN_OR);
		}
		else		// 하락
		{
			pDC->SelectObject(DnPen);

			pnt = sPoint + m_dOnePoint;
			pRgnPoint[nPoint*2 - ii - 1] = 
				CPoint(int(m_piXpos[ii])/* + 1*/, int(pnt - m_dOnePoint)/* + 1*/);

			m_arColorX.Add(m_pgraph->cInfo.aline[1].rgbLColor);
			m_arPointX.Add(int(m_piXpos[ii]));

			tmpRECT.bottom = -1;

			for (; int(pnt) <= int(ePoint);)
			{
				DrawR = CRect(int(m_piXpos[ii])/* + 1*/, int(pnt - m_dOnePoint) + 1,
					int(m_piXpos[ii + 1])/* - 1*/, int(pnt)/* - 1*/);

				pDC->Ellipse(&DrawR);

				if (tmpRECT.bottom < 0)
					tmpRECT.bottom = DrawR.bottom;
				pnt += m_dOnePoint;
			}

			tmpRECT.left = DrawR.left;
			tmpRECT.right = DrawR.right;
			tmpRECT.top = DrawR.top;

			tmpRGN.DeleteObject();
			tmpRGN.CreateEllipticRgnIndirect(&tmpRECT);
			m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CombineRgn(m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn, &tmpRGN, RGN_OR);

			dispPt.y = int(realStartPoint - m_dOnePoint - m_TkInfo.tkHeight[TICK_BOTTOM] * 5 / 4);
			tick.PenColor = Ycolor;
			tick.Context = dispText;
			tick.xyPoint = dispPt;
			Stick.Add(tick);
			Year = m_pLocal[kk].date.date.yy;

			pRgnPoint[ii] = dispPt;
		}
		bInc = !bInc;
	}

	if (bInc)
	{
		m_arColorX.Add(m_pgraph->cInfo.aline[1].rgbLColor);
		m_arPointX.Add(int(m_piXpos[m_iCurveN]));
	}
	else
	{
		m_arColorX.Add(m_pgraph->cInfo.aline[0].rgbLColor);
		m_arPointX.Add(int(m_piXpos[m_iCurveN]));
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

void CVPNF::DrawUnionChart(CDC* pDC)
{
	if (!m_piPnFData)
		return;

	CPen	*UpPen = GetPen(PS_SOLID, 1,m_UpperColor);
	CPen	*DnPen = GetPen(PS_SOLID, 1,m_LowerColor);

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

	CalXpos();

	bool bInc;

	if (m_piPnFData[m_iStartInx * 2] > m_piPnFData[m_iStartInx * 2 + 1])
		bInc = false;
	else if (m_piPnFData[m_iStartInx * 2] < m_piPnFData[m_iStartInx * 2 + 1])
		bInc = true;

	CPoint	tpoint, dispPt;
	CRect	DrawR;
	CString	dispText;
	CArray <PFTICK,PFTICK>	Stick;

	int	Start, End;
	int	kk = m_iStartInx;
	int	Year = 0;
	int	vHeight = m_piPnFData[kk * 2];

	double	sPoint, ePoint, pnt, realStartPoint, dPixel;
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

	bool	bLog = IsLogChart();

	for (int ii = 0; kk < m_iCurveN; ii++, kk++ )	
	{
		Start = m_piPnFData[kk * 2];
		End   = m_piPnFData[kk * 2 + 1];

		dPixel = double(m_iDrawHeight) / double(m_dYMax - m_dYMin);
		sPoint = double(m_rectGrp.top) + (m_dYMax - Start) * dPixel;
		ePoint = double(m_rectGrp.top) + (m_dYMax - End) * dPixel;

		if (bLog)
		{	
			sPoint = GetYPos(Start);
			ePoint = GetYPos(End);
		}

		if (realStartPoint != 0.0)
			sPoint = realStartPoint;

		// save date
		dispPt.x = int(m_piXpos[ii]);

		PFTICK	tick;
		if (Year != m_pLocal[ii].date.date.yy)
		{
			dispText.Format("%d.%d.%d", m_pLocal[kk].date.date.yy,
				m_pLocal[kk].date.date.mm, m_pLocal[kk].date.date.dd);
		}
		else
		{
			dispText.Format("%d.%d", m_pLocal[kk].date.date.mm, 
				m_pLocal[kk].date.date.dd);
		}

		// draw PnF
		if (bInc)	// 상승
		{
			pDC->SelectObject(UpPen);
			tpoint.x = int(m_piXpos[ii]);// + 1;
			tpoint.y = int(sPoint);
			
			pDC->MoveTo(tpoint);

			m_arColorX.Add(m_pgraph->cInfo.aline[0].rgbLColor);
			m_arPointX.Add(int(m_piXpos[ii]));

			tmpRECT.left = tpoint.x;
			tmpRECT.bottom = tpoint.y;

			pRgnPoint[nPoint*2 - ii - 1] = tpoint;

			bool bDir = true;
			pnt = sPoint - m_dOnePoint;

			for (; int(pnt) >= int(ePoint);)
			{
				if (bDir)
				{
					tpoint.x = int(m_piXpos[ii + 1]);// - 1;
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				else
				{
					tpoint.x = int(m_piXpos[ii]);// + 1;
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				bDir = !bDir;
				pnt -= m_dOnePoint;
			}

			tmpRECT.top = tpoint.y;

			tpoint.x = int(m_piXpos[ii + 1]);// - 1;
			tpoint.y = int(sPoint);
			pDC->MoveTo(tpoint);

			tmpRECT.right = tpoint.x;			

			bDir = true;
			pnt = sPoint - m_dOnePoint;
			for (; int(pnt) >= int(ePoint);)
			{
				if (bDir)
				{
					tpoint.x = int(m_piXpos[ii]);// + 1;
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				else
				{
					tpoint.x = int(m_piXpos[ii + 1]);// - 1;
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				bDir = !bDir;
				pnt -= m_dOnePoint;
			}

			realStartPoint = pnt + m_dOnePoint*2;

			dispPt.y = int(realStartPoint + m_dOnePoint + m_TkInfo.tkHeight[TICK_BOTTOM] / 5);
			tick.PenColor = Gcolor;
			tick.Context = dispText;
			tick.xyPoint = dispPt;
			Stick.Add(tick);
			Year = m_pLocal[kk].date.date.yy;

			pRgnPoint[ii] = dispPt;

			tmpRGN.DeleteObject();
			tmpRGN.CreateRectRgnIndirect(&tmpRECT);
			m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CombineRgn(m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn, &tmpRGN, RGN_OR);
		}
		else		// 하락
		{
			pDC->SelectObject(DnPen);

			pnt = sPoint + m_dOnePoint;
			pRgnPoint[nPoint*2 - ii - 1] = 
				CPoint(int(m_piXpos[ii])/* + 1*/, int(pnt - m_dOnePoint)/* + 1*/);

			m_arColorX.Add(m_pgraph->cInfo.aline[1].rgbLColor);
			m_arPointX.Add(int(m_piXpos[ii]));

			tmpRECT.bottom = -1;

			for (; int(pnt) <= int(ePoint);)
			{
				DrawR = CRect(int(m_piXpos[ii])/* + 1*/, int(pnt - m_dOnePoint) + 1,
					int(m_piXpos[ii + 1])/* - 1*/, int(pnt)/* - 1*/);

				pDC->SelectObject((HBRUSH)GetStockObject(NULL_BRUSH));
				pDC->Ellipse(&DrawR);

				if (tmpRECT.bottom < 0)
					tmpRECT.bottom = DrawR.bottom;
				pnt += m_dOnePoint;
			}

			tmpRECT.left = DrawR.left;
			tmpRECT.right = DrawR.right;
			tmpRECT.top = DrawR.top;

			tmpRGN.DeleteObject();
			tmpRGN.CreateEllipticRgnIndirect(&tmpRECT);
			m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CombineRgn(m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn, &tmpRGN, RGN_OR);

			realStartPoint = pnt - m_dOnePoint*2;
			dispPt.y = int(realStartPoint - m_dOnePoint - m_TkInfo.tkHeight[TICK_BOTTOM] * 5 / 4);
			tick.PenColor = Ycolor;
			tick.Context = dispText;
			tick.xyPoint = dispPt;
			Stick.Add(tick);
			Year = m_pLocal[kk].date.date.yy;

			pRgnPoint[ii] = dispPt;
		}
		bInc = !bInc;
	}

	if (bInc)
	{
		m_arColorX.Add(m_pgraph->cInfo.aline[1].rgbLColor);
		m_arPointX.Add(int(m_piXpos[m_iCurveN]));
	}
	else
	{
		m_arColorX.Add(m_pgraph->cInfo.aline[0].rgbLColor);
		m_arPointX.Add(int(m_piXpos[m_iCurveN]));
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
	rgnDraw.DeleteObject();

	m_RgnInfo.iCount++;
}

void CVPNF::DrawChart(CDC* pDC)
{
	if (!m_piPnFData)
		return;

	CPen	*UpPen = GetPen(PS_SOLID, 1,m_UpperColor);
	CPen	*DnPen = GetPen(PS_SOLID, 1,m_LowerColor);
	CBrush	*DnBrush = GetBrush(m_LowerColor);

	CPen	*sPen = pDC->SelectObject(DnPen);
	CBrush	*sBrush = pDC->SelectObject(DnBrush);
	COLORREF Ycolor = RGB(196, 196, 0);
	COLORREF Gcolor = RGB(192,  0, 192);

	CRgn rgnDraw;
	rgnDraw.CreateRectRgnIndirect(CRect(m_rectGrp.left, m_rectGrp.top - m_rectGrp.top, m_rectGrp.right, m_rectGrp.bottom));
	pDC->SelectClipRgn(&rgnDraw);
	
	if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_PRC)
	{
		m_dYMax = m_pCoDraw->PMax;
		m_dYMin = m_pCoDraw->PMin;
	}
	
	CalXpos();

	bool bInc;

	if (m_piPnFData[m_iStartInx * 2] > m_piPnFData[m_iStartInx * 2 + 1])
		bInc = false;
	else if (m_piPnFData[m_iStartInx * 2] < m_piPnFData[m_iStartInx * 2 + 1])
		bInc = true;
	/*else 
	{
		m_iStartInx++;

		if (m_piPnFData[m_iStartInx * 2] > m_piPnFData[m_iStartInx * 2 + 1])
			bInc = false;
		else if (m_piPnFData[m_iStartInx * 2] < m_piPnFData[m_iStartInx * 2 + 1])
			bInc = true;
	}*/

	CPoint	tpoint, dispPt;
	CRect	DrawR;
	CString	dispText;
	CArray <PFTICK,PFTICK>	Stick;

	int	Start, End;
	int	kk = m_iStartInx;
	int	Year = 0;
	int	vHeight = m_piPnFData[kk * 2];

	double	sPoint, ePoint, pnt, realStartPoint, dPixel;
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

	bool	bLog = IsLogChart();

	for (int ii = 0; kk < m_iCurveN; ii++, kk++ )	
	{
		Start = m_piPnFData[kk * 2];
		End   = m_piPnFData[kk * 2 + 1];

		dPixel = double(m_iDrawHeight) / double(m_dYMax - m_dYMin);
		sPoint = double(m_rectGrp.top) + (m_dYMax - Start) * dPixel;
		ePoint = double(m_rectGrp.top) + (m_dYMax - End) * dPixel;

		if (bLog)
		{	
			sPoint = GetYPos(Start);
			ePoint = GetYPos(End);
		}

		if (realStartPoint != 0.0)
			sPoint = realStartPoint;

		// save date
		dispPt.x = int(m_piXpos[ii]);

		PFTICK	tick;
		if (Year != m_pLocal[ii].date.date.yy)
		{
			dispText.Format("%d.%d.%d", m_pLocal[kk].date.date.yy,
				m_pLocal[kk].date.date.mm, m_pLocal[kk].date.date.dd);
		}
		else
		{
			dispText.Format("%d.%d", m_pLocal[kk].date.date.mm, 
				m_pLocal[kk].date.date.dd);
		}

		// draw PnF
		if (bInc)	// 상승
		{
			pDC->SelectObject(UpPen);
			tpoint.x = int(m_piXpos[ii]);// + 1;
			tpoint.y = int(sPoint);
			pDC->MoveTo(tpoint);

			tmpRECT.left = tpoint.x;
			tmpRECT.bottom = tpoint.y;

			pRgnPoint[nPoint*2 - ii - 1] = tpoint;

			bool bDir = true;
			pnt = sPoint - m_dOnePoint;

			for (; int(pnt) >= int(ePoint);)
			{
				if (bDir)
				{
					tpoint.x = int(m_piXpos[ii + 1]);// - 1;
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				else
				{
					tpoint.x = int(m_piXpos[ii]);// + 1;
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				bDir = !bDir;
				pnt -= m_dOnePoint;
			}

			tmpRECT.top = tpoint.y;

			tpoint.x = int(m_piXpos[ii + 1]);// - 1;
			tpoint.y = int(sPoint);
			pDC->MoveTo(tpoint);

			tmpRECT.right = tpoint.x;			

			bDir = true;
			pnt = sPoint - m_dOnePoint;
			for (; int(pnt) >= int(ePoint);)
			{
				if (bDir)
				{
					tpoint.x = int(m_piXpos[ii]);// + 1;
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				else
				{
					tpoint.x = int(m_piXpos[ii + 1]);// - 1;
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				bDir = !bDir;
				pnt -= m_dOnePoint;
			}

			realStartPoint = pnt + m_dOnePoint*2;

			dispPt.y = int(realStartPoint + m_dOnePoint + m_TkInfo.tkHeight[TICK_BOTTOM] / 5);
			//dispPt.y = int(realStartPoint - m_dOnePoint - m_TkInfo.tkHeight[TICK_BOTTOM] * 5 / 4);
			tick.PenColor = Gcolor;
			tick.Context = dispText;
			tick.xyPoint = dispPt;
			Stick.Add(tick);
			Year = m_pLocal[kk].date.date.yy;

			pRgnPoint[ii] = dispPt;

			tmpRGN.DeleteObject();
			tmpRGN.CreateRectRgnIndirect(&tmpRECT);
			m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CombineRgn(m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn, &tmpRGN, RGN_OR);
		}
		else		// 하락
		{
			pDC->SelectObject(DnPen);

			pnt = sPoint + m_dOnePoint;
			pRgnPoint[nPoint*2 - ii - 1] = 
				CPoint(int(m_piXpos[ii])/* + 1*/, int(pnt - m_dOnePoint)/* + 1*/);

			tmpRECT.bottom = -1;

			for (; int(pnt) <= int(ePoint);)
			{
				DrawR = CRect(int(m_piXpos[ii])/* + 1*/, int(pnt - m_dOnePoint) + 1,
					int(m_piXpos[ii + 1])/* - 1*/, int(pnt)/* - 1*/);
				pDC->Ellipse(&DrawR);

				if (tmpRECT.bottom < 0)
					tmpRECT.bottom = DrawR.bottom;
				pnt += m_dOnePoint;
			}

			tmpRECT.left = DrawR.left;
			tmpRECT.right = DrawR.right;
			tmpRECT.top = DrawR.top;

			tmpRGN.DeleteObject();
			tmpRGN.CreateEllipticRgnIndirect(&tmpRECT);
			m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CombineRgn(m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn, &tmpRGN, RGN_OR);

			realStartPoint = pnt - m_dOnePoint*2;
			dispPt.y = int(realStartPoint - m_dOnePoint - m_TkInfo.tkHeight[TICK_BOTTOM] * 5 / 4);

			tick.PenColor = Ycolor;
			tick.Context = dispText;
			tick.xyPoint = dispPt;
			Stick.Add(tick);
			Year = m_pLocal[kk].date.date.yy;

			pRgnPoint[ii] = dispPt;
		}
		bInc = !bInc;
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

	if (m_rectGrp.Height() > int(m_TkInfo.tkHeight[TICK_BOTTOM]))
	{
		CString	str;
		FormatVariableComma(str, int(m_iOnePrice), m_dYMax);
		dispText.Format ("P&&F차트 전환포인트 : %d, 한칸의 값 : %s", m_iJWPnt, str);
		DrawTickIndex(pDC, dispText, m_pgraph->rgbLabel, true);
	}

	delete[] pRgnPoint;
	
	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);

	pDC->SelectClipRgn(NULL);
	m_RgnInfo.iCount++;
}

int CVPNF::GetOnePrice()
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

void CVPNF::SetMember()
{
	m_UpperColor = m_pgraph->cInfo.aline[0].rgbLColor;
	m_LowerColor = m_pgraph->cInfo.aline[1].rgbLColor;
	
	m_iDrawHeight	= m_rectGrp.Height();
	m_iDrawWidth	= m_rectGrp.Width();

	m_iJWPnt = m_pgraph->cInfo.awValue[0];
}

bool CVPNF::CalculateMinMax(bool bForce)
{
	SetMember();

	if (!m_bCalculated || bForce)
	{
		if (!CreateData())
			return false;
		m_bCalculated = true;
	}

	int	iPointCnt = m_iDispEPos - m_iDispSPos;
	int	wWidth = m_rectGrp.left;
	int	wRight = m_rectGrp.right;
	
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return false;
	
	if (m_dwDspOpt & DO_SCREEN && m_pgraph->dwDraOption & GO_UNION)
		pOrgData->CalculateMinMax(m_iDispSPos, m_iDispEPos, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cPRC, bForce);
	else
		pOrgData->CalculateMinMax(0, m_iTtlDtCnt, &m_dYMin, &m_dYMax, m_pgraph->wCalOption, cPRC, bForce);

	ReviseMinMax(m_dYMax, m_dYMin);

	return true;
}

bool CVPNF::CreateData()
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

	if (!(m_pgraph->dwDraOption & GO_UNION))
		m_iTtlDtCnt = pOrgData->GetArraySize();

	CGrpBasic* gBasic;

	pPnF = new int[m_iTtlDtCnt];	

	int	Start = -1;

	m_BlkDataCnt = 0;

	for (int ii = 0 ; ii < m_iTtlDtCnt; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii);
		pPnF[ii] = gBasic->m_iClose;

		if (gBasic->m_iClose != GD_UNUSED && Start < 0)
			Start = ii;

		if (gBasic->m_iClose == GD_UNUSED)
			m_BlkDataCnt++;
	}

	if (Start > m_iTtlDtCnt - 2)
	{
		delete[] pPnF;	pPnF = NULL;
		return false;
	}

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

	m_piPnFData = new int[ m_iTtlDtCnt * 2];

	if (m_pLocal)
	{
		delete [] m_pLocal;
		m_pLocal = NULL;
	}
	
	m_pLocal = new struct _local_data[m_iTtlDtCnt];

	int PreVal = pPnF[Start], Cnt = 1;
	int Pivot = PreVal;
	int CurState = EMPTY;

	int		CurVal;
	int		mok, mod;
	int		igap = 0;
	int		iSOrgidx;
	double		dSDay, dEDay;
	CGrpBasic*	pBas = NULL;

	m_iCurveN = 0;
	m_piPnFData[0] = pPnF[Start];	// 시작값

	for (int ii = Start + 1; ii < m_iTtlDtCnt; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii);
		CurVal = pPnF[ii];

		if(igap == 0)
		{
			iSOrgidx = (ii >= 1) ? ii - 1 : ii;
		}

		igap++;

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
					m_pLocal[m_iCurveN].gap = igap;
					igap = 0;

					pBas = pOrgData->GetGraphData(iSOrgidx);
					dSDay = (pBas->m_index.date.yy * 10000 ) + (pBas->m_index.date.mm * 100 ) + pBas->m_index.date.dd;
					dSDay = (dSDay * 1000000) + (pBas->m_index.time.hh * 10000) + (pBas->m_index.time.mm * 100) + pBas->m_index.time.ss;
					
					pBas = pOrgData->GetGraphData(ii);
					dEDay = (pBas->m_index.date.yy * 10000 ) + (pBas->m_index.date.mm * 100 ) + pBas->m_index.date.dd;
					dEDay = (dEDay * 1000000) + (pBas->m_index.time.hh * 10000) + (pBas->m_index.time.mm * 100) + pBas->m_index.time.ss;
					
					m_pLocal[m_iCurveN].dSDay = dSDay;
					m_pLocal[m_iCurveN].dEDay = dEDay;

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

				if (CurState == EMPTY)
				{
					memcpy(&m_pLocal[m_iCurveN].date, &gBasic->m_index, sz_gIndex);
					m_pLocal[m_iCurveN].fpr = (double)gBasic->m_iOpen;
					m_pLocal[m_iCurveN].hpr = (double)gBasic->m_iHigh;
					m_pLocal[m_iCurveN].lpr = (double)gBasic->m_iLow;
					m_pLocal[m_iCurveN].epr = (double)gBasic->m_iClose;
					m_pLocal[m_iCurveN].gap = igap;
					igap = 0;

					pBas = pOrgData->GetGraphData(iSOrgidx);
					dSDay = (pBas->m_index.date.yy * 10000 ) + (pBas->m_index.date.mm * 100 ) + pBas->m_index.date.dd;
					dSDay = (dSDay * 1000000) + (pBas->m_index.time.hh * 10000) + (pBas->m_index.time.mm * 100) + pBas->m_index.time.ss;
					pBas = pOrgData->GetGraphData(ii);
					dEDay = (pBas->m_index.date.yy * 10000 ) + (pBas->m_index.date.mm * 100 ) + pBas->m_index.date.dd;
					dEDay = (dEDay * 1000000) + (pBas->m_index.time.hh * 10000) + (pBas->m_index.time.mm * 100) + pBas->m_index.time.ss;
					m_pLocal[m_iCurveN].dSDay = dSDay;
					m_pLocal[m_iCurveN].dEDay = dEDay;

					m_iCurveN++;
				}

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
					m_pLocal[m_iCurveN].gap = igap;
					igap = 0;
					
					pBas = pOrgData->GetGraphData(iSOrgidx);
					dSDay = (pBas->m_index.date.yy * 10000 ) + (pBas->m_index.date.mm * 100 ) + pBas->m_index.date.dd;
					dSDay = (dSDay * 1000000) + (pBas->m_index.time.hh * 10000) + (pBas->m_index.time.mm * 100) + pBas->m_index.time.ss;
					pBas = pOrgData->GetGraphData(ii);
					dEDay = (pBas->m_index.date.yy * 10000 ) + (pBas->m_index.date.mm * 100 ) + pBas->m_index.date.dd;
					dEDay = (dEDay * 1000000) + (pBas->m_index.time.hh * 10000) + (pBas->m_index.time.mm * 100) + pBas->m_index.time.ss;
				
					m_pLocal[m_iCurveN].dSDay = dSDay;
					m_pLocal[m_iCurveN].dEDay = dEDay;
					
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

				if (CurState == EMPTY)
				{
					memcpy(&m_pLocal[m_iCurveN].date, &gBasic->m_index, sz_gIndex);
					m_pLocal[m_iCurveN].fpr = (double)gBasic->m_iOpen;
					m_pLocal[m_iCurveN].hpr = (double)gBasic->m_iHigh;
					m_pLocal[m_iCurveN].lpr = (double)gBasic->m_iLow;
					m_pLocal[m_iCurveN].epr = (double)gBasic->m_iClose;
					m_pLocal[m_iCurveN].gap = igap;
					igap = 0;

					pBas = pOrgData->GetGraphData(iSOrgidx);
					dSDay = (pBas->m_index.date.yy * 10000 ) + (pBas->m_index.date.mm * 100 ) + pBas->m_index.date.dd;
					dSDay = (dSDay * 1000000) + (pBas->m_index.time.hh * 10000) + (pBas->m_index.time.mm * 100) + pBas->m_index.time.ss;
					pBas = pOrgData->GetGraphData(ii);
					dEDay = (pBas->m_index.date.yy * 10000 ) + (pBas->m_index.date.mm * 100 ) + pBas->m_index.date.dd;
					dEDay = (dEDay * 1000000) + (pBas->m_index.time.hh * 10000) + (pBas->m_index.time.mm * 100) + pBas->m_index.time.ss;
					m_pLocal[m_iCurveN].dSDay = dSDay;
					m_pLocal[m_iCurveN].dEDay = dEDay;

					m_iCurveN++;
				}

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

	delete[] pPnF;	pPnF = NULL;

	if (m_iCurveN == 1)
	{
		if (abs(m_piPnFData[0] - m_piPnFData[1]) < m_iOnePrice * m_iJWPnt)
			return false;
	}

	return true;
}

bool CVPNF::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
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

CString CVPNF::GetDisplayPosHeader(CPoint pt, bool bGrid)
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

CString CVPNF::GetDisplayPosData(CPoint pt, bool bGrid)
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
			unitStr1.Format("시가:%s\t고가:%s\t저가:%s\t$$종가:%s\t전환시가:%s\t전환종가:%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, sStart, sEnd);
		else
			unitStr1.Format("시가\t%s\t고가\t%s\t저가\t%s\t종가\t%s\t전환시가\t%s\t전환종가\t%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, sStart, sEnd);
	}

	return unitStr1;
}

CString CVPNF::GetXposValue(CPoint pt)
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

int CVPNF::GetState(int PreVal, int CurVal)
{
	if (PreVal > CurVal)
		return DEC;
	else if(PreVal < CurVal)
		return INC;
	else
		return EMPTY;
}

void CVPNF::FindMinMax(int *pArray, int Size)
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

void CVPNF::SetUserDefined(CString sVal)
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

void CVPNF::GetUsedValue(CString &sVal)
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

int CVPNF::GetYPos(double val)
{
	double fMinMax = m_dYMax - m_dYMin;
	double fHeight = m_rectGrp.Height();
	double yPos = 0;
	double fGab = m_dYMax - val;
	double Max = m_dYMax;
	double Min = m_dYMin;
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
		posValue = (double)m_rectGrp.top + yPos;
	
	return (int) posValue;
} 
// ljh
#include <math.h>

void CVPNF::GetValueToPoint(CPoint& pt, double dXValue, double dYValue)
{
	pt = CPoint(-1, -1);
	if (m_iCurveN < 1)
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
	
	struct	_local_data*	pObject = NULL;

	double	dX = 0;
	double	dMValue = dXValue;

	m_fUnit = (double)m_rectGrp.Width() / m_iCurveN;

	for (int ii = m_iStartInx; ii < m_iCurveN ; ii++ )
	{
		pObject = &m_pLocal[ii];

		if (ii < m_iTtlDtCnt && pObject->dSDay <= dMValue && dMValue < pObject->dEDay)
		{
			dX = (ii - m_iStartInx) * m_fUnit + (m_fUnit / 2);
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

void CVPNF::GetPtToPosValue(CPoint pt, double& dPos, double& dValue)
{
	double dMax = m_dYMax;
	double dMin = m_dYMin;

	if (m_iCurveN < 1)
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

	m_fUnit = (double)m_rectGrp.Width() / m_iCurveN;

	ixPos = (int)(double(pt.x - rectDraw.left) / m_fUnit); //* iDispCount) / double(rectDraw.Width());
	ixPos += GetDisplayPos();
	
	struct	_local_data*	pObject = NULL;

	if (ixPos >= m_iCurveN)
		ixPos = m_iCurveN - 1;

	if (ixPos < 0)
		ixPos = 0;

	if (ixPos < m_iTtlDtCnt)
	{
		pObject = &m_pLocal[ixPos];
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

void CVPNF::GetPtToEndvalPt(CPoint pt, CPoint &ept)
{
	double	dPos, dValue;
	GetPtToPosValue(pt, dPos, dValue);
	int	iXIdx = GetValueToXIdx(dPos);
	
	ept = pt;
	struct	_local_data*	pObject = NULL;

	if(iXIdx < m_iTtlDtCnt)
	{
		pObject = &m_pLocal[iXIdx];
		ept.y = GetYPos(pObject->hpr);
	}
	
}

CString CVPNF::GetIndexStrTool(int index, int &fpr, int &hpr, int &lpr, int &epr)
{
	if (index < 0)
		return _T("");
	
	if (index >= m_iCurveN)
		index = m_iCurveN - 1;
	
	struct	_local_data*	pObject = NULL;

	if (index > m_iTtlDtCnt)
		return _T("");

	pObject = &m_pLocal[index];
	
	fpr = (int)pObject->lpr;
	hpr = (int)pObject->hpr;
	lpr = (int)pObject->lpr;
	epr = (int)pObject->hpr;
	
	return _T("");
}

double CVPNF::GetOneDataSize()
{	
	return ((double)m_rectGrp.Width() / m_iCurveN);	
}

int	CVPNF::GetValueToXIdx(double dXValue)
{
	struct	_local_data*	pObject = NULL;

	double	dX = 0;
	double	dMValue = dXValue;
	
	m_fUnit = (double)m_rectGrp.Width() / m_iCurveN;

	for (int ii = m_iStartInx; ii < m_iCurveN ; ii++ )
	{
		pObject = &m_pLocal[ii];
		
		if (ii < m_iTtlDtCnt && pObject->dSDay <= dMValue && dMValue < pObject->dEDay)
			return (ii);
	}
	return 0;
}

void CVPNF::CalXpos()
{
	int WidthCnt = m_iCurveN;
	if (WidthCnt <= 0)	
		return;

	if ((m_iCurveN - WidthCnt) < 0)	
		m_iStartInx = 0;
	else				
		m_iStartInx = m_iCurveN - WidthCnt;

	m_dOnePoint = double(m_iDrawHeight) / (double(m_dYMax - m_dYMin) / double(m_iOnePrice));
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

	if (m_union)
	{
		for ( int ii = 0; ii < WidthCnt + 1; ii++)
		{
			dWidth = (double)m_rectGrp.Width() / (double)iPointCnt;

			if (ii == 0)
				m_piXpos[ii] = (wWidth - (dWidth * m_iDispSPos)) + dWidth * m_pLocal[ii].gap + (dWidth * m_BlkDataCnt);
			else if (ii == m_iCurveN)
				m_piXpos[ii] = m_piXpos[ii - 1] + (wRight - m_piXpos[ii - 1]);
			else
				m_piXpos[ii] = m_piXpos[ii - 1] + dWidth * m_pLocal[ii].gap;
		}
	}
	else
	{
		for ( int ii = 0; ii < WidthCnt + 1; ii++)
		{
			dWidth = (double)m_rectGrp.Width() / (double)m_iCurveN;
			m_piXpos[ii] = m_rectGrp.left + dWidth * ii + 1;
		}
	}
}

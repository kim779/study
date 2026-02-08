// BAVol.cpp: implementation of the CBAVol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BAVol.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBAVol::CBAVol(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
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

	m_iPoll = 10;
	m_rgbNorm = 0;
	m_rgbCurr = 0;
	m_rgbMax = 0;
	m_dUnit = 0.0;
	m_iDrawHeight = 0;
	m_iDrawWidth = 0;
	m_arData.RemoveAll();
	m_iCurr = 0;
	m_iMax = 0;
	m_dVMax = 0;
	m_iViewType = 0;
}

CBAVol::~CBAVol()
{
	RemoveData();
}

void CBAVol::RemoveData()
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

void CBAVol::DrawGraph(CDC* pDC)
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

//	DrawTick(pDC);	
	DrawChart(pDC);
}

void CBAVol::DrawHorizontalTick(CDC* pDC, int tkInfo)
{	
	CRect	tickRC = m_TkInfo.tkRect[tkInfo];
	int	tickHeight = tickRC.Height();	

	CFont	*tkFont = GetFont(TICK_LEFT);
	CFont	*sFont = pDC->SelectObject(tkFont);

	CPen	*tPen = GetPen(PS_SOLID, 1, RGB(126, 126, 126));
	CPen	*sPen = pDC->SelectObject(tPen);
	CRect	textRC;
	CSize	size = pDC->GetTextExtent("0");

	double	yPos = m_dMinCurr;
	int	nYpos = 0;
	int sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF sColor = pDC->SetTextColor(RGB(0, 0, 0));	
	CString	dispStr;

	textRC = tickRC;
	textRC.right -= size.cx;

	CRect	rcSave;
	bool	bEven = false;

	for ( int ii = 0 ; ii < m_iPoll + 1; ii++ )
	{
		nYpos = getYPos(yPos);		
		FormatVariableComma(dispStr, yPos, m_dMmaxCurr, 20);	
		yPos += m_dUnit;
		textRC.top = nYpos - size.cy * 2 / 3;
		textRC.bottom = nYpos + size.cy * 2 / 3;	
		textRC.NormalizeRect();

		if (bEven && !(ii % 2))
			continue;

		if (ii > 0 && textRC.bottom > rcSave.top)
		{
			bEven = true;
			continue;
		}

		pDC->DrawText(dispStr, textRC, DT_VCENTER | DT_SINGLELINE | DT_RIGHT);
		rcSave = textRC;
	}

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CBAVol::SetMember()
{
	m_iPoll = (int)m_pgraph->cInfo.awValue[0];
	m_iViewType = (int)m_pgraph->cInfo.awValue[1];
	m_rgbNorm = m_pgraph->cInfo.aline[0].rgbLColor;
	m_rgbMax = m_rgbNorm;

	int	rColor = GetRValue(m_rgbNorm);
	int	gColor = GetGValue(m_rgbNorm);
	int	bColor = GetBValue(m_rgbNorm);
	rColor = ((rColor - rColor/5) > 0)? rColor - rColor/5:0;
	gColor = ((gColor - gColor/5) > 0)? gColor - gColor/5:0;
	bColor = ((bColor - bColor/5) > 0)? bColor - bColor/5:0;
	m_rgbCurr = RGB(rColor, gColor, bColor);
	
	m_iDrawHeight	= m_rectGrp.Height();
	m_iDrawWidth	= m_rectGrp.Width() - 2;
	
	if (m_iPoll)
		if ((m_dMmaxCurr - m_dMinCurr) == 0)
			m_dUnit = m_dMmaxCurr;
		else
			m_dUnit = (m_dMmaxCurr - m_dMinCurr) / (double)m_iPoll;
	else
		m_dUnit = 0.0;
	
}

bool CBAVol::CalculateMinMax(bool bForce)
{
	FindMinMax();
	SetMember();
	
	if (!CreateData())
		return false;
	
	return true;
}

bool CBAVol::CreateData()		
{
	CGrpBasic* gBasic;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return false;
	}

	CMap<int, int, double, double>	mapValue;
	
	if (m_iPoll == 0 || m_dUnit == 0.0)
		return false;

	RemoveData();
	struct _local_data* pObject = NULL;           
	int	ii = 0, jj = 0;
	int	nSize = m_iDispEPos - m_iDispSPos;
	int	key = 0;
	double	val = 0.0, sum = 0.0;


	mapValue.RemoveAll();

	for ( ii = 0 ; ii < m_iPoll ; ii++ )
	{
		mapValue.SetAt(ii, 0);	
	}

	m_iMax = -1;
	m_iCurr = -1;
	m_dVMax = DBL_MIN;
		
	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		gBasic = pOrgData->GetGraphData(ii + m_iDispSPos);

		if (gBasic->m_iClose == GD_UNUSED)
			continue;		

		sum += (double)gBasic->m_dVolume;
		key = (int)(((double)gBasic->m_iClose - m_dMinCurr) / m_dUnit);

		if (gBasic->m_iClose == m_dMmaxCurr)
			key = m_iPoll - 1;

		if (ii == (nSize - 1))
			m_iCurr = key;

		val = 0.0;
		mapValue.Lookup(key, val);
		val += (double)gBasic->m_dVolume;		

		if (val > m_dVMax)
		{
			m_dVMax = val;
			m_iMax = key;
		}

		mapValue.SetAt(key, val);
	}	

	for ( ii = 0 ; ii < m_iPoll ; ii++ )
	{
		pObject = new struct _local_data;
		mapValue.Lookup(ii, val);
		pObject->amount = val;
		pObject->percent = val / sum * 100.0;
		m_arData.Add(pObject);
	}

	return true;
}

void CBAVol::FindMinMax()
{	
	m_dMmaxCurr = DBL_MIN;
	m_dMinCurr = DBL_MAX;
	
	m_dYMin = DBL_MAX;
	m_dYMax = DBL_MIN;

	CGrpBasic* gBasic;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return;
	}

	for ( int ii = 0 ; ii < m_iDispEPos - m_iDispSPos ; ii++ )
	{
		gBasic = pOrgData->GetGraphData(ii + m_iDispSPos);

		if (gBasic->m_iClose == GD_UNUSED)
			continue;	

		if (m_dYMin > gBasic->m_iLow)
			m_dYMin = gBasic->m_iLow;
		if (m_dYMax < gBasic->m_iHigh)
			m_dYMax = gBasic->m_iHigh;		

		if (m_dMmaxCurr < gBasic->m_iClose)
			m_dMmaxCurr = (double)gBasic->m_iClose;
		if (m_dMinCurr > gBasic->m_iClose)
			m_dMinCurr = (double)gBasic->m_iClose;
	}
}

int CBAVol::getYPos(double val)
{
	double	MaxValue = m_dYMax;
	double	MinValue = m_dYMin;
	double	yVal = val;
	
	if (IsLogChart())
	{
		MaxValue = GetLogData(MaxValue);
		MinValue = GetLogData(MinValue);
		yVal = GetLogData(yVal);
	}
	double	fMinMax = MaxValue - MinValue;
	double	fHeight = m_rectGrp.Height();
	double	yPos = 0;
	double	fGab = MaxValue - yVal;

	if (fMinMax <= 0.0)
		return m_rectGrp.bottom;

	yPos = fHeight * fGab / fMinMax;

	if (IsReverseChart())
		yPos = (double)m_rectGrp.bottom - yPos;
	else
		yPos = (double)m_rectGrp.top + yPos;

	return int(yPos);
}

void CBAVol::DrawChart(CDC* pDC)
{
	if (m_iPoll <= 0 || m_dVMax <= 0)
		return;

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreateRectRgn(0, 0, 0, 0);
	CRgn	tmpRGN[2];
	CRect	tmpRECT;

	CPen		*cPen[3];
	CBrush		*cBrush[3];
	COLORREF	color[3] = {m_rgbNorm, m_rgbCurr, m_rgbMax};
	COLORREF	selcolor;
	int		ii = 0, jj = 0, nWidth = 0;
	CRect		rcDraw, rcDisplay, rect;
	CPoint		pt[5];
	double		yPos = 0.0, val = 0.0;	
	struct	_local_data	*pObject = NULL;
	CString		dispText;
	bool		bPercent = true;
	CString		tmpstr;
	CSize		size1, size2;

	FormatNormalComma(tmpstr, m_dVMax, true, 0);
	dispText.Format(" %s (000.00%%) ", tmpstr);
	size1 = pDC->GetTextExtent(dispText);
	size2 = pDC->GetTextExtent("0");

	for ( ii = 0 ; ii < 3 ; ii++ )
	{
		cPen[ii] = GetPen(PS_SOLID, 1, color[ii]);
		cBrush[ii] = GetBrush(color[ii]);
	}

	CPen	*sPen = pDC->SelectObject(cPen[0]);
	CBrush	*sBrush = pDC->SelectObject(cBrush[0]);
	rcDisplay = rcDraw = m_rectGrp;
	rcDisplay.left = rcDraw.left = m_rectGrp.left + 2;
	rcDraw.right = rcDraw.left + m_iDrawWidth - size1.cx/4;
	rcDisplay.left = rcDraw.right;	
	nWidth = rcDraw.Width();	
	yPos = m_dMinCurr;	
	int	nMid = 0, nHalf = size1.cy / 2;
	bool*	bSelect = new bool[m_iPoll];
	CPoint	ptCenter;
	CArray<CPoint, CPoint>	arPoint;

	if (m_iPoll < 4)
	{
		for ( ii = m_iPoll - 1 ; ii >= 0 ; ii-- )
		{
			bSelect[ii] = true;
		}
	}
	else if (m_iPoll < 8)
	{
		for ( ii = m_iPoll - 1, jj = 0 ; ii >= 0 ; ii--, jj++)
		{
			if ((jj % 2) == 0)
				bSelect[ii] = true;
			else
				bSelect[ii] = false;
		}
	}
	else if (m_iPoll < 20)
	{
		for ( ii = m_iPoll - 1, jj = 0 ; ii >= 0 ; ii--, jj++)
		{
			if ((jj % 3) == 0)
				bSelect[ii] = true;
			else
				bSelect[ii] = false;
		}
	}
	else
	{
		int	nIndex = m_iPoll / 4;

		for ( ii = m_iPoll - 1, jj = 0 ; ii >= 0 ; ii--, jj++)
		{
			if ((jj % nIndex) == 0)
				bSelect[ii] = true;
			else
				bSelect[ii] = false;
		}
	}

	int	nHeight = m_rectGrp.Height();

	int	sMode = pDC->SetBkMode(TRANSPARENT);
	CFont	*tkFont = GetFont(TICK_BOTTOM);
	CFont	*sFont = pDC->SelectObject(tkFont);

	CRect	rcSave;

	for ( ii = 0 ; ii < m_iPoll ; ii++ )
	{
		if (ii == m_iMax)
		{
			pDC->SelectObject(cPen[2]);
			pDC->SelectObject(cBrush[2]);
			selcolor = color[2];
		}
		else
		{
			pDC->SelectObject(cPen[0]);
			pDC->SelectObject(cBrush[0]);
			selcolor = color[0];
		}

		if (ii == m_iCurr)
		{
			pDC->SelectObject(cPen[1]);
			pDC->SelectObject(cBrush[1]);
			selcolor = color[1];
		}

		pObject = m_arData.GetAt(ii);
		val = pObject->amount;

		if (IsReverseChart())
			pt[3].y = pt[2].y = getYPos(yPos) + 3;
		else
			pt[1].y = pt[0].y = getYPos(yPos) - 3;
		pt[3].x = pt[0].x = rcDraw.left;
		pt[4] = pt[0];
		pt[2].x = pt[1].x = (int)((double)rcDraw.left + val * nWidth / m_dVMax);
		yPos += m_dUnit;

		if (yPos > m_dMmaxCurr)
			yPos = m_dMmaxCurr;		

		if (IsReverseChart())
			pt[1].y = pt[0].y = getYPos(yPos) - 3;
		else
			pt[3].y = pt[2].y = getYPos(yPos) + 3;
			
		
		rect.SetRect(pt[3], pt[1]);

		if (val > 0.0)
		{
			pDC->Polygon(pt, 5);
//			pDC->Polyline(pt, 5);
//			Draw3DBar(pDC, rect, 1, selcolor);
		}

		tmpRECT = rect;
		tmpRECT.InflateRect(rgnLINEGAP, rgnLINEGAP);
		tmpRGN[0].DeleteObject();
		tmpRGN[0].CreateRectRgnIndirect(&tmpRECT);

		tmpRECT = rect;
		tmpRECT.DeflateRect(rgnLINEGAP, rgnLINEGAP);
		tmpRGN[1].DeleteObject();
		tmpRGN[1].CreateRectRgnIndirect(&tmpRECT);

		ptCenter = tmpRECT.CenterPoint();
		arPoint.Add(ptCenter);
		
		tmpRGN[0].CombineRgn(&tmpRGN[0], &tmpRGN[1], RGN_XOR);
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CombineRgn(m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn, &tmpRGN[0], RGN_OR);
		
		rcDisplay.left = pt[2].x + size2.cx;
		rcDisplay.bottom = pt[0].y;
		rcDisplay.top = pt[3].y;
		nMid = (pt[0].y + pt[2].y) / 2;		
		
		rcDisplay.bottom = nMid + size1.cy / 2 - 1;
		rcDisplay.top = nMid - size1.cy / 2;		

		switch (m_iViewType)
		{
		case 0:
			FormatNormalComma(dispText, pObject->amount, true, 0);
			break;
		case 1:
			dispText.Format("%.2lf%%", pObject->percent);
			break;
		case 2:
			{
				CString	tmpS1, tmpS2;	
				FormatNormalComma(tmpS1, pObject->amount, true, 0);
				tmpS2.Format("%.2lf%%", pObject->percent);
				dispText.Format("%s(%s)", tmpS1, tmpS2);
			}
			break;
		default:
			dispText.Empty();
			continue;
		}

		size1 = pDC->GetTextExtent(dispText);
		rcDisplay.right = rcDisplay.left = rcDisplay.left - size2.cx;
		rcDisplay.left -= size1.cx;
		if (rcDisplay.left < m_rectGrp.left)
			rcDisplay.OffsetRect(m_rectGrp.left - rcDisplay.left + 2, 0);
//		rcDisplay.right = rcDisplay.left + size1.cx;

		if (ii > 0 && rect.IntersectRect(rcDisplay, rcSave))
			continue;

		if (val > 0.0)
			pDC->DrawText(dispText, rcDisplay, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOCLIP);
		rcSave = rcDisplay;
	}

	for ( ii = 0 ; ii < 2 ; ii++ )
	{
		tmpRGN[ii].DeleteObject();
	}


	if (!(m_pgraph->dwDraOption & GO_UNION))
	{
		CPoint	point = (0,0);
		CString	cnvText;
		dispText.Format ("매물[%d]", m_iPoll);
		DrawTickIndex(pDC, dispText, m_pgraph->rgbLabel, true);
	}

	pDC->SetBkMode(sMode);
	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);
	pDC->SelectObject(sFont);

	if (m_RgnInfo.iSelect >= 0 && nHeight > 10)
	{
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);

		if (arPoint.GetSize() > 0)
		{	
			for ( ii = 0 ; ii < m_iPoll ; ii++ )
			{
				if (bSelect[ii])
				{
					ptCenter = arPoint.GetAt(ii);
					tmpRECT.SetRect(ptCenter.x - rgnMARKGAP, ptCenter.y - rgnMARKGAP, 
							ptCenter.x + rgnMARKGAP, ptCenter.y + rgnMARKGAP);
					tmpRECT.IntersectRect(&tmpRECT, &m_rectGrp);

					pDC->Rectangle(&tmpRECT);
				}
			}
		}

		pDC->SetROP2(sROP);
		pDC->SelectObject(srgnBrush);
	}

	delete [] bSelect;
	bSelect = NULL;
	m_RgnInfo.iCount++;
}

void CBAVol::DrawTick(CDC* pDC)
{
	if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_PRC)
	{
		m_dYMax = m_pCoDraw->PMax;
		m_dYMin = m_pCoDraw->PMin;
	}
	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
		return;

	if (m_pgraph->dwDraOption & GO_TKLEFT)
		DrawHorizontalTick(pDC, TICK_LEFT);
	if (m_pgraph->dwDraOption & GO_TKRIGHT)
		DrawTickRight(pDC, m_dYMax, m_dYMin, true);
}
bool CBAVol::Draw3DBar(CDC* pDC, CRect rc, int Gap, COLORREF orgColor)
{
	COLORREF	color[3];
	color[0] = orgColor;
	Get3DRGB(color);

	CPen	*cPen = GetPen(PS_SOLID, 1, color[0]);
	CPen	*sPen = pDC->SelectObject(cPen);
	CBrush	*cBrush = GetBrush(color[0]);
	CBrush	*sBrush = pDC->SelectObject(cBrush);
	CPoint	pointBAR[6];				// 정면

	pointBAR[0].x = rc.left;
	pointBAR[0].y = rc.top;
	pointBAR[1].x = rc.left;
	pointBAR[1].y = rc.bottom;
	pointBAR[2].x = rc.right;
	pointBAR[2].y = rc.bottom;
	pointBAR[3].x = rc.right;
	pointBAR[3].y = rc.top;
	pDC->Polygon(pointBAR, 4);

	cPen = GetPen(PS_SOLID, 1, color[1]);
	pDC->SelectObject(cPen);
	cBrush = GetBrush(color[1]);
	pDC->SelectObject(cBrush);

	pointBAR[0].x = rc.left;
	pointBAR[0].y = rc.bottom;
	pointBAR[1].x = rc.left;
	pointBAR[1].y = rc.top;
	pointBAR[2].x = rc.right;
	pointBAR[2].y = rc.top;
	pointBAR[3].x = rc.right + Gap;
	pointBAR[3].y = rc.top - Gap;
	pointBAR[4].x = rc.left - Gap;
	pointBAR[4].y = rc.top - Gap;
	pointBAR[5].x = rc.left - Gap;
	pointBAR[5].y = rc.bottom + Gap;
	pDC->Polygon(pointBAR, sizeof(pointBAR)/sizeof(CPoint));

	cPen = GetPen(PS_SOLID, 1, color[2]);
	pDC->SelectObject(cPen);
	cBrush = GetBrush(color[2]);
	pDC->SelectObject(cBrush);

	pointBAR[0].x = rc.left;
	pointBAR[0].y = rc.bottom;
	pointBAR[1].x = rc.right;
	pointBAR[1].y = rc.bottom;
	pointBAR[2].x = rc.right;
	pointBAR[2].y = rc.top;
	pointBAR[3].x = rc.right + Gap;
	pointBAR[3].y = rc.top - Gap;
	pointBAR[4].x = rc.right + Gap;
	pointBAR[4].y = rc.bottom + Gap;
	pointBAR[5].x = rc.left - Gap;
	pointBAR[5].y = rc.bottom + Gap;
	pDC->Polygon(pointBAR, sizeof(pointBAR)/sizeof(CPoint));

	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);

	return true;
}

// 0:origin color, [1]:light color, [2]:dark color
void CBAVol::Get3DRGB(COLORREF color[3])
{
	int	rColor = GetRValue(color[0]);
	int	gColor = GetGValue(color[0]);
	int	bColor = GetBValue(color[0]);
	int	rRate  = rColor / 5;
	int	gRate  = gColor / 5;
	int	bRate  = bColor / 5;

	int	r = (rColor + rRate > 255)? 255:rColor + rRate;
	int	g = (gColor + gRate > 255)? 255:gColor + gRate;
	int	b = (bColor + bRate > 255)? 255:bColor + bRate;
	color[1] = RGB(r, g, b);
	r = (rColor - rRate < 0)? 0:rColor - rRate;
	g = (gColor - gRate < 0)? 0:gColor - gRate;
	b = (bColor - bRate < 0)? 0:bColor - bRate;
	color[2] = RGB(r, g, b);
}


bool CBAVol::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
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


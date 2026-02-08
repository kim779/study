#include "stdafx.h"
#include "VPCV.h"
#include "../gData/DataMgr.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVPCV::CVPCV(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
: CVarsBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_pPVData = NULL;
	m_pVol = NULL;
	m_UnitSize = 20; // -----set n value;
	m_bCalculated = false;
}

CVPCV::~CVPCV()
{
	if (m_pPVData != NULL)
	{
		delete[] m_pPVData;
		m_pPVData = NULL;
	}

	if (m_pVol != NULL)
	{
		delete [] m_pVol;
		m_pVol = NULL;
	}

}

void CVPCV::DrawGraph(CDC* pDC)
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

	if (m_dYMin == DBL_MAX || m_dYMax == DBL_MIN)
		return;

	if (m_dXMin == DBL_MAX || m_dXMax == DBL_MIN)
		return;

//	CVarsBase::DrawTick(pDC);
	DrawBottomTick(pDC);
	DrawChart(pDC);
}

bool CVPCV::CalculateDisplay(double &div, double Max, double Min, double& drawMinValue, 
		int dispTICK, double* dispValue, double* logValue, int& dispCount)
{
	int	index;
	CString	temp;
	double	mok = Min / div;
	temp.Format("%f", mok);
	index = temp.Find('.'); 
	mok = atof(temp.Left(index));
	drawMinValue = mok * div;
	if (drawMinValue < Min)		
		drawMinValue += div;
	int	drawNum = int ((Max - drawMinValue) / div);
	if (drawNum >= dispTICK)
	{
		CString	tStr;
		tStr.Format("%f", div);
		if (tStr.GetAt(0) == '5')
		{
			if (drawNum <= dispTICK*2)
				div *= 2;
			else if (drawNum <= dispTICK*10)
				div *= 10;
			else if (drawNum <= dispTICK*20)
				div *= 20;
			else
				div *= 100;
		}
		else
		{
			if (drawNum <= dispTICK*5)
				div *= 5;
			else if (drawNum <= dispTICK*10)
				div *= 10;
			else if (drawNum <= dispTICK*50)
				div *= 50;
			else
				div *= 100;
		}


		mok = Min / div;
		temp.Format("%f", mok);
		index = temp.Find('.'); 
		mok = atof(temp.Left(index));
		drawMinValue = mok * div;
		if (drawMinValue < Min)		
			drawMinValue += div;
	}

	dispCount = 0;
	dispValue[dispCount++] = Min;
	dispValue[dispCount] = drawMinValue;
	bool	bLog = IsLogChart();
	if (bLog)
	{
		if (drawMinValue > 0)
			logValue[dispCount] = double(1000. * double(log(drawMinValue)));
		else if (drawMinValue< 0)
			logValue[dispCount] = (-1) * double(1000. * double(log(drawMinValue*(-1))));
		else
			logValue[dispCount] = 0;
	}
	double dVal = 0.0;
	while(1)
	{
		if (dispValue[dispCount] > Max + 0.01)
			break;

		dispValue[dispCount+1] = dispValue[dispCount] + div;
		if (bLog)
		{
			dVal = dispValue[dispCount+1];
			if (dVal > 0)
				logValue[dispCount+1] = double(1000. * double(log(dVal)));
			else if (dVal < 0)
				logValue[dispCount+1] = (-1) * double(1000. * double(log(dVal*(-1))));
			else
				logValue[dispCount+1] = 0;
		}
		dispCount++;
	}
	if (dispCount <= 0)
	{
		if (Max == Min)
		{
			dispValue[0] = Max;
			dispCount = 1;
		}
		else
			return false;
	}
	return true;
}


void CVPCV::DrawBottomTick(CDC *pDC)
{
	if (!(m_pgraph->dwDraOption & GO_TKBOTTOM))
	{
		TRACE("DrawTickRight Function Error\n");
		return;
	}

	int	dispCount;
	double	dispValue[64];
	double	logValue[64];
	CRect	tickRC = m_TkInfo.tkRect[TICK_BOTTOM];
	CRect	orgTickRC = tickRC;
	tickRC.left += 10;
	orgTickRC.top = m_rectOrgGrp.top;

	int	tickHeight = tickRC.Height();
	double	ValueHeight = m_dXMax - m_dXMin;
	if (ValueHeight <= 0) 	ValueHeight = 1;
	
	double	div;
	if (ValueHeight < 5)			div = 1;
	else if (ValueHeight < 50)		div = 5;
	else if (ValueHeight < 200)		div = 10;
	else if (ValueHeight < 500)		div = 50;
	else if (ValueHeight < 2000)		div = 100;
	else if (ValueHeight < 5000)		div = 500;
	else if (ValueHeight < 20000)		div = 1000;
	else if (ValueHeight < 50000)		div = 5000;
	else if (ValueHeight < 200000)		div = 10000;
	else if (ValueHeight < 500000)		div = 50000;
	else if (ValueHeight < 2000000)		div = 100000;
	else if (ValueHeight < 5000000)		div = 500000;
	else if (ValueHeight < 20000000)	div = 1000000;
	else if (ValueHeight < 50000000)	div = 5000000;
	else if (ValueHeight < 200000000)	div = 10000000;
	else if (ValueHeight < 500000000)	div = 50000000;
	else if (ValueHeight < 2000000000)	div = 100000000;
	//else					div = 500000000;
	else
	{
		CString	strTmp, strDiv;
		strTmp.Format("%.0f", ValueHeight);
		strDiv.Format("1%0*d", strTmp.GetLength()-1, 0);
		div = atof(strDiv);
	}

	int	dispTICK = GetTickCount(TICK_BOTTOM);
	if (dispTICK <= 0)
		return;

	double	drawMinValue= 0.0;
	double	dVal = 0.0;

	if (!CalculateDisplay(div, m_dXMax, m_dXMin, drawMinValue, dispTICK, dispValue, logValue, dispCount))
		return;

	int		sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF	sColor = pDC->SetTextColor(m_atick[TICK_BOTTOM].dwTColor);
	int		style = m_lineV.btLStyle;
	int		width = m_lineV.btLWidth;
	COLORREF	color = m_lineV.rgbLColor;

	CRect	textRC;
	CFont	*tkFont = GetFont(TICK_BOTTOM);
	CFont	*sFont = pDC->SelectObject(tkFont);
	CPen	*cPen = GetPen(style, width, color);
	CPen	*lPen = GetPen(PS_SOLID, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	CString	dispStr;
	CPoint	pointS, pointE;
	CRect	tkSaveRC = tickRC;

	CSize	size;
	int	maxdigit = 20;	

	FormatPCVComma(dispStr, m_dXMax, m_dXMax, maxdigit);
	size = pDC->GetOutputTextExtent(dispStr);
	int	PivotY = tickRC.left + gapTICK + size.cx;

	bool	bDrawLine = (m_dwDspOpt & DO_TKVGRID)? true:false;

	for ( int ii = 0, jj = 0 ; ii < dispCount ; ii++ )		// 가격별가로 (DOT)
	{
		if (dispValue[ii] < drawMinValue)
			continue;
		
		pointS.x = GetXPos(dispValue[ii]);
		pointS.y = m_rectOrgGrp.top;		

		if (!tickRC.PtInRect(pointS))
		{
			if (tickRC.bottom < pointS.y)
				continue;
		}

		pointE.x = pointS.x;
		pointE.y = m_rectOrgGrp.bottom - 1;

		if (bDrawLine)
		{
			pDC->SelectObject(cPen);
			pDC->MoveTo(pointE);
			pDC->LineTo(pointS);
		}

		
		pointS.y = tickRC.top;
		

		if (!tickRC.PtInRect(pointS))
		{
			if (tickRC.bottom < pointS.y)
				continue;
		}

		pointE.y = tickRC.top + lenTICK;

		pDC->MoveTo(pointE);
		pDC->LineTo(pointS);

		pDC->SelectObject(lPen);
		
		FormatPCVComma(dispStr, dispValue[ii], m_dXMax, maxdigit);

		size = pDC->GetOutputTextExtent(dispStr);
		textRC = DisplayRect(pDC, 0, dispStr, pointS);
		textRC.top += 4;

		textRC.left -= size.cx / 2;
		textRC.right = textRC.left + size.cx;

		if (jj != 0)
		{
			if (textRC.left - tkSaveRC.right <= 5)	
				continue;
		}

		if (textRC.right > tickRC.right)
			textRC.OffsetRect(tickRC.right - textRC.right, 0);
			
		pDC->DrawText(dispStr, &textRC, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOCLIP);
		tkSaveRC = textRC;
		jj++;
	}

	dispStr = GetPCVUnit(m_dXMax);
	pointS.x = m_rectOrgGrp.right;
	textRC = DisplayRect(pDC, 0, dispStr, pointS);
	textRC.left += 4;
	textRC.top += 4;	
	pDC->DrawText(dispStr, &textRC, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOCLIP);

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CVPCV::DrawChart(CDC* pDC)
{
	int	nPoint = (m_iDispEPos - m_StartInx);
	if (nPoint <= 0)
		return;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return;
	}

	CPoint*	pPoint = new CPoint[nPoint];
	CPoint	Drawxy;
	
	int ii = m_StartInx;
	
	for (int jj = 0; ii < m_iDispEPos; ii++, jj++)
	{
		Drawxy.x = GetXPos(m_pVol[ii]);
		Drawxy.y = GetYPos(m_pPVData[ii]);
		pPoint[jj] = Drawxy;
	}

	CPen	*pPen[5];
	for (ii = 0; ii < 5; ii++)
		pPen[ii] = GetPen(m_pLineStyle[ii], m_pLineWidth[ii], m_pLineColor[ii]);

	CPen	*sPen = pDC->SelectObject(pPen[0]);
	CFont	*tkFont = GetFont(TICK_BOTTOM);
	CFont	*sFont = pDC->SelectObject(tkFont);

	CString	dispText;
	CPoint	pt;

	int	ColorInx = -1;
	int	CurMonth, PreMonth = 0;
	int	sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF sTxtColor = pDC->SetTextColor(RGB(0, 0, 0));


	// draw graph
	pDC->MoveTo(pPoint[0]);

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	CPoint	*pRgnPoint = new CPoint[nPoint*2];

	Drawxy = pPoint[0];
	pRgnPoint[nPoint*2 - 1] = pRgnPoint[0] = Drawxy;
	pRgnPoint[0].y = Drawxy.y - rgnLINEGAP;
	pRgnPoint[nPoint*2 - 1].y = Drawxy.y + rgnLINEGAP;
	
	for (ii = 0; ii < nPoint; ii++) 
	{
		CGrpBasic* gBasic = pOrgData->GetGraphData(ii + m_StartInx);			
		if (GetDataIndex() == GI_DAY)
		{
			CurMonth = gBasic->m_index.date.mm;
			if (CurMonth != PreMonth)
			{
				ColorInx++;
				PreMonth = CurMonth;
				int x = ColorInx % 5;
				pDC->SelectObject(pPen[x]);

				CString		tmpstr;
				tmpstr.Format("%04d/%02d/%02d", gBasic->m_index.date.yy,
					gBasic->m_index.date.mm, gBasic->m_index.date.dd);
				pDC->SetTextColor(m_pgraph->cInfo.aline[x].rgbLColor);
				pDC->TextOut(pPoint[ii].x, pPoint[ii].y, tmpstr);
			}
		}

		pDC->LineTo(pPoint[ii]);

		Drawxy = pPoint[ii];
		pRgnPoint[nPoint*2 - ii - 1] = pRgnPoint[ii] = Drawxy;
		pRgnPoint[ii].y = Drawxy.y - rgnLINEGAP;
		pRgnPoint[nPoint*2 - ii - 1].y = Drawxy.y + rgnLINEGAP;
	}
	
	pDC->SetTextColor(sTxtColor);
	pDC->SetBkMode(sMode);
	pDC->SelectObject(sPen);
	pDC->SelectObject(sFont);

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, nPoint * 2, WINDING);	

	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);
		for (int ii = 0; ii < 4; ii++)
		{
			int idx = (nPoint) * (ii+1)/5;
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

	CString	str;
	dispText.Format("PV[%d]", m_UnitSize);
	DrawTickIndex(pDC, dispText, m_pgraph->rgbLabel, true);

	m_RgnInfo.iCount++;
	delete[] pPoint;	pPoint = NULL;
	delete[] pRgnPoint;	pRgnPoint = NULL;
}

bool CVPCV::CreateData()
{
	if (m_iTtlDtCnt <= 0)
		return false;

	COrgData*	pOrgData = NULL;

	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return false;

	int	nSize = m_iTtlDtCnt;
	int	ii = 0;
	CGrpBasic*	pBasic = NULL;
	int	Start = -1;

	if (m_pPVData)
	{
		delete [] m_pPVData;
		m_pPVData = NULL;
	}

	m_pPVData = new double[m_iTtlDtCnt];	

	if (m_pVol)
	{
		delete [] m_pVol;
		m_pVol = NULL;
	}

	m_pVol = new double[m_iTtlDtCnt];
	
	double*	pPV = new double[nSize];

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pBasic = pOrgData->GetGraphData(ii);				
		pPV[ii] = pBasic->m_iClose;

		if (Start < 0)
		{
			if (pBasic->m_iClose != GD_UNUSED)
			{
				Start = ii;
			}			
		}
	}

	m_StartInx = CDataMgr::CalcSMA(pPV, m_pPVData, m_iTtlDtCnt, Start, m_UnitSize);

	if (m_StartInx < 0 || m_StartInx >= m_iTtlDtCnt)
	{
		delete [] pPV ;
		pPV = NULL;
		return false;
	}
	
	ii = m_StartInx;	
	
	CGrpBasic* gBasic = NULL;
	gBasic	= pOrgData->GetGraphData(ii);
	double	val = 0;
	
	for (; ii < m_iDispEPos; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii);	
		val += gBasic->m_dVolume;
		m_pVol[ii] = val;
	}

	delete[] pPV;
	pPV = NULL;

	return true;
}

bool CVPCV::CalculateMinMax(bool bForce)
{
	SetMember();	
	
	if (!m_bCalculated || bForce)
	{
		if (!CreateData())
			return false;

		m_bCalculated = true;
	}

	FindMinMax(m_dYMin, m_dYMax, &m_pPVData[m_StartInx], m_iTtlDtCnt - m_StartInx);
	FindMinMax(m_dXMin, m_dXMax, &m_pVol[m_StartInx], m_iTtlDtCnt - m_StartInx);

	if (m_dXMin == m_dXMax)
		m_dXMax += 10;

	if (m_dYMin == m_dYMax)
		m_dYMax += 10;

	ReviseMinMax(m_dYMax, m_dYMin);
	return true;
}

int CVPCV::GetYPos(double val)
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

int CVPCV::GetXPos(double val)
{
	double	fMinMax = m_dXMax - m_dXMin;
	double	fHeight = m_rectGrp.Width();
	double	yPos = 0;
	double	fGab = val - m_dXMin;

	if (fMinMax <= 0.0)
		return m_rectGrp.left;

	yPos = fHeight * fGab / fMinMax;
	return (int)((double)m_rectGrp.left + yPos);
}

bool CVPCV::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
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

void CVPCV::SetMember()
{
	m_iDrawHeight	= m_rectGrp.Height();
	m_iDrawWidth	= m_rectGrp.Width();

	m_UnitSize = (int)m_pgraph->cInfo.awValue[0];

	if (m_UnitSize <= 0)
		m_UnitSize = 20; // -----set n value;

	for ( int ii = 0 ; ii < 5 ; ii++ )
	{
		m_pLineStyle[ii] = m_pgraph->cInfo.aline[ii].btLStyle;
		m_pLineWidth[ii] = m_pgraph->cInfo.aline[ii].btLWidth;
		m_pLineColor[ii] = m_pgraph->cInfo.aline[ii].rgbLColor;	
	}	
}

void CVPCV::FindMinMax(double &Min, double &Max, double *pArray, int Size)
{
	Min = DBL_MAX;
	Max = DBL_MIN;

	for (int ii = 0; ii < Size; ii ++)
	{
		if (pArray[ii] == GD_UNUSED)
			continue;
		
		if (Max < pArray[ii])
			Max = pArray[ii];

		if (Min > pArray[ii])
			Min = pArray[ii];
	}
}

void CVPCV::FormatPCVComma(CString &dispTxt, double dVal, double Max, int maxDigit)
{
	CString	edit1, edit2;
	double dotL = 0, dotR = 0;

	bool bSign = false;
	if (dVal < 0)
	{
		bSign = true;
		dVal = dVal * (-1);
	}

	dotL = dVal;

	if (Max > dispUNIT * 1000.0)
		dotL = int(dotL / 1000000.0);
	else
	{
		if (Max > dispUNIT)
			dotL = int(dotL / 1000.0);
	}

	dispTxt = _T("");
	edit1.Format("%.f", dotL);
	edit1.MakeReverse();

	for (int ii = 0; ii < edit1.GetLength(); ii++)
	{
		if (ii && !(ii % 3))
			edit2 += ',';
		edit2 += edit1[ii];
	}
	if (bSign) edit2 += "-";
	edit2.MakeReverse();

	edit1= _T("");
	
	int trimComma = edit2.GetLength() + edit1.GetLength() - maxDigit;
	if (trimComma > 0)
	{
		int trim = 0;
		for (int ii = 0; ii < edit2.GetLength(); ii++)
		{
			if (edit2[ii] == ',' && trim < trimComma)
			{
				trim++;
				continue;
			}
			dispTxt += edit2[ii];
		}
		dispTxt += edit1;
	}
	else
	{
		dispTxt = edit2 + edit1;
	}
}

CString CVPCV::GetPCVUnit(double Max)
{
	CString dispStr = _T("");
	
	switch (m_iUnit)
	{
	case GU_CODE:
		if (Max > dispUNIT * 1000)
		{
			dispStr = _T("[백만주]");	
		}
		else
		{
			if (Max > dispUNIT)
				dispStr = _T("[천주]");
			else
				dispStr = _T("[ 주 ]");			
		}
		break;
	case GU_INDEX:
		if (Max > dispUNIT * 1000)
		{
			dispStr = _T("[십억]");	
		}
		else
		{
			if (Max > dispUNIT)
				dispStr = _T("[백만]");
			else
				dispStr = _T("[천주]");			
		}
		break;
	case GU_COFUT:
	case GU_COOPT:
	case GU_FUTURE:
	case GU_OPTION:
		if (Max > dispUNIT * 1000)
		{
			dispStr = _T("[백만]");	
		}
		else
		{
			if (Max > dispUNIT)
				dispStr = _T("[천]");
			else
				dispStr = _T("[계약]");			
		}
		break;
	default:
		if (Max > dispUNIT * 1000)
			dispStr = _T("[백만]");	
		else if (Max > dispUNIT)
			dispStr = _T("[천]");

		break;
	}

	return dispStr;
}

CString CVPCV::GetDisplayPosHeader(CPoint pt, bool bGrid)
{
	CString	unitStr1 = _T("");

	if (bGrid)	return unitStr1;
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)	return unitStr1;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))	return unitStr1;
	
	CGrpBasic* gBasic = NULL;
	CPoint	chkPt, chkPt2;

	CRect	rc;
	int	ii = m_StartInx;
	if (ii < m_iDispEPos)
	{
		chkPt.x = GetXPos(m_pVol[ii]);
		chkPt.y = GetYPos(m_pPVData[ii]);
		rc.SetRect(chkPt.x - rgnLINEGAP, chkPt.y - rgnLINEGAP,
			chkPt.x + rgnLINEGAP, chkPt.y + rgnLINEGAP);
		if (rc.PtInRect(pt))
		{
			gBasic = pOrgData->GetGraphData(ii);
			if (gBasic->m_iClose == GD_UNUSED)
			{
				return _T("");
			}

			switch (GetDataIndex())
			{
			case GI_MINUTE:
			case GI_TICK:
				unitStr1.Format("시간\t%02d:%02d:%02d\t", gBasic->m_index.time.hh,
					gBasic->m_index.time.mm, gBasic->m_index.time.ss);
				break;
			default:
				unitStr1.Format("일자\t%04d/%02d/%02d\t", gBasic->m_index.date.yy,
					gBasic->m_index.date.mm, gBasic->m_index.date.dd);
				break;
			}

			return unitStr1;
		}
	}

	ii++;
	CRgn	Rgn;
	CPoint	pPoint[5];
	for (; ii < m_iDispEPos; ii++)
	{		
		chkPt.x = GetXPos(m_pVol[ii-1]);
		chkPt.y = GetYPos(m_pPVData[ii-1]);
		chkPt2.x = GetXPos(m_pVol[ii]);
		chkPt2.y = GetYPos(m_pPVData[ii]);

		pPoint[0].x = chkPt.x;
		pPoint[0].y = chkPt.y + rgnLINEGAP;
		pPoint[1].x = chkPt2.x;
		pPoint[1].y = chkPt2.y + rgnLINEGAP;
		pPoint[2].x = chkPt2.x;
		pPoint[2].y = chkPt2.y - rgnLINEGAP;
		pPoint[3].x = chkPt.x;
		pPoint[3].y = chkPt.y - rgnLINEGAP;
		pPoint[4].x = chkPt.x;
		pPoint[4].y = chkPt.y + rgnLINEGAP;

		Rgn.CreatePolygonRgn(pPoint, 5, WINDING);
		if (Rgn.PtInRegion(pt))
		{
			gBasic = pOrgData->GetGraphData(ii);
			if (gBasic->m_iClose == GD_UNUSED)
			{
				return _T("");
			}

			switch (GetDataIndex())
			{
			case GI_MINUTE:
			case GI_TICK:
				unitStr1.Format("시간\t%02d:%02d:%02d\t", gBasic->m_index.time.hh,
					gBasic->m_index.time.mm, gBasic->m_index.time.ss);
				break;
			default:
				unitStr1.Format("일자\t%04d/%02d/%02d\t", gBasic->m_index.date.yy,
					gBasic->m_index.date.mm, gBasic->m_index.date.dd);
				break;
			}
			Rgn.DeleteObject();
			break;
		}
		Rgn.DeleteObject();
	}

	return unitStr1;
}

CString CVPCV::GetDisplayPosData(CPoint pt, bool bGrid)
{
	CString	unitStr1 = _T("");
	if (bGrid)
		return unitStr1;
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return unitStr1;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return unitStr1;
	
	CGrpBasic* gBasic = NULL;
	CPoint	chkPt, chkPt2;

	CString strOpenP;	// Added By schbang 2006.03.20
	CString strHighP;
	CString strLowP;
	CString strCloseP;
	CString strVolP;
	CRect	rc;
	int	ii = m_StartInx;
	if (ii < m_iDispEPos)
	{
		chkPt.x = GetXPos(m_pVol[ii]);
		chkPt.y = GetYPos(m_pPVData[ii]);
		rc.SetRect(chkPt.x - rgnLINEGAP, chkPt.y - rgnLINEGAP,
			chkPt.x + rgnLINEGAP, chkPt.y + rgnLINEGAP);
		if (rc.PtInRect(pt))
		{
			gBasic = pOrgData->GetGraphData(ii);
			if (gBasic->m_iClose == GD_UNUSED)
			{
				return _T("");
			}
			else
			{
				if (ii - 1 >= 0)	// Added By schbang 2006.03.20
				{
					CGrpBasic* pgbPrev = pOrgData->GetGraphData(ii - 1);
					if (pgbPrev->m_iClose != GD_UNUSED)
					{
						double dPcnt = (double)(gBasic->m_iOpen - pgbPrev->m_iClose) / (double)pgbPrev->m_iClose * 100;
						dPcnt < 0 ? strOpenP.Format("  (%0.2f%%)", dPcnt) : strOpenP.Format("  (+%0.2f%%)", dPcnt);

						dPcnt = (double)(gBasic->m_iHigh - pgbPrev->m_iClose) / (double)pgbPrev->m_iClose * 100;
						dPcnt < 0 ? strHighP.Format("  (%0.2f%%)", dPcnt) : strHighP.Format("  (+%0.2f%%)", dPcnt);

						dPcnt = (double)(gBasic->m_iLow - pgbPrev->m_iClose) / (double)pgbPrev->m_iClose * 100;
						dPcnt < 0 ? strLowP.Format("  (%0.2f%%)", dPcnt) : strLowP.Format("  (+%0.2f%%)", dPcnt);

						dPcnt = (double)(gBasic->m_iClose - pgbPrev->m_iClose) / (double)pgbPrev->m_iClose * 100;
						dPcnt < 0 ? strCloseP.Format("  (%0.2f%%)", dPcnt) : strCloseP.Format("  (+%0.2f%%)", dPcnt);

						dPcnt = (double)(gBasic->m_dVolume - pgbPrev->m_dVolume) / (double)pgbPrev->m_dVolume * 100;
						dPcnt < 0 ? strVolP.Format("  (%0.2f%%)", dPcnt) : strVolP.Format("  (+%0.2f%%)", dPcnt);
					}
				}

				CString	sFpr, sHpr, sLpr, sEpr, sTrn;
				FormatVariableComma(sFpr, gBasic->m_iOpen, gBasic->m_iOpen);
				FormatVariableComma(sHpr, gBasic->m_iHigh, gBasic->m_iHigh);
				FormatVariableComma(sLpr, gBasic->m_iLow, gBasic->m_iLow);
				FormatVariableComma(sEpr, gBasic->m_iClose, gBasic->m_iClose);
				FormatNormalComma(sTrn, gBasic->m_dVolume, true, 0);
				unitStr1.Format("시가\t%s\t고가\t%s\t저가\t%s\t종가\t%s\t거래량\t%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, sTrn + strVolP);
			}
			return unitStr1;
		}
	}

	ii++;
	CRgn	Rgn;
	CPoint	pPoint[5];
	for (; ii < m_iDispEPos; ii++)
	{		
		chkPt.x = GetXPos(m_pVol[ii-1]);
		chkPt.y = GetYPos(m_pPVData[ii-1]);
		chkPt2.x = GetXPos(m_pVol[ii]);
		chkPt2.y = GetYPos(m_pPVData[ii]);

		pPoint[0].x = chkPt.x;
		pPoint[0].y = chkPt.y + rgnLINEGAP;
		pPoint[1].x = chkPt2.x;
		pPoint[1].y = chkPt2.y + rgnLINEGAP;
		pPoint[2].x = chkPt2.x;
		pPoint[2].y = chkPt2.y - rgnLINEGAP;
		pPoint[3].x = chkPt.x;
		pPoint[3].y = chkPt.y - rgnLINEGAP;
		pPoint[4].x = chkPt.x;
		pPoint[4].y = chkPt.y + rgnLINEGAP;

		Rgn.CreatePolygonRgn(pPoint, 5, WINDING);
		if (Rgn.PtInRegion(pt))
		{
			gBasic = pOrgData->GetGraphData(ii);
			if (gBasic->m_iClose == GD_UNUSED)
			{
				return _T("");
			}
			else
			{
				if (ii - 1 >= 0)	// Added By schbang 2006.03.20
				{
					CGrpBasic* pgbPrev = pOrgData->GetGraphData(ii - 1);
					if (pgbPrev->m_iClose != GD_UNUSED)
					{
						if (pgbPrev->m_iClose == 0)
						{
							strOpenP = "(0.00%)";
							strHighP = "(0.00%)";
							strLowP = "(0.00%)";
							strCloseP = "(0.00%)";
						}
						else
						{
							double dPcnt = (double)(gBasic->m_iOpen - pgbPrev->m_iClose) / (double)pgbPrev->m_iClose * 100;
							dPcnt < 0 ? strOpenP.Format(" (%0.2f%%)", dPcnt) : strOpenP.Format(" (+%0.2f%%)", dPcnt);

							dPcnt = (double)(gBasic->m_iHigh - pgbPrev->m_iClose) / (double)pgbPrev->m_iClose * 100;
							dPcnt < 0 ? strHighP.Format(" (%0.2f%%)", dPcnt) : strHighP.Format(" (+%0.2f%%)", dPcnt);

							dPcnt = (double)(gBasic->m_iLow - pgbPrev->m_iClose) / (double)pgbPrev->m_iClose * 100;
							dPcnt < 0 ? strLowP.Format(" (%0.2f%%)", dPcnt) : strLowP.Format(" (+%0.2f%%)", dPcnt);

							dPcnt = (double)(gBasic->m_iClose - pgbPrev->m_iClose) / (double)pgbPrev->m_iClose * 100;
							dPcnt < 0 ? strCloseP.Format(" (%0.2f%%)", dPcnt) : strCloseP.Format(" (+%0.2f%%)", dPcnt);
						}

						if (pgbPrev->m_dVolume  == 0)
						{
							strVolP = "(0.00%)";
						}
						else
						{
							double dPcnt = (double)(gBasic->m_dVolume - pgbPrev->m_dVolume) / (double)pgbPrev->m_dVolume * 100;
							dPcnt < 0 ? strVolP.Format(" (%0.2f%%)", dPcnt) : strVolP.Format(" (+%0.2f%%)", dPcnt);
						}
					}
				}

				CString	sFpr, sHpr, sLpr, sEpr, sTrn;
				FormatVariableComma(sFpr, gBasic->m_iOpen, gBasic->m_iOpen);
				FormatVariableComma(sHpr, gBasic->m_iHigh, gBasic->m_iHigh);
				FormatVariableComma(sLpr, gBasic->m_iLow, gBasic->m_iLow);
				FormatVariableComma(sEpr, gBasic->m_iClose, gBasic->m_iClose);
				FormatNormalComma(sTrn, gBasic->m_dVolume, true, 0);
				unitStr1.Format("시가\t%s\t고가\t%s\t저가\t%s\t종가\t%s\t거래량\t%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, sTrn + strVolP);
			}
			break;
		}
		Rgn.DeleteObject();
	}

	return unitStr1;
}

CString CVPCV::GetXposValue(CPoint pt)
{
	CString	unitStr1 = _T("");

	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)	return unitStr1;
	if (m_dXMin == DBL_MAX || m_dXMax == DBL_MIN)	return unitStr1;

	double	dValue = m_dXMax - m_dXMin;
	dValue = dValue * (double(pt.x - m_rectGrp.left)/double(m_rectGrp.Width())) + m_dXMin;

	FormatPCVComma(unitStr1, dValue, m_dXMax, 10);

	return unitStr1;
}
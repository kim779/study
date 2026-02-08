#include "stdafx.h"
#include "BParabolic.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBParabolic::CBParabolic(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
: CBongBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	class COrgData *pOrgData;
	if (m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
	{
		m_iDataKey = pDataMgr->CreateAppData(pOrgData, m_iDataKey, GetGraphKind(),
			&m_pgraph->cInfo, m_pgraph->wCalOption);
	}
}

CBParabolic::~CBParabolic()
{
	if (m_iDataKey > 0)
		m_pDataMgr->DetachData(m_iDataKey);
}

CString CBParabolic::ExcelData(int idx)
{
	CString	dispStr = _T("");
	CString	tmpS = _T("");
	char*	title[] = {"Parabolic"};
	
	int	nSize = sizeof(title) / sizeof(char*);
	
	if (idx < 0)
	{		
		for ( int ii = 0 ; ii < nSize ; ii++ )
		{
			if (ii != 0)
				dispStr += _T("\t");
			
			dispStr += title[ii];			
		}		
	}
	else
	{
		class CAppData *pAppData;

		if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
			return dispStr;

		for ( int ii = 0 ; ii < nSize ; ii++ )
		{
			if (ii != 0)
				dispStr += _T("\t");

			double	val = pAppData->GetVal(ii, idx);

			if (val == GD_UNUSED)
				tmpS = _T("0");
			else
				FormatVariableComma(tmpS, val, 0, 0);

			dispStr += tmpS;
		}
	}
	return dispStr;	
}

void CBParabolic::DrawGraph(CDC* pDC)
{
	m_RgnInfo.iCount = 0;

	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return;

	if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_PRC)
	{
		m_dYMax = m_pCoDraw->PMax;
		m_dYMin = m_pCoDraw->PMin;
	}

	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
	{
		CString dispText;
		dispText = "Parabolic";
		DrawTickIndex(pDC, dispText, m_pgraph->rgbLabel, true);
		m_RgnInfo.iCount++;
		return;
	}

//	CBongBase::DrawTick(pDC);
	drawChart(pDC);
}



void CBParabolic::drawChart(CDC* pDC)
{
	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return;

	CString	dispText;
	double	*pValue = new double[m_iTtlDtCnt];
	int	nStart = -1;

	for ( int ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		pValue[ii] = pAppData->GetVal(0, ii + m_iDispSPos);

		if (nStart < 0 && pValue[ii] != GD_UNUSED)
			nStart = ii;
	}

	if (nStart < 0)
	{
		delete [] pValue;
		pValue = NULL;
		return;
	}

	int	nPoint = m_iDispEPos - m_iDispSPos - nStart;
	if (nPoint <= 0)
	{
		delete [] pValue;
		pValue = NULL;
		return;
	}
	
	CPoint	*pRgnPoint = new CPoint[nPoint*2];

	COLORREF	LineColor = m_pgraph->cInfo.aline[0].rgbLColor;
	int	style, width;
	style = m_pgraph->cInfo.aline[0].btLStyle;
	width = m_pgraph->cInfo.aline[0].btLWidth;
	
	CPen	*cPen = GetPen(style, width, LineColor);
	CPen	*sPen = pDC->SelectObject(cPen);
	CBrush	*cBrush = GetBrush(LineColor);
	CBrush	*sBrush = pDC->SelectObject(cBrush);

	int DrawHeight = m_rectGrp.Height();
	int DrawWidth  = m_rectGrp.Width();

	double	MaxValue = m_dYMax;
	double	MinValue = m_dYMin;
	if (IsLogChart())
	{
		MaxValue = GetLogData(MaxValue);
		MinValue = GetLogData(MinValue);
	}

	int ValueHeight = int(MaxValue - MinValue);
	if (!ValueHeight) 
		ValueHeight = 1;
	int drawShift = 0;
	drawShift = int((DrawWidth/m_iDispDtCnt) / 2);	// Center

	double	dbTemp;
	CRect	rectPAR;
	int drawUNIT = int(DrawWidth / m_iDispDtCnt) - DISTANCE_GAP;
	if (drawUNIT <= 0)
		drawUNIT = 1;
	double dVal;
	int	ii = 0, jj = 0;

	for ( ii = nStart, jj = 0; ii < m_iDispEPos - m_iDispSPos; ii++, jj++)
	{
		dVal = pValue[ii];

		if (IsLogChart())
			dVal = GetLogData(dVal);

		if (dVal < MinValue || dVal > MaxValue)
			continue;
		rectPAR.left = m_rectGrp.left;
		dbTemp = double(DrawWidth * ii) / double(m_iDispDtCnt);
		rectPAR.left += int(dbTemp);
		rectPAR.right = rectPAR.left + drawUNIT;

		if (IsReverseChart())
		{
			rectPAR.bottom = m_rectGrp.bottom;
			dbTemp = double(DrawHeight) * (MaxValue - dVal);
			dbTemp /= double(ValueHeight);
			rectPAR.bottom -= int(dbTemp);
			rectPAR.bottom += drawUNIT / 2;
			rectPAR.top = rectPAR.bottom - drawUNIT;
		}
		else
		{
			rectPAR.top = m_rectGrp.top;
			dbTemp = double(DrawHeight) * (MaxValue - dVal);
			dbTemp /= double(ValueHeight);
			rectPAR.top += int(dbTemp);
			rectPAR.top -= drawUNIT / 2;
			rectPAR.bottom = rectPAR.top + drawUNIT;
		}
		int chkWidth = 6;	// max size 
		if (rectPAR.Width() > chkWidth*2-1)
		{
			int centerH = rectPAR.left + int((rectPAR.right-rectPAR.left)/2);
			int centerV = rectPAR.top + int((rectPAR.bottom-rectPAR.top)/2);
			rectPAR.left = centerH - chkWidth;
			rectPAR.right = centerH + chkWidth;
			rectPAR.top = centerV - chkWidth;
			rectPAR.bottom = centerV + chkWidth;
		}

		pRgnPoint[jj].x = rectPAR.left;
		pRgnPoint[jj].y = rectPAR.top;
		pRgnPoint[nPoint*2 - jj - 1].x = rectPAR.left;
		if (rectPAR.bottom - rectPAR.top <= rgnLINEGAP)
			pRgnPoint[nPoint*2 - jj - 1].y = rectPAR.bottom + rgnLINEGAP;
		else
			pRgnPoint[nPoint*2 - jj - 1].y = rectPAR.bottom;

		if (drawUNIT >= 2)
			pDC->Ellipse(&rectPAR);
		else
		{
			pDC->MoveTo(rectPAR.left, rectPAR.top);
			pDC->LineTo(rectPAR.right, rectPAR.bottom);
		}
	}

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, nPoint * 2, WINDING);

	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);
		for (int ii = 0; ii < 4; ii++)
		{
			int idx = (m_iDispEPos - m_iDispSPos) * (ii+1)/5;
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

	delete[] pRgnPoint;	pRgnPoint = NULL;
	delete [] pValue;	pValue = NULL;

	dispText.Format("Parabolic[증가단위:%.2f,최대값:%.2f]", m_pgraph->cInfo.awValue[1] / 100.0, m_pgraph->cInfo.awValue[0] / 100.0);
	DrawTickIndex(pDC, dispText, m_pgraph->cInfo.aline[0].rgbLColor, true);

	m_RgnInfo.iCount++;
}

CString CBParabolic::GetDisplayPosData(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData) || pAppData->GetDataCount() <= 0)
		return "";

	int DrawWidth = m_rectGrp.Width();
	int xPosition = pt.x - m_rectGrp.left;

	double szOneDay = double(DrawWidth) / double(m_iDispDtCnt);
	int dataPos = int(double(xPosition) / szOneDay);
	dataPos += m_iDispSPos;

	if (dataPos < 0 || dataPos >= m_iTtlDtCnt)
		return _T("");

	CString	unitStr = _T(""), tmpS, tmpS2;
	double	val = 0.0;
	CString		title;

	title.Format("Par[%.2f,%.2f]", m_pgraph->cInfo.awValue[1]/100.0, m_pgraph->cInfo.awValue[0]/100.0);
	
	val = pAppData->GetVal(0, dataPos);

	if (val == GD_UNUSED)
		return unitStr;

	FormatVariableComma(tmpS, val, val);
	if (bGrid)
		unitStr.Format("%s:%s", title, tmpS);
	else
		unitStr.Format("%s\t%s", title, tmpS);
	return unitStr;
}


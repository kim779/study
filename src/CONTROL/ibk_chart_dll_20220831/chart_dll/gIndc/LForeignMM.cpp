// LForeignMM.cpp: implementation of the CLForeignMM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LForeignMM.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLForeignMM::CLForeignMM(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_strDspUnit = "";
	m_iValidDigit = 0;
	if (m_iUnit == GU_INDEX)
	{
		m_strDspUnit = "천";
	}
	else // if (m_iUnit == GU_CODE)
	{
	}

	switch (m_pgraph->wGKind) 
	{
		case GK_PSNLPBUY:
			m_title = "개인순매수(수량)";
			break;
		case GK_FRGNPBUY:
			m_title = "외국인순매수(수량)";
			break;
		case GK_ORGNPBUY:
			m_title = "기관순매수(수량)";
			break;
	}
}

CLForeignMM::~CLForeignMM()
{

}

CString CLForeignMM::MakeLegend(int nIndex)
{
	return m_title;
}

void CLForeignMM::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	arTitle.Add(m_title);
}

void CLForeignMM::DrawAid(CDC* pDC)
{
	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return;
	m_pgraph->cInfo.aline[2].rgbLColor = 0x01;
	double	*pValue = new double[m_iTtlDtCnt];

	for ( int jj = m_iDispSPos ; jj < m_iDispEPos ; jj++ )
		pValue[jj] = pAppData->GetVal(0, jj);

	int	btLStyle = m_pgraph->cInfo.aline[0].btLStyle;
	int	btLWidth = m_pgraph->cInfo.aline[0].btLWidth;
	int	lcolor = m_pgraph->cInfo.aline[0].rgbLColor;

	DrawOsc(pDC, m_dYMax, m_dYMin, pValue, 0);
	CString dispText = MakeLegend(0);
	DrawTickIndex(pDC, dispText, lcolor, true);
	m_RgnInfo.iCount++;

	delete[] pValue;
}

void CLForeignMM::DrawOsc(CDC *pDC, double Max, double Min, double pValue[], int index)
{	
	int ii = 0;
	for (ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		if (pValue[ii+m_iDispSPos] != GD_UNUSED)
			break;
	}
	
	COLORREF	uColor = m_pgraph->cInfo.aline[0].rgbLColor ^ m_crBG;
	COLORREF	dColor = m_pgraph->cInfo.aline[1].rgbLColor ^ m_crBG;	

	CPen	*uPen = GetPen(PS_SOLID, 1, uColor);
	CPen	*dPen = GetPen(PS_SOLID, 1, dColor);
	CPen	*sPen = pDC->SelectObject(uPen);

	CBrush	*uBrush = GetBrush(uColor);
	CBrush	*dBrush = GetBrush(dColor);
	CBrush	*sBrush = pDC->SelectObject(uBrush);
	int	sROP = pDC->SetROP2(R2_XORPEN);

	int DrawHeight = m_rectGrp.Height();
	int DrawWidth  = m_rectGrp.Width();

	double ValueHeight = Max - Min;
	if (!ValueHeight) ValueHeight = 1;

	int drawShift = 0;
	drawShift = int((DrawWidth/m_iDispDtCnt));	// Center

	int	rgncnt = 0;
	CPoint pt[5];
	double	dVal, dValue;

	CRgn	tmpRGN[2];
	CRect	tmpRECT[2];
	CRect	rectTRN;

	CArray<CPoint, CPoint>	arPoint;

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreateRectRgn(0, 0, 0, 0);	

	for (; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		dVal = pValue[ii + m_iDispSPos];

		rectTRN.left = m_rectGrp.left;
		rectTRN.left += int((DrawWidth * ii) / m_iDispDtCnt+0.5);
		rectTRN.right = m_rectGrp.left;
		rectTRN.right += int((DrawWidth * (ii +1)) / m_iDispDtCnt+0.5);
		rectTRN.right -= DISTANCE_GAP;

		if (dVal > 0)
		{
			rectTRN.bottom = rectTRN.top = m_rectGrp.top;
			dValue = double(DrawHeight) * double(Max - dVal);
			rectTRN.top += int(dValue / ValueHeight);
			dValue = double(DrawHeight) * double(Max - 0);
			rectTRN.bottom += int(dValue / ValueHeight);
			pDC->SelectObject(uPen);
			pDC->SelectObject(uBrush);
		}
		else
		{
			rectTRN.bottom = rectTRN.top = m_rectGrp.top;
			dValue = double(DrawHeight) * double(Max - 0);
			rectTRN.top += int(dValue / ValueHeight);
			dValue = double(DrawHeight) * double(Max - dVal);
			rectTRN.bottom += int(dValue / ValueHeight);
			pDC->SelectObject(dPen);
			pDC->SelectObject(dBrush);
		}

		arPoint.Add(rectTRN.CenterPoint());

		rectTRN.right -= DISTANCE_GAP;
		if (dVal != 0)
		{
			if (rectTRN.Width() > 0)
			{
				CPoint	pt[5];

				pt[0].x = rectTRN.left; pt[0].y = rectTRN.top;
				pt[1].x = rectTRN.right; pt[1].y = rectTRN.top;
				pt[2].x = rectTRN.right; pt[2].y = rectTRN.bottom;
				pt[3].x = rectTRN.left; pt[3].y = rectTRN.bottom;
				pt[4].x = rectTRN.left; pt[4].y = rectTRN.top;
				pDC->Polygon(pt, 5);

				pDC->MoveTo(rectTRN.right-1, rectTRN.top);
				pDC->LineTo(rectTRN.left, rectTRN.top);
				pDC->LineTo(rectTRN.left, rectTRN.bottom);
			}
			else
			{
				pDC->MoveTo(rectTRN.left, rectTRN.top);
				pDC->LineTo(rectTRN.left, rectTRN.bottom);
			}
		}

		tmpRECT[0] = tmpRECT[1] = rectTRN;
		tmpRECT[0].InflateRect(2, 2);
		tmpRGN[0].DeleteObject();
		tmpRGN[0].CreateRectRgnIndirect(tmpRECT[0]);
		tmpRECT[1].DeflateRect(2, 2);
		tmpRGN[1].DeleteObject();
		tmpRGN[1].CreateRectRgnIndirect(tmpRECT[1]);

		tmpRGN[0].CombineRgn(&tmpRGN[0], &tmpRGN[1], RGN_XOR);
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CombineRgn(m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn, &tmpRGN[0], RGN_OR);
		rgncnt++;
	}

	tmpRGN[0].DeleteObject();
	tmpRGN[1].DeleteObject();

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
	pDC->SetROP2(sROP);

	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);
		CPoint	point;
		
		if (arPoint.GetSize() > 0)
		{
			for (int ii = 0; ii < 4 ; ii++)
			{
				int idx = (rgncnt) * (ii+1)/5;
				
				point = arPoint.GetAt(idx);

				CRect	rc;
				rc.SetRect(point.x - rgnMARKGAP, point.y - rgnMARKGAP, 
					point.x + rgnMARKGAP, point.y + rgnMARKGAP);
				pDC->Rectangle(&rc);
			}
		}
		
		pDC->SetROP2(sROP);
		pDC->SelectObject(srgnBrush);
	}
	

	arPoint.RemoveAll();
}

CString CLForeignMM::GetDisplayPosData(CPoint pt, bool bGrid)
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
	CStringArray	arTitle;

	arTitle.RemoveAll();
	MakeTitle(arTitle);	

	int	nCount = pAppData->GetDataCount();
	
	char	split = ':';
	if (!bGrid)	split = '\t';
	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		val = pAppData->GetVal(ii, dataPos);

		if (val == GD_UNUSED)
			continue;

		CIndcBase::FormatVariableComma(tmpS, val, 1, 100);
		tmpS2.Format("%s%c%s", arTitle.GetAt(ii), split, tmpS);

		if (!unitStr.IsEmpty())
			unitStr += "\t" + tmpS2;
		else
			unitStr = tmpS2;
	}

	unitStr += m_strDspUnit;

	return unitStr;
}


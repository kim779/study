#include "stdafx.h"
#include "LMACD.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLMACD::CLMACD(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_iValidDigit = 2;
}

CLMACD::~CLMACD()
{

}

CString CLMACD::MakeLegend(int nIndex)
{
	CString	ret;

	if (m_pgraph->wGKind == GK_MACDOS)
	{
		if (nIndex == 0)
			ret.Format("MACD Osc[%d, %d, %d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1], (int)m_pgraph->cInfo.awValue[2]);
		else
			ret = _T("");
	}
	else
	{
		if (nIndex == 0)
			ret.Format("MACD[%d, %d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1]);
		else if (nIndex == 1)
			ret.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[2]);
		else
			ret = _T("");	
	}

	return ret;
}

void CLMACD::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	if (m_pgraph->wGKind == GK_MACDOS)
	{
		tmpstr.Format("MACD Osc[%d, %d, %d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1], (int)m_pgraph->cInfo.awValue[2]);
		arTitle.Add(tmpstr);
	}
	else
	{
		tmpstr.Format("MACD[%d, %d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1]);
		arTitle.Add(tmpstr);
		tmpstr.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[2]);
		arTitle.Add(tmpstr);
	}
}

void CLMACD::DrawAid(CDC* pDC)
{
	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return;

// 2006.10.11 schbang
if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_MACD)
{
	m_dYMax = m_pCoDraw->dMacdMax;
	m_dYMin = m_pCoDraw->dMacdMin;
}

	CString	dispText;
	double	*pValue = new double[m_iTtlDtCnt];

	int btLStyle = 0;
	int btLWidth = 0;
	int lcolor = 0;

	int	nCount = pAppData->GetDataCount();

	for ( int ii = 0 ; ii < nCount ; ii++)
	{
		for ( int jj = m_iDispSPos ; jj < m_iDispEPos ; jj++ )
			pValue[jj] = pAppData->GetVal(ii, jj);

		btLStyle = m_pgraph->cInfo.aline[ii].btLStyle;
		btLWidth = m_pgraph->cInfo.aline[ii].btLWidth;
		lcolor = m_pgraph->cInfo.aline[ii].rgbLColor;

		if (m_pgraph->wGKind == GK_MACDOS)
			DrawOsc(pDC, m_dYMax, m_dYMin, pValue, ii);
		else
			DrawLine(pDC, m_dYMax, m_dYMin, pValue, btLStyle, btLWidth, lcolor, ii);

		dispText = MakeLegend(ii);
		DrawTickIndex(pDC, dispText, lcolor, true);
		m_RgnInfo.iCount++;
	}

	delete[] pValue;
}

void CLMACD::DrawOsc(CDC *pDC, double Max, double Min, double pValue[], int index)
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

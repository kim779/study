#include "stdafx.h"
#include "C_VolLine1.h"
#include "Grp_Line1.h"

#include "GrpWnd.h"
#include "Grp_Data.h"
#include "float.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CGrp_Line1::CGrp_Line1(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo)
: CGrp_Base(pView, pGrpWnd, pInfo)
{
	CalculateMinMax();
}

CGrp_Line1::~CGrp_Line1()
{

}

void CGrp_Line1::DrawGraph(CDC *pDC)
{
	struct _MinMaxChk *pMinMaxChk;
	if (pMinMaxChk = m_pGrpWnd->GetMinMaxChk(m_rKey))
	{
		if (pMinMaxChk->coMinMax)
		{
			m_Max = pMinMaxChk->Max;
			m_Min = pMinMaxChk->Min;
			if (m_Max == INT_MIN || m_Min == INT_MAX)
				return;
		}
	}

	if (m_Max == INT_MIN || m_Min == INT_MAX)
		return;

	if (m_pGrpWnd->m_bPivot && m_pGrpWnd->m_pJgga > 0)
	{
		if ( m_pGrpWnd->m_pJgga > m_Max)
			m_Max = m_pGrpWnd->m_pJgga;// + m_pGrpWnd->m_pJgga/10.;
		if ( m_pGrpWnd->m_pJgga < m_Min)
			m_Min = m_pGrpWnd->m_pJgga;// - m_pGrpWnd->m_pJgga/10.;
	}

	if (m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].bDraw)
		DrawTickHorizon(pDC, m_Max, m_Min, ctkLEFT);
	if (m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].bDraw)
		DrawTickHorizon(pDC, m_Max, m_Min, ctkRIGHT);

	DrawTickBottom(pDC);

	if (m_pGrpWnd->m_pJgga > 0)
		PDrawLine(pDC);
	else
		DrawLine(pDC);	

	DrawLegend(pDC);
}

void CGrp_Line1::DrawLine(CDC *pDC)
{
	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	CPen* cPen{};
	const COLORREF color = m_tRGB;
	const int lineWidth = 1;
	
	cPen = m_pApp->GetPen(m_pView, PS_SOLID, lineWidth, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	const int DrawHeight = m_DrawRect.Height();
	const int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = m_Max - m_Min;
	if (!ValueHeight) ValueHeight = 1;

	int drawShift = 0;
	drawShift = int((DrawWidth/dispDay) / 2);	// Center

	double epr{};
	
	CPoint pointEPR;
	const CPoint pointSPR = CPoint(-1, -1);

	ZeroMemory(&m_MaxDayIndex, sz_cgIndex);
	ZeroMemory(&m_MinDayIndex, sz_cgIndex);
	m_dispMaxVal = INT_MIN;
	m_dispMinVal = INT_MAX;

	int	bInit = true;
	struct _cgVal2* gVal2{};
	
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gVal2 = (struct _cgVal2 *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
		epr = gVal2->val[0];

		if (epr < m_Min || epr > m_Max)
			continue;

		pointEPR.x = m_DrawRect.left + drawShift;
		pointEPR.x += int((DrawWidth * ii) / dispDay);
		pointEPR.y = m_DrawRect.top;
		pointEPR.y += int((DrawHeight * (m_Max - double(epr))) / ValueHeight);

		if (bInit)
		{
			pDC->MoveTo(pointEPR);
			bInit = false;
		}
		else
			pDC->LineTo(pointEPR);

		if (epr > m_dispMaxVal)
		{
			m_dispMaxVal = epr;
			m_MaxDayPos = pointEPR;
			CopyMemory(&m_MaxDayIndex, &gVal2->index, sz_cgIndex);
		}
		if (epr < m_dispMinVal)
		{
			m_dispMinVal = epr;
			m_MinDayPos = pointEPR;
			CopyMemory(&m_MinDayIndex, &gVal2->index, sz_cgIndex);
		}
	}

	pDC->SelectObject(sPen);
	DrawMinMaxDay(pDC);
}

void CGrp_Line1::PDrawLine(CDC *pDC)
{
	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	if (dispDay < 1)
		return;

	const COLORREF color = m_tRGB;
	
	CPen	*dPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(0,0,255));
	CPen	*uPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(255,0,0));
	CPen	*sPen = pDC->SelectObject(uPen);

	const int DrawHeight = m_DrawRect.Height();
	const int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = m_Max - m_Min;
	if (!ValueHeight) ValueHeight = 1;
	const int drawShift = int((DrawWidth/dispDay) / 2);	// Center

	double epr{};
		
	CPoint ptEPR, ptPRE;

	ZeroMemory(&m_MaxDayIndex, sz_cgIndex);
	ZeroMemory(&m_MinDayIndex, sz_cgIndex);
	m_dispMaxVal = INT_MIN;
	m_dispMinVal = INT_MAX;

	struct _cgVal2* gVal2{};
	int	xPivot{};
	const int	yPivot = m_DrawRect.top +
		int((DrawHeight * (m_Max - double(m_pGrpWnd->m_pJgga))) / ValueHeight);

	const int	bPivot = (m_pGrpWnd->m_pJgga <= m_Max && m_pGrpWnd->m_pJgga >= m_Min)? true:false;
	if (bPivot && m_pGrpWnd->m_bPivot)
		DrawPrePrc(pDC);

	gVal2 = (struct _cgVal2 *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(dispPos);
	ptEPR.x = m_DrawRect.left + drawShift;
	ptEPR.y = m_DrawRect.top;
	ptEPR.y += int((DrawHeight * (m_Max - double(gVal2->val[0]))) / ValueHeight);

	if (yPivot < ptEPR.y)	pDC->SelectObject(dPen);
	pDC->MoveTo(ptEPR);
	ptPRE = ptEPR;

	if (bPivot)
	{
		for (int ii = 1; ii < dispEnd - dispPos; ii++)
		{
			gVal2 = (struct _cgVal2 *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
			epr = gVal2->val[0];

			if (epr < m_Min || epr > m_Max)
				continue;

			ptEPR.x = m_DrawRect.left + drawShift;
			ptEPR.x += int((DrawWidth * ii) / dispDay);
			ptEPR.y = m_DrawRect.top;
			ptEPR.y += int((DrawHeight * (m_Max - double(epr))) / ValueHeight);

			if (yPivot < ptPRE.y && yPivot < ptEPR.y)
			{
				pDC->SelectObject(dPen);
				pDC->LineTo(ptEPR);
			}
			else if (yPivot >= ptPRE.y && yPivot >= ptEPR.y)
			{
				pDC->SelectObject(uPen);
				pDC->LineTo(ptEPR);
			}
			else if (yPivot >= ptPRE.y && yPivot < ptEPR.y)	// up, down
			{
				xPivot = ptPRE.x + (yPivot-ptPRE.y)*(ptEPR.x-ptPRE.x)/(ptEPR.y-ptPRE.y);
				pDC->LineTo(xPivot, yPivot);
				pDC->SelectObject(dPen);
				pDC->LineTo(ptEPR);
			}
			else	// down, up
			{
				xPivot = ptPRE.x + (yPivot-ptPRE.y)*(ptEPR.x-ptPRE.x)/(ptEPR.y-ptPRE.y);
				pDC->LineTo(xPivot, yPivot);
				pDC->SelectObject(uPen);
				pDC->LineTo(ptEPR);
			}

			ptPRE = ptEPR;
		}
	}
	else
	{
		for (int ii = 1; ii < dispEnd - dispPos; ii++)
		{
			gVal2 = (struct _cgVal2 *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
			epr = gVal2->val[0];

			if (epr < m_Min || epr > m_Max)
				continue;

			ptEPR.x = m_DrawRect.left + drawShift;
			ptEPR.x += int((DrawWidth * ii) / dispDay);
			ptEPR.y = m_DrawRect.top;
			ptEPR.y += int((DrawHeight * (m_Max - double(epr))) / ValueHeight);
			pDC->LineTo(ptEPR);
		}
	}

	pDC->SelectObject(sPen);
}

void CGrp_Line1::DrawPrePrc(CDC *pDC)
{
	const double	pjgga = m_pGrpWnd->m_pJgga;

	CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(0, 196, 0));
	CPen	*sPen = pDC->SelectObject(cPen);

	const int y = m_DrawRect.top + int((m_DrawRect.Height() * (m_Max - double(pjgga))) / (m_Max - m_Min));
	pDC->MoveTo(m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].tkRect.right, y);
	pDC->LineTo(m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].tkRect.left, y);

	pDC->SelectObject(sPen);
}

void CGrp_Line1::DrawMinMaxDay(CDC* pDC)
{

}

bool CGrp_Line1::CalculateMinMax()
{
	m_Max = INT_MIN;
	m_Min = INT_MAX;

	if (!m_pGrpWnd->m_pDataInfo[m_dKey] || 
		m_pGrpWnd->m_pDataInfo[m_dKey]->GetDataCount() <= 0)
		return false;

	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	struct _cgVal2* gVal2{};

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gVal2 = (struct _cgVal2 *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
		if (m_Max < gVal2->val[0])	m_Max = gVal2->val[0];
		if (m_Min > gVal2->val[0])	m_Min = gVal2->val[0];
	}

	double gap = m_Max - m_Min;
	if (gap == 0)
	{
		if (m_Max != DBL_MIN)
		{
			gap = m_Max;
			m_Max += gap / 10;
			m_Min -= gap / 10;
		}
	}
	
	return true;
}

bool CGrp_Line1::IsChangeMinMax(bool bShift)
{
	const double	m_sMax = m_Max;
	const double	m_sMin = m_Min;

	if (!CalculateMinMax())
		return false;

	if (m_sMax != m_Max || m_sMin != m_Min)
		return true;

	return false;
}

CString CGrp_Line1::GetDisplayPosData(CPoint pt)
{
	CString	dispStr = _T("");

	if (m_DrawRect.left > pt.x || m_DrawRect.right <= pt.x)
		return dispStr;

	if (!m_pGrpWnd->m_pDataInfo[m_dKey] || 
		m_pGrpWnd->m_pDataInfo[m_dKey]->GetDataCount() <= 0)
		return dispStr;

	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	const int	xPosition = pt.x - m_DrawRect.left;
	const double	szOneDay = double(m_DrawRect.Width()) / double(dispDay);
	int	dataPos = int(double(xPosition) / szOneDay);
	dataPos += dispPos;

	CString	stmp, stmp2, sSign, sSign2;
	const struct _cgVal2	*gVal2 =
		 (struct _cgVal2 *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(dataPos);

	FormatVariableComma(stmp, gVal2->val[0], m_pGrpWnd->m_digit);
	dispStr = m_pGrpWnd->m_title[0] + "\t" + stmp;	
	
	return dispStr;
}

void CGrp_Line1::DrawLegend(CDC* pDC)
{
	if (m_pGrpWnd->m_pFont)
	{
		CFont	*oldfont = pDC->SelectObject(m_pGrpWnd->m_pFont);
		const int	bkMode = pDC->SetBkMode(TRANSPARENT);

		pDC->TextOut(m_RgnRect.left + 5, m_RgnRect.top + 3, m_pGrpWnd->m_title[0]);

		pDC->SelectObject(oldfont);
		pDC->SetBkMode(bkMode);
	}
}

CString CGrp_Line1::GetExcelData(int idx)
{
	CString	dispStr = __T("");

	if (!m_pGrpWnd->m_pDataInfo || 
		m_pGrpWnd->m_pDataInfo[m_dKey]->GetDataCount() <= 0)
		return dispStr;
	
	if (idx < 0)
	{
		dispStr = m_pGrpWnd->m_title[0];
	}
	else
	{
		CString	stmp;
		const struct _cgVal2* gVal2 = (struct _cgVal2 *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(idx);

		//FormatVariableComma(stmp, gVal2->val[0], 0);
		stmp.Format("%f", gVal2->val[0]);
		stmp = Setdouble(stmp);
		dispStr += stmp;
	}

	return dispStr;
}

CString	CGrp_Line1::Setdouble(CString data)
{
	int	datalen{};
	int	pos = -1;
	CString	tmp;

	if (data.IsEmpty())
		return  _T("");

	pos = data.Find(".");

	if (pos != -1)
		data = data.Left(pos);

	datalen = data.GetLength();

	if (datalen > 2)
	{
		tmp  = data.Left(datalen - 2);
		data = data.Right(2);
		tmp = tmp + "." + data;
	}
	else if (datalen == 2)
		tmp = "0." + data;
	else if (datalen == 1)
		tmp = "0.0" + data;
	else if (datalen == 0)
		return _T("");

	return tmp;
}
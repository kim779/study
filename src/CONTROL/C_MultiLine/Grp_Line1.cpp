#include "stdafx.h"
#include "C_MultiLine.h"
#include "Grp_Line1.h"

#include "GrpWnd.h"
#include "Grp_Data.h"
#include "float.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CGrp_Line1::CGrp_Line1(CWnd* pView, class CGrpWnd *pGrpWnd, char *pInfo)
: CGrp_Base(pView, pGrpWnd, pInfo)
{
	m_pView = pView;
	CalculateMinMax();

	m_arColor.Add(RGB(50, 200, 50));
	m_arColor.Add(RGB(255, 0, 0));
	m_arColor.Add(RGB(0, 0, 255));		
	m_arColor.Add(RGB(128, 64, 64));
	m_arColor.Add(RGB(0, 128, 128));
	m_arColor.Add(RGB(255, 0, 255));
	m_arColor.Add(RGB(0, 128, 64));	
	m_arColor.Add(RGB(128, 128, 0));
	m_arColor.Add(RGB(255, 128, 64));	
	m_arColor.Add(RGB(0, 0, 128));
}

CGrp_Line1::~CGrp_Line1()
{
	m_arColor.RemoveAll();
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

	if (m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].bDraw)	DrawTickHorizon(pDC, m_Max, m_Min, ctkLEFT);
	if (m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].bDraw)	DrawTickHorizon(pDC, m_Max, m_Min, ctkRIGHT);
	DrawTickBottom(pDC);
	DrawLine(pDC);	
	DrawLegend(pDC);
}

void CGrp_Line1::DrawLine(CDC *pDC)
{
	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;

	if (dispEnd - dispPos <= 0)
		return;

	CPen*	pen[10];
	COLORREF color = 0;
	int	lcnt = m_pGrpWnd->m_pRgnInfo[m_rKey]->lcnt;
	int	ii = 0, jj = 0;
	int	nMod = m_arColor.GetSize();
	bool	bPivot = m_pGrpWnd->m_bPivot;

	if (lcnt <= 0)
		return;

	int	nStart = 0, nEnd = 0;
	nStart = m_pGrpWnd->m_pRgnInfo[m_rKey]->start;
	nEnd = nStart + lcnt;

	int	nAdd = 0;
	int	nWidth = 1;

	if (m_rKey > 0)
	{
		nAdd = m_pGrpWnd->m_pRgnInfo[m_rKey - 1]->lcnt;
	}

	if (m_pGrpWnd->m_pRgnInfo[m_rKey]->lcnt <= 2)
		nWidth = 1;
	else
		nWidth = 1;


	for ( ii = 0 ; ii < lcnt ; ii++ )
	{
		color = m_arColor.GetAt((ii + nAdd) % nMod);
		pen[ii] = m_pApp->GetPen(m_pView, PS_SOLID, nWidth, color);
	}

	CPen	*sPen = pDC->SelectObject(pen[0]);
	CPen	*dPen = m_pApp->GetPen(m_pView, PS_SOLID, nWidth, RGB(0,0,255));
	CPen	*uPen = m_pApp->GetPen(m_pView, PS_SOLID, nWidth, RGB(255,0,0));
	CPen	*lPen = m_pApp->GetPen(m_pView, PS_SOLID, nWidth, RGB(100,100,100));

	int DrawHeight = m_DrawRect.Height();
	int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = m_Max - m_Min;
	if (!ValueHeight) ValueHeight = 1;

	int drawShift = 0;
	drawShift = int((DrawWidth/dispDay) / 2);	// Center

	double val;	
	CPoint pointEPR, pointPRE;
	struct _multi_line*	mline = NULL;
	double	**value = NULL;
	
	value = new double*[lcnt];

	for ( ii = 0 ; ii < lcnt ; ii++ )
	{
		value[ii] = new double[dispEnd - dispPos];
	}
		
	for ( ii = 0; ii < dispEnd - dispPos; ii++)
	{
		mline = (struct _multi_line*)m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);

		for ( jj = 0 ; jj < lcnt ; jj++ )
		{
			val = mline->data[jj + nStart];						
			value[jj][ii] = val;
		}
	}

	int	xPivot;
	int	yPivot = m_DrawRect.top + int((DrawHeight * (m_Max - 0.0)) / ValueHeight);
	bool	bInit = true;

	if (!bPivot)
	{
		for ( ii = 0 ; ii < lcnt ; ii++ )
		{
			bInit = true;
			for ( jj = 0 ; jj < dispEnd - dispPos ; jj++ )
			{
				val = value[ii][jj];

				if (val == UNUSED_VALUE)
					continue;

				pointEPR.x = m_DrawRect.left + drawShift;			
				pointEPR.x += int((DrawWidth * jj) / dispDay);
				pointEPR.y = m_DrawRect.top;
				pointEPR.y += int((DrawHeight * (m_Max - double(val))) / ValueHeight);

				pDC->SelectObject(pen[ii]);
				if (bInit)
				{
					pDC->MoveTo(pointEPR);
					bInit = false;
				}
				else
				{
					pDC->LineTo(pointEPR);
				}
			}				
		}
	}
	else
	{
		for ( ii = 0 ; ii < lcnt ; ii++ )
		{
			bInit = true;
			for ( jj = 0 ; jj < dispEnd - dispPos ; jj++ )
			{
				val = value[ii][jj];

				if (val == UNUSED_VALUE)
					continue;

				pointEPR.x = m_DrawRect.left + drawShift;			
				pointEPR.x += int((DrawWidth * jj) / dispDay);
				pointEPR.y = m_DrawRect.top;
				pointEPR.y += int((DrawHeight * (m_Max - double(val))) / ValueHeight);

				if (bInit)
				{
					pDC->MoveTo(pointEPR);
					bInit =false;
				}
				else if (yPivot < pointPRE.y && yPivot < pointEPR.y)
				{
					pDC->SelectObject(dPen);
					pDC->LineTo(pointEPR);
				}
				else if (yPivot >= pointPRE.y && yPivot >= pointEPR.y)
				{
					pDC->SelectObject(uPen);
					pDC->LineTo(pointEPR);
				}
				else if (yPivot >= pointPRE.y && yPivot < pointEPR.y)	// up, down
				{
					xPivot = pointPRE.x + (yPivot-pointPRE.y)*(pointEPR.x-pointPRE.x)/(pointEPR.y-pointPRE.y);
					pDC->LineTo(xPivot, yPivot);
					pDC->SelectObject(dPen);
					pDC->LineTo(pointEPR);
				}
				else	// down, up
				{
					xPivot = pointPRE.x + (yPivot-pointPRE.y)*(pointEPR.x-pointPRE.x)/(pointEPR.y-pointPRE.y);
					pDC->LineTo(xPivot, yPivot);
					pDC->SelectObject(uPen);
					pDC->LineTo(pointEPR);
				}

				pointPRE = pointEPR;
			}				
		}
		pDC->SelectObject(lPen);
		xPivot = m_RgnRect.left;
		yPivot = m_DrawRect.top + int((DrawHeight * (m_Max - 0.0)) / ValueHeight);
		pDC->MoveTo(xPivot, yPivot);
		pDC->LineTo(m_RgnRect.right - 1, yPivot);
	}

	pDC->SelectObject(sPen);

	for ( ii = 0 ; ii < lcnt ; ii++ )
	{
		delete [] value[ii];
	}

	delete [] value;
}

bool CGrp_Line1::CalculateMinMax()
{
	m_Max = INT_MIN;
	m_Min = INT_MAX;

	if (!m_pGrpWnd->m_pDataInfo || 
		m_pGrpWnd->m_pDataInfo[m_dKey].GetDataCount() <= 0)
		return false;

	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;

	struct _multi_line*	mline = NULL;
	double	val = 0.0;
	int	lcnt = m_pGrpWnd->m_pRgnInfo[m_rKey]->lcnt;
	int	nStart = 0, nEnd = 0;
	nStart = m_pGrpWnd->m_pRgnInfo[m_rKey]->start;
	nEnd = nStart + lcnt;
	
	for ( int ii = 0 ; ii < dispEnd - dispPos ; ii++ )
	{
		mline = (struct _multi_line*)m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);

	
		for ( int jj = nStart ; jj < nEnd ; jj++ )
		{
			val = mline->data[jj];

			if (val == UNUSED_VALUE)
				continue;

			if (m_Max < val)
				m_Max = val;
			if (m_Min > val)
				m_Min = val;
		}		
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
	double	m_sMax = m_Max;
	double	m_sMin = m_Min;

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

	if (!m_pGrpWnd->m_pDataInfo || 
		m_pGrpWnd->m_pDataInfo->GetDataCount() <= 0)
		return dispStr;

	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;
	int	xPosition = pt.x - m_DrawRect.left;
	double	szOneDay = double(m_DrawRect.Width()) / double(dispDay);
	int	dataPos = int(double(xPosition) / szOneDay);
	dataPos += dispPos;

	CString	sVal, sTitle = _T(""), stmp, szrslt = _T("");
	struct _multi_line*	mline = (struct _multi_line*)m_pGrpWnd->m_pDataInfo->GetGraphData(dataPos);
	int	lcnt = m_pGrpWnd->m_pRgnInfo[m_rKey]->lcnt;
	int	nSize = m_pGrpWnd->m_arTitle.GetSize();
	int	nStart = 0, nEnd = 0;
	nStart = m_pGrpWnd->m_pRgnInfo[m_rKey]->start;
	nEnd = nStart + lcnt;
	double	val = 0;

	for ( int ii = nStart ; ii < nEnd ; ii++ )
	{
		val = mline->data[ii];

		if (m_Max > VOLUNIT)
			val *= 1000;

		if (val == UNUSED_VALUE)
			sVal = _T("N/A");
		else
			FormatVariableComma(sVal, val, m_pGrpWnd->m_pRgnInfo[m_rKey]->digit);

		if (ii < nSize)
			sTitle = m_pGrpWnd->m_arTitle.GetAt(ii);
		else
			sTitle = _T("");

		if (!sTitle.IsEmpty())
			stmp = sTitle + "\t" + sVal;
		else
			stmp = _T("");

		if (ii != nStart)
		{
			if (!stmp.IsEmpty())
				stmp = "\n" + stmp;		
		}

		szrslt += stmp;
	}
		 
	return szrslt;
}

void CGrp_Line1::DrawLegend(CDC* pDC)
{
	CSize		size;
	COLORREF	color = 0;
	CString		title;
	int		nMod = m_arColor.GetSize();
	int		nSize = m_pGrpWnd->m_arTitle.GetSize();
	CPoint		pt;
	int	nStart = 0, nEnd = 0;
	nStart = m_pGrpWnd->m_pRgnInfo[m_rKey]->start;
	nEnd = nStart + m_pGrpWnd->m_pRgnInfo[m_rKey]->lcnt;
	int	nAdd = 0;
	
	if (m_rKey > 0)
	{
		nAdd = m_pGrpWnd->m_pRgnInfo[m_rKey - 1]->lcnt;
	}
	
	if (m_pGrpWnd->m_pFont)
	{
		int		bkMode = pDC->SetBkMode(TRANSPARENT);
		CFont		*oldfont = pDC->SelectObject(m_pGrpWnd->m_pFont);
		COLORREF	sColor = pDC->SetTextColor(RGB(0, 0, 0));

		pt = CPoint(m_RgnRect.left + 5, m_RgnRect.top + 3);

		for ( int ii = 0 ; ii < nSize && ii < nEnd; ii++ )
		{
			if (m_pGrpWnd->m_bPivot)
				color = RGB(0,0,0);
			else
				color = m_arColor.GetAt((ii + nAdd) % nMod);
			pDC->SetTextColor(color);

			if (ii + nStart < nSize)
				title = m_pGrpWnd->m_arTitle.GetAt(ii + nStart);
			else
				title = _T("");
			pDC->TextOut(pt.x, pt.y, title);	
			size = pDC->GetTextExtent(title + "  ");
			pt.x += size.cx;
		}
		
		pDC->SetTextColor(sColor);
		pDC->SelectObject(oldfont);
		pDC->SetBkMode(bkMode);
	}
}

CString CGrp_Line1::GetExcelData(int idx)
{
	CString	dispStr = __T("");

	if (!m_pGrpWnd->m_pDataInfo || 
		m_pGrpWnd->m_pDataInfo->GetDataCount() <= 0)
		return dispStr;
	
	if (idx < 0)
	{
		for (int ii = 0; ii < m_pGrpWnd->m_arTitle.GetSize(); ii++)
		{
			if (ii == 0)
				dispStr = m_pGrpWnd->m_arTitle.GetAt(ii);
			else
				dispStr += _T("\t") + m_pGrpWnd->m_arTitle.GetAt(ii);
		}
	}
	else
	{
		CString	stmp;
		
		struct _multi_line	*multi_line = (struct _multi_line*)m_pGrpWnd->m_pDataInfo->GetGraphData(idx);

		int	lcnt = m_pGrpWnd->m_pRgnInfo[m_rKey]->lcnt;
		for (int ii = 0; ii < lcnt; ii++)
		{
			FormatVariableComma(stmp, multi_line->data[ii], 0);
			if (ii == 0)
				dispStr += stmp;
			else 
				dispStr += _T("\t") + stmp;
		}
	}

	return dispStr;
}
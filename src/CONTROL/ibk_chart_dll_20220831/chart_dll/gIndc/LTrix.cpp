#include "stdafx.h"
#include "LTrix.h"
#include "../gData/DataMgr.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLTrix::CLTrix(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
}

CLTrix::~CLTrix()
{

}

CString CLTrix::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
		ret.Format("TRIX[%d]", (int)m_pgraph->cInfo.awValue[0]);
	else if (nIndex == 1)
		ret.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[1]);
	else
		ret = _T("");

	return ret;
}

void CLTrix::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	tmpstr.Format("TRIX[%d]", (int)m_pgraph->cInfo.awValue[0]);
	arTitle.Add(tmpstr);
	tmpstr.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[1]);
	arTitle.Add(tmpstr);
}

bool CLTrix::CalculateMinMax(bool bForce)
{
	bool	bret = CLineBase::CalculateMinMax(bForce);

	m_iValidDigit = 6;
	return bret;
}

void CLTrix::FormatVariableComma(CString &dispTxt, double dVal, double Max, int maxDigit)
{
	CString	edit1, edit2;
	double dotL = 0, dotR = 0;

	int graphKind = m_pgraph->wGKind;

	bool bSign = false;
	if (dVal < 0)
	{
		bSign = true;
		dVal = dVal * (-1);
	}
/*
	if ((m_dYMax > 0 && m_dYMax < 1000) || (m_YMin < 0 && m_YMin > (-1000)))
	{
		dVal = dVal * 1000.0;
	}
*/
	if (m_iValidDigit)
	{
		dotL = int(dVal / pow(10.0, m_iValidDigit));
		dotR = int(int(dVal) % (int)pow(10.0, m_iValidDigit));
	}
	else
		dotL = dVal;

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

	
	if (m_iValidDigit)
		edit1.Format(".%0*d", m_iValidDigit, int(dotR+0.5));		
	
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

	double	val = atof(dispTxt);
	dispTxt.Format("%g", val);

	if (dispTxt.Find("e", 0) >= 0 || dispTxt.Find("E", 0) >= 0)
	{
		dispTxt.Format("%g", val * 1000.0);
		dispTxt.Replace("0.", "0.000");		
	}
}

void CLTrix::DrawTickLeft(CDC* pDC, double Max, double Min)
{
	if (!(m_pgraph->dwDraOption & GO_TKLEFT))
	{
		TRACE("DrawTickLeft Function Error\n");
		return;
	}

	int	dispCount;
	double	dispValue[64];
	double	logValue[64];
	CRect	tickRC = m_TkInfo.tkRect[TICK_LEFT];
	CRect	orgTickRC = tickRC;
	tickRC.right -= 10;
	orgTickRC.top = m_rectOrgGrp.top;

	int	tickHeight = tickRC.Height();
	double	ValueHeight = Max - Min;
	if (ValueHeight <= 0)	ValueHeight = 1;

	double	div;
	if (ValueHeight < 5)			div = 1;
	else if (ValueHeight < 50)		div = 5;
	else if (ValueHeight < 100)		div = 10;
	else if (ValueHeight < 500)		div = 50;
	else if (ValueHeight < 1000)		div = 100;
	else if (ValueHeight < 5000)		div = 500;
	else if (ValueHeight < 10000)		div = 1000;
	else if (ValueHeight < 50000)		div = 5000;
	else if (ValueHeight < 100000)		div = 10000;
	else if (ValueHeight < 500000)		div = 50000;
	else if (ValueHeight < 1000000)		div = 100000;
	else if (ValueHeight < 5000000)		div = 500000;
	else if (ValueHeight < 10000000)	div = 1000000;
	else if (ValueHeight < 50000000)	div = 5000000;
	else if (ValueHeight < 100000000)	div = 10000000;
	else if (ValueHeight < 500000000)	div = 50000000;
	else if (ValueHeight < 1000000000)	div = 100000000;
	else					div = 500000000;

	int dispTICK = GetTickCount(TICK_LEFT);
	if (dispTICK <= 0)
		return;

	double	drawMinValue= 0.0;
	if (!CalculateDisplay(div, Max, Min, drawMinValue, dispTICK, dispValue, logValue, dispCount))
		return;

	int		sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF	sColor = pDC->SetTextColor(m_atick[TICK_LEFT].dwTColor);
	int		style = m_lineH.btLStyle;
	int		width = m_lineH.btLWidth;
	COLORREF	color = m_lineH.rgbLColor;

	CRect	textRC;
	CFont	*tkFont = GetFont(TICK_LEFT);
	CFont	*sFont = pDC->SelectObject(tkFont);
	CPen	*cPen = GetPen(style, width, color);
	CPen	*lPen = GetPen(PS_SOLID, width, m_crFG);
	CPen	*sPen = pDC->SelectObject(cPen);

	double	dValue;
	CString	dispStr;
	CPoint	pointS, pointE;
	CRect	tkSaveRC = tickRC;

	CSize	size;
	int	maxdigit = 7;
	char	*digitStr[10] = {"2,000,000,000", "200,000,000", "20,000,000", 
		"2,000,000", "200,000", "20,000", "2,000", "200", "20", "2"};
	for (int ii = 9; ii >= 0; ii--)
	{
		size = pDC->GetOutputTextExtent(digitStr[ii]);
		if (size.cx >= tkSaveRC.Width())
			break;
		else
			maxdigit = strlen(digitStr[ii]);
	}

	bool	bDrawLine = (m_dwDspOpt & DO_TKHGRID)? true:false;
	
	tkSaveRC.top = tickRC.bottom;

	int	nPos = 0;
	bool	bUnit = false;

	for ( int ii = 0 ; ii < dispCount ; ii++ )
	{
		FormatVariableComma(dispStr, dispValue[ii], Max, maxdigit);
		nPos = dispStr.Find(".", 0);

		if (nPos < 0)
			continue;
		else
		{
			CString		tmpstr;
			tmpstr = dispStr.Mid(nPos + 1);

			if (tmpstr.GetLength() > 4)
			{				
				bUnit = true;
				break;
			}
		}
	}

	for (int ii = 0; ii < dispCount; ii++)		// 가격별가로 (DOT)
	{
		pointS.x = m_rectOrgGrp.left+1;
		dValue = dispValue[ii];
		dValue = double(double(tickHeight) * double(Max - dValue));
		dValue /= double(ValueHeight);

		pointS.y = tickRC.top + int(dValue);

		if (!tickRC.PtInRect(CPoint(pointS.x -1, pointS.y)))
		{
			if (tickRC.bottom < pointS.y)
				continue;
		}

		pointE.x = m_rectOrgGrp.right;
		pointE.y = pointS.y;
		if (bDrawLine && pointS.y > m_rectChart.top && pointS.y < m_rectChart.bottom)
		{
			pDC->SelectObject(cPen);
			pDC->MoveTo(pointS);
			pDC->LineTo(pointE);
		}
		
		pDC->SelectObject(lPen);
		pDC->MoveTo(m_rectOrgGrp.left, pointS.y);
		pDC->LineTo(m_rectOrgGrp.left - lenTICK, pointS.y);

		if (ii == 0)
		{
			dispStr = _T("");

			if (bUnit)
				dispStr = _T("[1/1000]");

			if (!dispStr.IsEmpty())
			{
			
				size = pDC->GetOutputTextExtent(dispStr);
				textRC.SetRect(tickRC.left, tickRC.bottom - size.cy,
						m_rectOrgGrp.left - lenTICK, tickRC.bottom);
				textRC.IntersectRect(&textRC, &orgTickRC);
				pDC->DrawText(dispStr, textRC, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
				tkSaveRC = textRC;
				continue;
			}		
		}

		FormatVariableComma(dispStr, ((bUnit) ? dispValue[ii] * 1000.0 : dispValue[ii]), Max, maxdigit);

		CString szRep;

		szRep.Format(".%0*d", m_iValidDigit, 0);
		dispStr.Replace(szRep, "");

		size = pDC->GetOutputTextExtent(dispStr);

		textRC.SetRect(tickRC.left, pointS.y - size.cy/2,
			tickRC.right - gapTICK, pointS.y + size.cy/2);

		if (textRC.bottom > tkSaveRC.top)
			textRC.OffsetRect(0, tkSaveRC.top - textRC.bottom);

		textRC.IntersectRect(&textRC, &orgTickRC);
		pDC->DrawText(dispStr, textRC, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		tkSaveRC = textRC;
	}

	m_aiRight[TICK_LEFT] = textRC.right;

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CLTrix::DrawTickRight(CDC* pDC, double Max, double Min)
{
	if (!(m_pgraph->dwDraOption & GO_TKRIGHT))
	{
		TRACE("DrawTickRight Function Error\n");
		return;
	}

	int	dispCount;
	double	dispValue[64];
	double	logValue[64];
	CRect	tickRC = m_TkInfo.tkRect[TICK_RIGHT];
	CRect	orgTickRC = tickRC;
	tickRC.left += 10;
	orgTickRC.top = m_rectOrgGrp.top;

	int	tickHeight = tickRC.Height();
	double	ValueHeight = Max - Min;
	if (ValueHeight <= 0) 	ValueHeight = 1;
	
	double	div;
	if (ValueHeight < 5)			div = 1;
	else if (ValueHeight < 50)		div = 5;
	else if (ValueHeight < 100)		div = 10;
	else if (ValueHeight < 500)		div = 50;
	else if (ValueHeight < 1000)		div = 100;
	else if (ValueHeight < 5000)		div = 500;
	else if (ValueHeight < 10000)		div = 1000;
	else if (ValueHeight < 50000)		div = 5000;
	else if (ValueHeight < 100000)		div = 10000;
	else if (ValueHeight < 500000)		div = 50000;
	else if (ValueHeight < 1000000)		div = 100000;
	else if (ValueHeight < 5000000)		div = 500000;
	else if (ValueHeight < 10000000)	div = 1000000;
	else if (ValueHeight < 50000000)	div = 5000000;
	else if (ValueHeight < 100000000)	div = 10000000;
	else if (ValueHeight < 500000000)	div = 50000000;
	else if (ValueHeight < 1000000000)	div = 100000000;
	else					div = 500000000;

	int	dispTICK = GetTickCount(TICK_RIGHT);
	if (dispTICK <= 0)
		return;

	double	drawMinValue= 0.0;
	double	dVal = 0.0;

	if (!CalculateDisplay(div, Max, Min, drawMinValue, dispTICK, dispValue, logValue, dispCount))
		return;

	int		sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF	sColor = pDC->SetTextColor(m_atick[TICK_RIGHT].dwTColor);
	int		style = m_lineH.btLStyle;
	int		width = m_lineH.btLWidth;
	COLORREF	color = m_lineH.rgbLColor;

	CRect	textRC;
	CFont	*tkFont = GetFont(TICK_RIGHT);
	CFont	*sFont = pDC->SelectObject(tkFont);
	CPen	*cPen = GetPen(style, width, color);
	CPen	*lPen = GetPen(PS_SOLID, width, m_crFG);
	CPen	*sPen = pDC->SelectObject(cPen);

	double	dValue;
	CString	dispStr;
	CPoint	pointS, pointE;
	CRect	tkSaveRC = tickRC;

	CSize	size, size2;
	int	ii = 0, maxdigit = 7;
	char	*digitStr[10] = {"2,000,000,000", "200,000,000", "20,000,000", 
		"2,000,000", "200,000", "20,000", "2,000", "200", "20", "2"};
	for (ii = 9; ii >= 0; ii--)
	{
		size = pDC->GetOutputTextExtent(digitStr[ii]);
		if (size.cx >= tkSaveRC.Width())
			break;
		else
			maxdigit = strlen(digitStr[ii]);
	}

	FormatVariableComma(dispStr, Max, Max, maxdigit);
	size = pDC->GetOutputTextExtent(dispStr);
	
	FormatVariableComma(dispStr, Min, Max, maxdigit);
	size2 = pDC->GetOutputTextExtent(dispStr);

	size.cx = (size.cx > size2.cx) ? size.cx : size2.cx;
	
	bool	bDrawLine = (m_dwDspOpt & DO_TKHGRID)? true:false;

	tkSaveRC.top = tickRC.bottom;

	CArray<CRect, CRect>	arRect;
	CStringArray		arString;
	int	nMaxLen = 0;

	int	nPos = 0;
	bool	bUnit = false;

	for ( ii = 0 ; ii < dispCount ; ii++ )
	{
		FormatVariableComma(dispStr, dispValue[ii], Max, maxdigit);
		nPos = dispStr.Find(".", 0);

		if (nPos < 0)
			continue;
		else
		{
			CString		tmpstr;
			tmpstr = dispStr.Mid(nPos + 1);

			if (tmpstr.GetLength() > 4)
			{
				bUnit = true;
				break;
			}
		}
	}

	for ( ii = 0; ii < dispCount; ii++ )		// 가격별가로 (DOT)
	{
		pointS.x = m_rectOrgGrp.right;
		dValue = dispValue[ii];
		dValue = double(double(tickHeight) * double(Max - dValue));
		dValue /= double(ValueHeight);

		pointS.y = tickRC.top + int(dValue);

		if (!tickRC.PtInRect(pointS))
		{
			if (tickRC.bottom < pointS.y)
				continue;
		}

		pointE.x = m_rectOrgGrp.left+1;
		pointE.y = pointS.y;
		if (bDrawLine && pointS.y > m_rectChart.top && pointS.y < m_rectChart.bottom)
		{
			pDC->SelectObject(cPen);
			pDC->MoveTo(pointE);
			pDC->LineTo(pointS);
		}

		pDC->SelectObject(lPen);
		pDC->MoveTo(m_rectOrgGrp.right, pointS.y);
		pDC->LineTo(m_rectOrgGrp.right + lenTICK, pointS.y);

		if (ii == 0)
		{
			dispStr = _T("");
			if (bUnit) dispStr = _T("[1/1000]");

			if (!dispStr.IsEmpty())
			{
				size = pDC->GetOutputTextExtent(dispStr);
				textRC.SetRect(m_rectOrgGrp.right + lenTICK, tickRC.bottom - size.cy, tickRC.right, tickRC.bottom);
				textRC.IntersectRect(&textRC, &orgTickRC);
				pDC->DrawText(dispStr, &textRC, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOCLIP);
				tkSaveRC = textRC;
				continue;
			}				
		}

		FormatVariableComma(dispStr, ((bUnit) ? dispValue[ii] * 1000.0 : dispValue[ii]), Max, maxdigit);

		CString szRep;
		szRep.Format(".%0*d", m_iValidDigit, 0);
		dispStr.Replace(szRep, "");

		size = pDC->GetOutputTextExtent(dispStr);
			
		textRC.SetRect(tickRC.left + gapTICK, pointS.y - size.cy/2,
			0, pointS.y + size.cy/2);

		if (textRC.bottom > tkSaveRC.top)
			textRC.OffsetRect(0, tkSaveRC.top - textRC.bottom);

		arRect.Add(textRC);
		arString.Add(dispStr);		

		if (size.cx > nMaxLen)
			nMaxLen = size.cx;
		
		tkSaveRC = textRC;
	}

	int nSize = arRect.GetSize();

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		textRC = arRect.GetAt(ii);
		textRC.right = textRC.left + nMaxLen;
		pDC->DrawText(arString[ii], &textRC, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
	}

	m_aiRight[TICK_RIGHT] = textRC.right;

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

CString CLTrix::GetDisplayPosData(CPoint pt, bool bGrid)
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
	int	nPos = 0;
	
	char	split = ':';
	if (!bGrid)	split = '\t';
	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		val = pAppData->GetVal(ii, dataPos);

		if (val == GD_UNUSED)
			continue;

		FormatVariableComma(tmpS, val, m_dYMax, 100);

		if (tmpS.Find(".0", 0) >= 0)
		{
			if (tmpS.Find(".00", 0) >= 0)
			{
				if (tmpS.Find(".000", 0) >= 0)
				{
					if (tmpS.Find(".0000", 0) >= 0)
					{
						
					}
					else
					{
						nPos = tmpS.Find(".", 0);
						tmpS = tmpS.Left(nPos + 6);
					}
				}
				else
				{
					nPos = tmpS.Find(".", 0);
					tmpS = tmpS.Left(nPos + 5);
				}	
			}
			else
			{
				nPos = tmpS.Find(".", 0);
				tmpS = tmpS.Left(nPos + 4);
			}
		}
		else
		{
			nPos = tmpS.Find(".", 0);

			if (nPos >= 0)
				//tmpS = tmpS.Left(nPos + 3);			
				tmpS.Format("%.2f", atof(tmpS));
		}

		tmpS2.Format("%s%c%s", LPCTSTR(arTitle.GetAt(ii)), split, LPCTSTR(tmpS));

		if (!unitStr.IsEmpty())
			unitStr += "\t" + tmpS2;
		else
			unitStr = tmpS2;
	}

	return unitStr;
}

CString CLTrix::ExcelData(int idx)
{
	CString	dispStr = _T("");
	CString	tmpS = _T("");
	CStringArray	arString;

	MakeTitle(arString);

	class CAppData *pAppData;

	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return dispStr;

	int	nCount = pAppData->GetDataCount();

	if (idx < 0)
	{
		for ( int ii = 0 ; ii < nCount ; ii++ )
		{
			if (ii != 0)
				dispStr += _T("\t");
			tmpS = arString.GetAt(ii);
			dispStr += tmpS;			
		}		
	}
	else
	{
		int	nPos = 0;
		
		for ( int ii = 0 ; ii < nCount ; ii++ )
		{
			if (ii != 0)
				dispStr += _T("\t");

			double	val = pAppData->GetVal(ii, idx);			
			if (val == GD_UNUSED)
				tmpS = _T("0");
			else
				FormatVariableComma(tmpS, val, 0, 0);

			if (tmpS.Find(".0", 0) >= 0)
			{
				if (tmpS.Find(".00", 0) >= 0)
				{
					if (tmpS.Find(".000", 0) >= 0)
					{
						if (tmpS.Find(".0000", 0) >= 0)
						{
							
						}
						else
						{
							nPos = tmpS.Find(".", 0);
							tmpS = tmpS.Left(nPos + 6);
						}
					}
					else
					{
						nPos = tmpS.Find(".", 0);
						tmpS = tmpS.Left(nPos + 5);
					}	
				}
				else
				{
					nPos = tmpS.Find(".", 0);
					tmpS = tmpS.Left(nPos + 4);
				}
			}
			else
			{
				nPos = tmpS.Find(".", 0);
				if (nPos >= 0)
					//tmpS = tmpS.Left(nPos + 3);			
					tmpS.Format("%.2f", atof(tmpS));
			}

			if (tmpS.GetAt(tmpS.GetLength() - 1) == '0')
				tmpS = tmpS.Left(tmpS.GetLength() - 1);
			
			dispStr += tmpS;
		}
	}

	return dispStr;
}
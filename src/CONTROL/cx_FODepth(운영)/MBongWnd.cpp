// MBongWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FODepth.h"
#include "MBongWnd.h"
#include "MBongToolTip.h"
#include "MBongLib.h"
#include <float.h>
#include "../../h/memDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TIMER_PID	10000
#define	TIMER_PGAP	250
#define	TIMER_ID	10001
#define	TIMER_GAP	1000

const COLORREF	__MACOLOR__[] = { RGB(255, 0, 0), RGB(128, 128, 128), RGB(0, 0, 255),};

/////////////////////////////////////////////////////////////////////////////
// CMBongWnd

CMBongWnd::CMBongWnd(CWnd *pWizard, CWnd *pParent)
{
	m_pWizard = pWizard;
	m_pParent = pParent;
	m_pFont = nullptr;

	m_bToolTip = true;
	m_nCandle = 10;
	m_arMADay.RemoveAll();
	m_fUnit = 0.0;
	m_fBMax = 0.0;
	m_fBMin = 0.0;
	m_fVMax = 0.0;
	m_fVMin = 0.0;
	m_clrUp   = RGB(255, 0, 0);
	m_clrDown = RGB(0, 0, 255);
	m_nDigit = 0;
	m_dPower = 0.0;
	m_bVolume = false;
	m_bLine = false;
	m_bPrice = false;

	m_tooltip = std::make_unique<CMBongToolTip>(pWizard);
	m_szPlus = _T(""); 
	m_szMinus = _T("");
	m_nTitle = 0;
	m_rcBase.SetRectEmpty();

	m_pApp = (CCX_FODepthApp*)AfxGetApp();
	m_PosData = _T("");
}

CMBongWnd::~CMBongWnd()
{
	for (int ii = 0; ii < m_arData.GetSize(); ii++)
		m_arData.GetAt(ii).reset();
	m_arData.RemoveAll();
}


BEGIN_MESSAGE_MAP(CMBongWnd, CWnd)
	//{{AFX_MSG_MAP(CMBongWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMBongWnd message handlers

int CMBongWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_tooltip->m_bongwnd = this;
	m_tooltip->CreateEx(WS_EX_TOOLWINDOW, TITLETIP_CLASSNAME, NULL, WS_BORDER|WS_POPUP, CRect(0, 0, 100, 100), NULL, 0, NULL);
	m_tooltip->ShowWindow(SW_HIDE);

	Palette();
	m_arMADay.Add(5);
	m_arMADay.Add(20);

	return 0;
}

void CMBongWnd::OnDestroy() 
{
	const	int	nRow = m_MAData.size();

	for (int ii = 0 ; ii < nRow ; ii++)
	{
		m_MAData[ii].reset();
	}
	m_MAData.clear();

	if (m_tooltip)
		m_tooltip.reset();

	CWnd::OnDestroy();
}

BOOL CMBongWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

void CMBongWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	calcuArea(cx, cy);	
}

void CMBongWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (!m_pFont)
		return;

	xxx::CMemDC	mdc(&dc);
	mdc.FillSolidRect(&m_rcBase, RGB(255,255,255));

	if (m_nCandle > 0)	
		calcuMinMax();
	
	drawGraph(&mdc);

	CPen	*cPen = m_pApp->GetPen(m_pWizard, m_clrLine);
	CPen	*sPen = mdc.SelectObject(cPen);

	mdc.MoveTo(m_rcBase.left, m_rcBase.top);
	mdc.LineTo(m_rcBase.left, m_rcBase.bottom);
	mdc.LineTo(m_rcBase.right, m_rcBase.bottom);
	mdc.LineTo(m_rcBase.right, m_rcBase.top);
	mdc.LineTo(m_rcBase.left, m_rcBase.top);

	mdc.SelectObject(sPen);
}

void CMBongWnd::Init(bool bPrice, bool bVolume, bool bLine)
{
	m_bPrice = bPrice;
	m_bVolume = bVolume;
	m_bLine = bLine;
}

void CMBongWnd::Clear()
{
	const	int	nRow = m_MAData.size();

	for (int ii = 0 ; ii < nRow ; ii++)
	{
		m_MAData[ii].reset();
	}
	m_MAData.clear();

	m_arData.RemoveAll();

	m_szPlus = m_szMinus = "";
}

void CMBongWnd::Dispatch(char *pData, CString sUpper, CString sLower, int Digit)
{
	Clear();

	m_nDigit = Digit;
	m_dPower = pow(10.0, m_nDigit);

	m_pApp->VariableComma(m_szPlus, atof(sUpper), m_nDigit);
	m_pApp->VariableComma(m_szMinus, atof(sLower), m_nDigit);

	CString	sTmp;
	struct _EW_data	*ew = (_EW_data *)pData;
	int	count = atoi(CString(ew->nCandle, sizeof(ew->nCandle)));
	std::shared_ptr<CBongData> pBong = nullptr;	

	for (int ii = 0 ; ii < count; ii++)
	{
		sTmp.Format("%.*s", sizeof(ew->candle[ii].date), ew->candle[ii].date);
		sTmp.Trim();

		if (sTmp.GetLength() != sizeof(ew->candle[ii].date))
			break;
		
		pBong = std::make_shared<CBongData>();
		pBong->SetData(&ew->candle[ii]);
		m_arData.Add(pBong);
	}

	calculate();
	Invalidate(FALSE);
}

void CMBongWnd::Alert(CString sRts)
{
	if (m_arData.GetSize() == 0)
		return;

	CString	str = sRts;
	const	int	index = str.Find('\n');
	CString	sym, dat;
	double	siga = 0, koga = 0, jega = 0, jgga = 0, volume = 0;
	std::shared_ptr<CBongData> pData = m_arData.GetAt(0);

	if (index > 0)
		str = str.Left(index);
	while (!str.IsEmpty())
	{
		sym = m_pApp->Parser(str, "\t");
		dat = m_pApp->Parser(str, "\t");
		dat.Trim();
		if (!dat.Compare("0"))
			dat.Empty();

		switch (atoi(sym))
		{
		case 29: //시가
			siga = fabs(atof(dat)) * m_dPower;	break;
		case 30: //고가
			koga = fabs(atof(dat)) * m_dPower;	break;
		case 31: //저가
			jega = fabs(atof(dat)) * m_dPower;	break;
		case 23: //종가
			jgga = fabs(atof(dat)) * m_dPower;	break;
			break;
		case 27: //거래량
			volume = fabs(atof(dat));
			break;
		default:
			continue;
		}
	}

	if (siga == 0.0 || jgga == 0.0)
		return;

	pData->m_siga = siga;
	pData->m_koga = koga;
	pData->m_jega = jega;
	pData->m_jgga = jgga;
	pData->m_gvol = volume;

	for (int ii = 0 ; ii < m_arMADay.GetSize(); ii++)
	{
		m_MAData[ii][0] = calcuMA(m_arMADay.GetAt(ii), 0);
	}
}

void CMBongWnd::Alert(struct _alertR* alertR)
{
	int	nCount = m_arData.GetSize();
	if (nCount == 0)
		return;

	CString	sym, dat;
	double	siga = 0, koga = 0, jega = 0, jgga = 0, volume = 0;
	std::shared_ptr<CBongData> pData = m_arData.GetAt(0);
	DWORD* data = (DWORD*)alertR->ptr[0];
		
	if (data[29])
	{
		dat = (char*)data[29];
		siga = fabs(atof(dat)) * m_dPower;
	}
		
	if (data[30])
	{
		dat = (char*)data[30];
		koga = fabs(atof(dat)) * m_dPower;
	}
		
	if (data[31])
	{
		dat = (char*)data[31];
		jega = fabs(atof(dat)) * m_dPower;
	}
		
	if (data[23])
	{
		dat = (char*)data[23];
		jgga = fabs(atof(dat)) * m_dPower;
	}
		
	if (data[27])
	{
		dat = (char*)data[27];
		volume = fabs(atof(dat));
	}
		
	if (siga == 0.0 || jgga == 0.0)
		return;
		
	pData->m_siga = siga;
	pData->m_koga = koga;
	pData->m_jega = jega;
	pData->m_jgga = jgga;
	pData->m_gvol = volume;
		
	nCount = m_arMADay.GetSize();
	for (int ii = 0 ; ii < nCount ; ii++)
	{
		m_MAData[ii][0] = calcuMA(m_arMADay.GetAt(ii), 0);
	}
	Invalidate();
}

void CMBongWnd::Font(CFont *pFont)
{
	m_pFont = pFont;
}

void CMBongWnd::Palette()
{
	m_clrUp   = m_pApp->GetColor(m_pWizard, 94);
	m_clrDown = m_pApp->GetColor(m_pWizard, 95);
	m_clrLine = m_pApp->GetColor(m_pWizard, 75);
	m_clrTable = m_pApp->GetColor(m_pWizard, 96);
}

void CMBongWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bToolTip)
	{
		CString	str = GetTipStr(point);
		if (m_PosData != str)
		{
			m_tooltip->ShowWindow(SW_HIDE);
			m_PosData.Format("%s", str);
			KillTimer(TIMER_PID);
			SetTimer(TIMER_PID, TIMER_PGAP, NULL);
		}

	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CMBongWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TIMER_PID)
	{
		KillTimer(nIDEvent);

		CPoint	pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		if (m_rcGraph.PtInRect(pt))
		{
			CString str = GetTipStr(pt);

			if (m_PosData == str)
				MouseMove(pt);
		}
	}

	if (nIDEvent == TIMER_ID)
	{	
		if (!IsInner())
		{
			KillTimer(nIDEvent);
			m_tooltip->ShowWindow(SW_HIDE);
		}
		
	}
	
	CWnd::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
CSize CMBongWnd::CalcuWndSize(CString strExam)
{
	CSize	csSize;
	CDC*	pDC = GetDC();
	CFont*	sFont = pDC->SelectObject(m_pFont);

	csSize = pDC->GetTextExtent(strExam);
	pDC->SelectObject(sFont);

	ReleaseDC(pDC);	
	return csSize;
}

void CMBongWnd::calcuArea(int cx, int cy)
{
	int	nWidth = 0;
	CDC*	pDC = GetDC();
	CSize	csize;
	CRect	rect;
	CString	tmpstr;
	const	int	nGap = 1;

	CFont*	sFont = pDC->SelectObject(m_pFont);
	if (cx <= 0 || cy <= 0)
		rect = m_rcBase;
	else	rect.SetRect(0, 0, cx, cy);

	csize = pDC->GetTextExtent("상한 ");
	m_nTitle = csize.cx;
	pDC->SelectObject(sFont);
	ReleaseDC(pDC);	

	m_rcBase = rect;
	if (m_bPrice)
	{
		m_rcPlus = m_rcMinus = m_rcBase;
		rect.top += (csize.cy + 1);
		m_rcPlus.bottom = rect.top;
		rect.bottom -= (csize.cy + 1);
		m_rcMinus.top = rect.bottom;
	}

	rect.DeflateRect(2 * nGap, 2 * nGap, nGap * 2, nGap);
	m_rcGraph = m_rcBong = m_rcVolume = rect;

	int	nHeight = rect.Height();
	if (m_bVolume)
	{
		nHeight = nHeight / 3;
		m_rcBong.bottom = m_rcBong.top + nHeight * 2;
		m_rcVolume.top = m_rcBong.bottom + 2 * nGap;		
	}
	else
		m_rcVolume.SetRectEmpty();

	nWidth = m_rcGraph.Width();

	if (m_nCandle)
		m_fUnit = (double)nWidth / (double)m_nCandle;
	else
		m_fUnit = 0.0;
}


void CMBongWnd::calcuMinMax()
{
	int	nSize = 0; 
	double	tmpMin = INT_MAX, tmpMax = INT_MIN;
	double	tmpVMax = INT_MIN;
	std::shared_ptr<CBongData> pData = nullptr;
	const	int	nCount = m_arData.GetSize();
	
	for (int ii = 0 ; ii < m_nCandle && ii < nCount ; ii++)
	{
		pData = m_arData.GetAt(ii);
		if (pData->m_koga == UNUSED_VALUE)
			continue;

		if (pData->m_koga > tmpMax)
			tmpMax = pData->m_koga;

		if (pData->m_jega < tmpMin)
			tmpMin = pData->m_jega;

		if (pData->m_gvol > tmpVMax)
			tmpVMax = pData->m_gvol;	
	}

	nSize = m_arMADay.GetSize();

	for (int ii = 0 ; ii < nSize ; ii++)
	{
		for (int jj = 0 ; jj < m_nCandle && jj < nCount; jj++)
		{
			if (m_MAData[ii][jj] == UNUSED_VALUE)
				continue;

			if (tmpMax < m_MAData[ii][jj])
				tmpMax = m_MAData[ii][jj];
			if (tmpMin > m_MAData[ii][jj])
				tmpMin = m_MAData[ii][jj];
		}
	}

	m_fBMax = tmpMax;
	m_fBMin = tmpMin;
	m_fVMax = tmpVMax;
	m_fVMin = 0;

	double	gap = m_fBMax - m_fBMin;
	if (gap == 0)
	{
		if (m_fBMax != DBL_MIN)
		{
			gap = m_fBMax;
			m_fBMax += gap / 10;
			m_fBMin -= gap / 10;
		}
	}

	gap = m_fVMax - m_fVMin;
	if (gap == 0)
	{
		if (m_fVMax != DBL_MIN)
		{
			gap = m_fVMax;
			m_fVMax += gap / 10;
			m_fVMin -= gap / 10;
		}
	}
}

void CMBongWnd::drawPlusMinus(CDC* pDC)
{
	CRect	rect;
	bool	bTitle = true;
	CSize	size = CalcuWndSize(m_szPlus + " ");
	const	COLORREF sColor = pDC->SetTextColor(m_clrUp);
	const	int	sMode = pDC->SetBkMode(TRANSPARENT);
	
	if ((m_nTitle + m_rcPlus.left) >= (m_rcPlus.right - size.cx))
		bTitle = false;
	else
	{
		size = CalcuWndSize(m_szMinus + " ");

		if ((m_nTitle + m_rcMinus.left) >= (m_rcMinus.right - size.cx))
			bTitle = false;
	}

	CPen	*cPen = m_pApp->GetPen(m_pWizard, m_clrLine);
	CPen	*sPen = pDC->SelectObject(cPen);

	pDC->FillSolidRect(m_rcPlus, m_clrTable);
	if (m_bLine)
	{
		pDC->MoveTo(m_rcPlus.left, m_rcPlus.bottom);
		pDC->LineTo(m_rcPlus.right, m_rcPlus.bottom);
	}
	
	rect = m_rcPlus;
	rect.OffsetRect(0, 1);
	if (bTitle)
	{
		pDC->DrawText(" 상한", rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
		pDC->DrawText(m_szPlus + " ", rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
	}
	else
		pDC->DrawText(m_szPlus, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	
	

	pDC->FillSolidRect(m_rcMinus, m_clrTable);
	if (m_bLine)
	{
		pDC->MoveTo(m_rcMinus.left, m_rcMinus.top);
		pDC->LineTo(m_rcMinus.right, m_rcMinus.top);
	}
	
	rect = m_rcMinus;
	rect.OffsetRect(0, 1);
	pDC->SetTextColor(m_clrDown);
	if (bTitle)
	{
		pDC->DrawText(" 하한", rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
		pDC->DrawText(m_szMinus + " ", rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
	}
	else
		pDC->DrawText(m_szMinus, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CMBongWnd::drawGraph(CDC* pDC)
{
	CFont	*sFont = pDC->SelectObject(m_pFont);

	if (m_bPrice)
		drawPlusMinus(pDC);
	
	if (m_nCandle > 0)
	{
		drawBong(pDC);
		drawMA(pDC);
	}

	if (m_bVolume)
		drawVolume(pDC);

	pDC->SelectObject(sFont);
}


void CMBongWnd::drawBong(CDC* pDC)
{
	CPen	*upPen = m_pApp->GetPen(m_pWizard, m_clrUp);
	CPen	*dnPen = m_pApp->GetPen(m_pWizard, m_clrDown);
	CPen	*sPen = pDC->SelectObject(upPen);
	CBrush	*upBrush = m_pApp->GetBrush(m_pWizard, m_clrUp);
	CBrush	*dnBrush = m_pApp->GetBrush(m_pWizard, m_clrDown);
	CBrush	*sBrush = pDC->SelectObject(upBrush);

	CPoint	pt[11];
	int	nPos[3]{};
	std::shared_ptr<CBongData> pData  = nullptr;
	std::shared_ptr<CBongData> pPData = nullptr;
	const	int	nCount = m_arData.GetSize();
	
	for (int ii = 0 ; ii < m_nCandle && ii < nCount ; ii++)
	{
		pData = m_arData.GetAt(ii);
		if (pData == nullptr || pData->m_jgga == UNUSED_VALUE)
			continue;

		nPos[0] = getXPosR(ii+1) + 1;
		nPos[1] = getXPosR(ii) - 1;		
		nPos[2] = nPos[0] + nPos[1];

		pt[0] = pt[10] = CPoint(nPos[2] / 2, getYPos(pData->m_koga, m_fBMax, m_fBMin, m_rcBong));
		if (pData->m_jgga > pData->m_siga)
		{
			pDC->SelectObject(upPen);
			pDC->SelectObject(upBrush);
			pt[1] = CPoint(pt[0].x, getYPos(pData->m_jgga, m_fBMax, m_fBMin, m_rcBong));
			pt[4] = CPoint(pt[0].x, getYPos(pData->m_siga, m_fBMax, m_fBMin, m_rcBong));
		}
		else if (pData->m_jgga < pData->m_siga)
		{
			pDC->SelectObject(dnPen);
			pDC->SelectObject(dnBrush);
			pt[4] = CPoint(pt[0].x, getYPos(pData->m_jgga, m_fBMax, m_fBMin, m_rcBong));
			pt[1] = CPoint(pt[0].x, getYPos(pData->m_siga, m_fBMax, m_fBMin, m_rcBong));
		}
		else
		{
			pt[4] = pt[1] = CPoint(pt[0].x, getYPos(pData->m_jgga, m_fBMax, m_fBMin, m_rcBong));
			for (int jj = ii + 1; jj < nCount; jj++)
			{
				pPData = m_arData.GetAt(jj);
				if (pPData->m_jgga == pData->m_jgga)
					continue;

				if (pPData->m_jgga > pData->m_jgga)
				{
					pDC->SelectObject(dnPen);
					pDC->SelectObject(dnBrush);
				}
				else
				{
					pDC->SelectObject(upPen);
					pDC->SelectObject(upBrush);
				}
				break;
			}
		}

		pt[2] = CPoint(nPos[0], pt[1].y);
		pt[3] = CPoint(pt[2].x, pt[4].y);
		pt[5] = CPoint(pt[0].x, getYPos(pData->m_jega, m_fBMax, m_fBMin, m_rcBong));
		pt[6] = pt[4];
		pt[7] = CPoint(nPos[1], pt[4].y);
		pt[8] = CPoint(pt[7].x, pt[1].y);
		pt[9] = pt[1];

		pDC->MoveTo(pt[0]);
		pDC->LineTo(pt[5]);
/*
		CRect	rc(pt[2], pt[7]);
		rc.NormalizeRect();
		DrawGradRect(pDC, rc, sColor, eColor);
		pDC->Polyline(pt, 11);
*/
		pDC->Polygon(pt, 11);
	}

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
}

void CMBongWnd::calculate()
{
	const	int	nCount = m_arData.GetSize();
	const	int	nSize = m_arMADay.GetSize();

	for (int ii = 0; ii < (int)m_MAData.size(); ii++)
	{
		m_MAData[ii].reset();
	}
	m_MAData.clear();

	m_MAData.resize(nSize);
	for (int ii = 0, nMA = 0; ii < nSize; ii++)
	{
		nMA = m_arMADay.GetAt(ii);
		m_MAData[ii] = std::make_unique<double[]>(nCount);
		calcuMA(m_MAData[ii].get(), nMA);
	}
}

void CMBongWnd::calcuMA(double* target, int nDay)
{
	const	int	nCount = m_arData.GetSize();

	for (int ii = 0 ; ii < nCount ; ii++)
	{
		target[ii] = calcuMA(nDay, ii);
	}
}

double CMBongWnd::calcuMA(int nDay, int nIndex)
{
	double	ret = 0, val = 0;
	const	int	nCount = m_arData.GetSize();
	std::shared_ptr<CBongData> pData = nullptr;

	if ((nIndex + nDay) > nCount)
		return UNUSED_VALUE;

	for (int ii = 0 ; ii < nDay ; ii++)
	{
		pData = m_arData.GetAt(ii + nIndex);
		val += pData->m_jgga;
	}

	ret = val / (double) nDay;
	return ret;
}

int CMBongWnd::getDataIndex(CPoint pt)
{
	if (!m_rcGraph.PtInRect(pt))
		return -1;

	return (int)((double)(m_rcGraph.right - pt.x) / m_fUnit);
}

int CMBongWnd::getXPosR(int nIndex)
{
	int	ret = 0;

	if (nIndex >= (m_nCandle))
		ret = m_rcGraph.left;
	else
		ret = (int)((double)m_rcGraph.right - (m_fUnit * (double)nIndex));

	return ret;
}

int CMBongWnd::getXPosL(int nIndex)
{
	double	xPos = getXPosR(nIndex);

	if ((xPos - m_fUnit) < m_rcGraph.left)
		xPos = m_rcGraph.left;
	else
	{
		if (nIndex >= (m_nCandle - 1))
			xPos = m_rcGraph.left;
		else
			xPos -= m_fUnit;
	}
	return (int)xPos;
}

int CMBongWnd::getXPosM(int nIndex)
{
	double	xPos = getXPosL(nIndex);
	const	double  xPos2 = getXPosR(nIndex);
	xPos += ((xPos2 - xPos) / 2.0);
	return (int)xPos;
}

int CMBongWnd::getYPos(double val, double maxValue, double minValue, CRect rect)
{
	double	yPos = 0;
	const	double	fGab = maxValue - val;
	const	double	fHeight = rect.Height();
	const	double	fMinMax = maxValue - minValue;

	if (fMinMax <= 0.0)
		return rect.bottom;

	yPos = fHeight * fGab / fMinMax;
	return (int)((double)rect.top + yPos);
}

void CMBongWnd::drawMA(CDC* pDC)
{
	CPoint	pt;
	double	val = 0;
	int	nIndex = 0;
	CPen*	pen[3]{};
	const	int	nDCount = m_arData.GetSize();
	const	int	nCount = m_arMADay.GetSize();
	const	int	nSaveDC = pDC->SaveDC();
		
	for (int ii = 0 ; ii < nCount ; ii++)
	{
		nIndex = m_arMADay.GetAt(ii);

		switch (nIndex)
		{
		case 5:
			nIndex = 0;
			break;
		case 10:
			nIndex = 1;
			break;
		case 20:
			nIndex = 2;
			break;
		default:
			continue;
		}			
	
		pen[nIndex] = m_pApp->GetPen(m_pWizard, __MACOLOR__[nIndex]);
		pDC->SelectObject(pen[nIndex]);	

		for (int jj = 0 ; jj < m_nCandle && jj < nDCount ; jj++)
		{
			val = m_MAData[ii][jj];

			if (val == UNUSED_VALUE)
				continue;		

			pt.x = getXPosM(jj);
			pt.y = getYPos(val, m_fBMax, m_fBMin, m_rcBong);

			if (jj == 0)
				pDC->MoveTo(pt);
			else
				pDC->LineTo(pt);
		}
	}

	pDC->RestoreDC(nSaveDC);
}

void CMBongWnd::drawVolume(CDC* pDC)
{
	CPoint	pt[5];
	const	int	nCount = m_arData.GetSize();
	std::shared_ptr<CBongData> pData = nullptr;

	CPen	*upPen = m_pApp->GetPen(m_pWizard, m_clrUp);
	CPen	*dnPen = m_pApp->GetPen(m_pWizard, m_clrDown);
	CPen	*sPen = pDC->SelectObject(upPen);
	CBrush	*upBrush = m_pApp->GetBrush(m_pWizard, m_clrUp);
	CBrush	*dnBrush = m_pApp->GetBrush(m_pWizard, m_clrDown);
	CBrush	*sBrush = pDC->SelectObject(upBrush);
	double	val = 0, preVal = 0;

	for (int ii = 0 ; ii < m_nCandle && ii < nCount; ii++)
	{
		pData = m_arData.GetAt(ii);
		val = pData->m_gvol;
		if (ii+1 < nCount)
		{
			pData = m_arData.GetAt(ii+1);
			preVal = pData->m_gvol;
		}
		else
		{
			preVal = UNUSED_VALUE;
		}

		//if (pData == NULL)	continue;

		if (val == UNUSED_VALUE)
			continue;

		if (val >= preVal || preVal == UNUSED_VALUE)
		{
			pDC->SelectObject(upPen);
			pDC->SelectObject(upBrush);
		}
		else
		{
			pDC->SelectObject(dnPen);
			pDC->SelectObject(dnBrush);
		}

		pt[0] = pt[4] = CPoint(getXPosR(ii+1) + 1, getYPos(val, m_fVMax, m_fVMin, m_rcVolume));
		pt[1] = CPoint(pt[0].x, getYPos(0, m_fVMax, m_fVMin, m_rcVolume));
		pt[2] = CPoint(getXPosR(ii)-1, pt[1].y);
		pt[3] = CPoint(pt[2].x, pt[0].y);
		pt[4] = pt[0];

		pDC->Polygon(pt, 5);
		//CRect	rc(pt[0], pt[2]);
		//DrawGradRect(pDC, rc, RGB(0, 220, 0), RGB(0, 0, 0));
	}	

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
}

void CMBongWnd::makeString(CBongData* pData, int nIndex, CStringArray& arString)
{
	CString	tmpstr;
	CString	tmpstr2;
	const	int	nCount = m_arMADay.GetSize();

	// 일자
	tmpstr2 = pData->m_date;
	tmpstr.Format("일자:%.4s/%.2s/%.2s", tmpstr2, tmpstr2.Mid(4, 2), tmpstr2.Right(2));
	arString.Add(tmpstr);

	// 시가

	if (pData->m_siga == UNUSED_VALUE)
		tmpstr = _T("N/A");
	else
		m_pApp->VariableComma(tmpstr, pData->m_siga, m_nDigit);
	tmpstr = "시가:" + tmpstr;
	arString.Add(tmpstr);

	// 고가
	if (pData->m_koga == UNUSED_VALUE)
		tmpstr = _T("N/A");
	else		
		m_pApp->VariableComma(tmpstr, pData->m_koga, m_nDigit);
	tmpstr = "고가:" + tmpstr;
	arString.Add(tmpstr);

	// 저가
	if (pData->m_jega == UNUSED_VALUE)
		tmpstr = _T("N/A");
	else		
		m_pApp->VariableComma(tmpstr, pData->m_jega, m_nDigit);
	tmpstr = "저가:" + tmpstr;
	arString.Add(tmpstr);

	// 종가
	if (pData->m_jgga == UNUSED_VALUE)
		tmpstr = _T("N/A");
	else		
		m_pApp->VariableComma(tmpstr, pData->m_jgga, m_nDigit);
	tmpstr = "종가:" + tmpstr;
	arString.Add(tmpstr);	

	for (int ii = 0, nMA = 0; ii < nCount ; ii++)
	{
		nMA = m_arMADay.GetAt(ii);
		if (m_MAData[ii][nIndex] == UNUSED_VALUE)
			tmpstr2 = _T("N/A");
		else
			m_pApp->VariableComma(tmpstr2, m_MAData[ii][nIndex], m_nDigit);
		tmpstr.Format("MA%d:%s", nMA, tmpstr2);
		arString.Add(tmpstr);
	}

	if (m_bVolume)
	{
		// 거래량
		if (pData->m_gvol == UNUSED_VALUE)
			tmpstr = _T("N/A");
		else
			m_pApp->VariableComma(tmpstr, pData->m_gvol, 0);
		tmpstr = "거래량:" + tmpstr;
		arString.Add(tmpstr);
	}
}

BOOL CMBongWnd::IsInner()
{
	POINT	pt;
	CRect	rect;

	::GetCursorPos(&pt);
	GetWindowRect(rect);

	CWnd* pWnd = CWnd::WindowFromPoint(pt);
	if (pWnd != this)
		return FALSE;

	return (rect.PtInRect(pt));
}

void CMBongWnd::DrawGradRect(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight)
{
	CRect	rect;

	drawRC.right  += 1;
	drawRC.bottom += 1;
	const	int	nWidth = drawRC.Width(); 
	const	int	nHeight = drawRC.Height();
	const	int	rs = GetRValue(sColor);
	const	int	gs = GetGValue(sColor);
	const	int	bs = GetBValue(sColor);
	const	int	re = GetRValue(eColor);
	const	int	ge = GetGValue(eColor);
	const	int	be = GetBValue(eColor);
	float	rStep = 0, gStep = 0, bStep = 0;
	CPen*	sPen = nullptr;

	if (bRight)
	{
		if (!nWidth)
		{
			CPen*	cPen = m_pApp->GetPen(m_pWizard, sColor);
			sPen = pDC->SelectObject(cPen);			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);			
			return;
		}

		rStep = float(rs - re)/float(nWidth);
		gStep = float(gs - ge)/float(nWidth);
		bStep = float(bs - be)/float(nWidth);

		if (!nHeight)
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				CPen*	cPen = m_pApp->GetPen(m_pWizard, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(cPen);

				pDC->MoveTo(drawRC.left + ii, drawRC.top);
				pDC->LineTo(drawRC.left + ii + 1, drawRC.top);
				pDC->SelectObject(sPen);				
			}
		}
		else
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				rect.SetRect(drawRC.left + ii, drawRC.top, drawRC.left + ii + 1, drawRC.bottom);
				
				CBrush* brush = m_pApp->GetBrush(m_pWizard, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, brush);
			}
		}
	}
	else
	{
		if (!nHeight)
		{
			CPen*	cPen = m_pApp->GetPen(m_pWizard, sColor);
			sPen = pDC->SelectObject(cPen);
			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nHeight);
		gStep = float(gs - ge)/float(nHeight);
		bStep = float(bs - be)/float(nHeight);

		if (!nWidth)
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				CPen*	cPen = m_pApp->GetPen(m_pWizard, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(cPen);

				pDC->MoveTo(drawRC.left, drawRC.top + ii);
				pDC->LineTo(drawRC.left, drawRC.top + ii + 1);
				pDC->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				rect.SetRect(drawRC.left, drawRC.top + ii, drawRC.right, drawRC.top + ii + 1);
				
				CBrush* brush = m_pApp->GetBrush(m_pWizard, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, brush);
			}
		}
	}
}

CString CMBongWnd::GetTipStr(CPoint pt)
{
	CString	retStr;
	const	int	nIndex = getDataIndex(pt);

	m_arString.RemoveAll();
	if (nIndex < 0)
	{
		m_tooltip->ShowWindow(SW_HIDE);	
	}
	else
	{
		const	int	nCount = m_arData.GetSize();
		
		if (nIndex >= nCount)
			return retStr;

		std::shared_ptr<CBongData> pData = m_arData.GetAt(nIndex);	
		makeString(pData.get(), nIndex, m_arString);
		if (m_arString.GetSize() > 0)
			retStr = m_arString.GetAt(0);
	}

	return retStr;
}

void CMBongWnd::MouseMove(CPoint pt)
{
	if (!m_tooltip)
		return;

	if (!m_arData.GetSize())
	{
		m_tooltip->ShowWindow(SW_HIDE);
		return;
	}

	if (!m_arString.GetSize())
	{
		m_tooltip->ShowWindow(SW_HIDE);
		return;
	}

	if (m_tooltip->IsWindowVisible())
		return;

	CRect	rect = m_tooltip->SetData(m_arString);
	
	pt.x += 10;
	pt.y += 10;

	ClientToScreen(&pt);

	const	int	nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	if ((nWidth - pt.x) < rect.Width())
		pt.x -= rect.Width();

	CRect	rcTmp;
	rcTmp.left = pt.x;
	rcTmp.top = pt.y;
	rcTmp.right = rcTmp.left + rect.Width();
	rcTmp.bottom = rcTmp.top + rect.Height();
	rect = rcTmp;
	rect.bottom += 2;
	m_tooltip->SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);
	
	if (m_tooltip->IsWindowVisible())
		m_tooltip->Invalidate(FALSE);
	else
		m_tooltip->ShowWindow(SW_SHOWNOACTIVATE);
	
	KillTimer(TIMER_ID);
	SetTimer(TIMER_ID, TIMER_GAP, NULL);
}

COLORREF CMBongWnd::GetGradColor(COLORREF color, bool bDark)
{
	const	int	rColor = GetRValue(color);
	const	int	gColor = GetGValue(color);
	const	int	bColor = GetBValue(color);
	const	int	rRate  = rColor / 2;
	const	int	gRate  = gColor / 2;
	const	int	bRate  = bColor / 2;

	if (bDark)
	{
		const	int	r = (rColor - rRate < 0)? 0:rColor - rRate;
		const	int	g = (gColor - gRate < 0)? 0:gColor - gRate;
		const	int	b = (bColor - bRate < 0)? 0:bColor - bRate;
		return RGB(r, g, b);
	}

	const	int	r = (rColor + rRate > 255)? 255:rColor + rRate;
	const	int	g = (gColor + gRate > 255)? 255:gColor + gRate;
	const	int	b = (bColor + bRate > 255)? 255:bColor + bRate;
	return RGB(r, g, b);
}
void CMBongWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CString	strPrice;

	if (m_bPrice)
	{
		if (m_rcPlus.PtInRect(point))
			strPrice = m_szPlus;
		else if (m_rcMinus.PtInRect(point))
			strPrice = m_szMinus;
	}
	if (!strPrice.IsEmpty())
		GetParent()->SendMessage(WM_MBONG, 0, (LPARAM)strPrice.GetString());
	
	CWnd::OnLButtonDown(nFlags, point);
}

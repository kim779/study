#include "stdafx.h"
#include "C_MBong.h"
#include "GraphWnd.h"
#include <float.h>
#include "AxToolTipWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TIMER_PID	10000
#define	TIMER_PGAP	250
#define	TIMER_ID	10001
#define	TIMER_GAP	1000

#define	COLOR_UP	RGB(255, 000, 000)
#define	COLOR_DOWN	RGB(000, 000, 255)

void CGraphWnd::OnFinalRelease()
{
	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CGraphWnd, CWnd)
	//{{AFX_MSG_MAP(CGraphWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_MESSAGE(WM_USER, OnUser)
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CGraphWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CGraphWnd)
	DISP_PROPERTY_NOTIFY(CGraphWnd, "visible", m_visible, OnVisibleChanged, VT_BOOL)
	DISP_FUNCTION(CGraphWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CGraphWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IGraphWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {0403AEEA-A549-471F-9025-ED57C13392DA}
static const IID IID_IGraphWnd =
{ 0x403aeea, 0xa549, 0x471f, { 0x90, 0x25, 0xed, 0x57, 0xc1, 0x33, 0x92, 0xda } };

BEGIN_INTERFACE_MAP(CGraphWnd, CWnd)
	INTERFACE_PART(CGraphWnd, IID_IGraphWnd, Dispatch)
END_INTERFACE_MAP()

const COLORREF	__MACOLOR__[] = { 
					RGB(165, 111, 227),
					RGB(84, 164,   5),
					RGB(128, 128, 128)
				};


#define	R_SIGA	"029"	// 시가
#define	R_KOGA	"030"	// 고가
#define	R_JEGA	"031"	// 저가
#define	R_CURR	"023"	// 현재가
#define	R_GVOL	"027"	// 거래량

#define	ER_SIGA	"029"	// 시가
#define	ER_KOGA	"030"	// 고가
#define	ER_JEGA	"031"	// 저가
#define	ER_CURR	"023"	// 현재가
#define	ER_GVOL	"027"	// 거래량

#define TYPE_JONGMOK	1	// 종목
#define TYPE_JOB	2	// 업종
#define TYPE_SUNMUL	3	// 선물
#define TYPE_OPTION	4	// 옵션
#define TYPE_ECN	5	// ECN
#define TYPE_EOPTION	6	// 주식옵션
#define TYPE_KOFEX	7	// KOFEX

CGraphWnd::CGraphWnd(CWnd*  pView, struct _param* cInfo)
{
	EnableAutomation();
	
	m_pView =  pView;
	m_pBitmap = nullptr;	
	m_bToolTip = true;
	m_nCandle = 0;
	m_nDataType = 0;
	m_arMADay.RemoveAll();
	m_fUnit = 0.0;
	m_fBMax = 0.0;
	m_fBMin = 0.0;
	m_fVMax = 0.0;
	m_fVMin = 0.0;
	m_clrUp   = COLOR_UP;
	m_clrDown = COLOR_DOWN;
	m_nDataSize = sizeof(struct jbong30);
	m_MAData = nullptr;
	m_nDigit = 0;	
	m_nGab = 0;
	m_bVolume = false;
	m_bTimer = false;
	m_bLine = false;
	m_bPrice = false;
	m_tooltip = new CAxToolTipWnd(pView);
	m_nIndex = -1;
	m_nFontSize = cInfo->point;
	m_szFontName.Format("%s", cInfo->fonts);	
	m_szPlus = _T(""); 
	m_szMinus = _T("");
	m_nTitle = 0;
	m_rcBase.SetRectEmpty();
	parseParam((char*)cInfo->options.operator LPCTSTR());
	m_pApp = (CC_MBongApp*)AfxGetApp();

	m_PosData = _T("");
}

CGraphWnd::~CGraphWnd()
{
	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = nullptr;
	}

	m_arData.RemoveAll();
}

void CGraphWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	drawBitmap(&dc);
	loadBitmap(&dc);
}

CSize CGraphWnd::CalcuWndSize(CString strExam)
{
	CSize	csSize;
	CFont* font{}, * oldfont{};
	CDC*	pDC = GetDC();

	font = m_pApp->GetFont(m_pView, m_nFontSize, m_szFontName);
	oldfont = pDC->SelectObject(font);
	csSize = pDC->GetTextExtent(strExam);
	pDC->SelectObject(oldfont);
	ReleaseDC(pDC);	
	return csSize;
}

void CGraphWnd::drawBitmap(CDC* pDC)
{
	CDC		memDC;
	bool		bNew = false;	
	CBitmap* oldBitmap{};
	CRect		rect;
	CFont* font{}, * oldfont{};
	
	rect = m_rcBase;
	
	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = nullptr;
	}

	if (pDC == nullptr)
	{
		bNew = true;
		pDC = GetDC();
	}

	m_pBitmap = new CBitmap;
	memDC.CreateCompatibleDC(pDC);
	
	m_pBitmap->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	oldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);

	font = m_pApp->GetFont(m_pView, m_nFontSize, m_szFontName);
	oldfont = memDC.SelectObject(font);
	
	if (m_nCandle > 0)
	{
		calcuMinMax();
		drawGraph(&memDC);
	}
	
	memDC.SelectObject(oldfont);
	memDC.SelectObject(oldBitmap);	
	memDC.DeleteDC();

	if (bNew)
		ReleaseDC(pDC);
}

void CGraphWnd::loadBitmap(CDC *pDC)
{
	if (m_pBitmap)
	{
		CDC		memDC;
		CRect		rect;
		CBitmap*	oldBitmap = nullptr;
		bool		bNew = false;

		if (pDC == nullptr)
		{
			bNew = true;
			pDC = GetDC();
		}
		
		rect = m_rcBase;
		memDC.CreateCompatibleDC(pDC);
		oldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(oldBitmap);
		memDC.DeleteDC();

		if (bNew)
			ReleaseDC(pDC);
	}
}

void CGraphWnd::calcuArea(int cx, int cy)
{
	int	nHeight = GetSystemMetrics(SM_CXVSCROLL);
	int	nWidth = 0;
	const int nGab = nHeight/4;		
	CDC*	pDC = GetDC();
	CFont* font{}, * oldfont = nullptr;
	CSize	csize;
	CRect	rect;
	CString	tmpstr;

	font = m_pApp->GetFont(m_pView, m_nFontSize, m_szFontName);
	oldfont = pDC->SelectObject(font);
	
	if (cx <= 0 || cy <= 0)
		rect = m_rcBase;
	else
		rect.SetRect(0, 0, cx, cy);

	csize = pDC->GetTextExtent("상한 ");
	m_nTitle = csize.cx;
	pDC->SelectObject(oldfont);
	ReleaseDC(pDC);	
	m_rcBase = rect;
	m_nGab = nGab;	

	if (m_bPrice)
	{
		m_rcPlus = m_rcMinus = m_rcBase;
		rect.top += (csize.cy + csize.cy / 2);		
		m_rcPlus.bottom = rect.top;
		rect.bottom -= (csize.cy + csize.cy / 2);
		m_rcMinus.top = rect.bottom;
	}

	nHeight = (m_nGab);
	rect.DeflateRect(2 * nHeight, 2 * nHeight, nHeight * 2, nHeight * 2);
	m_rcGraph = m_rcBong = m_rcVolume = rect;
	nHeight = rect.Height();

	if (m_bVolume)
	{
		nHeight = nHeight / 3;
		m_rcBong.bottom = m_rcBong.top + nHeight * 2;
		m_rcVolume.top = m_rcBong.bottom + 2 * nGab;		
	}
	else
		m_rcVolume.SetRectEmpty();

	nWidth = m_rcGraph.Width();

	if (m_nCandle)
		m_fUnit = (double)nWidth / (double)m_nCandle;
	else
		m_fUnit = 0.0;
	
}

int compare(const void* input1, const void* input2)
{
	const int a1 = *((int*)input1);
	const int a2 = *((int*)input2);
	
	if (a1 > a2)
		return 1;
	else if (a1 == a2)
		return 0;
	
	return -1;
}

void CGraphWnd::parseParam(char* ptr)
{
	m_nIndex = -1;
	CString	strOrg, strTmp, strToken;

	int		nPos = 0;
	int		nLen = 0;
	int		nSize = 0;
	int		ii = 0;

	strOrg.Format("%s", ptr);
	strOrg.TrimLeft();
	strOrg.TrimRight();
	strOrg.Replace(" ", "");

// value option check			
	{
		const char* const	cToken[] = {"/b", "/u"};
		int*	nValue[] = {&m_nCandle, &m_nDataType};
		int	nDef[] = {10, 1};
	
		nSize = sizeof(cToken) / sizeof(char*);

		for ( ii = 0 ; ii < nSize ; ii++ )
		{
			strToken = cToken[ii];
			nPos = strOrg.Find(strToken, 0);

			if (nPos >= 0)
			{
				strTmp = strOrg.Mid(nPos + strToken.GetLength());
				*nValue[ii] = atoi(strTmp);		
			}
			else 
				*nValue[ii] = nDef[ii];
		}		
	}

// boolen option check
	{
		bool*	bWhat[] = {&m_bVolume, &m_bLine, &m_bToolTip, &m_bPrice};
		const char* const cToken[] = {"/v", "/l", "/t", "/p"};

		nSize = sizeof(cToken) / sizeof(char*);

		for ( ii = 0 ; ii < nSize ; ii++ )
		{
			nPos = strOrg.Find(cToken[ii], 0);

			if (nPos >= 0)
				*bWhat[ii] = true;
			else 
				*bWhat[ii] = false;
		}
	}

// muliple usage option check

	// 이평
	strToken = "/m";
	strTmp = strOrg;
	nLen = strToken.GetLength();

	nSize = m_arMADay.GetSize();
	int	nVal = 0;

	if (m_MAData)
	{
		for ( ii = 0 ; ii < nSize ; ii++ )
		{
			delete [] m_MAData[ii];
		}

		delete [] m_MAData;
		m_MAData = nullptr;
	}


	m_arMADay.RemoveAll();

	while (1)
	{
		if (strTmp.IsEmpty())
			break;
		
		nPos = strTmp.Find(strToken, 0);
		
		if (nPos < 0)
			break;
		
		strTmp = strTmp.Mid(nPos + nLen);
		nVal = atoi(strTmp);	

		if (nVal > 0)
			m_arMADay.Add(nVal);
	}

	nSize = m_arMADay.GetSize();
	int*		pMA = nullptr;

	pMA = new int[nSize]();

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pMA[ii] = m_arMADay.GetAt(ii);		
	}
	
	qsort((void*)pMA, nSize, sizeof(int), compare);

	m_arMADay.RemoveAll();

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		m_arMADay.Add(pMA[ii]);
	}

	delete [] pMA;
	pMA = nullptr;	
}

void CGraphWnd::calcuMinMax()
{
	int		ii = 0, jj = 0, nSize = 0; 
	CBongData*	pData = nullptr;
	double		tmpMin = INT_MAX, tmpMax = INT_MIN, tmpVMax = INT_MIN;
	const double	tmpVMin = INT_MAX;
	const int 	nCount = m_arData.GetSize();	
	
	for ( ii = 0 ; ii < m_nCandle && ii < nCount ; ii++ )
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

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		for ( jj = 0 ; jj < m_nCandle && jj < nCount; jj++ )
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

	double gap = m_fBMax - m_fBMin;
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

int CGraphWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	
		
	m_tooltip->m_bongwnd = this;
	m_tooltip->CreateEx(WS_EX_TOOLWINDOW, TITLETIP_CLASSNAME, nullptr, WS_BORDER|WS_POPUP, CRect(0, 0, 100, 100), nullptr, 0, nullptr);
	m_tooltip->ShowWindow(SW_HIDE);

	updatePalette();
	return 0;
}

void CGraphWnd::drawPlusMinus(CDC* pDC)
{
	bool		bTitle = true;	
	CSize		size = CalcuWndSize(m_szPlus + " ");
	const UINT		dtOpt = 0;
	CRect		rect;
	
	const COLORREF	sColor = pDC->SetTextColor(m_clrUp);
	const  int	sMode = pDC->SetBkMode(TRANSPARENT);
	CFont* font{}, * sfont{};
	
	font = m_pApp->GetFont(m_pView, m_nFontSize, m_szFontName);
	sfont = pDC->SelectObject(font);

	if ((m_nTitle + m_rcPlus.left) >= (m_rcPlus.right - size.cx))
		bTitle = false;
	else
	{
		size = CalcuWndSize(m_szMinus + " ");

		if ((m_nTitle + m_rcMinus.left) >= (m_rcMinus.right - size.cx))
			bTitle = false;
	}

	if (!m_bLine)
		pDC->FillSolidRect(m_rcPlus, RGB(235, 240, 240));
	else
	{
		CPen*	pen;

		pen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(135, 140, 140));
		CPen	*oldpen = pDC->SelectObject(pen);

		CBrush*	brush;
		brush = m_pApp->GetBrush(m_pView, RGB(235, 240, 240));
		CBrush	*oldbrush = pDC->SelectObject(brush);
		pDC->Rectangle(m_rcPlus);
		pDC->SelectObject(oldbrush);
		pDC->SelectObject(oldpen);		
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
	
	

	if (!m_bLine)
		pDC->FillSolidRect(m_rcMinus, RGB(235, 240, 240));
	else
	{
		CPen	*pen;

		pen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(135, 140, 140));
		CPen	*oldpen = pDC->SelectObject(pen);
		CBrush*	brush;
		brush = m_pApp->GetBrush(m_pView, RGB(235, 240, 240));
		CBrush	*oldbrush = pDC->SelectObject(brush);
		pDC->Rectangle(m_rcMinus);
		pDC->SelectObject(oldbrush);
		pDC->SelectObject(oldpen);		
	}
	
	pDC->SetTextColor(m_clrDown);

	rect = m_rcMinus;
	rect.OffsetRect(0, 1);

	if (bTitle)
	{
		pDC->DrawText(" 하한", rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
		pDC->DrawText(m_szMinus + " ", rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
	}
	else
		pDC->DrawText(m_szMinus, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	
	pDC->SelectObject(sfont);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CGraphWnd::drawGraph(CDC* pDC)
{

	if (!m_bLine)
		pDC->FillSolidRect(m_rcBase, RGB(255, 255, 255));
	else
	{
		if (m_arData.GetSize())
		{
			CPen	*pen;
			pen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(135, 140, 140));
			CPen	*oldpen = pDC->SelectObject(pen);
			CBrush*	brush;
			brush = m_pApp->GetBrush(m_pView, RGB(255, 255, 255));
			CBrush	*oldbrush = pDC->SelectObject(brush);			
			pDC->Rectangle(m_rcBase);
			pDC->SelectObject(oldbrush);
			pDC->SelectObject(oldpen);
		}
		else
			pDC->FillSolidRect(m_rcBase, RGB(255, 255, 255));
	}
	

	if (m_bPrice && m_arData.GetSize())
	{
		drawPlusMinus(pDC);
	}
	
	if (m_nCandle > 0)
	{
		drawBong(pDC);
		drawMA(pDC);
	}

	if (m_bVolume)
	{
		drawVolumn(pDC);
	}
}


void CGraphWnd::drawBong(CDC* pDC)
{
	COLORREF	sColor{}, eColor{};
	CPen	*upPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, m_clrUp);
	CPen	*dnPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, m_clrDown);
	CPen	*sPen = pDC->SelectObject(upPen);
	CBrush	*upBrush = m_pApp->GetBrush(m_pView, m_clrUp);
	CBrush	*dnBrush = m_pApp->GetBrush(m_pView, m_clrDown);
	CBrush	*sBrush = pDC->SelectObject(upBrush);

	CPoint		pt[11];
	CBongData*	pData = nullptr;
	CBongData*	pPData = nullptr;
	const int	nCount = m_arData.GetSize();
	int		nPos[3]{};
	
	for ( int ii = 0 ; ii < m_nCandle && ii < nCount ; ii++ )
	{
		pData = m_arData.GetAt(ii);

		if (pData == nullptr)
			continue;

		if (pData->m_jgga == UNUSED_VALUE)
			continue;

		nPos[0] = getXPosR(ii+1) + 1;
		nPos[1] = getXPosR(ii) - 1;		
		nPos[2] = nPos[0] + nPos[1];

/*		if ((nPos[2] % 2) != 0)
		{
			if (ii == (m_nCandle - 1) || ii == (nCount - 1))
				nPos[0]--;
			else
				nPos[0]++;
		}
*/
		nPos[2] = nPos[0] + nPos[1];
		
		pt[0] = pt[10] = CPoint(nPos[2] / 2, getYPos(pData->m_koga, m_fBMax, m_fBMin, m_rcBong));
		
		if (pData->m_jgga > pData->m_siga)
		{
			pDC->SelectObject(upPen);
			pDC->SelectObject(upBrush);
			pt[1] = CPoint(pt[0].x, getYPos(pData->m_jgga, m_fBMax, m_fBMin, m_rcBong));
			pt[4] = CPoint(pt[0].x, getYPos(pData->m_siga, m_fBMax, m_fBMin, m_rcBong));
			sColor = RGB(255, 196,196);
			eColor = m_clrUp;
		}
		else if (pData->m_jgga < pData->m_siga)
		{
			pDC->SelectObject(dnPen);
			pDC->SelectObject(dnBrush);
			pt[4] = CPoint(pt[0].x, getYPos(pData->m_jgga, m_fBMax, m_fBMin, m_rcBong));
			pt[1] = CPoint(pt[0].x, getYPos(pData->m_siga, m_fBMax, m_fBMin, m_rcBong));
			sColor = RGB(196, 196,255);
			eColor = m_clrDown;
		}
		else
		{
			pt[4] = pt[1] = CPoint(pt[0].x, getYPos(pData->m_jgga, m_fBMax, m_fBMin, m_rcBong));

			sColor = RGB(255, 196,196);
			eColor = m_clrUp;
			for (int jj = ii + 1; jj < nCount; jj++)
			{
				pPData = m_arData.GetAt(jj);
				if (pPData->m_jgga == pData->m_jgga)
					continue;

				if (pPData->m_jgga > pData->m_jgga)
				{
					pDC->SelectObject(dnPen);
					pDC->SelectObject(dnBrush);
					sColor = RGB(196, 196,255);
					eColor = m_clrDown;
				}
				else
				{
					pDC->SelectObject(upPen);
					pDC->SelectObject(upBrush);
					sColor = RGB(255, 196,196);
					eColor = m_clrUp;
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
		//CRect	rc(pt[2], pt[7]);
		//rc.NormalizeRect();
		//DrawGradRect(pDC, rc, sColor, eColor);
		//pDC->Polyline(pt, 11);
		pDC->Polygon(pt, 11);
	}

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
}

void CGraphWnd::inputData(char* lParam)
{
	struct jbong30*		tmpptr = (struct jbong30*)lParam;
	memcpy(&m_jbong30, tmpptr, sizeof(m_jbong30));
	CBongData*	pData = nullptr;
	int		ii = 0, nDigit = 0;	// nMA = 0;
	CString		tmpstr;		

	tmpstr.Format("%.*s", sizeof(m_jbong30.digit), m_jbong30.digit);
	m_nDigit = nDigit = atoi(tmpstr);
	m_szDigit = tmpstr;
	m_arData.RemoveAll();

	tmpstr.Format("%.*s", sizeof(m_jbong30.shan), m_jbong30.shan);
	LIB_FormatVariableComma(m_szPlus, atof(tmpstr), m_nDigit);

	tmpstr.Format("%.*s", sizeof(m_jbong30.hhan), m_jbong30.hhan);
	LIB_FormatVariableComma(m_szMinus, atof(tmpstr), m_nDigit);

	for ( ii = 0 ; ii < 30; ii++ )
	{
		tmpstr.Format("%.*s", sizeof(m_jbong30.jbong[ii].date), m_jbong30.jbong[ii].date);
		tmpstr.TrimLeft();
		tmpstr.TrimRight();

		if (tmpstr.GetLength() != sizeof(m_jbong30.jbong[ii].date))
			break;
		
		pData = new CBongData;
		pData->SetData(&m_jbong30.jbong[ii]);
		m_arData.Add(pData);
	}
}

void CGraphWnd::calculate()
{
	const int nCount = m_arData.GetSize();
	const int nSize = m_arMADay.GetSize();
	int	nMA = 0;

	if (m_MAData)
	{
		for (int ii = 0 ; ii < nSize ; ii++ )
			delete [] m_MAData[ii];

		m_MAData = nullptr;
	}

	m_MAData = new double*[nSize];

	for (int ii = 0 ; ii < nSize ; ii++ )
	{
		nMA = m_arMADay.GetAt(ii);
		m_MAData[ii] = new double[nCount];
		calcuMA(m_MAData[ii], nMA);
	}
}

void CGraphWnd::calcuMA(double* target, int nDay)
{
	const int	nCount = m_arData.GetSize();

	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		target[ii] = calcuMA(nDay, ii);
	}
}

double CGraphWnd::calcuMA(int nDay, int nIndex)
{
	double	ret = 0.0;
	CBongData*	pData = nullptr;
	int	ii = 0;
	double	val = 0.0;
	const int	nCount = m_arData.GetSize();

	if ((nIndex + nDay) >= nCount)
		return UNUSED_VALUE;

	for ( ii = 0 ; ii < nDay ; ii++ )
	{
		pData = m_arData.GetAt(ii + nIndex);
		val += pData->m_jgga;
	}

	ret = val / (double) nDay;
	return ret;
}

LONG CGraphWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		{
			struct _extTHx* exTH = (struct _extTHx*)lParam;
			const int dlen = exTH->size;
			char* pData = exTH->data;

			if (dlen < m_nDataSize)
			{
				clearAll();
				Invalidate(FALSE);
				break;
			}

			inputData(pData);
			calculate();

			Invalidate(FALSE);
			ret = 0;
		}
		break;
	case DLL_INB:
		{
			m_output = _T("");
			ret = (LONG)&m_output;
		}
		break;
	case DLL_ALERT:
		setReal((char*)lParam);
		Invalidate(FALSE);
		ret = 0;
		break;
	case DLL_SETFONT:
		{
			const int	point = HIWORD(wParam);
			CString	name;

			name.Format("%s", (char*)lParam);

			if (point > 0)
			{
				m_nFontSize = point;

				if (!name.IsEmpty())
					m_szFontName = name;
				Invalidate(FALSE);
			}
		}
		break;
	}

	return ret;
}

void CGraphWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_MAData)
	{
		const int nRow = m_arMADay.GetSize();

		for ( int ii = 0 ; ii < nRow ; ii++ )
		{
			delete [] m_MAData[ii];
		}

		delete [] m_MAData;
	}

	if (m_tooltip)
	{
		m_tooltip->DestroyWindow();
		delete m_tooltip;
		m_tooltip = nullptr;
	}
}

int CGraphWnd::getDataIndex(CPoint pt)
{
	if (!m_rcGraph.PtInRect(pt))
		return -1;

	int	nIndex = 0;
	nIndex = (int)((double)(m_rcGraph.right - pt.x) / m_fUnit);
	return nIndex;
}

int CGraphWnd::getXPosR(int nIndex)
{
	int	ret = 0;

	if (nIndex >= (m_nCandle))
		ret = m_rcGraph.left;
	else
		ret = (int)((double)m_rcGraph.right - (m_fUnit * (double)nIndex));

	return ret;
}

int CGraphWnd::getXPosL(int nIndex)
{
	double	xPos = getXPosR(nIndex);

	if ((xPos - m_fUnit) < m_rcGraph.left)
		xPos = m_rcGraph.left;
	else
	{
		if (nIndex >= (m_nCandle - 1))
		{
			xPos = m_rcGraph.left;
		}
		else
			xPos -= m_fUnit;
	}
	return (int)xPos;
}

int CGraphWnd::getXPosM(int nIndex)
{
	double	xPos = getXPosL(nIndex);
	const double  xPos2 = getXPosR(nIndex);
	xPos += ((xPos2 - xPos) / 2.0);
	return (int)xPos;
}

int CGraphWnd::getYPos(double val, double maxValue, double minValue, CRect rect)
{
	const double	fMinMax = maxValue - minValue;
	const double	fHeight = rect.Height();
	double	yPos = 0;
	const double	fGab = maxValue - val;

	if (fMinMax <= 0.0)
		return rect.bottom;

	yPos = fHeight * fGab / fMinMax;
	return (int)((double)rect.top + yPos);
}

void CGraphWnd::drawMA(CDC* pDC)
{
	const int nCount = m_arMADay.GetSize();
	int	ii = 0, jj = 0;
	CPoint	pt;
	double	val = 0;
	const int nDCount = m_arData.GetSize();
	int	nIndex = 0;
	const int nSaveDC = pDC->SaveDC();
	CPen* pen[3]{};
		
	for ( ii = 0 ; ii < nCount ; ii++ )
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
	
		pen[nIndex] = m_pApp->GetPen(m_pView, PS_SOLID, 1, __MACOLOR__[nIndex]);
		pDC->SelectObject(pen[nIndex]);	

		for ( jj = 0 ; jj < m_nCandle && jj < nDCount ; jj++ )
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

void CGraphWnd::drawVolumn(CDC* pDC)
{
	CPoint		pt[5];
	CBongData*	pData = nullptr;
	double		val = 0.0;
	const int	nCount = m_arData.GetSize();

	for ( int ii = 0 ; ii < m_nCandle && ii < nCount; ii++ )
	{
		pData = m_arData.GetAt(ii);

		if (pData == nullptr)
			continue;

		val = pData->m_gvol;

		if (pData->m_gvol == UNUSED_VALUE)
			continue;

		pt[0] = pt[4] = CPoint(getXPosR(ii+1) + 1, getYPos(val, m_fVMax, m_fVMin, m_rcVolume));
		pt[1] = CPoint(pt[0].x, getYPos(0, m_fVMax, m_fVMin, m_rcVolume));
		pt[2] = CPoint(getXPosR(ii)-1, pt[1].y);
		pt[3] = CPoint(pt[2].x, pt[0].y);

		const CRect	rc(pt[0], pt[2]);
		DrawGradRect(pDC, rc, RGB(0, 220, 0), RGB(0, 0, 0));
	}	
}

CString CGraphWnd::makeOption()
{
	CString tmpstr = _T("");
	CString output = _T("");
	int	ii = 0, nSize = 0;
	
// make value option 
	{
		const  char* const cToken[] = {"/b", "/u"};
		const int* const   nValue[] = {&m_nCandle, &m_nDataType};

		nSize = sizeof(cToken) / sizeof(char*);

		for ( ii = 0 ; ii < nSize ; ii++ )
		{
			if (*nValue[ii] > 0)
			{
				tmpstr.Format("%s%d", cToken[ii], *nValue[ii]);
				output += tmpstr;
			}
		}
	}

// make booleen option	
	{
		const bool* const bWhat[]  = {&m_bVolume, &m_bLine, &m_bPrice, &m_bToolTip};
		char*	cToken[] = {"/v", "/l", "/p", "/t"};

		nSize = sizeof(cToken) / sizeof(char*);

		for ( ii = 0 ; ii < nSize ; ii++ )
		{
			if (*bWhat[ii])
			{
				tmpstr.Format("%s", cToken[ii]);
				output += tmpstr;
			}
		}
	}
	

// make multiple option	
	nSize = m_arMADay.GetSize();

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		tmpstr.Format("/m%d", m_arMADay.GetAt(ii));
		output += tmpstr;
	}

	return output;
}


void CGraphWnd::setReal(char* ptr)
{
	if (ptr == nullptr)
		return;

	CString	tmpstr = ptr, szVal;
	CString	tmpstr2 = tmpstr;
	CString symbol, datatmp;
	char*	szdata = nullptr;
	int	nPos = 0;
	double	data[5]{};
	int	nCount = m_arData.GetSize(), nMA = 0, nSize = 0;
	CBongData*	pData = nullptr;
	memset(data, 0x00, sizeof(data));

	nPos = tmpstr2.Find("\n");
	if (nPos > 0)	tmpstr2 = tmpstr2.Left(nPos);

	CIHApartMaps<CMapStringToPtr, CString, char*>	mapSymbol;

	int ii = 0;
	while (1)
	{
		if (tmpstr2.GetLength() <= 0)
			break;

		nPos = tmpstr2.Find("\t", 0);
		
		if (nPos < 0)
			break;

		symbol = tmpstr2.Left(nPos);		
		tmpstr2 = tmpstr2.Mid(nPos + 1);
		nPos = tmpstr2.Find("\t", 0);

		if (nPos >= 0)
		{
			datatmp = tmpstr2.Left(nPos);	
			tmpstr2 = tmpstr2.Mid(nPos + 1);
		}
		else
		{
			datatmp = tmpstr2;
			tmpstr2 = _T("");
		}

		
		nSize = datatmp.GetLength();			
		szdata = new char[nSize + 1]();
		memset(szdata, 0x00, nSize + 1);
		memcpy(szdata, datatmp, nSize);
		mapSymbol.SetAt(symbol, szdata);	
		nPos += 1;
		ii++;
	}
	
	if (nCount == 0)
	{		
		return;
	}
	else
	{
		pData = m_arData.GetAt(0);
	}

	char*	buf = nullptr;
	char*	asymbol[] = {R_SIGA, R_KOGA, R_JEGA, R_CURR, R_GVOL};
	char*	bsymbol[] = {ER_SIGA, ER_KOGA, ER_JEGA, ER_CURR, ER_GVOL};
	char**	psymbol = nullptr;

	nSize = sizeof(asymbol) / sizeof(char*);

	if (m_nDataType == TYPE_ECN)
		psymbol = bsymbol;
	else
		psymbol = asymbol;

	const double	div = pow(10, m_nDigit); 

	for (int ii = 0 ; ii < nSize ; ii++ )
	{
		if (!mapSymbol.Lookup(psymbol[ii], buf))
			return;

		data[ii] = fabs(atof(buf));

		if (ii < (nSize - 1))
			data[ii] *= div;
	}	

	if (data[0] == 0.0 || data[3] == 0.0)	return;

	pData->m_siga = data[0];
	pData->m_koga = data[1];
	pData->m_jega = data[2];
	pData->m_jgga = data[3];
	pData->m_gvol = data[4];

	nCount = m_arMADay.GetSize();	

	for (int ii = 0 ; ii < nCount ; ii++ )
	{
		nMA = m_arMADay.GetAt(ii);
		m_MAData[ii][0] = calcuMA(nMA, 0);
	}

}

void CGraphWnd::makeString(CBongData* pData, int nIndex, CStringArray& arString)
{
	CString		tmpstr;
	CString		tmpstr2;
	int		ii = 0, nMA = 0;
	const int nCount = m_arMADay.GetSize();

	// 일자
	tmpstr2 = pData->m_date;
	tmpstr.Format("일자:%.2s/%.2s/%.2s", tmpstr2.Mid(2, 2), tmpstr2.Mid(4, 2), tmpstr2.Right(2));
	arString.Add(tmpstr);

	// 시가

	if (pData->m_siga == UNUSED_VALUE)
		tmpstr = _T("N/A");
	else
		LIB_FormatVariableComma(tmpstr, pData->m_siga, m_nDigit);
	tmpstr = "시가:" + tmpstr;
	arString.Add(tmpstr);

	// 고가
	if (pData->m_koga == UNUSED_VALUE)
		tmpstr = _T("N/A");
	else		
		LIB_FormatVariableComma(tmpstr, pData->m_koga, m_nDigit);
	tmpstr = "고가:" + tmpstr;
	arString.Add(tmpstr);

	// 저가
	if (pData->m_jega == UNUSED_VALUE)
		tmpstr = _T("N/A");
	else		
		LIB_FormatVariableComma(tmpstr, pData->m_jega, m_nDigit);
	tmpstr = "저가:" + tmpstr;
	arString.Add(tmpstr);

	// 종가
	if (pData->m_jgga == UNUSED_VALUE)
		tmpstr = _T("N/A");
	else		
		LIB_FormatVariableComma(tmpstr, pData->m_jgga, m_nDigit);
	tmpstr = "종가:" + tmpstr;
	arString.Add(tmpstr);	

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		nMA = m_arMADay.GetAt(ii);
		if (m_MAData[ii][nIndex] == UNUSED_VALUE)
			tmpstr2 = _T("N/A");
		else
			LIB_FormatVariableComma(tmpstr2, m_MAData[ii][nIndex], m_nDigit);
		tmpstr.Format("MA%d:%s", nMA, tmpstr2);
		arString.Add(tmpstr);
	}

	if (m_bVolume)
	{
		// 거래량
		if (pData->m_gvol == UNUSED_VALUE)
			tmpstr = _T("N/A");
		else
			LIB_FormatVariableComma(tmpstr, pData->m_gvol, 0);
		tmpstr = "거래량:" + tmpstr;
		arString.Add(tmpstr);
	}
}

void CGraphWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bToolTip)
	{
		CString	str = GetTipStr(point);
		if (m_PosData != str)
		{
			m_tooltip->ShowWindow(SW_HIDE);
			m_PosData.Format("%s", str);
			KillTimer(TIMER_PID);
			SetTimer(TIMER_PID, TIMER_PGAP, nullptr);
		}

	}
		
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CGraphWnd::IsInner()
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

CRect CGraphWnd::MakeAlign()
{
	CRect	rect, rcMy, rcTip, rcParent(0, 0, 0, 0);
	CWnd*	pWnd = (CWnd*)GetParent();
	int	x = 0, y = 0;
	POINT	pt{};

	x = GetSystemMetrics(SM_CXFULLSCREEN);
	y = GetSystemMetrics(SM_CYFULLSCREEN);

	::GetCursorPos(&pt);
	GetWindowRect(rcMy);

	m_tooltip->GetWindowRect(rcTip);

	if (pWnd)
	{
		pWnd->GetWindowRect(rcParent);
	}

	
	rect.left = rcParent.left;

	if (rect.left < 0)
	{
		rect.left = 0;
	}	

	rect.right = rect.left + rcTip.Width();

	if (rect.right > x)
	{
		rect.right = x;
		rect.left = rect.right - rcTip.Width();
	}

	rect.top = rcMy.bottom + 5;
	rect.bottom = rect.top + rcTip.Height();

	if (rect.bottom > y)
	{
		rect.bottom = rcMy.top - 5;
		rect.top = rect.bottom - rcTip.Height();
	}
	
	return rect;
}

void CGraphWnd::OnTimer(UINT nIDEvent) 
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
			m_bTimer = FALSE;
		}
		
	}
	
	CWnd::OnTimer(nIDEvent);
}

LRESULT CGraphWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_ERASEBKGND)
		return TRUE;
	return CWnd::WindowProc(message, wParam, lParam);
}

void CGraphWnd::DrawGradRect(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight)
{
	CRect	rect;
	int	nWidth{}, nHeight{};

	drawRC.right += 1;
	drawRC.bottom += 1;
	nWidth = drawRC.Width(); 
	nHeight = drawRC.Height();

	int rs{}, gs{}, bs{};
	rs = GetRValue(sColor);
	gs = GetGValue(sColor);
	bs = GetBValue(sColor);

	int re{}, ge{}, be{};
	re = GetRValue(eColor);
	ge = GetGValue(eColor);
	be = GetBValue(eColor);

	float	rStep{}, gStep{}, bStep{};
	CPen* sPen{};
	if (bRight)
	{
		if (!nWidth)
		{
			CPen*	cPen;

			cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, sColor);
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
				CPen*	cPen;
				cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
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
				
				CBrush* brush;
				brush = m_pApp->GetBrush(m_pView, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, brush);
			}
		}
	}
	else
	{
		if (!nHeight)
		{
			CPen*	cPen;
			cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, sColor);
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
				CPen*	cPen;
				cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
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
				
				CBrush* brush;
				brush = m_pApp->GetBrush(m_pView, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, brush);
			}
		}
	}
}

void CGraphWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);	
	calcuArea(cx, cy);	
}

void CGraphWnd::clearAll()
{
	if (m_MAData)
	{
		const int nRow = m_arMADay.GetSize();

		for ( int ii = 0 ; ii < nRow ; ii++ )
		{
			delete [] m_MAData[ii];
		}

		delete [] m_MAData;
		m_MAData = nullptr;
	}

	m_arData.RemoveAll();
}


void CGraphWnd::SetProperties(LPCTSTR str) 
{
	parseParam((char*)str);
	calcuArea();
	calculate();
	Invalidate(FALSE);
}

BSTR CGraphWnd::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	m_option = makeOption();
	strResult = m_option;
	return strResult.AllocSysString();
}


CString CGraphWnd::GetTipStr(CPoint pt)
{
	CString	retStr = _T("");
	m_arString.RemoveAll();

	const int nIndex = getDataIndex(pt);

	if (nIndex < 0)
	{
		m_tooltip->ShowWindow(SW_HIDE);	
		m_nIndex = -1;
	}
	else
	{
		const int nCount = m_arData.GetSize();
		
		if (nIndex >= nCount)	return retStr;

		m_nIndex = nIndex;
		CBongData*	pData = nullptr;
		pData = m_arData.GetAt(nIndex);	

		makeString(pData, nIndex, m_arString);
		if (m_arString.GetSize() > 0)
			retStr = m_arString.GetAt(0);
	}

	return retStr;
}

void CGraphWnd::MouseMove(CPoint pt)
{
	if (!m_tooltip)	return;

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

	if (m_tooltip->IsWindowVisible())	return;

	CRect	rect = m_tooltip->SetData(m_arString);
	
	pt.x += 10;
	pt.y += 10;

	ClientToScreen(&pt);

	const int nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
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
	SetTimer(TIMER_ID, TIMER_GAP, nullptr);
}

void CGraphWnd::OnVisibleChanged() 
{
	if (m_visible)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
}

void CGraphWnd::updatePalette()
{
	m_clrUp   = m_pApp->GetColor(m_pView, 56);
	m_clrDown = m_pApp->GetColor(m_pView, 57);
}

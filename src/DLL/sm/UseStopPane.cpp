// UseStopPane.cpp : implementation file
//

#include "stdafx.h"
#include "UseStopPane.h"
#include "../../axis/axMsg.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BKCOLOR		RGB(217, 217, 217)
#define BORDERCOLOR	RGB(  0, 140,   0)
#define INACT_BDR_COLOR	RGB( 90,  90,   90)

#define TM_BLINK	435
#define CX_LED		7
#define CY_LED		7
#define RGB_LED_NONE	RGB(192, 192, 192)
#define RGB_LED_1	RGB(0, 150, 0)
#define RGB_LED_2	RGB(0, 255, 0)

/////////////////////////////////////////////////////////////////////////////
// CUseStopPane

CUseStopPane::CUseStopPane(bool (*axiscall)(int, WPARAM, LPARAM))
{
	m_axiscall = axiscall;

	m_bStockUsing = FALSE;
	m_bFOUsing    = FALSE;
	m_bStockRunning = FALSE;
	m_bFORunning = FALSE;

	CString	fontName;
	LOGFONT	lf{};

	m_clrStock = RGB_LED_NONE;
	m_clrFO    = RGB_LED_NONE;

	//fontName.LoadString(ST_TEXT_CLOCKFONT);
	fontName = "굴림";
	m_font.CreatePointFont(90, "굴림");

	m_font.GetLogFont(&lf);
	m_font.DeleteObject();

	m_font.CreateFont(	lf.lfHeight,			// nHeight
				lf.lfWidth,			// nWidth
				lf.lfEscapement,		// nEscapement
				lf.lfOrientation,		// nOrientation
				FW_SEMIBOLD,			// nWeight
				lf.lfItalic,			// bItalic
				lf.lfUnderline,			// bUnderline
				lf.lfStrikeOut,			// cStrikeOut
				lf.lfCharSet,			// nCharSet
				lf.lfOutPrecision,		// nOutPrecision
				lf.lfClipPrecision,		// nClipPrecision
				lf.lfQuality,			// nQuality
				lf.lfPitchAndFamily,		// nPitchAndFamily
				(const char *)fontName);        // lpszFacename//*/
}

CUseStopPane::~CUseStopPane()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CUseStopPane, CWnd)
	//{{AFX_MSG_MAP(CUseStopPane)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUseStopPane message handlers

void CUseStopPane::SetUseStockStop(BOOL bUse)
{
	if (m_bStockUsing != bUse)
	{
		if (bUse)
			m_clrStock = RGB_LED_1;
		else
			m_clrStock = RGB_LED_NONE;
	}
	m_bStockUsing = bUse;
	
	Invalidate();
}

void CUseStopPane::SetUseFOStop(BOOL bUse)
{
	if (m_bFOUsing != bUse)
	{
		if (bUse)
			m_clrFO = RGB_LED_1;
		else
			m_clrFO = RGB_LED_NONE;
	}
	m_bFOUsing = bUse;
	Invalidate();
}

void CUseStopPane::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (!m_bStockRunning && !m_bFORunning)
		return;
	CFont*	font = dc.SelectObject(&m_font);
	CRect rc;

	GetClientRect(&rc);
	rc.DeflateRect(1, 1, 1, 0);

	dc.SetBkMode(TRANSPARENT);
	rc.InflateRect(1, 1, 1, 0);
	dc.FillSolidRect(rc, BKCOLOR);
	dc.SetTextColor(RGB(0, 0, 255));

	if (m_bStockRunning && m_bFORunning)
	{
		dc.DrawText("주식스탑 동작중", m_stockRc, DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		dc.Draw3dRect(m_stockRc, BORDERCOLOR, BORDERCOLOR);

		dc.DrawText("선옵스탑 동작중", m_foRc, DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		dc.Draw3dRect(m_foRc, BORDERCOLOR, BORDERCOLOR);
	}
	else
	{
		if (m_bStockRunning)
			dc.DrawText("주식스탑 동작중", rc, DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		else
			dc.DrawText("선옵스탑 동작중", rc, DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		dc.Draw3dRect(rc, BORDERCOLOR, BORDERCOLOR);
	}

	dc.SelectObject(font);
}

void CUseStopPane::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bStockRunning && m_bFORunning)
	{
		if (m_stockRc.PtInRect(point))
		{
			m_axiscall(AXI_STOPLOSS, 0, 0);
		}
		
		if (m_foRc.PtInRect(point))
		{
			m_axiscall(AXI_FOSTOPLOSS, 0, 0);
		}
	}
	else if (m_bStockRunning)
		m_axiscall(AXI_STOPLOSS, 0, 0);
	else if (m_bFORunning)
		m_axiscall(AXI_FOSTOPLOSS, 0, 0);
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CUseStopPane::OnTimer(UINT nIDEvent) 
{
	static COLORREF rgbs[] = {RGB_LED_1, RGB_LED_2
	};
	static int nIdx = 0;
	if (m_bStockRunning)
	{
		CRect rc = m_stockRc;
		rc.right = rc.left + CX_LED;
		rc.bottom = rc.top + CY_LED;
		drawUsing(rc, rgbs[nIdx]);
		m_clrStock = rgbs[nIdx];
	}

	if (m_bFORunning)
	{
		CRect rc = m_foRc;
		rc.right = rc.left + CX_LED;
		rc.bottom = rc.top + CY_LED;
		drawUsing(rc, rgbs[nIdx]);
		m_clrFO = rgbs[nIdx];
	}

	if (++nIdx >= sizeof(rgbs) / sizeof(COLORREF))
		nIdx = 0;
	CWnd::OnTimer(nIDEvent);
}

void CUseStopPane::drawUsing(CRect rc, COLORREF rgb)
{
	CClientDC dc(this);
	dc.FillSolidRect(rc, rgb);
	dc.Draw3dRect(rc, BORDERCOLOR, BORDERCOLOR);
}

void CUseStopPane::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rc;

	GetClientRect(&rc);
	rc.DeflateRect(1, 1, 1, 0);
	m_stockRc = m_foRc = rc;
	m_stockRc.right = rc.Width() / 2 - 1;
	m_foRc.left = rc.Width() / 2 + 1;	
}

void CUseStopPane::SetRunningStockStop(BOOL bRunning)
{
	m_bStockRunning = bRunning;
	m_clrStock = RGB_LED_1;

	if (bRunning)
	{
		/*KillTimer(TM_BLINK);
		SetTimer(TM_BLINK, 800, NULL);*/
	}
	else if (!m_bFORunning)
		//KillTimer(TM_BLINK);
		ShowWindow(SW_HIDE);
	Invalidate();
}

void CUseStopPane::SetRunningFOStop(BOOL bRunning)
{
	m_bFORunning = bRunning;
	m_clrFO = RGB_LED_1;

	if (bRunning)
	{
		/*KillTimer(TM_BLINK);
		SetTimer(TM_BLINK, 800, NULL);*/
	}
	else if (!m_bStockRunning)
		//KillTimer(TM_BLINK);
		ShowWindow(SW_HIDE);
	Invalidate();
}

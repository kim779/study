// TextProgressCtrl.cpp : implementation file
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TextProgressCtrl.h"
#include "CMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl

#define IDT_MARQUEE			1					
#define	EDGE_SPACE			2					

CTextProgressCtrl::CTextProgressCtrl()
{
	m_nMin = 0;
	m_nMax = 100;

	m_nStepSize = 10;
	m_nPos = 0;

	// default colors
	m_crBarClr = CLR_DEFAULT;
	m_crBarBkClr = CLR_DEFAULT;
	m_crTextClr = CLR_DEFAULT;
	m_crTextBkClr = CLR_DEFAULT;

	m_bShowPercent = TRUE;
	m_dwTextStyle = DT_CENTER;

	m_nMarqueeSize = 20;
}

CTextProgressCtrl::~CTextProgressCtrl()
{

	if (m_VerticalFont.m_hObject)
		m_VerticalFont.DeleteObject();
}

BEGIN_MESSAGE_MAP(CTextProgressCtrl, CProgressCtrl)
//{{AFX_MSG_MAP(CTextProgressCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(PBM_SETRANGE, OnSetRange)
	ON_MESSAGE(PBM_SETPOS, OnSetPos)
	ON_MESSAGE(PBM_DELTAPOS, OnOffsetPos)
	ON_MESSAGE(PBM_SETSTEP, OnSetStep)
	ON_MESSAGE(PBM_STEPIT, OnStepIt)
	ON_MESSAGE(PBM_SETRANGE32, OnSetRange32)
	ON_MESSAGE(PBM_GETRANGE, OnGetRange)
	ON_MESSAGE(PBM_GETPOS, OnGetPos)
	ON_MESSAGE(PBM_SETBARCOLOR, OnSetBarColor)
	ON_MESSAGE(PBM_SETBKCOLOR, OnSetBarBkColor)

	ON_MESSAGE(PBM_GETBARCOLOR, OnGetBarColor)
	ON_MESSAGE(PBM_GETBKCOLOR, OnGetBarBkColor)
	ON_MESSAGE(PBM_SETTEXTCOLOR, OnSetTextColor)
	ON_MESSAGE(PBM_GETTEXTCOLOR, OnGetTextColor)
	ON_MESSAGE(PBM_SETTEXTBKCOLOR, OnSetTextBkColor)
	ON_MESSAGE(PBM_GETTEXTBKCOLOR, OnGetTextBkColor)
	ON_MESSAGE(PBM_SETSHOWPERCENT, OnSetShowPercent)
	ON_MESSAGE(PBM_ALIGNTEXT, OnAlignText)
	ON_MESSAGE(PBM_SETMARQUEE, OnSetMarquee)
	ON_MESSAGE(PBM_SETMARQUEEOPTIONS, OnSetMarqueeOptions)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl message handlers
BOOL CTextProgressCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CTextProgressCtrl::OnPaint()
{
	if (m_nMin >= m_nMax)
		return;

	CPaintDC PaintDC(this);						
	CMemDC dc(&PaintDC);						

	const COLORREF crBarColor		= (COLORREF)OnGetBarColor(0, 0);
	const COLORREF crBarBkColor	= (COLORREF)OnGetBarBkColor(0, 0);
	const COLORREF crTextColor	= (COLORREF)OnGetTextColor(0, 0);
	const COLORREF crTextBkColor	= (COLORREF)OnGetTextBkColor(0, 0);;

	if (GetFont())
		dc.SelectObject(GetFont());
	else
		dc.SelectObject(GetParent()->GetFont());

	BOOL bLeft = true;
	const BOOL bMiddle = true;
	BOOL bRight = true;

	CRect LeftRect, MiddleRect, RightRect, ClientRect;
	GetClientRect(&ClientRect);
	DrawEdge(dc, ClientRect, EDGE_SUNKEN, BF_ADJUST | BF_RECT | BF_FLAT);
	LeftRect = MiddleRect = RightRect = ClientRect;

	const double dFraction = (double)(m_nPos - m_nMin) / ((double)(m_nMax - m_nMin));

	const DWORD dwStyle = GetStyle();
	if ((dwStyle & PBS_VERTICAL) && (!m_VerticalFont.m_hObject))
		CreateVerticalFont();

	if (dwStyle & PBS_MARQUEE)
	{
		if (dwStyle & PBS_VERTICAL)
		{
			MiddleRect.top = MiddleRect.bottom - (int)((MiddleRect.Height()) * dFraction);
			MiddleRect.bottom = MiddleRect.top + (((LeftRect.Height() * m_nMarqueeSize) + 50) / 100);
			if (MiddleRect.bottom >= LeftRect.bottom)
			{
				bLeft = false;
				MiddleRect.bottom = LeftRect.bottom;
			}
			else
				LeftRect.top = MiddleRect.bottom;
			if (MiddleRect.top <= RightRect.top)
			{
				bRight = false;
				MiddleRect.top = RightRect.top;
			}
			else
				RightRect.bottom = MiddleRect.top;
		}
		else
		{
			MiddleRect.right = MiddleRect.left + (int)((MiddleRect.Width()) * dFraction);
			MiddleRect.left = MiddleRect.right - (((LeftRect.Width() * m_nMarqueeSize) + 50) / 100);
			if (MiddleRect.left <= LeftRect.left)
			{
				bLeft = false;
				MiddleRect.left = LeftRect.left;
			}
			else
				LeftRect.right = MiddleRect.left;
			if (MiddleRect.right >= RightRect.right)
			{
				bRight = false;
				MiddleRect.right = RightRect.right;
			}
			else
				RightRect.left = MiddleRect.right;
		}
	}
	else
	{
		bLeft = false;
		if (dwStyle & PBS_VERTICAL)
		{
			MiddleRect.top = MiddleRect.bottom - (int)((MiddleRect.Height()) * dFraction);
			RightRect.bottom = MiddleRect.top;
		}
		else
		{
			MiddleRect.right = MiddleRect.left + (int)((MiddleRect.Width()) * dFraction);
			RightRect.left = MiddleRect.right;
		}
	}
	if (bLeft)
		dc.FillSolidRect(LeftRect, crBarBkColor);
	if (bMiddle)
		dc.FillSolidRect(MiddleRect, crBarColor);
	if (bRight)
		dc.FillSolidRect(RightRect, crBarBkColor);

	CString str;
	GetWindowText(str);
	if (m_bShowPercent)
		str.Format(_T("%d%%"), (int)((dFraction * 100.0) + 0.5));
	if (str.GetLength())
	{
		CFont *pOldFont = NULL;
		CPoint ptStart;
		CSize szText;

		if (dwStyle & PBS_VERTICAL)
		{
			pOldFont = (CFont*)dc.SelectObject(&m_VerticalFont);

			szText = dc.GetOutputTextExtent(str);
			switch (m_dwTextStyle)
			{
				case DT_LEFT:
					ptStart.y = ClientRect.bottom - 1 - EDGE_SPACE;
					break;
				case DT_RIGHT:
					ptStart.y = ClientRect.top + szText.cx + EDGE_SPACE;
					break;
				case DT_CENTER:
				default:
					ptStart.y = ClientRect.top + szText.cx + (ClientRect.Height() - szText.cx) / 2;
					break;
			}
			ptStart.x = ClientRect.left + ((ClientRect.Width() - szText.cy) / 2);
		}
		else
		{
			szText = dc.GetOutputTextExtent(str);
			switch (m_dwTextStyle)
			{
				case DT_LEFT:
					ptStart.x = ClientRect.left + EDGE_SPACE;
					break;
				case DT_RIGHT:
					ptStart.x = ClientRect.right - szText.cx - 1 - EDGE_SPACE;
					break;
				case DT_CENTER:
				default:
					ptStart.x = ClientRect.left + ((ClientRect.Width() - szText.cx) / 2);
					break;
			}
			ptStart.y = ClientRect.top + ((ClientRect.Height() - szText.cy) / 2);
		}

		dc.SetBkMode(TRANSPARENT);

		CRgn rgn;

		if (bLeft)
		{
			rgn.CreateRectRgn(LeftRect.left, LeftRect.top, LeftRect.right, LeftRect.bottom);
			dc.SelectClipRgn(&rgn);
			dc.SetTextColor(crTextColor);
			dc.ExtTextOut(ptStart.x, ptStart.y, ETO_CLIPPED, &ClientRect, str, NULL);
			rgn.DeleteObject();
		}

		if (bMiddle)
		{
			rgn.CreateRectRgn(MiddleRect.left, MiddleRect.top, MiddleRect.right, MiddleRect.bottom);
			dc.SelectClipRgn(&rgn);
			dc.SetTextColor(crTextBkColor);
			dc.ExtTextOut(ptStart.x, ptStart.y, ETO_CLIPPED, &ClientRect, str, NULL);
			rgn.DeleteObject();
		}

		if (bRight)
		{
			rgn.CreateRectRgn(RightRect.left, RightRect.top, RightRect.right, RightRect.bottom);
			dc.SelectClipRgn(&rgn);
			dc.SetTextColor(crTextColor);
			dc.ExtTextOut(ptStart.x, ptStart.y, ETO_CLIPPED, &ClientRect, str, NULL);
			rgn.DeleteObject();
		}

		if (pOldFont)
			dc.SelectObject(pOldFont);
	}
}

void CTextProgressCtrl::OnTimer(UINT_PTR nIDEvent)
{
	static int nDirection = +1;

	if (nIDEvent == IDT_MARQUEE)
	{
		const int nPosition = GetPos();

		const int nMax = (int)((((double)(100 + m_nMarqueeSize) * (double)m_nMax) + 0.5) / 100.0);
		if ((nDirection == 1) && (nPosition >= nMax))
			nDirection = -1;
		else if ((nDirection == -1) && (nPosition <= m_nMin))
			nDirection = +1;

		SetPos(nPosition + nDirection);
	}
}

LRESULT CTextProgressCtrl::OnSetRange(WPARAM, LPARAM lparamRange)
{
	return (OnSetRange32(LOWORD(lparamRange), HIWORD(lparamRange)));
}

LRESULT CTextProgressCtrl::OnSetPos(WPARAM nNewPos, LPARAM)
{
	const int nOldPos = m_nPos;
	m_nPos = (int)nNewPos;
	const DWORD dwStyle = GetStyle();
	if (m_nPos < m_nMin)
		m_nPos = m_nMin;
	if (dwStyle & PBS_MARQUEE)
	{
		const int nMax = (int)((((double)(100 + m_nMarqueeSize) * (double)m_nMax) + 0.5) / 100.0);
		if (m_nPos > nMax)
			m_nPos = nMax;
	}
	else if (m_nPos > m_nMax)
		m_nPos = m_nMax;
	CommonPaint();
	
	return ((LRESULT)nOldPos);
}

LRESULT CTextProgressCtrl::OnOffsetPos(WPARAM nIncrement, LPARAM)
{
	return (OnSetPos((WPARAM)(m_nPos + (int)nIncrement), 0));
}

LRESULT CTextProgressCtrl::OnSetStep(WPARAM nStepInc, LPARAM)
{
	const int nOldStep = m_nStepSize;
	m_nStepSize = (int)nStepInc;
	return ((LRESULT)nOldStep);
}

LRESULT CTextProgressCtrl::OnStepIt(WPARAM, LPARAM)
{
	int nPos = m_nPos + m_nStepSize;
	if (nPos < m_nMin)
		nPos = m_nMax - abs(m_nMin - nPos);
	else if (nPos > m_nMax)
		nPos = m_nMin + abs(nPos - m_nMax);
	return ((LRESULT)OnSetPos((WPARAM)nPos, 0));
}

LRESULT CTextProgressCtrl::OnSetRange32(WPARAM nLowLim, LPARAM nHighLim)
{
	const int nOldMin = m_nMin;
	const int nOldMax = m_nMax;

	m_nMin = (int)nLowLim;
	m_nMax = (int)nHighLim;

	return (MAKELRESULT(LOWORD(nOldMin), LOWORD(nOldMax)));
}

LRESULT CTextProgressCtrl::OnGetRange(WPARAM bWhichLimit, LPARAM pPBRange)
{
	const BOOL bType = (BOOL)bWhichLimit;
	PPBRANGE pRange = (PPBRANGE)pPBRange;

	if (pRange)
	{
		pRange->iLow = m_nMin;
		pRange->iHigh = m_nMax;
	}

	return ((LRESULT)(bType ? m_nMin : m_nMax));
}

LRESULT CTextProgressCtrl::OnGetPos(WPARAM, LPARAM)
{
	return ((LRESULT)m_nPos);
}

LRESULT CTextProgressCtrl::OnSetBarColor(WPARAM, LPARAM crBar)
{
	CommonPaint();

	const COLORREF crOldBarClr = m_crBarClr;
	m_crBarClr = (COLORREF)crBar;
	return ((LRESULT)crOldBarClr);
}

LRESULT CTextProgressCtrl::OnSetBarBkColor(WPARAM, LPARAM crBarBk)
{
	CommonPaint();
	const COLORREF crOldBarBkClr = m_crBarBkClr;
	m_crBarBkClr = (COLORREF)crBarBk;
	return ((LRESULT)crOldBarBkClr);
}

LRESULT CTextProgressCtrl::OnGetBarColor(WPARAM, LPARAM)
{
	return ((LRESULT)((m_crBarClr == CLR_DEFAULT)? ::GetSysColor(COLOR_HIGHLIGHT) : m_crBarClr));
}

LRESULT CTextProgressCtrl::OnGetBarBkColor(WPARAM, LPARAM)
{
	return ((LRESULT)((m_crBarBkClr == CLR_DEFAULT)? ::GetSysColor(COLOR_BTNFACE) : m_crBarBkClr));
}

LRESULT CTextProgressCtrl::OnSetTextColor(WPARAM, LPARAM crText)
{
	CommonPaint();

	const COLORREF crOldTextClr = m_crTextClr;
	m_crTextClr = (COLORREF)crText;
	return ((LRESULT)crOldTextClr);
}

LRESULT CTextProgressCtrl::OnGetTextColor(WPARAM, LPARAM)
{
	return ((LRESULT)((m_crTextClr == CLR_DEFAULT ? ::GetSysColor(COLOR_WINDOWTEXT) : m_crTextClr)));
}

LRESULT CTextProgressCtrl::OnSetTextBkColor(WPARAM, LPARAM crTextBkClr)
{
	CommonPaint();
	const COLORREF crOldTextBkClr = m_crTextBkClr;
	m_crTextBkClr = (COLORREF)crTextBkClr;
	return ((LRESULT)crOldTextBkClr);
}

LRESULT CTextProgressCtrl::OnGetTextBkColor(WPARAM, LPARAM)
{
	return ((LRESULT)((m_crTextBkClr == CLR_DEFAULT ? ::GetSysColor(COLOR_WINDOW) : m_crTextBkClr)));
}

LRESULT CTextProgressCtrl::OnSetShowPercent(WPARAM bShow, LPARAM)
{
	CommonPaint();
	const BOOL bOldShow = m_bShowPercent;
	m_bShowPercent = (BOOL)bShow;
	return ((LRESULT)bOldShow);
}

LRESULT CTextProgressCtrl::OnAlignText(WPARAM dwAlignment, LPARAM)
{
	const DWORD dwAlign = (DWORD)dwAlignment;
	ASSERT((dwAlign == DT_LEFT) || (dwAlign == DT_RIGHT) || (dwAlign == DT_CENTER));
	CommonPaint();
	const DWORD dwOldTextStyle = m_dwTextStyle;
	m_dwTextStyle = dwAlign;
	return ((LRESULT)dwOldTextStyle);
}

LRESULT CTextProgressCtrl::OnSetMarquee(WPARAM bShow, LPARAM uMsecBetweenUpdate)
{
	CommonPaint();
	m_bShowPercent = false;
	if ((BOOL)bShow)
	{
		ModifyStyle(0, PBS_MARQUEE);
		SetTimer(IDT_MARQUEE, (UINT)uMsecBetweenUpdate, NULL);
	}
	else
	{
		KillTimer(IDT_MARQUEE);
		ModifyStyle(PBS_MARQUEE, 0);
	}

	return ((LRESULT)bShow);
}

LRESULT CTextProgressCtrl::OnSetMarqueeOptions(WPARAM nBarSize, LPARAM)
{
	const int nBar = (int)nBarSize;
	CommonPaint();
	m_nMarqueeSize = nBar;

	return ((LRESULT)nBar);
}

////////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl helper functions
void CTextProgressCtrl::CreateVerticalFont()
{
	if (m_VerticalFont.m_hObject)
		m_VerticalFont.DeleteObject();
		  
	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	lf.lfEscapement = lf.lfOrientation = 900;
	m_VerticalFont.CreateFontIndirect(&lf);
}

void CTextProgressCtrl::CommonPaint()
{
	RedrawWindow();
}

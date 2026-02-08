// NClock.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "NClock.h"
#include "../dll/sm/MenuXP.h"

#include "mainFrm.h"
#include "MapHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INIT_POSITION -9999

#define CAL_LEFT	149
#define CAL_TOP		13
#define CAL_RIGHT	170
#define CAL_BOTTOM	34
	
/////////////////////////////////////////////////////////////////////////////
// CNClock

CNClock::CNClock()
{
	m_bSmall = FALSE;
	m_bOverCalendar = FALSE;

	CreateBoldFont(m_font, 120, "굴림체");
	CreateBoldFont(m_fontMiniBold, 90, "돋움");
	m_fontMini.CreatePointFont(90, "돋움");
	
	m_currentAMPM		= -1;
	m_currentWeekday	= -1;

	m_trace = FALSE;
	m_rcMain.left = INIT_POSITION;
	m_lastLButtonDown = GetTickCount();
}

CNClock::~CNClock()
{
	m_font.DeleteObject();

	Save();
}


BEGIN_MESSAGE_MAP(CNClock, CWnd)
	//{{AFX_MSG_MAP(CNClock)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNClock message handlers

void CNClock::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	Draw(dc);

// 	BITMAP	bm;
// 	CRect	wRc;
// 	
// 	m_bmpBackground->GetBitmap(&bm);
// 	GetDesktopWindow()->GetWindowRect(&wRc);
// 	SetWindowPos(&wndTopMost, wRc.right - bm.bmWidth, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE|SWP_HIDEWINDOW);
}

void CNClock::Draw(CDC& dc)
{
	CTime tm(CTime::GetCurrentTime());

	if ((tm.GetDayOfWeek() - 1 != m_currentWeekday) ||
	    (GetAMPM(tm.GetHour()) != m_currentAMPM))
	{
		m_currentAMPM = GetAMPM(tm.GetHour());
		m_currentWeekday = tm.GetDayOfWeek() - 1; // 1 ~ 7 to 0 ~ 6

		UpdateBackground(&dc);
	}

	CRect	cRc;
	GetClientRect(cRc);

	DrawImg(&dc, &m_back);
	DrawClock(&dc);
}

void CNClock::DrawClock(CDC* pDC)
{
	CTime tm(CTime::GetCurrentTime());
	int hour(tm.GetHour());

	if (hour > 12)
	{
		hour -= 12;
	}
	CString time;
	time.Format("%2d:%02d:%02d", hour, tm.GetMinute(), tm.GetSecond());

	if (m_bSmall)
		DrawClockMini(pDC, time);
	else
		DrawClock(pDC, time);
}

void CNClock::DrawClock(CDC* pDC, CString& time)
{
	CFont* oldFont = pDC->SelectObject(&m_font);
	pDC->SetTextColor(0x00FFFFFE);
	pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(time, m_bSmall? CRect(32, 3, 300, 100) : CRect(63, 14, 300, 100), DT_LEFT | DT_TOP);

	pDC->SelectObject(oldFont);
}

void CNClock::DrawClockMini(CDC* pDC, CString& time)
{
	CFont* oldFont = pDC->SelectObject(&m_fontMiniBold);
	pDC->SetTextColor(RGB(254, 186, 6));
	pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(time, CRect(32, 3, 300, 100), DT_LEFT | DT_TOP);
	pDC->SelectObject(oldFont);
}

void CNClock::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TM_CLOCK)
	{
		CWindowDC dc(this);
		Draw(dc);
	}
	

	CWnd::OnTimer(nIDEvent);
}

void CNClock::Init()
{
	if (m_bmpBackground->GetSafeHandle())
	{
		BITMAP	bm;
		CRect	wRc;
		
		m_bmpBackground->GetBitmap(&bm);
		GetDesktopWindow()->GetWindowRect(&wRc);
		SetWindowPos(&wndTopMost, wRc.right - bm.bmWidth, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE|SWP_HIDEWINDOW);
		//SetWindowPos(&CWnd::wndTopMost,wRc.right - bm.bmWidth, 0, bm.bmWidth, bm.bmHeight,SWP_NOMOVE|SWP_NOSIZE);
	}

	SetTimer(TM_CLOCK, 1000, NULL);
}

void CNClock::OnClose() 
{
	ShowWindow(SW_HIDE);
}

void CNClock::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CMenuXP	menu;
	CString	string;
	CPoint	lp(point);

	string.LoadString(ST_TEXT_HIDECLOCK);
	menu.CreatePopupMenu();
	ClientToScreen(&lp);
	menu.AppendMenuX(MF_STRING, 9999, string);
	if (m_bSmall)
		string.LoadString(ST_TEXT_NORMCLOCK);
	else
		string.LoadString(ST_TEXT_MINICLOCK);
	menu.AppendMenuX(MF_STRING, 10000, string);	

	menu.AppendMenuX(MF_STRING, 10001, "따라다니기");
	if (m_trace)
		menu.CheckMenuItem(10001, MF_CHECKED);

	int cmd = menu.TrackPopupMenu(TPM_RETURNCMD|TPM_LEFTALIGN | TPM_LEFTBUTTON, lp.x, lp.y, this);

	switch (cmd)
	{
	case 9999:
		ShowWindow(SW_HIDE);
		break;
	case 10000:
		SetMini(!m_bSmall);
		break;
	case 10001:
		SetTrace(!m_trace);
		break;
	}

	CWnd::OnRButtonDown(nFlags, point);
}

int CNClock::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Load();
	
	m_bmpBackground		= Axis::GetBitmap(IDB_CLOCK_BG);
	m_bmpBackgroundMini = Axis::GetBitmap(IDB_CLOCK_BG_MINI);
	m_bmpAMPM[0]		= Axis::GetBitmap(IDB_CLOCK_AM);
	m_bmpAMPM[1]		= Axis::GetBitmap(IDB_CLOCK_PM);
	m_bmpCalendar[0]	= Axis::GetBitmap(IDB_CLOCK_CAL);
	m_bmpCalendar[1]	= Axis::GetBitmap(IDB_CLOCK_CAL_OVER);

	const UINT weekday[7] = {
		IDB_CLOCK_SUN, IDB_CLOCK_MON, IDB_CLOCK_TUE, IDB_CLOCK_WED, 
		IDB_CLOCK_THU, IDB_CLOCK_FRI, IDB_CLOCK_SAT
	};
	for (int i = 0; i < ArraySize(weekday); i++)
		m_bmpWeekday[i] = Axis::GetBitmap(weekday[i]);

	SetWindowRegion();

	return 0;
}

void CNClock::SetMini(BOOL bMini)
{
	if (m_bSmall != bMini)
	{
		m_bSmall = bMini;
		SetWindowRegion();
		Invalidate();
	}
	m_bSmall = bMini;
	SetTrace(m_trace);
}

UINT CNClock::OnNcHitTest(CPoint point) 
{
	return CWnd::OnNcHitTest(point);
}

void CNClock::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bOverCalendar)
		((CMainFrame*)AfxGetMainWnd())->m_mapHelper->ChangeChild("IB880800");
	else
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	

	CWnd::OnLButtonDown(nFlags, point);
}

int CNClock::GetAMPM(int hour)
{
	if (hour >= 0 && hour < 12)
		return CLOCK_AM;
	else
		return CLOCK_PM;
}

void CNClock::UpdateBackground(CDC* dc)
{
	ASSERT(m_bmpBackground && m_bmpBackgroundMini);

	CRect rc;
	GetClientRect(&rc);

	CDC mdc;
	mdc.CreateCompatibleDC(dc);

	if (m_back.m_hObject)
		m_back.DeleteObject();

	m_back.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());

	CBitmap* oldBmp = mdc.SelectObject(&m_back);

	if (m_bSmall)
	{
		DrawImg(&mdc, m_bmpBackgroundMini);

		const char* ampm[] = {
			"AM", "PM"
		};
		CFont* oldFont = mdc.SelectObject(&m_fontMini);
		mdc.SetTextColor(RGB(254, 186, 6));
		mdc.SetBkMode(TRANSPARENT);
		mdc.DrawText(CString(ampm[m_currentAMPM]), CRect(6, 3, 300, 100), DT_LEFT | DT_TOP);
		mdc.SelectObject(oldFont);
	}
	else
	{
		DrawImg(&mdc, m_bSmall ? m_bmpBackgroundMini: m_bmpBackground);
		DrawImg(&mdc, m_bmpAMPM[m_currentAMPM], 16, m_currentAMPM == CLOCK_AM ? 11 : 25);
	}

	const int weekdayPos[7] = 
	{
		15, 39, 63, 86, 111, 135, 159 //** 이미지상 각 요일마다의 X 좌표 
	};
	const int weekdayY = 41;		  //** 이미지상 각 요일의 Y 좌표
	
	DrawImg(&mdc, m_bmpWeekday[m_currentWeekday], weekdayPos[m_currentWeekday], weekdayY);
	DrawImg(&mdc, m_bOverCalendar ? m_bmpCalendar[1] : m_bmpCalendar[0], CAL_LEFT, CAL_TOP);

	mdc.SelectObject(oldBmp);
}

void CNClock::DrawImg(CDC* dc, CBitmap* bmp, int x, int y)
{
	BITMAP bm;
	bmp->GetBitmap(&bm);

	CDC mdc;
	mdc.CreateCompatibleDC(dc);

	CBitmap* oldBmp = mdc.SelectObject(bmp);

	dc->BitBlt(x, y, bm.bmWidth, bm.bmHeight, &mdc, 0, 0, SRCCOPY);

	mdc.SelectObject(oldBmp);
}

void CNClock::SetWindowRegion()
{
	CBitmap* bmp = (m_bSmall ? m_bmpBackgroundMini : m_bmpBackground);

	ASSERT(bmp);

	HRGN hWndRgn = DIBToRgn((HBITMAP)bmp->m_hObject, Axis::maskColor, FALSE);
	if (hWndRgn)
	{
		SetWindowRgn(hWndRgn, TRUE);
		::DeleteObject(hWndRgn);
	}
	else
	{
		SetWindowRgn(NULL, TRUE);
	}

	m_currentAMPM = -1;
	m_currentWeekday = -1;
}

void CNClock::CreateBoldFont(CFont& font, int point, const char* fontName)
{
	LOGFONT	lf;

	font.CreatePointFont(point, fontName);
	font.GetLogFont(&lf);
	font.DeleteObject();

	font.CreateFont(	lf.lfHeight,			// nHeight
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
				(char*)(const char*)fontName);        // lpszFacename//*/

}

void CNClock::SetTrace(BOOL bTrace)
{
	m_trace = bTrace;

	AfxGetMainWnd()->GetWindowRect(&m_rcMain);
}

void CNClock::OnMainWndMoved()
{
	if (m_rcMain.left == INIT_POSITION)
	{
		AfxGetMainWnd()->GetWindowRect(&m_rcMain);
		return;
	}

	if (!m_trace)
		return;

	CRect rc;
	AfxGetMainWnd()->GetWindowRect(&rc);

	int cx = rc.left - m_rcMain.left;
	int cy = rc.top - m_rcMain.top;

	CRect r;
	GetWindowRect(&r);
	r.OffsetRect(cx, cy);

	//SetWindowPos(NULL, r.left, r.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	BITMAP	bm;
	CRect	wRc;
	
	m_bmpBackground->GetBitmap(&bm);
	GetDesktopWindow()->GetWindowRect(&wRc);
	SetWindowPos(&wndTopMost, wRc.right - bm.bmWidth, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE|SWP_HIDEWINDOW);

	m_rcMain = rc;
}

const char* szClock = "Clock";

void CNClock::Load()
{
	CProfile profile(pkUserConfig);

	m_bSmall = profile.GetInt(szClock, "Mini");
	m_trace = profile.GetInt(szClock, "Trace");
}

void CNClock::Save()
{
	CProfile profile(pkUserConfig);

	profile.Write(szClock, "Mini", m_bSmall);
	profile.Write(szClock, "Trace", m_trace);
}

void CNClock::OnMouseMove(UINT nFlags, CPoint point) 
{
	BOOL bOver = (CAL_LEFT<=point.x && point.x<=CAL_RIGHT) && (CAL_TOP<=point.y && point.y<=CAL_BOTTOM);
	if (bOver!=m_bOverCalendar)
	{
//		OutputDebugString("Change!!\n");
		m_bOverCalendar = bOver;

		CDC *pDC = GetDC();
		UpdateBackground(pDC);
		ReleaseDC(pDC);
		InvalidateRect(CRect(CAL_LEFT, CAL_TOP, CAL_RIGHT, CAL_BOTTOM), FALSE);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

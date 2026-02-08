// StgInfo.cpp : implementation file
//

#include "stdafx.h"
#include "IB410100.h"
#include "StgInfo.h"
#include "message.h"
#include "tooltip.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IMG_FILE	"전략상태.bmp"
#define IMG_WIDTH	382
#define IMG_HEIGHT	518 //389
#define	ROW_HEIGHT	30

#define TID_TOOLTIP	10

const int ColPoint[] = {0, 37, 123, 210, 295, IMG_WIDTH};
// 스트립매수 3,거트매수 10,콘돌매수 11,버터플라이매수 13,콘돌매수 18
// 거트매도 19,스트랩매수 22,콜풋레이쇼합성 27,컨버젼 28,리버설 29,데빗박스 30,크레딧박스 33
const int StgCode[13][5] = {
		{0, 0, 0, 0, 0},
		{0, 1, 20, 0, 8},
		{0, 2, 21, 0, 9},
		{0, 0, 0, 0, 12},
		{0, 7, 26, 14,0},
		{0, 17, 16, 15, 0},
		{0, 0, 0, 34, 0},	//13 --> 34 콜버터플라이 2014.04.16 KSJ
		{0, 0, 0, 35, 0},	//13 --> 35 풋버터플라이 2014.04.16 KSJ
		{0, 0, 0, 13, 0},
		{0, 4, 23, 0, 0},
		{0, 5, 25, 0, 0},
		{0, 6, 24, 0, 0},
		{0, 31, 32, 0, 0}
	};

const  char *StgText[13][5] = {
		{"", "", "", "", ""},
		{"", "콜매수", "풋매수", "", "스트래들매수"},
		{"", "콜백스프레드", "풋백스프레드", "", "스트랭글매수"},
		{"", "", "", "", "버터플라이매도"},
		{"", "풋매도", "콜매도", "스트래들매도", ""},
		{"", "풋레이쇼스프레드", "콜레이쇼스프레드", "스트랭글매도", ""},
		{"", "", "", "콜버터플라이매수", ""},
		{"", "", "", "풋버터플라이매수", ""},
		{"", "", "", "콜-풋버터플라이매수", ""},
		{"", "선물매수", "선물매도", "", ""},
		{"", "강세콜스프레드", "약세풋스프레드", "", ""},
		{"", "강세풋스프레드", "약세콜스프레드", "", ""},
		{"", "커버드콜", "방어적풋", "", ""}
	};
/////////////////////////////////////////////////////////////////////////////
// CStgInfo

CStgInfo::CStgInfo(CWnd* parent, CFont* pfont)
{
	m_pParent = parent;
	m_Scroll = 1;
	m_pBitmap = nullptr;
	m_pToolTip = nullptr;
	m_rect = CRect(0, 0, 0, 0);
	m_pFont = pfont;
}

CStgInfo::~CStgInfo()
{
}


BEGIN_MESSAGE_MAP(CStgInfo, CWnd)
	//{{AFX_MSG_MAP(CStgInfo)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStgInfo message handlers

int CStgInfo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_sRoot = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);

	GetClientRect(&m_rect);

	SCROLLINFO info;
	info.cbSize = 28;
	info.fMask = SIF_ALL;
	info.nMin = 0;
	info.nMax = IMG_HEIGHT;
	info.nPage = m_rect.Height();
	info.nPos = 0;
	info.nTrackPos = 0;

	SetScrollInfo(SB_VERT, &info);

//	m_pToolTip = std::make_unique<class CToolTip>(nullptr, this, m_pFont);
	m_pToolTip = new class CToolTip(nullptr, this, m_pFont);

	m_pToolTip->Create();
	m_pToolTip->ShowWindow(SW_HIDE);
	
	return 0;
}

void CStgInfo::OnDestroy() 
{
	//DELOBJ(m_pBitmap);
	DELWND(m_pToolTip);

	CWnd::OnDestroy();
}

void CStgInfo::DrawInfo(CDC* pDC)
{
	if (m_pBitmap)
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);

		CBitmap* pbitmapOld= dcMem.SelectObject(m_pBitmap.get());

		pDC->BitBlt(0, 0 - m_Scroll, IMG_WIDTH, IMG_HEIGHT, &dcMem, 0, 0, SRCCOPY);

		dcMem.SelectObject(pbitmapOld);
		dcMem.DeleteDC();
	}
	else
	{
		CRect rc;
		GetClientRect(&rc);
		pDC->FillSolidRect(rc, getIndexColor(30));
	}
}

BOOL CStgInfo::LoadImage()
{
	CString	filename;
	filename.Format("%s/image/%s", m_sRoot, IMG_FILE);

	//DELOBJ(m_pBitmap);
	m_pBitmap = nullptr;

	HBITMAP	hBmp;
	hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_pBitmap = std::make_unique<CBitmap>();
	m_pBitmap->Attach(hBmp);

	if (m_pBitmap)
	{
		SetScrollPos(SB_VERT, 0);
		m_Scroll = 0;

		Invalidate();
		return TRUE;
	}
	else
	{
		Invalidate();
		return FALSE;
	}
}

void CStgInfo::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nBar = 0;

	switch (nSBCode)
	{
	case SB_LINEUP:
		nBar = GetScrollPos(SB_VERT);
		SetScrollPos(SB_VERT, nBar - 10);
		break;
	case SB_LINEDOWN:
		nBar = GetScrollPos(SB_VERT);
		SetScrollPos(SB_VERT, nBar + 10);
		break;
	case SB_PAGEUP:
		nBar = GetScrollPos(SB_VERT);
		SetScrollPos(SB_VERT, nBar - m_rect.Height());
		break;
	case SB_PAGEDOWN:
		nBar = GetScrollPos(SB_VERT);
		SetScrollPos(SB_VERT, nBar + m_rect.Height());
		break;
	case SB_THUMBTRACK:
		SetScrollPos(SB_VERT, nPos);
		break;
	}

	nBar = GetScrollPos(SB_VERT);
	const double pos = ((double)(IMG_HEIGHT - m_rect.Height()) / (double)IMG_HEIGHT) * nBar;
	m_Scroll = (int)pos;
	Invalidate();
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CStgInfo::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	const int	wheel = 30;
	int	nBar = GetScrollPos(SB_VERT);

	if(zDelta > 0)
		SetScrollPos(SB_VERT, nBar - wheel);
	else
		SetScrollPos(SB_VERT, nBar + wheel);

	nBar = GetScrollPos(SB_VERT);
	const double pos = ((double)(IMG_HEIGHT - m_rect.Height()) / (double)IMG_HEIGHT) * nBar;
	m_Scroll = (int)pos;
	Invalidate();
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CStgInfo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);

	dc.FillSolidRect(rc, RGB(255, 255, 255));

	DrawInfo(&dc);
}

COLORREF CStgInfo::getIndexColor(int index)
{
	if (index & 0x02000000)
		return index;
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CPoint CStgInfo::GetCell(CPoint pt)
{
	int nCol=0, nRow=0;
	for (nCol = 0; nCol < 5; nCol++)
		if (pt.x < ColPoint[nCol+1])
			break;

	nRow = (int)((pt.y + m_Scroll) / ROW_HEIGHT);
	
	return CPoint(nCol, nRow);
}

void CStgInfo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (point.x >= 0)
	{
		int nStgCode;
		const CPoint pt = GetCell(point);
		nStgCode = StgCode[pt.y][pt.x];
		GetParent()->SendMessage(WM_USER + 1, MSG_SELECTSTG, nStgCode);
	}
		
	CWnd::OnLButtonDown(nFlags, point);
}

void CStgInfo::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_prvPoint = point;
	KillTimer(TID_TOOLTIP);
	SetTimer(TID_TOOLTIP, 100, NULL);

	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CStgInfo::OnMouseLeave(WPARAM wParam, LPARAM lParam) 
{
	if (m_pToolTip->IsWindowVisible())
	{
		ShowToolTip(false);
	}
	return 0;
}

void CStgInfo::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TID_TOOLTIP:
		KillTimer(TID_TOOLTIP);
		ShowToolTip(true);
		break;
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CStgInfo::ShowToolTip(bool show)
{
	if (!show)
	{
		if (m_pToolTip->IsWindowVisible())
			m_pToolTip->ShowWindow(SW_HIDE);
		return;
	}

	CPoint pt;
	CRect rcClient;
	GetClientRect(&rcClient);
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	if (pt.x < 0 || pt.y < 0 || pt.x > rcClient.Width() || pt.y > rcClient.Height())
	{
		if (m_pToolTip->IsWindowVisible())
			m_pToolTip->ShowWindow(SW_HIDE);
		return;
	}

	if (m_prvPoint.x != pt.x || m_prvPoint.y != pt.y)
	{
		return;
	}

	CString sToolTip;
	pt = GetCell(pt);
	sToolTip.Format("%s", StgText[pt.y][pt.x]);
	if (sToolTip.IsEmpty())
	{
		if (m_pToolTip->IsWindowVisible())
			m_pToolTip->ShowWindow(SW_HIDE);
	}
	else
	{
		sToolTip += "\t";

		CRect rect;
		CSize size;
		rect = m_pToolTip->SetData(sToolTip);

		size.cx = rect.Width();
		size.cy = rect.Height();

		GetCursorPos(&pt);
		rect.left = pt.x + 20;
		rect.right = rect.left + size.cx;
		rect.top = pt.y + 20;
		rect.bottom = rect.top + size.cy;
	
		m_pToolTip->SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);
		if (m_pToolTip->IsWindowVisible())
			m_pToolTip->Invalidate(FALSE);
		else
			m_pToolTip->ShowWindow(SW_SHOWNOACTIVATE);
	}
}
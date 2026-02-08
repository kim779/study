// StgBoard.cpp : implementation file
//

#include "stdafx.h"
#include "IB410100.h"
#include "StgBoard.h"
#include "message.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ROW_HEIGHT	30
#define ROW_CNT		13
#define BOARD_HEIGHT	((ROW_HEIGHT) * ROW_CNT)

const int ColPoint[] = {0, 30, 130, 240, 371};
const int StgCode[13][4] = {
		{0, 0, 0, 0},
		{0, 1, 20, 8},
		{0, 2, 21, 9},
		{0, 7, 26, 12},
		{0, 0/*"풋비율스프레드"*/, 0/*"콜비율스프레드"*/, 14},
		{0, 0, 0, 15},
		{0, 0, 0, 0/*"콜버터플라이매수"*/},
		{0, 0, 0, 0/*"풋버터플라이매수"*/},
		{0, 0, 0, 0/*"콜-풋버터플라이매수"*/},
		{0, 4, 23, 0},
		{0, 5, 25, 0},
		{0, 6, 24, 0},
		{0, 31, 32, 0}
	};


/////////////////////////////////////////////////////////////////////////////
// CStgBoard

CStgBoard::CStgBoard(CWnd* parent)
{
	m_pParent = parent;
	m_bmpBoard = NULL;
	m_Scroll = 0;
	m_rect = CRect(0, 0, 0, 0);
}

CStgBoard::~CStgBoard()
{
}


BEGIN_MESSAGE_MAP(CStgBoard, CWnd)
	//{{AFX_MSG_MAP(CStgBoard)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStgBoard message handlers

int CStgBoard::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	GetClientRect(&m_rect);

	struct _fontR fontR;
	fontR.name = "굴림";
	fontR.point = 9; 
	fontR.italic = false; 
	fontR.bold = 0;
	m_pFont = (CFont *)m_pParent->SendMessage(WM_USER, getFONT, (long)&fontR);

	SCROLLINFO info;
	info.cbSize = 28;
	info.fMask = SIF_ALL;
	info.nMin = 0;
	info.nMax = BOARD_HEIGHT;
	info.nPage = m_rect.Height();
	info.nPos = 0;
	info.nTrackPos = 0;

	SetScrollInfo(SB_VERT, &info);

	return 0;
}

void CStgBoard::OnDestroy() 
{
	//DELOBJ(m_bmpBoard);

	CWnd::OnDestroy();
}

BOOL CStgBoard::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	const int	wheel = 30;
	int	nBar = GetScrollPos(SB_VERT);

	if(zDelta > 0)
		SetScrollPos(SB_VERT, nBar - wheel);
	else
		SetScrollPos(SB_VERT, nBar + wheel);

	nBar = GetScrollPos(SB_VERT);
	const double pos = ((double)(BOARD_HEIGHT - m_rect.Height()) / (double)(BOARD_HEIGHT - m_rect.Height())) * nBar;
	m_Scroll = (int)pos;
	Invalidate();
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CStgBoard::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
	const double pos = ((double)(BOARD_HEIGHT - m_rect.Height()) / (double)(BOARD_HEIGHT - m_rect.Height())) * nBar;

	m_Scroll = (int)pos;
	Invalidate();
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CStgBoard::OnPaint() 
{
	CPaintDC dc(this); // device context for painting


	CDC dcMem;
	CBitmap* pbitmapOld = nullptr;
	dcMem.CreateCompatibleDC(&dc);
	if (m_bmpBoard == NULL)
	{

		m_bmpBoard = std::make_unique<CBitmap>();
		m_bmpBoard->CreateCompatibleBitmap(&dc, m_rect.Width(), BOARD_HEIGHT);

		pbitmapOld = dcMem.SelectObject(m_bmpBoard.get());
		drawSelection(&dcMem);
	}
	else
		pbitmapOld = dcMem.SelectObject(m_bmpBoard.get());

	dc.BitBlt(0, 0 - m_Scroll, m_rect.Width(), BOARD_HEIGHT, &dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(pbitmapOld);
	dcMem.DeleteDC();
}

COLORREF CStgBoard::getIndexColor(int index)
{
	if (index & 0x02000000)
		return index;
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

void CStgBoard::drawSelection(CDC *dc)
{
	CRect rc, rcBoard;
	GetClientRect(&rcBoard); rcBoard.bottom = rcBoard.top + BOARD_HEIGHT;
	CPen	*open, pen(PS_SOLID, 1, getIndexColor(65));
	open = dc->SelectObject(&pen);
	CFont	*ofont;
	ofont = dc->SelectObject(m_pFont);
	CBrush br(getIndexColor(65));
	const COLORREF bkClr = getIndexColor(30);
	const COLORREF bkTitle = getIndexColor(74);

	int nTop=0;
	int nBottom = 0, nRow = 0;
	const int nRowCnt = 13;
	// 1(bottom), 2(right)
	// 1 + 2 = 3(right + bottom)
	const int LineFormat[13][4] = {
		{3, 3, 3, 3},
		{2, 3, 3, 3},
		{3, 3, 3, 3},
		{2, 3, 3, 3},
		{2, 3, 3, 3},
		{2, 0, 2, 3},
		{2, 0, 2, 3},
		{2, 0, 2, 3},
		{3, 1, 3, 3},
		{2, 3, 3, 2},
		{2, 3, 3, 2},
		{3, 3, 3, 2},
		{3, 3, 3, 3}
	};

	CString CELLTEXT[13][4] = {
		{"◆", "상승", "하락", "횡보"},
		{"확", "콜매수", "풋매수", "스트래들매수"},
		{"대", "콜백스프레드", "풋백스프레드", "스트랭글매수"},
		{"", "풋매도", "콜매도", "버터플라이매도"},
		{"", "풋레이쇼스프레드", "콜레이쇼스프레드", "스트래들매도"},
		{"축", "", "", "스트랭글매도"},
		{"소", "", "", "콜버터플라이매수"},
		{"", "", "", "풋버터플라이매수"},
		{"", "", "", "콜-풋버터플라이매수"},
		{"불", "선물매수", "선물매도", ""},
		{"확", "강세콜스프레드", "약세풋스프레드", ""},
		{"실", "강세풋스프레드", "약세콜스프레드", ""},
		{"헷지", "커버드콜", "방어적풋", ""}
	};

	dc->FillSolidRect(rcBoard, bkClr);
	
	nRow = 0;
	nBottom = -1;
	int line=0;
	CString text;
	while (nRow < nRowCnt)
	{
		nTop = nBottom;
		nBottom = nTop + ROW_HEIGHT;
		for (int nCol = 0; nCol < 4; nCol++)
		{
			rc = CRect(ColPoint[nCol]+1, nTop, ColPoint[nCol+1], nBottom);
			if (nRow == 0 || nCol == 0)
				dc->FillSolidRect(rc, bkTitle);
			else
				dc->FillSolidRect(rc, bkClr);
			line = LineFormat[nRow][nCol];
			if ((line & 1) == 1) // bottom line
			{
				dc->MoveTo(rc.left, rc.bottom-1);
				dc->LineTo(rc.right, rc.bottom-1);
			}

			if ((line & 2) == 2) // right line
			{
				dc->MoveTo(rc.right, rc.top);
				dc->LineTo(rc.right, rc.bottom);
			}
			rc.DeflateRect(1, 1);
			text = CELLTEXT[nRow][nCol];
			dc->DrawText(text, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		}
		nRow++;
	}
	dc->MoveTo(rcBoard.left, rcBoard.bottom);
	dc->LineTo(rcBoard.left, rcBoard.top);
	dc->LineTo(rcBoard.right, rcBoard.top);

	dc->SelectObject(open);
	dc->SelectObject(ofont);
}

void CStgBoard::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (point.x >= 0)
	{
		int nCol=0, nRow=0, nStgCode=0;
		for (nCol = 0; nCol < 4; nCol++)
			if (point.x < ColPoint[nCol+1])
				break;

		nRow = (int)((point.y + m_Scroll) / ROW_HEIGHT);
		nStgCode = StgCode[nRow][nCol];
		GetParent()->SendMessage(WM_USER + 1, MSG_SELECTSTG, nStgCode);
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

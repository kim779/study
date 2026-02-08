// MGrpWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB410100.h"
#include "MGrpWnd.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMGrpWnd

CMGrpWnd::CMGrpWnd(CWnd* pView, CWnd* parent)
{
	m_pApp = (CIB410100App *)AfxGetApp();
	m_pParent = parent;
	m_pView   = pView;

	m_bRedraw = true;
	m_bmpChart = NULL;
	m_nStgCode = 1;
}

CMGrpWnd::~CMGrpWnd()
{
}


BEGIN_MESSAGE_MAP(CMGrpWnd, CWnd)
	//{{AFX_MSG_MAP(CMGrpWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMGrpWnd message handlers

int CMGrpWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bkColor = m_pApp->GetIndexColor(m_pView, 66);

	return 0;
}

void CMGrpWnd::OnDestroy() 
{
	m_bmpChart = nullptr;
	CWnd::OnDestroy();
}

void CMGrpWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);

	CDC dcMem;
	CBitmap* pbitmapOld = nullptr;
	dcMem.CreateCompatibleDC(&dc);
	if (m_bmpChart == NULL || m_bRedraw)
	{
		m_bRedraw = false;
		m_bmpChart = nullptr;

		m_bmpChart = std::make_unique<CBitmap>();
		m_bmpChart->CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());

		pbitmapOld = dcMem.SelectObject(m_bmpChart.get());
		drawChart(&dcMem, rc);
	}
	else
		pbitmapOld = dcMem.SelectObject(m_bmpChart.get());

	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(pbitmapOld);
	dcMem.DeleteDC();
}

void CMGrpWnd::Redraw()
{
	m_bRedraw = true;
	Invalidate();
}

void CMGrpWnd::drawChart(CDC *dc, CRect rcClient)
{
	CRect rc(rcClient);
	rc.DeflateRect(10, 10);

	dc->FillSolidRect(&rcClient, GetSysColor(COLOR_BTNFACE));
	dc->FillSolidRect(&rc, m_bkColor);

	int zeroX=0, zeroY=0;
	CPen *open, pen, penZero;
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	dc->SetTextColor(RGB(118, 145, 128));
	open = dc->SelectObject(&pen);
	switch (m_nStgCode)
	{
	case 1:	// 콜매수
	case 32:// 방어적인풋매수
		zeroX = 50; zeroY = 40; 
		DrawZero(dc, rc, zeroY, zeroX);
		dc->MoveTo(rc.left, GetPerentY(rc, 30));
		dc->LineTo(GetPerentX(rc, 50), GetPerentY(rc, 30));
		dc->LineTo(rc.right, GetPerentY(rc, 90));
		break;
	case 2: // 콜백스프레드
		// 종목, atmGap, 수량 매매구분
		//addCCode(2, +1, 2, +1);
		//addCCode(2, 0, 1, -1);
		zeroX = 40; zeroY = 40; 
		DrawZero(dc, rc, zeroY, zeroX);
		dc->MoveTo(rc.left, GetPerentY(rc, 42));
		dc->LineTo(GetPerentX(rc, zeroX), GetPerentY(rc, 42));
		dc->LineTo(GetPerentX(rc, 55), GetPerentY(rc, 20));
		dc->LineTo(rc.right, GetPerentY(rc, 80));
		break;
	case 3: // 스트랩매수
		break;
	case 4: // 선물매수
		zeroX = 50; zeroY = 50; 
		DrawZero(dc, rc, zeroY, zeroX);
		dc->MoveTo(rc.left, GetPerentY(rc, 10));
		dc->LineTo(rc.right, GetPerentY(rc, 90));
		break;
	case 5: // 강세콜스프레드
		zeroX = 40; zeroY = 50; 
		DrawZero(dc, rc, zeroY, zeroX);
		dc->MoveTo(rc.left, GetPerentY(rc, 40));
		dc->LineTo(GetPerentX(rc, 40), GetPerentY(rc, 40));
		dc->LineTo(GetPerentX(rc, 60), GetPerentY(rc, 80));
		dc->LineTo(rc.right, GetPerentY(rc, 80));
		break;
	case 6: // 강세풋스프레드
		zeroX = 60; zeroY = 50; 
		DrawZero(dc, rc, zeroY, zeroX, false);
		dc->MoveTo(rc.left, GetPerentY(rc, 20));
		dc->LineTo(GetPerentX(rc, 40), GetPerentY(rc, 20));
		dc->LineTo(GetPerentX(rc, 60), GetPerentY(rc, 60));
		dc->LineTo(rc.right, GetPerentY(rc, 60));
		break;
	case 26: // 풋매도		2013.05.31 KSJ 윤영선대리 풋매도, 콜매도 그림과 그래프 모양이 달라 수정 요청
	case 31: // 커버드콜매도
		zeroX = 50; zeroY = 60; 
		DrawZero(dc, rc, zeroY, zeroX, false);
		dc->MoveTo(rc.left, GetPerentY(rc, 10));
		dc->LineTo(GetPerentX(rc, 50), GetPerentY(rc, 70));
		dc->LineTo(rc.right, GetPerentY(rc, 70));
		break;
	case 8: // 스트래들매수
		zeroX = 50; zeroY = 40; 
		DrawZero(dc, rc, zeroY, zeroX);
		dc->MoveTo(rc.left, GetPerentY(rc, 80));
		dc->LineTo(GetPerentX(rc, 50), GetPerentY(rc, 20));
		dc->LineTo(rc.right, GetPerentY(rc, 80));
		break;
	case 9: // 스트랭글매수
		zeroX = 50; zeroY = 40; 
		DrawZero(dc, rc, zeroY, zeroX);
		dc->MoveTo(rc.left, GetPerentY(rc, 70));
		dc->LineTo(GetPerentX(rc, 37), GetPerentY(rc, 20));
		dc->LineTo(GetPerentX(rc, 63), GetPerentY(rc, 20));
		dc->LineTo(rc.right, GetPerentY(rc, 70));
		break;
	case 10: // 거트매수
		break;
	case 11: // 콘돌매도
		break;
	case 12: // 버터플라이매도
		zeroX = 50; zeroY = 55; 
		DrawZero(dc, rc, zeroY, zeroX);
		dc->MoveTo(rc.left, GetPerentY(rc, 80));
		dc->LineTo(GetPerentX(rc, 30), GetPerentY(rc, 80));
		dc->LineTo(GetPerentX(rc, 50), GetPerentY(rc, 20));
		dc->LineTo(GetPerentX(rc, 70), GetPerentY(rc, 80));
		dc->LineTo(rc.right, GetPerentY(rc, 80));
		break;
	case 13: // 버터플라이매수
	case 34: // 콜버터플라이매수	//2014.04.16 KSJ 추가
	case 35: // 풋버터플라이매수	//2014.04.16 KSJ 추가
		zeroX = 50; zeroY = 45; 
		DrawZero(dc, rc, zeroY, zeroX, false);
		dc->MoveTo(rc.left, GetPerentY(rc, 20));
		dc->LineTo(GetPerentX(rc, 30), GetPerentY(rc, 20));
		dc->LineTo(GetPerentX(rc, 50), GetPerentY(rc, 80));
		dc->LineTo(GetPerentX(rc, 70), GetPerentY(rc, 20));
		dc->LineTo(rc.right, GetPerentY(rc, 20));
		break;
	case 14: // 스트래들매도
		zeroX = 50; zeroY = 60; 
		DrawZero(dc, rc, zeroY, zeroX, false);
		dc->MoveTo(rc.left, GetPerentY(rc, 20));
		dc->LineTo(GetPerentX(rc, 50), GetPerentY(rc, 80));
		dc->LineTo(rc.right, GetPerentY(rc, 20));
		break;
	case 15: // 스트랭글매도
		zeroX = 50; zeroY = 60; 
		DrawZero(dc, rc, zeroY, zeroX, false);
		dc->MoveTo(rc.left, GetPerentY(rc, 30));
		dc->LineTo(GetPerentX(rc, 37), GetPerentY(rc, 80));
		dc->LineTo(GetPerentX(rc, 63), GetPerentY(rc, 80));
		dc->LineTo(rc.right, GetPerentY(rc, 30));
		break;
	case 16: // 콜레이쇼스프레드
		zeroX = 60; zeroY = 40; 
		DrawZero(dc, rc, zeroY, zeroX, false);
		dc->MoveTo(rc.right, GetPerentY(rc, 10));
		dc->LineTo(GetPerentX(rc, zeroX), GetPerentY(rc, 80));
		dc->LineTo(GetPerentX(rc, 45), GetPerentY(rc, 45));
		dc->LineTo(rc.left, GetPerentY(rc, 45));
		break;
	case 17: // 풋레이쇼스프레드
		zeroX = 40; zeroY = 40; 
		DrawZero(dc, rc, zeroY, zeroX, false);
		dc->MoveTo(rc.left, GetPerentY(rc, 10));
		dc->LineTo(GetPerentX(rc, zeroX), GetPerentY(rc, 80));
		dc->LineTo(GetPerentX(rc, 55), GetPerentY(rc, 45));
		dc->LineTo(rc.right, GetPerentY(rc, 45));
		break;
	case 18: // 콘돌매수
		break;
	case 19: // 거트매도
		break;
	case 20: // 풋매수
		zeroX = 50; zeroY = 40; 
		DrawZero(dc, rc, zeroY, zeroX);
		dc->MoveTo(rc.left, GetPerentY(rc, 90));
		dc->LineTo(GetPerentX(rc, 50), GetPerentY(rc, 30));
		dc->LineTo(rc.right, GetPerentY(rc, 30));
		break;
	case 21: // 풋백스프레드
		// 종목, atmGap, 수량 매매구분
		//addCCode(2, +1, 2, +1);
		//addCCode(2, 0, 1, -1);
		zeroX = 60; zeroY = 40; 
		DrawZero(dc, rc, zeroY, zeroX);
		dc->MoveTo(rc.right, GetPerentY(rc, 42));
		dc->LineTo(GetPerentX(rc, zeroX), GetPerentY(rc, 42));
		dc->LineTo(GetPerentX(rc, 45), GetPerentY(rc, 20));
		dc->LineTo(rc.left, GetPerentY(rc, 80));
		break;
	case 22: // 스트립매수
		break;
	case 23: // 선물매도
		zeroX = 50; zeroY = 50; 
		DrawZero(dc, rc, zeroY, zeroX);
		dc->MoveTo(rc.left, GetPerentY(rc, 90));
		dc->LineTo(rc.right, GetPerentY(rc, 10));
		break;
	case 24: // 약세콜스프레드
		zeroX = 60; zeroY = 50; 
		DrawZero(dc, rc, zeroY, zeroX);
		dc->MoveTo(rc.right, GetPerentY(rc, 30));
		dc->LineTo(GetPerentX(rc, 60), GetPerentY(rc, 30));
		dc->LineTo(GetPerentX(rc, 40), GetPerentY(rc, 70));
		dc->LineTo(rc.left, GetPerentY(rc, 70));
		break;
	case 25: // 약세풋스프레드
		zeroX = 60; zeroY = 50; 
		DrawZero(dc, rc, zeroY, zeroX);
		dc->MoveTo(rc.right, GetPerentY(rc, 40));
		dc->LineTo(GetPerentX(rc, 60), GetPerentY(rc, 40));
		dc->LineTo(GetPerentX(rc, 40), GetPerentY(rc, 80));
		dc->LineTo(rc.left, GetPerentY(rc, 80));
		break;
	case 7: // 콜매도		2013.05.31 KSJ 윤영선대리 풋매도, 콜매도 그림과 그래프 모양이 달라 수정 요청
		zeroX = 50; zeroY = 60; 
		DrawZero(dc, rc, zeroY, zeroX, false);
		dc->MoveTo(rc.left, GetPerentY(rc, 70));
		dc->LineTo(GetPerentX(rc, 50), GetPerentY(rc, 70));
		dc->LineTo(rc.right, GetPerentY(rc, 10));
		break;
	case 27: // 콜풋레이쇼합성
		break;
	case 28: // 컨버젼
		break;
	case 29: // 리버설
		break;
	case 30: // 데빗박스
		break;
	case 33: // 크레딧박스
		break;
	}
	dc->SelectObject(open);
}

int CMGrpWnd::GetPerentX(CRect rcDraw, double perX)
{
	return rcDraw.left + (int)((rcDraw.right - (rcDraw.left -1)) * perX / 100);
}

int CMGrpWnd::GetPerentY(CRect rcDraw, double perY)
{
	return rcDraw.bottom - (int)((rcDraw.bottom - (rcDraw.top -1)) * perY / 100);
}

void CMGrpWnd::DrawZero(CDC *dc, CRect rcDraw, int zeroY, int x, bool upText, CString text)
{
	CSize szText;
	CRect rcText;
	CPen *open, penZero;
	penZero.CreatePen(PS_SOLID, 2, RGB(118, 145, 128));
	open = dc->SelectObject(&penZero);
	dc->MoveTo(rcDraw.left, GetPerentY(rcDraw, zeroY));
	dc->LineTo(rcDraw.right, GetPerentY(rcDraw, zeroY));

	x = GetPerentX(rcDraw, x);
	dc->MoveTo(x, GetPerentY(rcDraw, zeroY -2));
	dc->LineTo(x, GetPerentY(rcDraw, zeroY +2));

	szText = dc->GetTextExtent(text);
	if (upText)
	{
		zeroY = GetPerentY(rcDraw, zeroY + 3);
		rcText = CRect(x - (szText.cx / 2 - 1),  (zeroY - szText.cy), x + (szText.cx / 2 + 1), zeroY);
	}
	else
	{
		zeroY = GetPerentY(rcDraw, zeroY - 3);
		rcText = CRect(x - (szText.cx / 2 - 1),  zeroY, x + (szText.cx / 2 + 1), (zeroY + szText.cy));
	}
	dc->DrawText(text, -1, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	dc->SelectObject(open);
}

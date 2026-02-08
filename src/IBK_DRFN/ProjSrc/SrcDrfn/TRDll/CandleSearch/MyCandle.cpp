// MyCandle.cpp : implementation file
//

#include "stdafx.h"
#include "MyCandle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyCandle
#define COLOR_FOCUSCOLOR RGB(245,245,220)
//////////////////////////////////////////////////////////////////////////
int CandleSet_DEFAULT[MAX_CANDLE][5] = {  // enable, 시, 고, 저, 종
	{FALSE, 35, 90, 10, 65} , {FALSE, 35, 90, 10, 65}, {FALSE,  35, 90, 10, 65},
	{FALSE, 35, 90, 10, 65},  {TRUE,  35, 90, 10, 65}};

#define CANDLE_DISABLE_LINE RGB(220,220,220)
#define CANCLE_DISABLE_BK1  RGB(220,220,220)
#define CANCLE_DISABLE_BK2	RGB(230,230,230)

#define HIT_NONE	0
#define HIT_HIGH	1
#define HIT_LOW		2
#define HIT_OPEN	3
#define HIT_CLOSE	4
#define HIT_BODY	5

CMyCandle::CMyCandle()
{
	m_bEnable = TRUE;
	
	m_nHigh = 90;
	m_nLow = 10;
	m_nOpen = 30;
	m_nClose = 70;

	m_bReverse = FALSE;
	m_bDragMode = FALSE;
	m_bRButtonClick = FALSE;

	m_bFocus = FALSE;
	
	m_bYScale = FALSE;
	m_nYScaleWidth = 0;

	m_bTopCheckBox = TRUE;

	m_nParkingMargin = 9; // 아이콘을 위한 상하 여백
	if(m_bTopCheckBox) m_nCheckMargin = 20;
	else m_nCheckMargin = 0;
}

CMyCandle::~CMyCandle()
{
}


BEGIN_MESSAGE_MAP(CMyCandle, CWnd)
	//{{AFX_MSG_MAP(CMyCandle)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
#if (_MFC_VER >= 0x0421)
    ON_WM_MOUSEWHEEL()
#endif
END_MESSAGE_MAP()

void CMyCandle::SetCandleData(int nIndex, BOOL bCheck, int nHigh, int nLow, int nOpen, int nClose)
{
	m_nCandleIndex = nIndex;
	m_bEnable = bCheck;

	if(!m_bEnable)
	{
		m_nHigh = CandleSet_DEFAULT[0][2];
		m_nClose = CandleSet_DEFAULT[0][4];
		m_nOpen = CandleSet_DEFAULT[0][1];
		m_nLow = CandleSet_DEFAULT[0][3];
	}
	else
	{
		m_nHigh = nHigh;
		m_nLow = nLow;
		m_nOpen = nOpen;
		m_nClose = nClose;
	}

	if(m_nOpen > m_nClose) m_bReverse = TRUE;
	else m_bReverse = FALSE;

	CalcRect();

	m_clrBackGround = (m_bEnable)?RGB(255,255,255):RGB(240,240,240);

	Invalidate();
}

void CMyCandle::SetYScale(BOOL bShow, int nWidth)
{
	m_bYScale = bShow;
	m_nYScaleWidth = (bShow)?nWidth:0;
}

void CMyCandle::SetCandleData(int nIndex, CANDLE_DATA cd)
{
	SetCandleData(nIndex, cd.bEnable, cd.nHigh, cd.nLow, cd.nOpen, cd.nClose);
}

/////////////////////////////////////////////////////////////////////////////
// CMyCandle message handlers

int CMyCandle::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcClient;
	GetClientRect(rcClient);

	m_rcDrawingArea = rcClient;
	m_rcDrawingArea.DeflateRect(1, m_nParkingMargin+m_nCheckMargin, 1, m_nParkingMargin);
	if(m_bYScale) m_rcDrawingArea.DeflateRect(0,m_nYScaleWidth,0,0);

	// hight per 1%
	m_dHeight = m_rcDrawingArea.Height() / 100.;
	// space for left/right side
	m_nLRSpace = static_cast<int>((m_rcDrawingArea.Width() * 1.7)/5.);

	CalcRect();

	// create check button
	if(m_bYScale)
		m_ptCheck = CPoint((rcClient.Width() - 13 - m_nYScaleWidth)/2, rcClient.top + 4);
	else
		m_ptCheck = CPoint((rcClient.Width() - 13)/2, rcClient.top + 4);

	// Loading Bitmap
	m_bitmapHigh.LoadBitmap(IDB_BITMAP_HIGH);
	m_bitmapLow.LoadBitmap(IDB_BITMAP_LOW);
	m_bitmapOpen.LoadBitmap(IDB_BITMAP_OPEN);
	m_bitmapClose.LoadBitmap(IDB_BITMAP_CLOSE);

	// create image list
	m_ImageList.Create(16,16, ILC_COLOR24 | ILC_MASK, 4, 1);
	m_ImageList.Add(&m_bitmapHigh, RGB(0,128,128));
	m_ImageList.Add(&m_bitmapLow, RGB(0,128,128));
	m_ImageList.Add(&m_bitmapOpen, RGB(0,128,128));
	m_ImageList.Add(&m_bitmapClose, RGB(0,128,128));

	// Create image list for checkbox
	m_CheckImageList.Create(16,16, ILC_COLOR24 | ILC_MASK, 2, 1);
	m_bitmapCheck.LoadBitmap(IDB_BITMAP_CHECK);
	m_CheckImageList.Add(&m_bitmapCheck, RGB(255,0,255));

	// load cursor
	m_hCursor = ::LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_CURSOR_MOVE));
	m_hCursorCross = ::LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_CURSOR_CROSS));
	m_hCursorCrossR = ::LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_CURSOR_CROSSR));

	return 0;
}

void CMyCandle::CalcRect()
{
	CPoint ptHigh = GetPoint_High();
	CPoint ptLow = GetPoint_Low();
	int yOpen = m_rcDrawingArea.bottom - static_cast<int>(m_nOpen*m_dHeight);
	int yClose = m_rcDrawingArea.bottom - static_cast<int>(m_nClose*m_dHeight);

	m_rcCandle = GetRect_Candle();
	m_rcHigh.SetRect(m_rcDrawingArea.left, ptHigh.y - 7, m_rcCandle.CenterPoint().x, ptHigh.y + 7);
	m_rcLow.SetRect(m_rcDrawingArea.left, ptLow.y - 7, m_rcCandle.CenterPoint().x, ptLow.y + 7);

	m_rcOpen.SetRect(m_rcCandle.CenterPoint().x, yOpen - 7, m_rcDrawingArea.right, yOpen + 7);
	m_rcClose.SetRect(m_rcCandle.CenterPoint().x, yClose - 7, m_rcDrawingArea.right, yClose + 7);
}

void CMyCandle::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	m_ImageList.DeleteImageList();
	m_CheckImageList.DeleteImageList();
}

BOOL CMyCandle::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
}

void CMyCandle::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC memDC(&dc);

	COLORREF clrBkGnd;
	if(m_bFocus && m_bEnable) clrBkGnd = COLOR_FOCUSCOLOR;
	else clrBkGnd = m_clrBackGround;

	CRect rcClient;
	GetClientRect(&rcClient);
	memDC.Rectangle(rcClient);

	rcClient.DeflateRect(1,1,1,1);
	memDC.FillSolidRect(rcClient, clrBkGnd);
	if(m_bYScale)
	{
		memDC.MoveTo(rcClient.right - m_nYScaleWidth, rcClient.top);
		memDC.LineTo(rcClient.right - m_nYScaleWidth, rcClient.bottom);
	}
	memDC.MoveTo(1, rcClient.top + 20);
	memDC.LineTo(rcClient.right, rcClient.top + 20);

	DrawBackground(&memDC);


	if(m_nCandleIndex == MAX_CANDLE-1)
	{
		CRect rcText = m_rcDrawingArea;
		rcText.top = rcText.bottom - 20;
		rcText.OffsetRect(0, 12);

		if(m_rcCandle.bottom < rcText.top)
		{
			CFont* pOldFont = memDC.SelectObject(m_pFont);
			memDC.SetTextColor(RGB(150,150,150));
			memDC.DrawText("기준봉", rcText, DT_SINGLELINE|DT_CENTER);
			memDC.SelectObject(pOldFont);
		}
	}
// 

	//memDC.FillSolidRect(m_rcCandle, clrCandle);
	CRect rcCandle = m_rcCandle;
	rcCandle.InflateRect(0,0,1,1);

	DrawGradientRect(&memDC, rcCandle);
// 	memDC.MoveTo(m_rcCandle.TopLeft());
// 	memDC.LineTo(m_rcCandle.right, m_rcCandle.top);
// 	memDC.LineTo(m_rcCandle.BottomRight());
// 	memDC.LineTo(m_rcCandle.left, m_rcCandle.bottom);
// 	memDC.LineTo(m_rcCandle.TopLeft());
	
 	COLORREF clrCandle = (m_nOpen < m_nClose)? RGB(125,0,0): (m_nOpen == m_nClose ? RGB(0,0,0):RGB(0,0,255));
	if(m_bEnable == FALSE) clrCandle = CANDLE_DISABLE_LINE;

	CPen penLine(PS_SOLID, 1, clrCandle), *pOldPen;
	pOldPen = memDC.SelectObject(&penLine);

	memDC.MoveTo(GetPoint_High());
	memDC.LineTo(GetPoint_High().x, m_rcCandle.top);

	memDC.MoveTo(GetPoint_Low());
	memDC.LineTo(GetPoint_Low().x, m_rcCandle.bottom);

	memDC.SelectObject(pOldPen);

	DrawMark(&memDC);

	m_CheckImageList.Draw(&memDC, m_bEnable?1:0, m_ptCheck,  ILD_NORMAL);

	// Do not call CWnd::OnPaint() for painting messages
}
void CMyCandle::DrawGradientRect(CDC *pDC, CRect rect, BOOL bTopDown)
{
	COLORREF Start, End;
	Start = CANCLE_DISABLE_BK1;
	End   = CANCLE_DISABLE_BK2;

	if(m_bEnable)
	{
		if(m_nOpen < m_nClose)
		{
			Start = RGB(60,0,0);
			End = RGB(255,0,0);
		}
		else if(m_nOpen > m_nClose)
		{
			Start = RGB(0,0,60);
			End = RGB(0,0,255);
		}
		else
		{
			Start = RGB(0,0,0);
			End = RGB(0,0,0);
		}
	}

	if(rect.Height() == 1)
	{
		CPen penLine(PS_SOLID, 1, End),* oldPen;
		oldPen = pDC->SelectObject(&penLine);
		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.right, rect.top);
		pDC->SelectObject(&oldPen);
		return;
	}

	CRect backup = rect;
		
	int r,g,b;//각 3가지 색상의 값의 차이..
	float rStep, gStep, bStep;//각 3가지 값들 각각의 색상값의 차이..
	float fStep;//최대 색상값 차이로 나누어서 한번에 이동할 옵셋..

	//각 생상 차이를 구한다..
	r = (GetRValue(End) - GetRValue(Start));
	g = (GetGValue(End) - GetGValue(Start));
	b = (GetBValue(End) - GetBValue(Start));

	//가장 큰차이가 나는것을 구한다..
	int Max = max( abs(r), max(abs(g), abs(b)) );

	//그릴 사각형을 차이가 나는것 만큼.. 분할할 길이를 구한다..
	if(bTopDown)
		fStep = (float)rect.Height()/(float)Max;     // 상-하 Gradation 
	else 
		fStep = (float)rect.Width()/(float)Max;     // 좌-우 Gradation 

	//각 색상별 단계 차이값..
	rStep = r/(float)Max;
	gStep = g/(float)Max;
	bStep = b/(float)Max;

	//r,g,b를 시작 색으로 재설정...
	r = GetRValue(Start);
	g = GetGValue(Start);
	b = GetBValue(Start);

	//색깔이 변해야 하는 횟수만큼.. 반복 그린다.
	for(int iOnBand=0 ; iOnBand <Max; iOnBand++)  
	{
		if(bTopDown)
 			::SetRect( &rect, rect.left, int(fStep*iOnBand) + backup.top ,rect.right, backup.bottom );// 상-하 Gradation 
 		else     
			::SetRect( &rect, int(fStep*iOnBand) + backup.left, rect.top, rect.right, rect.bottom);//좌-우
		pDC->FillSolidRect( &rect,  RGB( r + rStep*iOnBand, g + gStep*iOnBand, b + bStep*iOnBand ) );
	}
}

void CMyCandle::DrawBackground(CDC* pDC)
{
	COLORREF clrBkGnd;
	if(m_bFocus && m_bEnable) clrBkGnd = COLOR_FOCUSCOLOR;
	else clrBkGnd = m_clrBackGround;
	pDC->FillSolidRect(m_rcDrawingArea, clrBkGnd);
	
	CPen penLine(PS_DOT, 1, RGB(204,204,204)), *pOldPen;
	CPen penDark(PS_DOT, 1, RGB(180,180,180));
	pOldPen = pDC->SelectObject(&penLine);
	for(int i = 0; i < 11; i ++)
	{
		int nY = m_rcDrawingArea.bottom - static_cast<int>(i * 10 * m_dHeight);
		if(i == 0 || i == 5 || i == 10) pDC->SelectObject(&penDark);
		else pDC->SelectObject(&penLine);

		pDC->MoveTo(m_rcDrawingArea.left, nY);
		pDC->LineTo(m_rcDrawingArea.right, nY);

		if(i%2 == 0)
		{
			CRect rcText(m_rcDrawingArea.right+2,nY-6,m_rcDrawingArea.right+m_nYScaleWidth,nY+8);
			rcText.DeflateRect(1,1,1,1);
			pDC->SetBkMode(TRANSPARENT);
			CFont* pOldFont = pDC->SelectObject(m_pFont);
			CString strText;
			strText.Format("%d", 100 - i*10);
			pDC->SetTextColor(RGB(80,80,80));
			pDC->DrawText(strText, rcText, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
			pDC->SelectObject(pOldFont);
		}
	}
	pDC->SelectObject(pOldPen);

}

void CMyCandle::DrawMark(CDC* pDC)
{
	if(!m_bFocus || !m_bEnable) return;

	// Draw image
	CPoint ptHighMark(m_rcDrawingArea.left, m_rcDrawingArea.bottom - static_cast<int>(m_nHigh*m_dHeight) - 7);
	CPoint ptLowMark(m_rcDrawingArea.left, m_rcDrawingArea.bottom - static_cast<int>(m_nLow*m_dHeight) - 7);
	CPoint ptOpenMark(m_rcDrawingArea.right - 13, m_rcDrawingArea.bottom - static_cast<int>(m_nOpen*m_dHeight) - 7);
	CPoint ptCloseMark(m_rcDrawingArea.right - 13, m_rcDrawingArea.bottom - static_cast<int>(m_nClose*m_dHeight) - 7);
	
	m_ImageList.Draw(pDC, 0, ptHighMark, ILD_NORMAL);
	m_ImageList.Draw(pDC, 1, ptLowMark, ILD_NORMAL);
	m_ImageList.Draw(pDC, 2, ptOpenMark, ILD_NORMAL);
	m_ImageList.Draw(pDC, 3, ptCloseMark, ILD_NORMAL);

	//////////////////////////////////////////////////////////////////////////
	// draw arrow
	CPen penLine(PS_SOLID, 1, RGB(160,160,160)), *pOldPen;
	pOldPen = pDC->SelectObject(&penLine);

	// 고가
	pDC->MoveTo(m_rcDrawingArea.left + 14, ptHighMark.y + 7);
	if((!m_bReverse && m_nHigh == m_nClose) || (m_bReverse && m_nHigh == m_nOpen))
	{
		pDC->LineTo(m_rcCandle.left -1, ptHighMark.y + 7);
		pDC->LineTo(m_rcCandle.left -3, ptHighMark.y + 5);
		pDC->LineTo(m_rcCandle.left -3, ptHighMark.y + 9);
		pDC->LineTo(m_rcCandle.left -1, ptHighMark.y + 7);
	}
	else
	{
		pDC->LineTo(m_rcCandle.CenterPoint().x-1, ptHighMark.y + 7);
		pDC->LineTo(m_rcCandle.CenterPoint().x-3, ptHighMark.y + 5);
		pDC->LineTo(m_rcCandle.CenterPoint().x-3, ptHighMark.y + 9);
		pDC->LineTo(m_rcCandle.CenterPoint().x-1, ptHighMark.y + 7);
	}
	// 저가
	pDC->MoveTo(m_rcDrawingArea.left + 14, ptLowMark.y + 7); 
	if((!m_bReverse && m_nLow == m_nOpen) || (m_bReverse && m_nLow == m_nClose))
	{
		pDC->LineTo(m_rcCandle.left-1, ptLowMark.y + 7); 
		pDC->LineTo(m_rcCandle.left-3, ptLowMark.y + 5); 
		pDC->LineTo(m_rcCandle.left-3, ptLowMark.y + 9); 
		pDC->LineTo(m_rcCandle.left-1, ptLowMark.y + 7); 
	}
	else
	{
		pDC->LineTo(m_rcCandle.CenterPoint().x-1, ptLowMark.y + 7); 
		pDC->LineTo(m_rcCandle.CenterPoint().x-3, ptLowMark.y + 5); 
		pDC->LineTo(m_rcCandle.CenterPoint().x-3, ptLowMark.y + 9); 
		pDC->LineTo(m_rcCandle.CenterPoint().x-1, ptLowMark.y + 7); 
	}
	// 시가
	pDC->MoveTo(m_rcDrawingArea.right - 14, ptOpenMark.y + 7); 
	pDC->LineTo(m_rcCandle.right+1, ptOpenMark.y + 7); 
	pDC->LineTo(m_rcCandle.right+3, ptOpenMark.y + 5); 
	pDC->LineTo(m_rcCandle.right+3, ptOpenMark.y + 9); 
	pDC->LineTo(m_rcCandle.right+1, ptOpenMark.y + 7); 
	// 종가
	pDC->MoveTo(m_rcDrawingArea.right - 14, ptCloseMark.y + 7); 
	pDC->LineTo(m_rcCandle.right+1, ptCloseMark.y + 7); 
	pDC->LineTo(m_rcCandle.right+3, ptCloseMark.y + 5); 
	pDC->LineTo(m_rcCandle.right+3, ptCloseMark.y + 9); 
	pDC->LineTo(m_rcCandle.right+1, ptCloseMark.y + 7); 

	pDC->SelectObject(pOldPen);
}
//////////////////////////////////////////////////////////////////////////
CRect CMyCandle::GetRect_Candle()
{
	CRect rcBody;
	rcBody = m_rcDrawingArea;

	rcBody.top =  m_rcDrawingArea.bottom - static_cast<int>(m_nClose * m_dHeight);
	rcBody.bottom -=  static_cast<int>(m_nOpen * m_dHeight);

	rcBody.left += m_nLRSpace;
	rcBody.right -= m_nLRSpace;

	rcBody.NormalizeRect();

	return rcBody;
}

CPoint CMyCandle::GetPoint_High()
{
	CRect rcBody = m_rcDrawingArea;
	rcBody.top =  m_rcDrawingArea.bottom - static_cast<int>(m_nHigh * m_dHeight);

	return CPoint(rcBody.Width()/2 + rcBody.left, rcBody.top);
}

CPoint CMyCandle::GetPoint_Low()
{
	CRect rcBody = m_rcDrawingArea;
	rcBody.bottom -=  static_cast<int>(m_nLow * m_dHeight);

	return CPoint(rcBody.Width()/2 + rcBody.left, rcBody.bottom);
}

//////////////////////////////////////////////////////////////////////////
int CMyCandle::HitTest(CPoint pt)
{
	if(!m_bFocus) return HIT_NONE;

	// 캔들이 한줄의 선이 됐을 때 이동을 우선시 함
	if(m_nClose == m_nOpen && m_rcCandle.top == m_rcCandle.bottom)
	{
		CRect rcCandle = m_rcCandle;
		rcCandle.InflateRect(0,1,0,1);
		if(rcCandle.PtInRect(pt))
			return HIT_BODY;
	}

	if(m_rcHigh.PtInRect(pt))
		return HIT_HIGH;
	if(m_rcLow.PtInRect(pt))
		return HIT_LOW;
	if(m_rcOpen.PtInRect(pt))
		return HIT_OPEN;
	if(m_rcClose.PtInRect(pt))
		return HIT_CLOSE;
	if(m_rcCandle.PtInRect(pt))
		return HIT_BODY;

	return HIT_NONE;
}

BOOL CMyCandle::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(m_bDragMode) return TRUE;

	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	int nHitTestRes = HitTest(pt) ;
	if(nHitTestRes > HIT_NONE)
	{
		if(nHitTestRes == HIT_BODY)
			SetCursor((m_bRButtonClick)?m_hCursorCrossR:m_hCursorCross);
		else
			SetCursor(m_hCursor);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
//////////////////////////////////////////////////////////////////////////

void CMyCandle::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bEnable) 
		m_pCandleMgr->SetFocusCandle(this);

	m_nHitTestRes = HitTest(point) ;
	if(m_nHitTestRes > HIT_NONE)
	{
		m_bDragMode = TRUE;
		m_bRButtonClick = FALSE;
		m_ptClick = point;

// 		int nTPos = -1;
// 		if(m_nHitTestRes == HIT_HIGH) nTPos = m_nHigh;
// 		else if(m_nHitTestRes == HIT_LOW) nTPos = m_nLow;
// 		else if(m_nHitTestRes == HIT_OPEN) nTPos = m_nOpen;
// 		else if(m_nHitTestRes == HIT_CLOSE) nTPos = m_nClose;
// 		if(nTPos > 0)
// 			m_ptClick.y = m_rcDrawingArea.bottom - static_cast<int>(nTPos * m_dHeight);

		m_nHighT = m_nHigh;
		m_nLowT = m_nLow;
		m_nOpenT = m_nOpen;
		m_nCloseT = m_nClose;

		SetCapture();

		if(m_nHitTestRes == HIT_BODY)
			m_rcCandleOrign = m_rcCandle;
	}
	else
	{
		CPoint pt = m_ptCheck;
		pt.Offset(13,13);
		CRect rcCheck(m_ptCheck, pt);
		if(rcCheck.PtInRect(point))
		{
			m_bEnable = !m_bEnable;
			m_pCandleMgr->SetCheck(m_nCandleIndex, m_bEnable);

			if(!m_bEnable)
			{
				SetCandleData(m_nCandleIndex, FALSE, 0,0,0,0);
			}

			GetParent()->PostMessage(WM_NOTIFY_CANDLEPATTEN, (WPARAM)NM_CLICK, (LPARAM)NULL);
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CMyCandle::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bDragMode = FALSE;
	::ReleaseCapture();

	if(	m_nHighT != m_nHigh ||
		m_nLowT != m_nLow ||
		m_nOpenT != m_nOpen ||
		m_nCloseT != m_nClose )
			GetParent()->PostMessage(WM_NOTIFY_CANDLEPATTEN, (WPARAM)NM_CLICK, (LPARAM)NULL);
		

	CWnd::OnLButtonUp(nFlags, point);
}

void CMyCandle::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bEnable){
		//m_pCandleMgr->SetFocusCandle(this);
		return;
	}

	m_nHitTestRes = HitTest(point) ;
	if(m_nHitTestRes == HIT_BODY)
	{
		int nTmp = m_nOpen;
		m_nOpen = m_nClose;
		m_nClose = nTmp;

		m_bReverse = !m_bReverse;

		CalcRect();
		Invalidate();
	}
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CMyCandle::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bEnable) 
		m_pCandleMgr->SetFocusCandle(this);

	m_nHitTestRes = HitTest(point) ;
	if(m_nHitTestRes == HIT_BODY)
	{
		m_bDragMode = TRUE;
		m_bRButtonClick = TRUE;
		SetCursor(m_hCursorCrossR);

		m_nHighT = m_nHigh;
		m_nLowT = m_nLow;
		m_nOpenT = m_nOpen;
		m_nCloseT = m_nClose;

		m_ptClick = point;
		SetCapture();
	
		m_rcCandleOrign = m_rcCandle;
	}
	
	CWnd::OnRButtonDown(nFlags, point);
}

void CMyCandle::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_bDragMode = FALSE;
	m_bRButtonClick = FALSE;
	::ReleaseCapture();

	if(	m_nHighT != m_nHigh ||
		m_nLowT != m_nLow ||
		m_nOpenT != m_nOpen ||
		m_nCloseT != m_nClose )
			GetParent()->PostMessage(WM_NOTIFY_DRAWPATTEN, (WPARAM)NM_CLICK, (LPARAM)NULL);

	CWnd::OnRButtonUp(nFlags, point);
}

void CMyCandle::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bDragMode)
	{
		double dDiff = m_ptClick.y - point.y;
		int nDiff = (int)(dDiff/m_dHeight);
		if(nDiff == 0) return;

		int nAdjGap = 0;
		int nHigh = m_nHighT + nDiff, nLow = m_nLowT + nDiff;
		int nOpen = m_nOpenT + nDiff, nClose = m_nCloseT + nDiff;

		switch(m_nHitTestRes)
		{
		case HIT_HIGH:	// 고
			if(nHigh > 100) nHigh = 100;
			if(nHigh < 0) nHigh = 0;
			if(m_bReverse && nHigh < m_nOpen) nHigh = m_nOpen;
			else if(!m_bReverse && nHigh < m_nClose) nHigh = m_nClose;

			m_nHigh = nHigh;
			break;
		case HIT_LOW: // 저
			if(nLow > 100) nLow = 100;
			if(nLow < 0) nLow = 0;
			if(!m_bReverse && nLow > m_nOpen) nLow = m_nOpen;
			else if(m_bReverse && nLow > m_nClose) nLow = m_nClose;

			m_nLow = nLow;
			break;
		case HIT_OPEN:	// 시
			if(m_bReverse)
			{
				if(nOpen > 100) nOpen = 100;
				if(m_nHigh < nOpen) m_nHigh = nOpen;
			}
			else
			{
				if(nOpen < 0) nOpen = 0;
				if(m_nLow > nOpen) m_nLow = nOpen;
			}

			m_nOpen = nOpen;

			if(m_nOpen > m_nClose) m_bReverse = TRUE;
			else m_bReverse = FALSE;

			break;
		case HIT_CLOSE: // 종
			if(m_bReverse)
			{
				if(nClose < 0) nClose = 0;
				if(m_nLow > nClose) m_nLow = nClose;
			}
			else
			{
				if(nClose > 100) nClose = 100;
				if(m_nHigh < nClose) m_nHigh = nClose;
			}

			m_nClose = nClose;

			if(m_nOpen > m_nClose) m_bReverse = TRUE;
			else m_bReverse = FALSE;

			break;
		case HIT_BODY:
			if(m_bRButtonClick)
			{
				if(m_bReverse)
				{
					if(nOpen > m_nHigh)
					{
						nOpen = m_nHigh;
						m_nOpen = nOpen;
						m_nClose = m_nOpen - (m_nOpenT - m_nCloseT);
						break;
					}
					if(nClose < m_nLow)
					{
						nClose = m_nLow;
						m_nClose = nClose;
						m_nOpen = m_nClose + (m_nOpenT - m_nCloseT);
						break;
					}
					m_nOpen = nOpen;
					m_nClose = nClose;
				}
				else
				{
					if(nClose > m_nHigh)
					{
						nClose = m_nHigh;
						m_nClose = nClose;
						m_nOpen = m_nClose - (m_nCloseT - m_nOpenT);
						break;
					}
					if(nOpen < m_nLow)
					{
						nOpen = m_nLow;
						m_nOpen = nOpen;
						m_nClose = m_nOpen + (m_nCloseT - m_nOpenT);
						break;
					}
					m_nOpen = nOpen;
					m_nClose = nClose;
				}
			}
			else
			{
				if(nHigh > 100)
				{
					nHigh = 100;
					nAdjGap = m_nHighT - nHigh;

					if(nAdjGap != nDiff)
					{
						m_nHigh = m_nHighT - nAdjGap; m_nLow = m_nLowT - nAdjGap;
						m_nOpen = m_nOpenT -+ nAdjGap; m_nClose = m_nCloseT - nAdjGap;
						break;
					}
				}

				if(nLow < 0)
				{
					nLow = 0;
					nAdjGap = m_nLowT - nLow;
					if(nAdjGap != nDiff)
					{
						m_nHigh = m_nHighT - nAdjGap; m_nLow = m_nLowT - nAdjGap;
						m_nOpen = m_nOpenT - nAdjGap; m_nClose = m_nCloseT - nAdjGap;
						break;
					}
				}
				
				m_nHigh = nHigh; m_nLow = nLow;
				m_nOpen = nOpen; m_nClose = nClose;
			}

			break;
		}

		CalcRect();
		Invalidate();
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CMyCandle::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyCandle::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	m_bFocus = TRUE;
	Invalidate();
}

void CMyCandle::KillFocusCandle() 
{
	m_clrBackGround = (m_bEnable)?RGB(255,255,255):RGB(240,240,240);
	m_bFocus = FALSE;
	Invalidate();
}

void CMyCandle::MoveWindow(int nX, int nY, int nWidth, int nHeight)
{
	CRect rcClient(nX, nY, nX+nWidth, nY+nHeight);
	CWnd::MoveWindow(rcClient);

	GetClientRect(rcClient);

	m_rcDrawingArea = rcClient;
	m_rcDrawingArea.DeflateRect(1, m_nParkingMargin+m_nCheckMargin, 1, m_nParkingMargin);
	if(m_bYScale) m_rcDrawingArea.DeflateRect(0,0,m_nYScaleWidth,0);

	// hight per 1%
	m_dHeight = m_rcDrawingArea.Height() / 100.;
	// space for left/right side
	m_nLRSpace = static_cast<int>((m_rcDrawingArea.Width() * 1.7)/5.);

	if(m_bYScale)
		m_ptCheck = CPoint((rcClient.Width() - 13 - m_nYScaleWidth)/2, rcClient.top + 4);
	else
		m_ptCheck = CPoint((rcClient.Width() - 13)/2, rcClient.top + 4);

	CalcRect();
}

void CMyCandle::SetEnable(BOOL bEnable/*=TRUE*/)
{
	m_bEnable = bEnable;

	m_clrBackGround = (m_bEnable)?RGB(255,255,255):RGB(240,240,240);
	Invalidate();
}

#if (_MFC_VER >= 0x0421)
BOOL CMyCandle::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
/*	int nInc = 1;
	if(zDelta > 0) nInc *= -1;

	CPoint point = pt;
	ScreenToClient(&point);

	if(m_rcDrawingArea.PtInRect(point))
	{
		if(m_bReverse)
		{
			if(m_nOpen != m_nClose) 
				m_nClose += nInc;
			if(m_nOpen != m_nClose)
				m_nOpen -= nInc;
			
			if(m_nOpen > 100) m_nOpen = 100;
			if(m_nClose < 0) m_nClose = 0;

			if(m_nHigh < m_nOpen) m_nHigh = m_nOpen;
			if(m_nLow > m_nClose) m_nLow = m_nClose;
		}
		else
		{
			if(m_nOpen != m_nClose) 
				m_nOpen += nInc;
			if(m_nOpen != m_nClose) 
				m_nClose -= nInc;

			if(m_nClose > 100) m_nClose = 100;
			if(m_nOpen < 0) m_nOpen = 0;
			
			if(m_nHigh < m_nClose) m_nHigh = m_nClose;
			if(m_nLow > m_nOpen) m_nLow = m_nOpen;
		}
	}

	CalcRect();
	Invalidate();
	*/
    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
#endif // !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)

//////////////////////////////////////////////////////////////////////////
// 캔들 매니저
//////////////////////////////////////////////////////////////////////////
void CCandleManager::Create(int nID, CPoint ptWnd, CSize szCandle, CWnd* pParent, CString strRootPath)
{
	m_strRootPath = strRootPath;
	m_nID = nID;
	
	for(int i = 0; i < MAX_CANDLE; i++)
	{
		m_CandleWnd[i].Create(NULL, "",
			WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
			CRect(i*szCandle.cx + ptWnd.x, ptWnd.y, i*szCandle.cx + szCandle.cx + ptWnd.x + 1, ptWnd.y + szCandle.cy),
			pParent,
			nID + i);
		
		m_CandleWnd[i].SetCandleManager(this);
		
		memcpy(&m_CandleSet.cd[i], CandleSet_DEFAULT[i], sizeof(int)*5);
		m_CandleWnd[i].SetCandleData(i, m_CandleSet.cd[i]);
	}

	m_bCreated = TRUE;
}

void CCandleManager::DestroyCandle()
{
	for(int i = 0; i < MAX_CANDLE; i++)
	{
		m_CandleWnd[i].DestroyWindow();
	}
}

void CCandleManager::Reset()
{
	for(int i = 0; i < MAX_CANDLE; i++)
	{
		memcpy(&m_CandleSet.cd[i], CandleSet_DEFAULT[i], sizeof(int)*5);
		m_CandleWnd[i].SetCandleData(i, m_CandleSet.cd[i]);
	}

	SetFocusCandle(&m_CandleWnd[MAX_CANDLE-1]);
}

void CCandleManager::SetFocusCandle(CWnd* pFocusWnd)
{
	for(int i = 0; i < MAX_CANDLE; i++)
	{
		if(pFocusWnd->GetSafeHwnd() == m_CandleWnd[i].GetSafeHwnd())
		{
			m_CandleWnd[i].SetFocus();
			m_nFocusIndex = i;
		}
		else
		{
			m_CandleWnd[i].KillFocusCandle();
		}
	}
}

void CCandleManager::SetCandleSet(CANDLE_SET* pSet)
{
	for(int i = 0; i < MAX_CANDLE; i++)
	{
		m_CandleWnd[i].SetCandleData(i, pSet->cd[i]);
	}
}

void CCandleManager::ShowWindow(int nCmdShow)
{
	for(int i = 0; i < MAX_CANDLE; i++)
	{
		m_CandleWnd[i].ShowWindow(nCmdShow);
	}

}

void CCandleManager::EnableWindow(BOOL bEnable)
{
	for(int i = 0; i < MAX_CANDLE; i++)
	{
		m_CandleWnd[i].EnableWindow(bEnable);
	}

}

void CCandleManager::MoveWindow(int nX, int nY, int nWidth, int nHeight)
{
	if(m_bYScale)
		nWidth -= m_nYScaleWidth;

	int nCX = nWidth/MAX_CANDLE;
	int nRest = nWidth%MAX_CANDLE;

	for(int i = 0; i < MAX_CANDLE; i++)
	{
		if(i == MAX_CANDLE-1 && m_bYScale && m_nYScaleWidth >= 0)
		{
			m_CandleWnd[i].SetYScale(TRUE, m_nYScaleWidth);
			m_CandleWnd[i].MoveWindow(nX + i*nCX, nY, nCX+1+m_nYScaleWidth, nHeight);
		}else
			m_CandleWnd[i].MoveWindow(nX + i*nCX, nY, nCX+1, nHeight);
	}

}
//////////////////////////////////////////////////////////////////////////
int CCandleManager::GetFirstEnableCandle()
{
	int nFirstEnableCandle = 0;

	for(int i = MAX_CANDLE - 1; i >= 0; i--)
	{
		if(m_CandleWnd[i].m_bEnable)
		{
			nFirstEnableCandle = i;
			break;
		}
	}

	return nFirstEnableCandle;
}

int CCandleManager::GetLastEnableCandle()
{
	int  nLastEnabledCandle = 0;

	for(int i = MAX_CANDLE - 1; i >= 0; i--)
	{
		if(m_CandleWnd[i].m_bEnable)
		{
			nLastEnabledCandle = i;
		}
	}

	return nLastEnabledCandle;
}

int CCandleManager::GetEnableCandleCount()
{
	int  nCnt = 0;

	for(int i = MAX_CANDLE - 1; i >= 0; i--)
	{
		if(m_CandleWnd[i].m_bEnable)
		{
			nCnt++;
		}
	}

	return nCnt;
}

int CCandleManager::GetRange()
{
	int nCnt = GetFirstEnableCandle() - GetLastEnableCandle() + 1;

	if(GetEnableCandleCount() == 0)
		nCnt = 0;

	return nCnt;
}
//////////////////////////////////////////////////////////////////////////
void CCandleManager::GetCandlePacketData(CANDLE_IN_DATA* stIn)
{
	// 첫봉이 enable 상태가 아닐 때, 봉전 입력값을 바꾸고,
	// 캔들데이터를 첫봉이 enable인 봉이 오도록 shift한다.
	BOOL bStartFill = FALSE;

	for(int i = MAX_CANDLE-1; i >= 0; i--)
	{
		if(m_CandleWnd[i].m_bEnable)
		{
			stIn->iHigh = m_CandleWnd[i].m_nHigh;
			stIn->iClose = m_CandleWnd[i].m_nClose;
			stIn->iOpen = m_CandleWnd[i].m_nOpen;
			stIn->iLow = m_CandleWnd[i].m_nLow;
			stIn++;
			bStartFill = TRUE;
		}

		if(bStartFill && m_CandleWnd[i].m_bEnable == FALSE)
			stIn++;
	}
}

void CCandleManager::GetCandleSet(CANDLE_SET* pSet)
{
	for(int i = 0; i < MAX_CANDLE; i++)
	{
		pSet->cd[i].bEnable = m_CandleWnd[i].m_bEnable;
		pSet->cd[i].nHigh = m_CandleWnd[i].m_nHigh;
		pSet->cd[i].nClose = m_CandleWnd[i].m_nClose;
		pSet->cd[i].nOpen = m_CandleWnd[i].m_nOpen;
		pSet->cd[i].nLow = m_CandleWnd[i].m_nLow;
	}
}

void CCandleManager::SetCheck(int nIndex, BOOL bCheck)
{
	if(bCheck)
	{
		m_CandleWnd[nIndex].SetEnable(bCheck);

		SetFocusCandle(&m_CandleWnd[nIndex]);
	}
	else
	{
		m_CandleWnd[nIndex].SetEnable(bCheck);

		for(int i = MAX_CANDLE -1; i >= 0; i--)
		{
			if(m_CandleWnd[i].GetEnable())
			{
				SetFocusCandle(&m_CandleWnd[i]);
				break;
			}
		}
	}
}

void CCandleManager::DrawGradientRect(CDC *pDC, CRect rect, COLORREF Start, COLORREF End, BOOL bTopDown)
{
	CRect backup = rect;
		
	int r,g,b;//각 3가지 색상의 값의 차이..
	float rStep, gStep, bStep;//각 3가지 값들 각각의 색상값의 차이..
	float fStep;//최대 색상값 차이로 나누어서 한번에 이동할 옵셋..

	//각 생상 차이를 구한다..
	r = (GetRValue(End) - GetRValue(Start));
	g = (GetGValue(End) - GetGValue(Start));
	b = (GetBValue(End) - GetBValue(Start));

	//가장 큰차이가 나는것을 구한다..
	int Max = max( abs(r), max(abs(g), abs(b)) );

	//그릴 사각형을 차이가 나는것 만큼.. 분할할 길이를 구한다..
	if(bTopDown)
		fStep = (float)rect.Height()/(float)Max;     // 상-하 Gradation 
	else 
		fStep = (float)rect.Width()/(float)Max;     // 좌-우 Gradation 

	//각 색상별 단계 차이값..
	rStep = r/(float)Max;
	gStep = g/(float)Max;
	bStep = b/(float)Max;

	//r,g,b를 시작 색으로 재설정...
	r = GetRValue(Start);
	g = GetGValue(Start);
	b = GetBValue(Start);

	//색깔이 변해야 하는 횟수만큼.. 반복 그린다.
	for(int iOnBand=0 ; iOnBand <Max; iOnBand++)  
	{
		if(bTopDown)
 			::SetRect( &rect, rect.left, int(fStep*iOnBand) + backup.top ,rect.right, backup.bottom );// 상-하 Gradation 
 		else     
			::SetRect( &rect, int(fStep*iOnBand) + backup.left, rect.top, rect.right, rect.bottom);//좌-우
		pDC->FillSolidRect( &rect,  RGB( r + rStep*iOnBand, g + gStep*iOnBand, b + bStep*iOnBand ) );
	}
}

void CCandleManager::SetYScale(BOOL bShow, int nWidth)
{
	m_bYScale = bShow;
	m_nYScaleWidth = nWidth;
}

void CCandleManager::SetFont(CFont* pFont)
{ 
	m_pFont = pFont;
	
	for(int i = 0; i < MAX_CANDLE; i++)
		m_CandleWnd[i].SetFont(pFont);
}

BOOL CCandleManager::GetWindowRect(CRect& rcWnd, int nIndex)
{
	if(::IsWindow(m_CandleWnd[nIndex].GetSafeHwnd()))
		m_CandleWnd[nIndex].GetWindowRect(&rcWnd);
	else 
		return FALSE;

	return TRUE;
}
// DlgGlass.cpp : implementation file
//

#include "stdafx.h"
#include "StdDialog.h"
#include "DlgGlass.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGlass dialog

extern HINSTANCE g_hInstance;
const UINT  RMSG_MAGNIFYING_GLASS = ::RegisterWindowMessage("RMSG_MAGNIFYING_GLASS"); //05.09.30

CDlgGlass::CDlgGlass(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGlass::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGlass)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCount = 0;
	m_pOldBitmap = NULL;
	m_bAfterInit = false;
	m_bClicked = false;
	m_pParent = pParent;
	m_rctPos.SetRect(0, 0, 0, 0);
}

CDlgGlass::~CDlgGlass()
{
//	if(m_pOldBitmap != NULL)
//		m_memDC.SelectObject(m_pOldBitmap);

	m_memDC.DeleteDC();
	m_Bitmap.DeleteObject();
}


void CDlgGlass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGlass)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGlass, CDialog)
	//{{AFX_MSG_MAP(CDlgGlass)
	ON_WM_PAINT()
	ON_WM_MOVE()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_MAGNIFY_2, OnMagnify2)
	ON_COMMAND(ID_MAGNIFY_5, OnMagnify5)
	ON_COMMAND(ID_MAGNIFY_3, OnMagnify3)
	ON_COMMAND(ID_MAGNIFY_4, OnMagnify4)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_CLOSEMAGNIFY, OnClosemagnify)
	ON_UPDATE_COMMAND_UI(ID_MAGNIFY_3, OnUpdateMagnify3)
	ON_UPDATE_COMMAND_UI(ID_MAGNIFY_4, OnUpdateMagnify4)
	ON_UPDATE_COMMAND_UI(ID_MAGNIFY_5, OnUpdateMagnify5)
	ON_UPDATE_COMMAND_UI(ID_MAGNIFY_2, OnUpdateMagnify2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGlass message handlers

void CDlgGlass::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	int srcdivide;
	srcdivide = 200;//(int)sqrt(40000/m_nMultiple); // 면적을 선택된배율로 나누고 정사각형의 변을 구했다.

	dc.StretchBlt(0, 0, srcdivide, srcdivide, &m_memDC,
				  m_ptMove.x + (srcdivide/2) - (srcdivide/m_nMultiple)/2, m_ptMove.y + (srcdivide/2) - (srcdivide/m_nMultiple)/2,
				  srcdivide/m_nMultiple, srcdivide/m_nMultiple,	SRCCOPY);

	 CDialog::OnPaint();
}

void CDlgGlass::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);

	m_ptMove.x = x; 
	m_ptMove.y = y;
	Invalidate();  // 이거 없으면 다시 그리지 못한다.
}

BOOL CDlgGlass::OnInitDialog() 
{
	CDialog::OnInitDialog();

	MakeBitmap();
	m_bAfterInit = true;
/*
	::GetCursorPos(&m_pt);// 포인트 초기값.

	// 윈도우 DC 생성
	m_wndDesk = GetDesktopWindow();
	CClientDC dc(m_wndDesk);
	// 다이얼로그 크기 얻기;
	GetClientRect(&m_rect);
	// 윈도우의 크기 얻기
	m_ptScreen.x = GetSystemMetrics(SM_CXSCREEN);
	m_ptScreen.y = GetSystemMetrics(SM_CYSCREEN);
	
	// 메모리 DC 생성
	m_memDC.CreateCompatibleDC(&dc);
	m_Bitmap.CreateCompatibleBitmap(&dc, m_ptScreen.x, m_ptScreen.y);
	m_memDC.SelectObject(&m_Bitmap);
	m_memDC.BitBlt(0,0,m_ptScreen.x,m_ptScreen.y,&dc,0,0,SRCCOPY);
	
	m_bClicked = false;   // 윈도우 옮길때 마우스 클릭여부 초기화
	
	m_nMultiple = 2;	// 배율 초기값
	m_nScrRate = m_ptScreen.x/m_ptScreen.y;	// 스크린 가로세로 비율(세로기준)
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGlass::Save_Desktop()
{
	m_wndDesk = GetDesktopWindow();
	CClientDC dc(m_wndDesk);	

	m_memDC.BitBlt(0,0,m_ptScreen.x,m_ptScreen.y,&dc,0,0,SRCCOPY);// 메모리 DC에 화면을 그린다.(저장)
}

void CDlgGlass::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CMenu Context;
	CMenu * m_Popup = NULL;	

	HINSTANCE hInstResourceClient = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	BOOL bResult = Context.LoadMenu(IDR_MAGNIFY);

	AfxSetResourceHandle(hInstResourceClient);
	if(bResult)	{
		m_Popup = Context.GetSubMenu(0);
		ClientToScreen(&point);
	}
	m_Popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
					point.x, point.y, this, NULL);
	Context.DestroyMenu();

	CDialog::OnRButtonUp(nFlags, point);
}

void CDlgGlass::OnMagnify2() 
{
	m_nMultiple = 2;
	Invalidate();
}
void CDlgGlass::OnMagnify3() 
{
	m_nMultiple = 3;
	Invalidate();
}

void CDlgGlass::OnMagnify4() 
{
	m_nMultiple = 4;	
	Invalidate();
}

void CDlgGlass::OnMagnify5() 
{
	m_nMultiple = 5;
	Invalidate();
}
/*
// 마우스 이벤트를 모두 No Client로 바꿨기 때문에 마우스 조작시 NC 이벤트 발생한다.
void CDlgGlass::OnNcMouseMove( UINT nHitTest, CPoint point )
{
	if (!m_bClicked) return;
	MoveWindow( point.x-100, point.y-100, m_rect.Width(), m_rect.Height(), TRUE );
}

UINT CDlgGlass::OnNcHitTest(CPoint point) // 윈도우를 옮기기 위하여
{
	UINT hit = CDialog::OnNcHitTest(point); // 모든 마우스 메시지를 HTCAPTION으로 바꾼다.
	if(hit == HTCLIENT) return HTCAPTION;
	else return hit;
}

void CDlgGlass::OnNcLButtonDown( UINT nHitTest, CPoint point)
{
	m_bClicked = true;
}
void CDlgGlass::OnNcLButtonUp( UINT nHitTest, C Point point)
{
	m_bClicked = false;
}
*/
void CDlgGlass::MakeBitmap()
{
	::GetCursorPos(&m_pt);// 포인트 초기값.

	// 윈도우 DC 생성
	m_wndDesk = GetDesktopWindow();
	CClientDC dc(m_pParent);
	// 다이얼로그 크기 얻기;
	GetClientRect(&m_rect);
	//nami
	CRect rcChart;
	m_pParent->GetClientRect(&rcChart);
	// 윈도우의 크기 얻기
	m_ptScreen.x = GetSystemMetrics(SM_CXSCREEN);
	m_ptScreen.y = GetSystemMetrics(SM_CYSCREEN);

	// 메모리 DC 생성
	m_memDC.DeleteDC();
	m_Bitmap.DeleteObject();

	m_memDC.CreateCompatibleDC(&dc);
	m_Bitmap.CreateCompatibleBitmap(&dc, rcChart.Width(), rcChart.Height());
	m_memDC.SelectObject(&m_Bitmap);
//	m_pOldBitmap = m_memDC.SelectObject(&m_Bitmap);
	m_memDC.BitBlt(m_rect.top, m_rect.left, rcChart.Width(), rcChart.Height(), &dc, rcChart.top, rcChart.left, SRCCOPY);

	m_bClicked = false;   // 윈도우 옮길때 마우스 클릭여부 초기화
	
	m_nMultiple = 2;	// 배율 초기값
	m_nScrRate = rcChart.Width() / rcChart.Height();	// 스크린 가로세로 비율(세로기준)

//popup
/*
	// 윈도우 DC 생성
	m_wndDesk = GetDesktopWindow();
	CClientDC dc(m_wndDesk);
	// 다이얼로그 크기 얻기;
	GetClientRect(&m_rect);
	// 윈도우의 크기 얻기
	m_ptScreen.x = GetSystemMetrics(SM_CXSCREEN);
	m_ptScreen.y = GetSystemMetrics(SM_CYSCREEN);

	// 메모리 DC 생성
	          
	m_memDC.DeleteDC();
	m_Bitmap.DeleteObject();

	m_memDC.CreateCompatibleDC(&dc);
	m_Bitmap.CreateCompatibleBitmap(&dc, m_ptScreen.x, m_ptScreen.y);
	m_memDC.SelectObject(&m_Bitmap);
	m_memDC.BitBlt(m_rect.top, m_rect.left, m_ptScreen.x, m_ptScreen.y, &dc, 0, 0, SRCCOPY);

	m_bClicked = false;   // 윈도우 옮길때 마우스 클릭여부 초기화
	
	m_nMultiple = 2;	// 배율 초기값
	m_nScrRate = m_ptScreen.x/m_ptScreen.y;	// 스크린 가로세로 비율(세로기준)
*/
}

void CDlgGlass::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bClicked = false;
	ReleaseCapture();

	//타임이 100초가 되지 않아서 mousemove를 타지 않고 up이 되버리면 엉뚱한 위치에 dialog가 위치한다.
	CPoint ptOffset = point - m_ptStart;
	m_rctPos.OffsetRect( ptOffset);
	SetWindowPos( NULL, m_rctPos.left, m_rctPos.top, 0, 0, 
		SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgGlass::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bClicked = true;
	m_ptStart = point;
	
	//popup
//	GetWindowRect( &m_rctPos);
	//맨 첨에만 한다.
	if(m_rctPos.IsRectNull()) 
		GetClientRect( &m_rctPos);

	SetCapture();
	GetLocalTime( &m_stMouseTime);
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgGlass::OnMouseMove(UINT nFlags, CPoint point) 
{
//	if(m_bClicked == false)
//		return;

	if( nFlags & MK_LBUTTON)
	{
		SYSTEMTIME stNewMouse;
		GetLocalTime( &stNewMouse);
		if( GetTimeGap( &stNewMouse, &m_stMouseTime) > 100)
		{
			m_stMouseTime = stNewMouse;
			CPoint ptOffset = point - m_ptStart;
			m_rctPos.OffsetRect( ptOffset);
			SetWindowPos( NULL, m_rctPos.left, m_rctPos.top, 0, 0, 
				SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(&CWnd::wndTopMost, m_rctPos.left, m_rctPos.top, m_rctPos.Width(), m_rctPos.Height(), 
				SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2002.10.17
//
//	목적	:	SYSTEMTIME의 차를 구하는 Routine을 제공한다.
//
//	수정	:	(2002.10.21) Pointer로 직접 연산하기에 빌림을 나타내는 Flag을 활용한다.
///////////////////////////////////////////////////////////////////////////////
long CDlgGlass::GetTimeGap( SYSTEMTIME *p_stTo, SYSTEMTIME *p_stFrom)
{
	long lGap;

	// 0. 밀리초의 차를 구한다.
	int nMilliBelow = ( p_stTo->wMilliseconds < p_stFrom->wMilliseconds ? 1 : 0);
	lGap = ( nMilliBelow ? 1000 : 0) + p_stTo->wMilliseconds - p_stFrom->wMilliseconds;

	// 1. 초의 차를 구한다.
	int nSecondBelow = ( p_stTo->wSecond < p_stFrom->wSecond  + nMilliBelow ? 1 : 0);
	lGap += ( ( nSecondBelow ? 60 : 0) + p_stTo->wSecond - p_stFrom->wSecond - nMilliBelow) * 1000;

	// 2. 분의 차를 구한다.
	int nMinuteBelow = ( p_stTo->wMinute < p_stFrom->wMinute + nSecondBelow ? 1 : 0);
	lGap += ( ( nMinuteBelow ? 60 : 0) + p_stTo->wMinute - p_stFrom->wMinute - nSecondBelow) * 60000;

	return lGap;
}

void CDlgGlass::OnClosemagnify() 
{
	if(m_pParent)
		m_pParent->SendMessage(RMSG_MAGNIFYING_GLASS, (WPARAM)0);
	
	CDialog::OnOK();
}

void CDlgGlass::OnDraw(CDC* pdc, const CBitmap &bitmap)
{
	if(m_bAfterInit == false)
		return;
//	memcpy(m_Bitmap, bitmap, sizeof(bitmap));
//	Invalidate();

	// 다이얼로그 크기 얻기;
	GetClientRect(&m_rect);
	//nami
	CRect rcChart;
	m_pParent->GetClientRect(&rcChart);
	// 윈도우의 크기 얻기
	m_ptScreen.x = GetSystemMetrics(SM_CXSCREEN);
	m_ptScreen.y = GetSystemMetrics(SM_CYSCREEN);

	// 메모리 DC 생성
	m_memDC.DeleteDC();
	m_Bitmap.DeleteObject();

	m_memDC.CreateCompatibleDC(pdc);
	m_Bitmap.CreateCompatibleBitmap(pdc, rcChart.Width(), rcChart.Height());
	m_pOldBitmap = m_memDC.SelectObject(&m_Bitmap);
	m_memDC.BitBlt(m_rect.top, m_rect.left, rcChart.Width(), rcChart.Height(), pdc, rcChart.top, rcChart.left, SRCCOPY);

	m_bClicked = false;   // 윈도우 옮길때 마우스 클릭여부 초기화
	
	m_nScrRate = rcChart.Width() / rcChart.Height();	// 스크린 가로세로 비율(세로기준)
	Invalidate();
}

void CDlgGlass::OnUpdateMagnify3(CCmdUI* pCmdUI) 
{
	if(m_nMultiple == 3)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CDlgGlass::OnUpdateMagnify4(CCmdUI* pCmdUI) 
{
	if(m_nMultiple == 4)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CDlgGlass::OnUpdateMagnify5(CCmdUI* pCmdUI) 
{
	if(m_nMultiple == 5)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CDlgGlass::OnUpdateMagnify2(CCmdUI* pCmdUI) 
{
	if(m_nMultiple == 2)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

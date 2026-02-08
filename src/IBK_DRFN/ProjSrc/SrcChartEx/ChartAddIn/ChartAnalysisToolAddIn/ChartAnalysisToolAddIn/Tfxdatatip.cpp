#include "stdafx.h"
#include "tfxdatatip.h"

#define COMPILE_MULTIMON_STUBS 
#include "multimon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// define the offset above cursor
//static const CPoint _defOffset(10, 5);

// border area around the text
static const short _border = 2;

// define the active border for data tips
static const short _horizontal  = 2;
static const short _vertical    = 2;

// GDI objects used for drawing operations in module
static CFont   _font;
static CBrush* _brush = NULL;

// hook information
HHOOK TFXDataTip::m_s_hookProc = NULL;
TFXDataTip* TFXDataTip::m_s_pCurrent = NULL;

// special destructor object for clearing up GDI objects
struct _brush_destructor
{
    ~_brush_destructor()
    {
        delete _brush;
        _brush = NULL;
    }
};
static const _brush_destructor _destroyer;

BEGIN_MESSAGE_MAP(TFXDataTip, CWnd)
	//{{AFX_MSG_MAP(TFXDataTip)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void TFXDataTip::RegisterWnd()
{
	// check for prior registration
	if(m_bRegistered) 
		return;

	// initialise the basic information before registration
	HINSTANCE hInst = AfxGetInstanceHandle( );

	// initialise the window class information
	WNDCLASS wndcls;
	wndcls.style         = CS_SAVEBITS | CS_DBLCLKS;
	wndcls.lpfnWndProc   = ::DefWindowProc;
	wndcls.cbClsExtra    = 0;
	wndcls.cbWndExtra    = 0;
	wndcls.hInstance     = hInst;
	wndcls.hIcon         = NULL;
	wndcls.hCursor       = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndcls.hbrBackground = *_brush;
	wndcls.lpszMenuName  = NULL;
	wndcls.lpszClassName = "TFXDataTip";

	// register the window class
	if(!AfxRegisterClass(&wndcls))
		AfxThrowResourceException();

	m_bRegistered = TRUE;
}

LRESULT CALLBACK TFXDataTip::KeyboardHookCallback(const int code, WPARAM wParam, LPARAM lParam)
{
	// if keypress has occured then hide any visible data tips
	if( code >= 0)
	{
		if(m_s_pCurrent != NULL)
			m_s_pCurrent->Hide();
	}

	// chain to the next hook procedure
	return ::CallNextHookEx(m_s_hookProc, code, wParam, lParam);
}

// ========================================================================================
TFXDataTip::TFXDataTip():
	m_bOn( true),
	m_bReady(false),
	m_bRegistered(false),
	m_timer(0),
	m_timerToHide(0),
	m_MousePosition(0, 0),
	m_TipSurrounding(2, 2),
	m_sDelay(100),
	m_crBkColor(RGB(240,244,255))
//	m_crBkColor(::GetSysColor(COLOR_INFOBK))
{
	// (2006/4/11 - Seung-Won, Bae) Manage OCX Ctrl CWnd Pointer for LButton Processing on ToolTip.
	m_pChartWnd = NULL;
}

TFXDataTip::~TFXDataTip()
{
	if(m_s_hookProc != NULL){
		::UnhookWindowsHookEx(m_s_hookProc);
		m_s_hookProc = NULL;
	}

	// (2006/2/20 - Seung-Won, Bae) Do not call CWnd::DestroyWindow(). Because It is not called in Base Destructor.
	if (m_hWnd != NULL &&
		this != (CWnd*)&wndTop && this != (CWnd*)&wndBottom &&
		this != (CWnd*)&wndTopMost && this != (CWnd*)&wndNoTopMost)
	{
		TRACE(_T("Warning: calling DestroyWindow in CWnd::~CWnd; ")
		   _T("OnDestroy or PostNcDestroy in derived class will not be called.\n"));
		DestroyWindow();
	}

	// (2006/4/11 - Seung-Won, Bae) Manage OCX Ctrl CWnd Pointer for LButton Processing on ToolTip.
	m_pChartWnd = NULL;
}

bool TFXDataTip::Create( void) 
{
	if(CWnd::CreateEx(WS_EX_LEFT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
						_T("TFXDataTip"),
						NULL,
						WS_POPUP | WS_BORDER | WS_CLIPSIBLINGS, 
						0, 0, 10, 10,
						NULL, 
						NULL)){
		m_bReady = true;

		// (2006/2/20 - Seung-Won, Bae) Do not call Initialise() before Create().
		//		Because Create() has failed with UnhookWindow by Initialise().
		// (2006/2/21 - Seung-Won, Bae) Do not hook before Window Creation.
		// install the keyboard hook procedure
		if(m_s_hookProc == NULL){
			m_s_hookProc = ::SetWindowsHookEx(WH_KEYBOARD, 
								(HOOKPROC)KeyboardHookCallback,
								NULL,
								::GetCurrentThreadId( ));
		}

	}
	else
		m_bReady = false;

	return m_bReady;
}

BOOL TFXDataTip::DestroyWindow() 
{
	DataTipKillTimer();
	_font.DeleteObject();
	_brush_destructor();

	return CWnd::DestroyWindow();
}

// ----------------------------------------------------------------------------
void TFXDataTip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

    CRect rect;
    GetClientRect(&rect);

    // initialise the device context for drawing
    CFont* pOldFont = dc.SelectObject(&_font);
	//CFont* pOldFont = dc.SelectObject(m_pFont);
    dc.SetBkColor(::GetSysColor(COLOR_INFOBK));
	dc.SetBkColor(m_crBkColor); //tip
    dc.SetTextColor(::GetSysColor(COLOR_INFOTEXT));
    // draw the data tip
    dc.SetTextAlign(TA_TOP | TA_LEFT);//TA_CENTER);

	CSize extent;
	CString str;
	int cy = 0;
	POSITION pos = m_listStrings.GetHeadPosition();
	while(pos){
		str = m_listStrings.GetNext(pos);
		//dc.TextOut((rect.Width( ) / 2), (_border / 2)+cy, str);//가운데 중심
		dc.TextOut(0, (_border / 2)+cy, str);
		extent = dc.GetTextExtent(str);
		cy += extent.cy;
	}
    
	// restore the device context
    dc.SelectObject(pOldFont);
}

void TFXDataTip::OnTimer(UINT nIDEvent) 
{
	if(	nIDEvent == m_timer)
	{
		KillTimer( m_timer);
		m_timer = 0;

		if( m_timerToHide != 0)
		{
			KillTimer( m_timerToHide);
			m_timerToHide = 0;
		}

		// make sure that cursor hasn't moved before displaying the data tip
		CPoint point;
		::GetCursorPos( &point);
		if(IsPointNearOtherPoint(m_MousePosition, point, m_TipSurrounding)) 
			Display();			

// (2007/2/9 - Seung-Won, Bae) Do not hide with automatically.
//		m_timerToHide = SetTimer( 1011, 1000, NULL);
	}
    if( nIDEvent == m_timerToHide)
	{
		KillTimer( m_timerToHide);
		m_timerToHide = 0;
		Hide();
	}

	if( m_sDelay != 0 || nIDEvent != m_timer)
		CWnd::OnTimer(nIDEvent);
}

// public =====================================================================
void TFXDataTip::Initialise(const COLORREF crColor)
{
	m_crBkColor = crColor;

	if(_font.GetSafeHandle( ) == NULL)
	{
		char szLanguageID[ 10];
		GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_ILANGUAGE, szLanguageID, 10);
		if( !strcmp( "0412", szLanguageID))		// (2008/2/28 - Seung-Won, Bae) Korean
		{
			// create the default tip font
			LOGFONT lFont;
			GetObject( GetStockObject(ANSI_VAR_FONT), sizeof lFont, &lFont);
			//tip
			lFont.lfCharSet = 0x81; // 한글 Character Set
			lFont.lfHeight = 12; // 20 point 크기
			lFont.lfWidth = 6;
			strcpy(lFont.lfFaceName, "굴림체"); // 궁서체로 font setting
			_font.CreateFontIndirect(&lFont); 
		}
		else									// (2008/2/28 - Seung-Won, Bae) Etc (English)
		{
			LOGFONT lFont;
			GetObject( GetStockObject( ANSI_VAR_FONT), sizeof lFont, &lFont);
			//tip
			lFont.lfHeight = 14;
			lFont.lfWidth = 7;
			strcpy(lFont.lfFaceName, "Courier New");
			_font.CreateFontIndirect(&lFont); 
		}
	}

	// create the other GDI objects
	//_brush = new CBrush(::GetSysColor(COLOR_INFOBK));

	// (2006/2/16 - Seung-Won, Bae) Check NULL Pointer
	if( _brush)
	{
		if( _brush->GetSafeHandle() == NULL)
		{
			delete _brush;
			_brush = NULL;
		}
	}
	if( !_brush) _brush = new CBrush( m_crBkColor);

	// register the window class
	RegisterWnd();
}

void TFXDataTip::SetBackgrandColor(const COLORREF crColor)
{
	Initialise(m_crBkColor);
}

void TFXDataTip::SetSurrounding(const long& x, const long& y)
{
	m_TipSurrounding = CSize(x, y);
}

void TFXDataTip::SetSurrounding(const CSize& offset)
{
	m_TipSurrounding = offset;
}

void TFXDataTip::SetDelay(const short delay)
{
	m_sDelay = delay;
}

bool TFXDataTip::SetNewTip( CWnd* pParentWnd, CPoint p_ptCursor)
{
	// (2006/4/11 - Seung-Won, Bae) Manage OCX Ctrl CWnd Pointer for LButton Processing on ToolTip.
	m_pChartWnd = pParentWnd;

	// check whether tips are turned on
	if(!m_bOn) return false;

	if( m_hWnd == NULL) return false;

	// determine whether the point is in the parent window
	// (2008/11/9 - Seung-Won, Bae) Do not use current mouse position. It may be moved.
	//	::GetCursorPos( &m_MousePosition);
	m_MousePosition = p_ptCursor;
	
	// re-start the timer
	if( 0 != m_timer)
	{
		KillTimer( m_timer);
		m_timer = 0;
	}
	if( m_sDelay) m_timer = SetTimer( 1010, m_sDelay, NULL);
	else
	{
		m_timer = 1010;
		OnTimer( m_timer);
	}
	return true;
}

// ----------------------------------------------------------------------------
void TFXDataTip::ResetText()
{
	m_listStrings.RemoveAll();
}

void TFXDataTip::AddText(const char* szText)
{
	CString str(szText);
	m_listStrings.AddTail(str);
}

// ----------------------------------------------------------------------------
bool TFXDataTip::IsOn() const
{
	return m_bOn;
}

void TFXDataTip::On(const bool bOn)
{
	m_bOn = bOn;
}

bool TFXDataTip::Hide()
{
	if(!m_bReady) 
		return false;

	DataTipKillTimer();

	// (2006/2/6 - Seung-Won, Bae) Do not release capture without Real Tooltip Hiding.
	if( !m_s_pCurrent) return false;
	if( 100 < m_sDelay) ::ReleaseCapture();

	// reset the current tip window
	m_s_pCurrent = NULL;

	// (2006/4/11 - Seung-Won, Bae) Manage OCX Ctrl CWnd Pointer for LButton Processing on ToolTip.
	m_pChartWnd = NULL;

	// success of hiding
	return (ShowWindow(SW_HIDE) == TRUE ? true: false);
}


// private ====================================================================
void TFXDataTip::Display()
{
	TRACE("TTData - 0\n");
	if(!m_bReady)
		return;

    // hide any other currently visible data tips
	if( 100 < m_sDelay) if( m_s_pCurrent != NULL) m_s_pCurrent->Hide();

	TRACE("TTData - 1\n");
    if(m_listStrings.GetCount() == 0)
		return;

	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&_font);

	// determine the size of the data tip text
	// in order to size the window correctly
	CString	str;
	CSize sizeAll(0, 0), sizeLine(0, 0);
	POSITION pos = m_listStrings.GetHeadPosition();
	while(pos){
		str = m_listStrings.GetNext(pos);
		sizeLine = dc.GetTextExtent(str);
		sizeAll.cy += sizeLine.cy;
		sizeAll.cx = max(sizeAll.cx, sizeLine.cx);
	}

    // capture the mouse in order to be able to
	// determine when to hide the window
	if( 100 < m_sDelay) SetCapture();

	// determine the window size and position
	// displayed above and centered on the origin
	//	by yousun - 04.11.2001
	//	Tooltip의 위치를 Mouse Point의 왼쪽으로 수정
	CRect wndRect(
			m_MousePosition.x - sizeAll.cx - 3,
			m_MousePosition.y + 3,
			m_MousePosition.x - 3,
			m_MousePosition.y + sizeAll.cy + 3);

	// adjust window for offset and borders
	wndRect.InflateRect(2 * _border, _border);

	int nCXScreen = dc.GetDeviceCaps(HORZRES);

	CRect rcMonitor;
	GetMonitorRectFromRect(rcMonitor, &wndRect);
	if(!rcMonitor.IsRectEmpty())
	{
		if(wndRect.bottom > rcMonitor.bottom)
		{
			wndRect.OffsetRect(0, rcMonitor.bottom - wndRect.bottom);
		}
	}

	if(wndRect.left < 0)
		wndRect.OffsetRect(wndRect.Width(), 0);
	else if(wndRect.right > nCXScreen && wndRect.left < nCXScreen)
		wndRect.OffsetRect(wndRect.Width(), 0);
	
	// update window position and display
	SetWindowPos(&wndTop,
			 wndRect.left,
			 wndRect.top,
			 wndRect.Width(),
			 wndRect.Height(),
			 SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_FRAMECHANGED);

	// set the current data tip window
	m_s_pCurrent = this;

	// restore the device context
    dc.SelectObject(pOldFont);

	RedrawWindow();
	TRACE("TTData - 2\n");
}

void TFXDataTip::DataTipKillTimer()
{
	if( m_timer)
	{
		KillTimer( m_timer);
		m_timer = 0;
	}
	if(m_timerToHide)
		KillTimer(m_timerToHide);
	
	m_timer = m_timerToHide = 0;
}

bool TFXDataTip::IsPointNearOtherPoint(const CPoint& point, const CPoint& test, const CSize& distance)
{
	if(point == test)
		return TRUE;
	
	CRect rectOk(point.x - distance.cx, point.y - distance.cy, 
			point.x + distance.cx, point.y + distance.cy);

	return (rectOk.PtInRect(test) == TRUE);
}

void TFXDataTip::SetFont(CFont *pFont)
{
	m_pFont = pFont;
}

void TFXDataTip::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	// determine if a mouse button has been pressed or
	// we have moved outside of the active capture area
	if( 0 < m_sDelay)
	{
		ClientToScreen(&point);
		if(!IsPointNearOtherPoint( m_MousePosition, point, m_TipSurrounding)) Hide();
	}
}


// (2006/4/10 - Seung-Won, Bae) This Class did not support PreTranslateMessage()
LRESULT TFXDataTip::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	LRESULT lResult = CWnd::WindowProc(message, wParam, lParam);

	switch( message)
    {
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_NCLBUTTONDBLCLK:
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONUP:
		case WM_NCRBUTTONDBLCLK:
		case WM_NCMBUTTONDOWN:
		case WM_NCMBUTTONUP:
		case WM_NCMBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_CANCELMODE:
		case WM_MOUSEMOVE:
								// (2006/4/11 - Seung-Won, Bae) Manage OCX Ctrl CWnd Pointer for LButton Processing on ToolTip.
								if( m_pChartWnd)
								{
									POINT lp;
									lp.x = (LONG)(short)LOWORD( lParam);
									lp.y = (LONG)(short)HIWORD( lParam);
									ClientToScreen(&lp);
									m_pChartWnd->ScreenToClient(&lp);
									lParam = MAKELONG(lp.x, lp.y);
									m_pChartWnd->SendMessage( message, wParam, lParam);
								}
								// (2006/4/11 - Seung-Won, Bae) Hide after notify for reset pointer.
								Hide();	
    }
	
	return lResult;
}

void TFXDataTip::GetMonitorRectFromRect(CRect& rcMonitor, LPRECT prcRect)
{
    HMONITOR hMonitor;
    MONITORINFO mi;

    //
    // get the nearest monitor to the passed rect.
    //
    hMonitor = MonitorFromRect(prcRect, MONITOR_DEFAULTTONEAREST);

    //
    // get the work area or entire monitor rect.
    //
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hMonitor, &mi);

	if(mi.dwFlags == MONITORINFOF_PRIMARY)
		rcMonitor = mi.rcWork;		// except taskbar's height
	else
		rcMonitor = mi.rcMonitor;
}

/*
// (2006/4/10 - Seung-Won, Bae) This Class did not support PreTranslateMessage()
BOOL TFXDataTip::PreTranslateMessage(MSG* pMsg) 
{
	MSG	msgNew;

	memcpy(&msgNew, pMsg, sizeof(MSG));
	
	switch(pMsg->message)
    {
	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_NCLBUTTONDBLCLK:
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONUP:
	case WM_NCRBUTTONDBLCLK:
	case WM_NCMBUTTONDOWN:
	case WM_NCMBUTTONUP:
	case WM_NCMBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
    case WM_CANCELMODE:
		//TRACE("\n %04x", msgNew.message);
		POINT lp;
		lp.x = (LONG)(short)LOWORD(msgNew.lParam);
		lp.y = (LONG)(short)HIWORD(msgNew.lParam);
		ClientToScreen(&lp);
		m_pParent->ScreenToClient(&lp);
		msgNew.lParam = MAKELONG(lp.x, lp.y);
		m_pParent->SendMessage(msgNew.message, msgNew.wParam, msgNew.lParam);
		if(m_timerToHide == 0)
			m_timerToHide = SetTimer(1011, GetDoubleClickTime()+100, NULL);
		break;
    }

	return CWnd::PreTranslateMessage(pMsg);
}

// ----------------------------------------------------------------------------
void TFXDataTip::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// (2004.09.14, 배승원) ToolTip이 뜬 상태에서의 Mouse Event를 Parent에게 전달할 경우에는,
	//		Parent의 Client 좌표에 맞춰 전달한다.
	CPoint ptParent( point);
	ClientToScreen( &ptParent);
	m_pParent->ScreenToClient( &ptParent);

	// (2004.09.14, 배승원) Mouse Event Message 전달시 LParam의 값은 Point의 Address값을 보내는 것이 아니다.
	m_pParent->SendMessage( WM_LBUTTONDOWN, 0, MAKELONG( ptParent.x, ptParent.y));

	CWnd::OnLButtonDown(nFlags, point);
}

void TFXDataTip::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// (2004.09.14, 배승원) ToolTip이 뜬 상태에서의 Mouse Event를 Parent에게 전달할 경우에는,
	//		Parent의 Client 좌표에 맞춰 전달한다.
	CPoint ptParent( point);
	ClientToScreen( &ptParent);
	m_pParent->ScreenToClient( &ptParent);

	// (2004.09.14, 배승원) Mouse Event Message 전달시 LParam의 값은 Point의 Address값을 보내는 것이 아니다.
	m_pParent->SendMessage( WM_LBUTTONUP, 0, MAKELONG( ptParent.x, ptParent.y));

	CWnd::OnLButtonUp(nFlags, point);
}

void TFXDataTip::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDblClk(nFlags, point);
	ClientToScreen(&point);
	if(!IsPointNearOtherPoint(m_MousePosition, point, m_TipSurrounding)){
		Hide();
	}
	// (2004.09.14, 배승원) ToolTip이 뜬 상태에서의 Mouse Event를 Parent에게 전달할 경우에는,
	//		Parent의 Client 좌표에 맞춰 전달한다.
	CPoint ptParent( point);
	ClientToScreen( &ptParent);
	m_pParent->ScreenToClient( &ptParent);

	// (2004.09.14, 배승원) Mouse Event Message 전달시 LParam의 값은 Point의 Address값을 보내는 것이 아니다.
	m_pParent->SendMessage(WM_LBUTTONDBLCLK, 0, MAKELONG( ptParent.x, ptParent.y));
	//CWnd::OnLButtonDblClk(nFlags, point);
}

void TFXDataTip::OnRButtonDown(UINT nFlags, CPoint point) 
{
	Hide();
	CWnd::OnRButtonDown(nFlags, point);	
}

void TFXDataTip::OnRButtonUp(UINT nFlags, CPoint point) 
{
	GetCapture();
	// (2004.09.14, 배승원) ToolTip이 뜬 상태에서의 Mouse Event를 Parent에게 전달할 경우에는,
	//		Parent의 Client 좌표에 맞춰 전달한다.
	CPoint ptParent( point);
	ClientToScreen( &ptParent);
	m_pParent->ScreenToClient( &ptParent);

	// (2004.09.14, 배승원) Mouse Event Message 전달시 LParam의 값은 Point의 Address값을 보내는 것이 아니다.
	m_pParent->SendMessage( WM_RBUTTONUP, 0, MAKELONG( ptParent.x, ptParent.y));
	ReleaseCapture();
	CWnd::OnRButtonUp(nFlags, point);
}
*/

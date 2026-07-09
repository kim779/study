// Xcom.cpp : implementation file
//

#include "stdafx.h"
#include "Xcom.h"
#include "xedit.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	COLOR_BUTTON	::GetSysColor(COLOR_BTNFACE)
/////////////////////////////////////////////////////////////////////////////
// CXcom

CXcom::CXcom(UINT id)
{
	m_id = id;
	m_edit = NULL;
	m_bRc.SetRectEmpty();
	m_arr.LoadBitmap(IDB_COMARROW);
	m_bDown = FALSE;
	m_bHover = FALSE;
}

CXcom::~CXcom()
{
	if (m_edit)	delete m_edit;

	if (m_pen.GetSafeHandle())	m_pen.DeleteObject();
	if (m_brush.GetSafeHandle())	m_brush.DeleteObject();
	
	m_arr.DeleteObject();
}


BEGIN_MESSAGE_MAP(CXcom, CWnd)
	//{{AFX_MSG_MAP(CXcom)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SELECT, OnSelect)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXcom message handlers

void CXcom::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	client;
	GetClientRect(client);

	CBrush* pbr;
	CPen*	pen = dc.SelectObject(&m_pen);
	if (m_edit)
		pbr = dc.SelectObject(&(m_edit->m_brush));
	dc.Rectangle(client);
	dc.SelectObject(pen);
	if (m_edit)
		dc.SelectObject(pbr);


	draw_Button(&dc);
}

void CXcom::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bRc.PtInRect(point))
	{
		m_bDown = TRUE;
		if (m_edit)	m_edit->show_History();
		InvalidateRect(m_bRc);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CXcom::init(bool (*callback)(int, WPARAM, LPARAM), CString home)
{
	m_pen.CreatePen(PS_SOLID, 0, COLOR_COMLINE);
	m_brush.CreateSolidBrush(COLOR_BUTTON);

	m_edit = new CXedit(home);
	DWORD dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|ES_LEFT|ES_NOHIDESEL|ES_UPPERCASE|ES_AUTOHSCROLL;
	if (!m_edit->Create(dwStyle, CRect(0), this, -1))
	{
		delete m_edit;
		m_edit = NULL;
		return;
	}

	switch (m_id)
	{
	case CTRL_SCREEN:
		if (m_edit)	m_edit->setlen(4);
		break;
//	case CTRL_CODE:
//		if (m_edit)	m_edit->setlen(5);
//		break;
	}
	m_edit->setaxiscall(callback);
}

void CXcom::set_Owner()
{
	m_edit->set_Owner();
}

void CXcom::changepos()
{
	CRect	cRc;
	
	if (m_edit)
	{
		m_edit->MoveWindow(CRect(0, 0, 0, 0));
		GetClientRect(cRc);
		cRc.right -= COMBTN_WIDTH;
		cRc.DeflateRect(2, 2);
		m_edit->MoveWindow(cRc);
	}

	GetClientRect(cRc);
	m_bRc.CopyRect(&cRc);
#if 0	// old style
	m_bRc.left = cRc.right - (COMBTN_WIDTH+1);
	m_bRc.DeflateRect(0, 1, 1, 1);
#else
	m_bRc.left = cRc.right - COMBTN_WIDTH;
	m_bRc.DeflateRect(0, 2);
	m_bRc.OffsetRect(-2, 0);
#endif
}

void CXcom::draw_Button(CDC* pDC)
{
	if (m_bRc.IsRectEmpty())	return;

	if (m_bDown)
		m_arr.Draw(pDC, m_bRc.left, m_bRc.top, m_bRc.Width(), m_bRc.Height(), 15, 0, SRCCOPY);
	else if (m_bHover)
		m_arr.Draw(pDC, m_bRc.left, m_bRc.top, m_bRc.Width(), m_bRc.Height(), 30, 0, SRCCOPY);
	else 
		m_arr.Draw(pDC, m_bRc.left, m_bRc.top, m_bRc.Width(), m_bRc.Height(), 0, 0, SRCCOPY);
}

#define SRCMASK	0x00220326    // mask
BOOL CXcom::ShowBmp(CDC* pDC, int x, int y, UINT nResID, int maskcolor)
{
	CBitmap	bitmap;
	if (!bitmap.LoadBitmap(nResID))
		return FALSE;
	BITMAP  bm;   
	bitmap.GetBitmap(&bm);
	CRect  r(0,0,bm.bmWidth,bm.bmHeight);

	CDC		 buffDC ;
	buffDC.CreateCompatibleDC( pDC ) ;
	CBitmap  buffBitmap ;
	buffBitmap.CreateCompatibleBitmap( pDC, bm.bmWidth, bm.bmHeight ) ;
	CBitmap* oldbuffBitmap = buffDC.SelectObject( &buffBitmap ) ;

	CDC		 maskDC ;
	maskDC.CreateCompatibleDC( pDC ) ;
	CBitmap  maskBitmap ;  
	maskBitmap.CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL ) ;
	CBitmap* oldmaskBitmap = maskDC.SelectObject( &maskBitmap ) ;

	buffDC.FillSolidRect( &r, maskcolor) ;

	CDC		 memoryDC ;
	memoryDC.CreateCompatibleDC( pDC ) ;
	CBitmap* oldmemoryBitmap = memoryDC.SelectObject(&bitmap/* CBitmap::FromHandle( hBitmap ) */) ;
	buffDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &memoryDC, 0, 0, SRCCOPY ) ;
	maskDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC, 0, 0, SRCCOPY ) ;

	CDC		 copyDC ;
	copyDC.CreateCompatibleDC( pDC ) ;
	CBitmap  copyBitmap ;
	copyBitmap.CreateCompatibleBitmap( pDC, bm.bmWidth, bm.bmHeight ) ;
	CBitmap* oldcopyBitmap = copyDC.SelectObject( &copyBitmap ) ;

	copyDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, pDC,  x, y, SRCCOPY  ) ;
	copyDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, &maskDC,  0, 0, SRCAND  ) ;
	buffDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, &maskDC,  0, 0, SRCMASK  ) ;
	copyDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC,   0, 0, SRCPAINT  ) ;
	pDC->BitBlt(x,y, bm.bmWidth, bm.bmHeight, &copyDC,   0, 0, SRCCOPY  ) ;

	copyDC.SelectObject( oldcopyBitmap ) ;
	memoryDC.SelectObject( oldmemoryBitmap ) ;
	maskDC.SelectObject( oldmaskBitmap ) ;
	buffDC.SelectObject( oldbuffBitmap ) ;
	return TRUE;
}

void CXcom::set_Home()
{
	if (!m_edit)	
		return;

	switch (m_id)
	{
	case CTRL_SCREEN:
		m_edit->SetWindowText("");
		m_edit->SetSel(0, -1);
		m_edit->SetFocus();
		break;
	default:
		break;
	}
}

void CXcom::set_Maps(CString mapname)
{
	if (!m_edit)	return;
	if (mapname.IsEmpty())
	{
		m_edit->SetWindowText(_T(""));
		m_edit->SetFocus();
		return;
	}

	CString	str = mapname.Mid(2, 4);
	m_edit->SetWindowText(str);
}

void CXcom::set_Code(CString code, CString home)
{

	m_edit->set_Code(code, home);
}

void CXcom::set_List(CStringArray& array)
{
	if (m_edit)	m_edit->set_List(array);
}

void CXcom::add_History(CString dat)
{
	if (m_edit)	m_edit->add_History(dat);
}

LRESULT CXcom::OnSelect(WPARAM wParam, LPARAM lParam)
{
	if (m_edit)
	{
		m_edit->m_bSelect = true;
		m_edit->SetSel(0, -1);
		m_edit->SetFocus();
		m_edit->m_bSelect = false;
	}
	return 0;
}

bool CXcom::IsInputFocus()
{
	CWnd*	wnd = GetFocus();
	if (wnd == m_edit)
		return true;
	return false;
}


void CXcom::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bDown = FALSE;
	InvalidateRect(m_bRc);
	CWnd::OnLButtonUp(nFlags, point);
}


void CXcom::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_bHover && m_bRc.PtInRect(point)) {
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER/*|TME_NONCLIENT*/;
		tme.dwHoverTime = 1;
		m_bHover = _TrackMouseEvent(&tme) ? 1:0;
	}	
	CWnd::OnMouseMove(nFlags, point);
}


void CXcom::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bHover = FALSE;
	m_bDown  = FALSE;
	InvalidateRect(m_bRc);
	CWnd::OnMouseLeave();
}


void CXcom::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bRc.PtInRect(point))
	{
		m_bHover = TRUE;
		InvalidateRect(m_bRc);
	}
	CWnd::OnMouseHover(nFlags, point);
}


BOOL CXcom::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	return CWnd::PreTranslateMessage(pMsg);
}

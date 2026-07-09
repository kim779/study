// button.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "button.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF	colBLACK = PALETTERGB(  0,   0,   0);
const COLORREF	colWHITE = PALETTERGB(255, 255, 255);

/////////////////////////////////////////////////////////////////////////////
// Cbutton

Cbutton::Cbutton()
{
	m_bsubClass= false;
	m_toggle   = false;
	m_text     = _T("");
	m_bitmap   = (HBITMAP) NULL;
	m_bitmap_  = (HBITMAP) NULL;
	m_bitmap__ = (HBITMAP) NULL;
	m_tcolor   = colBLACK;
	m_align	   = DT_CENTER;

	m_rect.SetRectEmpty();
	m_bHover = false;
	m_EBcolor = RGB(0,0,0);
}

Cbutton::~Cbutton()
{
	if (m_bitmap)	DeleteObject(m_bitmap);
	if (m_bitmap_)	DeleteObject(m_bitmap_);
	if (m_bitmap__)	DeleteObject(m_bitmap__);
}


BEGIN_MESSAGE_MAP(Cbutton, CButton)
	//{{AFX_MSG_MAP(Cbutton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cbutton message handlers

void Cbutton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CButton::OnLButtonDown(nFlags, point);
	if (m_enable && !m_toggle)	SetState(true);
}

void Cbutton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CButton::OnLButtonUp(nFlags, point);
	if (m_enable && !m_toggle)	SetState();
}

BOOL Cbutton::Attach(const UINT nID, CWnd* pParent, CRect rect, CString file, CString file_, CString file__, CString text)
{
	if (!m_bsubClass)
	{
		if (!SubclassDlgItem(nID, pParent))
			return FALSE;
		m_bsubClass= true;
		m_enable   = true;
	}

	if (!m_toggle)
		m_down = false;

	m_rect     = rect;
	m_text     = text;

#ifdef _DEBUG
	if (m_bitmap)	DeleteObject(m_bitmap);
	if (m_bitmap_)	DeleteObject(m_bitmap_);
	if (m_bitmap__)	DeleteObject(m_bitmap__);

#endif
	m_bitmap   = (HBITMAP)LoadImage(NULL, file,   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_bitmap_  = (HBITMAP)LoadImage(NULL, file_,  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_bitmap__ = (HBITMAP)LoadImage(NULL, file__, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	return TRUE;
}

BOOL Cbutton::Attach(const UINT nID, CWnd* pParent, CRect rect, UINT cool, UINT down, UINT disable, CString text)
{
	if (!SubclassDlgItem(nID, pParent))
		return FALSE;

	m_down     = false;
	m_enable   = true;
	m_rect     = rect;
	m_text     = text;
	m_bitmap   = (HBITMAP)(HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			MAKEINTRESOURCE(cool),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
	m_bitmap_  = (HBITMAP)(HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			MAKEINTRESOURCE(cool),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
	m_bitmap__ = (HBITMAP)(HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			MAKEINTRESOURCE(cool),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
	return TRUE;
}

void Cbutton::SetText(CString text, bool draw)
{
	m_text = text;
	if (draw)
		Invalidate(FALSE);
}

void Cbutton::SetState(bool down)
{
	m_down = down;
	Invalidate(FALSE);
}

void Cbutton::SetEnable(bool enable)
{
	m_enable = enable;
	Invalidate(FALSE);
}

void Cbutton::drawBitmap(CDC* dc, CRect rect)
{
	if (m_bitmap == (HBITMAP) NULL)
	{
		CBrush	brush;
		brush.CreateSolidBrush(RGB(42, 46, 53));
		dc->FillRect(rect, &brush);
		return;
	}

	CDC memDC;
	memDC.CreateCompatibleDC(dc);
	int oldBkMode = memDC.SetBkMode(TRANSPARENT);
	HBITMAP	oldBitmap = NULL;
//	if (m_enable)
//	{
//		oldBitmap = (HBITMAP) memDC.SelectObject(m_bitmap___);
//	}
//	else
	{
		if (m_down && m_bitmap_ != (HBITMAP) NULL)
			oldBitmap = (HBITMAP) memDC.SelectObject(m_bitmap_);
		else if (m_bHover)
			oldBitmap = (HBITMAP) memDC.SelectObject(m_bitmap__);
		else
			oldBitmap = (HBITMAP) memDC.SelectObject(m_bitmap);
	}

	dc->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, m_rect.left, m_rect.top, SRCCOPY);
	if (oldBitmap)	memDC.SelectObject(oldBitmap);
	memDC.SetBkMode(oldBkMode);
}

void Cbutton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CRect	rc;
	CDC*	dc = CDC::FromHandle(lpDIS->hDC);
	
	rc.CopyRect(&lpDIS->rcItem);
	drawBitmap(dc, rc);
	
	if (!m_text.IsEmpty())
	{
		COLORREF oldText = dc->SetTextColor(m_tcolor);

		int bkMode = dc->SetBkMode(TRANSPARENT);
		//dc->DrawText(m_text, rc, m_align|DT_VCENTER|DT_SINGLELINE|DT_LEFT);
		dc->DrawText(m_text, rc, m_align|DT_VCENTER|DT_LEFT);

		dc->SetBkMode(bkMode);
		dc->SetTextColor(oldText);
	}
	
}

void Cbutton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_bHover) {
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER/*|TME_NONCLIENT*/;
		tme.dwHoverTime = 1;
		m_bHover = _TrackMouseEvent(&tme) ? 1:0;
	}	
	
	CButton::OnMouseMove(nFlags, point);
}


void Cbutton::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bHover= true;
	Invalidate();
	CButton::OnMouseHover(nFlags, point);
}


void Cbutton::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bHover = false;
	Invalidate();
	CButton::OnMouseLeave();
}


BOOL Cbutton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rc;
	GetClientRect(&rc);
	//pDC->FillSolidRect(rc, Global.GetRGB(IDX_RGB_BACKGROUND));	
	pDC->FillSolidRect(rc,m_EBcolor);	
	return TRUE;
//	return CButton::OnEraseBkgnd(pDC);
}


int Cbutton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
	//::SetLayeredWindowAttributes(m_hWnd, 0, 200, LWA_ALPHA);
	return 0;
}

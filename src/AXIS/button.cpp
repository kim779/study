// button.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "button.h"
#include "../dll/sm/EnBitmap.h"

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
	m_bTracking = false;
	m_rect.SetRectEmpty();
	m_pToolTip = NULL;
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
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
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

	m_bitmap   = CEnBitmap::LoadImageFile(file, RGB(255, 255, 255));
	m_bitmap_  = CEnBitmap::LoadImageFile(file_, RGB(255, 255, 255));
	m_bitmap__ = CEnBitmap::LoadImageFile(file__, RGB(255, 255, 255));

	/*m_bitmap   = (HBITMAP)LoadImage(NULL, file,   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_bitmap_  = (HBITMAP)LoadImage(NULL, file_,  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_bitmap__ = (HBITMAP)LoadImage(NULL, file__, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);*/
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
		brush.CreateSolidBrush(RGB(255, 255, 255));
		dc->FillRect(rect, &brush);
		return;
	}

	CDC memDC;
	memDC.CreateCompatibleDC(dc);
	const int oldBkMode = memDC.SetBkMode(TRANSPARENT);
	if (!m_enable)
	{
		memDC.SelectObject(m_bitmap__);
	}
	else
	{
		if (m_down && m_bitmap_ != (HBITMAP) NULL)
			memDC.SelectObject(m_bitmap_);
		else
			memDC.SelectObject(m_bitmap);
	}

	dc->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, m_rect.left, m_rect.top, SRCCOPY);
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
		/*
		CPoint	pts[5];
		pts[0].x = rc.left;	pts[0].y = rc.top;
		pts[1].x = rc.left;	pts[1].y = rc.bottom;
		pts[2].x = rc.right;	pts[2].y = rc.bottom;
		pts[3].x = rc.right;	pts[3].y = rc.top;
		pts[4] = pts[0];
		CBrush	brush(colWHITE);
		CBrush*	pOldBrush = dc->SelectObject(&brush);
		dc->Polygon(pts, 5);
		*/
		const COLORREF oldText = dc->SetTextColor(m_tcolor);

		dc->SetBkMode(TRANSPARENT);
		dc->DrawText(m_text, rc, m_align|DT_VCENTER|DT_SINGLELINE);

		dc->SetTextColor(oldText);
//		dc->SelectObject(pOldBrush);
	}
}


void Cbutton::SetToolTipText(BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (m_pToolTip == NULL)
		return;
	CString	spText = _T("");

	spText = GetText();
	spText.TrimLeft();
	if (spText.IsEmpty())
	{
		if (m_pToolTip->GetToolCount() == 1)	
			m_pToolTip->DelTool(this, 1);
		return;
	}

	// Initialize ToolTip
	// If there is no tooltip defined then add it
#if 1
	if (m_pToolTip->GetToolCount() == 1)	
		m_pToolTip-> DelTool(this, 1);

	CRect rectBtn; 
	GetClientRect(rectBtn);
	m_pToolTip->AddTool(this, (LPCTSTR)spText, rectBtn, 1);
#else
	if (m_pToolTip->GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_pToolTip->AddTool(this, (LPCTSTR)spText, rectBtn, 1);
	}
	else
	{
		TOOLINFO	ti;
		ti.uId = 1;
		ti.lpszText = (char*)(const char*)spText;
		m_pToolTip->SetToolInfo(&ti);
	}
#endif
	// Set text for tooltip
	m_pToolTip->UpdateTipText((LPCTSTR)spText, this, 1);
	m_pToolTip->Activate(bActivate);
}

void Cbutton::InitToolTip()
{
	if (m_pToolTip == NULL)
	{
#ifdef DF_USE_CPLUS17
		m_pToolTip = std::make_unique<CToolTipCtrl>();
#else
		m_pToolTip = new CToolTipCtrl;
#endif
		m_pToolTip->Create(this);
		m_pToolTip->Activate(FALSE);
	}
}


void Cbutton::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_pToolTip)
	{
		if (m_pToolTip->GetToolCount() == 0) return;

		m_pToolTip->Activate(bActivate);
	}
}

void Cbutton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
		SetToolTipText();
	}
	CButton::OnMouseMove(nFlags, point);
}

LRESULT Cbutton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	return 0;
}

BOOL Cbutton::PreTranslateMessage(MSG* pMsg) 
{
	if (m_pToolTip && IsWindow(m_pToolTip->GetSafeHwnd()))
		m_pToolTip->RelayEvent(pMsg);		
	return CButton::PreTranslateMessage(pMsg);
}

void Cbutton::OnDestroy() 
{
	CButton::OnDestroy();
	
	if (m_pToolTip)
	{
		m_pToolTip->DestroyWindow();
#ifndef DF_USE_CPLUS17
		delete m_pToolTip;
#endif
	}
}

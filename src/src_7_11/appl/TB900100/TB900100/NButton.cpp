// NButton.cpp : implementation file
//

#include "stdafx.h"
#include "NButton.h"
#include "UnBond.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define SRCMASK			0x00220326    // mask
#define	ID_TOOLTIP	1
#define WM_BTNMSG WM_USER + 888

#define CHECKDOWNLOAD 9999
/////////////////////////////////////////////////////////////////////////////
// CNButton
CNButton::CNButton()
{
	//m_id = id;
	m_over = false;
	m_push = false;
	m_expand = false;
	m_imgMode = false;
	m_defaultColor = 
	m_mode = MODE_3D;
	m_bk = m_overbk = RGB(212, 212, 212);;
	m_ToolTip = NULL;
	m_bMasking = false;
	m_bMaskDraw = false;
	m_rcMasking.SetRectEmpty();
	m_kind = 0;
}

CNButton::CNButton(UINT id, COLORREF color, int mode, bool defaultColor)
{
	m_id = id;
	m_over = false;
	m_push = false;
	m_expand = false;
	m_imgMode = false;
	m_defaultColor = defaultColor;
	m_mode = mode;
	m_bk = m_overbk = color;
	m_ToolTip = NULL;
	m_bMasking = false;
	m_bMaskDraw = false;
	m_rcMasking.SetRectEmpty();
}

CNButton::~CNButton()
{
	if (m_Icool.GetSafeHandle())	m_Icool.DeleteObject();
	if (m_Iover.GetSafeHandle())	m_Iover.DeleteObject();
	if (m_Idown.GetSafeHandle())	m_Idown.DeleteObject();
}


BEGIN_MESSAGE_MAP(CNButton, CWnd)
	//{{AFX_MSG_MAP(CNButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_CREATE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNButton message handlers

void CNButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (m_imgMode)
	{
		if(m_bMaskDraw)
		{
			if(DrawBitmapByMask(&dc, RGB(255,0,255)))
				return;
		}
		else 
		{
			if(DrawImg(&dc))
				return;
		}
	}

	CRect	cRc;
	GetClientRect(cRc);

	if (m_over)
	{
		if (m_push)
			dc.SetBkColor(m_defaultColor ? ::GetSysColor(COLOR_BTNFACE):m_bk);
		else	
			dc.SetBkColor(m_defaultColor ? ::GetSysColor(COLOR_BTNFACE):m_overbk);
	}
	else	
		dc.SetBkColor(m_defaultColor ? ::GetSysColor(COLOR_BTNFACE):m_bk);

	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);

	if (m_push)
	{
		CPoint	point;		GetCursorPos(&point);
		CWnd* wndUnderMouse = WindowFromPoint(point);
		if (wndUnderMouse == this)
		{
			cRc.right--;	cRc.bottom--;
			dc.Draw3dRect(cRc, COLOR_RB, RGB(255, 255, 255));
			cRc.right++;	cRc.bottom++;
			dc.Draw3dRect(cRc, COLOR_RB, COLOR_LT);
		}
		else	dc.Draw3dRect(cRc, COLOR_LT, COLOR_LT);
	}
	else if (m_mode == MODE_3D || m_over)
	{
		dc.Draw3dRect(cRc, COLOR_LT, COLOR_RB);
		cRc.left++;	cRc.top++;
		dc.Draw3dRect(cRc, RGB(255, 255, 255), COLOR_RB);
	}
	else	dc.Draw3dRect(cRc, COLOR_LT, COLOR_LT);
}

void CNButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CUnBond* pwnd = (CUnBond*)GetParent();
	if(pwnd->m_bDonwlading == true)
		return;

	SetCapture();
	m_push = true;
	Invalidate(FALSE);

	CWnd::OnLButtonDown(nFlags, point);
}

void CNButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();

	if(m_kind == 1)
		m_push = false;

	Invalidate(FALSE);

	if (m_over)	
		GetParent()->PostMessage(WM_BTNMSG, MAKEWPARAM(m_id, 0), 0);
	CWnd::OnLButtonUp(nFlags, point);
}

void CNButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd*			wndUnderMouse = NULL;
	CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME;

	CWnd::OnMouseMove(nFlags, point);
	CPoint	lpoint = point;
	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);
	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_over)
		{
			m_over = true;
			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		}
	}
	else 
		m_over = false;
	Invalidate(FALSE);
}

LRESULT CNButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_over = false;
	Invalidate(FALSE);
	return 0;
}

void CNButton::OnDestroy() 
{
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
			m_ToolTip->RelayEvent(NULL);
#if 0
			m_ToolTip->RelayEvent(NULL);
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
#endif
		}
		delete m_ToolTip;
		m_ToolTip = NULL;
	}

	CWnd::OnDestroy();
}

LRESULT CNButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		{
			if (!m_ToolTip)
				break;
			MSG	msg;
			POINT	point;
			
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			msg.hwnd = m_hWnd;
			msg.message = message;
			msg.wParam = wParam;
			msg.lParam = lParam;
			msg.pt = point;
		
			m_ToolTip->RelayEvent(&msg);
		}
		break;
	default:
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

void CNButton::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect	cRc;
	GetClientRect(cRc);

	if (m_ToolTip)			m_ToolTip->SetToolRect(this, ID_TOOLTIP, cRc);
}

void CNButton::set_ToolTipText(CString text, BOOL bActivate)
{
	if (text.IsEmpty())	return;

	InitToolTip();
	if (m_ToolTip->GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip->AddTool(this, text, rectBtn, ID_TOOLTIP);
	}

	m_ToolTip->UpdateTipText(text, this, ID_TOOLTIP);
	m_ToolTip->Activate(bActivate);
}

void CNButton::InitToolTip()
{
	if (!m_ToolTip)
	{
		m_ToolTip = new CToolTipCtrl();
		m_ToolTip->Create(GetDesktopWindow(), TTS_ALWAYSTIP);
		m_ToolTip->SetMaxTipWidth(500);
		m_ToolTip->Activate(TRUE);
	}
}

void CNButton::change_img(CString cool, CString over, CString down)
{
	set_Image(cool, over, down);
	Invalidate();
}

void CNButton::set_Image(UINT nID_cool, UINT nID_over, UINT nID_down)
{
	m_imgMode = true;
	if (m_Icool.GetSafeHandle())	m_Icool.DeleteObject();
	if (m_Iover.GetSafeHandle())	m_Iover.DeleteObject();
	if (m_Idown.GetSafeHandle())	m_Idown.DeleteObject();

	m_Icool.LoadBitmap(nID_cool);
	m_Iover.LoadBitmap(nID_over);
	m_Idown.LoadBitmap(nID_down);
}

void CNButton::set_Image(CString img_cool, CString img_over, CString img_down)
{
	m_imgMode = true;
	if (m_Icool.GetSafeHandle())	m_Icool.DeleteObject();
	if (m_Iover.GetSafeHandle())	m_Iover.DeleteObject();
	if (m_Idown.GetSafeHandle())	m_Idown.DeleteObject();

	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), img_cool,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	m_Icool.Attach(hBitmap);

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), img_over,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	m_Iover.Attach(hBitmap);

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), img_down,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	m_Idown.Attach(hBitmap);
}

void CNButton::set_BkMaskSize(CRect size)
{
	m_rcMasking = size;
	m_bMasking = true;
}

bool CNButton::DrawImg(CDC* pDC)
{
	CRect	bRC;
	GetClientRect(bRC);

	CDC	memDC;
	if (!memDC.CreateCompatibleDC(pDC))
		return false;
	BITMAP	bm;
	int	bx = bRC.Width();
	int	by = bRC.Height();
	CBitmap* oldbuffBitmap = (CBitmap *) 0;
	if (m_over)
	{
		if (m_push)	
			oldbuffBitmap = memDC.SelectObject(&m_Idown);
		else		
			oldbuffBitmap = memDC.SelectObject(&m_Iover);
	}
	else	
	{
		if (m_push)	
			oldbuffBitmap = memDC.SelectObject(&m_Idown);
		else		
			oldbuffBitmap = memDC.SelectObject(&m_Icool);
	}
	
	if (oldbuffBitmap == (CBitmap *) 0)
	{
		memDC.DeleteDC();
		return false;
	}

	m_Icool.GetBitmap(&bm);
	bx = bm.bmWidth;
	by = bm.bmHeight;

	if(m_bMasking)
	{
		transparentBlt(pDC, &m_Icool, bRC.left, bRC.top, bRC.Width(), bRC.Height());
	}
	else
	{
		pDC->StretchBlt(bRC.left, bRC.top, bRC.Width(), bRC.Height(), &memDC, 0, 0, bx, by, SRCCOPY);
		if (!IsWindowEnabled())
		{
			pDC->FillSolidRect(0, 0, bx, by, RGB(140, 140, 140));
			pDC->StretchBlt(bRC.left, bRC.top, bRC.Width(), bRC.Height(), &memDC, 0, 0, bx, by, SRCAND);
		}
		pDC->SetBkMode(TRANSPARENT);

		if(m_push)
			pDC->SetTextColor(RGB(249,249,249));
		else
			pDC->SetTextColor(m_fcolor);

		pDC->SelectObject(m_font);		
		pDC->DrawText(m_text, &bRC, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	memDC.SelectObject(oldbuffBitmap);
	return true;
}

void CNButton::SetMaskDraw(bool bmask, COLORREF backColr)
{
	m_bMaskDraw = bmask;
	m_bkMask = backColr;
}

int CNButton::DrawBitmapByMask(CDC* pDC,int maskcolor)
{
	CRect	bRC;
	GetClientRect(bRC);

	pDC->FillSolidRect(bRC, m_bkMask);

	int bx = bRC.Width();
	int by = bRC.Height();

	CDC		buffDC, maskDC, memoryDC, copyDC;
	CBitmap		buffBitmap, maskBitmap, copyBitmap;
	CBitmap		*oldbuffBitmap, *oldmaskBitmap, *oldmemoryBitmap, *oldcopyBitmap;

	buffDC.CreateCompatibleDC(pDC);
	buffBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldbuffBitmap = buffDC.SelectObject(&buffBitmap);

	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(bx, by, 1, 1, NULL);
	oldmaskBitmap = maskDC.SelectObject(&maskBitmap);

	CRect		maskRc( 0, 0, bx, by);
	buffDC.FillSolidRect(&maskRc, maskcolor);

	memoryDC.CreateCompatibleDC(pDC);

	if(m_over)
	{
		if (m_push)	
			oldmemoryBitmap = memoryDC.SelectObject(&m_Idown);
		else		
			oldmemoryBitmap = memoryDC.SelectObject(&m_Iover);
	}
	else	
		oldmemoryBitmap = memoryDC.SelectObject(&m_Icool);

	buffDC.BitBlt( 0, 0, bx, by, &memoryDC, 0, 0, SRCCOPY);
	maskDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCCOPY);

	copyDC.CreateCompatibleDC(pDC);
	copyBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldcopyBitmap = copyDC.SelectObject(&copyBitmap);

	copyDC.BitBlt( 0, 0, bx, by, pDC,  bRC.left, bRC.top, SRCCOPY);
	copyDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCAND);
	buffDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCMASK);
	copyDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCPAINT);
	pDC->BitBlt(bRC.left, bRC.top, bx, by, &copyDC, 0/*sx*/, 0, SRCCOPY);

	copyDC.SelectObject(oldcopyBitmap);
	memoryDC.SelectObject(oldmemoryBitmap);
	maskDC.SelectObject(oldmaskBitmap);
	buffDC.SelectObject(oldbuffBitmap);

	buffDC.DeleteDC();
	maskDC.DeleteDC();
	memoryDC.DeleteDC();
	copyDC.DeleteDC();
	return true;
}

void CNButton::transparentBlt(CDC *pDC, CBitmap *pBitmap, int x, int y, int cx, int cy)
{
	if (!pBitmap || !pBitmap->GetSafeHandle())
		return;
	
	int index = 0;

	CRect bRc = CRect(x, y, x+cx, y+cy);
	BITMAP  bm;
	pBitmap->GetBitmap(&bm);
	int bx = bm.bmWidth, by = bm.bmHeight;
 	
//	if (m_imageType == IT_INDEX)
 //		bx = bx / 3;

	CDC  buffDC, maskDC, memoryDC, copyDC;
	CBitmap  buffBitmap, maskBitmap, copyBitmap;
	CBitmap  *oldbuffBitmap, *oldmaskBitmap, *oldmemoryBitmap, *oldcopyBitmap;
	
	buffDC.CreateCompatibleDC(pDC);
	buffBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldbuffBitmap = buffDC.SelectObject(&buffBitmap);
	
	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(bx, by, 1, 1, NULL);
	oldmaskBitmap = maskDC.SelectObject(&maskBitmap);
	
	CRect  maskRc( 0, 0, bx, by);
	buffDC.FillSolidRect(&maskRc, RGB(255,0,255));
	
	memoryDC.CreateCompatibleDC(pDC);
	oldmemoryBitmap = memoryDC.SelectObject(pBitmap);
	buffDC.BitBlt( 0, 0, bx, by, &memoryDC, 0, 0, SRCCOPY);
	maskDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCCOPY);
	
	copyDC.CreateCompatibleDC(pDC);
	copyBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldcopyBitmap = copyDC.SelectObject(&copyBitmap);
	
	copyDC.BitBlt( 0, 0, bx, by, pDC, bRc.left, bRc.top, SRCCOPY);
	copyDC.BitBlt( 0, 0, bx, by, &maskDC, index * bx, 0, SRCAND);
	buffDC.BitBlt( 0, 0, bx, by, &maskDC, index * bx, 0, SRCMASK);
	copyDC.BitBlt( 0, 0, bx, by, &buffDC, index * bx, 0, SRCPAINT);
	pDC->StretchBlt(bRc.left, bRc.top, bRc.Width(), bRc.Height(), &copyDC, index * bx, 0, bx, by, SRCCOPY);
	
	copyDC.SelectObject(oldcopyBitmap);
	memoryDC.SelectObject(oldmemoryBitmap);
	maskDC.SelectObject(oldmaskBitmap);
	buffDC.SelectObject(oldbuffBitmap);
	
	buffDC.DeleteDC();
	maskDC.DeleteDC();
	memoryDC.DeleteDC();
	copyDC.DeleteDC();
	
	buffBitmap.DeleteObject();
	maskBitmap.DeleteObject();
	copyBitmap.DeleteObject();
} 


CSize CNButton::get_ImgSize()
{
	BITMAP	bm;
	if (!m_Icool.GetSafeHandle())
		return CSize(0, 0);
	m_Icool.GetBitmap(&bm);

	return CSize(bm.bmWidth, bm.bmHeight);
}


int CNButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	return 0;
}

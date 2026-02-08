// SGButton.cpp : implementation file
//

#include "stdafx.h"
#include "SButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSButton

#define	ID_TOOLTIP	1

CSButton::CSButton(UINT id, COLORREF color, int mode, bool defaultColor)
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
	m_hRgn = NULL;
}

CSButton::CSButton()
{
	m_id = 0;
	m_over = false;
	m_push = false;
	m_expand = false;
	m_imgMode = false;
	m_defaultColor = true;
	m_mode = MODE_3D;
	m_bk = m_overbk = COLOR_DEFAULT;
	m_ToolTip = NULL;
	m_hRgn = NULL;
}

CSButton::~CSButton()
{
	if (m_Icool.GetSafeHandle())	m_Icool.DeleteObject();
	if (m_Iover.GetSafeHandle())	m_Iover.DeleteObject();
	if (m_Idown.GetSafeHandle())	m_Idown.DeleteObject();

	if (CRgn::FromHandle(m_hRgn))	DeleteObject(m_hRgn);
}


BEGIN_MESSAGE_MAP(CSButton, CButton)
	//{{AFX_MSG_MAP(CSButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSButton message handlers
// NButton.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
// CNButton
#include "MapDlg.h"

void CSButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	cRc;

	if (m_imgMode)
	{
		if (DrawImg(&dc))
		{
			CString str;
			GetClientRect(cRc);
			GetWindowText(str);
			CFont* pOld= dc.SelectObject(GetFont());
			dc.SetBkMode(TRANSPARENT);
			dc.DrawText(str, cRc , DT_VCENTER|DT_CENTER|DT_SINGLELINE);
			dc.SelectObject(pOld);
			return;
		}
	}

	GetClientRect(cRc);

	if (m_over)
	{
		if (m_push)
			dc.SetBkColor(m_defaultColor ? ::GetSysColor(COLOR_BTNFACE):m_bk);
		else	
			dc.SetBkColor(m_defaultColor ? ::GetSysColor(COLOR_BTNFACE):m_overbk);
	}
	else	dc.SetBkColor(m_defaultColor ? ::GetSysColor(COLOR_BTNFACE):m_bk);
	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);

	if (m_push)
	{
		CPoint	point;		GetCursorPos(&point);
		const CWnd* wndUnderMouse = WindowFromPoint(point);
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

void CSButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_push = true;
	Invalidate(FALSE);
	CWnd::OnLButtonDown(nFlags, point);
}

void CSButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_push = false;
	Invalidate(FALSE);

	if (m_over)	GetParent()->PostMessage(WM_LBUTTONDOWN);
		//GetParent()->PostMessage(WM_INTERMSG, MAKEWPARAM(m_id, 0), 0);
	CWnd::OnLButtonUp(nFlags, point);
}

void CSButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd*			wndUnderMouse = NULL;
	const CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME{};

	CWnd::OnMouseMove(nFlags, point);
	const CPoint	lpoint = point;
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

LRESULT CSButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_over = false;
	Invalidate(FALSE);
	return 0;
}

void CSButton::OnDestroy() 
{
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
//			m_ToolTip->RelayEvent(NULL);
		}
		//delete m_ToolTip;	m_ToolTip = NULL;
	}

	CWnd::OnDestroy();
}

LRESULT CSButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
			MSG	msg{};
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

void CSButton::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect	cRc;
	GetClientRect(cRc);

	if (m_ToolTip)			m_ToolTip->SetToolRect(this, ID_TOOLTIP, cRc);
	if (CRgn::FromHandle(m_hRgn))	DeleteObject(m_hRgn);

	cRc.right++;	cRc.bottom++;
	m_hRgn = CreateRoundRectRgn(cRc.left, cRc.top, cRc.right, cRc.bottom, 2, 2);
	if (CRgn::FromHandle(m_hRgn))	SetWindowRgn(m_hRgn, TRUE);
}

void CSButton::set_ToolTipText(CString text, BOOL bActivate)
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

void CSButton::InitToolTip()
{
	if (!m_ToolTip)
	{
		m_ToolTip = std::make_unique<CToolTipCtrl>();
		m_ToolTip->Create(GetDesktopWindow(), TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
	}
}

void CSButton::change_img(CString cool, CString over, CString down)
{
	set_Image(cool, over, down);
	Invalidate();
}

void CSButton::set_Image(UINT nID_cool, UINT nID_over, UINT nID_down)
{
	m_imgMode = true;
	if (m_Icool.GetSafeHandle())	m_Icool.DeleteObject();
	if (m_Iover.GetSafeHandle())	m_Iover.DeleteObject();
	if (m_Idown.GetSafeHandle())	m_Idown.DeleteObject();

	m_Icool.LoadBitmap(nID_cool);
	m_Iover.LoadBitmap(nID_over);
	m_Idown.LoadBitmap(nID_down);
}

void CSButton::set_Image(CString img_cool, CString img_over, CString img_down)
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

bool CSButton::DrawImg(CDC* pDC)
{
	CRect	bRC;
	GetClientRect(bRC);

	CDC	memDC;
	if (!memDC.CreateCompatibleDC(pDC))
		return false;

	BITMAP	bm{};
	int	bx = bRC.Width();
	int	by = bRC.Height();
	CBitmap* oldbuffBitmap = (CBitmap *) 0;
	if (m_over)
	{
		if (m_push)	oldbuffBitmap = memDC.SelectObject(&m_Idown);
		else		oldbuffBitmap = memDC.SelectObject(&m_Iover);
	}
	else	oldbuffBitmap = memDC.SelectObject(&m_Icool);
	
	if (oldbuffBitmap == (CBitmap *) 0)
	{
		memDC.DeleteDC();
		return false;
	}

	m_Icool.GetBitmap(&bm);
	bx = bm.bmWidth;
	by = bm.bmHeight;

	pDC->StretchBlt(bRC.left, bRC.top, bRC.Width(), bRC.Height(), &memDC, 0, 0, bx, by, SRCCOPY);
	if (!IsWindowEnabled())
	{
		pDC->FillSolidRect(0, 0, bx, by, RGB(140, 140, 140));
		pDC->StretchBlt(bRC.left, bRC.top, bRC.Width(), bRC.Height(), &memDC, 0, 0, bx, by, SRCAND);
	}
	memDC.SelectObject(oldbuffBitmap);

	return true;
}

CSize CSButton::get_ImgSize()
{
	BITMAP	bm{};
	if (!m_Icool.GetSafeHandle())
		return CSize(0, 0);
	m_Icool.GetBitmap(&bm);

	return CSize(bm.bmWidth, bm.bmHeight);
}

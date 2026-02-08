// NButton.cpp : implementation file
//

#include "stdafx.h"
#include "NButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ID_TOOLTIP	1
/////////////////////////////////////////////////////////////////////////////
// CNButton

CNButton::CNButton(UINT id, COLORREF color, int mode, bool defaultColor, BOOL bCheck, BOOL bToggle, BOOL bRound, bool bMask)
{
	m_id      = id;
	m_over    = false;
	m_push    = false;
	m_expand  = false;
	m_imgMode = false;
	m_bMask   = bMask;
	m_mode    = mode;
	m_defaultColor = defaultColor;
	m_bk = m_overbk = color;
	m_ToolTip    = NULL;
	m_bFullImage = FALSE;
	m_bRound     = bRound;
	m_bToggle    = bToggle;
	m_bCheck     = bCheck;
	m_bChecked   = FALSE;
	m_bCBtnMsg   = FALSE;
	m_bText      = FALSE;
	m_sText.Empty();
	m_hRgn = nullptr;
	m_pFont = nullptr;
}

CNButton::~CNButton()
{
	if (m_Icool.GetSafeHandle())	m_Icool.DeleteObject();
	if (m_Iover.GetSafeHandle())	m_Iover.DeleteObject();
	if (m_Idown.GetSafeHandle())	m_Idown.DeleteObject();
	if (m_IFull.GetSafeHandle())	m_IFull.DeleteObject();

	if (CRgn::FromHandle(m_hRgn))	DeleteObject(m_hRgn);
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
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNButton message handlers

void CNButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (m_imgMode)
	{
		if (m_bFullImage)
		{
			if (DrawFullImg(&dc))
				return;
		}
		else
		{
			if (DrawImg(&dc))
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
	else	dc.SetBkColor(m_defaultColor ? ::GetSysColor(COLOR_BTNFACE):m_bk);
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
	if (m_bCheck)
	{
		m_bChecked = !m_bChecked;
		GetParent()->PostMessage(WM_USER, MAKEWPARAM(m_id, 0), (LPARAM)m_bChecked);
		Invalidate(FALSE);
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}
	SetCapture();
	
	m_push = true;
	Invalidate(FALSE);

	CWnd::OnLButtonDown(nFlags, point);
}

void CNButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bCheck)
	{
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}
	ReleaseCapture();
	m_push = false;
	Invalidate(FALSE);

	if (m_over)	
	{
		if (m_bCBtnMsg)
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(m_id, BN_CLICKED), (LPARAM)GetSafeHwnd());
		else
			GetParent()->PostMessage(WM_USER, MAKEWPARAM(m_id, 0), 0);
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CNButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	CWnd*	wndUnderMouse = nullptr;
	CWnd*	wndActive = this;
	TRACKMOUSEEVENT	csTME{};

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
			MSG	msg{};

			m_ToolTip->RelayEvent(&msg);
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
		}
		m_ToolTip.reset();
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
			if (!m_ToolTip || !IsWindow(m_ToolTip->GetSafeHwnd()))
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

void CNButton::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect	cRc;
	GetClientRect(cRc);

	if (m_ToolTip)			m_ToolTip->SetToolRect(this, ID_TOOLTIP, cRc);

	if (m_bRound)
	{
		if (CRgn::FromHandle(m_hRgn))	DeleteObject(m_hRgn);
		
		cRc.right++;	cRc.bottom++;
		m_hRgn = CreateRoundRectRgn(cRc.left, cRc.top, cRc.right, cRc.bottom, 2, 2);
		if (CRgn::FromHandle(m_hRgn))	SetWindowRgn(m_hRgn, TRUE);
	}
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
		m_ToolTip = std::make_unique<CToolTipCtrl>();
		m_ToolTip->Create(GetDesktopWindow(), TTS_ALWAYSTIP);
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
	m_bFullImage = FALSE;
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
	m_bFullImage = FALSE;
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

void CNButton::set_Image(CString fullImage)
{
	m_imgMode = true;
	m_bFullImage = TRUE;
	if (m_IFull.GetSafeHandle())	m_IFull.DeleteObject();

	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), fullImage,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	m_IFull.Attach(hBitmap);
}

bool CNButton::DrawImg(CDC* pDC)
{
	CRect	bRC;
	CDC	memDC;

	if (!memDC.CreateCompatibleDC(pDC))
		return false;

	GetClientRect(bRC);

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
	if (m_bText && !m_sText.IsEmpty())
	{
		CFont *pOldFont = NULL;
		if (GetFont())
			pOldFont = pDC->SelectObject(GetFont());
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(m_sText, bRC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		if (pOldFont)
			pDC->SelectObject(pOldFont);
	}
	memDC.SelectObject(oldbuffBitmap);

	return true;
}

bool CNButton::DrawFullImg(CDC *pDC)
{
	CRect	bRC;
	CDC	memDC;
	if (!m_bMask && !memDC.CreateCompatibleDC(pDC))
		return false;

	GetClientRect(bRC);

	int	bx = 0, by = 0;
	if (!m_expand)
	{
		bx = bRC.Width();
		by = bRC.Height();
	}else
	{
		BITMAP bm;
		m_IFull.GetBitmap(&bm);
		bx = bm.bmWidth/3;
		by = bm.bmHeight;
	}

	int	nIdx = 0;

	if (m_over)
	{
		if (m_push)	nIdx = 2;
		else		nIdx = 1;
	}

	if (m_bToggle && m_bCheck)
	{
		if (m_bChecked)
			nIdx += 3;
	}
	else if (m_bCheck)
	{
		if (m_bChecked)	nIdx = 2;
		else
		{
			if (m_over)	nIdx = 1;
			else		nIdx = 0;
		}
	}

	if (m_bMask)
	{
		pDC->FillSolidRect(bRC, m_bk);
		DrawBitmapByMask(pDC, &m_IFull, CPoint(bRC.left, bRC.top), RGB(255, 0, 255), CRect(bx *nIdx, 0, bx *nIdx + bRC.Width(), bRC.Height()));
	}
	else
	{
		CBitmap* oldbuffBitmap = (CBitmap *) memDC.SelectObject(&m_IFull);
		if (oldbuffBitmap == (CBitmap *) 0)
		{
			memDC.DeleteDC();
			return false;
		}
		
		//pDC->BitBlt(bRC.left, bRC.top, bRC.Width(), bRC.Height(), &memDC, bx * nIdx, 0, SRCCOPY);
		pDC->StretchBlt(bRC.left, bRC.top, bRC.Width(), bRC.Height(), &memDC, bx * nIdx, 0,bx,by,SRCCOPY);
		if (!IsWindowEnabled())
		{
			pDC->FillSolidRect(0, 0, bx, by, RGB(140, 140, 140));
			pDC->BitBlt(bRC.left, bRC.top, bRC.Width(), bRC.Height(), &memDC, bx * nIdx, 0, SRCAND);
		}
		memDC.SelectObject(oldbuffBitmap);
	}
	return true;
}

CSize CNButton::get_ImgSize()
{
	BITMAP	bm{};

	if (m_bFullImage)
	{
		if (!m_IFull.GetSafeHandle())
			return CSize(0, 0);

		m_IFull.GetBitmap(&bm);
		return CSize(bm.bmWidth / 3, bm.bmHeight);
	}
	else
	{
		if (!m_Icool.GetSafeHandle())
			return CSize(0, 0);

		m_Icool.GetBitmap(&bm);
	}

	return CSize(bm.bmWidth, bm.bmHeight);
}

BOOL CNButton::GetCheck()
{
	return m_bChecked;
}

void CNButton::Setcheck(BOOL bCheck)
{
	m_bChecked = bCheck;
}

void CNButton::SetCButtomMsg(BOOL bTF)
{
	m_bCBtnMsg = bTF;
}

void CNButton::SetText(CString sText)
{
	m_bText = TRUE;
	m_sText = sText;
}

void CNButton::SetFont(CFont *pFont)
{
	m_pFont = pFont;
}

CFont* CNButton::GetFont()
{
	return m_pFont;
}


#define SRCMASK			0x00220326    // mask
void CNButton::DrawBitmapByMask(CDC *pDC, CBitmap *pBitmap, CPoint pos, COLORREF maskcolor, CRect bmpRc)
{
	BITMAP	bm{};

	if (bmpRc.Width() == 0 && bmpRc.Height() == 0)
	{
		pBitmap->GetBitmap(&bm);
		bmpRc.left = bmpRc.top = 0;
		bmpRc.right = bm.bmWidth;
		bmpRc.bottom = bm.bmHeight;
	}
	
	CDC	buffDC, maskDC, memoryDC, copyDC;
	CBitmap	buffBitmap, maskBitmap, copyBitmap;

	buffDC.CreateCompatibleDC(pDC);
	buffBitmap.CreateCompatibleBitmap(pDC, bmpRc.Width(), bmpRc.Height());
	CBitmap* oldbuffBitmap = buffDC.SelectObject(&buffBitmap);

	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(bmpRc.Width(), bmpRc.Height(), 1, 1, NULL);
	CBitmap* oldmaskBitmap = maskDC.SelectObject(&maskBitmap);

	const	CRect	maskRc( 0, 0, bmpRc.Width(), bmpRc.Height());
	buffDC.FillSolidRect(&maskRc, maskcolor);

	memoryDC.CreateCompatibleDC(pDC);
	CBitmap* oldmemoryBitmap = memoryDC.SelectObject(pBitmap);
	buffDC.BitBlt( 0, 0, bmpRc.Width(), bmpRc.Height(), &memoryDC, bmpRc.left, bmpRc.top, SRCCOPY);
	maskDC.BitBlt( 0, 0, bmpRc.Width(), bmpRc.Height(), &buffDC, 0, 0, SRCCOPY);

	copyDC.CreateCompatibleDC(pDC);
	copyBitmap.CreateCompatibleBitmap(pDC, bmpRc.Width(), bmpRc.Height());
	CBitmap* oldcopyBitmap = copyDC.SelectObject(&copyBitmap);

	copyDC.BitBlt( 0, 0, bmpRc.Width(), bmpRc.Height(), pDC,  pos.x, pos.y, SRCCOPY);
	copyDC.BitBlt( 0, 0, bmpRc.Width(), bmpRc.Height(), &maskDC, 0, 0, SRCAND);
	buffDC.BitBlt( 0, 0, bmpRc.Width(), bmpRc.Height(), &maskDC, 0, 0, SRCMASK);
	copyDC.BitBlt( 0, 0, bmpRc.Width(), bmpRc.Height(), &buffDC, 0, 0, SRCPAINT);
	pDC->BitBlt(pos.x, pos.y, bmpRc.Width(), bmpRc.Height(), &copyDC, 0, 0, SRCCOPY);

	copyDC.SelectObject(oldcopyBitmap);
	memoryDC.SelectObject(oldmemoryBitmap);
	maskDC.SelectObject(oldmaskBitmap);
	buffDC.SelectObject(oldbuffBitmap);

	buffDC.DeleteDC();
	maskDC.DeleteDC();
	memoryDC.DeleteDC();
	copyDC.DeleteDC();
}

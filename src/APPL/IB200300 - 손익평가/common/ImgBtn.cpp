
#include "stdafx.h"
#include "ImgBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_TIPS		100

CImgBtn::CImgBtn()
{
	m_capture    = false;
	m_hBitmap    = NULL;
	m_hBitmap_dn = NULL;
	m_hBitmap_hv = NULL;
	m_bTracking  = FALSE;
	m_bHover     = FALSE;
	m_bCheck     = false;
	m_bChecked   = false;
	m_bToolTip   = true;
	m_pFont      = NULL;
	m_pTipCtrl   = NULL;
	m_clText     = RGB(0,0,0);
	m_bFocus     = false;
}

CImgBtn::~CImgBtn()
{
}


BEGIN_MESSAGE_MAP(CImgBtn, CButton)
	//{{AFX_MSG_MAP(CImgBtn)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	ON_MESSAGE(WM_SETFONT, HandleSetFont)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_DRAWITEM()
	ON_MESSAGE(BM_SETCHECK, OnSetCheck)
	ON_MESSAGE(BM_GETCHECK, OnGetCheck)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()


void CImgBtn::OnPaint() 
{
	CPaintDC dc(this);
	
	CRect rc;
	CString txt;
	GetClientRect(&rc);

	dc.SetBkMode(TRANSPARENT);

	if (m_hBitmap)
	{
		CDC		memDC;
		CBitmap		*pOldBitmap, *pBitmap;

		if (m_bCheck && m_bChecked && m_hBitmap_dn)
		{
			pBitmap = CBitmap::FromHandle(m_hBitmap_dn);
		}
		else if (m_capture && m_bHover && m_hBitmap_dn)
			pBitmap = CBitmap::FromHandle(m_hBitmap_dn);
		else if (m_bHover && m_hBitmap_hv)
			pBitmap = CBitmap::FromHandle(m_hBitmap_hv);
		else
			pBitmap = CBitmap::FromHandle(m_hBitmap);

		BITMAP stBitmapInfo ;
		pBitmap->GetBitmap(&stBitmapInfo);

		memDC.CreateCompatibleDC(&dc);	
		if (memDC.m_hDC != NULL) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
		//rc.DeflateRect(1, 1);
		dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
	
	if (!m_hBitmap)
		dc.DrawFrameControl(rc, DFC_BUTTON, DFCS_BUTTONPUSH | (m_capture && m_bHover?DFCS_PUSHED:NULL));
	
	CFont *pOldFont;
	if (!m_pFont)
		m_pFont = GetParent()->GetFont();
	pOldFont = dc.SelectObject(m_pFont);

	GetWindowText(txt);
	txt.TrimLeft();
	
	if (m_hBitmap)
	{
		rc.right = BitmapSize(m_hBitmap).cx;
		rc.bottom = BitmapSize(m_hBitmap).cy;
	}
	//rc.DeflateRect(2, 2, 0, 0);
	if (m_capture && m_bHover)
		rc.OffsetRect(1, 1);
	
//	if (IsWindowEnabled())
		dc.SetTextColor(m_clText);
		/*
	else
	{
		dc.SetTextColor(RGB(255, 255, 255));
		rc.OffsetRect(1, 1);
		dc.DrawText(txt, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		rc.OffsetRect(-1, -1);
		dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	}
	*/
	dc.DrawText(txt, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	if (m_bFocus)
	{
		rc.DeflateRect(1, 1, 2, 2);
		dc.DrawFocusRect(rc);
	}
	dc.SelectObject(pOldFont);
}

void CImgBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	if (m_capture)
		ReleaseCapture();
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}

void CImgBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CImgBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_capture)
		ReleaseCapture();
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CImgBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_capture)
	{
		CRect rc;
		m_capture = false;
		ReleaseCapture();
		GetClientRect(&rc);
		if (rc.PtInRect(point))
		{
			if (m_bCheck)
				m_bChecked = !m_bChecked;
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
		}
		Invalidate();
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CImgBtn::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	m_bHover=TRUE;
	Invalidate();
}


LRESULT CImgBtn::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;

	Invalidate();
	return 0;
}

BOOL CImgBtn::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CWnd::OnEraseBkgnd(pDC);
}

CSize CImgBtn::BitmapSize(HBITMAP hBitmap)
{
	if (!hBitmap)
		return CSize(0, 0);
	BITMAP BM;
	GetObject(hBitmap, sizeof(BITMAP), &BM);
	return CSize((int)BM.bmWidth, (int)BM.bmHeight);
}

void CImgBtn::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	
	Invalidate();	
}

bool CImgBtn::IsChecked()
{
	return m_bChecked;
}

void CImgBtn::setCheck(bool bCheck)
{
	m_bChecked = bCheck;
	Invalidate();
}

void CImgBtn::HandleSetFont(WPARAM wParam, LPARAM lParam)
{
	m_pFont = (CFont*)CFont::FromHandle((HFONT)wParam);
	if (m_pTipCtrl)
		m_pTipCtrl->SetFont(m_pFont);
	if (lParam)
		RedrawWindow();
}

int CImgBtn::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

LRESULT CImgBtn::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;
	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
		if (m_pTipCtrl)
			m_pTipCtrl->RelayEvent(&msg);
	return CWnd::WindowProc(message, wParam, lParam);
}

CString CImgBtn::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

LOGFONT CImgBtn::setFont(int fsize, CString fname, BOOL bBold, BOOL bItalic)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = fsize * 10; 
	
	strcpy(lf.lfFaceName, fname);   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = (bBold?FW_BLACK:FW_NORMAL);
	lf.lfItalic = bItalic;
	return lf;
}

void CImgBtn::setTextColor(COLORREF clText)
{
	m_clText = clText;
	RedrawWindow();
}

void CImgBtn::OnSetCheck(WPARAM wParam, LPARAM lParam)
{
	if (wParam == BST_CHECKED)
		setCheck();
	else if (wParam == BST_UNCHECKED)
		setCheck(false);
}

long CImgBtn::OnGetCheck(WPARAM wParam, LPARAM lParam)
{
	if (IsChecked())
		return BST_CHECKED;
	else
		return BST_UNCHECKED;
	return NULL;
}

void CImgBtn::CreateTooltip()
{
	CString txt;
	GetWindowText(txt);

	if (txt.Find("\n") >= 0)
		SetWindowText(Parser(txt, "\n"));
	
	CRect rc;
	GetClientRect(&rc);
	/*
	m_pTipCtrl = new CToolTipCtrl;
	m_pTipCtrl->Create(this);
	m_pTipCtrl->AddTool(this, txt, &rc, GetDlgCtrlID());
	m_pTipCtrl->Activate(TRUE);
	*/
	return;	
}

void CImgBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	
}

void CImgBtn::PreSubclassWindow() 
{
	if (m_bToolTip && !m_pTipCtrl)
		CreateTooltip();
	CButton::PreSubclassWindow();
}

void CImgBtn::OnDestroy() 
{
	if (m_pTipCtrl)
	{
		m_pTipCtrl->Activate(TRUE);
		delete m_pTipCtrl;
		m_pTipCtrl = NULL;
	}
	CButton::OnDestroy();
}

void CImgBtn::FitImageSize()
{
	if (m_hBitmap)
	{
		CSize sz = BitmapSize(m_hBitmap);
		SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOZORDER|SWP_NOMOVE);
	}
}

void CImgBtn::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	
	m_bFocus = true;
	RedrawWindow();
}

void CImgBtn::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);
	
	m_bFocus = false;
	RedrawWindow();
}

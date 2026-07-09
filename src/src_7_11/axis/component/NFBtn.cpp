// NFBtn.cpp : implementation file
//

#include "stdafx.h"
#include "NFBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNFBtn

CNFBtn::CNFBtn()
{
	m_capture = false;
	m_bCheck = false;
	m_hBitmap = NULL;
	m_hBitmap2 = NULL;
	m_pTipCtrl = NULL;
	m_ImageList.Create(16, 16, ILC_COLOR24|ILC_MASK, 4, 0);
}

CNFBtn::~CNFBtn()
{
	if (m_pTipCtrl)
		delete m_pTipCtrl;
	m_ImageList.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CNFBtn, CWnd)
	//{{AFX_MSG_MAP(CNFBtn)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_SETFONT, HandleSetFont)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNFBtn message handlers

void CNFBtn::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	CString txt;
	GetClientRect(&rc);

	m_dwState = 0;
	if (GetState() & 0x0004)
		m_dwState = DFCS_PUSHED;

	dc.DrawFrameControl(rc, DFC_BUTTON, DFCS_BUTTONPUSH | m_dwState);
	dc.SetBkMode(TRANSPARENT);

	if (m_hBitmap)
	{
		CBitmap *pBitmap;
		pBitmap = CBitmap::FromHandle(m_hBitmap);
		m_ImageList.Add(pBitmap, RGB(0, 255, 255));
		m_hBitmap = NULL;
	}
	if (m_hBitmap2)
	{
		CBitmap *pBitmap;
		pBitmap = CBitmap::FromHandle(m_hBitmap2);
		m_ImageList.Add(pBitmap, RGB(0, 255, 255));
		m_hBitmap2 = NULL;
	}

	if (m_ImageList.GetImageCount())
	{
		m_ImageList.Draw(&dc, ((GetState() & 0x0004) && m_ImageList.GetImageCount() == 2)?1:0, CPoint(1, 1), ILD_NORMAL);
	}
	else
	{
		GetWindowText(txt);
		dc.DrawText(txt, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	// Do not call CButton::OnPaint() for painting messages
}

void CNFBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}

void CNFBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	Invalidate();
	CWnd::OnMouseMove(nFlags, point);
}

void CNFBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CNFBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_capture)
	{
		CRect rc;
		m_capture = false;
		ReleaseCapture();
		GetClientRect(&rc);
		if (rc.PtInRect(point))
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
		Invalidate();
	}
	CWnd::OnLButtonUp(nFlags, point);
}

DWORD CNFBtn::GetState()
{
	if (m_capture || m_bCheck)
		return 0x0004;

	return 0;
}

void CNFBtn::SetCheck(bool bCheck)
{
	m_bCheck = bCheck;
	Invalidate();
}

bool CNFBtn::GetCheck()
{
	return m_bCheck;
}

int CNFBtn::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CString txt;
	GetWindowText(txt);
	SetWindowText(Parser(txt, "\n"));

	LOGFONT	lf;
	lf = setFont(9, "", false, false);
	m_font.CreatePointFontIndirect(&lf);

	CRect rc;
	GetClientRect(&rc);
	m_pTipCtrl = new CToolTipCtrl;
	m_pTipCtrl->Create(this);
	m_pTipCtrl->AddTool(this, txt, &rc, GetDlgCtrlID());
	m_pTipCtrl->Activate(TRUE);
	
	return 0;
}

LRESULT CNFBtn::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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

CString CNFBtn::Parser(CString &srcstr, CString substr)
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

LOGFONT CNFBtn::setFont(int fsize, CString fname, BOOL bBold, BOOL bItalic)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = fsize * 10; 
	
	strcpy_s(lf.lfFaceName, fname);   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = (bBold?FW_BLACK:FW_NORMAL);
	lf.lfItalic = bItalic;
	return lf;
}

LRESULT CNFBtn::HandleSetFont(WPARAM wParam, LPARAM lParam)
{
	m_pFont = (CFont*)CFont::FromHandle((HFONT)wParam);
	m_pTipCtrl->SetFont(m_pFont);
	if (lParam)
		RedrawWindow();
	return 0;
}

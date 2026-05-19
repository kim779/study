// MPropGridButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MPropGridButton.h"
#include "resource.h"

#define	ID_INBUTTON	1

// CMPropGridButton

IMPLEMENT_DYNAMIC(CMPropGridButton, CWnd)

CMPropGridButton::CMPropGridButton()
{
	m_capture  = false;
	m_check    = false;
	m_hbitmap  = NULL;
	m_hbitmap2 = NULL;
	m_imageList.Create(16, 16, ILC_COLOR24|ILC_MASK, 4, 0);
}

CMPropGridButton::~CMPropGridButton()
{
	m_imageList.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CMPropGridButton, CWnd)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMPropGridButton 메시지 처리기입니다.


int CMPropGridButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CString	tmps;
	LOGFONT	lf;
	int	pos;

	GetWindowText(tmps);
	pos = tmps.Find(_T("\n"));
	if (pos != -1)
		tmps = tmps.Left(pos);
	SetWindowText(tmps);

	setFont(&lf, 9, _T(""), false, false);
	m_font.CreatePointFontIndirect(&lf);
	return 0;
}


void CMPropGridButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}


void CMPropGridButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_capture)
	{
		CRect	rc;

		m_capture = false;
		ReleaseCapture();
		GetClientRect(&rc);
		if (rc.PtInRect(point))
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
		Invalidate();
	}
	CWnd::OnLButtonUp(nFlags, point);
}


void CMPropGridButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDblClk(nFlags, point);
}


void CMPropGridButton::OnMouseMove(UINT nFlags, CPoint point)
{
	Invalidate();
	CWnd::OnMouseMove(nFlags, point);
}


void CMPropGridButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect	rc;
	CString	tmps;

	GetClientRect(&rc);
	m_state = 0;
	if (getState() & 0x0004)
		m_state = DFCS_PUSHED;

	dc.DrawFrameControl(rc, DFC_BUTTON, DFCS_BUTTONPUSH | m_state);
	dc.SetBkMode(TRANSPARENT);

	if (m_hbitmap)
	{
		CBitmap *pBitmap;
		pBitmap = CBitmap::FromHandle(m_hbitmap);
		m_imageList.Add(pBitmap, RGB(0, 255, 255));
		m_hbitmap = NULL;
	}
	if (m_hbitmap2)
	{
		CBitmap *pBitmap;
		pBitmap = CBitmap::FromHandle(m_hbitmap2);
		m_imageList.Add(pBitmap, RGB(0, 255, 255));
		m_hbitmap2 = NULL;
	}

	if (m_imageList.GetImageCount())
		m_imageList.Draw(&dc, ((getState() & 0x0004) && m_imageList.GetImageCount() == 2) ? 1  :0, CPoint(1, 1), ILD_NORMAL);
	else
	{
		GetWindowText(tmps);
		dc.DrawText(tmps, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}

void CMPropGridButton::setFont(LOGFONT* lf, int fsize, CString fname, BOOL bBold, BOOL bItalic)
{
	ZeroMemory(lf, sizeof(LOGFONT));
	lf->lfHeight         = fsize * 10; 
	lf->lfCharSet        = DEFAULT_CHARSET;
	lf->lfOutPrecision   = OUT_DEFAULT_PRECIS;
	lf->lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	lf->lfQuality        = DEFAULT_QUALITY;
	lf->lfPitchAndFamily = DEFAULT_PITCH;
	lf->lfWeight         = (bBold ? FW_BLACK : FW_NORMAL);
	lf->lfItalic         = bItalic;
	strcpy_s(lf->lfFaceName, LF_FACESIZE, fname);   
}

DWORD CMPropGridButton::getState()
{
	if (m_capture || m_check)
		return 0x00000004;

	return 0;
}

// AxMToolTip.cpp : implementation file
//

#include "stdafx.h"
#include "AxMToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* _toolTipCtrlClassName = "__axMToolTipCtrlClass";
/////////////////////////////////////////////////////////////////////////////
// CAxMToolTip

CAxMToolTip::CAxMToolTip()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, _toolTipCtrlClassName, &wndcls)))
	{
		wndcls.style		= CS_SAVEBITS;
		wndcls.lpfnWndProc	= ::DefWindowProc;
		wndcls.cbClsExtra	= wndcls.cbWndExtra = 0;
		wndcls.hInstance	= hInst;
		wndcls.hIcon		= NULL;
		wndcls.hCursor		= LoadCursor(hInst, IDC_ARROW);
		wndcls.hbrBackground	= (HBRUSH)(COLOR_INFOBK + 1); 
		wndcls.lpszMenuName	= NULL;
		wndcls.lpszClassName	= _toolTipCtrlClassName;

		if (!AfxRegisterClass(&wndcls))	AfxThrowResourceException();
	}

	m_font.CreatePointFont(90, "±¼¸²");
}

CAxMToolTip::~CAxMToolTip()
{
}


BEGIN_MESSAGE_MAP(CAxMToolTip, CWnd)
	//{{AFX_MSG_MAP(CAxMToolTip)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxMToolTip message handlers

LRESULT CAxMToolTip::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_ERASEBKGND)
		return TRUE;
	
	return CWnd::WindowProc(message, wParam, lParam);
}

int CAxMToolTip::Create()
{
	DWORD dwStyle = WS_BORDER | WS_POPUP; 
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;

	return CreateEx(dwExStyle, _toolTipCtrlClassName, NULL, dwStyle, 
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		            NULL, NULL, NULL );
}


int CAxMToolTip::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_crBack = GetSysColor(COLOR_INFOBK);
	
	return 0;
}

void CAxMToolTip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(rc);

	CDC mdc;
	mdc.CreateCompatibleDC(&dc);
	CBitmap bmp, *oldBmp;
	bmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());

	oldBmp = mdc.SelectObject(&bmp);
	DrawTip(&mdc);

	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
	mdc.SelectObject(oldBmp);
}

void CAxMToolTip::DrawTip(CDC* dc)
{
	CRect rc, rectC;
	GetClientRect(rc);
	dc->FillSolidRect(rc, m_crBack);	
	CFont* pfontOld = dc->SelectObject(&m_font);

	dc->DrawText(m_text, rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	dc->SelectObject(pfontOld);
}

const int GAB = 2;

CSize CAxMToolTip::SetText(LPCTSTR text)
{
	m_text = text;

	if (!m_text.IsEmpty())
		m_text.Delete(0, 1);

	CWindowDC dc(this);

	CFont* oldFont = dc.SelectObject(&m_font);
	CSize sz = dc.GetTextExtent(m_text);
	dc.SelectObject(oldFont);

	sz.cx += GAB * 2;
	sz.cy += GAB * 2;

	return sz;
}

void CAxMToolTip::WindowMove(int x, int y, int cx, int cy)
{
	SetWindowPos(&wndTop, x, y, cx, cy, SWP_NOACTIVATE);
	Invalidate();
}


void CAxMToolTip::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	switch (nIDEvent) {
	case 1: 
		ShowWindow(SW_SHOW); 
		SetTimer(2, 2000, NULL);
		break;
	case 2:
		ShowWindow(SW_HIDE);
		break;
	}

	CWnd::OnTimer(nIDEvent);
}


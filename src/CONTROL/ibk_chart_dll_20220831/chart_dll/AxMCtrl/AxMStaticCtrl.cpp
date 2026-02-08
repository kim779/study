// AxMStaticCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AxMCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxMStaticCtrl

CAxMStaticCtrl::CAxMStaticCtrl()
{
	m_transparent = TRUE;
	m_textColor = clBlack;
	m_textColorHover = clBlue;

	m_bgColor = clSkyBlue;
	m_bgColorPercent = 70;
	m_bgShadeID = 3;

	m_brush.CreateSolidBrush(m_bgColor);

	m_mouseOverControl = FALSE;
	m_messageHandler = NULL;
}

CAxMStaticCtrl::~CAxMStaticCtrl()
{
}


BEGIN_MESSAGE_MAP(CAxMStaticCtrl, CStatic)
	//{{AFX_MSG_MAP(CAxMStaticCtrl)
	ON_MESSAGE(AXM_SETMSGHANDLER, OnSetMessageHandler)
	ON_MESSAGE(AXM_GETMSGHANDLER, OnGetMessageHandler)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxMStaticCtrl message handlers

void CAxMStaticCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);
	CString str;
	GetWindowText(str);

	CDC mdc;
	mdc.CreateCompatibleDC(&dc);
	CBitmap bmp, *oldBmp;
	bmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());

	CFont *font, *oldFont;

	font = GetFont();
	oldFont = (CFont*)mdc.SelectObject(font);

	oldBmp = (CBitmap*)mdc.SelectObject(&bmp);

	mdc.FillSolidRect(&rc, m_bgColor);
//	CAxMShade shade;
//	shade.DrawShade(&mdc, rc, m_bgShadeID, m_bgColor, m_bgColorPercent);

	mdc.SetTextColor(m_mouseOverControl ? m_textColorHover: m_textColor);
	mdc.SetBkMode(TRANSPARENT);
	mdc.DrawText(str, -1, rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	CAxMBorder bdr;
	bdr.DrawBorder(&mdc, rc, clGray);

	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);

	mdc.SelectObject(oldBmp);
	mdc.SelectObject(oldFont);
}

LRESULT CAxMStaticCtrl::Notify(UINT msg, WPARAM wParam, LPARAM lParam)
{
	NOTIFY(msg, wParam, lParam);
}

LRESULT CAxMStaticCtrl::OnSetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	SetMessageHandler((CWnd*)wParam);
	return 0L;
}

LRESULT CAxMStaticCtrl::OnGetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)GetMessageHandler();
}


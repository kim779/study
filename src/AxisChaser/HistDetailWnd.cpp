// HistDetailWnd.cpp : implementation of CHistDetailWnd
#include "stdafx.h"
#include "AxisChaser.h"
#include "HistDetailWnd.h"

BEGIN_MESSAGE_MAP(CHistDetailWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

CHistDetailWnd::CHistDetailWnd()
{
}

CHistDetailWnd::~CHistDetailWnd()
{
	m_font.DeleteObject();
}

int CHistDetailWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_edit.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
		ES_MULTILINE | ES_READONLY | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		CRect(0, 0, 0, 0), this, 1);

	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN, _T("Consolas"));
	m_edit.SetFont(&m_font);

	return 0;
}

void CHistDetailWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (m_edit.GetSafeHwnd())
		m_edit.MoveWindow(0, 0, cx, cy);
}

void CHistDetailWnd::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CHistDetailWnd::ShowText(CWnd* parent, const CString& title, const CString& text)
{
	if (!m_hWnd)
	{
		CRect rc(100, 100, 900, 700);
		CreateEx(WS_EX_TOOLWINDOW, AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, 0,
				(HBRUSH)(COLOR_WINDOW + 1), 0),
			title, WS_OVERLAPPEDWINDOW, rc, parent, 0, NULL);
	}
	SetWindowText(title);
	m_edit.SetWindowText(text);
	ShowWindow(SW_SHOW);
	SetForegroundWindow();
}

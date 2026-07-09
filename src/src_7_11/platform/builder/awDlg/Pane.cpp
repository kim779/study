// Pane.cpp : implementation file
//

#include "stdafx.h"
#include "Pane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPane

CPane::CPane()
{
	m_bkClr = RGB(255, 255, 255);
	m_frClr = RGB(0, 0, 0);
	m_bxClr = m_bkClr;
}

CPane::~CPane()
{
}


BEGIN_MESSAGE_MAP(CPane, CWnd)
	//{{AFX_MSG_MAP(CPane)
	ON_WM_PAINT()
	ON_MESSAGE(WM_SETTEXT, HandleSetText)
	ON_MESSAGE(WM_GETTEXT, HandleGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLength)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPane message handlers

void CPane::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	CFont	*pFont, *pOldFont;
	DWORD	dwStyle = DT_SINGLELINE|DT_VCENTER;

	if (GetStyle() & SS_CENTER)
		dwStyle |= DT_CENTER;
	else if (GetStyle() & SS_LEFT)
		dwStyle |= DT_LEFT;
	else if (GetStyle() & SS_RIGHT)
		dwStyle |= DT_RIGHT;

	GetClientRect(&rc);

	pFont = GetFont();
	pOldFont = dc.SelectObject(pFont);
	
	dc.FillSolidRect(rc, m_bkClr);
	
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_frClr);
	rc.DeflateRect(2, 0, 2, 0);
	dc.DrawText(m_text, rc, dwStyle);
	rc.InflateRect(2, 0, 2, 0);

	dc.Draw3dRect(rc, m_bxClr, m_bxClr);
	dc.SelectObject(pOldFont);

	// Do not call CStatic::OnPaint() for painting messages
}

void CPane::setColor(COLORREF bkClr, COLORREF frClr, COLORREF bxClr)
{
	m_bkClr = bkClr;
	m_frClr = frClr;
	m_bxClr = bxClr;
}

LRESULT CPane::HandleSetText(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
	{
		m_text = (char*)lParam;
		Invalidate();
	}
	return 0;
}

long CPane::HandleGetText(WPARAM wParam, LPARAM lParam)
{
	lstrcpyn((LPTSTR)lParam, (LPCTSTR)m_text, wParam);
	if ((int)wParam > m_text.GetLength())
		wParam = m_text.GetLength();
	return wParam;
}

long CPane::OnGetTextLength(WPARAM, LPARAM)
{
	return m_text.GetLength();
}

// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000A1.h"
#include "MapWnd.h"
#include "MapForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd()
{
	m_form = NULL;
}

CMapWnd::~CMapWnd()
{
	if (m_form)
		delete m_form;
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers
BOOL CMapWnd::CreateMap(CWnd* parent)
{
	m_parent = parent;
	int	cx = 436;
	int	cy = 620;

	if (!Create(NULL, "", WS_CHILD|WS_VISIBLE, CRect(0, 0, cx, cy), m_parent, (int) this))
		return FALSE;

	m_form = new CMapForm();
	if (!m_form->CreateMap(this, m_parent))
	{
		delete m_form;
		m_form = NULL;
		return FALSE;
	}

	CRect	cRc;
	GetClientRect(cRc);
	m_form->MoveWindow(0, 0, cx, cy);
	SetTimer(1000, 50, NULL);
	return TRUE;
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_form && IsWindow(m_form->m_hWnd))
	{
		CRect	cRc;
		GetClientRect(cRc);
		m_form->MoveWindow(0, 0, cRc.Width(), cRc.Height());
	}
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	 cRc;

	GetClientRect(cRc);
	dc.SetBkColor(clrBkGnd);
	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);
}

long CMapWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	if (m_form && IsWindow(m_form->m_hWnd))
		return m_form->OnUser(wParam, lParam);
	return 0;
}

BOOL CMapWnd::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
		TRACE("!!!!!!!!!!!\n");
	return CWnd::PreTranslateMessage(pMsg);
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);
	
	CEdit *pEdit = (CEdit*)m_form->GetDlgItem(IDC_PASS);
	if (pEdit)
	{
		pEdit->SetSel(0, -1, FALSE);
		pEdit->SetFocus();
	}
	CWnd::OnTimer(nIDEvent);
}

m// AxTabView.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "AxTabView.h"
#include "ntab.h"
#include "XCaption.h"

#include "axmsg.hxx"
#include "../h/axisvar.h"
#include "../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxTabView

IMPLEMENT_DYNCREATE(CAxTabView, CView)

CAxTabView::CAxTabView()
{
	m_tab		= NULL;
	m_axisres	= NULL;
}

CAxTabView::~CAxTabView()
{
	if (m_tab)	delete m_tab;
	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axCLOSEV, m_key), 0);
}


BEGIN_MESSAGE_MAP(CAxTabView, CView)
	//{{AFX_MSG_MAP(CAxTabView)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxTabView drawing

void CAxTabView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAxTabView diagnostics

#ifdef _DEBUG
void CAxTabView::AssertValid() const
{
	CView::AssertValid();
}

void CAxTabView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAxTabView message handlers

BOOL CAxTabView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_CLIPCHILDREN;	
	return CView::PreCreateWindow(cs);
}

void CAxTabView::SizeWindow(CSize size)
{
	if (!m_tab)	return;

	CWnd*	parent = GetParent();
	int x = GetSystemMetrics(SM_CXBORDER)*2;
	int y = TABH + GetSystemMetrics(SM_CYBORDER)*2 + XCAP_HEIGHT;
	int gap = 4;//GetSystemMetrics(SM_CXFRAME);

	m_tab->SetWindowPos(&wndBottom, 0, XCAP_HEIGHT/*0*/, size.cx + gap, size.cy + gap + TABH, SWP_SHOWWINDOW);
	SetWindowPos(&wndTop, x, y, size.cx, size.cy, SWP_SHOWWINDOW);
}

void CAxTabView::ChangeTab(CString aMapN)
{
	if (m_tab)	m_tab->ChangeTab(aMapN);
}

void CAxTabView::JustWindow(CString aMapN, bool resize)
{
	if (!m_tab)	return;
	ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);
	int	x, y, cx, cy, gap;
	CRect	cRc, wRc;
	GetClientRect(cRc);
	GetWindowRect(wRc);
//TRACE("cRc [%d][%d] wRc[%d][%d]\t", cRc.Width(), cRc.Height(), wRc.Width(), wRc.Height());
	ScreenToClient(&wRc);

	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME);
	x = GetSystemMetrics(SM_CXBORDER)*2;
	y = TABH + GetSystemMetrics(SM_CYBORDER)*2;
	gap = 0;//GetSystemMetrics(SM_CXFRAME)/2;
	cx = wRc.Width() + gap;	cy = wRc.Height() + gap;

	m_tab->SetWindowPos(&wndBottom, 0, XCAP_HEIGHT/*0*/, cx, cy, SWP_SHOWWINDOW);
	m_tab->ChangeTab(aMapN);
//TRACE("cx %d cy %d\n", cx, cy);
	//if (!resize) cy -= TABH;
	SetWindowPos(&wndTop, x, y + XCAP_HEIGHT, cRc.Width(), cRc.Height() - TABH, SWP_SHOWWINDOW);
}

void CAxTabView::JustWindow(CString home, CString key, class CTMenu* menu, 
			    CString aMapN, bool (*axiscall)(int, WPARAM, LPARAM))
{
	ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);
	if (!m_tab)
	{
		m_tab = new CNTab(home, key, menu, aMapN, axiscall, m_key);
		if (!m_tab->CreateEx(WS_EX_CLIENTEDGE, NULL, "tabview", WS_CHILD|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, CRect(0), GetParent(), (int) m_tab))
		{
			delete m_tab;
			m_tab = NULL;
			::MessageBox(m_hWnd, "Error, Tab create fail.", "ERROR", MB_ICONASTERISK );
		}
		else	m_tab->SetInfo(m_axisres);
	}

	int	x, y, cx, cy, gap;
	CRect	cRc, wRc;
	GetClientRect(cRc);
	GetWindowRect(wRc);
//TRACE("cRc [%d][%d] wRc[%d][%d]\t", cRc.Width(), cRc.Height(), wRc.Width(), wRc.Height());
	ScreenToClient(&wRc);

	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME);
	x = GetSystemMetrics(SM_CXBORDER)*2;
	y = TABH + GetSystemMetrics(SM_CYBORDER)*2;
	gap = 0;//GetSystemMetrics(SM_CXFRAME);
	cx = wRc.Width() + gap;
	cy = wRc.Height() +gap;

//TRACE("cx %d cy %d\n", cx, cy);
	if (m_tab)	m_tab->SetWindowPos(&wndBottom, 0, XCAP_HEIGHT/*0*/, cx, cy, SWP_SHOWWINDOW);
	SetWindowPos(&wndTop, x, y + XCAP_HEIGHT, cRc.Width(), cRc.Height() - TABH, SWP_SHOWWINDOW);
	if (m_tab)	m_tab->ChangeTab(aMapN);
}

void CAxTabView::ChangeSkin()
{
	if (!m_tab)	return;
	m_tab->ChangeSkin();
}

void CAxTabView::SetInfo(int key, class _axisRes* axisres)
{
	m_key		= key;
	m_axisres	= axisres;
}

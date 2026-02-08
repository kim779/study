// AxTabView.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "AxTabView.h"
#include "ntab.h"
#include "XCaption.h"

#include "axmsg.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxTabView
#pragma warning(disable : 26409)
IMPLEMENT_DYNCREATE(CAxTabView, CView)
#pragma warning (default : 26409)
CAxTabView::CAxTabView()
{
	m_tab		= NULL;

	m_resourceHelper = ResourceHelper();
}

CAxTabView::~CAxTabView()
{
#ifndef DF_USE_CPLUS17
	if (m_tab)	
		delete m_tab;
#endif
	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axCLOSEV, m_key), 0);
}


BEGIN_MESSAGE_MAP(CAxTabView, CView)
	//{{AFX_MSG_MAP(CAxTabView)
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxTabView drawing

void CAxTabView::OnDraw(CDC* pDC)
{
	const CDocument* pDoc = GetDocument();
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

	JustView();
	/**
	CWnd*	parent = GetParent();
	int x = GetSystemMetrics(SM_CXBORDER)*2;
	int y = TABH + GetSystemMetrics(SM_CYBORDER)*2 + XCAP_HEIGHT;
	int gap = GetSystemMetrics(SM_CXFRAME);

	m_tab->SetWindowPos(&wndBottom, 0, XCAP_HEIGHT, size.cx + gap, size.cy + gap + TABH, SWP_SHOWWINDOW);
	SetWindowPos(&wndTop, x, y, size.cx, size.cy, SWP_SHOWWINDOW);
	**/
}

void CAxTabView::ChangeTab(CString aMapN)
{
	if (m_tab)	m_tab->ChangeTab(aMapN);
}

void CAxTabView::JustWindow(CString aMapN, bool resize)
{
	
	if (!m_tab)	 return;

	m_mapN = aMapN;
	//ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);

	JustView(resize);
}

void CAxTabView::JustWindow(CString home, CString key, class CTMenu* menu, 
			    CString aMapN, bool (*axiscall)(int, WPARAM, LPARAM), bool resize)
{
	m_mapN = aMapN;
	//ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);

	if (m_tab && !m_tab->m_hWnd)
	{
#ifdef DF_USE_CPLUS17
		m_tab.reset();
#else
		delete m_tab;
		m_tab = NULL;
#endif
	}

	if (!m_tab)
	{
#ifdef DF_USE_CPLUS17
		m_tab = std::make_unique<CNTab>(key, menu, aMapN, axiscall, m_key);
		if (!m_tab->CreateEx(WS_EX_CLIENTEDGE, NULL, "tabview", WS_CHILD | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0,0,0,0), GetParent(), (int)m_tab.get()))
		{
			m_tab.reset();
			Axis::MessageBox(this, "Error, Tab create fail.", MB_ICONASTERISK);
		}
#else
		m_tab = new CNTab(key, menu, aMapN, axiscall, m_key);
		if (!m_tab->CreateEx(WS_EX_CLIENTEDGE, NULL, "tabview", WS_CHILD|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, CRect(0), GetParent(), (int) m_tab))
		{
			delete m_tab;
			m_tab = NULL;
			Axis::MessageBox(this, "Error, Tab create fail.", MB_ICONASTERISK);
		}
#endif
	}

	JustView(resize);
}

void CAxTabView::ChangeSkin()
{
	if (!m_tab)	return;
	m_tab->ChangeSkin();
}

void CAxTabView::SetInfo(int key)
{
	m_key = key;
	if (m_tab) m_tab->SetInfo(key);
}

void CAxTabView::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{	
	CView::OnNcCalcSize(bCalcValidRects, lpncsp);

	const int VF = 2;
	lpncsp->rgrc[0].top  -= VF;
	lpncsp->rgrc[0].left -= VF;
	lpncsp->rgrc[0].right += VF;
	lpncsp->rgrc[0].bottom += VF;
}


void CAxTabView::JustView(bool resize)
{
	if (!m_tab)
		return;

	int	x{}, y{}, cx{}, cy{}, gap{};
	CRect	cRc, wRc;
	GetClientRect(cRc);
	GetWindowRect(wRc);

	ScreenToClient(&wRc);

	x = 0;
	y = TABH;
	gap = 0;//GetSystemMetrics(SM_CXFRAME);
	cx = wRc.Width() + gap;
	cy = wRc.Height() +gap;

	m_tab->SetWindowPos(&wndBottom, 
			-2, XCAP_HEIGHT - 2, 
			cx + 4, cy + 2, SWP_SHOWWINDOW);
	if (resize)
		SetWindowPos(&wndTop, x, y + XCAP_HEIGHT, cRc.Width(), cRc.Height()-TABH , SWP_SHOWWINDOW);
	else
		SetWindowPos(&wndTop, x, y + XCAP_HEIGHT, cRc.Width(), cRc.Height() , SWP_SHOWWINDOW);

	m_tab->ChangeTab(m_mapN);
}

BOOL CAxTabView::IsControlKey(UINT key)
{
	const UINT controlKeys[] = 
	{
		VK_RETURN, VK_BACK, VK_LEFT, VK_UP, VK_TAB, VK_RIGHT, VK_DOWN, NULL
	};
	
	for (int i = 0; controlKeys[i]; i++)
	{
		if (key == controlKeys[i])
			return TRUE;
	}
	return FALSE;
}

BOOL CAxTabView::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYUP || pMsg->message == WM_KEYDOWN)
	{
		const CWnd *pWnd = GetWindow(GW_CHILD);
		if (pWnd && pWnd->m_hWnd)
		{
			const BOOL bRet = pWnd->SendMessage(WM_USER+1234, 0, LPARAM(pMsg)); // to WebBrowser
			if (bRet && IsControlKey(pMsg->wParam))
				return TRUE;
		}	
	}
	return CView::PreTranslateMessage(pMsg);
}

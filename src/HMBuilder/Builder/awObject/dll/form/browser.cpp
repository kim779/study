// browser.cpp : implementation file
//

#include "stdafx.h"
#include "browser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Cbrowser

IMPLEMENT_DYNCREATE(Cbrowser, CHtmlView)

Cbrowser::Cbrowser()
{
	m_url.Empty();
	//{{AFX_DATA_INIT(Cbrowser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

Cbrowser::~Cbrowser()
{
	
}

void Cbrowser::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Cbrowser)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Cbrowser, CHtmlView)
	//{{AFX_MSG_MAP(Cbrowser)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cbrowser diagnostics

#ifdef _DEBUG
void Cbrowser::AssertValid() const
{
	CHtmlView::AssertValid();
}

void Cbrowser::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Cbrowser message handlers

void Cbrowser::OnDestroy() 
{
	CHtmlView::OnDestroy();
	delete this;
}

BOOL Cbrowser::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.style |= WS_CLIPCHILDREN;
	return CHtmlView::PreCreateWindow(cs);
}

void Cbrowser::OnPaint() 
{
//	Refresh2(REFRESH_IFEXPIRED);
	CHtmlView::OnPaint();
}

void Cbrowser::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TM_WAIT)
	{
		if (m_url.IsEmpty())
			KillTimer(TM_WAIT);
		else if (!GetBusy())
		{
			KillTimer(TM_WAIT);
			Navigate2(m_url);
			m_url.Empty();
		}
	}

	CHtmlView::OnTimer(nIDEvent);
}

void Cbrowser::xNavigate(CString url)
{
	if (url.IsEmpty())
		return;

	if (GetBusy())
	{
		m_url = url;
		SetTimer(TM_WAIT, TMI_WAIT, NULL);
	}
	else
	{
		KillTimer(TM_WAIT);
		Navigate2(url);
		Sleep(150);
	}
}

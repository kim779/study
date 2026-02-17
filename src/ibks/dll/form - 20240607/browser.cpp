// browser.cpp : implementation file
//

#include "stdafx.h"
#include "browser.h"
#include <atlbase.h>

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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cbrowser diagnostics

#ifdef _DEBUG
void Cbrowser::AssertValid() const
{
	//CHtmlView::AssertValid();
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

void Cbrowser::OnPaint() 
{
//	Refresh2(REFRESH_IFEXPIRED);
	CHtmlView::OnPaint();
}

void Cbrowser::OnTimer(UINT nIDEvent) 
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

BOOL Cbrowser::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.style |= WS_CLIPCHILDREN;
	return CHtmlView::PreCreateWindow(cs);
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

void Cbrowser::navigate(CString url)
{
	int value = url.Find('?');
	if (value == -1)
	{
		Navigate2(url);
		return;
	}

	CString	text, url2;
	VARIANT vtPostData;

	url2 = url.Left(value+1);
	text = url.Mid(value+1);
TRACE("[%s, %s]\n", url2, text);
	value = text.GetLength();

	SAFEARRAY*	psa = SafeArrayCreateVector(VT_UI1, 0, value+1);
	if (!psa)
	{
		Navigate2(url);
		return;
	}

	LPSTR	wb;
	if (SafeArrayAccessData(psa, (LPVOID*)&wb) != S_OK)
	{
		Navigate2(url);
		return;
	}

	CopyMemory(wb, text, value);
	wb[value] = '\0';

	if (SafeArrayUnaccessData(psa) != S_OK)
	{
		Navigate2(url);
		return;
	}

	V_VT(&vtPostData) = VT_ARRAY|VT_UI1;
	V_ARRAY(&vtPostData) = psa;

	Navigate2(url2, 0, (char *)&vtPostData, NULL, NULL);
}
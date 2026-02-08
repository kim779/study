// ClearWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB300600.h"
#include "ClearWnd.h"
#include "MapWnd.h"
#include "Define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClearWnd

CClearWnd::CClearWnd(CWnd* pWizard)
	: m_pWizard(pWizard)
	, m_nMapKey(0)
{
}

CClearWnd::~CClearWnd()
{
}


BEGIN_MESSAGE_MAP(CClearWnd, CWnd)
	//{{AFX_MSG_MAP(CClearWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CClearWnd message handlers

void CClearWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMapWnd* pWnd = (CMapWnd*)GetParent();
	
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, pWnd->GetIndexColor(CLR_PANNEL_BACK));
}

void CClearWnd::Init()
{
	m_nMapKey = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(attachMAP, NULL), (LPARAM)this);

	if (m_nMapKey < 0)
		return;

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_nMapKey), (LPARAM)MAPNO_CLEAR);
}

void CClearWnd::OnDestroy() 
{
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(detachMAP, m_nMapKey), 0L);
	CWnd::OnDestroy();	
}

void CClearWnd::SetPallete()
{
	Invalidate();
}

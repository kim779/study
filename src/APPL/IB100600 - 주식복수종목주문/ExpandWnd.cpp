// ExpandWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB100600.h"
#include "ExpandWnd.h"
#include "MapWnd.h"
#include "Define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExpandWnd

CExpandWnd::CExpandWnd(CWnd* pWizard)
	: m_pWizard(pWizard)
	, m_nMapKey(0)
{
}

CExpandWnd::~CExpandWnd()
{
}


BEGIN_MESSAGE_MAP(CExpandWnd, CWnd)
	//{{AFX_MSG_MAP(CExpandWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CExpandWnd message handlers

void CExpandWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMapWnd* pWnd = (CMapWnd*)GetParent();
	
	CRect rcClient;
	GetClientRect(&rcClient);

//	dc.FillSolidRect(&rcClient, pWnd->GetIndexColor(CLR_PANNEL_BACK));
//	dc.FillSolidRect(&rcClient, 0x0000ff);

}

void CExpandWnd::Init()
{
	// Attach Map
	m_nMapKey = m_pWizard->SendMessage(WM_USER, 
					MAKEWPARAM(attachMAP, vtypeNRM), (LPARAM)this);

	//vtypeNRM
	if (m_nMapKey < 0)
		return;

	// Load Map
	m_pWizard->SendMessage(WM_USER, 
					MAKEWPARAM(loadMAP, m_nMapKey), (LPARAM)MAPNO_FHOGA);

}

void CExpandWnd::OnDestroy() 
{
	// Detach Map
	m_pWizard->SendMessage(WM_USER, 
					MAKEWPARAM(detachMAP, m_nMapKey), 0L);
	CWnd::OnDestroy();
}

void CExpandWnd::SetPallete()
{

}
/*
long CExpandWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	return m_pWizard->SendMessage(WM_USER, wParam, lParam)
}
*/


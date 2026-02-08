// ExpandWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB100700.h"
#include "ExpandWnd.h"
#include "MapWnd.h"

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CExpandWnd message handlers

void CExpandWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMapWnd* pWnd = (CMapWnd*)GetParent();
	
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, pWnd->GetIndexColor(CLR_PANNEL_BACK));
}

void CExpandWnd::Init()
{
	m_nMapKey = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(attachMAP, vtypeNRM), (LPARAM)this);

	if (m_nMapKey < 0)
		return;

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_nMapKey), (LPARAM)MAPNO_SHOGA);
}

void CExpandWnd::OnDestroy() 
{
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(detachMAP, m_nMapKey), 0L);
	CWnd::OnDestroy();
}

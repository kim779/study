// ExpandWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ib611000.h"
#include "ExpandWnd.h"

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
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}

void CExpandWnd::OnDestroy() 
{
	m_pWizard->SendMessage(WM_USER, 
					MAKEWPARAM(detachMAP, m_nMapKey), 0L);

	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
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
		MAKEWPARAM(loadMAP, m_nMapKey), (LPARAM)"IB611010");
}
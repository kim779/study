// WarningWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB100600.h"
#include "WarningWnd.h"
#include "Define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWarningWnd

CWarningWnd::CWarningWnd(CWnd* pWizard)
: m_pWizard(pWizard)
	, m_nMapKey(0)
{

}

CWarningWnd::~CWarningWnd()
{
}


BEGIN_MESSAGE_MAP(CWarningWnd, CWnd)
	//{{AFX_MSG_MAP(CWarningWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWarningWnd message handlers

void CWarningWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}

void CWarningWnd::OnDestroy() 
{
	m_pWizard->SendMessage(WM_USER, 
		MAKEWPARAM(detachMAP, m_nMapKey), 0L);
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CWarningWnd::Init()
{
	// Attach Map
	CString mapname;
	mapname = "IB100620";
	const int type = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(typeDLL, 0), 
		(long) mapname.operator LPCTSTR());
	m_nMapKey = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(attachMAP, type), (long)this);
	//vtypeNRM
	if (m_nMapKey < 0)
		return;
	
	// Load Map
	
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_nMapKey), (LPARAM)WARN_WND);
}


void CWarningWnd::SetPallete()
{
	
}

void CWarningWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);
	switch(nIDEvent)
	{
	case 9999:
		{
		
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

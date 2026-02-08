// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_OOPSise.h"
#include "MainWnd.h"
#include "ControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd()
{
}

CMainWnd::~CMainWnd()
{
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers
void CMainWnd::SetParam(struct	_param* param, CWnd *parent)
{
	m_param = param;
	m_pAxisWnd = parent;

}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_pControl = new CControlWnd();
	if( m_pControl->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, m_param->rect, this, 100))
	{
		m_pControl->SetParam(m_param, m_pAxisWnd);
	}
	
	return 0;
}

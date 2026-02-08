// Chaser.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "Chaser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChaser

CChaser::CChaser(CWnd* parent)
{
	m_parent = parent;
}

CChaser::~CChaser()
{
}


BEGIN_MESSAGE_MAP(CChaser, CWnd)
	//{{AFX_MSG_MAP(CChaser)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnChaser)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChaser message handlers

LRESULT CChaser::OnChaser(WPARAM wParam, LPARAM lParam)  //vc2019 voic->LRESULT
{
	if (m_parent->GetSafeHwnd())
		m_parent->SendMessage(WM_CHASER, wParam, lParam);

	return 0;
}
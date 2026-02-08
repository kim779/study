// STimeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "IB771000.h"
#include "STimeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTimeCtrl

CSTimeCtrl::CSTimeCtrl()
{
	m_bk.CreateSolidBrush(RGB(255,255,255));
}

CSTimeCtrl::~CSTimeCtrl()
{
	if (m_bk.GetSafeHandle())
		m_bk.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSTimeCtrl, CDateTimeCtrl)
	//{{AFX_MSG_MAP(CSTimeCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSTimeCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return CDateTimeCtrl::OnEraseBkgnd(pDC);
}


HBRUSH CSTimeCtrl::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)m_bk;
}

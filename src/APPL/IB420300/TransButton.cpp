// TransButton.cpp : implementation file
//

#include "stdafx.h"
#include "ib420300.h"
#include "TransButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransButton

CTransButton::CTransButton()
{
}

CTransButton::~CTransButton()
{
}


BEGIN_MESSAGE_MAP(CTransButton, CButton)
	//{{AFX_MSG_MAP(CTransButton)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransButton message handlers

HBRUSH CTransButton::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

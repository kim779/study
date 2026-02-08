// NumMASpinCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "chartenvsetup.h"
#include "NumMASpinCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNumMASpinCtrl

CNumMASpinCtrl::CNumMASpinCtrl()
{
}

CNumMASpinCtrl::~CNumMASpinCtrl()
{
}


BEGIN_MESSAGE_MAP(CNumMASpinCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CNumMASpinCtrl)
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumMASpinCtrl message handlers

void CNumMASpinCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	AfxMessageBox("´­¸²");
	*pResult = 0;
}

// ToolTip.cpp : implementation file
//

#include "stdafx.h"
#include "IB771000.h"
#include "ToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTip

CToolTip::CToolTip()
{
}

CToolTip::~CToolTip()
{
}


BEGIN_MESSAGE_MAP(CToolTip, CToolTipCtrl)
	//{{AFX_MSG_MAP(CToolTip)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolTip message handlers

void CToolTip::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Add your specialized code here and/or call the base class

	CToolTipCtrl::DoDataExchange(pDX);

}

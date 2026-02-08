// WarningReserve.cpp : implementation file
//

#include "stdafx.h"
#include "IB300200.h"
#include "WarningReserve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWarningReserve dialog


CWarningReserve::CWarningReserve(CWnd* pParent /*=NULL*/)
	: CDialog(CWarningReserve::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWarningReserve)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWarningReserve::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWarningReserve)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWarningReserve, CDialog)
	//{{AFX_MSG_MAP(CWarningReserve)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWarningReserve message handlers

HBRUSH CWarningReserve::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->GetDlgCtrlID() == IDC_STATICRED)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
		
		pDC->SetBkMode(TRANSPARENT);
	}
	
	return hbr;
}

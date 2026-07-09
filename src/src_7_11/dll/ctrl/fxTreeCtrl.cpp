// fxTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "fxTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfxTreeCtrl

CfxTreeCtrl::CfxTreeCtrl()
{
}

CfxTreeCtrl::~CfxTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CfxTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CfxTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomTreeDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfxTreeCtrl message handlers
void CfxTreeCtrl::OnCustomTreeDraw(NMHDR * pNMHDR, LRESULT * pResult)
{
	*pResult = CDRF_DODEFAULT;
	LPNMTVCUSTOMDRAW pCDRW = (LPNMTVCUSTOMDRAW) pNMHDR;
	switch( pCDRW->nmcd.dwDrawStage ) 
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW ;	// ask for item notifications
		break ;
	case CDDS_ITEMPREPAINT:
		{
			if(pCDRW->nmcd.uItemState & CDIS_SELECTED)
			{
				pCDRW->clrTextBk = ::GetSysColor(COLOR_HIGHLIGHT);
				pCDRW->clrText = RGB(255, 255, 255);
				return;
			}
		}
		break;
	default:
		break;
	}
}
// fxIGrid2.cpp: implementation of the CfxIGrid2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fxIGrid2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CfxIGrid2::CfxIGrid2()
{

}

CfxIGrid2::~CfxIGrid2()
{

}

BEGIN_MESSAGE_MAP(CfxIGrid2, CfxIGrid)
	//{{AFX_MSG_MAP(CContGrid)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CfxIGrid2::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	return CfxIGrid::OnMouseWheel(nFlags, zDelta, pt);
}

void CfxIGrid2::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	CfxIGrid::OnLButtonDown(nFlags, point);
}

void CfxIGrid2::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();
	//AfxMessageBox("DblClicked");
	
	CfxIGrid::OnLButtonDblClk(nFlags, point);
	GetParent()->SendMessage(WM_USER+13);
	
}
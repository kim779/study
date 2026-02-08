// ContGrid.cpp: implementation of the CContGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ContGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContGrid::CContGrid()
{

}

CContGrid::~CContGrid()
{

}

BEGIN_MESSAGE_MAP(CContGrid, CfxIGrid)
	//{{AFX_MSG_MAP(CContGrid)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COLORREF CContGrid::GetCellFgColor(int nRow, int nCol)
{
	CString sFgColor = GetData(nRow, COL_COLOR);
	if (sFgColor == "#")
		return RGB(215, 0, 0);
	return CfxIGrid::GetCellFgColor(nRow, nCol);
}

BOOL CContGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	return CfxIGrid::OnMouseWheel(nFlags, zDelta, pt);
}

void CContGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	CfxIGrid::OnLButtonDown(nFlags, point);
}

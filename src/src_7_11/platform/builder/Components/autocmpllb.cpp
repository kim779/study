// autocmpllb.cpp : implementation file
//

#include "stdafx.h"
#include "..\axiswork.h"
#include "autocmpllb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoCmplLB

CAutoCmplLB::CAutoCmplLB()
{
}

CAutoCmplLB::~CAutoCmplLB()
{
}


BEGIN_MESSAGE_MAP(CAutoCmplLB, CListBox)
	//{{AFX_MSG_MAP(CAutoCmplLB)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoCmplLB message handlers

void CAutoCmplLB::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int cy = 0;
	AddString("aa");
	for (int ii = 0; ii < GetCount(); ii++)
	{
		cy += GetItemHeight(ii);
		if (cy - GetItemHeight(ii) <= point.y && cy > point.y)
		{
			SetCurSel(ii + GetTopIndex());
		}
	}
	//CListBox::OnLButtonDown(nFlags, point);
}

int CAutoCmplLB::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	return MA_NOACTIVATE;
	
	return CListBox::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

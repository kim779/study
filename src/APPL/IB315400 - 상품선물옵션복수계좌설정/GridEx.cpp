// GridEx.cpp: implementation of the CGridEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IB315400.h"
#include "GridEx.h"
#include "MapWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridEx::CGridEx(CMapWnd* pMapWnd)
	: m_pMapWnd(pMapWnd)
{

}

CGridEx::~CGridEx()
{

}

void CGridEx::OnSetup()
{
	SetSH_Width(0);
	SetHighlightRow(TRUE);
	SetCurrentCellMode(UG_CELLMODE_HIGHLIGHT);
	SetDefRowHeight(SZ_ROWHEIGHT);
	SetDefFont(AddFont("±¼¸²Ã¼", 12, FW_NORMAL));
}

void CGridEx::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);
/*	
	rect->bottom --;
	rect->right --;
	dc->DrawFocusRect(rect);
*/
}


void CGridEx::OnRowChange(long oldrow, long newrow)
{
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);

	m_pMapWnd->OnRowChange(oldrow, newrow, this);
}

void CGridEx::OnDClicked(int col, long row, RECT *rect, POINT *point, BOOL processed)
{
	UNREFERENCED_PARAMETER(row);
	m_pMapWnd->OnDClicked(col, row, rect, point, this);
}
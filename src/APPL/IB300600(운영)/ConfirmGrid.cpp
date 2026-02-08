// ConfirmGrid.cpp: implementation of the CConfirmGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ib300600.h"
#include "ConfirmGrid.h"
#include "ConfirmDlg.h"
#include "MapWnd.h"
#include "Define.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConfirmGrid::CConfirmGrid(CMapWnd* pMapWnd)
	: m_pMapWnd(pMapWnd)
{

}

CConfirmGrid::~CConfirmGrid()
{

}

void CConfirmGrid::OnSetup()
{
	if (!m_pMapWnd)
		return;

	char* arrHeader[MAX_CONFIRMGRIDCOL] = { "계좌번호", "계좌명", "종목명", "구분", "유형", "주문수량", "주문가격" };
	int arrColWidth[MAX_CONFIRMGRIDCOL] = { 80, 80, 120, 40, 60, 60, 60 };

	m_nFontIDNormal = AddFont("굴림체", 12, FW_NORMAL);
	m_nFontIDBold = AddFont("굴림체", 12, FW_BOLD);

	SetNumberCols(MAX_CONFIRMGRIDCOL);
	SetNumberRows(DEF_CONFIRMROW);
	SetSH_Width(0);
	SetHighlightRow(TRUE);
	SetCurrentCellMode(UG_CELLMODE_HIGHLIGHT);
	SetDefRowHeight(SZ_ROWHEIGHT);
	SetDefFont(m_nFontIDNormal);

	CUGCell cell;
	m_pLinePen = new CPen(PS_SOLID, 1, m_pMapWnd->GetIndexColor(CLR_GRID_LINE));

	for (int i = 0; i < MAX_CONFIRMGRIDCOL; i++)
	{
		SetColWidth(i, arrColWidth[i]);

		GetCell(i, -1, &cell);
		cell.SetBackColor(m_pMapWnd->GetIndexColor(CLR_GRID_HBACK));
		cell.SetTextColor(m_pMapWnd->GetIndexColor(CLR_GRID_HTEXT));
		cell.SetText(arrHeader[i]);

		if (i == 0)
			cell.SetBorder(UG_BDR_LTHIN | UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
		else
			cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);

		cell.SetBorderColor(m_pLinePen);
		cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
		SetCell(i, -1, &cell);

		GetColDefault(i, &cell);
		
		cell.SetHBackColor(m_pMapWnd->GetIndexColor(CLR_GRID_SBACK));
		cell.SetHTextColor(m_pMapWnd->GetIndexColor(CLR_GRID_STEXT));
		cell.SetBackColor(m_pMapWnd->GetIndexColor(CLR_GRID_BACK));
		cell.SetTextColor(m_pMapWnd->GetIndexColor(CLR_GRID_TEXT));
		cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);

		cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);

		cell.SetBorderColor(m_pLinePen);
		SetColDefault(i, &cell);
	}
}

void CConfirmGrid::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);
/*	
	rect->bottom --;
	rect->right --;
	dc->DrawFocusRect(rect);
*/
}

int CConfirmGrid::OnCanSizeCol(int col){
	UNREFERENCED_PARAMETER(col);
	return FALSE;
}
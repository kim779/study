// ConfirmGrid.cpp: implementation of the CConfirmGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ib100600.h"
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

	const CString arrHeader[MAX_CONFIRMGRIDCOL] = { "계좌번호", "계좌명", "종목명", "구분", "유형", "주문수량", "주문가격" };
	const int arrColWidth[MAX_CONFIRMGRIDCOL] = { 80, 80, 120, 40, 60, 60, 60 };

	m_nFontIDNormal = AddFont("굴림체", 12, FW_NORMAL);
	m_nFontIDBold = AddFont("굴림체", 12, FW_BOLD);

	SetNumberCols(MAX_CONFIRMGRIDCOL);
	SetNumberRows(DEF_CONFIRMROW);
	SetSH_Width(0);
	SetHighlightRow(TRUE);
	//SetCurrentCellMode(UG_CELLMODE_HIGHLIGHT);
	SetDefRowHeight(SZ_ROWHEIGHT);
	SetDefFont(m_nFontIDNormal);

	CUGCell cell;
	m_pLinePen = std::make_unique<CPen>(PS_SOLID, 1, m_pMapWnd->GetIndexColor(CLR_GRID_LINE));

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

		cell.SetBorderColor(m_pLinePen.get());
		cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
		SetCell(i, -1, &cell);

		GetColDefault(i, &cell);
		
		//cell.SetHBackColor(m_pMapWnd->GetIndexColor(CLR_GRID_HBACK));
		//cell.SetHTextColor(m_pMapWnd->GetIndexColor(CLR_GRID_HTEXT));
		//cell.SetBackColor(m_pMapWnd->GetIndexColor(CLR_GRID_BACK));
		//cell.SetTextColor(m_pMapWnd->GetIndexColor(CLR_GRID_TEXT));

		if (i == 2)
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		else if (i == 5 || i == 6)
			cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
		else
			cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);

		cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);

		cell.SetBorderColor(m_pLinePen.get());
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

void CConfirmGrid::SetPallete()
{
	const int nCount = GetNumberRows();
	CUGCell cell;

	if(!m_pMapWnd) return;

	const COLORREF colorbg[] = {m_pMapWnd->GetIndexColor(CLR_GRID_BACK), m_pMapWnd->GetIndexColor(CLR_GRID_BACK2)};
	int ibgrow=0;
	for (int i = -1; i < nCount; i++)
	{
		for (int j = 0; j < MAX_CONFIRMGRIDCOL; j++)
		{
			GetCell(j, i, &cell);

// 			if (i == -1)
// 			{
// 				cell.SetBackColor(m_pMapWnd->GetIndexColor(CLR_GRID_HBACK));
// 				cell.SetTextColor(m_pMapWnd->GetIndexColor(CLR_GRID_HTEXT));
// 			}
// 			else
// 			{
// 				cell.SetHBackColor(m_pMapWnd->GetIndexColor(CLR_GRID_SBACK));
// 				cell.SetHTextColor(m_pMapWnd->GetIndexColor(CLR_GRID_STEXT));
// 
// 				cell.SetBackColor(colorbg[ibgrow]);
// 				cell.SetTextColor(m_pMapWnd->GetIndexColor(CLR_GRID_TEXT));
// 			}
			SetCell(j, i, &cell);
		}

		ibgrow++;
		if(ibgrow > 1) ibgrow = 0;

	}
}

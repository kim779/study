// ExcelGrid.cpp: implementation of the CExcelGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IB800100.h"
#include "ExcelGrid.h"
#include "MainWnd.h"
#include "griditem.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define SZ_ROWHEIGHT 18
#define MAX_EXECELGRIDCOL 15

#define CLR_GRID_HBACK 74
#define CLR_GRID_HTEXT 76
#define CLR_GRID_SBACK 78
#define CLR_GRID_STEXT 69
#define CLR_GRID_BACK  68
#define CLR_GRID_LINE  75


#define	ARR_UP		"↑"
#define	ARR_DN		"↓"
#define	TRI_UP		"▲"
#define	TRI_DN		"▼"

const COLORREF upCOLOR    = PALETTERGB(255,   0,   0);
const COLORREF dnCOLOR    = PALETTERGB(  0,   0, 255);
const COLORREF eqCOLOR    = PALETTERGB(  0,   0,   0);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExcelGrid::CExcelGrid()
{

}

CExcelGrid::~CExcelGrid()
{

}

void CExcelGrid::OnSetup()
{
	CMainWnd* pMapWnd = (CMainWnd*) GetParent();
	SetNumberCols(15);
	SetNumberRows(45);
	QuickSetText(0,0,"Text");
	
	SetDefRowHeight(SZ_ROWHEIGHT);
	SetDefFont(pMapWnd->m_pFont);
	SetSH_Width(0);
	
	SetHighlightRow(TRUE);
	SetCurrentCellMode(UG_CELLMODE_HIGHLIGHT);
	pMapWnd->m_pLinePen = new CPen(PS_SOLID, 1, pMapWnd->GetIndexColor(CLR_GRID_LINE));
	//int irowcnt = GetNumberRows();
	//for (int i=0; i<irowcnt;i++)
	//  SetRowHeight(i,SZ_ROWHEIGHT);
	
	char* arrHeader[MAX_EXECELGRIDCOL] = { "종목명", "현재가", "전일대비", "등락률", "매도호가", "매수호가", "전일거래량", "누적거래량", "일중가격변동", "일중거래량변동", "상승종목수", "상승거래량수", "고가", "저가", "전일종가"};
	int arrColWidth[MAX_EXECELGRIDCOL] = { 100, 80, 80, 60, 80, 120,120, 80, 120, 100, 100, 80,80, 80};
	CUGCell cell;
	//m_pLinePen = new CPen(PS_SOLID, 1, GetIndexColor(CLR_GRID_LINE));
	SetHighlightRow(TRUE);
	for (int i = 0; i < MAX_EXECELGRIDCOL; i++)
	{
		SetColWidth(i, arrColWidth[i]);
		
		GetCell(i, -1, &cell);
		cell.SetBackColor(pMapWnd->GetIndexColor(CLR_GRID_HBACK));
		cell.SetTextColor(pMapWnd->GetIndexColor(CLR_GRID_HTEXT));
		cell.SetText(arrHeader[i]);
		
		cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
		cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
		SetCell(i, -1, &cell);
		
		GetColDefault(i, &cell);
		cell.SetHBackColor(pMapWnd->GetIndexColor(CLR_GRID_SBACK));
		cell.SetHTextColor(pMapWnd->GetIndexColor(CLR_GRID_STEXT));
		cell.SetBackColor(pMapWnd->GetIndexColor(CLR_GRID_BACK));
		cell.SetTextColor(pMapWnd->GetIndexColor(CLR_GRID_STEXT));
		
		cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
		cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		cell.SetBorderColor(pMapWnd->m_pLinePen);
		
		
		//cell.attrx = GVAT_COMMA|GVAT_CONTRAST;
		/*
		GVITEM	gvitem;
		gvitem.row   = 0;
		gvitem.col   = 4;
		gvitem.state = 0;
		gvitem.mask  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		gvitem.format= GVFM_HEADER;
		gvitem.fgcol = PALETTERGB(  0,   0,   0);
		gvitem.bkcol = PALETTERGB(205, 205, 255);
		
		//CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		
		
		gvitem.attr  = GVAT_COMMA|GVAT_CONTRAST;;
		
		SetItem(&gvitem);
		*/

		SetColDefault(i, &cell);
		
	}
}

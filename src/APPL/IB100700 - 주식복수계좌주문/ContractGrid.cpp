// ContractGrid.cpp: implementation of the CContractGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ContractGrid.h"
#include "MapWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CContractGrid::OnSetup()
{
	CMapWnd* pMapWnd = (CMapWnd*)GetParent()->GetParent();

	const CString arrHeader[MAX_CONTRACTGRIDCOL] = { "차수", "미체결량", "유형", "펀드원주문번호" };
	const int arrColWidth[MAX_CONTRACTGRIDCOL] = { 30, 55, 50, 0 };

	m_nFontIDNormal = AddFont("굴림체", 12, FW_NORMAL);
	m_nFontIDBold = AddFont("굴림체", 12, FW_BOLD);

	m_iRowHeight = SZ_ROWHEIGHT;
	SetNumberCols(MAX_CONTRACTGRIDCOL);
	SetNumberRows(DEF_CONTRACTGRIDROW);
	SetSH_Width(0);
	SetHighlightRow(TRUE);
	SetCurrentCellMode(UG_CELLMODE_HIGHLIGHT);
	SetDefRowHeight(SZ_ROWHEIGHT);
	SetDefFont(m_nFontIDNormal);

	CUGCell cell;
	m_pLinePen = std::make_unique<CPen>(PS_SOLID, 1, pMapWnd->GetIndexColor(CLR_GRID_LINE));

	for (int i = 0; i < MAX_CONTRACTGRIDCOL; i++)
	{
		SetColWidth(i, arrColWidth[i]);

		GetCell(i, -1, &cell);
		cell.SetBackColor(pMapWnd->GetIndexColor(CLR_GRID_HBACK));
		cell.SetTextColor(pMapWnd->GetIndexColor(CLR_GRID_HTEXT));
		cell.SetText(arrHeader[i]);

		if (i == 0)
			cell.SetBorder(UG_BDR_LTHIN | UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
		else
			cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);

		cell.SetBorderColor(m_pLinePen.get());

		cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
		SetCell(i, -1, &cell);

		GetColDefault(i, &cell);

		cell.SetHBackColor(pMapWnd->GetIndexColor(CLR_GRID_SBACK));
		cell.SetHTextColor(pMapWnd->GetIndexColor(CLR_GRID_STEXT));
		cell.SetBackColor(pMapWnd->GetIndexColor(CLR_GRID_BACK));
		cell.SetTextColor(pMapWnd->GetIndexColor(CLR_GRID_TEXT));

		cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
		cell.SetBorderColor(m_pLinePen.get());

		if (i == 0 || i == 1)
			cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
		else
			cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);

		SetColDefault(i, &cell);
	}

	for (int i = 0; i < DEF_CONTRACTGRIDROW; i++)

		SetRowHeight(i, SZ_ROWHEIGHT);
}

void CContractGrid::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);
/*	
	rect->bottom --;
	rect->right --;
	dc->DrawFocusRect(rect);
*/
}

int CContractGrid::OnDropList(long ID,int col,long row,long msg,long param)
{
	CUGCell cell, cellCode;
	GetCell(col, row, &cell);
	CString strCode(_T(""));
	const int nCellTypeIndex = cell.GetCellType();
	const int nParam = cell.GetParam();

	if (msg == UGCT_DROPLISTSTART)
	{
		GetCell(col, row, &cellCode);
		strCode = cellCode.GetText();

		if (strCode.IsEmpty())
			return FALSE;
	}                             
	else if (msg == UGCT_DROPLISTSELECT)
	{
		GetCell(1, row, &cellCode);
		strCode = cellCode.GetText();

		if (strCode.IsEmpty())
		{
			CString * pString = (CString*) param;
			pString->Empty();
		}
	}

	return TRUE;
}

int CContractGrid::OnCheckbox(long ID,int col,long row,long msg,long param)
{
	CUGCell cell;
	const BOOL bState = IsChecked(row);
	const int nCount = GetNumberRows();

	if (col == 0 && row == -1)
	{
		for (int i = 0; i < nCount; i++)
		{
			CString strAccount = QuickGetText(1, i);

			if (strAccount.IsEmpty())
				continue;

			GetCell(col, i, &cell);
			cell.SetBool(bState);
			SetCell(col, i, &cell);
		}
	}
	else if (col == 0 && !bState)
		SetCheckState(-1, bState);

	Invalidate();

	return TRUE;
}

BOOL CContractGrid::IsChecked(long nRow)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	return cell.GetBool();
}

void CContractGrid::SetCheckState(int nRow, BOOL bState)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	cell.SetBool(FALSE);
	SetCell(0, nRow, &cell);

	RedrawCell(0, nRow);
}

void CContractGrid::UpdateCheckedRowData(int nCol, CString strVal)
{
	const long nCount = GetNumberRows();

	for (long i = 0; i < nCount; i++)
	{
		if (!IsChecked(i) || !IsAccComplete(i))
			continue;

		QuickSetText(nCol, i, strVal);
	}

	RedrawCol(nCol);
}

BOOL CContractGrid::IsAccComplete(long nRow)
{
	CString strAccount = QuickGetText(1, nRow);
	_TRIMDATA(strAccount);

	return !strAccount.IsEmpty();
}

void CContractGrid::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);

	if (oldrow == newrow)
		return;

	GetParent()->SendMessage(WM_USER + 100);
}

int CContractGrid::OnCanSizeCol(int col){
	UNREFERENCED_PARAMETER(col);
	return FALSE;
}


void CContractGrid::ChangeFont(CFont* pFont, double* lrate)
{
//	m_nFontIDNormal = AddFont(lszfont, ifont, FW_NORMAL);
//	m_nFontIDBold = AddFont(lszfont, ifont, FW_BOLD);
	CRect rc;
	GetWindowRect(&rc);
	SetDefFont(pFont);
	const int iscroll = 19;
	const int irowcnt = GetNumberRows();
	m_iRowHeight = (rc.Height()-iscroll) /  (irowcnt + 1);
	int ihdrH = m_iRowHeight;
	int itmp = 0;
	for(int i=0; i<irowcnt; i++)
	{
		SetRowHeight(i, m_iRowHeight);
		itmp += m_iRowHeight;
	}

	itmp += iscroll; //scroll
	if(itmp < rc.Height() )
		ihdrH = (rc.Height() - itmp);

	SetTH_Height(ihdrH);
	int iw = 0;
	for (int i = 0; i < MAX_SELLGRIDCOL; i++)
	{
		iw = GetColWidth(i);

		iw = iw + (int)(lrate[0] * (float)iw );
		SetColWidth(i, iw);
	}
}

void CContractGrid::SetPallete(CMapWnd* pMapWnd)
{
}
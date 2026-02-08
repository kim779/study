// ModifyGrid.cpp: implementation of the CModifyGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IB100700.h"
#include "ModifyGrid.h"
#include "ModifyWnd.h"
#include "MapWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void CModifyGrid::OnSetup()
{
	CMapWnd* pMapWnd = (CMapWnd*)((CModifyWnd*)GetParent())->GetParent();

	const CString arrHeader[MAX_MODIFYGRIDCOL] = { "", "원주문", "계좌번호", "구분", "주문수량", "주문단가", "미체결", "정정단가", "정정수량", "주문#", "주문상태" };
	const int arrColWidth[MAX_MODIFYGRIDCOL] = { 20, 40, 90, 40, 55, 60, 50, 60, 50, 40, 215};

	m_nFontIDNormal = AddFont("굴림체", 12, FW_NORMAL);
	m_nFontIDBold = AddFont("굴림체", 12, FW_BOLD);

	m_iRowHeight = SZ_ROWHEIGHT;
	SetNumberCols(MAX_MODIFYGRIDCOL);
	SetNumberRows(DEF_MODIFYGRIDROW);
	SetSH_Width(0);
	SetHighlightRow(TRUE);
	SetCurrentCellMode(UG_CELLMODE_HIGHLIGHT);
	SetDefRowHeight(SZ_ROWHEIGHT);
	SetDefFont(m_nFontIDNormal);

	CUGCell cell;
	m_pLinePen = std::make_unique<CPen>(PS_SOLID, 1, pMapWnd->GetIndexColor(CLR_GRID_LINE));

	for (int i = 0; i < MAX_MODIFYGRIDCOL; i++)
	{
		SetColWidth(i, arrColWidth[i]);

		GetCell(i, -1, &cell);
		cell.SetBackColor(pMapWnd->GetIndexColor(CLR_GRID_HBACK));
		cell.SetTextColor(pMapWnd->GetIndexColor(CLR_GRID_HTEXT));
		cell.SetText(arrHeader[i]);

		if (i == 0)
		{
			cell.SetCellType(UGCT_CHECKBOX);
			cell.SetCellTypeEx(UGCT_CHECKBOXCHECKMARK);
			cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
		}
		else
			cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);

		cell.SetBorderColor(m_pLinePen.get());

		if (i == 9)
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		else
			cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
		SetCell(i, -1, &cell);

		GetColDefault(i, &cell);

		cell.SetHBackColor(pMapWnd->GetIndexColor(CLR_GRID_SBACK));
		cell.SetHTextColor(pMapWnd->GetIndexColor(CLR_GRID_STEXT));
		cell.SetBackColor(pMapWnd->GetIndexColor(CLR_GRID_BACK));
		cell.SetTextColor(pMapWnd->GetIndexColor(CLR_GRID_TEXT));

		switch (i)
		{
		case 0:
			cell.SetCellType(UGCT_CHECKBOX);
			cell.SetCellTypeEx(UGCT_CHECKBOXCHECKMARK);
//			cell.SetBorder(UG_BDR_LTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
			break;
		
		case 4:
		case 5:
		case 6:
		case 7:
			cell.SetCellType(UG_EDITID);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
			break;
/*
		case 6:
		case 7:
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			cell.SetLabelText(arrData[i]);
			break;
*/
		default:
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
			break;
		}

		cell.SetBorderColor(m_pLinePen.get());

		if (i == 3)
			cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
		else if (i == 2 || i == 9 || i==10)
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		else
			cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);

		SetColDefault(i, &cell);
	}

	for (int i = 0; i < DEF_MODIFYGRIDROW; i++)
		SetRowHeight(i, SZ_ROWHEIGHT);
}

void CModifyGrid::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);
/*	
	rect->bottom --;
	rect->right --;
	dc->DrawFocusRect(rect);
*/
}

int CModifyGrid::OnDropList(long ID,int col,long row,long msg,long param)
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

int CModifyGrid::OnCheckbox(long ID,int col,long row,long msg,long param)
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

BOOL CModifyGrid::IsChecked(long nRow)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	return cell.GetBool();
}

void CModifyGrid::SetCheckState(int nRow, BOOL bState)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	cell.SetBool(bState);
	SetCell(0, nRow, &cell);

	RedrawCell(0, nRow);
}

void CModifyGrid::UpdateCheckedRowData(int nCol, CString strVal)
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

BOOL CModifyGrid::IsAccComplete(long nRow)
{
	CString strAccount = QuickGetText(1, nRow);
	_TRIMDATA(strAccount);

	return !strAccount.IsEmpty();
}

int CModifyGrid::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	if (ID == UGCT_DROPLIST)
		OnDropList(ID, col, row, msg, param);
	else if (ID == UGCT_CHECKBOX)
		OnCheckbox(ID, col, row, msg, param);

	return TRUE;
}

int CModifyGrid::GetSendData(CStringArray *pArr)
{
	CUGCell cell;
	const int nCount = GetNumberRows();
	CString strVal;
	CString strOrder;

	for (int i = 0; i < nCount; i++)
	{
		GetCell(0, i, &cell);
		if (cell.GetBool())
		{
			strVal = QuickGetText(1, i);
			_TRIMDATA(strVal);

			if (strVal.IsEmpty())
				continue;

			for (int j = 1; j < MAX_BUYGRIDCOL - 2; j++)
			{
				if (j == 6)
				{
					strVal = QuickGetText(8, i);
				}
				else
					strVal = QuickGetText(j, i);
				//strVal = QuickGetText(j, i);
				_TRIMDATA(strVal);

				strOrder += strVal;
				strOrder += _T("\t");
			}

			pArr->Add(strOrder);
			strOrder.Empty();
		}
	}

	return 0;
}

int CModifyGrid::OnCanSizeCol(int col){
	UNREFERENCED_PARAMETER(col);
	return FALSE;
}

void CModifyGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if (col != 8)
		return;

	StartEdit();
}


void CModifyGrid::ChangeFont(CFont* pFont, double* lrate)
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

void CModifyGrid::SetPallete(CMapWnd* pMapWnd)
{
	const COLORREF colorbg[] = { pMapWnd->GetIndexColor(CLR_GRID_BACK), pMapWnd->GetIndexColor(CLR_GRID_BACK2)};
	int ibgrow=0;

	if(m_pLinePen) {
		DeleteObject(m_pLinePen.get());

		m_pLinePen = nullptr;
	}
	m_pLinePen = std::make_unique<CPen>(PS_SOLID, 1, pMapWnd->GetIndexColor(CLR_GRID_LINE));
	CUGCell cell;

	for (int i = -1; i < GetNumberRows(); i++)
	{
		for (int j = 0; j < MAX_MODIFYGRIDCOL; j++)
		{
			GetCell(j, i, &cell);

			if (i == -1)
			{
				cell.SetBackColor(pMapWnd->GetIndexColor(CLR_GRID_HBACK));
				cell.SetTextColor(pMapWnd->GetIndexColor(CLR_GRID_HTEXT));
			}
			else
			{
				cell.SetHBackColor(pMapWnd->GetIndexColor(CLR_GRID_SBACK));
				cell.SetHTextColor(pMapWnd->GetIndexColor(CLR_GRID_STEXT));

				cell.SetBackColor(colorbg[ibgrow]);
				cell.SetTextColor(pMapWnd->GetIndexColor(CLR_GRID_TEXT));
			}

			cell.SetBorderColor(m_pLinePen.get());
			SetCell(j, i, &cell);
		}

		ibgrow++;
		if(ibgrow > 1) ibgrow = 0;

	}
}
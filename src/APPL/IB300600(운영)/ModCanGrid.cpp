// ModCanGrid.cpp: implementation of the CModCanGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IB300600.h"
#include "ModCanGrid.h"
#include "ModCanWnd.h"
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

CModCanGrid::CModCanGrid()
	: m_pLinePen(NULL)
	, m_nSpinButtonIndex(-1)
	, m_bInit(FALSE)
{

}

CModCanGrid::~CModCanGrid()
{

}

void CModCanGrid::OnSetup()
{
	CMapWnd* pMapWnd = (CMapWnd*)((CModCanWnd*)GetParent())->GetMapWnd();

	char* arrHeader[MAX_MODCANGRIDCOL] = { "", "종목코드", "원주문#", "종목명", "주문구분", "정정유형", "정정수량", "정정가격", "현재가", "주문번호", "주문상태" };
	int arrColWidth[MAX_MODCANGRIDCOL] = { 20, 0, 64, 120, 58, 58, 60, 60, 60, 54, 280 };

	m_nFontIDNormal = AddFont("굴림체", 12, FW_NORMAL);
	m_nFontIDBold = AddFont("굴림체", 12, FW_BOLD);

	SetNumberCols(MAX_MODCANGRIDCOL);
	SetNumberRows(DEF_MODCANGRIDROW);
	SetSH_Width(0);
	SetHighlightRow(TRUE);
	SetCurrentCellMode(UG_CELLMODE_HIGHLIGHT);
	SetDefRowHeight(SZ_ROWHEIGHT);
	SetDefFont(m_nFontIDNormal);

	CUGCell cell;
	m_pLinePen = new CPen(PS_SOLID, 1, pMapWnd->GetIndexColor(CLR_GRID_LINE));

	m_nSpinButtonIndex = AddCellType(&m_SpinButton);
	m_nEllipsisIndex = AddCellType(&m_Ellipsis);

	for (int i = 0; i < MAX_MODCANGRIDCOL; i++)
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
			cell.SetBorder(UG_BDR_LTHIN | UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
		}
		else
			cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);

		cell.SetBorderColor(m_pLinePen);
		
		if (i == 10)
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		else
			cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);

		SetCell(i, -1, &cell);

		GetColDefault(i, &cell);
		
		cell.SetHBackColor(pMapWnd->GetIndexColor(CLR_GRID_SBACK));
		cell.SetHTextColor(pMapWnd->GetIndexColor(CLR_GRID_STEXT));
		cell.SetBackColor(pMapWnd->GetIndexColor(CLR_GRID_BACK));
		cell.SetTextColor(pMapWnd->GetIndexColor(CLR_GRID_TEXT));
		cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);

		switch (i)
		{
		case 0:
			cell.SetCellType(UGCT_CHECKBOX);
			cell.SetCellTypeEx(UGCT_CHECKBOXCHECKMARK);
			cell.SetBorder(UG_BDR_LTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
			break;

		case 2:
		case 3:
		case 4:
		case 9:
		case 10:
			cell.SetReadOnly(TRUE);
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			break;
		
		case 8:
			cell.SetCellType(UG_EDITID);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
			cell.SetReadOnly(TRUE);
			break;

		case 5:
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON);
			cell.SetParam(DROPLIST_MODCANTYPE);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			cell.SetLabelText(_T("지정가\n시장가\n조건부\n최유리\nIOC\nFOK\n"));
			break;

		case 6:
		case 7:
			cell.SetCellType(m_nSpinButtonIndex);
			cell.SetCellTypeEx(UGCT_SPINBUTTONHIDEBUTTON);
			cell.SetParam(CELLTYPE_IS_EDITABLE);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			break;

		default:
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			break;
		}

		cell.SetBorderColor(m_pLinePen);
		SetColDefault(i, &cell);
	}

	m_bInit = TRUE;
	UseHints(TRUE);
}

void CModCanGrid::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);
/*	
	rect->bottom --;
	rect->right --;
	dc->DrawFocusRect(rect);
*/
}

int CModCanGrid::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	if (ID == UGCT_CHECKBOX)
		OnCheckbox(ID, col, row, msg, param);
	else if (ID == m_nSpinButtonIndex)
		OnSpinButton(ID, col, row, msg, param);
	else if (ID == UGCT_DROPLIST)
		OnDropList(ID, col, row, msg, param);

	return TRUE;
}

int CModCanGrid::OnCheckbox(long ID,int col,long row,long msg,long param)
{
	CUGCell cell;
	BOOL bState = IsChecked(row);
	int nCount = GetNumberRows();

	if (col == 0 && row == -1)
	{
		for (int i = 0; i < nCount; i++)
		{
			CString strCode = QuickGetText(1, i);

			if (strCode.IsEmpty())
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

int CModCanGrid::OnSpinButton(long ID,int col,long row,long msg,long param)
{
	CUGCell cell, cellCode;
	GetCell(col, row, &cell);
	int nCellTypeIndex = cell.GetCellType();
	int nParam = cell.GetParam();
	CString strText, strCode;

	GetCell(1, row, &cellCode);
	strCode = cellCode.GetText();
	
	if (strCode.IsEmpty())
		return FALSE;

	if (col == 6)
	{
		strText = cell.GetText();
		if (strText.IsEmpty())
			strText = _T("0");

		long num = atoi((LPCTSTR)strText);

		if (msg == UGCT_SPINBUTTONUP)
		{
			if (num < 99999)
				num++;
		}
		else if (msg == UGCT_SPINBUTTONDOWN)
		{
			if (num > 0)
				num--;
		}

		CString strData;
		strData.Format("%ld",num);
		cell.SetText(strData);
		SetCell(col, row, &cell);
	}
	else if (col == 7)
	{
		strText = cell.GetText();
		if (strText.IsEmpty())
			strText = _T("0");

		double fData = atof((LPCTSTR)strText);

		if(msg == UGCT_SPINBUTTONUP)
		{
			fData += 0.05;
			if (fData > 1000.00)
				fData = 0;
		}
		else if(msg == UGCT_SPINBUTTONDOWN)
		{
			fData -= 0.05;
			if (fData < 0)
				fData = 1000.00;
		}

		CString strData;
		strData.Format("%0.2f",fData);
		cell.SetText(strData);
		SetCell(col, row, &cell);
	}

	return TRUE;
}

void CModCanGrid::OnCharDown(UINT *vcKey,BOOL processed)
{
	int nCol = GetCurrentCol();
	if (nCol == 6 || nCol == 7)
		StartEdit(*vcKey);
}

void CModCanGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
//	if (col == 6 || col == 7)
//		return;

	CUGCell cell;
	GetCell(col, row, &cell);
	int nCellTypeIndex = cell.GetCellType();
	int nParam = cell.GetParam();
	CString string;

	if(cell.GetLabelText() != NULL)
		string = cell.GetLabelText();

	if (col != 1 && col != 9 && col != 10)
		StartEdit();

//	if(nParam == CELLTYPE_IS_EDITABLE || string == "CELLTYPE_IS_EDITABLE")
//	{
//		StartEdit();
//	}
}

void CModCanGrid::SetModCanState(CString strVal)
{
	UpdateCheckedRowData(4, strVal);
}

void CModCanGrid::SetModCanPrice(CString strVal)
{
	UpdateCheckedRowData(7, strVal);
}

void CModCanGrid::SetQuantity(CString strVal)
{
	UpdateCheckedRowData(6, strVal);
}

void CModCanGrid::SetModCanType(CString strVal)
{
	UpdateCheckedRowData(5, strVal);
}

BOOL CModCanGrid::IsChecked(long nRow)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	return cell.GetBool();
}

BOOL CModCanGrid::IsCodeComplete(long nRow)
{
	CString strCode = QuickGetText(1, nRow);

	return !strCode.IsEmpty();
}

void CModCanGrid::UpdateCheckedRowData(int nCol, CString strVal)
{
	long nCount = GetNumberRows();

	for (long i = 0; i < nCount; i++)
	{
		if (!IsChecked(i) || !IsCodeComplete(i))
			continue;

		QuickSetText(nCol, i, strVal);
	}

	RedrawCol(nCol);
}

void CModCanGrid::SetCheckState(int nRow, BOOL bState)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	cell.SetBool(FALSE);
	SetCell(0, nRow, &cell);

	RedrawCell(0, nRow);
}

void CModCanGrid::OnSetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);

	CMapWnd* pMapWnd = (CMapWnd*)((CModCanWnd*)GetParent())->GetMapWnd();
	CString strText(_T(""));

	if (row == -1)
		return;

	switch (col)
	{
	case 4:
		strText = CString(cell->GetText());

		if (strText == _T("매수"))
			cell->SetTextColor(pMapWnd->GetIndexColor(CLR_DATA_UP));
		else if (strText == _T("매도"))
			cell->SetTextColor(pMapWnd->GetIndexColor(CLR_DATA_DOWN));
		break;

	case 8:
		strText = CString(cell->GetText());

		if (!strText.IsEmpty())
		{
			if (strText.GetAt(0) == '-')
			{
				cell->SetTextColor(pMapWnd->GetIndexColor(CLR_DATA_DOWN));
				strText.Delete(0);
				cell->SetText(strText);
			}
			else if (strText.GetAt(0) == '+')
			{
				cell->SetTextColor(pMapWnd->GetIndexColor(CLR_DATA_UP));
				strText.Delete(0);
				cell->SetText(strText);
			}
			else
				cell->SetTextColor(pMapWnd->GetIndexColor(CLR_GRID_STEXT));
		}

		break;
	}
}

void CModCanGrid::SetTriggerData(CString strCode, CString strCurPrice)
{
	int nCurRow = GetCurrentRow();
	CString strCurCode = QuickGetText(1, nCurRow);

	if (strCode != strCurCode)
		return;

	QuickSetText(7, nCurRow, strCurPrice);
	RedrawCell(7, nCurRow);
}

void CModCanGrid::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);

	if (oldrow == newrow || !m_bInit)
		return;

	CString strCode = QuickGetText(1, newrow);
	CModCanWnd* pModCanWnd = (CModCanWnd*)GetParent();

	if (pModCanWnd->IsBlocking())
		return;

	CMapWnd* pMapWnd = (CMapWnd*)pModCanWnd->GetMapWnd();
	
	CString strProc;
	strProc.Format("change_code\t%s", strCode);
	pMapWnd->Procedure(strProc);
}

void CModCanGrid::GetCheckCode(CStringArray *pArr)
{
	CUGCell cell;
	int nCount = GetNumberRows();

	for (int i = 0; i < nCount; i++)
	{
		GetCell(0, i, &cell);
		if (cell.GetBool())
		{
			CString strVal = QuickGetText(4, i);

			if (strVal == _T("매도"))
				strVal = _T("1");
			else
				strVal = _T("2");

			strVal += QuickGetText(1, i);

			pArr->Add(strVal);
		}
	}
}

void CModCanGrid::SetFOCData(CStringArray *pArr, BOOL bHoga)
{
	CUGCell cell;
	int nCount = GetNumberRows();
	int nCurIndex = 0;
	CModCanWnd* pWnd = (CModCanWnd*)GetParent();
	CString strData(_T(""));
	CString strCurCode(_T(""));
	CString strCode(_T(""));
	CString strCurPrice(_T(""));
	CString strPrice(_T(""));

	if (bHoga)
	{
		for (int i = 0; i < nCount; i++)
		{
			GetCell(0, i, &cell);
			if (cell.GetBool())
			{
				strData = pArr->GetAt(nCurIndex++);
				strCode = pWnd->Parser(strData, _T("\t"));
				strCurPrice = pWnd->Parser(strData, _T("\t"));
				strPrice = pWnd->Parser(strData, _T("\t"));

				strCurCode = QuickGetText(1, i);

				if (strCode != strCurCode)
					continue;

				QuickSetText(7, i, (LPCTSTR)strPrice);
				QuickSetText(8, i, (LPCTSTR)strCurPrice);
			}
		}

		RedrawCol(7);
	}
	else
	{
		strData = pArr->GetAt(nCurIndex++);
		strCode = pWnd->Parser(strData, _T("\t"));
		strCurPrice = pWnd->Parser(strData, _T("\t"));
		strPrice = pWnd->Parser(strData, _T("\t"));

		for (int i = 0; i < nCount; i++)
		{
			strCurCode = QuickGetText(1, i);
			
			if (strCode == strCurCode) {
				QuickSetText(8, i, (LPCTSTR)strCurPrice);
			}
		}
	}

	RedrawCol(8);
}


void CModCanGrid::GetSendData(CStringArray *pArr)
{
	CUGCell cell;
	int nCount = GetNumberRows();
	CString strVal;
	CString strModCan;

	for (int i = 0; i < nCount; i++)
	{
		GetCell(0, i, &cell);
		if (cell.GetBool())
		{
			for (int j = 1; j < MAX_MODCANGRIDCOL - 3; j++)
			{
				strVal = QuickGetText(j, i);
				_TRIMDATA(strVal);

				strModCan += strVal;
				strModCan += _T("\t");
			}

			pArr->Add(strModCan);
			strModCan.Empty();
		}
	}
}

void CModCanGrid::SetModCanResult(CStringArray *pArr)
{
	int nCount = GetNumberRows();
	int nCurIndex = 0;
	CString strModCanNo(_T(""));
	CString strVal(_T(""));
	CUGCell cell;

	for (int i = 0; i < nCount; i++)
	{
		GetCell(0, i, &cell);
		if (cell.GetBool())
		{
			strVal = pArr->GetAt(nCurIndex++);
			if (strVal.GetLength() != 92)
				continue;

			strModCanNo = strVal.Left(6);
			strModCanNo.TrimLeft(_T("0"));
			QuickSetText(9, i, (LPCTSTR)strModCanNo);
		}		
	}
}

int CModCanGrid::OnCanSizeCol(int col){
	UNREFERENCED_PARAMETER(col);
	return FALSE;
}

int CModCanGrid::OnDropList(long ID,int col,long row,long msg,long param)
{
	CUGCell cell, cellCode;
	GetCell(col, row, &cell);
	CString strCode(_T(""));
	int nCellTypeIndex = cell.GetCellType();
	int nParam = cell.GetParam();

	if (msg == UGCT_DROPLISTSTART)
	{
		GetCell(1, row, &cellCode);
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

void CModCanGrid::SetOrderResult(CStringArray *pArr)
{
	int nCount = GetNumberRows();
	int nCurIndex = 0;
	CString strOrderNo(_T(""));
	CString strOrgOrderNo(_T(""));
	CString strVal(_T(""));
	CString strMsg(_T(""));
	CUGCell cell;

	for (int i = 0; i < nCount; i++)
	{
		GetCell(0, i, &cell);
		if (cell.GetBool())
		{
			strVal = pArr->GetAt(nCurIndex++);
			if (strVal.GetLength() != 92)
				continue;

			strOrderNo = strVal.Left(6);
			strOrderNo.TrimLeft(_T("0"));
			QuickSetText(9, i, (LPCTSTR)strOrderNo);
			RedrawCell(9, i);

			strVal.Delete(0, 6);
			strOrgOrderNo = strVal.Left(6);
			strOrgOrderNo.TrimLeft(_T("0"));
			QuickSetText(2, i, (LPCTSTR)strOrgOrderNo);
			RedrawCell(2, i);

			strVal.Delete(0, 6);

			strMsg = strVal.Right(strVal.GetLength() - 5);
			QuickSetText(10, i, (LPCTSTR)strMsg);
			RedrawCell(10, i);
		}		
	}
}

void CModCanGrid::SetPallete()
{
	int nCount = GetNumberRows();
	CUGCell cell;
	CMapWnd* pMapWnd = (CMapWnd*)((CModCanWnd*)GetParent())->GetMapWnd();
	
	for (int i = -1; i < nCount; i++)
	{
		for (int j = 0; j < MAX_MODCANGRIDCOL; j++)
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
				cell.SetBackColor(pMapWnd->GetIndexColor(CLR_GRID_BACK));
				cell.SetTextColor(pMapWnd->GetIndexColor(CLR_GRID_TEXT));
			}
			SetCell(j, i, &cell);
		}
	}
}

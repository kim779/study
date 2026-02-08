// OrderGrid.cpp: implementation of the COrderGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IB300600.h"
#include "OrderGrid.h"
#include "OrderWnd.h"
#include "MapWnd.h"
#include "Define.h"
#include "../../h/MistakeStopOrder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COrderGrid::COrderGrid()
	: m_pLinePen(NULL)
	, m_nSpinButtonIndex(-1)
	, m_bInit(FALSE)
{

}

COrderGrid::~COrderGrid()
{
	_SAFEDESTROY(m_pLinePen);
}

void COrderGrid::OnSetup()
{
	CMapWnd* pMapWnd = (CMapWnd*)((COrderWnd*)GetParent())->GetMapWnd();

	char* arrHeader[MAX_ORDERGRIDCOL] = { "", "종목코드", "", "종목명", "주문구분", "주문유형", "주문수량", "주문가", "현재가", "주문번호", "주문상태" };
	int arrColWidth[MAX_ORDERGRIDCOL] = { 20, 75, 20, 91, 58, 58, 60, 60, 60, 54, 280 };

	m_nFontIDNormal = AddFont("굴림체", 12, FW_NORMAL);
	m_nFontIDBold = AddFont("굴림체", 12, FW_BOLD);

	SetNumberCols(MAX_ORDERGRIDCOL);
	SetNumberRows(DEF_ORDERGRIDROW);
	SetSH_Width(0);
	SetHighlightRow(TRUE);
	SetCurrentCellMode(UG_CELLMODE_HIGHLIGHT);
	SetDefRowHeight(SZ_ROWHEIGHT);
	SetDefFont(m_nFontIDNormal);

	CUGCell cell;
	m_pLinePen = new CPen(PS_SOLID, 1, pMapWnd->GetIndexColor(CLR_GRID_LINE));

	m_nSpinButtonIndex = AddCellType(&m_SpinButton);
	m_nEllipsisIndex = AddCellType(&m_Ellipsis);

	for (int i = 0; i < MAX_ORDERGRIDCOL; i++)
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

		switch (i)
		{
		case 0:
			cell.SetCellType(UGCT_CHECKBOX);
			cell.SetCellTypeEx(UGCT_CHECKBOXCHECKMARK);
			cell.SetBorder(UG_BDR_LTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
			break;

		case 1:
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON);
			cell.SetBorder(UG_BDR_BTHIN);
			break;
			
		case 2:
			cell.SetCellType(m_nEllipsisIndex);
			cell.SetCellTypeEx(UGCT_ELLIPSISHIDEBUTTON);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			break;
		
		case 3:
		case 8:
			cell.SetCellType(UG_EDITID);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			cell.SetReadOnly(TRUE);
			break;

		case 4:
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON);
			cell.SetParam(DROPLIST_ORDERSTATE);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			cell.SetLabelText(_T("매도\n매수\n"));
			break;

		case 5:
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON);
			cell.SetParam(DROPLIST_ORDERTYPE);
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
			cell.SetCellType(UG_EDITID);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			break;
		}

		if (i == 10)
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		else if (i == 6 || i == 7 || i == 8 || i == 9)
			cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
		else
			cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);

		cell.SetBorderColor(m_pLinePen);
		SetColDefault(i, &cell);
	}

	JoinCells(1, -1, 2, -1);
	UseHints(TRUE);
	m_bInit = TRUE;
}

void COrderGrid::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);
/*	
	rect->bottom --;
	rect->right --;
	dc->DrawFocusRect(rect);
*/
}

int COrderGrid::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	if (ID == UGCT_DROPLIST)
		OnDropList(ID, col, row, msg, param);
	else if (ID == UGCT_CHECKBOX)
		OnCheckbox(ID, col, row, msg, param);
	else if (ID == m_nSpinButtonIndex)
		OnSpinButton(ID, col, row, msg, param);
	else if (ID == m_nEllipsisIndex)
		OnEllipsisButton( ID, col, row, msg, param);

	return TRUE;
}

int COrderGrid::OnDropList(long ID,int col,long row,long msg,long param)
{
	CUGCell cell, cellCode;
	GetCell(col, row, &cell);
	CString strCode(_T(""));
	int nCellTypeIndex = cell.GetCellType();
	int nParam = cell.GetParam();

	if (msg == UGCT_DROPLISTSTART)
	{
		switch (col)
		{
		case 1:
			InitHistoryCode((CStringList*)param);
			break;
		default:
			GetCell(1, row, &cellCode);
			strCode = cellCode.GetText();

			if (strCode.IsEmpty())
				return FALSE;

			break;
		}
	}
	else if (msg == UGCT_DROPLISTSELECT)
	{
		if (col == 1)
		{
			CString * pString = (CString*) param;
			CString strCodeName(_T(""));

			if (pString->GetLength() > 8)
			{
				CString strOldCode = QuickGetText(1, row);

				strCodeName = pString->Right(pString->GetLength() - 8);
				*pString = pString->Left(8);

				if (strOldCode != *pString)
				{
					strCodeName.TrimLeft();
					strCodeName.TrimRight();
					QuickSetText(3, row, (LPCTSTR)strCodeName);
					
					CodeComplete(row, TRUE);

					if (row == GetNumberRows() - 1)
						SetNumberRows(GetNumberRows() + 1);

					SetCheckState(row, FALSE);
					SetCheckState(-1, FALSE);

					CMapWnd* pMapWnd = (CMapWnd*)((COrderWnd*)GetParent())->GetMapWnd();
					COrderWnd* pWnd = (COrderWnd*)GetParent();
					
					CString strProc;
					strProc.Format("change_code\t%s", *pString);
					pMapWnd->Procedure(strProc);
					pWnd->QueryFOCHoga(FALSE, *pString);
				}
			}
		}
		else
		{
			GetCell(1, row, &cellCode);
			strCode = cellCode.GetText();

			if (strCode.IsEmpty())
			{
				CString * pString = (CString*) param;
				pString->Empty();
			}
		}
	}

	return TRUE;
}

int COrderGrid::OnEllipsisButton(long ID,int col,long row,long msg,long param)
{
	CUGCell cell;
	GetCell(col, row, &cell);
	int nCellTypeIndex = cell.GetCellType();
	int nParam = cell.GetParam();

	if (msg == UGCT_ELLIPSISBUTTONCLICK)
	{
		CRect rect;
		GetCellRect(col, row, &rect);
		ClientToScreen(&rect);
		COrderWnd* pWnd = (COrderWnd*)GetParent();

		ReleaseCapture();
		CString strCodeData = (char*) pWnd->GetWizard()->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(foptionCODE, NULL)), MAKELPARAM(rect.left, rect.bottom + 22));
		
		Invalidate();
		if (!strCodeData.IsEmpty())
		{
			int nPos = strCodeData.Find('\t');
			if (nPos > 0)
			{
				CString strCode = strCodeData.Left(nPos);
				CString strCodeName = strCodeData.Right(strCodeData.GetLength() - nPos);

				if (strCode.GetLength() == 5)
					strCode = strCode + "000";
				
				strCode.TrimLeft();
				strCode.TrimRight();
				strCodeName.TrimLeft();
				strCodeName.TrimRight();

				CString strOldCode = QuickGetText(col - 1, row);
				
				if (strCode != strOldCode)
				{
					QuickSetText(col - 1, row, (LPCTSTR)strCode);
					QuickSetText(col + 1, row, (LPCTSTR)strCodeName);
					CodeComplete(row, TRUE);

					pWnd->SetHistory(strCode);
					
					if (row == GetNumberRows() - 1)
						SetNumberRows(GetNumberRows() + 1);
					
					SetCheckState(row, FALSE);
					SetCheckState(-1, FALSE);

					CMapWnd* pMapWnd = (CMapWnd*)((COrderWnd*)GetParent())->GetMapWnd();
					
					CString strProc;
					strProc.Format("change_code\t%s", strCode);
					pMapWnd->Procedure(strProc);
					pWnd->QueryFOCHoga(FALSE, strCode);
				}
			}
		}
	}

	return TRUE;
}

int COrderGrid::OnCheckbox(long ID,int col,long row,long msg,long param)
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

int COrderGrid::OnSpinButton(long ID,int col,long row,long msg,long param)
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

void COrderGrid::OnCharDown(UINT *vcKey,BOOL processed)
{
	int nCol = GetCurrentCol();
	if (nCol == 6 || nCol == 7)
		StartEdit(*vcKey);
}

void COrderGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
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

	if (col != 1 && col != 4 && col != 9 && col != 10)
		StartEdit();

//	if(nParam == CELLTYPE_IS_EDITABLE || string == "CELLTYPE_IS_EDITABLE")
//	{
//		StartEdit();
//	}
}

void COrderGrid::InitHistoryCode(CStringList *pList)
{
	COrderWnd* pWnd = (COrderWnd*)GetParent();
	CString strHistory = pWnd->GetHistory(_T("ed_focod"));

	int nPos = 0;

	while (TRUE)
	{
		if (strHistory.IsEmpty())
			break;

		nPos = strHistory.Find(_T("\t"));
		
		if (nPos != -1)
			pList->AddTail(strHistory.Left(nPos));
		else
		{
			pList->AddTail(strHistory);
			break;
		}

		strHistory.Delete(0, nPos + 1);
	}
}

void COrderGrid::CodeComplete(int nRow, BOOL bSet)
{
	COrderWnd* pWnd = (COrderWnd*)GetParent();

	if (bSet)
	{
		QuickSetText(4, nRow, (LPCTSTR)pWnd->GetOrderState());
		QuickSetText(5, nRow, (LPCTSTR)pWnd->GetOrderType());
		QuickSetText(6, nRow, (LPCTSTR)pWnd->GetQuantity());
//		QuickSetText(7, nRow, (LPCTSTR)pWnd->GetOrderPrice());
	}
	else
	{
		QuickSetText(1, nRow, _T(""));
		QuickSetText(3, nRow, _T(""));
		QuickSetText(4, nRow, _T(""));
		QuickSetText(5, nRow, _T(""));
		QuickSetText(6, nRow, _T(""));
		QuickSetText(7, nRow, _T(""));
		QuickSetText(8, nRow, _T(""));
		QuickSetText(9, nRow, _T(""));
		QuickSetText(10, nRow, _T(""));
	}
}

int COrderGrid::OnEditFinish(int col, long row,CWnd* edit,LPCTSTR string,BOOL cancelFlag)
{
	CUGCell cell;
	GetCell(GetCurrentCol(),GetCurrentRow(),&cell);
	int nCellTypeIndex = cell.GetCellType();
	COrderWnd* pWnd = (COrderWnd*)GetParent();
	CString strCode = string;

	if(nCellTypeIndex == UGCT_DROPLIST || col == 1)
	{
		CString strCurCode = QuickGetText(col, row);
		if (strCode == strCurCode)
			return TRUE;

		CString strCodeName = (char*) pWnd->GetWizard()->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)(LPSTR)(LPCTSTR)strCode);

		strCodeName.TrimLeft();
		strCodeName.TrimRight();

		if (strCodeName.IsEmpty())
		{
			cell.SetText(_T(""));
			QuickSetText(col, row, _T(""));
		}
		else
		{
			QuickSetText(3, row, (LPCTSTR)strCodeName);
			CodeComplete(row, TRUE);
		}
	}

	return TRUE;
}

void COrderGrid::SetOrderState(CString strVal)
{
	UpdateCheckedRowData(4, strVal);
}

void COrderGrid::SetOrderPrice(CString strVal)
{
	UpdateCheckedRowData(7, strVal);
}

void COrderGrid::SetQuantity(CString strVal)
{
	UpdateCheckedRowData(6, strVal);
}

void COrderGrid::SetOrderType(CString strVal)
{
	UpdateCheckedRowData(5, strVal);
}

BOOL COrderGrid::IsChecked(long nRow)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	return cell.GetBool();
}

BOOL COrderGrid::IsCodeComplete(long nRow)
{
	CString strCode = QuickGetText(1, nRow);

	return !strCode.IsEmpty();
}

void COrderGrid::UpdateCheckedRowData(int nCol, CString strVal)
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

void COrderGrid::SetCheckState(int nRow, BOOL bState)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	cell.SetBool(FALSE);
	SetCell(0, nRow, &cell);

	RedrawCell(0, nRow);
}

void COrderGrid::OnSetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);

	CMapWnd* pMapWnd = (CMapWnd*)((COrderWnd*)GetParent())->GetMapWnd();
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

void COrderGrid::SetTriggerData(CString strCode, CString strCurPrice)
{
	int nCurRow = GetCurrentRow();
	CString strCurCode = QuickGetText(1, nCurRow);

	if (strCode != strCurCode)
		return;

	QuickSetText(7, nCurRow, strCurPrice);
	RedrawCell(7, nCurRow);
}

void COrderGrid::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);

	if (oldrow == newrow || !m_bInit)
		return;

	CString strCode = QuickGetText(1, newrow);
	COrderWnd* pOrderWnd = (COrderWnd*)GetParent();

	if (pOrderWnd->IsBlocking())
		return;

	CMapWnd* pMapWnd = (CMapWnd*)pOrderWnd->GetMapWnd();
	
	CString strProc;
	strProc.Format("change_code\t%s", strCode);
	pMapWnd->Procedure(strProc);
}

void COrderGrid::GetCheckCode(CStringArray *pArr)
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

void COrderGrid::SetFOCData(CStringArray *pArr, BOOL bHoga)
{
	CUGCell cell;
	int nCount = GetNumberRows();
	int nCurIndex = 0;
	COrderWnd* pWnd = (COrderWnd*)GetParent();
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

				if (pWnd->GetOrderPriceType() == _T("지정가"))
					QuickSetText(7, i, (LPCTSTR)pWnd->GetOrderPrice());
				else
					QuickSetText(7, i, (LPCTSTR)strPrice);

				QuickSetText(8, i, (LPCTSTR)strCurPrice);
			}
		}
	}
	else
	{
		strData = pArr->GetAt(nCurIndex++);
		strCode = pWnd->Parser(strData, _T("\t"));
		strCurPrice = pWnd->Parser(strData, _T("\t"));
		strPrice = pWnd->Parser(strData, _T("\t"));

		strCurCode = QuickGetText(1, GetCurrentRow());

		if (strCode == strCurCode)
		{
			QuickSetText(7, GetCurrentRow(), (LPCTSTR)strPrice);
			QuickSetText(8, GetCurrentRow(), (LPCTSTR)strCurPrice);
		}
	}

	RedrawCol(7);
	RedrawCol(8);
}


void COrderGrid::GetSendData(CStringArray *pArr)
{
	CUGCell cell;
	int nCount = GetNumberRows();
	CString strVal;
	CString strOrder;

	CString strData(_T(""));
	CString strCode(_T(""));		// 종목코드
	CString strName(_T(""));		// 종목코드
	CString strOrderState(_T(""));	// 매매유형
	CString strOrderType(_T(""));	// 호가구분
	CString strQuantity(_T(""));	// 수량
	CString strOrderPrice(_T(""));	// 주문단가
	CString strCount(_T(""));;
	CString strUser(_T(""));
	CString strPassword(_T(""));
	CString strEncPass(_T(""));
	CString strSendData(_T(""));
	CString strAccount(_T(""));

	for (int i = 0; i < nCount; i++)
	{
		GetCell(0, i, &cell);
		if (cell.GetBool())
		{
			strVal = QuickGetText(1, i);
			_TRIMDATA(strVal);

			if (strVal.IsEmpty())
				continue;

			for (int j = 1; j < MAX_ORDERGRIDCOL - 3; j++)
			{
				if (j == 2)
					continue;

				strVal = QuickGetText(j, i);
				_TRIMDATA(strVal);

				strOrder += strVal;
				strOrder += _T("\t");
			}
			strData = strOrder;
			strCode = Parser(strData, _T("\t"));
			strName = Parser(strData, _T("\t"));
			Parser(strData, _T("\t"));
			Parser(strData, _T("\t"));
			strQuantity = Parser(strData, _T("\t"));
			strOrderPrice = Parser(strData, _T("\t"));
			strOrderPrice.Remove(_T(','));
			strQuantity.Remove(_T(','));
			//CString tmp;
			//tmp.Format("%s | %s | %s | %s", strCode, strName, strQuantity, strOrderPrice);
			//MessageBox(tmp);
			BOOL bRet = multiCheckupMistakeOrder(this, atol((LPCTSTR)strOrderPrice), atol((LPCTSTR)strQuantity), strCode, "0", strName);
			if (!bRet)
			{
				SetCheckState(i, FALSE);
				strOrder.Empty();
				continue;
			}
			pArr->Add(strOrder);
			strOrder.Empty();
		}
	}
}

void COrderGrid::SetOrderResult(CStringArray *pArr)
{
	int nCount = GetNumberRows();
	int nCurIndex = 0;
	CString strOrderNo(_T(""));
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

			strVal.Delete(0, 12);
			strMsg = strVal.Right(strVal.GetLength() - 5);
			QuickSetText(10, i, (LPCTSTR)strMsg);
			RedrawCell(10, i);
		}		
	}
}

void COrderGrid::SetPallete()
{
	int nCount = GetNumberRows();
	CUGCell cell;
	CMapWnd* pMapWnd = (CMapWnd*)((COrderWnd*)GetParent())->GetMapWnd();
	
	for (int i = -1; i < nCount; i++)
	{
		for (int j = 0; j < MAX_ORDERGRIDCOL; j++)
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


int COrderGrid::OnCanSizeCol(int col){
	UNREFERENCED_PARAMETER(col);
	return FALSE;
}

CString COrderGrid::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

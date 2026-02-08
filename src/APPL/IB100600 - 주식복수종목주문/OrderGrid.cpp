// OrderGrid.cpp: implementation of the COrderGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IB100600.h"
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
{

}

COrderGrid::~COrderGrid()
{
	//_SAFEDESTROY(m_pLinePen);	
}

void COrderGrid::OnSetup()
{
	CMapWnd* pMapWnd = (CMapWnd*)((COrderWnd*)GetParent())->GetParent();

	const CString arrHeader[MAX_ORDERGRIDCOL] = { "", "종목코드", "", "종목명", "매매구분", "신용구분", "대출일", "유형", "조건", "주문수량", "주문단가", "주문번호", "주문상태", "주문단가", "기준가" };	//2015.06.10 KSJ 기준가 추가
	const int arrColWidth[MAX_ORDERGRIDCOL] = { 20, 60, 20, 75, 60, 0, 0, 95, 50, 60, 65, 65, 348, 0, 0 };
	const CString arrData[MAX_ORDERGRIDCOL] = { "", "", "", "",	"매도\n매수\n", "현금\n신용\n", "",
										"지정가\n시장가\n조건부지정가\n최유리지정가\n최우선지정가\n장개시전 시간외종가\n시간외종가\n",
										"없음\nFOK\nIOC\n", "", "", "", "" };

	m_nFontIDNormal = AddFont("굴림체", 12, FW_NORMAL);
	m_nFontIDBold = AddFont("굴림체", 12, FW_BOLD);

	m_iRowHeight = SZ_ROWHEIGHT;
	SetNumberCols(MAX_ORDERGRIDCOL);
	SetNumberRows(DEF_ORDERGRIDROW);
	SetSH_Width(0);
	SetHighlightRow(TRUE);
	SetCurrentCellMode(UG_CELLMODE_HIGHLIGHT);
	SetDefRowHeight(m_iRowHeight);

	SetDefFont(m_nFontIDNormal);

	CUGCell cell;
	m_pLinePen = std::make_unique<CPen>(PS_SOLID, 1, pMapWnd->GetIndexColor(CLR_GRID_LINE));

	m_nEllipsisIndex = AddCellType((CUGCellType *)&m_Ellipsis);

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

		cell.SetBorderColor(m_pLinePen.get());
		if (i == 12)
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
			cell.SetMask("######");
			break;

		case 2:
			cell.SetCellType(m_nEllipsisIndex);
			cell.SetCellTypeEx(UGCT_ELLIPSISHIDEBUTTON);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			break;
		
		case 4:
		case 5:
		case 7:
		case 8:
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			cell.SetLabelText(arrData[i]);
			break;

		default:
			cell.SetCellType(UG_EDITID);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
			break;
		}

		cell.SetBorderColor(m_pLinePen.get());

		if (i == 3 || i == 12)
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		else if (i == 9 || i == 10)
			cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
		else
			cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);

		SetColDefault(i, &cell);
	}

	JoinCells(1, -1, 2, -1);

	for (int i = 0; i < DEF_ORDERGRIDROW; i++)
	{
		SetRowHeight(i, m_iRowHeight);
	}

	m_bInit = TRUE;
}

void COrderGrid::SetPallete()
{
	if (m_pLinePen) {
		DeleteObject(m_pLinePen.get());
		m_pLinePen = nullptr;
	}

	const int nCount = GetNumberRows();
	CUGCell cell;
	CMapWnd* pMapWnd = ((COrderWnd*)GetParent())->GetMapWnd();
	m_pLinePen = std::make_unique<CPen>(PS_SOLID, 1, pMapWnd->GetIndexColor(CLR_GRID_LINE));

	const COLORREF colorbg[] = {pMapWnd->GetIndexColor(CLR_GRID_BACK), pMapWnd->GetIndexColor(CLR_GRID_BACK2)};
	int ibgrow=0;
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

void COrderGrid::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);
/*	
	rect->bottom --;
	rect->right --;
	dc->DrawFocusRect(rect);
*/
}


int COrderGrid::OnDropList(long ID,int col,long row,long msg,long param)
{
	CUGCell cell, cellCode;
	GetCell(col, row, &cell);
	CString strCode(_T(""));
	const int nCellTypeIndex = cell.GetCellType();
	const int nParam = cell.GetParam();

	if (msg == UGCT_DROPLISTSTART)
	{
		if (col == 1)
			InitHistoryCode((CStringList*)param);

		GetCell(col, row, &cellCode);
		strCode = cellCode.GetText();

		if (strCode.IsEmpty())
			return FALSE;
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
				pString->TrimLeft();
				pString->TrimRight();

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

					CMapWnd* pMapWnd = (CMapWnd*)((COrderWnd*)GetParent())->GetParent();
					COrderWnd* pWnd = (COrderWnd*)GetParent();
					
					CString strProc;
					strProc.Format("change_code\t%s", *pString);
					pMapWnd->Procedure(strProc);
					pWnd->QuerySCHoga(FALSE, *pString);
				}
			}
		}
		else
		{
			GetCell(1, row, &cellCode);
			strCode = cellCode.GetText();

			CString * pString = (CString*) param;

			if (strCode.IsEmpty())
				pString->Empty();
			else
			{
				if (col == 7)
				{
					if (*pString != _T("지정가") && *pString != _T("조건부지정가"))
						QuickSetText(10, row, _T("0"));
					else
					{
						((COrderWnd*)GetParent())->QuerySCHoga(TRUE, QuickGetText(1, row));
					}
				}
			}
		}
	}

	return TRUE;
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

	CMapWnd* pMapWnd = (CMapWnd*)pOrderWnd->GetParent();
	
	CString strProc;
	strProc.Format("change_code\t%s", strCode);
	pMapWnd->Procedure(strProc);
}

int COrderGrid::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	if (ID == UGCT_DROPLIST)
		OnDropList(ID, col, row, msg, param);
	else if (ID == UGCT_CHECKBOX)
		OnCheckbox(ID, col, row, msg, param);
	else if (ID == m_nEllipsisIndex)
		OnEllipsisButton( ID, col, row, msg, param);

	return TRUE;
}
#define DF_CNTMAX 9
int COrderGrid::OnCheckbox(long ID,int col,long row,long msg,long param)
{
	CUGCell cell;
	const BOOL bState = IsChecked(row);
	const int nCount = GetNumberRows();
	int ichkcnt = 0;

	CString slog;
	slog.Format("IB100600 col = %d row=%d bState=%d", col, row, bState);
	OutputDebugString(slog);

	ichkcnt = 0;
	if(col == 0 && row == -1 && bState)
	{
		for (int i = 0; i < nCount; i++)
		{
			CString strCode = QuickGetText(1, i);
			
			if (strCode.IsEmpty())
				continue;
			
			GetCell(col, i, &cell);
			if(cell.GetBool() == TRUE)
				ichkcnt++;	
		}
	}

	if (col == 0 && row == -1)
	{
slog.Format("IB100600 ichkcnt = %d ", ichkcnt);
OutputDebugString(slog);

		for (int i = 0; i < nCount; i++)
		{
			CString strCode = QuickGetText(1, i);

			if (strCode.IsEmpty())
				continue;

			GetCell(col, i, &cell);
			if(cell.GetBool() == FALSE)
			{
				ichkcnt++;	
				if(ichkcnt == DF_CNTMAX + 1 && bState)
				{
					slog.Format(_T("일괄주문은 최대 %d건 까지만 가능합니다."), DF_CNTMAX );
					AfxMessageBox(slog);
					Invalidate();
					return TRUE;
				}
			}
			cell.SetBool(bState);
			SetCell(col, i, &cell);
slog.Format("IB100600 [ ii = %d  ichkcnt=%d bState=%d", i, ichkcnt, bState);
OutputDebugString(slog);
		}
	}
	else if (col == 0 && !bState)
	{
		SetCheckState(-1, bState);
	}
	else if (col == 0 && bState)
	{
		ichkcnt = 0;
		for(int ii = 0 ; ii < nCount; ii++)
		{
			CString strCode = QuickGetText(1, ii);
			
			if (strCode.IsEmpty())
				continue;
			
			GetCell(0, ii, &cell);
			if(cell.GetBool() == TRUE)
				ichkcnt++;
			
			slog.Format("IB100600 ii = %d row=%d ichkcnt=%d ", ii, row, ichkcnt);
			OutputDebugString(slog);
			
			if(ichkcnt == DF_CNTMAX + 1)
			{
				GetCell(0, row, &cell);
				cell.SetBool(FALSE);
				SetCell(col, row, &cell);
				SetCheckState(row, FALSE);
				slog.Format(_T("일괄주문은 최대 %d건 까지만 가능합니다.."), DF_CNTMAX);
				AfxMessageBox(slog);
				return TRUE;
			}
		}
	}

	Invalidate();

	return TRUE;
}

int COrderGrid::OnEllipsisButton(long ID,int col,long row,long msg,long param)
{
	CUGCell cell;
	GetCell(col, row, &cell);
	const int nCellTypeIndex = cell.GetCellType();
	const int nParam = cell.GetParam();

	if (msg == UGCT_ELLIPSISBUTTONCLICK)
	{
		CRect rect;
		GetCellRect(col, row, &rect);
		ClientToScreen(&rect);
		COrderWnd* pWnd = (COrderWnd*)GetParent();

		ReleaseCapture();
		CString strCodeData = (char*) pWnd->GetWizard()->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(allCODE, NULL)), MAKELPARAM(rect.left, rect.bottom + 22));
		
		Invalidate();
		if (!strCodeData.IsEmpty())
		{
			const int nPos = strCodeData.Find('\t');
			if (nPos > 0)
			{
				CString strCode = strCodeData.Left(nPos);
				CString strCodeName = strCodeData.Right(strCodeData.GetLength() - nPos);

//				if (strCode.GetLength() == 5)
//					strCode = strCode + "000";
				
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

					CMapWnd* pMapWnd = (CMapWnd*)((COrderWnd*)GetParent())->GetParent();
					
					CString strProc;
					strProc.Format("change_code\t%s", strCode);
					pMapWnd->Procedure(strProc);
					pWnd->QuerySCHoga(FALSE, strCode);
				}
			}
		}
	}

	return TRUE;
}

int COrderGrid::OnEditStart(int col, long row,CWnd **edit)
{
	return TRUE;
}

int COrderGrid::OnEditFinish(int col, long row,CWnd* edit,LPCTSTR string,BOOL cancelFlag)
{
	CUGCell cell;
	GetCell(GetCurrentCol(),GetCurrentRow(),&cell);
	const int nCellTypeIndex = cell.GetCellType();
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

			if (row == GetNumberRows() - 1)
				SetNumberRows(GetNumberRows() + 1);
			
			SetCheckState(row, FALSE);
			SetCheckState(-1, FALSE);
			
			CMapWnd* pMapWnd = (CMapWnd*)((COrderWnd*)GetParent())->GetParent();
			COrderWnd* pWnd = (COrderWnd*)GetParent();
			
			CString strProc;
			strProc.Format("change_code\t%s", string);
			pMapWnd->Procedure(strProc);
			pWnd->QuerySCHoga(FALSE, string);
		}
	}

	return TRUE;
}

void COrderGrid::InitHistoryCode(CStringList *pList)
{
	COrderWnd* pWnd = (COrderWnd*)GetParent();
	CString strHistory = pWnd->GetHistory(_T("1301"));

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
		QuickSetText(5, nRow, (LPCTSTR)pWnd->GetCreditType());
		QuickSetText(7, nRow, (LPCTSTR)pWnd->GetOrderType());
		QuickSetText(8, nRow, (LPCTSTR)pWnd->GetCondition());
		QuickSetText(9, nRow, (LPCTSTR)pWnd->GetQuantity());
		QuickSetText(10, nRow, (LPCTSTR)pWnd->GetOrderPrice());
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

void COrderGrid::SetCheckState(int nRow, BOOL bState)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	cell.SetBool(FALSE);
	SetCell(0, nRow, &cell);

	RedrawCell(0, nRow);
}

BOOL COrderGrid::IsChecked(long nRow)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	return cell.GetBool();
}

void COrderGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if (col != 1 && col != 9 && col != 10)
		return;

	if (col == 9 || col == 10)
	{
		CString strText;
		CUGCell cell;
		GetCell(col, row, &cell);
		
		strText = cell.GetText();
		strText.Remove(_T(','));
		strText.Remove(_T('.'));
		
		SetCell(col, row, &cell);
	}

	StartEdit();

//	if(nParam == CELLTYPE_IS_EDITABLE || string == "CELLTYPE_IS_EDITABLE")
//	{
//		StartEdit();
//	}
}


void COrderGrid::OnSetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);

	CString strText(_T(""));

	if (row == -1)
		return;

	CMapWnd* pMapWnd = (CMapWnd*)((COrderWnd*)GetParent())->GetParent();

	switch (col)
	{
	case 4:
		strText = CString(cell->GetText());

		if (strText == _T("매수"))
			cell->SetTextColor(pMapWnd->GetIndexColor(CLR_DATA_UP));
		else if (strText == _T("매도"))
			cell->SetTextColor(pMapWnd->GetIndexColor(CLR_DATA_DOWN));
		break;

	case 6:
		break;
	case 9:
	case 10:
		strText = CString(cell->GetText());
		if (!strText.IsEmpty())
		{
			if (strText.GetAt(0) == _T('+') || strText.GetAt(0) == _T('-'))
				strText.Delete(0);

			FormatThousand(strText);
			cell->SetText(strText);
		}
		break;
	}
}

void COrderGrid::FormatThousand(CString& strVal)
{
	int nThound = 0;
	
	strVal.Remove(_T(','));

	for (int i = 0; i < strVal.GetLength(); i++)
	{
		if ((i - nThound) % 3 == 0 && i != (3 * nThound) + nThound)
		{
			strVal.Insert(strVal.GetLength() - i, _T(","));
			nThound++;
		}
	}
}

int COrderGrid::GetSendData(CStringArray *pArr)
{
	CUGCell cell;
	const int nCount = GetNumberRows();
	CString strVal;
	CString strOrder;
	COrderWnd* pWnd = (COrderWnd*)GetParent();

	for (int i = 0; i < nCount; i++)
	{
		GetCell(0, i, &cell);
		if (cell.GetBool())
		{
			strVal = QuickGetText(1, i);
			_TRIMDATA(strVal);

			if (strVal.IsEmpty())
				continue;

			for (int j = 1; j < MAX_ORDERGRIDCOL - 2; j++)
			{
				if (j == 2)
					continue;

				strVal = QuickGetText(j, i);
				_TRIMDATA(strVal);

				if (j == 1)
				{
					const int code_type = pWnd->GetWizard()->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (LPARAM)(LPCSTR)strVal);
					if (code_type==9) strVal = "J" + strVal;	// ELW
					else if (code_type==27) strVal = "Q" + strVal;	// 20104.11.13 KSJ ETN 추가
					else strVal = "A" + strVal;
				}

				if (j == 9)
				{
					if (strVal.IsEmpty())
						return 1;

					if (strVal == _T("0"))
						return 2;
				}

				if (j == 10)
				{
					if (strVal.IsEmpty())
						return 3;

					if (strVal == _T("0") && (QuickGetText(7, i) == _T("지정가") || QuickGetText(7, i) == _T("조건부지정가")))
						return 4;
				}

				strOrder += strVal;
				strOrder += _T("\t");
			}
			//============= 주문착오방지로직 Start ==============
			CString strData(_T(""));
			CString strCode(_T(""));		// 종목코드
			CString strCodeName(_T(""));		// 종목코드명
			CString strQuantity(_T(""));	// 수량
			CString strOrderPrice(_T(""));	// 주문단가
			CString tmp;
			strData = strOrder;
			strCode = Parser(strData, _T("\t"));
			strCodeName = Parser(strData, _T("\t"));
			tmp = Parser(strData, _T("\t"));
			tmp = Parser(strData, _T("\t"));
			Parser(strData, _T("\t"));
			tmp = Parser(strData, _T("\t"));
			tmp = Parser(strData, _T("\t"));
			strQuantity = Parser(strData, _T("\t"));
			strOrderPrice = Parser(strData, _T("\t"));
			CString istrOrderPrice, istrQuantity;
			istrOrderPrice  = strOrderPrice;
			istrQuantity	= strQuantity;
			istrOrderPrice.Remove(_T(','));
			istrQuantity.Remove(_T(','));
			//============= 주문착오방지로직 End ==============
			
			const BOOL bRet = multiCheckupMistakeOrder(this, atoi((LPCTSTR)istrOrderPrice), atoi((LPCTSTR)istrQuantity), strCode, "0", strCodeName);
			if (!bRet)
			{
				//m_pOrderGrid->SetCheckState(nCount, false);
				//cell.SetBool(false);
				SetCheckState(i, FALSE);
				strOrder.Empty();
				continue;
			}
			pArr->Add(strOrder);
			strOrder.Empty();
		}
	}
	Invalidate(true);
	return 0;
}

void COrderGrid::UpdateCheckedRowData(int nCol, CString strVal, BOOL bCalc /* = FALSE */)
{
	const int nCount = GetNumberRows();
	CString strPrice(_T(""));
	CString strDan(_T(""));
	CString strQuantity = strVal;

	for (long i = 0; i < nCount; i++)
	{
		if (!IsChecked(i) || !IsCodeComplete(i))
			continue;

		if (bCalc && nCol == 9)
			strVal = CalcQuantityPrice(i, strQuantity);

		QuickSetText(nCol, i, strVal);
	}

	RedrawCol(nCol);
}

BOOL COrderGrid::IsCodeComplete(long nRow)
{
	CString strCode = QuickGetText(1, nRow);

	return !strCode.IsEmpty();
}

void COrderGrid::GetCheckCode(CStringArray *pArr)
{
	CUGCell cell;
	const int nCount = GetNumberRows();

	for (int i = 0; i < nCount; i++)
	{
		GetCell(0, i, &cell);
		if (cell.GetBool())
		{
			CString strVal = QuickGetText(1, i);
			pArr->Add(strVal);
		}
	}
}

void COrderGrid::SetSCData(CStringArray *pArr, BOOL bHoga, BOOL bStdPrc)	//2015.06.12 KSJ 기준가 구하기 추가
{
	CUGCell cell;
	const int nCount = GetNumberRows();
	int nCurIndex = 0;
	COrderWnd* pWnd = (COrderWnd*)GetParent();
	CString strData(_T(""));
	CString strCurCode(_T(""));
	CString strCode(_T(""));
	CString strPrice(_T(""));
	CString strDan(_T(""));

	if (bHoga)
	{
		for (int i = 0; i < nCount; i++)
		{
			GetCell(0, i, &cell);
			if (cell.GetBool())
			{
				strData = pArr->GetAt(nCurIndex++);
				strCode = pWnd->Parser(strData, _T("\t"));
				strPrice = pWnd->Parser(strData, _T("\t"));
				strDan = pWnd->Parser(strData, _T("\t"));

				strCurCode = QuickGetText(1, i);

				if (strCode != strCurCode)
					continue;

				if(bStdPrc)	//2015.06.12 KSJ 기준가 추가
				{
					QuickSetText(10, i, (LPCTSTR)strPrice);
					QuickSetText(14, i, (LPCTSTR)strDan);
				}
				else
				{
					QuickSetText(10, i, (LPCTSTR)strPrice);
					QuickSetText(13, i, (LPCTSTR)strDan);
				}
			}
		}
	}
	else
	{
		strData = pArr->GetAt(nCurIndex++);
		strCode = pWnd->Parser(strData, _T("\t"));
		strPrice = pWnd->Parser(strData, _T("\t"));
		strDan = pWnd->Parser(strData, _T("\t"));

		strCurCode = QuickGetText(1, GetCurrentRow());
		_TRIMDATA(strCurCode);

		BOOL bMove = FALSE;

		if (strCurCode.IsEmpty())
		{
			strCurCode = QuickGetText(1, GetCurrentRow() - 1);

			if (strCode == strCurCode)
			{
				GotoRow(GetCurrentRow() - 1);
				bMove = TRUE;
			}
		}

		if (strCode == strCurCode)
		{
			
			if(bStdPrc)	//2015.06.12 KSJ 기준가 추가
			{
				QuickSetText(14, GetCurrentRow(), (LPCTSTR)strPrice);
				QuickSetText(13, GetCurrentRow(), (LPCTSTR)strDan);
			}
			else
			{
				QuickSetText(10, GetCurrentRow(), (LPCTSTR)strPrice);
				QuickSetText(13, GetCurrentRow(), (LPCTSTR)strDan);
			}

			if (pWnd->IsQuantityPriceType())
				QuickSetText(9, GetCurrentRow(), CalcQuantityPrice(GetCurrentRow(), QuickGetText(9, GetCurrentRow())));

			if (bMove)
				GotoRow(GetCurrentRow() + 1);
		}
	}

	if(bStdPrc)	//2015.06.12 KSJ 기준가 추가
		RedrawCol(14);
	else
		RedrawCol(10);
}

void COrderGrid::SetOrderResult(CStringArray *pArr)
{
	const int nCount = GetNumberRows();
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
			QuickSetText(11, i, (LPCTSTR)strOrderNo);
			RedrawCell(11, i);

			strVal.Delete(0, 12);
			strMsg = strVal.Right(strVal.GetLength() - 5);
			QuickSetText(12, i, (LPCTSTR)strMsg);
			RedrawCell(12, i);
		}		
	}
}

CString COrderGrid::CalcQuantityPrice(int nRow, CString strVal)
{
	int nPrice = 0;
	int nTotalPrice = 0;
	int nResult = 0;
	int nDan = 0;
	CString strPrice(_T(""));
	CString strDan(_T(""));
	CString strRet(_T(""));

	strPrice = QuickGetText(10, nRow);
	strPrice.Remove(_T(','));
	nPrice = atoi((LPCTSTR)strPrice);
	nPrice = nPrice == 0 ? 1 : nPrice;
	
	strDan = QuickGetText(13, nRow);
	nDan = atoi((LPCTSTR)strDan);
	nDan = nDan == 0 ? 1 : nDan;
	
	strVal.Remove(_T(','));
	nTotalPrice = atoi((LPCTSTR)strVal);
	
	if (nPrice > 0)
	{
		nResult = nTotalPrice / nPrice;
		nResult -= nResult % nDan;
		strRet.Format("%d", nResult);
	}

	return strRet;
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


void COrderGrid::ChangeFont(CFont* pFont, double* lrate)
{
//	m_nFontIDNormal = AddFont(lszfont, ifont, FW_NORMAL);
//	m_nFontIDBold = AddFont(lszfont, ifont, FW_BOLD);
	CRect rc;
	GetWindowRect(&rc);
	SetDefFont(pFont);

	const int iscroll = 19;
	m_iRowHeight = (rc.Height()-iscroll) /  (DEF_ORDERGRIDROW + 1);
	int ihdrH = m_iRowHeight;
	int itmp = 0;
	for(int i=0; i<DEF_ORDERGRIDROW; i++)
	{
		SetRowHeight(i, m_iRowHeight);
		itmp += m_iRowHeight;
	}

	itmp += iscroll; //scroll
	if(itmp < rc.Height() )
		ihdrH = (rc.Height() - itmp);

	SetTH_Height(ihdrH);
	for(int i=0; i<DEF_ORDERGRIDROW; i++)
	{
		
	}

	int iw = 0, itotw=0;
	for (int i = 0; i < MAX_ORDERGRIDCOL; i++)
	{
		iw = GetColWidth(i);
		iw = iw + (int)(lrate[0] * (float)iw );
		itotw += iw;
		SetColWidth(i, iw);
	}

//	SetColWidth(MAX_ORDERGRIDCOL-1, rc.right - itotw);
}
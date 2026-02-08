// ModCanGrid.cpp: implementation of the CModCanGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IB100600.h"
#include "ModCanGrid.h"
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


void CModCanGrid::OnSetup()
{
	CMapWnd* pMapWnd = (CMapWnd*)((COrderWnd*)GetParent())->GetParent();

	const CString arrHeader[MAX_MODCANGRIDCOL] = { "", "종목코드", "원주문", "종목명", "매매구분", "주문수량", "주문단가", "미체결", "정정단가", "주문번호", "주문상태", "기준가" };	//2015.06.10 KSJ 기준가 추가
	const int arrColWidth[MAX_MODCANGRIDCOL] = { 20, 0, 60, 140, 70, 60, 70, 70, 70, 80, 320, 0 };
/*	char* arrData[MAX_MODCANGRIDCOL] = { "", "", "", "",	"매도\n매수\n", "현금\n신용\n신용(유)\n대출\n", "",
										"보통\n시장가\n조건부지정가\n최유리지정가\n최우선지정가\n[장전]시간외\n[장후]시간외\n",
										"없음\nFOK\nIOC\n", "", "", "", "" };
*/
	m_nFontIDNormal = AddFont("굴림체", 12, FW_NORMAL);
	m_nFontIDBold = AddFont("굴림체", 12, FW_BOLD);

	m_iRowHeight = SZ_ROWHEIGHT;
	SetNumberCols(MAX_MODCANGRIDCOL);
	SetNumberRows(DEF_MODCANGRIDROW);
	SetSH_Width(0);
	SetHighlightRow(TRUE);
	SetCurrentCellMode(UG_CELLMODE_HIGHLIGHT);
	SetDefRowHeight(SZ_ROWHEIGHT);
	SetDefFont(m_nFontIDNormal);

	CUGCell cell;
	m_pLinePen = std::make_unique<CPen>(PS_SOLID, 1, pMapWnd->GetIndexColor(CLR_GRID_LINE));


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

		cell.SetBorderColor(m_pLinePen.get());
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

		default:
			cell.SetCellType(UG_EDITID);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
			break;
		}

		cell.SetBorderColor(m_pLinePen.get());

		if (i == 3 || i == 10)
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		else if (i == 5 || i == 6 || i == 7 || i == 8)
			cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
		else
			cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);

		SetColDefault(i, &cell);
	}

	m_bInit = TRUE;

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

void CModCanGrid::SetSCData(CStringArray *pArr, BOOL bHoga, BOOL bStdPrc)	//2015.06.12 KSJ 기준가 구하기 추가
{
	CUGCell cell;
	const int nCount = GetNumberRows();
	int nCurIndex = 0;
	COrderWnd* pWnd = (COrderWnd*)GetParent();
	CString strData(_T(""));
	CString strCurCode(_T(""));
	CString strCode(_T(""));
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
				strPrice = pWnd->Parser(strData, _T("\t"));
				pWnd->Parser(strData, _T("\t"));

				strCurCode = QuickGetText(1, i);

				if (strCode != strCurCode)
					continue;

				if(bStdPrc)
					QuickSetText(11, i, (LPCTSTR)strPrice);	//2015.06.12 KSJ 기준가 세팅
				else
					QuickSetText(8, i, (LPCTSTR)strPrice);
			}
		}
	}
	else
	{
		strData = pArr->GetAt(nCurIndex++);
		strCode = pWnd->Parser(strData, _T("\t"));
		strPrice = pWnd->Parser(strData, _T("\t"));
		pWnd->Parser(strData, _T("\t"));

		for (int i = 0; i < nCount; i++)
		{
			strCurCode = QuickGetText(1, i);
			_TRIMDATA(strCurCode);	
		
			if (strCode == strCurCode)
			{
				if(bStdPrc)
					QuickSetText(11, i, (LPCTSTR)strPrice);	//2015.06.12 KSJ 기준가 세팅
				else
					QuickSetText(8, i, (LPCTSTR)strPrice);
			}
		}
	}
	if(bStdPrc)
		RedrawCol(11);	//2015.06.12 KSJ 기준가 세팅
	else
		RedrawCol(8);
}

void CModCanGrid::GetCheckCode(CStringArray *pArr)
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

void CModCanGrid::FormatThousand(CString& strVal)
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

void CModCanGrid::OnSetCell(int col,long row,CUGCell *cell)
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

	case 5:
	case 6:
	case 7:
	case 8:
		strText = CString(cell->GetText());
		if (!strText.IsEmpty())
		{
			FormatThousand(strText);
			cell->SetText(strText);
		}
		break;
	}
}

void CModCanGrid::ConditionSet(int nCol, CString strVal)
{
	CString strCurVal(_T(""));

	for (int i = 0; i < GetNumberRows(); i++)
	{
		strCurVal = QuickGetText(nCol, i);
		_TRIMDATA(strCurVal);

		if (strCurVal.IsEmpty())
			continue;

		if (strCurVal != strVal)
		{
			DeleteRow(i);
			i--;
		}
	}
}

void CModCanGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if (col != 8 && col != 7)
		return;

	CString strText;
	CUGCell cell;
	GetCell(col, row, &cell);
	
	strText = cell.GetText();
	strText.Remove(_T(','));
	strText.Remove(_T('.'));
	
	SetCell(col, row, &cell);
	
	StartEdit();

//	if(nParam == CELLTYPE_IS_EDITABLE || string == "CELLTYPE_IS_EDITABLE")
//	{
//		StartEdit();
//	}
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

	CMapWnd* pMapWnd = (CMapWnd*)pModCanWnd->GetParent();
	
	CString strProc;
	strProc.Format("change_code\t%s", strCode);
	pMapWnd->Procedure(strProc);
}

void CModCanGrid::GetSendData(CStringArray *pArr)
{
	CUGCell cell;
	const int nCount = GetNumberRows();
	CString strVal;
	CString strModCan;
	CModCanWnd* pWnd = (CModCanWnd*)GetParent();

	for (int i = 0; i < nCount; i++)
	{
		GetCell(0, i, &cell);
		if (cell.GetBool())
		{
			for (int j = 1; j < MAX_MODCANGRIDCOL - 2; j++)
			{
				strVal = QuickGetText(j, i);
				_TRIMDATA(strVal);

				if (j == 1)
				{
					const int code_type = pWnd->GetWizard()->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (LPARAM)(LPCSTR)strVal);
					if (code_type==9) strVal = "J" + strVal;	// ELW
					else if (code_type==27) strVal = "Q" + strVal;	// 20104.11.13 KSJ ETN 추가
					else strVal = "A" + strVal;
				}

				strModCan += strVal;
				strModCan += _T("\t");
			}

			pArr->Add(strModCan);
			strModCan.Empty();
		}
	}
}

void CModCanGrid::SetOrderResult(CStringArray *pArr)
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
			QuickSetText(9, i, (LPCTSTR)strOrderNo);
			RedrawCell(9, i);

			strVal.Delete(0, 12);
			strMsg = strVal.Right(strVal.GetLength() - 5);
			QuickSetText(10, i, (LPCTSTR)strMsg);
			RedrawCell(10, i);
		}		
	}
}
#define DF_MAXJUNGCNT 9
int CModCanGrid::OnCheckbox(long ID,int col,long row,long msg,long param)
{
	CUGCell cell;
	const BOOL bState = IsChecked(row);
	const int nCount = GetNumberRows();

	CString stmp;
	int ichkcnt = 0;

	if(col == 0 && row == -1 && bState)
	{
		for(int i = 0; i < nCount; i++)
		{
			CString strCode = QuickGetText(1, i);

			if(strCode.IsEmpty())
				continue;

			GetCell(col, i, &cell);
			if(cell.GetBool() == TRUE)
				ichkcnt++;
		}
	}



	if (col == 0 && row == -1)
	{
		for (int i = 0; i < nCount; i++)
		{
			CString strCode = QuickGetText(1, i);

			if (strCode.IsEmpty())
				continue;

			GetCell(col, i, &cell);

			if(cell.GetBool() == FALSE)
			{
				ichkcnt++;
				if(ichkcnt == DF_MAXJUNGCNT + 1 && bState)
				{
					stmp.Format(_T("일괄정정주문은 최대 %d건 까지만 가능합니다."), DF_MAXJUNGCNT);
					AfxMessageBox(stmp);
					Invalidate();
					return TRUE;
				}
			}


			cell.SetBool(bState);
			SetCell(col, i, &cell);
		}
	}
	else if (col == 0 && !bState)
		SetCheckState(-1, bState);
	else if(col == 0 && bState)
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
			
			stmp.Format("IB100600 ii = %d row=%d ichkcnt=%d ", ii, row, ichkcnt);
			OutputDebugString(stmp);
			
			if(ichkcnt == DF_MAXJUNGCNT + 1)
			{
				GetCell(0, row, &cell);
				cell.SetBool(FALSE);
				SetCell(col, row, &cell);
				SetCheckState(row, FALSE);
				stmp.Format(_T("일괄주문은 최대 %d건 까지만 가능합니다.."), DF_MAXJUNGCNT);
				AfxMessageBox(stmp);
				return TRUE;
			}
		}
	}

	Invalidate();

	return TRUE;
}

BOOL CModCanGrid::IsChecked(long nRow)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	return cell.GetBool();
}

void CModCanGrid::SetCheckState(int nRow, BOOL bState)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	cell.SetBool(FALSE);
	SetCell(0, nRow, &cell);

	RedrawCell(0, nRow);
}

int CModCanGrid::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	if (ID == UGCT_CHECKBOX)
		OnCheckbox(ID, col, row, msg, param);

	return TRUE;
}

void CModCanGrid::SetPallete()
{
	const int nCount = GetNumberRows();
	CUGCell cell;
//	CMapWnd* pMapWnd = ((COrderWnd*)GetParent())->GetMapWnd();
	CWnd* pWnd = GetParent();
	CMapWnd* pMapWnd = NULL;
	if(pWnd)
	{
		pMapWnd = (CMapWnd*)pWnd->GetParent();
	}

	if(!pMapWnd) return;

	const COLORREF colorhb = pMapWnd->GetIndexColor(CLR_GRID_HBACK);
	const COLORREF colorht = pMapWnd->GetIndexColor(CLR_GRID_HTEXT);
	const COLORREF colorsb = pMapWnd->GetIndexColor(CLR_GRID_SBACK);
	const COLORREF colorst = pMapWnd->GetIndexColor(CLR_GRID_STEXT);
	const COLORREF colorb = pMapWnd->GetIndexColor(CLR_GRID_BACK);
	const COLORREF colort = pMapWnd->GetIndexColor(CLR_GRID_TEXT);
	const COLORREF colorbg[] = {pMapWnd->GetIndexColor(CLR_GRID_BACK), pMapWnd->GetIndexColor(CLR_GRID_BACK2)};
	int ibgrow=0;

	if (m_pLinePen) {
		DeleteObject(m_pLinePen.get());
		m_pLinePen = nullptr;
	}

	m_pLinePen = std::make_unique<CPen>(PS_SOLID, 1, pMapWnd->GetIndexColor(CLR_GRID_LINE));
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

int CModCanGrid::OnCanSizeCol(int col){
	UNREFERENCED_PARAMETER(col);
	return FALSE;
}

void CModCanGrid::ChangeFont(CFont* pFont, double* lrate)
{
	CRect rc;
	GetWindowRect(&rc);
	SetDefFont(pFont);

	const int iscroll = 0;
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

	int iw = 0;
	for (int i = 0; i < MAX_ORDERGRIDCOL; i++)
	{
		iw = GetColWidth(i);
		iw = iw + (int)(lrate[0] * (float)iw );
		SetColWidth(i, iw);
	}
}
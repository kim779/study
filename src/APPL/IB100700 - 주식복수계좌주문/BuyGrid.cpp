// BuyGrid.cpp: implementation of the CBuyGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IB100700.h"
#include "BuyGrid.h"
#include "BuyWnd.h"
#include "MapWnd.h"
#include "JumunCheck.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void CBuyGrid::OnSetup()
{
	CMapWnd* pMapWnd = (CMapWnd*)((CBuyWnd*)GetParent())->GetParent();

	const CString arrHeader[MAX_BUYGRIDCOL] = { "", "계좌번호", "계좌명", "배수", "가능수량", "주문수량", "주문단가", "주문유형", "조건", "주문번호", "주문상태" };
	const int arrColWidth[MAX_BUYGRIDCOL] = { 20, 90, 60, 40, 60, 60, 60, 95, 45, 60, 400};
	const CString arrData[MAX_BUYGRIDCOL] = { "", "", "", "",	"", "", "", "지정가\n시장가\n조건부지정가\n최유리지정가\n최우선지정가\n장개시전 시간외종가\n시간외종가\n",
										"없음\nFOK\nIOC\n", "", "" };

	m_nFontIDNormal = AddFont("굴림체", 12, FW_NORMAL);
	m_nFontIDBold = AddFont("굴림체", 12, FW_BOLD);

	m_iRowHeight = SZ_ROWHEIGHT;
	SetNumberCols(MAX_BUYGRIDCOL);
	SetNumberRows(DEF_BUYGRIDROW);
	SetSH_Width(0);
	SetHighlightRow(TRUE);
	SetCurrentCellMode(UG_CELLMODE_HIGHLIGHT);
	SetDefRowHeight(SZ_ROWHEIGHT);
	SetDefFont(m_nFontIDNormal);

	CUGCell cell;
	m_pLinePen = std::make_unique<CPen>(PS_SOLID, 1, pMapWnd->GetIndexColor(CLR_GRID_LINE));
	int i = 0;

	for (i = 0; i < MAX_BUYGRIDCOL; i++)
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
		
		case 5:
		case 6:
			cell.SetCellType(UG_EDITID);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
			break;

		case 7:
		case 8:
			cell.SetCellType(UGCT_DROPLIST);
			cell.SetCellTypeEx(UGCT_DROPLISTHIDEBUTTON);
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			cell.SetLabelText(arrData[i]);
			break;

		default:
			cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
			break;
		}

		cell.SetBorderColor(m_pLinePen.get());

		if (i==4 || i == 5 || i == 6)
			cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
		else
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);

		SetColDefault(i, &cell);
	}

	for (i = 0; i < DEF_BUYGRIDROW; i++)
		SetRowHeight(i, SZ_ROWHEIGHT);
}

void CBuyGrid::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);
/*	
	rect->bottom --;
	rect->right --;
	dc->DrawFocusRect(rect);
*/
}

int CBuyGrid::OnDropList(long ID,int col,long row,long msg,long param)
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

int CBuyGrid::OnCheckbox(long ID,int col,long row,long msg,long param)
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

BOOL CBuyGrid::IsChecked(long nRow)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	return cell.GetBool();
}

void CBuyGrid::SetCheckState(int nRow, BOOL bState)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	cell.SetBool(bState);
	SetCell(0, nRow, &cell);

	RedrawCell(0, nRow);
}

void CBuyGrid::UpdateCheckedRowData(int nCol, CString strVal)
{
	const long nCount = GetNumberRows();
	CUGCell cell;

	m_bApplyRate = TRUE;
	for (long i = 0; i < nCount; i++)
	{
		if (!IsChecked(i) || !IsAccComplete(i))
			continue;

		GetCell(nCol, i, &cell);
		cell.SetText(strVal);
		SetCell(nCol, i, &cell);
	}
	m_bApplyRate = FALSE;

	RedrawCol(nCol);
}

BOOL CBuyGrid::IsAccComplete(long nRow)
{
	CString strAccount = QuickGetText(1, nRow);
	_TRIMDATA(strAccount);

	return !strAccount.IsEmpty();
}

int CBuyGrid::GetSendData(CStringArray *pArr)
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
				if (j == 2 || j == 4)
					continue;

				strVal = QuickGetText(j, i);
				_TRIMDATA(strVal);

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
			
			Parser(strData, _T("\t"));
			Parser(strData, _T("\t"));
			strQuantity = Parser(strData, _T("\t"));
			strOrderPrice = Parser(strData, _T("\t"));
			CString istrOrderPrice, istrQuantity;
			istrOrderPrice  = strOrderPrice;
			istrQuantity	= strQuantity;
			istrOrderPrice.Remove(_T(','));
			istrQuantity.Remove(_T(','));
			strCode = "000000";
			//AfxMessageBox(strCode +")"+ istrQuantity+"[]"+istrOrderPrice);
			//============= 주문착오방지로직 End ==============
			if (fListedQty=="") fListedQty = "0";
			//AfxMessageBox(fListedQty);
			
			const BOOL bRet = CheckupMistakeOrder(this, atoi((LPCTSTR)istrOrderPrice), atoi((LPCTSTR)istrQuantity), strCode, fListedQty);
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

	return 0;
}

void CBuyGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if (col != 5 && col != 6)
		return;

	StartEdit();
}

void CBuyGrid::OnSetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);

	CString strText(_T(""));

	if (row == -1)
		return;

	CMapWnd* pMapWnd = (CMapWnd*)((CBuyWnd*)GetParent())->GetParent();

	switch (col)
	{
	case 5:
		{
			CString strRate = QuickGetText(col - 2, row);
			if (strRate.IsEmpty())
				return;

			float fRate = (float)1.0;
			if (m_bApplyRate)
				fRate = (float)atof((LPCTSTR)strRate);

			CString strVal(cell->GetText());
			const int nQty = atoi(strVal);

			strVal.Format("%d", (int)(nQty * fRate));

			cell->SetText((LPCTSTR)strVal);
		}
	case 6:
		strText = CString(cell->GetText());

		if (!strText.IsEmpty())
		{
			if (strText.GetAt(0) == _T('+') || strText.GetAt(0) == _T('-'))
				strText.Delete(0);

			if (atoi((LPCTSTR)strText) == 0)
				strText = _T("0");
			else
				pMapWnd->FormatThousand(strText);
			
			cell->SetText(strText);
		}
		break;
	}
}


int CBuyGrid::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	if (ID == UGCT_DROPLIST)
		OnDropList(ID, col, row, msg, param);
	else if (ID == UGCT_CHECKBOX)
		OnCheckbox(ID, col, row, msg, param);

	return TRUE;
}

int CBuyGrid::OnCanSizeCol(int col){
	UNREFERENCED_PARAMETER(col);
	return FALSE;
}

CString CBuyGrid::Parser(CString &srcstr, CString substr)
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

BOOL CBuyGrid::CheckupMistakeOrder(CWnd *pWnd, double strPrice, int strQuantity, CString strCode, CString strListedQuantity)
{
	ASSERT(pWnd != NULL);
	//ASSERT(!strPrice.IsEmpty());
	//ASSERT(!strQuantity.IsEmpty());
	ASSERT(!strCode.IsEmpty());

	CString strMsg(_T(""));
	CString strMsg2(_T(""));
	
	const long double lPrice = strPrice;//atol((LPCTSTR)strPrice);
	long double lQty = strQuantity;//atol((LPCTSTR)strQuantity);
	const ULONG lListedQty = atol((LPCTSTR)strListedQuantity) * 1000;
	long double lAmount = 0;
	const int smode = ALLOW_ORDER;


	

	UINT nType = MB_YESNO | MB_ICONINFORMATION | MB_DEFBUTTON2;
	//AfxMessageBox(strCode);
	switch (strCode.GetLength())
	{
	case 6:		// 주식, ELW
		{
			lAmount = lPrice * lQty;
			/*
			CString tmp;
			tmp.Format("%f = %f * %f",lAmount, lQty, lPrice);
			AfxMessageBox(tmp);	
			*/	
			if (lAmount > 2000000000 && lAmount <= 5000000000)
				strMsg.Format("%s\n", _T("주문금액이 20억을 초과하는 이상주문입니다."));
			else if (lAmount > 5000000000)
			{
				strMsg.Format("%s\n", _T("주문금액이 50억을 초과하는 이상주문입니다."));
				nType = MB_OK | MB_ICONINFORMATION;
			}
			//else 
			{
				if (strCode.Left(1) == _T("5") || strCode.Left(1) == _T("6") || 
					strCode.Left(1) == _T("7") || strCode.Left(1) == _T("8"))
					return TRUE;
				if (lListedQty!=0)
				{
					lQty = lQty * 100;
					
					if (lQty > lListedQty && lQty / 3 <= lListedQty)
						strMsg2.Format("%s\n", _T("주문수량이 상장주식수 대비 1%를 초과하는 이상주문입니다."));
					else if (lQty / 3 > lListedQty)
					{
						strMsg2.Format("%s\n", _T("주문수량이 상장주식수 대비 3%를 초과하는 이상주문입니다."));
						nType = MB_OK | MB_ICONINFORMATION;
					}
				}
			}
		} 
		break;

	case 8:		// 선물옵션
		{
			if (strCode.Left(1) == _T("1") || strCode.Left(1) == _T("4"))
			{
				if (lQty > 300 && lQty <= 500)
					strMsg.Format("%s\n", _T("주문수량이 300계약을 초과한 이상주문입니다."));
				else if (lQty > 500)
				{
					strMsg.Format("%s\n", _T("주문수량이 500계약을 초과한 이상주문입니다."));
					nType = MB_OK | MB_ICONINFORMATION;
				}
			}
			else if (strCode.Left(1) == _T("2") || strCode.Left(1) == _T("3"))
			{
				lAmount = lQty * lPrice * 100000;


				if (lAmount > 2000000000 && lAmount <= 5000000000)
					strMsg.Format("%s\n", _T("주문금액이 20억원을 초과하는 이상주문입니다."));
				else if (lAmount > 5000000000)
				{
					strMsg.Format("%s\n", _T("주문금액이 50억원을 초과하는 이상주문입니다."));
					nType = MB_OK | MB_ICONINFORMATION;
				}
			}
			else
				return TRUE;
		}	
		break;

	default:
		break;
	}

	strCode = 	((CBuyWnd*)GetParent())->GetStockCode();

	int type = -1;
	CString strInput = CString(strCode.operator LPCTSTR());	// buffer가 깨짐을 방지
	if (strInput.GetLength() > 0)
	{
		if (strInput.GetAt(0) == 'J' || strInput.GetAt(0) == 'A' || strInput.GetAt(0) == 'Q')
			strInput = strInput.Mid(1);
		type = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (LPARAM)strInput.operator LPCTSTR());
	}

//CString slog;
//slog.Format("%s %d", strCode, type);
//AfxMessageBox(slog);

	if(type == 6 || type == 27)  //ETF ,ETN 투자유의사항 경고 팝업
	{
		std::unique_ptr<CJumunCheck> jc = std::make_unique<CJumunCheck>();
		
		jc->m_msg = "ETF ETN 투자유의";
		const UINT ret = jc->DoModal();
		if (jc)	jc = nullptr;
		//UINT ret = pWnd->MessageBox(strMsg, "주문확인", nType);
		
		if (ret == IDCANCEL)
		{
			return false;
		}	
	}
	

	if (strMsg.IsEmpty()&&strMsg2.IsEmpty())
		return TRUE;
	else
	{
		std::unique_ptr<CJumunCheck> jc = std::make_unique<CJumunCheck>();
		if ((strMsg2 != "") && (strMsg2 != ""))	strMsg = strMsg + strMsg2;
		else if (strMsg2 != "")	strMsg = strMsg2;
		
		jc->m_msg = strMsg;
		const UINT ret = jc->DoModal();
		if (jc)	jc = nullptr;
		//UINT ret = pWnd->MessageBox(strMsg, "주문확인", nType);
		if (ret == IDOK)
		{
			return true;
		
		} else if (ret == IDCANCEL)
		{
			return false;
		}	
		//return false;
	}

	return false;
}


void CBuyGrid::ChangeFont(CFont* pFont, double* lrate)
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

void CBuyGrid::SetPallete(CMapWnd* pMapWnd)
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
		for (int j = 0; j < MAX_BUYGRIDCOL; j++)
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
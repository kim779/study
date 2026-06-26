// CAxGrid.cpp: 구현 파일
//

#include "pch.h"
#include "CAxGrid.h"
#include "griditem.h"

// CAxGrid

IMPLEMENT_DYNAMIC(AxGrid, CListCtrl)

AxGrid::AxGrid()
{
	_brSelected.CreateSolidBrush(RGB(200, 200, 200));
	_brColumnSelected.CreateSolidBrush(RGB(191, 191, 255));
	_brBack.CreateSolidBrush(RGB(255, 255, 255));

	_clrSelect = RGB(255, 255, 255);
	_clrNormal = RGB(0, 0, 0);
	_clrUp     = fontred;
	_clrDown   = fontblue;
}

AxGrid::~AxGrid()
{
}

BEGIN_MESSAGE_MAP(AxGrid, CListCtrl)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &AxGrid::OnLvnColumnclick)
	ON_MESSAGE(gridmessage, OnDrawGrid)
	ON_NOTIFY_REFLECT(NM_CLICK, &AxGrid::OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &AxGrid::OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, &AxGrid::OnNMRClick)
END_MESSAGE_MAP()


// CAxGrid 메시지 처리기
BOOL AxGrid::creategrid(CRect rect, CWnd* pWnd, int rowheight, int time, UINT nID)
{
	// TODO: 여기에 구현 코드 추가.
	_Height  = rowheight;

	BOOL bFlag = CreateEx(0,
		              LVS_OWNERDATA | WS_VISIBLE | WS_CHILD | LVS_REPORT | WS_CLIPCHILDREN  | LVS_OWNERDRAWFIXED | LVS_AUTOARRANGE | LVS_SORTASCENDING | LVS_SORTDESCENDING, rect, pWnd, nID);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | TVS_EX_AUTOHSCROLL | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE);

	const int row    = _vData.size();
	const int column = _vHeader.size();
	int col = 0;
	for_each(_vHeader.begin(), _vHeader.end(), [&](const auto& item){		
		InsertColumn(col++, std::get<headerinfo_name>(item), std::get<headerinfo_format>(item), std::get<headerinfo_width>(item));
	});

	for (int ii = 0; ii < row; ii++)
		InsertItem(ii, _T(""), 0);
	
	_font.CreatePointFont(90, _T("굴림체"));

	if (_itemfont == nullptr)
		SetFont(&_font);
	else
		SetFont(_itemfont);

	SetTimer(TIMER_DRAW, time, [](HWND hwnd, UINT id, UINT_PTR ss, DWORD dw){
		::KillTimer(hwnd, id);
		::PostMessage(hwnd, gridmessage, grid_onedraw, 0);
	});
	return bFlag;
}

void AxGrid::SetBKSelColor(COLORREF clr)
{
	_brSelected.DeleteObject();
	_brSelected.CreateSolidBrush(clr);
};


LRESULT AxGrid::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CListCtrl::WindowProc(message, wParam, lParam);
}


void AxGrid::DrawItem(LPDRAWITEMSTRUCT lp)
{
	// TODO:  지정된 항목을 그리는 코드를 추가합니다.
	_drawing = true;
	const int iItem     = lp->itemID;
	const int iColCount = GetHeaderCtrl()->GetItemCount();
	CDC* pDC = CDC::FromHandle(lp->hDC);
	CFont* oldfont{};
	if (_itemfont)  
		oldfont = pDC->SelectObject(_itemfont);
	for (int ii = 0; ii < iColCount; ii++)
		DrawSubItem(pDC, lp, iItem, ii);
	if (oldfont)	
		pDC->SelectObject(oldfont);
	_drawing = false;
}

void AxGrid::SetHeaderColor(COLORREF txtclr, COLORREF bgclr)
{
	_AxHeader.SetHeaderColor(txtclr, bgclr);
	_AxHeader.Invalidate();
}

void AxGrid::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if(_AxHeader.GetSafeHwnd() == NULL)
		_AxHeader.SubclassWindow(GetHeaderCtrl()->m_hWnd);
	lpMeasureItemStruct->itemHeight = _Height;
}

void AxGrid::DrawSubItem(CDC* pDC, LPDRAWITEMSTRUCT lp, const int& nItem, const int& nSubItem)
{
	const int rowmax = _vData.size() - 1;
	if (rowmax < nItem)
		return;

	const auto& item = _vData[nItem].first;
//	const auto hidden =  _vData[nItem].second;
	const int colmax = item.size() - 1;
	
	if (colmax < nSubItem)
		return;

	CRect rc;
	if (!GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rc))
		return;

	CString strText = item[nSubItem];
	strText.Trim();
	
	if (lp->itemState & CDIS_SELECTED)
	{
		pDC->FillRect(&rc, &_brSelected);
	}
	else if (_sort != 0 && nSubItem == _sortSel)
	{
		pDC->FillRect(&rc, &_brSelected);
	}
	else
	{
		CBrush* bg =  &_brBack;
		if (_step)
			bg = ((nItem % (_step * 2)) < _step) ? &_br1st : &_br2st;			
		pDC->FillRect(&rc, bg);
	}

	auto[stxt, fontcolor, contrast] = Formating(strText, nSubItem);
	if (stxt.CompareNoCase(_T("0")) != 0)
	{
		pDC->SetTextColor(fontcolor);
		
		if (contrast != -1)
			drawMark(pDC, rc, contrast);
		rc.DeflateRect(4, 0);
		pDC->DrawText(stxt, &rc, DT_SINGLELINE | DT_VCENTER | std::get<headerinfo_itemformat>(_vHeader[nSubItem]));
	}
}

void  AxGrid::drawMark(CDC* pDC, CRect rc, const int& type)
{	
	CRect rcRow(rc);

	rc.right = rc.left + 20;
	rc.DeflateRect(4,4);
	rc.OffsetRect(-4, 0);
	COLORREF clr = _clrNormal;
	CString text;
	switch (type)
	{
	case 1:	 text = ARR_UP; 
		 clr  = _clrUp;
		break;
	case 2:  text = TRI_UP; 
		 clr  = _clrUp;
		break;
	case 4:	 text = ARR_DN; 
		 clr  = _clrDown;
		break;
	case 5:  text = TRI_DN; 
		 clr  = _clrDown;
		break;
	default: text = _T(""); 
	 	//clr  = _clrNormal;
	 	break;
	}
	if ((type == 1) || (type == 4))
	{	
		pDC->FillSolidRect(rcRow, clr);
		pDC->SetTextColor(_clrSelect);
	}
	drawContrast(pDC, rc, text, ((type == 1) || (type == 4)) ? _clrSelect : clr);
}


std::tuple<CString, COLORREF, int>  AxGrid::Formating(CString& str, const int& nSubItem)
{
	int contrast = -1;
	CString strtemp;
	COLORREF fontcolor = _clrNormal;
	if (str[0] == '-')
		fontcolor = _clrDown;
	else if (str[0] == '+')
		fontcolor = _clrUp;
	
	const UINT attr = std::get<headerinfo_itemtype>(_vHeader[nSubItem]);
	
	if (attr & GVAT_CONTRAST)
	{
		if (str.GetLength() > 1)
		{
			strtemp = str[0];
			contrast = atoi(strtemp);
			switch (strtemp[0])
			{
			case '+':
				contrast = 2;
			case '1':
			case '2':
				fontcolor = _clrUp;
				break;
			case '-':
				contrast = 5;
			case '4':
			case '5':
				fontcolor = _clrDown;
				break;
			case '3':
			default:
			contrast  = -1;
				break;
			}
			strtemp = str.Mid(1, str.GetLength());
			str = strtemp;
		}
	}

	str.Remove('-');
	str.Remove('+');

	if (attr & GVAT_COMMA)
	{
		str = makeComma(str);
		if (atof(str) == 0.0)
			str = "0";
	}
	return std::tuple(std::move(str), std::move(fontcolor), std::move(contrast));
}

void AxGrid::PreSubclassWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CListCtrl::PreSubclassWindow();
}

CString& AxGrid::makeComma(CString& str)
{
	CString strtemp, stail;
	if (str.GetLength() > 0)
	{
		const int index = str.Find(_T("."));
		if (index != -1)
		{
			stail = str.Right(str.GetLength() - index);
			str   = str.Left(index);
		}

		strtemp = str.MakeReverse();
		const int n = (str.GetLength() - 1) / 3;
		for (int ii = n; ii > 0; ii--)
			strtemp.Insert(3 * ii, ',');
		str = strtemp.MakeReverse();

		if (!stail.IsEmpty())
			str += stail;
	}
	return str;
}


void AxGrid::OnDestroy()
{
	CListCtrl::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	clear();
	KillTimer(TIMER_DRAW);
	gdiclear();
	
}

void AxGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void AxGrid::OnLvnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	const UINT col = pNMLV->iSubItem;
	
	NM_USER user{};
	user.hdr.code = LVN_COLUMNCLICK;
	user.item.row = -1;
	user.item.col = col;
	GetParent()->SendMessage(WM_NOTIFY, MAKEWPARAM(0, GetDlgCtrlID()), (LPARAM)&user);

	const bool type = std::get<headerinfo_sort>(_vHeader[col]);
		
	_sort = (_sort + 2) % 3 - 1;
	if (type)
	{	
		std::sort(_vData.begin(), _vData.end(), [&](const auto& item1, const auto& item2) {
			if (_sort == 0)
			{
				const CString s1 = item1.second.at("order");
				const CString s2 = item2.second.at("order");
				return  s2.CompareNoCase(s1) >= 0;
			}
			else if (item1.first.size() > col && item2.first.size() > col)
			{
				const CString val1 = item1.first.at(col);
				const CString val2 = item2.first.at(col);

				if (col == 0)
					return (_sort * val1.Compare(val2)) >= 0;
				else
					return  _sort > 0 ? atof(val1) > atof(val2) : atof(val1) < atof(val2);
			}
			return false;
		});

		_sortSel = col;
		_AxHeader.SetSortArrow(col, _sort);
		Invalidate();	
		
		std::future work = AxStd::async([&](){
			return ResetSymbolKey();
		});
		work.get();
	}
	*pResult = 0;
}

void AxGrid::drawitem(int row, std::vector<int>& vCol)
{
	auto& item = _mDraw.emplace(std::move(row), std::move(vCol));		
	if (!item.second)
	{
		if(vCol.size() > item.first->second.size())
			_mDraw.at(row) = vCol;
	}
}

afx_msg LRESULT AxGrid::OnDrawGrid(WPARAM wParam, LPARAM lParam)
{
	if (_mDraw.size() == 0 || _drawing)
		return 0;

	const int top = GetTopIndex();
	const int page = top + GetCountPerPage();

	for_each(_mDraw.begin(), _mDraw.end(), [&](auto& item) {
		const int& row   = item.first;
		const auto& second = item.second;
		if (IS_WITHIN(top, page, row))
		{
			CRect rcDraw, rc;
			for_each(second.begin(), second.end(), [&](const int& col) {		
				if (!GetSubItemRect(row, col, LVIR_LABEL, rc))
					return;
				rcDraw.UnionRect(rcDraw, rc);
			});
			InvalidateRect(rcDraw);
		}
		_mDraw.erase(row);
	});
	return  0;
}

void AxGrid::clear()
{
	_sortSel = -1;
	_mDraw.clear();
	_vData.clear();
	_keymap.clear();
	Invalidate();
}

bool AxGrid::ResetSymbolKey()
{
	_keymap.clear();
	int row  = 0;	
	for_each(_vData.begin(), _vData.end(), [&](const auto& item){	
		auto& hidden = item.second;	
		if (auto& ft = hidden.find(_T("1021")); ft != hidden.end())
			_keymap.emplace(ft->second, row);	
		row++;
	});
	return true;
}

void AxGrid::drawContrast(CDC *pDC, CRect cellRc, CString sText, COLORREF clr)
{
	CRect	pRc = cellRc;
	float	hh = (float)cellRc.Height();
	CSize	size;
	CPoint	pts[7];
	CPen	pen, * pOldPen{};
	CBrush	brush, * pOldBrush{};
	
	pen.CreatePen(PS_SOLID, 1, clr);
	pOldPen = (CPen *) pDC->SelectObject(&pen);

	brush.CreateSolidBrush(clr); // bkcolor
	pOldBrush = (CBrush *) pDC->SelectObject(&brush);

	pRc.top = (cellRc.top + cellRc.Height()/2) - (int)(hh/2);
	pRc.bottom = (int)(pRc.top + hh);
	
	size = pDC->GetOutputTextExtent(ARR_UP);
	if (size.cx > 10)
		pRc.left = (int)(pRc.right - (size.cx * 0.8));
	else
		pRc.left = pRc.right - size.cx;
	
	hh = (float)(int)(pRc.Height()+1)/2;
	hh = (float)(int)((hh+1)/2) * 2 - 1;
	int ww = pRc.Width();
	pRc.right = pRc.left + 8;
	ww = pRc.Width() / 2 - 1;
	
	if (pRc.left >= cellRc.left && pRc.right <= cellRc.right)
	{
		pDC->SetPolyFillMode(ALTERNATE);
		if (!sText.Compare(ARR_UP))
		{
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.top;
			pts[3].y = pRc.bottom;
			pts[4].y = pRc.bottom;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.left + 1;
			pts[6].x = pRc.right - 1;
			pts[2].x = pRc.left + ww;
			pts[3].x = pRc.left + ww;
			pts[4].x = pRc.right - ww;
			pts[5].x = pRc.right - ww;
			pDC->Polygon(pts, 7);
		}
		else if (!sText.Compare(ARR_DN))	
		{
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.bottom;
			pts[3].y = pRc.top;
			pts[4].y = pRc.top;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.right - 1;
			pts[6].x = pRc.left + 1;
			pts[2].x = pRc.right - ww;
			pts[3].x = pRc.right - ww;
			pts[4].x = pRc.left + ww;
			pts[5].x = pRc.left + ww;
			pDC->Polygon(pts, 7);
		}
		else if (!sText.Compare(TRI_UP))	
		{
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.top;
			pts[1].y = pRc.bottom;
			pts[2].y = pRc.bottom;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.left;
			pts[2].x = pRc.right;
			pDC->Polygon(pts, 3);
		}
		else if (!sText.Compare(TRI_DN))	
		{
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.bottom;
			pts[1].y = pRc.top;
			pts[2].y = pRc.top;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.right;
			pts[2].x = pRc.left;
			pDC->Polygon(pts, 3);
		}
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
	pen.DeleteObject();
}

void AxGrid::gdiclear()
{
	_brSelected.DeleteObject();
	_brColumnSelected.DeleteObject();
	_brBack.DeleteObject();
	if(_br1st.m_hObject)
		_br1st.DeleteObject();
	if (_br2st.m_hObject)
		_br2st.DeleteObject();
	_font.DeleteObject();
}

void AxGrid::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pactive = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if  (_sortSel >=  0)
	{
		_sortSel = -1;
		RedrawWindow();
	}

	NM_USER user{};
	user.hdr = pactive->hdr;
	user.item.row = pactive->iItem;
	user.item.col = pactive->iSubItem;
	GetParent()->SendMessage(WM_NOTIFY, MAKEWPARAM(0, GetDlgCtrlID()), (LPARAM)&user);
	*pResult = 0;
}

void AxGrid::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pactive = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NM_USER user{};
	user.hdr = pactive->hdr;
	user.item.row = pactive->iItem;
	user.item.col = pactive->iSubItem;
	GetParent()->SendMessage(WM_NOTIFY, MAKEWPARAM(0, GetDlgCtrlID()), (LPARAM)&user);
	*pResult = 0;
}

void AxGrid::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LPNMITEMACTIVATE pactive = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	NM_USER user{};
	user.hdr = pactive->hdr;
	user.item.row = pactive->iItem;
	user.item.col = pactive->iSubItem;

	GetParent()->SendMessage(WM_NOTIFY, MAKEWPARAM(0, GetDlgCtrlID()), (LPARAM)&user);
	*pResult = 0;
}

void AxGrid::runExcelThread()
{
	auto future = AxStd::async([&]() {
		toExcel();	
		runExcel();	
	});

	const std::chrono::milliseconds wtime(200);
	while (future.wait_for(wtime) != std::future_status::ready)
		AxStd::_Msg("Thread runing");
}

bool AxGrid::toExcel(bool visible)
{
	_excelS.Empty();
	CString	text = _T("");

	for (const auto& vitem : _vData)
	{	
		int col = 0;
		for_each(vitem.first.begin(), vitem.first.end(), [&](CString text) {
			CString temps;
			auto& cell = _vHeader.at(col++);
			UINT attr = std::get<headerinfo_itemtype>(cell);
			if (text.GetLength() > 0)
			{
				//_vHeader
				if (attr & GVAT_CONTRAST)
				{
					switch (text[0])
					{
					case '1':		temps = ARR_UP; break;
					case '2': case '+':	temps = TRI_UP; break;
					case '4':		temps = ARR_DN; break;
					case '5': case '-':	temps = TRI_DN; break;
					default:		temps = _T("  "); break;
					}
					text = text.Mid(1); text.Insert(0, temps);
				}

				if (attr & GVAT_CONDITIONx)
				{
					text.Remove('+'); text.Remove('-');
				}
			}

			text.Replace("\r\n", "/");	// 일봉데이터
			text.Replace("\t", "/");	// 
			_excelS += text; _excelS += "\t";
		});
		_excelS += "\n";
	}
	return true;
}

bool AxGrid::runExcel()
{
	if (_excelS.IsEmpty())
		return false;

	::OleInitialize(nullptr);

	_Application	excel;
	if (!excel.CreateDispatch("Excel.Application"))
	{
		TRACE("CreateDispatch error....\n");
		::OleUninitialize();
		return true;
	}

	COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	Workbooks  Wbs; _Workbook  Wb;
	Worksheets Wss; _Worksheet Ws;
	Range	range;

	Wbs = excel.GetWorkbooks();
	Wb  = Wbs.Add(vOpt);

	Wss = Wb.GetWorksheets();
	Ws  = Wss.Add(vOpt, vOpt, vOpt, vOpt);

	Ws.SetName("Document");

	if (_excelS.IsEmpty())
	{
		excel.ReleaseDispatch();
		::OleUninitialize(); 
		return false;
	}

	const int alphaC = 24;
	const int cols = _vHeader.size();
	const int rows = _vData.size();
	const int fixedRows = 1;
	CString	string; string.Format("A1:%c%d", (char)('A' + cols), fixedRows + rows);
	
	
	if (cols >= alphaC)
		string.Format("A1:%c%c%d", (char)('A' + ((cols/alphaC) -1)),
					(char)('A' + cols % alphaC), fixedRows + rows);

	range = Ws.GetRange(COleVariant((LPCTSTR)string), vOpt);
	range.Clear();

	if (_excelS.IsEmpty())
	{
		excel.ReleaseDispatch();
		::OleUninitialize();	// m_excelH = nullptr;
		return false;
	}

	for (int jj = 0; fixedRows && jj < cols; jj++)
	{
		string.Format("%c%d:%c%d", (char)('A' + jj), fixedRows, (char)('A' + jj), rows);
		if (jj >= alphaC)
		{
			string.Format("%c%c%d:%c%c%d", (char)('A' + ((jj / alphaC)-1)),
							(char)('A' + jj % alphaC), fixedRows,
							(char)('A' + ((jj / alphaC)-1)),
							(char)('A' + jj % alphaC), rows);
		}

		Range rangex = Ws.GetRange(COleVariant((LPCTSTR)string), vOpt);
		string = _T("@"); rangex.SetNumberFormat(COleVariant(string));
	}

	long row{}, col{};
	bool last = false;
	int idx{}; CString stringX;
	for (int ii = 0; !_excelS.IsEmpty() && ii < rows; ii++)
	{
		idx = _excelS.Find("\n");
		if (idx != -1)
		{
			stringX = _excelS.Left(idx);
			_excelS = _excelS.Mid(idx+1);
		}
		else
		{
			stringX = _excelS; _excelS = _T(""); last = true;
		}

		for (int jj = 0; !stringX.IsEmpty() && jj < cols; jj++)
		{
			idx = stringX.Find("\t");
			if (idx != -1)
			{
				string  = stringX.Left(idx);
				stringX = stringX.Mid(idx+1);
			}
			else
			{
				string = stringX; stringX = _T("");
			}

			if (string.IsEmpty())
				continue;

			TRY
			{
				row = ii+1; col = jj+1;
				range.Set_Default(COleVariant(row), COleVariant(col), COleVariant((LPCTSTR)string));
			}
			CATCH(COleDispatchException, e)
			{
				TRACE("runexcel error [%s]\n", e->m_strDescription);
				break;
			}
			END_CATCH
		}

		if (!Ws.m_lpDispatch) break;
	}

	excel.SetVisible(_excelF);
	if (!_excelF)
	{
		Wbs.Close(); excel.Quit();
	}

	excel.ReleaseDispatch();
	::OleUninitialize();
	return true;
}

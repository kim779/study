// RtsInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "RtsInfo.h"
#include "afxdialogex.h"

#include "../h/mapform.h"
#include "../h/mainvar.h"
#include "../h/seq.H"

#define IDC_LIST_INFO		1000
#define IDC_LIST_RTS		1001

#define	GB_RTS			0x08
#define GB_RTSSYMBOL		0x0c

//#define GB_XML_RTSSYMBOL	'#'

// CRtsInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRtsInfo, CDialogEx)

CRtsInfo::CRtsInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRtsInfo::IDD, pParent)
{
	m_row = -1;
	m_data = _T("");
	m_parent = pParent;
	m_rtsMap.RemoveAll();
}

CRtsInfo::~CRtsInfo()
{
	m_rtsMap.RemoveAll();
}

void CRtsInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RTSCOMBO, m_rtscombo);
}


BEGIN_MESSAGE_MAP(CRtsInfo, CDialogEx)
	ON_BN_CLICKED(IDC_RTS_SAVE, &CRtsInfo::OnBnClickedRtsSave)
	ON_BN_CLICKED(IDOK, &CRtsInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RTS_DELETE, &CRtsInfo::OnBnClickedRtsDelete)
END_MESSAGE_MAP()


// CRtsInfo 메시지 처리기입니다.

BOOL CRtsInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect	rect;

	// Saved RTS Information
	((CStatic*)GetDlgItem(IDC_STATIC1))->GetWindowRect(&rect);
	ScreenToClient(rect);
	if (!m_info.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LVS_REPORT | LVS_ALIGNTOP | LVS_NOSORTHEADER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, this, IDC_LIST_INFO))
		return -1;

	m_info.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_info.InsertColumn(0, _T("Saved Keys"), LVCFMT_CENTER, 150);
	m_info.m_focus = false;
	
	// Second RTS Symbol
	((CStatic*)GetDlgItem(IDC_STATIC2))->GetWindowRect(&rect);
	ScreenToClient(rect);
	if (!m_list.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LVS_REPORT | LVS_ALIGNTOP | LVS_NOSORTHEADER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, this, IDC_LIST_RTS))
		return -1;

	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0, _T("Object Name"), LVCFMT_CENTER, 90);
	m_list.InsertColumn(1, _T("Type"), LVCFMT_CENTER, 90);
	m_list.InsertColumn(2, _T("RTS Symbol"), LVCFMT_CENTER, 85);
	m_list.InsertColumn(3, _T("RTS Info"), LVCFMT_CENTER, 70);
	m_list.m_focus = false;
	m_list.m_skipCol = 3;

	setFont();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CRtsInfo::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	LPNMHDR	pHdr = (LPNMHDR)lParam;
	LPNMLISTVIEW pNlv = (LPNMLISTVIEW)lParam;

	switch (LOWORD(wParam))
	{
	case IDC_LIST_INFO:
		if (pHdr->code == NM_CLICK)
		{
			CString	tmps = m_info.GetItemText(m_info.m_focusRow, 0);
			makeRtsSymbol(tmps);
			break;
		}
		break;
	
	case IDC_LIST_RTS:
		if (pHdr->code == NM_CLICK)
		{
			CString tmps;
			bool	checkAll = false;

			if (m_list.m_focusCol > 1)
			{
				tmps = m_list.GetItemText(m_list.m_focusRow, 3);
				if (tmps.IsEmpty())
					m_list.SetItemText(m_list.m_focusRow, 3, _T("■"));
				else
					m_list.SetItemText(m_list.m_focusRow, 3, _T(""));
			}

			// select All
			for (int jj = 0; jj < m_list.GetItemCount(); jj++)
			{
				if (m_list.GetItemText(jj, 3).CompareNoCase(_T("■")))
				{
					checkAll = false;
					break;
				}
				checkAll = true;
			}
			((CButton*)GetDlgItem(IDC_CHECK_ALL))->SetCheck(checkAll);
		}
		break;
	}
	return CDialogEx::OnNotify(wParam, lParam, pResult);
}

BOOL CRtsInfo::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_RTSCOMBO:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			CString	text, tmps;
			int	index = m_rtscombo.GetCurSel();

			m_rtscombo.GetLBText(index, text);
			tmps = text;
			if (text.Find(" (") != -1)
				text = text.Left(text.Find(" ("));

			m_row = -1;
			selectSymbolList(text);
			((CButton*)GetDlgItem(IDC_CHECK_ALL))->SetCheck(false);

			// 테이블에 있는지 없는지 체크
			for (int ii = 0; ii < m_info.GetItemCount(); ii++)
			{
				if (!m_info.GetItemText(ii, 0).CompareNoCase(tmps))
				{
					makeRtsSymbol(tmps);
					break;
				}
			}
		}
		break;

	case IDC_CHECK_ALL:
		selectAll(IsDlgButtonChecked(IDC_CHECK_ALL) ? true : false);
		break;
	}
	return CDialogEx::OnCommand(wParam, lParam);
}

void CRtsInfo::OnBnClickedRtsSave()
{
	CString name, tmp, tmpx, tmps, str, type;
	CString rtskey;
	int	index, pos = -1;
	bool	bfirst = true;
		
	index = m_rtscombo.GetCurSel();
	m_rtscombo.GetLBText(index, name);
	if (name.IsEmpty())
		return;

	rtskey = name;
	if (name.Find(" (") != -1)
		rtskey = name.Left(name.Find(" ("));
	
	tmp.Format("%s%c%s", _T("RTSINFO"), GB_RTSSYMBOL, rtskey);
	tmpx.Format("%s%c%s", _T("RTSINFO"), GB_XML_SYMBOL, rtskey);
	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		tmps = m_list.GetItemText(ii, 3);
		if (!tmps.IsEmpty() && tmps.CompareNoCase(_T("key")))
		{
			type = m_list.GetItemText(ii, 1);
		/*	if (m_bgrid)	// always false
			{
				if (type.CompareNoCase(_T("GRID")))
					continue;
			}*/

			if (bfirst)
			{
				tmp += _T("|");
				tmpx += _T("|");
				bfirst = false;
			}
			else
			{
				
				tmp += _T(",");
				tmpx += _T(",");
			}

		/*	if (m_bgrid && !type.CompareNoCase(_T("GRID")))		// always false
			{
				tmp += _T("$");
				tmpx +=  _T("$");
			}
		*/

			tmp += m_list.GetItemText(ii, 0);
			tmpx += m_list.GetItemText(ii, 0);
		}
	}
	tmps.Format(_T("%c"), GB_RTS);
	tmp += tmps;

	if (!name.IsEmpty())
	{
		m_rtsMap.SetAt(name, tmpx);
		bool	check = false;
		for (int ii = 0; ii < m_info.GetItemCount(); ii++)
		{
			if (!m_info.GetItemText(ii, 0).CompareNoCase(name))
			{
				check = true;
				break;
			}
		}
		if (!check)
			m_info.InsertItem(m_info.GetItemCount(), name);
	}

	setMapH();
}

void CRtsInfo::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CRtsInfo::OnBnClickedRtsDelete()
{
	CString name, str, tmp;

	m_rtscombo.GetLBText(m_rtscombo.GetCurSel(), name);
	if (name.IsEmpty())
		return;
		
	if (m_rtsMap.Lookup(name, str))
	{
		m_rtsMap.RemoveKey(name);
		for (int ii = 0; ii < m_info.GetItemCount(); ii++)
		{
			if (!m_info.GetItemText(ii, 0).CompareNoCase(name))
			{
				m_info.DeleteItem(ii);
				break;
			}
		}
		for (int jj = 0; jj < m_list.GetItemCount(); jj++)
			m_list.SetItemText(jj, 3, _T(""));
	}
	else
	{
		for (int ii = 0; ii < m_info.GetItemCount(); ii++)
		{
			if (!m_info.GetItemText(ii, 0).CompareNoCase(name))
				m_info.DeleteItem(ii);
		}
	}
	setMapH();
}

void CRtsInfo::SetRtsInfo(CString info)
{
	int	ii = 0;
	CString tmp, key, tr, lay, tmpkey, title;

	clear();
	if (info.IsEmpty())
		return;

	while (!info.IsEmpty())
	{
		tr = parser(info, _T("^"));
		key = parser(tr, _T("\t"));
		if (ii == 0)
			tmpkey = key;
		if (tr.IsEmpty())
			continue;
				
		if (!compareRTS(key))
			continue;

		m_info.InsertItem(ii, key);
		m_rtsMap.SetAt(key, tr);
		ii++;
	}

	if (m_rtscombo.GetCount() > 0)
		makeRtsSymbol(tmpkey);
}

void CRtsInfo::SetFormInfo(CString text)
{
	CString symbol, data, gb;
	
	m_list.DeleteAllItems();
	m_data = text;
	m_row = -1;
	gb.Format("%c", GB_RTSSYMBOL);

	while (!text.IsEmpty())
	{
		data = parser(text, gb);
		makeData(data);
	}

	if (m_rtscombo.GetCount() > 0)
	{
		m_rtscombo.SetCurSel(0);

		m_rtscombo.GetLBText(0, symbol);
		if (symbol.Find(" (") != -1)
			text = symbol.Left(symbol.Find(" ("));

		selectSymbolList(text);
	}
}

void CRtsInfo::makeData(CString trsymbol)
{
	CString tmp, tmps, symbol;
	CString type = _T("");
	bool	bgrid = false;
	bool	sgrid = false;
	bool	sgridex = false;
	bool	stable = false;
	int	col = 0;
	int	idx = m_rtscombo.GetCount();

	if (!trsymbol.IsEmpty())
	{
		if (trsymbol.Find(_T("$")) > 0)
		{
			bgrid = true;
			trsymbol.Replace(_T("$"), _T(""));
		}
	}

	if (!bgrid)
	{
		tmp = parser(trsymbol, _T("="));
		tmps.Format("%s (%s)", tmp, trsymbol.Left(trsymbol.Find("=")));
		m_rtscombo.InsertString(idx, tmps);
	}
	else
	{
		tmp = parser(trsymbol, _T("="));		
		tmps = tmp;
		tmp = parser(trsymbol, _T(":"));

		if (!tmp.CompareNoCase(_T("GRID")) || !tmp.CompareNoCase(_T("GRIDEX")))
		{
			CString str;
			str.Format("%s (%s)", tmps, tmp);
		}

		if (!tmp.CompareNoCase(_T("GRID")))
		{
			sgrid = true;
			symbol = tmps;
			type = _T("GRID");
			m_selectMap.SetAt(tmps, trsymbol);
		}

		if (!tmp.CompareNoCase(_T("GRIDEX")))
		{
			sgrid = true;
			symbol = tmps;
			type = _T("GRIDEX");
			m_selectMap.SetAt(tmps, trsymbol);
		}
		
		if (!tmp.CompareNoCase(_T("TABLE")))
		{
			symbol = tmps;
			type = _T("TABLE");
			stable = true;			
		}

		while (!trsymbol.IsEmpty())
		{
			col = 0;
			tmp = parser(trsymbol, _T(":"));				
			tmps.Format(_T("%s.%s (%s)"), symbol, tmp.Left(tmp.Find(_T("="))), type);
			idx = m_rtscombo.GetCount();
			m_rtscombo.InsertString(idx, tmps);				
		}
	}
}

void CRtsInfo::makeRtsSymbol(CString key)
{
	CString tmp, str;
	int	row;
	bool	checkAll = false;
	
	if (!m_rtsMap.Lookup(key, str))
		return;

	m_row = -1;
	tmp = parser(str, _T("|"));
	parser(tmp, _T("/"));

	if (!tmp.IsEmpty())
	{
		row = m_rtscombo.FindString(0, tmp);
		if (row > -1)
		{
			m_rtscombo.SetCurSel(row);
			selectSymbolList(tmp);
		}
	}

	while (!str.IsEmpty())
	{
		tmp = parser(str, _T(","));
		if (!tmp.IsEmpty())
		{
			if (tmp.Find(_T("$")) >= 0)
				tmp.Replace(_T("$"), _T(""));

			row = findRow(tmp);
			if (row > -1)
				m_list.SetItemText(row, 3, _T("■"));
		}
	}

	// select All
	for (int jj = 0; jj < m_list.GetItemCount(); jj++)
	{
		if (m_list.GetItemText(jj, 3).CompareNoCase(_T("■")))
		{
			checkAll = false;
			break;
		}
		checkAll = true;
	}
	((CButton*)GetDlgItem(IDC_CHECK_ALL))->SetCheck(checkAll);

}

void CRtsInfo::selectSymbolList(CString symbol)
{
	CString trsymbol, tmp, tmps, tmpSymbol, tmpSubSymbol;
	CString strName, strType, rtsData, data;
	CString gb, sym;
	bool	bgrid = false;
	int	col;

	rtsData = m_data;
	gb.Format("%c", GB_RTSSYMBOL);
	
	m_list.DeleteAllItems();

	tmpSubSymbol = symbol;
	tmpSymbol = parser(tmpSubSymbol, _T("."));
	if (m_selectMap.Lookup(tmpSymbol, trsymbol))
	{
		tmp.Format(_T("%s$="), tmpSymbol);
		strType = rtsData.Mid(rtsData.Find(tmp)+tmp.GetLength());
		strType = strType.Left(strType.Find(':'));
		while (!trsymbol.IsEmpty())
		{
			tmp = parser(trsymbol, _T(":"));
			sym = tmp;
			tmp = parser(sym, _T("="));

			if (!tmpSubSymbol.CompareNoCase(tmp))
				continue;

			col = 0;
			m_row++;			
			tmps.Format(_T("%s.%s"), tmpSymbol, tmp);
			m_list.AddItem(m_row, col, tmps);
			col++;

			m_list.AddItem(m_row, col++, strType);
			m_list.AddItem(m_row, col, sym);
		}
	}
	else
	{
		while (!rtsData.IsEmpty())
		{
			data = parser(rtsData, gb);
			if (data.Find(_T("GRID")) != -1)
			{
				col = 0;
				m_row++;
				tmp = parser(data, _T("$="));
				m_list.AddItem(m_row, col++, tmp);
				tmp = parser(data, _T(":"));
				m_list.AddItem(m_row, col, tmp);
				continue;
			}

			if (data.Find(_T("$")) == -1)
			{
				if (data.Find(symbol) != -1)
					continue;
				
				col = 0;
				m_row++;
				tmp = parser(data, _T("="));
				m_list.AddItem(m_row, col++, tmp);
				tmp = parser(data, _T("="));
				m_list.AddItem(m_row, col++, tmp);
				m_list.AddItem(m_row, col, data);
			}
			else		// table
			{
				col = 0;
				data.Replace(_T("$"), _T(""));				
				strName = parser(data, _T("="));
				strType = parser(data, _T(":"));

				while (!data.IsEmpty())
				{
					col = 0;					
					tmp = parser(data, _T(":"));
					sym = tmp;
					tmp = parser(sym, _T("="));

					symbol = symbol.Mid(symbol.Find(".") + 1);
					if (!tmp.CompareNoCase(symbol))
						continue;

					m_row++;					
					tmps.Format(_T("%s.%s"), strName, tmp);
					m_list.AddItem(m_row, col++, tmps);
					m_list.AddItem(m_row, col++, strType);
					m_list.AddItem(m_row, col, sym);
				}				
			}						
		}
	}
}

bool CRtsInfo::compareRTS(CString key)
{
	CString symbol;
	for (int ii = 0; ii < m_rtscombo.GetCount(); ii++)
	{
		m_rtscombo.GetLBText(ii, symbol);
		if (!symbol.Compare(key))
			return true;
	}
	return false;
}

int CRtsInfo::findRow(CString symbol)
{
	int	row = -1;
	CString	tmps;

	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		tmps = m_list.GetItemText(ii, 0);
		if (!tmps.CompareNoCase(symbol))
		{
			row = ii;
			break;
		}
	}
	return row;
}

void CRtsInfo::clear()
{
	m_info.DeleteAllItems();
	m_rtsMap.RemoveAll();
}

void CRtsInfo::selectAll(bool check)
{
	for (int jj = 0; jj < m_list.GetItemCount(); jj++)
	{
		if (check)
			m_list.SetItemText(jj, 3, _T("■"));
		else
			m_list.SetItemText(jj, 3, _T(""));
	}
}

void CRtsInfo::setMapH()
{
	CString rtsinfo, sKey, str, tmp;

	for (POSITION pos = m_rtsMap.GetStartPosition(); pos != NULL; )
	{
		m_rtsMap.GetNextAssoc(pos, sKey, str);
		tmp.Format(_T("%s\t%s"), sKey, str);
		rtsinfo += tmp; 
		rtsinfo += _T("^");
	}

	m_parent->SendMessage(WM_USER, MAKEWPARAM(ID_USR_DATASET, 1), (LPARAM)rtsinfo.operator LPCSTR());
}

CString CRtsInfo::parser(CString &srcstr, CString substr)
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
	return _T("");
}

void CRtsInfo::setFont()
{
	::DeleteObject(m_font.Detach());

	LOGFONT	lf;
	NONCLIENTMETRICS info;

	afxGlobalData.fontRegular.GetLogFont(&lf);

	info.cbSize = sizeof(info);
	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_font.CreateFontIndirect(&lf);

	m_info.SetFont(&m_font);
	m_list.SetFont(&m_font);
}

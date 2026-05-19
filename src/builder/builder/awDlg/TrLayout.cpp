// TrLayout.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TrLayout.h"
#include "afxdialogex.h"

#include "../h/mapform.h"
#include "../h/mainvar.h"
#include "../h/seq.H"

#define IDC_LIST_INFO		1000
#define IDC_LIST_FORM		1001

#define symNONE		_T("**")
#define GAP		17

// CTrLayout 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTrLayout, CDialogEx)

CTrLayout::CTrLayout(CWnd* pParent /*=NULL*/, CString root)
	: CDialogEx(CTrLayout::IDD, pParent)
{
	m_root = root;
	m_parent = pParent;
	m_row = -1;
	m_frow = -1;
	m_symbol = _T("");
	m_desc = _T("");
	m_type = tNo;
	m_in.RemoveAll();
	m_out.RemoveAll();
	m_trMap.RemoveAll();
}

CTrLayout::~CTrLayout()
{
	m_in.RemoveAll();
	m_out.RemoveAll();
	m_trMap.RemoveAll();
}

void CTrLayout::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INOUT, m_inoutCB);
	DDX_Control(pDX, IDC_FOMRTR, m_trCB);
}

BEGIN_MESSAGE_MAP(CTrLayout, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_SAVE, &CTrLayout::OnBnSave)
	ON_BN_CLICKED(IDC_CLEAR, &CTrLayout::OnBnClickedClear)
	ON_BN_CLICKED(IDC_RESET, &CTrLayout::OnBnClickedReset)
	ON_BN_CLICKED(IDOK, &CTrLayout::OnBnClickedOk)
	ON_BN_CLICKED(IDC_NEW, &CTrLayout::OnBnClickedNew)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CTrLayout::OnBnClickedBtnDelete)
END_MESSAGE_MAP()

// CTrLayout 메시지 처리기입니다.

BOOL CTrLayout::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect	rc, rect;

	GetWindowRect(&rc);
	ScreenToClient(&rc);
	
	((CStatic*)GetDlgItem(IDC_STATIC_SERVER))->GetWindowRect(&rect);
	ScreenToClient(rect);
	rect.bottom = rc.bottom - GAP; rect.left = rc.left + GAP;
	if (!m_info.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LVS_REPORT | LVS_ALIGNTOP | LVS_NOSORTHEADER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, this, IDC_LIST_INFO))
		return -1;

	((CStatic*)GetDlgItem(IDC_STATIC_USER))->GetWindowRect(&rect);
	ScreenToClient(rect);
	rect.bottom = rc.bottom - GAP; rect.right = rc.right - GAP;
	if (!m_list.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LVS_REPORT | LVS_ALIGNTOP | LVS_NOSORTHEADER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, this, IDC_LIST_FORM))
		return -1;

	m_info.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE);
	m_info.InsertColumn(0, _T("Symbol"), LVCFMT_CENTER, 80);
	m_info.InsertColumn(1, _T("Description"), LVCFMT_CENTER, 90);
	m_info.InsertColumn(2, _T("Type"), LVCFMT_CENTER, 65);
	m_info.InsertColumn(3, _T("Size"), LVCFMT_CENTER, 65);
	m_info.InsertColumn(4, _T("I/O"), LVCFMT_CENTER, 45);

	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE);
	m_list.InsertColumn(0, _T("Object Name"), LVCFMT_CENTER, 85);
	m_list.InsertColumn(1, _T("Description"), LVCFMT_CENTER, 90);
	m_list.InsertColumn(2, _T("Type"), LVCFMT_CENTER, 65);
	m_list.InsertColumn(3, _T("Input"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(4, _T("Output"), LVCFMT_CENTER, 80);

	m_inoutCB.InsertString(0, _T("All"));
	m_inoutCB.InsertString(1, _T("Output"));
	m_inoutCB.InsertString(2, _T("Input"));
	m_inoutCB.SetCurSel(0);

	setFont();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CTrLayout::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_INOUT:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			CString tmps;
			int	index = m_inoutCB.GetCurSel();

			m_info.DeleteAllItems();
			m_row = -1;
			switch (index)
			{
			case 0:
				for (int ii = 0; ii < m_in.GetCount(); ii++)
				{
					tmps = m_in.GetAt(ii);
					makeIn(tmps);
				}
				for (int jj = 0; jj < m_out.GetCount(); jj++)
				{
					tmps = m_out.GetAt(jj);
					makeOut(tmps);
				}
				break;
			case 1:
				for (int jj = 0; jj < m_out.GetCount(); jj++)
				{
					tmps = m_out.GetAt(jj);
					makeOut(tmps);
				}
				break;
			case 2:
				for (int ii = 0; ii < m_in.GetCount(); ii++)
				{
					tmps = m_in.GetAt(ii);
					makeIn(tmps);
				}
				break;
			default:
				break;
			}
		}
		break;

	case IDC_FOMRTR:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			CString	path, tmps;

			m_trCB.GetLBText(m_trCB.GetCurSel(), tmps);
			path.Format("%s\\%s\\%s",m_root, TRDIR, tmps);
			ReadTrList(path);
		}
		break;
	default:
		break;
	}
	return CDialogEx::OnCommand(wParam, lParam);
}

BOOL CTrLayout::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CString	tmps;
	LPNMHDR pHdr = (LPNMHDR)lParam;
	LPNMLISTVIEW pNlv = (LPNMLISTVIEW)lParam;

	switch (LOWORD(wParam))
	{
	case IDC_LIST_INFO:
		if (pHdr->code == NM_CLICK)
		{
			CString	tmp, desc;
			bool	isgrid = false;

			m_symbol = _T("");
			m_desc = _T("");

			tmp = m_info.GetItemText(m_info.m_focusRow, 0);
			desc = m_info.GetItemText(m_info.m_focusRow, 1);

			if (!tmp.IsEmpty())
			{
				tmp.TrimLeft();
				tmp.TrimRight();
				if (tmp.Find(_T("-")) != -1)
				{
					tmp.Replace(_T("-"), _T(""));
					tmps = findGridParent(m_info.m_focusRow);
					isgrid = true;
				}
			}

			m_symbol = tmp;
			m_desc = desc;

			if (isgrid)
				m_symbol.Format(_T("%s.%s"), tmps, m_symbol);

			if (!m_info.GetItemText(m_info.m_focusRow, 4).CompareNoCase(_T("I")))
				m_selKind = tINPUT;
			else if (!m_info.GetItemText(m_info.m_focusRow, 4).CompareNoCase(_T("O")))
				m_selKind = tOUTPUT;
			else
				m_selKind = tNONE;
		}
		break;

	case IDC_LIST_FORM:
		switch (pHdr->code)
		{
		case NM_CLICK:
			switch (m_selKind)
			{
			case tINPUT:
				m_list.SetItemText(m_list.m_focusRow, 1, m_desc);
				m_list.SetItemText(m_list.m_focusRow, 3, m_symbol);
				break;
			case tOUTPUT:
				m_list.SetItemText(m_list.m_focusRow, 1, m_desc);
				m_list.SetItemText(m_list.m_focusRow, 4, m_symbol);
				break;
			case tNONE:
			default:
				break;
			}
			m_symbol = _T("");
			m_selKind = tNONE;
			break;

		case NM_DBLCLK:
			if (m_list.m_focusCol == 3 || m_list.m_focusCol == 4)
			{
				if (!m_list.GetItemText(m_list.m_focusRow, m_list.m_focusCol).IsEmpty())
				{
					m_list.SetItemText(m_list.m_focusRow, 1, _T(""));
					m_list.SetItemText(m_list.m_focusRow, m_list.m_focusCol, _T(""));
				}
			}
			break;
		}
		break;
	}
	return CDialogEx::OnNotify(wParam, lParam, pResult);
}

void CTrLayout::OnBnSave()
{
	int	pos, cnt;
	CString	combo, tmp, tmps, symbol, tablesymbol, in, out, xml, tmpx, desc;
	CString type, gridsym, key;
	bool	isgrid = false;
	bool	istable = false;

	cnt = m_list.GetItemCount();
	pos = m_trCB.GetCurSel();

	if (cnt <= 0 || pos < 0 || m_trCB.GetCount() <= 0)
		return;

	m_trCB.GetLBText(pos, combo);
	tmp.Format(_T("%s%c%s"), _T("TRINFO"), GB_SYMBOL, combo);
	for (int ii = 0; ii < cnt; ii++)
	{
		symbol = m_list.GetItemText(ii, 0);
		if (symbol.Find(symNONE) == 0)
			symbol.Replace(symNONE, _T(""));

		type = m_list.GetItemText(ii, 2);
		if (!type.CompareNoCase(_T("GRID")) || !type.CompareNoCase(_T("GRIDEX")))
		{
			gridsym = symbol;
			isgrid = true;
			istable = false;
			continue;

		}
		else if (!type.CompareNoCase(_T("TABLE")))
		{
			isgrid = false;
			istable = true;
			tablesymbol = symbol;
			continue;
		}
		else if (!type.CompareNoCase(_T("OUTPUT")) || !type.CompareNoCase(_T("EDIT"))
			|| !type.CompareNoCase(_T("COMBO")) || !type.CompareNoCase(_T("CONTROL")))
		{
			isgrid = false;
			istable = false;
		}

		if (isgrid || istable)
			symbol = m_list.GetItemText(ii, 2);

		in = m_list.GetItemText(ii, 3);
		desc = m_list.GetItemText(ii, 1);
		if (!in.IsEmpty())
		{
			if (isgrid)
			{
				tmps.Format(_T("%c$%s.%s=%s=%s"), GB_SYMBOL, gridsym, symbol, in, desc);
				tmpx.Format(_T("%c$%s.%s=%s=%s"), GB_XML_SYMBOL, gridsym, symbol, in, desc);
			}
			else
			{
				if (istable)
				{
					tmps.Format(_T("%c%s.%s=%s=%s"), GB_SYMBOL, tablesymbol, symbol, in, desc);
					tmpx.Format(_T("%c%s.%s=%s=%s"), GB_XML_SYMBOL, tablesymbol, symbol, in, desc);
				}
				else
				{
					tmps.Format(_T("%c%s=%s=%s"), GB_SYMBOL, symbol, in, desc);
					tmpx.Format(_T("%c%s=%s=%s"), GB_XML_SYMBOL, symbol, in, desc);
				}
			}
			tmp += tmps;
			xml += tmpx;
		}
	}

	isgrid = false;
	istable = false;

	tmps.Format(_T("%c"), GB_INOUT);
	tmpx.Format(_T("%c"), GB_XML_INOUT);
	tmp += tmps;
	xml += tmpx;

	for (int ii = 0; ii < cnt; ii++)
	{
		symbol = m_list.GetItemText(ii, 0);
		if (symbol.Find(symNONE) == 0)
			symbol.Replace(symNONE, "");

		type = m_list.GetItemText(ii, 2);
		if (!type.CompareNoCase(_T("GRID")))
		{
			gridsym = symbol;
			isgrid = true;
			istable = false;
			continue;
		}
		if (!type.CompareNoCase(_T("GRIDEX")))
		{
			gridsym = symbol;
			isgrid = true;
			istable = false;
			continue;
		}
		else if (!type.CompareNoCase(_T("TABLE")))
		{
			isgrid = false;
			istable = true;
			tablesymbol = symbol;
			continue;
		}
		else if (!type.CompareNoCase(_T("OUTPUT")) || !type.CompareNoCase(_T("EDIT")) ||
				 !type.CompareNoCase(_T("COMBO")) || !type.CompareNoCase(_T("CONTROL")))
		{
			isgrid = false;
			istable = false;
		}

		if (isgrid || istable)
			symbol = m_list.GetItemText(ii, 2);

		out = m_list.GetItemText(ii, 4);
		desc = m_list.GetItemText(ii, 1);
		if (!out.IsEmpty())
		{
			if (isgrid)
			{
				tmps.Format(_T("$%s.%s=%s=%s%c"), gridsym, symbol, out, desc, GB_SYMBOL);
				tmpx.Format(_T("$%s.%s=%s=%s%c"), gridsym, symbol, out, desc, GB_XML_SYMBOL);
			}
			else
			{
				if (istable)
				{
					tmps.Format(_T("%s.%s=%s=%s%c"), tablesymbol, symbol, out, desc, GB_SYMBOL);
					tmpx.Format(_T("%s.%s=%s=%s%c"), tablesymbol, symbol, out, desc, GB_XML_SYMBOL);
				}
				else
				{
					tmps.Format(_T("%s=%s=%s%c"), symbol, out, desc, GB_SYMBOL);
					tmpx.Format(_T("%s=%s=%s%c"), symbol, out, desc, GB_XML_SYMBOL);
				}
			}
			tmp += tmps;
			xml += tmpx;
		}
	}

	tmps.Format(_T("%c"), GB_TR);
	tmp += tmps;
	m_trMap.SetAt(combo, xml);
	setMapH();		
}

void CTrLayout::OnBnClickedClear()
{
	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		m_list.SetItemText(ii, 1, _T(""));
		m_list.SetItemText(ii, 3, _T(""));
		m_list.SetItemText(ii, 4, _T(""));
	}
}

void CTrLayout::OnBnClickedReset()
{
	CString	text;
	int	count = m_list.GetItemCount();

	for (int ii = 0; ii < count; ii++)
	{
		text = m_list.GetItemText(ii, 0);
		if (text.Find(symNONE) == 0)
		{
			text = m_list.GetItemText(ii, 2);
			m_list.DeleteItem(ii--);
			count--;

			if (!text.CompareNoCase(_T("GRID")) || !text.CompareNoCase(_T("GRIDEX")) || !text.CompareNoCase(_T("TABLE")))
			{
				for (; ++ii < count; )
				{
					text = m_list.GetItemText(ii, 0);
					if (!text.IsEmpty())
					{
						ii--;
						break;
					}
					m_list.DeleteItem(ii--);
					count--;
				}
			}
		}
	}
}

void CTrLayout::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CTrLayout::OnBnClickedNew()
{
	clear();
}

void CTrLayout::OnBnClickedBtnDelete()
{
	CString str, tmp, stmp;

	if (m_trCB.GetCount() > 0)
		m_trCB.GetLBText(m_trCB.GetCurSel(), tmp);

	if (tmp.IsEmpty())
		return;

	if (m_trMap.Lookup(tmp, str))
	{
		clear();

		m_trMap.RemoveKey(tmp);
		m_trCB.DeleteString(m_trCB.GetCurSel());
		if (m_trCB.GetCount() <= 0)
		{
			m_trCB.Invalidate();
			setMapH();
			return;
		}
		
		m_trCB.SetCurSel(0);
		m_trCB.GetLBText(m_trCB.GetCurSel(), tmp);
		if (!tmp.IsEmpty())
			makeInOut(tmp);
	}
	setMapH();
}

void CTrLayout::OnClose()
{
	CDialogEx::OnClose();
}

void CTrLayout::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
	if (!m_info || !m_list)
		return;

	CRect rc, rect;

	GetWindowRect(&rc);
	ScreenToClient(&rc);

	m_info.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom = rc.bottom - GAP;
	m_info.MoveWindow(rect);

	m_list.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom = rc.bottom - GAP;
	m_list.MoveWindow(rect);	
}

void CTrLayout::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{	
	lpMMI->ptMinTrackSize.x = 831;
	lpMMI->ptMinTrackSize.y = 536;

	lpMMI->ptMaxTrackSize.x = 831;
	lpMMI->ptMaxTrackSize.y = 1000;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

bool CTrLayout::ReadTrList(CString path)
{
	CFile	file;
	CString text, tmps;

	if (path.IsEmpty())
		return false;
	
	path.Trim();
	tmps = path;
	int	pos = tmps.ReverseFind(_T('\\'));
	if (pos != -1)
		tmps = tmps.Mid(pos + 1);

	text.Format(_T("%s\\exe\\tmp\\%s"), (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), tmps);
	CopyFile(path, text, FALSE);

	if (file.Open(path, CFile::modeRead))
	{
		UINT	nRead, size = (UINT)file.GetLength();;
		char*	p = new char[size+1];

		nRead = file.Read(p, size);
		if (size != nRead)
		{
			file.Close();
			delete[] p;
			return false;
		}
		file.Close();
		p[size] = '\0';

		text = UTF8ToAnsi(p);

		// remove garbage
		if (text.Find("TR=") != -1)
			text = text.Mid(text.Find("TR="));
		
		makeData(text, tmps);
		return true;
	}
	return false;
}

void CTrLayout::FormObjectLayout(CString info, CString text, bool init)
{
	CString data, gb, key, title;
	bool	isfirst = true;

	if (text.IsEmpty())
	{
		isfirst = false;
		text = m_formObjects;
	}
	else
		m_formObjects = text;
	m_list.DeleteAllItems();

	m_frow = -1;
	gb.Format("%c", GB_SYMBOL);
	while (!text.IsEmpty())
	{
		data = parser(text, gb);
		makeFormData(data);
	}

	if (isfirst && !init)
		addTrLayout(false, info);
	else if (!info.IsEmpty())
	{
		addTrLayout(true, info);
		init = false;
	}

	if (!init)
	{
		if (m_trCB.GetCount() > 0)
		{
			int	 idx  = m_trCB.GetCurSel();
			if (idx < 0)
			{
				idx = 0;
				m_trCB.SetCurSel(idx);
			}
			m_trCB.GetLBText(idx, key);
		}
		initCombo(key, true);
	}
}

void CTrLayout::makeData(CString data, CString fileName)
{
	CString text, tmps;

	m_info.DeleteAllItems();
	m_in.RemoveAll();
	m_out.RemoveAll();
	while (!data.IsEmpty())
	{
		text = parser(data, _T("\n"));
		text.Trim();
		tmps = parser(text, _T("="));
		if (!tmps.CompareNoCase(_T("TR")))
			makeName(fileName);

		if (!tmps.CompareNoCase(_T("DESCRIPTION")) || !tmps.CompareNoCase(_T("PATH")))
			continue;

		if (!tmps.CompareNoCase(_T("FORMAT")))
			makeFormat(text);
		if (!tmps.CompareNoCase(_T("in")))
			m_in.Add(text);
		if (!tmps.CompareNoCase(_T("OUT")))
			m_out.Add(text);
	}

	for (int ii = 0; ii < m_in.GetCount(); ii++)
	{
		text = m_in.GetAt(ii);
		makeIn(text);
	}

	for (int ii = 0; ii < m_out.GetCount(); ii++)
	{
		text = m_out.GetAt(ii);
		makeOut(text);
	}
}

void CTrLayout::makeName(CString trname)
{
	CString tmps;

	if (!trname.IsEmpty())
	{
		tmps.Format("%s : %s", _T("TR Name"), trname);
		((CStatic*)GetDlgItem(IDC_STATIC_NAME))->SetWindowText(tmps);
		initCombo(trname, false);
	}
}

void CTrLayout::makeFormat(CString trformat)
{
	CString tmps;

	if (!trformat.IsEmpty())
	{
		tmps.Format("[%s]", trformat);
		((CStatic*)GetDlgItem(IDC_STATIC_FORMAT))->SetWindowText(tmps);
	}
}


void CTrLayout::makeIn(CString trin)
{
	CString tmps;

	m_row = -1;
	if (m_inoutCB.GetCurSel() == 0 || m_inoutCB.GetCurSel() == 2)
	{
		while (!trin.IsEmpty())
		{
			tmps = parser(trin, _T(","));
			setSymbol(&m_info, _T("I"), tmps);
		}
	}
}

void CTrLayout::makeOut(CString trout)
{
	CString tmps;

	if (m_inoutCB.GetCurSel() == 0 || m_inoutCB.GetCurSel() == 1)
	{
		while (!trout.IsEmpty())
		{
			tmps = parser(trout, _T(","));
			setSymbol(&m_info, _T("O"), tmps);
		}
	}
}

void CTrLayout::makeFormData(CString trsymbol)
{
	CString tmps;
	bool	isgrid = false, isdesc = false;
	int	col = 0;

	m_frow++;
	if (!trsymbol.IsEmpty())
	{
		if (trsymbol.Find(_T("$")) > 0)
		{
			isgrid = true;
			trsymbol.Replace(_T("$"), _T(""));
		}
		
		if (trsymbol.Find(_T("@")) > 0)
		{
			isdesc = true;
			trsymbol.Replace(_T("@"), _T(""));
		}
	}

	if (isgrid || isdesc)
	{
		tmps = parser(trsymbol, _T("="));
		m_list.AddItem(m_frow, col, tmps);
		col += 2;		// symbol = desciption = type 이므로 +2

		tmps = parser(trsymbol, _T(":"));
		m_list.AddItem(m_frow, col, tmps);
		while (!trsymbol.IsEmpty())
		{
			col = 0;
			m_frow++;
			m_list.AddItem(m_frow, col, _T(""));
			col += 2;
			tmps = parser(trsymbol, _T(":"));
			m_list.AddItem(m_frow, col, tmps);
		}
	}
	else
	{
		tmps = parser(trsymbol, _T("="));
		m_list.AddItem(m_frow, col, tmps);
		col += 2;		// symbol = desciption = type 이므로 +2
		m_list.AddItem(m_frow, col, trsymbol);
	}
}

void CTrLayout::makeInOut(CString key)
{
	CString	tmps, trsymbol;
	CString ins, ous;

	if (!m_trMap.Lookup(key, trsymbol))
		return;

	clear();
	ins = parser(trsymbol, CString(GB_XML_INOUT));
	ous = trsymbol;

	while (!ins.IsEmpty())
	{
		tmps = parser(ins, CString(GB_XML_SYMBOL));
		makeInOutSet(0, tmps);
	}

	while (!ous.IsEmpty())
	{
		tmps = parser(ous, CString(GB_XML_SYMBOL));
		makeInOutSet(1, tmps);
	}
}

void CTrLayout::makeInOutSet(int kind, CString tr)
{
	CString tmp, object, symbol, ctmp, ctmp1, description;
	CString duplicate, text;

	tmp = parser(tr, _T("="));
	if (tmp.IsEmpty())
		return;

	int	idx = tmp.Find('.');
	if (idx != -1)
	{
		bool	isGrid = false;
		int	iGridKind = 0;

		text = tmp.Left(idx++);
		object = tmp.Mid(idx);
		if (text[0] == '$')
		{
			isGrid = true;
			m_type = tGrid;
			text = text.Mid(1);
		}
		else if (text[0] == '@')
		{
			isGrid = true;
			m_type = tGridDesc;
			text = text.Mid(1);
		}

		if (m_type == tNo || (m_sGrid.CompareNoCase(text) != 0))
		{
			m_sGrid = text;
			if (!isGrid)
				m_type = tTable;
		}
	}
	else
	{
		object  = tmp;
		m_type = tNo;
	}
	// description 추가
	symbol = parser(tr, "=");
	description = tr;

	bool	matching = false;
	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		ctmp = m_list.GetItemText(ii, 0);
		ctmp1 = m_list.GetItemText(ii, 2);

		if (!ctmp.IsEmpty())
			duplicate = ctmp;
						
		if (!ctmp.CompareNoCase(object) || !ctmp1.CompareNoCase(object))
		{			
			matching = true;
			if (m_sGrid.CompareNoCase(text) != 0)
				m_type = tNo;

			if (ctmp.IsEmpty() && duplicate.CompareNoCase(text))
				continue;

			switch (kind)
			{
			case 0:
				m_list.SetItemText(ii, 1, description);
				m_list.SetItemText(ii, 3, symbol);
				break;
			case 1:
				m_list.SetItemText(ii, 1, description);
				m_list.SetItemText(ii, 4, symbol);
				break;
			}			
		}
	}
	if (!matching)
	{
		int	col = 0;
		idx = m_list.GetItemCount();
		switch (m_type)
		{
		case tNo:
			object.Insert(0, symNONE);
			break;
		case tTable:
			text.Insert(0, symNONE);
			m_list.AddItem(idx, col++, text);
			m_list.AddItem(idx++, ++col, _T("TABLE"));
			m_type = tDone;
			col = 0;
			m_list.AddItem(idx, col++, _T(""));
			break;
		case tGrid:
			text.Insert(0, symNONE);
			m_list.AddItem(idx, col++, text);
			m_list.AddItem(idx++, ++col, _T("GRID"));
			m_type = tDone;
			col = 0;
			m_list.AddItem(idx, col++, _T(""));
			break;
		case tGridDesc:
			text.Insert(0, symNONE);
			m_list.AddItem(idx, col++, text);
			m_list.AddItem(idx++, ++col, _T("GRID"));
			m_type = tDone;
			col = 0;
			m_list.AddItem(idx, col++, _T(""));
			break;
		default:
			m_list.AddItem(idx, col++, _T(""));
			break;
		}
		m_list.AddItem(idx, ++col, object);
		m_list.AddItem(idx, kind ? 4 : 3, symbol);
	}
}

void CTrLayout::addTrLayout(bool init, CString data)
{
	CString key, tr, tmp;

	if (!init)
	{
		m_trMap.RemoveAll();
		clear(true);
	}
	while (!data.IsEmpty())
	{
		tr = parser(data, CString(GB_XML_SEQ));
		key = parser(tr, _T("\t"));

		key.Trim();
		key.MakeUpper();
		if (!m_trMap.Lookup(key, tmp))
		{
			m_trMap.SetAt(key, tr);
			if (!checkCombo(key))
				m_trCB.AddString(key);
		}
	}
}

bool CTrLayout::checkCombo(CString key)
{
	bool	add = false;
	CString tmps;

	for (int ii = 0; ii < m_trCB.GetCount(); ii++)
	{
		m_trCB.GetLBText(ii, tmps);
		tmps.Trim();
		if (!tmps.CompareNoCase(key))
		{
			add = true;
			break;
		}
	}
	return add;
}

void CTrLayout::initCombo(CString sel, bool find)
{
	CString	tmps;

	if (find)
	{
		makeInOut(sel);
		return;
	}

	for (int ii = 0; ii < m_trCB.GetCount(); ii++)
	{
		m_trCB.GetLBText(ii, tmps);
		if (!sel.CompareNoCase(tmps))
		{
			m_trCB.SetCurSel(ii);
			makeInOut(tmps);
			return;
		}
	}
	m_trCB.SetCurSel(m_trCB.AddString(sel));
	makeInOut(sel);
}

void CTrLayout::setMapH()
{
	CString trinfo = _T(""), sKey, str, tmps;

	for (POSITION pos = m_trMap.GetStartPosition(); pos != NULL; )
	{
		m_trMap.GetNextAssoc(pos, sKey, str);
		tmps.Format(_T("%s\t%s"), sKey, str);
		trinfo += tmps; 
		trinfo += CString(GB_XML_SEQ);
	}
	m_parent->SendMessage(WM_USER, MAKEWPARAM(ID_USR_DATASET, 0), (LPARAM)trinfo.operator LPCSTR());
}

void CTrLayout::clear(bool combo)
{
	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		m_list.SetItemText(ii, 1, _T(""));
		m_list.SetItemText(ii, 3, _T(""));
		m_list.SetItemText(ii, 4, _T(""));
	}

	if (combo)
	{
		for (int ii = m_trCB.GetCount() - 1; ii >= 0; ii--)
			m_trCB.DeleteString(ii);
	}
}

CString CTrLayout::findGridParent(int row)
{
	CString tmps;

	for (int ii = row; ii >= 0; ii--)
	{
		tmps = m_info.GetItemText(ii, 0);
		if (tmps.Find(_T("+")) != -1)
		{
			tmps.Replace(_T("+"), _T(""));
			tmps.Trim();
			break;
		}
	}
	return tmps;
}

void CTrLayout::setSymbol(CGridListCtrl* list, CString inout, CString trsymbol, bool grid, bool gridDesc, CString grids)
{
	CString tmp, tmps, gridsymbol;
	bool	bgrid = false, bgridDesc = false;
	bool	bskip = false;
	bool	bgridch = false;
	int	col = 0;

	if (!trsymbol.IsEmpty())
	{
		if (trsymbol.GetAt(0) == '$')
		{
			bgrid = true;
			bskip = true;
			trsymbol = trsymbol.Mid(1);
			tmp = trsymbol;
			tmps = parser(tmp, _T(":"));
			gridsymbol = tmps;
		}
		else if (trsymbol.GetAt(0) == '@')
		{
			bgrid = true;
			bgridDesc = true;
			bskip = true;
			trsymbol = trsymbol.Mid(1);
			tmp = trsymbol;
			tmps = parser(tmp, _T(":"));
			gridsymbol = tmps;
		}

		if (grid)
		{
			bgrid = true;
			if (gridDesc)
				bgridDesc = true;
			gridsymbol = grids;			
		}
	}


	while (!trsymbol.IsEmpty())
	{
		col = 0;
		m_row++;

		tmp = parser(trsymbol, _T(":"));
		if (bgrid && tmp.CompareNoCase(gridsymbol))
		{
			int len = gridsymbol.GetLength() * 2 + 2;
			CString tmpx;
			for (int ii = 0; ii < len; ii++)
				tmpx += _T(" ");
			tmps.Format(_T("%s-%s"), tmpx, tmp);
			tmp = tmps;
			bgridch = true;
		}

		if (bgrid)
		{	
			if (bgridch)
				tmps.Format(_T("%s"), tmp);
			else
				tmps.Format(_T("+%s"), tmp);
			list->AddItem(m_row, col, + tmps);
		}
		else
			list->AddItem(m_row, col, tmp);
		col++;

		// description 추가
		int	descCol = col++;

		if (bskip)
			col++;

		tmp = parser(trsymbol, _T(":"));
		list->AddItem(m_row, col, tmp);

		if (bskip)
		{
			if (bgridDesc)
			{
				tmp = parser(trsymbol, _T(":"));
				list->AddItem(m_row, descCol, tmp);
			}
			bskip = false;
			list->AddItem(m_row, 4, inout);
			continue;
		}

		col++;
		tmp = parser(trsymbol, _T(":"));
		list->AddItem(m_row, col, tmp);

		col++;
		list->AddItem(m_row, col, inout);

		if (!bgrid || bgridDesc)
		{
			tmp = parser(trsymbol, _T(":"));
			list->AddItem(m_row, descCol, tmp);
		}

		if (bgrid)
		{
			setSymbol(list, inout, trsymbol, bgrid, bgridDesc, gridsymbol);
			trsymbol = trsymbol.Mid(trsymbol.GetLength());
		}
	}
}

CString CTrLayout::parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}

	CString  temp = srcstr.Left(srcstr.Find(substr));
	srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	return temp;
}

void CTrLayout::setFont()
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

CString CTrLayout::UTF8ToAnsi(char* pszUTF8)
{	
	int	nU8StrLen, nBStrLen, nAStrLen;
	BSTR	bstr;
	CString	str;
	char*	pszANSI;
 
	nU8StrLen = lstrlen(pszUTF8);
	nBStrLen = MultiByteToWideChar(CP_UTF8, 0, pszUTF8, nU8StrLen + 1, NULL, NULL);
	bstr = SysAllocStringLen(NULL, nBStrLen);
	MultiByteToWideChar(CP_UTF8, 0, pszUTF8, nU8StrLen + 1, bstr, nBStrLen);
 
	nAStrLen = WideCharToMultiByte(CP_ACP, 0, bstr, -1, NULL, 0, NULL, NULL);
	pszANSI = new char[nAStrLen];
	WideCharToMultiByte(CP_ACP, 0, bstr, -1, pszANSI, nAStrLen, NULL, NULL);
	str = (CString)bstr;
	SysFreeString(bstr);
	delete [] pszANSI;

	return str;
}

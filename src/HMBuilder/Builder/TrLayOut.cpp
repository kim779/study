// TrLayOut.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "TrLayOut.h"
#include "afxdialogex.h"
#include "MainFrm.h"

#include "h/mapform.h"
#include "h/mainvar.h"
#include "amCc/LibBuild.h"

#define symNONE		_T("**")
#define GAP		17

// CTrLayOut 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTrLayOut, CDialogEx)

CTrLayOut::CTrLayOut(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrLayOut::IDD, pParent)
{
	m_pParent = pParent;
	m_trname = _T("");
	m_trtype = _T("");
	m_trformat = _T("");
	m_ntrformat = -1;
	m_trin.RemoveAll();
	m_trout.RemoveAll();
	m_row = -1;
	m_frow = -1;
	m_bsave = false;
	m_CMaptr.RemoveAll();
	m_serverTR = _T("");
	m_disciptionTR = _T("");
	m_tGrid = t_NONE;
}

CTrLayOut::~CTrLayOut()
{
	m_CMaptr.RemoveAll();
	m_trin.RemoveAll();
	m_trout.RemoveAll();
}

void CTrLayOut::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INOUT, m_inoutcombo);
	DDX_Control(pDX, IDC_FOMRTR, m_formTr);
}


BEGIN_MESSAGE_MAP(CTrLayOut, CDialogEx)
	ON_BN_CLICKED(IDC_SAVE, &CTrLayOut::OnBnSave)
	ON_BN_CLICKED(IDC_CLEAR, &CTrLayOut::OnBnClickedClear)
	ON_BN_CLICKED(IDC_RESET, &CTrLayOut::OnBnClickedReset)
	ON_BN_CLICKED(IDOK, &CTrLayOut::OnBnClickedOk)
	ON_BN_CLICKED(IDC_NEW, &CTrLayOut::OnBnClickedNew)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CTrLayOut::OnBnClickedBtnDelete)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CTrLayOut 메시지 처리기입니다.


BOOL CTrLayOut::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect	rc, rect;

	GetWindowRect(&rc);
	ScreenToClient(&rc);
	
	((CStatic*)GetDlgItem(IDC_STATIC_SERVER))->GetWindowRect(&rect);
	ScreenToClient(rect);
	rect.bottom = rc.bottom - GAP; rect.left = rc.left + GAP;
	if (!m_serverlist.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LVS_REPORT | LVS_ALIGNTOP | LVS_NOSORTHEADER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, this, IDC_LIST_SERVERIN))
		return -1;

	((CStatic*)GetDlgItem(IDC_STATIC_USER))->GetWindowRect(&rect);
	ScreenToClient(rect);
	rect.bottom = rc.bottom - GAP; rect.right = rc.right - GAP;
	if (!m_formlist.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LVS_REPORT | LVS_ALIGNTOP | LVS_NOSORTHEADER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, this, IDC_LIST_FORM))
		return -1;

	

	m_serverlist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE);
	m_serverlist.InsertColumn(0, _T("Symbol"), LVCFMT_CENTER, 80);
	m_serverlist.InsertColumn(1, _T("Description"), LVCFMT_CENTER, 90);
	m_serverlist.InsertColumn(2, _T("Type"), LVCFMT_CENTER, 65);
	m_serverlist.InsertColumn(3, _T("Size"), LVCFMT_CENTER, 65);
	m_serverlist.InsertColumn(4, _T("I/O"), LVCFMT_CENTER, 45);

	m_formlist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE);
	m_formlist.InsertColumn(0, _T("Object Name"), LVCFMT_CENTER, 85);
	m_formlist.InsertColumn(1, _T("Description"), LVCFMT_CENTER, 90);
	m_formlist.InsertColumn(2, _T("Type"), LVCFMT_CENTER, 65);
	m_formlist.InsertColumn(3, _T("Input"), LVCFMT_CENTER, 80);
	m_formlist.InsertColumn(4, _T("Output"), LVCFMT_CENTER, 80);

	m_inoutcombo.InsertString(0, _T("All"));
	m_inoutcombo.InsertString(1, _T("Output"));
	m_inoutcombo.InsertString(2, _T("Input"));
	m_inoutcombo.SetCurSel(0);

	setFont();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

#define FILE_SIZE	128
bool CTrLayOut::readTrList(CString path)
{
	CFile	file;
	CString sData;
	CString tmp, tmps;

	//char ch[1024 * 32];
	//ZeroMemory(ch, 1024 * 32);

	char ch[1024 * FILE_SIZE];
	ZeroMemory(ch, 1024 * FILE_SIZE);

	if (path.IsEmpty())
		return false;
	
	tmps = path;
	int pos = tmps.ReverseFind(_T('\\'));
	if (pos != -1)
		tmps = tmps.Mid(pos + 1);
		
	tmp.Format(_T("%s\\exe\\tmp\\%s"), (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), tmps);
	CopyFile(path, tmp, FALSE);
	/*
	if (file.Open(path, CFile::modeRead))
	{
		UINT	nRead;
		char	wb[1024 * 32];

		nRead = file.Read(wb, sizeof(wb)-1);
		if (nRead)
		{
			wb[nRead] = '\0';
			sData = CString(wb, nRead);
		}
		file.Close();
	}
	*/
	path.Trim();
	if (file.Open(path, CFile::modeRead))
	{
		UINT	nRead;
		int	len  = 1024 * FILE_SIZE;

		nRead = file.Read(ch, len - 1);
		//nRead = file.Read(sData.GetBuffer(len), len - 1);
		//sData.ReleaseBuffer();
		sData = ((CMainFrame *)m_pParent)->UTF8ToAnsi(ch);

		// 불순물 제거
		if (sData.Find("TR=") > -1)
			sData = sData.Mid(sData.Find("TR="));
		
		if (nRead)		
			sData = sData.Left(nRead);
		
		file.Close();
	}
	makeData(sData, tmps);

	return false;
}

void CTrLayOut::makeData(CString data, CString fileName)
{
	CString tmp, tmps, xtmp;

	m_serverlist.DeleteAllItems();
	m_trin.RemoveAll();
	m_trout.RemoveAll();

	while (!data.IsEmpty())
	{
		tmp = Parser(data, _T("\n"));
		xtmp = tmp;
		tmps = Parser(tmp, _T("="));
		if (!tmps.CompareNoCase(_T("TR")))
		{
			fileName.Trim();
			xtmp.Format("TR=%s", fileName);
			makeTrname(xtmp);
		}
		if (!tmps.CompareNoCase(_T("DESCRIPTION")))
			continue;
		if (!tmps.CompareNoCase(_T("PATH")))
			makeTrtype(xtmp);
		if (!tmps.CompareNoCase(_T("FORMAT")))
			makeTrformat(xtmp);
		if (!tmps.CompareNoCase(_T("in")))
			m_trin.Add(xtmp);
		if (!tmps.CompareNoCase(_T("OUT")))
			m_trout.Add(xtmp);
	}

	for (int ii = 0; ii < m_trin.GetCount(); ii++)
	{
		tmp = m_trin.GetAt(ii);
		makeTrin(tmp);
	}

	for (int jj = 0; jj < m_trout.GetCount(); jj++)
	{
		tmp = m_trout.GetAt(jj);
		makeTrout(tmp);
	}

}

CString CTrLayOut::Parser(CString &srcstr, CString substr)
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

void CTrLayOut::makeTrname(CString trname)
{
	CString tmp;

	if (!trname.IsEmpty())
		trname = trname.Left(trname.GetLength());//- 1);
	m_trname = trname;
	Parser(trname, _T("="));

	if (!trname.IsEmpty())
	{
		tmp.Format("%s : %s", _T("TR Name"), trname);
		((CStatic*)GetDlgItem(IDC_STATIC_NAME))->SetWindowText(tmp);
		initcombo(trname, false);
	}
}

void CTrLayOut::initcombo(CString sel, bool bfind)
{
	CString	tmp;

	if (bfind)
	{
		makeforminout(sel);
		return;
	}
	for (int ii = 0; ii < m_formTr.GetCount(); ii++)
	{
		m_formTr.GetLBText(ii, tmp);
		if (!sel.CompareNoCase(tmp))
		{
			m_formTr.SetCurSel(ii);
			makeforminout(tmp);
			return;
		}
	}

	m_formTr.SetCurSel(m_formTr.AddString(sel));
	makeforminout(sel);
}

void CTrLayOut::makeTrtype(CString trkind)
{
	if (!trkind.IsEmpty())
		trkind = trkind.Left(trkind.GetLength() - 1);
	m_trtype = trkind;
	Parser(trkind, _T("="));
}

void CTrLayOut::makeTrformat(CString trformat)
{
	CString tmp;

	if (!trformat.IsEmpty())
		trformat = trformat.Left(trformat.GetLength() - 1);
	m_trformat = trformat;
	Parser(trformat, _T("="));

	if (!trformat.IsEmpty())
	{
		trformat.TrimLeft();
		trformat.TrimRight();
	}

	if (!trformat.CompareNoCase(_T("STRUCTURE")))
		m_ntrformat = TR_TYPE_STRUCTURE;
	if (!trformat.CompareNoCase(_T("BINARY")))
		m_ntrformat = TR_TYPE_BINARY;
	if (!trformat.CompareNoCase(_T("TAB")))
		m_ntrformat = TR_TYPE_TAB;
	if (!trformat.CompareNoCase(_T("FID")))
		m_ntrformat = TR_TYPE_FID;

	if (!trformat.IsEmpty())
	{
		tmp.Format("[%s]", trformat);
		((CStatic*)GetDlgItem(IDC_STATIC_FORMAT))->SetWindowText(tmp);
	}
}

void CTrLayOut::makeTrin(CString trin)
{
	CString tmp;
	
	if (!trin.IsEmpty())
		trin = trin.Left(trin.GetLength() - 1);

	Parser(trin, _T("="));
	m_row = -1;

	if (m_inoutcombo.GetCurSel() == 0 || m_inoutcombo.GetCurSel() == 2)
	{
		while (!trin.IsEmpty())
		{
			tmp = Parser(trin, _T(","));
			setsymbol(&m_serverlist, _T("I"), tmp);
		}
	}
}

void CTrLayOut::makeTrout(CString trout)
{
	CString tmp;

	//if (!trout.IsEmpty())
	//	trout = trout.Left(trout.GetLength() - 1);

	Parser(trout, _T("="));

	switch (m_ntrformat)
	{
	case TR_TYPE_STRUCTURE:
	case TR_TYPE_BINARY:
	case TR_TYPE_TAB:
	case TR_TYPE_FID:
	default:
		if (m_inoutcombo.GetCurSel() == 0 || m_inoutcombo.GetCurSel() == 1)
		{
			while (!trout.IsEmpty())
			{
				tmp = Parser(trout, _T(","));
				setsymbol(&m_serverlist, _T("O"), tmp);
			}
		}
		break;
	}
}

void CTrLayOut::setsymbol(CTrListctrl* list, CString inout, CString trsymbol, bool grid, bool gridDesc, CString grids)
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
			tmps = Parser(tmp, _T(":"));
			gridsymbol = tmps;
		}
		else if (trsymbol.GetAt(0) == '@')
		{
			bgrid = true;
			bgridDesc = true;
			bskip = true;
			trsymbol = trsymbol.Mid(1);
			tmp = trsymbol;
			tmps = Parser(tmp, _T(":"));
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

		tmp = Parser(trsymbol, _T(":"));

		if (bgrid && tmp.CompareNoCase(gridsymbol))
		{
			int len = gridsymbol.GetLength() * 2 + 2;
			CString tmpx;
			for (int zz = 0; zz < len; zz++)
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

		// TR Discription 추가
		int discCol = col++;

		if (bskip)
			col++;

		tmp = Parser(trsymbol, _T(":"));
		list->AddItem(m_row, col, tmp);

		if (bskip)
		{
			if (bgridDesc)
			{
				tmp = Parser(trsymbol, _T(":"));
				list->AddItem(m_row, discCol, tmp);
			}
			bskip = false;
			list->AddItem(m_row, 4, inout);
			continue;
		}

		col++;
		tmp = Parser(trsymbol, _T(":"));
		list->AddItem(m_row, col, tmp);

		col++;
		list->AddItem(m_row, col, inout);

		if (!bgrid || bgridDesc)
		{
			tmp = Parser(trsymbol, _T(":"));
			list->AddItem(m_row, discCol, tmp);
		}

		if (bgrid)
		{
			setsymbol(list, inout, trsymbol, bgrid, bgridDesc, gridsymbol);
			trsymbol = trsymbol.Mid(trsymbol.GetLength());
		}
	}
}

void CTrLayOut::setFont()
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

	m_serverlist.SetFont(&m_font);
	m_formlist.SetFont(&m_font);
}

BOOL CTrLayOut::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_INOUT:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				m_serverlist.DeleteAllItems();
				CString tmp;
				int index = m_inoutcombo.GetCurSel();
				m_row = -1;

				switch (index)
				{
				case 2:
					for (int ii = 0; ii < m_trin.GetCount(); ii++)
					{
						tmp = m_trin.GetAt(ii);
						makeTrin(tmp);
					}
					break;
				case 1:
					for (int jj = 0; jj < m_trout.GetCount(); jj++)
					{
						tmp = m_trout.GetAt(jj);
						makeTrout(tmp);
					}
					break;
				case 0:
					for (int ii = 0; ii < m_trin.GetCount(); ii++)
					{
						tmp = m_trin.GetAt(ii);
						makeTrin(tmp);
					}
					for (int jj = 0; jj < m_trout.GetCount(); jj++)
					{
						tmp = m_trout.GetAt(jj);
						makeTrout(tmp);
					}
				default:
					break;
				}
			}
		}
		break;
	case IDC_FOMRTR:
		switch (HIWORD(wParam))
		{
		case CBN_SELCHANGE:
			{
				CString path, tmp;
				m_formTr.GetLBText(m_formTr.GetCurSel(), tmp);
				path.Format("%s\\trlayout\\%s", ((CMainFrame*)m_pParent)->m_root, tmp);
				readTrList(path);
				formObjectLayout();
			}
			break;
		}
		break;
	default:
		break;
	}
	return CDialogEx::OnCommand(wParam, lParam);
}

BOOL CTrLayOut::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	LPNMHDR pHdr;
	LPNMLISTVIEW pNlv;

	pHdr = (LPNMHDR)lParam;
	pNlv = (LPNMLISTVIEW)lParam;

	switch (LOWORD(wParam))
	{
	
	case IDC_LIST_SERVERIN:
		{
			switch(pHdr->code)
			{
			case NM_CLICK:
				{
					int row, col;
					CString tmp, tmps, tmpDiscription;
					bool bgrid = false;

					row = m_serverlist.m_FocusRow;
					col = m_serverlist.m_FocusCol;
					m_serverTR = _T("");
					m_disciptionTR = _T("");

					tmp = m_serverlist.GetItemText(row, 0);
					tmpDiscription = m_serverlist.GetItemText(row, 1);

					if (!tmp.IsEmpty())
					{
						tmp.TrimLeft();
						tmp.TrimRight();

						if (tmp.Find(_T("-")) != -1)
						{
							tmp.Replace(_T("-"), _T(""));
							tmps = findgridparent(row);
							bgrid = true;
						}
					}

					m_serverTR = tmp;
					m_disciptionTR = tmpDiscription;

					if (bgrid)
						m_serverTR.Format(_T("%s.%s"), tmps, m_serverTR);

					if (!m_serverlist.GetItemText(row, 4).CompareNoCase(_T("I")))
						m_selKind = INPUT;
					else if (!m_serverlist.GetItemText(row, 4).CompareNoCase(_T("O")))
						m_selKind = OUTPUT;
					else
						m_selKind = NONE;
				}
				break;
			}
		}
		break;
	case IDC_LIST_FORM:
		{
			switch(pHdr->code)
			{
			case NM_CLICK:
				{
					int row, col;
					CString tmp;

					row = m_formlist.m_FocusRow;
					col = m_formlist.m_FocusCol;

					switch (m_selKind)
					{
					case INPUT:
						m_formlist.SetItemText(row, 1, m_disciptionTR);
						m_formlist.SetItemText(row, 3, m_serverTR);
						break;
					case OUTPUT:
						m_formlist.SetItemText(row, 1, m_disciptionTR);
						m_formlist.SetItemText(row, 4, m_serverTR);
						break;
					case NONE:
					default:
						break;
					}

					m_serverTR = _T("");
					m_selKind = NONE;
					m_bsave = true;
				}
				break;
			case NM_DBLCLK:
				{
					int row, col;
					row = m_formlist.m_FocusRow;
					col = m_formlist.m_FocusCol;

					if (col == 3 || col == 4)
					{
						if (!m_formlist.GetItemText(row, col).IsEmpty())
						{
							m_formlist.SetItemText(row, 1, _T(""));
							m_formlist.SetItemText(row, col, _T(""));
						}
					}
				}
				break;
			}
		}
		break;
	}
	return CDialogEx::OnNotify(wParam, lParam, pResult);
}

CString CTrLayOut::findgridparent(int row)
{
	CString tmp = _T("");

	for (int ii = row; ii >= 0; ii--)
	{
		tmp = m_serverlist.GetItemText(ii, 0);

		if (tmp.Find(_T("+")) != -1)
		{
			tmp.Replace(_T("+"), _T(""));
			tmp.TrimLeft();
			tmp.TrimRight();
			break;
		}
	}

	return tmp;
}

void CTrLayOut::formObjectLayout(struct _mapH* mapH, CString tmp, bool binit)
{
	CString data, gb, key, title, trinfo = _T("");
	bool	isFirst = true;

	if (mapH == NULL)// || mapH->trinfo == NULL)
		return;

	if (mapH->trinfo != NULL)
		trinfo = CString(mapH->trinfo, strlen(mapH->trinfo));
	//title.Format(_T("Transaction Layout_%s"), CString(mapH->mapN, L_MAPN));
	//SetWindowText(title);

	if (tmp.IsEmpty())
	{
		isFirst = false;
		tmp = m_formObjects;
	}
	else
		m_formObjects = tmp;
	m_formlist.DeleteAllItems();
	m_bsave = false;

	m_frow = -1;
	gb.Format("%c", GB_SYMBOL);
	while (!tmp.IsEmpty())
	{
		data = Parser(tmp, gb);
		makeformdata(data);
	}

	if (isFirst && !binit)
	{
		/*CString tr;

		m_CMaptr.RemoveAll();
		Cleardata(true);

		tmp = trinfo;//getTrLayOut();
TRACE("-------[formObjectLayout]-----------%s\n", tmp);
		while (!tmp.IsEmpty())
		{
			tr = Parser(tmp, _T("^"));
			key = Parser(tr, _T("\t"));
			m_CMaptr.SetAt(key, tr);
			m_formTr.AddString(key);
		}*/
		addTrlayout(false, trinfo);
	}
	else if (!trinfo.IsEmpty())
	{
		addTrlayout(true, trinfo);
		binit = false;
	}

	if (!binit)
	{
		if (m_formTr.GetCount() > 0)
		{
			int idx  = m_formTr.GetCurSel();
			if (idx < 0)
			{
				idx = 0;
				m_formTr.SetCurSel(idx);
			}
			m_formTr.GetLBText(idx, key);
		}

		initcombo(key, true);
	}
}

void CTrLayOut::addTrlayout(bool binit, CString data)
{
	CString key, tr, tmp;

	if (!binit)
	{
		m_CMaptr.RemoveAll();
		Cleardata(true);
	}
	while (!data.IsEmpty())
	{
		tr = Parser(data, _T("^"));
		key = Parser(tr, _T("\t"));

		key.Trim();

		if (m_CMaptr.Lookup(key, tmp))
			continue;
		m_CMaptr.SetAt(key, tr);
		if (!checkComboname(key))
			m_formTr.AddString(key);
	}
}

void CTrLayOut::makeforminout(CString key)
{
	CString	tmp, trsymbol;
	CString infm, outfm;

	if (!m_CMaptr.Lookup(key, trsymbol))
		return;

	Cleardata();

	infm = Parser(trsymbol, _T("*"));
	outfm = trsymbol;

	while (!infm.IsEmpty())
	{
		tmp = Parser(infm, _T("/"));
		makeforminoutset(0, tmp);
	}

	while (!outfm.IsEmpty())
	{
		tmp = Parser(outfm, _T("/"));
		//TRACE("[%s]\n", tmp);
		makeforminoutset(1, tmp);
	}
}

void CTrLayOut::makeforminoutset(int kind, CString tr)
{
	CString tmp, object, symbol, ctmp, ctmp1, description;
	CString duplicate;

	tmp = Parser(tr, _T("="));

	if (tmp.IsEmpty())
		return;

	CString	text;

	int	idx = tmp.Find('.');
	if (idx != -1)
	{
		bool	isGrid = false;
		int iGridKind = 0;

		text = tmp.Left(idx++);
		object = tmp.Mid(idx);
		if (text[0] == '$')
		{
			isGrid = true;
			m_tGrid = t_GRID;
			text = text.Mid(1);
		}
		else if (text[0] == '@')
		{
			isGrid = true;
			m_tGrid = t_GRIDDESC;
			text = text.Mid(1);
		}

		if (m_tGrid == t_NONE || (m_sGrid.CompareNoCase(text) != 0))
		{
			m_sGrid = text;
			if (!isGrid)
				m_tGrid = t_TABLE;
		}
	}
	else
	{
		object  = tmp;
		m_tGrid = t_NONE;
	}
	// Description 추가
	symbol = Parser(tr, "=");
	description = tr;

	bool	matching = false;
	for (int ii = 0; ii < m_formlist.GetItemCount(); ii++)
	{
		ctmp = m_formlist.GetItemText(ii, 0);
		ctmp1 = m_formlist.GetItemText(ii, 2);

		if (!ctmp.IsEmpty())
			duplicate = ctmp;
						
		if (!ctmp.CompareNoCase(object) || !ctmp1.CompareNoCase(object))
		{			
			matching = true;
			if (m_sGrid.CompareNoCase(text) != 0)
				m_tGrid = t_NONE;

			if (ctmp.IsEmpty() && duplicate.CompareNoCase(text))
				continue;

			switch (kind)
			{
			case 0:
				m_formlist.SetItemText(ii, 1, description);
				m_formlist.SetItemText(ii, 3, symbol);
				break;
			case 1:
				m_formlist.SetItemText(ii, 1, description);
				m_formlist.SetItemText(ii, 4, symbol);
				break;
			}			
		}
	}
	if (!matching)
	{
		int	col = 0;
		idx = m_formlist.GetItemCount();
		switch (m_tGrid)
		{
		case t_NONE:
			object.Insert(0, symNONE);
			break;
		case t_TABLE:
			text.Insert(0, symNONE);
			m_formlist.AddItem(idx, col++, text);
			m_formlist.AddItem(idx++, ++col, _T("TABLE"));
			m_tGrid = t_DONE;
			col = 0;
			m_formlist.AddItem(idx, col++, _T(""));
			break;
		case t_GRID:
			text.Insert(0, symNONE);
			m_formlist.AddItem(idx, col++, text);
			m_formlist.AddItem(idx++, ++col, _T("GRID"));
			m_tGrid = t_DONE;
			col = 0;
			m_formlist.AddItem(idx, col++, _T(""));
			break;
		case t_GRIDDESC:
			text.Insert(0, symNONE);
			m_formlist.AddItem(idx, col++, text);
			m_formlist.AddItem(idx++, ++col, _T("GRID"));
			m_tGrid = t_DONE;
			col = 0;
			m_formlist.AddItem(idx, col++, _T(""));
			break;
		default:
			m_formlist.AddItem(idx, col++, _T(""));
			break;
		}
		m_formlist.AddItem(idx, ++col, object);
		m_formlist.AddItem(idx, kind ? 4 : 3, symbol);
	}
}

// TR Discription추가로 col 변경
void CTrLayOut::makeformdata(CString trsymbol)
{
	CString tmp;
	bool	bgrid = false, bgridDescription = false;
	int	col = 0;

	m_frow++;

	if (!trsymbol.IsEmpty())
	{
		if (trsymbol.Find(_T("$")) > 0)
		{
			bgrid = true;
			trsymbol.Replace(_T("$"), _T(""));
		}
		
		if (trsymbol.Find(_T("@")) > 0)
		{
			bgridDescription = true;
			trsymbol.Replace(_T("@"), _T(""));
		}
	}

	if (bgrid || bgridDescription)
	{
		col = 0;
		tmp = Parser(trsymbol, _T("="));
		m_formlist.AddItem(m_frow, col, tmp);
		col+=2;		// symbol = disciption = type 이므로 +2
		tmp = Parser(trsymbol, _T(":"));
		m_formlist.AddItem(m_frow, col, tmp);
		while (!trsymbol.IsEmpty())
		{
			col = 0;
			m_frow++;
			m_formlist.AddItem(m_frow, col, _T(""));
			col+=2;
			tmp = Parser(trsymbol, _T(":"));
			m_formlist.AddItem(m_frow, col, tmp);
		}
	}
	else
	{
		col = 0;
		tmp = Parser(trsymbol, _T("="));
		m_formlist.AddItem(m_frow, col, tmp);
		col+=2;		// symbol = disciption = type 이므로 +2
		m_formlist.AddItem(m_frow, col, trsymbol);
	}
	
}

void CTrLayOut::OnBnSave()
{
	int pos, cnt;
	CString	combo, tmp, tmps, symbol, tablesymbol, in, out, xml, xmltmp, desc;
	CString type, gridsym, key;
	bool bgrid = false;
	bool btable = false;

	cnt = m_formlist.GetItemCount();
	pos = m_formTr.GetCurSel();

	if (cnt <= 0 || pos < 0 || m_formTr.GetCount() <= 0)
		return;
	m_formTr.GetLBText(pos, combo);

	tmp.Format(_T("%s%c%s"), _T("TRINFO"), GB_SYMBOL, combo);
	for (int ii = 0; ii < cnt; ii++)
	{
		symbol = m_formlist.GetItemText(ii, 0);
		if (symbol.Find(symNONE) == 0)
			symbol.Replace(symNONE, "");

		type = m_formlist.GetItemText(ii, 2);
		if (!type.CompareNoCase(_T("GRID")))
		{
			gridsym = symbol;
			bgrid = true;
			btable = false;
			continue;

		}
		else if (!type.CompareNoCase(_T("GRIDEX")))
		{
			gridsym = symbol;
			bgrid = true;
			btable = false;
			continue;

		}
		else if (!type.CompareNoCase(_T("TABLE")))
		{
			bgrid = false;
			btable = true;
			tablesymbol = symbol;
			continue;
		}
		else if (!type.CompareNoCase(_T("OUTPUT")) || !type.CompareNoCase(_T("EDIT")) ||
			 !type.CompareNoCase(_T("COMBO")) || !type.CompareNoCase(_T("CUSTOM")))
		{
			bgrid = false;
			btable = false;
		}

		if (bgrid || btable)
			symbol = m_formlist.GetItemText(ii, 2);

		in = m_formlist.GetItemText(ii, 3);
		desc = m_formlist.GetItemText(ii, 1);
		if (!in.IsEmpty())
		{
			if (bgrid)
			{
				tmps.Format(_T("%c$%s.%s=%s=%s"), GB_SYMBOL, gridsym, symbol, in, desc);
				xmltmp.Format(_T("%c$%s.%s=%s=%s"), GB_XML_SYMBOL, gridsym, symbol, in, desc);
			}
			else
			{
				if (btable)
				{
					tmps.Format(_T("%c%s.%s=%s=%s"), GB_SYMBOL, tablesymbol, symbol, in, desc);
					xmltmp.Format(_T("%c%s.%s=%s=%s"), GB_XML_SYMBOL, tablesymbol, symbol, in, desc);
				}
				else
				{
					tmps.Format(_T("%c%s=%s=%s"), GB_SYMBOL, symbol, in, desc);
					xmltmp.Format(_T("%c%s=%s=%s"), GB_XML_SYMBOL, symbol, in, desc);
				}
			}
			tmp += tmps;
			xml += xmltmp;
		}
	}

	bgrid = false;
	btable = false;

	tmps.Format(_T("%c"), GB_INOUT);
	xmltmp.Format(_T("%c"), GB_XML_INOUT);
	tmp += tmps;
	xml += xmltmp;

	for (int ii = 0; ii < cnt; ii++)
	{
		symbol = m_formlist.GetItemText(ii, 0);
		if (symbol.Find(symNONE) == 0)
			symbol.Replace(symNONE, "");

		type = m_formlist.GetItemText(ii, 2);
		if (!type.CompareNoCase(_T("GRID")))
		{
			gridsym = symbol;
			bgrid = true;
			btable = false;
			continue;
		}
		if (!type.CompareNoCase(_T("GRIDEX")))
		{
			gridsym = symbol;
			bgrid = true;
			btable = false;
			continue;
		}
		else if (!type.CompareNoCase(_T("TABLE")))
		{
			bgrid = false;
			btable = true;
			tablesymbol = symbol;
			continue;
		}
		else if (!type.CompareNoCase(_T("OUTPUT")) || !type.CompareNoCase(_T("EDIT")) ||
				 !type.CompareNoCase(_T("COMBO")) || !type.CompareNoCase(_T("CUSTOM")))
		{
			bgrid = false;
			btable = false;
		}

		if (bgrid || btable)
			symbol = m_formlist.GetItemText(ii, 2);

		out = m_formlist.GetItemText(ii, 4);
		desc = m_formlist.GetItemText(ii, 1);
		if (!out.IsEmpty())
		{
			if (bgrid)
			{
				tmps.Format(_T("$%s.%s=%s=%s%c"), gridsym, symbol, out, desc, GB_SYMBOL);
				xmltmp.Format(_T("$%s.%s=%s=%s%c"), gridsym, symbol, out, desc, GB_XML_SYMBOL);
			}
			else
			{
				if (btable)
				{
					tmps.Format(_T("%s.%s=%s=%s%c"), tablesymbol, symbol, out, desc, GB_SYMBOL);
					xmltmp.Format(_T("%s.%s=%s=%s%c"), tablesymbol, symbol, out, desc, GB_XML_SYMBOL);
				}
				else
				{
					tmps.Format(_T("%s=%s=%s%c"), symbol, out, desc, GB_SYMBOL);
					xmltmp.Format(_T("%s=%s=%s%c"), symbol, out, desc, GB_XML_SYMBOL);
				}
			}
			tmp += tmps;
			xml += xmltmp;
		}
	}

	tmps.Format(_T("%c"), GB_TR);
	tmp += tmps;
	//xmltmp.Format(_T("%c"), GB_XML_TYPE);
	//xml += xmltmp;

	//CString trlayout = _T("");
	//trlayout.Format(_T("%s\t%s"), combo, tmp);
	//TRACE("------------------%s\n", trlayout);
	//wccTrLayOut(0, trlayout);
	//trlayout.Format(_T("%s\t%s"), combo, xml);
	//wccTrLayOut(1, trlayout);
	//TRACE("------------------%s\n", trlayout);
	if (m_CMaptr.Lookup(combo, tmp))
	{
		m_CMaptr.RemoveKey(combo);
		m_CMaptr.SetAt(combo, xml);
	}
	else
	{
		m_CMaptr.SetAt(combo, xml);
	}

	SetMapH();		
}

void CTrLayOut::SetMapH()
{
	CString trinfo, sKey, str, tmp;

	for (POSITION pos = m_CMaptr.GetStartPosition(); pos != NULL; )
	{
		m_CMaptr.GetNextAssoc(pos, sKey, str);
		tmp.Format(_T("%s\t%s"), sKey, str);
		trinfo += tmp; 
		trinfo += _T("^");
	}

	((CMainFrame*)m_pParent)->SettTrRtsDataset(0, trinfo);
}

void CTrLayOut::OnBnClickedClear()
{
	int cnt;
	cnt = m_formlist.GetItemCount();

	for (int ii = 0; ii < cnt; ii++)
	{
		m_formlist.SetItemText(ii, 1, _T(""));
		m_formlist.SetItemText(ii, 3, _T(""));
		m_formlist.SetItemText(ii, 4, _T(""));
	}
}

void CTrLayOut::OnBnClickedReset()
{
	int nCount = m_formlist.GetItemCount();
	CString	text;

	for (int ii = 0; ii < nCount; ii++)
	{
		text = m_formlist.GetItemText(ii, 0);
		if (text.Find(symNONE) == 0)
		{
			text = m_formlist.GetItemText(ii, 2);
			m_formlist.DeleteItem(ii--);
			nCount--;

			if (!text.CompareNoCase(_T("GRID")) || !text.CompareNoCase(_T("GRIDEX")) || !text.CompareNoCase(_T("TABLE")))
			{
				for (; ++ii < nCount; )
				{
					text = m_formlist.GetItemText(ii, 0);
					if (!text.IsEmpty())
					{
						ii--;
						break;
					}
					m_formlist.DeleteItem(ii--);
					nCount--;
				}
			}
		}
	}
}


void CTrLayOut::OnBnClickedOk()
{
	//OnBnSave();
	CDialogEx::OnOK();
}

void CTrLayOut::Cleardata(bool bcombo)
{
	for (int ii = 0; ii < m_formlist.GetItemCount(); ii++)
	{
		m_formlist.SetItemText(ii, 1, _T(""));
		m_formlist.SetItemText(ii, 3, _T(""));
		m_formlist.SetItemText(ii, 4, _T(""));
	}

	if (bcombo)
	{
		for (int ii = m_formTr.GetCount() - 1; ii >= 0; ii--)
			m_formTr.DeleteString(ii);
	}
}

void CTrLayOut::OnBnClickedNew()
{
	Cleardata();
}

void CTrLayOut::OnBnClickedBtnDelete()
{
	CString str, tmp, stmp;

	if (m_formTr.GetCount() > 0)
		m_formTr.GetLBText(m_formTr.GetCurSel(), tmp);

	if (tmp.IsEmpty())
		return;

	if (m_CMaptr.Lookup(tmp, str))
	{
		Cleardata();

		m_CMaptr.RemoveKey(tmp);
		m_formTr.DeleteString(m_formTr.GetCurSel());
		//stmp.Format(_T("%s\t"), tmp);
		//wccTrLayOut(0, stmp);
		//wccTrLayOut(1, stmp);
		if (m_formTr.GetCount() <= 0)
		{
			m_formTr.Invalidate();
			SetMapH();
			return;
		}
		
		m_formTr.SetCurSel(0);
		m_formTr.GetLBText(m_formTr.GetCurSel(), tmp);

		if (!tmp.IsEmpty())
			makeforminout(tmp);
	}

	SetMapH();
}


void CTrLayOut::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnClose();
}


bool CTrLayOut::checkComboname(CString key)
{
	bool badd = false;
	CString tmp;

	for (int ii = 0; ii < m_formTr.GetCount(); ii++)
	{
		m_formTr.GetLBText(ii, tmp);
		tmp.Trim();
		if (!tmp.CompareNoCase(key))
		{
			badd = true;
			break;
		}
	}
	return badd;
}

void CTrLayOut::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
	if (!m_serverlist || !m_formlist)
		return;

	CRect rc, rect;

	GetWindowRect(&rc);
	ScreenToClient(&rc);

	m_serverlist.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom = rc.bottom - GAP;
	m_serverlist.MoveWindow(rect);

	m_formlist.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom = rc.bottom - GAP;
	m_formlist.MoveWindow(rect);	
}


void CTrLayOut::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{	
	lpMMI->ptMinTrackSize.x = 831;
	lpMMI->ptMinTrackSize.y = 536;

	lpMMI->ptMaxTrackSize.x = 831;
	lpMMI->ptMaxTrackSize.y = 1000;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

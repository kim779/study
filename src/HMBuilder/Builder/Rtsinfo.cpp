// Rtsinfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "Rtsinfo.h"
#include "afxdialogex.h"
#include "MainFrm.h"

#include "h/mapform.h"
#include "amCc/LibBuild.h"


// CRtsinfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRtsinfo, CDialogEx)

CRtsinfo::CRtsinfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRtsinfo::IDD, pParent)
{
	m_frow = -1;
	m_rtssymbol.RemoveAll();
	m_tmprtssymbol.RemoveAll();
	m_rtskey = _T("");
	m_rtskeyType = _T("");
	m_rtsData = _T("");
	m_bgrid = false;
	m_pParent = pParent;
}

CRtsinfo::~CRtsinfo()
{
	m_rtssymbol.RemoveAll();
	m_tmprtssymbol.RemoveAll();
}

void CRtsinfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RTSCOMBO, m_rtscombo);
}


BEGIN_MESSAGE_MAP(CRtsinfo, CDialogEx)
	ON_BN_CLICKED(IDC_RTS_SAVE, &CRtsinfo::OnBnClickedRtsSave)
	ON_BN_CLICKED(IDOK, &CRtsinfo::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RTS_DELETE, &CRtsinfo::OnBnClickedRtsDelete)
END_MESSAGE_MAP()


// CRtsinfo 메시지 처리기입니다.


BOOL CRtsinfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect	rect;

	// Saved RTS Information
	((CStatic*)GetDlgItem(IDC_STATIC1))->GetWindowRect(&rect);
	ScreenToClient(rect);
	if (!m_rtsinfo.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LVS_REPORT | LVS_ALIGNTOP | LVS_NOSORTHEADER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, this, IDC_LIST_RTSINFO))
		return -1;

	m_rtsinfo.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_rtsinfo.InsertColumn(0, _T("Saved Keys"), LVCFMT_CENTER, 150);
	m_rtsinfo.m_bfocus = false;
	
	// Second RTS Symbol
	((CStatic*)GetDlgItem(IDC_STATIC2))->GetWindowRect(&rect);
	ScreenToClient(rect);
	if (!m_rtsList.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LVS_REPORT | LVS_ALIGNTOP | LVS_NOSORTHEADER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, this, IDC_LIST_RTSLIST))
		return -1;

	m_rtsList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_rtsList.InsertColumn(0, _T("Object Name"), LVCFMT_CENTER, 90);
	m_rtsList.InsertColumn(1, _T("Type"), LVCFMT_CENTER, 90);
	m_rtsList.InsertColumn(2, _T("RTS Symbol"), LVCFMT_CENTER, 85);
	m_rtsList.InsertColumn(3, _T("RTS Info"), LVCFMT_CENTER, 70);
	m_rtsList.m_bfocus = false;
	m_rtsList.m_skipcol = 3;

	setFont();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CRtsinfo::makeformTR(CString tmp)
{
	CString data, gb;
	
	m_rtsList.DeleteAllItems();
	m_rtsData = tmp;

	m_frow = -1;
	gb.Format("%c", GB_RTSSYMBOL);

	while (!tmp.IsEmpty())
	{
		data = Parser(tmp, gb);
		makeformdata(data);
	}

	if (m_rtscombo.GetCount() > 0)
	{
		m_rtscombo.SetCurSel(0);

		CString symbol;
		m_rtscombo.GetLBText(0, symbol);
		if (symbol.Find(" (") != -1)
			tmp = symbol.Left(symbol.Find(" ("));

		SelectSymbolList(tmp);
	}

	//makeFormsymbol();
}

void CRtsinfo::makeformdata(CString trsymbol)
{
	CString tmp, tmps, gridsymbol, gridexsymbol, tablesymbol;
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
		tmp = Parser(trsymbol, _T("="));
		tmps.Format("%s (%s)", tmp, trsymbol.Left(trsymbol.Find("=")));
		m_rtscombo.InsertString(idx, tmps);
	}
	else
	{
		tmp = Parser(trsymbol, _T("="));		
		tmps = tmp;
		tmp = Parser(trsymbol, _T(":"));

		if (!tmp.CompareNoCase(_T("GRID")) || !tmp.CompareNoCase(_T("GRIDEX")))
		{
			CString str;
			str.Format("%s (%s)", tmps, tmp);
			//m_rtscombo.InsertString(idx, str);
		}

		if (!tmp.CompareNoCase(_T("GRID")))
		{
			sgrid = true;
			tablesymbol = tmps;
			type = _T("GRID");
			m_selectsymbol.SetAt(tmps, trsymbol);
		}

		if (!tmp.CompareNoCase(_T("GRIDEX")))
		{
			sgrid = true;
			tablesymbol = tmps;
			type = _T("GRIDEX");
			m_selectsymbol.SetAt(tmps, trsymbol);
		}
		
		if (!tmp.CompareNoCase(_T("TABLE")))
		{
			tablesymbol = tmps;
			type = _T("TABLE");
			stable = true;			
		}

		while (!trsymbol.IsEmpty())
		{
			col = 0;
			tmp = Parser(trsymbol, _T(":"));				
			//tmps.Format(_T("%s.%s"), tablesymbol, tmp);
			tmps.Format(_T("%s.%s (%s)"), tablesymbol, tmp.Left(tmp.Find(_T("="))), type);
			idx = m_rtscombo.GetCount();
			m_rtscombo.InsertString(idx, tmps);				
		}
	}
}

void CRtsinfo::SelectSymbolList(CString symbol)
{
	CString trsymbol, tmp, tmps, tmpSymbol, tmpSubSymbol;
	CString strName, strType, rtsData, data;
	CString gb;

	bool	bgrid = false;

	int col;

	rtsData = m_rtsData;
	gb.Format("%c", GB_RTSSYMBOL);
		
	m_rtsList.DeleteAllItems();

	tmpSubSymbol = symbol;
	tmpSymbol = Parser(tmpSubSymbol, _T("."));
	if (m_selectsymbol.Lookup(tmpSymbol, trsymbol))
	{
		tmp.Format(_T("%s$="), tmpSymbol);
		strType = rtsData.Mid(rtsData.Find(tmp)+tmp.GetLength());
		strType = strType.Left(strType.Find(':'));
		while (!trsymbol.IsEmpty())
		{
			tmp = Parser(trsymbol, _T(":"));
			CString sym = tmp;
			tmp = Parser(sym, _T("="));

			if (!tmpSubSymbol.CompareNoCase(tmp))
				continue;

			col = 0;
			m_frow++;			
			tmps.Format(_T("%s.%s"), tmpSymbol, tmp);
			m_rtsList.AddItem(m_frow, col, tmps);
			col++;

		//	strType = rtsData.Mid(rtsData.Find(symbol));
		//	strType = strType.Left(strType.Find(":"));
		//	strType = strType.Mid(strType.Find("$=") + 2);

			m_rtsList.AddItem(m_frow, col++, strType);
			m_rtsList.AddItem(m_frow, col, sym);

		}
	}
	else
	{
		while (!rtsData.IsEmpty())
		{
			data = Parser(rtsData, gb);
			
			if (data.Find(_T("GRID")) != -1)
			{
				
				col = 0;
				m_frow++;
				tmp = Parser(data, _T("$="));
				m_rtsList.AddItem(m_frow, col, tmp);
				col++;
				tmp = Parser(data, _T(":"));
				m_rtsList.AddItem(m_frow, col, tmp);
				continue;
			}

			if (data.Find(_T("$")) == -1)
			{
				if (data.Find(symbol) != -1)
					continue;
				
				col = 0;
				m_frow++;
				tmp = Parser(data, _T("="));
				m_rtsList.AddItem(m_frow, col, tmp);
				col++;
				tmp = Parser(data, _T("="));
				m_rtsList.AddItem(m_frow, col, tmp);				
				col++;
				m_rtsList.AddItem(m_frow, col, data);
			}
			else		// table
			{
				col = 0;
				data.Replace(_T("$"), _T(""));				
				strName = Parser(data, _T("="));
				strType = Parser(data, _T(":"));

				while (!data.IsEmpty())
				{
					col = 0;					
					tmp = Parser(data, _T(":"));
					
					CString sym = tmp;
					tmp = Parser(sym, _T("="));

					symbol = symbol.Mid(symbol.Find(".") + 1);
					if (!tmp.CompareNoCase(symbol))
						continue;

					m_frow++;					
					tmps.Format(_T("%s.%s"), strName, tmp);
					m_rtsList.AddItem(m_frow, col, tmps);
					col++;
					m_rtsList.AddItem(m_frow, col, strType);
					col++;
					m_rtsList.AddItem(m_frow, col, sym);
				}				
			}						
		}
	}
}

void CRtsinfo::makeFormsymbol(struct _mapH* mapH)
{
	int ii = 0;
	CString tmp, key, tr, lay, tmpkey, title;
	CString rtsinfo;

	Clear();

	if (mapH == NULL || mapH->rtsinfo == NULL)
		return;

	rtsinfo = CString(mapH->rtsinfo, strlen(mapH->rtsinfo));
	//title.Format(_T("RTS Information_%s"), CString(mapH->mapN, L_MAPN));
	//SetWindowText(title);

	//tmp = getRTSSymbol();

	while (!rtsinfo.IsEmpty())
	{
		tr = Parser(rtsinfo, _T("^"));
		key = Parser(tr, _T("\t"));
		if (ii == 0)
			tmpkey = key;
		if (tr.IsEmpty())
			continue;
				
		if (!CompareRTS(key))
			continue;

		//m_rtscombo.InsertString(ii, key);
		m_rtsinfo.InsertItem(ii, key);
		m_rtssymbol.SetAt(key, tr);
		ii++;
	}

	if (m_rtscombo.GetCount() > 0)
	{
		//m_rtscombo.SetCurSel(0);
		makertssymbol(tmpkey);
	}
}

// RTS Symbol name 변경 시 기존 symbol사용 제한
bool CRtsinfo::CompareRTS(CString key)
{
	CString symbol;
	for (int ii = 0; ii < m_rtscombo.GetCount(); ii++)
	{
		m_rtscombo.GetLBText(ii, symbol);
		if (strcmp(symbol, key) == 0)
			return true;
	}
	return false;
}

void CRtsinfo::makertssymbol(CString key)
{
	CString tmp, str;
	int	row;
	bool	checkAll = false;
	
	if (!m_rtssymbol.Lookup(key, str))
		return;

	m_frow = -1;

	tmp = Parser(str, _T("|"));
	Parser(tmp, _T("/"));

	if (!tmp.IsEmpty())
	{
		//row = findrow(tmp);		
		row = m_rtscombo.FindString(0, tmp);
		
		if (row > -1)
		{
			//m_rtsList.SetItemText(row, 2, _T("key"));
			m_rtscombo.SetCurSel(row);
			SelectSymbolList(tmp);
			m_rtskey = tmp;
		}
	}

	while (!str.IsEmpty())
	{
		tmp = Parser(str, _T(","));
		if (!tmp.IsEmpty())
		{
			if (tmp.Find(_T("$")) >= 0)
				tmp.Replace(_T("$"), _T(""));

			row = findrow(tmp);
			if (row > -1)
				m_rtsList.SetItemText(row, 3, _T("■"));
		}
	}

	// select All
	for (int jj = 0; jj < m_rtsList.GetItemCount(); jj++)
	{
		if (m_rtsList.GetItemText(jj, 3).CompareNoCase(_T("■")))
		{
			checkAll = false;
			break;
		}
		checkAll = true;
	}
	((CButton*)GetDlgItem(IDC_CHECK_All))->SetCheck(checkAll);

}

int CRtsinfo::findrow(CString symbol)
{
	int row = -1;
	CString tmp;

	for (int ii = 0; ii < m_rtsList.GetItemCount(); ii++)
	{
		tmp = m_rtsList.GetItemText(ii, 0);
		if (!tmp.CompareNoCase(symbol))
		{
			row = ii;
			break;
		}
	}
	return row;
}

CString CRtsinfo::Parser(CString &srcstr, CString substr)
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

void CRtsinfo::setFont()
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

	m_rtsinfo.SetFont(&m_font);
	m_rtsList.SetFont(&m_font);
}
/*
void CRtsinfo::OnBnClickedRtsBtn()
{
	for (int ii = 0; ii < m_rtsList.GetItemCount(); ii++)
		m_rtsList.SetItemText(ii, 2, _T(""));
}
*/

BOOL CRtsinfo::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	LPNMHDR pHdr;
	LPNMLISTVIEW pNlv;

	pHdr = (LPNMHDR)lParam;
	pNlv = (LPNMLISTVIEW)lParam;

	switch (LOWORD(wParam))
	{
	case IDC_LIST_RTSINFO:
		{
			switch(pHdr->code)
			{
			case NM_CLICK:
				{
					int row = 0;
					CString tmp;

					row = m_rtsinfo.m_FocusRow;
					tmp = m_rtsinfo.GetItemText(row, 0);
					makertssymbol(tmp);
				}
				break;
			}
		}
		break;
	
	case IDC_LIST_RTSLIST:
		{
			switch(pHdr->code)
			{
			case NM_CLICK:
				{
					int row, col;
					CString tmp;
					bool checkAll = false;

					row = m_rtsList.m_FocusRow;
					col = m_rtsList.m_FocusCol;

					if (col > 1)
					{
						tmp = m_rtsList.GetItemText(row, 3);
						if (tmp.IsEmpty())
							m_rtsList.SetItemText(row, 3, _T("■"));
						else
							m_rtsList.SetItemText(row, 3, _T(""));
					}

					// select All
					for (int jj = 0; jj < m_rtsList.GetItemCount(); jj++)
					{
						if (m_rtsList.GetItemText(jj, 3).CompareNoCase(_T("■")))
						{
							checkAll = false;
							break;
						}
						checkAll = true;
					}
					((CButton*)GetDlgItem(IDC_CHECK_All))->SetCheck(checkAll);

					/*
					int row, col, pos;
					CString	symbol, tmp, sKey;
					bool bfirst = true;
					bool bchkcnt = false;

					row = m_rtsList.m_FocusRow;
					col = m_rtsList.m_FocusCol;

					if (col > 1)
					{						
						for (int ii = 0; ii < m_rtsList.GetItemCount(); ii++)
						{
							tmp = m_rtsList.GetItemText(ii, 2);
							if (!tmp.IsEmpty())
							{
								bfirst = false;
								break;
							}
						}

						pos = -1;
						for (int ii = 0; ii < m_rtsList.GetItemCount(); ii++)
						{
							if (!m_rtsList.GetItemText(ii, 2).CompareNoCase(_T("key")))
							{
								if (!m_rtsList.GetItemText(ii, 1).CompareNoCase(_T("GRID")))
								{
									m_bgrid = true;

									symbol = m_rtsList.GetItemText(ii, 0);
									pos = symbol.Find(_T("."));
									if (pos > 0)
										symbol = symbol.Left(pos);
								}
								else
									m_bgrid = false;
							}

							if (!m_rtsList.GetItemText(ii, 2).CompareNoCase(_T("■")))
								bchkcnt = true;
						}

						if (bfirst)
						{
							m_rtsList.SetItemText(row, 2, _T("key"));
							m_rtskey = m_rtsList.GetItemText(row, 0);
							m_rtskeyType = m_rtsList.GetItemText(row, 1);
						}
						else
						{
							if (m_bgrid && m_rtsList.GetItemText(row, 1).CompareNoCase(_T("GRID")))
								break;

							// 외부 Key일 경우 Grid Column은 클릭 되지 않음
							if ((m_rtskey.Find(".") == -1 || !m_rtskeyType.CompareNoCase(_T("TABLE")))
								&& !m_rtsList.GetItemText(row, 1).CompareNoCase(_T("GRID")) 
								&& (m_rtskeyType.CompareNoCase(_T("GRID")) && (m_rtsList.GetItemText(row, 0).Find(".") != -1)))
								break;

							// key가 Grid Column의 경우 외부 Control은 클릭 되지 않음
							sKey = m_rtskey.Left(m_rtskey.Find("."));
							if (!m_rtskeyType.CompareNoCase(_T("GRID")) && m_rtskey.Find(".") != -1
								&& ((m_rtsList.GetItemText(row, 0).Find(".") == -1) 
								|| strcmp(m_rtsList.GetItemText(row, 0).Left(m_rtsList.GetItemText(row, 0).Find(".")), sKey) != 0))
								break;


							tmp = m_rtsList.GetItemText(row, 0);
							pos = -1;
							pos = tmp.Find(_T("."));
							if (pos > 0)
								tmp = tmp.Left(pos);

							if (m_bgrid && symbol.CompareNoCase(tmp))
								break;

							tmp = m_rtsList.GetItemText(row, 2);
							if (!tmp.IsEmpty())
							{
								if (tmp.CompareNoCase(_T("key")) && bchkcnt)
									m_rtsList.SetItemText(row, 2, _T(""));
								if (!bchkcnt)
									m_rtsList.SetItemText(row, 2, _T(""));
							}
							else
								m_rtsList.SetItemText(row, 2, _T("■"));
						}
					}*/
				}
				break;
			//// 더블클릭 시 key면 전체 취소
			//case NM_DBLCLK:
			//	{
			//		int row;
			//		CString tmp;

			//		row = m_rtsList.m_FocusRow;
			//		tmp = m_rtsList.GetItemText(row, 2);
			//		
			//		if (!tmp.IsEmpty())
			//		{
			//			if (!tmp.CompareNoCase(_T("key")))
			//			{
			//				for (int ii = 0; ii < m_rtsList.GetItemCount(); ii++)
			//					m_rtsList.SetItemText(ii, 2, _T(""));
			//			}						
			//		}
			//	}
			//	break;
			}	
		}
		break;
	}
	return CDialogEx::OnNotify(wParam, lParam, pResult);
}

void CRtsinfo::reindexmap(CString key)
{
	CString skey, str, tmp, symbol;

	m_rtssymbol.Lookup(key, symbol);
	m_rtssymbol.RemoveKey(key);
	m_tmprtssymbol.RemoveAll();

	for (POSITION pos = m_rtssymbol.GetStartPosition(); pos != NULL; )
	{
		m_rtssymbol.GetNextAssoc(pos, skey, str);

		if (atoi(skey) > atoi(key))
		{
			tmp.Format("%d", atoi(skey) - 1);
			m_tmprtssymbol.SetAt(tmp, str);
		}
		else
			m_tmprtssymbol.SetAt(skey, str);
	}

	for (int ii = 0; ii < m_rtsList.GetItemCount(); ii++)
		m_rtsList.SetItemText(ii, 2, _T(""));

	m_rtssymbol.RemoveAll();

	tmp.Format(_T("%d"), m_tmprtssymbol.GetCount());
	m_tmprtssymbol.SetAt(tmp, symbol);

	for (POSITION pos = m_tmprtssymbol.GetStartPosition(); pos != NULL; )
	{
		m_tmprtssymbol.GetNextAssoc(pos, skey, str);
		m_rtssymbol.SetAt(skey, str);

		for (int ii = 0; ii < m_rtsList.GetItemCount(); ii++)
		{
			if (atoi(skey) == 0)
				skey = _T("key");
			tmp = m_rtsList.GetItemText(ii, 0);
			if (!str.CompareNoCase(tmp))
			{
				m_rtsList.SetItemText(ii, 2, skey);
				continue;
			}
		}
	}

	m_tmprtssymbol.RemoveAll();
}


void CRtsinfo::OnBnClickedRtsSave()
{
	CString rtsname, tmp, tmpxml, tmps, str, type;
	CString rtskey;
	int pos = -1;
	bool	bfirst = true;
		
	int index = m_rtscombo.GetCurSel();
	m_rtscombo.GetLBText(index, rtsname);
	if (rtsname.IsEmpty())
		return;

	rtskey = rtsname;
	if (rtsname.Find(" (") != -1)
		rtskey = rtsname.Left(rtsname.Find(" ("));
	
	tmp.Format("%s%c%s", _T("RTSINFO"), GB_RTSSYMBOL, rtskey);
	tmpxml.Format("%s%c%s", _T("RTSINFO"), GB_XML_RTSSYMBOL, rtskey);
	for (int ii = 0; ii < m_rtsList.GetItemCount(); ii++)
	{
		tmps = m_rtsList.GetItemText(ii, 3);
		if (!tmps.IsEmpty() && tmps.CompareNoCase(_T("key")))
		{
			type = m_rtsList.GetItemText(ii, 1);
			if (m_bgrid)
			{
				if (type.CompareNoCase(_T("GRID")))
					continue;
			}

			if (bfirst)
			{
				tmp += _T("|");
				tmpxml += _T("|");
				bfirst = false;
			}
			else
			{
				
				tmp += _T(",");
				tmpxml += _T(",");
			}

			if (m_bgrid && !type.CompareNoCase(_T("GRID")))
			{
				tmp += _T("$");
				tmpxml +=  _T("$");
			}

			tmp += m_rtsList.GetItemText(ii, 0);
			tmpxml += m_rtsList.GetItemText(ii, 0);
		}
	}
	tmps.Format(_T("%c"), GB_RTS);
	tmp += tmps;

	if (!rtsname.IsEmpty())
	{
		//tmps.Format(_T("%s\t%s"), rtsname, tmp);
		//wccRTSSymbol(0, tmps);
		//tmps.Format(_T("%s\t%s"), rtsname, tmpxml);
		//wccRTSSymbol(1, tmps);

		if (m_rtssymbol.Lookup(rtsname, str))
			m_rtssymbol.RemoveKey(rtsname);
		m_rtssymbol.SetAt(rtsname, tmpxml);

		/*if (!findcombo(rtsname))
		{
			m_rtscombo.InsertString(0, rtsname);
			m_rtscombo.SetCurSel(0);
		}*/
		bool check = false;
		for (int ii = 0; ii < m_rtsinfo.GetItemCount(); ii++)
		{
			if (!m_rtsinfo.GetItemText(ii, 0).CompareNoCase(rtsname))
			{
				check = true;
			}
		}
		if (!check)
			m_rtsinfo.InsertItem(m_rtsinfo.GetItemCount(), rtsname);

	}

	SetMapH();
}

void CRtsinfo::SetMapH()
{
	CString rtsinfo, sKey, str, tmp;

	for (POSITION pos = m_rtssymbol.GetStartPosition(); pos != NULL; )
	{
		m_rtssymbol.GetNextAssoc(pos, sKey, str);
		tmp.Format(_T("%s\t%s"), sKey, str);
		rtsinfo += tmp; 
		rtsinfo += _T("^");
	}

	((CMainFrame*)m_pParent)->SettTrRtsDataset(1, rtsinfo);
}

bool CRtsinfo::findcombo(CString comboname)
{
	CString cname;
	bool	bret = false;
	
	for (int ii = 0; ii < m_rtscombo.GetCount(); ii++)
	{
		m_rtscombo.GetLBText(ii, cname);
		if (!comboname.CompareNoCase(cname))
		{
			bret = true;
			break;
		}
	}

	return bret;
}

void CRtsinfo::OnBnClickedOk()
{
	//OnBnClickedRtsSave();
	CDialogEx::OnOK();
}

BOOL CRtsinfo::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_RTSCOMBO:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				CString tmp, tmps;				
				int index = m_rtscombo.GetCurSel();
				m_rtscombo.GetLBText(index, tmp);
				tmps = tmp;
				if (tmp.Find(" (") != -1)
					tmp = tmp.Left(tmp.Find(" ("));

				m_frow = -1;
				SelectSymbolList(tmp);
				//makertssymbol(tmp);
				((CButton*)GetDlgItem(IDC_CHECK_All))->SetCheck(false);

				// 테이블에 있는지 없는지 체크
				for (int ii = 0; ii < m_rtsinfo.GetItemCount(); ii++)
				{
					if (!m_rtsinfo.GetItemText(ii, 0).CompareNoCase(tmps))
					{
						makertssymbol(tmps);
						break;
					}
				}
			}
		}
		break;
	case IDC_CHECK_All:
		{
			UINT check = IsDlgButtonChecked(IDC_CHECK_All);
			SelectAll(check == 1);
		}
		break;
	}
	return CDialogEx::OnCommand(wParam, lParam);
}

void CRtsinfo::SelectAll(bool check)
{
	for (int jj = 0; jj < m_rtsList.GetItemCount(); jj++)
	{
		if (check)
			m_rtsList.SetItemText(jj, 3, _T("■"));
		else
			m_rtsList.SetItemText(jj, 3, _T(""));
	}
}


void CRtsinfo::Clear()
{
	/*for (int ii = m_rtscombo.GetCount() - 1; ii >= 0; ii--)
	{
		m_rtscombo.DeleteString(ii);
		m_rtscombo.RedrawWindow();
	}*/
	m_rtsinfo.DeleteAllItems();

	m_rtssymbol.RemoveAll();
	m_tmprtssymbol.RemoveAll();
}

void CRtsinfo::OnBnClickedRtsDelete()
{
	CString rtsname, str, tmp;

	//if (rtsname.IsEmpty() && m_rtscombo.GetCount() > 0)
	//	m_rtscombo.GetLBText(m_rtscombo.GetCurSel(), rtsname);

	//int row;
	//row = m_rtsinfo.m_FocusRow;
	//rtsname = m_rtsinfo.GetItemText(row, 0);

	m_rtscombo.GetLBText(m_rtscombo.GetCurSel(), rtsname);
	
	if (rtsname.IsEmpty())
		return;
		
	if (m_rtssymbol.Lookup(rtsname, str))
	{
		m_rtssymbol.RemoveKey(rtsname);

		//if (row < 0)
		//{
			for (int ii = 0; ii < m_rtsinfo.GetItemCount(); ii++)
			{
				if (!m_rtsinfo.GetItemText(ii, 0).CompareNoCase(rtsname))
				{
					m_rtsinfo.DeleteItem(ii);
					break;
				}
			}
		//}
		//else
		//	m_rtsinfo.DeleteItem(row);

		for (int jj = 0; jj < m_rtsList.GetItemCount(); jj++)
			m_rtsList.SetItemText(jj, 3, _T(""));
		//m_rtscombo.DeleteString(m_rtscombo.GetCurSel());

		//tmp.Format(_T("%s\t"), rtsname);
		//wccRTSSymbol(0, tmp);
		//wccRTSSymbol(1, tmp);
		/*
		if (m_rtscombo.GetCount() <= 0)
		{
			OnBnClickedRtsBtn();
			m_rtscombo.Invalidate();
			SetMapH();
			return;
		}
		*/

		//m_rtscombo.SetCurSel(0);
		//m_rtscombo.GetLBText(m_rtscombo.GetCurSel(), rtsname);

		/*if (m_rtssymbol.Lookup(rtsname, str))
		{
			makertssymbol(rtsname);
		}*/
	}
	else
	{
		for (int ii = 0; ii < m_rtsinfo.GetItemCount(); ii++)
		{
			if (!m_rtsinfo.GetItemText(ii, 0).CompareNoCase(rtsname))
				m_rtsinfo.DeleteItem(ii);
		}
	}

	SetMapH();
}
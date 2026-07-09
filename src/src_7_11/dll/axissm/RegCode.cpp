// RegCode.cpp : implementation file
//

#include "stdafx.h"
#include "RegCode.h"
#include "../../h/comcode.h"
#include "../../h/axisvar.h"

#define L_SCODE		sizeof(struct scode)
#define L_CFHEAD	sizeof(struct cfhead)
#define L_CFCODE	sizeof(struct cfcode)

#define M_GROUP		'A'
#define M_THEME		'B'
#define M_INGRE		'C'

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegCode dialog


CRegCode::CRegCode(CString home, CWnd* pParent /*=NULL*/)
	: CDialog(CRegCode::IDD, pParent)
{
	m_home = home;
	m_userName = AfxGetApp()->GetProfileString(WORKSTATION, SIGNONID);
	int pos = m_userName.Find('|');
	if (pos < 0)
		m_userName = _T("Anonymous");

	else	
		m_userName = m_userName.Mid(pos+1);

	if (m_userName.IsEmpty())
		m_userName = _T("Anonymous");

	m_mapCodeToSelect.RemoveAll();
	m_arCodeByName.RemoveAll();
}

CRegCode::~CRegCode()
{
	for (int ii = 0; ii < m_arCode.GetSize(); ii++)
		delete m_arCode.GetAt(ii);

	m_mapCodeToSelect.RemoveAll();
	m_arCodeByName.RemoveAll();
}


void CRegCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegCode)
	DDX_Control(pDX, IDC_SELECTLIST, m_selectList);
	DDX_Control(pDX, IDC_CODELIST, m_codeList);
	DDX_Control(pDX, IDC_CATEGORYTREE, m_tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRegCode, CDialog)
	//{{AFX_MSG_MAP(CRegCode)
	ON_NOTIFY(TVN_SELCHANGED, IDC_CATEGORYTREE, OnSelchangedCategorytree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CRegCode User define function
void CRegCode::Init()
{
	initList();
	loadInfo();
	LoadSCODE();
	LoadCFCODE();
	LoadOTHER(M_GROUP);
	LoadOTHER(M_THEME);
	LoadOTHER(M_INGRE);
}

void CRegCode::initList()
{
	CClientDC	dc(this);
	CRect		cRc;
	CSize		size;
	CString		text1, text2;

	text1.LoadString(ST_TEXT_CODE);
	text2.LoadString(ST_TEXT_CODENAME);

	m_codeList.GetClientRect(cRc);
	size = dc.GetTextExtent(" 999999 ");
	addColumn(m_codeList, text1, 0, size.cx);
	addColumn(m_codeList, text2, 1, cRc.Width() - size.cx);
	m_codeList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	addColumn(m_selectList, text1, 0, size.cx);
	addColumn(m_selectList, text2, 1, cRc.Width() - size.cx);
	m_selectList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void CRegCode::loadInfo()
{
	CFile		file;
	CString		path, code, name;
	path.Format("%s\\%s\\%s\\regCode", m_home, USRDIR, m_userName);
	if (!file.Open(path, CFile::modeRead))	return;

	struct _regCode	regCode;
	while (file.Read(&regCode, sizeof(regCode)))
	{
		code = CString(regCode.code, sizeof(regCode.code));
		name = CString(regCode.name, sizeof(regCode.name));
		code.TrimRight();	name.TrimRight();
	
		int row = m_selectList.GetItemCount();
		addListItem(m_selectList, code, row, 0);
		addListItem(m_selectList, name, row, 1);
	}
	file.Close();

	fit(m_selectList);
}

void CRegCode::saveInfo()
{
	CFile		file;
	CString		path, path_, code, name;
	path.Format("%s\\%s\\%s\\regCode", m_home, USRDIR, m_userName);
	path_.Format("%s\\%s\\%s\\regCode_", m_home, USRDIR, m_userName);

	CopyFile(path, path_, FALSE);
	DeleteFile(path);
	if (!file.Open(path, CFile::modeCreate|CFile::modeWrite))
	{
		::MessageBox(m_hWnd, "Don't file open", "Error", MB_ICONASTERISK);
		MoveFile(path_, path);
		return;
	}

	char	writeB[128];
	struct _regCode* regCode = (struct _regCode*) writeB;
	for (int ii = 0; ii < m_selectList.GetItemCount(); ii++)
	{
		memset(writeB, ' ', sizeof(writeB));
		code = m_selectList.GetItemText(ii, 0);
		name = m_selectList.GetItemText(ii, 1);
		//CopyMemory(regCode->code, code, code.GetLength());
		//CopyMemory(regCode->name, name, name.GetLength());
		sprintf_s(regCode->code, code);
		sprintf_s(regCode->name, name);
		file.Write(regCode, sizeof(struct _regCode));
	}

	file.Close();
	DeleteFile(path_);
}

void CRegCode::LoadSCODE()
{
	CFile		fileH;
	CString		file, Code, Name;
	char*		readB;
	struct	scode*	scode;
	_stockcode*	stockCode;
	
	file.Format("%s\\%s\\%s", m_home, TABDIR, SCODE_FN);
  	if (!fileH.Open(file, CFile::modeRead|CFile::typeBinary))
	{
		::MessageBox(m_hWnd, "Don't read cfcode file.", "Error!", MB_ICONASTERISK);
		return;
	}

	int fileL = (int)fileH.GetLength();
	if (fileL < L_SCODE)
	{
		fileH.Close();
		return;
	}

	readB = new char [fileL+1];
	fileH.Read(readB, fileL);
	fileH.Close();
	scode = (struct scode *) readB;
	for (int ii = 0; fileL >= L_SCODE; ii++)
	{
		stockCode = new _stockcode;
		stockCode->Name = CString(scode[ii].name, L_SNAM);
		stockCode->Code = CString(scode[ii].scod, L_SCOD);
		stockCode->Catagory = atoi(CString(scode[ii].kind, sizeof(scode[ii].kind)));
		stockCode->market = scode[ii].mket;

		m_arCode.Add(stockCode);
		m_arCodeByName.SetAt(stockCode->Code, stockCode->Name);
		fileL -= L_SCODE;
	}

	delete [] readB;
}

void CRegCode::LoadCFCODE()
{
	CFile		fileH;
	CString		file, Name, string;
	struct	cfhead*	head;
	struct	cfcode*	code;
	char*	readB;
	int	cfCnt;
	
	file.Format("%s\\%s\\%s", m_home, TABDIR, CFCODE_FN);
  	if (!fileH.Open(file, CFile::modeRead|CFile::typeBinary))
	{
		::MessageBox(m_hWnd, "Don't read cfcode file.", "Error!", MB_ICONASTERISK);
		return;
	}

	int fileL = (int)fileH.GetLength();
	if (fileL < L_CFHEAD)
	{
		fileH.Close();
		return;
	}

	readB = new char [fileL+1];
	fileH.Read(readB, fileL);
	fileH.Close();

	int fIndex = 1, cfN = int (readB[0] - '0');
	HTREEITEM	parent;
	for (int ii = 0; ii < cfN; ii++)
	{
		head = (struct cfhead *) &readB[fIndex];
		fIndex += L_CFHEAD;

		cfCnt = atoi(CString(head->mket, sizeof(head->mket)));
		Name = CString(head->name, sizeof(head->name));
		string.Format("%c", head->mknd);
		parent = addItemEx(Name, string);

		for (int jj = 0; jj < cfCnt && fileL >= L_CFCODE; jj++)
		{
			code = (struct cfcode *) &readB[fIndex];
			fIndex += L_CFCODE;

			Name = CString(code->name, sizeof(code->name));
			string.Format("%c%s", head->mknd, CString(code->kind, sizeof(code->kind)));
			addItemEx(Name, string, 0, parent);
		}
	}

	delete [] readB;
}

void CRegCode::LoadOTHER(char kind)
{
	CString		file, keys, value;
	char		buf[512];

	switch (kind)
	{
	case M_GROUP:
		file.Format("%s\\%s\\group.ini", m_home, TABDIR);
		value.LoadString(ST_REGCODE_GROUP);
		break;
	case M_THEME:
		file.Format("%s\\%s\\theme.ini", m_home, TABDIR);
		value.LoadString(ST_REGCODE_THEME);
		break;
	case M_INGRE:
		file.Format("%s\\%s\\ingre.ini", m_home, TABDIR);
		value.LoadString(ST_REGCODE_INGRE);
		break;
	default:	return;
	}

	HTREEITEM	parent = addItemEx(value, "@");
	for (int ii = 1; ; ii++)
	{
		keys.Format("%02d", ii);
		DWORD dw = GetPrivateProfileString("GROUP", keys, "", buf, sizeof(buf), file);
		if (dw <= 0)	break;
		
		value.Format("%c", kind);
		addItemEx(buf, value, ii, parent);
	}
}

void CRegCode::GetCodeAry(CStringArray& ary, CString dat)
{
	ary.RemoveAll();

	CString	tmps;
	while (!dat.IsEmpty())
	{
		int	pos = dat.Find(';');
		if (pos == -1)	break;

		tmps = dat.Left(pos++);
		dat = dat.Mid(pos);
		ary.Add(tmps);
	}
}

HTREEITEM CRegCode::addItem(CString itemS, DWORD dwData, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	HTREEITEM tItem = m_tree.InsertItem(TVIF_TEXT, itemS, 0, 0, 0, 0, 0, hParent, hInsertAfter);
	if (dwData > 0)	m_tree.SetItemData(tItem, dwData);
	return tItem;
}

HTREEITEM CRegCode::addItemEx(CString itemS, CString string, DWORD dwData, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	HTREEITEM tItem = m_tree.InsertItem(TVIF_TEXT, itemS, 0, 0, 0, 0, 0, hParent, hInsertAfter);
	m_tree.SetItemDataEx(tItem, string, dwData);
	return tItem;
}

BOOL CRegCode::addColumn(CListCtrl& list, LPCTSTR columns, int col, int width)
{
	LV_COLUMN lvc;
	if (width == 0)
	{
		CClientDC	dc(this);
		CString		str = columns;
		CSize		size = dc.GetTextExtent(str);
		
		width = size.cx + 5;
	}

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR)columns;
	lvc.cx	     = width;
	lvc.iSubItem = -1;

	return list.InsertColumn(col, &lvc);
}

BOOL CRegCode::addListItem(CListCtrl& list, CString items, int item, int subi)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subi;
	lvi.pszText  = (LPTSTR)items.operator LPCTSTR();

	if (!subi)	return list.InsertItem(&lvi);
	return list.SetItem(&lvi);
}

void CRegCode::fit(CListCtrl& list)
{
	if (list.GetSafeHwnd())
	{
		int	width;
		CRect	cRc;
		list.GetClientRect(cRc);
		width = list.GetColumnWidth(0);
		list.SetColumnWidth(0, width);
		list.SetColumnWidth(1, cRc.Width() - width);
	}
}

void CRegCode::setCode(char* dat, DWORD dw)
{
	LockWindowUpdate();
	BeginWaitCursor();

	m_codeList.DeleteAllItems();
	CStringArray	arCode, arName;

	bool		bloadCode = false;
	char		buf[512], market = dat[0];
	CString		file;
	
	switch (market)
	{
	case M_TSE:
	case M_OTC:
		{
			int	Catagory = atoi(&dat[1]);
			for (int ii = 0; ii < m_arCode.GetSize(); ii++)
			{
				_stockcode* stockcode = m_arCode.GetAt(ii);
				if (stockcode->market != market)
					continue;
				if (Catagory)
				{
					if (stockcode->Catagory != Catagory)
						continue;
				}

				arCode.Add(stockcode->Code);
				arName.Add(stockcode->Name);
			}
		}
		break;
	case M_GROUP:
		bloadCode = true;
		file.Format("%s\\%s\\group.ini", m_home, TABDIR);
		break;
	case M_THEME:
		bloadCode = true;
		file.Format("%s\\%s\\theme.ini", m_home, TABDIR);
		break;
	case M_INGRE:
		bloadCode = true;
		file.Format("%s\\%s\\ingre.ini", m_home, TABDIR);
		break;
	default:
		break;;
	}

	if (bloadCode)
	{
		CStringArray	ary;
		CString		keys, Code, Name;

		keys.Format("%02d", dw);
		GetPrivateProfileString("CODE", keys, "", buf, sizeof(buf), file);
		GetCodeAry(ary, buf);

		for (int ii = 0; ii < ary.GetSize(); ii++)
		{
			Code = ary.GetAt(ii);
			if (m_arCodeByName.Lookup(Code, Name))
			{
				arCode.Add(Code);
				arName.Add(Name);
			}
		}
		ary.RemoveAll();
	}

	for (int ii = 0; ii < arCode.GetSize(); ii++)
	{
		int	itemCnt = m_codeList.GetItemCount();
		addListItem(m_codeList, arCode.GetAt(ii), itemCnt);
		addListItem(m_codeList, arName.GetAt(ii), itemCnt, 1);
	}

	arCode.RemoveAll();
	arName.RemoveAll();
	fit(m_codeList);
	
	EndWaitCursor();
	UnlockWindowUpdate();
	
}

void CRegCode::InsertItem()
{
	CString	Code, Name;
	int	index, row;

	for (POSITION pos = m_codeList.GetFirstSelectedItemPosition(); pos; )
	{
		index = m_codeList.GetNextSelectedItem(pos);
		Code = m_codeList.GetItemText(index, 0);
		Name = m_codeList.GetItemText(index, 1);

		if (IsDuplication(Code))	continue;
		row = m_selectList.GetItemCount();
		addListItem(m_selectList, Code, row, 0);
		addListItem(m_selectList, Name, row, 1);

		m_mapCodeToSelect.SetAt(Code, Name);
	}
}

void CRegCode::DeleteItem()
{
	int	index;
	CString	Code;
	while ((index = m_selectList.GetNextItem(-1, LVNI_SELECTED)) != -1)
	{
		Code = m_selectList.GetItemText(index, 0);
		m_selectList.DeleteItem(index);
		m_mapCodeToSelect.RemoveKey(Code);
	}
}

void CRegCode::InsertAll()
{
	CString	Code, Name;
	for (int ii = 0 ; ii < m_codeList.GetItemCount() ; ii++)
	{
		Code = m_codeList.GetItemText(ii, 0);
		Name = m_codeList.GetItemText(ii, 1);

		if (IsDuplication(Code)) continue;
		int row = m_selectList.GetItemCount();
		addListItem(m_selectList, Code, row, 0);
		addListItem(m_selectList, Name, row, 1);

		m_mapCodeToSelect.SetAt(Code, Name);
	}
}

void CRegCode::DeleteAll()
{
	m_selectList.DeleteAllItems();
	m_mapCodeToSelect.RemoveAll();
}

bool CRegCode::IsDuplication(CString Code)
{
	CString itemS;
	if (m_mapCodeToSelect.Lookup(Code, itemS))
		return true;
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CRegCode message handlers

BOOL CRegCode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	Init();
	m_tree.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRegCode::OnSelchangedCategorytree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	DWORD		dw;
	CString		dat;

	LPNMTREEVIEW	tree = (LPNMTREEVIEW) pNMTreeView;
	m_tree.GetItemDataEx(tree->itemNew.hItem, dat, dw);
	setCode((char *) dat.operator LPCTSTR(), dw);
}

BOOL CRegCode::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int	comm = LOWORD(wParam);
	switch (comm)
	{
	case IDC_ADDREGCODE:	InsertItem();	break;
	case IDC_DELREGCODE:	DeleteItem();	break;
	case IDC_ALLADDREGCODE:	InsertAll();	break;
	case IDC_ALLDELREGCODE:	DeleteAll();	break;
	case IDOK:		saveInfo();	break;
	}
	return CDialog::OnCommand(wParam, lParam);
}

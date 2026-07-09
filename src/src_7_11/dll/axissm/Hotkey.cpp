// Hotkey.cpp : implementation file
//

#include "stdafx.h"
#include "Hotkey.h"
#include "tmenu.h"

#include "../../h/axisvar.h"

#define	SPLIT		'\t'
#define	SPLIT2		'\n'
#define	SEP		'-'
#define	MAP		'#'
#define	HASCHILD	20000


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHotkey dialog


CHotkey::CHotkey(class CTMenu* menu, CString home, CString user, CWnd* pParent /*=NULL*/)
	: CExDialog(CHotkey::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHotkey)
	m_Edit = _T("");
	m_Result = _T("");
	m_aEdit = _T("");
	//}}AFX_DATA_INIT
	m_arItem.RemoveAll();
	m_sel = 0;

	m_menu = menu;
	m_home = home;
	m_user = user;
	m_pos = CPoint(0, 0);

	m_cnt = 0;
	m_count = 0;
	m_kIndex = -1;
	m_nRes = 0;

	for (int ii = 65; ii < 92; ii++)
		m_hlist.Add(ii);
	m_hlist.Add(93);	m_hlist.Add(59);
	m_hlist.Add(39);	m_hlist.Add(44);
	m_hlist.Add(46);	m_hlist.Add(47);
}

CHotkey::~CHotkey()
{
	ChangeEnglishMode(this->m_hWnd);
	m_arItem.RemoveAll();
	m_hlist.RemoveAll();
	m_arMap.RemoveAll();
	m_arDesc.RemoveAll();
}

void CHotkey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHotkey)
	DDX_Control(pDX, IDC_KEYLIST, m_keyList);
	DDX_Control(pDX, IDC_TREE, m_Tree);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT, m_Edit);
	DDX_Text(pDX, IDC_RESULT, m_Result);
	DDX_Text(pDX, IDC_EDIT_ALL, m_aEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHotkey, CDialog)
	//{{AFX_MSG_MAP(CHotkey)
	ON_BN_CLICKED(IDC_AFTER, OnAfter)
	ON_BN_CLICKED(IDC_BEFORE, OnBefore)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	ON_BN_CLICKED(IDC_SEARCH_ALL, OnSearchAll)
	ON_BN_CLICKED(IDC_VIEW_ALL, OnViewAll)
	ON_BN_CLICKED(IDC_VIEW_MENU, OnViewMenu)
	ON_NOTIFY(NM_CLICK, IDC_KEYLIST, OnClickKey)
	ON_NOTIFY(NM_DBLCLK, IDC_KEYLIST, OnDblclkKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHotkey message handlers

BOOL CHotkey::OnInitDialog() 
{
	CExDialog::OnInitDialog();
	
	ChangeHangulMode(this->m_hWnd);

	m_mode = MODE_SEARCH;
	((CButton *)GetDlgItem(IDC_VIEW_ALL))->SetCheck(1);
	OnViewAll();

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_TREEICON);
	m_Tree.InitTreeImage(bitmap);

	initList();
	initkeyList();

	setTree();
	setList();
	enableButton(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHotkey::OnAfter() 
{
	if (!m_cnt)	return;
	if (m_sel+1 >= m_cnt)
		m_sel = 0;
	else	m_sel++;

	setCount(m_sel, m_cnt);
	treeSelect(m_sel);
}

void CHotkey::OnBefore() 
{
	if (!m_cnt)	return;
	if (m_sel-1 < 0)	
		m_sel = m_cnt-1;
	else	m_sel--;

	setCount(m_sel, m_cnt);
	treeSelect(m_sel);
}

void CHotkey::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CHotkey::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	int pos = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (pos < 0 || pos >= m_arTr.GetSize())
		return;
	CString tmpS = m_arTr.GetAt(pos);

	m_keyList.SetItemText(m_kIndex, 1, m_menu->GetMapping(tmpS).Mid(2, 4));
	tmpS = m_List.GetItemText(pos, 1);
	m_keyList.SetItemText(m_kIndex, 2, tmpS);
}

void CHotkey::OnSearch() 
{
	m_cnt = m_sel = 0;

	UpdateData(TRUE);
	CString com, comS = m_Edit;
	UpdateData(FALSE);
	m_arItem.RemoveAll();

	HTREEITEM	item;
	if (!(item = m_Tree.GetRootItem()))
		return;

	findItem(item, comS);

	if (m_cnt == 0)
	{
		setCount();
		GotoDlgCtrl(GetDlgItem(IDC_EDIT));
		enableButton(false);
	}
	else
	{
		setCount(m_sel, m_cnt);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT));
		treeSelect(m_sel);
		enableButton(true);
	}
}

void CHotkey::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	HTREEITEM item = m_Tree.GetSelectedItem();
	if (item == NULL || m_kIndex == -1)
		return;

	CString	code, desc;
	DWORD dw = (DWORD)m_Tree.GetItemData(item);
	if (dw >= 0 && (int) dw < m_arMap.GetSize())
	{
		code = m_arMap.GetAt((int) dw);
		desc = m_Tree.GetItemText(item);
		m_keyList.SetItemText(m_kIndex, 1, m_menu->GetMapping(code).Mid(2, 4));
		m_keyList.SetItemText(m_kIndex, 2, desc);
	}
}

BOOL CHotkey::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetDlgItem(IDC_EDIT)->m_hWnd == pMsg->hwnd ||
			GetDlgItem(IDC_EDIT_ALL)->m_hWnd == pMsg->hwnd)
		{
			if (m_mode == MODE_MENU)
				OnSearch();
			else
				OnSearchAll();

			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
		else
			pMsg->wParam = VK_TAB; 
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		pMsg->wParam = VK_TAB; 
	else if (pMsg->message == WM_KEYDOWN &&
		(pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN))
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		
		if (GetDlgItem(IDC_LIST)->m_hWnd == pMsg->hwnd)
		{
			int pos = m_List.GetNextItem(-1, LVNI_SELECTED);
			if (pos < 0)
			{
				return TRUE;
			}
			m_sel = pos;
			treeSelect(pos);
		}
		return TRUE;
	}

	return CExDialog::PreTranslateMessage(pMsg);
}

void CHotkey::initList()
{
	CString hData[] = {"TR", "화면이름",};

	CClientDC	dc(this);
	CRect		cRc;
	int		cW[2];

	m_List.GetClientRect(cRc);
	cW[0] = dc.GetTextExtent(" 9999 ").cx;
	cW[1] = cRc.Width() - cW[0] - 16;

	m_List.InitHeaderData(hData, cW, 2, LVCFMT_LEFT);
	m_List.SetFullRowSelect();
	m_List.HeaderFix();
	m_List.SetDisableSort();
}

void CHotkey::initkeyList()
{
	CString hData[] = {"K", "TR", "화면이름",};

	CClientDC	dc(this);
	CRect		cRc;
	int		cW[3];

	m_keyList.GetClientRect(cRc);
	cW[0] = dc.GetTextExtent(" W ").cx;
	cW[1] = dc.GetTextExtent("9999 ").cx;
	cW[2] = cRc.Width() - cW[0] -cW[1] - 16;

	m_keyList.InitHeaderData(hData, cW, 3, LVCFMT_LEFT);
	m_keyList.SetFullRowSelect();
	m_keyList.HeaderFix();
	m_keyList.SetDisableSort();

	loadKey();
}

void CHotkey::loadKey()
{
	char	buf[256];
	CString	file, key, tmpS, str;
	file.Format("%s\\%s\\%s\\hkey.ini", m_home, USRDIR, m_user);
	for (int ii = 0; ii < m_hlist.GetSize(); ii++)
	{
		key.Format("%c", m_hlist.GetAt(ii));
		m_keyList.InsertItem(ii, key);
		DWORD dw = GetPrivateProfileString("HKEY", key, "", buf, sizeof(buf), file);
		if (dw <= 0)	continue;
		str = buf;
		int	pos = str.Find(',');
		if (pos == -1)	continue;
		tmpS = str.Left(pos++);
		m_keyList.SetItemText(ii, 1, tmpS);
		tmpS = str.Mid(pos);	tmpS.TrimLeft();
		m_keyList.SetItemText(ii, 2, tmpS);
	}
	int state = GetPrivateProfileInt("HKEY", "state", 0, file);
	((CButton *) GetDlgItem(IDC_USEKEY))->SetCheck(state);
}

void CHotkey::setCount(int cnt, int tot)
{
	CString tmpS = _T("");
	if (cnt == -1 && tot == -1)
		tmpS = _T("");
	else
		tmpS.Format("%d/%d", cnt+1, tot);
	UpdateData(TRUE);
	m_Result = tmpS;
	UpdateData(FALSE);
}

void CHotkey::treeSelect(int idx)
{
	if (idx >= m_arItem.GetSize())
		return;
	m_Tree.SelectItem(m_arItem.GetAt(idx));
}

void CHotkey::enableButton(bool bEnable)
{
	CButton* before = (CButton *)GetDlgItem(IDC_BEFORE);
	before->EnableWindow(bEnable);
	CButton* after = (CButton *)GetDlgItem(IDC_AFTER);
	after->EnableWindow(bEnable);
}

void CHotkey::OnSearchAll() 
{
	setList();
}

void CHotkey::OnViewAll() 
{
	m_mode = MODE_SEARCH;

	GetDlgItem(IDC_TEXT1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_ALL)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SEARCH_ALL)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_LIST)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_TEXT1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_SEARCH_ALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_LIST)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SEARCH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RESULT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BEFORE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_AFTER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TREE)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEARCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESULT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BEFORE)->EnableWindow(FALSE);
	GetDlgItem(IDC_AFTER)->EnableWindow(FALSE);
	GetDlgItem(IDC_TREE)->EnableWindow(FALSE);

	GotoDlgCtrl(GetDlgItem(IDC_EDIT_ALL));
}

void CHotkey::OnViewMenu() 
{
	m_mode = MODE_MENU;

	GetDlgItem(IDC_EDIT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SEARCH)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RESULT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BEFORE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_AFTER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_TREE)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_SEARCH)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESULT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BEFORE)->EnableWindow(TRUE);
	GetDlgItem(IDC_AFTER)->EnableWindow(TRUE);
	GetDlgItem(IDC_TREE)->EnableWindow(TRUE);

	GetDlgItem(IDC_TEXT1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SEARCH_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LIST)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_TEXT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEARCH_ALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_LIST)->EnableWindow(FALSE);

	GotoDlgCtrl(GetDlgItem(IDC_EDIT));

	if (m_cnt == 0)	enableButton(false);
	else		enableButton(true);
}

void CHotkey::fit()
{
	if (m_List.GetSafeHwnd())
	{
		int		W0, W1;
		CRect		cRc;
		CClientDC	dc(this);

		m_List.GetClientRect(cRc);
		W0 = dc.GetTextExtent(" 9999 ").cx;
		W1 = cRc.Width() - W0;
		m_List.SetColumnWidth(0, W0);
		m_List.SetColumnWidth(1, W1);
	}
}

BOOL CHotkey::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int	index = -1, comm = LOWORD(wParam);
	switch (comm)
	{
	case IDC_SLASH:	index++;
	case IDC_DAT:	index++;
	case IDC_COMMA:	index++;
	case IDC_SQ:	index++;
	case IDC_SC:	index++;
	case IDC_RB:	index++;
	case IDC_LB:	index++;
	case IDC_Z:	index++;
	case IDC_Y:	index++;
	case IDC_X:	index++;
	case IDC_W:	index++;
	case IDC_V:	index++;
	case IDC_U:	index++;
	case IDC_T:	index++;
	case IDC_S:	index++;
	case IDC_R:	index++;	
	case IDC_Q:	index++;
	case IDC_P:	index++;
	case IDC_O:	index++;
	case IDC_N:	index++;
	case IDC_M:	index++;
	case IDC_L:	index++;
	case IDC_K:	index++;
	case IDC_J:	index++;
	case IDC_I:	index++;
	case IDC_H:	index++;
	case IDC_G:	index++;	
	case IDC_F:	index++;
	case IDC_E:	index++;
	case IDC_D:	index++;
	case IDC_C:	index++;
	case IDC_B:	index++;
	case IDC_A:	index++;	break;
	}
	if (index >= 0 && index < 33 && m_kIndex != index)
	{
		CSize		sz(0, 0);
		CRect		cRc;
		SCROLLINFO	info;
		
		m_keyList.GetScrollInfo(SB_VERT, &info, SIF_ALL);
		int	pos = index - info.nPos;
		m_keyList.GetItemRect(0, cRc, LVIR_BOUNDS);
		sz.cy = cRc.Height()*pos;
		m_keyList.Scroll(sz);

		m_keyList.SetItemState(index, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		m_keyList.SetScrollPos(SB_VERT, index, TRUE);
		m_keyList.SetFocus();
		changeState(index);
		return TRUE;
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CHotkey::OnClickKey(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	UINT index = m_keyList.GetNextItem(-1, LVNI_SELECTED);
	changeState(index);
}

void CHotkey::OnDblclkKey(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	UINT index = m_keyList.GetNextItem(-1, LVNI_SELECTED);
	if (index >= 0 && index < 33)
	{
		m_keyList.SetItemText(index, 1, "");
		m_keyList.SetItemText(index, 2, "");
	}
}

void CHotkey::changeState(int index)
{
	if (index < 0 || index > 32 || (int)index == m_kIndex)
		return;
	UINT	nRes;
	switch (index)
	{
	case 32:	nRes = IDC_SLASH;	break;	
	case 31:	nRes = IDC_DAT;		break;
	case 30:	nRes = IDC_COMMA;	break;
	case 29:	nRes = IDC_SQ;		break;
	case 28:	nRes = IDC_SC;		break;
	case 27:	nRes = IDC_RB;		break;
	case 26:	nRes = IDC_LB;		break;
	case 25:	nRes = IDC_Z;		break;
	case 24:	nRes = IDC_Y;		break;
	case 23:	nRes = IDC_X;		break;
	case 22:	nRes = IDC_W;		break;
	case 21:	nRes = IDC_V;		break;
	case 20:	nRes = IDC_U;		break;
	case 19:	nRes = IDC_T;		break;
	case 18:	nRes = IDC_S;		break;
	case 17:	nRes = IDC_R;		break;
	case 16:	nRes = IDC_Q;		break;
	case 15:	nRes = IDC_P;		break;
	case 14:	nRes = IDC_O;		break;
	case 13:	nRes = IDC_N;		break;
	case 12:	nRes = IDC_M;		break;
	case 11:	nRes = IDC_L;		break;
	case 10:	nRes = IDC_K;		break;
	case 9:		nRes = IDC_J;		break;
	case 8:		nRes = IDC_I;		break;
	case 7:		nRes = IDC_H;		break;
	case 6:		nRes = IDC_G;		break;
	case 5:		nRes = IDC_F;		break;
	case 4:		nRes = IDC_E;		break;
	case 3:		nRes = IDC_D;		break;
	case 2:		nRes = IDC_C;		break;
	case 1:		nRes = IDC_B;		break;
	case 0:		nRes = IDC_A;		break;
	}

	GetDlgItem(nRes)->ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);
	if (m_kIndex >= 0)
	{
		UINT uStyle = ((CButton *) GetDlgItem(m_nRes))->GetButtonStyle();
		uStyle = (uStyle & ~BS_DEFPUSHBUTTON) | BS_PUSHBUTTON;
		((CButton *) GetDlgItem(m_nRes))->SetButtonStyle(uStyle);
		GetDlgItem(m_nRes)->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME);
	}
	m_kIndex = index;
	m_nRes = nRes;
}

void CHotkey::OnOK() 
{
	writeKey();	
}

void CHotkey::writeKey()
{
	CString	file, key, dat, item1, item2;

	file.Format("%s\\%s\\%s\\hkey.ini", m_home, USRDIR, m_user);
	dat.Format("%d", ((CButton *) GetDlgItem(IDC_USEKEY))->GetCheck());

	WritePrivateProfileString("HKEY", "state", dat, file);
	for (int ii = 0; ii < m_hlist.GetSize(); ii++)
	{
		key.Format("%c", m_hlist.GetAt(ii));
		item1 = m_keyList.GetItemText(ii, 1);
		item2 = m_keyList.GetItemText(ii, 2);
		if (item1.IsEmpty())	dat.Empty();
		else	dat.Format("%s, %s", item1, item2);
		WritePrivateProfileString("HKEY", key, dat, file);
	}
}

void CHotkey::setTree()
{
	m_Tree.DeleteAllItems();

	char	buf[1024];
	CString	file, dat;

	file.Format("%s\\%s\\%s", m_home, TABDIR, AXISMENU);

//	FILE *fp = fopen(file, "r");

	FILE *fp = NULL;
	errno_t err = fopen_s(&fp, file, "r");
	if (!fp)	return;

	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		dat = buf;
		if (dat.Find("www#") != -1)
			continue;
		make_Tree(dat);
	}
	fclose(fp);
}

void CHotkey::setList(BOOL flag)
{
	UpdateData(TRUE);
	CString desc, com, comS = m_aEdit;
	UpdateData(FALSE);

	m_List.DeleteAllItems();
	m_arTr.RemoveAll();
	for (int ii = 0; ii < m_arMap.GetSize(); ii++)
	{
		com = m_arMap.GetAt(ii);
		m_arDesc.Lookup(com, desc);
		if (desc.Find(comS) == -1)	continue;
		if (com.Find("#") != -1)	continue;
		
		int row = m_List.GetItemCount();

		m_arTr.Add(com);
		m_List.InsertItem(row, m_menu->GetMapping(com).Mid(2, 4));
		m_List.SetItemText(row, 1, desc);
	}

	if (!flag)	fit();
}

void CHotkey::make_Tree(CString dat)
{
	int	treeN, step = 0;
	BOOL	map, sep;
	CString	str, desc;
	
	HTREEITEM item;
	map = sep = FALSE;
	while (!dat.IsEmpty())
	{
		int pos = dat.Find(SPLIT);
		if (pos == -1)	break;

		str = dat.Left(pos++);
		dat = dat.Mid(pos);
		if (str.IsEmpty())	continue;

		char	ch = str.GetAt(0);
		switch (ch)
		{
		case SEP:	continue;
		case MAP:	map = TRUE;	break;
		default:			break;
		}
		if (map)	str = str.Mid(1);

		switch (step)
		{
		case 0:
			if (!m_treeN[step].Compare(str))
			{
				step++;
				continue;
			}
			item = m_Tree.InsertItem(str, 0, 1);
			m_Tree.SetItemData(item, (DWORD) HASCHILD);

			m_itemN[step]  = item;
			m_treeN[step++] = str;
			for (treeN = step; treeN < MAX; treeN++)
			{
				m_itemN[treeN] = NULL;
				m_treeN[treeN].Empty();
			}
			break;
		default:
			if (!m_itemN[step-1])	return;
			if (map)
			{
				desc = str;
				item = m_Tree.InsertItem(desc, 2, 3, m_itemN[step-1]);
				m_Tree.SetItemData(item, (DWORD) m_count++);
				if (!dat.IsEmpty())
				{
					int pos = dat.Find(SPLIT);
					int pos2 = dat.Find(SPLIT2);
					if (pos == -1 && pos2 == -1)
						str.Empty();
					else
					{
						if (pos == -1)	pos = pos2;
						str = dat.Left(pos++);
						dat = dat.Mid(pos);
					}
				}
				m_arMap.Add(str);
				m_arDesc.SetAt(str, desc);
				return;
			}

			if (!m_treeN[step].Compare(str))
			{
				step++;
				continue;
			}
			item = m_Tree.InsertItem(str, 0, 1, m_itemN[step-1]);
			m_Tree.SetItemData(item, (DWORD) HASCHILD);

			m_itemN[step]  = item;
			m_treeN[step++] = str;
			for (treeN = step; treeN < MAX; treeN++)
			{
				m_itemN[treeN] = NULL;
				m_treeN[treeN].Empty();
			}
			break;
		}
	}
}

void CHotkey::findItem(HTREEITEM item, CString comS)
{
	CString		com;
	HTREEITEM	sItem;

	do
	{
		com = m_Tree.GetItemText(item);
		if (com.Find(comS) != -1)
		{
			m_arItem.Add(item);
			m_cnt++;
		}

		if (!m_Tree.ItemHasChildren(item))
			continue;
		sItem = m_Tree.GetNextItem(item, TVGN_CHILD);
		findItem(sItem,  comS);

	} while ((item = m_Tree.GetNextItem(item, TVGN_NEXT)) != NULL);
}

void CHotkey::ApplySetup()
{
	writeKey();
}

void CHotkey::DefaultSetup()
{
	char	buf[256];
	CString	file, key, tmpS, str;
	file.Format("%s\\%s\\hkey.ini", m_home, MTBLDIR);

	m_keyList.DeleteAllItems();

	for (int ii = 0; ii < m_hlist.GetSize(); ii++)
	{
		key.Format("%c", m_hlist.GetAt(ii));
		m_keyList.InsertItem(ii, key);
		DWORD dw = GetPrivateProfileString("HKEY", key, "", buf, sizeof(buf), file);
//		if (dw <= 0)	continue;
		str = buf;
		int	pos = str.Find(',');
//		if (pos == -1)	continue;
		tmpS = str.Left(pos++);
		m_keyList.SetItemText(ii, 1, tmpS);
		tmpS = str.Mid(pos);	tmpS.TrimLeft();
		m_keyList.SetItemText(ii, 2, tmpS);
	}

	int state = GetPrivateProfileInt("HKEY", "state", 0, file);
	((CButton *) GetDlgItem(IDC_USEKEY))->SetCheck(state);
}
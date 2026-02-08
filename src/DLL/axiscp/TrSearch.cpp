// TrSearch.cpp : implementation file
//

#include "stdafx.h"
#include "TrSearch.h"
#include "tmenu.h"

#include "../../axis/axMsg.hxx"
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
// CTrSearch dialog


CTrSearch::CTrSearch(class CTMenu* menu, bool (*axiscall)(int, WPARAM, LPARAM), CWnd* pParent /*=NULL*/)
	: CExDialog(CTrSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrSearch)
	m_Edit = _T("");
	m_Result = _T("");
	//}}AFX_DATA_INIT

	m_menu		= menu;
	m_axiscall	= axiscall;
	m_pos		= CPoint(0, 0);

	m_sel		= 0;
	m_cnt		= 0;
	m_count		= 0;

	m_arItem.RemoveAll();
	m_arMap.RemoveAll();
	m_arDesc.RemoveAll();
}

CTrSearch::~CTrSearch()
{
	ChangeEnglishMode(this->m_hWnd);

	m_arItem.RemoveAll();
	m_arMap.RemoveAll();
	m_arDesc.RemoveAll();
}

void CTrSearch::DoDataExchange(CDataExchange* pDX)
{
	CExDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrSearch)
	DDX_Control(pDX, IDC_TREE, m_Tree);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT, m_Edit);
	DDX_Text(pDX, IDC_RESULT, m_Result);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrSearch, CExDialog)
	//{{AFX_MSG_MAP(CTrSearch)
	ON_BN_CLICKED(IDC_AFTER, OnAfter)
	ON_BN_CLICKED(IDC_BEFORE, OnBefore)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrSearch message handlers

void CTrSearch::OnAfter() 
{
	if (!m_cnt)	return;
	if (m_sel+1 >= m_cnt)
		m_sel = 0;
	else	m_sel++;

	setCount(m_sel, m_cnt);
	treeSelect(m_sel);
	listSelect(m_sel);
}

void CTrSearch::OnBefore() 
{
	if (!m_cnt)	return;
	if (m_sel-1 < 0)	
		m_sel = m_cnt-1;
	else	m_sel--;

	setCount(m_sel, m_cnt);
	treeSelect(m_sel);
	listSelect(m_sel);
}

void CTrSearch::OnSearch()
{
	UpdateData(TRUE);
	CString com, comS = m_Edit;

	comS.TrimRight();
	if (comS.IsEmpty())
		return;

	m_cnt = m_sel = 0;
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

	setList();
}

void CTrSearch::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	int pos = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (pos < 0 || pos >= m_arTr.GetSize())
		return;

	m_sel = pos;
	setCount(m_sel, m_cnt);
	treeSelect(m_sel);
}

void CTrSearch::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	int pos = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (pos < 0 || pos >= m_arTr.GetSize())
		return;
	
	m_trcode = m_arTr.GetAt(pos);
	if (((CButton *) GetDlgItem(IDC_AFTERCLOSE))->GetCheck() == BST_CHECKED)
		EndDialog(IDOK);
	else if (m_axiscall)
		(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *)m_trcode.operator LPCTSTR());
}

void CTrSearch::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM item = m_Tree.GetSelectedItem();
	if (item == NULL)	return;

	DWORD dw = m_Tree.GetItemData(item);
	if (dw >= 0 && (int) dw < m_arMap.GetSize())
	{
		m_trcode = m_arMap.GetAt((int) dw);
		if (((CButton *) GetDlgItem(IDC_AFTERCLOSE))->GetCheck() == BST_CHECKED)
			EndDialog(IDOK);
		else if (m_axiscall)
			(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *)m_trcode.operator LPCTSTR());
	}
	*pResult = 0;
}

BOOL CTrSearch::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetDlgItem(IDC_EDIT)->m_hWnd == pMsg->hwnd)
		{
			OnSearch();

			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
		else
			pMsg->wParam = VK_TAB; 
	}
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
			setCount(m_sel, m_cnt);
			treeSelect(pos);
		}
		return TRUE;
	}
	return CExDialog::PreTranslateMessage(pMsg);
}

BOOL CTrSearch::OnInitDialog() 
{
	CExDialog::OnInitDialog();
	
	ChangeHangulMode(this->m_hWnd);

/*	CRect wRc;
	GetWindowRect(&wRc);
	int	screenX = GetSystemMetrics(SM_CXSCREEN);
	int	screenY = GetSystemMetrics(SM_CYSCREEN);
	if (m_pos.x + wRc.Width() > screenX)	m_pos.x = screenX - wRc.Width() - 5;
	if (m_pos.y + wRc.Height() > screenY)	m_pos.y = screenY - wRc.Height() - 5;
	SetWindowPos(NULL, m_pos.x, m_pos.y, wRc.Width(), wRc.Height(), SWP_NOZORDER);
*/	
	((CButton *) GetDlgItem(IDC_AFTERCLOSE))->SetCheck(1);
	if (!m_axiscall)	GetDlgItem(IDC_AFTERCLOSE)->EnableWindow(FALSE);
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_TREEICON);
	m_Tree.InitTreeImage(bitmap);

	initList();
	setTree();
	setList();
	enableButton(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrSearch::initList()
{
	CString hData[2];
	hData[0].LoadString(ST_TSETUP_TEXT_TR);
	hData[1].LoadString(ST_TSETUP_TEXT_MAPNAME);

	CClientDC	dc(this);
	CRect		cRc;
	int		cW[2];

	m_List.GetClientRect(cRc);
	cW[0] = dc.GetTextExtent(" 9999 ").cx;
	cW[1] = cRc.Width() - cW[0];

	m_List.InitHeaderData(hData, cW, 2, LVCFMT_LEFT);
	m_List.SetFullRowSelect();
	m_List.HeaderFix();
	m_List.SetDisableSort();
}

void CTrSearch::setCount(int cnt, int tot)
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

void CTrSearch::treeSelect(int idx)
{
	if (idx >= m_arItem.GetSize())
		return;
	m_Tree.SelectItem(m_arItem.GetAt(idx));
}

void CTrSearch::listSelect(int idx)
{
	if (idx >= m_List.GetItemCount())
		return;

	SCROLLINFO	info;
	CSize		sz;
	CRect		rc;

	m_List.SetItemState(idx, LVIS_SELECTED, LVIS_SELECTED);
	m_List.GetScrollInfo(SB_VERT, &info, SIF_ALL);
	int pos = idx - (int)info.nPos;
	m_List.GetItemRect(0, rc, LVIR_BOUNDS);
	sz.cy = rc.Height()*pos;
	m_List.Scroll(sz);
}

void CTrSearch::enableButton(bool bEnable)
{
	CButton* before = (CButton *)GetDlgItem(IDC_BEFORE);
	before->EnableWindow(bEnable);
	CButton* after = (CButton *)GetDlgItem(IDC_AFTER);
	after->EnableWindow(bEnable);
}

void CTrSearch::fit()
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

void CTrSearch::setTree()
{
	m_Tree.DeleteAllItems();

	char	buf[1024];
	CString	dat;

	FILE *fp = fopen(GetProfileFileName(pkAxisMenu), "r");
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

void CTrSearch::setList(BOOL flag)
{
	UpdateData(TRUE);
	CString desc, com, comS = m_Edit;//m_aEdit;
	UpdateData(FALSE);

	m_List.DeleteAllItems();
	m_arTr.RemoveAll();
	comS.TrimRight();
	for (int ii = 0; ii < m_arMap.GetSize() && !comS.IsEmpty(); ii++)
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

	if (m_List.GetItemCount())
	{
		m_List.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
		m_List.SetScrollPos(SB_VERT, 0, TRUE);
	}

	if (!flag)	fit();
}

void CTrSearch::make_Tree(CString dat)
{
	dat.Replace("\"", "");
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

void CTrSearch::findItem(HTREEITEM item, CString comS)
{
	CString		com;
	HTREEITEM	sItem;

	do
	{
		com = m_Tree.GetItemText(item);
		if (com.Find(comS) != -1)
		{
			if (!m_Tree.ItemHasChildren(item))
			{
				m_arItem.Add(item);
				m_cnt++;
			}
		}

		if (!m_Tree.ItemHasChildren(item))
			continue;
		sItem = m_Tree.GetNextItem(item, TVGN_CHILD);
		findItem(sItem,  comS);

	} while ((item = m_Tree.GetNextItem(item, TVGN_NEXT)) != NULL);
}

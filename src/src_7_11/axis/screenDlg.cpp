// screenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "axmsg.hxx"
#include "screenDlg.h"

#include "../h/axisvar.h"
//#include "../h/shs_msg.h"

#include <imm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreenDlg dialog


CScreenDlg::CScreenDlg(CWnd* pParent /*=NULL*/, CString root)
	: CDialog(CScreenDlg::IDD, pParent)
{
	m_parent = pParent;
	m_root = root;
	m_changed = false;
	m_select  = -1;

	m_colName.LoadString(ST_TEXT_USERWORKSPACELIST);
	CWinApp* app = AfxGetApp();
	m_user = app->GetProfileString(WORKSTATION, SIGNONID);
	int pos = m_user.Find('|');
//	if (pos < 0)	
//		m_user = _T("Anonymous");
//	else		
	m_user = m_user.Mid(pos+1);
//	if (m_user.IsEmpty())	
//		m_user = _T("Anonymous");
	//{{AFX_DATA_INIT(CScreenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreenDlg)
	DDX_Control(pDX, IDC_ALIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreenDlg, CDialog)
	//{{AFX_MSG_MAP(CScreenDlg)
	ON_BN_CLICKED(IDC_ADD, OnADD)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(NM_CLICK, IDC_ALIST, OnClickAlist)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenDlg message handlers

BOOL CScreenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect	rc;

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_list.GetClientRect(rc);
	addColumn(m_colName, 0, rc.Width());

	fillList();

	DWORD	dwConversion, dwSentence;
	HIMC hImc = ImmGetContext(this->m_hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
		ImmSetConversionStatus(hImc, dwConversion|IME_CMODE_HANGEUL, dwSentence);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScreenDlg::OnADD() 
{
	CString	mapN, tmps, msg, title;
	int ii = 0;
	GetDlgItem(IDC_SNAME)->GetWindowText(mapN);
	mapN.TrimLeft();
	mapN.TrimRight();
	if (mapN.IsEmpty())
	{
		title.LoadString(ST_TEXT_OK);
		msg.LoadString(ST_MSG_INPUT_SCREENNAME);
		::MessageBox(m_hWnd, msg, title, MB_ICONINFORMATION);
		return;
	}

	for (ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		tmps = m_list.GetItemText(ii, 0);
		if (!mapN.CompareNoCase(tmps))
		{
			title.LoadString(ST_TEXT_OK);
			msg.LoadString(ST_MSG_CHANGE_WORKSPACENAME);
			if (MessageBox(msg, title, MB_YESNO) == IDNO)
			{
				GetDlgItem(IDC_SNAME)->SetFocus();
				return;
			}
			else
			{
				m_list.DeleteItem(ii);
				break;
			}
		}
	}

	int	index = ii;
	tmps.Format("%02d=%s", index+1, mapN);
	if (m_parent->SendMessage(WM_AXIS, MAKEWPARAM(axSCREEN, 0), (LPARAM)(char *)tmps.operator LPCTSTR()))
	{
		addItem(mapN, index, 0);
		m_changed = true;
		if (m_list.GetItemCount() > 8)
		{
			LV_COLUMN col;
			CRect	rc;
			col.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
			col.fmt = LVCFMT_LEFT;
			col.pszText = (char *) m_colName.operator LPCTSTR();
			col.iSubItem = 0;
			m_list.GetClientRect(rc);
			col.cx = rc.Width();
			m_list.SetColumn(0, &col);
		}
	}	
}

void CScreenDlg::OnDelete() 
{
	if (m_select < 0 || m_select >= m_list.GetItemCount())
		return;

	CString	tmps;

	tmps.Format("%02d", m_select+1);
	if (m_parent->SendMessage(WM_AXIS, MAKEWPARAM(axSCREEN, 1), (LPARAM)(char *)tmps.operator LPCTSTR()))
	{
		m_list.DeleteItem(m_select);
		m_changed = true;

		if (m_list.GetItemCount() < 9)
		{
			LV_COLUMN col;
			CRect	rc;
			col.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
			col.fmt = LVCFMT_LEFT;
			col.pszText = (char *) m_colName.operator LPCTSTR();
			col.iSubItem = 0;
			m_list.GetClientRect(rc);
			col.cx = rc.Width();
			m_list.SetColumn(0, &col);
		}
	}
}

BOOL CScreenDlg::addColumn(LPCTSTR columns, int col, int width)
{
	LV_COLUMN lvc;

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR)columns;
	lvc.cx	     = width;
	lvc.iSubItem = 0;

	return m_list.InsertColumn(col, &lvc);
}

BOOL CScreenDlg::addItem(CString items, int item, int subi)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subi;
	lvi.pszText  = (LPTSTR)items.operator LPCTSTR();

	if (!subi)
		return m_list.InsertItem(&lvi);

	return m_list.SetItem(&lvi);
}

void CScreenDlg::OnOK() 
{
	if (m_changed)
		CDialog::OnOK();
	else
		CDialog::OnCancel();
}

void CScreenDlg::fillList()
{
	char	wb[1024];
	CString	file, tmps, tmpx;
	
	tmps.Format("%s\\%s\\%s\\%s.ini", m_root, USRDIR, m_user, m_user);
	file.Format("%s\\%s\\%s\\%s.tmp", m_root, USRDIR, m_user, m_user);
	CopyFile(tmps, file, FALSE);

	m_list.DeleteAllItems();
	CMapStringToString	ary;
	for (int ii = 1; ; ii++)
	{
		tmps.Format("%02d", ii);
		DWORD dwRc = GetPrivateProfileString("ROOT", tmps, "", wb, sizeof(wb), file);
		if (dwRc <= 0)
			break;

		tmps = wb;
		if (ary.Lookup(tmps, tmpx))
			continue;
		ary.SetAt(tmps, "1");
		tmps.TrimLeft();	tmps.TrimRight();
		addItem(tmps, ii-1, 0);
	}
	ary.RemoveAll();

	if (m_list.GetItemCount() > 8)
	{
		LV_COLUMN col;
		CRect	rc;
		col.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
		col.fmt = LVCFMT_LEFT;
		col.pszText = (char *) m_colName.operator LPCTSTR();
		col.iSubItem = 0;
		m_list.GetClientRect(rc);
		col.cx = rc.Width();
		m_list.SetColumn(0, &col);
	}
}

void CScreenDlg::OnClickAlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pListView = (NM_LISTVIEW *)pNMHDR;
	*pResult = 0;
	if (pListView->iItem < 0 || pListView->iItem >= m_list.GetItemCount())
		return;

	m_select = pListView->iItem;
	GetDlgItem(IDC_SNAME)->SetWindowText(m_list.GetItemText(m_select, 0));
	*pResult = 0;
}

void CScreenDlg::OnUp() 
{
	if (m_list.GetSelectedCount() != 1)
		return; 

	int sel = -1;
	CString moveItem1, moveItem2, itemData = _T("");

	sel = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (sel == 0)	return;
	moveItem1 = m_list.GetItemText(sel, 0);
	moveItem2 = m_list.GetItemText(sel, 1);
	m_list.DeleteItem(sel);

	sel--;
	m_list.InsertItem(sel, moveItem1);
	m_list.SetItemText(sel, 1, moveItem2);
	m_list.SetItemState(sel, LVIS_SELECTED, LVIS_SELECTED);
	
	m_changed = true;
	SaveOrder();
}

void CScreenDlg::OnDown() 
{
	if (m_list.GetSelectedCount() != 1)
		return; 

	int sel = -1;
	CString moveItem1, moveItem2, itemData = _T("");

	sel = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (sel == m_list.GetItemCount()-1)	return;

	moveItem1 = m_list.GetItemText(sel, 0);
	moveItem2 = m_list.GetItemText(sel, 1);
	m_list.DeleteItem(sel);

	sel++;
	m_list.InsertItem(sel, moveItem1);
	m_list.SetItemText(sel, 1, moveItem2);
	m_list.SetItemState(sel, LVIS_SELECTED, LVIS_SELECTED);

	m_changed = true;
	SaveOrder();
}

void CScreenDlg::SaveOrder()
{
	CString file, key, dat;

	file.Format("%s\\%s\\%s\\%s.tmp", m_root, USRDIR, m_user, m_user);
	WritePrivateProfileSection("ROOT", "", file);

	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		key.Format("%02d", ii+1);
		dat = m_list.GetItemText(ii, 0);
	
		WritePrivateProfileString("ROOT", key, dat, file);
	}
}

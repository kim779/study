// passdlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "passdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPassDlg dialog


CPassDlg::CPassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPassDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPassDlg)
	//}}AFX_DATA_INIT
	m_select = -1;
	m_acs.RemoveAll();
}

CPassDlg::~CPassDlg()
{
	for (int ii = 0; ii < m_acs.GetSize(); ii++)
		delete m_acs.GetAt(ii);
	m_acs.RemoveAll();
}

void CPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPassDlg)
	DDX_Control(pDX, IDC_ALIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPassDlg, CDialog)
	//{{AFX_MSG_MAP(CPassDlg)
	ON_BN_CLICKED(IDC_CHANGE, OnSave)
	ON_NOTIFY(NM_CLICK, IDC_ALIST, OnListClick)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPassDlg message handlers
BOOL CPassDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

#ifdef DF_USE_CPLUS17
	m_changer = std::make_unique<CControlChanger>(this);
#else
	m_changer = new CControlChanger(this);
#endif

	((CEdit *)GetDlgItem(IDC_SPASS))->LimitText(8);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CString	str;
	str.LoadString(ST_TEXT_ACCNAME);
	addColumn(str, 0, 40);
	str.LoadString(ST_TEXT_ACCOUNTNO);
	addColumn(str, 1, 40);
	str.LoadString(ST_TEXT_ACCPASS);
	addColumn(str, 2, 40);
	
	m_list.DeleteAllItems();
	for (int ii = 0; ii < m_acs.GetSize(); ii++)
	{
		const Caccount* ac = m_acs.GetAt(ii);
		addItem(ac->m_name, ii, 0);
		addItem(editAccount(ac->m_account), ii, 1);
		addItem(ac->m_pass, ii, 2, true);
	}

	if (m_list.GetItemCount() > 0)
	{
		m_select = 0;
		setAccountData(m_select);
		m_list.SetFocus();

		LV_ITEM	lvi;

		lvi.mask      = LVIF_TEXT|LVIF_STATE;
		lvi.iItem     = m_select;
		lvi.iSubItem  = 0;
		lvi.pszText   = (char*)(const char*)m_acs.GetAt(m_select)->m_name;
		lvi.stateMask = LVIS_FOCUSED|LVIS_SELECTED;
		lvi.state     = LVIS_FOCUSED|LVIS_SELECTED;

		m_list.SetItem(&lvi);
	}
	else
	{
		str.LoadString(ST_MSG_NOACCINFO);
		setStatus(str);
	}

	fit();
	((CEdit *)GetDlgItem(IDC_SPASS))->LimitText(8);
	GotoDlgCtrl(GetDlgItem(IDC_SPASS));
	return FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPassDlg::OnSave()
{
	if (m_select < 0 || m_acs.GetSize() <= 0)
		return;

	CString	tmps = _T("");
	GetDlgItem(IDC_SPASS)->GetWindowText(tmps);
	m_acs.GetAt(m_select)->m_pass = tmps;
	if (addItem(tmps, m_select, 2, true))
	{
		tmps.LoadString(ST_MSG_SAVEACCOK);
		setStatus(tmps);
	}
	else
	{
		tmps.LoadString(ST_MSG_SAVEACCFAIL);
		setStatus(tmps);
	}
}

void CPassDlg::OnListClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	const NMLISTVIEW* pListView = (NMLISTVIEW *)pNMHDR;
	if (pListView->iItem < 0 || pListView->iItem >= m_acs.GetSize())
		return;

	m_select = pListView->iItem;
	setAccountData(m_select);
	GotoDlgCtrl(GetDlgItem(IDC_SPASS));
}

BOOL CPassDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_UP:
		case VK_DOWN:
			if (m_list.GetItemCount() <= 0 || pMsg->hwnd != GetDlgItem(IDC_ALIST)->m_hWnd)
				break;

			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			if (pMsg->wParam == VK_UP)
				m_select = max(0, m_select-1);
			else
				m_select = min(m_acs.GetUpperBound(), m_select+1);
			setAccountData(m_select);
			return TRUE;

		case VK_RETURN:
			if (pMsg->hwnd == GetDlgItem(IDC_ALIST)->m_hWnd)
			{
				::TranslateMessage(pMsg);
				::DispatchMessage(pMsg);

				CEdit*	epass = (CEdit *)GetDlgItem(IDC_SPASS);
				epass->SetFocus();
				epass->SetSel(0, -1);
				return TRUE;
			}
			else if (pMsg->hwnd == GetDlgItem(IDC_SPASS)->m_hWnd)
			{
				::TranslateMessage(pMsg);
				::DispatchMessage(pMsg);
				OnSave();
				if (m_select < m_acs.GetSize()-1)
				{
					m_select++;
					setAccountData(m_select);
					LV_ITEM	lvi;

					lvi.mask      = LVIF_TEXT|LVIF_STATE;
					lvi.iItem     = m_select;
					lvi.iSubItem  = 0;
					lvi.pszText   = (LPTSTR)(const char*)m_acs.GetAt(m_select)->m_name;
					lvi.stateMask = LVIS_FOCUSED|LVIS_SELECTED;
					lvi.state     = LVIS_FOCUSED|LVIS_SELECTED;
					m_list.SetItem(&lvi);
					GetDlgItem(IDC_ALIST)->SetFocus();
					GotoDlgCtrl(GetDlgItem(IDC_SPASS));
				}
				else
					GotoDlgCtrl(GetDlgItem(IDOK));

				return TRUE;
			}
			else if (pMsg->hwnd == GetDlgItem(IDOK)->m_hWnd)
				return CDialog::PreTranslateMessage(pMsg);

		case VK_ESCAPE:
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
#pragma warning (disable : 26400 )
#pragma warning (disable : 26409 )
void CPassDlg::SetData(CString data)
{
	int	pos{};
	Caccount* acc{};
	CString	tmps, tmpx;

	m_acs.RemoveAll();
	for ( ; !data.IsEmpty(); )
	{
		pos = data.Find('\n');
		if (pos < 0)
			break;

		tmps = data.Left(pos);
		data = (data.GetLength() > pos+1) ? data.Mid(pos+1) : _T("");

		acc = new Caccount;
		for (int ii = 0; ii < 3; ii++)
		{
			pos = tmps.Find('\t');
			if (pos >= 0)
			{
				tmpx = tmps.Left(pos);
				tmps = (tmps.GetLength() > pos+1) ? tmps.Mid(pos+1) : _T("");
			}
			else
			{
				tmpx = tmps;
				tmps = _T("");
			}

			tmpx.TrimRight();
			switch (ii)
			{
			case 0:
				acc->m_account = tmpx;	break;
			case 1:
				acc->m_pass = tmpx;	break;
			case 2:
				acc->m_name = tmpx;	break;
			}
		}

		if (acc->m_account.GetLength() != 10)
		{
			delete acc;
			continue;
		}
		m_acs.Add(acc);
	}
}
#pragma warning (default : 26400 )
#pragma warning (default : 26409 )

bool CPassDlg::setAccountData(int sel)
{
	if (sel < 0 || sel >= m_acs.GetSize())
		return false;

	GetDlgItem(IDC_ANAME)->SetWindowText(m_acs.GetAt(sel)->m_name);
	GetDlgItem(IDC_ANUM)->SetWindowText(editAccount(m_acs.GetAt(sel)->m_account));
	GetDlgItem(IDC_SPASS)->SetWindowText(m_acs.GetAt(sel)->m_pass);
	return true;
}

void CPassDlg::setStatus(CString status)
{
	GetDlgItem(IDC_STATUS)->SetWindowText(status);
}

CString CPassDlg::editAccount(CString account)
{
	CString	tmps = account;

	tmps.Insert(8, '-');
	//tmps.Insert(3, '-');

	return tmps;
}

BOOL CPassDlg::addColumn(LPCTSTR columns, int col, int width)
{
	LV_COLUMN lvc;

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR)columns;
	lvc.cx       = m_list.GetStringWidth(lvc.pszText) + width;
	lvc.iSubItem = -1;

	return m_list.InsertColumn(col, &lvc);
}

BOOL CPassDlg::addItem(CString items, int item, int subi, bool press)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subi;
	if (press)
		items = CString('*', items.GetLength());
	lvi.pszText  = (LPTSTR)(const char*)items;

	if (!subi)
		return m_list.InsertItem(&lvi);

	return m_list.SetItem(&lvi);
}

void CPassDlg::fit()
{
	if (m_list.GetSafeHwnd())
	{
		int		W0{}, W1{}, fit{};
		CRect		cRc;

		m_list.GetClientRect(cRc);
		W0 = m_list.GetColumnWidth(0);
		W1 = m_list.GetColumnWidth(1);

		fit = cRc.Width() - W0 - W1;
		m_list.SetColumnWidth(2, fit);
	}
}

void CPassDlg::OnUp() 
{
	if (m_list.GetSelectedCount() != 1)
		return;
	int idx = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (idx == 0)	return;

	Caccount* acc = m_acs.GetAt(idx);
	m_acs.RemoveAt(idx);
	m_list.DeleteItem(idx);
	idx--;

	addItem(acc->m_name, idx, 0);
	addItem(editAccount(acc->m_account), idx, 1);
	addItem(acc->m_pass, idx, 2, true);

	m_acs.InsertAt(idx, acc);
	m_list.SetItemState(idx, LVIS_SELECTED, LVIS_SELECTED);

	m_select--;
}
/*
 	for (int ii = 0; ii < m_acs.GetSize(); ii++)
	{
		Caccount* ac = m_acs.GetAt(ii);
		addItem(ac->m_name, ii, 0);
		addItem(editAccount(ac->m_account), ii, 1);
		addItem(ac->m_pass, ii, 2, true);
	}
 */
void CPassDlg::OnDown() 
{
/*	if (m_list.GetSelectedCount() != 1)
		return; 

	int sel = -1, cnt = m_list.GetColumns();
	CStringArray ary;

	sel = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (sel == m_list.GetItemCount()-1)	return;

	for (int ii = 0; ii < cnt; ii++)
		ary.Add(m_list.GetItemText(sel, ii));
	int state = m_list.GetCheckbox(sel, 0);
	int nextstate = m_list.GetCheckbox(sel+1, 0);

	Caccount* acc = m_acs.GetAt(sel);
	m_acs.RemoveAt(sel);
	m_list.DeleteItem(sel);
	m_list.SetCheckbox(sel, 0, nextstate);

	sel++;
	for (ii = 0; ii < ary.GetSize(); ii++)
	{
		if (ii == COL_ACCPASS)
			addItem(ary.GetAt(ii), sel, ii, true);
		else	addItem(ary.GetAt(ii), sel, ii, false, state);
	}
	m_acs.InsertAt(sel, acc);
	m_list.SetItemState(sel, LVIS_SELECTED, LVIS_SELECTED);
	
	m_select++;
	ary.RemoveAll();*/	
}

BOOL CPassDlg::OnEraseBkgnd(CDC* pDC) 
{
	if (Axis::dialogBgColor > 0)
	{
		CRect rc;
		GetClientRect(&rc);
		
CBrush* oldBrush = pDC->SelectObject(&Axis::dialogBgBrush);

		pDC->PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(oldBrush);

		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

HBRUSH CPassDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CPassDlg::OnDestroy() 
{
	CDialog::OnDestroy();
#ifndef DF_USE_CPLUS17
	delete m_changer;	
#endif
}

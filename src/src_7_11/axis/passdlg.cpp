// PassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "PassDlg.h"
#include "axMsg.hxx"

#define TM_PASS		333
#define TMI_PASS	3000
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

	m_mainframe	= pParent;
	m_select	= -1;
	m_bNext		= false;
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
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_AXIS, OnCheckPass)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPassDlg message handlers

BOOL CPassDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	((CEdit *)GetDlgItem(IDC_PASS))->LimitText(8);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	addColumn("계 좌 명", 0, 50);
	addColumn("계좌번호", 1, 50);
	addColumn("비밀번호", 2, 20);
	
	m_list.DeleteAllItems();
	for (int ii = 0; ii < m_acs.GetSize(); ii++)
	{
		Caccount* ac = m_acs.GetAt(ii);
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
		lvi.pszText   = (LPTSTR)m_acs.GetAt(m_select)->m_name.operator LPCTSTR();
		lvi.stateMask = LVIS_FOCUSED|LVIS_SELECTED;
		lvi.state     = LVIS_FOCUSED|LVIS_SELECTED;

		m_list.SetItem(&lvi);
	}
	else
		setStatus(_T("계좌정보가 없습니다."));

	fit();
	((CEdit *)GetDlgItem(IDC_PASS))->LimitText(4);
	GotoDlgCtrl(GetDlgItem(IDC_PASS));
	return FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPassDlg::OnSave()
{
	if (m_select < 0 || m_acs.GetSize() <= 0)
		return;

	CString	tmps = _T("");
	GetDlgItem(IDC_PASS)->GetWindowText(tmps);
	m_acs.GetAt(m_select)->m_pass = tmps;

	sprintf_s(m_sndB, sizeof(m_sndB), "%10s%4s", m_acs.GetAt(m_select)->m_account, tmps);
	m_mainframe->PostMessage(WM_AXIS, MAKEWPARAM(axPWDCHK, 0), (long) m_sndB);
	EnableWindow(false);
	SetTimer(TM_PASS, TMI_PASS, NULL);
/*	if (addItem(tmps, m_select, 2, true))
		setStatus(_T("비밀번호가 저장되었습니다"));
	else
		setStatus(_T("비밀번호 저장에 실패하였습니다"));
*/
}

void CPassDlg::OnListClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	NMLISTVIEW* pListView = (NMLISTVIEW *)pNMHDR;
	if (pListView->iItem < 0 || pListView->iItem >= m_acs.GetSize())
		return;

	m_select = pListView->iItem;
	setAccountData(m_select);
	GotoDlgCtrl(GetDlgItem(IDC_PASS));
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
				m_select = (int)min(m_acs.GetUpperBound(), m_select+1);
			setAccountData(m_select);
			return TRUE;

		case VK_RETURN:
			if (pMsg->hwnd == GetDlgItem(IDC_ALIST)->m_hWnd)
			{
				::TranslateMessage(pMsg);
				::DispatchMessage(pMsg);

				CEdit*	epass = (CEdit *)GetDlgItem(IDC_PASS);
				epass->SetFocus();
				epass->SetSel(0, -1);
				return TRUE;
			}
			else if (pMsg->hwnd == GetDlgItem(IDC_PASS)->m_hWnd)
			{
				::TranslateMessage(pMsg);
				::DispatchMessage(pMsg);

//				CString	pwdS;
//				GetDlgItem(IDC_PASS)->GetWindowText(pwdS);

//				if (!pwdS.IsEmpty())
//				{
					OnSave();
					m_bNext = true;
	/*				if (m_select < m_acs.GetSize()-1)
					{
						m_select++;
						setAccountData(m_select);
						LV_ITEM	lvi;

						lvi.mask      = LVIF_TEXT|LVIF_STATE;
						lvi.iItem     = m_select;
						lvi.iSubItem  = 0;
						lvi.pszText   = (LPTSTR)m_acs.GetAt(m_select)->m_name.operator LPCTSTR();
						lvi.stateMask = LVIS_FOCUSED|LVIS_SELECTED;
						lvi.state     = LVIS_FOCUSED|LVIS_SELECTED;
						m_list.SetItem(&lvi);
						GetDlgItem(IDC_ALIST)->SetFocus();
						GotoDlgCtrl(GetDlgItem(IDC_PASS));
					}
					else
						GotoDlgCtrl(GetDlgItem(IDOK));
	*/
					return TRUE;
//				}
//				else	GotoDlgCtrl(GetDlgItem(IDOK));
			}
			else if (pMsg->hwnd == GetDlgItem(IDOK)->m_hWnd)
				return CDialog::PreTranslateMessage(pMsg);
			break;
		case VK_ESCAPE:
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CPassDlg::SetData(CString data)
{
	int	pos;
	Caccount* acc;
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

bool CPassDlg::setAccountData(int sel)
{
	if (sel < 0 || sel >= m_acs.GetSize())
		return false;

	GetDlgItem(IDC_ANAME)->SetWindowText(m_acs.GetAt(sel)->m_name);
	GetDlgItem(IDC_ANUM)->SetWindowText(editAccount(m_acs.GetAt(sel)->m_account));
	GetDlgItem(IDC_PASS)->SetWindowText(m_acs.GetAt(sel)->m_pass);
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
	lvi.pszText  = (LPTSTR)items.operator LPCTSTR();

	if (!subi)
		return m_list.InsertItem(&lvi);

	return m_list.SetItem(&lvi);
}

void CPassDlg::fit()
{
	if (m_list.GetSafeHwnd())
	{
		int		W0, W1, fit;
		CRect		cRc;

		m_list.GetClientRect(cRc);
		W0 = m_list.GetColumnWidth(0);
		W1 = m_list.GetColumnWidth(1);

		fit = cRc.Width() - W0 - W1;
		m_list.SetColumnWidth(2, fit);
	}
}

void CPassDlg::OnTimer(UINT_PTR nIDEvent) 
{
	if (nIDEvent == TM_PASS)
	{
		KillTimer(TM_PASS);
		setStatus(_T("비밀번호확인 오류입니다."));
		EnableWindow(true);
	}
	CDialog::OnTimer(nIDEvent);
}

LRESULT CPassDlg::OnCheckPass(WPARAM wp, LPARAM lp)
{
	KillTimer(TM_PASS);
	EnableWindow(true);

	if (m_sndB[0] == '0')
	{
		addItem(m_acs.GetAt(m_select)->m_pass, m_select, 2, true);
		setStatus(_T("비밀번호가 저장되었습니다"));
	}
	else
	{
		addItem("", m_select, 2, true);
		setStatus(&m_sndB[1]);
		m_acs.GetAt(m_select)->m_pass = "";

		setAccountData(m_select);
		LV_ITEM	lvi;

		lvi.mask      = LVIF_TEXT|LVIF_STATE;
		lvi.iItem     = m_select;
		lvi.iSubItem  = 0;
		lvi.pszText   = (LPTSTR)m_acs.GetAt(m_select)->m_name.operator LPCTSTR();
		lvi.stateMask = LVIS_FOCUSED|LVIS_SELECTED;
		lvi.state     = LVIS_FOCUSED|LVIS_SELECTED;
		m_list.SetItem(&lvi);
		GetDlgItem(IDC_ALIST)->SetFocus();
		GotoDlgCtrl(GetDlgItem(IDC_PASS));
		return 0;
	}

	if (m_bNext)
	{
		if (m_select < m_acs.GetSize()-1)
		{
			m_select++;
			setAccountData(m_select);
			LV_ITEM	lvi;

			lvi.mask      = LVIF_TEXT|LVIF_STATE;
			lvi.iItem     = m_select;
			lvi.iSubItem  = 0;
			lvi.pszText   = (LPTSTR)m_acs.GetAt(m_select)->m_name.operator LPCTSTR();
			lvi.stateMask = LVIS_FOCUSED|LVIS_SELECTED;
			lvi.state     = LVIS_FOCUSED|LVIS_SELECTED;
			m_list.SetItem(&lvi);
			m_list.EnsureVisible(m_select, TRUE); 
			GetDlgItem(IDC_ALIST)->SetFocus();
			GotoDlgCtrl(GetDlgItem(IDC_PASS));
		}
		else
			GotoDlgCtrl(GetDlgItem(IDOK));
	}

	return 0;
}

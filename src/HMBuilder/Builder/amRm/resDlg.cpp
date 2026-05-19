// resDlg.cpp : implementation file
//

#include "stdafx.h"
#include "amRm.h"
#include "resDlg.h"
#include "LoginDlg.h"
#include "transDlg.h"
#include "MoveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResDlg dialog


CResDlg::CResDlg(int mode, CWnd* pParent /*=NULL*/)
	: CDialog(CResDlg::IDD, pParent)
{
	m_select = -1;
	m_mode   = mode;
	m_kind   = kindFORMB;
	m_uid = _T("");
	m_desc = _T("");

	//{{AFX_DATA_INIT(CResDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CResDlg::~CResDlg()
{
	m_files.RemoveAll();
}

void CResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResDlg)
	DDX_Control(pDX, IDC_ELIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResDlg, CDialog)
	//{{AFX_MSG_MAP(CResDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_CBN_SELCHANGE(IDC_P1COMBO, OnComboSelChangeP1)
	ON_CBN_SELCHANGE(IDC_P2COMBO, OnComboSelChangeP2)
	ON_BN_CLICKED(IDC_DEL, OnDelete)
	ON_BN_CLICKED(IDC_TRANS, OnTrans)
	ON_NOTIFY(NM_CLICK, IDC_ELIST, OnListClick)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_P1, IDC_P2, OnKind)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResDlg message handlers

BOOL CResDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	addColumn(_T("File"), 0, 290);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	fillList(true);

	m_kind = kindFORMB;
	switch (m_mode)
	{
	case modeADMIN:
		((CButton *)GetDlgItem(IDC_P1))->SetCheck(1);
		((CComboBox *)GetDlgItem(IDC_P1COMBO))->SetCurSel(0);
		((CComboBox *)GetDlgItem(IDC_P2COMBO))->SetCurSel(0);
		break;

	case modeUSER:
	default:
		(((CButton *)GetDlgItem(IDC_P1)))->EnableWindow(FALSE);
		((CComboBox *)GetDlgItem(IDC_P1COMBO))->EnableWindow(FALSE);

		((CButton *)GetDlgItem(IDC_P2))->SetCheck(1);
		((CComboBox *)GetDlgItem(IDC_P2COMBO))->SetCurSel(0);
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResDlg::OnAdd() 
{
	CFileDialog fDlg(TRUE, NULL, NULL, OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,
				"AXIS Files (*.*)|*.*||");

	const	int	maxFiles = 1024*16;
	fDlg.m_ofn.lpstrFile = new char[maxFiles];
	fDlg.m_ofn.nMaxFile  = maxFiles;
	ZeroMemory(fDlg.m_ofn.lpstrFile, maxFiles);

	if (fDlg.DoModal() != IDOK)
	{
		delete[] fDlg.m_ofn.lpstrFile;
		return;
	}

	POSITION pos = fDlg.GetStartPosition();
	if (pos == NULL)
	{
		delete[] fDlg.m_ofn.lpstrFile;
		return;
	}

	CString	tmps;
	int	ii;
	while (pos != NULL)
	{
		tmps = fDlg.GetNextPathName(pos);
		if (!tmps.IsEmpty())
		{
			for (ii = 0; ii < m_files.GetSize(); ii++)
			{
				if (!tmps.CompareNoCase(m_files.GetAt(ii)))
					break;
			}

			if (ii >= m_files.GetSize())
				m_files.Add(tmps);
		}
	}

	if (m_files.GetSize() > 0)
		fillList();

	delete[] fDlg.m_ofn.lpstrFile;
}

void CResDlg::OnComboSelChangeP1() 
{
	int	kind = m_kind;

	m_kind = ((CComboBox *)GetDlgItem(IDC_P1COMBO))->GetCurSel()+1;
	if (kind != m_kind)
	{
		m_list.DeleteAllItems();
		m_files.RemoveAll();
		m_select = -1;
	}
}

void CResDlg::OnComboSelChangeP2() 
{
	int	kind = m_kind;

	m_kind = ((CComboBox *)GetDlgItem(IDC_P2COMBO))->GetCurSel()+1;
	if (kind != m_kind)
	{
		m_list.DeleteAllItems();
		m_files.RemoveAll();
		m_select = -1;
	}
}

void CResDlg::OnDelete() 
{
	if (m_select < 0)
	{
		AfxMessageBox("Invalid Item!");
		return;
	}

	m_files.RemoveAt(m_select);
	m_select = -1;
	fillList();
}

void CResDlg::OnTrans() 
{
	if (m_mode == modeADMIN	&& ((CButton *)GetDlgItem(IDC_P1))->GetCheck())
	{
		if (m_uid.IsEmpty())
		{
			CLoginDlg loginDlg(this);
			loginDlg.DoModal();

			if (!loginDlg.m_loginOK)
				return;

			m_uid = loginDlg.m_userid;
			m_desc = loginDlg.m_desc;
		}

		CTransDlg transDlg(this, m_kind, m_uid, m_desc);
		transDlg.m_files.Copy(m_files);
		transDlg.DoModal();
		return;
	}
	
	if (((CButton *)GetDlgItem(IDC_P2))->GetCheck())
	{
		CMoveDlg moveDlg(this, m_kind);
		moveDlg.m_files.Copy(m_files);
		moveDlg.DoModal();
	}
}

void CResDlg::OnListClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pListView = (NM_LISTVIEW *) pNMHDR;
	*pResult = 0;

	if (pListView->iItem < 0 || pListView->iItem >= m_files.GetSize())
	{
		m_select = -1;
		return;
	}
	
	m_select = pListView->iItem;
}

void CResDlg::OnKind(UINT cmdID)
{
	int kind = m_kind;
	switch (cmdID)
	{
	case IDC_P1:
		m_kind = ((CComboBox *)GetDlgItem(IDC_P1COMBO))->GetCurSel()+1;
		break;
	case IDC_P2:
		m_kind = ((CComboBox *)GetDlgItem(IDC_P2COMBO))->GetCurSel()+1;
		break;
	}

	if (kind != m_kind)
	{
		m_list.DeleteAllItems();
		m_files.RemoveAll();
		m_select = -1;
	}
}

void CResDlg::fillList(bool reset)
{
	if (reset)
	{
		m_files.RemoveAll();
		m_select = -1;
	}

	m_list.DeleteAllItems();
	for (int ii = 0; ii < m_files.GetSize(); ii++)
		addItem(m_files.GetAt(ii), ii, 0);

	if (m_files.GetSize() <= 0)
	{
		GetDlgItem(IDC_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_TRANS)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_TRANS)->EnableWindow(TRUE);
	}
}

BOOL CResDlg::addColumn(CString columns, int col, int width)
{
	LV_COLUMN lvc;

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR)columns.operator LPCTSTR();
	lvc.cx       = m_list.GetStringWidth(lvc.pszText) + width;
	lvc.iSubItem = -1;

	return m_list.InsertColumn(col, &lvc);
}

BOOL CResDlg::addItem(CString items, int item, int subi)
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
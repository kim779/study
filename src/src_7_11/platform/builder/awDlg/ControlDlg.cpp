// ControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDlg.h"
#include "../mainvar.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlDlg dialog


CControlDlg::CControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CControlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CControlDlg)
	m_desc = _T("");
	m_input = _T("");
	//}}AFX_DATA_INIT
}


void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CControlDlg)
	DDX_Control(pDX, IDC_NAMELIST, m_list);
	DDX_Text(pDX, IDC_DESC, m_desc);
	DDX_Text(pDX, IDC_INPUT, m_input);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CControlDlg, CDialog)
	//{{AFX_MSG_MAP(CControlDlg)
	ON_NOTIFY(NM_CLICK, IDC_NAMELIST, OnClickNamelist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_NAMELIST, OnItemchangedNamelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlDlg message handlers

BOOL CControlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	DWORD dwStyle = m_list.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_list.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);

	addColumn(&m_list, "Name", 0, 36);
	addColumn(&m_list, "Symbol", 1, 18);
	addColumn(&m_list, "Description", 2, 36);
	m_beforeIdx = 0;
	LoadFile();

	CRect rc, listRc;
	GetClientRect(&rc);
	m_list.GetClientRect(&listRc);

	if (!m_singleProperty.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_TABSTOP, CRect(6, listRc.Height() + 30, rc.right - 7, rc.bottom - 35), this, 110))
		return -1;

	m_singleProperty.EraseCtrls();
	m_singleProperty.ResetGrid();
	m_singleProperty.Initialize("");
	m_singleProperty.setHeadInfo(m_beforeIdx);
	
	if (m_dataArr.GetSize() <= m_beforeIdx)
	{
		AfxMessageBox("File not found \"control.ini\"");
		PostMessage(WM_CLOSE);
	}
	else
	{
		m_dataArr.SetAt(m_beforeIdx, m_singleProperty.convertData(m_ctrlParam));
		m_singleProperty.ResetGrid();
		
		m_singleProperty.Initialize(m_dataArr.GetAt(m_beforeIdx));
		m_singleProperty.setHeadInfo(m_beforeIdx);
		m_singleProperty.m_bInvalidate = true;
		m_singleProperty.Invalidate();
		
		//m_input = m_ctrlParam;
		UpdateData(FALSE);
		OnClickNamelist(NULL, NULL);
		m_list.SetFocus();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CControlDlg::Parser(CString &srcstr, CString substr)
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
	return "";
}

void CControlDlg::LoadFile()
{
	CString path, info = "", desc;
	char	twb[1024];
	int	helpIdx = 0, nItem = 0, nSelItem = -1;
	path.Format("%s\\%s\\control.ini", (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), TABDIR);

	for (int ii = 1; ii < 100; ii++)
	{
		CString tmps, tmpstr = "", sym = "";

		tmps.Format("%02d", ii);
		GetPrivateProfileString("CONTROL", tmps, "", twb, sizeof(twb), path);
		desc = twb;
		tmps = Parser(desc, ";");
		sym = Parser(desc, ";");
		
		if (tmps.IsEmpty())
			break;

		info.Empty();		

		DWORD value = GetPrivateProfileSection(tmps, twb, sizeof(twb), path);
		for (int jj = 0; jj < (int)value; jj++)
		{
			if (twb[jj] == '[')
				break;
			(twb[jj] == '\0' || twb[jj] == '\n')?tmpstr += "\t":tmpstr += twb[jj];
		}

		m_singleProperty.setHeadInfo(m_ctrlArr.Add(tmpstr), tmpstr);
		addItem(&m_list, nItem, 0, tmps);
		addItem(&m_list, nItem, 1, sym);
		addItem(&m_list, nItem++, 2, Parser(desc, ";"));
		
		if (!m_ctrlName.IsEmpty() && !tmps.CompareNoCase(m_ctrlName))
		{
			m_list.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);
			nSelItem = nItem -1;
			m_desc = info;
			m_beforeIdx = m_dataArr.Add("");
		}
		else
			m_dataArr.Add("");
	}
	if (nSelItem >= 0)
	{
		CRect rc;
		m_list.GetItemRect(0, rc, LVIR_LABEL);
		m_list.Scroll(CSize(0, nSelItem * rc.Height()));
	}
	UpdateData(FALSE);
}

BOOL CControlDlg::addColumn(CListCtrl *LC, LPCTSTR str, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) str;
	lvc.cx      = LC->GetStringWidth(lvc.pszText) + width;
	lvc.iSubItem = -1;

	return LC->InsertColumn(item, &lvc);
}

BOOL CControlDlg::addItem(CListCtrl *LC, int item, int subItem, CString &str)
{
	LV_ITEM lvi;

        lvi.mask     = LVIF_TEXT;
        lvi.iItem    = item;
        lvi.iSubItem = subItem;
        lvi.pszText  = (LPTSTR) str.operator LPCTSTR();
        if (subItem == 0)
                return LC->InsertItem(&lvi);

        return LC->SetItem(&lvi);
}

void CControlDlg::OnClickNamelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	//*pResult = 0;
}

void CControlDlg::OnOK() 
{
	UpdateData();
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int idx = m_list.GetNextSelectedItem(pos);
	m_ctrlName = m_list.GetItemText(idx, 0);
	
	//m_input = m_singleProperty.getValues();
	m_ctrlParam = m_singleProperty.getValues();
	
	CDialog::OnOK();
}

void CControlDlg::DebugData(char* buf, long bufsize)
{
	CFile file;
	CString filename = "debug.txt";
	CFileFind finder;
	if (finder.FindFile(filename))
	{
		if (!file.Open(filename, CFile::modeReadWrite|CFile::modeReadWrite|CFile::typeBinary))
			AfxMessageBox(filename + " File Open Error");
	}
	else
	{
		if (!file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary))
			AfxMessageBox(filename + " File Open Error");
	}
	file.SeekToEnd();
	file.Write((void*)buf, bufsize);
	file.SeekToEnd();
	file.Write("\r\n", 2);
	file.Close();
}

void CControlDlg::OnItemchangedNamelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int idx = m_list.GetNextSelectedItem(pos);
	if (idx >= 0 && idx < m_ctrlArr.GetSize() && IsWindow(m_singleProperty.m_hWnd))
	{
		m_dataArr.SetAt(m_beforeIdx, m_singleProperty.getAllData());
		m_desc = m_ctrlArr.GetAt(idx);
		m_singleProperty.m_sControlName = m_list.GetItemText(idx, 0);
		m_singleProperty.EraseCtrls(false);
		m_singleProperty.ResetGrid();
		m_singleProperty.Initialize(m_dataArr.GetAt(idx));
		m_singleProperty.setHeadInfo(idx);
		m_singleProperty.m_bInvalidate = true;
		m_singleProperty.Invalidate();
		UpdateData(FALSE);
		m_beforeIdx = idx;
	}
	m_list.SetFocus();
	*pResult = 0;
}

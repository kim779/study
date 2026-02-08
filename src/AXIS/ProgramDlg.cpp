// ProgramDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "ProgramDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgramDlg dialog


CProgramDlg::CProgramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgramDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgramDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arF.RemoveAll();
}

CProgramDlg::~CProgramDlg()
{
	m_arF.RemoveAll();
}

void CProgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgramDlg)
	DDX_Control(pDX, IDC_PLIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgramDlg, CDialog)
	//{{AFX_MSG_MAP(CProgramDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_SEARCHP, OnSearchp)
	ON_NOTIFY(NM_DBLCLK, IDC_PLIST, OnDblclkPlist)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgramDlg message handlers

BOOL CProgramDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

#ifdef DF_USE_CPLUS17
	m_changer = std::make_unique<CControlChanger>(this);
#else
	m_changer = new CControlChanger(this);
#endif
	
	init_List();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgramDlg::OnOK() 
{
	CString	dir, file, key, dat;

	dir.Format("%s\\user\\%s", Axis::home, Axis::user);

	CProfile profile(pkUserProgram);

	::CreateDirectory(dir, NULL);
	profile.WriteSection(szUserTool, "");
	
	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		key = m_list.GetItemText(ii, 0);
		dat = m_arF.GetAt(ii);
		profile.Write(szUserTool, key, dat);
	}
	CDialog::OnOK();
}

void CProgramDlg::OnAdd() 
{
	CString	nameP, nameF, tmpS;

	GetDlgItem(IDC_PNAME)->GetWindowText(nameP);
	GetDlgItem(IDC_FNAME)->GetWindowText(nameF);
	if (nameP.IsEmpty())
	{
		//**::MessageBox(m_hWnd, "선택한 프로그램이 없습니다.", "오류", MB_ICONSTOP);
		Axis::MessageBox(this, "선택한 프로그램이 없습니다.",  MB_ICONSTOP);
		return;
	}

	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		tmpS = m_list.GetItemText(ii, 0);
		if (!nameP.Compare(tmpS))
		{
			//**::MessageBox(m_hWnd, "이미 등록되어있습니다.", "오류", MB_ICONSTOP);
			Axis::MessageBox(this, "이미 등록되어 있습니다. ", MB_ICONSTOP);
			return;
		}
	}

	m_arF.Add(nameF);
	const int row = m_list.GetItemCount();
	addItem(nameP, row);
	addItem(nameF, row, 1);
	GetDlgItem(IDC_PNAME)->SetWindowText(_T(""));
	GetDlgItem(IDC_FNAME)->SetWindowText(_T(""));
}

void CProgramDlg::OnDel() 
{
	int idx = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (idx == -1)
	{
		//**::MessageBox(m_hWnd, "선택된 삭제항목이 없습니다.", "오류", MB_ICONSTOP);
		Axis::MessageBox(this, "선택된 삭제항목이 없습니다.",  MB_ICONSTOP);
		return;
	}
	m_list.DeleteItem(idx);
	m_arF.RemoveAt(idx);

	if (idx >= m_list.GetItemCount())
		idx = m_list.GetItemCount() -1;
	m_list.SetItemState(idx, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
}

void CProgramDlg::OnSearchp() 
{
	char	buf[1024];
	GetWindowsDirectory(buf, sizeof(buf));
	CFileDialog	dlg(TRUE, "exe", NULL, OFN_OVERWRITEPROMPT, 
		"실행파일(*.exe)|*.exe||", NULL);
	if (dlg.DoModal() != IDOK)	return;

	GetDlgItem(IDC_PNAME)->SetWindowText(dlg.GetFileTitle());
	GetDlgItem(IDC_FNAME)->SetWindowText(dlg.GetPathName());
}

void CProgramDlg::OnDblclkPlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	const int pos = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (pos < 0)	return;
	m_list.DeleteItem(pos);
}

void CProgramDlg::init_List()
{
	CRect	rc;
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_list.GetClientRect(rc);
	addColumn("실행명", 0, 100);
	addColumn("실행파일", 1, rc.Width());

	fill_List();
}

void CProgramDlg::fill_List()
{
	CProfile profile(pkUserProgram);

	int	pos{}, idx = 0;
	char	buf[1024*2];
	CString	str, tmpS;
	
	m_list.DeleteAllItems();

	strcpy_s(buf, 2048, profile.GetSectionLF(szUserTool));
	const DWORD dw = strlen(buf);
	if (dw > 0)
	{
		CString cstr = CString(buf,dw);
		for (int ii = 0;; ii++)
		{
			//str = &buf[idx];
			
			str = Parser(cstr,"\n");
			
			if (str.IsEmpty())	break;
			idx += str.GetLength()+1;
			pos = str.Find('=');
			if (pos == -1)		continue;
			tmpS = str.Left(pos++);
			tmpS.TrimLeft(); tmpS.TrimRight();
			addItem(tmpS, ii);
			
			tmpS = str.Mid(pos);
			tmpS.TrimLeft(); tmpS.TrimRight();
			m_arF.Add(tmpS);
			addItem(tmpS, ii, 1);
		}	
	}
	fit();
}

void CProgramDlg::fit()
{
	if (m_list.GetSafeHwnd())
	{
		CRect	cRc;
		m_list.GetClientRect(cRc);
		m_list.SetColumnWidth(0, 100);
		m_list.SetColumnWidth(1, cRc.Width() - 100);
	}
}

BOOL CProgramDlg::addColumn(LPCTSTR columns, int col, int width)
{
	LV_COLUMN lvc;

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR)columns;
	lvc.cx	     = /*list.GetStringWidth(lvc.pszText) + */width;
	lvc.iSubItem = -1;

	return m_list.InsertColumn(col, &lvc);
}

BOOL CProgramDlg::addItem(CString items, int item, int subi)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subi;
	lvi.pszText  = (char*)(const char*)items;

	if (!subi)	return m_list.InsertItem(&lvi);
	return m_list.SetItem(&lvi);
}

BOOL CProgramDlg::OnEraseBkgnd(CDC* pDC) 
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

HBRUSH CProgramDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CProgramDlg::OnDestroy() 
{
	CDialog::OnDestroy();

#ifndef DF_USE_CPLUS17
	delete m_changer;	
#endif
}

CString CProgramDlg::Parser(CString &srcstr, CString substr)
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

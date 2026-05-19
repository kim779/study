// controlDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "controlDlg.h"
#include "afxdialogex.h"
#include "propGrid.h"

#include "../h/mainvar.h"
#include "../../h/axisvar.h"

// CControlDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CControlDlg, CDialogEx)

CControlDlg::CControlDlg(int fN, CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlDlg::IDD, pParent)
{
	m_fN = fN;
	m_grid = NULL;
}

CControlDlg::~CControlDlg()
{
	if (m_grid)
		delete m_grid;
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAMELIST, m_list);
}


BEGIN_MESSAGE_MAP(CControlDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_NAMELIST, &CControlDlg::OnItemChangedNameList)
END_MESSAGE_MAP()


// CControlDlg 메시지 처리기입니다.


BOOL CControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DWORD	dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_list.SetExtendedStyle(dwStyle);

	addColumn(&m_list, _T("Name"), 0, 58);
	addColumn(&m_list, _T("Symbol"), 1, 18);
	addColumn(&m_list, _T("Description"), 2, 52);

	CRect rc, listRc;

	GetClientRect(&rc);
	m_list.GetClientRect(&listRc);

	m_grid = new CPropGrid();
	if (!m_grid->Create(WS_VISIBLE | WS_CHILD, CRect(6, listRc.Height() + 30, rc.right - 7, rc.bottom - 35), this, 110))
		return -1;

	m_grid->EnableHeaderCtrl(FALSE);
	m_grid->EnableDescriptionArea(FALSE);
	m_grid->SetVSDotNetLook();
	m_grid->MarkModifiedProperties();

	m_beforeIdx = -1;
	loadFile();

	if (m_beforeIdx >= m_dataArr.GetSize())
	{
		AfxMessageBox(_T("File not found \"control.ini\""));
		PostMessage(WM_CLOSE);
	}
	else
	{
		CString	data = m_grid->ConvertData(m_ctrlParam);
		m_dataArr.SetAt(m_beforeIdx, data);
		m_grid->Initialize(data);
		m_grid->SetProperties(m_beforeIdx);
		
		UpdateData(FALSE);
		m_list.SetFocus();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CControlDlg::OnOK()
{
	POSITION pos;

	UpdateData();
	pos = m_list.GetFirstSelectedItemPosition();
	if (pos !=  NULL)
	{
		int	idx;
		
		idx = m_list.GetNextSelectedItem(pos);
		m_ctrlName = m_list.GetItemText(idx, 0);
		m_ctrlParam = m_grid->GetValues();
	}

	CDialogEx::OnOK();
}


void CControlDlg::OnItemChangedNameList(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	pos = m_list.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int	idx = m_list.GetNextSelectedItem(pos);
		if (m_beforeIdx == idx)
			return;

		if (idx >= 0 && idx < m_ctrlArr.GetSize() && IsWindow(m_grid->m_hWnd))
		{
			if (m_beforeIdx >= 0)
				m_dataArr.SetAt(m_beforeIdx, m_grid->GetData());
			m_grid->SetControl(m_list.GetItemText(idx, 0), m_fN);
			m_grid->Initialize(m_dataArr.GetAt(idx));
			m_grid->SetProperties(idx);

			UpdateData(FALSE);
			m_beforeIdx = idx;
		}
	}
	m_list.SetFocus();
}


void CControlDlg::loadFile()
{
	CString path, desc;
	CString tmps, text, sym ;
	char	wb[1024];
	DWORD	value;
	int	nItem = 0, nSelItem = -1;

	path.Format(_T("%s\\%s\\control.ini"), (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), TABDIR);
	for (int ii = 1; ii < 100; ii++)
	{
		tmps.Format(_T("%02d"), ii);
		GetPrivateProfileString(_T("CONTROL"), tmps, _T(""), wb, sizeof(wb), path);
		desc = wb;
		tmps = parse(desc, _T(";"));
		sym  = parse(desc, _T(";"));
		text = _T("");
		if (tmps.IsEmpty())
			break;

		value = GetPrivateProfileSection(tmps, wb, sizeof(wb), path);
		for (int jj = 0; jj < (int)value; jj++)
		{
			if (wb[jj] == _T('['))
				break;
			if (wb[jj] == _T('\0') || wb[jj] == _T('\n'))
				text += _T("\t");
			else
				text += wb[jj];
		}

		m_grid->SetPropInfo((int)m_ctrlArr.Add(text), text);
		addItem(&m_list, nItem, 0, tmps);
		addItem(&m_list, nItem, 1, sym);
		addItem(&m_list, nItem++, 2, parse(desc, _T(";")));
		
		if (!m_ctrlName.IsEmpty() && !tmps.CompareNoCase(m_ctrlName))
		{
			nSelItem = nItem -1;
		//	m_beforeIdx = (int)m_dataArr.Add(_T(""));
		}
		else
			m_dataArr.Add(_T(""));
	}
//	if (nSelItem >= 0)
	nSelItem = max(0, nSelItem);
	{
		CRect rc;

		m_list.SetItemState(nSelItem, LVIS_SELECTED, LVIS_SELECTED);
		m_list.GetItemRect(0, rc, LVIR_LABEL);
		m_list.Scroll(CSize(0, nSelItem * rc.Height()));
	}
	UpdateData(FALSE);
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

CString CControlDlg::parse(CString &src, CString sub)
{
	CString	tmps;
	int	pos = src.Find(sub);

	if (pos == -1)
	{
		tmps = src;
		src.Empty();
		return tmps;
	}
	tmps = src.Left(pos);
	src = src.Mid(pos + sub.GetLength());
	return tmps;
}

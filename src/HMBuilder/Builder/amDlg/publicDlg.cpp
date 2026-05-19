// publicDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "publicDlg.h"
#include "afxdialogex.h"

#include "../h/mainvar.h"
#include "../h/axisvar.h"

// CPublicDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPublicDlg, CDialogEx)

CPublicDlg::CPublicDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPublicDlg::IDD, pParent)
{
	m_helps = _T("");
}

CPublicDlg::~CPublicDlg()
{
}

void CPublicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HELPSTATIC, m_helps);
	DDX_Control(pDX, IDC_NEWLIST, m_newList);
	DDX_Control(pDX, IDC_OLDLIST, m_oldList);
	DDX_Control(pDX, IDC_PROPERTIES, m_propList);
}


BEGIN_MESSAGE_MAP(CPublicDlg, CDialogEx)
	ON_BN_CLICKED(IDC_INBTN, &CPublicDlg::OnInBtn)
	ON_BN_CLICKED(IDC_OUTBTN, &CPublicDlg::OnOutBtn)
	ON_LBN_SELCHANGE(IDC_NEWLIST, &CPublicDlg::OnSelChangeNewlist)
	ON_LBN_DBLCLK(IDC_NEWLIST, &CPublicDlg::OnDblclkNewlist)
	ON_LBN_SELCHANGE(IDC_OLDLIST, &CPublicDlg::OnSelChangeOldlist)
	ON_LBN_DBLCLK(IDC_OLDLIST, &CPublicDlg::OnDblclkOldlist)
END_MESSAGE_MAP()


// CPublicDlg 메시지 처리기입니다.


BOOL CPublicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	loadFile();
	addColumn(&m_propList, _T("Name"), 0, 50);
	addColumn(&m_propList, _T("Type"), 1, 25);
	addColumn(&m_propList, _T("Description"), 2, 95);

	DWORD	dwStyle = m_propList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_propList.SetExtendedStyle(dwStyle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPublicDlg::OnOK()
{
	CString tmps;

	m_publicR = _T("");
	for (int ii = 0; ii < m_newList.GetCount(); ii++)
	{
		m_newList.GetText(ii, tmps);
		m_publicR += tmps;
		m_publicR += _T(";");
	}
	CDialogEx::OnOK();
}


void CPublicDlg::OnInBtn()
{
	CString tmps;

	for (int ii = m_oldList.GetCount() - 1; ii >= 0 ; ii--)
	{
		if (m_oldList.GetSel(ii))
		{
			m_oldList.GetText(ii, tmps);
			m_newList.SetItemData(m_newList.AddString(tmps), m_oldList.GetItemData(ii));
			m_oldList.DeleteString(ii);
		}
	}
}


void CPublicDlg::OnOutBtn()
{
	CString txt;

	for (int ii = m_newList.GetCount() - 1; ii >=0 ; ii--)
	{
		if (m_newList.GetSel(ii))
		{
			m_newList.GetText(ii, txt);
			m_oldList.SetItemData(m_oldList.AddString(txt), m_newList.GetItemData(ii));
			m_newList.DeleteString(ii);
		}
	}
}


void CPublicDlg::OnSelChangeNewlist()
{
	if (m_newList.GetCurSel() != LB_ERR)
	{
		m_oldList.SetCurSel(-1);
		m_helps = m_HelpArr.GetAt(m_newList.GetItemData(m_newList.GetCurSel()));
		onPublicInfo(m_proArr.GetAt(m_newList.GetItemData(m_newList.GetCurSel())));
		UpdateData(FALSE);
	}	
}


void CPublicDlg::OnDblclkNewlist()
{
	OnOutBtn();
}


void CPublicDlg::OnSelChangeOldlist()
{
	if (m_oldList.GetCurSel() != LB_ERR)
	{
		m_newList.SetCurSel(-1);
		m_helps = m_HelpArr.GetAt(m_oldList.GetItemData(m_oldList.GetCurSel()));
		onPublicInfo(m_proArr.GetAt(m_oldList.GetItemData(m_oldList.GetCurSel())));
		UpdateData(FALSE);
	}
}


void CPublicDlg::OnDblclkOldlist()
{
	OnInBtn();
}

void CPublicDlg::loadFile()
{
	CString tmps, str;
	CString path, info;
	char	wb[128];
	int	helpIdx = 0;

	path.Format(_T("%s\\%s\\public.ini"), (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), TABDIR);
	for (int ii = 1; ii < 100; ii++)
	{
		tmps.Format(_T("%02d"), ii);
		GetPrivateProfileString(_T("PUBLIC"), tmps, _T(""), wb, sizeof(wb), path);
		info = wb;
		tmps = parse(info, _T(";"));
		
		if (tmps.IsEmpty())
			break;

		helpIdx = (int)m_HelpArr.Add(info);
		info = _T("");
		for (int jj = 1; jj < 100; jj++)
		{
			str.Format(_T("%02d"), jj);
			GetPrivateProfileString(tmps, str, "", wb, sizeof(wb), path);
			str = wb;
			if (str.IsEmpty())
				break;
			
			info += str;
			info += '\t';
		}

		if (!info.IsEmpty())
		{
			m_proArr.Add(info);
			str = m_publicR;
			str.MakeUpper();
			tmps.MakeUpper();
			str = _T(";") + str + _T(";");
			if (str.Find(_T(";") + tmps + _T(";")) >= 0)
				m_newList.SetItemData(m_newList.AddString(tmps), helpIdx);
			else
				m_oldList.SetItemData(m_oldList.AddString(tmps), helpIdx);
		}
	}
}

BOOL CPublicDlg::addColumn(CListCtrl* LC, LPCTSTR str, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_CENTER;
	lvc.pszText = (LPTSTR)str;
	lvc.cx      = LC->GetStringWidth(lvc.pszText) + width;
	lvc.iSubItem = -1;

	return LC->InsertColumn(item, &lvc);
}

BOOL CPublicDlg::addItem(CListCtrl* LC, int item, int subItem, CString& str)
{
	LV_ITEM lvi;

        lvi.mask     = LVIF_TEXT;
        lvi.iItem    = item;
        lvi.iSubItem = subItem;
        lvi.pszText  = (LPTSTR)str.operator LPCTSTR();
        if (subItem == 0)
                return LC->InsertItem(&lvi);

        return LC->SetItem(&lvi);
}

void CPublicDlg::onPublicInfo(CString data)
{
	CString	tmps;
	CString prop, type, desc;
	int	nItem = 0;

	m_propList.DeleteAllItems();
	while (!data.IsEmpty())
	{
		tmps = parse(data, _T("\t"));
		prop = parse(tmps, _T(";"));
		prop = parse(prop, _T(":"));
		type = parse(tmps, _T(";"));
		desc = tmps;

		addItem(&m_propList, nItem, 0, prop);
		addItem(&m_propList, nItem, 1, type);
		addItem(&m_propList, nItem, 2, desc);
		nItem++;
	}
}

CString CPublicDlg::parse(CString &src, CString sub)
{
	CString	tmps;
	int	pos = src.Find(sub);

	if (pos == -1)
	{
		tmps = src;
		src.Empty();
		return tmps;
	}
	else
	{
		tmps = src.Left(pos);
		src = src.Mid(pos + sub.GetLength());
		return tmps;
	}
	return _T("");
}

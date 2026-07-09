// InputTmpl.cpp : implementation file
//

#include "stdafx.h"
#include "InputTmpl.h"
#include "../awWcc/libwcc.h"
#include "../awSock/wSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputTmpl dialog
#define	DN_TMPLFILE	"template"

CInputTmpl::CInputTmpl(CWnd* pParent /*=NULL*/)
	: CDialog(CInputTmpl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputTmpl)
	m_groupName = _T("");
	m_tmplName = _T("");
	m_bExport = FALSE;
	//}}AFX_DATA_INIT
}


void CInputTmpl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputTmpl)
	DDX_Control(pDX, IDC_TMPLLIST, m_listctrl);
	DDX_Control(pDX, IDC_GROUPCBO, m_combo);
	DDX_Text(pDX, IDC_GROUPNAME, m_groupName);
	DDX_Text(pDX, IDC_TMPLNAME, m_tmplName);
	DDX_Check(pDX, IDC_EXPORT, m_bExport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputTmpl, CDialog)
	//{{AFX_MSG_MAP(CInputTmpl)
	ON_CBN_SELENDOK(IDC_GROUPCBO, OnSelendokGroupcbo)
	ON_NOTIFY(NM_CLICK, IDC_TMPLLIST, OnClickTmpllist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputTmpl message handlers

void CInputTmpl::loadFile()
{
	char twb[6144];
	CString filename, temp;
	wccSetTempPath(filename, DN_TMPLFILE);

	m_groupArr.RemoveAll();
	m_nameArr.RemoveAll();

	GetPrivateProfileString("TEMPLATE", "$NAME", "", twb, 6144, filename);
	temp = twb;
	while (!temp.IsEmpty())
	{
		CString sGroup = Parser(temp, ";"), tmp, sName = "";
		if (sGroup.IsEmpty())
			break;
		m_groupArr.Add(sGroup);
		GetPrivateProfileString("TEMPLATE", sGroup, "", twb, 6144, filename);
		tmp = twb;
		while(!tmp.IsEmpty())
		{
			CString sFName = Parser(tmp, ";"), tmp2;
			if (sFName.IsEmpty())
				break;
			GetPrivateProfileString("TEMPLATE", sFName, "", twb, 6144, filename);
			tmp2 = twb;
			sFName += '\t';
			sFName += Parser(tmp2, ";");
			sName += sFName;
			sName += ';';
		}
		m_nameArr.Add(sName);
	}
}

CString CInputTmpl::Parser(CString &srcstr, CString substr)
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

void CInputTmpl::fillCombo(CComboBox *CBO)
{
	CBO->ResetContent();
	for (int ii = 0; ii < m_groupArr.GetSize(); ii++)
		CBO->SetItemData(CBO->AddString(m_groupArr.GetAt(ii)), ii);
}

void CInputTmpl::fillList(CListCtrl *LC, int idx)
{
	LC->DeleteAllItems();
	CString temp = m_nameArr.GetAt(idx);
	while(!temp.IsEmpty())
	{
		CString tmp = Parser(temp, ";");
		if (tmp.IsEmpty())
			break;
		Parser(tmp, "\t");
		addItem(LC, 0, 0, tmp);
	}
}

BOOL CInputTmpl::addItem(CListCtrl *LC, int item, int subItem, CString &str)
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

BOOL CInputTmpl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	fillCombo(&m_combo);
	if (m_combo.GetCount())
	{
		m_combo.SetCurSel(0);
		OnSelendokGroupcbo();
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputTmpl::OnSelendokGroupcbo() 
{
	if (m_combo.GetCurSel() != LB_ERR)
	{
		fillList(&m_listctrl, m_combo.GetItemData(m_combo.GetCurSel()));
		m_combo.GetLBText(m_combo.GetCurSel(), m_groupName);
		UpdateData(FALSE);
	}
}

void CInputTmpl::OnClickTmpllist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_listctrl.GetFirstSelectedItemPosition();
	if (pos)
	{
		m_tmplName = m_listctrl.GetItemText(m_listctrl.GetNextSelectedItem(pos), 0);
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CInputTmpl::OnOK() 
{
	UpdateData();	
	CDialog::OnOK();
}

// PublicSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PublicSetDlg.h"
#include "../mainvar.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPublicSetDlg dialog


CPublicSetDlg::CPublicSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPublicSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPublicSetDlg)
	m_sHelpStatic = _T("");
	//}}AFX_DATA_INIT
}


void CPublicSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPublicSetDlg)
	DDX_Control(pDX, IDC_PROPERTIES, m_List_pro);
	DDX_Control(pDX, IDC_NEWLIST, m_newList);
	DDX_Control(pDX, IDC_OLDLIST, m_oldList);
	DDX_Text(pDX, IDC_HELPSTATIC, m_sHelpStatic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPublicSetDlg, CDialog)
	//{{AFX_MSG_MAP(CPublicSetDlg)
	ON_BN_CLICKED(IDC_INBTN, OnInbtn)
	ON_BN_CLICKED(IDC_OUTBTN, OnOutbtn)
	ON_LBN_SELCHANGE(IDC_OLDLIST, OnSelchangeOldlist)
	ON_LBN_SELCHANGE(IDC_NEWLIST, OnSelchangeNewlist)
	ON_LBN_DBLCLK(IDC_OLDLIST, OnDblclkOldlist)
	ON_LBN_DBLCLK(IDC_NEWLIST, OnDblclkNewlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPublicSetDlg message handlers

BOOL CPublicSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	LoadFile();
	addColumn(&m_List_pro, "Name", 0, 50);
	addColumn(&m_List_pro, "Type", 1, 25);
	addColumn(&m_List_pro, "Description", 2, 95);

	DWORD dwStyle = m_List_pro.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_List_pro.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPublicSetDlg::LoadFile()
{
	CString path, info = "";
	char	twb[128];
	int	helpIdx = 0;
	path.Format("%s\\%s\\public.ini", (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), TABDIR);

	for (int ii = 1; ii < 100; ii++)
	{
		CString tmps;

		tmps.Format("%02d", ii);
		GetPrivateProfileString("PUBLIC", tmps, "", twb, sizeof(twb), path);
		info = twb;
		tmps = Parser(info, ";");
		
		if (tmps.IsEmpty())
			break;

		helpIdx = m_HelpArr.Add(info);
		info.Empty();

		for (int jj = 1; jj < 100; jj++)
		{
			CString tmpstr, sPro, sType;

			tmpstr.Format("%02d", jj);
			GetPrivateProfileString(tmps, tmpstr, "", twb, sizeof(twb), path);
			tmpstr = twb;
			if (tmpstr.IsEmpty())
				break;
			
			/*sPro = Parser(tmpstr, ";");
			sPro = Parser(sPro, ":");
			sType = tmpstr;*/
			info += tmpstr;
			info += '\t';
		}

		if (!info.IsEmpty())
		{
			m_proArr.Add(info);
			CString sTmp = m_publicR;
			sTmp.MakeUpper();
			tmps.MakeUpper();
			sTmp = ";" + sTmp + ";";
			if (sTmp.Find(";" + tmps + ";") >= 0)
				m_newList.SetItemData(m_newList.AddString(tmps), helpIdx);
			else
				m_oldList.SetItemData(m_oldList.AddString(tmps), helpIdx);
		}
	}
}

CString CPublicSetDlg::Parser(CString &srcstr, CString substr)
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

void CPublicSetDlg::OnInbtn() 
{
	for (int ii = m_oldList.GetCount() - 1; ii >= 0 ; ii--)
	{
		if (m_oldList.GetSel(ii))
		{
			CString txt;
			m_oldList.GetText(ii, txt);
			m_newList.SetItemData(m_newList.AddString(txt), m_oldList.GetItemData(ii));
			m_oldList.DeleteString(ii);
		}
	}
}

void CPublicSetDlg::OnOutbtn() 
{
	for (int ii = m_newList.GetCount() - 1; ii >=0 ; ii--)
	{
		if (m_newList.GetSel(ii))
		{
			CString txt;
			m_newList.GetText(ii, txt);
			m_oldList.SetItemData(m_oldList.AddString(txt), m_newList.GetItemData(ii));
			m_newList.DeleteString(ii);
		}
	}	
}

void CPublicSetDlg::OnSelchangeOldlist() 
{
	if (m_oldList.GetCurSel() != LB_ERR)
	{
		m_newList.SetCurSel(-1);
		m_sHelpStatic = m_HelpArr.GetAt(m_oldList.GetItemData(m_oldList.GetCurSel()));
		OnPublicInfo(m_proArr.GetAt(m_oldList.GetItemData(m_oldList.GetCurSel())));
		UpdateData(FALSE);
	}
}

void CPublicSetDlg::OnSelchangeNewlist() 
{
	if (m_newList.GetCurSel() != LB_ERR)
	{
		m_oldList.SetCurSel(-1);
		m_sHelpStatic = m_HelpArr.GetAt(m_newList.GetItemData(m_newList.GetCurSel()));
		OnPublicInfo(m_proArr.GetAt(m_newList.GetItemData(m_newList.GetCurSel())));
		UpdateData(FALSE);
	}	
}

void CPublicSetDlg::OnOK() 
{
	m_publicR = "";
	for (int ii = 0; ii < m_newList.GetCount(); ii++)
	{
		CString tmps;
		m_newList.GetText(ii, tmps);
		m_publicR += tmps;
		m_publicR += ";";
	}
	CDialog::OnOK();
}

void CPublicSetDlg::OnDblclkOldlist() 
{
	OnInbtn();	
}

void CPublicSetDlg::OnDblclkNewlist() 
{
	OnOutbtn();
}

BOOL CPublicSetDlg::addColumn(CListCtrl *LC, LPCTSTR str, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_CENTER;
	lvc.pszText = (LPTSTR) str;
	lvc.cx      = LC->GetStringWidth(lvc.pszText) + width;
	lvc.iSubItem = -1;

	return LC->InsertColumn(item, &lvc);
}

void CPublicSetDlg::OnPublicInfo(CString data)
{
	CString sPro, sType, sDesc;
	int nItem = 0;
	m_List_pro.DeleteAllItems();

	while(!data.IsEmpty())
	{
		CString sTmp = Parser(data, "\t");
		sPro = Parser(sTmp, ";");
		sPro = Parser(sPro, ":");
		sType = Parser(sTmp, ";");
		sDesc = sTmp;

		addItem(&m_List_pro, nItem, 0, sPro);
		addItem(&m_List_pro, nItem, 1, sType);
		addItem(&m_List_pro, nItem, 2, sDesc);
		nItem++;
	}
}

BOOL CPublicSetDlg::addItem(CListCtrl *LC, int item, int subItem, CString &str)
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

// ShareGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB202201.h"
#include "ShareGroupDlg.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShareGroupDlg dialog


CShareGroupDlg::CShareGroupDlg(CWnd* pParent, CString root, CString name, BOOL bImport)
	: CDialog(CShareGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShareGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT


	m_strUserDir.Format("%s/%s/%s",    root, USRDIR, name);
	m_bImport = bImport;
}


void CShareGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShareGroupDlg)
	DDX_Control(pDX, IDC_LIST, m_listGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShareGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CShareGroupDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShareGroupDlg message handlers

BOOL CShareGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rcList;

	m_listGroup.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_listGroup.GetClientRect(&rcList);

	addColumn("그룹명", 0, rcList.Width());
	CString strText;
	GetWindowText(strText);

	if (m_bImport)
	{
		strText += _T(" - 불러오기");
		LoadImportGroup();
	}
	else
	{
		strText += _T(" - 내보내기");
		LoadExportGroup();
	}

	SetWindowText(strText); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CShareGroupDlg::addColumn(LPCTSTR strItem, int nItem, int nWidth)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR)strItem;
	lvc.cx = m_listGroup.GetStringWidth(lvc.pszText) + nWidth;
	lvc.iSubItem = -1;

	return m_listGroup.InsertColumn(nItem, &lvc);
}

int CShareGroupDlg::addItem(const CString &strItem, int nItem)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = nItem;
	lvi.iSubItem = 0;
	lvi.pszText  = (LPTSTR) strItem.operator LPCTSTR();

	return m_listGroup.InsertItem(&lvi);
}

CString CShareGroupDlg::parseX(CString &srcstr, CString substr)
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

void CShareGroupDlg::LoadImportGroup()
{
}

void CShareGroupDlg::LoadExportGroup()
{
	int nIdx = 0;
	int readL = 0; char readB[2048]{};
	CString	string, gno, gname;

	// readL = GetPrivateProfileString("GROUPORDER", "00", "", readB, sizeof(readB), m_strTempFile);
	// string = CString(readB, readL);
	
	// int addIdx = 0;
	// while (!string.IsEmpty())
	// {
	// 	gno = parseX(string, ";");
	// 	readL = GetPrivateProfileString(_T("GROUPNAME"), gno, "", readB, sizeof(readB), m_strTempFile);
	// 	gname = CString(readB, readL);

	// 	if (!gname.IsEmpty())
	// 	{
	// 		addIdx = addItem(gname, nIdx++);
	// 		m_listGroup.SetItemData(addIdx, atoi(gno));
	// 	}
	// }
}



void CShareGroupDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CShareGroupDlg::OnOK() 
{
/*
	CString strMsg, strTemp;
	int nCount = m_listGroup.GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		if (m_listGroup.GetCheck(i))
			;
	}
*/	
	CDialog::OnOK();
}

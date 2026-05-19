// Options.cpp : implementation file
//

#include "stdafx.h"
#include "AxisChaser.h"
#include "Options.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptions dialog


COptions::COptions(CWnd* pParent /*=NULL*/)
	: CDialog(COptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptions)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptions)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptions, CDialog)
	//{{AFX_MSG_MAP(COptions)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptions message handlers

BOOL COptions::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char buf[512];		FillMemory(buf, sizeof(buf), ' ');
	GetCurrentDirectory(sizeof(buf), buf);
	m_file.Format("%s\\chaser.ini", buf);

	DWORD styles = LVS_EX_GRIDLINES | LVS_OWNERDRAWFIXED;
	ListView_SetExtendedListViewStyleEx(m_list.m_hWnd, styles, styles); 


	m_list.InsertColumn(0, "ÄÚµå", LVCFMT_LEFT, 100);

	loadOptions();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptions::loadOptions()
{
	char buf[512];		FillMemory(buf, sizeof(buf), ' ');
	CString value, key, section = "Options";
	int	chk = 0;

	chk = GetPrivateProfileInt(section, "send", 1, m_file);
	((CButton *) GetDlgItem(IDC_SEND))->SetCheck(chk);

	chk = GetPrivateProfileInt(section, "receive", 1, m_file);
	((CButton *) GetDlgItem(IDC_RECEIVE))->SetCheck(chk);

	chk = GetPrivateProfileInt(section, "header", 1, m_file);
	((CButton *) GetDlgItem(IDC_HEADER))->SetCheck(chk);

	chk = GetPrivateProfileInt(section, "data", 1, m_file);
	((CButton *) GetDlgItem(IDC_DATA))->SetCheck(chk);

	chk = GetPrivateProfileInt(section, "rts", 0, m_file);
	if (!chk)
	{
		((CButton *) GetDlgItem(IDC_ALLCODE))->SetCheck(1);
		((CButton *) GetDlgItem(IDC_FILTERINGCODE))->SetCheck(0);
	}
	else
	{
		((CButton *) GetDlgItem(IDC_ALLCODE))->SetCheck(0);
		((CButton *) GetDlgItem(IDC_FILTERINGCODE))->SetCheck(1);
	}

	section = "code";
	for (int ii = 0; ;ii++)
	{
		key.Format("%03d", ii);
		DWORD dw = GetPrivateProfileString(section, key, "", buf, sizeof(buf), m_file);
		if (dw <= 0)	break;
		
		value = buf;	value.TrimLeft();	value.TrimRight();
		if (value.IsEmpty())	break;

		m_list.InsertItem(ii, value, 0);
	}
}

void COptions::saveOptions()
{
	char buf[512];		FillMemory(buf, sizeof(buf), ' ');
	CString value, key, section = "Options";
	int	chk = 0;

	chk = ((CButton *) GetDlgItem(IDC_SEND))->GetCheck();
	value.Format("%d", chk);
	WritePrivateProfileString(section, "send", value, m_file);

	chk = ((CButton *) GetDlgItem(IDC_RECEIVE))->GetCheck();
	value.Format("%d", chk);
	WritePrivateProfileString(section, "receive", value, m_file);

	chk = ((CButton *) GetDlgItem(IDC_HEADER))->GetCheck();
	value.Format("%d", chk);
	WritePrivateProfileString(section, "header", value, m_file);

	chk = ((CButton *) GetDlgItem(IDC_DATA))->GetCheck();
	value.Format("%d", chk);
	WritePrivateProfileString(section, "data", value, m_file);

	chk = ((CButton *) GetDlgItem(IDC_FILTERINGCODE))->GetCheck();
	value.Format("%d", chk);
	WritePrivateProfileString(section, "rts", value, m_file);

	section = "code";
	WritePrivateProfileSection(section, "", m_file);
	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		value = m_list.GetItemText(ii, 0);
		if (value.IsEmpty())	continue;
		key.Format("%03d", ii);

		WritePrivateProfileString(section, key, value, m_file);
	}
}

void COptions::OnOK() 
{
	saveOptions();
	
	CDialog::OnOK();
}

void COptions::OnAdd() 
{
	CString code;
	GetDlgItem(IDC_EDIT)->GetWindowText(code);

	if (code.IsEmpty())	return;

	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		CString sItem = m_list.GetItemText(ii, 0);
		if (sItem == code)	return;
	}

	m_list.InsertItem(m_list.GetItemCount(), code, 0);
}

void COptions::OnRemove() 
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();

	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			int nItem = m_list.GetNextSelectedItem(pos);
			
			m_list.DeleteItem(nItem);
		}
	}
}

BOOL COptions::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		pMsg->wParam = VK_TAB; 
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		pMsg->wParam = VK_TAB; 
	
	return CDialog::PreTranslateMessage(pMsg);
}

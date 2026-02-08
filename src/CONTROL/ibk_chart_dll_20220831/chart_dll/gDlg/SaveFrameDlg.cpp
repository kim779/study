#include "stdafx.h"
#include "axisgdlg.h"
#include "SaveFrameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FILE_CONTENTCHANGE	0x01
#define FILE_NAME_LIMIT		64

#define STATE_SAVE		1
#define STATE_LOAD		2
#define STATE_CANCEL		3

CSaveFrameDlg::CSaveFrameDlg(CWnd* pMain, char *pName, char *fList, char *Path, CWnd* pParent)
	: CDialog(CSaveFrameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveFrameDlg)
	//}}AFX_DATA_INIT
	
	m_pMain = pMain;
	m_pName = pName;
	m_nState = STATE_CANCEL;
	CString	tmpstr;
	int	nPos = 0;

	tmpstr.Format("%s", pName);
	nPos = tmpstr.Find(".", 0);
	
	if (nPos < 0)
	{
		m_identity = _T("");
		m_extention = _T("");
	}
	else
	{
		m_identity = tmpstr.Left(nPos);
		m_extention = tmpstr.Mid(nPos);
	}

	m_Path.Format("%s", Path);
	memset(m_pName, 0x00, FILE_NAME_LIMIT);

	CString szList;
	CString	szToken = "\t";
	CString szTmp = _T("");
	CString	szItem = _T("");
	int	nToken = szToken.GetLength();

	szList.Format("%s", fList);
	m_arFileList.RemoveAll();

	if (!szList.IsEmpty())
	{
		szTmp = szList;

		while (1)
		{
			nPos = szTmp.Find(szToken, 0);

			if (nPos < 0)
			{
				if (!szTmp.IsEmpty())
					m_arFileList.Add(szTmp);
				break;
			}

			szItem = szTmp.Left(nPos);
			szTmp = szTmp.Mid(nPos + nToken);

			if (!szItem.IsEmpty())
				m_arFileList.Add(szItem);
		}
	}
}

void CSaveFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveFrameDlg)
	DDX_Control(pDX, IDC_ET_FILENAME, m_FileName);
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSaveFrameDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveFrameDlg)
	ON_BN_CLICKED(IDC_BT_SAVE, OnBtSave)
	ON_BN_CLICKED(IDC_BT_DEL, OnBtDel)
	ON_BN_CLICKED(IDC_BT_LOAD, OnBtLoad)
	ON_LBN_DBLCLK(IDC_LIST, OnDblclkList)
	ON_LBN_SELCHANGE(IDC_LIST, OnSelchangeList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSaveFrameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	((CEdit*)GetDlgItem(IDC_ET_FILENAME))->SetLimitText(FILE_NAME_LIMIT);

	CString		filename;		
	int		nCount = m_arFileList.GetSize();
	int		ii = 0;

	m_list.ResetContent();	

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		filename = m_arFileList.GetAt(ii);		
		m_list.AddString(filename);
	}

	return TRUE;
}

void CSaveFrameDlg::OnBtSave() 
{	
	m_nState = STATE_CANCEL;
	
	CString	szTmp;	

	GetDlgItem(IDC_ET_FILENAME)->GetWindowText(szTmp);

	if (szTmp.IsEmpty())
		AfxGetMainWnd()->MessageBox("파일명을 입력하세요", COMPANYNAME);
	else if (szTmp.GetLength() > FILE_NAME_LIMIT)
		AfxGetMainWnd()->MessageBox("파일명이 너무 깁니다.", COMPANYNAME);
	else
	{
		char	chkStr[9] = { "\\;/|*?<>"};
		for (int ii = 0; ii < 8; ii++)
		{
			if (szTmp.Find(chkStr[ii]) >= 0)
			{
				CString	msg;
				msg.Format("파일이름에 특수문자(%s)를 사용할 수 없습니다", chkStr);
				AfxGetMainWnd()->MessageBox(msg, COMPANYNAME);
				SetEditFocus();
				return;
			}
		}

		int	nCount = m_list.GetCount();
		CString	szList;
		int	nIndex = -1;

		for ( int ii = 0 ; ii < nCount ; ii++ )
		{
			m_list.GetText(ii, szList);
			if (szList.CompareNoCase(szTmp) == 0)
				nIndex = ii;
		}

		if (nIndex >= 0)
		{
			m_list.SetCurSel(nIndex);
			if (AfxGetMainWnd()->MessageBox("이미 존재하는 파일명입니다. 덮어쓰시겠습니까?", "확인", MB_YESNO) == IDNO)
			{
				SetEditFocus();
				return;
			}
		}

		memcpy(m_pName, szTmp, szTmp.GetLength());
		m_nState = STATE_SAVE;
		CDialog::OnOK();
	}

	SetEditFocus();
}

void CSaveFrameDlg::OnBtDel() 
{
	m_nState = STATE_CANCEL;

	int	nCurSel = m_list.GetCurSel();
	CString	szTmp;
	CString	filename;

	if (nCurSel < 0)
		return;

	m_list.GetText(nCurSel, szTmp);

	if (szTmp.IsEmpty())
		return;

	CString	szEdit;
	GetDlgItem(IDC_ET_FILENAME)->GetWindowText(szEdit);

	filename.Format(_T("%s%s%s%s"), LPCTSTR(m_Path), LPCTSTR(m_identity), LPCTSTR(szTmp), LPCTSTR(m_extention));

	if (DeleteFile(filename))
	{
		m_list.DeleteString(nCurSel);
		
		if (szEdit.CompareNoCase(szTmp) == 0)
			GetDlgItem(IDC_ET_FILENAME)->SetWindowText("");
	}
}

void CSaveFrameDlg::OnBtLoad() 
{

	m_nState = STATE_CANCEL;
	
	int	nCurSel = m_list.GetCurSel();
	CString	szTmp;
	
	if (nCurSel < 0)
		return;

	m_list.GetText(nCurSel, szTmp);

	if (szTmp.IsEmpty())
		return;

	if (szTmp.GetLength() > FILE_NAME_LIMIT)
	{
		AfxGetMainWnd()->MessageBox("파일명이 너무 깁니다.", COMPANYNAME);
		SetEditFocus();
	}
	else
	{
		memcpy(m_pName, szTmp, szTmp.GetLength());
		m_nState = STATE_LOAD;
		CDialog::OnOK();
	}

}

void CSaveFrameDlg::SetEditFocus()
{
	m_FileName.SetFocus();
	m_FileName.SetSel(0, -1);
}

void CSaveFrameDlg::OnDblclkList() 
{
	OnBtLoad();	
}

void CSaveFrameDlg::OnSelchangeList() 
{
	int	nCurSel = m_list.GetCurSel();
	CString	szTmp;

	if (nCurSel < 0)
		return;
	
	m_list.GetText(nCurSel, szTmp);
	GetDlgItem(IDC_ET_FILENAME)->SetWindowText(szTmp);
}

BOOL CSaveFrameDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (GetDlgItem(IDC_ET_FILENAME) == GetFocus())
			{
				OnBtSave();
				return TRUE;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


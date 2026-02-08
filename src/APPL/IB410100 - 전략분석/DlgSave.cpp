// DlgSave.cpp : implementation file
//

#include "stdafx.h"
#include "IB410100.h"
#include "DlgSave.h"
#include "message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgSave dialog

CDlgSave::CDlgSave(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSave::IDD, pParent)
{
	m_pParent = pParent;
	m_pApp = (CIB410100App *)AfxGetApp();
	//{{AFX_DATA_INIT(CDlgSave)
	//}}AFX_DATA_INIT
}

void CDlgSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSave)
	DDX_Control(pDX, IDC_LIST_STRATEGY, m_ListStrategy);
	DDX_Control(pDX, IDC_EDIT_NAME, m_Edit_Name);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSave, CDialog)
	//{{AFX_MSG_MAP(CDlgSave)
	ON_BN_CLICKED(IDOK, OnSave)
	ON_NOTIFY(NM_CLICK, IDC_LIST_STRATEGY, OnClickListStrategy)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSave message handlers

BOOL CDlgSave::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	addColumn(&m_ListStrategy, "전략명", 0, 195);
	m_ListStrategy.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	setList();

	m_Edit_Name.SetLimitText(30);

	return TRUE;
}

void CDlgSave::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	m_Edit_Name.SetFocus();
}

BOOL CDlgSave::addColumn(CListCtrl *LC, LPCTSTR str, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) str;
	lvc.cx      = LC->GetStringWidth(lvc.pszText) + width;
//	lvc.iSubItem = -1;

	return LC->InsertColumn(item, &lvc);
}

void CDlgSave::setList()
{
	CFileFind finder;
	CString	file, strNo;
	CHAR inBuf[80]{};
	
	bool bContinue = TRUE;
	int ii = 0;
	while (bContinue)
	{
		strNo.Format("%02d", ii);
		file.Format("%s\\%s%s", m_sPath, STRATEGY_FILE, strNo);

		if (finder.FindFile(file))
		{
			ZeroMemory(inBuf, sizeof(inBuf));
			GetPrivateProfileString (SN_NAME, SN_PROJ, NULL, inBuf, 80, file);

			m_ListStrategy.InsertItem(LVIF_TEXT, ii, CString(inBuf), NULL, NULL, 0, 0);
		}
		else
			bContinue = FALSE;

		ii++;
	}
}

void CDlgSave::OnSave() 
{
	CString strName, iName, strMsg, strCount;
	int result = 0;

	m_Edit_Name.GetWindowText(strName);

	if (strName == _T(""))
		return;

	const int iCnt = m_ListStrategy.GetItemCount();
	int ii = 0;
	for (ii = 0 ; ii < iCnt ; ii++)
	{
		iName = m_ListStrategy.GetItemText(ii, 0);
		if (strName.Compare(iName) == 0)
		{
			strMsg.Format("%s은(는) 이미 있습니다. 바꾸시겠습니까?", strName);
			result = MessageBox(strMsg, "저장", MB_OK|MB_OKCANCEL|MB_ICONEXCLAMATION);
			break;
		}
	}

	int	nCount=0;

	switch (result)
	{
	case 0: // default OK
		nCount = m_ListStrategy.GetItemCount();
		m_pParent->SendMessage(WM_PROSave, (WPARAM) (LPCTSTR) strName, nCount);
		CDialog::OnOK();
		break;

	case 1: // MessageBox OK
		nCount = ii;
		m_pParent->SendMessage(WM_PROSave, (WPARAM) (LPCTSTR) strName, nCount);
		CDialog::OnOK();
		break;
	}

//	CDialog::OnOK();
}

void CDlgSave::OnClickListStrategy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int	nItem=0;
	CString	name;
	POSITION pos = m_ListStrategy.GetFirstSelectedItemPosition();
	
	if (pos != NULL)
	{
		nItem = m_ListStrategy.GetNextSelectedItem(pos);

		name = m_ListStrategy.GetItemText(nItem, 0);
		m_Edit_Name.SetWindowText(name);
		m_Edit_Name.SetFocus();
		m_Edit_Name.SetSel(0, name.GetLength());
	}
			
	*pResult = 0;
}

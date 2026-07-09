// choiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "choiceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ITEM_COUNT	3
/////////////////////////////////////////////////////////////////////////////
// CChoiceDlg dialog


CChoiceDlg::CChoiceDlg(CWnd* pParent, bool userScreen)
	: CDialog(CChoiceDlg::IDD, pParent)
{
	m_type       = N_NORM;
	m_userScreen = userScreen;

	//{{AFX_DATA_INIT(CChoiceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChoiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChoiceDlg)
	DDX_Control(pDX, IDC_CHOOSE, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChoiceDlg, CDialog)
	//{{AFX_MSG_MAP(CChoiceDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_CHOOSE, OnListDblClk)
	ON_NOTIFY(NM_RETURN, IDC_CHOOSE, OnListReturn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CHOOSE, OnListItemChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChoiceDlg message handlers

BOOL CChoiceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	const	char	text[ITEM_COUNT][16] = {
		"Map",	"Template",	" "
	};

	CImageList	imageList;
	imageList.Create(32, 32, ILC_COLORDDB|ILC_MASK, ITEM_COUNT, 1);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_NORMAL));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_USEROB));
	m_list.SetImageList(&imageList, LVSIL_NORMAL);
	for (int ii = 0; ii < ITEM_COUNT; ii++)
		if (ii < ITEM_COUNT-1)
			addLVItem(ii, CString(text[ii]));

	imageList.Detach();
	m_list.SetFocus();
	
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChoiceDlg::OnOK() 
{
	CDialog::OnOK();
}

void CChoiceDlg::OnListDblClk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	OnOK();
}

void CChoiceDlg::OnListReturn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	OnOK();
}

void CChoiceDlg::OnListItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1 && (pNMListView->uNewState & LVIS_SELECTED))
		m_type = NTYPE (pNMListView->lParam);
		
	*pResult = 0;
}

void CChoiceDlg::addLVItem(int nItem, CString& strItem)
{
	LV_ITEM	lvi;

	ZeroMemory(&lvi, sizeof(LV_ITEM));
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	if (nItem == 0)
		lvi.state = LVIS_SELECTED | LVIS_FOCUSED;
	else
		lvi.state = 0;
	lvi.iItem    = nItem;
	lvi.iImage   = nItem;
	lvi.lParam   = (LPARAM) nItem;
	lvi.iSubItem = 0;
	lvi.pszText  = (LPTSTR) strItem.operator LPCTSTR();
	lvi.cchTextMax = strItem.GetLength()+1;

	m_list.InsertItem(&lvi);
}

// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB771100.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, IDC_SITE_TREE, m_SiteTreeCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_NOTIFY(NM_CLICK, IDC_SITE_TREE, OnClickSiteTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

HTREEITEM CConfigDlg::FillTreeControl(HTREEITEM hParent, CString strDate)
{
	// fill items
	std::unique_ptr<TCHAR[]> FirstName = std::make_unique<TCHAR[]>(255);
	std::unique_ptr<TCHAR[]> SecndName = std::make_unique<TCHAR[]>(255);
//	TCHAR* FirstName = new TCHAR[255];
//	TCHAR* SecndName= new TCHAR[255];
	
	strcpy(FirstName.get(),"Item 1");
	strcpy(SecndName.get(),"Item 2");
	
	TV_INSERTSTRUCT	First;
	memset (&First,0,sizeof(First));
	
	First.hParent = hParent;
	First.hInsertAfter = NULL;
	First.item.mask = TVIF_STATE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	// !!! DO NOT USE Index 0 for Statusimage 
	First.item.state = INDEXTOSTATEIMAGEMASK (1);
	First.item.stateMask = 0;
	// !!! keep care when copying this string
	First.item.pszText = (char*)(LPCSTR)strDate;
	First.item.cchTextMax = 255;
	
	First.item.iImage = 0;
	First.item.iSelectedImage = 0;

	if(m_SiteTreeCtrl.GetSafeHwnd())
		return m_SiteTreeCtrl.InsertItem(&First);

	return NULL;
}

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitLoadData();
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::InitLoadData() 
{
	CRect rFrameRect;
	GetDlgItem(IDC_SITE_TREE)->GetWindowRect(rFrameRect);
	ScreenToClient(rFrameRect);
	m_SiteTreeCtrl.MoveWindow(&rFrameRect, TRUE);
	
	// TODO: Add your specialized creation code here
	HTREEITEM hTreeItem{};
	
	NewsList NewsList;
	const int nInitCount = m_arNewsList.GetSize()-4;
	int iSubcnt = 0;
	//전체(0) 을 빼고 공시(1) 부터, 마지막(조회수상위),마지막-1(스크랩) 빼고 
	for(int nIndex = 1;nIndex<nInitCount;nIndex++)
	{
		NewsList = m_arNewsList.GetAt(nIndex);
		hTreeItem = FillTreeControl(NULL, NewsList.sGubnName);
		iSubcnt = m_arNewsList.GetAt(nIndex).ibulruCnt;
		for(int j=0; j<iSubcnt-1; j++)
		{
			FillTreeControl(hTreeItem, m_arNewsList.GetAt(nIndex).sBulruNameList[j]);
		}
	}
	

}

void CConfigDlg::OnClickSiteTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR; 
	
	*pResult = 0;
}

// IndexSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axmctrl.h"
#include "IndexSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexSearchDlg dialog


CIndexSearchDlg::CIndexSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIndexSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// 부모 트리의 포인터를 간직한다.
	m_pParentTree = (CAxMTreeCtrl*)pParent;
}


void CIndexSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexSearchDlg)
	DDX_Control(pDX, IDC_LIST_RESULT, m_ResultList);
	DDX_Control(pDX, IDC_EDIT_INDEX, m_IndexEdit);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_SearchButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CIndexSearchDlg)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RESULT, OnDblclkListResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexSearchDlg message handlers

void CIndexSearchDlg::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	// "찾기" 버튼이 눌리면 트리에서 검색하고자 하는 문자열을 포함한
	// 지표 목록을 받아온다.
	SearchIndexName();
}


void CIndexSearchDlg::SearchIndexName() 
{
	// 트리에서 검색하고자 하는 문자열을 포함한
	// 지표 목록을 받아온다.
	CString			 strIndexName;

	m_IndexEdit.GetWindowText(strIndexName);
	m_TextMapItemList.RemoveAll();
	m_pParentTree->GetTreeItemTextList(&m_TextMapItemList, strIndexName);

	// 지표 목록을 받으면 리스트 컨트롤에 출력하는 작업을 진행한다.	
	// 우선 리스트 컨트롤를 초기화한다.
	m_ResultList.DeleteAllItems();

	// 리스트 컨트롤에 아이템을 추가한다.
	POSITION	position;
	CString		strFoundIndex;
	HTREEITEM	hItem;
	
	position = m_TextMapItemList.GetStartPosition();
	while (NULL != position)
	{
		m_TextMapItemList.GetNextAssoc(position, strFoundIndex, (void *&)hItem);
		m_ResultList.InsertItem(0, (LPCSTR)strFoundIndex);
	}
}



BOOL CIndexSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 리스트 컨트롤에 컬럼을 추가한다.
	m_ResultList.InsertColumn(0, "검색된 지표", LVCFMT_CENTER, 200);

	m_TextMapItemList.RemoveAll();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///void CIndexSearchDlg::OnItemdblclickListResult(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
//	// TODO: Add your control notification handler code here
//	AfxMessageBox("OK");
//	
//	*pResult = 0;
//}

void CIndexSearchDlg::OnDblclkListResult(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//AfxMessageBox("OK");
	NM_LISTVIEW *pNMListVIew = (NM_LISTVIEW*)pNMHDR;
	int			nSelected = pNMListVIew->iItem;
	CString		strSelectedIndex;
	HTREEITEM	hItem;

	// 선택된 아이템의 텍스트를 알아낸다.
	if (nSelected != -1)
	{
		strSelectedIndex = m_ResultList.GetItemText(nSelected, 0);
		m_TextMapItemList.Lookup((LPCSTR)strSelectedIndex, (void *&)hItem);

		// 부모 트리의 아이템 핸들을 넘겨 트리에서 해당 지표를 select한다.
		m_pParentTree->SelectTreeItem(hItem);

		// 지금 떠 있는 이 다이얼로그 박스를 없앤다.
		EndDialog(IDOK);
	}
	
	*pResult = 0;
}

void CIndexSearchDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CIndexSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	// 엔터키 입력시 다이얼로그 박스가 없어지지 않도록 한다.
	//CDialog::OnOK();
}

BOOL CIndexSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	// 에디트 박스에 엔터키를 치면 지표 검색하도록 한다.
	if (m_IndexEdit.GetSafeHwnd() == pMsg->hwnd)
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
			SearchIndexName();
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

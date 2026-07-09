// AccnInterestDLG.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "AccnInterestDLG.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccnInterestDLG dialog


CAccnInterestDLG::CAccnInterestDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CAccnInterestDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccnInterestDLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAccnInterestDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccnInterestDLG)
	DDX_Control(pDX, IDC_AccnTree, m_AccnTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccnInterestDLG, CDialog)
	//{{AFX_MSG_MAP(CAccnInterestDLG)
	ON_NOTIFY(NM_DBLCLK, IDC_AccnTree, OnDblclkAccnTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_AccnTree, OnSelchangedAccnTree)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccnInterestDLG message handlers

void CAccnInterestDLG::OnDblclkAccnTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_TRSymbol == "")
		return;
	
//	((CMainFrame *)AfxGetMainWnd())->SetAccnTrriger(m_TRSymbol, m_TRData);
	
	*pResult = 0;
}

void CAccnInterestDLG::OnSelchangedAccnTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	HTREEITEM rootItem;
	rootItem = m_AccnTree.GetParentItem(pNMTreeView->itemNew.hItem);
	CString data = m_AccnTree.GetItemText(pNMTreeView->itemNew.hItem);

	CString tmp;
	int	pos;
	pos = data.Find(' ');
	if (pos == -1)
		tmp = data;
	else
		tmp = data.Left(pos);
	
	if (m_AccnTree.GetParentItem(rootItem) == m_itemACCN01)
	{
		m_TRSymbol = _T("ACCN01");
		m_TRData = tmp;
		*pResult = 0;
		return;
	}

	if (m_AccnTree.GetParentItem(rootItem) == m_itemACCN02)
	{
		m_TRSymbol = _T("ACCN02");
		m_TRData = tmp;
		*pResult = 0;
		return;
	}

	m_TRSymbol = _T("");
	m_TRData = _T("");	
	
	*pResult = 0;
}

BOOL CAccnInterestDLG::SetTreeAccn(int vsn)
{
	return TRUE;
}

void CAccnInterestDLG::RedrawCtrl(bool bResize)
{
	CRect	cRc;

	GetClientRect(cRc);

	m_AccnTree.SetWindowPos(NULL, cRc.left, cRc.top, cRc.Width(), cRc.Height(), SWP_SHOWWINDOW);
}

void CAccnInterestDLG::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	//((CMainFrame *)AfxGetMainWnd())->m_AccnDLG = NULL;
	
	delete this;	
	CDialog::PostNcDestroy();
}

BOOL CAccnInterestDLG::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	AfxGetApp()->LoadIcon(IDI_CLIENT);
	
	return CDialog::PreCreateWindow(cs);
}

BOOL CAccnInterestDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_TRSymbol = _T("");
	m_TRData = _T("");

	m_image.Create(IDB_TREEIMAGE, 16, 3, RGB(255, 0, 255));

	m_AccnTree.SetImageList(&m_image, TVSIL_NORMAL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAccnInterestDLG::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if (cx == 0 && cy == 0)
		return;

	if(IsWindowVisible())
	{
		RedrawCtrl(true);
		Invalidate();
	}
	
}

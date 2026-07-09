// AccnTreeDLG.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "AccnTreeDLG.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccnTreeDLG dialog


CAccnTreeDLG::CAccnTreeDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CAccnTreeDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccnTreeDLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAccnTreeDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccnTreeDLG)
	DDX_Control(pDX, IDC_AccnTree, m_AccnTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccnTreeDLG, CDialog)
	//{{AFX_MSG_MAP(CAccnTreeDLG)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CLOSE, OnHIDE)
	ON_WM_PAINT()
	ON_NOTIFY(NM_DBLCLK, IDC_AccnTree, OnDblclkAccnTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_AccnTree, OnSelchangedAccnTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccnTreeDLG message handlers

BOOL CAccnTreeDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_TRSymbol[0] = _T("");
	m_TRSymbol[1] = _T("");
	m_TRData = _T("");

	m_image.Create(IDB_TREEIMAGE, 16, 3, RGB(255, 0, 255));

	m_AccnTree.SetImageList(&m_image, TVSIL_NORMAL);
	
//	int	vsn;
//	vsn = ((CMainFrame *)AfxGetMainWnd())->GetVSN();

//	SetTreeAccn(vsn);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAccnTreeDLG::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if (cx == 0 && cy == 0)
		return;

	if(IsWindowVisible())
	{
		RedrawCtrl(true);
		Invalidate();
	}

	// TODO: Add your message handler code here
	
}

BOOL CAccnTreeDLG::SetTreeAccn(int vsn)
{
	return false;
}

void CAccnTreeDLG::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	//DestroyWindow();
	
	CDialog::OnClose();
}

void CAccnTreeDLG::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
//	((CMainFrame *)AfxGetMainWnd())->m_AccnDLG = NULL;
	
	delete this;	
	CDialog::PostNcDestroy();
}

void CAccnTreeDLG::OnHIDE() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);	
}

void CAccnTreeDLG::RedrawCtrl(bool bResize)
{
// 	CRect	WindowRC;
// 	CRect	ClientRC;
// 	GetWindowRect(WindowRC);
// 	if (WindowRC.IsRectEmpty())
// 		return;
// 
// 	ClientRC.SetRect(0, 0, WindowRC.Width(), WindowRC.Height());
// 	ClientRC.DeflateRect(0, 0, +10, +25);

	CRect	cRc;

	GetClientRect(cRc);

	m_AccnTree.SetWindowPos(NULL, cRc.left, cRc.top, cRc.Width(), cRc.Height(), SWP_SHOWWINDOW);

	//m_AccnTree.MoveWindow(cRc);
}

void CAccnTreeDLG::OnDblclkAccnTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_TRSymbol[0] == "")
		return;
	
//	((CMainFrame *)AfxGetMainWnd())->SetAccnTrriger(m_TRSymbol[0], m_TRData);
//	((CMainFrame *)AfxGetMainWnd())->SetAccnTrriger(m_TRSymbol[1], m_TRData);

	*pResult = 0;
}

void CAccnTreeDLG::OnSelchangedAccnTree(NMHDR* pNMHDR, LRESULT* pResult) 
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
		m_TRSymbol[0] = _T("ACCN01");
		m_TRSymbol[1] = _T("ACCN05");
		m_TRData = tmp;
		*pResult = 0;
		return;
	}

	if (m_AccnTree.GetParentItem(rootItem) == m_itemACCN02)
	{
		m_TRSymbol[0] = _T("ACCN02");
		m_TRSymbol[1] = _T("ACCN06");
		m_TRData = tmp;
		*pResult = 0;
		return;
	}

	if (rootItem == m_itemACCN03)
	{
		m_TRSymbol[0] = _T("ACCN03");
		m_TRSymbol[1] = _T("ACCN05");
		m_TRData = tmp;
		*pResult = 0;
		return;
	}

	if (rootItem == m_itemACCN04)
	{
		m_TRSymbol[0] = _T("ACCN04");
		m_TRSymbol[1] = _T("ACCN06");
		m_TRData = tmp;
		*pResult = 0;
		return;
	}

	m_TRSymbol[0] = _T("");
	m_TRSymbol[1] = _T("");
	m_TRData = _T("");	
	
	*pResult = 0;
}

BOOL CAccnTreeDLG::PreCreateWindow(CREATESTRUCT& cs) 
{
	AfxGetApp()->LoadIcon(IDI_CLIENT);
	
	return CDialog::PreCreateWindow(cs);
}

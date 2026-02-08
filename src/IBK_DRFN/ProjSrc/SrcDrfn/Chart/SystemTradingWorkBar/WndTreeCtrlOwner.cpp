// WndTreeCtrlOwner.cpp : implementation file
//

#include "stdafx.h"
#include "systemtradingworkbar.h"
#include "WndTreeCtrlOwner.h"

#include "TreeCtrlSysTrading.h"
#include "../include_ST/DeclareDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		IDC_TREE	3010

/////////////////////////////////////////////////////////////////////////////
// CWndTreeCtrlOwner

CWndTreeCtrlOwner::CWndTreeCtrlOwner(UINT nTreeType) :
	m_nTreeType(nTreeType)
{
	m_pTree = NULL;
}

CWndTreeCtrlOwner::~CWndTreeCtrlOwner()
{
	if(m_pTree)
	{
		delete m_pTree;
		m_pTree = NULL;
	}
}


BEGIN_MESSAGE_MAP(CWndTreeCtrlOwner, CWnd)
	//{{AFX_MSG_MAP(CWndTreeCtrlOwner)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndTreeCtrlOwner message handlers

int CWndTreeCtrlOwner::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
			
//	m_DropTarget.Initialize(this);

	return 0;
}

void CWndTreeCtrlOwner::InitTreeCtrl(CImageList* pImageList)
{
	m_pTree = new CTreeCtrlSysTrading(m_nTreeType);
	if(!m_pTree->Create(TVS_HASBUTTONS|TVS_HASLINES|TVS_SHOWSELALWAYS|WS_BORDER|TVS_EDITLABELS, CRect(0, 0, 0, 0), this, IDC_TREE))
	{
		delete m_pTree;
		m_pTree = NULL;
	}
	else
	{
		m_pTree->EnableMultiSelect();
		m_pTree->SetImageList(pImageList, TVSIL_NORMAL);
		
		if(m_nTreeType == FT_USERFUNCTION)
			ListupUserFunction();
		else if(m_nTreeType == FT_STRATEGY)
			ListupStrategy();

		m_pTree->SetOwner(this);
	}

	m_pTree->LoadTreeGroup();
}

void CWndTreeCtrlOwner::ListupUserFunction()
{
	if(!m_pTree)
		return;

	HTREEITEM hItem = m_pTree->InsertItem(_T("모든 사용자함수"), 0, 0);
	m_pTree->SetItemBold(hItem);
	m_pTree->SetItemData(hItem, TREEINDEX_TOPGROUP);

	VT_SCRIPTINFO* pVtUserFuncInfo = theApp.GetUserFuncInfo();
	VT_SCRIPTINFO::iterator it = pVtUserFuncInfo->begin();

	HTREEITEM hSubItem = 0;
	while(it != pVtUserFuncInfo->end())
	{
		int nImage = (*it)->GetCompiledFlag();
		hSubItem = m_pTree->InsertItem((*it)->GetFunctionName(), nImage, nImage, hItem);

		if(hSubItem)
			m_pTree->SetItemData(hSubItem, (*it)->GetUniqueID());

		it++;
	}

	m_pTree->SortChildren(hItem);
	m_pTree->Expand(hItem, TVE_EXPAND);
}

void CWndTreeCtrlOwner::ListupStrategy()
{
	if(!m_pTree)
		return;

	HTREEITEM hItem = m_pTree->InsertItem(_T("모든 전략"), 0, 0);
	m_pTree->SetItemBold(hItem);
	m_pTree->SetItemData(hItem, TREEINDEX_TOPGROUP);

	VT_SCRIPTINFO* pVtUserStrategy = theApp.GetStrategyInfo();
	VT_SCRIPTINFO::iterator it = pVtUserStrategy->begin();

	HTREEITEM hSubItem = 0;
	while(it != pVtUserStrategy->end())
	{
		int nImage = (*it)->GetCompiledFlag();
		hSubItem = m_pTree->InsertItem((*it)->GetFunctionName(), nImage, nImage, hItem);

		if(hSubItem)
			m_pTree->SetItemData(hSubItem, (*it)->GetUniqueID());

		it++;
	}

	m_pTree->SortChildren(hItem);
	m_pTree->Expand(hItem, TVE_EXPAND);
}

CWnd* CWndTreeCtrlOwner::GetTreeCtrl()
{
	return m_pTree;
}

//DEL void CWndTreeCtrlOwner::OnBegindragTree(NMHDR* pNMHDR, LRESULT* pResult) 
//DEL {
//DEL 	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//DEL 	m_pTree->StartDrag(pNMTreeView);
//DEL 	
//DEL 	*pResult = 0;
//DEL }

void CWndTreeCtrlOwner::OnDestroy() 
{
	m_pTree->SaveTreeGroup();
	
	CWnd::OnDestroy();
}

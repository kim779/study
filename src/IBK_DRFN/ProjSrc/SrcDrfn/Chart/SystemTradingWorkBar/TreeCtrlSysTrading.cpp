// TreeCtrlSysTrading.cpp: implementation of the CTreeCtrlSysTrading class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "systemtradingworkbar.h"
#include "TreeCtrlSysTrading.h"

#include "../include_ST/MessageDefine.h"
#include "../include_ST/DeclareDefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define		INDEX_GROUPCOPY		6
#define		INDEX_GROUPMOVE		7

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTreeCtrlSysTrading, CXTTreeCtrl)

CTreeCtrlSysTrading::CTreeCtrlSysTrading(UINT nTreeType) :
	m_nTreeType(nTreeType),
	m_nGroupCount(0)
{
}

CTreeCtrlSysTrading::~CTreeCtrlSysTrading()
{

}

BEGIN_MESSAGE_MAP(CTreeCtrlSysTrading, CXTTreeCtrl)
	//{{AFX_MSG_MAP(CTreeCtrlSysTrading)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDM_NEWGROUP, OnNewgroup)
	ON_COMMAND(IDM_OPENFILE, OnOpenfile)
	ON_COMMAND(IDM_DELSCRIPT, OnDelscript)
	ON_UPDATE_COMMAND_UI(IDM_DELSCRIPT, OnUpdateDelscript)
	ON_COMMAND(IDM_SINGLECOMPILE, OnSinglecompile)
	ON_COMMAND(IDM_PROPERTY, OnProperty)
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_COMMAND(IDM_DELGROUP, OnDelgroup)
	ON_COMMAND(IDM_RENAMEGROUP, OnRenamegroup)
	ON_UPDATE_COMMAND_UI(IDM_DELGROUP, OnUpdateDelgroup)
	ON_UPDATE_COMMAND_UI(IDM_RENAMEGROUP, OnUpdateRenamegroup)
	ON_UPDATE_COMMAND_UI(IDM_OPENFILE, OnUpdateOpenfile)
	ON_UPDATE_COMMAND_UI(IDM_PROPERTY, OnUpdateProperty)
	ON_COMMAND(ID_BUILD_SINGLE_COMPILE_DLL, OnBuildSingleCompileDll)
	ON_UPDATE_COMMAND_UI(IDM_NEWGROUP, OnUpdateNewgroup)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_UPDATE_COMMAND_UI(IDM_SINGLECOMPILE, OnUpdatSinglecompile)
	ON_UPDATE_COMMAND_UI(ID_BUILD_SINGLE_COMPILE_DLL, OnUpdatBuildSingleCompileDll)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDM_GROUPCOPY_BEGIN, IDM_GORUPCOPY_END, OnGroupCopy)
	ON_COMMAND_RANGE(IDM_GROUPMOVE_BEGIN, IDM_GORUPMOVE_END, OnGroupMove)
	ON_UPDATE_COMMAND_UI_RANGE(IDM_GROUPCOPY_BEGIN, IDM_GORUPCOPY_END, OnUpdateGroupCopy)
	ON_UPDATE_COMMAND_UI_RANGE(IDM_GROUPMOVE_BEGIN, IDM_GORUPMOVE_END, OnUpdateGroupMove)
END_MESSAGE_MAP()

void CTreeCtrlSysTrading::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CXTTreeCtrl::OnRButtonDown(nFlags, point);
	HTREEITEM hItem = GetFirstSelectedItem();
	if(!hItem)
		return;
	
	CMenu* pPopupMenu = m_mnuPopup.GetSubMenu(0);
	//	DWORD dwData = GetItemData(hItem);
	ClientToScreen(&point);

//	SetMenuStatement(hItem, pPopupMenu);

//	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y, this);
	CXTPCommandBars::TrackPopupMenu(pPopupMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CTreeCtrlSysTrading::SetMenuStatement(HTREEITEM hItem, CMenu* pPopupMenu)
{
	CString strItem = GetItemText(hItem);

//	MENUITEMINFO info = {sizeof(MENUITEMINFO), MIIM_STATE};
	
	if(!strItem.CompareNoCase(_T("모든 전략")) || !strItem.CompareNoCase(_T("모든 사용자함수")))
	{
		int nPos[] = {1, 2, 4, 5, 6, 7, 9, 10};
		for(int i = 0; i < 8; i++)
			pPopupMenu->DeleteMenu(nPos[i], MF_BYPOSITION);
	}
}

void CTreeCtrlSysTrading::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	HTREEITEM hItem = GetFirstSelectedItem();
	if(hItem)
		OpenSelectedItem(hItem);
	
	CXTTreeCtrl::OnLButtonDblClk(nFlags, point);
}

void CTreeCtrlSysTrading::OpenSelectedItem(HTREEITEM hItem)
{
	if(m_nTreeType == FT_ALLFILE)
	{
		int nImage, nSelImage;
		GetItemImage(hItem, nImage, nSelImage);

		CString strItemName = GetItemText(hItem);
		if(nImage < 7)
			theApp.GetFrameMainWnd()->SendMessage(RMSG_OPENFILE, FT_STRATEGY, (LPARAM)(LPCSTR)strItemName);
		else
			theApp.GetFrameMainWnd()->SendMessage(RMSG_OPENFILE, FT_USERFUNCTION, (LPARAM)(LPCSTR)strItemName);

		return;
	}
	
	DWORD dwData = GetItemData(hItem);
	if(dwData < TREEINDEX_DEFUALTITEM)
		return;
	
	CString strItemName = GetItemText(hItem);
	theApp.GetFrameMainWnd()->SendMessage(RMSG_OPENFILE, m_nTreeType, (LPARAM)(LPCSTR)strItemName);
}

void CTreeCtrlSysTrading::OnNewgroup() 
{
	char szName[32];
	::lstrcpy(szName, _T("새로운 그룹"));

	int nIndex = 1;
	while(FindItem(szName, NULL, TRUE))
		::wsprintf(szName, _T("새로운 그룹(%d)"), nIndex++);

	AddNewGroup(szName);
}

HTREEITEM CTreeCtrlSysTrading::AddNewGroup(LPCSTR szName)
{
	HTREEITEM hItem = InsertItem(szName, 0, 0);
	SetItemData(hItem, TREEINDEX_GROUP);
	SetItemBold(hItem);

	CMenu* pGroupMenu = m_mnuPopup.GetSubMenu(0)->GetSubMenu(INDEX_GROUPCOPY);
	if(m_nGroupCount == 0)
		pGroupMenu->ModifyMenu(IDM_NOGROUP, MF_BYCOMMAND, m_nMenuCopyID++, szName);
	else
		pGroupMenu->AppendMenu(MF_STRING, m_nMenuCopyID++, szName);

	pGroupMenu = m_mnuPopup.GetSubMenu(0)->GetSubMenu(INDEX_GROUPMOVE);
	if(m_nGroupCount == 0)
		pGroupMenu->ModifyMenu(IDM_NOGROUP, MF_BYCOMMAND, m_nMenuMoveID++, szName);
	else
		pGroupMenu->AppendMenu(MF_STRING, m_nMenuMoveID++, szName);

	m_nGroupCount++;

	return hItem;
}

void CTreeCtrlSysTrading::OnOpenfile() 
{
	HTREEITEM hItem = GetFirstSelectedItem();
	while(hItem)
	{
		OpenSelectedItem(hItem);
		hItem = GetNextSelectedItem(hItem);
	}
}

void CTreeCtrlSysTrading::OnDelscript() 
{
	CArray<HTREEITEM, HTREEITEM> arhItem;

	HTREEITEM hItem = GetFirstSelectedItem();

	while(hItem)
	{
		arhItem.Add(hItem);
		hItem = GetNextSelectedItem(hItem);
	}

	for(int i = 0; i < arhItem.GetSize(); i++)
		DelSelectedItem(arhItem.GetAt(i));
}

void CTreeCtrlSysTrading::DelSelectedItem(HTREEITEM hItem)
{
	DWORD dwData = GetItemData(hItem);
	if(dwData < TREEINDEX_DEFUALTITEM)
	{
		SetItemState(hItem, ~TVIS_SELECTED, TVIS_SELECTED);
		return;
	}
	
	HTREEITEM hItemSub = GetParentItem(hItem);
	DWORD dwItem = GetItemData(hItemSub);
	BOOL bDeleteFile = dwItem == TREEINDEX_TOPGROUP;
	CString strText = GetItemText(hItem);

	if(dwItem == TREEINDEX_GROUP)
	{
		if(AfxMessageBox("영구 삭제하시겠습니까? '아니오'를 선택하면 현재 그룹의 항목만 삭제합니다.", MB_YESNO) == IDYES)
			bDeleteFile = TRUE;
	}

	DeleteItem(hItem);

	if(bDeleteFile)
	{
		theApp.GetFrameMainWnd()->SendMessage(RMSG_DELFILE, m_nTreeType, (LPARAM)dwData);

		hItem = FindItem(strText, NULL, FALSE);
		while(hItem)
		{
			DeleteItem(hItem);
			hItem = FindItem(strText, NULL, FALSE);
		}
	}
}

void CTreeCtrlSysTrading::OnSinglecompile() 
{
	HTREEITEM hItem = GetFirstSelectedItem();

	if(GetNextSelectedItem(hItem))
		MultiCompileSelectedItem(hItem, FALSE);
	else
		SingleCompileSelectedItem(hItem, FALSE);

//	hItem = GetNextSelectedItem(hItem);
//
//	while(hItem)
//	{
//		SingleCompileSelectedItem(hItem, FALSE);
//		hItem = GetNextSelectedItem(hItem);
//	}
}

void CTreeCtrlSysTrading::MultiCompileSelectedItem(HTREEITEM hItem, BOOL bMakeDll)
{
	CStringArray arScriptName;

	while(hItem)
	{
		if(GetItemData(hItem) < TREEINDEX_DEFUALTITEM)
			continue;

		arScriptName.Add(GetItemText(hItem));
		hItem = GetNextSelectedItem(hItem);
	}

	if(arScriptName.GetSize() == 0)
		return;

	if(m_nTreeType == FT_STRATEGY)
		theApp.GetFrameMainWnd()->SendMessage(RMSG_MULTICOMPILEDLLMAIN_TREE, bMakeDll, (LPARAM)&arScriptName);
	else if(m_nTreeType == FT_USERFUNCTION)
		theApp.GetFrameMainWnd()->SendMessage(RMSG_MULTICOMPILEDLLFUNCTION_TREE, bMakeDll, (LPARAM)&arScriptName);
}

void CTreeCtrlSysTrading::SingleCompileSelectedItem(HTREEITEM hItem, BOOL bMakeDll)
{
	CString strItem = GetItemText(hItem);

	if(m_nTreeType == FT_ALLFILE)
	{
		int nImage, nSelImage;
		GetItemImage(hItem, nImage, nSelImage);

		if(nImage < 7)
			theApp.GetFrameMainWnd()->SendMessage(RMSG_SINGLIECOMPILEDLLMAIN_TREE, bMakeDll, (LPARAM)(LPSTR)(LPCTSTR)strItem);
		else
			theApp.GetFrameMainWnd()->SendMessage(RMSG_SINGLIECOMPILEDLLFUNCTION_TREE, bMakeDll, (LPARAM)(LPSTR)(LPCTSTR)strItem);

		return;
	}
	
	DWORD dwData = GetItemData(hItem);
	if(dwData < TREEINDEX_DEFUALTITEM)
		return;
	
	if(m_nTreeType == FT_STRATEGY)
		theApp.GetFrameMainWnd()->SendMessage(RMSG_SINGLIECOMPILEDLLMAIN_TREE, bMakeDll, (LPARAM)(LPSTR)(LPCTSTR)strItem);
	else if(m_nTreeType == FT_USERFUNCTION)
		theApp.GetFrameMainWnd()->SendMessage(RMSG_SINGLIECOMPILEDLLFUNCTION_TREE, bMakeDll, (LPARAM)(LPSTR)(LPCTSTR)strItem);
}

void CTreeCtrlSysTrading::OnProperty() 
{
	HTREEITEM hItem = GetFirstSelectedItem();
	DWORD dwData = GetItemData(hItem);

	CString strItem = GetItemText(hItem);

	if(m_nTreeType == FT_ALLFILE)
	{
		int nImage, nSelImage;
		GetItemImage(hItem, nImage, nSelImage);

		if(nImage < 7)
			theApp.GetFrameMainWnd()->SendMessage(RMSG_VIEWPROPERTY, FT_STRATEGY, (LPARAM)(LPSTR)(LPCTSTR)strItem);
		else
			theApp.GetFrameMainWnd()->SendMessage(RMSG_VIEWPROPERTY, FT_USERFUNCTION, (LPARAM)(LPSTR)(LPCTSTR)strItem);

		return;
	}

	if(dwData < TREEINDEX_DEFUALTITEM)
		return;
	
	theApp.GetFrameMainWnd()->SendMessage(RMSG_VIEWPROPERTY, m_nTreeType, (LPARAM)(LPSTR)(LPCTSTR)strItem);
}

int CTreeCtrlSysTrading::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	XTPImageManager()->SetIcons(IDR_TOOLBAR_POPUP);

	m_nMenuCopyID = IDM_GROUPCOPY_BEGIN;
	m_nMenuMoveID = IDM_GROUPMOVE_BEGIN;
	m_mnuPopup.LoadMenu(IDR_MENU_POPUP);
		
	m_DropTaget.Initialize(this);
	m_DropTaget2.Register(this);

	return 0;
}

void CTreeCtrlSysTrading::StartDrag(NM_TREEVIEW* pNMTreeView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	CMyDragObject* MyObj = new CMyDragObject();

	CString Text;
	int nCnt = GetSelectedCount();
	if(nCnt > 1)
	{
//		MyObj->m_bMulti = TRUE;


		HTREEITEM hItem = GetFirstSelectedItem();
		while(hItem) 
		{
			CString strItem = GetItemText(hItem);

//			if(!GetChildItem(hItem))
//				MyObj->m_arData.Add(GetItemText(hItem));
			hItem = GetNextSelectedItem(hItem);
		}
	}
	else
	{
//		MyObj->m_bMulti = FALSE;
		HTREEITEM hItem = GetSelectedItem();

//		if(!GetChildItem(hItem))
//			MyObj->Data = GetItemText(GetSelectedItem());
	}

//	if(::strlen(MyObj->Data) || MyObj->m_arData.GetSize())
	{
		CRect rcClient;
		GetClientRect(rcClient);
//		ClientToScreen(rcClient);
		
		CPoint point(pNMTreeView->ptDrag);

//		BOOL ba = rcClient.PtInRect(point);
//		DROPEFFECT RETURNA = m_DropSource.StartDragging((DWORD)this, &rcClient, &point);
	}

//	delete MyObj;
}

void CTreeCtrlSysTrading::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CXTTreeCtrl::OnLButtonUp(nFlags, point);
}

void CTreeCtrlSysTrading::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	*pResult = 0;

	if(pTVDispInfo->item.pszText && ::lstrlen(pTVDispInfo->item.pszText))
	{
		HTREEITEM hNewItem = FindItem(pTVDispInfo->item.pszText, pTVDispInfo->item.hItem, TRUE);
		if(hNewItem && hNewItem != pTVDispInfo->item.hItem)
		{
			CString strMessage;
			strMessage.Format(_T("'%s' 존재하는 그룹이름입니다."), pTVDispInfo->item.pszText);
			
			MessageBox(strMessage, _T("그룹이름 변경"));
			return;
		}

		if(m_nGroupCount)
		{
			CString strItem = GetItemText(pTVDispInfo->item.hItem);
			CMenu* pGroupMenu = m_mnuPopup.GetSubMenu(0)->GetSubMenu(INDEX_GROUPCOPY);
			CMenu* pGroupMenu2 = m_mnuPopup.GetSubMenu(0)->GetSubMenu(INDEX_GROUPMOVE);

			CString strMenu;
			for(int i = 0; i < m_nGroupCount; i++)
			{
				pGroupMenu->GetMenuString(i, strMenu, MF_BYPOSITION);
				if(!strMenu.CompareNoCase(strItem))
				{
					UINT nID = pGroupMenu->GetMenuItemID(i);
					pGroupMenu->ModifyMenu(i, MF_BYPOSITION, nID, pTVDispInfo->item.pszText);

					nID = pGroupMenu2->GetMenuItemID(i);
					pGroupMenu2->ModifyMenu(i, MF_BYPOSITION, nID, pTVDispInfo->item.pszText);

					break;
				}
			}
		}
		
		SetItemText(pTVDispInfo->item.hItem, pTVDispInfo->item.pszText);
	}
}

HTREEITEM CTreeCtrlSysTrading::FindItem(LPCTSTR lpszSearch, HTREEITEM hItem, BOOL bOnlyRoot)
{
	CString str = lpszSearch;

	int lenSearchStr = str.GetLength();
	if (lenSearchStr == 0) {
		return NULL;
	}

	// For the first pass, set the current item equal to the selection
	HTREEITEM htiSel = hItem ? hItem : GetSelectedItem();
	
	// If NULL, use root item.
	if (htiSel == NULL)
		htiSel = GetRootItem();

	HTREEITEM htiCur = htiSel;
	CString strSearch = str;

	// make sure it ends if we started with no selection
	if ((htiCur == NULL) && (htiSel != NULL))
		htiCur = GetRootItem();

	// For the first pass only, we check to see if it 
	// is the item we're looking for.
	BOOL bFirstPass = TRUE;
	DWORD dwGroup = 0;
	while( htiCur && (htiCur != htiSel || bFirstPass) )
	{
		bFirstPass = FALSE;
		dwGroup = GetItemData(htiCur);

		if(bOnlyRoot)
		{
			if(dwGroup != TREEINDEX_GROUP)
			{
				htiCur = GetNextItem( htiCur );
				if( htiCur == NULL )
					htiCur = GetRootItem();
				continue;
			}
		}
		else
			if(dwGroup == TREEINDEX_GROUP)
			{
				htiCur = GetNextItem( htiCur );
				if( htiCur == NULL )
					htiCur = GetRootItem();
				continue;
			}

		CString strItemText = GetItemText( htiCur );
		if(!strItemText.CompareNoCase(strSearch))
		{
			if (IsFindValid(htiCur))
				return htiCur;
		}

		htiCur = GetNextItem( htiCur );
		if( htiCur == NULL )
			htiCur = GetRootItem();
	}

	return NULL;
}

void CTreeCtrlSysTrading::OnGroupCopy(UINT nID)
{
	CMenu* pGroupMenu = m_mnuPopup.GetSubMenu(0)->GetSubMenu(INDEX_GROUPCOPY);
	CString strMenuText;
	pGroupMenu->GetMenuString(nID, strMenuText, MF_BYCOMMAND);

	HTREEITEM hItem = FindItem(strMenuText, NULL, TRUE);
	if(hItem)
	{
		HTREEITEM hSubItem = GetFirstSelectedItem();
		while(hSubItem)
		{
			UINT nUniqueID = GetItemData(hSubItem);
			int nImage = 0;
			CScriptFind fs(nUniqueID);

			if(m_nTreeType == FT_STRATEGY)
			{
				VT_SCRIPTINFO::iterator it = std::find_if(theApp.GetStrategyInfo()->begin(), theApp.GetStrategyInfo()->end(), fs);
				if(it != theApp.GetStrategyInfo()->end())
					nImage = (*it)->GetCompiledFlag();
			}
			else if(m_nTreeType == FT_USERFUNCTION)
			{
				VT_SCRIPTINFO::iterator it = std::find_if(theApp.GetUserFuncInfo()->begin(), theApp.GetUserFuncInfo()->end(), fs);
				if(it != theApp.GetUserFuncInfo()->end())
					nImage = (*it)->GetCompiledFlag();
			}

			HTREEITEM hNewItem = InsertItem(GetItemText(hSubItem), nImage, nImage, hItem);
			SetItemData(hNewItem, nUniqueID);

			hSubItem = GetNextSelectedItem(hSubItem);
		}
	}

	Expand(hItem, TVE_EXPAND);
}

void CTreeCtrlSysTrading::OnGroupMove(UINT nID)
{
	CMenu* pGroupMenu = m_mnuPopup.GetSubMenu(0)->GetSubMenu(INDEX_GROUPMOVE);
	CString strMenuText;
	pGroupMenu->GetMenuString(nID, strMenuText, MF_BYCOMMAND);

	CArray<HTREEITEM, HTREEITEM&> arDelItem;

	HTREEITEM hItem = FindItem(strMenuText, NULL, TRUE);
	if(hItem)
	{
		HTREEITEM hSubItem = GetFirstSelectedItem();
		while(hSubItem)
		{
			arDelItem.Add(hSubItem);

			UINT nUniqueID = GetItemData(hSubItem);
			int nImage = 0;
			CScriptFind fs(nUniqueID);

			if(m_nTreeType == FT_STRATEGY)
			{
				VT_SCRIPTINFO::iterator it = std::find_if(theApp.GetStrategyInfo()->begin(), theApp.GetStrategyInfo()->end(), fs);
				if(it != theApp.GetStrategyInfo()->end())
					nImage = (*it)->GetCompiledFlag();
			}
			else if(m_nTreeType == FT_USERFUNCTION)
			{
				VT_SCRIPTINFO::iterator it = std::find_if(theApp.GetUserFuncInfo()->begin(), theApp.GetUserFuncInfo()->end(), fs);
				if(it != theApp.GetUserFuncInfo()->end())
					nImage = (*it)->GetCompiledFlag();
			}

			HTREEITEM hNewItem = InsertItem(GetItemText(hSubItem), nImage, nImage, hItem);
			SetItemData(hNewItem, nUniqueID);

			hSubItem = GetNextSelectedItem(hSubItem);
		}
	}

	Expand(hItem, TVE_EXPAND);

	for(int i = 0; i < arDelItem.GetSize(); i++)
	{
		hItem = arDelItem.GetAt(i);
		if(hItem)
			DeleteItem(hItem);
	}
}

void CTreeCtrlSysTrading::OnDelgroup() 
{
	HTREEITEM hItem = GetFirstSelectedItem();

	if(GetItemData(hItem) >= TREEINDEX_DEFUALTITEM)
		hItem = GetParentItem(hItem);

	CMenu* pGroupMenu = m_mnuPopup.GetSubMenu(0)->GetSubMenu(INDEX_GROUPCOPY);
	CString strName = GetItemText(hItem);
	CString strMenuName;
	for(int i = 0; i < m_nGroupCount; i++)
	{
		pGroupMenu->GetMenuString(i, strMenuName, MF_BYPOSITION);
		if(!strName.CompareNoCase(strMenuName))
		{
			pGroupMenu->DeleteMenu(i, MF_BYPOSITION);
			break;
		}
	}

	DeleteItem(hItem);
}

void CTreeCtrlSysTrading::OnRenamegroup() 
{
	HTREEITEM hItem = GetFirstSelectedItem();

	if(GetItemData(hItem) >= TREEINDEX_DEFUALTITEM)
		hItem = GetParentItem(hItem);
	
	EditLabel(hItem);
}

void CTreeCtrlSysTrading::SetMenuStatementGroup(CCmdUI* pCmdUI)
{
	if(m_nTreeType == FT_ALLFILE)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	HTREEITEM hItem = GetFirstSelectedItem();

	if(GetItemData(hItem) == TREEINDEX_TOPGROUP)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		hItem = GetParentItem(hItem);
		if(hItem && GetItemData(hItem) == TREEINDEX_TOPGROUP)
			pCmdUI->Enable(FALSE);
	}
}

void CTreeCtrlSysTrading::SetMenuStatementScript(CCmdUI* pCmdUI)
{
	HTREEITEM hItem = GetFirstSelectedItem();
	DWORD dwData = GetItemData(hItem);

	if(dwData < TREEINDEX_DEFUALTITEM)
	{
		pCmdUI->Enable(FALSE);
	}
/*
	else
	{
		hItem = GetParentItem(hItem);
		if(hItem && GetItemData(hItem) == TREEINDEX_TOPGROUP)
			pCmdUI->Enable(FALSE);
	}
*/
}

void CTreeCtrlSysTrading::OnUpdateDelgroup(CCmdUI* pCmdUI) 
{
	SetMenuStatementGroup(pCmdUI);
}

void CTreeCtrlSysTrading::OnUpdateRenamegroup(CCmdUI* pCmdUI) 
{
	SetMenuStatementGroup(pCmdUI);
}

void CTreeCtrlSysTrading::OnUpdateOpenfile(CCmdUI* pCmdUI) 
{
	if(m_nTreeType == FT_ALLFILE)
		pCmdUI->Enable(TRUE);
	else
		SetMenuStatementScript(pCmdUI);
}

void CTreeCtrlSysTrading::OnUpdateDelscript(CCmdUI* pCmdUI) 
{
	if(m_nTreeType == FT_ALLFILE)
		pCmdUI->Enable(FALSE);
	else
		SetMenuStatementScript(pCmdUI);
}

void CTreeCtrlSysTrading::OnUpdateProperty(CCmdUI* pCmdUI) 
{
	if(m_nTreeType == FT_ALLFILE)
		pCmdUI->Enable(TRUE);
	else
		SetMenuStatementScript(pCmdUI);
}

void CTreeCtrlSysTrading::OnBuildSingleCompileDll() 
{
	HTREEITEM hItem = GetFirstSelectedItem();
	if(GetNextSelectedItem(hItem))
		MultiCompileSelectedItem(hItem, TRUE);
	else
		SingleCompileSelectedItem(hItem, TRUE);

//	hItem = GetNextSelectedItem(hItem);
//	while(hItem)
//	{
//		SingleCompileSelectedItem(hItem, TRUE);
//		hItem = GetNextSelectedItem(hItem);
//	}
}

void CTreeCtrlSysTrading::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	if(m_nTreeType == FT_ALLFILE)
	{
		*pResult = 1;
		return;
	}

	*pResult = 0;

	if(pTVDispInfo->item.hItem)
	{
		DWORD dwItem = GetItemData(pTVDispInfo->item.hItem);
		if(dwItem >= TREEINDEX_DEFUALTITEM || dwItem == TREEINDEX_TOPGROUP)
		{
			pTVDispInfo->item.hItem = NULL;
			*pResult = -1;
		}
	}
}

void CTreeCtrlSysTrading::OnUpdateGroupMove(CCmdUI* pCmdUI)
{
	SetMenuStatementGroup(pCmdUI);

	if(!pCmdUI->m_bEnableChanged)
	{
		HTREEITEM hItem = GetFirstSelectedItem();
		if(GetItemData(hItem) >= TREEINDEX_DEFUALTITEM)
			hItem = GetParentItem(hItem);

		CString strItem = GetItemText(hItem);
		CString strMenu;
		CMenu* pGroupMenu = m_mnuPopup.GetSubMenu(0)->GetSubMenu(INDEX_GROUPMOVE);
		pGroupMenu->GetMenuString(pCmdUI->m_nID, strMenu, MF_BYCOMMAND);

		if(!strItem.CompareNoCase(strMenu))
			pCmdUI->Enable(FALSE);
	}
}

void CTreeCtrlSysTrading::OnUpdateGroupCopy(CCmdUI* pCmdUI)
{
	HTREEITEM hItem = GetFirstSelectedItem();
	if(GetItemData(hItem) >= TREEINDEX_DEFUALTITEM)
		hItem = GetParentItem(hItem);

	CString strItem = GetItemText(hItem);
	CString strMenu;
	CMenu* pGroupMenu = m_mnuPopup.GetSubMenu(0)->GetSubMenu(INDEX_GROUPCOPY);
	pGroupMenu->GetMenuString(pCmdUI->m_nID, strMenu, MF_BYCOMMAND);

	if(!strItem.CompareNoCase(strMenu))
		pCmdUI->Enable(FALSE);
}

void CTreeCtrlSysTrading::SaveTreeGroup()
{
	char szFile[MAX_PATH] = {NULL, };
	if(m_nTreeType == FT_STRATEGY)
//@유진삭제::wsprintf(szFile, _T("%s%sGroupItemS.sav"), theApp.GetRootPath(), PATH_USER);
		::wsprintf(szFile, _T("%s%sGroupItemS.sav"), theApp.GetUserPath(), PATH_USER2);
	else if(m_nTreeType == FT_USERFUNCTION)
//@유진삭제::wsprintf(szFile, _T("%s%sGroupItemF.sav"), theApp.GetRootPath(), PATH_USER);
		::wsprintf(szFile, _T("%s%sGroupItemF.sav"), theApp.GetUserPath(), PATH_USER2);
	else
		return;

	HTREEITEM hItem = GetRootItem();
	hItem = GetNextItem(hItem, TVGN_NEXT);

	if(!hItem)
	{
		::DeleteFile(szFile);
		return;
	}

	CStdioFile fileSave(szFile, CFile::modeCreate|CFile::modeWrite);

	CString strSave, strID;
	
	while(hItem)
	{
		strSave = GetItemText(hItem);
		strSave += _T("|");

		HTREEITEM hSubItem = GetNextItem(hItem, TVGN_CHILD);
		while(hSubItem)
		{
			strSave += GetItemText(hSubItem);
			strSave += _T("|");
			
			DWORD dwID = GetItemData(hSubItem);
			CScriptFind fs(dwID);
			if(m_nTreeType == FT_STRATEGY)
			{
				VT_SCRIPTINFO::iterator it = std::find_if(theApp.GetStrategyInfo()->begin(), theApp.GetStrategyInfo()->end(), fs);
				if(it != theApp.GetStrategyInfo()->end())
				{
					CString strIndex;
					strIndex.Format(_T("%d"), (*it)->GetCompiledFlag());
					strSave += strIndex;
				}
			}
			else if(m_nTreeType == FT_USERFUNCTION)
			{
				VT_SCRIPTINFO::iterator it = std::find_if(theApp.GetUserFuncInfo()->begin(), theApp.GetUserFuncInfo()->end(), fs);
				if(it != theApp.GetUserFuncInfo()->end())
				{
					CString strIndex;
					strIndex.Format(_T("%d"), (*it)->GetCompiledFlag());
					strSave += strIndex;
				}
			}
			
			strSave += _T("|");
	
			strID.Format(_T("%d"), dwID);
			strSave += strID;

			strSave += _T("|");
			
			strID.Empty();
			hSubItem = GetNextItem(hSubItem, TVGN_NEXT);
		}

		fileSave.WriteString(strSave);
		fileSave.Write("\n", 1);
		strSave.Empty();

		hItem = GetNextItem(hItem, TVGN_NEXT);
	}

	fileSave.Close();
}

void CTreeCtrlSysTrading::LoadTreeGroup()
{
	char szFile[MAX_PATH] = {NULL, };
	if(m_nTreeType == FT_STRATEGY)
//@유진삭제::wsprintf(szFile, _T("%s%sGroupItemS.sav"), theApp.GetRootPath(), PATH_USER);
		::wsprintf(szFile, _T("%s%sGroupItemS.sav"), theApp.GetUserPath(), PATH_USER2);
	else if(m_nTreeType == FT_USERFUNCTION)
//@유진삭제::wsprintf(szFile, _T("%s%sGroupItemF.sav"), theApp.GetRootPath(), PATH_USER);
		::wsprintf(szFile, _T("%s%sGroupItemF.sav"), theApp.GetUserPath(), PATH_USER2);
	else
		return;

	CFile fileLoad;
	if(!fileLoad.Open(szFile, CFile::modeRead))
		return;
	
	UINT nSize = fileLoad.GetLength();
	if(!nSize)
		return;

	CString strLoad;
	char* szRead = new char[nSize + 1];
	::memset(szRead, 0x00, nSize + 1);

	fileLoad.Read(szRead, nSize);
	fileLoad.Close();

	int i = 0, j = 0;
	CString strLine;
	while(AfxExtractSubString(strLine, szRead, i++))
	{
		if(!strLine.GetLength())
			break;

		CString strItem;
		AfxExtractSubString(strItem, strLine, j++, '|');
		HTREEITEM hItem = AddNewGroup(strItem);
		
		CString strName(_T(""));
		int nImage = 0;
		DWORD dwID = 0;
		while(AfxExtractSubString(strItem, strLine, j, '|'))
		{
			int nIndex = j % 3;
			if(nIndex == 1)
				strName = strItem;

			if(nIndex == 2)
				nImage = atoi(strItem);

			if(nIndex == 0)
			{
				dwID = atoi(strItem);
				if(strName.GetLength())
				{
					HTREEITEM hNewItem = InsertItem(strName, nImage, nImage, hItem);
					SetItemData(hNewItem, dwID);
				}

				strName.Empty();
				nImage = 0;
				dwID = 0;
			}

			j++;
		}

		SortChildren(hItem);
		Expand(hItem, TVE_EXPAND);
		j = 0;
	}

	delete [] szRead;
}

void CTreeCtrlSysTrading::OnUpdateNewgroup(CCmdUI* pCmdUI) 
{
	if(m_nTreeType == FT_ALLFILE)
		pCmdUI->Enable(FALSE);
}

void CTreeCtrlSysTrading::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

/*
	CRect rcClient;
	GetClientRect(rcClient);
	
	CPoint point(pNMTreeView->ptDrag);
	CStartDrag* pStartDrag = new CStartDrag(this, &rcClient, &point);

	theApp.GetFrameMainWnd()->PostMessage(RMSG_STARTDRAG, 0, (LPARAM)pStartDrag);
*/

/*
	HTREEITEM hTSelItem = pNMTreeView->itemNew.hItem;
	// Highlight selected item
	SelectItem(hTSelItem);
	Select(hTSelItem, TVGN_DROPHILITE);
	
	COleDataSource *poleSourceObj = new COleDataSource ;
	CTreeDropTarget::m_shWndTreeCtrl = theApp.GetFrameMainWnd()->m_hWnd;
	// Begin Drag operation

	DROPEFFECT dropeffect = poleSourceObj->DoDragDrop();
	// Remove the highlighting
	SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE,0);
	// If user is moving item by pressing Shift, delete selected item
	if ( dropeffect == DROPEFFECT_MOVE)
		DeleteItem(hTSelItem); 
	delete poleSourceObj;
*/

	*pResult = 0;
}

void CTreeCtrlSysTrading::OnUpdatSinglecompile(CCmdUI* pCmdUI) 
{
	if(m_nTreeType == FT_ALLFILE)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(!theApp.GetFrameMainWnd()->SendMessage(RMSG_GETCOMPILESTATE_TREE));
}

void CTreeCtrlSysTrading::OnUpdatBuildSingleCompileDll(CCmdUI* pCmdUI) 
{
	if(m_nTreeType == FT_ALLFILE)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(!theApp.GetFrameMainWnd()->SendMessage(RMSG_GETCOMPILESTATE_TREE));
}
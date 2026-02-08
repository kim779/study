// STTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "STTreeCtrl.h"
#include "../../inc/ISTManager.h" // 시스템트레이딩 관련된 컨텐츠 인터페이스

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT RMSG_OPENNEWMAP = ::RegisterWindowMessage("RMSG_OPENNEWMAP");

/////////////////////////////////////////////////////////////////////////////
// CSTTreeCtrl

CSTTreeCtrl::CSTTreeCtrl()
{
	m_hSelTreeItem = NULL;
	m_hTreeUser = NULL;
}

CSTTreeCtrl::~CSTTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CSTTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CSTTreeCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSTTreeCtrl::OnDestroy()
{
	//TRACE("\n----------------Start---------------------\n");
	DeleteAllItemData(GetRootItem());
	//TRACE("\n-----------------End--------------------\n");
	CTreeCtrl::OnDestroy();
}

void CSTTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UINT nHF;
	m_hSelTreeItem = HitTest(point, &nHF);
	if(m_hSelTreeItem && !ItemHasChildren(m_hSelTreeItem))
	{
		STMNG_ITEMINFO_EX1 itemInfo;
		itemInfo.hParentItem = NULL;
		itemInfo.hTreeItem = m_hSelTreeItem;
		itemInfo.bIsItem = !ItemHasChildren(m_hSelTreeItem);

		// 폴더형일때는 정보를 세팅한다.
		if(itemInfo.bIsItem)
		{
			STTREEITEM_INFO* pItemInfo = (STTREEITEM_INFO*)GetItemData(m_hSelTreeItem);
			itemInfo.hParentItem = GetParentItem(m_hSelTreeItem);

			if(itemInfo.hParentItem)
			{
				STTREEFOLDER_INFO* pInfo = (STTREEFOLDER_INFO*)GetItemData(itemInfo.hParentItem);
				if(pInfo && pItemInfo)
				{
					itemInfo.nSTLevel = pInfo->nSTLevel;
					itemInfo.nSTType	= pInfo->nSTType;

					itemInfo.szSTRData = GetItemText(itemInfo.hParentItem);
					itemInfo.szSTName = pItemInfo->szRawName ;		// GetItemText(m_hSelTreeItem);
					itemInfo.szSTPath = pInfo->szBaseFolder;		// 선택된 전략의 폴더경로
					itemInfo.nFolderIndx = 0;						// Tree형에서는 의미없음.
					itemInfo.nItemIndx	 = 0;						// Tree형에서는 의미없음.

					GetParent()->GetParent()->SendMessage(RMSG_STTREE_ITEMCLICK, (WPARAM)m_hWnd, (LPARAM)&itemInfo);
				}
			}
		}
	}

	CTreeCtrl::OnLButtonDown(nFlags, point);
}


BOOL CSTTreeCtrl::Create(DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID)
{
	CTreeCtrl::Create(dwStyle|TVS_HASLINES|TVS_LINESATROOT|TVS_DISABLEDRAGDROP|TVS_HASBUTTONS|TVS_NOTOOLTIPS,
												CRect(0, 0, 0, 0),
												pParentWnd,	
												nID);
	CTreeCtrl::ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	return TRUE;
}

void CSTTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	STMNG_ITEMINFO_EX1 itemInfo;
	itemInfo.hParentItem = NULL;
	itemInfo.hTreeItem = m_hSelTreeItem;
	itemInfo.bIsItem = !ItemHasChildren(m_hSelTreeItem);

	// 폴더형일때는 정보를 세팅한다.
	if(itemInfo.bIsItem)
	{
		STTREEITEM_INFO* pItemInfo = (STTREEITEM_INFO*)GetItemData(m_hSelTreeItem);
		itemInfo.hParentItem = GetParentItem(m_hSelTreeItem);
		if(itemInfo.hParentItem)
		{
			STTREEFOLDER_INFO* pInfo = (STTREEFOLDER_INFO*)GetItemData(itemInfo.hParentItem);
			if(pInfo && pItemInfo)
			{
				itemInfo.nSTLevel = pInfo->nSTLevel;
				itemInfo.nSTType	= pInfo->nSTType;

				itemInfo.szSTRData = GetItemText(itemInfo.hParentItem);
				itemInfo.szSTName = pItemInfo->szRawName ;		// GetItemText(m_hSelTreeItem);
				itemInfo.szSTPath = pInfo->szBaseFolder;		// 선택된 전략의 폴더경로
				itemInfo.nFolderIndx = 0;						// Tree형에서는 의미없음.
				itemInfo.nItemIndx	 = 0;						// Tree형에서는 의미없음.

				GetParent()->GetParent()->SendMessage(RMSG_STTREE_ITEMDBLCLK, (WPARAM)m_hWnd, (LPARAM)&itemInfo);
			}
		}
	}

	*pResult = 0;
}


void  CSTTreeCtrl:: DeleteInfo(STTREEBASE_INFO *pInfo)
{
	if(!pInfo) return;

	STTREEFOLDER_INFO	*pFInfo;
	STTREEITEM_INFO		*pIInfo;

	if(pInfo)
	{
		if(pInfo->nType==STTreeType_Folder)
		{
			pFInfo = (STTREEFOLDER_INFO*)pInfo;
			delete pFInfo;
			// SetItemData(hItem, NULL);
		}
		else
		{
			pIInfo = (STTREEITEM_INFO*)pInfo;
			delete pIInfo;
		}
	}
}

void CSTTreeCtrl::DeleteAllItemData(HTREEITEM hItem)
{
	// Delete all of the children of hmyItem.
	if(!hItem) return;

	STTREEBASE_INFO		*pInfo;

	HTREEITEM hCurItem = hItem;
	HTREEITEM hNextItem;
	while (hCurItem != NULL)
	{
		// TRACE("Tree[%s]\n", GetItemText(hCurItem));
		if (ItemHasChildren(hCurItem)) 
		{
			pInfo = (STTREEBASE_INFO*)GetItemData(hCurItem);
			DeleteInfo(pInfo);
			
			HTREEITEM hFolerTree = GetChildItem(hCurItem);
			DeleteAllItemData(hFolerTree);
		}
		else
		{
			pInfo = (STTREEBASE_INFO*)GetItemData(hCurItem);
			DeleteInfo(pInfo);

		}
		hNextItem = GetNextItem(hCurItem, TVGN_NEXT);
		hCurItem = hNextItem;
	}
}

// nType=0; 다 로딩.
//		 1: @.st또는 @2.st만 로딩.
//		 2: @0.st 또는 @1.st 인것만 로딩 -> 지표리스트
void CSTTreeCtrl::LoadSTFolderList(HTREEITEM hTreeItem, LPCSTR szDir, STTREEFOLDER_INFO* pFolder, int nType)
{
	CStringArray arr;
	CLoadSTFile::GetSTFileList(szDir, arr);

	STTREEITEM_INFO *pItemInfo = NULL;
	CString szText;
	HTREEITEM hItem;
	int nFind = -1;
	for(int i=0; i<arr.GetSize(); i++)
	{
		szText = arr.GetAt(i); 

		pItemInfo = new STTREEITEM_INFO;
		pItemInfo->nType = STTreeType_Item;
		pItemInfo->pFolder = pFolder;
		pItemInfo->szRawName = szText;

		nFind = szText.Find("@");
		if(nFind>0)
		{
			if(nType==1)
			{
				if(szText[nFind+1]=='0' || szText[nFind+1]=='1')
				{
					delete pItemInfo;
					continue;
				}
			}
			else if(nType==2)	// 결과적으로는 지표리스트.
			{
				if(szText[nFind+1]!='0' && szText[nFind+1]!='1')
				{
					delete pItemInfo;
					continue;
				}
			}
			szText = szText.Left(nFind);
		}
		else
		{
			if(nType==2)
			{
				delete pItemInfo;
				continue;
			}

			nFind = szText.Find(SZSTEXT);
			if(nFind>0)
			{
				szText = szText.Left(nFind);
			}
		}
		hItem = InsertItem(szText, hTreeItem);
		SetItemData(hItem, (DWORD)pItemInfo);
	}
}


// 'STB_' 로 시작하는 초급전략을 로딩한다.
// 삼성의 버전처리기가 같은 이름을 처리할 수 없기 때문에 궁여지책으로 만든 기능.
/*
void CSTTreeCtrl::LoadSTFolderListSTB(HTREEITEM hTreeItem, LPCSTR szDir, STTREEFOLDER_INFO* pFolder)
{
	CStringArray arr;
	CLoadSTFile::GetSTFileList(szDir, arr);

	STTREEITEM_INFO *pItemInfo = NULL;
	CString szText, szTmp;
	HTREEITEM hItem;
	int nFind = -1;
	for(int i=0; i<arr.GetSize(); i++)
	{
		szText = arr.GetAt(i); 

		pItemInfo = new STTREEITEM_INFO;
		pItemInfo->nType = STTreeType_Item;
		pItemInfo->pFolder = pFolder;
		pItemInfo->szRawName = szText;

		// 처음시작이 STB_ 인것만 처리한다.
		szTmp = szText; szTmp.MakeUpper();
		if(szText.GetLength()>4 && szText.Find("STB_")!=0) continue;

		nFind = szText.Find("@");
		if(nFind>0)
		{
			szText = szText.Left(nFind);
		}
		else
		{
			nFind = szText.Find(SZSTEXT);
			if(nFind>0)
			{
				szText = szText.Left(nFind);
			}
		}
		hItem = InsertItem(szText, hTreeItem);
		SetItemData(hItem, (DWORD)pItemInfo);
	}
}
*/

HTREEITEM CSTTreeCtrl::SearchFolderItem(LPCSTR szText)
{
	HTREEITEM hCurItem = GetRootItem();
	HTREEITEM hNextItem;
	CString szTitle;
	while (hCurItem != NULL)
	{
		szTitle = GetItemText(hCurItem);
		if(szTitle.Find(szText)>=0) return hCurItem;

		hNextItem = GetNextItem(hCurItem, TVGN_NEXT);
		hCurItem = hNextItem;
	}
	return NULL;
}

HTREEITEM CSTTreeCtrl::SearchItem(HTREEITEM hStart, LPCSTR szText)
{
	// Delete all of the children of hmyItem.
	if(!hStart) return NULL;

	HTREEITEM hCurItem = hStart;
	HTREEITEM hNextItem;
	CString szTitle;
	while (hCurItem != NULL)
	{
		if (ItemHasChildren(hCurItem)) 
		{
			hNextItem = SearchItem(GetChildItem(hCurItem), szText);
			if(hNextItem) return hNextItem;
		}
		else
		{
			szTitle = GetItemText(hCurItem);
			if(szTitle.CompareNoCase(szText)==0) return hCurItem;

		}
		hNextItem = GetNextItem(hCurItem, TVGN_NEXT);
		hCurItem = hNextItem;
	}
	return NULL;
}

BOOL CSTTreeCtrl::DeleteSTItem(LPCSTR szFolder, LPCSTR szItem)
{
	HTREEITEM hFolder = SearchFolderItem(szFolder);
	if(hFolder)
	{
		HTREEITEM hItem = SearchItem(hFolder, szItem);
		if(hItem) {
			STTREEBASE_INFO* pInfo = (STTREEBASE_INFO*)GetItemData(hItem);
			DeleteInfo(pInfo);
			DeleteItem(hItem);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CSTTreeCtrl::AddSTItem(LPCSTR szFolder, LPCSTR szItem)
{
	HTREEITEM hFolder = SearchFolderItem(szFolder);
	if(hFolder)
	{
		if(SearchItem(hFolder, szItem)==NULL) 
		{
			HTREEITEM hItem = InsertItem(szItem, hFolder);
			if(hItem)
			{
				STTREEITEM_INFO* pItemInfo = new STTREEITEM_INFO;
				pItemInfo->nType = STTreeType_Item;
				pItemInfo->pFolder = (STTREEFOLDER_INFO*)GetItemData(hFolder);
				pItemInfo->szRawName = szItem;
				if(m_nSTLevel==STLevel_Basic) pItemInfo->szRawName += SZSTBASICEXT0;//"@2";
				SetItemData(hItem, (DWORD)pItemInfo);
				return TRUE;
			}
		}
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CSTExtTree

CSTExtTree::CSTExtTree()
{
	m_nSTLevel = STLevel_Ext;
}

CSTExtTree::~CSTExtTree()
{
}


BEGIN_MESSAGE_MAP(CSTExtTree, CSTTreeCtrl)
	//{{AFX_MSG_MAP(CSTExtTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CSTExtTree::LoadSTTree()
{
	CString szDir;
	szDir.Format("%s%s", m_szRootDir , "\\Data\\STData\\ST");
	CStringArray arrDir;

	// 1. 제공되는 전략을 로딩한다.
	CLoadSTFile::GetSTFolderList(szDir, arrDir);

	HTREEITEM hTreeItem = NULL;
	STTREEFOLDER_INFO *pInfo = NULL;
	for(int i=0; i<arrDir.GetSize(); i++)
	{
		hTreeItem = InsertItem(arrDir.GetAt(i), NULL);

		pInfo = new STTREEFOLDER_INFO;
		pInfo->nType = STTreeType_Folder;
		pInfo->nSTLevel = STLevel_Ext;
		pInfo->nSTType  = STType_System;
		pInfo->szBaseFolder.Format("%s\\%s", szDir, arrDir.GetAt(i));

		SetItemData(hTreeItem, (DWORD)pInfo);

		LoadSTFolderList(hTreeItem, pInfo->szBaseFolder, pInfo, 1);
	}

	//2. 사용자단위전략을 로딩한다.
	szDir.Format("%s%s", m_szRootDir , "\\User\\STData\\ST\\사용자단위전략");
	hTreeItem = InsertItem("사용자단위전략", NULL);
	pInfo = new STTREEFOLDER_INFO;
	pInfo->nType = STTreeType_Folder;
	pInfo->nSTLevel = STLevel_Ext;
	pInfo->nSTType  = STType_User;
	pInfo->szBaseFolder = szDir;
	SetItemData(hTreeItem, (DWORD)pInfo);
	LoadSTFolderList(hTreeItem, szDir, pInfo, 1);

	//3. 사용자조합전략을 로딩한다.
	szDir.Format("%s%s", m_szRootDir , "\\User\\STData\\ST\\사용자조합전략");
	hTreeItem = InsertItem("사용자조합전략", NULL);
	m_hTreeUser = hTreeItem;		// 사용자조합전략의 핸들을 저장해둔다.
	pInfo = new STTREEFOLDER_INFO;
	pInfo->nType = STTreeType_Folder;
	pInfo->nSTLevel = STLevel_Ext;
	pInfo->nSTType  = STType_User;
	pInfo->szBaseFolder = szDir;
	SetItemData(hTreeItem, (DWORD)pInfo);
	LoadSTFolderList(hTreeItem, szDir, pInfo, 1);

	//4. 사용자지표를 로딩한다. 사용자 단위전략에 있는 파일중에서 로딩.
	szDir.Format("%s%s", m_szRootDir , "\\User\\STData\\ST\\사용자단위전략");
	hTreeItem = InsertItem(SZSTKEY_USERINDEXN, NULL);
	pInfo = new STTREEFOLDER_INFO;
	pInfo->nType = STTreeType_Folder;
	pInfo->nSTLevel = STLevel_Ext;
	pInfo->nSTType  = STType_User;
	pInfo->szBaseFolder = szDir;
	SetItemData(hTreeItem, (DWORD)pInfo);
	LoadSTFolderList(hTreeItem, szDir, pInfo, 2);
	if(ItemHasChildren(hTreeItem)==FALSE)
	{
		delete pInfo;
		DeleteItem(hTreeItem);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CSTExtTree

CSTBasicTree::CSTBasicTree()
{
	m_nSTLevel = STLevel_Basic;
}

CSTBasicTree::~CSTBasicTree()
{
}


BEGIN_MESSAGE_MAP(CSTBasicTree, CSTTreeCtrl)
	//{{AFX_MSG_MAP(CSTBasicTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
void CSTBasicTree::LoadSTTree()
{
	CString szDir;
	szDir.Format("%s%s", m_szRootDir , "\\Data\\STData\\STBasic");
	CLoadSTFile::LoadST(szDir, "*.*", TRUE, this, NULL);

	m_hTreeUser = InsertItem(SZUSERFOLDER, NULL);
	szDir.Format("%s%s%s\\%s", m_szRootDir , "\\User" , SZSTBASICDIR, SZUSERFOLDER);
	//szDir = m_szRootDir + "\\User" + SZSTBASICDIR + "\\" + SZUSERFOLDER;

	STTREEFOLDER_INFO* pFolder = new STTREEFOLDER_INFO;
	pFolder->nType = STTreeType_Folder;
	pFolder->nSTLevel = STLevel_Ext;
	pFolder->nSTType  = STType_User;
	pFolder->szBaseFolder = szDir;
	SetItemData(m_hTreeUser, (DWORD)pFolder);

//	CStringArray arr;
//	CLoadSTFile::GetSTFileList(szDir, arr);
	LoadSTFolderList(m_hTreeUser, szDir, pFolder);
}
*/

void CSTBasicTree::LoadSTTree()
{
	CString szDir;
	szDir.Format("%s%s", m_szRootDir , "\\Data\\STData\\STBasic");
	CStringArray arrDir;

	// 1. 제공되는 전략을 로딩한다.
	CLoadSTFile::GetSTFolderList(szDir, arrDir);

	HTREEITEM hTreeItem = NULL;
	STTREEFOLDER_INFO *pInfo = NULL;
	for(int i=0; i<arrDir.GetSize(); i++)
	{
		hTreeItem = InsertItem(arrDir.GetAt(i), NULL);

		pInfo = new STTREEFOLDER_INFO;
		pInfo->nType = STTreeType_Folder;
		pInfo->nSTLevel = STLevel_Basic;
		pInfo->nSTType  = STType_System;
		pInfo->szBaseFolder.Format("%s\\%s", szDir, arrDir.GetAt(i));

		SetItemData(hTreeItem, (DWORD)pInfo);

		LoadSTFolderList(hTreeItem, pInfo->szBaseFolder, pInfo);
	}


	//2. 사용자단위전략을 로딩한다.
	szDir.Format("%s%s%s", m_szRootDir , "\\User\\STData\\STBasic\\", SZUSERFOLDER);
	hTreeItem = InsertItem(SZUSERFOLDER, NULL);
	pInfo = new STTREEFOLDER_INFO;
	pInfo->nType = STTreeType_Folder;
	pInfo->nSTLevel = STLevel_Basic;
	pInfo->nSTType  = STType_User;
	pInfo->szBaseFolder = szDir;
	SetItemData(hTreeItem, (DWORD)pInfo);
	LoadSTFolderList(hTreeItem, szDir, pInfo);
}

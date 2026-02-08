// ExXmlTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ExXmlTreeCtrl.h"

#include "../resource.h"
#include <afx.h>	//#<=CStdioFile

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int m_nTreeIconIndex0St = 0;
const int m_nTreeIconIndex0St_File = 2;   //# Add : 2006.09
const int m_nTreeIconIndex1St = 4;
const int m_nTreeIconIndex2St = 6;
const int m_nTreeIconIndexNormal = 8;		//#<= Normal
const int m_nTreeIconIndexNonAlert = 9;	//#<= Alert
const int m_nTreeIconIndexPort = 10;			//# PortPolio

const int m_nTreeIconIndex_NormalFile = 11;				//#<= Normal File
const int m_nTreeIconIndex_NormalFile_Unit = 12;	//#<= Unit Normal File
const int m_nTreeIconIndex_AlertFile = 13;				//#<= Alert File
const int m_nTreeIconIndex_AlertFile_Unit = 14;		//#<= Alert Unit File

/////////////////////////////////////////////////////////////////////////////
// CExXmlTreeCtrl

CExXmlTreeCtrl::CExXmlTreeCtrl()
{
}

CExXmlTreeCtrl::~CExXmlTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CExXmlTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CExXmlTreeCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExXmlTreeCtrl message handlers

int CExXmlTreeCtrl::Create(CWnd* pParentWnd, UINT nID) 
{
	int nResult = -1;

	nResult = CTreeCtrl::Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | TVS_HASBUTTONS | 
												TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP,
												CRect(0, 0, 0, 0),
												pParentWnd,	
												nID);
	CTreeCtrl::ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	SetIconLoad();
	
	//SetItemAllView(FALSE);

	return nResult;
}

int CExXmlTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}


void CExXmlTreeCtrl::SetIconLoad()
{
	m_imglistTree.DeleteImageList();
	m_imglistTree.Create(16, 16, ILC_COLOR8 | ILC_MASK, 3, 3);

	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE00));	//# 0 st
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE01));
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE00_FILE));	//# 0 st - File
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE01_FILE));
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE10));	//# 1 st
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE11));	
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE10));	//# 2 st
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE11));	
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_NORMAL));		//# 일반
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_NONALERT));	//# Non Alert
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_NORMAL));		//# PortPolio
	
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE));					//# 일반 File
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_UNIT));			//# 조합 일반 File
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_ALERT));		//# Alert File
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_ALERTUNIT));	//# Alert 조합 File
		
	SetImageList(&m_imglistTree, TVSIL_NORMAL);
}

int CExXmlTreeCtrl::GetTreeIconIndex_File(BOOL bAlertReg, BOOL bUnitCan)
{
	int nResult = m_nTreeIconIndex_NormalFile;

	if (bAlertReg == TRUE)
	{
		if (bUnitCan == TRUE)
			nResult = m_nTreeIconIndex_AlertFile_Unit;
		else
			nResult = m_nTreeIconIndex_AlertFile;
	}
	else
	{
		if (bUnitCan == TRUE)
			nResult = m_nTreeIconIndex_NormalFile_Unit;
		else
			nResult = m_nTreeIconIndex_NormalFile;
	}

	return nResult;
}



void CExXmlTreeCtrl::SetControlState(BOOL bState)
{
	return;
	
	if (bState == TRUE)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
}

BOOL CExXmlTreeCtrl::SetInitTreeList(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	BOOL bResult = TRUE;
		
	XMLTreeInfoFile    stXMLTreeInfoFile;
	XMLTreeInfoFolder		stXMLTreeInfoFolder;
	POSITION posFolder, posFile;

	TRACE("\n\n [After Sync Folder File Info]");

	int nTreeIconIndex;

	HTREEITEM	htiSubRoot;
	m_htiRoot = GetRootItem();
	CString strFolder = _T("");
// 	try {
// 		int nIndex = 0;
// 		int nFolderCount = pFolderList->GetCount();
// 		for(int nFolderIndex = 0; nFolderIndex < nFolderCount; nFolderIndex++)
// 		{
// 			posFolder = pFolderList->FindIndex(nFolderIndex);
// 			if(!posFolder) continue;
// 
// 			stXMLTreeInfoFolder = pFolderList->GetAt(posFolder);
// 			
// 			htiSubRoot = SetTreeItem(m_htiRoot, stXMLTreeInfoFolder.strName, m_nTreeIconIndex0St, m_nTreeIconIndex0St + 1, -1);
// 			if(!htiSubRoot) continue;
// 
// 			int nFolderCount = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount();
// 			for (int nFileIndex = 0; nFileIndex < nFolderCount; nFileIndex++)
// 			{
// 				posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFileIndex);
// 				if(!posFile) continue;
// 
// 				stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);
// 
// 				nTreeIconIndex = GetTreeIconIndex_File((stXMLTreeInfoFile.strAlertReg == "TRUE")?TRUE:FALSE, 
//                                                 stXMLTreeInfoFile.bUnitCan);
// 				
// 				SetTreeItem(htiSubRoot, stXMLTreeInfoFile.strFileName, nTreeIconIndex, nTreeIconIndex, nIndex);
// 				nIndex++;
// 			}
// 		}
// 	} catch( CException e )
// 	{
// 		bResult = FALSE;
// 	}
	
	int nIndex = 0;
	int nFolderCount = pFolderList->GetCount();
	for(int nFolderIndex = 0; nFolderIndex < nFolderCount; nFolderIndex++)
	{
		posFolder = pFolderList->FindIndex(nFolderIndex);
		if(!posFolder) continue;

		stXMLTreeInfoFolder = pFolderList->GetAt(posFolder);

		htiSubRoot = SetTreeItem(m_htiRoot, stXMLTreeInfoFolder.strName, m_nTreeIconIndex0St, m_nTreeIconIndex0St + 1, -1);
		if(!htiSubRoot) continue;

		int nFolderCount = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount();
		for (int nFileIndex = 0; nFileIndex < nFolderCount; nFileIndex++)
		{
			posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFileIndex);
			if(!posFile) continue;

			stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);

			nTreeIconIndex = GetTreeIconIndex_File((stXMLTreeInfoFile.strAlertReg == "TRUE")?TRUE:FALSE, 
				stXMLTreeInfoFile.bUnitCan);

			SetTreeItem(htiSubRoot, stXMLTreeInfoFile.strFileName, nTreeIconIndex, nTreeIconIndex, nIndex);
			nIndex++;
		}
	}
	
	return bResult;
}


HTREEITEM CExXmlTreeCtrl::SetTreeItem(HTREEITEM htiParent, CString strItem, 
																		int nImgIndex, int nImgSelIndex, int lParam) 
{
	TV_INSERTSTRUCT tvstruct;
	
	tvstruct.hParent = htiParent;
	tvstruct.hInsertAfter = TVI_LAST;
		
	tvstruct.item.pszText = (LPTSTR)(LPCSTR)strItem;	
		
	tvstruct.item.lParam = (long)lParam;
	tvstruct.item.iImage = nImgIndex;
	tvstruct.item.iSelectedImage = nImgSelIndex;
	tvstruct.item.mask = TVIF_TEXT | TVIF_PARAM |TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			
	//HTREEITEM hItem = GetChildItem(htiParent);
	HTREEITEM hItem = InsertItem(&tvstruct);
	Expand(hItem, TVE_COLLAPSE);
	UpdateWindow();
		
	return hItem; 
}


BOOL CExXmlTreeCtrl::GetFindFile(CString strFolderPath, CString strFilePath,
                            CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	BOOL bResult = FALSE;
		
	XMLTreeInfoFile      stXMLTreeInfoFile;
	XMLTreeInfoFolder			stXMLTreeInfoFolder;

	strFilePath.MakeUpper();
	strFolderPath.MakeUpper();

	POSITION posFolder, posFile;
	CString strTargetFilePath, strTargetFolderPath;
	for (int nFolderIndex = 0; nFolderIndex < pFolderList->GetCount(); nFolderIndex++)
	{
		posFolder = pFolderList->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = pFolderList->GetAt(posFolder);
		
		strTargetFolderPath = stXMLTreeInfoFolder.strPath;
		strTargetFolderPath.MakeUpper();

		if (strFolderPath == strTargetFolderPath)
		{
			for (int nFileIndex = 0; nFileIndex < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nFileIndex++)
			{
				posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFileIndex);
				stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);

				strTargetFilePath = stXMLTreeInfoFile.strFileName;
				strTargetFilePath.MakeUpper();

				if (strFilePath == strTargetFilePath)
				{
					bResult = TRUE;
					break;
				}
			}
		}
	}

	return bResult;
}

BOOL CExXmlTreeCtrl::GetFindFolder(CString strFolderPath, 
                            CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	BOOL bResult = FALSE;
	
	XMLTreeInfoFile      stXMLTreeInfoFile;
	XMLTreeInfoFolder			stXMLTreeInfoFolder;

	POSITION pos;
	strFolderPath.MakeUpper();
	CString strTargetFolderPath;
	for (int nIndex = 0; nIndex < pFolderList->GetCount(); nIndex++)
	{
		pos = pFolderList->FindIndex(nIndex);
		stXMLTreeInfoFolder = pFolderList->GetAt(pos);
		
		strTargetFolderPath = stXMLTreeInfoFile.strFolderPath;
		strTargetFolderPath.MakeUpper();
		if (strFolderPath == strTargetFolderPath)
		{
			bResult = TRUE;
			break;
		}
	}
	
	m_plistXMLTreeInfoFolder = pFolderList;

	return bResult;
}



void CExXmlTreeCtrl::SetItemAllView(BOOL bAllItemView)
{
	HTREEITEM	htiRoot;
	htiRoot = GetRootItem();
	
	if (bAllItemView == m_bAllItemView)
		return;

	while (htiRoot != NULL)
	{
		if (bAllItemView == TRUE)
			Expand(htiRoot, TVE_EXPAND);
		else
			Expand(htiRoot, TVE_COLLAPSE);

		SetItemChildView(htiRoot, bAllItemView);
		htiRoot = GetNextSiblingItem(htiRoot);
	}

	m_bAllItemView = bAllItemView;
}

void CExXmlTreeCtrl::SetItemChildView(HTREEITEM hItem, BOOL bAllItemView)
{
	HTREEITEM hChild = GetChildItem(hItem);
	while (hChild != NULL)
	{
		if (bAllItemView == TRUE)
			Expand(hChild, TVE_EXPAND);
		else
			Expand(hChild, TVE_COLLAPSE);

		if (ItemHasChildren(hChild) == TRUE)
			SetItemChildView(hChild, bAllItemView);

		hChild = GetNextItem(hChild, TVGN_NEXT);
	}
}


void CExXmlTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

BOOL CExXmlTreeCtrl::SetDeleteItem(HTREEITEM hSelItem)
{
  BOOL bResult = FALSE;

  if (hSelItem != NULL)
  {
    DeleteItem(hSelItem);
    hSelItem = GetSelectedItem();
    if (hSelItem != NULL)
      GetSelectItemData(hSelItem);

    bResult = TRUE;
  }

  return bResult;
}

void CExXmlTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	HTREEITEM hSelItem = GetSelectedItem();
	GetSelectItemData(hSelItem);

	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CExXmlTreeCtrl::GetSelectItemData(HTREEITEM hSelItem)
{
  CStringArray saData;
	CString strData;

  strData = GetItemText(hSelItem);
	saData.Add(strData);

	HTREEITEM hParentItem = GetParentItem(hSelItem);
	while (hParentItem != NULL)
	{
		strData = GetItemText(hParentItem);
		saData.Add(strData);

		hParentItem = GetParentItem(hParentItem);
	}

	CWnd *pwndParent = GetParent();
	pwndParent->SendMessage(WM_XMLTREE_CTRL, NM_SELECT_FILE, (LPARAM)&saData);
}


BOOL CExXmlTreeCtrl::SetSelectItme(CString strFolderPath, CString strFileName)
{
	if (strFolderPath.IsEmpty() == TRUE)
		return FALSE;
	else
	{
		HTREEITEM	htiSelect;
		GetFindTreeItem(m_htiRoot, strFolderPath, htiSelect);

		Expand(m_htiRoot, TVE_EXPAND);
		Expand(htiSelect, TVE_EXPAND);

		if (strFileName.IsEmpty() == FALSE)
		{
			if (GetFindTreeItem(htiSelect, strFileName, htiSelect) == FALSE)
				return FALSE;
		}
		
		SelectItem(htiSelect);
		Invalidate();
	}

	return TRUE;
}

BOOL CExXmlTreeCtrl::GetFindTreeItem(HTREEITEM htItemNode, CString strFindText, 
								  HTREEITEM &hItem)
{
	BOOL bResult = FALSE;

	if (strFindText.IsEmpty())
		return 0;
	strFindText.MakeUpper();

	HTREEITEM htChildNode = GetChildItem(htItemNode);
	while (htChildNode != NULL)
	{
		CString	strItemText =	GetItemText(htChildNode);
		strItemText.MakeUpper();
		if (strItemText.Find((LPCTSTR)strFindText) != -1)
		{
			hItem = htChildNode;
			bResult = TRUE;
			break;
		}

		htChildNode = GetNextItem(htChildNode, TVGN_NEXT);
	}

	return bResult;
}
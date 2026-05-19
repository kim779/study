// fileTree.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "fileTree.h"
#include "h/mainvar.h"

// CFileTree

IMPLEMENT_DYNAMIC(CFileTree, CMFCShellTreeCtrl)

CFileTree::CFileTree()
{
	m_mapOnly = false;
	m_dwFlags = SHCONTF_FOLDERS|SHCONTF_NONFOLDERS;

}

CFileTree::~CFileTree()
{
}


BEGIN_MESSAGE_MAP(CFileTree, CMFCShellTreeCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CFileTree::OnNMDblclk)
END_MESSAGE_MAP()

HRESULT CFileTree::EnumObjects(HTREEITEM hParentItem, LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent)
{
	ASSERT_VALID(this);
//	ASSERT_VALID(afxShellManager);
	
	LPENUMIDLIST pEnum = NULL;
	HRESULT hr = pParentFolder->EnumObjects(NULL, m_dwFlags, &pEnum);
	if (FAILED(hr) || pEnum == NULL)
		return hr;
	
	LPITEMIDLIST pidlTemp;DWORD dwFetched = 1;
	// Enumerate the item's PIDLs:
	while (pEnum->Next(1, &pidlTemp, &dwFetched) == S_OK && dwFetched)
	{
		TVITEM	tvItem;

		ZeroMemory(&tvItem, sizeof(tvItem));
		// Fill in the TV_ITEM structure for this item:
		tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;
		// AddRef the parent folder so it's pointer stays valid:
		pParentFolder->AddRef();
		// Put the private information in the lParam:
		LPAFX_SHELLITEMINFO pItem = (LPAFX_SHELLITEMINFO)GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFO));
		ENSURE(pItem != NULL);


		pItem->pidlRel = pidlTemp;
		pItem->pidlFQ =((CWinAppEx *)AfxGetApp())->GetShellManager()->ConcatenateItem(pidlParent, pidlTemp);
		pItem->pParentFolder = pParentFolder;
		tvItem.lParam = (LPARAM)pItem;
		
		CString strItem = OnGetItemText(pItem);
		tvItem.pszText = strItem.GetBuffer(strItem.GetLength());
		tvItem.iImage = OnGetItemIcon(pItem, FALSE);
		tvItem.iSelectedImage = OnGetItemIcon(pItem, TRUE);

		// Determine if the item has children:
		DWORD dwAttribs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_DISPLAYATTRMASK | SFGAO_CANRENAME | SFGAO_FILESYSANCESTOR;
		pParentFolder->GetAttributesOf(1, (LPCITEMIDLIST*) &pidlTemp, &dwAttribs);
		
		tvItem.cChildren = (dwAttribs & (SFGAO_HASSUBFOLDER | SFGAO_FILESYSANCESTOR));
		// Determine if the item is shared:
		if (dwAttribs & SFGAO_SHARE)
		{
			tvItem.mask |= TVIF_STATE;
			tvItem.stateMask |= TVIS_OVERLAYMASK;
			tvItem.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image
		}

		if (dwAttribs & SFGAO_GHOSTED)
		{
			tvItem.mask |= LVIF_STATE;
			tvItem.stateMask |= LVIS_CUT;
			tvItem.state |= LVIS_CUT;
		}
		
		if (dwAttribs & SFGAO_FOLDER){}
		else
		{
			if (m_mapOnly)
			{
				CString	path = GetPath(pItem);
				if (path.IsEmpty() || !checkMapOnly(path))
					continue;
			}
		}
		
		// Fill in the TV_INSERTSTRUCT structure for this item
		
		TVINSERTSTRUCT tvInsert;
		tvInsert.item = tvItem;
		tvInsert.hInsertAfter = TVI_LAST;
		tvInsert.hParent = hParentItem;
		InsertItem(&tvInsert);
		dwFetched = 0;
	}


	pEnum->Release();
	return S_OK;
}

BOOL CFileTree::GetItemPathName(CString& strPath, HTREEITEM htreeItem)
{
	ASSERT_VALID(this);

	strPath = _T("");
	if (htreeItem == NULL)
		htreeItem = GetSelectedItem();
	if (htreeItem == NULL)
		return FALSE;

	LPAFX_SHELLITEMINFO pItem = (LPAFX_SHELLITEMINFO)GetItemData(htreeItem);
	if (pItem == NULL || pItem->pidlFQ == NULL || pItem->pidlRel == NULL)
		return FALSE;

	LPSHELLFOLDER lpShellFolder = NULL;
	HRESULT hRes;
	
	if (pItem->pParentFolder == NULL)
	{
		hRes = SHGetDesktopFolder(&lpShellFolder);
		if (FAILED(hRes))
			return FALSE;

		GetName(lpShellFolder, pItem->pidlRel, SHGDN_FORPARSING, strPath);
		lpShellFolder->Release();
	}
	else
		GetName(pItem->pParentFolder, pItem->pidlRel, SHGDN_FORPARSING, strPath);

	return TRUE;
}

CString CFileTree::GetPath(LPAFX_SHELLITEMINFO pItem)
{
	ASSERT_VALID(this);

	CString	strPath;

	if (pItem == NULL || pItem->pidlFQ == NULL || pItem->pidlRel == NULL)
		return _T("");

	LPSHELLFOLDER lpShellFolder = NULL;
	HRESULT hRes;
	
	strPath = _T("");
	if (pItem->pParentFolder == NULL)
	{
		hRes = SHGetDesktopFolder(&lpShellFolder);
		if (FAILED(hRes))
			return FALSE;

		GetName(lpShellFolder, pItem->pidlRel, SHGDN_FORPARSING, strPath);
		lpShellFolder->Release();
	}
	else
		GetName(pItem->pParentFolder, pItem->pidlRel, SHGDN_FORPARSING, strPath);

	return strPath;
}


BOOL CFileTree::GetName(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, DWORD dwFlags, CString& strName)
{
	BOOL	bSuccess=TRUE;
	STRRET	str;

	if (NOERROR==lpsf->GetDisplayNameOf(lpi, dwFlags, &str))
	{
		LPTSTR	lpStr;

		StrRetToStr(&str, lpi, &lpStr);
		strName = lpStr;
		CoTaskMemFree(lpStr);
	}
	else
		bSuccess = FALSE;

	return bSuccess;
}

void CFileTree::SetMapOnly()
{
	m_mapOnly = !m_mapOnly;
}

bool CFileTree::checkMapOnly(CString path)
{
	CFile	file;

	if (file.Open(path, CFile::modeRead))
	{
		UINT	nRead;
		char	wb[32];

		nRead = file.Read(wb, sizeof(wb)-1);
		if (nRead)
		{
			wb[nRead] = '\0';
			CString sData = CString(wb, nRead);
			if (sData.Find(_T("xml")) != -1)
			{
				file.Close();
				return true;
			}
		}
		file.Close();
	}
	return false;
}

void CFileTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CString	tmps;
	HTREEITEM hitem = GetSelectedItem();
	if (hitem)
	{
		if (GetItemPathName(tmps, hitem) && checkMapOnly(tmps))
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_FILEOPEN, (LPARAM)tmps.operator LPCSTR());
		else
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_TESTTRLAYOUT, (LPARAM)tmps.operator LPCSTR());
	}
	*pResult = 0;
}

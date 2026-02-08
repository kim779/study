#include "stdafx.h"
#include "Controls.h"
#include "ControlEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CAxTreeCtrl::CAxTreeCtrl()
{
	m_mapObject.RemoveAll();	
	m_font.CreatePointFont(9*10, "±¼¸²Ã¼");	
}

CAxTreeCtrl::~CAxTreeCtrl()
{
	m_mapObject.RemoveAll();	
	m_font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CAxTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CAxTreeCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

HTREEITEM CAxTreeCtrl::AddItem(HTREEITEM hParent, CString input, int nData, struct _graph* pGraph, HTREEITEM hAt)
{
	TV_INSERTSTRUCT		TreeCtrlItem;
	HTREEITEM		hItem = NULL;

	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.iImage = 1; 	//ÀÌ¹ÌÁöindex
	TreeCtrlItem.item.iSelectedImage = 1;
	TreeCtrlItem.hInsertAfter = hAt;	

	TreeCtrlItem.hParent = hParent;
	TreeCtrlItem.item.pszText = (LPSTR)(LPCTSTR)input;
	TreeCtrlItem.item.lParam = nData;
	TreeCtrlItem.item.cchTextMax = input.GetLength();
	hItem = this->InsertItem(&TreeCtrlItem);
	
	if (pGraph)
	{
		INDI_OBJ*	pObject = NULL;	

		pObject = new INDI_OBJ;
		pObject->SetData(nData, input, pGraph, false);
		m_mapObject.SetAt(hItem, pObject);
	}
	return hItem;
}

int CAxTreeCtrl::GetLevel(HTREEITEM hItem)
{
	if (hItem == NULL)
		hItem = GetSelectedItem();
	
	HTREEITEM	hRoot = GetRootItem();
	HTREEITEM	hParent = NULL;
	HTREEITEM	hChild = NULL;

	if (hItem == NULL)
		return -1;

	hParent = GetNextItem(hItem, TVGN_PARENT);

	if (hParent == NULL)
		return 0;

	return 1;
}

INDI_OBJ* CAxTreeCtrl::GetIndiObj(HTREEITEM hItem)
{
	INDI_OBJ*	pObject = NULL;

	if (hItem == NULL)
		hItem = GetSelectedItem();

	if (hItem == NULL)
		return pObject;

	m_mapObject.Lookup(hItem, pObject);
	return pObject;
}

DWORD CAxTreeCtrl::GetItemData(HTREEITEM hItem)
{
	if (hItem == NULL)
		hItem = GetSelectedItem();

	if (hItem == NULL)
		return 0;

	return CTreeCtrl::GetItemData(hItem);
}

BOOL CAxTreeCtrl::DeleteItem(HTREEITEM hItem)
{
	HTREEITEM	hChild = GetChildItem(hItem);
	
	while (hChild)
	{
		m_mapObject.RemoveKey(hChild);
		hChild = GetNextItem(hChild, TVGN_NEXT);	
	}

	m_mapObject.RemoveKey(hItem);
	return CTreeCtrl::DeleteItem(hItem);
}

BOOL CAxTreeCtrl::SetItemText(LPCTSTR lpszItem, HTREEITEM hItem)
{
	if (hItem == NULL)
		hItem = GetSelectedItem();

	if (hItem == NULL)
		return FALSE;

	return CTreeCtrl::SetItemText(hItem, lpszItem);
}

void CAxTreeCtrl::PreSubclassWindow() 
{
	SetFont(&m_font);
	CTreeCtrl::PreSubclassWindow();
}

/********************************************************************/
// CScrTreeCtrl class
/********************************************************************/
CScrTreeCtrl::CScrTreeCtrl()
{
	m_mapObject.RemoveAll();
	m_font.CreatePointFont(9*10, "±¼¸²Ã¼");
}

CScrTreeCtrl::~CScrTreeCtrl()
{
	m_mapObject.RemoveAll();	
	m_font.DeleteObject();

}

BEGIN_MESSAGE_MAP(CScrTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CScrTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CScrTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	this->ModifyStyle(TVS_DISABLEDRAGDROP, 0);
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CScrTreeCtrl::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM	hItem = pNMTreeView->itemNew.hItem;

	if (GetParentItem(hItem) == NULL)
	{
		if (pNMTreeView->action == 2)
		{
			SetItemImage(hItem, 1, 1);
		}
		else
		{
			SetItemImage(hItem, 0, 0);
		}
	}
	*pResult = 0;
}

HTREEITEM CScrTreeCtrl::AddItem(HTREEITEM hParent, CString input, int nData, struct _graph* pGraph, HTREEITEM hAt)
{
	TV_INSERTSTRUCT		TreeCtrlItem;
	HTREEITEM		hItem = NULL;

	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;	
	TreeCtrlItem.hInsertAfter = hAt;
	TreeCtrlItem.hParent = hParent;

	if (hParent == NULL)
	{
		TreeCtrlItem.item.iImage = 0;
		TreeCtrlItem.item.iSelectedImage = 0;
	}
	else
	{
		TreeCtrlItem.item.iImage = 2;
		TreeCtrlItem.item.iSelectedImage = 2;
	}

	TreeCtrlItem.item.pszText = (LPSTR)(LPCTSTR)input;
	TreeCtrlItem.item.lParam = nData;
	TreeCtrlItem.item.cchTextMax = input.GetLength();
	hItem = this->InsertItem(&TreeCtrlItem);
	
	if (pGraph)
	{
		INDI_OBJ*	pObject = NULL;	

		pObject = new INDI_OBJ;
		pObject->SetData(nData, input, pGraph, true);		
		m_mapObject.SetAt(hItem, pObject);
	}
	return hItem;
}

HTREEITEM CScrTreeCtrl::AddItem(HTREEITEM hParent, INDI_OBJ* pObject, HTREEITEM hAt)
{
	TV_INSERTSTRUCT		TreeCtrlItem;
	HTREEITEM		hItem = NULL;
	INDI_OBJ*		pNew = NULL;

	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TreeCtrlItem.hInsertAfter = hAt;
	TreeCtrlItem.hParent = hParent;

	if (hParent == NULL)
	{
		TreeCtrlItem.item.iImage = 0;
		TreeCtrlItem.item.iSelectedImage = 0;
	}
	else
	{
		TreeCtrlItem.item.iImage = 2;
		TreeCtrlItem.item.iSelectedImage = 2;
	}
	
	TreeCtrlItem.item.pszText = (LPSTR)(LPCTSTR)pObject->GetName();
	TreeCtrlItem.item.lParam = pObject->GetKind();
	TreeCtrlItem.item.cchTextMax = pObject->GetName().GetLength();
	hItem = this->InsertItem(&TreeCtrlItem);	
	pNew = new INDI_OBJ;
	pNew->SetData(pObject->GetKind(), pObject->GetName(), pObject->GetGraph(), true);	
	m_mapObject.SetAt(hItem, pNew);
	return hItem;
}

HTREEITEM CScrTreeCtrl::Move(HTREEITEM hFrom, HTREEITEM hTo)
{
	if (hFrom == NULL)
		return NULL;

	if (hTo == NULL)
	{
		DeleteItem(hFrom);
		return NULL;
	}

	HTREEITEM	hFromParent = GetParentItem(hFrom);
	HTREEITEM	hToParent = GetParentItem(hTo);
	INDI_OBJ*	pObject = NULL;
	HTREEITEM	hRet = NULL;
	

	if (hFromParent == NULL)
	{
		CIHArray<INDI_OBJ*, INDI_OBJ*>	arItem;
		INDI_OBJ*	pNew = NULL;
		HTREEITEM	hChild = GetChildItem(hFrom);
		HTREEITEM	hOld = NULL;
		int		ii = 0, nCount = 0;	

		while (hChild)
		{
			pObject = GetIndiObj(hChild);
			pNew = new INDI_OBJ;
			pNew->SetData(pObject->GetKind(), pObject->GetName(), pObject->GetGraph(), true);
			arItem.Add(pNew);
			hOld = hChild;
			hChild = GetNextItem(hChild, TVGN_NEXT);
			DeleteItem(hOld);
		}

		nCount = arItem.GetSize();

		if (hToParent == NULL)
		{
			hChild = TVI_LAST;
			hToParent = hTo;
		}
		else
		{
			hChild = hTo;
		}

		for ( ii = 0 ; ii < nCount ; ii++ )
		{
			pObject = arItem.GetAt(ii);
			AddItem(hToParent, pObject, hChild);
		}	
		
		hRet = hTo;
	}
	else	
	{
		pObject = GetIndiObj(hFrom);		

		if (hToParent == NULL)
			hRet = AddItem(hTo, pObject, TVI_FIRST);	
		else
			hRet = AddItem(hToParent, pObject, hTo);		

		DeleteItem(hFrom);
	}

	return hRet;
}

HTREEITEM CScrTreeCtrl::GetLastChild(HTREEITEM hItem)
{
	HTREEITEM	hChild = GetChildItem(hItem);
	HTREEITEM	hOld = NULL;

	while (hChild)
	{
		hOld = GetNextItem(hChild, TVGN_NEXT);

		if (hOld == NULL)
			break;

		hChild = hOld;
	}

	return hChild;
}

HTREEITEM CScrTreeCtrl::MoveUp(HTREEITEM hItem)
{
	HTREEITEM	hRet = NULL;
	HTREEITEM	hInsert = NULL;
	HTREEITEM	hParent = NULL;
	HTREEITEM	hChild = NULL;
	HTREEITEM	hRoot = GetRootItem();
	INDI_OBJ*	pObject = GetIndiObj(hItem);

	if (hItem == NULL)
		return NULL;
	
	hParent = GetParentItem(hItem);

	if (hParent == NULL)
	{
		hRet = GetNextItem(hItem, TVGN_PREVIOUS);

		if (hRet == NULL)
		{
			hRet = GetLastChild(NULL);
		}

		Swap(hRet, hItem);		
	}
	else
	{
		hInsert = GetNextItem(hItem, TVGN_PREVIOUS);

		if (hInsert)
		{
			hInsert = GetNextItem(hInsert, TVGN_PREVIOUS);
			
			if (hInsert)
				hRet = AddItem(hParent, pObject, hInsert);
			else
				hRet = AddItem(hParent, pObject, TVI_FIRST);
		}
		else
		{
			hInsert = GetNextItem(hParent, TVGN_PREVIOUS);

			if (hInsert)
			{				
				hRet = AddItem(hInsert, pObject);				
			}
			else
			{
				hInsert = GetLastChild(NULL);
				hRet = AddItem(hInsert, pObject);				
			}
		}

		DeleteItem(hItem);
	}

	return hRet;
}

HTREEITEM CScrTreeCtrl::MoveDown(HTREEITEM hItem)
{
	HTREEITEM	hRet = NULL;
	HTREEITEM	hInsert = NULL;
	HTREEITEM	hParent = NULL;
	HTREEITEM	hChild = NULL;
	HTREEITEM	hRoot = GetRootItem();
	INDI_OBJ*	pObject = GetIndiObj(hItem);

	if (hItem == NULL)
		return NULL;
	
	hParent = GetParentItem(hItem);

	if (hParent == NULL)
	{
		hRet = GetNextItem(hItem, TVGN_NEXT);

		if (hRet == NULL)
		{
			hRet = GetChildItem(NULL);
		}

		Swap(hRet, hItem);		
	}
	else
	{
		hInsert = GetNextItem(hItem, TVGN_NEXT);

		if (hInsert)
		{
			hRet = AddItem(hParent, pObject, hInsert);			
		}
		else
		{
			hInsert = GetNextItem(hParent, TVGN_NEXT);

			if (hInsert)
			{				
				hRet = AddItem(hInsert, pObject, TVI_FIRST);				
			}
			else
			{
				hInsert = GetChildItem(NULL);
				hRet = AddItem(hInsert, pObject, TVI_FIRST);
			}
		}

		DeleteItem(hItem);
	}

	return hRet;
}

INDI_OBJ* CScrTreeCtrl::GetIndiObj(HTREEITEM hItem)
{
	INDI_OBJ*	pObject = NULL;
	if (hItem == NULL)
		hItem = GetSelectedItem();

	if (hItem == NULL)
	{
		return pObject;
	}

	m_mapObject.Lookup(hItem, pObject);
	return pObject;
}

DWORD CScrTreeCtrl::GetItemData(HTREEITEM hItem)
{
	if (hItem == NULL)
		hItem = GetSelectedItem();

	if (hItem == NULL)
	{
		return 0;
	}

	return CTreeCtrl::GetItemData(hItem);
}

BOOL CScrTreeCtrl::DeleteItem(HTREEITEM hItem)
{
	if (hItem == NULL)
		return false;
	
	HTREEITEM	hChild = GetChildItem(hItem);
	
	while (hChild)
	{
		m_mapObject.RemoveKey(hChild);
		hChild = GetNextItem(hChild, TVGN_NEXT);	
	}

	m_mapObject.RemoveKey(hItem);
	return CTreeCtrl::DeleteItem(hItem);
}

void CScrTreeCtrl::Swap(HTREEITEM hItemA, HTREEITEM hItemB)
{
	if (hItemA == NULL || hItemB == NULL || hItemA == hItemB)
		return;
	CIHArray<INDI_OBJ*, INDI_OBJ*>	arItemA;
	CIHArray<INDI_OBJ*, INDI_OBJ*>	arItemB;
	INDI_OBJ*	pObject = NULL;
	INDI_OBJ*	pNew = NULL;
	HTREEITEM	hChild = GetChildItem(hItemA);
	HTREEITEM	hOld = NULL;
	int		nCount = 0, ii = 0;
	
	while (hChild)
	{
		m_mapObject.Lookup(hChild, pObject);
		pNew = new INDI_OBJ;
		pNew->SetData(pObject->GetKind(), pObject->GetName(), pObject->GetGraph(), true);
		arItemA.Add(pNew);
		hOld = hChild;		
		hChild = GetNextItem(hChild, TVGN_NEXT);
		DeleteItem(hOld);
	}

	hChild = GetChildItem(hItemB);
	
	while (hChild)
	{
		m_mapObject.Lookup(hChild, pObject);
		pNew = new INDI_OBJ;
		pNew->SetData(pObject->GetKind(), pObject->GetName(), pObject->GetGraph(), true);
		arItemB.Add(pNew);
		hOld = hChild;
		hChild = GetNextItem(hChild, TVGN_NEXT);
		DeleteItem(hOld);
	}

	nCount = arItemB.GetSize();

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		pObject = arItemB.GetAt(ii);
		AddItem(hItemA, pObject);
	}

	nCount = arItemA.GetSize();

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		pObject = arItemA.GetAt(ii);
		AddItem(hItemB, pObject);
	}
}

BOOL CScrTreeCtrl::SetItemText(LPCTSTR lpszItem, HTREEITEM hItem)
{
	if (hItem == NULL)
		hItem = GetSelectedItem();

	if (hItem == NULL)
		return FALSE;

	return CTreeCtrl::SetItemText(hItem, lpszItem);
}

int CScrTreeCtrl::GetChildCount(HTREEITEM hItem)
{
	int	nCount = 0;

	hItem = GetNextItem(hItem, TVGN_CHILD);

	if (hItem != NULL)
		nCount++;

	for ( ;; )
	{
		hItem = GetNextItem(hItem, TVGN_NEXT);

		if (hItem)
			nCount++;
		else
			break;
	}

	return nCount;
}

void CScrTreeCtrl::PreSubclassWindow() 
{
	SetFont(&m_font);	
	CTreeCtrl::PreSubclassWindow();
}

/********************************************************************/
// CCheckTree class
/********************************************************************/
CCheckTree::CCheckTree()
{
	clearData();
	m_font.CreatePointFont(9*10, "±¼¸²Ã¼");
}

CCheckTree::~CCheckTree()
{
	clearData();
	m_font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CCheckTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CCheckTree)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_CTRLMSG, OnControls)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCheckTree::OnDestroy() 
{
	CTreeCtrl::OnDestroy();
}

LRESULT CCheckTree::OnControls(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == (GetDlgCtrlID() + 1))
	{
		switch (HIWORD(wParam))
		{
		case CTRL_COMBOSELECT:
			break;
		case CTRL_COMBOCHECK:
			break;
		case CTRL_COMBOCLOSE:
		
			break;
		}
	}

	return 0;
}

DWORD CCheckTree::GetItemData(HTREEITEM hItem)
{
	if (hItem == NULL)
		hItem = GetSelectedItem();

	if (hItem == NULL)
	{
		return 0;
	}

	return CTreeCtrl::GetItemData(hItem);
}


HTREEITEM CCheckTree::GetPanelInfo(int nFlag)
{
	HTREEITEM	hItem = NULL;

	m_mapParent.Lookup(nFlag, hItem);
	return hItem;
}

HTREEITEM CCheckTree::GetItemInfo(int nParent, int nFlag)
{
	CTypedPtrMap<CMapWordToPtr, int, HTREEITEM>*	pObject = NULL;
	HTREEITEM	hItem = NULL;

	if (m_mapChild.Lookup(nParent, pObject))
	{
		pObject->Lookup(nFlag, hItem);
	}

	return	hItem;
}

void CCheckTree::clearData()
{
	CTypedPtrMap<CMapWordToPtr, int, HTREEITEM>*	pObject = NULL;
	int	nCount = m_mapChild.GetCount();

	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		pObject = m_mapChild.GetAt(ii);
		pObject->RemoveAll();
	}

	m_mapChild.RemoveAll();
	m_mapParent.RemoveAll();	
}

void CCheckTree::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	
	if (pTVKeyDown->wVKey == VK_SPACE)
	{
		HTREEITEM	hItem = GetSelectedItem();
		SetCheck(hItem, GetCheck(hItem));
	}

	*pResult = 0;
}

LRESULT CCheckTree::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDBLCLK)
	{		
		HTREEITEM	hItem = GetSelectedItem();
		CString	tmpstr;
		CString	tmpstr2;
		int	nPos = 0;
		bool	bShow = false;

		tmpstr = GetItemText(hItem);

		nPos = tmpstr.Find("(");

		if (tmpstr.Find("Show", 0) >= 0)
			bShow = true;
		else
			bShow = false;

		tmpstr = tmpstr.Left(nPos);

		if (bShow)
			tmpstr += "(Hide)";
		else
			tmpstr += "(Show)";

		SetItemText(tmpstr, hItem);
		return TRUE;
	}
	
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}

HTREEITEM CCheckTree::AddGroup(CString group, int nGroup, HTREEITEM hAt)
{
	TV_INSERTSTRUCT		TreeCtrlItem;
	HTREEITEM		hItem = NULL;
	
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM |TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.iImage = 0; 	//ÀÌ¹ÌÁöindex
	TreeCtrlItem.item.iSelectedImage = 1;
	TreeCtrlItem.hInsertAfter = hAt;	
	TreeCtrlItem.hParent = NULL;
	TreeCtrlItem.item.pszText = (LPSTR)(LPCTSTR)group;
	TreeCtrlItem.item.lParam = nGroup;
	TreeCtrlItem.item.cchTextMax = group.GetLength();
	hItem = this->InsertItem(&TreeCtrlItem);
	m_mapParent.SetAt(nGroup, hItem);
	return hItem;
}

HTREEITEM CCheckTree::AddGroupItem(CString group, CString name, int nKind, HTREEITEM hAt)
{
	TV_INSERTSTRUCT		TreeCtrlItem;
	HTREEITEM		hItem = NULL, hGroup = NULL;


	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM |TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.iImage = 0; 	//ÀÌ¹ÌÁöindex
	TreeCtrlItem.item.iSelectedImage = 1;
	TreeCtrlItem.hInsertAfter = hAt;

	hGroup = FindParent(group);
	
	if (hGroup == NULL && group.GetLength() != 0)
		return NULL;

	TreeCtrlItem.hParent = hGroup;
	TreeCtrlItem.item.pszText = (LPSTR)(LPCTSTR)name;
	TreeCtrlItem.item.lParam = nKind;
	TreeCtrlItem.item.cchTextMax = name.GetLength();
	hItem = this->InsertItem(&TreeCtrlItem);

	int	nGroup = GetItemData(hGroup);
	CTypedPtrMap<CMapWordToPtr, int, HTREEITEM>*	pObject = NULL;

	if (!m_mapChild.Lookup(nGroup, pObject))	
	{
		pObject = new CTypedPtrMap<CMapWordToPtr, int, HTREEITEM>;
		m_mapChild.SetAt(nGroup, pObject);		
	}

	pObject->SetAt(nKind, hItem);
	return hItem;
}


HTREEITEM CCheckTree::FindParent(CString parent)
{
	HTREEITEM	hRoot = NULL, hParent = NULL;
	CString		tmpstr;
	
	hRoot = this->GetRootItem();
	tmpstr = this->GetItemText(hRoot);

	if (tmpstr == parent)
		return hRoot;

	hParent = this->GetNextItem(hRoot, TVGN_NEXT);

	while (hParent)
	{
		tmpstr = this->GetItemText(hParent);
				
		if (tmpstr == parent)
			return hParent;
		
		hParent = this->GetNextItem(hParent, TVGN_NEXT);
	}
	return NULL;
}

BOOL CCheckTree::DeleteItem(HTREEITEM hItem)
{
	return CTreeCtrl::DeleteItem(hItem);
}

BOOL CCheckTree::SetItemText(LPCTSTR lpszItem, HTREEITEM hItem)
{
	if (hItem == NULL)
		hItem = GetSelectedItem();

	if (hItem == NULL)
		return FALSE;

	return CTreeCtrl::SetItemText(hItem, lpszItem);
}

void CCheckTree::PreSubclassWindow() 
{
	SetFont(&m_font);
	ModifyStyle(0, TVS_CHECKBOXES);
	CTreeCtrl::PreSubclassWindow();
}

/********************************************************************/
// CAxHeaderCtrl
/********************************************************************/

CAxHeaderCtrl::CAxHeaderCtrl()
{
}

CAxHeaderCtrl::~CAxHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CAxHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CAxHeaderCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT CAxHeaderCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK)
	{
		CWnd*	pWnd = GetParent();
		int	nCount = GetItemCount(), ii = 0;
		CRect	rect;
		CPoint	pt;
		int	nSub = -1;

		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		for ( ii = 0 ; ii < nCount ; ii++ )
		{
			GetItemRect(ii, rect);
			if (rect.PtInRect(pt))
			{
				nSub = ii;
				break;
			}
		}

		if (nSub >= 0)
			pWnd->SendMessage(WM_CTRLMSG, MAKEWPARAM(GetDlgCtrlID(), CTRL_HDRCLICK), (LPARAM)nSub);
	}
	return CHeaderCtrl::WindowProc(message, wParam, lParam);
}

/********************************************************************/
// CItemList class
/********************************************************************/

CItemList::CItemList()
{
	m_nMaxItem = -1;
	m_nColCnt = 0;	
	m_font.CreatePointFont(10 * 10, _T("±¼¸²Ã¼"));
	m_fontH.CreatePointFont(9 * 10, _T("±¼¸²Ã¼"));
}

CItemList::~CItemList()
{
	m_font.DeleteObject();
	m_fontH.DeleteObject();
}


BEGIN_MESSAGE_MAP(CItemList, CListCtrl)
	//{{AFX_MSG_MAP(CItemList)
	ON_MESSAGE(WM_CTRLMSG, OnControls)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CItemList::OnDestroy() 
{
	CListCtrl::OnDestroy();
	m_header.UnsubclassWindow();
}

LRESULT CItemList::OnControls(WPARAM wParam, LPARAM lParam)
{
	if (m_ctrlID == LOWORD(wParam))
	{
		switch (HIWORD(wParam))
		{
		case CTRL_HDRCLICK:
			{				
				int	nSubItem = (int)lParam;
				int	nCount = GetItemCount();
				int	nData = 0;

				if (nSubItem <= 0 || nCount <= 0)
					return 0;

				if (GetData(0, nSubItem))
					nData = 0;
				else
					nData = 1;
				
				for ( int ii = 0 ; ii < nCount ; ii++ )
				{
					SetData(ii, nSubItem, nData);
				}
			}
			break;
		}
	}

	return 0;
}

void CItemList::PreSubclassWindow() 
{	
	DWORD	style = LVS_OWNERDRAWFIXED;
	
	this->SetExtendedStyle(style);
	ModifyStyle(0, LVS_OWNERDRAWFIXED);	
	this->SetFont(&m_font);
	GetHeaderCtrl()->SetFont(&m_fontH);
	m_header.SubclassWindow(GetHeaderCtrl()->m_hWnd);
	m_ctrlID = GetDlgCtrlID() + 1;
	m_header.SetDlgCtrlID(m_ctrlID);
	CListCtrl::PreSubclassWindow();
}

int CItemList::GetSubItem(CPoint pt)
{
	int	nWidth = 0;

	for ( int ii = 0 ; ii < m_nColCnt ; ii++ )
	{
		nWidth += GetColumnWidth(ii);
		
		if (pt.x < nWidth)
			return ii;
	}

	return -1;
}

void CItemList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC	dc;

	dc.Attach(lpDrawItemStruct->hDC);
	int	nSaveDC = dc.SaveDC();
	int	nItem = lpDrawItemStruct->itemID;
	int	iState = lpDrawItemStruct->itemState;
	int	iAction = lpDrawItemStruct->itemAction;	
	
	COLORREF	crSelected = RGB(0, 0, 0);
	COLORREF crText = GetSysColor(COLOR_WINDOWTEXT);
	COLORREF crNormal = GetSysColor(COLOR_WINDOW);
	
	TCHAR	buf[256];
	CString	tmpstr;
	int	nData = 0;

	LV_ITEM	lvitem;

	lvitem.pszText = buf;
	lvitem.mask = LVIF_TEXT | LVIF_PARAM;
	lvitem.iItem = lpDrawItemStruct->itemID;
	lvitem.iSubItem = 0;
	lvitem.cchTextMax = 255;
	GetItem(&lvitem);

	CRect	rect[3];	
	CPen	pen, *oldpen = NULL;
	
	pen.CreatePen(PS_SOLID, 1, RGB(150, 150, 150));

	oldpen = dc.SelectObject(&pen);
	
	for ( int ii = 0 ; ii < 3 ; ii++ )
	{
		this->GetSubItemRect(nItem, ii, LVIR_BOUNDS, rect[ii]);	
	}
	
	dc.SelectObject(oldpen);
	pen.DeleteObject();
	dc.SetBkMode(TRANSPARENT);
	crSelected = GetSysColor(COLOR_HIGHLIGHT);

	if (GetFocus() == this && iState & ODS_SELECTED)				// If Selected
	{							// Set Selected Attributes
		dc.SetTextColor((0x00FFFFFF & ~(crText)));	// Set Inverted Text Color (With Mask)
		dc.FillSolidRect(&rect[0], crSelected);		// Erase Item
	}
	else							// If Not Selected
	{							// Set Standard Attributes
		dc.SetTextColor(crText);		// Set Inverted Text Color (With Mask)
		dc.FillSolidRect(&rect[0], crNormal);		// Erase Item
	}
	
	if (GetFocus() == this && iState & ODS_FOCUS)					// If Item Has The Focus
		dc.DrawFocusRect(&rect[0]);			// Draw Focus Rect
	
	tmpstr.Format(_T("%s"), buf);
	dc.DrawText(tmpstr, rect[0], DT_SINGLELINE|DT_LEFT|DT_VCENTER);	
	
	for ( int ii = 0 ; ii < 3 ; ii++ )
	{
		lvitem.iSubItem = ii;
		GetItem(&lvitem);
	}
	
	for ( int ii = 1 ; ii < m_nColCnt ; ii++ )
	{
		nData = GetData(nItem, ii);
		dc.DrawFrameControl(rect[ii], DFC_BUTTON, (nData) ? DFCS_CHECKED : DFCS_BUTTONCHECK);
	}
	
	dc.RestoreDC(nSaveDC);
	dc.Detach();
}

int CItemList::GetData(int nItem, int nSubItem)
{
	DWORD	dword = GetItemData(nItem);
	WORD	nOpt = HIWORD(dword);
	BYTE	nFlag = 0x01;
	int	nRet = 0;

	if (!nSubItem)
		nRet = 0x0000FFFF & dword;
	else
	{
		if (nSubItem >= 17)
			return -1;

		nFlag = nFlag << (nSubItem - 1);
		nRet = nFlag & nOpt;
	}

	return nRet;
}

void CItemList::SetData(int nItem, int nSubItem, int nData)
{
	DWORD	dword = GetItemData(nItem);
	WORD	nOpt = HIWORD(dword);
	BYTE	nFlag = 0x01;

	if (!nSubItem)
	{
		 dword = (dword & 0xFFFF0000) |(0x0000FFFF & nData);	 
	}
	else
	{
		if (nSubItem >= 17)
			return;

		nFlag = nFlag << (nSubItem - 1);

		if (!nData)
			nOpt &= ~nFlag;
		else
			nOpt |= nFlag;

		dword = (DWORD)(MAKEWPARAM(LOWORD(dword), nOpt));
	}

	SetItemData(nItem, dword);
}



// return code
// 0 : normal end
// -1 : input is empty
// -2 : content duplicated
// -3 : list is overflowed

int CItemList::AddItem(CString input, int nValue, int nRow, int nCol)
{
	LV_ITEM		lvi;
	int		nIndex = 0;
	
	lvi.mask = LVIF_TEXT | LVIF_PARAM;

	if (input.IsEmpty())	
		return -1;		// input is empty

	if (IsExist(input, nCol))
		return -2;		// duplicate

	int	nCnt = this->GetItemCount();

	if (m_nMaxItem >= 0)
	{
		if (nCnt == m_nMaxItem)
			return -3;
	}
	
	lvi.iSubItem = nCol;

	if (nRow < 0)
		nIndex = lvi.iItem = this->GetItemCount();
	else
		nIndex = lvi.iItem = nRow;
	
	lvi.pszText = (LPSTR)(LPCTSTR)input;
	lvi.lParam = 0x0000FFFF & nValue;
	this->InsertItem(&lvi); 
	return 0;
}

BOOL CItemList::IsExist(CString input, int nCol)
{
	int	ii = 0;
	CString	tmpstr;

	for ( ii = 0 ; ii < this->GetItemCount() ; ii++ )
	{
		tmpstr = this->GetItemText(ii, nCol);

		if (tmpstr == input)
			return TRUE;
	}

	return FALSE;
}

int CItemList::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
{
	++m_nColCnt;
	return CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);	
}

int CItemList::InsertColumn(int nCol, const LVCOLUMN *pColumn)
{
	++m_nColCnt;
	return CListCtrl::InsertColumn(nCol, pColumn);
}

// -1 : selection clear
// nRow > 0 : Select
void CItemList::SetSelection(int nRow)
{	
	if (nRow < 0)
	{
		SetItemState(-1, 0, LVIS_SELECTED|LVIS_FOCUSED);
		return;
	}

	SetItemState(-1, 0, LVIS_SELECTED|LVIS_FOCUSED);
	SetSelectionMark(nRow);
	SetItemState(nRow, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
}

int CItemList::FindRow(CString value, int nCol)
{
	int	ii = 0;
	CString	tmpstr;

	for ( ii = 0 ; ii < this->GetItemCount() ; ii++ )
	{
		tmpstr = this->GetItemText(ii, nCol);

		if (tmpstr == value)
			return ii;
	}

	return -1;
}
	
int CItemList::GetSelection()
{
	int	nCnt = GetSelectedCount();

	if (nCnt != 1)
		return -1;
	
	return GetSelectionMark();
}

LRESULT CItemList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{	
	if (message == WM_KEYUP)
	{
		CWnd*	pWnd = GetParent();

		if (
			wParam == VK_UP		|| 
			wParam == VK_DOWN	|| 
			wParam == VK_NEXT	|| 
			wParam == VK_PRIOR	||
			wParam == VK_HOME	||
			wParam == VK_END
			)
		{
			if (pWnd)
			{
				UINT	ctrlID = this->GetDlgCtrlID();
				NMHDR	nmhdr;
				nmhdr.hwndFrom = this->m_hWnd;
				nmhdr.idFrom = ctrlID;
				nmhdr.code = NM_CLICK;
				NM_LISTVIEW	nmlistview;

				memset(&nmlistview, 0x00, sizeof(nmlistview));
				nmlistview.hdr = nmhdr;
				nmlistview.iItem = this->GetSelection();
			
				pWnd->SendMessage(WM_NOTIFY, (WPARAM)this->GetDlgCtrlID(), (LPARAM)&nmlistview);
				return TRUE;
			}
		}
	}
	else if (	
			message == WM_LBUTTONDOWN	|| 
			message == WM_LBUTTONDBLCLK	|| 
			message == WM_RBUTTONDOWN	||
			message == WM_RBUTTONDBLCLK	||
			message == WM_LBUTTONUP
		)
	{
		CRect	rect;		
		int	nCount = this->GetItemCount();
		CPoint	pt;
		int	nItem = 0;		
		int	nSubItem = 0; 		

		pt.y = HIWORD(lParam);
		pt.x = LOWORD(lParam);
		nItem = this->HitTest(pt, NULL);
		nSubItem = GetSubItem(pt);
		
		if (nItem < 0 || nSubItem < 0)
			return TRUE;

		if (message == WM_LBUTTONDOWN)
		{
			if (nSubItem > 0)
				SetData(nItem, nSubItem, !GetData(nItem, nSubItem));				
		} 
		else if (message == WM_LBUTTONDBLCLK)
		{			
			if (nSubItem > 0)
				SetData(nItem, nSubItem, !GetData(nItem, nSubItem));				
			else
				GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(GetDlgCtrlID(), CTRL_LISTDBCLICK), MAKELPARAM(nItem, nSubItem));
		}		
	}
	
	return CListCtrl::WindowProc(message, wParam, lParam);
}

/********************************************************************/
// CLineListBox class
/********************************************************************/

CLineListBox::CLineListBox()
{
	m_bCheck = false;
}

CLineListBox::~CLineListBox()
{
	
}

BEGIN_MESSAGE_MAP(CLineListBox, CListBox)
	//{{AFX_MSG_MAP(CLineListBox)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CLineListBox::GetIndex(int nData)
{
	DWORD	dword = 0;
	int	ii = 0, nCount = GetCount();

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		dword = GetItemData(ii);
		dword = getColorBit(dword);

		if (nData == (int)dword)
			return ii;
	}

	return -1;
}

void CLineListBox::OnSelChange()
{
	bool	bCheck = false;
	if (m_bCheck)
	{
		int	nCurSel = GetCurSel();
		POINT	pt;
		::GetCursorPos(&pt);
		this->ScreenToClient(&pt);
		CRect	rect;
		int	nHeight = 0;
		this->GetItemRect(nCurSel, rect);
		nHeight = rect.Height();

		if (pt.x < (rect.left + nHeight))
		{
			SetCheck(!this->GetCheck(nCurSel), nCurSel);			
		}
		bCheck = true;
	}

	GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(this->GetDlgCtrlID(), CTRL_LISTSELECT), (LPARAM)bCheck);
}

void CLineListBox::DrawItem(LPDRAWITEMSTRUCT pDIStruct) 
{
	if (GetCount() <= 0)
		return;
	
	CDC	dc;
	CDC	memDC;
	CRect	rcTotal(pDIStruct->rcItem);
	CRect	rect;
	GetWindowRect(rect);
	rcTotal.right = rcTotal.left + rect.Width();
	
	CRect	rcColor;
	CRect	rcText;

	rcColor = rcText = rcTotal;

	CBrush	brFrameBrush;
	int	iFourthWidth = 0;
	int	iItem = pDIStruct->itemID;
	int	iAction = pDIStruct->itemAction;
	int	iState = pDIStruct->itemState;
	DWORD	dword = GetItemData(iItem);
	int	nColor = getColorBit(dword);
	int	nCheck = getCheckBit(dword);	
	int	nUse = getUseBit(dword);

	COLORREF crColor = 0;
	COLORREF crNormal = GetSysColor(COLOR_WINDOW);
	COLORREF crSelected;

	if (GetFocus() == this)
		crSelected = GetSysColor(COLOR_HIGHLIGHT);
	else
		crSelected = GetSysColor(COLOR_INACTIVECAPTIONTEXT);
	
	COLORREF crText = GetSysColor(COLOR_WINDOWTEXT);

	if (!dc.Attach(pDIStruct->hDC))			// Attach CDC Object
		return;						// Stop If Attach Failed
	

	if (nUse)
	{
		iFourthWidth = (rcTotal.Width() / 4);		// Get 1/4 Of Item Area
	}

	int	nHeight = 0;
	if (m_bCheck)
		nHeight = drawCheck(&dc, rcTotal, nCheck);

	rcColor.left += nHeight + 3;

	if (nHeight == 0)
		rcColor.left -= 3;
	rcColor.right = rcColor.left + iFourthWidth;

	if (nColor >= 0 && (int)dword >= 0)
		rcText.left = rcColor.right;
	else
		rcText.left += nHeight + 3;

	brFrameBrush.CreateStockObject(BLACK_BRUSH);		// Create Black Brush

	if (iState & ODS_SELECTED)				// If Selected
	{							// Set Selected Attributes
		if (GetFocus() == this)
			dc.SetTextColor((0x00FFFFFF & ~(crText)));	// Set Inverted Text Color (With Mask)
		dc.SetBkColor(crSelected);			// Set BG To Highlight Color
		dc.FillSolidRect(&rcText, crSelected);	// Erase Item
	}
	else							// If Not Selected
	{							// Set Standard Attributes
		dc.SetTextColor(crText);			// Set Text Color
		dc.SetBkColor(crNormal);			// Set BG Color
		dc.FillSolidRect(&rcText, crNormal);	// Erase Item
	}
	if (iState & ODS_FOCUS)					// If Item Has The Focus
		dc.DrawFocusRect(&rcText);			// Draw Focus Rect

			
	//
	// Draw Color Text And Block
	//
	if (iItem != -1)					// If Not An Empty Item
	{
		GetText(iItem, m_szColor);			// Get Color Text

		if (iState & ODS_DISABLED)			// If Disabled
		{
			crColor = GetSysColor(COLOR_INACTIVECAPTIONTEXT);
			dc.SetTextColor(crColor);	// Set Text Color
		}
		else	
		{// If Normal			
			crColor = (COLORREF)nColor;		// Get Color Value
		}

		dc.SetBkMode(TRANSPARENT);		// Transparent Background
		rcText.DeflateRect(2, 0);
		dc.DrawText(m_szColor, rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);		

		if (nUse)
		{
			rcColor.DeflateRect(2, 2);
			dc.FillSolidRect(&rcColor, crColor);	// Draw Color      
			dc.FrameRect(&rcColor, &brFrameBrush);// Draw Frame
		}
	}

	brFrameBrush.DeleteObject();
	dc.Detach();					// Detach DC From Object
}

BOOL CLineListBox::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	if (m_bCheck)
	{		
		if (pMsg->message == WM_KEYDOWN)
		{				
			if (pMsg->wParam == VK_SPACE)
			{
				int	nIndex = GetCurSel();
				SetCheck(!GetCheck(nIndex), nIndex);
				GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(GetDlgCtrlID(), CTRL_LISTSELECT), (LPARAM)m_hWnd);
			}			
		}
	}
	return CListBox::PreTranslateMessage(pMsg);
}

void CLineListBox::PreSubclassWindow() 
{	
	Initialize();						// Initialize Contents
	CListBox::PreSubclassWindow();				// Subclass Control
	return;							// Done!
}

int CLineListBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	Initialize();						// Initialize Contents

	return 0;
}

void CLineListBox::Initialize(void)
{
	ResetContent();
}

COLORREF CLineListBox::GetSelectedColorValue()
{
	int	iSelectedItem = GetCurSel();              	// Get Selected Item

	if (iSelectedItem == CB_ERR)                   		// If Nothing Selected
		return(RGB(0, 0, 0));                    	// Return Black

	DWORD	dword = GetItemData(iSelectedItem);

	return(getColorBit(dword));               	// Return Selected Color
}


CString CLineListBox::GetSelectedColorName()
{
	int	iSelectedItem = GetCurSel();           	// Get Selected Item

	if (iSelectedItem == CB_ERR)
	{
		m_sColorName = _T("");
		return (m_sColorName);
	}

	GetText(iSelectedItem, m_sColorName);          		// Store Name Of Color
	return(m_sColorName);                         		// Return Selected Color Name
}


void CLineListBox::SetSelectedColorValue(COLORREF color)
{
	int	iItems = GetCount();
	DWORD	dword = 0;

	for ( int ii = 0 ; ii < iItems ; ii++ )
	{
		dword = GetItemData(ii);

		if (color == getColorBit(dword))         		// If Match Found
			SetCurSel(ii);                      	// Select It
	}

	return;                                      		// Done!
}

void CLineListBox::SetSelectedColorName(CString szColor)
{
	int	iItems = GetCount();
	CString	sColorName;

	for ( int ii = 0 ; ii < iItems ; ii++ )
	{
		GetText(ii, sColorName);                 	// Get Color Name

		if (!sColorName.CompareNoCase(szColor))   	// If Match Found
			SetCurSel(ii);                       	// Select It
	}
	return;                                      		// Done!
}


bool CLineListBox::RemoveColor(CString szColor)
{
	int	iItems = GetCount();
	CString	sColor;
	bool	bRemoved = false;

	for ( int ii = 0 ; ii < iItems ; ii++ )
	{
		GetText(ii, sColor);                     	// Get Color Name
		
		if (!sColor.CompareNoCase(szColor))          	// If Match Found
		{
			DeleteString(ii);                   	// Remove It
			bRemoved = true;                       	// Set Flag
			break;                                 	// Stop Checking
		}
	}
	return(bRemoved);                             		// Done!
}

bool CLineListBox::RemoveColor(COLORREF color)
{
	int	iItems = GetCount();
	bool	bRemoved = false;

	for ( int ii = 0; ii < iItems; ii++ )
	{
		if (color == GetItemData(ii))       		// If Desired Color Found
		{
			DeleteString(ii);                 	// Remove It
			bRemoved = true;                       	// Set Flag
			break;                                 	// Stop Checking
		}
	}

	return(bRemoved);                             		// Done!
}

int CLineListBox::AddColor(CString szColor, int color, bool bUseColor)
{
	int      iItem = -1;

	iItem = InsertString(-1, szColor);             		// Insert String
	DWORD	dword = 0;
	dword = setColorBit(dword, color);
	dword = setUseBit(dword, bUseColor);
	
	if (iItem != LB_ERR)                           		// If Insert Good
		SetItemData(iItem, dword);
	int	nCount = this->GetCount();
	return(iItem);                             		// Done! Return Location
}

void CLineListBox::SetColor(COLORREF color, int nIndex)
{
	if (nIndex < 0)
		nIndex = GetCurSel();

	if (nIndex < 0)
		return;

	DWORD	dword = GetItemData(nIndex);
	
	dword = setColorBit(dword, color);	
	SetItemData(nIndex, dword);

	Invalidate(FALSE);
}

COLORREF CLineListBox::GetColor(int nIndex)
{
	COLORREF	color = RGB(0, 0, 0);
	
	if (nIndex < 0)
	{
		nIndex = GetCurSel();
	}
	
	if (nIndex >= 0)
	{
		DWORD	dword = GetItemData(nIndex);
		color = getColorBit(dword);		
	}

	return color;
}

void CLineListBox::SetCheck(bool bCheck, int nIndex)
{
	if (!m_bCheck)
		return;
	
	if (nIndex < 0)
		nIndex = GetCurSel();

	if (nIndex < 0)
		return;

	DWORD	dword = GetItemData(nIndex);
	dword = setCheckBit(dword, bCheck);
	SetItemData(nIndex, dword);	
	Invalidate(FALSE);
}

bool CLineListBox::GetCheck(int nIndex)
{
	if (!m_bCheck)
		return FALSE;

	if (nIndex < 0)
	{
		nIndex = GetCurSel();
	}
	
	if (nIndex >= 0)
	{
		DWORD dword = GetItemData(nIndex);
		dword = getCheckBit(dword);

		if (dword)
			return true;
	}

	return false;
}

void CLineListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{

}

int CLineListBox::drawCheck(CDC* pDC, CRect rect, int Check)
{
	CRect	rcCheck;
	int	nHeight = 0;
	
	rect.DeflateRect(1, 1);
	rcCheck = rect;
	nHeight = rect.Height();
	rcCheck.right = rcCheck.left + nHeight;
	pDC->DrawFrameControl(rcCheck, DFC_BUTTON, (Check) ? DFCS_CHECKED : DFCS_BUTTONCHECK);
	return nHeight;
	
}

void CLineListBox::SetItemText(int nIndex, CString input)
{
	COLORREF	color = this->GetColor(nIndex);
	DWORD		dword = this->GetItemData(nIndex);
	this->InsertString(nIndex, input);
	this->SetItemData(nIndex, dword);
	DeleteString(nIndex + 1);
	this->SetCurSel(nIndex);
}

/********************************************************************/
// CListCtrlSort class
/********************************************************************/

CListCtrlSort::CListCtrlSort(CListCtrl *pList, const int nCol)
{
	m_pList = pList;
	ASSERT(m_pList);
	
	int totItems = m_pList->GetItemCount();

	for (int ii = 0; ii < totItems; ii++)
	{
		DWORD dw = m_pList->GetItemData(ii);
		CString txt = m_pList->GetItemText(ii, nCol);
		m_pList->SetItemData(ii, (DWORD) new CSortItem(dw, txt));
	}
}

CListCtrlSort::~CListCtrlSort()
{
	ASSERT(m_pList);
	int totItems = m_pList->GetItemCount();
	for (int ii = 0; ii < totItems; ii++)
	{
		CSortItem *pItem = (CSortItem*)(m_pList->GetItemData(ii));
		ASSERT(pItem);
		m_pList->SetItemData(ii, pItem->m_dw);
		delete pItem;
	}
}

void CListCtrlSort::Sort(BOOL bAsc, SortDataType dtType)
{
	long lParamSort = bAsc ? dtType : -dtType;
	m_pList->SortItems(Compare, lParamSort);
}

int CALLBACK CListCtrlSort::Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortItem *pItem1 = (CSortItem*)lParam1;
	CSortItem *pItem2 = (CSortItem*)lParam2;
	ASSERT(pItem1 && pItem2);

	int nOrder = (lParamSort < 0) ? -1 : 1;
	SortDataType dtType = (SortDataType) (lParamSort * nOrder); 

	switch (dtType)
	{
	case SortDataType::dtINT:
		return (atol(pItem1->m_txt) - atol(pItem2->m_txt)) * nOrder;
	case SortDataType::dtSTRING:
		return pItem1->m_txt.Compare(pItem2->m_txt) * nOrder;
	default:
		ASSERT("Error: attempt to sort a column without type.");
		return 0;
	}
}

/********************************************************************/
// CAxListCtrl class
/********************************************************************/
#ifndef ListView_SetCheckState
   #define ListView_SetCheckState(hwndLV, i, fCheck) \
      ListView_SetItemState(hwndLV, i, \
      INDEXTOSTATEIMAGEMASK((fCheck)+1), LVIS_STATEIMAGEMASK)
#endif

CAxListCtrl::CAxListCtrl()
{
	m_bSort = TRUE;
	m_nMaxItem = -1;
	m_nColCnt = 0;
	m_bCheckBox = FALSE;
}

CAxListCtrl::~CAxListCtrl()
{
}


BEGIN_MESSAGE_MAP(CAxListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CAxListCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CAxListCtrl::PreSubclassWindow() 
{	
	DWORD	style = LVS_EX_FULLROWSELECT | LVS_OWNERDRAWFIXED;
	
	this->SetExtendedStyle(style);
	CListCtrl::PreSubclassWindow();
}

void CAxListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
}

// return code
// 0 : normal end
// -1 : input is empty
// -2 : content duplicated
// -3 : list is overflowed

int CAxListCtrl::AddItem(CString input, int nRow, int nCol, LPARAM param)
{
	LV_ITEM		lvi;
	int		nIndex = 0;
	
	lvi.mask = LVIF_TEXT|LVIF_PARAM;

	if (input.IsEmpty())	
		return -1;		// input is empty

	if (IsExist(input, nCol))
		return -2;		// duplicate

	int	nCnt = this->GetItemCount();

	if (m_nMaxItem >= 0)
	{
		if (nCnt == m_nMaxItem)
			return -3;
	}
	
	lvi.iSubItem = nCol;

	if (nRow < 0)
		nIndex = lvi.iItem = this->GetItemCount();
	else
		nIndex = lvi.iItem = nRow;

	lvi.lParam = param;
	
	lvi.pszText = (LPSTR)(LPCTSTR)input;
	this->InsertItem(&lvi); 
	return nIndex;			
}

BOOL CAxListCtrl::IsExist(CString input, int nCol)
{
	int	ii = 0;
	CString	tmpstr;

	for ( ii = 0 ; ii < this->GetItemCount() ; ii++ )
	{
		tmpstr = this->GetItemText(ii, nCol);

		if (tmpstr == input)
			return TRUE;
	}

	return FALSE;
}

int CAxListCtrl::DelItem(int nRow)
{
	if (nRow >= 0)
	{
		this->DeleteItem(nRow);
		return 0;
	}

	POSITION pos = this->GetFirstSelectedItemPosition();
	CArray<int, int>	templist;

	while (pos)
	{		
		templist.Add(this->GetNextSelectedItem(pos));
	}
	
	for ( int ii = templist.GetSize() - 1 ; ii >= 0 ; ii-- )
	{
		this->DeleteItem(templist.GetAt(ii));
	}
	return 0;
}

// return value
// 0 : normal status
// -1 : non select
// -2 : multi select can't move
int CAxListCtrl::MoveItem(BOOL bUp)
{
	int	nCnt = this->GetSelectedCount();


	if (nCnt <= 0)
		return -1;	
	else if (nCnt > 1)
		return -2;

	int	nTotCnt = this->GetItemCount();
	int	nIndex = this->GetSelectionMark();
	int	NewIndex = 0, ii = 0, jj = 0;
	int	nColCnt = this->GetColCount();

	
	CStringArray	arInput;

	arInput.RemoveAll();
	for ( ii = 0 ; ii < nColCnt ; ii++ )
	{
		arInput.Add(this->GetItemText(nIndex, ii));
	}

	if (bUp)
	{
		NewIndex = nIndex - 1;
		if (NewIndex < 0)
			return 0;

	}
	else
	{
		NewIndex = nIndex + 1;

		if (NewIndex >= nTotCnt)
			return 0;

		
	}

	this->DelItem(nIndex);		

	for ( ii = 0 ; ii < nColCnt ; ii++ )
	{			
		this->AddItem(arInput.GetAt(ii), NewIndex, ii);
	}
	
	SetSelection(NewIndex);
	
	return 0;

}

int CAxListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
{
	++m_nColCnt;
	return CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}

int CAxListCtrl::InsertColumn(int nCol, const LVCOLUMN *pColumn)
{
	++m_nColCnt;
	return CListCtrl::InsertColumn(nCol, pColumn);
}

BOOL CAxListCtrl::DeleteColumn(int nCol)
{
	m_nColCnt--;
	return CListCtrl::DeleteColumn(nCol);
}

// -1 : selection clear
// nRow > 0 : Select
void CAxListCtrl::SetSelection(int nRow)
{	
	if (nRow < 0)
	{
		SetItemState(-1, 0, LVIS_SELECTED|LVIS_FOCUSED);
		return;
	}

	SetItemState(-1, 0, LVIS_SELECTED|LVIS_FOCUSED);
	SetSelectionMark(nRow);
	SetItemState(nRow, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
}

void CAxListCtrl::ListSort(int nCol, BOOL bToggle)
{
	
	CWaitCursor	wait;
	CListCtrlSort sc(this, nCol);
	sc.Sort(m_bSort, CListCtrlSort::SortDataType::dtSTRING);		
	AfxGetApp()->RestoreWaitCursor();

	if (bToggle)
		m_bSort = (BOOL)!m_bSort;
	else
		m_bSort = TRUE;
}

int CAxListCtrl::ModifyItem(CString newval, int nRow, int nCol)
{
	if (newval.IsEmpty())
		return -1;

	if (IsExist(newval, nCol))
		return -2;

	this->SetItemText(nRow, nCol, newval);
	return 0;
}

int CAxListCtrl::FindRow(CString value, int nCol)
{
	int	ii = 0;
	CString	tmpstr;

	for ( ii = 0 ; ii < this->GetItemCount() ; ii++ )
	{
		tmpstr = this->GetItemText(ii, nCol);

		if (tmpstr == value)
			return ii;
	}

	return -1;
}
	
int CAxListCtrl::GetSelection()
{
	int	nCnt = GetSelectedCount();

	if (nCnt != 1)
		return -1;

	return GetSelectionMark();
}

BOOL CAxListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CListCtrl::PreCreateWindow(cs);
}

BOOL CAxListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYUP)
	{
		CWnd*	pWnd = GetParent();
		if (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN || 
			pMsg->wParam == VK_NEXT || pMsg->wParam == VK_PRIOR ||
			pMsg->wParam == VK_HOME || pMsg->wParam == VK_END)
		{
			if (pWnd)
			{
				UINT	ctrlID = this->GetDlgCtrlID();
				NMHDR	nmhdr;
				nmhdr.hwndFrom = this->m_hWnd;
				nmhdr.idFrom = ctrlID;
				nmhdr.code = NM_CLICK;
				NM_LISTVIEW	nmlistview;

				memset(&nmlistview, 0x00, sizeof(nmlistview));
				nmlistview.hdr = nmhdr;
				nmlistview.iItem = this->GetSelection();
			
				pWnd->SendMessage(WM_NOTIFY, (WPARAM)this->GetDlgCtrlID(), (LPARAM)&nmlistview);
				return TRUE;
			}
		}

	}
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CAxListCtrl::setCheck(int index, BOOL bCheck)
{
	int	nCount = this->GetItemCount();

	if (index < 0 || index >= nCount)
		return;

	ListView_SetCheckState(this->m_hWnd, index, bCheck);
}

BOOL CAxListCtrl::getCheck(int index)
{
	int	nCount = this->GetItemCount();

	if (index < 0 || index >= nCount)
		return FALSE;

	return ListView_GetCheckState(this->m_hWnd, index);
}

void CAxListCtrl::HaveCheckBox()
{
	DWORD	dwStyle = this->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_CHECKBOXES;
	this->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
	m_bCheckBox = TRUE;
}

int CAxListCtrl::GetCheckCount()
{
	if (m_bCheckBox == FALSE)
		return 0;

	int	nIndex = this->GetSelection();	
	int	nCount = this->GetItemCount();
	int	nCheck = 0;

	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		if (this->getCheck(ii))
			nCheck++;
	}
	
	return nCheck;
}

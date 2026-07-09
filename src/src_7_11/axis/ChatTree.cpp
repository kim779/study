// ChatTree.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "ChatTree.h"


// CChatTree

IMPLEMENT_DYNAMIC(CChatTree, CTreeCtrl)

static int CALLBACK mapSortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the tree control.
	// The lParam of an item is just its handle.
	CChatTree* pTree = (CChatTree*) lParamSort;
	//CString    strItem1 = pTree->m_treeArr.GetAt(lParam1).mapn;
	//CString    strItem2 = pTree->m_treeArr.GetAt(lParam2).mapn;
	CString    strItem1 = pTree->m_userTree.GetAt(lParam1)->m_name;
	CString    strItem2 = pTree->m_userTree.GetAt(lParam2)->m_name;
	
	/*
	if (strItem1.GetLength() >= L_MAPN && strItem2.GetLength() >= L_MAPN)
	{
		strItem1 = strItem1.Left(L_MAPN);
		strItem2 = strItem2.Left(L_MAPN);
		return _tcscmp(strItem1.Mid(L_SGID), strItem2.Mid(L_SGID));
	}
	*/
	return _tcscmp(strItem2, strItem1);
}

CChatTree::CChatTree()
{

}

CChatTree::~CChatTree()
{
}


BEGIN_MESSAGE_MAP(CChatTree, CTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, &CChatTree::OnTvnItemexpanded)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, &CChatTree::OnTvnKeydown)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CChatTree::OnTvnSelchanged)
END_MESSAGE_MAP()



// CChatTree message handlers

CString CChatTree::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return _T("");
}

void CChatTree::LoadFile(CString path)   
{
	m_userTree.RemoveAll();
	class CUserTree*  userTree;

	if(path.IsEmpty())
		path = _T("chatlist.ini");
	
	CFile	file;
	int Size = 0;
	char* sbuff;
	CString treedata, tmp, data;

	if(file.Open(path, CFile::modeRead))
	{
		Size = (UINT)file.GetLength();

		sbuff = new char[Size + 1];
		ZeroMemory(sbuff, Size + 1);
		UINT rc = file.Read(sbuff, Size);

		if(rc != Size)
		{
			delete[] sbuff;
			file.Close();
		}

		treedata = CString(sbuff, Size);
		delete[] sbuff;

		while(!treedata.IsEmpty())
		{
			tmp = Parser(treedata, _T("\n"));

			if(tmp.IsEmpty())
				break;

			int ii = 0;
			userTree = new CUserTree();
			while(!tmp.IsEmpty())
			{
				data = Parser(tmp, _T("\t"));
				switch(ii)
				{
				case 0:
					userTree->m_mgub = data;
					break;
				case 1:
					userTree->m_name = data;
					break;
				case 2:
					userTree->m_desc = data;
					break;
				case 3:
					userTree->m_ckgb = data;
					break;
				case 4:
					userTree->m_ckid = data;
					break;
				}
				ii++;
			}
			m_userTree.Add(userTree);
		}
	}
}

void CChatTree::InitTree()
{
	Initialize();
//	POSITION pos;
	CString sKey;
//	bool is_splash;
	//void* ptr;
	int iFind = 0;

	DeleteAllItems();	
	m_itemMap1.RemoveAll();
	m_itemMap2.RemoveAll();

	for(int ii = 0 ; ii < m_userTree.GetSize(); ii++)
		insert(m_userTree.GetAt(ii)->m_mgub, m_userTree.GetAt(ii)->m_name, ii);
}

void CChatTree::Initialize()
{
	CImageList	imglist;
	CBitmap		bitmap;

	bitmap.LoadBitmap(IDB_TREEIMAGE);
	imglist.Create(16, 16, ILC_COLOR24|ILC_MASK, 5, 1);
	imglist.Add(&bitmap, RGB(255, 0, 255));
	imglist.SetBkColor(CLR_NONE);
	SetImageList(&imglist, TVSIL_NORMAL);
	imglist.Detach();
	bitmap.Detach();
}


void CChatTree::insert(CString strgubn, CString strname, int idx)
{
	void* ptr;
	CString tmps;
	HTREEITEM hitem1 = NULL, hitem2 = NULL;

	tmps = strgubn;

	if (!m_itemMap1.Lookup(tmps, ptr))
	{
		hitem1 = InsertItem(tmps, 0, 0);
		SetItemData(hitem1, R_NONE);
		m_itemMap1.SetAt(tmps, hitem1);
	}
	else
		hitem1 = (HTREEITEM)ptr;

	hitem1 = InsertItem(strname, 2, 2, hitem1);

	SetItemData(hitem1, idx);
}


void CChatTree::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CTreeCtrl::PreSubclassWindow();
}

void CChatTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CChatTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CChatTree::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CTreeCtrl::OnMouseMove(nFlags, point);
}


void CChatTree::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CTreeCtrl::OnLButtonUp(nFlags, point);
}


void CChatTree::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CTreeCtrl::OnTimer(nIDEvent);
}


void CChatTree::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CTreeCtrl::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
}


void CChatTree::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CChatTree::OnTvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CChatTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

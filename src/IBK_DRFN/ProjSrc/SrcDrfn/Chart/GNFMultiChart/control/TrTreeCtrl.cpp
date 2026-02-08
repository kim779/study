// TrTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "WinNet.h"
#include "TrTreeCtrl.h"
//#include "FormTrTree.h"
#include "../define/DragNDropDefine.h"

//#include "../ToolKit/Utility/DongbuMenuTreeUtil/MenuTreeUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT RMSG_OPENNEWMAP = ::RegisterWindowMessage("RMSG_OPENNEWMAP");

/////////////////////////////////////////////////////////////////////////////
// CTrTreeCtrl

CTrTreeCtrl::CTrTreeCtrl()
{
	m_hSelTreeItem = NULL;
	m_bLBtnDown = FALSE;
	m_bDragNDrop = FALSE;
}

CTrTreeCtrl::~CTrTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CTrTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CTrTreeCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrTreeCtrl message handlers
#define UDM_MAP_OPEN_SEARCH			WM_USER+611 // 4Byte Code Enter
#define UDM_TREE_CLICK				WM_USER+612 // 4Byte Code Enter
#define UDM_TREE_RBUTTON			WM_USER+613 // 4Byte Code Enter

void CTrTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = GetSelectedItem();
	if(hItem == NULL)
		return;

	if(ItemHasChildren(hItem))
		return;

	CString selString = GetItemText(hItem);
	GetParent()->GetParent()->SendMessage(UDM_MAP_OPEN_SEARCH, 0, (LPARAM)(LPTSTR)(LPCTSTR)selString);

	*pResult = 0;
}

void CTrTreeCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//HTREEITEM hItem = GetSelectedItem();
	HTREEITEM hItem = m_hSelTreeItem;
	if(hItem == NULL)
		return;

	if(ItemHasChildren(hItem))
		return;

	DWORD selData = GetItemData(hItem);
	if(selData)
	{	
		//<<솔로몬. 저장화면들 동시에 로드하다가 죽는 현상 발생하여 수정
		//GetParent()->GetParent()->SendMessage(UDM_TREE_CLICK, 0, (LPARAM)selData);
		CWnd* pWnd = GetParent();
		if( pWnd && ::IsWindow(pWnd->GetSafeHwnd()) )
		{
			pWnd = pWnd->GetParent();
			if( pWnd && ::IsWindow(pWnd->GetSafeHwnd()) )
				pWnd->SendMessage(UDM_TREE_CLICK, 0, (LPARAM)selData);
		}
		//>>
	}

//	CString selString = GetItemText(hItem);
//	CString szTr = GetTrString(selString);
//	AfxGetMainWnd()->SendMessage(UDM_MAP_OPEN_SEARCH, 0, (LPARAM)(LPTSTR)(LPCTSTR)szTr);

	*pResult = 0;
}

void CTrTreeCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hItem = GetSelectedItem();
	if(hItem == NULL)
		return;

	if(ItemHasChildren(hItem))
		return;

//	CTrFileData* pTrFileData = (CTrFileData*)GetItemData(hItem);
//	CTFMenuitemInfo* pMenuitemInfo = (CTFMenuitemInfo*)pTrFileData->m_pClass;

	long lTest = (long)GetItemData(hItem);

	CString szTr;
	szTr.Format("%4.4d",lTest);
	szTr += "0";
	
//	AfxGetMainWnd()->SendMessage(UDM_MAP_OPEN_SEARCH, 0, (LPARAM)(LPTSTR)(LPCTSTR)szTr);

	//CString selString = GetItemText(hItem);
	//CString szTr = GetTrString(selString);



	
	*pResult = 0;
}

CString CTrTreeCtrl::GetTrString(CString szText)
{
	int nStart = szText.Find("[");
	int nEnd   = szText.Find("]");
	if(nStart < 0 || nStart >= szText.GetLength())
		return "";
	if(nEnd < 0 || nEnd >= szText.GetLength())
		return "";
	CString szTrcode = szText.Mid(nStart+1 , nEnd-nStart-1);
	szTrcode += "0";

	return szTrcode;
}

void CTrTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call 
	UINT nHF;
	m_hSelTreeItem = HitTest(point, &nHF);
	SelectItem(m_hSelTreeItem);

	CTreeCtrl::OnLButtonDown(nFlags, point);
}


BOOL CTrTreeCtrl::Create(DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID)
{
//	CTreeCtrl::Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | TVS_HASBUTTONS | 
//												TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP,
//												CRect(0, 0, 0, 0),
//												pParentWnd,	
//												nID);
	CTreeCtrl::Create(dwStyle|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_NOTOOLTIPS,
												CRect(0, 0, 0, 0),
												pParentWnd,	
												nID);
	//CTreeCtrl::ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	return TRUE;

	//GFXOUTBARCTRL_CLASSNAME
}

void CTrTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call 
	UINT uFlags;
	HTREEITEM hSelItem = HitTest(point, &uFlags);

	if (hSelItem != NULL)
	{
		if (TVHT_ONITEM & uFlags)
			TRACE("\n# TVHT_ONITEM");		//# item
		else
			return;

		if (TVHT_ONITEM & uFlags)
		{
			SelectItem(hSelItem);
		}
	}
	else
		return;

	if(ItemHasChildren(hSelItem))
		return;

	CString selString = GetItemText(hSelItem);
	GetParent()->GetParent()->SendMessage(UDM_TREE_RBUTTON, (WPARAM)&point, (LPARAM)(LPTSTR)(LPCTSTR)selString);

	CTreeCtrl::OnRButtonDown(nFlags, point);
}

void CTrTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_bLBtnDown = TRUE;
	SetCapture();	
	*pResult = 0;
}

void CTrTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bLBtnDown)
	{
		m_bLBtnDown = FALSE;
		ReleaseCapture();

		if(m_bDragNDrop)	// 드래그 중이었으면
		{
			HTREEITEM hSelItem = GetSelectedItem();

			CPoint ptDrag(point);
			ClientToScreen(&ptDrag);
			// 데이터 생성
			CDragNDrop* pDnD = new CDragNDrop;				
			CString strPacketShared;
			CString selString = GetItemText(hSelItem);
			strPacketShared = selString;
			pDnD->SetData(&ptDrag, m_hWnd, _T("지표-드래그"), (LPTSTR)(LPCTSTR)strPacketShared);
			GetParent()->GetParent()->SendMessage(RMSG_DRAGEND, 0, (LPARAM)pDnD);
		}
		m_bDragNDrop = FALSE;
	}

	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CTrTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bLBtnDown)
	{
		// 드래그임을 표시
		m_bDragNDrop = TRUE;
		CPoint ptDrag(point);
		ClientToScreen(&ptDrag);
		GetParent()->GetParent()->SendMessage(RMSG_DRAGSTART, (WPARAM)&ptDrag, (LPARAM)m_hWnd);
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}
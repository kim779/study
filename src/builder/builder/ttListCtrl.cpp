// ttListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisBuilder.h"
#include "ttListCtrl.h"

#include "../h/axisvar.h"
#include "h/mainvar.h"
#include "awWcc/libWcc.h"

#define INITIAL_HITMASK	LVHT_ONITEMLABEL

// CTTListCtrl

IMPLEMENT_DYNAMIC(CTTListCtrl, CListCtrl)

CTTListCtrl::CTTListCtrl()
{
	m_wHitMask = INITIAL_HITMASK;
	m_files.RemoveAll();
	m_kind = tMAP;

	m_dragImg = NULL;
}

CTTListCtrl::~CTTListCtrl()
{
	deleteAllToolTips();
}


BEGIN_MESSAGE_MAP(CTTListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CTTListCtrl::OnLvnBegindrag)
	ON_NOTIFY_REFLECT(NM_CLICK, &CTTListCtrl::OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CTTListCtrl::OnNMDblclk)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CTTListCtrl 메시지 처리기입니다.


INT_PTR CTTListCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	int	nItem, nSubItem;
	UINT	nFlags;
	LVHITTESTINFO	lvhi;
	
	lvhi.pt = point;
	nItem    = ListView_SubItemHitTest(this->m_hWnd, &lvhi);
	nSubItem = lvhi.iSubItem;
	nFlags   =   lvhi.flags;
	
	if (nFlags & m_wHitMask)
	{
		CRect	rc;

		GetClientRect(&rc);
		//Fill in the TOOLINFO structure
		pTI->hwnd = m_hWnd;
		pTI->uId  = (UINT) (nItem * 100 + nSubItem);
		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->rect = rc;
		
		return pTI->uId;
	}
	else
		return -1;
}


void CTTListCtrl::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if (GetSelectedCount() <= 0)
		return;

	CPoint	pt;

	m_dragImg = createDragImage(pt);
	if (m_dragImg == (CImageList *)NULL)
		return;

	int	nItem;
	CPoint	startpt = pNMLV->ptAction;
	startpt -= pt;

	m_files.RemoveAll();
	for (POSITION pos = GetFirstSelectedItemPosition(); pos; )
	{
		nItem = GetNextSelectedItem(pos);
		addFiles((int)GetItemData(nItem));
	}
	m_dragImg->BeginDrag(0, CPoint(0, 0));
	m_dragImg->DragEnter(GetDesktopWindow(), startpt);
	SetCapture();
}


void CTTListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
}


void CTTListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	if (m_kind == tSYM)
		return;

	POSITION pos = GetFirstSelectedItemPosition();
	int	idx = (int)GetItemData(GetNextSelectedItem(pos));
	addFiles(idx, true);
	GetParent()->SendMessage(WM_USER+100, MAKEWPARAM(5, 0), (LPARAM)idx);
}

void CTTListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_dragImg != NULL)
	{
		CPoint	pt = point;

		ClientToScreen(&pt);
		m_dragImg->DragMove(pt);
	}
	CListCtrl::OnMouseMove(nFlags, point);
}


void CTTListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_dragImg != NULL)
	{
		m_dragImg->DragLeave(this);
		m_dragImg->EndDrag();
		::ReleaseCapture();

		m_dragImg->DeleteImageList();
		delete m_dragImg;
		m_dragImg = NULL;

		switch (m_kind)
		{
		case tMAP:
			if (m_files.GetCount() > 0)
			{
				CString	maps = m_files.GetAt(0);
				GetParent()->SendMessage(WM_USER+100, MAKEWPARAM(6, 0), (LPARAM)maps.operator LPCSTR());
			}
			break;

		case tTMPL:
		case tSYM:
			{
				short x = (WORD)point.x;
				short y = (WORD)point.y;
		
				if (x < 0)
					point.x = 0;
				else
					x = 0;
		
				if (y < 0)
					point.y = 0;
				else
					y = 0;
		
				ClientToScreen(&point);
				point.x += x;
				point.y += y;

				if (m_kind == tTMPL)
				{
					CString	tmps;
					CString	fullN;

					wccSetTempPath(tmps, _T("template"));
					fullN = setMapPath(m_name) + _T(";") + tmps;
					GetParent()->SendMessage(WM_USER+100, MAKEWPARAM(3, 0), (LPARAM)fullN.operator LPCSTR());
					
					AfxGetMainWnd()->PostMessage(WM_USER, MAKEWPARAM(ID_USR_LOADTEMPLATE, 1), MAKELPARAM(point.x, point.y));
				}
				else
					AfxGetMainWnd()->PostMessage(WM_USER, MAKEWPARAM(ID_USR_DROP, 1), MAKELPARAM(point.x, point.y));
			}
			break;
		}
	}

	CListCtrl::OnLButtonUp(nFlags, point);
}

BOOL CTTListCtrl::OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
	//Handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	
	//Ignore messages from the built in tooltip, we are processing them internally
	if ((pNMHDR->idFrom == (UINT)m_hWnd)
		&& (((pNMHDR->code == TTN_NEEDTEXTA) && (pTTTA->uFlags & TTF_IDISHWND)) || ((pNMHDR->code == TTN_NEEDTEXTW) && (pTTTW->uFlags & TTF_IDISHWND))))
	{
		return FALSE;
	}
	
	*pResult = 0;

	CPoint	pt;
	CString strTipText;
	const	MSG* pMessage;
	int	nItem, nSubItem;
	UINT	nFlags;
	LVHITTESTINFO lvhitTestInfo;

	pMessage = GetCurrentMessage();
	ASSERT (pMessage);
	pt = pMessage->pt;	// Get the point from the message
	ScreenToClient(&pt);	// Convert the point's coords to be relative to this control
	
	// See if the point falls onto a list item
	
	lvhitTestInfo.pt = pt;
	nItem    = SubItemHitTest(&lvhitTestInfo);
	nSubItem = lvhitTestInfo.iSubItem;
	nFlags   = lvhitTestInfo.flags;
	
	// nFlags is 0 if the SubItemHitTest fails
	// Therefore, 0 & <anything> will equal false
	if (nFlags & m_wHitMask)
	{
		// If it did fall on a list item,
		// and it was also hit one of the
		// item specific sub-areas we wish to show tool tips for
		// Lookup the list item's text in the ToolTip Map
		
		CString	strKey;
		
		strKey.Format(_T("%d"),  nItem * 100 + nSubItem);
		if (m_ttMap.Lookup(strKey, strTipText))
		{
			//If there was a CString associated with the list item,
			//copy it's text (up to 80 characters worth, limitation of the TOOLTIPTEXT structure)
			//into the TOOLTIPTEXT structure's szText member
			
#ifndef _UNICODE
			if (pNMHDR->code == TTN_NEEDTEXTA)
				lstrcpyn(pTTTA->szText, strTipText, 80);
			else
				_mbstowcsz(pTTTW->szText, strTipText, 80);
#else
			if (pNMHDR->code == TTN_NEEDTEXTA)
				_wcstombsz(pTTTA->szText, strTipText, 80);
			else
				lstrcpyn(pTTTW->szText, strTipText, 80);
#endif
			return FALSE;    // We found a tool tip, tell the framework this message has been handled
		}
	}
	
	return FALSE; // We didn't handle the message,
}

BOOL CTTListCtrl::AddItem(int item, int subItem, CString str, DWORD_PTR data, CString tipText)
{
	BOOL	ret;
	LV_ITEM	lvi;

        lvi.mask     = LVIF_TEXT | LVIF_PARAM;
        lvi.iItem    = item;
        lvi.iSubItem = subItem;
	lvi.lParam   = data;
        lvi.pszText  = (LPTSTR) str.operator LPCTSTR();

        if (subItem == 0)
                ret = InsertItem(&lvi);
	else
		ret = SetItem(&lvi);
	setItemToolTipText(item, subItem, tipText);
	return ret;
}

BOOL CTTListCtrl::DeleteAllItems()
{
	BOOL	retVal = CListCtrl::DeleteAllItems();
	if (retVal)
		deleteAllToolTips();
	
	return retVal;
}

int CTTListCtrl::GetFiles(CStringArray& files)
{
	files.Copy(m_files);
	return (int)files.GetSize();
}

void CTTListCtrl::AddFiles(CString maps)
{
	m_files.RemoveAll();
	m_files.Add(maps);
}

BOOL CTTListCtrl::setItemToolTipText(int nItem, int nSubItem, LPCTSTR lpszToolTipText)
{
	CString strKey;
	
	strKey.Format(_T("%d"), nItem * 100 + nSubItem);
	m_ttMap.SetAt(strKey, lpszToolTipText);
	return TRUE;
}

void CTTListCtrl::deleteAllToolTips()
{
	m_ttMap.RemoveAll();
}

CImageList* CTTListCtrl::createDragImage(CPoint& point)
{
	CRect	singleRc, completeRc;
	int	width, itop, ibottom;

	GetClientRect(singleRc);
	completeRc.SetRectEmpty();
	width = singleRc.Width();
	itop  = GetTopIndex() - 1;
	ibottom = GetTopIndex() + GetCountPerPage() - 1;

	if (ibottom > (GetItemCount() - 1))
		ibottom = GetItemCount()-1;

	while ((itop = GetNextItem(itop, LVNI_SELECTED)) != -1)
	{
		if (itop > ibottom)
			break; 

		GetItemRect(itop, singleRc, LVIR_BOUNDS);
		singleRc.left  = max(0, singleRc.left);
		singleRc.right = min(width, singleRc.right);
		completeRc.UnionRect(completeRc, singleRc);
	}
		
	CClientDC dcClient(this);
	CDC	memDC;
	CBitmap bitmap;

	if (!memDC.CreateCompatibleDC(&dcClient))
		return NULL;

	if (!bitmap.CreateCompatibleBitmap(&dcClient, completeRc.Width(), completeRc.Height()))
		return NULL;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
 	memDC.FillSolidRect(0, 0, completeRc.Width(), completeRc.Height(), RGB(255, 255,255));

	itop = GetTopIndex() - 1;
	while ((itop = GetNextItem(itop, LVNI_SELECTED)) != -1)
	{	
		if (itop > ibottom)
			return NULL;

		CPoint	pt;
		CImageList* pSingleImageList = CreateDragImage(itop, &pt);
		if (pSingleImageList)
		{
			GetItemRect(itop, singleRc, LVIR_BOUNDS);
			pSingleImageList->Draw(&memDC, 0, CPoint(singleRc.left - completeRc.left,
						singleRc.top - completeRc.top), ILD_MASK);
			pSingleImageList->DeleteImageList();
			delete pSingleImageList;
		}
	}

 	memDC.SelectObject(pOldMemDCBitmap);
	CImageList* pCompleteImageList = new CImageList;
	pCompleteImageList->Create(completeRc.Width(), completeRc.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	pCompleteImageList->Add(&bitmap, RGB(0, 255, 0)); 
	bitmap.DeleteObject();

	point.x = completeRc.left;
	point.y = completeRc.top;

	return pCompleteImageList;
}

void CTTListCtrl::addFiles(int idx, bool reset)
{
	if  (reset)
		m_files.RemoveAll();

	m_name = (char *)GetParent()->SendMessage(WM_USER+100, MAKEWPARAM(4, 0), (LPARAM)idx);
	m_files.Add(m_name);
}

CString CTTListCtrl::setMapPath(CString maps)
{
	CString	fullN;

	switch (m_kind)
	{
	case tMAP:
		fullN.Format(_T("map/%s/%s/%s"), maps.Left(L_SGID), maps.Left(L_TGID), maps);
		break;

	case tTMPL:
		fullN.Format(_T("template/%s/%s/%s"), maps.Left(L_SGID), maps.Left(L_TGID), maps);
		break;

	default:
		return _T("");
	}
	return fullN;
}

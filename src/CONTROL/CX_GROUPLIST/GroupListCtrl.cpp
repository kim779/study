// GroupListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CX_GROUPLIST.h"
#include "GroupListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupListCtrl

CGroupListCtrl::CGroupListCtrl()
{
	VERIFY(m_fontBold.CreateFont(	13,                        // nHeight
					0,                         // nWidth
					0,                         // nEscapement
					0,                         // nOrientation
					FW_BOLD,		// nWeight
					FALSE,                     // bItalic
					FALSE,                     // bUnderline
					0,                         // cStrikeOut
					ANSI_CHARSET,              // nCharSet
					OUT_DEFAULT_PRECIS,        // nOutPrecision
					CLIP_DEFAULT_PRECIS,       // nClipPrecision
					DEFAULT_QUALITY,           // nQuality
					DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
					_T("굴림"))
	);

	VERIFY(m_fontNormal.CreateFont(	13,                        // nHeight
					0,                         // nWidth
					0,                         // nEscapement
					0,                         // nOrientation
					FW_NORMAL,		// nWeight
					FALSE,                     // bItalic
					FALSE,                     // bUnderline
					0,                         // cStrikeOut
					ANSI_CHARSET,              // nCharSet
					OUT_DEFAULT_PRECIS,        // nOutPrecision
					CLIP_DEFAULT_PRECIS,       // nClipPrecision
					DEFAULT_QUALITY,           // nQuality
					DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
					_T("굴림"))
				);

	m_bFont = FALSE;
}

CGroupListCtrl::~CGroupListCtrl()
{
}


BEGIN_MESSAGE_MAP(CGroupListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CGroupListCtrl)	
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDrawList)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_SETDISPINFO, OnSetdispinfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT CGroupListCtrl::OnSetFont(WPARAM wParam, LPARAM)
{
	if (m_bFont == FALSE)
	{
		CRect	rc;
		WINDOWPOS wp{};

		GetWindowRect(&rc);
		wp.hwnd = this->m_hWnd;
		wp.cx = rc.Width();
		wp.cy = rc.Height();
		wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;

		const	LRESULT lrReturn(Default());

		SendMessage(WM_WINDOWPOSCHANGED, 0, reinterpret_cast<LPARAM> (&wp));

		m_bFont = TRUE;

		return lrReturn;

	}

	const	LRESULT lrReturn(Default());
	return lrReturn;
}


BOOL CGroupListCtrl::DeleteAllItems()
{
	ReleaseData();
	return CListCtrl::DeleteAllItems();
}

void CGroupListCtrl::ReleaseData()
{
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		sGLI* gli = m_list.GetNext(pos);
		if (gli)
			delete gli;
	}

	m_list.RemoveAll();
}

int CGroupListCtrl::GetItemIdx(sGLI* p)
{
	LVFINDINFO info{};

	info.flags = LVFI_PARAM;
	info.lParam = (LPARAM)p;

	return CListCtrl::FindItem(&info);
}

sGLI* CGroupListCtrl::FindGroup(CString gid)
{
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		sGLI* p = m_list.GetNext(pos);
		if (p->id == gid)
			return p;
	}

	return nullptr;
}

sGLI* CGroupListCtrl::FindItem(CString gid, CString id, CString jucode)
{
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		sGLI* p = m_list.GetNext(pos);
		if (p->type == TYPE_GROUP)
			continue;

		if (p->gid == gid && p->id == id)
		{
			if (p->data == jucode)
				return p;
		}
	}

	return nullptr;
}

int CGroupListCtrl::InsertGroup(LPCTSTR lszItem, CString id, CString strData)
{
	if (GetGroupGLI(id))
		return -1;

	const	int	iCount = CListCtrl::GetItemCount();
	//g_log.write("CGroupListCtrl::InsertGroup iCount", iCount);

	const	int	index = CListCtrl::InsertItem(iCount, lszItem, 1);
	if (index < 0)
		return -1;

	sGLI* gli = new sGLI;

	gli->clrText = RGB(0, 0, 0);
	gli->state = GROUP_STATE_EXPAND;
	gli->type = TYPE_GROUP;
	gli->id = id;
	gli->data = strData;

	CListCtrl::SetItemData(index, (DWORD)gli);

	m_list.AddTail(gli);

	return index;
}

void CGroupListCtrl::DeleteItem(sGLI* p)
{
	const	int	nItem = GetItemIdx(p);
	if (nItem < 0)
		return;

	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		POSITION oldPos = pos;
		sGLI* pg = m_list.GetNext(pos);
		if (pg == p)
		{
			delete p;
			m_list.RemoveAt(oldPos);
			return;
		}
	}

	CListCtrl::DeleteItem(nItem);
}

int CGroupListCtrl::GetGroupItemCount(CString gid)
{
	int	iCount = 0;
	POSITION pos = m_list.GetHeadPosition();

	while (pos)
	{
		sGLI* p = m_list.GetNext(pos);
		if (p->type == TYPE_GROUP)
			continue;

		if (p->gid == gid)
			iCount++;
	}
	return iCount;
}

int CGroupListCtrl::InsertItem(LPCTSTR lszItem, CString gid, CString id, COLORREF clr, CString strData)
{
	sGLI*	pg = GetGroupGLI(gid);
	if (pg == nullptr)
		return -1;

	const	int	nItem = GetItemIdx(pg);
	if (nItem < 0)
		return -1;

//	g_log.write(gid,nItem+1);

	const	int	idx = CListCtrl::InsertItem(nItem + 1, lszItem, -1);
	if (idx < 0)
		return idx;

	//	g_log.write("ii",ii);
	//	g_log.write("GetItemCount",GetItemCount());

	sGLI* gli = new sGLI;

	gli->clrText = clr;
	gli->state = ITEM_STATE_VISIBLE;
	gli->type = TYPE_ITEM;
	gli->id = id;
	gli->gid = gid;
	gli->data = strData;

	const	int	iCount = CListCtrl::GetHeaderCtrl()->GetItemCount();
	for (int jj = 0; jj < iCount; jj++)
		gli->text.Add("");

	CListCtrl::SetItemData(idx, (DWORD)gli);
	m_list.AddTail(gli);

	return idx;
}

int CGroupListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
{
	const	int	idx = CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);

	if (idx < 0)
		return idx;

	HDITEM	info{};

	info.mask = HDI_FORMAT;
	if (nFormat == LVCFMT_RIGHT)
		info.fmt = HDF_OWNERDRAW | HDF_RIGHT;
	else
		info.fmt = HDF_OWNERDRAW;

	GetHeaderCtrl()->SetItem(idx, &info);

	return idx;
}

BOOL CGroupListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{
	if (!CListCtrl::SetItemText(nItem, nSubItem, lpszText))
		return FALSE;

	sGLI* p = (sGLI*)GetItemData(nItem);

	if (p->text.GetSize() <= nSubItem)
		return FALSE;

	p->text.SetAt(nSubItem, lpszText);
	return TRUE;
}

void CGroupListCtrl::GetGroupGLI(sGLI* pg, CList<sGLI*, sGLI*>& list)
{
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		sGLI* p = m_list.GetNext(pos);
		if (p->type == TYPE_GROUP)
			continue;

		if (p->gid == pg->id)
			list.AddTail(p);
	}
}

sGLI* CGroupListCtrl::GetGroupGLI(CString id)
{
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		sGLI* p = m_list.GetNext(pos);
		if (p->type != TYPE_GROUP)
			continue;

		if (p->id == id)
			return p;
	}
	return nullptr;
}

sGLI* CGroupListCtrl::GetItemGLI(int index)
{
	int	idx = 0;

	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		sGLI* p = m_list.GetNext(pos);
		if (idx == index)
			return p;
		idx++;
	}

	return nullptr;
}

void CGroupListCtrl::OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;
	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		const	int	iRow = (int)pLVCD->nmcd.dwItemSpec;
		sGLI*	p = (sGLI*)GetItemData(iRow);
		CDC*	pDC = CDC::FromHandle(pLVCD->nmcd.hdc);

		//pDC->SetTextAlign(TA_RIGHT);

		if (p->type == TYPE_GROUP)
			pDC->SelectObject(m_fontBold);
		else
			pDC->SelectObject(m_fontNormal);

		pLVCD->clrText = p->clrText;

		// We want item post-paint notifications, so...
		*pResult = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;

	}
	else if (CDDS_ITEMPOSTPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_DODEFAULT;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGroupListCtrl message handlers

void CGroupListCtrl::MakeImageList(void)
{
	CFont	font1, font2;
	CBitmap bm, bm1;

	font1.CreatePointFont(110, "굴림체");
	SetFont(&font1);

	font2.CreatePointFont(100, "굴림체");
	m_header.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());
	m_header.SetFont(&font2);

	m_imageList.Create(9, 9, ILC_COLOR24, 0, 2);
	bm.LoadBitmap(IDB_EXPAND);
	m_imageList.Add(&bm, RGB(0, 0, 0));

	bm1.LoadBitmap(IDB_SHRINK);
	m_imageList.Add(&bm1, RGB(0, 0, 0));

	SetImageList(&m_imageList, LVSIL_SMALL);
}

int CGroupListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(LVS_SORTASCENDING | LVS_SORTDESCENDING, /*LVS_OWNERDRAWFIXED*/ 0); // sorting 없도록 
	//ModifyStyleEx(0, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	MakeImageList();

	return TRUE;
}

void CGroupListCtrl::OnDestroy()
{
	CListCtrl::OnDestroy();

	ReleaseData();
}

void CGroupListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE* nm = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	DWORD	dw = GetItemData(nm->iItem);
	sGLI*	lgp = reinterpret_cast<sGLI*>(dw);

	if (lgp)
	{
		if (lgp->type == TYPE_ITEM)
		{
			CString strCode = GetItemText(nm->iItem, 2);

			m_strSelectCode = strCode;
			::SendMessage(GetParent()->GetSafeHwnd(), WM_ITEM_CLICK, 0, 0);
		}
	}

	*pResult = 0;
}

void CGroupListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE* nm = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);

	DWORD dw = GetItemData(nm->iItem);
	sGLI* lgp = reinterpret_cast<sGLI*>(dw);

	if (lgp)
	{
		if (lgp->type == TYPE_GROUP)
		{
			OnDoGroupDbclk(lgp, nm->iItem);
		}
	}

	*pResult = 0;
}

void CGroupListCtrl::ExpandAllItems(BOOL bExpand)
{
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		sGLI* pg = m_list.GetNext(pos);
		if (pg->type != TYPE_GROUP)
			continue;

		if (pg->state == GROUP_STATE_EXPAND)
		{
			if (bExpand)
			{ // notthing
			}
			else
			{
				DoShrink(pg);

				LVITEM	item{};
				item.mask = LVIF_IMAGE;
				item.iImage = 0;
				item.iSubItem = 0;
				item.iItem = GetItemIdx(pg);
				CListCtrl::SetItem(&item);

				pg->state = GROUP_STATE_SHRINK;
			}
		}
		else
		{
			if (bExpand)
			{
				DoExpand(pg);

				LVITEM	item{};
				item.mask = LVIF_IMAGE;
				item.iImage = 1;
				item.iItem = GetItemIdx(pg);
				item.iSubItem = 0;
				CListCtrl::SetItem(&item);

				pg->state = GROUP_STATE_EXPAND;
			}
			else // notthing
			{
			}
		}
	}
}

void CGroupListCtrl::DoExpand(sGLI* p)
{
	const	int	nItem = GetItemIdx(p);
	if (nItem < 0)
		return;

	int	newItem = 0;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		sGLI* pg = m_list.GetNext(pos);
		if (pg->gid != p->id)
			continue;
		if (pg->type != TYPE_ITEM)
			continue;
		if (pg->state == ITEM_STATE_VISIBLE)
			continue;

		newItem = CListCtrl::InsertItem(nItem + 1, "", -1);
		CListCtrl::SetItemData(newItem, (DWORD)pg);

		for (int ii = 0; ii < pg->text.GetSize(); ii++)
		{
			CString str = pg->text.GetAt(ii);
			CListCtrl::SetItemText(newItem, ii, str);
		}

		pg->state = ITEM_STATE_VISIBLE;
	}
}

void CGroupListCtrl::SortGroup(sGLI* pg)
{
	int	nItem = 0;

	/* Group item 지운다 */
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		sGLI* p = m_list.GetNext(pos);
		if (p->type == TYPE_GROUP)
			continue;

		if (p->gid == pg->id)
		{
			nItem = GetItemIdx(p);
			CListCtrl::DeleteItem(nItem);
		}
	}

	CString	strKey;
	int	new_item = GetItemIdx(pg) + 1;
	sGLI*	p = nullptr;

	/* 매수 추가 */
	pos = m_list.GetHeadPosition();
	while (pos)
	{
		sGLI* p = m_list.GetNext(pos);
		if (p->type == TYPE_GROUP)
			continue;

		if (p->gid != pg->id)
			continue;


		strKey = p->text.GetAt(4);
		if (strKey.Find("매수") > 0)
		{
			nItem = CListCtrl::InsertItem(new_item, "", -1);
			CListCtrl::SetItemData(new_item, (DWORD)p);

			for (int ii = 0; ii < p->text.GetSize(); ii++)
			{
				CString str = p->text.GetAt(ii);
				CListCtrl::SetItemText(new_item, ii, str);
			}
			new_item++;
		}
	}

	/* 매도 추가 */
	pos = m_list.GetHeadPosition();
	while (pos)
	{
		sGLI* p = m_list.GetNext(pos);
		if (p->type == TYPE_GROUP)
			continue;

		if (p->gid != pg->id)
			continue;

		CString strKey = p->text.GetAt(4);
		if (strKey.Find("매도") > 0)
		{
			nItem = CListCtrl::InsertItem(new_item, "", -1);
			CListCtrl::SetItemData(new_item, (DWORD)p);

			for (int ii = 0; ii < p->text.GetSize(); ii++)
			{
				CString str = p->text.GetAt(ii);
				CListCtrl::SetItemText(new_item, ii, str);
			}
			new_item++;
		}
	}
}

void CGroupListCtrl::DoShrink(sGLI* p)
{
	int	nItem = 0;
	POSITION pos = m_list.GetHeadPosition();

	while (pos)
	{
		sGLI* pg = m_list.GetNext(pos);
		if (pg->gid != p->id)
			continue;

		if (pg->type != TYPE_ITEM)
			continue;

		if (pg->state == ITEM_STATE_HIDE)
			continue;

		nItem = GetItemIdx(pg);
		if (nItem >= 0)
			CListCtrl::DeleteItem(nItem);

		pg->state = ITEM_STATE_HIDE;
	}
}

void CGroupListCtrl::OnDoGroupDbclk(sGLI* p, int nItem)
{
	LVITEM item{};

	if (p->state == GROUP_STATE_EXPAND)
	{
		DoShrink(p);

		item.mask = LVIF_IMAGE;
		item.iImage = 0;
		item.iItem = nItem;
		CListCtrl::SetItem(&item);

		p->state = GROUP_STATE_SHRINK;
	}
	else
	{
		DoExpand(p);

		item.mask = LVIF_IMAGE;
		item.iImage = 1;
		item.iItem = nItem;
		CListCtrl::SetItem(&item);

		p->state = GROUP_STATE_EXPAND;
	}
}

void CGroupListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CGroupListCtrl::OnSetdispinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
}
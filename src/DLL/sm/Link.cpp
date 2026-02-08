// Link.cpp : implementation file
//

#include "stdafx.h"
#include "Link.h"
#include "tmenu.h"
#include "../../h/axisvar.h"
#include "ihutils.h"
#define COLOR_DRAG	RGB(129, 167, 229)

#define	SPLIT		'\t'
#define	SPLIT2		'\n'
#define	SEP		'-'
#define	MAP		'#'
#define REC		'$'
#define NEW		'@'
#define EDT		'^'
#define	HASCHILD	20000

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLink dialog


CLink::CLink(class CTMenu* menu, CString mapN, CWnd* pParent /*=NULL*/)
	: CExDialog(CLink::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLink)
	m_Edit = _T("");
	m_Result = _T("");
	m_editN = _T("");
	//}}AFX_DATA_INIT
	m_arItem.RemoveAll();
	m_arMap.RemoveAll();
	m_arDesc.RemoveAll();

	m_sel    = 0;
	m_cnt    = 0;
	m_count  = 0;
	m_mapN   = mapN;

	m_bDrag      = false;
	m_pDragImage = NULL;
	m_menu       = menu;

	m_lastFocusedSourceControl = LFSC_TREE;
}

CLink::~CLink()
{
	ChangeEnglishMode(this->m_hWnd);
	m_arItem.RemoveAll();
	m_arMap.RemoveAll();
	m_arDesc.RemoveAll();
}

void CLink::DoDataExchange(CDataExchange* pDX)
{
	CExDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLink)
	DDX_Control(pDX, IDC_GLIST, m_gList);
	DDX_Control(pDX, IDC_TREE, m_Tree);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT, m_Edit);
	DDX_Text(pDX, IDC_RESULT, m_Result);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLink, CDialog)
	//{{AFX_MSG_MAP(CLink)
	ON_BN_CLICKED(IDC_AFTER, OnAfter)
	ON_BN_CLICKED(IDC_BEFORE, OnBefore)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	ON_BN_CLICKED(IDC_ADDTR, OnAddTR)
	ON_BN_CLICKED(IDC_DELTR, OnDelTR)
	ON_NOTIFY(NM_DBLCLK, IDC_GLIST, OnDblclkGlist)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE, OnBegindragTree)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST, OnBegindragList)
	ON_BN_CLICKED(IDC_DELALL, OnDelall)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMovedown)
	ON_BN_CLICKED(IDC_DEFAULTLINK, OnDefaultlink)
	ON_BN_CLICKED(IDC_SAVESNAME, OnSaveSName)
	ON_NOTIFY(NM_CLICK, IDC_GLIST, OnClickGlist)
	ON_NOTIFY(NM_SETFOCUS, IDC_TREE, OnSetfocusTree)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetfocusList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLink message handlers
void CLink::OnAfter() 
{
	if (!m_cnt)	return;
	if (m_sel+1 >= m_cnt)
		m_sel = 0;
	else	m_sel++;

	setCount(m_sel, m_cnt);
	treeSelect(m_sel);
	listSelect(m_sel);
}

void CLink::OnBefore() 
{
	if (!m_cnt)	return;
	if (m_sel-1 < 0)	
		m_sel = m_cnt-1;
	else	m_sel--;

	setCount(m_sel, m_cnt);
	treeSelect(m_sel);
	listSelect(m_sel);
}

void CLink::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	const int pos = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (pos < 0 || pos >= m_arTr.GetSize())
		return;

	m_sel = pos;
	setCount(m_sel, m_cnt);
	treeSelect(m_sel);
}

void CLink::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	const int pos = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (pos < 0)	return;

	CString tmpS, trcode = m_arTr.GetAt(pos);
	LVFINDINFO pFindInfo;
	pFindInfo.flags = LVFI_STRING;
	tmpS = m_menu->GetDispN(trcode);
	pFindInfo.psz = (const char *)tmpS;
        if (m_gList.FindItem(&pFindInfo) != -1)
		return;

	CString	desc = m_List.GetItemText(pos, 1);
	const int	row = m_gList.GetItemCount();
	m_gList.InsertItem(row, tmpS);
	m_gList.SetItemText(row, 1, desc);
	fitG();
}

void CLink::OnSearch() 
{
	UpdateData(TRUE);
	CString com, comS = m_Edit;
	comS.TrimRight();
	if (comS.IsEmpty())
		return;

	m_cnt = m_sel = 0;
	UpdateData(FALSE);
	m_arItem.RemoveAll();

	HTREEITEM	item;
	if (!(item = m_Tree.GetRootItem()))
		return;

	findItem(item, comS);

	if (m_cnt == 0)
	{
		setCount();
		GotoDlgCtrl(GetDlgItem(IDC_EDIT));
		enableButton(false);
	}
	else
	{
		setCount(m_sel, m_cnt);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT));
		treeSelect(m_sel);
		enableButton(true);
	}

	setList();
}

void CLink::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	AddFromTree();
}

void CLink::AddFromTree(BOOL fromAddButton)
{
	HTREEITEM item = m_Tree.GetSelectedItem();
	if (item == NULL)	return;

	const DWORD dw = m_Tree.GetItemData(item);
	if (dw < 0 || (int) dw >= m_arMap.GetSize())
		return;

	CString data, desc, trcode, tmpS;
	trcode = m_arMap.GetAt((int) dw);
	tmpS = m_menu->GetDispN(trcode);

	LVFINDINFO pFindInfo;
	pFindInfo.flags = LVFI_STRING;
	pFindInfo.psz = (const char *)tmpS;
	if (m_gList.FindItem(&pFindInfo) == -1)
	{
		m_arDesc.Lookup(trcode, desc);
		const int row = m_gList.GetItemCount();
		m_gList.InsertItem(row, tmpS);
		m_gList.SetItemText(row, 1, desc.Mid(6,desc.GetLength()));
		fitG();
	}
}

BOOL CLink::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetDlgItem(IDC_EDIT)->m_hWnd == pMsg->hwnd)
		{
			OnSearch();

			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
		else if (GetDlgItem(IDC_EDITSNAME)->m_hWnd == pMsg->hwnd ||
			GetDlgItem(IDC_SAVESNAME)->m_hWnd == pMsg->hwnd)
		{
			OnSaveSName();
			return TRUE;
		}
		else
			pMsg->wParam = VK_TAB; 
	}
	else if (pMsg->message == WM_KEYDOWN &&
		(pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN))
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		
		if (GetDlgItem(IDC_LIST)->m_hWnd == pMsg->hwnd)
		{
			const int pos = m_List.GetNextItem(-1, LVNI_SELECTED);
			if (pos < 0)
			{
				return TRUE;
			}
			m_sel = pos;
			setCount(m_sel, m_cnt);
			treeSelect(pos);
		}
		return TRUE;
	}
	return CExDialog::PreTranslateMessage(pMsg);
}

BOOL CLink::OnInitDialog() 
{
	CExDialog::OnInitDialog();
	ChangeHangulMode(this->m_hWnd);

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_TREEICON);
	m_Tree.InitTreeImage(bitmap);

	initList();
	setTree();
	setList();
	redrawGList();
	enableButton(false);

	LoadInfo();
	
	GetDlgItem(IDC_EDIT)->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLink::initList()
{
	CString hData[2];
	hData[0].LoadString(ST_TSETUP_TEXT_TR);
	hData[1].LoadString(ST_TSETUP_TEXT_MAPNAME);
	
	CClientDC	dc(this);
	CRect		cRc;
	int		cW[2];

	m_List.GetClientRect(cRc);
	cW[0] = dc.GetTextExtent(" 9999 ").cx;
	cW[1] = cRc.Width() - cW[0] - 16;

	m_List.InitHeaderData(hData, cW, 2, LVCFMT_LEFT);
	m_List.SetFullRowSelect();
	m_List.HeaderFix();
	m_List.SetDisableSort();

	m_gList.InitHeaderData(hData, cW, 2, LVCFMT_LEFT);
	m_gList.SetFullRowSelect();
	m_gList.HeaderFix();
	m_gList.SetDisableSort();
}

void CLink::setCount(int cnt, int tot)
{
	CString tmpS = _T("");
	if (cnt == -1 && tot == -1)
		tmpS = _T("");
	else
		tmpS.Format("%d/%d", cnt+1, tot);
	UpdateData(TRUE);
	m_Result = tmpS;
	UpdateData(FALSE);
}

void CLink::treeSelect(int idx)
{
	if (idx >= m_arItem.GetSize())
		return;
	m_Tree.SelectItem(m_arItem.GetAt(idx));
}

void CLink::listSelect(int idx)
{
	if (idx >= m_List.GetItemCount())
		return;

	SCROLLINFO	info;
	CSize		sz;
	CRect		rc;

	m_List.SetItemState(idx, LVIS_SELECTED, LVIS_SELECTED);
	m_List.GetScrollInfo(SB_VERT, &info, SIF_ALL);
	const int pos = idx - (int)info.nPos;
	m_List.GetItemRect(0, rc, LVIR_BOUNDS);
	sz.cy = rc.Height()*pos;
	m_List.Scroll(sz);
}

void CLink::enableButton(bool bEnable)
{
	CButton* before = (CButton *)GetDlgItem(IDC_BEFORE);
	before->EnableWindow(bEnable);
	CButton* after = (CButton *)GetDlgItem(IDC_AFTER);
	after->EnableWindow(bEnable);
}

void CLink::fit()
{
	if (m_List.GetSafeHwnd())
	{
		int		W0{}, W1{};
		CRect		cRc;
		CClientDC	dc(this);

		m_List.GetClientRect(cRc);
		W0 = dc.GetTextExtent(" 9999 ").cx;
		W1 = cRc.Width() - W0;
		m_List.SetColumnWidth(0, W0);
		m_List.SetColumnWidth(1, W1);
	}
}

void CLink::fitG()
{
	if (m_gList.GetSafeHwnd())
	{
		int		W0{}, W1{};
		CRect		cRc;
		CClientDC	dc(this);

		m_gList.GetClientRect(cRc);
		W0 = dc.GetTextExtent(" 9999 ").cx;
		W1 = cRc.Width() - W0;
		m_gList.SetColumnWidth(0, W0);
		m_gList.SetColumnWidth(1, W1);
	}
}

void CLink::OnAddTR() 
{
	if (m_lastFocusedSourceControl == LFSC_TREE ||
		m_List.GetItemCount() == 0)
	{
		AddFromTree();
	}
	else
	{
		int	row{};
		CString data, desc, trcode, tmpS;
		LVFINDINFO pFindInfo{};

		for (int ii = 0, pos = -1; ii < (int)m_List.GetSelectedCount(); ii++)
		{
			pos = m_List.GetNextItem(pos, LVNI_SELECTED);
			trcode = m_arTr.GetAt(pos);
			tmpS = m_menu->GetDispN(trcode);
			pFindInfo.flags = LVFI_STRING;
			pFindInfo.psz = (const char *)tmpS;
			if (m_gList.FindItem(&pFindInfo) != -1)
				continue;
			desc = m_List.GetItemText(pos, 1);
			row = m_gList.GetItemCount();
			m_gList.InsertItem(row, tmpS);
			m_gList.SetItemText(row, 1, desc);
		}
		fitG();
	}
}

void CLink::OnDelTR() 
{
	int pos{}, first = -1;
	while ((pos = m_gList.GetNextItem(-1, LVNI_SELECTED)) != -1)
	{
		if (first < 0)
			first = pos;
		m_gList.DeleteItem(pos);
	}
	fitG();

	if (m_gList.GetItemCount() > 0)
	{
		if (m_gList.GetItemCount() >= first)
			first = m_gList.GetItemCount() - 1;
		if (first >= 0 && first < m_gList.GetItemCount())
		{
			m_gList.SetItemState(first, LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}

void CLink::OnDelall() 
{
	m_gList.DeleteAllItems();
	fitG();
}

void CLink::OnClickGlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	const int pos = m_gList.GetNextItem(-1, LVNI_SELECTED);
	if (pos < 0)	return;

	CString dispN = m_gList.GetItemText(pos, 0);
	m_smapN  = m_menu->GetMap(dispN);

	CString	file, sName;

	CProfile profile(pkLinkName);
	sName = profile.GetString(szSName, m_smapN);
	Trim(sName);
	if (sName.IsEmpty())
		sName = m_menu->GetSName(m_smapN);

	GetDlgItem(IDC_EDITSNAME)->SetWindowText(sName);
}

void CLink::OnDblclkGlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	const int pos = m_gList.GetNextItem(-1, LVNI_SELECTED);
	if (pos < 0)	return;
	
	m_gList.DeleteItem(pos);
	fitG();
}

void CLink::OnBegindragTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	m_hCurItem = pNMTreeView->itemNew.hItem;
	if (!m_hCurItem)	return;

	UINT uFlags{};
	HTREEITEM hItem = m_Tree.HitTest(pNMTreeView->ptDrag, &uFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		m_Tree.SelectItem(hItem);
	}

	hItem = m_Tree.GetSelectedItem();
	if ((hItem != NULL) && m_Tree.ItemHasChildren(hItem))
		return;


	POINT pt{};
	m_pDragImage = createDragImageEx(&m_Tree, &pt);
	if (m_pDragImage == NULL)	return;

	m_pDragWnd = &m_Tree;
	m_bDrag = true;

	CPoint ptStart = pNMTreeView->ptDrag;
	ptStart -= pt;
	CPoint kk = ptStart;
	CPoint jj = ptStart;

	m_Tree.ClientToScreen(&kk);
	m_Tree.ScreenToClient(&jj);
	m_pDragImage->BeginDrag(0, ptStart);
	m_pDragImage->DragEnter(GetDesktopWindow(), ptStart);

	SetCapture();
}

void CLink::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDrag)
	{
		if (m_pDragImage)
		{
			CPoint ptDropPoint(point);
			ClientToScreen(&ptDropPoint);
			m_pDragImage->DragMove(ptDropPoint);
		}
	}
	CExDialog::OnMouseMove(nFlags, point);
}

void CLink::OnLButtonUp(UINT nFlags, CPoint point) 
{
	DWORD	dw{};
	if (m_bDrag)
	{
		CRect	gRc;
		int	row{};
		CPoint	newPoint, pt(point);
		CString data, desc, trcode, tmpS;

		m_bDrag = false;
		LVFINDINFO pFindInfo{};
		::ReleaseCapture();
		ClientToScreen(&pt);
		const CWnd* pDropWnd = CWnd::WindowFromPoint(pt);
		
		if (!m_pDragImage)
		{
			m_pDragImage->DeleteImageList();
			if (m_pDragImage)
				delete m_pDragImage;
			m_pDragImage = NULL;
			m_pDragWnd = NULL;
			CExDialog::OnLButtonUp(nFlags, point);
			return;
		}

		m_pDragImage->DragLeave(GetDesktopWindow());
		m_pDragImage->EndDrag();
			
		if (m_pDragWnd == &m_List && pDropWnd == &m_gList)
		{
			m_gList.GetWindowRect(gRc);
			
			newPoint.x = pt.x - (int)gRc.left;
			newPoint.y = pt.y - (int)gRc.top;

			for (int ii = 0, pos = -1; ii < (int)m_List.GetSelectedCount(); ii++)
			{
				pos = m_List.GetNextItem(pos, LVNI_SELECTED);
				trcode = m_arTr.GetAt(pos);
				tmpS = m_menu->GetDispN(trcode);
				pFindInfo.flags = LVFI_STRING;
				pFindInfo.psz = (const char *)tmpS;
				if (m_gList.FindItem(&pFindInfo) != -1)
					continue;
				desc = m_List.GetItemText(pos, 1);
				row = m_gList.HitTest(newPoint);
				if (row == -1)	row = m_gList.GetItemCount();
				m_gList.InsertItem(row, tmpS);
				m_gList.SetItemText(row, 1, desc);
			}
		}
		else if (m_pDragWnd == &m_Tree && pDropWnd == &m_gList)
		{
			HTREEITEM item = m_Tree.GetSelectedItem();
			if (item == NULL)
			{
				m_pDragImage->DeleteImageList();
				if (m_pDragImage)
					delete m_pDragImage;
				m_pDragImage = NULL;
				m_pDragWnd = NULL;
				CExDialog::OnLButtonUp(nFlags, point);
				return;
			}
			
			m_gList.GetWindowRect(gRc);
			newPoint.x = pt.x - (int)gRc.left;
			newPoint.y = pt.y - (int)gRc.top;

			dw = m_Tree.GetItemData(item);
			if (dw < 0 || (int) dw >= m_arMap.GetSize())
				return;
			trcode = m_arMap.GetAt((int) dw);
			tmpS = m_menu->GetDispN(trcode);
			pFindInfo.flags = LVFI_STRING;
			pFindInfo.psz = (const char *)tmpS;
			if (m_gList.FindItem(&pFindInfo) == -1)
			{
				desc = m_Tree.GetItemText(item);
				row = m_gList.HitTest(newPoint);
				if (row == -1)	row = m_gList.GetItemCount();
				m_gList.InsertItem(row, tmpS);
				m_gList.SetItemText(row, 1, desc);
			}
		}

		m_pDragImage->DeleteImageList();
		if (m_pDragImage)	delete m_pDragImage;
		m_pDragImage = NULL;
		m_pDragWnd = NULL;

		fitG();
	}
	CExDialog::OnLButtonUp(nFlags, point);
}

void CLink::OnBegindragList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	if (m_List.GetSelectedCount() <= 0)
		return; 

	POINT pt{};
 	m_pDragImage = createDragImageEx(&m_List, &pt);
	if (m_pDragImage == NULL)
		return;

	m_pDragWnd = &m_List;
	m_bDrag = true;

	CPoint ptStart = pNMListView->ptAction;
	ptStart -= pt;
	m_pDragImage->BeginDrag(0, ptStart);
	m_pDragImage->DragEnter(GetDesktopWindow(), ptStart);
	SetCapture();

	*pResult = 0;
}

CImageList* CLink::createDragImageEx(CListCtrl *pList, LPPOINT lpPoint)
{
	if (pList->GetSelectedCount() <= 0)
		return NULL;

	CRect rS;
	CRect rC(0,0,0,0);

	pList->GetClientRect(rS);
	const int nWidth  = rS.Width();

	int nIndex = pList->GetTopIndex() - 1;
	int nBottomIndex = pList->GetTopIndex() + pList->GetCountPerPage() - 1;
	if (nBottomIndex > (pList->GetItemCount() - 1))
		nBottomIndex = pList->GetItemCount() - 1;

	while ((nIndex = pList->GetNextItem(nIndex, LVNI_SELECTED)) != -1)
	{
		if (nIndex > nBottomIndex)	break; 
		pList->GetItemRect(nIndex, rS, LVIR_BOUNDS);
		if (rS.left < 0) rS.left = 0;
		if (rS.right > nWidth)	rS.right = nWidth;
		rC.UnionRect(rC, rS);
	}
		
	CBitmap bitmap;	CDC dcMem;
	CClientDC dcClient(this);

	if (!dcMem.CreateCompatibleDC(&dcClient))
		return NULL;

	if (!bitmap.CreateCompatibleBitmap(&dcClient, rC.Width(), rC.Height()))
		return NULL;

	CBitmap *pOldMemDCBitmap = dcMem.SelectObject(&bitmap);
 	dcMem.FillSolidRect(0, 0, rC.Width(), rC.Height(), COLOR_DRAG);

	nIndex = pList->GetTopIndex() - 1;
	while ((nIndex = pList->GetNextItem(nIndex, LVNI_SELECTED)) != -1)
	{	
		if (nIndex > nBottomIndex)	return NULL;

		CPoint pt;
		CImageList* pSingleImageList = pList->CreateDragImage(nIndex, &pt);
		if (pSingleImageList)
		{
			pList->GetItemRect(nIndex, rS, LVIR_BOUNDS);
			pSingleImageList->Draw(&dcMem, 0, 
				CPoint(rS.left - rC.left, 
				rS.top - rC.top), ILD_MASK);
			pSingleImageList->DeleteImageList();
			delete pSingleImageList;
		}
	}

 	dcMem.SelectObject(pOldMemDCBitmap);

	std::unique_ptr< CImageList> pCompleteImageList = std::make_unique< CImageList>();
	pCompleteImageList->Create(rC.Width(), rC.Height(), ILC_COLOR | ILC_MASK, 0, 1);

	pCompleteImageList->Add(&bitmap, RGB(0 , 255, 0)); 
	bitmap.DeleteObject();

	if (lpPoint)
	{
		lpPoint->x = rC.left;
		lpPoint->y = rC.top;
	}

	return pCompleteImageList.release();
}

CImageList* CLink::createDragImageEx(CTreeCtrl *pTree, LPPOINT lpPoint)
{
	HTREEITEM item = pTree->GetSelectedItem();
	if (!item)	return NULL;
	const int	gap = 20;

	CRect	rS;
	CRect	rC(0,0,0,0);

	pTree->GetClientRect(rS);
	pTree->GetItemRect(item, rC, TRUE);
	rC.left -= gap;
	
	CBitmap bitmap;	CDC dcMem;
	CClientDC dcClient(this);

	if (!dcMem.CreateCompatibleDC(&dcClient))	return NULL;

	if (!bitmap.CreateCompatibleBitmap(&dcClient, rC.Width(), rC.Height()))
		return NULL;

	CBitmap *pOldMemDCBitmap = dcMem.SelectObject(&bitmap);
 	dcMem.FillSolidRect(0, 0, rC.Width(), rC.Height(), COLOR_DRAG);

	CImageList* pSingleImageList = pTree->CreateDragImage(item);
	if (pSingleImageList)
	{
		pTree->GetItemRect(item, rS, TRUE);
		rS.left -= gap;
		pSingleImageList->Draw(&dcMem, 0, CPoint(rS.left-rC.left, rS.top-rC.top), ILD_MASK);
		pSingleImageList->DeleteImageList();
		delete pSingleImageList;
	}

 	dcMem.SelectObject(pOldMemDCBitmap);
#ifdef DF_USESTL_LINK
	std::unique_ptr<CImageList> pCompleteImageList =std::make_unique<CImageList>() ;
#else
	CImageList* pCompleteImageList = new CImageList;
#endif

	pCompleteImageList->Create(rC.Width(), rC.Height(), ILC_COLOR | ILC_MASK, 0, 1);

	pCompleteImageList->Add(&bitmap, RGB(0, 255, 0)); 
	bitmap.DeleteObject();

	if (lpPoint)
	{
		lpPoint->x = rC.left;
		lpPoint->y = rC.top;
	}

#ifdef DF_USESTL_LINK
	return pCompleteImageList.release();
#else
	return pCompleteImageList;
#endif

}

void CLink::LoadInfo()
{
	CProfile profile(Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, m_mapN.Left(4)));

	m_gList.DeleteAllItems();

	CString buf(profile.GetSectionLF(m_mapN));

	CString	str, mapN;
	int dw = buf.GetLength();

	if (dw > 0)
	{
		for (int ii = 0; ii < (int) dw; ii++)
		{
			str += buf[ii];
			if (buf[ii] != '\n')	continue;
	 		int	pos = str.Find('=');
			if (pos == -1)		continue;
			mapN = str.Left(pos++);
			str = str.Mid(pos);
			int	row;// = m_gList.GetItemCount();
			row = m_gList.InsertItem(0, m_menu->GetDispN(mapN));
			str.Remove(_T(13)); str.Remove(_T(10));
			m_gList.SetItemText(row, 1, str.Mid(5));
			str.Empty();
		}

		fitG();
	}
	else
	{
		profile.SetFileName(Format("%s\\%s\\%s", Axis::home, MTBLDIR, m_mapN.Left(4)));
		buf = profile.GetSectionLF(m_mapN);
		dw = buf.GetLength();

		if (dw > 0)
		{
			for (int ii = 0; ii < (int) dw; ii++)
			{
				str += buf[ii];
				if (buf[ii] != '\n')	continue;
	 			int	pos = str.Find('=');
				if (pos == -1)		continue;
				mapN = str.Left(pos++);
				str = str.Mid(pos);
				int	row;// = m_gList.GetItemCount();
				row = m_gList.InsertItem(0, m_menu->GetDispN(mapN));
				str.Remove(_T(13)); str.Remove(_T(10));
				m_gList.SetItemText(row, 1, str.Mid(5));
				str.Empty();
			}

			fitG();
		}
	}

	str.Format(" [%s] %s", m_menu->GetDispN(m_mapN), m_menu->GetDesc(m_mapN));
	GetDlgItem(IDC_MAPN)->SetWindowText(str);

	profile.SetFileName(pkLinkBar);
	if (!profile.GetInt(szShowBar, m_mapN))
		((CButton*)GetDlgItem(IDC_USELINKBAR))->SetCheck(TRUE);

	/**
	profile.SetFileName(pkLinkName);
	if (!profile.GetInt(szSName, "DISP", 1))
	if (!profile.GetInt(szSName, "DISP2", 0))
		((CButton*)GetDlgItem(IDC_DISPSNAME))->SetCheck(TRUE);
	**/
}

void CLink::SaveInfo()
{
	CProfile profile(Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, m_mapN.Left(4)));
	CString	keys, head, value;
	const int tot = m_gList.GetItemCount();
	
	profile.WriteSection(m_mapN, "");
	
	for (int ii = tot - 1 ; ii >= 0; ii--)
	{
		keys = m_menu->GetMap(m_gList.GetItemText(ii, 0));
		value.Format("%s %s", m_menu->GetDispN(keys), m_gList.GetItemText(ii, 1));
		profile.Write(m_mapN, keys, value);
	}

	profile.SetFileName(pkLinkBar);
	profile.Write(szShowBar, m_mapN, !((CButton*)GetDlgItem(IDC_USELINKBAR))->GetCheck());

	/**
	profile.SetFileName(pkLinkName);
	profile.Write(szSName, "DISPN", !((CButton*)GetDlgItem(IDC_DISPSNAME))->GetCheck());
	profile.Write(szSName, "DISPN2", !((CButton*)GetDlgItem(IDC_DISPSNAME))->GetCheck());
	**/
}

void CLink::redrawGList()
{
	CString key = _T("");

	m_gList.DeleteAllItems();
	m_gList.FreePtr();

	fitG();
}

void CLink::OnMoveup() 
{
	if (m_gList.GetSelectedCount() != 1)
		return; 

	int sel = -1;
	CString moveItem1, moveItem2, itemData = _T("");

	sel = m_gList.GetNextItem(-1, LVNI_SELECTED);
	if (sel == 0)	return;
	moveItem1 = m_gList.GetItemText(sel, 0);
	moveItem2 = m_gList.GetItemText(sel, 1);
	m_gList.DeleteItem(sel);

	sel--;
	m_gList.InsertItem(sel, moveItem1);
	m_gList.SetItemText(sel, 1, moveItem2);
	m_gList.SetItemState(sel, LVIS_SELECTED, LVIS_SELECTED);
}

void CLink::OnMovedown() 
{
	if (m_gList.GetSelectedCount() != 1)
		return; 

	int sel = -1;
	CString moveItem1, moveItem2, itemData = _T("");

	sel = m_gList.GetNextItem(-1, LVNI_SELECTED);
	if (sel == m_gList.GetItemCount()-1)	return;

	moveItem1 = m_gList.GetItemText(sel, 0);
	moveItem2 = m_gList.GetItemText(sel, 1);
	m_gList.DeleteItem(sel);

	sel++;
	m_gList.InsertItem(sel, moveItem1);
	m_gList.SetItemText(sel, 1, moveItem2);
	m_gList.SetItemState(sel, LVIS_SELECTED, LVIS_SELECTED);
}

void CLink::OnOK() 
{
	SaveInfo();
	CExDialog::OnOK();
}

void CLink::setTree()
{
	m_Tree.DeleteAllItems();

	CString dat, file(GetProfileFileName(pkAxisMenu));
	char	buf[1024]{};

	FILE *fp = fopen(file, "r");
	if (!fp)	return;

	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		dat = buf;
		if (dat.Find("www#") != -1)
			continue;
		make_Tree(dat);
	}
	fclose(fp);
}

void CLink::setList(BOOL flag)
{
	UpdateData(TRUE);
	CString desc, com, comS = m_Edit;
	UpdateData(FALSE);

	m_List.DeleteAllItems();
	m_arTr.RemoveAll();
	comS.TrimRight();
	for (int ii = 0; ii < m_arMap.GetSize() && !comS.IsEmpty(); ii++)
	{
		com = m_arMap.GetAt(ii);
		m_arDesc.Lookup(com, desc);
		if (desc.Find(comS) == -1)	continue;
		if (com.Find("#") != -1)	continue;
		
		const int row = m_List.GetItemCount();

		m_arTr.Add(com);
		m_List.InsertItem(row, m_menu->GetDispN(com));
		m_List.SetItemText(row, 1, desc.Mid(6,desc.GetLength()-5));
	}

	if (m_List.GetItemCount())
	{
		m_List.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
		m_List.SetScrollPos(SB_VERT, 0, TRUE);
	}

	if (!flag)	fit();
}

void CLink::make_Tree(CString dat)
{
	int	treeN{}, step = 0;
	BOOL	map{}, sep{};
	CString	str, desc, scrnum;
	
	HTREEITEM item{};
	map = sep = FALSE;
	while (!dat.IsEmpty())
	{
		int pos = dat.Find(SPLIT);
		if (pos == -1)	break;
	
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
		
		str.TrimRight();
		
		if (str.IsEmpty())	continue;

		const char	ch = str.GetAt(0);
		switch (ch)
		{
		case SEP:	continue;
		case MAP:	map = TRUE;	break;
		case REC:	map = TRUE;	break;
		case NEW:	map = TRUE;	break;
		case EDT:	map = TRUE;	break;
		default:			break;
		}
		if (map)	str = str.Mid(1);

		switch (step)
		{
		case 0:
			if (!m_treeN[step].Compare(str))
			{
				step++;
				continue;
			}
			item = m_Tree.InsertItem(str, 0, 1);
			m_Tree.SetItemData(item, (DWORD) HASCHILD);

			m_itemN[step]  = item;
			m_treeN[step++] = str;
			for (treeN = step; treeN < MAX; treeN++)
			{
				m_itemN[treeN] = NULL;
				m_treeN[treeN].Empty();
			}
			break;
		default:
			if (!m_itemN[step-1])	return;
			if (map)
			{
/////////////////////////////////////////////////////////////////////////////
//	 특정사용자에게만 메뉴 오픈
// 2006 11 08
				/**
				CMapStringToString	mapList;
				int	ncnt = IH::GetUnitInList(mapList, str, ",", "{", "}");
				if (ncnt)
				{
					CString	sztmp = _T("");
					if (!mapList.Lookup(m_id, sztmp))
						return;
					int	nFind = str.Find("}");
					if (nFind >= 0)
						str = str.Mid(nFind + 1);
				}
				**/
/////////////////////////////////////////////////////////////////////////////
				//asdfasdf
				scrnum = dat.Mid(2,4);

				desc = "["+scrnum+"]"+str;
				
				item = m_Tree.InsertItem(desc, 2, 3, m_itemN[step-1]);
				m_Tree.SetItemData(item, (DWORD) m_count++);
				if (!dat.IsEmpty())
				{
					int pos = dat.Find(SPLIT);
					const int pos2 = dat.Find(SPLIT2);
					if (pos == -1 && pos2 == -1)
						str.Empty();
					else
					{
						if (pos == -1)	pos = pos2;
						str = dat.Left(pos++);
						dat = dat.Mid(pos);
					}
				}
				m_arMap.Add(str);
				m_arDesc.SetAt(str, desc);
				return;
			}

			if (!m_treeN[step].Compare(str))
			{
				step++;
				continue;
			}
			item = m_Tree.InsertItem(str, 0, 1, m_itemN[step-1]);
			m_Tree.SetItemData(item, (DWORD) HASCHILD);

			m_itemN[step]  = item;
			m_treeN[step++] = str;
			for (treeN = step; treeN < MAX; treeN++)
			{
				m_itemN[treeN] = NULL;
				m_treeN[treeN].Empty();
			}
			break;
		}
	}
}

void CLink::findItem(HTREEITEM item, CString comS)
{
	CString		com;
	HTREEITEM	sItem{};

	do
	{
		com = m_Tree.GetItemText(item);

		if (com.Find(comS) != -1)
		{
			if (!m_Tree.ItemHasChildren(item))
			{
				m_arItem.Add(item);
				m_cnt++;
			}
		}

		if (!m_Tree.ItemHasChildren(item))
			continue;
		sItem = m_Tree.GetNextItem(item, TVGN_CHILD);
		findItem(sItem,  comS);

	} while ((item = m_Tree.GetNextItem(item, TVGN_NEXT)) != NULL);
}

void CLink::OnDefaultlink() 
{
	CProfile profile(Format("%s\\%s\\%s", Axis::home, MTBLDIR, m_mapN.Left(4)));
	CString	str, mapN, buf;

	m_gList.DeleteAllItems();

	buf = profile.GetSectionLF(m_mapN);
	const int dw = buf.GetLength();
	
	if (dw > 0)
	{
		for (int ii = 0; ii < (int) dw; ii++)
		{
			str += buf[ii];
			if (buf[ii] != '\n')	continue;
	 		int	pos = str.Find('=');
			if (pos == -1)		continue;
			mapN = str.Left(pos++);
			str = str.Mid(pos);
			const int	row = m_gList.GetItemCount();
			m_gList.InsertItem(row, mapN.Mid(2, 4));
			m_gList.SetItemText(row, 1, str.Mid(5));
			str.Empty();
		}

		fitG();
	}	
}

void CLink::OnSaveSName() 
{
	const int pos = m_gList.GetNextItem(-1, LVNI_SELECTED);
	if (pos < 0)
	{
		m_smapN.Empty();
		return;
	}
	
	if (m_smapN.IsEmpty())	return;

	CProfile profile(pkLinkName);
	CString	sName;

	GetDlgItem(IDC_EDITSNAME)->GetWindowText(sName);
	profile.Write(szSName, m_smapN, sName);
}

void CLink::OnSetfocusTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_lastFocusedSourceControl = LFSC_TREE;
	
	*pResult = 0;
}

void CLink::OnSetfocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_lastFocusedSourceControl = LFSC_LIST;
	
	*pResult = 0;
}

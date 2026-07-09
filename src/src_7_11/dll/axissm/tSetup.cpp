// tSetup.cpp : implementation file
//

#include "stdafx.h"
#include "tSetup.h"
#include "imm.h"
#include "tmenu.h"

#include "../../h/axisvar.h"

#define COLOR_DRAG	RGB(255, 255, 255)//RGB(129, 167, 229)
#define	MAX_GROUP	20
#define MAX_SUBGROUP	100

#define	SPLIT		'\t'
#define	SPLIT2		'\n'
#define	SEP		'-'
#define	MAP		'#'
#define	HASCHILD	20000

#define WM_TNOTIFY	WM_USER+33
#define MOVECURSOR	MAKEINTRESOURCE(32649)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTSetup dialog


CTSetup::CTSetup(class CTMenu* menu, CString home, CString user, CWnd* pParent /*=NULL*/)
	: CSetupDialog(CTSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTSetup)
	m_Edit = _T("");
	m_Result = _T("");
	m_editN = _T("");
	//}}AFX_DATA_INIT
	m_caption = "사용자툴바 설정";

	m_arItem.RemoveAll();
	m_sel = 0;
	m_uIndex = 0;
	
	m_home = home;
	m_user = user;

	m_mGroup.RemoveAll();
	m_subGroup.RemoveAll();

	m_cnt = 0;
	m_count = 0;
	m_bDrag = false;
	m_pDragImage = NULL;
	m_menu = menu;
}

CTSetup::~CTSetup()
{
	ChangeEnglishMode(this->m_hWnd);

	POSITION pos = NULL;
	CString key = _T("");
	_Group*	pGroup = (_Group *)NULL;
	for(pos = m_mGroup.GetStartPosition(); pos != NULL; )
	{
		m_mGroup.GetNextAssoc(pos, key, (CObject*&)pGroup);
		delete pGroup;
	}
	m_mGroup.RemoveAll();

	_subGroup* subGroup = (_subGroup *)NULL;
	for(pos = m_subGroup.GetStartPosition(); pos != NULL; )
	{
		m_subGroup.GetNextAssoc(pos, key, (CObject*&)subGroup);
		delete subGroup;
	}
	m_subGroup.RemoveAll();

	m_arItem.RemoveAll();
	m_arMap.RemoveAll();
	m_arDesc.RemoveAll();
	m_arTr.RemoveAll();
	m_gName.RemoveAll();
}

void CTSetup::DoDataExchange(CDataExchange* pDX)
{
	CSetupDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTSetup)
	DDX_Control(pDX, IDC_USERTREE, m_usrtree);
	DDX_Control(pDX, IDC_GNAME, m_Combo);
	DDX_Control(pDX, IDC_TREE, m_Tree);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT, m_Edit);
	DDX_Text(pDX, IDC_RESULT, m_Result);
	DDX_Text(pDX, IDC_ENAME, m_editN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTSetup, CSetupDialog)
	//{{AFX_MSG_MAP(CTSetup)
	ON_BN_CLICKED(IDC_AFTER, OnAfter)
	ON_BN_CLICKED(IDC_BEFORE, OnBefore)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	ON_BN_CLICKED(IDC_ADDTR, OnAddTR)
	ON_BN_CLICKED(IDC_DELTR, OnDelTR)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE, OnBegindragTree)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST, OnBegindragList)
	ON_CBN_SELCHANGE(IDC_GNAME, OnSelchangeGname)
	ON_BN_CLICKED(IDC_DELALL, OnDelall)
	ON_BN_CLICKED(IDC_ADDGROUP, OnAddgroup)
	ON_BN_CLICKED(IDC_DELGROUP, OnDelgroup)
	ON_BN_CLICKED(IDC_CHANGEGROUPNAME, OnChangegroupname)
	ON_BN_CLICKED(IDC_TOOLINIT, OnToolinit)
	ON_BN_CLICKED(IDC_ADDSUBGROUP, OnAddsubgroup)
	ON_NOTIFY(NM_DBLCLK, IDC_USERTREE, OnDblclkUsertree)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_USERTREE, OnBegindragUsertree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_USERTREE, OnChangeSubName)
	ON_BN_CLICKED(IDC_CHANGESUBGROUP, OnChangesubgroup)
	ON_BN_CLICKED(IDC_DELSUBGROUP, OnDelsubgroup)
	ON_BN_CLICKED(IDC_TOOLVIEW, OnToolview)
	ON_BN_CLICKED(IDC_UPMOVE, &CTSetup::OnBnClickedUpmove)
	ON_BN_CLICKED(IDC_DOWNMOVE, &CTSetup::OnBnClickedDownmove)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TNOTIFY, OnUSER)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTSetup message handlers

void CTSetup::OnAfter() 
{
	if (!m_cnt)	return;
	if (m_sel+1 >= m_cnt)
		m_sel = 0;
	else	m_sel++;

	setCount(m_sel, m_cnt);
	treeSelect(m_sel);
	listSelect(m_sel);
}

void CTSetup::OnBefore() 
{
	if (!m_cnt)	return;
	if (m_sel-1 < 0)	
		m_sel = m_cnt-1;
	else	m_sel--;

	setCount(m_sel, m_cnt);
	treeSelect(m_sel);
	listSelect(m_sel);
}

void CTSetup::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	int pos = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (pos < 0 || pos >= m_arTr.GetSize())
		return;

	m_sel = pos;
	setCount(m_sel, m_cnt);
	treeSelect(m_sel);
}

void CTSetup::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	if (m_Combo.GetCount() <= 0)
	{
		SetGuide(ST_MSG_TOOL_NOGROUP);
		return;
	}
	int pos = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (pos < 0)	return;

	AddTreeItem(m_arTr.GetAt(pos));
}

void CTSetup::OnSearch() 
{
	UpdateData(TRUE);
	CString com, comS = m_Edit;
	comS.TrimRight();
	if (comS.IsEmpty())
		return;
	
	m_cnt = m_sel = 0;
	UpdateData(FALSE);
	m_arItem.RemoveAll();

	HTREEITEM item = m_Tree.GetRootItem();
	if (!item)	return;

	findItem(item, comS);
	if (!m_cnt)
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

void CTSetup::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	if (m_Combo.GetCount() <= 0)
	{
		SetGuide(ST_MSG_TOOL_NOGROUP);
		return;
	}

	HTREEITEM item = m_Tree.GetSelectedItem();
	if (item)
	{
		DWORD_PTR dw = m_Tree.GetItemData(item);
		if (dw < 0 || (int) dw >= m_arMap.GetSize())
			return;
		AddTreeItem(m_arMap.GetAt((int) dw));
	}
}

BOOL CTSetup::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CEdit* edit = m_usrtree.GetEditControl();
		if (edit && edit->m_hWnd == pMsg->hwnd)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
		else if (GetDlgItem(IDC_EDIT)->m_hWnd == pMsg->hwnd)
		{
			OnSearch();

			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
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
			int pos = m_List.GetNextItem(-1, LVNI_SELECTED);
			if (pos < 0)	return TRUE;
			m_sel = pos;
			treeSelect(pos);
		}
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		CEdit* edit = m_usrtree.GetEditControl();
		if (edit && edit->m_hWnd == pMsg->hwnd)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
		else
			pMsg->wParam = VK_TAB;
	}
	
	return CSetupDialog::PreTranslateMessage(pMsg);
}

BOOL CTSetup::OnInitDialog() 
{
	CSetupDialog::OnInitDialog();
	ChangeHangulMode(this->m_hWnd);
	
	CBitmap bmp1, bmp2;
	bmp1.LoadBitmap(IDB_TREEICON);
	bmp2.LoadBitmap(IDB_TREEICON);
	m_Tree.InitTreeImage(bmp1);
	m_usrtree.InitTreeImage(bmp2);

	initList();
	setTree();
	setList();
	enableButton(false);

	LoadInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTSetup::initList()
{
	CString		hData[2];
	CClientDC	dc(this);
	CRect		cRc;
	int		cW[2];

	m_List.GetClientRect(cRc);
	cW[0] = dc.GetTextExtent(" 9999 ").cx;
	cW[1] = cRc.Width() - cW[0];
	hData[0].LoadString(ST_TSETUP_TEXT_TR);
	hData[1].LoadString(ST_TSETUP_TEXT_MAPNAME);

	m_List.InitHeaderData(hData, cW, 2, LVCFMT_LEFT);
	m_List.SetFullRowSelect();
	m_List.HeaderFix();
	m_List.SetDisableSort();
}

void CTSetup::setCount(int cnt, int tot)
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

void CTSetup::treeSelect(int idx)
{
	if (idx >= m_arItem.GetSize())
		return;
	m_Tree.SelectItem(m_arItem.GetAt(idx));
}

void CTSetup::listSelect(int idx)
{
	if (idx >= m_List.GetItemCount())
		return;

	SCROLLINFO	info;
	CSize		sz;
	CRect		rc;

	m_List.SetItemState(idx, LVIS_SELECTED, LVIS_SELECTED);
	m_List.GetScrollInfo(SB_VERT, &info, SIF_ALL);
	int pos = idx - (int)info.nPos;
	m_List.GetItemRect(0, rc, LVIR_BOUNDS);
	sz.cy = rc.Height()*pos;
	m_List.Scroll(sz);
}

void CTSetup::enableButton(bool bEnable)
{
	CButton* before = (CButton *)GetDlgItem(IDC_BEFORE);
	before->EnableWindow(bEnable);
	CButton* after = (CButton *)GetDlgItem(IDC_AFTER);
	after->EnableWindow(bEnable);
}

void CTSetup::fit()
{
	if (m_List.GetSafeHwnd())
	{
		int		W0, W1;
		CRect		cRc;
		CClientDC	dc(this);

		m_List.GetClientRect(cRc);
		W0 = dc.GetTextExtent(" 9999 ").cx;
		W1 = cRc.Width() - W0;
		m_List.SetColumnWidth(0, W0);
		m_List.SetColumnWidth(1, W1);
	}
}

void CTSetup::OnAddTR() 
{
	if (m_Combo.GetCount() <= 0)
	{
		SetGuide(ST_MSG_TOOL_NOGROUP);
		return;
	}

	HTREEITEM item = m_Tree.GetSelectedItem();
	if (item)
	{
		DWORD_PTR dw = m_Tree.GetItemData(item);
		if (dw < 0 || (int) dw >= m_arMap.GetSize())
			return;

		AddTreeItem(m_arMap.GetAt((int)dw));
	}
}

void CTSetup::OnDelTR() 
{
	if (m_Combo.GetCount() <= 0)
	{
		SetGuide(ST_MSG_TOOL_NOGROUP);
		return;
	}

	HTREEITEM item = m_usrtree.GetSelectedItem();
	if (item)
	{
		CString	keys = GetSubKey(item);
		if (!keys.IsEmpty())
		{
			_subGroup* subGroup = NULL;
			m_subGroup.Lookup(keys, (CObject*&)subGroup);
			if (subGroup)	delete subGroup;
			m_subGroup.RemoveKey(keys);
		}
		m_usrtree.DeleteItem(item);
		SaveTreeData();
	}
}

void CTSetup::OnDelall() 
{
	if (m_Combo.GetCount() <= 0)
	{
		SetGuide(ST_MSG_TOOL_NOGROUP);
		return;
	}

	_Group* pGroup = GetCurGroup();
	for (int ii = 0; ii < pGroup->m_pGroup.GetSize(); ii++)
	{
		pGroup->gData = pGroup->m_pGroup.GetAt(ii);
		if (pGroup->gData.sGroup == SUBGROUPEXIST)
		{
			CString		subKey = pGroup->gData.Code;
			_subGroup*	subGroup = NULL;
			m_subGroup.Lookup(subKey, (CObject*&)subGroup);
			if (subGroup)	delete subGroup;
			m_subGroup.RemoveKey(subKey);
		}
	}

	m_usrtree.DeleteAllItems();
	SaveTreeData();
}

void CTSetup::OnBegindragTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pTView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	if (m_Combo.GetCount() <= 0)
	{
		SetGuide(ST_MSG_TOOL_NOGROUP);
		return;
	}
	if (!pTView->itemNew.hItem)	return;

	UINT uFlags;
	HTREEITEM hItem = m_Tree.HitTest(pTView->ptDrag, &uFlags);
	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
		m_Tree.SelectItem(hItem);

	hItem = m_Tree.GetSelectedItem();
	if (hItem && m_Tree.ItemHasChildren(hItem))
		return;

	POINT pt;
	m_pDragImage = createDragImageEx(&m_Tree, &pt);
	if (!m_pDragImage)	return;

	m_pDragWnd = &m_Tree;
	m_bDrag = true;

	CPoint ptStart = pTView->ptDrag;
	ptStart -= pt;
	CPoint kk = ptStart;
	CPoint jj = ptStart;

	m_Tree.ClientToScreen(&kk);
	m_Tree.ScreenToClient(&jj);
	m_pDragImage->BeginDrag(0, ptStart);
	m_pDragImage->DragEnter(GetDesktopWindow(), ptStart);

	SetCapture();
}

void CTSetup::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDrag)
	{
		CPoint	newPoint, pt(point);
		CRect	gRc;
		if (m_pDragImage)
		{
			ClientToScreen(&pt);
			m_pDragImage->DragMove(pt);
			CWnd* pDropWnd = CWnd::WindowFromPoint(pt);
			if (&m_usrtree == pDropWnd)
			{
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				/*m_usrtree.GetWindowRect(gRc);
				newPoint.x = pt.x - (int)gRc.left;
				newPoint.y = pt.y - (int)gRc.top;
				HTREEITEM hItem = m_usrtree.HitTest(newPoint);
				m_usrtree.SelectDropTarget(hItem);*/
			}
			else	SetCursor(LoadCursor(NULL, IDC_NO));
		}
		else if (m_pDragWnd == &m_usrtree)
		{
			ClientToScreen(&pt);
			CWnd* pDropWnd = CWnd::WindowFromPoint(pt);
			if (&m_usrtree == pDropWnd)
			{
				SetCursor(LoadCursor(NULL, MOVECURSOR));
				m_usrtree.GetWindowRect(gRc);
				newPoint.x = pt.x - (int)gRc.left;
				newPoint.y = pt.y - (int)gRc.top;
				HTREEITEM hItem = m_usrtree.HitTest(newPoint);
				m_usrtree.SelectDropTarget(hItem);
			}
			else	SetCursor(LoadCursor(NULL, IDC_NO));
		}
	}

	CSetupDialog::OnMouseMove(nFlags, point);
}

void CTSetup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDrag)
	{
		UINT	uFlags;
		CPoint	newPoint, pt(point);

		m_bDrag = false;
		::ReleaseCapture();
		ClientToScreen(&pt);
		CWnd* pDropWnd = CWnd::WindowFromPoint(pt);
		
		m_pDragImage->DragLeave(GetDesktopWindow());
		m_pDragImage->EndDrag();
		if (pDropWnd == &m_usrtree)
		{
			CRect		gRc;
			DWORD_PTR	dw;
			HTREEITEM	hItem;

			m_usrtree.GetWindowRect(gRc);
			newPoint.x = pt.x - (int)gRc.left;
			newPoint.y = pt.y - (int)gRc.top;
			
			if (m_pDragWnd == &m_List)
			{
				dw = m_List.GetNextItem(-1, LVNI_SELECTED);
				hItem = m_usrtree.HitTest(newPoint);
				AddTreeItem(m_arTr.GetAt(dw), hItem);
			}
			else if (m_pDragWnd == &m_Tree)
			{
				HTREEITEM item = m_Tree.GetSelectedItem();
				
				dw = m_Tree.GetItemData(item);
				if (dw < 0 || (int) dw >= m_arMap.GetSize())
					return;
				hItem = m_usrtree.HitTest(newPoint, &uFlags);
				AddTreeItem(m_arMap.GetAt((int) dw), hItem);
			}
			else if (m_pDragWnd == &m_usrtree)
			{
				hItem = m_usrtree.GetDropHilightItem();
				MoveItem(m_hCurItem, hItem);
			}
		}
		m_usrtree.SelectDropTarget(NULL);
		
		if (m_pDragImage)
		{
			m_pDragImage->DeleteImageList();
			delete m_pDragImage;
		}
		m_pDragImage = NULL;	m_pDragWnd = NULL;
	}

	CSetupDialog::OnLButtonUp(nFlags, point);
}

void CTSetup::OnBegindragList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	if (m_Combo.GetCount() <= 0)
	{
		SetGuide(ST_MSG_TOOL_NOGROUP);
		return;
	}

	POINT pt;
 	m_pDragImage = createDragImageEx(&m_List, &pt);
	if (!m_pDragImage)	return;

	m_pDragWnd = &m_List;
	m_bDrag = true;

	CPoint ptStart = pNMListView->ptAction;
	ptStart -= pt;
	m_pDragImage->BeginDrag(0, ptStart);
	m_pDragImage->DragEnter(GetDesktopWindow(), ptStart);
	SetCapture();
}

CImageList* CTSetup::createDragImageEx(CListCtrl *pList, LPPOINT lpPoint)
{
	if (pList->GetSelectedCount() <= 0)
		return NULL;

	CRect rS;
	CRect rC(0,0,0,0);

	pList->GetClientRect(rS);
	int nWidth  = rS.Width();

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
	CImageList* pCompleteImageList = new CImageList;
	pCompleteImageList->Create(rC.Width(), rC.Height(), ILC_COLOR | ILC_MASK, 0, 1);

	pCompleteImageList->Add(&bitmap, RGB(0 , 255, 0)); 
	bitmap.DeleteObject();

	if (lpPoint)
	{
		lpPoint->x = rC.left;
		lpPoint->y = rC.top;
	}

	return pCompleteImageList;
}

CImageList* CTSetup::createDragImageEx(CTreeCtrl *pTree, LPPOINT lpPoint)
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
	CImageList* pCompleteImageList = new CImageList;
	pCompleteImageList->Create(rC.Width(), rC.Height(), ILC_COLOR | ILC_MASK, 0, 1);

	pCompleteImageList->Add(&bitmap, RGB(0, 255, 0)); 
	bitmap.DeleteObject();

	if (lpPoint)
	{
		lpPoint->x = rC.left;
		lpPoint->y = rC.top;
	}

	return pCompleteImageList;
}

void CTSetup::LoadInfo(CString setupfile)
{
	const int	bufSize = 1024*4;
	char		buf[bufSize];
	CString		file;

	if (setupfile.IsEmpty())
		file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_user);
	else	file = setupfile;
	int dwRc = GetPrivateProfileString(USERSN, USEREN,
				"0, 0", buf, bufSize, file);

	m_tool2 = GetPrivateProfileInt(USERSN, TOOLTYPE, 1, file);
	((CButton *)GetDlgItem(IDC_TOOLVIEW))->SetCheck(m_tool2 == 1 ? 0 : 1);

	CString text = CString(buf, dwRc);
	int idx =text.Find(',');
	if (idx >= 0)	m_uIndex = atoi(text.Left(idx++));
	int tot = atoi(text.Mid(idx));

	CString code = _T("");
	CString gName = _T("");

	CString key = _T("");
	_Group*	pGroup = (_Group *)NULL;
	for(POSITION pos = m_mGroup.GetStartPosition(); pos != NULL; )
	{
		m_mGroup.GetNextAssoc(pos, key, (CObject*&)pGroup);
		delete pGroup;
	}
	m_mGroup.RemoveAll();
	m_gName.RemoveAll();

	for (int ii = 0; ii < tot; ii++)
	{
		code.Format("%02d", ii);
		dwRc = GetPrivateProfileString(USERSN, code, "99=", buf, bufSize, file);
		if (dwRc <= 0 || !strcmp(buf, "99="))
			break;
		gName = CString(buf, dwRc);
		if(gName.IsEmpty())
		{
			m_editN = _T("DefaultTOOL");
			OnAddgroup();
		}

		m_gName.Add(gName);
		pGroup = new _Group();
		if (GetPrivateProfileSection(code, buf, bufSize, file) <= 0)
		{
			m_mGroup.SetAt(code, (CObject *)pGroup);
			continue;
		}

		getUserScreen(code, buf, bufSize, pGroup);
		m_mGroup.SetAt(code, (CObject *)pGroup);
	}
	
	LoadSubInfo(file);
	redrawCombo();
}

void CTSetup::LoadSubInfo(CString setupfile)
{
	const int	bufSize = 1024*4;
	char		buf[bufSize];
	CString		file, key, str;

	_subGroup* subGroup = (_subGroup *)NULL;
	for(POSITION pos = m_subGroup.GetStartPosition(); pos != NULL; )
	{
		m_subGroup.GetNextAssoc(pos, key, (CObject*&)subGroup);
		delete subGroup;
	}
	m_subGroup.RemoveAll();

	if (setupfile.IsEmpty())
		file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_user);
	else 	file = setupfile;
	for (int ii = 0; ii < MAX_SUBGROUP; ii++)
	{
		key.Format("%03d", ii);
		GetPrivateProfileString("SUB_GROUP", key, "", buf, bufSize, file);
		
		str = buf;		str.TrimRight();
		if (str.IsEmpty())	break;

		subGroup = new _subGroup();
		if (GetPrivateProfileSection(str, buf, bufSize, file) <= 0)
		{
			m_subGroup.SetAt(str, (CObject *)subGroup);
			continue;
		}

		getUserScreenSub(str, buf, bufSize, subGroup);
		m_subGroup.SetAt(str, (CObject *)subGroup);
	}
}

void CTSetup::getUserScreen(CString gName, char* tmp, int len, _Group* pGroup)
{
	CString mapCode, mapName, mapShort, text = tmp;
	int	idx = 0, pos = 0;

	while (!text.IsEmpty())
	{
		idx = text.Find('=');
		if (idx > 0)
		{
			mapCode = text.Left(idx++);
			pos = text.Find(';');
			if (pos > 0)
			{
				mapName = text.Mid(idx, pos-idx);
				mapShort = text.Mid(++pos);

				if (mapShort.IsEmpty() && (mapCode.GetLength() != 4))
					mapShort = mapCode.Mid(2, 4);
			}
			else
			{
				mapName = text.Mid(idx);
				mapShort = mapName;
			}

			if (mapCode.Find('#') != -1)
				pGroup->gData.sGroup = SUBGROUPEXIST;
			else	pGroup->gData.sGroup = SUBGROUPNOEXIST;
			
			pGroup->gData.Code = mapCode;
			pGroup->gData.Name = mapName;
			pGroup->gData.Short = mapShort;
			pGroup->m_pGroup.Add(pGroup->gData);
		}
		
		tmp += text.GetLength();
		len -= text.GetLength();
		if (--len <= 0)
			break;
		text = ++tmp;
	}
}

void CTSetup::getUserScreenSub(CString gName, char* tmp, int len, _subGroup* subGroup)
{
	CString mapCode, mapName, mapShort, text = tmp;
	int	idx = 0, pos = 0;

	while (!text.IsEmpty())
	{
		idx = text.Find('=');
		if (idx > 0)
		{
			mapCode = text.Left(idx++);
			pos = text.Find(';');
			if (pos > 0)
			{
				mapName = text.Mid(idx, pos-idx);
				mapShort = text.Mid(++pos);

				if (mapShort.IsEmpty() && (mapCode.Find('#') == -1))
					mapShort = m_menu->GetDispN(mapCode);
			}
			else
			{
				mapName = text.Mid(idx);
				mapShort = mapName;
			}

			subGroup->subData.Code = mapCode;
			subGroup->subData.Name = mapName;
			subGroup->subData.Short = mapShort;
			subGroup->m_sGroup.Add(subGroup->subData);
		}
		
		tmp += text.GetLength();
		len -= text.GetLength();
		if (--len <= 0)
			break;
		text = ++tmp;
	}
}

void CTSetup::loadUserTree()
{
	m_usrtree.DeleteAllItems();
	_Group*	pGroup = GetCurGroup();
	if (!pGroup)	return;

	HTREEITEM	parent, child;
	CString		info;

	
	for (int ii = 0; ii < pGroup->m_pGroup.GetSize(); ii++)
	{
		pGroup->gData = pGroup->m_pGroup.GetAt(ii);
		if (pGroup->gData.sGroup == SUBGROUPEXIST)
		{
			parent = m_usrtree.InsertItem(pGroup->gData.Name, 0, 1);
			m_usrtree.SetItemData(parent, (DWORD) HASCHILD);
			info.Format("%s\t%s", pGroup->gData.Code, pGroup->gData.Short);
			m_usrtree.SetItemDataEx(parent, info, 0);

			_subGroup* subGroup = NULL;
			CString	key = pGroup->gData.Code;
			if (!m_subGroup.Lookup(key, (CObject*&)subGroup))
				continue;

			for (int jj = 0; jj < subGroup->m_sGroup.GetSize(); jj++)
			{
				subGroup->subData = subGroup->m_sGroup.GetAt(jj);
				child = m_usrtree.InsertItem(subGroup->subData.Name, 2, 3, parent);
				info.Format("%s\t%s", subGroup->subData.Code, subGroup->subData.Short);
				m_usrtree.SetItemDataEx(child, info, 0);
			}

		}
		else
		{
			child = m_usrtree.InsertItem(pGroup->gData.Name, 2, 3);
			info.Format("%s\t%s", pGroup->gData.Code, pGroup->gData.Short);
			m_usrtree.SetItemDataEx(child, info, 0);
		}
	}
}

void CTSetup::redrawCombo()
{
	POSITION pos = NULL;
	CString key = _T("");
	_Group*	pGroup = (_Group *)NULL;

	m_Combo.ResetContent();
	if (!m_gName.GetSize())	return;
	for (int ii = 0; ii < m_gName.GetSize(); ii++)
	{
		m_Combo.AddString(m_gName.GetAt(ii));
		key.Format("%02d", ii);
		m_Combo.SetItemDataEx(ii, key);
	}
	
	m_Combo.SetCurSel(m_uIndex);

	UpdateData(TRUE);
	m_Combo.GetLBText(m_uIndex, m_editN);
	UpdateData(FALSE);

	loadUserTree();
}

void CTSetup::OnSelchangeGname() 
{
	m_uIndex = m_Combo.GetCurSel();
	UpdateData(TRUE);
	m_Combo.GetLBText(m_uIndex, m_editN);
	UpdateData(FALSE);
	loadUserTree();
}

void CTSetup::SaveTreeData()
{
	DWORD	dw;
	int	pos;
	CString subKey, info, sName;
	_Group*	pGroup = GetCurGroup();

	pGroup->m_pGroup.RemoveAll();
	HTREEITEM item = m_usrtree.GetRootItem();
	while (item)
	{
		m_usrtree.GetItemDataEx(item, info, dw);

		pos = info.Find('\t');
		pGroup->gData.Code = info.Left(pos++);
		pGroup->gData.Short = info.Mid(pos);
		pGroup->gData.Name = m_usrtree.GetItemText(item);
		if (pGroup->gData.Code.Find('#') != -1)
			pGroup->gData.sGroup = SUBGROUPEXIST;
		else	pGroup->gData.sGroup = SUBGROUPNOEXIST;
		pGroup->m_pGroup.Add(pGroup->gData);

		subKey = GetSubKey(item);
		if (!subKey.IsEmpty())
		{
			_subGroup* subG;
			if (m_subGroup.Lookup(subKey, (CObject *&) subG))
			{
				HTREEITEM sItem;
				subG->m_sGroup.RemoveAll();
				sItem = m_usrtree.GetChildItem(item);
				while (sItem)
				{
					m_usrtree.GetItemDataEx(sItem, info, dw);
					pos = info.Find('\t');
					subG->subData.Name = m_usrtree.GetItemText(sItem);
					subG->subData.Code = info.Left(pos++);
					subG->subData.Short = info.Mid(pos);
					subG->m_sGroup.Add(subG->subData);

					sItem = m_usrtree.GetNextSiblingItem(sItem);
				}
			}
		}
		item = m_usrtree.GetNextItem(item, TVGN_NEXT);
	}
	SetGuide(0);
}

void CTSetup::OnAddgroup() 
{
	if (m_Combo.GetCount() == MAX_GROUP)
	{
		SetGuide(ST_MSG_MAXTOOLGROUPNOTIFY);
		return;
	}

	UpdateData(TRUE);
	CString	tmpS;
	for (int ii = 0; ii < m_Combo.GetCount(); ii++)
	{
		m_Combo.GetLBText(ii, tmpS);
		if (m_editN.Compare(tmpS) == 0)
		{
			SetGuide(ST_MSG_EXISTGROUPCHANGEGROUP);
			GotoDlgCtrl(GetDlgItem(IDC_EDIT));
			return;
		}
	}

	if (m_editN.IsEmpty())
	{
		SetGuide(ST_MSG_REGGROUPBYMENU);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT));
		return;
	}

	_Group* pGroup = new _Group();
	CString key = makeKey();
	m_uIndex = m_Combo.GetCount();
	m_Combo.AddString(m_editN);
	m_Combo.SetCurSel(m_uIndex);

	m_Combo.SetItemDataEx(m_uIndex, key);
	m_mGroup.SetAt(key, (CObject *)pGroup);

	UpdateData(FALSE);
	loadUserTree();
}

void CTSetup::OnDelgroup() 
{
	if (m_Combo.GetCount() <= 0)
	{
		SetGuide(ST_MSG_TOOL_NOGROUP);
		return;
	}
	
	int idx = m_Combo.GetCurSel();
	if (idx < 0)
	{
		SetGuide(ST_MSG_SELECTDELGROUP);
		return;
	}

	CString sName = _T("");
	m_Combo.GetLBText(idx, sName);
	CString key;
	m_Combo.GetItemDataEx(idx, key);
	UpdateData(TRUE);
	m_Combo.DeleteString(idx);
	UpdateData(FALSE);
	
	
	CObject* object = (CObject *)NULL;
	m_mGroup.Lookup(key, (CObject*&)object);
	if (object == NULL)		return;
	_Group*	pGroup = (_Group *)object;
	pGroup->m_pGroup.RemoveAll();
	m_mGroup.RemoveKey(key);
	delete pGroup;

	UpdateData(TRUE);
	
	if (m_Combo.GetCount() > 0)
	{
		m_uIndex = 0;
		m_Combo.SetCurSel(m_uIndex);
		m_Combo.GetLBText(m_uIndex, m_editN);
	}
	else
	{
		m_uIndex = -1;
		m_editN = _T("");
		m_Combo.ResetContent();
	}

	UpdateData(FALSE);
	loadUserTree();
//	redrawGList();	
}

void CTSetup::OnChangegroupname() 
{
	UpdateData(TRUE);
	if (m_editN.IsEmpty())
	{
		SetGuide(ST_MSG_NOCHANGEGROUP);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT));
		return;
	}

	CString	tmpS;
	for (int ii = 0; ii < m_Combo.GetCount(); ii++)
	{
		m_Combo.GetLBText(ii, tmpS);
		if (m_Edit.Compare(tmpS) == 0)
		{
			SetGuide(ST_MSG_EXISTCHANGEGROUP);
			GotoDlgCtrl(GetDlgItem(IDC_EDIT));
			return;
		}
	}

	CString key = _T("");
	CString sName = _T("");
	m_Combo.GetItemDataEx(m_uIndex, key);
	m_Combo.GetLBText(m_uIndex, sName);
	m_Combo.DeleteString(m_uIndex);
	m_Combo.InsertString(m_uIndex, m_editN);
	m_Combo.SetCurSel(m_uIndex);
	m_Combo.SetItemDataEx(m_uIndex, key);

	UpdateData(FALSE);
}

CString CTSetup::makeKey()
{
	int ii = 0;
	int jj = 0;
	CString key = _T("");
	CString tmpS;
	bool bIs = false;
	for (ii = 0; ii < MAX_GROUP; ii++)
	{
		key.Format("%02d", ii);
		bIs = false;
		for (jj = 0; jj < m_Combo.GetCount(); jj++)
		{
			m_Combo.GetItemDataEx(jj, tmpS);
			if (tmpS == key)
			{
				bIs = true;
				break;
			}
		}
		if (!bIs && jj == m_Combo.GetCount())
			break;
	}
	return key;
}

void CTSetup::saveData()
{
	int tot = m_Combo.GetCount();
	int idx = m_Combo.GetCurSel();
	if (idx < 0)	idx = 0;
	CString tmpS = _T("");
	CString		file;

	tmpS.Format("%d, %d", idx, tot);
	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_user);

	DeleteFile(file);
	WritePrivateProfileSection(USERSN, "", file);
	WritePrivateProfileString(USERSN, USEREN, tmpS, file);

	_Group*	pGroup = NULL;
	CString key = _T("");
	CString name = _T("");

	m_sIndex = 0;
	for (int ii = 0; ii < tot; ii++)
	{
		m_Combo.GetItemDataEx(ii, key);
		m_mGroup.Lookup(key, (CObject*&) pGroup);
		if (!pGroup)	continue;

		m_Combo.GetLBText(ii, name);
		key.Format("%02d", ii);
		WritePrivateProfileString(USERSN, key, name, file);
		saveScreen(key, pGroup);
	}

	tmpS.Format("%d", m_tool2);
	WritePrivateProfileString(USERSN, TOOLTYPE, tmpS, file);
}

void CTSetup::saveScreen(CString key, _Group* pGroup)
{
	CString	tmpS, code, file;

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_user);
	WritePrivateProfileSection(key, "", file);
	for (int ii = 0; ii < pGroup->m_pGroup.GetSize(); ii++)
	{
		pGroup->gData = pGroup->m_pGroup.GetAt(ii);
		if (pGroup->gData.sGroup == SUBGROUPEXIST)
		{
			code = pGroup->gData.Code;
			tmpS = pGroup->gData.Name + ";" + pGroup->gData.Short;
		
			WritePrivateProfileString(key, code, tmpS, file);
			tmpS.Format("%03d", m_sIndex++);
			WritePrivateProfileString("SUB_GROUP", tmpS, code, file);
			saveScreenSub(code);
		}
		else
		{
			code = pGroup->gData.Code;
			tmpS = pGroup->gData.Name + ";" + pGroup->gData.Short;
			WritePrivateProfileString(key, code, tmpS, file);
		}
	}
}

void CTSetup::saveScreenSub(CString key)
{
	CString	tmpS, code, file;

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_user);
	WritePrivateProfileSection(key, "", file);

	_subGroup* subG = NULL;
	m_subGroup.Lookup(key, (CObject *&) subG);
	if (!subG)	return;

	for (int ii = 0; ii < subG->m_sGroup.GetSize(); ii++)
	{
		subG->subData = subG->m_sGroup.GetAt(ii);
		
		code = subG->subData.Code;
		tmpS = subG->subData.Name + ";" + subG->subData.Short;
		WritePrivateProfileString(key, code, tmpS, file);
	}
}

void CTSetup::OnOK() 
{
	saveData();
	CSetupDialog::OnOK();
}

void CTSetup::setTree()
{
	m_Tree.DeleteAllItems();

	char	buf[1024];
	CString	file, dat;

	file.Format("%s\\%s\\%s", m_home, TABDIR, AXISMENU);

	FILE *fp; 
	errno_t err = fopen_s(&fp, file, "r");
	if (err)	return;

	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		dat = buf;
		if (dat.Find("www#") != -1)
			continue;
		make_Tree(dat);
	}
	fclose(fp);
}

void CTSetup::setList(BOOL flag)
{
	UpdateData(TRUE);
	CString desc, com, comS = m_Edit;
	UpdateData(FALSE);

	m_List.DeleteAllItems();
	m_arTr.RemoveAll();
	comS.TrimRight();
	if (comS.IsEmpty())	return;
	for (int ii = 0; ii < m_arMap.GetSize(); ii++)
	{
		com = m_arMap.GetAt(ii);
		m_arDesc.Lookup(com, desc);
		if (desc.Find(comS) == -1)	continue;
		if (com.Find("#") != -1)	continue;
		
		int row = m_List.GetItemCount();

		m_arTr.Add(com);
		m_List.InsertItem(row, m_menu->GetMapping(com).Mid(2, 4));
		m_List.SetItemText(row, 1, desc);
	}

	if (m_List.GetItemCount())
	{
		m_List.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
		m_List.SetScrollPos(SB_VERT, 0, TRUE);
	}

	if (!flag)	fit();
}

void CTSetup::make_Tree(CString dat)
{
	int	treeN, step = 0;
	BOOL	map, sep;
	CString	str, desc;
	
	HTREEITEM item;
	map = sep = FALSE;
	while (!dat.IsEmpty())
	{
		int pos = dat.Find(SPLIT);
		if (pos == -1)	break;

		str = dat.Left(pos++);
		dat = dat.Mid(pos);
		if (str.IsEmpty())	continue;

		char	ch = str.GetAt(0);
		switch (ch)
		{
		case SEP:	continue;
		case MAP:	map = TRUE;	break;
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
				desc = str;
				item = m_Tree.InsertItem(desc, 2, 3, m_itemN[step-1]);
				m_Tree.SetItemData(item, (DWORD) m_count++);
				if (!dat.IsEmpty())
				{
					int pos = dat.Find(SPLIT);
					int pos2 = dat.Find(SPLIT2);
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

void CTSetup::findItem(HTREEITEM item, CString comS)
{
	CString		com;
	HTREEITEM	sItem;

	do
	{
		com = m_Tree.GetItemText(item);
		if (com.Find(comS) != -1)
		{
			m_arItem.Add(item);
			m_cnt++;
		}

		if (!m_Tree.ItemHasChildren(item))
			continue;
		sItem = m_Tree.GetNextItem(item, TVGN_CHILD);
		findItem(sItem,  comS);

	} while ((item = m_Tree.GetNextItem(item, TVGN_NEXT)) != NULL);
}

void CTSetup::OnToolinit() 
{
	CString	msg, title;
	msg.LoadString(ST_MSG_TOOLINIT);
	title.LoadString(ST_TEXT_TOOLINIT);
	if (::MessageBox(m_hWnd, msg, title, MB_ICONQUESTION|MB_YESNO) != IDYES)	return;

	CString	file, cpfile;
	CString user = AfxGetApp()->GetProfileString(WORKSTATION, SIGNONID);
	int pos  = user.Find('|');
	if (pos < 0)	
		user = _T("Anonymous");
	else		
		user = user.Mid(pos+1);

	if (user.IsEmpty())	
		user = _T("Anonymous");

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, user);
	cpfile.Format("%s\\%s\\usertool.ini", m_home, MTBLDIR);
	CopyFile(cpfile, file, FALSE);

	CSetupDialog::OnOK();
}

void CTSetup::ChangeHangulMode(HWND hWnd)
{
	if (PRIMARYLANGID(GetSystemDefaultLangID()) != LANG_KOREAN)
		return;

	DWORD	dwConversion, dwSentence;
	HIMC hImc = ImmGetContext(hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
		ImmSetConversionStatus(hImc, dwConversion | IME_CMODE_HANGEUL, dwSentence);
}

void CTSetup::ChangeEnglishMode(HWND hWnd)
{
	DWORD	dwConversion, dwSentence;
	HIMC hImc = ImmGetContext(hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		if (dwConversion & IME_CMODE_HANGEUL)
			dwConversion -= IME_CMODE_HANGEUL;
		ImmSetConversionStatus(hImc, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
		ImmReleaseContext(hWnd, hImc);
	}
}

void CTSetup::OnAddsubgroup() 
{
	if (m_Combo.GetCount() <= 0)
	{
		SetGuide(ST_MSG_TOOL_NOGROUP);
		return;
	}

	if (m_subGroup.GetCount() > MAX_SUBGROUP)
	{
		SetGuide(ST_MSG_NOTIFYMAXSUBGROUP);
		return;
	}

	AddTreeGroup();
}

_Group* CTSetup::GetCurGroup()
{
	CString key = _T("");
	int idx = m_Combo.GetCurSel();
	if (idx < 0)	return NULL;
	m_Combo.GetItemDataEx(idx, key);

	_Group*	pGroup = NULL;
	m_mGroup.Lookup(key, (CObject*&)pGroup);
	return pGroup;
}

CString CTSetup::GetShortName(CString dat)
{
return dat;
	char	sName[16];
	ZeroMemory(sName, sizeof(sName));

	char	ch;
	int	count = 0, len = dat.GetLength();
	for (int ii = 0, hf = 0; ii < len && 4 >= count; ii++)
	{
		ch = dat.GetAt(ii);
		if (ch & 0x80)
		{
			hf ^= 1;
			sName[ii] = ch;
			if (!hf) count++;
		}
		else if (hf)
		{
			hf = 0;
			sName[ii] = ' ';
			count++;
		}
		else
		{
			sName[ii] = ch;
			count++;
		}
	}
	return sName;
}

void CTSetup::OnDblclkUsertree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	PostMessage(WM_TNOTIFY, 2, 0);
}

void CTSetup::OnBegindragUsertree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pTView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	if (m_Combo.GetCount() <= 0)
	{
		SetGuide(ST_MSG_TOOL_NOGROUP);
		return;
	}
	if (!pTView->itemNew.hItem)	return;

	UINT uFlags;
	HTREEITEM hItem = m_usrtree.HitTest(pTView->ptDrag, &uFlags);

	if (hItem && (TVHT_ONITEM & uFlags))
		m_usrtree.SelectItem(hItem);

	hItem = m_usrtree.GetSelectedItem();
	if (!hItem)	return;

	m_pDragImage = NULL;
	m_pDragWnd = &m_usrtree;
	m_bDrag = true;

	m_hCurItem = hItem;
	SetCapture();
	SetCursor(LoadCursor(NULL, MOVECURSOR));
}

BOOL CTSetup::IsExistMap(CString mapN, CString subKey)
{
	if (subKey.IsEmpty())
	{
		_gData	gData;
		_Group*	pGroup = GetCurGroup();
		for (int ii = 0; ii < pGroup->m_pGroup.GetSize(); ii++)
		{
			gData = pGroup->m_pGroup.GetAt(ii);
			if (!gData.Code.Compare(mapN))
				return TRUE;
		}
	}
	else
	{
		_subGroup* subGroup = NULL;
		if (!m_subGroup.Lookup(subKey, (CObject*&)subGroup))
			return FALSE;

		_subData subData;
		for (int ii = 0; ii < subGroup->m_sGroup.GetSize(); ii++)
		{
			subData = subGroup->m_sGroup.GetAt(ii);
			if (!subData.Code.Compare(mapN))
				return TRUE;
		}
	}
	return FALSE;
}

void CTSetup::AddTreeItem(CString mapN, HTREEITEM sItem)
{
	DWORD	dw;
	BOOL	bHasChild = FALSE;
	CString sName, desc, str, key;

	desc  = m_menu->GetDesc(mapN);
	sName = m_menu->GetSName(mapN);

	_Group*	pGroup = GetCurGroup();

	HTREEITEM item;
	if (!sItem)	
		sItem = m_usrtree.GetSelectedItem();
	if (sItem)
	{
		m_usrtree.GetItemDataEx(sItem, str, dw);
		if (str.Find('#') != -1) 
			bHasChild = TRUE;
	}

	if (sItem && bHasChild)
	{
		m_usrtree.GetItemDataEx(sItem, str, dw);
		int	pos = str.Find('\t');
		key = str.Left(pos);

		if (IsExistMap(mapN, key))
		{
			SetGuide(ST_TSETUP_MSG_EXISTMAP);
			return;
		}

		item = m_usrtree.InsertItem(desc, 2, 3, sItem);
		str.Format("%s\t%s", mapN, sName);
		m_usrtree.SetItemDataEx(item, str, 0);
		m_usrtree.Expand(sItem, TVE_EXPAND);
	}
	else
	{
		if (sItem)
		{
			HTREEITEM parent = m_usrtree.GetParentItem(sItem);
			if (parent)
			{
				m_usrtree.GetItemDataEx(parent, str, dw);
				int	pos = str.Find('\t');
				key = str.Left(pos);

				if (IsExistMap(mapN, key))
				{
					SetGuide(ST_TSETUP_MSG_EXISTMAP);
					return;
				}
				
				item = m_usrtree.InsertItem(desc, 2, 3, parent);
				str.Format("%s\t%s", mapN, sName);
				m_usrtree.SetItemDataEx(item, str, 0);
			}
			else
			{
				if (IsExistMap(mapN))
				{
					SetGuide(ST_TSETUP_MSG_EXISTMAP);
					return;
				}

				item = m_usrtree.InsertItem(desc, 2, 3);
				str.Format("%s\t%s", mapN, sName);
				m_usrtree.SetItemDataEx(item, str, 0);
			}
		}
		else
		{
			if (IsExistMap(mapN))
			{
				SetGuide(ST_TSETUP_MSG_EXISTMAP);
				return;
			}

			item = m_usrtree.InsertItem(desc, 2, 3);
			str.Format("%s\t%s", mapN, sName);
			m_usrtree.SetItemDataEx(item, str, 0);
		}
	}

	SaveTreeData();
}

void CTSetup::AddTreeGroup()
{
	CString		gKeys, gName, info, gText;
	_subGroup*	subG;
	int ii = 0;

	gText.LoadString(ST_TSETUP_SUBGROUP);
	for (ii = 0; ii <= m_subGroup.GetCount(); ii++)
	{
		gKeys.Format("#%03d", ii);
		if (m_subGroup.Lookup(gKeys, (CObject *&) subG))
			continue;

		subG = new _subGroup();
		m_subGroup.SetAt(gKeys, (CObject *) subG);
		break;
	}

	gName.Format("%s01", gText);
	for (ii = 1; !IsUniqueName(gName); ii++)
		gName.Format("%s%02d", gText, ii);

	_Group*	pGroup = GetCurGroup();
	pGroup->gData.Name = gName;
	pGroup->gData.Code = gKeys;
	pGroup->gData.Short = GetShortName(gName);
	pGroup->gData.sGroup = SUBGROUPEXIST;
	pGroup->m_pGroup.Add(pGroup->gData);

	m_hCurItem = m_usrtree.InsertItem(pGroup->gData.Name, 0, 1);
	m_usrtree.SetItemData(m_hCurItem, (DWORD) HASCHILD);
	info.Format("%s\t%s", pGroup->gData.Code, pGroup->gData.Short);
	m_usrtree.SetItemDataEx(m_hCurItem, info, 0);

	m_usrtree.SelectItem(m_hCurItem);
	m_usrtree.SetEditState(true, m_hCurItem);
}

CString CTSetup::GetSubKey(HTREEITEM item)
{
	DWORD	dw;
	CString	info, keys;

	m_usrtree.GetItemDataEx(item, info, dw);
	keys = info.Left(info.Find('\t'));
	if (keys.Find('#') != -1)
		return keys;
	return _T("");
}

void CTSetup::delTree(HTREEITEM item)
{
	if (!item)		return;
	CString	subKey = GetSubKey(item);
	if (subKey.IsEmpty())
	{
		m_usrtree.DeleteItem(item);
		SaveTreeData();
	}
	else
	{
		if (!m_usrtree.ItemHasChildren(item))
		{
			_subGroup* subG = NULL;
			m_subGroup.Lookup(subKey, (CObject*&)subG);
			if (subG)	delete subG;
			m_subGroup.RemoveKey(subKey);
			m_usrtree.DeleteItem(item);
			SaveTreeData();
		}
	}
}

LRESULT CTSetup::OnUSER(WPARAM wParam, LPARAM lParam)
{
	int	cmd = (int)wParam;
	HTREEITEM item = m_usrtree.GetSelectedItem();
	switch (cmd)
	{
	case 1:	// click
		if (!item)	break;
		m_usrtree.SetEditState(true, item);
		break;
	case 2:	// db click
		if (!item)	break;
		delTree(item);
		break;
	default:
		break;
	}
	return 0;
}

void CTSetup::OnChangeSubName(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	*pResult = 0;
	CString subName = (LPCTSTR)pTVDispInfo->item.pszText;
	HTREEITEM item = m_usrtree.GetSelectedItem();
	if (subName.IsEmpty())
		subName = m_usrtree.GetItemText(item);
	if (!IsUniqueName(subName, item))
	{
		SetGuide(ST_MSG_EXISTCHANGEGROUP);
		m_usrtree.SetEditState(true, item);
		return;
	}
	m_usrtree.SetItemText(item/*m_hCurItem*/, subName);
	m_usrtree.SetEditState(false);
	CString  info, gKey;
	DWORD  dw;
	
	m_usrtree.GetItemDataEx(item, info, dw);
	int pos = info.Find(0x09);
	if (pos != -1)
	{
		gKey = info.Left(pos);
		_Group* pGroup = GetCurGroup();
		for (int ii = 0; ii < pGroup->m_pGroup.GetSize(); ii++)
		{
			pGroup->gData = pGroup->m_pGroup.GetAt(ii);
			if (!gKey.Compare(pGroup->gData.Code))
			{
				pGroup->gData.Name = subName;
				pGroup->gData.Short = GetShortName(subName);
				pGroup->m_pGroup.SetAt(ii, pGroup->gData);

				info.Format("%s\t%s", pGroup->gData.Code, pGroup->gData.Short);
				m_usrtree.SetItemDataEx(item, info, 0);
				break;
			}
		}
	}
}

BOOL CTSetup::IsUniqueName(CString name, HTREEITEM owner)
{
	HTREEITEM item = m_usrtree.GetRootItem();
	while (item)
	{
		CString subKey = GetSubKey(item);
		if (!subKey.IsEmpty())
		{
			CString sName = m_usrtree.GetItemText(item);
			if (owner)
			{
				if (item != owner && !sName.Compare(name))
					return FALSE;
			}
			else if (!sName.Compare(name))
				return FALSE;
		}

		item = m_usrtree.GetNextItem(item, TVGN_NEXT);
	}

	return TRUE;
}

void CTSetup::OnChangesubgroup() 
{
	HTREEITEM item = m_usrtree.GetSelectedItem();
	if (!item)
	{
		SetGuide(ST_TSETUP_MSG_NOSELECTITEM);
		return;
	}
	
	if (GetSubKey(item).IsEmpty())
	{
		SetGuide(ST_TSETUP_MSG_NOSUBGROUP);
		return;
	}

	m_usrtree.SetEditState(true, item);
}

void CTSetup::OnDelsubgroup() 
{
	HTREEITEM item = m_usrtree.GetSelectedItem();
	if (!item)
	{
		SetGuide(ST_TSETUP_MSG_NOSELECTITEM);
		return;
	}

	CString	keys = GetSubKey(item);
	if (keys.IsEmpty())
	{
		SetGuide(ST_TSETUP_MSG_NOSUBGROUP);
		return;
	}

	_subGroup* subGroup = NULL;
	m_subGroup.Lookup(keys, (CObject*&)subGroup);
	if (subGroup)	delete subGroup;
	m_subGroup.RemoveKey(keys);
	m_usrtree.DeleteItem(item);
	SaveTreeData();
}

void CTSetup::SetGuide(UINT nID)
{
	CString	guide;
	guide.LoadString(nID);

	GetDlgItem(IDC_TSETUP_ERRMSG)->SetWindowText(guide);
}

void CTSetup::MoveItem(HTREEITEM dragItem, HTREEITEM dropItem)
{
	CString	dragKey, dropKey, title, info, mapN;
	DWORD	dw;
	HTREEITEM parent, newItem, childItem;

	dragKey = GetSubKey(dragItem);
	if (dropItem)
		dropKey = GetSubKey(dropItem);
	else	dropItem = TVI_ROOT;

	title = m_usrtree.GetItemText(dragItem);
	if (dragKey.IsEmpty())
	{
		m_usrtree.GetItemDataEx(dragItem, info, dw);
		mapN = info.Left(info.Find('\t'));
		m_usrtree.DeleteItem(dragItem);
		
		if (dropKey.IsEmpty())
		{
			if (dropItem == TVI_ROOT)
				newItem = m_usrtree.InsertItem(title, 2, 3);
			else
			{
				parent = m_usrtree.GetParentItem(dropItem);
				newItem = m_usrtree.InsertItem(title, 2, 3, parent, dropItem);
			}
		}
		else
			newItem = m_usrtree.InsertItem(title, 2, 3, dropItem);
		m_usrtree.SetItemDataEx(newItem, info, 0);
	}
	else
	{
		if (dropItem == TVI_ROOT)
			parent = m_usrtree.InsertItem(title, 0, 1);
		else	
			parent = m_usrtree.InsertItem(title, 0, 1, TVI_ROOT, dropItem);

		childItem = m_usrtree.GetChildItem(dragItem);
		while (childItem)
		{
			title = m_usrtree.GetItemText(childItem);
			newItem = m_usrtree.InsertItem(title, 2, 3, parent);
			m_usrtree.GetItemDataEx(childItem, info, dw);
			m_usrtree.SetItemDataEx(newItem, info, dw);

			childItem = m_usrtree.GetNextSiblingItem(childItem);
		}
		m_usrtree.GetItemDataEx(dragItem, info, dw);
		m_usrtree.DeleteItem(dragItem);
		m_usrtree.SetItemDataEx(parent, info, dw);
	}

	SaveTreeData();
}

void CTSetup::ApplySetup()
{
	saveData();
}

void CTSetup::DefaultSetup()
{
	CString file;
	file.Format("%s\\%s\\usertool.ini", m_home, MTBLDIR);
	
	LoadInfo(file);	
}

void CTSetup::OnToolview() 
{
	int chk = ((CButton *)GetDlgItem(IDC_TOOLVIEW))->GetCheck();
	m_tool2 = chk ? 2 : 1;
}


void CTSetup::OnBnClickedUpmove()
{
	if (m_Combo.GetCount() <= 0)
	{
		SetGuide(ST_MSG_TOOL_NOGROUP);
		return;
	}

	HTREEITEM hItem = m_usrtree.GetSelectedItem();
	HTREEITEM hItemInsert = NULL;

	m_usrtree.Select(hItem, TVGN_CARET);
	hItemInsert = m_usrtree.GetPrevVisibleItem(hItem);

	if (hItem && hItemInsert)
	{
		MoveItem(hItemInsert, hItem);
		m_usrtree.Select(hItem, TVGN_CARET);
	}
}


void CTSetup::OnBnClickedDownmove()
{
	if (m_Combo.GetCount() <= 0)
	{
		SetGuide(ST_MSG_TOOL_NOGROUP);
		return;
	}
	
	HTREEITEM hItem = m_usrtree.GetSelectedItem();
	HTREEITEM hItemInsert = NULL;
	CString strTmp;
	
	m_usrtree.Select(hItem, TVGN_CARET);
	hItemInsert = m_usrtree.GetNextVisibleItem(hItem);
	
	if (hItem && hItemInsert)
	{
		MoveItem(hItem, hItemInsert);
		m_usrtree.Select(hItem, TVGN_CARET);
	}
}

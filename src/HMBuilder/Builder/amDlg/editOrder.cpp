// editOrder.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "editOrder.h"
#include "afxdialogex.h"

#include "editListCtrl.h"
#include "popListCtrl.h"
#include "sepWnd.h"
#include "../amCom/formItem.h"
#include "../amCom/cellItem.h"

#define	EK_IOORDER	0
#define	EK_TABORDER	1

// CEditOrder 대화 상자입니다.

// Sort the item in reverse alphabetical order.
static int CALLBACK orderSortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the tree control.
	// The lParam of an item is just its handle.
	CEditOrder* pOrder = (CEditOrder*) lParamSort;
	if (lParam1 != -1 && lParam2 != -1)
	{
		int	nItem1 = pOrder->m_formItem->GetForm((int)lParam1)->index;
		int	nItem2 = pOrder->m_formItem->GetForm((int)lParam2)->index;

		if (nItem1 < nItem2)
			return -1;
		else
			return 1;
	}
	return 1;	
}

IMPLEMENT_DYNAMIC(CEditOrder, CDialogEx)

CEditOrder::CEditOrder(CWnd* pParent, struct _mapH* mapH, int iKind)
	: CDialogEx(CEditOrder::IDD, pParent)
{
	m_pimglstDragImage = NULL;

	m_mapH = mapH;
	m_formItem = new CFormItem(mapH);
	m_popup  = NULL;
	m_sepWnd = NULL;

	m_DlgKind = iKind;
}

CEditOrder::~CEditOrder()
{
}

void CEditOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OLDLIST, m_oList);
}


BEGIN_MESSAGE_MAP(CEditOrder, CDialogEx)
//	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_USER+1, &CEditOrder::OnMessage)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_OLDLIST, &CEditOrder::OnBegindragOldlist)
	ON_NOTIFY(NM_DBLCLK, IDC_OLDLIST, &CEditOrder::OnDblclkOldlist)
	ON_BN_CLICKED(IDC_DOWN_BTN, &CEditOrder::OnDownBtn)
	ON_BN_CLICKED(IDC_UP_BTN, &CEditOrder::OnUpBtn)
END_MESSAGE_MAP()


// CEditOrder 메시지 처리기입니다.


BOOL CEditOrder::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	addColumn(&m_oList, _T("#"),    0, 15);
	addColumn(&m_oList, _T("Form"), 1, 30);
	addColumn(&m_oList, _T("Name"), 2, 50);
	addColumn(&m_oList, _T("I/O"),  3, 20);
	addColumn(&m_oList, _T("Size"), 4, 12);
	addColumn(&m_oList, _T("Data"), 5, 50);

	m_oList.SetColumnType(_T("000012"));	// 0:output, 1:edit, 2:combo

	m_oList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_oList.m_strList.AddTail(_T("Numeric"));
	m_oList.m_strList.AddTail(_T("Alpha"));
	m_oList.m_strList.AddTail(_T("MultiByte"));
	m_oList.m_strList.AddTail(_T("Mixed"));
	m_oList.m_strList.AddTail(_T("Digit"));
	fillList();

	if (!m_DlgKind)
		SetWindowText(_T("Edit Tab Order"));
	else
		SetWindowText(_T("Edit I/O Order"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEditOrder::OnOK()
{
	CString	sSize, sData;
	int	idx, nCnt, fmCnt = m_formItem->GetCount();
	
	nCnt = 0;
	for (int ii = 0; ii < m_oList.GetItemCount(); ii++)
	{
		sSize = m_oList.GetItemText(ii, 4);
		sData = m_oList.GetItemText(ii, 5);
		idx = (int)m_oList.GetItemData(ii);

		if (idx >= fmCnt)
			continue;

		if (m_formItem->GetForm(idx)->size != atoi(sSize))
			m_formItem->GetForm(idx)->size = atoi(sSize);
		if (!getDataKind(m_formItem->GetForm(idx)->type).IsEmpty() && getDataKind(m_formItem->GetForm(idx)->type).Compare(sData))
		{
			if (!sData.Compare(_T("Numeric")))
				m_formItem->GetForm(idx)->type = 1;
			else if (!sData.Compare(_T("Alpha")))
				m_formItem->GetForm(idx)->type = 2;
			else if (!sData.Compare(_T("MultiByte")))
				m_formItem->GetForm(idx)->type = 3;
			else if (!sData.Compare(_T("Mixed")))
				m_formItem->GetForm(idx)->type = 4;
			else if (!sData.Compare(_T("Digit")))
				m_formItem->GetForm(idx)->type = 5;
		}

		m_formItem->GetForm(idx)->index = (WORD)++nCnt;
	}

	CDialogEx::OnOK();
}


void CEditOrder::OnDestroy()
{
	if (m_pimglstDragImage) 
		delete m_pimglstDragImage;

	if (m_sepWnd)
	{
		m_sepWnd->DestroyWindow();
		delete m_sepWnd;
	}
	if (m_formItem)
		delete m_formItem;

	CDialogEx::OnDestroy();
}


void CEditOrder::OnLButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();
	
	if (m_pimglstDragImage)
	{
		m_pimglstDragImage->DragLeave(GetDesktopWindow());
		m_pimglstDragImage->EndDrag();
		m_pimglstDragImage->DeleteImageList();

		endDrag(point);
		delete m_pimglstDragImage;
		m_pimglstDragImage = NULL;
		m_sepWnd->ShowWindow(SW_HIDE);
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


afx_msg LRESULT CEditOrder::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 0:
		{
			int	nItem = LOWORD(lParam), nSubItem = HIWORD(lParam);
			int	nIdx  = (int)m_oList.GetItemData(nItem);
			int	kind  = m_formItem->GetForm(nIdx)->kind;
			if (kind == FM_BUTTON || kind == FM_BROWSER || kind == FM_TABLE || kind == FM_GRID || kind == FM_GRIDEX || kind == FM_OBJECT)
				return FALSE;
			if (nSubItem == 5 && getDataKind(m_formItem->GetForm(nIdx)->type).IsEmpty())
				return FALSE;
		}
		return TRUE;
	}
	return TRUE;

}


void CEditOrder::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_pimglstDragImage)
	{
		CRect	rc;
		LVHITTESTINFO	lvhti;
		CPoint	pt, ptMove = point;

		ClientToScreen(&ptMove);
		m_pimglstDragImage->DragMove(ptMove);
		m_oList.ScreenToClient(&ptMove);
		lvhti.pt = ptMove;
		
		m_oList.GetItemRect(0, &rc, LVIR_BOUNDS);
		int	nItem = m_oList.HitTest(&lvhti);
		if (nItem < 0)
			nItem = (ptMove.y < rc.Height()?0:m_oList.GetItemCount());

		adjustScroll(nItem);
		m_oList.GetItemPosition((nItem == m_oList.GetItemCount() ? nItem - 1 : nItem), &pt);
		
		m_sepWnd->ShowWindow(SW_SHOW);
		if (nItem == m_oList.GetItemCount())
			m_sepWnd->MoveSWWnd(pt.x, pt.y - 3 + rc.Height() * 2);
		else
			m_sepWnd->MoveSWWnd(pt.x, pt.y - 3 + rc.Height());
	}
	else if (m_sepWnd)
		m_sepWnd->ShowWindow(SW_HIDE);

	CDialogEx::OnMouseMove(nFlags, point);
}


void CEditOrder::OnBegindragOldlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	*pResult = 0;
	if (m_oList.GetSelectedCount() == 0)
		return ;

	m_item = pNMLV->iItem;
	saveItems();

	if (m_pimglstDragImage)
		delete m_pimglstDragImage;

	CPoint	pt;
	m_pimglstDragImage = createDragImageSymbol(&pt, &m_oList);

	if (m_pimglstDragImage == (CImageList *)NULL)
		return;

	CPoint	ptStart = pNMLV->ptAction;
	m_oList.ClientToScreen(&ptStart);

	m_pimglstDragImage->BeginDrag(0, CPoint(0, 0));
	m_pimglstDragImage->DragEnter(GetDesktopWindow(), ptStart);
	
	if (m_sepWnd)
	{
		m_sepWnd->DestroyWindow();
		delete m_sepWnd;
	}
	CRect rc;
	m_sepWnd = new CSepWnd();
	m_oList.GetClientRect(&rc);
	m_sepWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 7, 6), this, 3);

	SetCapture();
}


void CEditOrder::OnDblclkOldlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	if (m_oList.GetSelectedCount() == 0)
		return ;

	int	kind, nItem, nIdx;

	nItem = pNMItemActivate->iItem;
	nIdx  = (int)m_oList.GetItemData(nItem);
	kind  = m_formItem->GetForm(nIdx)->kind;

	if (kind == FM_GRID || kind == FM_TABLE)
	{
		if (m_popup)
		{
			m_popup->DestroyWindow();
			delete m_popup;
			m_popup = NULL;
		}
		if (!m_popup)
		{
			CPoint pt;
			int	nCnt;
			CString sClassName  = AfxRegisterWndClass(0);

			GetCursorPos(&pt);
			pt.x += 10;
			pt.y += 10;
			m_popup = new CPopListCtrl();
					
			m_popup->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, sClassName, NULL, WS_POPUP|WS_BORDER|WS_THICKFRAME, CRect(pt.x, pt.y, pt.x + 260, pt.y + 150), NULL, NULL, NULL);
			addColumn(m_popup->m_editListCtrl, _T("Name"), 1, 50);
			addColumn(m_popup->m_editListCtrl, _T("I/O"),  2, 25);
			addColumn(m_popup->m_editListCtrl, _T("Size"), 3, 12);
			addColumn(m_popup->m_editListCtrl, _T("Data"), 4, 60);
			
			nCnt = 0;
			struct _repR* repR = (struct _repR*)m_formItem->GetForm(nIdx)->auxR;
			if (repR)
			{
				CCellItem cellItem(repR);
				for (int ii = 0; ii < cellItem.GetCount(); ii++)
				{
					CString sIO, sName, sData, sSize;
					if (kind != FM_TABLE)
						sIO = getIO(cellItem.GetCell(ii)->iok);
					else
						sIO = _T("O");
						
					sSize.Format(_T("%d"), cellItem.GetCell(ii)->size);
					sName = cellItem.GetCell(ii)->name;
					sData = getDataKind(cellItem.GetCell(ii)->type);

					int nItem = addItem(m_popup->m_editListCtrl, sName, nCnt);
					addItem(m_popup->m_editListCtrl, sIO,   nItem, 1);
					addItem(m_popup->m_editListCtrl, sSize, nItem, 2);
					addItem(m_popup->m_editListCtrl, sData, nItem, 3);
					nCnt++;
				}
			}
			m_popup->ShowWindow(SW_SHOW);
		}
	}
}


void CEditOrder::OnDownBtn()
{
	int	nItem;
	CString	sItem, sItem1, sItem2, sItem3, sItem4, sItem5;
	CArray	<int, int> itemArr;

	for (POSITION pos = m_oList.GetFirstSelectedItemPosition(); pos; )
	{
		nItem = m_oList.GetNextSelectedItem(pos);
		if (nItem == m_oList.GetItemCount() - 1)
		{
			itemArr.RemoveAll();
			return;
		}
		
		if (!pos)
			adjustScroll(nItem + 1);
		itemArr.InsertAt(0, nItem);
	}

	for (int ii = 0; ii < itemArr.GetSize(); ii++)
	{
		nItem = itemArr.GetAt(ii);
		sItem = m_oList.GetItemText(nItem, 0);
		DWORD dData = m_oList.GetItemData(nItem);
		sItem1 = m_oList.GetItemText(nItem, 1);
		sItem2 = m_oList.GetItemText(nItem, 2);
		sItem3 = m_oList.GetItemText(nItem, 3);
		sItem4 = m_oList.GetItemText(nItem, 4);
		sItem5 = m_oList.GetItemText(nItem, 5);
		m_oList.DeleteItem(nItem);
		
		nItem = addItem(&m_oList, sItem, nItem + 1);
		m_oList.SetItemData(nItem, dData);
		addItem(&m_oList, sItem1, nItem, 1);
		addItem(&m_oList, sItem2, nItem, 2);
		addItem(&m_oList, sItem3, nItem, 3);
		addItem(&m_oList, sItem4, nItem, 4);
		addItem(&m_oList, sItem5, nItem, 5);
		
		setSelected(&m_oList, nItem, FALSE);
	}
}


void CEditOrder::OnUpBtn()
{
	int	nItem = -1;
	CString	sItem, sItem1, sItem2, sItem3, sItem4, sItem5;

	for (POSITION pos = m_oList.GetFirstSelectedItemPosition(); pos; )
	{
		if (nItem == -1)
		{
			nItem = m_oList.GetNextSelectedItem(pos);
			if (nItem == 0)
				break;
			adjustScroll(nItem - 1);
		}
		else
		{
			nItem = m_oList.GetNextSelectedItem(pos);
			if (nItem == 0)
				break;
		}
		
		sItem = m_oList.GetItemText(nItem, 0);
		DWORD dData = m_oList.GetItemData(nItem);
		sItem1 = m_oList.GetItemText(nItem, 1);
		sItem2 = m_oList.GetItemText(nItem, 2);
		sItem3 = m_oList.GetItemText(nItem, 3);
		sItem4 = m_oList.GetItemText(nItem, 4);
		sItem5 = m_oList.GetItemText(nItem, 5);
		m_oList.DeleteItem(nItem);
		
		nItem = addItem(&m_oList, sItem, nItem - 1);
		m_oList.SetItemData(nItem, dData);
		addItem(&m_oList, sItem1, nItem, 1);
		addItem(&m_oList, sItem2, nItem, 2);
		addItem(&m_oList, sItem3, nItem, 3);
		addItem(&m_oList, sItem4, nItem, 4);
		addItem(&m_oList, sItem5, nItem, 5);
		
		//killSelected(&m_oList);
		setSelected(&m_oList, nItem, FALSE);
	}	
}

void CEditOrder::adjustScroll(int nItem)
{
	CRect	rc;
	CSize	size;

	m_oList.GetItemRect(0, &rc, LVIR_BOUNDS);
	if (m_oList.GetTopIndex() > nItem)
	{
		size.cx = 0;
		size.cy = rc.Height() * (nItem - m_oList.GetTopIndex());
		m_oList.Scroll(size);
		m_oList.RedrawWindow();
	}
	else if (m_oList.GetTopIndex() + m_oList.GetCountPerPage() < nItem)
	{
		size.cx = 0;
		size.cy = rc.Height();
		m_oList.Scroll(size);
		m_oList.RedrawWindow();
	}
}

CString CEditOrder::getCellIO(struct _formR *formR, int idx)
{
	if (formR == NULL || formR->auxR == NULL)
		return _T("");

	CString sRtn = _T("");
	CCellItem cellItem((struct _repR*)formR->auxR);

	if (idx >= 0)
		return getIO(cellItem.GetCell(idx)->iok);
	else
	{
		for (int ii =0; ii < cellItem.GetCount(); ii++)
		{
			CString sIO = getIO(cellItem.GetCell(idx)->iok);
			if (!sIO.IsEmpty())
			{
				if (sRtn.IsEmpty())
					sRtn = sIO;
				if (!sRtn.CompareNoCase(sIO))
					continue;
				sRtn = _T("IO");
			}
		}
	}
	return sRtn;
}

int CEditOrder::cellSize(struct _formR *formR, int idx)
{
	if (formR == NULL || formR->auxR == NULL)
		return 0;

	int	size = 0;
	CCellItem cellItem((struct _repR*)formR->auxR);

	if (idx >= 0)
		return cellItem.GetCell(idx)->size;
	else
	{
		for (int ii =0; ii < cellItem.GetCount(); ii++)
			size += cellItem.GetCell(ii)->size;
	}
	return size;
}

CString CEditOrder::getIO(int iok)
{
	switch (iok)
	{
	case EIO_INOUT:
		return _T("IO");
	case EIO_INPUT:
		return _T("I");
	case EIO_OUTPUT:
		return _T("O");
	}
	return _T("");
}

CString CEditOrder::getDataKind(int type)
{
	switch(type)
	{
	case 1:
		return _T("Numeric");
	case 2:
		return _T("Alpha");
	case 3:
		return _T("MultiByte");
	case 4:
		return _T("Mixed");
	case 5:
		return _T("Digit");
	}
	return _T("");
}

bool CEditOrder::insertItemOfIdx(int idx, bool isInsert)
{
	CString	item, tmps, lens, sForm, sName, sIO, sSize, sData;
	int	kind, nCnt, nItem;

	kind = m_formItem->GetForm(idx)->kind;
	nCnt = 0;
	if (isValidItem(idx))
	{		
		if (kind == FM_TABLE)
		{
			if (!(m_formItem->GetForm(idx)->attr2 & GO_MULTI) && !(m_formItem->GetForm(idx)->attr2 & GO_XHEAD))
				return false;
		}
		
		item = _T("");
		tmps.Format(_T("%d"), m_formItem->GetForm(idx)->size);
		
		sData = getDataKind(m_formItem->GetForm(idx)->type);
		sIO   = getIO(m_formItem->GetForm(idx)->iok);
		sSize = tmps;
		if (m_formItem->GetForm(idx)->attr & FA_SKIP)
			sName.Format(_T("<%s>"), m_formItem->GetForm(idx)->name);
		else
			sName = m_formItem->GetForm(idx)->name;

		switch (kind)
		{
		case FM_BUTTON:
			sSize = _T("");
			sForm = _T("Button");
			break;

		case FM_RADIO:
			sSize = _T("1");
			sForm = _T("Radio");
			break;

		case FM_CHECK:
			sSize = _T("1");
			sForm = _T("Check");
			break;
			
		case FM_EDIT:
			sForm = _T("Edit");
			break;
			
		case FM_OUT:
			sForm = _T("Output");
			sIO   = _T("O");
			break;

		case FM_CONTROL:
			sForm = _T("Control");
			break;
			
		case FM_COMBO:
			sForm = _T("Combo");
			break;

		case FM_GRID:
			sForm = _T("Grid");
			sSize.Format(_T("%d"), cellSize(m_formItem->GetForm(idx)));
			sIO   = getCellIO(m_formItem->GetForm(idx));
			break;
			
		case FM_GRIDEX:
			sForm = _T("GridEX");
			sSize.Format(_T("%d"), cellSize(m_formItem->GetForm(idx)));
			sIO   = getCellIO(m_formItem->GetForm(idx));
			break;
			
		case FM_TABLE:
			if (m_formItem->GetForm(idx)->iok == EIO_OUTPUT)
			{
				sIO   = _T("O");
				sForm = _T("Table");
				sSize.Format(_T("%d"), cellSize(m_formItem->GetForm(idx)));
			}
			break;
			
		case FM_OBJECT:
			sForm = _T("Object");
			break;
			
		case FM_BROWSER:
			sForm = _T("Browser");
			break;		
		
		}

		tmps.Format(_T("%d"), m_formItem->GetForm(idx)->index);
		if (m_formItem->GetForm(idx)->attr & FA_SKIP)
			tmps = _T("*") + tmps;

		nItem = addItem(&m_oList, tmps, isInsert ? nCnt : 0);
		m_oList.SetItemData(isInsert?nCnt:0, idx);
		addItem(&m_oList, sForm, isInsert ? nCnt : 0, 1);
		addItem(&m_oList, sName, isInsert ? nCnt : 0, 2);
		addItem(&m_oList, sIO,   isInsert ? nCnt : 0, 3);
		addItem(&m_oList, sSize, isInsert ? nCnt : 0, 4);
		addItem(&m_oList, sData, isInsert ? nCnt++ : 0, 5);
	}
	return true;
}

void CEditOrder::fillList()
{
	int	count = m_formItem->GetCount();

	for (int ii = 0; ii < count; ii++)
		insertItemOfIdx(ii);

	m_oList.SortItems(orderSortProc, (LPARAM)this);
}

void CEditOrder::setSelected(CListCtrl* LC, int nItem, BOOL scroll)
{
	CRect	rc;
	CSize	size;

	LC->SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	if (scroll)
	{
		LC->GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - LC->GetTopIndex());
		LC->Scroll(size);
	}
}

void CEditOrder::killSelected(CListCtrl* LC)
{
	int	nItem;

	for (POSITION pos = LC->GetFirstSelectedItemPosition(); pos; )
	{
		nItem = LC->GetNextSelectedItem(pos);
		LC->SetItemState(nItem, NULL, LVIS_SELECTED | LVIS_FOCUSED);
	}
}

int CEditOrder::addItem(CListCtrl* LC, CString strItem, int nItem, int iSubItem)
{
	if (iSubItem >= 0)
	{
		if (LC->SetItemText(nItem, iSubItem, strItem))
			return nItem;
		else
			return -1;
	}

	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = nItem;
	lvi.iSubItem = 0;
	lvi.pszText  = (LPTSTR) strItem.operator LPCTSTR();
	return LC->InsertItem(&lvi);
}

BOOL CEditOrder::addColumn(CListCtrl* LC, LPCTSTR strItem, int nItem, int nWidth)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR)strItem;
	lvc.cx = LC->GetStringWidth(lvc.pszText) + nWidth;
	//lvc.iSubItem = -1;
	return LC->InsertColumn(nItem, &lvc);
}

void CEditOrder::saveItems()
{
	int	nItem;
	CString tmps;

	m_itemArr.RemoveAll();
	for (POSITION pos = m_oList.GetFirstSelectedItemPosition(); pos; )
	{
		nItem = m_oList.GetNextSelectedItem(pos);
		tmps.Format("%d\t%s\t%s\t%s\t%s\t%s\t%s\t%ld", nItem, 
								m_oList.GetItemText(nItem, 0),
								m_oList.GetItemText(nItem, 1),
								m_oList.GetItemText(nItem, 2),
								m_oList.GetItemText(nItem, 3),
								m_oList.GetItemText(nItem, 4),
								m_oList.GetItemText(nItem, 5),
								m_oList.GetItemData(nItem));
		m_itemArr.Add(tmps);
	}
}

void CEditOrder::moveItems(int nItem)
{
	int	ii, nIdx;
	CString sItem;

	for (ii = 0; ii < m_itemArr.GetSize(); ii++)
	{
		sItem = m_itemArr.GetAt(ii);
		nIdx = atoi(parse(sItem, _T("\t")));
		m_oList.SetItemText(nIdx, 0, _T(""));
	}

	for (ii = 0; ii < m_itemArr.GetSize(); ii++)
	{
		CString sItem = m_itemArr.GetAt(ii);
		parse(sItem, _T("\t"));
		addItem(&m_oList, parse(sItem, _T("\t")), nItem);
		addItem(&m_oList, parse(sItem, _T("\t")), nItem, 1);
		addItem(&m_oList, parse(sItem, _T("\t")), nItem, 2);
		addItem(&m_oList, parse(sItem, _T("\t")), nItem, 3);
		addItem(&m_oList, parse(sItem, _T("\t")), nItem, 4);
		addItem(&m_oList, parse(sItem, _T("\t")), nItem, 5);
		setSelected(&m_oList, nItem, FALSE);
		m_oList.SetItemData(nItem++, atol(parse(sItem, _T("\t"))));
	}

	for (ii = 0; ii < m_oList.GetItemCount(); ii++)
	{
		CString sItem = m_oList.GetItemText(ii, 0);
		if (sItem.IsEmpty())
		{
			m_oList.DeleteItem(ii);
			ii--;
		}
	}
	m_itemArr.RemoveAll();
}

CImageList* CEditOrder::createDragImageSymbol(LPPOINT lpPoint, CListCtrl* LC)
{
	int	count = LC->GetSelectedCount();
	if (count <= 0)
		return NULL;

	CRect	rcItem, rcTotal;

	rcTotal.SetRectEmpty();
	POSITION pos = LC->GetFirstSelectedItemPosition();
	if (pos == NULL)
		return NULL;

	int	item = LC->GetNextSelectedItem(pos);
	LC->GetItemRect(item, rcItem, LVIR_BOUNDS);
	rcTotal = rcItem;
	rcTotal.bottom = rcTotal.top + rcTotal.Height() * count;

	CClientDC dcClient(this);
	CDC	memDC;

	if (!memDC.CreateCompatibleDC(&dcClient))
		return NULL;

	CBitmap bitmap;
	if (!bitmap.CreateCompatibleBitmap(&dcClient, rcTotal.Width(), rcTotal.Height()))
		return NULL;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(0, 0, rcTotal.Width(), rcTotal.Height(), RGB(255, 255, 255));

	CImageList* pCompleteImageList = new CImageList;
	pCompleteImageList->Create(rcTotal.Width(), rcTotal.Height(), ILC_COLOR | ILC_MASK, 0, 1);

	CPoint	pt;
	int	index = 0;
	CImageList* pimglstSingle;
	for (pos = LC->GetFirstSelectedItemPosition(); pos; )
	{
		item = LC->GetNextSelectedItem(pos);
		LC->GetItemRect(item, rcItem, LVIR_LABEL);

		pimglstSingle = LC->CreateDragImage(item, &pt);
		if (pimglstSingle)
		{
			pimglstSingle->Draw(&memDC, 0, CPoint(0, rcItem.Height() * index++), ILD_MASK);
			delete pimglstSingle;
			pimglstSingle = NULL;
		}
	}

	memDC.SelectObject(pOldMemDCBitmap);
	pCompleteImageList->Add(&bitmap, RGB(255, 255, 0)); 
	
	if (lpPoint)
	{
		lpPoint->x = rcItem.left;
		lpPoint->y = rcItem.top;
	}

	return pCompleteImageList;
}

void CEditOrder::endDrag(CPoint point)
{
	CRect	ListRc;
	CString	sItem, sItem1, sItem2, sItem3;

	m_oList.GetClientRect(&ListRc);
	m_oList.ClientToScreen(&ListRc);
	ClientToScreen(&point);
	
	//if (ListRc.PtInRect(point))
	{
		LVHITTESTINFO lvhti;
		m_oList.ScreenToClient(&point);
		lvhti.pt = point;
		
		int	nItem = m_oList.HitTest(&lvhti);
		if (nItem < 0)
		{
			m_oList.GetItemRect(0, &ListRc, LVIR_BOUNDS);
			nItem = (point.y < ListRc.Height()?0:m_oList.GetItemCount());
		}
		moveItems(nItem);		
	}
}

CString CEditOrder::parse(CString& src, CString sub)
{
	CString	tmps;
	int	pos = src.Find(sub);

	if (pos == -1)
	{
		tmps = src;
		src.Empty();
		return tmps;
	}
	else
	{
		tmps = src.Left(pos);
		src = src.Mid(pos + sub.GetLength());
		return tmps;
	}
	return _T("");
}

bool CEditOrder::isValidItem(int idx)
{
	switch (m_formItem->GetForm(idx)->kind)
	{
	case FM_EDIT:
	case FM_BUTTON:
	case FM_COMBO:
	case FM_OBJECT:
	case FM_GRID:
	case FM_GRIDEX:
		return true;
	case FM_CONTROL:
		if (m_formItem->GetForm(idx)->iok == EIO_INPUT || m_formItem->GetForm(idx)->iok == EIO_INOUT)
			return true;
		break;
	}
	return false;
}

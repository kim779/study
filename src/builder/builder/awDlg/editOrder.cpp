// editOrder.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "editOrder.h"
#include "afxdialogex.h"

#include "editListCtrl.h"
#include "popListCtrl.h"
#include "sepWnd.h"
#include "../h/fmProperties.h"
#include "../awTool/awObjectLoad.h"
#include "../h/mapform.H"

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
		int    nItem1;
		int    nItem2;
		if (pOrder->m_kind == 0)	// ioorder
		{
			nItem1 = ((CObjectLoad*)pOrder->m_mapH->forms.GetAt((int)lParam1))->getProp()->iorder;
			nItem2 = ((CObjectLoad*)pOrder->m_mapH->forms.GetAt((int)lParam2))->getProp()->iorder;
		}
		else if (pOrder->m_kind == 1)	// taborder
		{
			nItem1 = ((CObjectLoad*)pOrder->m_mapH->forms.GetAt((int)lParam1))->getProp()->index;
			nItem2 = ((CObjectLoad*)pOrder->m_mapH->forms.GetAt((int)lParam2))->getProp()->index;
		}
		if (nItem1 < nItem2)
			return -1;
		else
			return 1;
	}
	return 1;	
}

IMPLEMENT_DYNAMIC(CEditOrder, CDialogEx)

CEditOrder::CEditOrder(CWnd* pParent, class mapForm* mapForm, int nKind)
	: CDialogEx(CEditOrder::IDD, pParent)
{
	m_pimglstDragImage = NULL;

	m_mapH = mapForm;
	m_kind   = nKind;
	m_popup  = NULL;
	m_sepWnd = NULL;
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

	if (m_kind == 0)
		SetWindowText(_T("Edit IO Order"));
	else 
		SetWindowText(_T("Edit Tab Order"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEditOrder::OnOK()
{
	CString	sSize, sData;
	int	idx, nCnt, fmCnt = m_mapH->forms.GetCount();
	
	nCnt = 0;
	for (int ii = 0; ii < m_oList.GetItemCount(); ii++)
	{
		sSize = m_oList.GetItemText(ii, 4);
		sData = m_oList.GetItemText(ii, 5);
		idx = (int)m_oList.GetItemData(ii);

		if (idx >= fmCnt)
			continue;

		CformProp* prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp();
		if (prop->size != atoi(sSize))
			prop->size = atoi(sSize);
		if (!getDataKind(prop->charType).IsEmpty() && getDataKind(prop->charType).Compare(sData))
		{
			if (!sData.Compare(_T("Numeric")))
				prop->charType = 1;
			else if (!sData.Compare(_T("Alpha")))
				prop->charType = 2;
			else if (!sData.Compare(_T("MultiByte")))
				prop->charType = 3;
			else if (!sData.Compare(_T("Mixed")))
				prop->charType = 4;
			else if (!sData.Compare(_T("Digit")))
				prop->charType = 5;
		}

		if (m_kind == 0)	// ioorder
			prop->iorder = (WORD)++nCnt;
		else if (m_kind == 1)	// taborder
			prop->index = (WORD)++nCnt;
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
			int	kind  = ((CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(nIdx))->getProp())->kind;
			if (kind == fmBUTTON || kind == fmBROWSER || kind == fmTABLE || kind == fmGRID || kind == fmGRIDEX || kind == fmFORM)
				return FALSE;
			if (nSubItem == 5 && getDataKind(((CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(nIdx))->getProp())->charType).IsEmpty())
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
	CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(nIdx))->getProp();
	kind  = Prop->kind;

	if (kind == fmGRID || kind == fmGRIDEX || kind == fmTABLE)
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
			if (Prop->cellProperties != NULL)
			{
				for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
				{
					cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
					CString sIO, sName, sData, sSize;
					if (kind != fmTABLE)
						sIO = getIO(cellProp->ioType);
					else
						sIO = _T("O");
						
					sSize.Format(_T("%d"), cellProp->size);
					sName = cellProp->name;
					sData = getDataKind(cellProp->charType);

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
		DWORD_PTR dData = m_oList.GetItemData(nItem);
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
		DWORD_PTR dData = m_oList.GetItemData(nItem);
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

CString CEditOrder::getCellIO(class CformProp* Prop, int idx)
{
	if (Prop->cellProperties == NULL || Prop->cellProperties->IsEmpty())
		return _T("");

	CString sRtn = _T("");

	if (idx >= 0)
		return getIO(((cellOb*)Prop->cellProperties->GetAt(idx))->ioType);
	else
	{
		for (int ii =0; ii < Prop->cellProperties->GetCount(); ii++)
		{
			CString sIO = getIO(((cellOb*)Prop->cellProperties->GetAt(idx))->ioType);
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

int CEditOrder::cellSize(CformProp* Prop, int idx)
{
	if (Prop->cellProperties == NULL || Prop->cellProperties->IsEmpty())
		return 0;

	int	size = 0;
	if (idx >= 0)
		return ((cellOb*)Prop->cellProperties->GetAt(idx))->size;
	else
	{
		for (int ii =0; ii < Prop->cellProperties->GetCount(); ii++)
			size += ((cellOb*)Prop->cellProperties->GetAt(ii))->size;
	}
	return size;
}

CString CEditOrder::getIO(int ioType)
{
	switch (ioType)
	{
	case ioBOTH:
		return _T("IO");
	case ioINPUT:
		return _T("I");
	case ioOUTPUT:
		return _T("O");
	}
	return _T("");
}

CString CEditOrder::getDataKind(int charType)
{
	switch(charType)
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

	CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp();
	kind = Prop->kind;
	nCnt = 0;
	if (isValidItem(idx))
	{		
		if (kind == fmTABLE)
		{
			if (!(Prop->auxAttributes & gaMULTI) && !(Prop->auxAttributes & gaXHEAD))
				return false;
		}
		
		item = _T("");
		tmps.Format(_T("%d"), Prop->size);
		sData = getDataKind(Prop->charType);
		sIO   = getIO(Prop->ioType);
		sSize = tmps;
		if (Prop->attributes & faSKIP)
			sName.Format(_T("<%s>"), Prop->name);
		else
			sName = Prop->name;

		switch (kind)
		{
		case fmBUTTON:
			sSize = _T("");
			sForm = _T("Button");
			break;
		case fmRADIO:
			sSize = _T("1");
			sForm = _T("Radio");
			break;
		case fmCHECK:
			sSize = _T("1");
			sForm = _T("Check");
			break;
			
		case fmEDIT:
			sForm = _T("Edit");
			break;
			
		case fmOUTPUT:
			sForm = _T("Output");
			sIO   = _T("O");
			break;

		case fmOBJECT:
			sForm = _T("Object");
			break;
			
		case fmCOMBO:
			sForm = _T("Combo");
			break;
			
		case fmGRID:
			sForm = _T("Grid");
			sSize.Format(_T("%d"), cellSize(Prop));
			sIO   = getCellIO(Prop);
			break;
		case fmGRIDEX:
			sForm = _T("GridEx");
			sSize.Format(_T("%d"), cellSize(Prop));
			sIO   = getCellIO(Prop);
			break;	
		case fmTREE:
			sForm = _T("Tree");
			sIO   = _T("I");
			break;
			
		case fmTABLE:
			if (Prop->ioType == ioOUTPUT)
			{
				sIO   = _T("O");
				sForm = _T("Table");
				sSize.Format(_T("%d"), cellSize(Prop));
			}
			break;
			
		case fmFORM:
			sForm = _T("Form");
			break;

		case fmBROWSER:
			sForm = _T("Browser");
			break;
			
		case fmMEMO: 
			sForm = _T("Memo");
			break;
		}

		if (m_kind == 0)
			tmps.Format(_T("%d"), Prop->iorder);
		else if (m_kind == 1)
			tmps.Format(_T("%d"), Prop->index);

		if (Prop->attributes & faSKIP)
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
	int	count = m_mapH->forms.GetCount();

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
	CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp();
	int	kind = Prop->kind;

	if (m_kind == 0)	// ioorder
	{
		switch (kind)
		{
		case fmEDIT:
		case fmOUTPUT:
		case fmCOMBO:
		case fmGRID:
		case fmGRIDEX:
		case fmTABLE:
		case fmFORM:
		case fmMEMO:
		case fmTREE:
		//case FM_SHEET:
			return true;

		case fmBROWSER:
			if (Prop->ioType == ioOUTPUT)
				return true;
			break;

		case fmRADIO:
		case fmCHECK:
			if (Prop->ioType != ioFREE)
				return true;
			break;
		case fmOBJECT:
			if (Prop->ioType != ioFREE)
				return true;
		}
		return false;
	}
	else if (m_kind == 1)	// taborder
	{
		switch (kind)
		{
		case fmEDIT:
		case fmBUTTON:
		case fmRADIO:
		case fmCHECK:
		case fmCOMBO:
		case fmFORM:
		case fmGRID:
		case fmGRIDEX:
		case fmMEMO:
			return true;
		case fmOBJECT:
			if (Prop->ioType == ioINPUT || Prop->ioType == ioBOTH)
				return true;
			break;
		}
		return false;
	}
	return false;
}

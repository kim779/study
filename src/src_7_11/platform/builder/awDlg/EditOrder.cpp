// EditOrder.cpp : implementation file
//

#include "stdafx.h"
#include "EditOrder.h"
#include "../awCommon/CellItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	EK_IOORDER	0
#define	EK_TABORDER	1

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
		if (pOrder->m_nKind == 0)	// ioorder
		{
			nItem1 = pOrder->m_pFormItem->getFormPoint(lParam1)->iorder;
			nItem2 = pOrder->m_pFormItem->getFormPoint(lParam2)->iorder;
		}
		else if (pOrder->m_nKind == 1)	// taborder
		{
			nItem1 = pOrder->m_pFormItem->getFormPoint(lParam1)->index;
			nItem2 = pOrder->m_pFormItem->getFormPoint(lParam2)->index;
		}
		if (nItem1 < nItem2)
			return -1;
		else
			return 1;
	}
	return 1;	
}


/////////////////////////////////////////////////////////////////////////////
// CEditOrder dialog


CEditOrder::CEditOrder(CWnd* pParent, struct _mapH* mapH, int nKind)
	: CDialog(CEditOrder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditOrder)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pimglstDragImage = NULL;

	m_mapH  = mapH;
	m_pFormItem = new CFormItem(mapH);
	m_nKind = nKind;
	m_pPopup = NULL;
	m_pSepWnd = NULL;
}


void CEditOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditOrder)
	DDX_Control(pDX, IDC_OLDLIST, m_oList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditOrder, CDialog)
	//{{AFX_MSG_MAP(CEditOrder)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_OLDLIST, OnBegindragOldlist)
	ON_WM_CLOSE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_UP_BTN, OnUpBtn)
	ON_BN_CLICKED(IDC_DOWN_BTN, OnDownBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_OLDLIST, OnDblclkOldlist)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 1, OnMessage1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditOrder message handlers

BOOL CEditOrder::addColumn(CListCtrl *LC, LPCTSTR strItem, int nItem, int nWidth)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR)strItem;
	lvc.cx = LC->GetStringWidth(lvc.pszText) + nWidth;
	//lvc.iSubItem = -1;

	return LC->InsertColumn(nItem, &lvc);
}

int CEditOrder::addItem(CListCtrl *LC, CString strItem, int nItem, int iSubItem /* = -1 */)
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

BOOL CEditOrder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	addColumn(&m_oList, "#",    0, 15);
	addColumn(&m_oList, "Form", 1, 30);
	addColumn(&m_oList, "Name", 2, 50);
	addColumn(&m_oList, "I/O",  3, 20);
	addColumn(&m_oList, "Size", 4, 12);
	addColumn(&m_oList, "Data", 5, 50);

	m_oList.SetColumnType("000012");	// 0:utput, 1:edit, 2:combo

	m_oList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_oList.m_strList.AddTail("Numeric");
	m_oList.m_strList.AddTail("Alpha");
	m_oList.m_strList.AddTail("MultiByte");
	m_oList.m_strList.AddTail("Mixed");
	m_oList.m_strList.AddTail("Digit");
	fillList();

	if (m_nKind == 0)
		SetWindowText("Edit IO Order");
	else 
		SetWindowText("Edit Tab Order");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
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
		
		int nItem = m_oList.HitTest(&lvhti);
		
		if (nItem < 0)
		{
			m_oList.GetItemRect(0, &ListRc, LVIR_BOUNDS);
			nItem = (point.y < ListRc.Height()?0:m_oList.GetItemCount());
		}
		
		MoveItems(nItem);		
	}
}

void CEditOrder::OnBegindragOldlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	*pResult = 0;
	if (m_oList.GetSelectedCount() == 0)
		return ;

	m_nItem = pNMListView->iItem;
	SaveItems();

	if (m_pimglstDragImage)
		delete m_pimglstDragImage;

	CPoint	pt;
	m_pimglstDragImage = CreateDragImageSymbol(&pt, &m_oList);

	if (m_pimglstDragImage == (CImageList *)NULL)
		return;

	CPoint	ptStart = pNMListView->ptAction;
	m_oList.ClientToScreen(&ptStart);

	m_pimglstDragImage->BeginDrag(0, CPoint(0, 0));
	
	m_pimglstDragImage->DragEnter(GetDesktopWindow(), ptStart);
	
	if (m_pSepWnd)
	{
		m_pSepWnd->DestroyWindow();
		delete m_pSepWnd;
	}
	CRect rc;
	m_pSepWnd = new CSepWnd();
	m_oList.GetClientRect(&rc);
	m_pSepWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 7, 6), this, 3);

	SetCapture();
}

CImageList* CEditOrder::CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl *ListCtrl)
{
	int iCount = ListCtrl->GetSelectedCount();
	// 선택된게 없다면
	if (iCount < 1)
		return NULL;

	// 전체 Rect를 구함
	CRect rectItem;
	CRect rectTotal;
	rectTotal.SetRectEmpty();

	
	POSITION pos = ListCtrl->GetFirstSelectedItemPosition();
	if (!pos)
	{
		return NULL;
	}

	int iItem = ListCtrl->GetNextSelectedItem(pos);
	ListCtrl->GetItemRect(iItem, rectItem, LVIR_BOUNDS);
	rectTotal = rectItem;
	rectTotal.bottom = rectTotal.top + rectTotal.Height() * iCount;

	CClientDC dcClient(this);
	CDC	memDC;
	if (!memDC.CreateCompatibleDC(&dcClient))
	{
		return NULL;
	}

	CBitmap bitmap;
	if (!bitmap.CreateCompatibleBitmap(&dcClient, rectTotal.Width(), rectTotal.Height()))
	{
		return NULL;
	}

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(0, 0, rectTotal.Width(), rectTotal.Height(), RGB(255, 255, 255));

	CImageList* pCompleteImageList = new CImageList;
	pCompleteImageList->Create(rectTotal.Width(), rectTotal.Height(), ILC_COLOR | ILC_MASK, 0, 1);

	int iIndex = 0;
	pos = ListCtrl->GetFirstSelectedItemPosition();
	while (pos)
	{
		int iItem = ListCtrl->GetNextSelectedItem(pos);
		ListCtrl->GetItemRect(iItem, rectItem, LVIR_LABEL);

		CPoint	pt;
		CImageList* pimglstSingle = ListCtrl->CreateDragImage(iItem, &pt);
		
		if (pimglstSingle)
		{
			pimglstSingle->Draw(	&memDC,
						0, 
						CPoint(0, rectItem.Height() * iIndex++),
						ILD_MASK);
			delete pimglstSingle;
			pimglstSingle = NULL;
		}
	}

	memDC.SelectObject(pOldMemDCBitmap);
	pCompleteImageList->Add(&bitmap, RGB(255, 255, 0)); 
	
	if (lpPoint)
	{
		lpPoint->x = rectItem.left;
		lpPoint->y = rectItem.top;
	}

	return pCompleteImageList;
}

void CEditOrder::OnClose() 
{
	CDialog::OnClose();
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
		m_pSepWnd->ShowWindow(SW_HIDE);
	}	
	CDialog::OnLButtonUp(nFlags, point);
}

void CEditOrder::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_pimglstDragImage)
	{
		CPoint	ptMove = point;
		ClientToScreen(&ptMove);
		
		m_pimglstDragImage->DragMove(ptMove);

		CRect rc;

		LVHITTESTINFO lvhti;
		m_oList.ScreenToClient(&ptMove);
		lvhti.pt = ptMove;
		
		m_oList.GetItemRect(0, &rc, LVIR_BOUNDS);
		int nItem = m_oList.HitTest(&lvhti);
		if (nItem < 0)
			nItem = (ptMove.y < rc.Height()?0:m_oList.GetItemCount());

		adjustScroll(nItem);
		CPoint pt;
		m_oList.GetItemPosition((nItem == m_oList.GetItemCount()?nItem - 1:nItem), &pt);
		
		m_pSepWnd->ShowWindow(SW_SHOW);
		if (nItem == m_oList.GetItemCount())
			m_pSepWnd->MoveSWWnd(pt.x, pt.y - 3 + rc.Height() * 2);
		else
			m_pSepWnd->MoveSWWnd(pt.x, pt.y - 3 + rc.Height());
	}
	else if (m_pSepWnd)
		m_pSepWnd->ShowWindow(SW_HIDE);
	CDialog::OnMouseMove(nFlags, point);
}

void CEditOrder::OnUpBtn() 
{
	int nItem = -1;
	CString	sItem, sItem1, sItem2, sItem3, sItem4, sItem5;

	POSITION pos = m_oList.GetFirstSelectedItemPosition();
	while(pos)
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

void CEditOrder::OnDownBtn() 
{
	int nItem;
	CString	sItem, sItem1, sItem2, sItem3, sItem4, sItem5;
	CArray	<int, int> itemArr;

	POSITION pos = m_oList.GetFirstSelectedItemPosition();
	while (pos)
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

void CEditOrder::killSelected(CListCtrl *ListCtrl)
{
	int nItem;
	POSITION pos = ListCtrl->GetFirstSelectedItemPosition();

	while (pos)
	{
		nItem = ListCtrl->GetNextSelectedItem(pos);
		ListCtrl->SetItemState(nItem, NULL, LVIS_SELECTED | LVIS_FOCUSED);
	}

}

void CEditOrder::setSelected(CListCtrl *ListCtrl, int nItem, BOOL scroll/* = TRUE */)
{
	CRect	rc;
	CSize	size;

	ListCtrl->SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	if (scroll)
	{
		ListCtrl->GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - ListCtrl->GetTopIndex());
		ListCtrl->Scroll(size);
	}
}

void CEditOrder::OnOK() 
{
	int	fmCnt = m_pFormItem->getFormCount(), nCnt = 0;
	for (int ii = 0; ii < m_oList.GetItemCount(); ii++)
	{
		CString sSize = m_oList.GetItemText(ii, 4),
			sData = m_oList.GetItemText(ii, 5);
		int idx = m_oList.GetItemData(ii);

		if (idx >= fmCnt)
			continue;
		if (m_pFormItem->getFormPoint(idx)->size != atoi(sSize))
			m_pFormItem->getFormPoint(idx)->size = atoi(sSize);
		if (!getDataKind(m_pFormItem->getFormPoint(idx)->type).IsEmpty() && 
			getDataKind(m_pFormItem->getFormPoint(idx)->type).Compare(sData))
		{
			if (!sData.Compare("Numeric"))
				m_pFormItem->getFormPoint(idx)->type = 1;
			else if (!sData.Compare("Alpha"))
				m_pFormItem->getFormPoint(idx)->type = 2;
			else if (!sData.Compare("MultiByte"))
				m_pFormItem->getFormPoint(idx)->type = 3;
			else if (!sData.Compare("Mixed"))
				m_pFormItem->getFormPoint(idx)->type = 4;
			else if (!sData.Compare("Digit"))
				m_pFormItem->getFormPoint(idx)->type = 5;
		}
		if (m_nKind == 0)	// ioorder
			m_pFormItem->getFormPoint(idx)->iorder = (WORD)++nCnt;
		else if (m_nKind == 1)	// taborder
			m_pFormItem->getFormPoint(idx)->index = (WORD)++nCnt;
	}
	CDialog::OnOK();
}

void CEditOrder::fillList()
{
	int	count = m_pFormItem->getFormCount();

	for (int ii = 0; ii < count; ii++)
		insertItemOfIdx(ii);

	m_oList.SortItems(orderSortProc, (LPARAM)this);
}

bool CEditOrder::insertItemOfIdx(int idx, bool isInsert)
{
	CString	item, tmps, lens, sForm, sName, sIO, sSize, sData;
	int kind = m_pFormItem->getFormPoint(idx)->kind, nCnt = 0;
	if (isValidItem(idx))
	{		
		if (kind == FM_TABLE)
		{
			if (!(m_pFormItem->getFormPoint(idx)->attr2 & GO_MULTI) && !(m_pFormItem->getFormPoint(idx)->attr2 & GO_XHEAD))
				return false;
		}
		
		item.Empty();
		tmps.Format("%d", m_pFormItem->getFormPoint(idx)->size);
		
		sData = getDataKind(m_pFormItem->getFormPoint(idx)->type);
		sIO   = getIO(m_pFormItem->getFormPoint(idx)->iok);
		sSize = tmps;
		if (m_pFormItem->getFormPoint(idx)->attr & FA_SKIP)
			sName.Format("<%s>", m_pFormItem->getFormPoint(idx)->name);
		else
			sName = m_pFormItem->getFormPoint(idx)->name;

		switch (kind)
		{
		case FM_BUTTON:
			sSize = "1";
			switch (m_pFormItem->getFormPoint(idx)->type)
			{
			case BTN_RADIO:
				sForm = "Radio";
				break;
			case BTN_CHECK:
				sForm ="Check";
				break;
			default:
				sSize = "";
				sForm = "Button";
				break;
			}
			break;
			
		case FM_EDIT:
			sForm = "Edit";
			break;
			
		case FM_OUT:
			sForm = "Output";
			sIO   = "O";
			break;

		case FM_CONTROL:
			sForm = "Control";
			break;
			
		case FM_COMBO:
			sForm = "Combo";
			break;
			
		case FM_GRID:
			sForm = "Grid";
			sSize.Format("%d", cellSize(m_pFormItem->getFormPoint(idx)));
			sIO   = getCellIO(m_pFormItem->getFormPoint(idx));
			break;
			
		case FM_TREE:
			sForm = "Tree";
			sIO   = "I";
			break;
			
		case FM_TABLE:
			if (m_pFormItem->getFormPoint(idx)->iok == EIO_OUTPUT)
			{
				sIO   = "O";
				sForm = "Table";
				sSize.Format("%d", cellSize(m_pFormItem->getFormPoint(idx)));
			}
			break;
			
		case FM_OBJECT:
			sForm = "Object";
			break;
			
		case FM_SHEET:
			sForm = "Sheet";
			break;
			
		case FM_BROWSER:
			sForm = "Browser";
			break;
			
		case FM_MEMO: 
			sForm = "Memo";
			break;
		}

		if (m_nKind == 0)
			tmps.Format("%d", m_pFormItem->getFormPoint(idx)->iorder);
		else if (m_nKind == 1)
			tmps.Format("%d", m_pFormItem->getFormPoint(idx)->index);

		if (m_pFormItem->getFormPoint(idx)->attr & FA_SKIP)
			tmps = "*" + tmps;

		int nItem = addItem(&m_oList, tmps, isInsert?nCnt:0);
		m_oList.SetItemData(isInsert?nCnt:0, idx);
		addItem(&m_oList, sForm, isInsert?nCnt:0, 1);
		addItem(&m_oList, sName, isInsert?nCnt:0, 2);
		addItem(&m_oList, sIO,   isInsert?nCnt:0, 3);
		addItem(&m_oList, sSize, isInsert?nCnt:0, 4);
		addItem(&m_oList, sData, isInsert?nCnt++:0, 5);
		}
	return true;
}

CString CEditOrder::getDataKind(int type)
{
	switch(type)
	{
	case 1:
		return "Numeric";
	case 2:
		return "Alpha";
	case 3:
		return "MultiByte";
	case 4:
		return "Mixed";
	case 5:
		return "Digit";
	}
	return "";
}

CString CEditOrder::getIO(int iok)
{
	switch (iok)
	{
	case EIO_INOUT:
		return "IO";
	case EIO_INPUT:
		return "I";
	case EIO_OUTPUT:
		return "O";
	}
	return "";
}

int CEditOrder::cellSize(_formR *formR, int idx)
{
	if (formR == NULL)
		return 0;
	if (formR->auxR == NULL)
		return 0;

	int size = 0;
	CCellItem cellItem((struct _repR*)formR->auxR);
	if (idx >= 0)
		return cellItem.getCellPoint(idx)->size;
	else
	{
		for (int ii =0; ii < cellItem.getCellCount(); ii++)
			size += cellItem.getCellPoint(ii)->size;
	}
	return size;
}

bool CEditOrder::isValidItem(int idx)
{
	int kind = m_pFormItem->getFormPoint(idx)->kind;
	if (m_nKind == 0)	// ioorder
	{
		/*if (m_pFormItem->getFormPoint(idx)->attr & FA_SKIP)
			return false;*/
		switch(kind)
		{
		case FM_EDIT:
		case FM_OUT:
		case FM_COMBO:
		case FM_GRID:
		case FM_TABLE:
		case FM_OBJECT:
		case FM_MEMO:
		case FM_TREE:
		case FM_SHEET:
			return true;
		case FM_BROWSER:
			if (m_pFormItem->getFormPoint(idx)->iok == EIO_OUTPUT)
				return true;
			break;
		case FM_BUTTON:
			if ((m_pFormItem->getFormPoint(idx)->type == BTN_RADIO || m_pFormItem->getFormPoint(idx)->type == BTN_CHECK)
				&& m_pFormItem->getFormPoint(idx)->iok != EIO_NOP)
				return true;
			break;
		case FM_CONTROL:
			if (m_pFormItem->getFormPoint(idx)->iok != EIO_NOP)
				return true;
		}
		return false;
	}
	else if (m_nKind == 1)	// taborder
	{
		switch(kind)
		{
		case FM_EDIT:
		case FM_BUTTON:
		case FM_COMBO:
		case FM_OBJECT:
		case FM_GRID:
		case FM_MEMO:
			return true;
		case FM_CONTROL:
			if (m_pFormItem->getFormPoint(idx)->iok == EIO_INPUT || 
				m_pFormItem->getFormPoint(idx)->iok == EIO_INOUT)
				return true;
			break;
		}
		return false;
	}
	return false;
}

long CEditOrder::OnMessage1(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case 0:
		{
			int nItem = LOWORD(lParam), nSubItem = HIWORD(lParam);
			int nIdx  = m_oList.GetItemData(nItem);
			int kind  = m_pFormItem->getFormPoint(nIdx)->kind;
			if (kind == FM_BUTTON || kind == FM_BROWSER || 
				kind == FM_TABLE || kind == FM_GRID ||
				kind == FM_OBJECT)
				return FALSE;
			if (nSubItem == 5 && getDataKind(m_pFormItem->getFormPoint(nIdx)->type).IsEmpty())
				return FALSE;
		}
		return TRUE;
		break;
	}
	return TRUE;
}

void CEditOrder::MoveItems(int nItem)
{
	for (int ii = 0; ii < m_itemArr.GetSize(); ii++)
	{
		CString sItem = m_itemArr.GetAt(ii);
		int nIdx = atoi(Parser(sItem, "\t"));
		m_oList.SetItemText(nIdx, 0, "");
	}
	for (int ii = 0; ii < m_itemArr.GetSize(); ii++)
	{
		CString sItem = m_itemArr.GetAt(ii);
		Parser(sItem, "\t");
		addItem(&m_oList, Parser(sItem, "\t"), nItem);
		addItem(&m_oList, Parser(sItem, "\t"), nItem, 1);
		addItem(&m_oList, Parser(sItem, "\t"), nItem, 2);
		addItem(&m_oList, Parser(sItem, "\t"), nItem, 3);
		addItem(&m_oList, Parser(sItem, "\t"), nItem, 4);
		addItem(&m_oList, Parser(sItem, "\t"), nItem, 5);
		setSelected(&m_oList, nItem, FALSE);
		m_oList.SetItemData(nItem++, atol(Parser(sItem, "\t")));
	}
	for (int ii = 0; ii < m_oList.GetItemCount(); ii++)
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

CString CEditOrder::Parser(CString &srcstr, CString substr)
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
	return "";
}

void CEditOrder::SaveItems()
{
	m_itemArr.RemoveAll();
	POSITION pos = m_oList.GetFirstSelectedItemPosition();

	while(pos)
	{
		int nItem = m_oList.GetNextSelectedItem(pos);
		CString sItem, sTmp;

		sItem.Format("%d\t%s\t%s\t%s\t%s\t%s\t%s\t%ld", nItem, 
			m_oList.GetItemText(nItem, 0),
			m_oList.GetItemText(nItem, 1),
			m_oList.GetItemText(nItem, 2),
			m_oList.GetItemText(nItem, 3),
			m_oList.GetItemText(nItem, 4),
			m_oList.GetItemText(nItem, 5),
			m_oList.GetItemData(nItem));
		m_itemArr.Add(sItem);
	}
}

CString CEditOrder::getCellIO(_formR *formR, int idx)
{
	if (formR == NULL)
		return "";
	if (formR->auxR == NULL)
		return "";
	CString sRtn = "";
	
	CCellItem cellItem((struct _repR*)formR->auxR);
	if (idx >= 0)
		return getIO(cellItem.getCellPoint(idx)->iok);
	else
	{
		for (int ii =0; ii < cellItem.getCellCount(); ii++)
		{
			CString sIO = getIO(cellItem.getCellPoint(idx)->iok);
			if (!sIO.IsEmpty())
			{
				if (sRtn.IsEmpty())
					sRtn = sIO;
				if (!sRtn.CompareNoCase(sIO))
					continue;
				sRtn = "IO";
			}
		}
	}
	return sRtn;
}

void CEditOrder::OnDblclkOldlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;		
	*pResult = 0;

	if (m_oList.GetSelectedCount() == 0)
	{
		return ;
	}

	int nItem = pNMListView->iItem, nIdx = m_oList.GetItemData(nItem);

	int kind  = m_pFormItem->getFormPoint(nIdx)->kind;

	if (kind == FM_GRID || kind == FM_TABLE)
	{
		if (m_pPopup)
		{
			m_pPopup->DestroyWindow();
			delete m_pPopup;
			m_pPopup = NULL;
		}
		if (!m_pPopup)
		{
			CString sClassName  = AfxRegisterWndClass(0);
			CPoint pt;
			
			GetCursorPos(&pt);
			pt.x += 10;
			pt.y += 10;
			m_pPopup = new CPopListCtrl();
					
			m_pPopup->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, sClassName, NULL,
				WS_POPUP|WS_BORDER|WS_THICKFRAME, CRect(pt.x, pt.y, pt.x + 260, pt.y + 150),
				NULL, NULL, NULL);
			addColumn(m_pPopup->m_pListCtrl, "Name", 1, 50);
			addColumn(m_pPopup->m_pListCtrl, "I/O", 2, 25);
			addColumn(m_pPopup->m_pListCtrl, "Size", 3, 12);
			addColumn(m_pPopup->m_pListCtrl, "Data", 4, 60);
			
			int nCnt = 0;
			struct _repR* repR = (struct _repR*)m_pFormItem->getFormPoint(nIdx)->auxR;
			if (repR)
			{
				CCellItem cellItem(repR);
				for (int ii = 0; ii < cellItem.getCellCount(); ii++)
				{
					CString sIO, sName, sData, sSize;
					if (kind != FM_TABLE)
						sIO = getIO(cellItem.getCellPoint(ii)->iok);
					else
						sIO = "O";
						
					sSize.Format("%d", cellItem.getCellPoint(ii)->size);
					sName = cellItem.getCellPoint(ii)->name;
					sData = getDataKind(cellItem.getCellPoint(ii)->type);

					int nItem = addItem(m_pPopup->m_pListCtrl, sName, nCnt);
					addItem(m_pPopup->m_pListCtrl, sIO,   nItem, 1);
					addItem(m_pPopup->m_pListCtrl, sSize, nItem, 2);
					addItem(m_pPopup->m_pListCtrl, sData, nItem, 3);
					nCnt++;
				}
			}
			m_pPopup->ShowWindow(SW_SHOW);
		}
	}
	
	
}

void CEditOrder::adjustScroll(int nItem)
{
	CRect rc;
	m_oList.GetItemRect(0, &rc, LVIR_BOUNDS);
	if (m_oList.GetTopIndex() > nItem)
	{
		CSize size;
		size.cx = 0;
		size.cy = rc.Height() * (nItem - m_oList.GetTopIndex());
		m_oList.Scroll(size);
		m_oList.RedrawWindow();
	}
	else if (m_oList.GetTopIndex() + m_oList.GetCountPerPage() < nItem)
	{
		CSize size;
		size.cx = 0;
		size.cy = rc.Height();
		m_oList.Scroll(size);
		m_oList.RedrawWindow();
	}
}

void CEditOrder::OnDestroy() 
{
	if (m_pimglstDragImage) 
		delete m_pimglstDragImage;
	if (m_pSepWnd)
	{
		m_pSepWnd->DestroyWindow();
		delete m_pSepWnd;
	}
	if (m_pFormItem)
		delete m_pFormItem;
	CDialog::OnDestroy();	
}

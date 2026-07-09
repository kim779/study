// SelectionBar.cpp: implementation of the CSelectionBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axiswork.h"
#include "SelectionBar.h"

#include "mainvar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define	CE_CLICK	1
#define	CE_CHANGE	2
#define	CE_DBLCLK	4

CSelectionBar::CSelectionBar()
{
	m_pFormItem = NULL;
	m_hMapItem  = NULL;
	m_mapH      = NULL;
}

CSelectionBar::~CSelectionBar()
{
	if (m_pFormItem)
		delete m_pFormItem;
}

BEGIN_MESSAGE_MAP(CSelectionBar, CSizingControlBarG)
	//{{AFX_MSG_MAP(CSelectionBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 10, OnSelectMessage)
END_MESSAGE_MAP()

int CSelectionBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_treeCtrl.Create(WS_CHILD | WS_VISIBLE | WS_DLGFRAME | WS_TABSTOP | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS, 
			CRect (0, 0, 0, 0), this, 100);	
	CImageList	imglist;
	CBitmap		bitmap;

	bitmap.LoadBitmap(IDB_SELECTIONBAR);
	imglist.Create(17, 17, ILC_COLOR24|ILC_MASK, 26, 1);
	imglist.Add(&bitmap, RGB(0, 255, 255));
	imglist.SetBkColor(CLR_NONE);
	imglist.Add(AfxGetApp()->LoadIcon(IDI_MAP));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_TEMPLATE));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_PROCEDURES));
	m_treeCtrl.SetImageList(&imglist, TVSIL_NORMAL);
	imglist.Detach();
	bitmap.Detach();
	return 0;
}

void CSelectionBar::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBarG::OnSize(nType, cx, cy);
	CRect rc;
	GetClientRect(rc);
	rc.DeflateRect(3, 5, 3, 5);
	m_treeCtrl.MoveWindow(&rc);
}

void CSelectionBar::Initialize(_mapH *mapH)
{
	if (m_pFormItem)
		delete m_pFormItem;
	m_mapH = mapH;
	m_pFormItem = new CFormItem(mapH);
	m_hMapItem = GetMapItem(mapH);
	if (!m_hMapItem)
	{
		m_hMapItem = AddTreeItem(GetMapName(), (DWORD)mapH, 0);
	}
	else
		ReplaceMapName(mapH);
	m_treeCtrl.SelectItem(m_hMapItem);
	m_treeCtrl.Expand(m_hMapItem, TVE_EXPAND);
}

HTREEITEM CSelectionBar::AddTreeItem(CString sStr, DWORD data, int nDepth)
{
	HTREEITEM hItem = TVI_ROOT;
	int	nImage = 23;
	if (nDepth == 1)
	{
		hItem = m_hMapItem;
		if (m_pFormItem->getFormPoint(data)->tabN)
		{
			DWORD nTabIdx = m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(data)->tabN);
			hItem = FindItem(nTabIdx, m_hMapItem);
		}
		nImage = m_pFormItem->getFormPoint(data)->kind;
		if (nImage == FM_BUTTON)
		{
			if (m_pFormItem->getFormPoint(data)->type == BTN_RADIO)
				nImage = 21;
			else if (m_pFormItem->getFormPoint(data)->type == BTN_CHECK)
				nImage = 22;
		}
		else if (nImage >= FM_GRAPH)
			nImage--;
	}
	else if (nDepth == 0)
		nImage += m_mapH->mapK;
	
	hItem = m_treeCtrl.InsertItem(sStr, nImage, nImage, hItem);
	m_treeCtrl.SetItemData(hItem, data);

	if (nDepth == 1)
		AddEventItem(data, hItem);
	m_treeCtrl.RedrawWindow();
	return hItem;
}

void CSelectionBar::ReplaceTreeItem(CString sStr, DWORD data)
{
	HTREEITEM hItem = FindItem(data, m_hMapItem);
	if (hItem)
		m_treeCtrl.SetItemText(hItem, sStr);
}

void CSelectionBar::ResetTree(int type /*= 1*/)
{
	HTREEITEM hItem = m_treeCtrl.GetChildItem(m_hMapItem);
	while (hItem)
	{
		HTREEITEM hNextItem = m_treeCtrl.GetNextSiblingItem(hItem);
		m_treeCtrl.DeleteItem(hItem);
		hItem = hNextItem;
	}
	AddEventItem(-1, m_hMapItem);

	if (type != 1)
		m_treeCtrl.DeleteAllItems();
}

HTREEITEM CSelectionBar::GetMapItem(_mapH *mapH)
{
	HTREEITEM hItem = m_treeCtrl.GetRootItem();
	while (hItem)
	{
		if (m_treeCtrl.GetItemData(hItem) == (DWORD)mapH)
			return hItem;
		hItem = m_treeCtrl.GetNextSiblingItem(hItem);
	}
	return NULL;
}

CString CSelectionBar::GetMapName(_mapH *mapH)
{
	if (!mapH)
		mapH = m_mapH;
	if (mapH && strlen(mapH->mapN) == 8)
		return m_mapH->mapN;
	if (mapH && mapH->mapK == MK_PROCEDURES)
		return "Procedures";
	else if (mapH && mapH->mapK == MK_TEMPLATE)
		return "Template";
	return "Map";
}

void CSelectionBar::SelectControl(int selctl)
{
	ClearSelection();
	if (selctl == -2)
	{
		for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
		{
			if (m_pFormItem->getFormPoint(ii)->bCommon)
			{
				m_treeCtrl.SetItemState(FindItem(ii, m_hMapItem), TVIS_SELECTED, TVIS_SELECTED);
			}
		}
		m_treeCtrl.Invalidate();
		return;
	}
	else if (selctl >= 0 && m_pFormItem->getFormPoint(selctl)->group)
	{
		struct _formR *pGroup = m_pFormItem->getFormPoint(selctl)->group;
		for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
		{
			if (m_pFormItem->getFormPoint(ii)->group == pGroup)
			{
				m_treeCtrl.SetItemState(FindItem(ii, m_hMapItem), TVIS_SELECTED, TVIS_SELECTED);
			}
		}
		return;
	}
	else if (selctl == -1)
	{
		m_treeCtrl.SelectItem(m_hMapItem);
		return;
	}

	m_treeCtrl.SelectItem(FindItem(selctl, m_hMapItem));
}

HTREEITEM CSelectionBar::FindItem(DWORD data, HTREEITEM hBaseItem)
{
	HTREEITEM hItem = m_treeCtrl.GetChildItem(hBaseItem);
	while (hItem)
	{
		if (m_treeCtrl.GetItemData(hItem) == data)
			return hItem;
		if (m_treeCtrl.ItemHasChildren(hItem))
		{
			HTREEITEM hRecItem = FindItem(data, hItem);
			if (hRecItem)
				return hRecItem;
		}
		hItem = m_treeCtrl.GetNextSiblingItem(hItem);
	}
	return NULL;
}

void CSelectionBar::ClearSelection()
{
	m_treeCtrl.ClearSelection();
}

long CSelectionBar::OnSelectMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 0:	// selete control to child
		{
			CArray <int, int> idxArr;
			BOOL bMulti = FALSE;
			HTREEITEM hItem = m_treeCtrl.getFirstSelectedItem(), hParentItem = GetParentItem((HTREEITEM)lParam);
			while (hItem)
			{
				if (m_treeCtrl.GetParentItem(hItem))
				{
					if (GetParentItem(hItem) == hParentItem)
					{
						DWORD data = m_treeCtrl.GetItemData(hItem);
						idxArr.Add(data);
					}
				}
				else
				{
					idxArr.RemoveAll();
					idxArr.Add(-1);
					break;
				}
				hItem = m_treeCtrl.getNextSelectedItem(hItem);
			}
			
			if (hParentItem)
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_FOCUSCHILD, m_treeCtrl.GetItemData(hParentItem));
			
			if (!idxArr.GetSize())
				idxArr.Add(-1);
			
			for (int ii = 0; ii < idxArr.GetSize(); ii++)
			{
				int idx = idxArr.GetAt(ii), nEvent = -1;
				if (!bMulti)
					m_treeCtrl.SelectItem((idx >= 0 ? FindItem(idx, m_hMapItem) : m_hMapItem));
				
				if (idx == -1)
					m_treeCtrl.SetItemState(m_hMapItem, TVIS_SELECTED, TVIS_SELECTED);
				else
					m_treeCtrl.SetItemState(FindItem(idx, m_hMapItem), TVIS_SELECTED, TVIS_SELECTED);
				
				if (idx >= 0 && HIWORD(idx))
				{
					nEvent = (short)HIWORD(idx);
					idx = (short)LOWORD(idx);
				}
				AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_SELCTRLTOCHILD, bMulti), MAKELPARAM(idx, SC_SELECTIONBAR));
				if (idxArr.GetSize() == 1 && nEvent >= 0)
					MainSendMsg(ID_USR_SELECTEVENT, nEvent);
				bMulti = TRUE;
			}
			m_treeCtrl.SetFocus();
		}
		break;
	case 1:	// delete selection controls
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_DELETESELFORMS);
		break;
	}
	return 0;
}

HTREEITEM CSelectionBar::GetParentItem(HTREEITEM hItem)
{
	if (!hItem)
		return NULL;
	HTREEITEM hParentItem = m_treeCtrl.GetParentItem(hItem);
	if (!hParentItem)
		hParentItem = hItem;
	while (m_treeCtrl.GetParentItem(hParentItem))
		hParentItem = m_treeCtrl.GetParentItem(hParentItem);
	return hParentItem;
}

void CSelectionBar::RemoveMap(_mapH *mapH)
{
	HTREEITEM hItem = GetMapItem(mapH);
	m_treeCtrl.DeleteItem(hItem);
}

void CSelectionBar::ReplaceMapName(_mapH *mapH)
{
	HTREEITEM hItem = GetMapItem(mapH);
	if (hItem)
	{
		m_treeCtrl.SetItemText(hItem, GetMapName(mapH));
		m_treeCtrl.SetItemImage(hItem, 23 + mapH->mapK, 23 + mapH->mapK);
	}
}

void CSelectionBar::AddEventItem(int nIndex, HTREEITEM hMyItem)
{
	if (!m_mapH)
		return;
	switch (nIndex)
	{
	case -1:
		if (m_mapH->onDeclaration)
			AddTreeEventItem(nIndex, 1, m_hMapItem);
		if (m_mapH->onInDeclaration)
			AddTreeEventItem(nIndex, 2, m_hMapItem);
		if (m_mapH->onStart)
			AddTreeEventItem(nIndex, 3, m_hMapItem);
		if (m_mapH->onSend)
			AddTreeEventItem(nIndex, 4, m_hMapItem);
		if (m_mapH->onReceive)
			AddTreeEventItem(nIndex, 5, m_hMapItem);
		if (m_mapH->onAlert)
			AddTreeEventItem(nIndex, 6, m_hMapItem);
		if (m_mapH->onService)
			AddTreeEventItem(nIndex, 7, m_hMapItem);
		if (m_mapH->onFile)
			AddTreeEventItem(nIndex, 8, m_hMapItem);
		if (m_mapH->onSelect)
			AddTreeEventItem(nIndex, 9, m_hMapItem);
		if (m_mapH->onTimer)
			AddTreeEventItem(nIndex, 10, m_hMapItem);
		if (m_mapH->onFocus)
			AddTreeEventItem(nIndex, 11, m_hMapItem);
		if (m_mapH->onClose)
			AddTreeEventItem(nIndex, 12, m_hMapItem);
		if (m_mapH->onDevice)
			AddTreeEventItem(nIndex, 13, m_hMapItem);
		if (m_mapH->onApprove)
			AddTreeEventItem(nIndex, 14, m_hMapItem);
		if (m_mapH->onKey)
			AddTreeEventItem(nIndex, 15, m_hMapItem);
		m_treeCtrl.RedrawWindow();
		break;
	default:
		{
			struct _formR* formR = m_pFormItem->getFormPoint(nIndex);
			switch (formR->kind)
			{
			case FM_EDIT:
				if (formR->onChangeL)
					AddTreeEventItem(nIndex, 1, hMyItem);
				if (formR->onCharL)
					AddTreeEventItem(nIndex, 2, hMyItem);
				if (formR->onKeyL)
					AddTreeEventItem(nIndex, 3, hMyItem);
				break;
			case FM_MEMO:
			case FM_COMBO:
				if (formR->onChange != -1)
					AddTreeEventItem(nIndex, 1, hMyItem);
				break;
			case FM_GRID:
				if (formR->onClickL)
					AddTreeEventItem(nIndex, 1, hMyItem);
				if (formR->onDblClkL)
					AddTreeEventItem(nIndex, 2, hMyItem);
				if (formR->onChangeL)
					AddTreeEventItem(nIndex, 3, hMyItem);
				if (formR->onCharL)
					AddTreeEventItem(nIndex, 4, hMyItem);
				if (formR->onKeyL)
					AddTreeEventItem(nIndex, 5, hMyItem);
				break;
			case FM_LABEL:
			case FM_OUT:
			case FM_PANEL:
			case FM_BUTTON:
			case FM_TAB:
			case FM_UTAB:
			case FM_TABLE:
				if (formR->onClickL)
					AddTreeEventItem(nIndex, 1, hMyItem);
				if (formR->onDblClkL)
					AddTreeEventItem(nIndex, 2, hMyItem);
				break;
			case FM_TREE:
			case FM_SHEET:
				if (formR->onClickL)
					AddTreeEventItem(nIndex, 1, hMyItem);
				break;
			case FM_CONTROL:
				{
					CString sym = m_pFormItem->getFormStr(nIndex, "str");
					long eventMask = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (long)sym.operator LPCTSTR());
					if (eventMask & CE_CLICK)
						if (formR->onClickL)
							AddTreeEventItem(nIndex, 1, hMyItem);
					if (eventMask & CE_DBLCLK)
						if (formR->onDblClkL)
							AddTreeEventItem(nIndex, 2, hMyItem);
					if (eventMask & CE_CHANGE)
						if (formR->onChangeL)
							AddTreeEventItem(nIndex, 3, hMyItem);
				}
				break;
			}
		}
		break;
	}
}

void CSelectionBar::AddTreeEventItem(int nIndex, int nEvent, HTREEITEM hParent)
{
	CString sStr;
	switch (nIndex)
	{
	case -1:
		switch (nEvent)
		{
		case 1:
			sStr = "Declaration";			break;
		case 2:
			sStr = "include Declaration";		break;
		case 3:
			sStr = "OnStart";			break;
		case 4:
			sStr = "OnSend";			break;
		case 5:
			sStr = "OnReceive";			break;
		case 6:
			sStr = "OnAlert(code, data, state)";	break;
		case 7:
			sStr = "OnService(data, length)";	break;
		case 8:
			sStr = "OnFile(upload, data, length)";	break;
		case 9:
			sStr = "OnSelect(data)";		break;
		case 10:
			sStr = "OnTimer";			break;
		case 11:
			sStr = "OnFocus";			break;
		case 12:
			sStr = "OnClose";			break;
		case 13:
			sStr = "OnDevice";			break;
		case 14:
			sStr = "OnApprove(key, data, length)";	break;
		case 15:
			sStr = "OnKey";				break;
		}
		break;
	default:
		{
			struct _formR* formR = m_pFormItem->getFormPoint(nIndex);
			switch (formR->kind)
			{
			case FM_EDIT:
				switch (nEvent)
				{
				case 1:
					sStr = "OnChange";	break;
				case 2:
					sStr = "OnCharChange";	break;
				case 3:
					sStr = "OnKey";		break;
				}
				break;
			case FM_MEMO:
			case FM_COMBO:
				if (nEvent == 1)
					sStr = "OnChange";
				break;
			case FM_GRID:
				switch (nEvent)
				{
				case 1:
					sStr = "OnClick";	break;
				case 2:
					sStr = "OnDoubleClick";	break;
				case 3:
					sStr = "OnChange";	break;
				case 4:
					sStr = "OnCharChange";	break;
				case 5:
					sStr = "OnKey";		break;
				}
				break;
			case FM_LABEL:
			case FM_OUT:
			case FM_PANEL:
			case FM_BUTTON:
			case FM_TAB:
			case FM_UTAB:
			case FM_TABLE:
				switch (nEvent)
				{
				case 1:
					sStr = "OnClick";	break;
				case 2:
					sStr = "OnDoubleClick";	break;
				}
				break;
			case FM_TREE:
			case FM_SHEET:
				if (nEvent == 1)
					sStr = "OnClick";
				break;
			case FM_CONTROL:
				switch (nEvent)
				{
				case 1:
					sStr = "OnClick";	break;
				case 2:
					sStr = "OnDoubleClick";	break;
				case 3:
					sStr = "OnChange";	break;
				}
				break;
			}
		}
	}
	HTREEITEM hItem = m_treeCtrl.InsertItem(sStr, 0, 0, hParent);
	m_treeCtrl.SetItemData(hItem, MAKELONG(nIndex, nEvent));
}

void CSelectionBar::UpdateEventItem(int nEvent, int nIndex, bool bAdd)
{
	if (nIndex == -2)
	{
		nIndex = -1;
		nEvent ++;
	}
	else if (nIndex == -1)
		nEvent += 2;
	HTREEITEM hItem = FindItem(MAKELONG(nIndex, nEvent), m_hMapItem);
	if (bAdd && !hItem)
	{
		if (nIndex == -1)
			hItem = m_hMapItem;
		else
			hItem = FindItem(MAKELONG(nIndex, 0), m_hMapItem);
		if (hItem)
		{
			AddTreeEventItem(nIndex, nEvent, hItem);
			m_treeCtrl.RedrawWindow();
		}
	}
	else if (!bAdd && hItem)
	{
		m_treeCtrl.DeleteItem(hItem);
		m_treeCtrl.RedrawWindow();
	}

}

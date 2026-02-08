// ToolandRButtonGrid_ListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "ToolandRButtonGrid_ListCtrl.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _LTOM4()
#include "ComboInListView.h"
#include "ToolandRButtonItemMgr.h"
#include "Conversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonGrid_ListCtrl

CToolandRButtonGrid_ListCtrl::CToolandRButtonGrid_ListCtrl()
{
	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;

	m_bDrag = TRUE;
	m_pRootItem = NULL;
	m_pNumericalInquiryOptionDlg = NULL;
}

CToolandRButtonGrid_ListCtrl::~CToolandRButtonGrid_ListCtrl()
{
}


BEGIN_MESSAGE_MAP(CToolandRButtonGrid_ListCtrl, CSuperGridCtrl)
	//{{AFX_MSG_MAP(CToolandRButtonGrid_ListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeydown)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DRAGCHARTDATA, OnDragChartData)
	ON_MESSAGE(WM_DROPCHARTDATA, OnDropChartData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonGrid_ListCtrl message handlers

int CToolandRButtonGrid_ListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(m_image.m_hImageList == NULL)
	{
		ML_SET_DEFAULT_RES();
		if(!m_image.Create(IDB_FOLDERS,16,1,RGB(0, 255, 255)))
			return -1;
	}

	SetImageList(&m_image, LVSIL_SMALL);
	CImageList *pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList)
		ImageList_GetIconSize(pImageList->m_hImageList, &m_cxImage, &m_cyImage);
	else
		return -1;	

	return 0;
}

void CToolandRButtonGrid_ListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	PostMessage(WM_KEYDOWN, (WPARAM)(int)VK_RETURN);	
	*pResult = 0;
}

void CToolandRButtonGrid_ListCtrl::InitializeGrid()
{
	InsertColumnHeader();
	InsertRoot();
//	SetColumnHeader();
//	SetINTERNALItem();
///	SetEXTERNALItem();
//	SetANOTEHRItem();
}

//helper function to copy CItemInfo used when drag/drop you must override this this function to suit your own CItemInfo class
CItemInfo* CToolandRButtonGrid_ListCtrl::CopyData(CItemInfo* lpSrc)
{
	ASSERT(lpSrc!=NULL);
	CItemInfo* lpDest = new CItemInfo;
	//well okay I put all the copy thing in one function, located in CItemInfo class, 
	//so you should check out this function, remember to modify this function each time you add new data to CItemInfo class.
	lpDest->CopyObjects(lpSrc);
	return lpDest;
}


//override, like red!
COLORREF CToolandRButtonGrid_ListCtrl::GetCellRGB()
{
	return RGB(192,0,0);
}


//this is my override of GetIcon, override this to set what ever icon suits you
int CToolandRButtonGrid_ListCtrl::GetIcon(const CTreeItem* pItem)
{
	if(pItem!=NULL)
	{
		int n = GetData(pItem)->GetImage();
		if(n!=-1)
			return n;
		
		int iImage = 0;
		if(ItemHasChildren(pItem))
		{
			IsCollapsed(pItem) ? iImage = 1/*close icon*/:iImage = 0;/*open icon*/
		}
		else
			iImage = 2;//doc icon
		return iImage;
	}
	return 0;
}



//override
void CToolandRButtonGrid_ListCtrl::OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem)
{
	CItemInfo *lp = GetData(lpItem);

	m_pItemInfoManager->ChangeItemInfo(lp->GetItemText(), plvItem);
	CSuperGridCtrl::OnUpdateListViewItem(lpItem, plvItem);
}

BOOL CToolandRButtonGrid_ListCtrl::OnItemLButtonDown(LVHITTESTINFO& ht)
{
	return 1;
}

//override called when OnLButtondown
void CToolandRButtonGrid_ListCtrl::OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
	//now I am sure I added a combobox some where, so check for this control
	CTreeItem* pSelItem = GetTreeItem(ht.iItem);
	if(pSelItem!=NULL)
	{	
		CItemInfo* pInfo = GetData(pSelItem);
		CItemInfo::CONTROLTYPE ctrlType;
		if(pInfo->GetControlType(ht.iSubItem-1, ctrlType))
		{	
			if(ctrlType==pInfo->CONTROLTYPE::combobox) 
			{
				CStringList* list=NULL;
				pInfo->GetListData(ht.iSubItem-1, list);
				CComboBox * pList = ShowList(ht.iItem, ht.iSubItem, list);
			}
		}	
		/*
		else //activate default edit control
			CSuperGridCtrl::OnControlLButtonDown(nFlags, point, ht);
		*/
	}		
}

BOOL CToolandRButtonGrid_ListCtrl::OnVkReturn()
{
	BOOL bResult=FALSE;
	int iItem = GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if( GetCurSubItem() != -1 && iItem != -1)
	{
		CTreeItem*pSelItem = GetTreeItem(iItem);
		if(pSelItem!=NULL)
		{	
			CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
			int iSubItem = Header_OrderToIndex(pHeader->m_hWnd, GetCurSubItem());
			CItemInfo* pInfo = GetData(pSelItem);
			CItemInfo::CONTROLTYPE ctrlType;
			if(pInfo->GetControlType(iSubItem-1, ctrlType))
			{	
				switch(ctrlType)
				{
					/*put in your own control here*/
					case pInfo->CONTROLTYPE::datecontrol:break;
					case pInfo->CONTROLTYPE::spinbutton:break;
					case pInfo->CONTROLTYPE::dropdownlistviewwhatevercontrol:break;
					case pInfo->CONTROLTYPE::combobox: 
						{
							CStringList* list=NULL;
							pInfo->GetListData(iSubItem-1, list);
							CComboBox * pList = ShowList(iItem, iSubItem, list);
							bResult=TRUE; //I'll handle it from here
						}break;
					default:break;
				}
			}
		}
	}
	return bResult;
}

#define IDC_COMBOBOXINLISTVIEW 0x1235
CComboBox* CToolandRButtonGrid_ListCtrl::ShowList(int nItem, int nCol, CStringList *lstItems)
{
	if(GetItemText(nItem, 0).Compare( _LTEXT4( C4_ZOOMIN_ZOOMOUT)) == 0) 
		SetComboListWhen_Extension_Recuction_Item(nItem, nCol, lstItems);

	CString strFind = GetItemText(nItem, nCol);
	//basic code start
	CRect rect;
	int offset = 0;
	// Make sure that the item is visible
	if( !EnsureVisible(nItem, TRUE)) return NULL;
	GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rect);
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}
	
	rect.left += offset;	
	rect.right = rect.left + GetColumnWidth(nCol);
	if(rect.right > rcClient.right) 
	   rect.right = rcClient.right;
	//basic code end

	rect.bottom += 10 * rect.Height();//dropdown area
	
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL|CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;
	CComboBox *pList = new CComboInListView(nItem, nCol, lstItems);
	pList->Create(dwStyle, rect, this, IDC_COMBOBOXINLISTVIEW);
	pList->ModifyStyleEx(0,WS_EX_CLIENTEDGE);//can we tell at all
	pList->SetHorizontalExtent(CalcHorzExtent(pList, lstItems));
	pList->ShowDropDown();
	pList->SelectString(-1, strFind.GetBuffer(1));
	// The returned pointer should not be saved
	return pList;
}



int CToolandRButtonGrid_ListCtrl::CalcHorzExtent(CWnd* pWnd, CStringList *pList)
{
	int nExtent=0;
	if(pWnd!=NULL)
	{
		CDC* pDC = pWnd->GetDC();
		HFONT hFont = (HFONT)pWnd->SendMessage(WM_GETFONT); //why not pWnd->GetFont();..I like the send thing alot and
		CFont *pFont = CFont::FromHandle(hFont);			//this way I get to use this function..cool :)
		if(pFont!=NULL)										//ya what ever makes me happy,.right :}
		{
			CFont* pOldFont = pDC->SelectObject(pFont);
			CSize newExtent;
			POSITION pos = pList->GetHeadPosition();
			while(pos != NULL)
			{
				CString str(pList->GetNext(pos));
				newExtent = pDC->GetTextExtent(str);
				newExtent.cx += 6;
				if (newExtent.cx > nExtent)
				{
					nExtent = newExtent.cx;
				}
			}
			pDC->SelectObject(pOldFont);
		}
		pWnd->ReleaseDC(pDC);
	}
	return nExtent;
}

BOOL CToolandRButtonGrid_ListCtrl::OnDeleteItem(CTreeItem* pItem, int nIndex)
{
	return 1;
}



BOOL CToolandRButtonGrid_ListCtrl::OnItemExpanding(CTreeItem *pItem, int iItem)
{
	return 1;
}


BOOL CToolandRButtonGrid_ListCtrl::OnItemExpanded(CTreeItem* pItem, int iItem)
{
	return 1;
}


BOOL CToolandRButtonGrid_ListCtrl::OnCollapsing(CTreeItem *pItem)
{
	return 1;
}



BOOL CToolandRButtonGrid_ListCtrl::OnItemCollapsed(CTreeItem *pItem)
{
	return 1;
}


CImageList *CToolandRButtonGrid_ListCtrl::CreateDragImageEx(int nItem)
{
	if(m_bDrag)
		return CSuperGridCtrl::CreateDragImageEx(GetDragItem());
	else
		return NULL;
}

void CToolandRButtonGrid_ListCtrl::InitDropTarget()
{
    BOOL bSuccess = m_dropTarget.Register(this);
	m_dropTarget.SetCurrentWindow(this);
    if (!bSuccess)
       AfxMessageBox("Ole Register Drop Target Failed");
}

LONG CToolandRButtonGrid_ListCtrl::OnDragChartData(UINT wParam, LPARAM lParam)
{
	UINT uHitTest = TVHT_ONITEM;
	CPoint point;
	point.x = LOWORD((DWORD)lParam);
	point.y = HIWORD((DWORD)lParam);
	return 0L;
}

LONG CToolandRButtonGrid_ListCtrl::OnDropChartData(UINT wParam, LPARAM lParam)
{
	ToolItem *pData = (ToolItem *)lParam;
	CString strItems = (LPSTR)(LPCTSTR)pData->ChartName;

	// REMOVE: 종찬(04/03/25) 내부툴바의 '수치조회창'버튼과 연동하지 않음.
//	if(_T("내부툴바") == m_strTabName)
//	if(strItems.Find(_T("수치조회창")) != -1)
//	{// 수치조회옵션에 추가함.
//		m_pNumericalInquiryOptionDlg->AddNumericalInquiryOption(0x1);
//	}

	int nCount = CDataConversion::GetStringDataCount(strItems, ";");
	for(int i = 0; i< nCount; i++) {
		CString Item = CDataConversion::GetStringData(strItems, ";");
		CItemInfo* pItemInfo = m_pItemInfoManager->GetItemInfowithItemName(Item);

		if(pItemInfo == NULL)
			return 0L;

		CItemInfo* pNewItemInfo = CopyData(pItemInfo);
		CTreeItem* pNew = InsertItem(m_pRootItem, pNewItemInfo);  			//insert the iteminfo with ParentPtr
		InsertOneItemToListCtrl(m_pRootItem, pNew, pNewItemInfo, TRUE);
	}
	SetRedraw(1);
	InvalidateRect(NULL);
	UpdateWindow();

	return 0L;
}

void CToolandRButtonGrid_ListCtrl::InsertColumnHeader()
{
	SetExtendedStyle(LVS_EX_GRIDLINES);  //|LVS_EX_CHECKBOXES);

	UINT uCols[] =
	{
		IDS_TOOLSET_FUNCTION,
		IDS_TOOLSET_LIMIT_1,
		IDS_TOOLSET_LIMIT_2
	};

	ML_SET_LANGUAGE_RES();
	CString strCols;
	LV_COLUMN   lvColumn;
	//initialize the columns
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 130;
	for(int x = 0; x < sizeof( uCols) / sizeof( UINT); x++)
	{
		if(x != 0) lvColumn.cx = 100;
		//make the secondary columns smaller
		strCols.LoadString( uCols[ x]);
		lvColumn.pszText = ( char *)( const char *)strCols;
		InsertColumn(x,&lvColumn);
    }
}

void CToolandRButtonGrid_ListCtrl::SetRootItem(const CString& szItemName, CItemInfo *lpItemInfo)
{
	//add item text
	lpItemInfo->SetItemText(szItemName);
	//add subitem text
	lpItemInfo->AddSubItemText(_T(""));      // 0 zero based subitems...
	lpItemInfo->AddSubItemText(_T(""));      // 1 zero based subitems...
	lpItemInfo->AddSubItemText(_T(""));      // 2 zero based subitems...
	lpItemInfo->AddSubItemText(_T(""));      // 3 zero based subitems...
}

void CToolandRButtonGrid_ListCtrl::InsertRoot()
{
	CItemInfo* lp = new CItemInfo();
	SetRootItem("Item", lp);
  //Create root item
	m_pRootItem = InsertRootItem(lp);//previous on N.Y.P.D we call it CreateTreeCtrl(lp)
	InsertRootItemToListCtrl(m_pRootItem);

	//could now expand one level
	Expand(m_pRootItem, 0 /*listview index 0*/); 
	UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
	SetItemState(0, uflag, uflag);
}

CString CToolandRButtonGrid_ListCtrl::GetListItemInfo()
{
	CString strValue = "";
	int Count = GetItemCount();
	for(int i = 0; i < Count ; i++)
	{
		CString temp = GetItemText(i,0);
		if( temp == "Item") continue;
		for(int y = 0 ; y <= 2; y++)
		{
			strValue += _LTOM4( GetItemText(i, y));
			strValue += ";";
			if(y == 2) strValue += "\r\n";
		}
	}
	return strValue;
}

// MODIFY: 종찬(04/03/17) 수치조회관련. 내부툴바 탭페이지와 수치조회탭페이지의 옵션값 연동을 위해서
void CToolandRButtonGrid_ListCtrl::SetListItemInfoMgrObject(CToolandRButtonItemMgr *pItemInfoMgr, LPCTSTR pcszTabName, CNumericalInquiryOptionDlg* pNumericalInquiryOptionDlg)
{
	m_pItemInfoManager = pItemInfoMgr;
	m_strTabName = pcszTabName;
	m_pNumericalInquiryOptionDlg = pNumericalInquiryOptionDlg;
}

void CToolandRButtonGrid_ListCtrl::InsertSavedItemInfo(const CString &strItemName, const CString &strLimit1, const CString &strLimit2)
{
	CItemInfo* pItemInfo = m_pItemInfoManager->ChangeItemInfo(strItemName, strLimit1, strLimit2);
	if(pItemInfo == NULL)
		return;

	CItemInfo* pItem = CopyData(pItemInfo);
	CTreeItem* pNew = InsertItem(m_pRootItem, pItem);  			//insert the iteminfo with ParentPtr
	InsertOneItemToListCtrl(m_pRootItem, pNew, pItem, TRUE);
}

void CToolandRButtonGrid_ListCtrl::SetComboListWhen_Extension_Recuction_Item(const int nItem, const int nColumn, CStringList *lstItems)
{
	CString strbefore = GetItemText(nItem, (nColumn-1));
	if( strbefore == _LTEXT4( C4_COUNT))
	{
		lstItems->RemoveAll();
		lstItems->AddTail( _LTEXT4( C4_10_UNIT));
		lstItems->AddTail( _LTEXT4( C4_20_UNIT));
		lstItems->AddTail( _LTEXT4( C4_30_UNIT));
		lstItems->AddTail( _LTEXT4( C4_40_UNIT));
		lstItems->AddTail( _LTEXT4( C4_50_UNIT));
		lstItems->AddTail("");
		lstItems->AddTail("");
	}
	else if( strbefore == _LTEXT4( C4_RATE)){
		lstItems->RemoveAll();
		lstItems->AddTail("10%");
		lstItems->AddTail("20%");
		lstItems->AddTail("30%");
		lstItems->AddTail("40%");
		lstItems->AddTail("50%");
		lstItems->AddTail("");
		lstItems->AddTail("");
	}
}


/*-------------------------------------------------------------------------------
 - Function    :  OnKeydown
 - Created at  :  2004-03-17   13:21
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Description :  수치조회관련. CSuperGridCtrl에서 따왔음. 소스 내용은 잘 모름. --;
 -------------------------------------------------------------------------------*/
void CToolandRButtonGrid_ListCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	switch(pLVKeyDow->wVKey)
	{
		case VK_SPACE: 
			{
				if(GetExtendedStyle() & LVS_EX_CHECKBOXES)
				{	
					int nIndex = GetSelectedItem();
					if(nIndex !=-1)
					{
						CTreeItem* pItem = GetTreeItem(nIndex);
						if(pItem!=NULL)
						{
							CItemInfo *pInfo = GetData(pItem);
							pInfo->SetCheck(!pInfo->GetCheck());
						}
					}
				}
			}break;
		case VK_DELETE: 
			{
				int nItem = GetSelectedItem();
				CString item = GetItemText(nItem, 0);
				//부모 일때는 지우지 않는다.
				if(item.CompareNoCase("Item") == 0)
					return;
				if(nItem!=-1)
				{
					// REMOVE: 종찬(04/03/25) 내부툴바의 '수치조회창'버튼과 연동하지 않음.
//					if(_T("내부툴바") == m_strTabName && item.Find(_T("수치조회창")) != -1)
//					{// 수치조회옵션에서 제거
//						m_pNumericalInquiryOptionDlg->SubNumericalInquiryOption(0x1);
//					}
					// REMOVE: 04/03/25

					CTreeItem* pSelItem = GetTreeItem(nItem);
					if(pSelItem != NULL)
					{
						if(OnDeleteItem(pSelItem, nItem))
							DeleteItemEx(pSelItem, nItem);
					}	

					// REMOVE: 종찬(04/03/25) 내부툴바의 '수치조회창'버튼과 연동하지 않음.
					// ADD: 종찬(04/03/17) 수치조회관련 옵션. 아이템갯수가 1개이하(즉, item만 남았을경우)
					// 일 경우는 모든 아이템이 지워졌으므로 내부툴바또한 사라진다. 그러므로 수치조회관련
					// 옵션 모두를 UnCheck한다.
//					if(_T("내부툴바") == m_strTabName && GetItemCount() <= 1)
//					{
//						m_pNumericalInquiryOptionDlg->SubNumericalInquiryOption(0x7);
//					}
				}
			}	break;
		case VK_MULTIPLY:
			{  
				int nIndex = GetSelectedItem();
				if(nIndex != -1)
				{
					CWaitCursor wait;
					SetRedraw(0);
					CTreeItem *pParent = GetTreeItem(nIndex);
					int nScroll=0;
					if(OnVKMultiply(pParent, nIndex))
					{	
						ExpandAll(pParent, nScroll);
					}		
					SetRedraw(1);
					RedrawItems(nIndex, nScroll);
					EnsureVisible(nScroll, TRUE);
				 }
			 }break;
		case VK_ADD:
			{
					int nIndex = GetSelectedItem();
					if(nIndex!=-1)
					{
						CWaitCursor wait;
						CTreeItem *pSelItem = GetTreeItem(nIndex);
						int nScrollIndex = 0;
						if(OnVKAdd(pSelItem, nIndex))
						{
							 nScrollIndex = Expand(pSelItem, nIndex);
						}
						CRect rc;
						GetItemRect(nIndex, rc, LVIR_BOUNDS);
						InvalidateRect(rc);
						UpdateWindow();
						EnsureVisible(nScrollIndex, 1);
					}

			}break;
		case VK_SUBTRACT:
			{
				int nIndex = GetSelectedItem();
				if(nIndex!=-1)
				{
					CWaitCursor wait;
					CTreeItem *pSelItem = GetTreeItem(nIndex);
					if(OnVkSubTract(pSelItem, nIndex))
					{
						Collapse(pSelItem);
					}
					CRect rc;
					GetItemRect(nIndex, rc, LVIR_BOUNDS);
					InvalidateRect(rc);
					UpdateWindow();
				}
			}break;
		default :break;
	}
	*pResult = 0;
}
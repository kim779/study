// PacketSuperGrid.cpp : implementation file
//

#include "stdafx.h"
#include "PacketSuperGrid.h"
#include "ComboInListView.h"
#include "PacketPage.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "ToolListData.h"								// for NUM_PACKETNAMEITEM

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketSuperGrid


CPacketSuperGrid::CPacketSuperGrid()
{
	m_bDrag = TRUE;
}

CPacketSuperGrid::~CPacketSuperGrid()
{
}


BEGIN_MESSAGE_MAP(CPacketSuperGrid, CSuperGridCtrl)
	//{{AFX_MSG_MAP(CPacketSuperGrid)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketSuperGrid message handlers

int CPacketSuperGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	/////////////////////
	//remember this	
	///////////////////
	//associate imagelist with listviewctrl	

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

void CPacketSuperGrid::InitializeGrid()
{
	SetColumnHeader();
	SetPacketRootInit();
	SetUserDefineRootInit();
}

//helper function to copy CItemInfo used when drag/drop you must override this this function to suit your own CItemInfo class
CItemInfo* CPacketSuperGrid::CopyData(CItemInfo* lpSrc)
{
	ASSERT(lpSrc!=NULL);
	CItemInfo* lpDest = new CItemInfo;
	//well okay I put all the copy thing in one function, located in CItemInfo class, 
	//so you should check out this function, remember to modify this function each time you add new data to CItemInfo class.
	lpDest->CopyObjects(lpSrc);
	return lpDest;
}


//override, like red!
COLORREF CPacketSuperGrid::GetCellRGB()
{
	return RGB(192,0,0);
}


//this is my override of GetIcon, override this to set what ever icon suits you
int CPacketSuperGrid::GetIcon(const CTreeItem* pItem)
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
void CPacketSuperGrid::OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem)
{
	//put some extra validation here 
	CString str = (CString)plvItem->pszText;
/*
	CItemInfo *lp = GetData(lpItem);
	if(plvItem->iSubItem == 3 && plvItem->iItem == 1 || plvItem->iItem == 2 ) {
		CString strLength;
		strLength.Format("%d", str.GetLength());
		lp->SetSubItemText(plvItem->iSubItem-2, strLength);
	    UpdateData(lpItem, lp);
	}*/
/*
	if(!str.Compare(_T("Bugs: Impossible")))
	{
		str+=_T(" (selfconfidence allright)");//that's valid enough	:�)
		CItemInfo *lp = GetData(lpItem);
		if(lp!=NULL)
		{
			if(plvItem->iSubItem==0)
				lp->SetItemText(str);
			else //subitem data 
				lp->SetSubItemText(plvItem->iSubItem-1, str);
		   UpdateData(lpItem, lp); //update internal nodes
		}
		SetItemText(plvItem->iItem, plvItem->iSubItem, str.GetBuffer(1));
	}
	else
		CSuperGridCtrl::OnUpdateListViewItem(lpItem, plvItem);
*/
		CSuperGridCtrl::OnUpdateListViewItem(lpItem, plvItem);
}



			
BOOL CPacketSuperGrid::OnItemLButtonDown(LVHITTESTINFO& ht)
{
	return 1;
}

//override called when OnLButtondown
void CPacketSuperGrid::OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
	//now I am sure I added a combobox some where, so check for this control
	CTreeItem* pSelItem = GetTreeItem(ht.iItem);
	if(pSelItem!=NULL)
	{	
		CItemInfo* pInfo = GetData(pSelItem);
		CItemInfo::CONTROLTYPE ctrlType;
		if(pInfo->GetControlType(ht.iSubItem-1, ctrlType)) {	
			if(ctrlType==pInfo->CONTROLTYPE::combobox) {
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



BOOL CPacketSuperGrid::OnVkReturn()
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
CComboBox* CPacketSuperGrid::ShowList(int nItem, int nCol, CStringList *lstItems)
{
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



int CPacketSuperGrid::CalcHorzExtent(CWnd* pWnd, CStringList *pList)
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

//HOWTO: 
void CPacketSuperGrid::HowToInsertItemsAfterTheGridHasBeenInitialized(const int nIndex, const int nCheck, const CString& str,const CString& szField, const CString& szLength, const CString& szType, const CString& szReal)
{

	for(int i = 0; i < GetItemCount() ; i++) {
		CTreeItem *pItem = GetTreeItem(i);
		if((GetData(pItem)->GetItemText()).CompareNoCase(str) == 0)
			return;
	}

	CTreeItem *pSelItem = GetTreeItem(nIndex);
	if(pSelItem != NULL) {
		SetRedraw(0);
		BOOL bUpdate = FALSE;
		if(!IsCollapsed(pSelItem))
			bUpdate = TRUE;//Children are expanded, want to see update right away if not no visual update

		CItemInfo* lpRelative = new CItemInfo();

		lpRelative->SetCheck(nCheck);
		lpRelative->SetItemText(str);
		lpRelative->AddSubItemText(szField);
		lpRelative->AddSubItemText(szLength);
		int index = GetItemIndex(str);
		SetThirdSubItem(index, szType, str, lpRelative);
		SetFourSubItem(szReal, lpRelative);

		CTreeItem* pParent = InsertItem(pSelItem, lpRelative, bUpdate);
		InsertItemToListCtrl(pSelItem, pParent, lpRelative, bUpdate);
	
		SetRedraw(1);
		InvalidateRect(NULL);
		UpdateWindow();
	}

}

BOOL CPacketSuperGrid::OnDeleteItem(CTreeItem* pItem, int nIndex)
{
	return 1;
}



BOOL CPacketSuperGrid::OnItemExpanding(CTreeItem *pItem, int iItem)
{
	return 1;
}


BOOL CPacketSuperGrid::OnItemExpanded(CTreeItem* pItem, int iItem)
{
	return 1;
}


BOOL CPacketSuperGrid::OnCollapsing(CTreeItem *pItem)
{
	return 1;
}



BOOL CPacketSuperGrid::OnItemCollapsed(CTreeItem *pItem)
{
	return 1;
}


CImageList *CPacketSuperGrid::CreateDragImageEx(int nItem)
{
		if(m_bDrag)
			return CSuperGridCtrl::CreateDragImageEx(GetDragItem());
		else
			return NULL;
}

void CPacketSuperGrid::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	CSuperGridCtrl::OnDblclk(pNMHDR, pResult);
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    int itemindex = pNMListView->iItem;

	CString strItem = GetItemText(itemindex, 0);
	bool input = false;
	if(strItem == _LTEXT6( C6_INPUT_ITEM))
		input = true;

	if(input)
	{
		int nIndex = GetSelectedItem();
		if(nIndex != -1)
		{
			ML_SET_LANGUAGE_RES();
			CPacketUserDefine dlg( m_hOcxWnd);
			if(dlg.DoModal()==IDOK)
			{
				CString szReal;
				if(dlg.m_bCheckReal == 0)
					szReal = _LTEXT6( C6_NO);
				else
					szReal = _LTEXT6( C6_YES);
				//nami
	//			HowToInsertItemsAfterTheGridHasBeenInitialized(nIndex, 1, dlg.m_strPacketName ,dlg.m_strFieldName ,dlg.m_strLength , dlg.m_strType, szReal);
				HowToInsertItemsAfterTheGridHasBeenInitialized(nIndex, 1, dlg.m_strPacketName ,dlg.m_strFieldName ,dlg.m_strLength , dlg.m_strDetailPacketType, szReal);
			}
		}
	}
	else
		PostMessage(WM_KEYDOWN, (WPARAM)(int)VK_RETURN);
	*pResult = 0;
}


void CPacketSuperGrid::AddItem(const int nCheck, const CString& szpacket, const CString& szField, const CString& szLength, const CString& szType, const CString& szReal)
{
	HowToInsertItemsAfterTheGridHasBeenInitialized(0, nCheck, szpacket, szField,  szLength, szType, szReal);
}


void CPacketSuperGrid::SetSubItem(const int nItemIndex, const CString& szPacketName, CItemInfo* lpItemInfo)
{
	int nCol = GetNumCol();
	for(int y=0 ;  y < nCol ; y++) {
		switch(y) {
			case 0:
			case 1:
				lpItemInfo->AddSubItemText(_T(""));     // 1 zero based subitems...
				break;
			case 2:
				SetThirdSubItem(nItemIndex,"",szPacketName, lpItemInfo);
				break;
			case 3:
				SetFourSubItem( _LTEXT6( C6_NO), lpItemInfo);
				break;
		}
	}
}

CPacketSuperGrid::DATATYPE CPacketSuperGrid::GetDataType(const int nDataType)
{
	switch(nDataType)
	{
		case CPacketSuperGrid::SECONDDATE_ITEM:		// (2007/3/14 - Seung-Won, Bae) Dummy Packet for Date FID in Koscom
		case CPacketSuperGrid::INFODATE_ITEM:
			return CPacketSuperGrid::DATE;
		case CPacketSuperGrid::TEXT_ITEM:
			return CPacketSuperGrid::TEXT;
		case CPacketSuperGrid::STARTPRICE_ITEM:
		case CPacketSuperGrid::TOPPRICE_ITEM:
		case CPacketSuperGrid::BOTTOMPRICE_ITEM:
		case CPacketSuperGrid::ENDPRICE_ITEM:
			return CPacketSuperGrid::PRICE;
		case CPacketSuperGrid::VOLUME_ITEM:
		case CPacketSuperGrid::ACCUMULATEVOLUME_ITEM:
		case CPacketSuperGrid::UP_ITEMVOLUME_ITEM:
		case CPacketSuperGrid::DOWN_ITEMVOLUME_ITEM:
		case CPacketSuperGrid::LEAD_ITEMVOLUME_ITEM:
			return CPacketSuperGrid::VOLUME;
		case CPacketSuperGrid::STOCKMULTIPLE_ITEM:
			return CPacketSuperGrid::STOCKMULTIPLE;
		case CPacketSuperGrid::EXCHANGERATE_ITEM:
			return CPacketSuperGrid::EXCHANGERATE;
		default :
			return CPacketSuperGrid::ANOTHER;
	}
	return ANOTHER;
}

void CPacketSuperGrid::SetThirdDATEItem(const CString& szDefaultItemofCombo, CItemInfo *lpItemInfo)
{
	if(szDefaultItemofCombo.GetLength() == 0)
		lpItemInfo->AddSubItemText(_T("YYYYMMDD"));      // 2 zero based subitems..
	else
		lpItemInfo->AddSubItemText(szDefaultItemofCombo);      // 2 zero based subitems..
	lpItemInfo->SetControlType(lpItemInfo->CONTROLTYPE::combobox, 2);
	//initalized list for the edit
	CStringList list;
	list.AddTail("YYYYMMDD");
	list.AddTail("YYMMDD");
	list.AddTail("YYYYMM");
	list.AddTail("YYMM");
	list.AddTail("MMDD");
	list.AddTail("MMDDHH");
	list.AddTail("MMDDHHMM");
	list.AddTail("DDHH");
	list.AddTail("DDHHMM");
	list.AddTail("DDHHMMSS");
	list.AddTail("HHMMSS");
//	list.AddTail("HHMMSSNN");
	list.AddTail("");
	list.AddTail("");
	lpItemInfo->SetListData(2, &list);
}

void CPacketSuperGrid::SetThirdTEXTItem(const CString& szDefaultItemofCombo, CItemInfo *lpItemInfo)
{
	if(szDefaultItemofCombo.GetLength() == 0)
		lpItemInfo->AddSubItemText( _LTEXT6( C6_CHARACTER));      // 2 zero based subitems..
	else
		lpItemInfo->AddSubItemText(szDefaultItemofCombo);      // 2 zero based subitems..
}

void CPacketSuperGrid::SetThirdPRICEItem(const CString& szDefaultItemofCombo, CItemInfo *lpItemInfo)
{
	if(szDefaultItemofCombo.GetLength() == 0)
		lpItemInfo->AddSubItemText(_T("x 1"));      // 2 zero based subitems..
	else
		lpItemInfo->AddSubItemText(szDefaultItemofCombo);      // 2 zero based subitems..

	lpItemInfo->SetControlType(lpItemInfo->CONTROLTYPE::combobox, 2);
	CStringList list;
	//initalized list for the edit
	list.AddTail("x 1000");
	list.AddTail("x 1");
	list.AddTail("x 0.1");
	list.AddTail("x 0.01");
	list.AddTail("x 0.001");
	list.AddTail("x 0.0001");
	list.AddTail("x 0.000001");
	list.AddTail("");
	lpItemInfo->SetListData(2, &list);
}

void CPacketSuperGrid::SetThirdVOLUMEItem(const CString& szPacketName, const CString& szDefaultItemofCombo, CItemInfo *lpItemInfo)
{
	if(szDefaultItemofCombo.GetLength() == 0)	{
		if(szPacketName.Compare( _MTEXT( C0_VOLUME)) == 0)
			lpItemInfo->AddSubItemText(_T("x 1"));      // 2 zero based subitems..
		else 
			lpItemInfo->AddSubItemText(_T("x 1000"));      // 2 zero based subitems..
	}
	else
		lpItemInfo->AddSubItemText(szDefaultItemofCombo);      // 2 zero based subitems..

	lpItemInfo->SetControlType(lpItemInfo->CONTROLTYPE::combobox, 2);
	CStringList list;
	//initalized list for the edit
	list.AddTail("x 1");
	list.AddTail("x 100");
	list.AddTail("x 1000");
	list.AddTail("x 100000");
	list.AddTail("x 100000000");
	list.AddTail("");
	list.AddTail("");
	lpItemInfo->SetListData(2, &list);	
}

void CPacketSuperGrid::SetThirdSTOCKMULTIPLEItem(const CString& szDefaultItemofCombo, CItemInfo *lpItemInfo)
{
	if(szDefaultItemofCombo.GetLength() == 0)
		lpItemInfo->AddSubItemText(_T("x 1%"));      // 2 zero based subitems..
	else
		lpItemInfo->AddSubItemText(szDefaultItemofCombo);      // 2 zero based subitems..

	lpItemInfo->SetControlType(lpItemInfo->CONTROLTYPE::combobox, 2);

	CStringList list;
	//initalized list for the edit
	list.AddTail("x 0.01%");
	list.AddTail("x 0.1%");
	list.AddTail("x 1%");
	list.AddTail( _LTEXT6( C6_POINT));
	list.AddTail("");
	list.AddTail("");
	lpItemInfo->SetListData(2, &list);	

}

void CPacketSuperGrid::SetThirdEXCHANGERATEItem(const CString& szDefaultItemofCombo, CItemInfo *lpItemInfo)
{
	if(szDefaultItemofCombo.GetLength() == 0)
		lpItemInfo->AddSubItemText(_T("$"));      // 2 zero based subitems..
	else
		lpItemInfo->AddSubItemText(szDefaultItemofCombo);      // 2 zero based subitems..

	lpItemInfo->SetControlType(lpItemInfo->CONTROLTYPE::combobox, 2);
	CStringList list;
	//initalized list for the edit
	list.AddTail("$");
	list.AddTail("￥");
	list.AddTail("￡");
	list.AddTail("");
	list.AddTail("");
	lpItemInfo->SetListData(2, &list);	
}

void CPacketSuperGrid::SetFourSubItem(const CString& szReal, CItemInfo *lpItemInfo)
{
	lpItemInfo->AddSubItemText(szReal);
	lpItemInfo->SetControlType(lpItemInfo->CONTROLTYPE::combobox, 3);

	CStringList list;
	list.AddTail( _LTEXT6( C6_NO));
	list.AddTail( _LTEXT6( C6_YES));
	list.AddTail("");
	list.AddTail("");
	lpItemInfo->SetListData(3, &list);	
}

void CPacketSuperGrid::SetThirdSubItem(const int nItemIndex, const CString& szDefaultItemofCombo, const CString& szPacketName, CItemInfo *lpItemInfo)
{
	DATATYPE type = GetDataType(nItemIndex);
	if(type == CPacketSuperGrid::DATE)
		SetThirdDATEItem(szDefaultItemofCombo, lpItemInfo);
	else if(type == CPacketSuperGrid::TEXT)
		SetThirdTEXTItem(szDefaultItemofCombo,lpItemInfo);
	else if(type == CPacketSuperGrid::PRICE)
		SetThirdPRICEItem(szDefaultItemofCombo,lpItemInfo);
	else if(type == CPacketSuperGrid::VOLUME)
		SetThirdVOLUMEItem(szPacketName,szDefaultItemofCombo, lpItemInfo);
	else if(type == CPacketSuperGrid::STOCKMULTIPLE)
		SetThirdSTOCKMULTIPLEItem(szDefaultItemofCombo,lpItemInfo);
	else if(type == CPacketSuperGrid::EXCHANGERATE)
		SetThirdEXCHANGERATEItem(szDefaultItemofCombo,lpItemInfo);
	else 
		lpItemInfo->AddSubItemText(_T(szDefaultItemofCombo));     

/*
	switch(type)
	{
		case ANOTHER:
			lpItemInfo->AddSubItemText(_T(""));      // 1 zero based subitems..
			break;
		case DATE:
			SetDATEItem(szDefaultItemofCombo, lpItemInfo);
			break;
		case TEXT:
			SetTEXTItem(szDefaultItemofCombo,lpItemInfo);
			break;
		case PRICE:
			SetPRICEItem(szDefaultItemofCombo,lpItemInfo);
			break;
		case VOLUME:
			SetVOLUMEItem(szPacketName,szDefaultItemofCombo, lpItemInfo);
			break;
		case STOCKMULTIPLE:
			SetSTOCKMULTIPLEItem(szDefaultItemofCombo,lpItemInfo);
			break;
		case EXCHANGERATE:
			SetEXCHANGERATEItem(szDefaultItemofCombo,lpItemInfo);
			break;
	}*/
}

int CPacketSuperGrid::GetItemIndex(const CString& szPacketName)
{
	for(int i=0; i < NUM_PACKETNAMEITEM; i++)
	{
		if(szPacketName.CompareNoCase( _LTEXTN( PACKET_NAME, ePacketNameItem[i])) == 0){
			return i;		
		}
	}
	return -1;
}

void CPacketSuperGrid::SetRootItem(const CString& szItemName, CItemInfo *lpItemInfo)
{
	//add item text
	lpItemInfo->SetItemText(szItemName);
	//add subitem text
	lpItemInfo->AddSubItemText(_T(""));      // 0 zero based subitems...
	lpItemInfo->AddSubItemText(_T(""));      // 1 zero based subitems...
	lpItemInfo->AddSubItemText(_T(""));      // 2 zero based subitems...
	lpItemInfo->AddSubItemText(_T(""));      // 3 zero based subitems...
}

void CPacketSuperGrid::SetColumnHeader()
{
	SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);

	UINT uCols[] =
	{
		IDS_PACKETGRID_ITEMNAME,
		IDS_PACKETGRID_FIELDNAME,
		IDS_PACKETGRID_LENGTH,
		IDS_PACKETGRID_TYPE,
		IDS_PACKETGRID_REALTYPE
	};
	int Size[] = { 150, 90, 50, 100, 60 };

	ML_SET_LANGUAGE_RES();
	CString strCols;
	LV_COLUMN   lvColumn;
	//initialize the columns
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 150;
	for(int x = 0; x < sizeof( uCols) / sizeof( UINT); x++)
	{
		lvColumn.cx = Size[x];
		strCols.LoadString( uCols[ x]);
		lvColumn.pszText = ( char *)( const char *)strCols;
		InsertColumn(x,&lvColumn);
    }
}

void CPacketSuperGrid::SetPacketRootInit()
{
//내부
	CItemInfo* lp = new CItemInfo();
	SetRootItem("Packet", lp);
  //Create root item
	CTreeItem * pRoot = InsertRootItem(lp);//previous on N.Y.P.D we call it CreateTreeCtrl(lp)
	InsertRootItemToListCtrl(pRoot);

	if( pRoot == NULL ) return;

	if((((CPacketPage *)GetParent())->GetReceivedAllDataFromOcx()).GetLength() == 0)
	{
		for(int i=0; i < NUM_PACKETNAMEITEM; i++)
		{
			CItemInfo* lpItemInfo = new CItemInfo();
			lpItemInfo->SetItemText( _LTEXTN( PACKET_NAME, ePacketNameItem[i]));  //add items text
			SetSubItem(i, _MTEXTN( ePacketNameItem[i]), lpItemInfo); //add subitem text
			CTreeItem* pParent = InsertItem(pRoot, lpItemInfo);  			//insert the iteminfo with ParentPtr
			InsertItemToListCtrl(pRoot, pParent, lpItemInfo);
		}
	}
	//could now expand one level
	Expand(pRoot, 0 /*listview index 0*/); 
	UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
	SetItemState(0, uflag, uflag);
}

void CPacketSuperGrid::SetUserDefineRootInit()
{
//사용자정의
	{
		CItemInfo* lpuser = new CItemInfo();
		SetRootItem( _LTEXT6( C6_USER_DEFINE), lpuser);
		CTreeItem * pRoot2 = InsertRootItem(lpuser);
		InsertRootItemToListCtrl(pRoot2);

		if( pRoot2 == NULL )
			return;

		CItemInfo* lpItemInfo = new CItemInfo();
		SetRootItem( _LTEXT6( C6_INPUT_ITEM), lpItemInfo);
		CTreeItem* pParent = InsertItem(pRoot2, lpItemInfo);
		InsertItemToListCtrl(pRoot2, pParent, lpItemInfo);

		//could now expand one level
		Expand(pRoot2, NodeToIndex(pRoot2) ); 
		UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
		SetItemState(0, uflag, uflag);
	}
}

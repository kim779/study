// AxMTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AxMCtrl.h"
#include "IndexSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxMTreeCtrl

CAxMTreeCtrl::CAxMTreeCtrl()
{
	m_hitemDrag = NULL;
	m_hitemDrop= NULL;
	m_pDragImage = NULL;
	
	m_bDrag = false;

	m_messageHandler = NULL;
}

CAxMTreeCtrl::~CAxMTreeCtrl()
{
	if (m_pDragImage)
	{
		delete m_pDragImage;
		m_pDragImage = NULL;
	}
}

IMPLEMENT_DYNAMIC(CAxMTreeCtrl, CTreeCtrl)
BEGIN_MESSAGE_MAP(CAxMTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CAxMTreeCtrl)
	ON_MESSAGE(AXM_SETMSGHANDLER, OnSetMessageHandler)
	ON_MESSAGE(AXM_GETMSGHANDLER, OnGetMessageHandler)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()	
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxMTreeCtrl message handlers

void CAxMTreeCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	HTREEITEM hItem;
	switch (nChar)
	{
	case VK_SPACE:
		hItem = GetSelectedItem();

		//If the space key is hit, and the item is a combo item, then toggle the check value
		if (IsCheckBox(hItem))
		{
			BOOL bEnable;
			VERIFY(GetCheckBoxEnable(hItem, bEnable));
			if (bEnable)
			{
				BOOL bCheck;
				VERIFY(GetCheckBox(hItem, bCheck));
				VERIFY(SetCheckBox(hItem, !bCheck));
				BOOL result = Notify(WM_AXUSER, (WPARAM)this);
				 
				if (result <= 0) { // return false
					SetCheckBox(hItem, bCheck);
				}
			}
			else
			{
				//Pass on to the parent since we didn't handle it
				CTreeCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
			}
		}
		else if (IsRadioButton(hItem)) //If the item is a radio item, then check it and uncheck all other items
		{
			BOOL bEnable;
			VERIFY(GetRadioButtonEnable(hItem, bEnable));
			if (bEnable)
			{
				//Check the radio button if not already checked
				BOOL bCheck;
				VERIFY(GetRadioButton(hItem, bCheck));
				if (!bCheck)
					VERIFY(SetRadioButton(hItem));

				//** macho remark GetParent()->SendMessage(WM_AXUSER, 0, 0);
				Notify(WM_AXUSER, (WPARAM)this);
			}
			else
			{
				//Pass on to the parent since we didn't handle it
				CTreeCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
			}
		}
		else
		{
			//Pass on to the parent since we didn't handle it
			CTreeCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
		}
		break;
	case VK_UP:
	case VK_DOWN:
	case VK_PRIOR:
	case VK_NEXT:
		hItem = GetSelectedItem();
		Select(hItem, TVGN_DROPHILITE);
		break;
	default:
		//Pass on to the parent since we didn't handle it
		CTreeCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
		break;
	}
	
	CTreeCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CAxMTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UINT uFlags=0;
	HTREEITEM hItem = HitTest(point, &uFlags);

	//If the mouse was over the label, icon or to the left or right of the item ?
	if ((uFlags & TVHT_ONITEM))// || (uFlags & TVHT_ONITEMINDENT) || (uFlags & TVHT_ONITEMRIGHT))
	{
		int result = Notify(WM_AXITEMCLICK, (WPARAM)hItem, (WPARAM)this);
		BOOL bEnable = FALSE;
		if (IsCheckBox(hItem))
		{
			if (result < 0) {
				return;
			}

			VERIFY(GetCheckBoxEnable(hItem, bEnable));
			if (bEnable)
			{
				BOOL	bCheck;
				CRect	rc;
				GetItemRect(hItem, rc, true);	// get text rect
				if (!rc.PtInRect(point)) //Toggle the state of check items
				{
					VERIFY(GetCheckBox(hItem, bCheck));
					VERIFY(SetCheckBox(hItem, !bCheck));
					Select(hItem, TVGN_DROPHILITE);
					CTreeCtrl::OnLButtonDown(nFlags, point);
				}
				else
				{
//////////////////////////////////////////////
					Select(hItem, TVGN_DROPHILITE);
					OnDrag(point, hItem);
/*					m_hitemDrag = hItem;
					Select(m_hitemDrag, TVGN_DROPHILITE);
					m_bDrag = true;
*/
//////////////////////////////////////////////
					CTreeCtrl::OnLButtonDown(nFlags, point);
					return;
				}
			}
		}
		else if (IsRadioButton(hItem))
		{
			VERIFY(GetRadioButtonEnable(hItem, bEnable));
			if (bEnable)
			{
				//Check the radio button if not already checked
				BOOL bCheck;
				VERIFY(GetRadioButton(hItem, bCheck));
				if (!bCheck)
					VERIFY(SetRadioButton(hItem));

				Select(hItem, TVGN_CARET);
				Select(hItem, TVGN_DROPHILITE);
			}
		}
		else
		{
			Select(hItem, TVGN_CARET);
			Select(hItem, TVGN_DROPHILITE);
		}

		//Pass on to the parent now that we that we have done our stuff
		CTreeCtrl::OnLButtonDown(nFlags, point);
		//** macho remark
		/**
		if (bEnable)	// message send for parent
			GetParent()->SendMessage(WM_AXUSER, 0, 0);
		**/
		if (bEnable)
			Notify(WM_AXUSER, (WPARAM)this);
	}
	else
	{
		//Pass on to the parent since we didn't handle it
		CTreeCtrl::OnLButtonDown(nFlags, point);
	}
}

void CAxMTreeCtrl::OnDrag(CPoint point, HTREEITEM hItem)
{
	if (GetDlgCtrlID() != 1005 && GetDlgCtrlID() != 1007)	// 관심종목,보유종목
		return;

	CString	strText = GetItemText(hItem);
	if (strText.IsEmpty() || strText.GetLength() < 6)
		return;

	int	index = strText.Find(" ");
	if (index > 0)
		strText = strText.Left(index);
	if (strText.GetLength() != 6)
		return;

	CString	sDragData = "1301\t" + strText;

	// Create global memory for sharing dragdrop text
	HGLOBAL hgData=GlobalAlloc(GPTR, sDragData.GetLength() + 1);   
	ASSERT(hgData!=NULL);
	
	// Lock global data (get pointer)
	LPSTR   lpData = (LPSTR)GlobalLock(hgData);
	ASSERT(lpData!=NULL);
	
	CopyMemory(lpData, sDragData.operator LPCTSTR(), sDragData.GetLength() + 1);
	
	if (!sDragData.GetLength())    // If no string, don't do dragdrop
	{
		GlobalUnlock(hgData);
		return;
	}
	
	// Get the client rect. Pass this into DoDragDrop
	// so the dragdrop will not be initiated until text is dragged out of the control
	RECT rClient;
	GetClientRect(&rClient);
	
	// Cache the data, and initiate DragDrop
	m_COleDataSource.CacheGlobalData(CF_TEXT, hgData);   
	
	// Set drag/drop source rectangle (screen coord) so we don't drop on the source rectangle
	DROPEFFECT dropEffect=m_COleDataSource.DoDragDrop( DROPEFFECT_COPY|DROPEFFECT_MOVE,(LPCRECT)&rClient );  
	
	// Clear the Data Source's cache
	m_COleDataSource.Empty();
}

void CAxMTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDrag) 
	{
		if (m_pDragImage == NULL)
		{
			m_pDragImage = CreateDragImage(m_hitemDrag);
							
			if (!m_pDragImage)
				return;	
			
			m_pDragImage->BeginDrag(0, CPoint(-15,-15));
			m_pDragImage->SetBkColor(RGB(255, 0, 0));
			POINT pt = point;
			ClientToScreen( &pt );
			m_pDragImage->DragEnter(NULL, pt);
			SetCapture();
		}

		HTREEITEM	hitem;
		UINT		flags;
		POINT		pt = point;

		ClientToScreen( &pt );
		CImageList::DragMove(pt);
		ScreenToClient(&pt);

		if ((hitem = HitTest(pt, &flags)) != NULL)
		{
			CImageList::DragShowNolock(FALSE);
			SelectDropTarget(hitem);
			CImageList::DragShowNolock(TRUE);		
		}
		
	}
	
	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CAxMTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDrag)
	{
		HTREEITEM	hItem = NULL;
		UINT		flags = 0;
		
		if (m_pDragImage)
		{
			CImageList::DragLeave(this);
			CImageList::EndDrag();
			ReleaseCapture();
			m_pDragImage->DeleteImageList();
			delete m_pDragImage;
			m_pDragImage = NULL;
		}
		
		m_bDrag = false;

		if (m_hitemDrag == NULL)
		{
			Select(m_hitemDrag, TVGN_DROPHILITE);
			m_hitemDrag = NULL;
			Invalidate(FALSE);

			CTreeCtrl::OnLButtonUp(nFlags, point);
			return;
		}

		if ((m_hitemDrop = HitTest(point, &flags)) != NULL)
		{
			SelectItem(m_hitemDrop);
			Select(m_hitemDrop, TVGN_DROPHILITE);
			//** macho remark GetParent()->SendMessage(WM_AXDROPITEM, (WPARAM)m_hitemDrag, (LPARAM)m_hitemDrop);
			Notify(WM_AXDROPITEM, (WPARAM)m_hitemDrag, (LPARAM)m_hitemDrop);
		}
		else
		{
			SelectItem(m_hitemDrag);
			Select(m_hitemDrag, TVGN_DROPHILITE);
			//** macho remark GetParent()->SendMessage(WM_AXDROPITEM, (WPARAM)m_hitemDrag, (LPARAM)NULL);
			Notify(WM_AXDROPITEM, (WPARAM)m_hitemDrag);
		}

		Invalidate(FALSE);
	}

	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CAxMTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	UINT uFlags=0;
	HTREEITEM hItem = HitTest(point, &uFlags);

	//If the mouse was over the label, icon or to the left or right of the item ?
	if ((uFlags & TVHT_ONITEM))// || (uFlags & TVHT_ONITEMINDENT) || (uFlags & TVHT_ONITEMRIGHT))
	{
		int result = Notify(WM_AXITEMCLICK, (WPARAM)hItem, (WPARAM)this);
		if (result >= 0) {
			BOOL bEnable = FALSE;
			if (IsCheckBox(hItem))
			{
				VERIFY(GetCheckBoxEnable(hItem, bEnable));
				if (bEnable)
				{
					BOOL	bCheck;
					CRect	rc;
					GetItemRect(hItem, rc, true);	// get text rect
					if (rc.PtInRect(point)) //Toggle the state of check items
					{
						VERIFY(GetCheckBox(hItem, bCheck));

						if (!bCheck)	
							VERIFY(SetCheckBox(hItem, !bCheck));
					}
				}
				else
				{
					CTreeCtrl::OnLButtonDblClk(nFlags, point);
					return;
				}
			}

			if (bEnable)
				Notify(WM_AXUSER, (WPARAM)this);
		}
	}
	
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}

void CAxMTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_messageHandler) {
		CTreeCtrl::OnRButtonDown(nFlags, point);
		return;
	}

	UINT uFlags=0;
	HTREEITEM hItem = HitTest(point, &uFlags);
	Select(hItem, TVGN_CARET);
	Select(hItem, TVGN_DROPHILITE);
	
	int index = Notify(WM_AXRMOUSE, getBookMark); 


	///**
	CMenu cMenu;
	cMenu.CreatePopupMenu();

	switch (index)
	{
	case 0:	// nothing
		cMenu.DestroyMenu();	
		CTreeCtrl::OnRButtonDown(nFlags, point);	
		return;
	case 1:	// 이미 등록된 리스트
		cMenu.DestroyMenu();	
		CTreeCtrl::OnRButtonDown(nFlags, point);	
		return;
//		cMenu.AppendMenu(MFT_STRING, 4, _T("기본값 설정"));
//		break;
	case 2:	// 리스트
		cMenu.AppendMenu(MFT_STRING, 2, _T("즐겨찾기에 추가"));
		//cMenu.AppendMenu(MFT_STRING, 4, _T("기본값 설정"));
		break;
	case 3:	// 즐겨찾기 리스트
		cMenu.AppendMenu(MFT_STRING, 3, _T("즐겨찾기에서 삭제"));
		//cMenu.AppendMenu(MFT_STRING, 4, _T("기본값 설정"));
		break;
	case 4: // "지표" 아이템일 경우
		// 2009.11.03 : ADD : 지표 찾기 기능 추가
		cMenu.AppendMenu(MFT_STRING, 5, _T("지표 찾기"));
		break;
	}

	CPoint	pt;
	GetCursorPos(&pt);

	int	menuID = cMenu.TrackPopupMenu(TPM_LEFTBUTTON|TPM_RETURNCMD, pt.x, pt.y, this, NULL);
	cMenu.DestroyMenu();

	switch (menuID)
	{
	case 2:
		Notify(WM_AXRMOUSE, addBookMark);
		break;
	case 3:
		Notify(WM_AXRMOUSE, delBookMark);
		break;
	case 4:
		Notify(WM_AXRMOUSE, configIndc);
		break;
	case 5:
		{
			// 2009.11.03 : ADD : 지표 찾기 기능 추가
			HINSTANCE hinst = AfxGetResourceHandle();
			AfxSetResourceHandle(::GetModuleHandle("AxMCtrl.dll"));

			CIndexSearchDlg dialogBox(this);

			dialogBox.DoModal();
			AfxSetResourceHandle(hinst);
		}
		break;
	default:
		break;
	}

	CTreeCtrl::OnRButtonDown(nFlags, point);
	//**/
}

void CAxMTreeCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CAxMTreeCtrl::PreSubclassWindow() 
{
	//Let the parent class do its thing	
	CTreeCtrl::PreSubclassWindow();
	
	//Hook it up to the tree control
	SetImageList(&m_ilTree, TVSIL_NORMAL);
}

void CAxMTreeCtrl::setImageList(CString path, int cx, int cy)
{
	HBITMAP hbitmap = (HBITMAP) LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbitmap)
	{
		CBitmap	bitmap;
		bitmap.DeleteObject();
		bitmap.Attach(hbitmap);
		m_ilTree.Create(cx, cy, ILC_COLOR16|ILC_MASK, 0, 1);
		m_ilTree.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}
}

HTREEITEM CAxMTreeCtrl::InsertGroup(LPCTSTR lpszItem, int nImage, HTREEITEM hParent)
{
	ASSERT(nImage > 3); //You must specify an image index greater than three as the 
		      //first 8 images in the image list are reserved for the 
		      //checked and unchecked check box and radio buttons images

	HTREEITEM hItem = InsertItem(lpszItem, nImage, nImage, hParent);
	SetItemData(hItem, 0); //Item data must be 0
	return hItem;
}

HTREEITEM CAxMTreeCtrl::InsertCheckBox(LPCTSTR lpszItem, HTREEITEM hParent, BOOL bCheck)
{
	ASSERT(IsGroup(hParent)); //The parent of a combo item must be a group item

	HTREEITEM hItem = InsertItem(lpszItem, 0, 0, hParent);
	SetItemData(hItem, 0);    //Item data must be 0
	BOOL bSuccess = SetCheckBox(hItem, bCheck);
	ASSERT(bSuccess);

	return hItem;
}

HTREEITEM CAxMTreeCtrl::InsertRadioButton(LPCTSTR lpszItem, HTREEITEM hParent, BOOL bCheck)
{
	ASSERT(IsGroup(hParent)); //The parent of a radio item must be a group item

	HTREEITEM hItem = InsertItem(lpszItem, 2, 2, hParent);
	SetItemData(hItem, 0);    //Item data must be 0
	if (bCheck)
	{
		//if requested to, check the newly added radio button
		BOOL bSuccess = SetRadioButton(hItem);
		ASSERT(bSuccess);
	}

	return hItem;
}

BOOL CAxMTreeCtrl::IsGroup(HTREEITEM hItem)
{
	int nImage;
	int nSelectedImage;
	BOOL bSuccess = GetItemImage(hItem, nImage, nSelectedImage);
	ASSERT(bSuccess);

	return (nImage > 3);
}

BOOL CAxMTreeCtrl::IsCheckBox(HTREEITEM hItem)
{
	int nImage;
	int nSelectedImage;
	BOOL bSuccess = GetItemImage(hItem, nImage, nSelectedImage);
	ASSERT(bSuccess);

	return (nImage == 0 || nImage == 1);
}

BOOL CAxMTreeCtrl::IsRadioButton(HTREEITEM hItem)
{
	int nImage;
	int nSelectedImage;
	BOOL bSuccess = GetItemImage(hItem, nImage, nSelectedImage);
	ASSERT(bSuccess);

	return (nImage == 2 || nImage == 3);
}

BOOL CAxMTreeCtrl::SetCheckBox(HTREEITEM hItem, BOOL bCheck)
{
	//Validate our parameters
	ASSERT(IsCheckBox(hItem)); //Must be a combo item to check it

	BOOL bSuccess;
	if (bCheck)
		bSuccess = SetItemImage(hItem, 1, 1);
	else
		bSuccess = SetItemImage(hItem, 0, 0);   

	return bSuccess;
}

BOOL CAxMTreeCtrl::GetCheckBox(HTREEITEM hItem, BOOL& bCheck)
{
	//Validate our parameters
	ASSERT(IsCheckBox(hItem)); //Must be a combo item to check it

	int nImage;
	int nSelectedImage;
	BOOL bSuccess = GetItemImage(hItem, nImage, nSelectedImage);
	ASSERT(bSuccess);

	bCheck = (nImage == 1);

	return bSuccess;
}

BOOL CAxMTreeCtrl::SetRadioButton(HTREEITEM hParent, int nIndex)
{
	//Validate our parameters
	ASSERT(IsGroup(hParent)); //Parent item must be a group item

	//Iterate through the child items and turn on the specified one and turn off all the other ones
	HTREEITEM hChild = GetNextItem(hParent, TVGN_CHILD);

	//Turn of redraw to Q all the changes we're going to make here
	SetRedraw(FALSE);

	int i=0;
	BOOL bCheckedSomeItem = FALSE;
	while (hChild)
	{
		ASSERT(IsRadioButton(hChild)); //Child item must be a radio item

		if (i == nIndex)
		{
			//Turn this item on
			VERIFY(SetItemImage(hChild, 3, 3));
			bCheckedSomeItem = TRUE;
		}
		else
		{
			BOOL bEnable;
			VERIFY(GetRadioButtonEnable(hChild, bEnable));

			//Turn this item off
			if (bEnable)
				VERIFY(SetItemImage(hChild, 2, 2));
			else
				VERIFY(SetItemImage(hChild, 2, 2));
		}

		//Move on to the next item
		hChild = GetNextItem(hChild, TVGN_NEXT);
		++i;
	}
	ASSERT(bCheckedSomeItem); //You specified an index which does not exist

	//Reset the redraw flag
	SetRedraw(TRUE);

	return TRUE;
}

BOOL CAxMTreeCtrl::SetRadioButton(HTREEITEM hItem)
{
	//Validate our parameters
	ASSERT(IsRadioButton(hItem)); //Must be a radio item to check it

	//Iterate through the sibling items and turn them all off except this one
	HTREEITEM hParent = GetNextItem(hItem, TVGN_PARENT);
	ASSERT(IsGroup(hParent)); //Parent item must be a group item

	//Iterate through the child items and turn on the specified one and turn off all the other ones
	HTREEITEM hChild = GetNextItem(hParent, TVGN_CHILD);

	//Turn of redraw to Q all the changes we're going to make here
	SetRedraw(FALSE);

	while (hChild)
	{
		ASSERT(IsRadioButton(hChild)); //Child item must be a radio item

		if (hChild == hItem)
		{
			//Turn this item on
			BOOL bSuccess = SetItemImage(hChild, 3, 3);
			ASSERT(bSuccess);
		}
		else
		{
			BOOL bEnable;
			VERIFY(GetRadioButtonEnable(hChild, bEnable));

			//Turn this item off
			if (bEnable)
				VERIFY(SetItemImage(hChild, 2, 2));
			else
				VERIFY(SetItemImage(hChild, 2, 2));
		}

		//Move on to the next item
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}

	//Reset the redraw flag
	SetRedraw(TRUE);

	return TRUE;
}

BOOL CAxMTreeCtrl::GetRadioButton(HTREEITEM hParent, int& nIndex, HTREEITEM& hCheckItem)
{
	ASSERT(IsGroup(hParent)); //Parent item must be a group item

	//Iterate through the child items and turn on the specified one and turn off all the other ones
	HTREEITEM hChild = GetNextItem(hParent, TVGN_CHILD);
	ASSERT(hChild); //Must have some child items

	//Find the checked item  
	nIndex = 0;
	BOOL bFound = FALSE;
	while (!bFound)
	{
		BOOL bSuccess = GetRadioButton(hChild, bFound);
		ASSERT(bSuccess);

		//Move on to the next sibling if not found
		if (!bFound)
		{
			hChild = GetNextItem(hChild, TVGN_NEXT);
			ASSERT(hChild);
			++nIndex;
		}
		else
			hCheckItem = hChild;
	}

	return TRUE;
}

BOOL CAxMTreeCtrl::GetRadioButton(HTREEITEM hItem, BOOL& bCheck)
{
	ASSERT(IsRadioButton(hItem)); //Must be a radio item to check it

	int nImage;
	int nSelectedImage;
	BOOL bSuccess = GetItemImage(hItem, nImage, nSelectedImage);
	ASSERT(bSuccess);

	bCheck = (nImage == 3);

	return bSuccess;
}

BOOL CAxMTreeCtrl::SetGroupEnable(HTREEITEM hItem, BOOL bEnable)
{
	//Allows you to quickly enable / disable all the items in a group

	ASSERT(IsGroup(hItem)); //Must be group item

	//Iterate through the child items and enable / disable all the items
	HTREEITEM hChild = GetNextItem(hItem, TVGN_CHILD);

	//Turn of redraw to Q all the changes we're going to make here
	SetRedraw(FALSE);

	while (hChild)
	{
		if (IsRadioButton(hChild))
		{
			int nImage;
			int nSelectedImage;
			VERIFY(GetItemImage(hChild, nImage, nSelectedImage));
			BOOL bCheck = (nImage == 3);
			if (bCheck)
			{
				if (bEnable)
					VERIFY(SetItemImage(hChild, 3, 3));
				else
					VERIFY(SetItemImage(hChild, 3, 3)); // disable image
			}
			else
			{
				if (bEnable)
					VERIFY(SetItemImage(hChild, 2, 2));
				else
					VERIFY(SetItemImage(hChild, 2, 2)); // disable image
			}
		}
		else if (IsCheckBox(hChild))
			VERIFY(SetCheckBoxEnable(hChild, bEnable));
		else
			ASSERT(FALSE);

		//Move onto the next child
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}

	//Reset the redraw flag
	SetRedraw(TRUE);

	return TRUE;
}

BOOL CAxMTreeCtrl::SetCheckBoxEnable(HTREEITEM hItem, BOOL bEnable)
{
	ASSERT(IsCheckBox(hItem)); //Must be a check box
	BOOL bSuccess = FALSE;

	if (bEnable)
	{
		BOOL bCheck;
		VERIFY(GetCheckBox(hItem, bCheck));
		if (bCheck)
			bSuccess = SetItemImage(hItem, 1, 1);
		else
			bSuccess = SetItemImage(hItem, 0, 0);
	}
	else
	{	 // disable image
		BOOL bCheck;
		VERIFY(GetCheckBox(hItem, bCheck));
		if (bCheck)
			bSuccess = SetItemImage(hItem, 1, 1);
		else
			bSuccess = SetItemImage(hItem, 0, 0);
	}	

	return bSuccess;
}

BOOL CAxMTreeCtrl::SetRadioButtonEnable(HTREEITEM hItem, BOOL bEnable)
{
	ASSERT(IsRadioButton(hItem)); //Must be a radio button
	BOOL bSuccess = FALSE;

	if (bEnable)
	{
		BOOL bCheck;
		VERIFY(GetRadioButton(hItem, bCheck));
		if (bCheck)
			bSuccess = SetItemImage(hItem, 3, 3);
		else
			bSuccess = SetItemImage(hItem, 2, 2);
	}
	else
	{	 // disable image
		BOOL bCheck;
		VERIFY(GetRadioButton(hItem, bCheck));
		if (bCheck)
			bSuccess = SetItemImage(hItem, 3, 3);
		else
			bSuccess = SetItemImage(hItem, 2, 2);
	}

	return bSuccess;
}

BOOL CAxMTreeCtrl::GetCheckBoxEnable(HTREEITEM hItem, BOOL& bEnable)
{
	ASSERT(IsCheckBox(hItem)); //Must be a check box

	int nImage;
	int nSelectedImage;
	BOOL bSuccess = GetItemImage(hItem, nImage, nSelectedImage);
	ASSERT(bSuccess);

	bEnable = (nImage == 0 || nImage == 1);

	return bSuccess;  
}

BOOL CAxMTreeCtrl::GetRadioButtonEnable(HTREEITEM hItem, BOOL& bEnable)
{
	ASSERT(IsRadioButton(hItem)); //Must be a radio button

	int nImage;
	int nSelectedImage;
	BOOL bSuccess = GetItemImage(hItem, nImage, nSelectedImage);
	ASSERT(bSuccess);

	bEnable = (nImage == 2 || nImage == 3);

	return bSuccess;  
}

//////////////////////////////////////////////////////////////////////////////////////////////
void DDX_TreeCheck(CDataExchange* pDX, int nIDC, HTREEITEM hItem, BOOL& bCheck)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	CAxMTreeCtrl* ptree = (CAxMTreeCtrl*) CWnd::FromHandlePermanent(hWndCtrl);
	ASSERT(ptree);
	ASSERT(ptree->IsKindOf(RUNTIME_CLASS(CAxMTreeCtrl)));

	if (pDX->m_bSaveAndValidate)
	{
		BOOL bSuccess = ptree->GetCheckBox(hItem, bCheck);
		ASSERT(bSuccess);
	}
	else
	{
		BOOL bSuccess = ptree->SetCheckBox(hItem, bCheck);
		ASSERT(bSuccess);
	}
}

void DDX_TreeRadio(CDataExchange* pDX, int nIDC, HTREEITEM hParent, int& nIndex)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	CAxMTreeCtrl* ptree = (CAxMTreeCtrl*) CWnd::FromHandlePermanent(hWndCtrl);
	ASSERT(ptree);
	ASSERT(ptree->IsKindOf(RUNTIME_CLASS(CAxMTreeCtrl)));

	if (pDX->m_bSaveAndValidate)
	{
		HTREEITEM hCheckItem;
		BOOL bSuccess = ptree->GetRadioButton(hParent, nIndex, hCheckItem);
		ASSERT(bSuccess);
	}
	else
	{
		BOOL bSuccess = ptree->SetRadioButton(hParent, nIndex);
		ASSERT(bSuccess);
	}
}

void CAxMTreeCtrl::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if (pNMTreeView->action == 2)
		SetItemImage(pNMTreeView->itemNew.hItem, 4, 4);
	else if (pNMTreeView->action == 1)
		SetItemImage(pNMTreeView->itemNew.hItem, 5, 5);
	*pResult = 0;
}

BOOL CAxMTreeCtrl::PreTranslateMessage(MSG* pMsg) 
{
	return CTreeCtrl::PreTranslateMessage(pMsg);
}

LRESULT CAxMTreeCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}

LRESULT CAxMTreeCtrl::Notify(UINT msg, WPARAM wParam, LPARAM lParam)
{
	NOTIFY(msg, wParam, lParam);
}

LRESULT CAxMTreeCtrl::OnSetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	SetMessageHandler((CWnd*)wParam);
	return 0L;
}

LRESULT CAxMTreeCtrl::OnGetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)GetMessageHandler();
}

// 2009.11.04 : ADD : 지표 검색 기능 추가
// 트리에서 각 아이템의 지표이름을 찾아 해당 트리 아이템과 연결하여
// CMapStringToPtr 리스트로 반환한다.
bool CAxMTreeCtrl::GetTreeItemTextList(CMapStringToPtr *pList, CString strItemText)
{
	if (pList == NULL)
		return false;

	if (strItemText.IsEmpty())
		return false;

	// 지표 리스트를 받아갈 CMapStringToPtr 초기화한다.
	pList->RemoveAll();
	
	HTREEITEM	hRootItem = GetRootItem();
	CString		strText;
	bool		bFoundIndexCategory = false;

	// 트리에서 지표 항목을 찾는다.
	while (hRootItem)
	{
		strText = GetItemText(hRootItem);
		if (strText == "지표(찾기)")
		{
			bFoundIndexCategory = true;
			break;
		}
		hRootItem = GetNextSiblingItem(hRootItem);
	}

	if (bFoundIndexCategory == false)
		return false;

	// 지표 항목에서 자식 노드를 탐색한다.
	HTREEITEM hChildItem = GetChildItem(hRootItem);

	while (hChildItem)
	{
		HTREEITEM hGrandChildItem = GetChildItem(hChildItem);

		while (hGrandChildItem)
		{
			strText = GetItemText(hGrandChildItem);
			
			// 해당 문자열이 아이템 텍스트에 있는지 확인하여 있으면
			// pList에 지표 이름과 HTREEITEM를 등록한다.
			// 우선 영문자는 대문자로 변경하여 검색한다.
			strText.MakeUpper();
			strItemText.MakeUpper();

			if (strText.Find(strItemText) > -1)
			{
				pList->SetAt((LPCTSTR)strText, hGrandChildItem);
			}

			hGrandChildItem = GetNextSiblingItem(hGrandChildItem);
		}
		
		hChildItem = GetNextSiblingItem(hChildItem);		
	}

	return true;
}

// 2009.11.04 : ADD : 지표 검색 기능 추가
void CAxMTreeCtrl::SelectTreeItem(HTREEITEM hItem)
{
	Select(hItem, TVGN_CARET);
	Select(hItem, TVGN_DROPHILITE);
	//SelectItem(hItem);
}
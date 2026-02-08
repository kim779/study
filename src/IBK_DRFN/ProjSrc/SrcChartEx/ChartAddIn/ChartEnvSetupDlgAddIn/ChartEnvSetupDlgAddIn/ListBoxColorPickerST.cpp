#include "stdafx.h"
#include "ListBoxColorPickerST.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	MASK_DWDATA		0x01	// dwItemData is valid
#define	MASK_LPDATA		0x02	// pData is valid
#define	MASK_CRCOLOR	0x04	// crColor is valid
#define MASK_ALL		0xff	// All fields are valid

CListBoxColorPickerST::CListBoxColorPickerST()
{
}


CListBoxColorPickerST::~CListBoxColorPickerST()
{
}


void DDX_ListBoxColorPickerST(CDataExchange *pDX, int nIDC, COLORREF& rgbColor)
{
	HWND hWndCtrl = pDX->PrepareCtrl( nIDC );
	ASSERT( hWndCtrl );

	CListBoxColorPickerST *pPicker = (CListBoxColorPickerST*)CWnd::FromHandle(hWndCtrl);
	ASSERT( pPicker );

	// Only support getting of color
	if(pDX->m_bSaveAndValidate)
	{
		rgbColor = pPicker->GetSelectedColor();
	}
} // End of DDX_ListBoxColorPickerST


BEGIN_MESSAGE_MAP(CListBoxColorPickerST, CListBox)
	//{{AFX_MSG_MAP(CListBoxColorPickerST)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CListBoxColorPickerST::OnDestroy() 
{
	FreeResources();
	CListBox::OnDestroy();
} // End of OnDestroy

void CListBoxColorPickerST::DrawItem(LPDRAWITEMSTRUCT pDIStruct)
{
	CString			strColour;
	CDC				dcContext;
	CRect			rItemRect(pDIStruct->rcItem);
	CRect			rBlockRect(rItemRect);
	CRect			rTextRect(rBlockRect);
	CBrush			brFrameBrush;
	int				iFourthWidth = 0;
	int				iItem = pDIStruct->itemID;
	int				iAction = pDIStruct->itemAction;
	int				iState = pDIStruct->itemState;
	COLORREF		crColour = NULL;
	COLORREF		crNormal = GetSysColor(COLOR_WINDOW);
	COLORREF		crSelected = GetSysColor(COLOR_HIGHLIGHT);
	COLORREF		crText = GetSysColor(COLOR_WINDOWTEXT);
	STRUCT_LBDATA*	lpLBData = NULL;

	if( !dcContext.Attach( pDIStruct->hDC ) )
	{
		return;
	}

	iFourthWidth = ( rBlockRect.Width() / 4 );
	brFrameBrush.CreateStockObject( BLACK_BRUSH );

	if( iState & ODS_SELECTED )
	{
		dcContext.SetTextColor(	( 0x00FFFFFF & ~( crText ) ) );
		dcContext.SetBkColor( crSelected );
		dcContext.FillSolidRect( &rBlockRect, crSelected );
	}
	else
	{
		dcContext.SetTextColor( crText );
		dcContext.SetBkColor( crNormal );
		dcContext.FillSolidRect( &rBlockRect, crNormal );
	}

	if( iState & ODS_FOCUS )
	{
		dcContext.DrawFocusRect( &rItemRect );
	}

	// calculate text area.
	rTextRect.left += ( iFourthWidth + 2 );
	rTextRect.top += 2;

	// calculate colour block area.

	rBlockRect.DeflateRect( CSize( 2, 2 ) );
	rBlockRect.right = iFourthWidth;

	// draw colour text and block.
	if( iItem != -1 )
	{
		CListBox::GetText(iItem, strColour);

		if( iState & ODS_DISABLED )
		{
			crColour = GetSysColor( COLOR_INACTIVECAPTIONTEXT );
			dcContext.SetTextColor( crColour );
		}
		else
		{
			crColour = GetColor(iItem);
		}

		dcContext.SetBkMode( TRANSPARENT );
		dcContext.TextOut( rTextRect.left, rTextRect.top,	strColour );

		dcContext.FillSolidRect( &rBlockRect, crColour );

//		dcContext.TextOut(rBlockRect.left + 5, rBlockRect.top, "»ö");
				
		dcContext.FrameRect( &rBlockRect, &brFrameBrush );
	}

	dcContext.Detach();
}

void CListBoxColorPickerST::FreeResources()
{
	int	nCount = 0;

	nCount = GetCount();
	if (nCount != LB_ERR)
		for (;nCount > 0; nCount--)
		{
			DeleteItemData(nCount-1);
		} // for
} // End of FreeResources

int CListBoxColorPickerST::ReplaceItemData(int nIndex, DWORD dwItemData, LPVOID pData, COLORREF crColor, BYTE byMask)
{
	STRUCT_LBDATA*	lpLBData = NULL;
	int				nRetValue = LB_ERR;

	// Get pointer to associated datas (if any)
	lpLBData = (STRUCT_LBDATA*)CListBox::GetItemDataPtr(nIndex);
	// If no datas exist create a new one
	if (lpLBData == NULL)
	{
		lpLBData = new STRUCT_LBDATA;
		if (lpLBData)	::ZeroMemory(lpLBData, sizeof(STRUCT_LBDATA));
	} // if

	if (lpLBData)
	{
		if ((byMask & MASK_DWDATA) == MASK_DWDATA)
			lpLBData->dwItemData = dwItemData;
		if ((byMask & MASK_LPDATA) == MASK_LPDATA)
			lpLBData->pData = pData;
		if ((byMask & MASK_CRCOLOR) == MASK_CRCOLOR)
			lpLBData->crColor = crColor;

		nRetValue = CListBox::SetItemDataPtr(nIndex, lpLBData);
	} // if

	return nRetValue;
} // End of ReplaceItemData

void CListBoxColorPickerST::DeleteItemData(int nIndex)
{
	STRUCT_LBDATA*	lpLBData = NULL;

	// Get pointer to associated datas (if any)
	lpLBData = (STRUCT_LBDATA*)CListBox::GetItemDataPtr(nIndex);
	// If datas exist
		if (lpLBData != (LPVOID)-1L)	delete lpLBData;

	CListBox::SetItemDataPtr(nIndex, NULL);
} // End of DeleteItemData

// Adds a string to the list box.
//
// Parameters:
//		[IN]	lpszItem
//				Points to the null-terminated string that is to be added.
//		[IN]	crColor
//				Color to be associated with the string.
//
// Return value:
//		The zero-based index of the string in the list box.
//		The return value is LB_ERR if an error occurs; the return value 
//		is LB_ERRSPACE if insufficient space is available to store the new string.
//
int CListBoxColorPickerST::AddString(LPCTSTR lpszItem, COLORREF crColor)
{
	int	nIndex   = LB_ERR;

	nIndex = CListBox::AddString(lpszItem);
	if (nIndex != LB_ERR && nIndex != LB_ERRSPACE)
	{
		ReplaceItemData(nIndex, 0, NULL, crColor, MASK_ALL);
	} // if

	return nIndex;
} // End of AddString

// Inserts a string at a specific location in the list box.
//
// Parameters:
//		[IN]	nIndex
//				Specifies the zero-based index of the position to insert the string.
//				If this parameter is -1, the string is added to the end of the list.
//		[IN]	lpszItem
//				Pointer to the null-terminated string that is to be inserted.
//		[IN]	crColor
//				Color to be associated with the string.
//
// Return value:
//		The zero-based index of the position at which the string was inserted.
//		The return value is LB_ERR if an error occurs; the return value 
//		is LB_ERRSPACE if insufficient space is available to store the new string.
//
int CListBoxColorPickerST::InsertString(int nIndex, LPCTSTR lpszString, COLORREF crColor)
{
	int	nNewIndex   = LB_ERR;

	nNewIndex = CListBox::InsertString(nIndex, lpszString);
	if (nNewIndex != LB_ERR && nNewIndex != LB_ERRSPACE)
	{
		ReplaceItemData(nNewIndex, 0, NULL, crColor, MASK_ALL);
	} // if

	return nNewIndex;
} // End of InsertString

// Deletes a string from the list box.
//
// Parameters:
//		[IN]	nIndex
//				Specifies the zero-based index of the string to be deleted.
//
// Return value:
//		A count of the strings remaining in the list box.
//		The return value is LB_ERR if nIndex specifies an index greater than 
//		the number of items in the list.
//
int CListBoxColorPickerST::DeleteString(int nIndex)
{
	int	nRetValue = LB_ERR;

	DeleteItemData(nIndex);
	nRetValue = CListBox::DeleteString(nIndex);

	return nRetValue;
} // End of DeleteString

// Replaces a string at a specific location in the list box.
//
// Parameters:
//		[IN]	nIndex
//				Specifies the zero-based index of the position to replace the string.
//		[IN]	lpszItem
//				Pointer to the null-terminated string that is to be replaced.
//		[IN]	crColor
//				Color to be associated with the string.
//
// Return value:
//		The zero-based index of the position at which the string was replaced.
//		The return value is LB_ERR if an error occurs; the return value 
//		is LB_ERRSPACE if insufficient space is available to store the new string.
//
int CListBoxColorPickerST::ReplaceString(int nIndex, LPCTSTR lpszString, COLORREF crColor)
{
	int	nRetValue;

	nRetValue = DeleteString(nIndex);
	if (nRetValue != LB_ERR)
	{
		nRetValue = InsertString(nIndex, lpszString, crColor);
	} // if

	return nRetValue;
} // End of ReplaceString

// Clears all the entries from the list box.
//
void CListBoxColorPickerST::ResetContent()
{
	FreeResources();
	CListBox::ResetContent();
} // End of ResetContent

// Associates a color with a string at a specific location in the list box.
//
// Parameters:
//		[IN]	nIndex
//				Specifies the zero-based index of the string.
//		[IN]	crColor
//				Color to be associated with the string.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
// Return value:
//		LB_ERR if an error occurs.
//
int CListBoxColorPickerST::SetColor(int nIndex, COLORREF crColor, BOOL bRepaint)
{
	COLORREF	crOldColor;
	int			nRetValue = LB_ERR;

	crOldColor = GetColor(nIndex);
	if (crColor != crOldColor)
	{
		nRetValue = ReplaceItemData(nIndex, 0, NULL, crColor, MASK_CRCOLOR);
		if (bRepaint)	Invalidate();
	} // if

	return nRetValue;
} // End of SetColor

// Returns the color associated with a string at a specific location in the list box.
//
// Parameters:
//		[IN]	nIndex
//				Specifies the zero-based index of the color to be retrieved.
//
// Return value:
//		The color associated with the specified string.
//		The return value is -1 if an error occurs.
//
COLORREF CListBoxColorPickerST::GetColor(int nIndex)
{
	STRUCT_LBDATA*	lpLBData = NULL;

	lpLBData = (STRUCT_LBDATA*)CListBox::GetItemData(nIndex);
	if (lpLBData != (LPVOID)-1L)
		return lpLBData->crColor;

	return -1L;
} // End of GetColor

// Returns the color associated with the currently selected string in the list box.
//
// Return value:
//		The color associated with the currently selected string.
//		The return value is -1 if an error occurs or no string is selected.
//
COLORREF CListBoxColorPickerST::GetSelectedColor()
{
	return GetColor(GetCurSel());
} // End of GetSelectedColor

// Sets the 32-bit value associated with the list box item.
//
// Parameters:
//		[IN]	nIndex
//				Specifies the zero-based index of the item.
//		[IN]	dwItemData
//				Specifies the value to be associated with the item.
//
// Return value:
//		LB_ERR if an error occurs.
//
int CListBoxColorPickerST::SetItemData(int nIndex, DWORD dwItemData)
{
	return ReplaceItemData(nIndex, dwItemData, NULL, 0, MASK_DWDATA);
} // End of SetItemData

// Returns the 32-bit value associated with the list box item.
//
// Parameters:
//		[IN]	nIndex
//				Specifies the zero-based index of the item.
//
// Return value:
//		The 32-bit value associated with the item, or LB_ERR if an error occurs.
//
DWORD CListBoxColorPickerST::GetItemData(int nIndex)
{
	STRUCT_LBDATA*	lpLBData = NULL;

	lpLBData = (STRUCT_LBDATA*)CListBox::GetItemDataPtr(nIndex);
	if (lpLBData != (LPVOID)-1L)
		return lpLBData->dwItemData;

	return LB_ERR;
} // End of GetItemData

// Sets a pointer to a list box item.
//
// Parameters:
//		[IN]	nIndex
//				Specifies the zero-based index of the item.
//		[IN]	pData
//				Specifies the pointer to be associated with the item.
//
// Return value:
//		LB_ERR if an error occurs.
//
int CListBoxColorPickerST::SetItemDataPtr(int nIndex, void* pData)
{
	return ReplaceItemData(nIndex, 0, pData, 0, MASK_LPDATA);
} // End of SetItemDataPtr

// Returns a pointer of a list box item.
//
// Parameters:
//		[IN]	nIndex
//				Specifies the zero-based index of the item.
//
// Return value:
//		Pointer associated with the item, or -1 if an error occurs.
//
void* CListBoxColorPickerST::GetItemDataPtr(int nIndex)
{
	STRUCT_LBDATA*	lpLBData = NULL;

	lpLBData = (STRUCT_LBDATA*)CListBox::GetItemDataPtr(nIndex);
	if (lpLBData != (LPVOID)-1L)
		return lpLBData->pData;

	return (LPVOID)-1L;
} // End of GetItemDataPtr

// Searches for a particular color in the list box. If found the associated
// string is then selected. The search ends at the first occurence of the color.
//
// Parameters:
//		[IN]	crColor
//				Color to search for.
//
// Return value:
//		The zero-based index of the position at which the color was found.
//		The return value is LB_ERR if an error occurs or the spcified color
//		is not found.
//
int CListBoxColorPickerST::SelectColor(COLORREF crColor)
{
	int	nCount = 0;
	int	nLoop = 0;

	// Search all the listbox
	nCount = GetCount();
	if (nCount != LB_ERR)
		for (nLoop = 0; nLoop < nCount; nLoop++)
		{
			// If color is found
			if (crColor == GetColor(nLoop))	
			{
				SetCurSel(nLoop);
				return nLoop;
			} // if
		} // for

	return LB_ERR;
} // End of SelectColor

// Returns a list box item and the associated color.
//
// Parameters:
//		[IN]	nIndex
//				Specifies the zero-based index of the item.
//		[OUT]	lpszBuffer
//				Pointer to the buffer that will receive the string.
//				The buffer must have sufficient space for the string and a 
//				terminating null character. The size of the string can be 
//				determined ahead of time by calling the GetTextLen member function.
//		[OUT]	lpcrColor
//				Pointer to a COLORREF variable that will receive the color associated with
//				the specified item.
//
// Return value:
//		The length (in bytes) of the string, excluding the terminating null character. 
//		If nIndex does not specify a valid index, the return value is LB_ERR.
//
int CListBoxColorPickerST::GetTextAndColor(int nIndex, LPTSTR lpszBuffer, COLORREF* lpcrColor)
{
	COLORREF	crColor;

	// Get color
	crColor = GetColor(nIndex);
	if (crColor != -1L)
	{
		*lpcrColor = crColor;
		// Call base-class method
		return CListBox::GetText(nIndex, lpszBuffer);
	} // if

	return LB_ERR;
} // End of GetTextAndColor

// Returns a list box item and the associated color.
//
// Parameters:
//		[IN]	nIndex
//				Specifies the zero-based index of the item.
//		[OUT]	rString
//				A reference to a CString object.
//		[OUT]	lpcrColor
//				Pointer to a COLORREF variable that will receive the color associated with
//				the specified item.
//
// Return value:
//		The length (in bytes) of the string, excluding the terminating null character. 
//		If nIndex does not specify a valid index, the return value is LB_ERR.
//
int CListBoxColorPickerST::GetTextAndColor(int nIndex, CString &rString, COLORREF* lpcrColor)
{
	COLORREF	crColor;

	// Get color
	crColor = GetColor(nIndex);
	if (crColor != -1L)
	{
		*lpcrColor = crColor;
		// Call base-class method
		CListBox::GetText(nIndex, rString);
		return rString.GetLength();
	} // if

	return LB_ERR;
} // End of GetTextAndColor

#undef	MASK_DWDATA
#undef	MASK_LPDATA
#undef	MASK_CRCOLOR
#undef	MASK_ALL

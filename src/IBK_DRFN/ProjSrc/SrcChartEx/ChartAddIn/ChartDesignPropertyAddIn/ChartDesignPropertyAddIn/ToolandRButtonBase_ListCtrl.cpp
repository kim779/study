////////////////////////////////////////////////////////////////////
// CToolandRButtonBase_ListCtrl.cpp -- implementation file
//
// Extended List Control class with OLE Drag & Drop and optional:
//             - local drag & drop
//             - automatic scrolling,
//             - Removing of dragged items 
// 
//
// Author: Jean-Claude DAUPHIN, CI/INF UNESCO
//
// The following source of information have been invaluable for
// succeeding in this task:
//
// Lstdrg.exe -- Microsoft sample which implement OLE drag & drop
// for a ListBox Control and an Edit Control.
//
// Wicked code article written by J. Prosise (MSJ August 98) 
///////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolandRButtonBase_ListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonBase_ListCtrl


//------------------------------------------------------------------------------
// void CToolandRButtonBase_ListCtrl::Initialize()
//
// Calls the Register method of m_DropTarget to register this control as a drop
// target with the OLE dll's.
//
// We need to call this method in the Dialog CMyDlg::OnInitDialog() or 
// CFormView::OnInitialUpdate for controls created by a dialog template since
// the OnCreate does not get called for template created  controls (this is
// because the windows controls get created before the CWnd wrappers).
//-----------------------------------------------------------------------------
void CToolandRButtonBase_ListCtrl::InitDropTarget()
{   

    // Register the COleDropTarget object with the OLE dll's
    BOOL bSuccess = m_DropTarget.Register(this);
	m_DropTarget.SetCurrentWindow(this);
    if (!bSuccess)
       AfxMessageBox("Ole Register Drop Target Failed");
}                                                                             


CToolandRButtonBase_ListCtrl::CToolandRButtonBase_ListCtrl()
{
}

CToolandRButtonBase_ListCtrl::~CToolandRButtonBase_ListCtrl()
{
}


BEGIN_MESSAGE_MAP(CToolandRButtonBase_ListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CToolandRButtonBase_ListCtrl)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonBase_ListCtrl message handlers


//-------------------------------------------------------------------------------
// void CToolandRButtonBase_ListCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
//
// LVN_BEGINDRAG notifies a list view control's parent window that a drag-and-drop
// operation involving the left mouse button is being initiated. Message 
// reflection lets us handle messages for the control, within the control itself.
//-------------------------------------------------------------------------------

void CToolandRButtonBase_ListCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
    int   iItem = pNMListView->iItem; 
    int   iSubItem = pNMListView->iSubItem; 

	HGLOBAL hGlobal = NULL;
	ToolItem *pszGlobal = NULL;
	hGlobal = GlobalAlloc( GMEM_SHARE, sizeof(ToolItem) * SelectedItemCount());
	pszGlobal = (ToolItem *)((LPSTR)GlobalLock( hGlobal ));
	ASSERT( pszGlobal );
	memset(pszGlobal, 0, sizeof(ToolItem) * SelectedItemCount());
	strcpy(pszGlobal->ChartName, (LPCTSTR)SelectedItemNames());             // CString -> char *

	//GetItemText( iCurSel - 1, 0, pszGlobal->ChartName, 64 ) ;
	GlobalUnlock( hGlobal );
	if(hGlobal)	{
		m_COleDataSource.CacheGlobalData( CF_TEXT, hGlobal );
		DROPEFFECT  de;
		de = m_COleDataSource.DoDragDrop( DROPEFFECT_COPY);
	}
}

/////////////////////////////////////////////////////////////////////////////
// COleListCtrlDropTarget - Enable OLE dragdrop for the ListCtrl control

COleListCtrlDropTarget::COleListCtrlDropTarget()
{}

COleListCtrlDropTarget::~COleListCtrlDropTarget() {}

//-----------------------------------------------------------------------------------
// DROPEFFECT COleListCtrlDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* 
//           pDataObject, DWORD dwKeyState, CPoint point )
// 
// Called by OLE dll's when drag cursor FIRST enters a window that is REGISTERED
//  with the OLE dll's
//
//Parameters
//----------
//
// pWnd        -- Points to the window the cursor is entering.
// pDataObject -- Points to the data object containing the data
//                that can be dropped.
// dwKeyState  -- Contains the state of the modifier keys.
//                This is a combination of any number of the following:
//                MK_CONTROL, MK_SHIFT, MK_ALT, MK_LBUTTON,
//                MK_MBUTTON, and MK_RBUTTON.
//
// point       -- Contains the current location of the cursor in
//                client coordinates.
//
// Return Value : The effect that would result if a drop were 
//-------------   attempted at the location specified by point.
//                It can be one or more of the following: 
//
//     DROPEFFECT_NONE   -A drop would not be allowed.
//     DROPEFFECT_COPY   -A copy operation would be performed.
//     DROPEFFECT_MOVE   -A move operation would be performed.
//     DROPEFFECT_LINK   -A link from the dropped data to the 
//                        original data would be established.
//     DROPEFFECT_SCROLL -A drag scroll operation is about to occur 
//                        or is occurring in the target. 
//-----------------------------------------------------------------------------------
DROPEFFECT COleListCtrlDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* 
           pDataObject, DWORD dwKeyState, CPoint point )
{
	// Check if the Control key was pressed
    if ((dwKeyState&MK_CONTROL) == MK_CONTROL)
        return DROPEFFECT_COPY; // Copy source text
    else
        return DROPEFFECT_MOVE; // Move source text   
}

//---------------------------------------------------------------------------------
// void COleListCtrlDropTarget::OnDragLeave(CWnd* pWnd)
//
// Called by OLE dll's when drag cursor leaves a window that is REGISTERed with 
// the OLE dll's.
//
// pWnd points to the window the cursor is leaving.
//---------------------------------------------------------------------------------
void COleListCtrlDropTarget::OnDragLeave(CWnd* pWnd)
{
     // Call base class implementation
    COleDropTarget:: OnDragLeave(pWnd);
}

//----------------------------------------------------------------------------------
// DROPEFFECT COleListCtrlDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* 
//           pDataObject, DWORD dwKeyState, CPoint point )
//
// Called by OLE dll's when cursor is dragged over a window that is REGISTERed with
// the OLE dll's.
//
//Parameters
//----------
// pWnd        -- points to the window that the cursor is over.
// pDataObject -- points to the data object that contains the data
//                to be dropped.
// dwKeyState  -- Contains the state of the modifier keys.
//                This is a combination of any number of the
//                following: 
//                MK_CONTROL, MK_SHIFT, MK_ALT, MK_LBUTTON,
//                MK_MBUTTON, and MK_RBUTTON.
// point       -- Contains the current location of the cursor in 
//                client coordinates.
// 
//Return Value:
//------------   The effect that would result if a drop were attempted
//               at the location specified by point. 
//               It can be one or more of the following: 
//
//DROPEFFECT_NONE   A drop would not be allowed.
//
//DROPEFFECT_COPY   A copy operation would be performed.
//
//DROPEFFECT_MOVE   A move operation would be performed.
//
//DROPEFFECT_LINK   A link from the dropped data to the original data would be established.
//
//DROPEFFECT_SCROLL   Indicates that a drag scroll operation is about to occur or is occurring in the target. 
//
// Note: OnDragOver is like WM_MOUSEMOVE message, in that CALLS TO IT
// ARE GENERATED REPEATEDLY as the pointer moves around the window
//----------------------------------------------------------------------------------
DROPEFFECT COleListCtrlDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* 
           pDataObject, DWORD dwKeyState, CPoint point )
{                              
	if(m_pWnd != NULL) {
		m_pWnd->SendMessage(WM_DRAGCHARTDATA, 0, 
					(LPARAM)MAKELONG(point.x, point.y));
		
	}
	DROPEFFECT deResult = DROPEFFECT_NONE;
	if(pDataObject->IsDataAvailable(CF_TEXT)) {
		if(dwKeyState & MK_CONTROL )
			deResult = DROPEFFECT_COPY;
		else
			deResult = DROPEFFECT_MOVE;
	}
	return deResult;
}

//---------------------------------------------------------------------------------- 
// BOOL COleListCtrlDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, 
//                 DROPEFFECT dropEffect, CPoint point )
//
// Called by OLE dll's when item is dropped in a window that is REGISTERed with the
// OLE dll's.
//
//Parameters
//----------
// pWnd        -- Points to the window the cursor is currently over.
//
// pDataObject -- points to the data object that contains the data
//                to be dropped.
//
// dropEffect  -- The effect that the user chose for the drop operation. It can be one or more of the following: 
//
//DROPEFFECT_COPY   A copy operation would be performed.
//
//DROPEFFECT_MOVE   A move operation would be performed.
//
//DROPEFFECT_LINK   A link from the dropped data to the original data would be established. 
//
// point       -- Contains the location of the cursor, in pixels,
//                relative to the screen.
//---------------------------------------------------------------------------------- 
BOOL COleListCtrlDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, 
                 DROPEFFECT dropEffect, CPoint point )
{       
   	HGLOBAL	hGlobal = NULL;
	hGlobal= pDataObject->GetGlobalData(CF_TEXT);
	if(!hGlobal)
		return false;
	LPCSTR pszObj = (LPCSTR)GlobalLock(hGlobal);
	ToolItem *pData = (ToolItem *)pszObj;
	if(pszObj)
	{
		m_pWnd->SendMessage(WM_DROPCHARTDATA, 0, (LPARAM) pData);
		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);
		return TRUE;
	}
	return FALSE; 
}

void COleListCtrlDropTarget::SetCurrentWindow(CWnd *pWnd)
{
	m_pWnd = pWnd;
}

CString CToolandRButtonBase_ListCtrl::SelectedItemNames()
{
	CString strSelected = "";
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos == NULL)
	   TRACE0("No items were selected!\n");
	else {
	   while (pos) {
		  int nItem = GetNextSelectedItem(pos);
     	  char plist[64];
		  memset(plist, 0, sizeof(plist));
		  GetItemText( nItem, 0, plist, 64);
		  CString temp = (CString)plist;
		  temp.TrimRight();
          strSelected += temp + ";";
	   }
	}
	return strSelected;
}

int CToolandRButtonBase_ListCtrl::SelectedItemCount()
{
   int nValue = 0;
   POSITION pos = GetFirstSelectedItemPosition();
   while (pos) {
	  int nItem = GetNextSelectedItem(pos);
	  nValue++;
   }
   return nValue;
}

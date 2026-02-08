// AxMListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AxMCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxMListCtrl

CAxMListCtrl::CAxMListCtrl()
{
	m_messageHandler = NULL;
}

CAxMListCtrl::~CAxMListCtrl()
{
}


BEGIN_MESSAGE_MAP(CAxMListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CAxMListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(AXM_SETMSGHANDLER, OnSetMessageHandler)
	ON_MESSAGE(AXM_GETMSGHANDLER, OnGetMessageHandler)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxMListCtrl message handlers

void CAxMListCtrl::SetMessageHandler(CWnd* wnd)
{
	m_messageHandler = wnd;
}

LRESULT CAxMListCtrl::Notify(UINT msg, WPARAM wParam, LPARAM lParam)
{
	NOTIFY(msg, wParam, lParam);
}

LRESULT CAxMListCtrl::OnSetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	SetMessageHandler((CWnd*)wParam);
	return 0L;
}

LRESULT CAxMListCtrl::OnGetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)m_messageHandler;
}

void CAxMListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CListCtrl::OnLButtonDown(nFlags, point);
	UINT uFlags=0;
	int index = HitTest(point, &uFlags);
	
	if ((uFlags & LVHT_ONITEM)) 
	{
		BOOL bLabel = ((uFlags & LVHT_ONITEMLABEL) == LVHT_ONITEMLABEL);
		Notify(WM_AXITEMCLICK, MAKEWPARAM(index, bLabel), (LPARAM)this);
	}

	OnDrag(point, index);
}

void CAxMListCtrl::OnDrag(CPoint point, int ListIndex)
{
	if (GetDlgCtrlID() != 1014)	// 자동목록
		return;

	if (ListIndex > GetItemCount())
		return;

	CString	strText = GetItemText(ListIndex, 0);
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


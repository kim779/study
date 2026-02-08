// ExListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ExListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExListCtrl

CExListCtrl::CExListCtrl()
{
	m_pControlPtrArray = new CPtrArray();
	m_nSelctedItemIdx = -1;
	m_nControlIDCounter = 786;
}

CExListCtrl::~CExListCtrl()
{
	ReleaseControls();
}


BEGIN_MESSAGE_MAP(CExListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CExListCtrl)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExListCtrl message handlers


BOOL CExListCtrl::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	BOOL bCreated = CListCtrl::Create(dwStyle, rect, pParentWnd,nID );
	return bCreated;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 8
// Return BOOL	: 
// Param  CString szEachRatio : "2:1:1"와 같은 형식으로 값을 넣어준다.
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CExListCtrl::SetColumnRatio( CString szEachRatio )
{
	CRect rcListCtrlRect;
	this->GetClientRect( rcListCtrlRect );

	CString szRatio = "";
	double dbRatio[ MAX_COLUMN ] = {-1,};


	// 2. 인자로 받은 전체 비율을 계산한다.
	int nCount;
	double dbAllRatio = 0;
	for( int nIdx = 0 ; nIdx < MAX_COLUMN ; nIdx++ )
	{
		nCount = szEachRatio.Find( ":", 0 );
		if( nCount < 0 )
		{
			dbRatio[nIdx] = atof(szEachRatio);
			dbAllRatio += dbRatio[nIdx];
			break;
		}

		szRatio = szEachRatio.Left( nCount );
		
		dbRatio[nIdx] = atof(szRatio);
		dbAllRatio += dbRatio[nIdx];

		szEachRatio = szEachRatio.Mid( nCount + 1 );
	}

	
	// 3. 전체 비율에 맞춰 각 Column의 크기를 폭을 계산한다.
	


	return TRUE;
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  int nRowIdx : 
// Param  int nColumnIdx : 
// Comments		: 
//-----------------------------------------------------------------------------
void CExListCtrl::SetItemControl( int nColumnIdx )
{
	SubItemControlsInfo *pSubItemInfo = new SubItemControlsInfo();
	pSubItemInfo->m_nIdx = nColumnIdx;
	
	if( m_nSelctedItemIdx < 0 )
		m_nSelctedItemIdx = 0;

	CFont *pFont = GetFont();
	DWORD dwStyle = WS_BORDER | WS_CHILD | WS_TABSTOP | LBS_NOINTEGRALHEIGHT | WS_VSCROLL;
	
	CRect ControlSize( 10, 10, 10 + GetColumnWidth( nColumnIdx ), 10 );
	
	pSubItemInfo->m_pWnd = new CListBox();
	((CListBox*)pSubItemInfo->m_pWnd)->Create( dwStyle, ControlSize, this, m_nControlIDCounter );
	pSubItemInfo->m_pWnd->SetFont(pFont);
	int iDupIndex = ControlIndexDuplicating( nColumnIdx );

	if(iDupIndex > -1)
	{
		DetachControl(iDupIndex);
	}

	m_pControlPtrArray->Add(pSubItemInfo);
	m_nControlIDCounter+=4;
}


void CExListCtrl::SetItemControl( int nRowIdx, int nColumnIdx )
{
	SubItemControlsInfo *pSubItemInfo = new SubItemControlsInfo();
	pSubItemInfo->m_nIdx = nColumnIdx;
	
	CFont *pFont = GetFont();
	DWORD dwStyle = WS_BORDER | WS_CHILD | WS_TABSTOP | LBS_NOINTEGRALHEIGHT | WS_VSCROLL;
	
	CRect ControlSize( 10, 10, 10 + GetColumnWidth( nColumnIdx ), 10 );
	pSubItemInfo->m_pWnd = new CListBox();
	((CListBox*)pSubItemInfo->m_pWnd)->Create( dwStyle, ControlSize, this, m_nControlIDCounter );
	pSubItemInfo->m_pWnd->SetFont(pFont);
	int iDupIndex = ControlIndexDuplicating( nColumnIdx );

	if(iDupIndex > -1)
	{
		DetachControl(iDupIndex);
	}

	m_pControlPtrArray->Add(pSubItemInfo);
	m_nControlIDCounter+=4;
}



int CExListCtrl::ControlIndexDuplicating(int nIndex)
{
	SubItemControlsInfo *pInfo;
	for( int i = 0 ;i < m_pControlPtrArray->GetSize() ; i++ )
	{
		pInfo = (SubItemControlsInfo *) m_pControlPtrArray->GetAt(i);
		
		if(pInfo->m_nIdx == nIndex)
			return i;
	}

	return -1;
}


BOOL CExListCtrl::DetachControl( int nIndex )
{
	SubItemControlsInfo * pInfo;
	
	pInfo = (SubItemControlsInfo *)m_pControlPtrArray->GetAt(nIndex);
	if(pInfo)
	{
		if(pInfo->m_pWnd)
		{
			pInfo->m_pWnd->DestroyWindow();
			pInfo->m_pWnd->m_hWnd = NULL;
			delete pInfo->m_pWnd;
			pInfo->m_pWnd = NULL;
		}
		delete pInfo;
		m_pControlPtrArray->RemoveAt(nIndex);
		return TRUE;
	}
	return FALSE;
}


void CExListCtrl::MapControlsForSubItems( void )
{
	int iItem = m_nSelctedItemIdx;
	CRect CtrlRect;
	CRect iconRect;
	SubItemControlsInfo * pSubItemControlsInfo;

	CString strData = "";
	for( int i = 0 ; i < m_pControlPtrArray->GetSize() ; i++ )
	{
		pSubItemControlsInfo = (SubItemControlsInfo *) m_pControlPtrArray->GetAt(i);

		if(pSubItemControlsInfo)
		{
			GetSubItemRect(iItem,pSubItemControlsInfo->m_nIdx,LVIR_LABEL,CtrlRect);
			GetSubItemRect(iItem,pSubItemControlsInfo->m_nIdx,LVIR_ICON,iconRect);
			
			//CtrlRect.bottom += 3;
			if(pSubItemControlsInfo->m_nIdx == 0)
			{
				CtrlRect.left += iconRect.right;
			}

			if(pSubItemControlsInfo->m_pWnd)
					pSubItemControlsInfo->m_pWnd->MoveWindow(&CtrlRect);

			strData = GetItemText(m_nSelctedItemIdx,pSubItemControlsInfo->m_nIdx);		
			
			((CListBox*)pSubItemControlsInfo->m_pWnd)->SelectString(-1,strData);
			//CtrlRect.bottom += (((CListBox*)pSubItemControlsInfo->m_pWnd)->GetCount ()-1)*2;
			((CListBox*)pSubItemControlsInfo->m_pWnd)->GetItemHeight(0);
			pSubItemControlsInfo->m_pWnd->MoveWindow(&CtrlRect);
			//break;
		}
	}
}


void CExListCtrl::ShowControls( BOOL bShow )
{
	SubItemControlsInfo * pSubItemControlsInfo;

	for(int i =0 ;i <m_pControlPtrArray->GetSize() ;i++)
	{
		pSubItemControlsInfo = (SubItemControlsInfo *) m_pControlPtrArray->GetAt(i);
		if(pSubItemControlsInfo)
		{
			if(pSubItemControlsInfo->m_pWnd)
				pSubItemControlsInfo->m_pWnd->ShowWindow(bShow);
		}
	}
}


void CExListCtrl::EnableControls(BOOL bEnable)
{
	SubItemControlsInfo * pSubItemControlsInfo;

	for(int i =0 ;i <m_pControlPtrArray->GetSize() ;i++)
	{
		pSubItemControlsInfo = (SubItemControlsInfo *) m_pControlPtrArray->GetAt(i);
		if(pSubItemControlsInfo)
		{
			if(pSubItemControlsInfo->m_pWnd)
				pSubItemControlsInfo->m_pWnd->EnableWindow(bEnable);
		}
	}
}

void CExListCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect  rc;
	GetParent()->GetClientRect(&rc);

	if (rc.Size() == CSize(0,0))
		return;

	CDC     MemoryDC;
	CBitmap Bitmap;
	CBrush  Brush;

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	int rcWndW = rcWnd.Width(), rcWndH = rcWnd.Height();

	// Paint to a compatible DC
	MemoryDC.CreateCompatibleDC(&dc);
	Bitmap.CreateCompatibleBitmap(&dc, rcWndW, rcWndH);
	CBitmap *pOldBitmap = MemoryDC.SelectObject(&Bitmap);

	// Fill background (does what WM_ERASEBKGND would do)
	Brush.CreateSolidBrush(GetBkColor());
	MemoryDC.FillRect(&CRect(0,0, rcWndW,rcWndH), &Brush);

	Print(&MemoryDC, PRF_CLIENT|PRF_CHILDREN);
	Print(&MemoryDC, PRF_NONCLIENT);

	// Offset the MemoryDC drawing rectangle by rcBounds 
	CRect rcDraw = rc - rc.TopLeft();

	// Dump bits to real device context.
	dc.BitBlt(rc.left,rc.top, rc.Width(),rc.Height(), &MemoryDC, rcDraw.left,rcDraw.top, SRCCOPY);

	MemoryDC.SelectObject(pOldBitmap);
	
	CListCtrl::OnPaint();	
	// Do not call CListCtrl::OnPaint() for painting messages
}


void CExListCtrl::ReleaseControls( void )
{
	for(int i =0 ;i < m_pControlPtrArray->GetSize() ;i++)
	{
		DetachControl(i);
	}
}

BOOL CExListCtrl::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CListCtrl::DestroyWindow();
}


BOOL CExListCtrl::AddToControlsData(int nIndex, CString strData)
{
	SubItemControlsInfo *pSubItemInfo;
	CRect CtrlRect;

	for( int i = 0 ; i < m_pControlPtrArray->GetSize() ; i++ )
	{
		pSubItemInfo = (SubItemControlsInfo *) m_pControlPtrArray->GetAt(i);
			
		if(pSubItemInfo->m_nIdx == nIndex)
		{
			((CListBox *)pSubItemInfo->m_pWnd)->AddString(strData);
			GetSubItemRect( nIndex, pSubItemInfo->m_nIdx, LVIR_LABEL, CtrlRect);
			((CListBox *)pSubItemInfo->m_pWnd)->SetItemHeight( nIndex, CtrlRect.Height() );
			return TRUE;
		}
	}
	return FALSE;
}


BOOL CExListCtrl::AddToControlsData(int nIndex, int nCurVal, int nStart, int nEnd )
{
	SubItemControlsInfo *pSubItemInfo;
	CRect CtrlRect;

	for( int i = 0 ; i < m_pControlPtrArray->GetSize() ; i++ )
	{
		pSubItemInfo = (SubItemControlsInfo *) m_pControlPtrArray->GetAt(i);
			
		if(pSubItemInfo->m_nIdx == nIndex)
		{
			((CListBox *)pSubItemInfo->m_pWnd)->SelItemRange( TRUE, nStart, nEnd );
			GetSubItemRect( nIndex, pSubItemInfo->m_nIdx, LVIR_LABEL, CtrlRect);
			((CListBox *)pSubItemInfo->m_pWnd)->SetItemHeight( nIndex, CtrlRect.Height() );
			((CListBox *)pSubItemInfo->m_pWnd)->SetCurSel( nCurVal );
			return TRUE;
		}
	}
	return FALSE;
}




void CExListCtrl::AdjustControlsAccordingToHeaderWidth(HD_NOTIFY *pNotify)
{
	if(m_nSelctedItemIdx < 0 )
		return;

	int iItem = pNotify->iItem;
	int iWidth = pNotify->pitem->cxy;
	int iSubItem = 0 ; 
	CRect CtrlRect;
	CRect iconRect;
	SubItemControlsInfo * pSubItemControlsInfo;

	CString strData = "";
	
	BOOL bSetItem = FALSE ;
	
	int iIncreaseWidth = 0;

	for( int i = 0 ; i < m_pControlPtrArray->GetSize() ; i++ )
	{
		pSubItemControlsInfo = (SubItemControlsInfo *) m_pControlPtrArray->GetAt(i);

		if(pSubItemControlsInfo)
		{
			GetSubItemRect(m_nSelctedItemIdx,pSubItemControlsInfo->m_nIdx,LVIR_LABEL,CtrlRect);
			GetSubItemRect(m_nSelctedItemIdx,pSubItemControlsInfo->m_nIdx,LVIR_ICON,iconRect);
			
			if(pSubItemControlsInfo->m_nIdx == iItem)
			{
				bSetItem = TRUE;
				if(iItem == 0)
					iIncreaseWidth = iWidth - (CtrlRect.Width()+iconRect.right);
				else
					iIncreaseWidth = iWidth - CtrlRect.Width();
			}
			else
			{
				CtrlRect.left +=iIncreaseWidth;
			}
			CtrlRect.right += iIncreaseWidth;
		
			if(pSubItemControlsInfo->m_pWnd)
			{
				if(bSetItem)
				{
					pSubItemControlsInfo->m_pWnd->MoveWindow(&CtrlRect);
				}
			}
			strData = GetItemText(m_nSelctedItemIdx,pSubItemControlsInfo->m_nIdx);		

			((CListBox*)pSubItemControlsInfo->m_pWnd)->SelectString(-1,strData);
			CtrlRect.bottom += (((CListBox*)pSubItemControlsInfo->m_pWnd)->GetCount ()-1)*((CListBox*)pSubItemControlsInfo->m_pWnd)->GetItemHeight(0);
			pSubItemControlsInfo->m_pWnd->MoveWindow(&CtrlRect);
		}
	}

	EnableControls(TRUE);
	ShowControls(TRUE);
	Invalidate();
}


void CExListCtrl::AttachEvent_HandlerForItemControl(int iIndex, UINT unMessage, void (__cdecl *pEventHandler)(LPVOID lpvoid))
{
	SubItemControlsInfo * pSubItemInfo;
	for(int i =0 ;i < m_pControlPtrArray->GetSize() ;i++)
	{
		pSubItemInfo = (SubItemControlsInfo *) m_pControlPtrArray->GetAt(i);
		if(pSubItemInfo->m_nIdx == iIndex)
		{	
			ControlMessageInfo MessageInfo;
			MessageInfo.m_iMessageID = unMessage;
			MessageInfo.pEventHandler = pEventHandler;
			pSubItemInfo->m_arrControlMessageInfo.Add(MessageInfo);
			break;
		}
	}
}

void CExListCtrl::SetControlBKColor(int index ,COLORREF col)
{
	SubItemControlsInfo  * pInfo;
	for(int i =0 ;i < m_pControlPtrArray->GetSize () ;i++)
	{
		pInfo = (SubItemControlsInfo *) m_pControlPtrArray->GetAt (i);
		if(pInfo->m_nIdx == index)
		{
			pInfo->m_BKColor = col;
		}
	}	
	Invalidate();
}


void CExListCtrl::GetAndDisplayControlsData()
{
	CString strData = "";
	int iCurIndex = -1 ;
	for(int i =0 ;i < m_pControlPtrArray->GetSize();i++)
	{
		SubItemControlsInfo * pSubItemInfo;

		pSubItemInfo = (SubItemControlsInfo *) m_pControlPtrArray->GetAt(i);
			
		iCurIndex = ((CListBox*)pSubItemInfo->m_pWnd)->GetCurSel();
		if(iCurIndex != LB_ERR)
		{
			((CListBox*)pSubItemInfo->m_pWnd)->GetText(iCurIndex,strData);
			SetItemText(m_nSelctedItemIdx ,pSubItemInfo->m_nIdx,strData);
		}
	}
}

BOOL CExListCtrl::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	SubItemControlsInfo * pSubItemInfo;
	ControlMessageInfo MessageInfo;
	if(nCode == HDN_TRACK)
	{
		AfxMessageBox("Error");
	}
	
	for(int i =0 ;i < m_pControlPtrArray->GetSize() ;i++)
	{
		pSubItemInfo = (SubItemControlsInfo *) m_pControlPtrArray->GetAt(i);
		if(pSubItemInfo->m_pWnd)
		{
			if(pSubItemInfo->m_pWnd->GetDlgCtrlID() == (int) nID)
			{
				for(int ii =0 ;ii < pSubItemInfo->m_arrControlMessageInfo.GetSize() ;ii++)
				{
					MessageInfo = (ControlMessageInfo)pSubItemInfo->m_arrControlMessageInfo.GetAt(ii);
					if(MessageInfo.m_iMessageID == nCode)
					{
						MessageInfo.pEventHandler(pSubItemInfo->m_pWnd);
						break;
					}
				}
			}
		}
	}

	return CListCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CExListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	
	LockWindowUpdate();
	///////// Check Item is in Visible Area ///////////////
	BOOL bShowItem = FALSE;
	int iTopMostItemIndex = GetTopIndex();
	for(int i = iTopMostItemIndex ; i < (iTopMostItemIndex + GetCountPerPage() ) ; i++)
	{
		if(m_nSelctedItemIdx == i)
		{
			bShowItem = TRUE;
			break;
		}
	}
	
	ShowControls(FALSE);
	/////////////////////////////////////////////////////	
	switch(nSBCode)
	{
		case SB_BOTTOM :
			
			break;
		case SB_ENDSCROLL:
			if(bShowItem)
			{
				ShowControls(TRUE);
				MapControlsForSubItems();
			}
			break;
		case SB_LINEDOWN :
			break;

		case SB_LINEUP:
			break;
		case SB_PAGEDOWN:
			break;
		case SB_PAGEUP	:
			break;
		case SB_THUMBPOSITION :
			break;
		case SB_THUMBTRACK	:
			break;
		case SB_TOP :
			break;
	}
	UnlockWindowUpdate();
}

void CExListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);

	LockWindowUpdate();
	///////// Check Item is in Visible Area ///////////////
	BOOL bShowItem = FALSE;
	int iTopMostItemIndex = GetTopIndex();
	for(int i = iTopMostItemIndex ; i < (iTopMostItemIndex + GetCountPerPage() ) ; i++)
	{
		if(m_nSelctedItemIdx == i)
		{
			bShowItem = TRUE;
			break;
		}
	}
	
	ShowControls(FALSE);
	/////////////////////////////////////////////////////	
	switch(nSBCode)
	{
		case SB_BOTTOM :
			
			break;
		case SB_ENDSCROLL:
			if(bShowItem)
			{
				ShowControls(TRUE);
				MapControlsForSubItems();
			}
			break;
		case SB_LINERIGHT:
			break;
		case SB_LINELEFT :
			break;
		case SB_PAGELEFT:
			break;
		case SB_PAGERIGHT	:
			break;
		case SB_THUMBPOSITION :
			break;
		case SB_THUMBTRACK	:
			break;
		case SB_TOP :
			break;
	}
	UnlockWindowUpdate();
}

void CExListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_nSelctedItemIdx > -1)
	{
		GetAndDisplayControlsData();
	}
	
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW*) pNMHDR;
	
	if( pNMListView->iItem < 0 || pNMListView->iSubItem < 0 )
	{
		return;
	}

	int iItem = pNMListView->iItem;
	m_nSelctedItemIdx = iItem;
	m_nSubItem = pNMListView->iSubItem;

	if(iItem >= 0)
	{
		BOOL bChecked = TRUE;

		MapControlsForSubItems();
		ShowControls(TRUE);
		EnableControls(bChecked);
		SetItemState(m_nSelctedItemIdx,LVIS_SELECTED,LVIS_SELECTED);
	}
	
	*pResult = 0;
}


HBRUSH CExListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CListCtrl::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	SubItemControlsInfo * pItemInfo;
	for(int i =0 ;i < m_pControlPtrArray->GetSize() ;i++)
	{
		pItemInfo = (SubItemControlsInfo*) m_pControlPtrArray->GetAt(i);
		if(pWnd == pItemInfo->m_pWnd)
		{
			hbr = ::CreateSolidBrush(pItemInfo->m_BKColor);
		}
	}	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}



BOOL CExListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CListCtrl::OnEraseBkgnd(pDC);
	return TRUE;
}

void CExListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	switch(nChar)
	{
		case VK_UP:
			if(m_nSelctedItemIdx-1 > -1)
				m_nSelctedItemIdx--;
		break;
		case VK_DOWN:
			if(m_nSelctedItemIdx+1 < GetItemCount())
				m_nSelctedItemIdx++;
		break;
		case VK_LEFT:
			 if(m_nSubItem-1 < -1)
				 m_nSubItem--;
		break;
		case VK_RIGHT:
			if(m_nSubItem +1 < GetHeaderCtrl()->GetItemCount())
				m_nSubItem++;
		break;

	}
	BOOL bChecked = GetCheck(m_nSelctedItemIdx );
	ShowControls(TRUE);
	EnableControls(bChecked);
	MapControlsForSubItems();
	SetItemState(m_nSelctedItemIdx,LVIS_SELECTED,LVIS_SELECTED);
		
	return; 
}

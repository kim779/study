// EditableListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "EditableListCtrl.h"

#include "LVEdit.h"	// for CLVEdit

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditableListCtrl

CEditableListCtrl::CEditableListCtrl()
{

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.08
//
//	목적	:	Label 편집시의 Edit Control을 관리한다.
///////////////////////////////////////////////////////////////////////////////
	m_pLabelEdit = new CLVEdit;
}

CEditableListCtrl::~CEditableListCtrl()
{

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.08
//
//	목적	:	Label 편집시의 Edit Control을 관리한다.
///////////////////////////////////////////////////////////////////////////////
	delete m_pLabelEdit;
}


BEGIN_MESSAGE_MAP(CEditableListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CEditableListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditableListCtrl message handlers

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	Editable List Control의 기본 Style을 설정한다.
//
//	수정	:	(2003.01.08) 기본 속성에 LVS_EDITLABELS도 추가한다.
//				(2003.01.08) 기본 속성으로 항상 Report Type이 되도록 한다.
///////////////////////////////////////////////////////////////////////////////
void CEditableListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	ModifyStyle( LVS_ICON | LVS_SMALLICON | LVS_LIST, LVS_SINGLESEL | LVS_EDITLABELS | LVS_REPORT);
	SetExtendedStyle( LVS_EX_FULLROWSELECT);


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.08
//
//	목적	:	Label 편집시의 Edit Control을 관리한다.
//
//	내용	:	보이지 않게 생성하여 놓는다.
///////////////////////////////////////////////////////////////////////////////
	CRect rctLabelEdit( 0, 0, 0, 0);
	m_pLabelEdit->Create( WS_BORDER | ES_AUTOHSCROLL, rctLabelEdit, this, 107);
	m_pLabelEdit->SetFont( GetFont());

	CListCtrl::PreSubclassWindow();
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	선택한 Item을 다시 Click한 경우에는 Edit Control을 생성하여 보여준다.
///////////////////////////////////////////////////////////////////////////////
void CEditableListCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	EditSubItem( point);

	*pResult = 1;
}
void CEditableListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	EditSubItem( point);

	CListCtrl::OnLButtonDblClk(nFlags, point);
}
void CEditableListCtrl::EditSubItem( CPoint point)
{
	// 1. 먼저 선택된 Item의 Index를 구하여 Selection 여부를 확인한다.
	//	(Column Info도 확인할 수 있도록 SubItemHitTest를 이용한다.)
	LVHITTESTINFO lvInfo;
	lvInfo.pt = point;
	SubItemHitTest( &lvInfo);

	// 2. Index로 SubItem 편집을 진행한다.
	EditSubItem( lvInfo.iItem, lvInfo.iSubItem);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	Index로 SubItem을 편집케 할 수 있도록 Index의 Interface도 제공한다.
//
//	수정	:	(2003.01.08) 편집창의 생성이 성공적이었는지 확인할 수 있도록 BOOL을 Return한다.
//				(2003.01.08) 편집창의 생성이 성공한 경우에는 List Control을 Scoll하여 볼 수 있도록 한다.
///////////////////////////////////////////////////////////////////////////////
BOOL CEditableListCtrl::EditSubItem( int p_nItem, int p_nSubItem)
{
	// 1. 선택한 Column이 편집 가능인지 확인하고 처리한다.
	if( p_nItem < 0 || p_nItem >= GetItemCount() || !m_bEditableColumn[ p_nSubItem]) return FALSE;

	// 2. 선택된 Item을 Click한 경우에는 해당 Column의 위치에 Edit Control을 생성한다.
	if( !GetItemState( p_nItem, LVIS_SELECTED)) return FALSE;

	// (2003.01.08) 편집창의 생성이 성공한 경우에는 List Control을 Scoll하여 볼 수 있도록 한다.
	// 2.5.1 Scroll시에 Edit의 잔영이 남지 않도록 우선 Hide시킨다.
	m_pLabelEdit->ShowWindow( SW_HIDE);
	// 2.5.2 각 SubItem의 위치 정보를 확인할 수 있도록 사전에 Scoll 시킨다.
	EnsureVisible( p_nItem, FALSE);
	// 2.5.3 해당 Item의 Sub Item 위치를 구해 조정한다.
	CRect rctSubItem, rctClientRect;
	GetSubItemRect( p_nItem, p_nSubItem, LVIR_LABEL, rctSubItem);
	if( rctSubItem.left < 0) Scroll( CSize( rctSubItem.left, 0));
	else
	{
		// 2.5.4 Edit Control의 우측이 Scoll영역 밑으로 들어가는 경우에도 Scroll하여 보여준다.
		GetClientRect( rctClientRect);
		if( rctSubItem.right > rctClientRect.right)
		{
			int nScroll = rctSubItem.right - rctClientRect.right;
			if( rctSubItem.left - nScroll < 0) Scroll( CSize( rctSubItem.left, 0));
			else Scroll( CSize( nScroll, 0));
		}
	}

	// 3. 해당 Item의 Sub Item 위치를 다시 구한다.
	GetSubItemRect( p_nItem, p_nSubItem, LVIR_LABEL, rctSubItem);
	rctSubItem.InflateRect( 0, 2, 0, 1);

	// 4. Edit Control은 Scroll 영역 밑으로 들어가지 않도록 크기를 조정한다.
	GetClientRect( rctClientRect);
	if( rctSubItem.left < 0) rctSubItem.left = 0;
	if( rctSubItem.right > rctClientRect.right) rctSubItem.right = rctClientRect.right;

	// 5. 편집할 Sub Item의 위치에 Edit Control을 생성하여 보여준다.
	m_pLabelEdit->m_nItem = p_nItem;
	m_pLabelEdit->m_nSubItem = p_nSubItem;
	m_pLabelEdit->MoveWindow( rctSubItem);
	m_pLabelEdit->ShowWindow( SW_SHOW);

	// 6. 편집할 기본 Data를 보여준다.
	//	(특히 빠른 편집이 가능하도록 기본적으로 선택을 설정한다.)
	m_pLabelEdit->SetWindowText( GetItemText( p_nItem, p_nSubItem));
	m_pLabelEdit->SetFocus();
	m_pLabelEdit->SetSel( 0, -1);

	// 7. Label Edit 작업중이기때문인지 Item Label의 Highlight가 Focus 손실로 복원되지 않는다.
	//	(강제로 Drawing시킨다.)
	GetSubItemRect( p_nItem, 0, LVIR_LABEL, rctSubItem);
	InvalidateRect( rctSubItem);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	Enter Key나 F2를 이용하여 선택된 Item을 편집할 수 있도록한다.
//
//	참고	:	1. 특히 처리가 완료된 경우에는 다른 Window가 Message를 받지 못하도록 True를 Return한다.
//				2. 다른 Class에 이미 PreTranslateMessage()가 있어서인지, OnKeyDown()이 없으면,
//					PreTranslateMessage()가 불리지않는다. (원이 미확인. 나눠서 처리 설정)
///////////////////////////////////////////////////////////////////////////////
BOOL CEditableListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		StartEditSubItem();
		return TRUE;
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}
void CEditableListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	if( nChar == VK_F2)	StartEditSubItem();

	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	Key Input으로 편집을 시작하는 Routine를 추가한다.
///////////////////////////////////////////////////////////////////////////////
void CEditableListCtrl::StartEditSubItem( void)
{
	int nItem = GetNextItem( -1,  LVNI_SELECTED);
	int nSubItem, nColumnCount = m_bEditableColumn.GetSize();
	for( nSubItem = 0; nSubItem < nColumnCount; nSubItem++) if( m_bEditableColumn[ nSubItem]) break;
	EditSubItem( nItem, nSubItem);
}


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	Scroll 시도시에는 Label Edit Control을 숨긴다.
///////////////////////////////////////////////////////////////////////////////
void CEditableListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	m_pLabelEdit->ShowWindow( SW_HIDE);
	
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}
void CEditableListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	m_pLabelEdit->ShowWindow( SW_HIDE);
	
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}
BOOL CEditableListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	m_pLabelEdit->ShowWindow( SW_HIDE);
	
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.02.21
//
//	목적	:	Enter를 이용하여야만 Update를 지원할지 여부를 설정하는 Interface를 관리한다.
//
//	내용	:	1. 기본적으로 Flase이며, Enter외에도 단순히 Kill Focus 시점에도 Update된다.
//				2. 그렇다 하여도, Esc Key를 이용한 Kill Focus의 시점에는 Update되지 않는다.
///////////////////////////////////////////////////////////////////////////////
void CEditableListCtrl::SetUpdateOnlyEnter( BOOL p_bUpdateOnlyEnter)
{
	m_pLabelEdit->m_bUpdateOnlyEnter = p_bUpdateOnlyEnter;
}

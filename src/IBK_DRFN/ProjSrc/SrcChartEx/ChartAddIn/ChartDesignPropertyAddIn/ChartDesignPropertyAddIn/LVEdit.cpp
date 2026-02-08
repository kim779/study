// LVEdit.cpp : implementation file
//

#include "stdafx.h"
#include "LVEdit.h"

#include "EditableListCtrl.h"	// for CEditableListCtrl

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLVEdit

CLVEdit::CLVEdit()
{

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.08
//
//	목적	:	Ctrl Key의 눌린 상태를 나타내는 Flag을 관리한다.
///////////////////////////////////////////////////////////////////////////////
	m_bCtrl = FALSE;

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.02.21
//
//	목적	:	Enter를 이용하여야만 Update를 지원할지 여부를 나타내는 Flag을 관리한다.
//
//	내용	:	1. 기본적으로 Flase이며, Enter외에도 단순히 Kill Focus 시점에도 Update된다.
//				2. 그렇다 하여도, Esc Key를 이용한 Kill Focus의 시점에는 Update되지 않는다.
///////////////////////////////////////////////////////////////////////////////
	m_bUpdateOnlyEnter = FALSE;
}

CLVEdit::~CLVEdit()
{
}


BEGIN_MESSAGE_MAP(CLVEdit, CEdit)
	//{{AFX_MSG_MAP(CLVEdit)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLVEdit message handlers

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	Enter가 입력면 Focus를 잃을때 편집중이던 값을 반영하고 종료시킨다.
//
//	내용	:	이때 하위 Item이 있으면 다음 편집으로 진행한다.
//
//	참고	:	특히 처리가 완료된 경우에는 다른 Window가 Message를 받지 못하도록 True를 Return한다.
//
//	수정	:	(2003.01.08) Label Edit Control을 매번 생성하지 않고, 하나만을 생성하여 유지하도록 조정한다.
//				(2003.01.08) 또한 Label Edit의 이동도 Ctrl Key를 누른 상태의 Arrow로 변경한다.
//				(2003.02.21) UpdateOnlyEnter Option을 제공하기 위하여, Update Routine을 별도로 제공한다.
//				(2003.02.21) ESC에 의한 Kill Focus시에는 Update가 되지 않도록 m_nSubItem을 확인한다.
//					m_nSubItem이 -1이면 ESC가 눌린 다음이다.
///////////////////////////////////////////////////////////////////////////////
BOOL CLVEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	// 0. 먼저 ESC인 경우에는 종료시킨다.
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)		
	{
		// (2003.02.21) ESC에 의한 Kill Focus시에는 Update가 되지 않도록 m_nSubItem을 확인한다.
		//	m_nSubItem이 -1이면 ESC가 눌린 다음이다.
		m_nSubItem = -1;

		ShowWindow( SW_HIDE);
		return TRUE;
	}

	// 0.5 입력이 완료된 경우를 처리한다.
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)		
	{
		// 1. 값을 반영한다.
		//	(2003.02.21) UpdateOnlyEnter Option을 제공하기 위하여, Update Routine을 별도로 제공한다.
		UpdateToListCtrl();

		// 2. Hidden 시킨다.
		ShowWindow( SW_HIDE);

		// 3. 다음 Item의 편집을 진행한다.
		// 3.1 다음 Item이 있는지 확인한다.
		int nNextItem = m_nItem + 1;
		if( nNextItem <= m_pEditableListCtrl->GetItemCount())
		{
			// 3.2 다음 Item을 선택시킨다.
			m_pEditableListCtrl->SetItemState( nNextItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_pEditableListCtrl->EditSubItem( nNextItem, m_nSubItem);
		}

		return TRUE;
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	Focus를 잃으면 편집중이던 값을 무시하고 그냥 종료시킨다.
//
//	수정	:	(2003.01.08) Label Edit Control을 매번 생성하지 않고, 하나만을 생성하여 유지하도록 조정한다.
//				(2003.02.21) UpdateOnlyEnter Option이 아니라면 KillFocus시에 Update시킨다.
///////////////////////////////////////////////////////////////////////////////
void CLVEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
	// (2003.02.21) UpdateOnlyEnter Option이 아니라면 KillFocus시에 Update시킨다.
	if( !m_bUpdateOnlyEnter) UpdateToListCtrl();

	ShowWindow( SW_HIDE);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	Alt+Arrow Key가 입력되면 편집창을 이동시킨다.
//
//	참고	:	1. 먼저 Ctrl Key를 이용하려 하였으나, 별도의 Flag를 관리하여야 하고,
//					연동에 문제가 생겨 Alt로 전환하였다.
//				2. 그러나 Alt는 Message 자체가 전달되지 않는 문제가 있어 Caret의 위치로 설정하였다.
//
//	수정	:	(2003.01.08) Label Edit의 이동도 Ctrl Key를 누른 상태의 Arrow로 변경한다.
///////////////////////////////////////////////////////////////////////////////
void CLVEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	// 0. Ctrl Key를 확인한다.
	if( nChar == VK_CONTROL) m_bCtrl = TRUE;

	// 1. Ctrl+Arrow Key인 경우를 처리한다.
	else if( m_bCtrl)
	{
		if( nChar == VK_UP)		if( TryEditSubItem( m_nItem - 1,	m_nSubItem))		return;
		if( nChar == VK_DOWN)	if( TryEditSubItem( m_nItem + 1,	m_nSubItem))		return;

		// 2. Left와 Right는 편집 불가능 Column을 건너뛸 수 있도록 한다.
		if( nChar == VK_LEFT)
		{
			for(  i = m_nSubItem - 1; i >= 0; i--) if( m_pEditableListCtrl->m_bEditableColumn[ i]) break;
			if( i >= 0) if( TryEditSubItem( m_nItem, i))	return;
		}
		if( nChar == VK_RIGHT)
		{
			int nColumn = m_pEditableListCtrl->m_bEditableColumn.GetSize();
			for( int i = m_nSubItem + 1; i < nColumn; i++) if( m_pEditableListCtrl->m_bEditableColumn[ i]) break;
			if( i < nColumn) if( TryEditSubItem( m_nItem, i))	return;
		}
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CLVEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	// 0. Ctrl Key를 확인한다.
	if( nChar == VK_CONTROL) m_bCtrl = FALSE;
	
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	Ctrl+Arrow Key를 통하여 편집 Sub Item의 조정을 시도하는 Routine를 제공한다.
//
//	수정	:	(2003.01.08) 이동 성공 여부를 확인하기 위하여 BOOL을 Return한다.
///////////////////////////////////////////////////////////////////////////////
BOOL CLVEdit::TryEditSubItem( int p_nItem, int p_nSubItem)
{
	// 1. 해당 Index가 편집 가능인지를 확인한다.
	if( p_nItem < 0 || p_nItem >= m_pEditableListCtrl->GetItemCount() || p_nSubItem < 0 || p_nSubItem >= m_pEditableListCtrl->m_bEditableColumn.GetSize()) return FALSE;
	if( !m_pEditableListCtrl->m_bEditableColumn[ p_nSubItem]) return FALSE;

	// 2. 다음 Item을 선택시킨다.
	m_pEditableListCtrl->SetItemState( p_nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	return m_pEditableListCtrl->EditSubItem( p_nItem, p_nSubItem);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.02.21
//
//	목적	:	UpdateOnlyEnter Option을 제공하기 위하여, Update Routine을 별도로 제공한다.
//
//	수정	:	(2003.01.29) Byte Array를 BOOL Array로 활용하였으나, 다음과 같이 Flag으로 활용하여 
//					ReadOnly, CopyOnly, Editable로 분류하여 처리토록한다.
//					0	: ReadOnly
//					1	: CopyOnly
//					2	: Editable
//				(2003.01.31) Parent Window에게 편집완료됨을 알리는 Notification을 처리한다.
//				(2003.02.02) Notification에 수정 이전의 Data도 전달하도록 한다.
//				(2003.02.21) ESC에 의한 Kill Focus시에는 Update가 되지 않도록 m_nSubItem을 확인한다.
//					m_nSubItem이 -1이면 ESC가 눌린 다음이다.
///////////////////////////////////////////////////////////////////////////////
void CLVEdit::UpdateToListCtrl( void)
{
	// (2003.02.21) ESC에 의한 Kill Focus시에는 Update가 되지 않도록 m_nSubItem을 확인한다.
	//	m_nSubItem이 -1이면 ESC가 눌린 다음이다.
	if( m_nSubItem < 0) return;

	//	(2003.01.29) Byte Array를 BOOL Array로 활용하였으나, 다음과 같이 Flag으로 활용하여 
	//		ReadOnly, CopyOnly, Editable로 분류하여 처리토록한다.
	if( m_pEditableListCtrl->m_bEditableColumn[ m_nSubItem] == 2)
	{
		// (2003.02.02) Notification에 수정 이전의 Data도 전달하도록 한다.
		CString strOldValue = m_pEditableListCtrl->GetItemText( m_nItem, m_nSubItem);

		CString strNewValue;
		GetWindowText( strNewValue);
		m_pEditableListCtrl->SetItemText( m_nItem, m_nSubItem, strNewValue);

		// (2003.01.31) Parent Window에게 편집완료됨을 알리는 Notification을 처리한다.
		NMEDITABLELISTCTRL nmEditableListCtrl;
		nmEditableListCtrl.m_nItemIndex = m_nItem;
		nmEditableListCtrl.m_nColumnIndex = m_nSubItem;
		nmEditableListCtrl.m_szOldData = strOldValue;
		nmEditableListCtrl.m_szNewData = strNewValue;
		m_pEditableListCtrl->GetParent()->SendMessage( RMSG_ELC_CELLEDITED, m_pEditableListCtrl->GetDlgCtrlID(), ( LPARAM)&nmEditableListCtrl);
	}
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.02.21
//
//	목적	:	Parent인 List Control의 Pointer를 관리한다.
///////////////////////////////////////////////////////////////////////////////
BOOL CLVEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class
	
	m_pEditableListCtrl = ( CEditableListCtrl *)pParentWnd;
	
	return CEdit::Create( dwStyle, rect, pParentWnd, nID);
}

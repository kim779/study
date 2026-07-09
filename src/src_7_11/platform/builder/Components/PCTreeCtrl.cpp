// $Workfile:   PCTreeCtrl.cpp  $$Revision:   1.0  $$Date:   02 MAY 2002 15:33:28  $
/*
*************************************************************************************************************
*																										
*	Module Name:	PCTreeCtrl.cpp
*	
*	Func Group:		
*	
*	Description:	MFC Tree Control when, Tooltip is checked for the Tree Ctrl in 
*					rsrc editor, Tooltips are created only for those TreeItems that exceed
*					the Text size that does'nt fit in the visible client area of the Tree ctrl.
*					So I had to show the tooltips even when the window was shrunk (MFC Tree ctrl shall
*					display tooltips that were initially bigger to fit into the visible client area).Also
*					this Tooltip displays strings of any length.
*					
*					
*	History:		Programmer		Date		Comment
*					-----------------------------------------------
*					Megavarnan  	05/02/02	Created module.
*					
*
*	Copyright:		(C) 2002-03 by Megavarnan Selvaraj
*	 
**************************************************************************************************************
*/ 

// PCTreeCtrl.cpp : implementation file
//
#include "stdafx.h"
#include "PCTreeCtrl.h"


BEGIN_MESSAGE_MAP(CPCTreeCtrl,CTreeCtrl)
	//{{AFX_MSG_MAP(CPCTreeCtrl)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CPCTreeCtrl::CPCTreeCtrl()
{
	m_pchTip = NULL;
	m_pwchTip = NULL;
}

CPCTreeCtrl::~CPCTreeCtrl()
{
	if(m_pwchTip != NULL)
		delete m_pwchTip;
	
	if(m_pchTip != NULL)
		delete m_pchTip;
}


void CPCTreeCtrl::PreSubclassWindow()
{
	CTreeCtrl::PreSubclassWindow();
	EnableToolTips(TRUE);
}

int CPCTreeCtrl::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	RECT rect;
	
	UINT nFlags;
	HTREEITEM hitem = HitTest( point, &nFlags );
	if( nFlags & TVHT_ONITEM)
	{
		GetItemRect( hitem, &rect, TRUE );
		pTI->hwnd = m_hWnd;
		pTI->uId = (UINT)hitem;
		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->rect = rect;
		return pTI->uId;
	}
	
	return -1;
}


BOOL CPCTreeCtrl::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT nID = pNMHDR->idFrom;
	
	// Do not process the message from built in tooltip 
	if( nID == (UINT)m_hWnd &&
		(( pNMHDR->code == TTN_NEEDTEXTA && pTTTA->uFlags & TTF_IDISHWND ) ||
		( pNMHDR->code == TTN_NEEDTEXTW && pTTTW->uFlags & TTF_IDISHWND ) ) )
		return FALSE;
	
	// Get the mouse position
	const MSG* pMessage;
	CPoint pt;
	pMessage = GetCurrentMessage();
	ASSERT ( pMessage );
	pt = pMessage->pt;
	ScreenToClient( &pt );
	
	UINT nFlags;
	HTREEITEM hitem = HitTest( pt, &nFlags );
	if( nFlags & TVHT_ONITEM)
	{
		CString strTxt = GetItemText((HTREEITEM)nID);
		strTipText = strTxt;
	}
	
#ifndef _UNICODE
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip != NULL)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()+1];
		lstrcpyn(m_pchTip, strTipText, strTipText.GetLength());
		m_pchTip[strTipText.GetLength()] = 0;
		pTTTW->lpszText = (WCHAR*)m_pchTip;
	}
	else
	{
		if(m_pwchTip != NULL)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()+1];
		_mbstowcsz(m_pwchTip, strTipText, strTipText.GetLength());
		m_pwchTip[strTipText.GetLength()] = 0; // end of text
		pTTTW->lpszText = (WCHAR*)m_pwchTip;
	}
#else
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip != NULL)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()+1];
		_wcstombsz(m_pchTip, strTipText, strTipText.GetLength());
		m_pchTip[strTipText.GetLength()] = 0; // end of text
		pTTTA->lpszText = (LPTSTR)m_pchTip;
	}
	else
	{
		if(m_pwchTip != NULL)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()+1];
		lstrcpyn(m_pwchTip, strTipText, strTipText.GetLength());
		m_pwchTip[strTipText.GetLength()] = 0;
		pTTTA->lpszText = (LPTSTR) m_pwchTip;
	}
#endif
	
	*pResult = 0;
	
	return TRUE;    // message was handled
}

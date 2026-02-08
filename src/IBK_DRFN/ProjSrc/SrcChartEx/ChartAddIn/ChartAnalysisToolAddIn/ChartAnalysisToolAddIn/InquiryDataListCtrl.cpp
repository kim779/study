// InquiryDataListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "InquiryDataListCtrl.h"
#include "NumericalInquiryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInquiryDataListCtrl

CInquiryDataListCtrl::CInquiryDataListCtrl()
{
}

CInquiryDataListCtrl::~CInquiryDataListCtrl()
{
}


BEGIN_MESSAGE_MAP(CInquiryDataListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CInquiryDataListCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInquiryDataListCtrl message handlers


/*-------------------------------------------------------------------------------
 - Function    :  OnMouseMove
 - Created at  :  2005-04-13   17:50
 - Author      :  최종찬
 - Description :  ListCtrl에서는 MouseMove를 처리할것이 없다. 다만 ListCtrl에서 받은
				  메세지를 CToolBarManagerImplementation에서 처리하도록 해준다.
 -------------------------------------------------------------------------------*/
void CInquiryDataListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// (2006/2/15 - Seung-Won, Bae) for Manual Move
	if( MK_LBUTTON & nFlags)
	{
		// 1. Make Difference
		CSize szMove = point - m_ptStart;
		if( 0 == szMove.cx && 0 == szMove.cy) return;
		
		// 2. Move Window
		CWnd *pParentWnd = GetParent();
		CRect rctWindow;
		pParentWnd->GetWindowRect( &rctWindow);
		rctWindow.OffsetRect( szMove);
		pParentWnd->GetParent()->ScreenToClient( &rctWindow);
		pParentWnd->MoveWindow( rctWindow);
	}
	else
	{
		CPoint ptCurrent;
		GetCursorPos(&ptCurrent);
		((CNumericalInquiryDlg*)GetParent())->OnMouseMove(ptCurrent);
	}
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/2/15
// Modifier		: 
// Comments		: for Drag Move (If you can drag. It means Manual Move.
//-----------------------------------------------------------------------------
void CInquiryDataListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	// 1. Backup Start Point
	m_ptStart = point;

	// 2. Take mouse messages more.
	//	SetCapture();					// It does not work!
	
	CListCtrl::OnLButtonDown(nFlags, point);
}

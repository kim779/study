// GridDropTarget.cpp : implementation file
//
// MFC Grid Control - Drag/Drop target implementation
//
// CGridDropTarget is an OLE drop target for CGridCtrl. All it does
// is handle the drag and drop windows messages and pass them
// directly onto the grid control.
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2001. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.10+
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCtrl.h"

#ifndef GRIDCONTROL_NO_DRAGDROP
#include "GridDropTarget.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridDropTarget
HWND CGridDropTarget::m_staticGridHwnd = NULL;

CGridDropTarget::CGridDropTarget()
{
    m_pGridCtrl			 = NULL;
    m_bRegistered		 = FALSE;
}

CGridDropTarget::~CGridDropTarget()
{
}

// Overloaded Register() function performs the normal COleDropTarget::Register
// but also serves to connect this COleDropTarget with the parent grid control,
// where all drop messages will ultimately be forwarded.
BOOL CGridDropTarget::Register(CGridCtrl *pGridCtrl)
{
    if (m_bRegistered)
        return FALSE;

    // Stop re-entry problems
    static BOOL bInProcedure = FALSE;
    if (bInProcedure)
        return FALSE;
    bInProcedure = TRUE;

    ASSERT(pGridCtrl->IsKindOf(RUNTIME_CLASS(CGridCtrl)));
    ASSERT(pGridCtrl);

    if (!pGridCtrl || !pGridCtrl->IsKindOf(RUNTIME_CLASS(CGridCtrl)))
    {
        bInProcedure = FALSE;
        return FALSE;
    }

    m_pGridCtrl = pGridCtrl;

    m_bRegistered = COleDropTarget::Register(pGridCtrl);

    bInProcedure = FALSE;
    return m_bRegistered;
}

void CGridDropTarget::Revoke()
{
    m_bRegistered = FALSE;
    COleDropTarget::Revoke();
}

// Drag N' Drop Start -> Set Grid hwnd
void CGridDropTarget::SetGridHwnd_DragBegin()
{
	m_staticGridHwnd = this->m_hWnd;
}

// Drag N' Drop End	  -> Release Grid hwnd
void CGridDropTarget::RelGridHwnd_DragEnd()
{
	m_staticGridHwnd = NULL;
}


BEGIN_MESSAGE_MAP(CGridDropTarget, COleDropTarget)
    //{{AFX_MSG_MAP(CGridDropTarget)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridDropTarget message handlers

DROPEFFECT CGridDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint /*point*/)
{
    //TRACE("In CGridDropTarget::OnDragScroll\n");
    //if (pWnd->GetSafeHwnd() == m_pGridCtrl->GetSafeHwnd())
    {
		// 수 정 자 : 김용식 (2002년2월14일   16시55분)
		// 수정사항 : 카피 방지
		/*
		if (dwKeyState & MK_CONTROL)
            return DROPEFFECT_COPY;
        else
		*/
		// 수정완료 :(2002년2월14일 16시55분)        
            return DROPEFFECT_MOVE;
    } 
	//else
    //    return DROPEFFECT_NONE;
}

DROPEFFECT CGridDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
    TRACE(_T("In CGridDropTarget::OnDragEnter\n"));
    ASSERT(m_pGridCtrl);

	//if (pWnd->GetSafeHwnd() == m_pGridCtrl->GetSafeHwnd())
	return m_pGridCtrl->OnDragEnter(pDataObject, dwKeyState, point);
    //else
    //    return DROPEFFECT_NONE;
}

void CGridDropTarget::OnDragLeave(CWnd* pWnd)
{
    TRACE(_T("In CGridDropTarget::OnDragLeave\n"));
    ASSERT(m_pGridCtrl);

    //if (pWnd->GetSafeHwnd() == m_pGridCtrl->GetSafeHwnd())
    m_pGridCtrl->OnDragLeave();
}

DROPEFFECT CGridDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
 //   TRACE("In CGridDropTarget::OnDragOver\n");
    ASSERT(m_pGridCtrl);

	// JSJ_Add_040616 COMMENT: 드래그 시작 그리드와 타겟 그리드가 틀리면 리턴
	if (m_staticGridHwnd != m_pGridCtrl->m_hWnd)
		return 	DROPEFFECT_NONE;


	CCellID cell, cellLast;
	cell = m_pGridCtrl->GetCellFromPt(point);
	cellLast = m_pGridCtrl->GetLeftClickDownCell();
//	TRACE("[Before] cell.row = %d, cell.col = %d    cellLast.row = %d, cellLast.col = %d\n", 
//		cell.row, cell.col, cellLast.row, cellLast.col);

	
	// 	JSJ_Add_0405610 ---------------------------------------->>
	// 최소 타이틀 다이아로그 드래그 오버시에 Show, Hide 메시지
	if ( cell.row > TENHOGA_ROW_FIRST)
	{
		if (cell.col > MAEDO_COL && cell.col < MAESU_COL)
		{
			m_pGridCtrl->SendMessageToCancelDlg(cell.row, cell.col, UDM_CREATE_CANCEL_DLG);
			m_pGridCtrl->ReleaseAllSelectedCell();	// 열선택 해제
		}
	}
	// <<----------------------------------------------------------

	// JSJ_ITCastle_Modify
	// 드래그 시작점이 매수주문 이거나, 매도주문 행이 아니면 리턴
	if( !(cellLast.col == MAEDO_COL || cellLast.col == MAESU_COL || 
		  cellLast.col == MAEDO_RESERVE_COL || cellLast.col == MAESU_RESERVE_COL || cellLast.col == HOGA_COL) )
	{
		return DROPEFFECT_NONE;
	}

	// 취소 행의 추가로 인한 드래그앤 드랍 명령 변경
	if( cell.row >= m_pGridCtrl->Get5HogaSumRow() )
	{
		if (m_pGridCtrl->GetBtmCancelArea() == TRUE)
		{
			if ( cell.col != HOGA_COL )
				return DROPEFFECT_NONE;
		}
		else
		{
			if (cell.col != LEFT_CAN_COL && cell.col != RIGHT_CAN_COL)
				return DROPEFFECT_NONE;
		}
	}
	else if( cell.col != cellLast.col )
	{
		// 취소 스태틱
		if( cell.col == LEFT_CAN_COL || cell.col == RIGHT_CAN_COL )
		{
			if (cell.row == TENHOGA_ROW_FIRST)		// JSJ_Modify
				return DROPEFFECT_NONE;
		}
		else
			return DROPEFFECT_NONE;
	}

	
	// 매수, 매도 칼럼인 경우에만 Drag Over
	if( cell.col == MAEDO_COL || cell.col == MAESU_COL || 
		cell.col == MAEDO_RESERVE_COL || cell.col == MAESU_RESERVE_COL|| cellLast.col == HOGA_COL )
	{
		CString strText = m_pGridCtrl->GetItemText(cell.row, HOGA_COL);
		strText.TrimLeft();
		strText.TrimRight();
		if( strText == "" )
			return DROPEFFECT_NONE;
		else			// Drag Over 상태인 경우에도 열선택 표시
		{
			if (cell.col == MAEDO_COL)
				m_pGridCtrl->SetSelectedRange(cell.row, MAEDO_COL, cell.row, HOGA_COL, FALSE);
			else
				m_pGridCtrl->SetSelectedRange(cell.row, HOGA_COL, cell.row, MAESU_COL, FALSE);

			// ** 주문예약 칼럼에 값이 있으면 있으면 "DROPEFFECT_NONE"
			if (cell.col == MAEDO_RESERVE_COL || cell.col == MAESU_RESERVE_COL)
			{
				strText = m_pGridCtrl->GetItemText(cell.row, cell.col);
				strText.TrimLeft();			strText.TrimRight();
				if (strText != "")
					return DROPEFFECT_NONE;
			}
		}
	}
	else	// 열선택 해제
	{
		m_pGridCtrl->ReleaseAllSelectedCell();
	}
		
	//TRACE("[After] cell.row = %d, cell.col = %d    cellLast.row = %d, cellLast.col = %d\n", 
	//	cell.row, cell.col, cellLast.row, cellLast.col);
	
	return m_pGridCtrl->OnDragOver(pDataObject, dwKeyState, point);
}

BOOL CGridDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,  DROPEFFECT dropEffect, CPoint point)
{
//    TRACE(_T("In CGridDropTarget::OnDrop\n"));
    ASSERT(m_pGridCtrl);

	// JSJ_Modify_040520 COMMENT: 그리드내 Drop 수용범위
	CCellID cell, cellLast;
	cell = m_pGridCtrl->GetCellFromPt(point);
	cellLast = m_pGridCtrl->GetLeftClickDownCell();
//	TRACE("row = %d, col = %d\n", cell.row, cell.col);


	// 	JSJ_Add_0405610 --------------------------------------------------->>
	// 최소 타이틀 다이아로그 드랍시에 소멸
	m_pGridCtrl->SendMessageToCancelDlg(cell.row, cell.col, UDM_DESTROY_CANCEL_DLG);
	// <<--------------------------------------------------------------------


	// JSJ_ITCastle_Modify
	// 드래그 시작점이 매수주문 이거나, 매도주문 행이 아니면 리턴
	if( !(cellLast.col == MAEDO_COL || cellLast.col == MAESU_COL || 
		  cellLast.col == MAEDO_RESERVE_COL || cellLast.col == MAESU_RESERVE_COL || cellLast.col == HOGA_COL) )
	{
		return DROPEFFECT_NONE;
	}
	
	// 취소 행의 추가로 인한 드래그앤 드랍 명령 변경
	if( cell.row >= m_pGridCtrl->Get5HogaSumRow() )
	{
		if (m_pGridCtrl->GetBtmCancelArea() == TRUE)
		{
			if ( cell.col != HOGA_COL )
				return DROPEFFECT_NONE;
		}
		else
		{
			if (cell.col != LEFT_CAN_COL && cell.col != RIGHT_CAN_COL)
				return DROPEFFECT_NONE;
		}
	}
	else if( cell.col != cellLast.col )				
	{													  
		// 취소 스태틱
		if( cell.col == LEFT_CAN_COL || cell.col == RIGHT_CAN_COL )
		{
			if (cell.row == TENHOGA_ROW_FIRST)		// JSJ_Modify
				return DROPEFFECT_NONE;
		}
		else 
			return DROPEFFECT_NONE;
	}	

	if( cell.col == MAEDO_COL || cell.col == MAESU_COL || 
		cellLast.col == MAEDO_RESERVE_COL || cell.col == MAESU_RESERVE_COL || cellLast.col == HOGA_COL)
	{
		CString strText = m_pGridCtrl->GetItemText(cell.row, HOGA_COL);
		strText.TrimLeft();
		strText.TrimRight();
		if( strText == "" )
			return DROPEFFECT_NONE;
	}

    return m_pGridCtrl->OnDrop(pDataObject, dropEffect, point);

}

#endif // GRIDCONTROL_NO_DRAGDROP
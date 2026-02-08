/***********************************************
	Ultimate Grid 97
	Copyright 1994 - 1997 Dundas Software Ltd.

	Class 
		CUGLstBox
	Purpose
		This class is used by the CUGDropList class
		as the listbox that appears when the droplist
		is activated.
	Details
		This class is responsible for returning any
		selection make in the listbox, and destroying
		itself when an item is selected or when the
		selection process is canceled.
************************************************/

#include "stdafx.h"

#include "../UGCtrl.h"
//#include "ugLstBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/***************************************************
****************************************************/
CUGLstBox::CUGLstBox()
{
	m_HasFocus	= FALSE;
	m_col		= NULL;
	m_row		= NULL;
	m_selected	= FALSE;

	m_ctrl		= NULL;
	m_bFirst	= FALSE;
}

/***************************************************
****************************************************/
CUGLstBox::~CUGLstBox()
{
}


/***************************************************
****************************************************/
BEGIN_MESSAGE_MAP(CUGLstBox, CListBox)
	//{{AFX_MSG_MAP(CUGLstBox)
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_GETDLGCODE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CUGLstBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ReleaseCapture();
	SetCapture();
	// TODO: Add your specialized creation code here
	
	return 0;
}


/***************************************************
****************************************************/
void CUGLstBox::OnSetFocus(CWnd* pOldWnd) 
{
	CListBox::OnSetFocus(pOldWnd);

	m_HasFocus = TRUE;
	m_selected = FALSE;
}

/***************************************************
****************************************************/
void CUGLstBox::OnKillFocus(CWnd* pNewWnd) 
{
	ReleaseCapture();

	CListBox::OnKillFocus(pNewWnd);

	DestroyWindow();
	
	m_bFirst = FALSE;
	m_HasFocus = FALSE;
	*m_col = -1;
	*m_row = -1;
//	if(m_selected == FALSE){
//		*m_col = -2;
//		*m_row = -2;
//	}
	m_selected = FALSE;
	m_ctrl->m_CUGGrid->SendMessage(WM_PAINT,0,0);
}

/***************************************************
****************************************************/
int CUGLstBox::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	return MA_ACTIVATE;	
}

/***************************************************
****************************************************/
void CUGLstBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
///	if(m_bFirst)
//		ReleaseCapture();

	CRect rect;
	GetClientRect(rect);
	if(m_bFirst && !rect.PtInRect(point))	
	{
		m_bFirst = FALSE;
		m_ctrl->m_CUGGrid->SetFocus();
		m_ctrl->Invalidate();
	}

	CListBox::OnLButtonDown(nFlags, point) ;
}

void CUGLstBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//update the string in the current cell if the list box had a selected item
	if(m_bFirst)
		ReleaseCapture();
	
	if(GetCurSel( )  != LB_ERR)
	{		
		Select();		
	}

	m_bFirst = TRUE;
}

/***************************************************
****************************************************/
void CUGLstBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	if(nChar == VK_ESCAPE)
		DestroyWindow();

	if(nChar == VK_RETURN){
		if(GetCurSel( )  != LB_ERR){
			Select();
		}
		else{
			m_HasFocus = FALSE;
			DestroyWindow();
		}
	}
	
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

/***************************************************
****************************************************/
void CUGLstBox::Select(){

	CUGCell cell;
	CString string;
	int col = *m_col; //m_ctrl->GetCurrentCol();
	long row = *m_row; //m_ctrl->GetCurrentRow();

	m_selected = TRUE;

	//get the string selected
	GetText(GetCurSel(),string);

	//notify the user of the selection
	if(m_ctrl->OnCellTypeNotify(m_cellTypeId,col,row, //set the id
		UGCT_DROPLISTSELECT,(long)&string) != FALSE){

		m_ctrl->GetCell(col,row,&cell);
		cell.SetText(string);
		m_ctrl->SetCell(col,row,&cell);

		m_ctrl->RedrawCell(col,row);
	}
	
	m_HasFocus = FALSE;
	if(m_col != NULL)
		*m_col = -1;
	if(m_row != NULL)
		*m_row = -1;

	m_ctrl->m_CUGGrid->SetFocus();
}

UINT CUGLstBox::OnGetDlgCode() 
{
	return CListBox::OnGetDlgCode() | DLGC_WANTALLKEYS | DLGC_WANTARROWS;
}

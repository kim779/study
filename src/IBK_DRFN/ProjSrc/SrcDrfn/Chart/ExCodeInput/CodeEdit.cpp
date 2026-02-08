// CodeEdit.cpp : implementation file
//

#include "stdafx.h"
//#include "excodeinput.h"
#include "CodeEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodeEdit

CCodeEdit::CCodeEdit()
{
	m_hItemCode = NULL;
}

CCodeEdit::~CCodeEdit()
{
}


BEGIN_MESSAGE_MAP(CCodeEdit, CEdit)
	//{{AFX_MSG_MAP(CCodeEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeEdit message handlers

void	CCodeEdit::SetDataText(LPCTSTR szText)
{
}

void	CCodeEdit::SetDataText2(LPCTSTR szCode)
{
}

CString	CCodeEdit::GetDataText()
{
	return "";
}

void	CCodeEdit::SetEnabled(BOOL propVal)
{
	//::EnableWindow(m_hItemCode, propVal);
	EnableWindow(propVal);
}

BOOL	CCodeEdit::GetEnabled()
{
	return ::IsWindowEnabled(m_hItemCode);
}

long	CCodeEdit::GetBase(LPCTSTR lpszKey)
{
	return 1L;
}

void	CCodeEdit::SetFocus()
{
	//@solomon::SetFocus(m_hItemCode);
}

void	CCodeEdit::SetCurSel(short nIndex)
{
}

OLE_HANDLE	CCodeEdit::GetHWnd()
{
	return NULL;
}

HWND	CCodeEdit::GetSafeHwnd()
{
	return m_hItemCode;
}

void	CCodeEdit::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
//@신영삭제	m_pControlWnd->MoveWindow(x, y, nWidth, nHeight, bRepaint);
	//@solomon::MoveWindow(m_hItemCode, x, y, nWidth, nHeight, bRepaint);
}

void	CCodeEdit::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
//@신영삭제	
//	if(m_pControlWnd)
//		m_pControlWnd->MoveWindow(lpRect, bRepaint);
	//@solomon::MoveWindow(m_hItemCode, lpRect->left, lpRect->top, (lpRect->right-lpRect->left), (lpRect->bottom-lpRect->top), bRepaint);
}

BOOL	CCodeEdit::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
{
	return FALSE;	//@solomon
	HWND hWndInsertAfter = NULL;
	if(pWndInsertAfter)
		hWndInsertAfter = pWndInsertAfter->GetSafeHwnd();

	if(cx != 0)
		return ::SetWindowPos(m_hItemCode, hWndInsertAfter, x, y, cx, cy, nFlags);
	else 
		return FALSE;
}

BOOL	CCodeEdit::ShowWindow(int nCmdShow)
{
	return FALSE;	//@solomon
	return ::ShowWindow(m_hItemCode, nCmdShow);
}

void	CCodeEdit::GetWindowRect(LPRECT lpRect)
{
	return ;	//@solomon
	::GetWindowRect(m_hItemCode, lpRect);
}

void	CCodeEdit::GetClientRect(LPRECT lpRect)
{
	return ;	//@solomon
	::GetClientRect(m_hItemCode, lpRect);
}

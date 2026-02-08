// SfnCodeInput.cpp: implementation of the CSfnCodeInput class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SfnCodeInput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSfnCodeInput::CSfnCodeInput()
{
//@신영제거	
//	m_cmbJCodeDisp = NULL;
//	m_pControlWnd = NULL;
// 	m_pCtlItemCode = NULL;
// 	m_pItemCode = NULL;
	m_hItemCode = NULL;
}

CSfnCodeInput::~CSfnCodeInput()
{
}

void	CSfnCodeInput::SetDataText(LPCTSTR szText)
{
//	m_pItemCode->SetCaption(szText);
//	HWND hWnd = m_pCtlItemCode->GetHwnd();
//	if(::IsWindow(hWnd))
//		m_pItemCode->EditFullCaption(szText);
}

void	CSfnCodeInput::SetDataText2(LPCTSTR szCode)
{
//	m_pItemCode->SetCaption(szCode);
}

CString	CSfnCodeInput::GetDataText()
{
//	return m_pItemCode->GetCaption();
	return "";
}

void	CSfnCodeInput::SetEnabled(BOOL propVal)
{
	::EnableWindow(m_hItemCode, propVal);
}

BOOL	CSfnCodeInput::GetEnabled()
{
	return ::IsWindowEnabled(m_hItemCode);
}

long	CSfnCodeInput::GetBase(LPCTSTR lpszKey)
{
	return 1L;
}

void	CSfnCodeInput::SetFocus()
{
	::SetFocus(m_hItemCode);
}

void	CSfnCodeInput::SetCurSel(short nIndex)
{
}

OLE_HANDLE	CSfnCodeInput::GetHWnd()
{
	return NULL;
}

HWND	CSfnCodeInput::GetSafeHwnd()
{
	return m_hItemCode;
}

void	CSfnCodeInput::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
//@신영삭제	m_pControlWnd->MoveWindow(x, y, nWidth, nHeight, bRepaint);
	::MoveWindow(m_hItemCode, x, y, nWidth, nHeight, bRepaint);
}

void	CSfnCodeInput::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
//@신영삭제	
//	if(m_pControlWnd)
//		m_pControlWnd->MoveWindow(lpRect, bRepaint);
	::MoveWindow(m_hItemCode, lpRect->left, lpRect->top, (lpRect->right-lpRect->left), (lpRect->bottom-lpRect->top), bRepaint);
}

BOOL	CSfnCodeInput::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
{
//@신영삭제	
//	if(cx != 0)
//		return m_pControlWnd->SetWindowPos(pWndInsertAfter, x, y, cx, cy, nFlags);
//	else 
//		return FALSE;
	HWND hWndInsertAfter = NULL;
	if(pWndInsertAfter)
		hWndInsertAfter = pWndInsertAfter->GetSafeHwnd();

	if(cx != 0)
		return ::SetWindowPos(m_hItemCode, hWndInsertAfter, x, y, cx, cy, nFlags);
	else 
		return FALSE;
}

BOOL	CSfnCodeInput::ShowWindow(int nCmdShow)
{
//@신영삭제	return m_pControlWnd->ShowWindow(nCmdShow);
	return ::ShowWindow(m_hItemCode, nCmdShow);
}

void	CSfnCodeInput::GetWindowRect(LPRECT lpRect)
{
//@신영삭제	m_pControlWnd->GetWindowRect(lpRect);
	::GetWindowRect(m_hItemCode, lpRect);
}

void	CSfnCodeInput::GetClientRect(LPRECT lpRect)
{
//@신영삭제	m_pControlWnd->GetClientRect(lpRect);
	::GetClientRect(m_hItemCode, lpRect);
}

//void	CSfnCodeInput::SetMember(ICtlBase* pCtlItemCode, CWrpItemCode* pItemCode)
//{
// 		m_pCtlItemCode = pCtlItemCode;
// 		m_pItemCode = pItemCode;
// 		m_hItemCode = m_pCtlItemCode->GetHwnd();
// 	//	m_pControlWnd = pWnd;
// 	//	m_cmbJCodeDisp = pComboCtl;
//}
// ExCodeInput.h : main header file for the EXCODEINPUT DLL
//

#if !defined(AFX_EXCODEINPUT_H__815B455B_CCE6_4427_AFCA_A28FF8F07531__INCLUDED_)
#define AFX_EXCODEINPUT_H__815B455B_CCE6_4427_AFCA_A28FF8F07531__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
//#include "Sfncodeinput.h"
#include "CodeEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CExCodeInputApp
// See ExCodeInput.cpp for the implementation of this class
//
class CStsCodeInput;
class CExCodeInputApp : public CWinApp
{
public:
	CExCodeInputApp();

	LPVOID	CreateCodeInput(LPVOID lpPointer, CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, UINT nProperties);
	LPVOID	CreateCodeInput(LPVOID lpPointer, CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, CStringList* pListProperty);
	LPVOID	CreateCodeInput(LPVOID lpPointer, CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR* lpszProperties, int nSize);
	LPVOID	CreateCodeInput(LPVOID lpPointer, CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR lpszProperties);
	void	ExitCodeInput(LPVOID lpPointer);

	short	GetGubunFromCode(LPCTSTR szCode);

	void	SetDataText(LPCTSTR szText);
	void	SetDataText2(LPCTSTR szCode);
	CString	GetDataText();

	void	SetEnabled(BOOL propVal);
	BOOL	GetEnabled();
	
	long	GetBase(LPCTSTR lpszKey);
	
	void	SetFocus();
	
	void	SetCurSel(short nIndex);

	OLE_HANDLE	GetHWnd();
	HWND	GetSafeHwnd();
	
	void	MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint);
	void	MoveWindow(LPCRECT lpRect, BOOL bRepaint);
	BOOL	SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags);
	BOOL	ShowWindow(int nCmdShow);
	void	GetWindowRect(LPRECT lpRect);
	void	GetClientRect(LPRECT lpRect);

	//CSfnCodeInput*	m_pCurMember;
	CCodeEdit*	m_pCurMember;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExCodeInputApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExCodeInputApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCODEINPUT_H__815B455B_CCE6_4427_AFCA_A28FF8F07531__INCLUDED_)

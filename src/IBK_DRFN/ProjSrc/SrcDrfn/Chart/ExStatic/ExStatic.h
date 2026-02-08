// ExStatic.h : main header file for the EXSTATIC DLL
//

#if !defined(AFX_EXSTATIC_H__A072548B_84DA_48AC_9DC7_B43FAF9BF72B__INCLUDED_)
#define AFX_EXSTATIC_H__A072548B_84DA_48AC_9DC7_B43FAF9BF72B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CExStaticApp
// See ExStatic.cpp for the implementation of this class
//

interface ISkinColorManager;
class CMember;
class CExStaticApp : public CWinApp
{
public:
	CExStaticApp();

public:
	CMember*	m_pCurMember;
	ISkinColorManager*	m_pISkinColorManager;

	long	ActionControl(LPVOID lpAction);
	void	ExitStatic(LPVOID lpPointer);

	LPVOID	InitDrawStatic(LPVOID lpPointer, BOOL bUseDefSkin = FALSE);
	BOOL	SetStaticColor(COLORREF clrOut, COLORREF clrBk, COLORREF clrText);
	BOOL	SetStaticString(LPCTSTR lpszOutLine, LPCTSTR lpszBackground, LPCTSTR lpszTextClr);
	BOOL	SetStaticIndex(UINT nOutLine, UINT nBackground, UINT nText);
	void	OnPaint(CDC* pDC);
	BOOL	SetIcon(CSize size, HICON hIcon, UINT nIconAlign);
	void	SetText(LPCTSTR lpszText);

	FpChangeSkinNotify	GetSkinNotifyFunction();
	void	PreSubclassWindow_Statc();
	LRESULT		WindowProc_Static(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL OnEraseBkgnd(CDC* pDC);
	void GetWindowText(CString& rString);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExStaticApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExStaticApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void	OffsetPoint(CPoint& point, CSize size, CRect rcClient);
	void	DrawStaticIcon(CDC* pDC, CRect& rcItem);
	void	DrawStaticText(CDC* pDC, CRect& rcItem);
	CPoint	CalculateImagePosition(CDC* pDC, CRect& rcItem);
};


/////////////////////////////////////////////////////////////////////////////

extern CExStaticApp theApp;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXSTATIC_H__A072548B_84DA_48AC_9DC7_B43FAF9BF72B__INCLUDED_)

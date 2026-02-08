// ExEdit.h : main header file for the EXEDIT DLL
//

#if !defined(AFX_EXEDIT_H__E862D20C_E7FA_4850_9B83_ABE6507B3B33__INCLUDED_)
#define AFX_EXEDIT_H__E862D20C_E7FA_4850_9B83_ABE6507B3B33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CExEditApp
// See ExEdit.cpp for the implementation of this class
//

interface ISkinColorManager;
interface IMainInfoManager;
class CMember;
class CExEditApp : public CWinApp
{
public:
	CExEditApp();

	CMember*	m_pCurMember;

	long	ActionControl(LPVOID lpAction);
	LPVOID	InitEdit(LPVOID lpPointer, BOOL bUseDefSkin = FALSE);
	void	ExitEdit(LPVOID lpPointer);
	void	OnPaint(LPVOID lpPointer);
	void	UseNumericType(BOOL bUse);
	void	OnChar_Edit(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	PutComma();

	//////////////////////////////////////////////////////////////////////////
	// ExEditMask
	LPVOID	InitEditMask(LPVOID lpPointer, BOOL bUseDefSkin = FALSE);
	void	ExitEditMask(LPVOID lpPointer);
	void	OnPaintMaskEdit();

	BOOL	SetEditMask(LPCTSTR lpszMask, LPCTSTR lpszLiteral, LPCTSTR lpszDefault = NULL);
	void	SetPromptChar(TCHAR ch);
	BOOL	CanOverType();
	BOOL	CanUseMask();
	BOOL	CheckChar(UINT& nChar, BOOL bBeep = TRUE);
	BOOL	CorrectPosition(int& nPos, BOOL bForward = TRUE, BOOL bBeep = TRUE);
	CString GetMaskedText(int nPos = 0);
	TCHAR	GetPromptChar();
	void	InsertCharAt(int nPos, TCHAR nChar);
	BOOL	IsModified();
	void	MaskClear();
	void	MaskCopy();
	void	MaskCut();
	void	MaskPaste();
	void	MaskSelectAll();
	void	MaskUndo();
	void	OnChar_ExMaskEdit(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnLButtonDown_ExMaskEdit(UINT nFlags, CPoint point);
	BOOL	OnCommand_ExMaskEdit(WPARAM wParam, LPARAM lParam);
	void	OnKeyDown_ExMaskEdit(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnSetFocus_ExMaskEdit(CWnd* pOldWnd);
	void	OnLButtonDblClk_ExMaskEdit(UINT nFlags, CPoint point);
	void	OnUpdateEditUndo_ExMaskEdit(CCmdUI* pCmdUI);
	
	void	SetUseMask(BOOL bUseMask);
	void	SetOverType(BOOL bOverType);
	BOOL	PosInRange(int nPos);
	
	BOOL	ProcessMask(UINT& nChar, int nEndPos);

	BOOL	PreTranslateMessage(MSG* pMsg);

	void	DeleteCharAt(int nPos);
	BOOL	CopyToClipboard(LPCTSTR lpszText);
	BOOL	SelectionMade();

	ISkinColorManager*	m_pISkinColorManager;
	IMainInfoManager*	m_pIMainInfoManager;
	//////////////////////////////////////////////////////////////////////////
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExEditApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExEditApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL InitInstance();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXEDIT_H__E862D20C_E7FA_4850_9B83_ABE6507B3B33__INCLUDED_)

/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Juraj Rojko jrojko@twist.cz
// All rights reserved
//
#if !defined(AFX_TWSCRIPTEDIT_H__82F5E419_1C74_11D1_87FA_00403395B157__INCLUDED_)
#define AFX_TWSCRIPTEDIT_H__82F5E419_1C74_11D1_87FA_00403395B157__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TWScriptEdit.h : header file
//
#include "../awCommon/FormItem.h"
#include "../AutoCmplDlg.h"
#include "../AutoToolTip.h"

/////////////////////////////////////////////////////////////////////////////
// CTWScriptEdit window

class CUndoAction
{
public:
	typedef enum {eKeyPaste, ePaste, eCut} eAction;

	CString		sAddText;
	CString		sRemoveText;
	CHARRANGE	beforeRange;
	CHARRANGE	afterRange;
	eAction		nType;

	CUndoAction()
	{
		sAddText.Empty();
		sRemoveText.Empty();
		beforeRange.cpMin = afterRange.cpMin = 0;
		beforeRange.cpMax = afterRange.cpMax = 0;
		nType	          = eKeyPaste;
	}

	CUndoAction& operator = (const CUndoAction& src)
	{
		sAddText	= src.sAddText;
		sRemoveText	= src.sRemoveText;
		beforeRange	= src.beforeRange;
		afterRange	= src.afterRange;
		nType		= src.nType;
		return *this;
	}
};

class CTWScriptEdit : public CRichEditCtrl
{
// Construction
public:
	CTWScriptEdit();

	bool	m_bShowList, m_bShowTip;
	BOOL	m_bEmpty;
	CMap	<int, int, CString, CString>	m_AutoListMap;
	CMapStringToPtr	m_publicStrMap, m_ScriptStrMap;

	CPoint	m_mousePnt;
	CString	m_sRtn, m_sControlName;
	CString	m_sBeforeText;
	CString	m_sAfterText;
	bool	m_bUndoLock;

	// 커스터마이징을 위해서 추가 
	CHARRANGE	m_scBeforeRange;

	long	m_nMaxFmRange;
	//////////////////////////////

// Attributes
public:
	void	Initialize();
	void	SetCaseSensitive(BOOL bSensitive);
	void	SetChangeCase(BOOL bChange);

	void	SetSLComment(TCHAR chComment, TCHAR chComment2 = 0);
	void	SetSLComment(LPCTSTR lpszComment);
	void	SetStringQuotes(LPCTSTR lpszStrQ);

	void	AddKeywords(LPCTSTR lpszKwd);
	void	AddConstants(LPCTSTR lpszKwd);
	void	ClearKeywords();
	void	ClearConstants();

	void	SetKeywordColor(COLORREF clr, BOOL bBold);
	void	SetConstantColor(COLORREF clr, BOOL bBold);
	void	SetCommentColor(COLORREF clr, BOOL bBold);
	void	SetNumberColor(COLORREF clr, BOOL bBold);
	void	SetStringColor(COLORREF clr, BOOL bBold);

// Operations
public:
	void FormatAll();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTWScriptEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void NextFormat();
	void ProcessEmpty();
	void * SetRedoArray(void *pRedoArr);
	void RedoAction();
	void * SetUndoArray(void *pUndoArr);
	void ReplaceSel( LPCTSTR lpszNewText, BOOL bCanUndo = FALSE);
	void UndoAction();
	CString GetBaseControlInfo(int kind);
	CString GetTypeInfoString(ITypeInfo *pTypeInfo, TYPEATTR *pTypeAttr);
	CString GetVtType(VARTYPE var);
	int	GetTabCount(CString sText);
	void	hidePopup();
	void	showTip(CString sKey);
	void	ReplaceFTab();
	CString	GetControlInfo(CString str);
	void	ReplaceTab();
	CString	ParseWord(CString str, int &pos, CString sub);
	bool	ReplaceComment();
	CString	Parser(CString &srcstr, CString substr);
	void	ShowMenu();
	void	showList(int kind);
	int	IsSymbol(CString &sym);
	void	Initialize(_mapH *mapH);
	virtual ~CTWScriptEdit();

	// Generated message map functions
protected:
	BOOL	IsStringQuote(TCHAR ch);
	int	IsKeyword(LPCTSTR lpszSymbol);
	int	IsConstant(LPCTSTR lpszSymbol);

	void	SetFormatRange(int nStart, int nEnd, BOOL bBold, COLORREF clr);
	void	FormatTextRange(int nStart, int nEnd);
	void	FormatTextLines(int nStart, int nEnd);
	void	ChangeCase(int nStart, int nEnd, LPCTSTR lpszStr);

	struct SymbolColor {
		COLORREF clrColor;
		BOOL bBold;
	};

	enum ChangeType {ctUndo, ctUnknown, ctReplSel, ctDelete, ctBack, ctCut, ctPaste, ctMove, ctKeyDown};

	BOOL m_bCaseSensitive;
	BOOL m_bChangeCase;
	TCHAR m_chComment;
	TCHAR m_chComment2;
	CString m_strComment;
	CString m_strStringQuotes;
	CString m_strKeywords;
	CString m_strKeywordsLower;
	CString m_strConstants;
	CString	m_strConstantsLower;

	SymbolColor m_icComment;
	SymbolColor m_icNumber;
	SymbolColor m_icString;
	SymbolColor m_icKeyword;
	SymbolColor m_icConstant;

	BOOL	m_bInForcedChange;
	ChangeType m_changeType;
	CHARRANGE m_crOldSel;
	int	m_curPos;

	int	m_nEndLineLen;

	bool		m_bActivate;

	CAutoCmplDlg	*m_pAutoDlg;
	CAutoToolTip	*m_pAutoTip;

	struct _mapH *m_mapH;
private:
	CArray <CHARRANGE, CHARRANGE> m_rangeArr;
	CArray	<CUndoAction, CUndoAction>	*m_pUndoArr;
	CArray	<CUndoAction, CUndoAction>	*m_pRedoArr;
protected:

	//{{AFX_MSG(CTWScriptEdit)
	afx_msg void OnChange();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillfocus();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnVscroll();
	//}}AFX_MSG
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg void OnProtected(NMHDR*, LRESULT* pResult);
	afx_msg void OnSelChange(NMHDR*, LRESULT* pResult);
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TWSCRIPTEDIT_H__82F5E419_1C74_11D1_87FA_00403395B157__INCLUDED_)

// ExMaskEdit.h: interface for the CExMaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXMASKEDIT_H__5CBB0784_4C64_4D30_AEAD_AF031A4F5FD4__INCLUDED_)
#define AFX_EXMASKEDIT_H__5CBB0784_4C64_4D30_AEAD_AF031A4F5FD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ExEdit.h"

class CLoaderMaskEdit;
class CExMaskEdit : public CEdit
{
	DECLARE_DYNAMIC(CExMaskEdit)
	
public:
	CExMaskEdit(BOOL bUseDefSkin = FALSE);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: 메모리 포인터나 long형 값
	// Paramter
	//		LPVOID lpAction : 메모리 포인터 (구조체, 클래스등등..)
	// Comments		: 정의된 함수이외에 작업을 해야 하는 경우 ActionControl 함수를
	//				  사용해 클래스 수정없이 프로젝트 진행 가능.
	//				  파라메터와 리턴값은 각 화면에서의 요구사항데로 정의 가능함
	//				  ActionControl을 사용해 추가된 사항은 추후 클래스 수정으로 함수가 추가되지만
	//				  원활한 프로젝트 진행을 위해 임시로 이 함수를 사용합니다.
	//-----------------------------------------------------------------------------	
	long		ActionControl(LPVOID lpAction);

	BOOL	CanOverType();
	BOOL	CanUseMask();
	BOOL	IsModified();
	BOOL	PosInRange(int nPos);
	TCHAR	GetPromptChar();
	void	MaskClear();
	void	MaskCopy();
	void	MaskCut();
	void	MaskPaste();
	void	MaskSelectAll();
	void	MaskUndo();
	void	SetOverType(BOOL bOverType);
	void	SetPromptChar(TCHAR ch);
	void	SetUseMask(BOOL bUseMask);
	
	virtual void SetEditMask(LPCTSTR lpszMask, LPCTSTR lpszLiteral, LPCTSTR lpszDefault = NULL);
	virtual BOOL CheckChar(UINT& nChar, BOOL bBeep = TRUE);
	virtual BOOL ProcessMask(UINT& nChar, int nEndPos);

protected:
	CLoaderMaskEdit*		m_pLoader;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExAcctEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	virtual BOOL CorrectPosition(int& nPos, BOOL bForward = TRUE, BOOL bBeep = TRUE);
	virtual void DeleteCharAt(int nPos);
	virtual void InsertCharAt(int nPos, TCHAR nChar);
	virtual BOOL CopyToClipboard(LPCTSTR lpszText);
	virtual CString GetMaskedText(int nPos = 0);
	virtual BOOL SelectionMade();

// Implementation
public:
	virtual ~CExMaskEdit();
	
protected:
	//{{AFX_MSG(CExAcctEdit)
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_EXMASKEDIT_H__5CBB0784_4C64_4D30_AEAD_AF031A4F5FD4__INCLUDED_)

#pragma once

#include "cedefs.h"
#include "editcmd.h"

class CCrystalEditView;

// CEditReplaceDlg 대화 상자입니다.

class CEditReplaceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditReplaceDlg)

public:
	CEditReplaceDlg(CCrystalEditView* pBuddy);   // 표준 생성자입니다.
	virtual ~CEditReplaceDlg();

	BOOL	m_enableScopeSelection;
	CPoint	m_ptCurrentPos;
	CPoint	m_ptBlockBegin;
	CPoint	m_ptBlockEnd;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EDIT_REPLACE };
	BOOL	m_matchCase;
	BOOL	m_wholeWord;
	CString	m_sText;
	CString	m_sNewText;
	int	m_scope;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CCrystalEditView *m_pBuddy;
	BOOL	m_found;
	CPoint	m_ptFoundAt;

	BOOL	doHighlightText();

public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnChangeEditText();
	afx_msg void OnEditReplace();
	afx_msg void OnEditReplaceAll();
	afx_msg void OnEditSkip();
};

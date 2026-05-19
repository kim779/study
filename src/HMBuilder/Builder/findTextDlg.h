#pragma once

#include "cedefs.h"
#include "editcmd.h"

class CCrystalTextView;

// CFindTextDlg 대화 상자입니다.

class CFindTextDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFindTextDlg)

public:
	CFindTextDlg(CCrystalTextView* pBuddy);   // 표준 생성자입니다.
	virtual ~CFindTextDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EDIT_FIND };
	int	m_direction;
	BOOL	m_matchCase;
	CString	m_sText;
	BOOL	m_wholeWord;

	CPoint	m_currentPt;


private:
	CCrystalTextView *m_pBuddy;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChangeEditText();

};

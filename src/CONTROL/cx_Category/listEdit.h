#pragma once

#include "ExListCtrl.h"

class CListEdit : public CEdit
{
public:
	CListEdit();
	CListEdit(CExListCtrl* pList, int iItem, int iSubItem, CString sInitText);
	virtual ~CListEdit();

	//{{AFX_VIRTUAL(CListEdit)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CListEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void		SetListText();

private:
	CExListCtrl*	m_pListCtrl;
	int		m_Item;
	int		m_SubItem;
	CString		m_InitText;
	BOOL		m_bEscape;
};
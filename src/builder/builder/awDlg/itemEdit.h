#pragma once

// CItemEdit

class CItemEdit : public CEdit
{
	DECLARE_DYNAMIC(CItemEdit)

public:
	CItemEdit(int item, int subItem, CString& content);
	virtual ~CItemEdit();

protected:
	int	m_item;
	int	m_subItem;
	CString m_content;
	BOOL	m_escape;

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNcDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};



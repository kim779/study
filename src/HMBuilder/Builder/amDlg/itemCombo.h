#pragma once


// CItemCombo

class CItemCombo : public CComboBox
{
	DECLARE_DYNAMIC(CItemCombo)

public:
	CItemCombo(int item, int subItem, CStringList* psList = NULL);
	virtual ~CItemCombo();

protected:
	CStringList m_strList;
	int	m_item;
	int	m_subItem;
	BOOL	m_escape;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnCbnCloseup();
	afx_msg void OnCbnDropdown();
};



#pragma once


// CCheckComboBox

class CCheckComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CCheckComboBox)

public:
	CCheckComboBox();
	virtual ~CCheckComboBox();

	BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	int	SetCheck(int nIndex, BOOL bFlag);
	BOOL	GetCheck(int nIndex);
	void	SelectAll(BOOL bCheck = TRUE);

protected:
	HWND	m_listBox;
	CString	m_strText;
	BOOL	m_bTextUpdated;
	BOOL	m_bItemHeightSet;

	void recalcText();
	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMS);
	afx_msg LRESULT OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDropDown();

};



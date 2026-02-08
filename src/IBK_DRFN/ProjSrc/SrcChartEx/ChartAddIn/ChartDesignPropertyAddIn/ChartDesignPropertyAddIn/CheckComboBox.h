#if !defined(AFX_CHECKCOMBOBOX_H__66750D93_95DB_11D3_9325_444553540000__INCLUDED_)
#define AFX_CHECKCOMBOBOX_H__66750D93_95DB_11D3_9325_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCheckComboBox : public CComboBox
{
public:
	//현재 선택된 항목이 없을땐 false를 넘겨주고, 선택된 항목이 있을때는 true를 넘김. 
	bool GetCurState();
	CCheckComboBox();
	virtual ~CCheckComboBox();

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	// Selects all/unselects the specified item
	INT SetCheck(INT nIndex, BOOL bFlag);
	// Returns checked state
	BOOL GetCheck(INT nIndex);
	// Selects all/unselects all
	void SelectAll(BOOL bCheck = TRUE);

	// The subclassed COMBOLBOX window (notice the 'L')
	HWND m_hListBox;

public:
	void SetItemCheck(const CString& strAllItmes);

protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckComboBox)
	protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCheckComboBox)
	afx_msg LRESULT OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDropDown();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	// Routine to update the text
	void RecalcText();
	// The string containing the text to display
	CString m_strText;
	BOOL m_bTextUpdated;

	// A flag used in MeasureItem, see comments there
	BOOL m_bItemHeightSet;

private:
	//Check된 항목이 몇개인지..
	int m_nCheckedCount;

private:
	void ChangeItemText();
	void AddItemText(const CString& strItemName);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKCOMBOBOX_H__66750D93_95DB_11D3_9325_444553540000__INCLUDED_)

#pragma once


// CEditListCtrl

class CEditListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEditListCtrl)

public:
	CEditListCtrl();
	virtual ~CEditListCtrl();

	CStringList m_strList;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void	SetColumnType(CString func) { m_type = func;}

protected:
	CString m_type;

	char	getType(int col);
	CComboBox* makeComboItem( int nItem,  int nSubItem);
	CEdit* makeEditItem( int nItem, int nSubItem);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};



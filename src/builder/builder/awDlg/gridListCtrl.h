#pragma once


// CGridListCtrl

class CGridListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CGridListCtrl)

public:
	CGridListCtrl();
	virtual ~CGridListCtrl();

	int	m_focusCol;
	int	m_focusRow;
	bool	m_focus;
	int	m_skipCol;

public:
	void	AddItem(int item, int subItem, CString str);
	void	UpdateFocusCell(int nCol);
	BOOL	GetCellRect(int nRow, int nCol, CRect& rect);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
};



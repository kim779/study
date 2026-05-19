#pragma once


// CSearchListCtrl

class CSearchListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CSearchListCtrl)

public:
	CSearchListCtrl();
	virtual ~CSearchListCtrl();
public:
	void	AddItem(int item, int subItem, CString str);
	void	UpdateFocusCell(int nCol);
	BOOL	GetCellRect(int nRow, int nCol, CRect& rect);

public:
	int	m_FocusCol;
	int	m_FocusRow;
	bool	m_bfocus;
	int	m_skipcol;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
};



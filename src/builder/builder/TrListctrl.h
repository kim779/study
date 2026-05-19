#pragma once


// CTrListctrl

class CTrListctrl : public CListCtrl
{
	DECLARE_DYNAMIC(CTrListctrl)

public:
	CTrListctrl();
	virtual ~CTrListctrl();

public:
	void	AddItem(int item, int subItem, CString str);
	void	UpdateFocusCell(int nCol);
	BOOL	GetCellRect(int nRow, int nCol, CRect& rect);
	void	InitToolTip();

public:
	int	m_FocusCol;
	int	m_FocusRow;
	bool	m_bfocus;
	int	m_skipcol;
	CToolTipCtrl m_ToolTip;
	CStringArray m_ArryMdsc;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};



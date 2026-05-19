#pragma once


// CTTListCtrl

class CTTListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CTTListCtrl)

public:
	CTTListCtrl();
	virtual ~CTTListCtrl();

public:
	CStringArray	m_files;
	CString	m_name;

protected:
	enum	{ tMAP=0, tTMPL, tSYM }	m_kind;

	CMapStringToString m_ttMap; 
	WORD	m_wHitMask;	// A bit mask of LVHT_* flags the control will show tooltips for
	CImageList* m_dragImg;

	BOOL	setItemToolTipText( int nItem, int nSubItem, LPCTSTR lpszToolTipText ); 
	void	deleteAllToolTips();
	CImageList* createDragImage(CPoint& point);
	void	addFiles(int idx, bool reset = false);
	CString	setMapPath(CString maps);

	DECLARE_MESSAGE_MAP()

public:
	BOOL	AddItem(int item, int subItem, CString str, DWORD_PTR data, CString tipText = _T(""));
	BOOL	DeleteAllItems();
	int	GetFiles(CStringArray& arr);
	void	AddFiles(CString maps);

	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	afx_msg BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult); 

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};



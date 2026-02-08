#pragma once

class CListCtrlSort  
{
public:
	enum SortDataType { 
		dtINT, 
		dtSTRING, 
	};

	CListCtrlSort(CListCtrl* pList, const int nCol);
	virtual ~CListCtrlSort();
	void Sort(BOOL bAsc, SortDataType dtType);

protected:

	static int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	class CSortItem
	{
	public:
		CSortItem(const DWORD dw, const CString &txt) : m_txt(txt) 
		{ 
			m_dw = dw; 
		}
		virtual ~CSortItem() {};
		DWORD	m_dw;
		CString m_txt;
	};
	CListCtrl* m_pList{};
	std::vector<std::unique_ptr<CSortItem>> _vsort;
};

//==================================================

class CExListCtrl : public CListCtrl
{
public:
	CExListCtrl();
	virtual ~CExListCtrl();

	//{{AFX_VIRTUAL(CExListCtrl)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExListCtrl)
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void	SetScroll(int index, bool bSel = true);
	void	SetGridLines();
	void	SetFullRowSelect();
	void	ListSort();
	void	ListSort(int col);

private:
	int		m_nSortItem;
	BOOL		m_bSortAsc;
	bool		m_bSort;


};


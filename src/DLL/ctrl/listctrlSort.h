#ifndef	_H_LISTCTRLSORT
#define	_H_LISTCTRLSORT

class CListCtrlSort  
{
public:
	enum SortDataType { 
		dtINT, 
		dtSTRING, 
	};

	CListCtrlSort(CListCtrl *pList, const int nCol);
	virtual ~CListCtrlSort();
	void Sort(BOOL bAsc, SortDataType dtType);

protected:
	CListCtrl *m_pList;

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
};

#endif // _H_LISTCTRLSORT

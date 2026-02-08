#if !defined(AFX_CONTROLEX_H__90291C77_D834_4913_81A0_1892F38071F8__INCLUDED_)
#define AFX_CONTROLEX_H__90291C77_D834_4913_81A0_1892F38071F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class INDI_OBJ
{
public:
	INDI_OBJ() { Reset(); }
	~INDI_OBJ() { Reset(); }

	// public interface
	void Reset() 
	{ 
		m_szName = _T("");
		m_nKind = 0;
		m_pGraph = NULL;
		memset(&m_graphdata, 0x00, SZ_GRAPH); 
	}

	void SetData(int nKind, CString szName, struct _graph* pGraph, bool bNew) 
	{ 
		m_nKind = nKind;
		m_szName = szName;
		
		if (bNew)
		{
			memcpy(&m_graphdata, pGraph, SZ_GRAPH); 
			m_pGraph = &m_graphdata; 
		}
		else
			m_pGraph = pGraph;
	}
	
	struct _graph* SetGraph(struct _graph* pGraph, bool bNew)
	{
		if (bNew)
		{
			memcpy(&m_graphdata, pGraph, SZ_GRAPH); 
			m_pGraph = &m_graphdata; 
		}
		else
			m_pGraph = pGraph;
	}

	void	SetName(CString szName) { m_szName = szName; }
	CString	GetName() { return m_szName; }
	void	SetKind(int nKind) { m_nKind = nKind; }
	int	GetKind() { return m_nKind; }
	struct _graph* GetGraph() { return m_pGraph; }

private:
	CString		m_szName;
	int		m_nKind;	
	struct _graph*	m_pGraph;
	struct _graph	m_graphdata;
};

class CAxTreeCtrl : public CTreeCtrl
{
public:
	CAxTreeCtrl();
	virtual ~CAxTreeCtrl();

	//{{AFX_VIRTUAL(CAxTreeCtrl)
	protected:
	virtual void PreSubclassWindow();	
	//}}AFX_VIRTUAL

public:
	HTREEITEM	AddItem(HTREEITEM hParent, CString input, int nData, struct _graph* pGraph = NULL, HTREEITEM hAt = TVI_LAST);
	int		GetLevel(HTREEITEM hItem = NULL);
	INDI_OBJ*	GetIndiObj(HTREEITEM hItem = NULL);
	DWORD		GetItemData(HTREEITEM hItem = NULL);
	BOOL		DeleteItem(HTREEITEM hItem);
	BOOL		SetItemText(LPCTSTR lpszItem, HTREEITEM hItem = NULL);
protected:
	//{{AFX_MSG(CAxTreeCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CIHMaps<CMapPtrToPtr, HTREEITEM, class INDI_OBJ*>	m_mapObject;	
	CFont							m_font;
	CImageList						m_imgList;
};

class CScrTreeCtrl : public CTreeCtrl
{
public:
	CScrTreeCtrl();
	virtual ~CScrTreeCtrl();

	//{{AFX_VIRTUAL(CScrTreeCtrl)
	protected:
	virtual void PreSubclassWindow();	
	//}}AFX_VIRTUAL

public:
	HTREEITEM	AddItem(HTREEITEM hParent, CString input, int nData, struct _graph* pGraph = NULL, HTREEITEM hAt = TVI_LAST);
	HTREEITEM	AddItem(HTREEITEM hParent, INDI_OBJ* pObject, HTREEITEM hAt = TVI_LAST);
	int		GetChildCount(HTREEITEM hItem);
	INDI_OBJ*	GetIndiObj(HTREEITEM hItem = NULL);
	DWORD		GetItemData(HTREEITEM hItem = NULL);
	BOOL		DeleteItem(HTREEITEM hItem);
	BOOL		SetItemText(LPCTSTR lpszItem, HTREEITEM hItem = NULL);
	HTREEITEM	MoveUp(HTREEITEM hItem);
	HTREEITEM	MoveDown(HTREEITEM hItem);
	void		Swap(HTREEITEM	hItemA, HTREEITEM hItemB);
	HTREEITEM	GetLastChild(HTREEITEM hItem);
	HTREEITEM	Move(HTREEITEM	hFrom, HTREEITEM hTo);

protected:
	//{{AFX_MSG(CScrTreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CIHMaps<CMapPtrToPtr, HTREEITEM, class INDI_OBJ*>	m_mapObject;
	CFont	m_font;
};

class CCheckTree : public CTreeCtrl
{
public:
	CCheckTree();
	virtual ~CCheckTree();

	//{{AFX_VIRTUAL(CCheckTree)
	protected:
	virtual void PreSubclassWindow();	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	HTREEITEM	FindParent(CString parent);
	HTREEITEM	AddGroup(CString group, int nGroup, HTREEITEM hAt = TVI_LAST);
	HTREEITEM	AddGroupItem(CString group, CString name, int nKind, HTREEITEM hAt = TVI_LAST);
	HTREEITEM	GetPanelInfo(int nFlag);
	HTREEITEM	GetItemInfo(int nParent, int nFlag);	
	DWORD		GetItemData(HTREEITEM hItem = NULL);
	BOOL		DeleteItem(HTREEITEM hItem);
	BOOL		SetItemText(LPCTSTR lpszItem, HTREEITEM hItem = NULL);

	
protected:
	//{{AFX_MSG(CCheckTree)
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void clearData();	
	
	CTypedPtrMap<CMapWordToPtr, int, HTREEITEM>	m_mapParent;
	CIHMaps<CMapWordToPtr, int, CTypedPtrMap<CMapWordToPtr, int, HTREEITEM>*> m_mapChild;
	CFont		m_font;	
	
};

/********************************************************************/
// CAxHeaderCtrl class
/********************************************************************/
class CAxHeaderCtrl : public CHeaderCtrl
{
public:
	CAxHeaderCtrl();
	virtual ~CAxHeaderCtrl();

public:
	//{{AFX_VIRTUAL(CAxHeaderCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CAxHeaderCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/********************************************************************/
// CItemList class
/********************************************************************/
class CItemList : public CListCtrl
{
public:
	CItemList();
	virtual ~CItemList();
	int	AddItem(CString input, int nValue, int nRow = -1, int nCol = 0);
	void	SetFlags(int nFirst, ...);
	BOOL	IsExist(CString input, int nCol);
	int	InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	int	InsertColumn(int nCol, const LVCOLUMN *pColumn);
	void	SetSelection(int nRow);
	int	GetSelection();
	int	FindRow(CString value, int nCol = 0);
	int	GetColCount() { return m_nColCnt; }
	void	SetMaxItem(int nMax) { m_nMaxItem = nMax; }
	int	GetMaxItem() { return m_nMaxItem; }
	int	GetData(int nItem, int nSubItem);
	void	SetData(int nItem, int nSubItem, int nData);
	int	GetSubItem(CPoint pt);

public:
	//{{AFX_VIRTUAL(CItemList)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);	
	protected:
	virtual void PreSubclassWindow();	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CItemList)
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int		m_nMaxItem;
	int		m_nColCnt;
	CFont		m_font;
	CFont		m_fontH;
	CAxHeaderCtrl	m_header;
	UINT		m_ctrlID;	
};


/********************************************************************/
// CLineListBox class
/********************************************************************/
class CLineListBox : public CListBox
{
public:
	CLineListBox();
	virtual ~CLineListBox();



public:	
	void	SetSelectedColorValue(COLORREF color);
	void	SetSelectedColorName(CString szColor);
	bool	RemoveColor(CString szColor);
	bool	RemoveColor(COLORREF color);
	void	SetColor(COLORREF color, int nIndex = -1);
	void	SetCheck(bool bCheck, int nIndex = -1);
	bool	GetCheck(int nIndex = -1);
	int	AddColor(CString szColor, int color, bool bUseColor = true);
	int	drawCheck(CDC* pDC, CRect rect, int Check);
	int	GetIndex(int nData);
	void	setCheckList(bool check) { m_bCheck = check; }
	bool	getCheckList() { return m_bCheck; }
	DWORD	getColorBit(DWORD dword) { return (0x00FFFFFF & dword); }
	DWORD	getCheckBit(DWORD dword) { return ((0x0F000000 & dword) >> 24); }
	DWORD	getUseBit(DWORD dword) { return ((0xF0000000 & dword) >> 28); }

	DWORD	setColorBit(DWORD org, int color) 
	{ 
		DWORD	dword = 0;
		dword = (0xFF000000 & org) | (0x00FFFFFF & color);
		return dword;
	}

	DWORD	setCheckBit(DWORD org, int check) 
	{ 
		DWORD	dword = 0;
		dword = (0xF0FFFFFF & org) | (0x0F000000 & (check << 24));
		return dword;
	}

	DWORD	setUseBit(DWORD org, int use) 
	{ 
		DWORD	dword = 0;
		dword = (0x0FFFFFFF & org) | (0xF0000000 & (use << 28));
		return dword;
	}

	void	SetItemText(int nIndex, CString input);

	COLORREF	GetSelectedColorValue();
	CString		GetSelectedColorName();
	COLORREF	GetColor(int nIndex = -1);	

	//{{AFX_VIRTUAL(CLineListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	

protected:
	//{{AFX_MSG(CLineListBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void	Initialize();

	CString		m_sColorName;
	CString		m_szColor;
	bool		m_bCheck;
};

/********************************************************************/
// CListCtrlSort class
/********************************************************************/

class CListCtrlSort  
{
public:
	enum SortDataType 
	{ 
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

/********************************************************************/
// CAxListCtrl class
/********************************************************************/


class CAxListCtrl : public CListCtrl
{
public:
	CAxListCtrl();
	virtual ~CAxListCtrl();
	int	AddItem(CString input, int nRow = -1, int nCol = 0, LPARAM param = 0);
	BOOL	IsExist(CString input, int nCol);
	int	DelItem(int nRow = -1);
	int	MoveItem(BOOL bUp = TRUE);
	int	InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	int	InsertColumn(int nCol, const LVCOLUMN *pColumn);
	BOOL	DeleteColumn(int nCol);	
	void	SetSelection(int nRow);
	int	GetSelection();
	int	GetColCount() { return m_nColCnt; }
	void	SetMaxItem(int nMax) { m_nMaxItem = nMax; }
	int	GetMaxItem() { return m_nMaxItem; }
	void	ListSort(int nCol, BOOL bToggle = TRUE);
	int	ModifyItem(CString newval, int nRow, int nCol = 0);
	int	FindRow(CString value, int nCol = 0);
	void	setCheck(int index, BOOL bCheck);
	BOOL	getCheck(int index);
	int	GetCheckCount();
	void	HaveCheckBox();
public:
	//{{AFX_VIRTUAL(CAxListCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAxListCtrl)
	//}}AFX_MSG

private:
	int	m_nMaxItem;
	int	m_nColCnt;
	BOOL	m_bSort;
	BOOL	m_bCheckBox;
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_CONTROLEX_H__90291C77_D834_4913_81A0_1892F38071F8__INCLUDED_)

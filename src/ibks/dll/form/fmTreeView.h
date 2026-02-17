#if !defined(AFX_FMTREEVIEW_H__79D0C03E_99C6_4260_886D_C10C984F2A73__INCLUDED_)
#define AFX_FMTREEVIEW_H__79D0C03E_99C6_4260_886D_C10C984F2A73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmTreeView.h : header file
//

#include <afxtempl.h>
#include "fmbase.h"

/////////////////////////////////////////////////////////////////////////////
// CfmTreeView command target

class Ctreeitem
{
public:
	CString	m_name;
	CString	m_data;
	bool	m_expanded;
	bool	m_selected;
	enum	{ tiItem, tiFolder }	m_type;
	CRect	m_rect;
	CRect	m_button;
	CArray < Ctreeitem *, Ctreeitem *> m_children;
};

class AFX_EXT_CLASS CfmTreeView : public CfmBase
{
	DECLARE_DYNCREATE(CfmTreeView)

public:
	CfmTreeView();	           // protected constructor used by dynamic creation
	CfmTreeView(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmTreeView();

// Attributes
protected:
	CArray	< Ctreeitem *, Ctreeitem *> m_tree;
	CArray	< Ctreeitem *, Ctreeitem *> m_select;
	Ctreeitem* m_item;
	Ctreeitem* m_shift;
	Ctreeitem* m_start;
	Ctreeitem* m_drop;

	CString	m_path;
	CString	m_name;
	int	m_type;
	int	m_indentX;
	bool	m_drawing;
	bool	m_changed;
	int	m_items;	// current visible item count
	int	m_showItem;	// show item count
	int	m_totalItem;	// total item count
	int	m_startpos;
	float	m_itemH;
	float	m_gap;
	CRect	m_scr;
	CRect	m_thumbRect;
	CPoint	m_lastPt;
	bool	m_scroll;

	enum	{ ckNone=0, ckExpand, ckItem, ckFolder, ckScrollUp, ckScrollDown, ckRgnUp, ckRgnDown, ckThumb } m_click;

// Operations
public:
	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	void	Reload();
	void	Draw(CDC* dc);

	void	OnLButton(bool down, CPoint pt, int& result);
	void	OnDblClick(CPoint pt, int& result);
	bool	OnRepeat(CPoint pt, int& result);
	bool	OnMouseWheel(int delta, int& result);
	int	OnDrag(CPoint pt, int& result);
	bool	OnDrop(CPoint pt, CString data, CString& key);
	
	void	SetFocus(bool focus);
	
	void	ReadAll(CString& data, bool edit = false, bool head = false);
	void	ReadData(CString& data, bool edit = false, int size = -1, int col = -1, int row = -1);

	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ClearData(bool selectOnly = false);

	void	GetEnum(CString& text, int& value, int func = -1);
	void	SetEnum(CString text, int value, int func = -1);

	bool	IsChanged(bool reset = true);
	int	GetLength();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmTreeView)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool	loadTree();
	bool	loadItem(CString name, Ctreeitem* parent, CString finds);
	void	saveItem();
	void	saveItem(CString data);
	void	removeItem(Ctreeitem* parent);
	
	void	drawTreeItem(CDC* dc, CRect& rc, int indention, Ctreeitem* item, bool home = false, bool last = false);
	void	drawButton(CDC* dc, CRect& rc, int indention, bool expanded = false);
	void	drawLine(CDC* dc, CPoint pt1, CPoint pt2);
	void	clearRect(Ctreeitem* item);

	int	keyArrowLeft(int& result);
	void	keyArrowRight(int& result);
	void	keyArrowUp(int& result);
	void	keyArrowDown(int& result);
	void	keyPrior(int& result);
	void	keyNext(int& result);
	void	keyHome(int& result);
	void	keyEnd(int& result);

	Ctreeitem* getParent(Ctreeitem* item);
	Ctreeitem* getParent(Ctreeitem* item, bool& root);
	bool	getParent(Ctreeitem* item, Ctreeitem*& parent);
	Ctreeitem* getSibling(Ctreeitem* item, bool elder = true);
	Ctreeitem* getFirstChild(Ctreeitem* item);
	Ctreeitem* getLastChild(Ctreeitem* item);

	void	setItem(Ctreeitem* item);
	int	addSelect(Ctreeitem* item);
	CRect	removeSelect(Ctreeitem* item, bool all = false);
	void	resetSelect(Ctreeitem* item);
	bool	isItemSelected(Ctreeitem* item, int& index);
	bool	isItemVisible(Ctreeitem* item);
	void	shiftState(Ctreeitem* item);
	void	findStartItem();
	bool	findItem(Ctreeitem* item, Ctreeitem* find, int& order);
	bool	findItem(Ctreeitem* item, int& order, Ctreeitem*& find);
	bool	findItem(int order, Ctreeitem*& find);
	bool	findItem(Ctreeitem* item, CString data, bool all = false);
	bool	leftButton(Ctreeitem* item, CPoint pt, int state = 0);
	bool	doubleClick(Ctreeitem* item, CPoint pt);
	bool	drop(Ctreeitem* item, CPoint pt);

	bool	isScroll();
	void	countVisibleItem(Ctreeitem* item, int& count);

	// Generated message map functions
	//{{AFX_MSG(CfmTreeView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmTreeView)
	afx_msg BSTR _getText();
	afx_msg BSTR _getData();
	afx_msg void _setData(LPCTSTR lpszNewValue);
	afx_msg long _getPRgb();
	afx_msg void _setPRgb(long nNewValue);
	afx_msg long _getTRgb();
	afx_msg void _setTRgb(long nNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg long _getHitPos();
	afx_msg BOOL _getEnable();
	afx_msg void _setEnable(BOOL bNewValue);
	afx_msg void _Reload();
	afx_msg void _Refresh();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMTREEVIEW_H__79D0C03E_99C6_4260_886D_C10C984F2A73__INCLUDED_)

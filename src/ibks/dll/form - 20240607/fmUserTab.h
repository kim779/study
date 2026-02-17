#if !defined(AFX_FMUSERTAB_H__2B7B120B_6227_4D47_ADE2_39E05466E470__INCLUDED_)
#define AFX_FMUSERTAB_H__2B7B120B_6227_4D47_ADE2_39E05466E470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmUserTab.h : header file
//

#include "fmTab.h"
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CfmUserTab command target

class AFX_EXT_CLASS CfmUserTab : public CfmBase
{
	DECLARE_DYNCREATE(CfmUserTab)

public:
	CfmUserTab();	           // protected constructor used by dynamic creation
	CfmUserTab(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmUserTab();

// Attributes
protected:
	int		m_count;
	int		m_start;
	int		m_last;
	int		m_index;
	bool		m_scroll;

	COLORREF	m_fpRGB;
	COLORREF	m_ftRGB;

	CStringArray	m_map;
	CStringArray	m_caption;

	CRect		m_scr;
	CRect		m_cell;
	CRect		m_more;
	enum	{ ckNone=0, ckTab, ckMore, ckLeft, ckRight } m_click;
	enum	{ mvNone=0, mvTab, mvScroll, mvMore } m_move;
	int		m_imove;

	bool		m_bForm;
	int		m_type;
	CString		m_name;
	bool		m_vertical;
	bool		m_hasfocus;
	CMap	< int, int, CBitmap*, CBitmap* > m_images;
	CMap	< int, int, CBitmap*, CBitmap* > m_orders;
	class	Cimage*	m_moreBM;
	class	Cimage*	m_scrBM;

	int		m_leftW;
	int		m_rightW;
	int		m_moreW;

// Operations
public:
	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	void	Reload();
	int	IsPointInRect(CPoint pt);
	void	SetFont(int point, int style, CString fonts, bool resize);

	void	Draw(CDC* dc);
	void	OnLButton(bool down, CPoint pt, int& result);
	int	OnDrag(CPoint pt, int& result);
	bool	OnDrop(CPoint pt, CString Data, CString& key);

	void	SetFgColor(int rgb, int col = -1, int row = -1);
	void	SetBkColor(int rgb, int col = -1, int row = -1);

	void	ReadAll(CString& data, bool edit = false, bool head = false);
	void	ReadData(CString& data, bool edit = false, int size = -1, int col = -1, int row = -1);
	void	WriteAll(CString str, bool redraw = true);
	void	WriteData(CString data, bool redraw = true, int key = -1, int visible = -1);

	void	SetCurSel(int index, bool redraw = true);
	int	GetCurSel();
	int	GetSize();

	void	GetEnum(CString& text, int& value, int func = -1);
	void	SetEnum(CString text, int value, int func = -1);

	BOOL	IsTabVisible(long ID);
	void	SetTabVisible(long ID, BOOL visible);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmUserTab)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	loadTab(CString text);
	void	loadBitmap(CString theme = _T(""));
	void	loadABitmap();
	void	loadOrderBitmap();

	void	drawTab(CDC* dc, CRect pRc, bool scroll);
	void	drawScroll(CDC* dc, CRect rect);
	void	drawMore(CDC* dc, CRect rect);
	void	drawVText(CDC* dc, CString tabs, CRect rect, UINT style);
	void	drawNoneTab(CDC* dc, CRect tRc, CRect& tRc1, int cx, bool focus_this, UINT& style);

	bool	isScrolled(CDC* dc, int& index, int& width, int start = -1);
	int	getSize(CDC* dc, int index, bool focus = false);
	void	getString(CString string, CStringArray& arrStr);
	int	getFirstVisibleItem(int start);
	int	getLastVisibleItem();
	void	scroll(bool prev = true);
	int	getIndexByID(int ID);
	int	getStartIndex(int index);
	bool	isJustBeforeSelectItem(int index);

	// Generated message map functions
	//{{AFX_MSG(CfmUserTab)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmUserTab)
	afx_msg long _getID();
	afx_msg void _setID(long nNewValue);
	afx_msg BSTR _getText();
	afx_msg void _setText(LPCTSTR lpszNewValue);
	afx_msg long _getIndex();
	afx_msg void _setIndex(long nNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg BOOL _getEnable();
	afx_msg void _setEnable(BOOL bNewValue);
	afx_msg void _Reload();
	afx_msg BSTR _GetText(long ID);
	afx_msg void _SetText(long ID, LPCTSTR text);
	afx_msg BOOL _IsTabVisible(long ID);
	afx_msg void _SetTabVisible(long ID, BOOL visible);
	afx_msg void _Change(long ID, BOOL visible, LPCTSTR text);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMUSERTAB_H__2B7B120B_6227_4D47_ADE2_39E05466E470__INCLUDED_)

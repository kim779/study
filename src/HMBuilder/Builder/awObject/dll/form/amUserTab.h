#if !defined(AFX_FMUSERTAB_H__2B7B120B_6227_4D47_ADE2_39E05466E470__INCLUDED_)
#define AFX_FMUSERTAB_H__2B7B120B_6227_4D47_ADE2_39E05466E470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmUserTab.h : header file
//

//#include "amTab.h"
#include "amBase.h"
#include <afxtempl.h>
#include "image.h"

#define	TAB_EXTRA	14
#define	TAB_EXTRA2	25

/////////////////////////////////////////////////////////////////////////////
// CamUserTab command target

class AFX_EXT_CLASS CamUserTab : public CamBase
{
	DECLARE_DYNCREATE(CamUserTab)

public:
	CamUserTab();	           // protected constructor used by dynamic creation
	CamUserTab(CAmForm* axform, struct _formR* formR);
	virtual ~CamUserTab();

// Attributes
protected:
	int		m_vcount;
	int		m_count;
	int		m_start;
	int		m_last;
	int		m_index;
	bool		m_scroll;

	COLORREF	m_fpRGB;
	COLORREF	m_ftRGB;

	CStringArray	m_map;
	CStringArray	m_caption;
	CStringArray	m_object;

	CRect		m_scr;
	CRect		m_cell;
	enum	{ ckNone=0, ckTab, ckLeft, ckRight } m_click;

	bool		m_bForm;
	int		m_type;
	CString		m_namex;
	CString		m_path;
	bool		m_vertical;
	bool		m_hasfocus;
	CMap		< int, int, Bitmap*, Bitmap* > m_images;
	Cimage*		m_pic;
//	CImageList	m_simages;

// Operations
public:
	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	void	Reload();
	int		IsPointInRect(CPoint pt);
	void	SetFont(int point, int style, CString fonts, bool resize);

	void	Draw(CDC* dc);
	void	OnLButton(bool down, CPoint pt, int& result);
	int		OnDrag(CPoint pt, int& result);
	bool	OnDrop(CPoint pt, CString Data, CString& key);

	void	SetBkColor(int rgb, int col = -1, int row = -1);

	void	ReadAll(CString& data, bool edit = false, bool head = false);
	void	ReadData(CString& data, bool edit = false, int col = -1, int row = -1);
	void	WriteAll(CString str, bool redraw = true);
	void	WriteData(CString data, bool redraw = true, int key = -1, int visible = -1);

	void	SetCurSel(int index, bool redraw = true);
	int	GetCurSel();
	int	GetSize();

	void	GetEnum(CString& text, int& value, int func = -1);
	void	SetEnum(CString text, int value, int func = -1);

	int		getSizeOld(CDC* dc, int index, bool focus);
	BOOL	IsTabVisible(long ID);
	void	SetTabVisible(long ID, BOOL visible);


	long	GetID();
	void	SetID(long nNewValue);
	LPCTSTR	GetText();
	void	SetText(LPCTSTR lpszNewValue);
	LPCTSTR	GetData();
	void	SetData(LPCTSTR lpszNewValue);
	long	GetIndex();
	void	SetIndex(long nNewValue);
	long	GetPRgb();
	void	SetPRgb(long nNewValue);
	long	GetTRgb();
	void	SetTRgb(long nNewValue);
	BOOL	GetVisible();
	void	SetVisible(BOOL bNewValue);
	BOOL	GetEnable();
	void	SetEnable(BOOL bNewValue);

	void _Reload();
	BSTR _GetText(long ID);
	void _SetText(long ID, LPCTSTR text);
	BOOL _IsTabVisible(long ID);
	void _SetTabVisible(long ID, BOOL visible);
	void _Change(long ID, BOOL visible, LPCTSTR text);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamUserTab)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	loadTab();
	void	loadBitmap(CString prefix = _T(""));
	void	loadSBitmap();

	void	drawTab(CDC* dc, CRect pRc, bool scroll);
	void	drawScroll(CDC* dc, CRect scrRc);
	void	drawVText(CDC* dc, CString tabs, CRect rect, UINT style, StringFormat& sf);
	void	drawNoneTab(CDC* dc, CRect tRc, CRect& tRc1, int cx, bool focus_this, UINT& style, StringFormat& sf);

	bool	isScrolled(CDC* dc, int& index, int& width, int start = -1);
	int		getSize(Graphics& g, int index, bool focus = false);
	int		getSize(CDC* dc, int index, bool focus = false);
	void	getString(CString string, CStringArray& arrStr);
	int		getFirstVisibleItem(int start);
	int		getLastVisibleItem();
	void	scroll(bool prev = true);
	int		getIndexByID(int ID);
	int		getStartIndex(int index);
	bool	isJustBeforeSelectItem(int index);
	bool	SelectCustomForm(CString sForm, CString sMap);
	

	// Generated message map functions
	//{{AFX_MSG(CamUserTab)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamUserTab)
	
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMUSERTAB_H__2B7B120B_6227_4D47_ADE2_39E05466E470__INCLUDED_)

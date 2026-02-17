#if !defined(AFX_FMGRID_H__273F70F6_BEFC_4165_84A9_A3F55CA4829B__INCLUDED_)
#define AFX_FMGRID_H__273F70F6_BEFC_4165_84A9_A3F55CA4829B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmGrid.h : header file
//

#include <afxtempl.h>
#include "fmbase.h"
#include "axarray.h"

#define	sASCEND		"¡â"
#define	sDESCEND	"¡ä"

#define	isUp		0
#define	isDown		1
#define	isHome		2
#define	isPageUp	3
#define	isPageDn	4
#define	isLeft		5
#define	isRight		6

/////////////////////////////////////////////////////////////////////////////
// CfmGrid command target

class Ccolumn
{
public:
	Ccolumn();
	~Ccolumn();

public:
	int		m_rts;

	int		m_line;		// index in multi-line
	float		m_width;	// resized width
	WORD		m_widthx;	// original width

	CString		m_head;		// head string
	CString		m_hcell;	// head cell
	WORD		m_ihead;	// head line count
	CStringArray	m_heads;	// head strings (GO_MERGE)

	COLORREF	m_tRGB;		// head text  RGB
	COLORREF	m_pRGB;		// head paint RGB

	CAxDWordArray	m_tRGBs;
	CAxDWordArray	m_pRGBs;
	CWordArray	m_itRGBs;	// palette index of tRGB. if tRGB & userCOLOR, itRGB = -1

	CString		m_edit;
	CStringArray	m_display;
	CStringArray	m_data;
	CMap	< int, int, CString, CString& > m_items;

	CAxByteArray	m_valids;
	int		m_ref;
	bool		m_visible;
	
	CAxDWordArray	m_attrs;
	CAxByteArray	m_enables;
	CAxByteArray	m_floats;

	CStringArray	m_hdisplay;
	CStringArray	m_hdata;

	class	Cimage*	m_hpic;
	class	Cimage*	m_pic;
	CMap	< int, int, Cimage*, Cimage* > m_images;
};


class AFX_EXT_CLASS CfmGrid : public CfmBase
{
	DECLARE_DYNCREATE(CfmGrid)

public:
	CfmGrid();	           // protected constructor used by dynamic creation
	CfmGrid(CAxisForm* axform, struct _formR* formR, int iform, struct _cellR* cellR = NULL);
	virtual ~CfmGrid();

// Attributes
public:
	class	CcbList* m_list;

protected:
	struct	_cellR*	m_cellR;
	CArray	< Ccolumn*, Ccolumn* > m_columns;

	int	m_index;
	int	m_stCol;		// horizontal start column
	int	m_enCol;		// horizontal end column
	int	m_stRow;		// vertical start index

	int	m_showRows;
	int	m_validRows;
	int	m_totRows;
	int	m_totCols;

	int	m_multiN;
	int	m_headN;

	float	m_totWidth;		// total column width
	float	m_showWidth;		// shown width
	float	m_scrolledWidth;	// scrolled width
	float	m_fixedWidth;		// fixed width

	bool	m_hscroll;		// visible or invisible
	bool	m_vscroll;		// act or deact
	bool	m_enableScroll;		// enable scroll

	int	m_result;
	enum	{ ckNone=0, ckHead, ckItem, ckPageUp, ckPageDown, ckFirst, ckScrollUp, ckScrollDown, ckRgnUp, ckRgnDown,
			ckScrollLeft, ckScrollRight, ckRgnLeft, ckRgnRight, ckVthumb, ckHthumb, ckCombo, ckOrigin, ckEdit } m_click;
	enum	SAVE { svExchange=0, svInsert, svAppend }	m_save;

	float	m_vgap;
	float	m_hgap;
	int	m_cellH;		// original cell height
	float	m_cy;			// cell height
	bool	m_fixRows;

	CRect	m_vscr;			// vscroll rect.
	CRect	m_hscr;			// hscroll rect.
	CRect	m_pscr;			// scroll page rect.
	CRect	m_vthumb;
	CRect	m_hthumb;
	CRect	m_editRc;
	CPoint	m_point;

	CAxStringArray	m_data;
	CAxStringArray	m_edit;
	int	m_slit;
	bool	m_writingAll;
	bool	m_drawing;

	enum	{ mvNone=0, mvMove, mvDrag, mvSel, mvHead, mvItem } m_move;
	CRect	m_moveRc;
	int	m_moveCol;
	int	m_delta;
	Cpos	m_movepos;	// 0 base

	bool	m_useInfo;
	int	m_stat;
	CString	m_page;
	int	m_dir;
	int	m_sortc;
	CString	m_inits;

	COLORREF m_fRGB;		// GO_ROW, 1st Color
	COLORREF m_sRGB;		//         2nd Color
	COLORREF m_rRGB;		// isEdit == FI_RBAR, reverse Color

// INPUT
	enum	{ stNone=0, stFocus, stEdit, stCheck, stCombo, stButton } m_state;
	bool	m_statechange;
	CRect	m_cell;
	bool	m_input;

// stEdit
	bool	m_edcaret;
	bool	m_edfocusing;
	int	m_edcaretpos;
	enum	{ esNone=0, esReverse, esClick, esNormal } m_edstate;
	CPoint	m_edclick;

// stCombo
	bool	m_cbspread;
	CRect	m_cbbtn;

// focus, change, select
	bool	m_setfocus;
	bool	m_changed;
	Cpos	m_cellpos;	// 1 base
	Cpos	m_changedpos;	// 1 base
	Cpos	m_curpos;	// 1 base
	Cpos	m_lastpos;	// 1 base
	bool	m_select;

// combobox image
	class	Cimage*	m_pic;

// checkbox image
	CMap	< int, int, CBitmap*, CBitmap* > m_checks;

// edit-column image
	class	Cimage*	m_epic;

// GO_FIX
	int	m_bottom_gap;
	int	m_right_gap;

// scroll image
	CMap	< int, int, Cimage*, Cimage* > m_scrolls;

// symbol in portfolio
	int	m_portfolio;

// Operations
public:
	int	GetName(int col = -1);
	bool	IsValid(int& guide);
	void	Adjust(CPoint pt, float hR, float vR);
	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	void	Reload();
	int	IsPointInRect(CPoint pt);
	void	SetFont(int point, int style, CString fonts, bool resize);

	void	Draw(CDC* dc);

	void	OnLButton(bool down, CPoint pt, int& result);
	void	OnRButton(bool down, CPoint pt, int& result);
	void	OnDblClick(CPoint pt, int& result);
	bool	OnRepeat(CPoint pt, int& result);
	bool	OnMouseWheel(int delta, int& result);
	int	OnDrag(CPoint pt, int& result);
	bool	OnDrop(CPoint pt, CString data, CString& key);
	int	OnKey(int key, int& result);

	void	InsertRow(int index, CString str = _T(""));
	void	InsertRows(int index, CString str = _T(""));
	void	RemoveRow(int index);
	void	WriteAll(CString str, bool redraw = true);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ReadAll(CString& data, bool edit = false, bool head = false);
	void	ReadData(CString& data, bool edit = false, int size = -1, int col = -1, int row = -1);
	void	ClearData(bool selectOnly = false);
	void	UpdateData(int key, bool moving, int& result);
	void	InsertData(int key, bool moving, int& result);

	void	GetEnum(CString& text, int& value, int func = -1);
	void	SetEnum(CString text, int value, int func = -1);

	void	SetFocus(bool focus);

	bool	IsChanged(bool reset = true);
	void	SetCellRow(int row);
	void	SetCellColumn(int col);
	int	GetCellRow();
	int	GetCellColumn();
	int	GetValidRows();

	void	SetAttributes(DWORD attr, bool set = true, int col = -1, int row = -1);
	void	SetVisible(bool visible, int col = -1);
	bool	IsVisible(int col = -1);
	void	SetEnable(bool enable);
	bool	IsEnable();
	void	SetCurSel(int index, bool redraw = true);
	int	GetCurSel();
	void	SetFgColor(int rgb, int col = -1, int row = -1);
	void	SetBkColor(int rgb, int col = -1, int row = -1);
	int	GetFgColor(int col = -1, int row = -1);
	int	GetBkColor(int col = -1, int row = -1);
	void	SetRowColor(int fRgb, int sRgb);
	void	SetDomino(bool redraw = true);

	int	GetSize();
	void	Sort(int col, bool ascend, bool string);

	BOOL	IsColumnVisible(long column);
	void	SetColumnVisible(long column, BOOL visible);
	BOOL	IsCellEnable(long row, long column);
	void	SetCellEnable(long row, long column, BOOL enable);
	BOOL	IsColumnSortable(long column);
	CString	GetColumnName(long column);
	void	SetDataPosition(long method);

	void	SetItems(int col, CString items);
	void	SetCellItems(long row, long col, CString items);
	void	SetCellFocus(long row, long col);
	void	Import(CString files);
	void	SetCellAttr(long row, long column, long attrAdd, long attrRemove);

	void	Refresh();
	void	DoListBox(bool apply, bool focus = false);
	bool	IsFocus(HWND hWnd);
	bool	IsAutoThousand();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmGrid)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	int	insertRow(int index, CString str = _T(""), bool dataOnly = false, BOOL skipInput = FALSE);
	void	readData(CString& data, bool edit, int col, int row);
	CString	readAll(bool head = false);
	CString	editData(CString str, int col, int row, bool input = false);

	void	makeSignData(CString& data, int col = -1, int row = -1);
	void	doColumnEditCORGB(int row, int col, CString& data);

	void	drawEdit(CDC* dc, CRect rc, Ccolumn* column, int col, int row, CString data, COLORREF pRGB=0xffffffff, COLORREF tRGB=0xffffffff);
	void	drawCombo(CDC* dc, CRect rc, Ccolumn* column, int col, int row, CString data, COLORREF pRGB=0xffffffff, COLORREF tRGB=0xffffffff);
	void	drawCheck(CDC* dc, CRect rc, Ccolumn* column, int col, int row, CString data, COLORREF pRGB=0xffffffff, COLORREF tRGB=0xffffffff);
	void	drawButton(CDC* dc, CRect rc, Ccolumn* column, int col, int row);
	void	drawHead(CDC* dc, int col, CString data, CRect rc, UINT format);
	void	drawPage(CDC* dc, CRect rc, int type, bool up, bool enable = true);
	void	drawHead(CDC* dc, CRect rc);
	void	drawHead(CDC* dc, int col, CRect rc, CString data);
	void	drawMark(CDC* dc, CRect rc);
	void	drawCombo(CDC* dc, CRect rc, int col);
	void	drawCheck(CDC* dc, CRect rc, int col, CString data);

	void	lbutton(bool down, CPoint pt, int& result);
	bool	lbuttonEdit(bool down, CPoint pt, int& result);
	bool	lbuttonCombo(bool down, CPoint pt, int& result);
	bool	lbuttonCheck(bool down, CPoint pt, int& result);
	bool	lbuttonButton(bool down, CPoint pt, int& result);
	void	rbutton(bool down, CPoint pt, int& result);
	int	drag(CPoint pt, int& result);

	void	keyDelete(int& result);
	void	keyBackspace(int& result);
	int	keyArrowLeft(int& result);
	void	keyArrowRight(int& result);
	void	keyArrowUp(int& result);
	void	keyArrowDown(int& result);
	void	keyReturn(int& result);
	void	keyEscape(int& result);
	void	keyPrior(int& result);
	void	keyNext(int& result);
	void	keyHome(int& result);
	void	keyEnd(int& result);
	void	keyTab(int& result);

	int	findColumn(CString name, bool origin = true);
	int	findColumn(int ref);
	Ccolumn* getColumn(int ref);
	int	findRow(int py, bool isrow = true);
	int	findRow(int py, CRect& rc, CRect& rcx);
	int	findStartColumn(int col);
	bool	findCell(bool back, int row, int col);
	bool	findCell(CPoint pt, int& row, int& col, CRect& hRc, CRect& vRc);

	bool	setFgColor(int row, int col, int rgb);
	bool	setBkColor(int row, int col, int rgb);

	void	setInformation(CString info);
	void	getInformation(CString& info);
	void	setSortInfo(CString info);

	int	validCell(int index, int col, int& ref);
	bool	getRect(int row, int col, CRect& rect, bool head = false);
	bool	makeListBox(int row, int col, CRect rItem, bool head = false);
	bool	makePopup(int col, CPoint pt);
	CRect	setRect(CDC* dc, int col, CRect rItem, CStringArray& arr, int height);
	int	getStart(int index, int show, int count);
	int	getIndex(int col = -1, int row = -1, CString data = _T(""), bool force = false);
	CString	getString(int col = -1, int row = -1, CString data = _T(""));
	CString	getString(int col, int row, int sel, bool display = true);
	int	calcCaret(CString data, CString edit);

	int	getRowIndex(CPoint pt);
	int	getVisibleRows();
	bool	getMovableColumn(CPoint pt);
	bool	isHScrollEnabled();
	bool	isReverseCell(int row, int col);
	void	refreshSel(Cpos pos, int key);

	void	changeIME(int row, int col);
	double	compare(bool string, CString arg1, CString arg2, int col = -1);

	void	loadBitmap(int type);

	// Generated message map functions
	//{{AFX_MSG(CfmGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmGrid)
	afx_msg BSTR _getText();
	afx_msg BSTR _getData();
	afx_msg void _setData(LPCTSTR lpszNewValue);
	afx_msg BSTR _getCell();
	afx_msg void _setCell(LPCTSTR lpszNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg long _getIndex();
	afx_msg void _setIndex(long nNewValue);
	afx_msg long _getRow();
	afx_msg void _setRow(long nNewValue);
	afx_msg long _getColumn();
	afx_msg void _setColumn(long nNewValue);
	afx_msg BSTR _getHead();
	afx_msg void _setHead(LPCTSTR lpszNewValue);
	afx_msg long _getRows();
	afx_msg long _getColumns();
	afx_msg long _getPos();
	afx_msg void _setPos(long nNewValue);
	afx_msg BSTR _getPage();
	afx_msg void _setPage(LPCTSTR lpszNewValue);
	afx_msg long _getDirection();
	afx_msg void _setDirection(long nNewValue);
	afx_msg BSTR _getSortColumn();
	afx_msg void _setSortColumn(LPCTSTR lpszNewValue);
	afx_msg long _getAction();
	afx_msg BSTR _getInfo();
	afx_msg void _setInfo(LPCTSTR lpszNewValue);
	afx_msg long _getRealTm();
	afx_msg long _getHitPos();
	afx_msg BOOL _getFocus();
	afx_msg long _getRangeColumn1();
	afx_msg long _getRangeColumn2();
	afx_msg long _getRangeRow1();
	afx_msg long _getRangeRow2();
	afx_msg BSTR _getHeadCell();
	afx_msg void _setHeadCell(LPCTSTR lpszNewValue);
	afx_msg void _SetFocus();
	afx_msg void _Refresh();
	afx_msg BSTR _GetCell(long row, long col);
	afx_msg void _SetCell(long row, long col, LPCTSTR cells);
	afx_msg BSTR _GetText(long row, long col);
	afx_msg long _GetPRgb(long row, long col);
	afx_msg void _SetPRgb(long row, long col, long rgb);
	afx_msg long _GetTRgb(long row, long col);
	afx_msg void _SetTRgb(long row, long col, long rgb);
	afx_msg BOOL _IsColumnVisible(long col);
	afx_msg void _SetColumnVisible(long col, BOOL visible);
	afx_msg BOOL _IsCellEnable(long row, long col);
	afx_msg void _SetCellEnable(long row, long col, BOOL enable);
	afx_msg BSTR _GetHead(long col);
	afx_msg void _SetHead(long col, LPCTSTR head);
	afx_msg BOOL _IsColumnSortable(long col);
	afx_msg BSTR _GetColumnName(long col);
	afx_msg void _SetDataPosition(long method);
	afx_msg void _Sort(long col, BOOL ascend, BOOL string);
	afx_msg void _InsertRow(long index);
	afx_msg void _RemoveRow(long index);
	afx_msg void _Key(long key);
	afx_msg void _Push(long row, long col, LPCTSTR name);
	afx_msg void _Pop(long row, long col, LPCTSTR name);
	afx_msg void _Print();
	afx_msg void _Clear();
	afx_msg void _SetItems(long col, LPCTSTR items);
	afx_msg void _SetCellFocus(long row, long col);
	afx_msg void _Import(LPCTSTR files);
	afx_msg void _SetCellAttr(long row, long column, long attrAdd, long attrRemove);
	afx_msg void _Excel(BOOL run, BOOL head, LPCTSTR path);
	afx_msg void _Export(BOOL head, LPCTSTR path, BOOL append);
	afx_msg void _Edit();
	afx_msg void _SetSkip(LPCTSTR cname, BOOL skip);
	afx_msg void _ExportEx(BOOL run, LPCTSTR top, LPCTSTR bottom, LPCTSTR path);
	afx_msg void _SetCellItems(long row, long col, LPCTSTR items);
	afx_msg void _MultiSort(LPCTSTR cols, BOOL ascend);
	afx_msg void _Blink(long row, long col, BOOL set, long color);
	afx_msg void _SetFormat(long row, long col, LPCTSTR formats);
	afx_msg void _Excel2(BOOL run, BOOL head, LPCTSTR path);
	afx_msg BOOL _GetCellAttr(long row, long col, long cc);
	afx_msg void _SetRows(long rows);
	afx_msg void _ExcelEx(BOOL head);
	afx_msg void _Update(long index, LPCTSTR data);
	afx_msg void _InsertRows(long index, LPCTSTR data, BOOL skipInput);
	afx_msg BSTR _GetCellx(long row, LPCTSTR colN);
	afx_msg void _SetCellx(long row, LPCTSTR colN, LPCTSTR data);
	afx_msg long _GetColumnWidth(long column);
	afx_msg void _SetColumnWidth(long column, long width);
	afx_msg BSTR _GetHeadCell(long col);
	afx_msg void _SetHeadCell(long col, LPCTSTR cells);
	afx_msg long _GetColumnDataKind(long column);
	afx_msg long _GetColumnMode(long column);
	afx_msg long _GetColumnLength(long column);
	afx_msg BSTR _GetColumnFormat(long column);
	afx_msg long _GetColumn(LPCTSTR colN);
	afx_msg void _SetText(long row, long col, LPCTSTR text);
	afx_msg void _SetTextx(long row, LPCTSTR colN, LPCTSTR text);
	afx_msg long _GetFloatDigit(long col);
	afx_msg void _SetFloatDigit(long col, long digit);
	afx_msg void _SetBlockMode(BOOL on);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMGRID_H__273F70F6_BEFC_4165_84A9_A3F55CA4829B__INCLUDED_)

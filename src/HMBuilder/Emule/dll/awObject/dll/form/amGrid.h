#if !defined(AFX_AMGRID_H__273F70F6_BEFC_4165_84A9_A3F55CA4829B__INCLUDED_)
#define AFX_AMGRID_H__273F70F6_BEFC_4165_84A9_A3F55CA4829B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmGrid.h : header file
//

#include <afxtempl.h>
#include "ambase.h"
#include "axarray.h"

#define	sASCEND		"¡â"
#define	sDESCEND	"¡ä"

/////////////////////////////////////////////////////////////////////////////
// CamGrid command target

class Ccolumn
{
public:
	Ccolumn();
	~Ccolumn();

public:
	int		m_rts;		// rts symbol

	int			m_line;		// index in multi-line
	float		m_width;	// resized width
	WORD		m_widthx;	// original width

	CString		m_head;		// head string
	WORD		m_ihead;	// head line count
	CStringArray	m_heads;	// head strings (GO_MERGE)

	ARGB	m_tRGB;		// head text  RGB
	ARGB	m_pRGB;		// head paint RGB

	CAxDWordArray	m_tRGBs;
	CAxDWordArray	m_pRGBs;
	CWordArray	m_itRGBs;	// palette index of tRGB. if tRGB & userCOLOR, itRGB = -1

	CString		m_edit;
	CStringArray	m_display;
	CStringArray	m_data;

	CAxByteArray	m_valids;
	int		m_ref;
	bool		m_visible;
	
	CAxDWordArray	m_attrs;
	CAxByteArray	m_enables;
	CAxByteArray	m_floats;

	class	Cimage*	m_pic;
};


class AFX_EXT_CLASS CamGrid : public CamBase
{
	DECLARE_DYNCREATE(CamGrid)

public:
	CamGrid();	           // protected constructor used by dynamic creation
	CamGrid(CAmForm* axform, struct _formR* formR, /*struct _cellR**/vector  <struct _cellR*>* cellR = NULL);
	virtual ~CamGrid();

// Attributes
public:
	class	CcbList* m_list;

protected:
	//struct	_cellR*	m_cellR;
	vector  <struct _cellR*>* m_cellR;
	CArray	< Ccolumn*, Ccolumn* > m_columns;
	CMap	< DWORD, DWORD, CString, CString& > m_names;

	int	m_index;
	int	m_stCol;		// horizontal start column
	int	m_enCol;		// horizontal end column
	int	m_stRow;		// vertical start index
	int	m_HeaderH;
	int	m_nfixRows;

	int	m_showRows;
	int	m_validRows;
	int	m_totRows;
	int	m_totCols;

	int	m_multiN;
	int	m_headN;

	float	m_totWidth;			// total column width
	float	m_showWidth;		// shown width
	float	m_scrolledWidth;	// scrolled width
	float	m_fixedWidth;		// fixed width

	bool	m_hscroll;			// visible or invisible
	bool	m_vscroll;			// act or deact
	bool	m_enableScroll;		// enable scroll

	int	m_result;
	enum	{ ckNone=0, ckHead, ckItem, ckPageUp, ckPageDown, ckFirst, ckScrollUp, ckScrollDown, ckRgnUp, ckRgnDown,
			ckScrollLeft, ckScrollRight, ckRgnLeft, ckRgnRight, ckVthumb, ckHthumb, ckCombo, ckOrigin } m_click;
	enum	SAVE { svExchange=0, svInsert, svAppend }	m_save;

	float	m_vgap;
	float	m_hgap;
	int		m_cellH;		// original cell height
	float	m_cy;			// cell height
	bool	m_fixRows;

	CRect	m_vscr;			// vscroll rect.
	CRect	m_hscr;			// hscroll rect.
	CRect	m_pscr;			// scroll page rect.
	CRect	m_vthumb;
	CRect	m_hthumb;
	CPoint	m_point;

	CAxStringArray	m_data;
	CAxStringArray	m_edit;
	int	m_slit;
	bool	m_writingAll;
	bool	m_drawing;

	enum	MSTATE { mvNone=0, mvMove, mvDrag, mvSel } m_move;
	CRect	m_moveRc;
	int	m_moveCol;
	int	m_delta;

	bool	m_useInfo;
	int	m_stat;
	CString	m_page;
	int	m_dir;
	int	m_sortc;
	CString	m_inits;

#ifdef _last
	ARGB m_fRGB;		// GO_ROW, 1st Color
	ARGB m_sRGB;		//         2nd Color
	ARGB m_rRGB;		// isEdit == FI_RBAR, reverse Color
#else
	COLORREF m_fRGB;		// GO_ROW, 1st Color
	COLORREF m_sRGB;		//         2nd Color
	COLORREF m_rRGB;		// isEdit == FI_RBAR, reverse Color
#endif

// INPUT
	enum	ISTATE { stNone=0, stFocus, stEdit, stCheck, stCombo, stButton } m_state;
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

// stButton
	bool	m_down;

// focus, change, select
	bool	m_setfocus;
	bool	m_changed;
	Cpos	m_cellpos;
	Cpos	m_changedpos;
	Cpos	m_curpos;
	Cpos	m_lastpos;
	bool	m_select;

// combo color, scroll color (GO_X3D)
	int	m_csRGB;		// GO_X3D, Combo Scroll Color
	int	m_cbRGB;		//         Combo Back Color
	int	m_ssRGB;		//         Scroll Color
	int	m_sbRGB;		//         Scroll Back Color
	int	m_slRGB;		//         Scroll Line Color

// combo image
	class	Cimage*	m_pic;	
	int	m_picW;

// Operations
public:
	CString	GetSymbolName(int col = -1);
	int	GetName(int col = -1);
	bool	IsValid(int& guide);
	void	Adjust(CPoint pt, float hR, float vR);
	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	void	Reload();
	int	IsPointInRect(CPoint pt);
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
	void	ReadData(CString& data, bool edit = false, int col = -1, int row = -1);
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
	void	_SetVisible(bool visible, int col = -1);
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
	void	SetCellFocus(long row, long col);
	void	Import(CString files);
	void	SetCellAttr(long row, long column, long attrAdd, long attrRemove);

	void	Refresh();
	void	DoListBox(bool apply, bool focus = false);
	bool	IsFocus(HWND hWnd);



	LPCTSTR	GetText();
	LPCTSTR	GetData();
	void	SetData(LPCTSTR lpszNewValue);
	LPCTSTR	GetCell();
	void	SetCell(LPCTSTR lpszNewValue);
	BOOL	GetVisible();
	void	SetVisible(BOOL bNewValue);
	long	GetIndex();
	void	SetIndex(long nNewValue);
	long	GetRow();
	void	SetRow(long nNewValue);
	long	GetColumn();
	void	SetColumn(long nNewValue);
	LPCTSTR	GetHead();
	void	SetHead(LPCTSTR lpszNewValue);
	long	GetRows();
	long	GetColumns();
	long	GetPos();
	void	SetPos(long nNewValue);
	LPCTSTR	GetPage();
	void	SetPage(LPCTSTR lpszNewValue);
	long	GetDirection();
	void	SetDirection(long nNewValue);
	LPCTSTR	GetSortColumn();
	void	SetSortColumn(LPCTSTR lpszNewValue);
	long	GetAction();
	LPCTSTR	GetInfo();
	void	SetInfo(LPCTSTR lpszNewValue);
	long	GetRealTm();
	long	GetHitPos();
	BOOL	GetFocus();
	
	void	_SetFocus();
	void	_Refresh();
	BSTR	_GetCell(long row, long col);
	void	_SetCell(long row, long col, LPCTSTR cells);
	BSTR	_GetText(long row, long col);
	long	_GetPRgb(long row, long col);
	void	_SetPRgb(long row, long col, long rgb);
	long	_GetTRgb(long row, long col);
	void	_SetTRgb(long row, long col, long rgb);
	BOOL	_IsColumnVisible(long col);
	void	_SetColumnVisible(long col, BOOL visible);
	BOOL	_IsCellEnable(long row, long col);
	void	_SetCellEnable(long row, long col, BOOL enable);
	BSTR	_GetHead(long col);
	void	_SetHead(long col, LPCTSTR head);
	BOOL	_IsColumnSortable(long col);
	BSTR	_GetColumnName(long col);
	void	_SetDataPosition(long method);
	void	_Sort(long col, BOOL ascend, BOOL string);
	void	_InsertRow(long index);
	void	_RemoveRow(long index);
	void	_Key(long key);
	void	_Push(long row, long col, LPCTSTR name);
	void	_Pop(long row, long col, LPCTSTR name);
	void	_Print();
	void	_Clear();
	void	_SetItems(long col, LPCTSTR items);
	void	_SetCellFocus(long row, long col);
	void	_Import(LPCTSTR files);
	void	_SetCellAttr(long row, long column, long attrAdd, long attrRemove);
	void	_Excel(BOOL run, BOOL head, LPCTSTR path);
	void	_Export(BOOL head, LPCTSTR path, BOOL append);
	void	_Edit();
	void	_SetSkip(LPCTSTR cname, BOOL skip);
	void	_ExportEx(BOOL run, LPCTSTR top, LPCTSTR bottom, LPCTSTR path);
	void	_SetRows(long rows);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamGrid)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	int		insertRow(int index, CString str = _T(""), bool dataOnly = false, BOOL skipInput = FALSE);
	void	readData(CString& data, bool edit, int col, int row);
	CString	editData(CString str, int col, int row, bool input = false);

	void	makeSignData(CString& data, int col = -1, int row = -1);
	void	doColumnEditCORGB(int row, int col, CString& data);

	void	drawEdit(CDC* dc, CRect rc, Ccolumn* column, int col, int row, CString data, COLORREF pRGB=0xffffffff, COLORREF tRGB=0xffffffff);
	void	drawCombo(CDC* dc, CRect rc, Ccolumn* column, int col, int row, CString data, COLORREF pRGB=0xffffffff, COLORREF tRGB=0xffffffff);
	void	drawCheck(CDC* dc, CRect rc, Ccolumn* column, int col, int row, CString data, COLORREF pRGB=0xffffffff, COLORREF tRGB=0xffffffff);
	void	drawButton(CDC* dc, CRect rc, Ccolumn* column, int col, int row);
	//void	drawHead(CDC* dc, int col, CString data, CRect rc, StringFormat* sf, COLORREF tRGB);
	void	drawHead(CDC* dc, int col, CString data, CRect rc, UINT format, StringFormat* sf, COLORREF tRGB);
	void	drawPage(CDC* dc, CRect rc, int type, bool up, bool enable = true, bool flat = false);
	void	drawScroll(CDC* dc, CRect rc, int type, bool enable = true, bool pushed = false);

	void	lbutton(bool down, CPoint pt, int& result);
	bool	lbuttonEdit(bool down, CPoint pt, int& result);
	bool	lbuttonCombo(bool down, CPoint pt, int& result);
	bool	lbuttonCheck(bool down, CPoint pt, int& result);
	bool	lbuttonButton(bool down, CPoint pt, int& result);
	void	rbutton(bool down, CPoint pt, int& result);

	void	keyDelete(int& result);
	void	keyBackspace(int& result);
	int		keyArrowLeft(int& result);
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

	int	findColumn(CString name);
	int	findColumn(int ref);
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
	bool	getRect(int row, int col, CRect& rect);
	bool	makeListBox(int col, CRect rItem);
	CRect	setRect(CDC* dc, int col, CRect rItem);
	int	getStart(int index, int show, int count);
	int	getIndex(int col = -1, int row = -1, CString data = _T(""), bool force = false);
	void	setData(int index, int col = -1, int row = -1);
	int	calcCaret(CString data, CString edit);

	int	getRowIndex(CPoint pt);
	int	getVisibleRows();
	bool	getMovableColumn(CPoint pt);
	bool	isHScrollEnabled();
	bool	isReverseCell(int row, int col);
	void	refreshSel(Cpos pos, int key);

	void	changeIME(int row, int col);
	double	compare(bool string, CString arg1, CString arg2, int col);

	// Generated message map functions
	//{{AFX_MSG(CamGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamGrid)	
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMGRID_H__273F70F6_BEFC_4165_84A9_A3F55CA4829B__INCLUDED_)

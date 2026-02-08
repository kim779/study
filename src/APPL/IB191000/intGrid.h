#pragma once
// fxGrid.h : header file
//
#include "afxtempl.h"
#include "afxmt.h"
#include "range.h"
#include "griditem.h"
#include "droptarget.h"
#include "IHCodeCtrl.h"
#include "TipDlg.h"	
class CDropData
{
public:
	CDropData() {}
	~CDropData() {}

	void Reset() { SetData(NULL, -1, -1), SetCode(""); }
	CintGrid* GetGrid() { return m_pGrid; }
	int GetRow() { return m_nRow; }
	int GetCol() { return m_nCol; }
	CString GetCode() { return m_code; }
	void SetCode(CString code) { m_code = code; }
	
	void SetData(CintGrid* pGrid, int nRow, int nCol)
	{
		m_pGrid = pGrid;
		m_nRow = nRow;
		m_nCol = nCol;
	}
	
private:
	
	CintGrid*	m_pGrid;
	int		m_nRow;
	int		m_nCol;
	CString		m_code;
};
class CCell : public CObject
{
public:
	CCell ()
	{
		state = 0;
		attr  = 0;
		format = 0;
		text = _T("");
		dtext = _T("");
		image = -1;
		fgcol = CLR_DEFAULT;
		bkcol = CLR_DEFAULT;
		param = 0;
	}

	UINT	 mask;
	UINT	 state;
	UINT	 attr;
	UINT	 format;
	CString	 text;
	CString	 dtext;
	int	 image;
	LOGFONT	 font;
	COLORREF fgcol;
	COLORREF bkcol;

	CString	 symbol;
	CString	 misc;

	LPARAM	 param;
};

typedef	CTypedPtrArray <CObArray, CCell*> GRID_ROW;

const	int maxMERGE = 10;

class _fixMerge
{
public:
	int	cols[maxMERGE];
	CRect	rect;
	LOGFONT	font;

	COLORREF fgcol;
	COLORREF bkcol;
	UINT	format;
	CString	text;
};

class CGridWnd;
class CintGrid;
void  AFXAPI DDX_GridControl(CDataExchange* pDX, int nIDC, CintGrid& rControl);

/////////////////////////////////////////////////////////////////////////////
// CintGrid window
enum { clr_UpIndex = 0, clr_NormalIndex, clr_DnIndex};

#define MAX_EQUALIZER		(10)

class CintGrid : public CWnd
{
	DECLARE_DYNCREATE(CintGrid)
// Construction
public:
	static CDropData	m_dropdata;	
	CintGrid() {}
	void FreeDragDrop() { m_idDrag.row = -1, m_idDrag.col, m_idDrop.row = -1, m_idDrag.col = -1; }
	CintGrid(CWnd* pMainWnd, CWnd* pParent, LOGFONT* logfont = NULL);
	CWnd*	Create(const RECT& rect, CWnd* parent, UINT nID, int NOscroll, int dragdrop = GVDD_FULL,
					DWORD dwStyle = WS_CHILD|WS_TABSTOP|WS_VISIBLE);
	bool	Initial(int nRow, int nCol, int fixRow, int fixCol);
	void	Refresh(bool NOselect = true);
	void	Adjust();
	void	Clear(int sidxRow = -1, int sidxCol = -1);
	BOOL	Sort(int nCol, bool ascending);
	void	Blink(int nRow, int nCol, COLORREF color, int interval = 500);
	BOOL	GetCellSize(int fromRow, int endRow, int& colW, int& rowH);
	void	GetCellFromPoint(CPoint pt, int& nRow, int& nCol);
	void	GetTopLeftNonFixedCell(int& nRow, int& nCol);
	void	GetSelectedCellRange(int& minRow, int& maxRow, int& minCol, int& maxCol);
	void	ClearAllRTM();
	void	ClearMerge() { m_mapMerge.RemoveAll(); }
	LONG	MessageToGrid(UINT msg, int scode);	
	BOOL	SubclassWindow(HWND hWnd);
	void	RunExcel(bool visible = true);
	bool	RunExcel_();
	CString	GetCodeType(CString sname);	

	BOOL IsSelectedRow(int nRow);
	void dragScrollGrid(CPoint pt);				//그리드 드래그시 스크롤
	int		GetGridWidth();
// Operations
public:
	void SetSortColumn(int sortColumn) { m_sortColumn = sortColumn; } 
	int	GetRowCount() const                    { return m_nRows; }
	BOOL	SetRowCount(int nRows = 10);
	int	GetColumnCount() const                 { return m_nCols; }
	BOOL	SetColumnCount(int nCols = 10);
	int	GetFixedRowCount() const               { return m_nFixedRows; }
	BOOL	SetFixedRowCount(int nFixedRows = 1);
	int	GetFixedColumnCount() const            { return m_nFixedCols; }
	BOOL	SetFixedColumnCount(int nFixedCols = 1);
	
	
	int	GetRowHeight(int nRow) const;
	BOOL	SetRowHeight(int row, int height);
	int	GetColumnWidth(int nCol) const;
	BOOL	SetColumnWidth(int col, int width);

	int	GetFixedRowHeight() const;
	int	GetFixedColumnWidth() const;
	long	GetVirtualWidth() const;
	long	GetVirtualHeight() const;

	// grid property
	COLORREF GetGridColor()				{ return m_gridCOL; }
	void	SetGridColor(COLORREF color)		{ m_gridCOL = color; }
	void	SetConditionColor(COLORREF upCOL, COLORREF dnCOL, COLORREF eqCOL);
	COLORREF GetUpColor()				{ return m_upCOL; }
	void	SetUpColor(COLORREF color)		{ m_upCOL = color; }
	COLORREF GetEqColor()				{ return m_eqCOL; }
	void	SetEqColor(COLORREF color)		{ m_eqCOL = color; }
	COLORREF GetDnColor()				{ return m_dnCOL; }
	void	SetDnColor(COLORREF color)		{ m_dnCOL = color; }
	COLORREF GetFGSelColor()			{ return m_fgSELCOL; }
	void	SetFGSelColor(COLORREF color)		{ m_fgSELCOL = color; }
	COLORREF GetBKSelColor()			{ return m_bkSELCOL; }
	void	SetBKSelColor(COLORREF color)		{ m_bkSELCOL = color; }
	void	SetBkColor(COLORREF color)		{ m_bkCOL = color; }
	int	GetGridLine()				{ return m_gridLine; }
	void	SetGridLine(int nWhichLine)		{ m_gridLine = nWhichLine; if (::IsWindow(GetSafeHwnd())) Invalidate(); }
	int	GetGridLineType()			{ return m_gridLineType; }
	void	SetGridLineType(int LineType)		{ m_gridLineType = LineType; if (::IsWindow(GetSafeHwnd())) Invalidate(); }
	int	GetGridScroll()				{ return m_NOscroll; }
	void	SetGridScroll(int scroll)		{ m_NOscroll = scroll; if (::IsWindow(GetSafeHwnd())) Adjust(); }
	void	SetGridDIScroll(bool disable)		{ m_DIscroll = disable; }

	void	SetGridFocusLine(BOOL focus, BOOL select) { m_focusLine = focus; m_selectLine = select; }
	void	SetRowColResize(BOOL enRow, BOOL enCol)	{ m_enableRowResize = enRow; m_enableColResize = enCol; }
	void	SetHandleTabKey(BOOL bHandleTab = TRUE)	{ m_handletabkey = bHandleTab; }
	void	SetUseOLE(BOOL bUseOLE = FALSE)		{ m_useOLE = bUseOLE; }
	void	SetTitleTips(BOOL bEnable = TRUE);
	void	SetMouseTrace(bool bTrace = false)	{ m_enableMouseTrace = bTrace; }
	void	SetImageList(CImageList* pList)		{ m_imageList = pList; }
	CIdCell	GetFocusCell() const			{ return m_idCurrent; }
	void	SetSelectRow(int row);
	void	SetFocusCellEdit(int row, int col, bool edit = true);
	void	SetFocusCellSensitive(bool sensitive)	{ m_sensitive = sensitive; }
	void	SetKeepingColumn(int col)		{ m_keepcolumn = col; }		// column no move
	void	SetRoot(CString root) { m_root = root; }
	CString GetRoot() { return m_root; }
	CString GetUser() { return m_user; }
	void	Setuser(CString user) { m_user = user; }

	CString	validAttribute(int row, int col, CString text);
	bool	SetImageListIndex(int nRow, int nCol, int index);
	BOOL	GetItemRect(int nRow, int nCol, LPRECT pRect) const { return GetCellRect(nRow, nCol, pRect); }

	// grid cell setting
	BOOL	SetItem(const GVITEM* gvitem);
	BOOL	GetItem(GVITEM* gvitem);
	BOOL	SetColumnItems(int nCol, GVITEM* gvitem);

	CString  GetItemDrawText   (int nRow, int nCol);
	BOOL	 SetItemText   (int nRow, int nCol, LPCTSTR str);
	CString  GetItemText   (int nRow, int nCol);
	BOOL	 SetItemData   (int nRow, int nCol, LPARAM lParam);
	LPARAM	 GetItemData   (int nRow, int nCol) const;
	BOOL	 SetItemImage  (int nRow, int nCol, int image);
	int	 GetItemImage  (int nRow, int nCol) const;
	BOOL	 SetItemState  (int nRow, int nCol, UINT state);
	UINT	 GetItemState  (int nRow, int nCol) const;
	BOOL	 SetItemAttr   (int nRow, int nCol, UINT attr);
	UINT	 GetItemAttr   (int nRow, int nCol) const;
	BOOL	 SetItemFormat (int nRow, int nCol, UINT format);
	UINT	 GetItemFormat (int nRow, int nCol) const;
	BOOL	 SetItemBkColor(int nRow, int nCol, COLORREF col = CLR_DEFAULT, bool force = false);
	COLORREF GetItemBkColor(int nRow, int nCol) const;
	BOOL	 SetItemFgColor(int nRow, int nCol, COLORREF col = CLR_DEFAULT, bool force = false);
	COLORREF GetItemFgColor(int nRow, int nCol) const;
	BOOL	 SetItemFont   (int nRow, int nCol, LOGFONT* logfont);
	LOGFONT* GetItemFont   (int nRow, int nCol);
	BOOL	 SetItemParam  (int nRow, int nCol, LPARAM param);
	LPARAM	 GetItemParam  (int nRow, int nCol);

	BOOL	SetItemSymbol  (int nRow, int nCol, LPCTSTR str);
	CString	GetItemSymbol  (int nRow, int nCol);
	BOOL	SetItemMisc    (int nRow, int nCol, LPCTSTR str);
	CString	GetItemMisc    (int nRow, int nCol);
	int	GetTopRow() 
	{
		int	nRow = 0, nCol = 0;
		GetTopLeftNonFixedCell(nRow, nCol); 
		return nRow;
	}

	void	ShowLine(int nRow)
	{
		EnsureVisible(nRow, 0);
	}

	// grid row/column insert/delete
	int	InsertColumn(LPCTSTR strHeading, UINT nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE, int nColumn = -1);
	int	InsertRow(LPCTSTR strHeading, int nRow = -1);
	BOOL	InsertRow(int drag, int& drop);
	BOOL	DeleteColumn(int nColumn);
	BOOL	DeleteRow(int nRow);
	BOOL	DeleteAllItems();

	BOOL	AutoSizeRow(int nRow);
	BOOL	AutoSizeColumn(int nCol);
	void	AutoSizeRows();
	void	AutoSizeColumns();
	void	AutoSize();

	void	DrawBong(CDC* pDC, CRect rect, int nRow, int nCol);
	void	DrawSig(CDC* pDC, CRect rect, int nRow);
	int	GetTypeInRect(CRect rect, CPoint pt);
	void	DrawNews(CDC* pDC, CRect rect);
	void	DrawSise(CDC* pDC, CRect rect, bool bChart = false);
	void	DrawExpect(CDC* pDC, CRect rect, CString str);

	// step color
	void	SetStepColor(int unit, COLORREF color1 = 0, COLORREF color2 = 0) { m_stepUnit = unit; m_stepCOL1 = color1; m_stepCOL2 = color2; }

	// ADD PSH 20070911
	void	DrawMarginRatio(CDC *pDC, CRect, int nRow);
	
	void	SetHScroll(UINT nSBCode, UINT nPos);
	void	SetHeader(BOOL bHeadSet = TRUE) { m_bHeadSet = bHeadSet; }
	void	SetEnableSelect(BOOL bEnSelect = TRUE) { m_bEnSelect = bEnSelect; }
	void	SetMarginRatio(BOOL bMRatio = TRUE,  BOOL bErase = TRUE);
	void	SetInfo(BOOL bInfo = TRUE,  BOOL bErase = TRUE);

	void	SetFixBkColor(COLORREF color)		{ m_fixbkCOL = color; }

	void	SortBase(CMapStringToString *pMap = NULL);
	void	SortBase(CMap <CString, LPCTSTR, int, int>& pBaseMap, CMap <CString, LPCTSTR, int, int>& pCurMap);
	void	RowSwitch(int nRow1, int nRow2);

	void	SetBkMarkerColor(COLORREF color)	{ m_clrBkMarkerColor = color; }
	void	SetMarkerColor(COLORREF color)		{ m_clrMarkerColor = color; }
	void	SetMarkerShadow(BOOL bShadow)		{ m_bMkShadow = bShadow; }
	COLORREF	GetBkMarkerColor()	{ return m_clrBkMarkerColor; }
	COLORREF	GetMarkerColor()	{ return m_clrMarkerColor; }
	BOOL	GetMarkerShadow() { return m_bMkShadow; }
	void	SetBmpIcon(CString strFile, int nType);


	int		GetHscroll() { return m_Hscroll; }
	// END ADD
	
// Attributes
protected:
	CGridWnd*   m_pParent;

	CString		m_root;
	CString		m_user;
	bool		m_oleInit;
	int		m_NOscroll;
	bool		m_DIscroll;

	COLORREF	m_fgCOL, m_bkCOL;
	COLORREF	m_fixfgCOL, m_fixbkCOL,m_gridCOL;
	COLORREF	m_winfgCOL, m_3dfaceCOL, m_shadowCOL;
	COLORREF	m_upCOL, m_dnCOL, m_eqCOL;
	COLORREF	m_bkSELCOL, m_fgSELCOL;
	
	// step color
	int		m_stepUnit;
	COLORREF	m_stepCOL1, m_stepCOL2;

	int		m_gridLine;
	int		m_gridLineType;
	BOOL		m_focusLine;
	BOOL		m_selectLine;
	int		m_dragdrop;
	BOOL		m_handletabkey;
	BOOL		m_useOLE;
	BOOL		m_enabletip;
	BOOL		m_sensitive;
	int		m_keepcolumn;
	
	// cell size
	int		m_nRows, m_nFixedRows;
	int		m_nCols, m_nFixedCols;
	int		m_margin;
	int		m_defWidth, m_defHeight;
	int		m_Vscroll, m_Hscroll;
	int		m_baseFixed;

	CUIntArray	m_aryRowHeight, m_aryColWidth;

	LOGFONT		m_logfont;
	CFont		m_font;
	CFont		m_prnfont;
	CImageList*	m_imageList;
	
	CTypedPtrArray	<CObArray, GRID_ROW*> m_aryGridRows;

	// mouse operation
	CPoint		m_ptClick;
	CPoint		m_ptLast;
	CPoint		m_ptMove;
	CIdCell		m_idClick;
	CIdCell		m_idSelect;
	CIdCell		m_idCurrent;
	int		m_timerID;

	// row/column resize
	BOOL		m_enableRowResize;
	BOOL		m_enableColResize;
	int		m_rowCountWheel;

	// selection range
	CMap <DWORD, DWORD, CIdCell, CIdCell&>	m_mapSelCell;
	CMap <DWORD, DWORD, CIdCell, CIdCell&>	m_mapPreCell;
	
	// sorting
	int		m_ascending;
	int		m_sortColumn;
	enum	{ cellNot = 0, cellVal, cellAbs, cellStr};
	CTipDlg*		m_pTooltip;
	CTipDlg*		m_pToolChart;
	CTipDlg*		m_pToolNews;
	// blink (for rtm)
	CCriticalSection	m_sync;
	CMap	<int, int, COLORREF, COLORREF&> m_mapRTM;

	// printing information
	CSize		m_szChar;
	int		m_pgHeight;
	CSize		m_szLogical;
	CSize		m_szPaper;

	// drag & drop
	CIdCell		m_idDrag, m_idDrop;
	CDropTarget	m_dropTarget;
	CWnd*		m_dragWnd;
	CWnd*		m_pMainWnd;
	int		m_mousemode;
	enum mousemode { mouseNOTHING, mouseSELECTALL, mouseSELECTCOL, mouseSELECTROW, mouseSELECTCELLS,
				mouseSCROLLINGCELLS, mouseOVERROWDIVIDE, mouseOVERCOLDIVIDE,
				mouseSIZINGROW,  mouseSIZINGCOL,
				mouseEDITCELL, mouseCHECKCELL, mouseCOMBOCELL, mouseDATECELL, mouseTIMECELL, mouseCODECELL,
				mousePREPAREDRAG, mouseDRAGGING, mousePREPAREMOVE, mouseMOVING};
	// scroll message
	bool		m_noMessage;
	CMap <int, int, _fixMerge, _fixMerge&> m_mapMerge;

	bool		m_bCopy;
	bool		m_enableMouseTrace;

	CWnd*			m_pViewWnd;
	class CMemo*		m_memoWnd;
	CIHCodeCtrl*		m_history;
	CFont*			m_pFont;

	CArray<bool, bool>	m_memoAry;
//	CRect			m_memoRC;
	CRgn			m_memoRGN;
	int			m_updateRow;
	int			m_nClickCol;
	int			m_nClickRow;
	int			m_nSIGRow;
	CFont*			m_pFontS;

	// ADD PSH 20070911
	BOOL		m_bHeadSet;
	BOOL		m_bEnSelect;
	int			m_nVirHScroll;
	int			m_nVirHPos;
	BOOL		m_bVirHPos;
	int			m_nBeforeScrollPos;
	BOOL		m_bMargin;
	CFont		*m_pFontMR;

	COLORREF	m_clrBkMarkerColor;
	COLORREF	m_clrMarkerColor;
	BOOL		m_bMkShadow;

	CBitmap*	m_pBmpMarker;

	BOOL		m_bInfo;
	CBitmap*	m_pBmpInfo1;
	CBitmap*	m_pBmpInfo2;
	CBitmap*	m_pBmpInfo3;
	CBitmap*	m_pBmpInfo4;
	CBitmap*	m_pBmpInfo5;
	CBitmap*	m_pBmpInfo6;
	// END ADD

public:
	bool	m_excelF;
	CString	m_excelS;
	HANDLE	m_excelH;
	CCriticalSection m_syncx;

	COLORREF	m_clrKospi;
	COLORREF	m_clrKosdaq;
	BOOL		m_bMKClrApply;
	
	BOOL		m_isLbtnClick;		//091013 추가 

	// Equalizer ------------------------------------------------
	int m_eq_type;
	int m_eq_cnt;
	BOOL m_eq_alarm[2];					// 0:매도 , 1:매수
	int m_eq_alarm_lev[2];				// 0:매도 , 1:매수
	CString m_eq_alarm_wav[2];			// 0:매도 , 1:매수
	CBitmap eq_bmp[3];					// 0:매도, 1:매수, 2:공백
	double m_eq_lev[MAX_EQUALIZER];		// 체결량 구간 정의
	int m_eq_vol[MAX_EQUALIZER];		// 분할공간 넓이 정의
	COLORREF m_eq_mdcol[MAX_EQUALIZER];	// 매도 색상
	COLORREF m_eq_mscol[MAX_EQUALIZER]; // 매수 색상
	COLORREF m_eq_nmcol[MAX_EQUALIZER]; // 일반 색상
	void	loadEqualizer();		//이퀄라이저 설정 불러오기
	void	MakeEqualizerBmp();		//
	void	DrawEqualizer(CDC *pDC, CRect& rc, int nRow, int nCol);
	int		SearchCurrValStep(double curr);
	int		m_eq_levs[MAX_EQUALIZER][MAX_EQUALIZER];	// 10X10 으로 체결량 구간 미리 정의

	bool	m_isEqualizerField;
	// ----------------------------------------------------------


protected:
	HBITMAP getBitmap(CString path);
	BOOL	GetCellOrigin(int nRow, int nCol, LPPOINT pt) const;
	BOOL	GetCellRect(int nRow, int nCol, LPRECT pRect) const;
	BOOL	GetTextRect(int nRow, int nCol, LPRECT pRect);
	
	CSize	GetTextExtent(LPCTSTR str, BOOL bUseSelectedFont = FALSE);
	void	EnsureVisible(CIdCell &cell)       { EnsureVisible(cell.row, cell.col); }
	void	EnsureVisible(int nRow, int nCol);
	BOOL	IsCellVisible(int nRow, int nCol) const;
	BOOL	IsCellVisible(CIdCell cell) const;
	BOOL	IsCellAttribute(CIdCell& cell, UINT attr) const;

	// draw
	BOOL	RedrawCell(const CIdCell& cell, CDC* pDC = NULL);
	BOOL	RedrawCell(int nRow, int nCol, CDC* pDC = NULL);

	void	Print(); 

	CRangeCell GetCellRange() const;
	CRangeCell GetSelectedCellRange(bool onlyrow = false) const;
	void	SetSelectedRange(const CRangeCell& range, BOOL force = FALSE);
	void	SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol, BOOL force = FALSE);
	void	UpdateRowSelectedRange(int row, bool insert);
	void	UpdateColSelectedRange(int col, bool insert);

	BOOL	IsValid(const CIdCell& cell) const;
	BOOL	IsValid(const CRangeCell& range) const;

	BOOL	SortTextItems(int nCol, BOOL bAscending);
	BOOL	SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data = 0);

protected:
	BOOL	RegisterWindowClass();
	LRESULT MessageToParent(int nRow, int nCol, int nMessage);

	BOOL	InvalidateCellRect(const CIdCell& cell);
	BOOL	InvalidateCellRect(const CRangeCell& range);
	void	EraseBkgnd(CDC* pDC);

	BOOL	GetCellRangeRect(const CRangeCell& range, LPRECT pRect) const;

	CCell*	GetCell(int nRow, int nCol) const;
	BOOL	SetCell(int nRow, int nCol, CCell* pCell);

	BOOL	IsRowResizeArea(CPoint& point) const;
	BOOL	IsColResizeArea(CPoint& point) const;

	CIdCell GetCellFromPoint(CPoint point, BOOL bAllowFixedCellCheck = TRUE) const;
	CIdCell GetTopleftNonFixedCell() const;
	CRangeCell GetUnobstructedNonFixedCellRange() const;
	CRangeCell GetVisibleNonFixedCellRange(LPRECT pRect = NULL) const;

	CIdCell SetFocusCell(CIdCell focusCell);
	CIdCell SetFocusCell(int nRow, int nCol);

	void	ResetSelectedRange();
	int	GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	BOOL	SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);

	BOOL	SortTextItems(int sortcol, BOOL ascending, int srow, int erow);
	BOOL	SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, int low, int high);

	// Mouse Clicks
	virtual void  OnFixedColumnClick(CIdCell& cell);
	virtual void  OnFixedRowClick(CIdCell& cell);

	// Editing
	virtual CSize GetCellExtent(int nRow, int nCol, CDC* pDC);
	virtual bool  OnEditCell(int nRow, int nCol, UINT nChar);
	virtual void  CreateInPlaceEdit(CRect& rect, DWORD dwStyle, UINT nID,
				int nRow, int nCol, LPCTSTR szText, int limit, int nChar);
	virtual void  OnEndEditCell(int nRow, int nCol, CString str);

	// combobox
	virtual bool OnComboCell(int nRow, int nCol);
	virtual void CreateInPlaceCombo(CRect& rect, DWORD dwStyle, UINT nID,
						int nRow, int nCol, LPCTSTR szText, int active);
	virtual void  OnEndComboCell(int nRow, int nCol, CString str, LPARAM lparam);
	// date control
	virtual bool OnDateCell(int nRow, int nCol);
	virtual void CreateInPlaceDate(CRect& rect, UINT nID, LPCTSTR init, int nRow, int nCol, LPCTSTR szText);
	virtual void OnEndDateCell(int nRow, int nCol, CString str);

	// time control
	virtual bool OnTimeCell(int nRow, int nCol);
	virtual void CreateInPlaceTime(CRect& rect, UINT nID, LPCTSTR init, int nRow, int nCol, LPCTSTR szText);
	virtual void OnEndTimeCell(int nRow, int nCol, CString str);

	// Drawing
	virtual void  OnDraw(CDC* pDC);
	virtual BOOL  DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk=FALSE);
	virtual BOOL  DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk=FALSE);

	// Cell Creation and Cleanup
	virtual CCell* CreateCell(int nRow, int nCol);
	virtual void EmptyCell(CCell* pCell, int nRow, int nCol);
	
	// selection cells
	void	SelectAllCells();
	void	SelectColumns(CIdCell current);
	void	SelectRows(CIdCell current);
	void	SelectCells(CIdCell current);
	void	OnSelecting(const CIdCell& current);

	COLORREF DisplayAttribute(int attr, CString& data);
	CString CommaModify(CString string);

	void	drawContrast(CDC *pDC, CRect cellRc, CString sText, COLORREF clr);
	COLORREF GetColumnColor(int row, int color);
	void	SetColorSEQ(COLORREF color1, COLORREF color2);
	double	TOfabs(CString string);

public:


	void	ResetScrollBars();
	void	OnBeginDrag();
	DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	void	OnDragLeave();
	BOOL	OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CintGrid)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);	// ADD PSH 20070914
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	virtual void OnEditCut();
	virtual void OnEditCopy();
	virtual void OnEditPaste();
	virtual void OnEditSelectAll();

	virtual void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnPrint(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);

// Operations
public:
	virtual void PrintColumnHeadings(CDC *pDC, CPrintInfo *pInfo);
	virtual void PrintHeader(CDC *pDC, CPrintInfo *pInfo);
	virtual void PrintFooter(CDC *pDC, CPrintInfo *pInfo);

	virtual CImageList* CreateDragImage(CPoint *pHotSpot);

	virtual void CutSelectedText();
	virtual COleDataSource* CopyTextFromGrid();
	virtual BOOL PasteTextToGrid(CIdCell cell, COleDataObject* pDataObject);

	virtual COleDataSource*    DragRowAction();
	virtual BOOL IsValidDrop(COleDataObject*pDataObject);
	virtual BOOL DropRowAction(COleDataObject* pDataObject);
	virtual BOOL MoveColAction(int sCol, int dCol);
	int GetMarketByCode(CString strCode);

// Implementation
public:
	virtual ~CintGrid();

	void	DestroyCode();
	void	CreateCode(CRect rect, CString szText);
	
	CString Variant(int comm, CString data = "");
public:
	bool m_bOutPos;
	BOOL ParseFromOuter(COleDataObject* pDataObject);
	int m_nAlmRow;
	int hScrolltoleft;
	void	memoReDraw();
	void	memoDraw(CDC* pDC);
	void	memoRefresh();
	bool	memoCheck();
	void	memoDelete(int xrow);
	bool	memoCheck(int xrow);
	void	memoClear(int xrow = -1);
	void	memoDropAction(int drag, int drop);
	void	memoWindow(CString code, CString name, CRect rect);
	void	memoWindowEdit(int row);
	void	delectMemo(int row);

	void	HideCode();
	void	drawIndicator1(CDC* dc, CRect rect, COLORREF clr);
	void	drawIndicator2(CDC* dc, CRect rect, COLORREF clr, CRgn& rgn);
	CFont*	 GetAxFont(CString fName = _T("굴림체"), int point = 9, bool bBold = false, bool bItalic = false);
	CFont*	 GetAxFont(struct _fontR* font);
	CBrush*  GetAxBrush(COLORREF clr);	
	CPen*	GetAxPen(COLORREF clr, int width = 1, int style = PS_SOLID);
	CPen*	GetAxPen(struct _penR* pen);
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CintGrid)
	afx_msg void OnPaint();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSysColorChange();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnSetFont(WPARAM hFont, LPARAM lParam);
	afx_msg LRESULT OnGetFont(WPARAM hFont, LPARAM lParam);
	afx_msg void OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndInPlaceCombo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndInPlaceDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndInPlaceTime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangeInPlaceCombo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg	void OnHistorySelEndOk();
	afx_msg LONG OnManage (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

inline CCell* CintGrid::GetCell(int nRow, int nCol) const
{
	if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
		return NULL;

	GRID_ROW* pRow = m_aryGridRows[nRow];
	if (!pRow) return NULL;
	return pRow->GetAt(nCol);
}

inline BOOL CintGrid::SetCell(int nRow, int nCol, CCell* pCell)
{
	if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
		return FALSE;

	GRID_ROW* pRow = m_aryGridRows[nRow];
	if (!pRow) return FALSE;

	pRow->SetAt(nCol, pCell);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


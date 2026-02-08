#if !defined(AFX_PNWNDGRID_H__AAB5B2F9_6C74_4996_B8A5_3A8C240EBEC1__INCLUDED_)
#define AFX_PNWNDGRID_H__AAB5B2F9_6C74_4996_B8A5_3A8C240EBEC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnWndGrid.h : header file
//

#include "CellRange.h"
#include <afxtempl.h>

#ifdef _WIN32_WCE

#ifdef WCE_NO_CURSOR
#define _WIN32_WCE_NO_CURSOR
#endif

#else

#undef TRY
#undef CATCH
#undef END_CATCH
#define TRY try
#define CATCH(ex_class, ex_object) catch(ex_class* ex_object)
#define END_CATCH
#endif

#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

#define MOUSE_LBUTTON_DOWN	1
#define MOUSE_RBUTTON_DOWN	2
#define MOUSE_LBUTTON_DBLCLK	3

typedef struct _GV_ITEM { 
	int      row,col;     // Row and Column of item
	UINT     mask;        // Mask for use in getting/setting cell data
	UINT     state;       // cell state (focus/hilighted etc)
	UINT     nFormat;     // Format of cell
	CString  szText;      // Text in cell
	int      iImage;      // index of the list view item’s icon
	COLORREF crBkClr;     // Background colour (or CLR_DEFAULT)
	COLORREF crFgClr;     // Forground colour (or CLR_DEFAULT)
	LPARAM   lParam;      // 32-bit value to associate with item 
	LOGFONT  lfFont;      // Cell font
} GV_ITEM; 

// Grid line selection
#define GVL_NONE                0
#define GVL_HORZ                1
#define GVL_VERT                2
#define GVL_BOTH                3

// Cell data mask
#define GVIF_TEXT               LVIF_TEXT
#define GVIF_IMAGE              LVIF_IMAGE
#define GVIF_PARAM              LVIF_PARAM
#define GVIF_STATE              LVIF_STATE
#define GVIF_BKCLR              (GVIF_STATE<<1)
#define GVIF_FGCLR              (GVIF_STATE<<2)
#define GVIF_FORMAT             (GVIF_STATE<<3)
#define GVIF_FONT               (GVIF_STATE<<4)

// Cell states
#define GVIS_FOCUSED            0x0001
#define GVIS_SELECTED           0x0002
#define GVIS_DROPHILITED        0x0004
#define GVIS_READONLY           0x0008
#define GVIS_FIXED              0x0010  // not yet used
#define GVIS_MODIFIED           0x0020

// Cell Searching options
#define GVNI_FOCUSED            0x0001
#define GVNI_SELECTED           0x0002
#define GVNI_DROPHILITED        0x0004
#define GVNI_READONLY           0x0008
#define GVNI_FIXED              0x0010  // not yet used
#define GVNI_MODIFIED           0x0020

#define GVNI_ABOVE              LVNI_ABOVE
#define GVNI_BELOW              LVNI_BELOW
#define GVNI_TOLEFT             LVNI_TOLEFT
#define GVNI_TORIGHT            LVNI_TORIGHT
#define GVNI_ALL                (LVNI_BELOW|LVNI_TORIGHT|LVNI_TOLEFT)
#define GVNI_AREA               (LVNI_BELOW|LVNI_TORIGHT)

// Hit test values (not yet implemented)
#define GVHT_DATA               0x0000
#define GVHT_TOPLEFT            0x0001
#define GVHT_COLHDR             0x0002
#define GVHT_ROWHDR             0x0004
#define GVHT_COLSIZER           0x0008
#define GVHT_ROWSIZER           0x0010
#define GVHT_LEFT               0x0020
#define GVHT_RIGHT              0x0040
#define GVHT_ABOVE              0x0080
#define GVHT_BELOW              0x0100

// Edit Types
#define GVET_NOEDIT             0x0000
#define GVET_CHECKBOX           0x0008

typedef struct tagNM_GRIDVIEW { 
    NMHDR hdr; 
    int   iRow; 
    int   iColumn; 
} NM_GRIDVIEW;

typedef struct tagGV_DISPINFO { 
    NMHDR   hdr; 
    GV_ITEM item; 
} GV_DISPINFO;


class TE_CELLINFO {
public:
    TE_CELLINFO() 
    {
		bOnlyDigital	= FALSE;
		bUseComma		= FALSE;
		nMaxDigitalCount= 20;
    }

	BOOL		bOnlyDigital;
	BOOL		bUseComma;
	int			nMaxDigitalCount;
};

#define	GVN_BEGINDRAG           LVN_BEGINDRAG        // LVN_FIRST-9
#define	GVN_BEGINLABELEDIT      LVN_BEGINLABELEDIT   // LVN_FIRST-5
#define	GVN_BEGINRDRAG          LVN_BEGINRDRAG
#define GVN_COLUMNCLICK         LVN_COLUMNCLICK
#define	GVN_DELETEITEM          LVN_DELETEITEM
#define GVN_ENDLABELEDIT        LVN_ENDLABELEDIT     // LVN_FIRST-6
#define GVN_SELCHANGING         LVN_ITEMCHANGING 
#define GVN_SELCHANGED          LVN_ITEMCHANGED

class CGridCell : public CObject
{
public:
	CGridCell()
	{
		state = 0;
		nFormat = 0;
		szText.Empty();
		iImage = -1;
		crBkClr = CLR_DEFAULT;
		crFgClr = CLR_DEFAULT;
		lParam = 0;
	}
	virtual ~CGridCell() {}
	//{{AFX_VIRTUAL(CGridCell)
	//}}AFX_VIRTUAL

public:
	UINT     state;       // Cell state (selected/focus etc)
	UINT     nFormat;     // Cell format
	CString  szText;      // Cell text (or binary data if you wish...)
	int      iImage;      // Index of the list view item’s icon 
	COLORREF crBkClr;     // Background colour (or CLR_DEFAULT)
	COLORREF crFgClr;     // Forground colour (or CLR_DEFAULT)
	LOGFONT  lfFont;      // Cell font
	LPARAM   lParam;      // 32-bit value to associate with item
};

typedef CTypedPtrArray<CObArray, CGridCell*> GRID_ROW;

class CPnWndGrid;
__declspec(dllexport) void AFXAPI DDX_GridControl(CDataExchange* pDX, int nIDC, CPnWndGrid& rControl);

/////////////////////////////////////////////////////////////////////////////
// CPnWndGrid window

class CPnWndGrid;
__declspec(dllexport) void AFXAPI DDX_GridControl(CDataExchange* pDX, int nIDC, CPnWndGrid& rControl);

class CPnWndGrid : public CWnd
{
	DECLARE_DYNCREATE(CPnWndGrid)

public:
	CPnWndGrid(int nRows = 0, int nCols = 0, int nFixedRows = 0, int nFixedCols = 0);
	virtual ~CPnWndGrid();

	BOOL Create(const RECT& rect, CWnd* parent, UINT nID,
                DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
	BOOL SubclassWindow(HWND hWnd);

public:
	BOOL			m_bForwardDBClick;
	CObArray		m_pCellInfo;
	CPoint			m_nPoint;
	LOGFONT     m_Logfont;

	BOOL SetCellMask(int nColumn, int nMaxDigitalCount, 
			BOOL bOnlyDigital = FALSE, BOOL bUseComma = FALSE);

	void SetForwardDBClick(BOOL bVal);
	void SetParentWnd(HWND *pHwnd);
	TE_CELLINFO GetCellMask(int nColumn);

	BOOL bRowColor;
	BOOL bFixedRowColor;
	COLORREF cOddColor;
	COLORREF cEvenColor;
	COLORREF cOddFixedColor;
	COLORREF cEvenFixedColor;

	void SetRowColor(BOOL bSet, COLORREF cOdd, COLORREF cEven);
	void SetFixedRowColor(BOOL bSet, COLORREF cOdd, COLORREF cEven);
	int  GetRowCount() const                    { return m_nRows; }
	int  GetColumnCount() const                 { return m_nCols; }
	int  GetFixedRowCount() const               { return m_nFixedRows; }
	int  GetFixedColumnCount() const            { return m_nFixedCols; }
	BOOL SetRowCount(int nRows = 10);
	BOOL SetColumnCount(int nCols = 10);
	BOOL SetFixedRowCount(int nFixedRows = 1);
	BOOL SetFixedColumnCount(int nFixedCols = 1);

	int  GetRowHeight(int nRow) const;
	BOOL SetRowHeight(int row, int height);
	int  GetColumnWidth(int nCol) const;
	BOOL SetColumnWidth(int col, int width);
	int  GetColumnType(int nCol) const;
	BOOL SetColumnType(int nCol, int nType);

	BOOL GetCellOrigin(int nRow, int nCol, LPPOINT p) const;
	BOOL GetCellOrigin(const CCellID& cell, LPPOINT p) const;
	BOOL GetCellRect(int nRow, int nCol, LPRECT pRect) const;
	BOOL GetCellRect(const CCellID& cell, LPRECT pRect) const;

	BOOL GetTextRect(const CCellID& cell, LPRECT pRect);
	BOOL GetTextRect(int nRow, int nCol, LPRECT pRect);

	int  GetFixedRowHeight() const;
	int  GetFixedColumnWidth() const;
	long GetVirtualWidth() const;
	long GetVirtualHeight() const;

	void     SetTextColor(COLORREF clr)           { m_crTextColour = clr;             }
	COLORREF GetTextColor() const                 { return m_crTextColour;            }
	void     SetTextBkColor(COLORREF clr)         { m_crTextBkColour = clr;           }
	COLORREF GetTextBkColor() const               { return m_crTextBkColour;          }
	void     SetBkColor(COLORREF clr)             { m_crBkColour = clr;               }
	COLORREF GetBkColor() const                   { return m_crBkColour;              }
	void     SetFixedTextColor(COLORREF clr)      { m_crFixedTextColour = clr;        }
	COLORREF GetFixedTextColor() const            { return m_crFixedTextColour;       }
	void     SetFixedBkColor(COLORREF clr)        { m_crFixedBkColour = clr;          }
	COLORREF GetFixedBkColor() const              { return m_crFixedBkColour;         } 
	void     SetGridColor(COLORREF clr)           { m_crGridColour = clr;             }
	COLORREF GetGridColor() const                 { return m_crGridColour;            }

	int GetSelectedCount() const                  { return m_SelectedCellMap.GetCount(); }
	BOOL IsSelectedItem(int nRow, int nCol);
	BOOL GetSelectedItem(int nNum, int* nRow, int* nCol);

	CCellID GetFocusCell() const                  { return m_idCurrentCell;           }

	void SetImageList(CImageList* pList)          { m_pImageList = pList;             }
	CImageList* GetImageList() const              { return m_pImageList;              }

	void SetGridLines(int nWhichLines = GVL_BOTH) { m_nGridLines = nWhichLines; 
						    if (::IsWindow(GetSafeHwnd())) Invalidate(); }
	int  GetGridLines() const                     { return m_nGridLines;              }

	//void SetEditable(BOOL bEditable = TRUE)       { m_bEditable = bEditable;          }
	//BOOL IsEditable() const                       { return m_bEditable;               }
	void SetListMode(BOOL bEnableListMode = TRUE) { m_bListMode = bEnableListMode;    }
	BOOL GetListMode() const                      { return m_bListMode;               }
	void SetSingleRowSelection(BOOL bSing = TRUE) { m_bSingleRowSelection = bSing;    }
	BOOL GetSingleRowSelection()                  { return m_bSingleRowSelection;     }
	void EnableSelection(BOOL bEnable = TRUE)     { ResetSelectedRange(); m_bEnableSelection = bEnable; ResetSelectedRange(); }
	BOOL IsSelectable() const                     { return m_bEnableSelection;        }
	void SetRowResize(BOOL bResize = TRUE)        { m_bAllowRowResize = bResize;      }
	BOOL GetRowResize() const                     { return m_bAllowRowResize;         }
	void SetColumnResize(BOOL bResize = TRUE)     { m_bAllowColumnResize = bResize;   }
	BOOL GetColumnResize() const                  { return m_bAllowColumnResize;      }
	void SetHeaderSort(BOOL bSortOnClick = TRUE)  { m_bSortOnClick = bSortOnClick;    }
	BOOL GetHeaderSort() const                    { return m_bSortOnClick;            }
	void SetHandleTabKey(BOOL bHandleTab = TRUE)  { m_bHandleTabKey = bHandleTab;     }
	BOOL GetHandleTabKey() const                  { return m_bHandleTabKey;           }
	void SetDoubleBuffering(BOOL bBuffer = TRUE)  { m_bDoubleBuffer = bBuffer;        }
	BOOL GetDoubleBuffering() const               { return m_bDoubleBuffer;           }
	void EnableTitleTips(BOOL bEnable = TRUE)     { m_bTitleTips = bEnable;           }
	BOOL GetTitleTips()                           { return m_bTitleTips;              }

	void SetModified(BOOL bModified = TRUE, int nRow = -1, int nCol = -1);
	BOOL GetModified(int nRow = -1, int nCol = -1);
	BOOL IsCellFixed(int nRow, int nCol);

	BOOL   SetItem(const GV_ITEM* pItem);
	BOOL   GetItem(GV_ITEM* pItem);
	BOOL   SetItemText(int nRow, int nCol, LPCTSTR str);
	virtual CString GetItemText(int nRow, int nCol);
	BOOL   SetItemData(int nRow, int nCol, LPARAM lParam);
	LPARAM GetItemData(int nRow, int nCol) const;
	BOOL   SetItemImage(int nRow, int nCol, int iImage);
	int    GetItemImage(int nRow, int nCol) const;
	BOOL   SetItemState(int nRow, int nCol, UINT state);
	UINT   GetItemState(int nRow, int nCol) const;
	BOOL   SetItemFormat(int nRow, int nCol, UINT nFormat);
	UINT   GetItemFormat(int nRow, int nCol) const;
	BOOL   SetItemBkColour(int nRow, int nCol, COLORREF cr = CLR_DEFAULT);
	COLORREF GetItemBkColour(int nRow, int nCol) const;
	BOOL   SetItemFgColour(int nRow, int nCol, COLORREF cr = CLR_DEFAULT);
	COLORREF GetItemFgColour(int nRow, int nCol) const;
	BOOL SetItemFont(int nRow, int nCol, LOGFONT* lf);
	LOGFONT* GetItemFont(int nRow, int nCol);

	CCellID GetTopleftNonFixedCell() const;

public:
	int  InsertColumn(LPCTSTR strHeading, UINT nFormat = DT_CENTER|DT_VCENTER|DT_NOPREFIX|DT_SINGLELINE,
		      int nColumn = -1, int nType = GVET_NOEDIT);
	int  InsertRow(LPCTSTR strHeading, UINT nFormat = DT_CENTER|DT_VCENTER|DT_NOPREFIX|DT_SINGLELINE, int nRow = -1);
	BOOL DeleteColumn(int nColumn);
	BOOL DeleteRow(int nRow);
	BOOL DeleteNonFixedRows();
	BOOL DeleteAllItems();

	BOOL AutoSizeRow(int nRow);
	BOOL AutoSizeColumn(int nCol);
	void AutoSizeRows();
	void AutoSizeColumns();
	void AutoSize();
	void ExpandColumnsToFit();
	void ExpandRowsToFit();
	void ExpandToFit();

	CSize GetTextExtent(LPCTSTR str, BOOL bUseSelectedFont = TRUE);
	void EnsureVisible(CCellID &cell)       { EnsureVisible(cell.row, cell.col); }
	void EnsureVisible(int nRow, int nCol);
	BOOL IsCellVisible(int nRow, int nCol) const;
	BOOL IsCellVisible(CCellID cell) const;
	//BOOL IsCellEditable(int nRow, int nCol) const;
	//BOOL IsCellEditable(CCellID &cell) const;

	void SetScrollShow(BOOL bHScrollShow=TRUE, BOOL bVScrollShow=TRUE);

	void SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars = FALSE);
	BOOL RedrawCell(int nRow, int nCol, CDC* pDC = NULL);
	BOOL RedrawCell(const CCellID& cell, CDC* pDC = NULL);
	BOOL RedrawRow(int row);
	BOOL RedrawColumn(int col);
	CCellRange GetCellRange() const;
	CCellRange GetSelectedCellRange() const;
	void SetSelectedRange(const CCellRange& Range, BOOL bForceRepaint = FALSE);
	void SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol,
			  BOOL bForceRepaint = FALSE);
	BOOL IsValid(int nRow, int nCol) const;
	BOOL IsValid(const CCellID& cell) const;
	BOOL IsValid(const CCellRange& range) const;
	void	SetFirst();

	CCellID GetNextItem(CCellID& cell, int nFlags) const;

	BOOL SortTextItems(int nCol, BOOL bAscending);
	BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data = 0);

	CCellID SetFocusCell(CCellID cell);
	CCellID SetFocusCell(int nRow, int nCol);
	void SetFocusRowCell(int nRow, int nCol);
	void ResetScrollBars();
	
	//{{AFX_VIRTUAL(CPnWndGrid)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	
	
protected:
	BOOL RegisterWindowClass();
	LRESULT SendMessageToParent(int nRow, int nCol, int nMessage);

	BOOL InvalidateCellRect(const CCellID& cell);
	BOOL InvalidateCellRect(const CCellRange& cellRange);
	void EraseBkgnd(CDC* pDC);

	BOOL GetCellRangeRect(const CCellRange& cellRange, LPRECT lpRect) const;

	CGridCell* GetCell(int nRow, int nCol) const;
	BOOL SetCell(int nRow, int nCol, CGridCell* pCell);

	int  SetMouseMode(int nMode) { int nOldMode = m_MouseMode; m_MouseMode = nMode; return nOldMode; }
	int  GetMouseMode() const    { return m_MouseMode; }

	BOOL MouseOverRowResizeArea(CPoint& point) const;
	BOOL MouseOverColumnResizeArea(CPoint& point) const;

	CCellID GetCellFromPt(CPoint point, BOOL bAllowFixedCellCheck = TRUE) const;
	
	CCellRange GetUnobstructedNonFixedCellRange() const;
	CCellRange GetVisibleNonFixedCellRange(LPRECT pRect = NULL) const;

	void	killFocus();
protected:
	void ResetSelectedRange();
	int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);

	BOOL SortTextItems(int nCol, BOOL bAscending, int low, int high);
	BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, 
		   int low, int high);

	// Mouse Clicks
	virtual void  OnFixedColumnClick(CCellID& cell);
	virtual void  OnFixedRowClick(CCellID& cell);
	virtual CSize GetCellExtent(int nRow, int nCol, CDC* pDC);
	// Drawing
	virtual void  Draw(CDC* pDC);
	virtual BOOL  DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk=FALSE);
	virtual BOOL  DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk=FALSE);

	// GridCell Creation and Cleanup
	virtual CGridCell* CreateCell(int nRow, int nCol);
	virtual void EmptyCell(CGridCell* pCell, int nRow, int nCol);

protected:
	CWnd*		m_pParent;
	COLORREF    m_crTextColour, m_crTextBkColour, m_crBkColour,   // Grid colours
		m_crFixedTextColour, m_crFixedBkColour, m_crGridColour;

	int         m_nGridLines;
	//BOOL        m_bEditable;
	BOOL        m_bModified;
	BOOL        m_bListMode;
	BOOL        m_bSingleRowSelection;
	BOOL        m_bAllowDraw;
	BOOL        m_bEnableSelection;
	BOOL        m_bSortOnClick;
	BOOL        m_bHandleTabKey;
	BOOL        m_bDoubleBuffer;
	BOOL        m_bTitleTips;
	BOOL        m_bTitleTipsCreate;
	UINT		m_uToolTipTimer;
	int			m_nTimerCheck;

	int         m_nRows, m_nFixedRows, m_nCols, m_nFixedCols;
	CUIntArray  m_arRowHeights, m_arColWidths, m_arColType;
	int         m_nMargin;
	int         m_nDefCellWidth, m_nDefCellHeight;
	int         m_nVScrollMax, m_nHScrollMax;
	BOOL		m_bVScrollShow, m_bHScrollShow;

	CFont	m_Font; 
	CImageList* m_pImageList;

	CImageList* m_pCheckImage;
	void DrawCheckImage(CDC* dc,CRect rc,CString sText);

	CTypedPtrArray<CObArray, GRID_ROW*> m_RowData;

	int         m_MouseMode;
	CPoint      m_LeftClickDownPoint, m_LastMousePoint;
	CCellID     m_LeftClickDownCell, m_SelectionStartCell;
	CCellID     m_idCurrentCell;
	int         m_nTimerID;
	int         m_nTimerInterval;
	int         m_nResizeCaptureRange;
	BOOL        m_bAllowRowResize, m_bAllowColumnResize;
	int         m_nRowsPerWheelNotch;
	CMap<DWORD,DWORD, CCellID, CCellID&> m_SelectedCellMap, m_PrevSelectedCellMap;
	CCellID     m_LastDragOverCell;

	// sorting
	int         m_bAscending;
	int         m_SortColumn;

protected:
	void SelectAllCells();
	void SelectColumns(CCellID currentCell);
	void SelectRows(CCellID currentCell);
	void SelectCells(CCellID currentCell);
	void OnSelecting(const CCellID& currentCell);

	//{{AFX_MSG(CPnWndGrid)
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
#endif
	afx_msg LRESULT OnSetFont(WPARAM hFont, LPARAM lParam);
	afx_msg LRESULT OnGetFont(WPARAM hFont, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	enum eMouseModes  { 
		MOUSE_NOTHING, MOUSE_SELECT_ALL, MOUSE_SELECT_COL, MOUSE_SELECT_ROW,
		MOUSE_SELECT_CELLS, MOUSE_SCROLLING_CELLS,
		MOUSE_OVER_ROW_DIVIDE, MOUSE_SIZING_ROW, 
		MOUSE_OVER_COL_DIVIDE, MOUSE_SIZING_COL,
		MOUSE_PREPARE_EDIT
	};
};

inline CGridCell* CPnWndGrid::GetCell(int nRow, int nCol) const
{
	if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
		return NULL;

	GRID_ROW* pRow = m_RowData[nRow];
	if (!pRow)
		return NULL;
	return pRow->GetAt(nCol);
}

inline BOOL CPnWndGrid::SetCell(int nRow, int nCol, CGridCell* pCell)
{
	if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
		return FALSE;

	GRID_ROW* pRow = m_RowData[nRow];
	if (!pRow)
		return FALSE;

	pRow->SetAt(nCol, pCell);
	return TRUE;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNWNDGRID_H__AAB5B2F9_6C74_4996_B8A5_3A8C240EBEC1__INCLUDED_)

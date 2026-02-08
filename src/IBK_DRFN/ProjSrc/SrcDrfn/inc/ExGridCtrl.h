// ExGridCtrl.h: interface for the CExGridCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXGRIDCTRL_H__924B72D4_F1A5_4949_A736_6EA5F23F2435__INCLUDED_)
#define AFX_EXGRIDCTRL_H__924B72D4_F1A5_4949_A736_6EA5F23F2435__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ExGridCtrlDefine.h"

class CLoaderGridCtrl;
class CExGridCtrl : public CWnd  
{
    DECLARE_DYNCREATE(CExGridCtrl)
    friend class CGridCell;
    friend class CGridCellBase;

// Construction
public:
	CLoaderGridCtrl*		m_pLoader;
	long	ActionControl(LPVOID lpAction);

    CExGridCtrl(int nRows = 0, int nCols = 0, int nFixedRows = 0, int nFixedCols = 0);

	UINT	GetMouseScrollLines();
    BOOL Create(RECT& rect, CWnd* parent, UINT nID,
                DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);

///////////////////////////////////////////////////////////////////////////////////
// Attributes
///////////////////////////////////////////////////////////////////////////////////
public:
    int  GetRowCount();
    int  GetColumnCount();
    int  GetFixedRowCount();
    int  GetFixedColumnCount();
    BOOL SetRowCount(int nRows = 10);
    BOOL SetColumnCount(int nCols = 10);
    BOOL SetFixedRowCount(int nFixedRows = 1);
    BOOL SetFixedColumnCount(int nFixedCols = 1);

    int  GetRowHeight(int nRow);
    BOOL SetRowHeight(int row, int height);
    int  GetColumnWidth(int nCol);
    BOOL SetColumnWidth(int col, int width);

    BOOL GetCellOrigin(int nRow, int nCol, LPPOINT p);
    BOOL GetCellOrigin(CCellID& cell, LPPOINT p);
    BOOL GetCellRect(int nRow, int nCol, LPRECT pRect);
    BOOL GetCellRect(CCellID& cell, LPRECT pRect);

    BOOL GetTextRect(CCellID& cell, LPRECT pRect);
    BOOL GetTextRect(int nRow, int nCol, LPRECT pRect);

    CCellID GetCellFromPt(CPoint point, BOOL bAllowFixedCellCheck = TRUE);

    int  GetFixedRowHeight();
    int  GetFixedColumnWidth();
    long GetVirtualWidth();
    long GetVirtualHeight();

    CSize GetTextExtent(int nRow, int nCol, LPCTSTR str);
    // EFW - Get extent of current text in cell
    inline CSize GetCellTextExtent(int nRow, int nCol);

    void     SetGridBkColor(COLORREF clr);
    COLORREF GetGridBkColor();
    void     SetGridLineColor(COLORREF clr);
    COLORREF GetGridLineColor();

	void	 SetTitleTipBackClr(COLORREF clr = CLR_DEFAULT);
	COLORREF GetTitleTipBackClr();
	void	 SetTitleTipTextClr(COLORREF clr = CLR_DEFAULT);
	COLORREF GetTitleTipTextClr();

	// 추가 LYH : fixed cell 의 테두리가 "COLOR_3DDKSHADOW"&"COLOR_3DHIGHLIGHT"이 되는게 싫은 경우에 사용한다.
    void     SetFixedGridLineColor_Light(COLORREF clr);
    void     SetFixedGridLineColor_Dark(COLORREF clr);
    COLORREF GetFixedGridLineColor_Light();
    COLORREF GetFixedGridLineColor_Dark();
	// 추가 LYH : highlight 가 COLOR_HIGHLIGHT(군청색)&COLOR_HIGHLIGHTTEXT(기존text색무시)으로 반전되지 않도록 설정할때 사용한다.
    void     SetHighlightColor(COLORREF clr);
    void     SetHighlightTextColor(COLORREF clr);
    COLORREF GetHighlightColor();
    COLORREF GetHighlightTextColor();
    void     SetUseHighlightText(BOOL bUse); // default : FALSE 로 설정했음.
    BOOL     GetUseHighlightText();
	// 추가 LYH : 기본은 FALSE라서 평상시에는 영향없음. 단, SetFixedRowSelection(TRUE)를 설정해도 click 기능만 되는데, highlight 도 같이 될건지 옵션을 추가함. 20050422 LYH
	void	SetFixedRowSelectionHighlight(BOOL bUse);
	BOOL	GetFixedRowSelectionHighlight();
	void	SetGridScrollBarVisible(BOOL bHScroll, BOOL bVScroll);


    // ***************************************************************************** //
    // These have been deprecated. Use GetDefaultCell and then set the colors
    void     SetTextColor(COLORREF clr);
    COLORREF GetTextColor();
    void     SetTextBkColor(COLORREF clr);
    COLORREF GetTextBkColor();
    void     SetFixedTextColor(COLORREF clr);
	COLORREF GetFixedTextColor();
    void     SetFixedBkColor(COLORREF clr);
    COLORREF GetFixedBkColor();
    void     SetGridColor(COLORREF clr);
    COLORREF GetGridColor();
    void     SetBkColor(COLORREF clr);
    COLORREF GetBkColor();

    void     SetDefCellMargin( int nMargin);
    int      GetDefCellMargin();

    int      GetDefCellHeight();
    void     SetDefCellHeight(int nHeight);
    int      GetDefCellWidth();
    void     SetDefCellWidth(int nWidth);

    // ***************************************************************************** //

    int GetSelectedCount();

    CCellID SetFocusCell(CCellID cell);
    CCellID SetFocusCell(int nRow, int nCol);
    CCellID GetFocusCell();


    void SetVirtualMode(BOOL bVirtual);
    BOOL GetVirtualMode();
    void SetCallbackFunc(GRIDCALLBACK pCallback, 
                         LPARAM lParam);
    GRIDCALLBACK GetCallbackFunc();


    void SetImageList(CImageList* pList);
    CImageList* GetImageList();
    void SetGridLines(int nWhichLines = GVL_BOTH);
    int  GetGridLines();
    void SetEditable(BOOL bEditable = TRUE);
    BOOL IsEditable();
    void SetListMode(BOOL bEnableListMode = TRUE);
    BOOL GetListMode();
    void SetSingleRowSelection(BOOL bSing = TRUE);
    BOOL GetSingleRowSelection();
    void SetSingleColSelection(BOOL bSing = TRUE);
    BOOL GetSingleColSelection();
    void EnableSelection(BOOL bEnable = TRUE);
    BOOL IsSelectable();
    void SetFixedColumnSelection(BOOL bSelect);
    BOOL GetFixedColumnSelection();
    void SetFixedRowSelection(BOOL bSelect);
    BOOL GetFixedRowSelection();
    void EnableDragAndDrop(BOOL bAllow = TRUE);
    BOOL GetDragAndDrop();
    void SetRowResize(BOOL bResize = TRUE);
    BOOL GetRowResize();
    void SetColumnResize(BOOL bResize = TRUE);
    BOOL GetColumnResize();
    void SetHeaderSort(BOOL bSortOnClick = TRUE);
    BOOL GetHeaderSort();
    void SetHandleTabKey(BOOL bHandleTab = TRUE);
    BOOL GetHandleTabKey();
    void SetDoubleBuffering(BOOL bBuffer = TRUE);
    BOOL GetDoubleBuffering();
    void EnableTitleTips(BOOL bEnable = TRUE);
    BOOL GetTitleTips();
    void SetSortColumn(int nCol);
    int  GetSortColumn();
    void SetSortAscending(BOOL bAscending);
    BOOL GetSortAscending();
    void SetTrackFocusCell(BOOL bTrack);
    BOOL GetTrackFocusCell();
    void SetFrameFocusCell(BOOL bFrame);
    BOOL GetFrameFocusCell();
    void SetAutoSizeStyle(int nStyle = GVS_BOTH);
    int  GetAutoSizeStyle();

    void EnableHiddenColUnhide(BOOL bEnable = TRUE);
    BOOL GetHiddenColUnhide();
    void EnableHiddenRowUnhide(BOOL bEnable = TRUE);
    BOOL GetHiddenRowUnhide();

    void EnableColumnHide(BOOL bEnable = TRUE);
    BOOL GetColumnHide();
    void EnableRowHide(BOOL bEnable = TRUE);
    BOOL GetRowHide();

	// cell 생성할때 default 속성에 readonly 를 넣을지 여부 LYH 20050418
	// editable 을 설정하면 inplaceedit 가 기본으로 실행되는데, checkbox 등을 위해서 막는다.
    void SetDefaultReadOnly(BOOL bReadOnly = TRUE);
    BOOL IsDefaultReadOnly();

	void SetUseFocusRect(BOOL bUse);		// 포커스를 그릴것인지 ojtaso
	BOOL GetUseFocusRect();					// 포커스를 그릴것인지 ojtaso

	void	SetDrawFixedLine(BOOL bDraw);

///////////////////////////////////////////////////////////////////////////////////
// default Grid cells. Use these for setting default values such as colors and fonts
///////////////////////////////////////////////////////////////////////////////////
public:
    CGridCellBase* GetDefaultCell(BOOL bFixedRow, BOOL bFixedCol);

///////////////////////////////////////////////////////////////////////////////////
// Grid cell Attributes
///////////////////////////////////////////////////////////////////////////////////
public:
    CGridCellBase* GetCell(int nRow, int nCol);   // Get the actual cell!

    void SetModified(BOOL bModified = TRUE, int nRow = -1, int nCol = -1);
    BOOL GetModified(int nRow = -1, int nCol = -1);
    BOOL IsCellFixed(int nRow, int nCol);

    BOOL   SetItem(GV_ITEM* pItem);
    BOOL   GetItem(GV_ITEM* pItem);
    BOOL   SetItemText(int nRow, int nCol, LPCTSTR str);
    // The following was virtual. If you want to override, use 
    //  CGridCellBase-derived class's GetText() to accomplish same thing
    CString GetItemText(int nRow, int nCol);

    // EFW - 06/13/99 - Added to support printf-style formatting codes.
    // Also supports use with a string resource ID
#if !defined(_WIN32_WCE) || (_WIN32_WCE >= 210)
    BOOL   SetItemTextFmt(int nRow, int nCol, LPCTSTR szFmt, ...);
    BOOL   SetItemTextFmtID(int nRow, int nCol, UINT nID, ...);
#endif

    BOOL   SetItemData(int nRow, int nCol, LPARAM lParam);
    LPARAM GetItemData(int nRow, int nCol);
    BOOL   SetItemImage(int nRow, int nCol, int iImage);
    int    GetItemImage(int nRow, int nCol);
    BOOL   SetItemState(int nRow, int nCol, UINT state);
    UINT   GetItemState(int nRow, int nCol);
    BOOL   SetItemFormat(int nRow, int nCol, UINT nFormat);
    UINT   GetItemFormat(int nRow, int nCol);
    BOOL   SetItemBkColour(int nRow, int nCol, COLORREF cr = CLR_DEFAULT);
    COLORREF GetItemBkColour(int nRow, int nCol);
    BOOL   SetItemFgColour(int nRow, int nCol, COLORREF cr = CLR_DEFAULT);
    COLORREF GetItemFgColour(int nRow, int nCol);
    BOOL SetItemFont(int nRow, int nCol, LOGFONT* lf);
    LOGFONT* GetItemFont(int nRow, int nCol);

    BOOL IsItemEditing(int nRow, int nCol);

    BOOL SetCellType(int nRow, int nCol, CRuntimeClass* pRuntimeClass);
    BOOL SetDefaultCellType(CRuntimeClass* pRuntimeClass);

///////////////////////////////////////////////////////////////////////////////////
// Operations
///////////////////////////////////////////////////////////////////////////////////
public:
    int  InsertColumn(LPCTSTR strHeading, UINT nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE, int nColumn = -1);
    int  InsertRow(LPCTSTR strHeading, int nRow = -1);
    BOOL DeleteColumn(int nColumn);
    BOOL DeleteRow(int nRow);
    BOOL DeleteNonFixedRows();
    BOOL DeleteAllItems();

	void ClearCells(CCellRange Selection);

    BOOL AutoSizeRow(int nRow, BOOL bResetScroll = TRUE);
    BOOL AutoSizeColumn(int nCol, UINT nAutoSizeStyle = GVS_DEFAULT, BOOL bResetScroll = TRUE);
    void AutoSizeRows();
    void AutoSizeColumns(UINT nAutoSizeStyle = GVS_DEFAULT);
    void AutoSize(UINT nAutoSizeStyle = GVS_DEFAULT);
    void ExpandColumnsToFit(BOOL bExpandFixed = TRUE);
    void ExpandLastColumn();
    void ExpandRowsToFit(BOOL bExpandFixed = TRUE);
    void ExpandToFit(BOOL bExpandFixed = TRUE);

    void Refresh();
    void AutoFill();   // Fill grid with blank cells

    void EnsureVisible(CCellID &cell);
    void EnsureVisible(int nRow, int nCol);
    BOOL IsCellVisible(int nRow, int nCol);
    BOOL IsCellVisible(CCellID cell);
    BOOL IsCellEditable(int nRow, int nCol);
    BOOL IsCellEditable(CCellID &cell);
    BOOL IsCellSelected(int nRow, int nCol);
    BOOL IsCellSelected(CCellID &cell);

    // SetRedraw stops/starts redraws on things like changing the # rows/columns
    // and autosizing, but not for user-intervention such as resizes
    void SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars = FALSE);
    BOOL RedrawCell(int nRow, int nCol, CDC* pDC = NULL);
    BOOL RedrawCell(CCellID& cell, CDC* pDC = NULL);
    BOOL RedrawRow(int row);
    BOOL RedrawColumn(int col);

#ifndef _WIN32_WCE
    BOOL Save(LPCTSTR filename, TCHAR chSeparator = _T(','));
    BOOL Load(LPCTSTR filename, TCHAR chSeparator = _T(','));
#endif

///////////////////////////////////////////////////////////////////////////////////
// Cell Ranges
///////////////////////////////////////////////////////////////////////////////////
 public:
    CCellRange GetCellRange();
    CCellRange GetSelectedCellRange();
    void SetSelectedRange(CCellRange& Range, BOOL bForceRepaint = FALSE, BOOL bSelectCells = TRUE);
    void SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol,
                          BOOL bForceRepaint = FALSE, BOOL bSelectCells = TRUE);
    BOOL IsValid(int nRow, int nCol);
    BOOL IsValid(CCellID& cell);
    BOOL IsValid(CCellRange& range);

///////////////////////////////////////////////////////////////////////////////////
// Clipboard, drag and drop, and cut n' paste operations
///////////////////////////////////////////////////////////////////////////////////
#ifndef GRIDCONTROL_NO_CLIPBOARD
    virtual void CutSelectedText();
    virtual COleDataSource* CopyTextFromGrid();
    virtual BOOL PasteTextToGrid(CCellID cell, COleDataObject* pDataObject, BOOL bSelectPastedCells=TRUE);
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP
 public:
    virtual void OnBeginDrag();
    virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    virtual void OnDragLeave();
    virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
    virtual void OnEditCut();
    virtual void OnEditCopy();
    virtual void OnEditPaste();
#endif
    virtual void OnEditSelectAll();

///////////////////////////////////////////////////////////////////////////////////
// Misc.
///////////////////////////////////////////////////////////////////////////////////
public:
    CCellID GetNextItem(CCellID& cell, int nFlags);

	BOOL SortItems(int nCol, BOOL bAscending, LPARAM data = 0);
    BOOL SortTextItems(int nCol, BOOL bAscending, LPARAM data = 0);
    BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data = 0);

	void SetCompareFunction(PFNLVCOMPARE pfnCompare);

///////////////////////////////////////////////////////////////////////////////////
// Printing
///////////////////////////////////////////////////////////////////////////////////
#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
public:
    void Print(CPrintDialog* pPrntDialog = NULL);

    // EFW - New printing support functions
    void EnableWysiwygPrinting(BOOL bEnable = TRUE);
    BOOL GetWysiwygPrinting();

    void SetShadedPrintOut(BOOL bEnable = TRUE);
    BOOL GetShadedPrintOut(void);

    // Use -1 to have it keep the existing value
    void SetPrintMarginInfo(int nHeaderHeight, int nFooterHeight,
        int nLeftMargin, int nRightMargin, int nTopMargin,
        int nBottomMargin, int nGap);

    void GetPrintMarginInfo(int &nHeaderHeight, int &nFooterHeight,
        int &nLeftMargin, int &nRightMargin, int &nTopMargin,
        int &nBottomMargin, int &nGap);

#endif // #if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)

// Implementation
public:
    virtual ~CExGridCtrl();

protected:
    BOOL RegisterWindowClass();
    BOOL Initialise();
    void SetupDefaultCells();

    LRESULT SendMessageToParent(int nRow, int nCol, int nMessage);
    LRESULT SendDisplayRequestToParent(GV_DISPINFO* pDisplayInfo);
    LRESULT SendCacheHintToParent(CCellRange& range);

    BOOL InvalidateCellRect(int row, int col);
    BOOL InvalidateCellRect(CCellID& cell);
    BOOL InvalidateCellRect(CCellRange& cellRange);
    void EraseBkgnd(CDC* pDC);

    BOOL GetCellRangeRect(CCellRange& cellRange, LPRECT lpRect);

    BOOL SetCell(int nRow, int nCol, CGridCellBase* pCell);

    int  SetMouseMode(int nMode);
    int  GetMouseMode();

    BOOL MouseOverRowResizeArea(CPoint& point);
    BOOL MouseOverColumnResizeArea(CPoint& point);

    CCellID GetTopleftNonFixedCell(BOOL bForceRecalculation = FALSE);
    CCellRange GetUnobstructedNonFixedCellRange(BOOL bForceRecalculation = FALSE);
    CCellRange GetVisibleNonFixedCellRange(LPRECT pRect = NULL, BOOL bForceRecalculation = FALSE);

    BOOL IsVisibleVScroll();
    BOOL IsVisibleHScroll();
    void ResetSelectedRange();
    void ResetScrollBars();
    void EnableScrollBars(int nBar, BOOL bEnable = TRUE);
    int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
    BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);

    BOOL SortTextItems(int nCol, BOOL bAscending, int low, int high);
    BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data,
                   int low, int high);

    CPoint GetPointClicked(int nRow, int nCol, CPoint& point);

	void ValidateAndModifyCellContents(int nRow, int nCol, LPCTSTR strText);
// Overrrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExGridCtrl)
    protected:
    virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

protected:
#ifndef GRIDCONTROL_NO_DRAGDROP
    // Drag n' drop
    virtual CImageList* CreateDragImage(CPoint *pHotSpot);    // no longer necessary
#endif

    // Mouse Clicks
    virtual void  OnFixedColumnClick(CCellID& cell);
    virtual void  OnFixedRowClick(CCellID& cell);

    // Editing
    virtual void  OnEditCell(int nRow, int nCol, CPoint point, UINT nChar);
    virtual void  OnEndEditCell(int nRow, int nCol, CString str);
	virtual BOOL  ValidateEdit(int nRow, int nCol, LPCTSTR str);
    virtual void  EndEditing();

    // Drawing
    virtual void  OnDraw(CDC* pDC);

    // CGridCellBase Creation and Cleanup
    virtual CGridCellBase* CreateCell(int nRow, int nCol);
    virtual void DestroyCell(int nRow, int nCol);

protected:
    void SelectAllCells();
    void SelectColumns(CCellID currentCell, BOOL bForceRedraw=FALSE, BOOL bSelectCells=TRUE);
    void SelectRows(CCellID currentCell, BOOL bForceRedraw=FALSE, BOOL bSelectCells=TRUE);
    void SelectCells(CCellID currentCell, BOOL bForceRedraw=FALSE, BOOL bSelectCells=TRUE);
    void OnSelecting(CCellID& currentCell);

    // Generated message map functions
    //{{AFX_MSG(CExGridCtrl)
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
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
    //}}AFX_MSG
#ifndef _WIN32_WCE_NO_CURSOR
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
#endif
#ifndef _WIN32_WCE
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);    // EFW - Added
    afx_msg void OnSysColorChange();
#endif
#ifndef _WIN32_WCE_NO_CURSOR
    afx_msg void OnCaptureChanged(CWnd *pWnd);
#endif
#ifndef GRIDCONTROL_NO_CLIPBOARD
    afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
#endif
#if (_MFC_VER >= 0x0421) || (_WIN32_WCE >= 210)
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
#endif
#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
#endif
    afx_msg LRESULT OnSetFont(WPARAM hFont, LPARAM lParam);
    afx_msg LRESULT OnGetFont(WPARAM hFont, LPARAM lParam);
    afx_msg LRESULT OnImeChar(WPARAM wCharCode, LPARAM lParam);
    afx_msg void OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()

    enum eMouseModes { MOUSE_NOTHING, MOUSE_SELECT_ALL, MOUSE_SELECT_COL, MOUSE_SELECT_ROW,
                       MOUSE_SELECT_CELLS, MOUSE_SCROLLING_CELLS,
                       MOUSE_OVER_ROW_DIVIDE, MOUSE_SIZING_ROW,
                       MOUSE_OVER_COL_DIVIDE, MOUSE_SIZING_COL,
                       MOUSE_PREPARE_EDIT,
#ifndef GRIDCONTROL_NO_DRAGDROP
                       MOUSE_PREPARE_DRAG, MOUSE_DRAGGING
#endif
    };
};

#endif // !defined(AFX_EXGRIDCTRL_H__924B72D4_F1A5_4949_A736_6EA5F23F2435__INCLUDED_)

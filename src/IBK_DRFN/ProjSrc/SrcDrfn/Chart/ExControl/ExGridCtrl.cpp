// ExGridCtrl.cpp: implementation of the CExGridCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Inc/ExGridCtrl.h"
#include "Include/GirdLoadHelper.h"

#include "ExGridCtrl/MemDC.h"

// OLE stuff for clipboard operations
#include <afxadv.h>            // For CSharedFile
#include <afxconv.h>           // For LPTSTR -> LPSTR macros

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Spit out some messages as a sanity check for programmers
#ifdef GRIDCONTROL_NO_TITLETIPS
#pragma message(" -- CExGridCtrl: No titletips for cells with large data")
#endif
#ifdef GRIDCONTROL_NO_DRAGDROP
#pragma message(" -- CExGridCtrl: No OLE drag and drop")
#endif
#ifdef GRIDCONTROL_NO_CLIPBOARD
#pragma message(" -- CExGridCtrl: No clipboard support")
#endif
#ifdef GRIDCONTROL_NO_PRINTING
#pragma message(" -- CExGridCtrl: No printing support")
#endif


IMPLEMENT_DYNCREATE(CExGridCtrl, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl

CExGridCtrl::CExGridCtrl(int nRows, int nCols, int nFixedRows, int nFixedCols)
{
	m_pLoader = new CLoaderGridCtrl(this);

    RegisterWindowClass();
	
    SetupDefaultCells();
    SetGridBkColor(::GetSysColor(COLOR_3DSHADOW));

    // Set up the initial grid size
    SetRowCount(nRows);
    SetColumnCount(nCols);
    SetFixedRowCount(nFixedRows);
    SetFixedColumnCount(nFixedCols);

    SetTitleTipTextClr(CLR_DEFAULT);  //FNA
    SetTitleTipBackClr(CLR_DEFAULT); 
}

CExGridCtrl::~CExGridCtrl()
{
    DeleteAllItems();

    DestroyWindow();

#if !defined(GRIDCONTROL_NO_DRAGDROP) || !defined(GRIDCONTROL_NO_CLIPBOARD)
    // BUG FIX - EFW
    COleDataSource *pSource = COleDataSource::GetClipboardOwner();
    if(pSource)
        COleDataSource::FlushClipboard();
#endif

	delete m_pLoader;
}

BEGIN_MESSAGE_MAP(CExGridCtrl, CWnd)
//EFW - Added ON_WM_RBUTTONUP
//{{AFX_MSG_MAP(CExGridCtrl)
    ON_WM_PAINT()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_SIZE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
    ON_WM_GETDLGCODE()
    ON_WM_KEYDOWN()
    ON_WM_CHAR()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
    ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
    ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
    ON_WM_SYSKEYDOWN()
	ON_WM_CONTEXTMENU()
//}}AFX_MSG_MAP
#ifndef _WIN32_WCE_NO_CURSOR
    ON_WM_SETCURSOR()
#endif
#ifndef _WIN32_WCE
    ON_WM_RBUTTONUP()
    ON_WM_SYSCOLORCHANGE()
    ON_WM_CAPTURECHANGED()
#endif
#ifndef GRIDCONTROL_NO_CLIPBOARD
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
    ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
#endif
#if (_WIN32_WCE >= 210)
    ON_WM_SETTINGCHANGE()
#endif
#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
    ON_WM_MOUSEWHEEL()
#endif
    ON_MESSAGE(WM_SETFONT, OnSetFont)
    ON_MESSAGE(WM_GETFONT, OnGetFont)
    ON_MESSAGE(WM_IME_CHAR, OnImeChar)
    ON_NOTIFY(GVN_ENDLABELEDIT, IDC_INPLACE_CONTROL, OnEndInPlaceEdit)
END_MESSAGE_MAP()

long CExGridCtrl::ActionControl(LPVOID lpAction)
{
	return m_pLoader->ActionControl(lpAction);
}

UINT CExGridCtrl::GetMouseScrollLines()
{
	return m_pLoader->GetMouseScrollLines();
}

int CExGridCtrl::GetFixedColumnCount()
{
	return m_pLoader->GetFixedColumnCount();
}

void CExGridCtrl::SetGridBkColor(COLORREF clr)
{
	m_pLoader->SetGridBkColor( clr);
}

int CExGridCtrl::GetRowCount()
{
	return m_pLoader->GetRowCount();
}

int CExGridCtrl::GetColumnCount()
{
	return m_pLoader->GetColumnCount();
}

int CExGridCtrl::GetFixedRowCount()
{
	return m_pLoader->GetFixedRowCount();
}

COLORREF CExGridCtrl::GetGridBkColor()
{
	return m_pLoader->GetGridBkColor();
}

void CExGridCtrl::SetGridLineColor(COLORREF clr)
{
	m_pLoader->SetGridLineColor(clr);
}

COLORREF CExGridCtrl::GetGridLineColor()
{
	return m_pLoader->GetGridLineColor();
}

CSize CExGridCtrl::GetCellTextExtent(int nRow, int nCol)
{
	return m_pLoader->GetCellTextExtent(nRow, nCol);
}

void CExGridCtrl::SetTitleTipBackClr(COLORREF clr)
{
	m_pLoader->SetTitleTipBackClr(clr);
}

COLORREF CExGridCtrl::GetTitleTipBackClr()
{
	return m_pLoader->GetTitleTipBackClr();
}

void CExGridCtrl::SetTitleTipTextClr(COLORREF clr)
{
	m_pLoader->SetTitleTipTextClr(clr);
}

COLORREF CExGridCtrl::GetTitleTipTextClr()
{
	return m_pLoader->GetTitleTipTextClr();
}

void CExGridCtrl::SetFixedGridLineColor_Light(COLORREF clr)
{
	m_pLoader->SetFixedGridLineColor_Light(clr);
}

void CExGridCtrl::SetFixedGridLineColor_Dark(COLORREF clr)
{
	m_pLoader->SetFixedGridLineColor_Dark(clr);
}

COLORREF CExGridCtrl::GetFixedGridLineColor_Light()
{
	return m_pLoader->GetFixedGridLineColor_Light();
}

COLORREF CExGridCtrl::GetFixedGridLineColor_Dark()
{
	return m_pLoader->GetFixedGridLineColor_Dark();
}

void CExGridCtrl::SetHighlightColor(COLORREF clr)
{
	m_pLoader->SetHighlightColor(clr);
}

void CExGridCtrl::SetHighlightTextColor(COLORREF clr)
{
	m_pLoader->SetHighlightTextColor(clr);
}

COLORREF CExGridCtrl::GetHighlightColor()
{
	return m_pLoader->GetHighlightColor();
}

COLORREF CExGridCtrl::GetHighlightTextColor()
{
	return m_pLoader->GetHighlightTextColor();
}

void CExGridCtrl::SetUseHighlightText(BOOL bUse)
{
	m_pLoader->SetUseHighlightText(bUse);
} 

BOOL CExGridCtrl::GetUseHighlightText()
{
	return m_pLoader->GetUseHighlightText();
}

void CExGridCtrl::SetFixedRowSelectionHighlight(BOOL bUse)
{
	m_pLoader->SetFixedRowSelectionHighlight(bUse);
}

BOOL CExGridCtrl::GetFixedRowSelectionHighlight()
{
	return m_pLoader->GetFixedRowSelectionHighlight();
}

void CExGridCtrl::SetTextColor(COLORREF clr)
{
	m_pLoader->SetTextColor(clr);
}

COLORREF CExGridCtrl::GetTextColor()
{
	return m_pLoader->GetTextColor();
}

void CExGridCtrl::SetTextBkColor(COLORREF clr)
{
	m_pLoader->SetTextBkColor(clr);
}

COLORREF CExGridCtrl::GetTextBkColor()
{
	return m_pLoader->GetTextBkColor();
}

void CExGridCtrl::SetFixedTextColor(COLORREF clr)
{
	m_pLoader->SetFixedTextColor(clr);
}

COLORREF CExGridCtrl::GetFixedTextColor()
{
	return m_pLoader->GetFixedTextColor();
}

void CExGridCtrl::SetFixedBkColor(COLORREF clr)
{
	m_pLoader->SetFixedBkColor(clr);
}

void CExGridCtrl::SetGridColor(COLORREF clr)
{
	m_pLoader->SetGridColor(clr);
}

COLORREF CExGridCtrl::GetGridColor()
{
	return m_pLoader->GetGridColor();
}

void CExGridCtrl::SetBkColor(COLORREF clr)
{
	m_pLoader->SetBkColor(clr);
}

COLORREF CExGridCtrl::GetBkColor()
{
	return m_pLoader->GetBkColor();
}

void CExGridCtrl::SetDefCellMargin(int nMargin)
{
	m_pLoader->SetDefCellMargin(nMargin);
}

int CExGridCtrl::GetDefCellMargin()
{
	return m_pLoader->GetDefCellMargin();
}

int CExGridCtrl::GetDefCellHeight()
{
	return m_pLoader->GetDefCellHeight();
}

void CExGridCtrl::SetDefCellHeight(int nHeight)
{
	m_pLoader->SetDefCellHeight(nHeight);
}

int CExGridCtrl::GetDefCellWidth()
{
	return m_pLoader->GetDefCellWidth();
}

void CExGridCtrl::SetDefCellWidth(int nWidth)
{
	m_pLoader->SetDefCellWidth(nWidth);
}

int CExGridCtrl::GetSelectedCount()
{
	return m_pLoader->GetSelectedCount();
}

CCellID CExGridCtrl::GetFocusCell()
{
	return m_pLoader->GetFocusCell();
}

BOOL CExGridCtrl::GetVirtualMode()
{
	return m_pLoader->GetVirtualMode();
}

void CExGridCtrl::SetCallbackFunc(GRIDCALLBACK pCallback, LPARAM lParam)
{
	m_pLoader->SetCallbackFunc(pCallback, lParam);
}

GRIDCALLBACK CExGridCtrl::GetCallbackFunc()
{
	return m_pLoader->GetCallbackFunc();
}

void CExGridCtrl::SetImageList(CImageList* pList)
{
	m_pLoader->SetImageList(pList);
}

CImageList* CExGridCtrl::GetImageList()
{
	return m_pLoader->GetImageList();
}

int CExGridCtrl::GetGridLines()
{
	return m_pLoader->GetGridLines();
}

void CExGridCtrl::SetEditable(BOOL bEditable)
{
	m_pLoader->SetEditable(bEditable);
}

BOOL CExGridCtrl::IsEditable()
{
	return m_pLoader->IsEditable();
}

BOOL CExGridCtrl::GetListMode()
{
	return m_pLoader->GetListMode();
}

void CExGridCtrl::SetSingleRowSelection(BOOL bSing)
{
	m_pLoader->SetSingleRowSelection(bSing);
}

BOOL CExGridCtrl::GetSingleRowSelection()
{
	return m_pLoader->GetSingleRowSelection();
}

void CExGridCtrl::SetSingleColSelection(BOOL bSing)
{
	m_pLoader->SetSingleColSelection(bSing);
}

BOOL CExGridCtrl::GetSingleColSelection()
{
	return m_pLoader->GetSingleColSelection();
}

void CExGridCtrl::EnableSelection(BOOL bEnable)
{
	m_pLoader->EnableSelection(bEnable);
}

BOOL CExGridCtrl::IsSelectable()
{
	return m_pLoader->IsSelectable();
}

void CExGridCtrl::SetFixedColumnSelection(BOOL bSelect)
{
	m_pLoader->SetFixedColumnSelection(bSelect);
}

BOOL CExGridCtrl::GetFixedColumnSelection()
{
	return m_pLoader->GetFixedColumnSelection();
}

void CExGridCtrl::SetFixedRowSelection(BOOL bSelect)
{
	m_pLoader->SetFixedRowSelection(bSelect);
}

BOOL CExGridCtrl::GetFixedRowSelection()
{
	return m_pLoader->GetFixedRowSelection();
}

void CExGridCtrl::EnableDragAndDrop(BOOL bAllow)
{
	m_pLoader->EnableDragAndDrop(bAllow);
}

BOOL CExGridCtrl::GetDragAndDrop()
{
	return m_pLoader->GetDragAndDrop();
}

void CExGridCtrl::SetRowResize(BOOL bResize)
{
	m_pLoader->SetRowResize(bResize);
}

BOOL CExGridCtrl::GetRowResize()
{
	return m_pLoader->GetRowResize();
}

void CExGridCtrl::SetColumnResize(BOOL bResize)
{
	m_pLoader->SetColumnResize(bResize);
}

BOOL CExGridCtrl::GetColumnResize()
{
	return m_pLoader->GetColumnResize();
}

void CExGridCtrl::SetHeaderSort(BOOL bSortOnClick)
{
	m_pLoader->SetHeaderSort(bSortOnClick);
}

BOOL CExGridCtrl::GetHeaderSort()
{
	return m_pLoader->GetHeaderSort();
}

void CExGridCtrl::SetHandleTabKey(BOOL bHandleTab)
{
	m_pLoader->SetHandleTabKey(bHandleTab);
}

BOOL CExGridCtrl::GetHandleTabKey()
{
	return m_pLoader->GetHandleTabKey();
}

void CExGridCtrl::SetDoubleBuffering(BOOL bBuffer)
{
	m_pLoader->SetDoubleBuffering(bBuffer);
}

BOOL CExGridCtrl::GetDoubleBuffering()
{
	return m_pLoader->GetDoubleBuffering();
}

void CExGridCtrl::EnableTitleTips(BOOL bEnable)
{
	m_pLoader->EnableTitleTips(bEnable);
}

BOOL CExGridCtrl::GetTitleTips()
{
	return m_pLoader->GetTitleTips();
}

int CExGridCtrl::GetSortColumn()
{
	return m_pLoader->GetSortColumn();
}

void CExGridCtrl::SetSortAscending(BOOL bAscending)
{
	m_pLoader->SetSortAscending(bAscending);
}

BOOL CExGridCtrl::GetSortAscending()
{
	return m_pLoader->GetSortAscending();
}

void CExGridCtrl::SetTrackFocusCell(BOOL bTrack)
{
	m_pLoader->SetTrackFocusCell(bTrack);
}

BOOL CExGridCtrl::GetTrackFocusCell()
{
	return m_pLoader->GetTrackFocusCell();
}

void CExGridCtrl::SetFrameFocusCell(BOOL bFrame)
{
	m_pLoader->SetFrameFocusCell(bFrame);
}

BOOL CExGridCtrl::GetFrameFocusCell()
{
	return m_pLoader->GetFrameFocusCell();
}

void CExGridCtrl::SetAutoSizeStyle(int nStyle)
{
	m_pLoader->SetAutoSizeStyle(nStyle);
}

int CExGridCtrl::GetAutoSizeStyle()
{
	return m_pLoader->GetAutoSizeStyle();
}

void CExGridCtrl::EnableHiddenColUnhide(BOOL bEnable)
{
	m_pLoader->EnableHiddenColUnhide(bEnable);
}

BOOL CExGridCtrl::GetHiddenColUnhide()
{
	return m_pLoader->GetHiddenColUnhide();
}

void CExGridCtrl::EnableHiddenRowUnhide(BOOL bEnable)
{
	m_pLoader->EnableHiddenRowUnhide(bEnable);
}

BOOL CExGridCtrl::GetHiddenRowUnhide()
{
	return m_pLoader->GetHiddenRowUnhide();
}

void CExGridCtrl::EnableColumnHide(BOOL bEnable)
{
	m_pLoader->EnableColumnHide(bEnable);
}

BOOL CExGridCtrl::GetColumnHide()
{
	return m_pLoader->GetColumnHide();
}

void CExGridCtrl::EnableRowHide(BOOL bEnable)
{
	m_pLoader->EnableRowHide(bEnable);
}

BOOL CExGridCtrl::GetRowHide()
{
	return m_pLoader->GetRowHide();
}

void CExGridCtrl::SetDefaultReadOnly(BOOL bReadOnly)
{
	m_pLoader->SetDefaultReadOnly(bReadOnly);
}

BOOL CExGridCtrl::IsDefaultReadOnly()
{
	return m_pLoader->IsDefaultReadOnly();
}

void CExGridCtrl::EnsureVisible(CCellID &cell)
{
	m_pLoader->EnsureVisible(cell);
}

int CExGridCtrl::SetMouseMode(int nMode)
{
	return m_pLoader->SetMouseMode(nMode);
}

int CExGridCtrl::GetMouseMode()
{
	return m_pLoader->GetMouseMode();
}

BOOL CExGridCtrl::IsVisibleVScroll()
{
	return m_pLoader->IsVisibleVScroll();
} 

BOOL CExGridCtrl::IsVisibleHScroll()
{
	return m_pLoader->IsVisibleHScroll();
}

BOOL CExGridCtrl::RegisterWindowClass()
{
	return m_pLoader->RegisterWindowClass();
}

BOOL CExGridCtrl::Initialise()
{
	return m_pLoader->Initialise();
}

// creates the control - use like any other window create control
BOOL CExGridCtrl::Create(RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!CWnd::Create(GRIDCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
        return FALSE;

	return m_pLoader->Create(rect, pParentWnd, nID, dwStyle);
}

void CExGridCtrl::SetupDefaultCells()
{
	m_pLoader->SetupDefaultCells();
}

void CExGridCtrl::PreSubclassWindow()
{
	m_pLoader->PreSubclassWindow();
}

// 추가 20050426 LYH : Grid 를 parent 로 삼는 컨트롤의 메세지를 받도록 설정한다.
LRESULT CExGridCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return m_pLoader->WindowProc(message, wParam, lParam);
}

// Sends a message to the parent in the form of a WM_NOTIFY message with
// a NM_GRIDVIEW structure attached
LRESULT CExGridCtrl::SendMessageToParent(int nRow, int nCol, int nMessage)
{
	return m_pLoader->SendMessageToParent(nRow, nCol, nMessage);
}

void CExGridCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	m_pLoader->OnContextMenu(pWnd, point) ;
}

// Send a request to the parent return 
LRESULT CExGridCtrl::SendDisplayRequestToParent(GV_DISPINFO* pDisplayInfo)
{
	return m_pLoader->SendDisplayRequestToParent(pDisplayInfo);
}

// Send a hint to the parent about caching information
LRESULT CExGridCtrl::SendCacheHintToParent(CCellRange& range)
{
	return m_pLoader->SendCacheHintToParent(range);
}

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl message handlers

void CExGridCtrl::OnPaint()
{
	m_pLoader->OnPaint();
}

BOOL CExGridCtrl::OnEraseBkgnd(CDC* pDC)
{
	return m_pLoader->OnEraseBkgnd(pDC);
}

// Custom background erasure. pMember->m_pGrid gets called from within the OnDraw function,
// since we will (most likely) be using a memory DC to stop flicker. If we just
// erase the background normally through OnEraseBkgnd, and didn't fill the memDC's
// selected bitmap with colour, then all sorts of vis problems would occur
void CExGridCtrl::EraseBkgnd(CDC* pDC)
{
	m_pLoader->EraseBkgnd(pDC);
}

void CExGridCtrl::OnSize(UINT nType, int cx, int cy)
{
	m_pLoader->OnSize(nType, cx, cy);
}

UINT CExGridCtrl::OnGetDlgCode()
{
	return m_pLoader->OnGetDlgCode();
}

#ifndef _WIN32_WCE
// If system colours change, then redo colours
void CExGridCtrl::OnSysColorChange()
{
	m_pLoader->OnSysColorChange();
}
#endif

#ifndef _WIN32_WCE_NO_CURSOR
// If we are drag-selecting cells, or drag and dropping, stop now
void CExGridCtrl::OnCaptureChanged(CWnd *pWnd)
{
	m_pLoader->OnCaptureChanged(pWnd);
}
#endif

#if (_MFC_VER >= 0x0421) || (_WIN32_WCE >= 210)
// If system settings change, then redo colours
void CExGridCtrl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	m_pLoader->OnSettingChange(uFlags, lpszSection);
}
#endif

// For drag-selection. Scrolls hidden cells into view
// TODO: decrease timer interval over time to speed up selection over time
void CExGridCtrl::OnTimer(UINT nIDEvent)
{
	m_pLoader->OnTimer(nIDEvent);
}

// move about with keyboard
void CExGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_pLoader->OnKeyDown(nChar, nRepCnt, nFlags);
}

void CExGridCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_pLoader->OnKeyUp(nChar, nRepCnt, nFlags);
}

void CExGridCtrl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_pLoader->OnSysKeyDown(nChar, nRepCnt, nFlags);
}

// Instant editing of cells when keys are pressed
void CExGridCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_pLoader->OnChar(nChar, nRepCnt, nFlags);
}

// Added by KiteFly
LRESULT CExGridCtrl::OnImeChar(WPARAM wCharCode, LPARAM lParam)
{
	return m_pLoader->OnImeChar(wCharCode, lParam);
}

// Callback from any CInPlaceEdits that ended. pMember->m_pGrid just calls OnEndEditCell,
// refreshes the edited cell and moves onto next cell if return 
// from the edit says we should.
void CExGridCtrl::OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_pLoader->OnEndInPlaceEdit(pNMHDR, pResult);
}

// Handle horz scrollbar notifications
void CExGridCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_pLoader->OnHScroll(nSBCode, nPos, pScrollBar);
}

// Handle vert scrollbar notifications
void CExGridCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_pLoader->OnVScroll(nSBCode, nPos, pScrollBar);
}

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl implementation functions

void CExGridCtrl::OnDraw(CDC* pDC)
{
	m_pLoader->OnDraw(pDC);
}

////////////////////////////////////////////////////////////////////////////////////////
// CExGridCtrl Cell selection stuff

// Is a given cell designation valid (ie within the bounds of our number
// of columns/rows)?
BOOL CExGridCtrl::IsValid(int nRow, int nCol)
{
	return m_pLoader->IsValid(nRow, nCol);
}

BOOL CExGridCtrl::IsValid(CCellID& cell)
{
	return m_pLoader->IsValid(cell);
}

// Is a given cell range valid (ie within the bounds of our number
// of columns/rows)?
BOOL CExGridCtrl::IsValid(CCellRange& range)
{
	return m_pLoader->IsValid(range);
}

// Enables/Disables redraw for certain operations like columns auto-sizing etc,
// but not for user caused things such as selection changes.
void CExGridCtrl::SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars /* = FALSE */)
{
	m_pLoader->SetRedraw(bAllowDraw, bResetScrollBars);
}

// Forces a redraw of a cell immediately (using a direct DC construction,
// or the supplied dc)
BOOL CExGridCtrl::RedrawCell(CCellID& cell, CDC* pDC /* = NULL */)
{
	return m_pLoader->RedrawCell(cell, pDC);
}

BOOL CExGridCtrl::RedrawCell(int nRow, int nCol, CDC* pDC /* = NULL */)
{
	return m_pLoader->RedrawCell(nRow, nCol, pDC);
}

// redraw a complete row
BOOL CExGridCtrl::RedrawRow(int row)
{
	return m_pLoader->RedrawRow(row);
}

// redraw a complete column
BOOL CExGridCtrl::RedrawColumn(int col)
{
	return m_pLoader->RedrawColumn(col);
}


// Sets the currently selected cellreturn 
CCellID CExGridCtrl::SetFocusCell(int nRow, int nCol)
{
	return m_pLoader->SetFocusCell(nRow, nCol);
}

CCellID CExGridCtrl::SetFocusCell(CCellID cell)
{
	return m_pLoader->SetFocusCell(cell);
}

// Sets the range of currently selected cells
void CExGridCtrl::SetSelectedRange(CCellRange& Range, BOOL bForceRepaint /* = FALSE */, BOOL bSelectCells/*=TRUE*/)
{
	m_pLoader->SetSelectedRange(Range, bForceRepaint, bSelectCells);
}

void CExGridCtrl::SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol, BOOL bForceRepaint /* = FALSE */, BOOL bSelectCells/*=TRUE*/)
{
	m_pLoader->SetSelectedRange(nMinRow, nMinCol, nMaxRow, nMaxCol, bForceRepaint, bSelectCells);
}

// selects all cells
void CExGridCtrl::SelectAllCells()
{
	m_pLoader->SelectAllCells();
}

// selects columns
void CExGridCtrl::SelectColumns(CCellID currentCell, BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
	m_pLoader->SelectColumns(currentCell, bForceRedraw, bSelectCells);
}

// selects rows
void CExGridCtrl::SelectRows(CCellID currentCell, BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
	m_pLoader->SelectRows(currentCell, bForceRedraw , bSelectCells);
}

// selects cells
void CExGridCtrl::SelectCells(CCellID currentCell, BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
	m_pLoader->SelectCells(currentCell, bForceRedraw, bSelectCells);
}

// Called when mouse/keyboard selection is a-happening.
void CExGridCtrl::OnSelecting(CCellID& currentCell)
{
	m_pLoader->OnSelecting(currentCell);
}

void CExGridCtrl::ValidateAndModifyCellContents(int nRow, int nCol, LPCTSTR strText)
{
	m_pLoader->ValidateAndModifyCellContents(nRow, nCol, strText);
}

void CExGridCtrl::ClearCells(CCellRange Selection)
{
	m_pLoader->ClearCells(Selection);
}

#ifndef GRIDCONTROL_NO_CLIPBOARD

////////////////////////////////////////////////////////////////////////////////////////
// Clipboard functions

// Deletes the contents from the selected cells
void CExGridCtrl::CutSelectedText()
{
	m_pLoader->CutSelectedText();
}

// Copies text from the selected cells to the clipboard
COleDataSource* CExGridCtrl::CopyTextFromGrid()
{
	return m_pLoader->CopyTextFromGrid();
}

// Pastes text from the clipboard to the selected cells
BOOL CExGridCtrl::PasteTextToGrid(CCellID cell, COleDataObject* pDataObject, BOOL bSelectPastedCells /*=TRUE*/)
{
	return m_pLoader->PasteTextToGrid(cell, pDataObject, bSelectPastedCells);
}
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP

// Start drag n drop
void CExGridCtrl::OnBeginDrag()
{
	m_pLoader->OnBeginDrag();
}

// Handle drag over grid
DROPEFFECT CExGridCtrl::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return m_pLoader->OnDragOver(pDataObject, dwKeyState, point);
}

// Something has just been dragged onto the grid
DROPEFFECT CExGridCtrl::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return m_pLoader->OnDragEnter(pDataObject, dwKeyState, point);
}

// Something has just been dragged away from the grid
void CExGridCtrl::OnDragLeave()
{
	m_pLoader->OnDragLeave();
}

// Something has just been dropped onto the grid
BOOL CExGridCtrl::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	return m_pLoader->OnDrop(pDataObject, dropEffect, point);
}
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CExGridCtrl::OnEditCut()
{
	m_pLoader->OnEditCut();
}

void CExGridCtrl::OnEditCopy()
{
	m_pLoader->OnEditCopy();
}

void CExGridCtrl::OnEditPaste()
{
	m_pLoader->OnEditPaste();
}
#endif

void CExGridCtrl::OnEditSelectAll()
{
	m_pLoader->OnEditSelectAll();
}

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CExGridCtrl::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	m_pLoader->OnUpdateEditCopy(pCmdUI);
}

void CExGridCtrl::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	m_pLoader->OnUpdateEditCut(pCmdUI);
}

void CExGridCtrl::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	m_pLoader->OnUpdateEditPaste(pCmdUI);
}
#endif

void CExGridCtrl::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
	m_pLoader->OnUpdateEditSelectAll(pCmdUI);
}

////////////////////////////////////////////////////////////////////////////////////////
// hittest-like functions

// TRUE if the mouse is over a row resize area
BOOL CExGridCtrl::MouseOverRowResizeArea(CPoint& point)
{
	return m_pLoader->MouseOverRowResizeArea(point);
}

// TRUE if the mouse is over a column resize area. point is in Client coords
BOOL CExGridCtrl::MouseOverColumnResizeArea(CPoint& point)
{
	return m_pLoader->MouseOverColumnResizeArea(point);
}

// Get cell from point.
// point - client coordinates
// bAllowFixedCellCheck - if TRUE then fixed cells are checked
CCellID CExGridCtrl::GetCellFromPt(CPoint point, BOOL bAllowFixedCellCheck /*=TRUE*/)
{
	return m_pLoader->GetCellFromPt(point, bAllowFixedCellCheck);
}

////////////////////////////////////////////////////////////////////////////////
// CExGridCtrl cellrange functions

// Gets the first non-fixed cell ID
CCellID CExGridCtrl::GetTopleftNonFixedCell(BOOL bForceRecalculation /*=FALSE*/)
{
	return m_pLoader->GetTopleftNonFixedCell(bForceRecalculation);
}

// pMember->m_pGrid gets even partially visible cells
CCellRange CExGridCtrl::GetVisibleNonFixedCellRange(LPRECT pRect /*=NULL*/, BOOL bForceRecalculation /*=FALSE*/)
{
	return m_pLoader->GetVisibleNonFixedCellRange(pRect, bForceRecalculation);
}

// used by ResetScrollBars() - pMember->m_pGrid gets only fully visible cells
CCellRange CExGridCtrl::GetUnobstructedNonFixedCellRange(BOOL bForceRecalculation /*=FALSE*/)
{
	return m_pLoader->GetUnobstructedNonFixedCellRange(bForceRecalculation);
}

// If no selection, then return 
CCellRange CExGridCtrl::GetSelectedCellRange()
{
	return m_pLoader->GetSelectedCellRange();
}

CCellRange CExGridCtrl::GetCellRange()
{
	return m_pLoader->GetCellRange();
}

// Resets the selected cell range to the empty set.
void CExGridCtrl::ResetSelectedRange()
{
	m_pLoader->ResetSelectedRange();
}

// Get/Set scroll position using 32 bit functions
int CExGridCtrl::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
	return m_pLoader->GetScrollPos32(nBar, bGetTrackPos);
}

BOOL CExGridCtrl::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
	return m_pLoader->SetScrollPos32(nBar, nPos, bRedraw);
}

void CExGridCtrl::EnableScrollBars(int nBar, BOOL bEnable /*=TRUE*/)
{
	m_pLoader->EnableScrollBars(nBar, bEnable);
}

// If resizing or cell counts/sizes change, call pMember->m_pGrid - it'll fix up the scroll bars
void CExGridCtrl::ResetScrollBars()
{
	m_pLoader->ResetScrollBars();
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column position functions

BOOL CExGridCtrl::GetCellOrigin(int nRow, int nCol, LPPOINT p)
{
	return m_pLoader->GetCellOrigin(nRow, nCol, p);
}

BOOL CExGridCtrl::GetCellOrigin(CCellID& cell, LPPOINT p)
{
	return m_pLoader->GetCellOrigin(cell, p);
}

BOOL CExGridCtrl::GetCellRect(CCellID& cell, LPRECT pRect)
{
	return m_pLoader->GetCellRect(cell, pRect);
}

BOOL CExGridCtrl::GetCellRect(int nRow, int nCol, LPRECT pRect)
{
	return m_pLoader->GetCellRect(nRow, nCol, pRect);
}

BOOL CExGridCtrl::GetTextRect(CCellID& cell, LPRECT pRect)
{
	return m_pLoader->GetTextRect(cell, pRect);
}

BOOL CExGridCtrl::GetTextRect(int nRow, int nCol, LPRECT pRect)
{
	return m_pLoader->GetTextRect(nRow, nCol, pRect);
}

BOOL CExGridCtrl::GetCellRangeRect(CCellRange& cellRange, LPRECT lpRect)
{
	return m_pLoader->GetCellRangeRect(cellRange, lpRect);
}

////////////////////////////////////////////////////////////////////////////////////
// Grid attribute functions

LRESULT CExGridCtrl::OnSetFont(WPARAM hFont, LPARAM lParam)
{
	return m_pLoader->OnSetFont(hFont, lParam);
}

LRESULT CExGridCtrl::OnGetFont(WPARAM wParam, LPARAM lParam)
{
	return m_pLoader->OnGetFont(wParam, lParam);
}

#ifndef _WIN32_WCE_NO_CURSOR
BOOL CExGridCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return m_pLoader->OnSetCursor(pWnd, nHitTest, message);
}
#endif

////////////////////////////////////////////////////////////////////////////////////
// Row/Column count functions

BOOL CExGridCtrl::SetFixedRowCount(int nFixedRows)
{
	return m_pLoader->SetFixedRowCount(nFixedRows);
}

BOOL CExGridCtrl::SetFixedColumnCount(int nFixedCols)
{
	return m_pLoader->SetFixedColumnCount(nFixedCols);
}

BOOL CExGridCtrl::SetRowCount(int nRows)
{
	return m_pLoader->SetRowCount(nRows);
}

BOOL CExGridCtrl::SetColumnCount(int nCols)
{
	return m_pLoader->SetColumnCount(nCols);
}

// Insert a column at a given position, or add to end of columns (if nColumn = -1)
int CExGridCtrl::InsertColumn(LPCTSTR strHeading, UINT nFormat/* = DT_CENTER|DT_VCENTER|DT_SINGLELINE*/, int nColumn/* = -1*/)
{
	return m_pLoader->InsertColumn(strHeading, nFormat, nColumn);
}

// Insert a row at a given position, or add to end of rows (if nRow = -1)
int CExGridCtrl::InsertRow(LPCTSTR strHeading, int nRow /* = -1 */)
{
	return m_pLoader->InsertRow(strHeading, nRow);
}

///////////////////////////////////////////////////////////////////////////////
// Cell creation stuff

BOOL CExGridCtrl::SetCellType(int nRow, int nCol, CRuntimeClass* pRuntimeClass)
{
	return m_pLoader->SetCellType(nRow, nCol, pRuntimeClass);
}

BOOL CExGridCtrl::SetDefaultCellType(CRuntimeClass* pRuntimeClass)
{
	return m_pLoader->SetDefaultCellType(pRuntimeClass);
}

// Creates a new grid cell and performs any necessary initialisation
/*virtual*/ CGridCellBase* CExGridCtrl::CreateCell(int nRow, int nCol)
{
	return m_pLoader->CreateCell(nRow, nCol);
}

// Performs any cell cleanup necessary to maintain grid integrity
/*virtual*/ void CExGridCtrl::DestroyCell(int nRow, int nCol)
{
	m_pLoader->DestroyCell(nRow, nCol);
}

BOOL CExGridCtrl::DeleteColumn(int nColumn)
{
	return m_pLoader->DeleteColumn(nColumn);
}

BOOL CExGridCtrl::DeleteRow(int nRow)
{
	return m_pLoader->DeleteRow(nRow);
}

// Handy function that removes all non-fixed rows
BOOL CExGridCtrl::DeleteNonFixedRows()
{
	return m_pLoader->DeleteNonFixedRows();
}

// Removes all rows, columns and data from the grid.
BOOL CExGridCtrl::DeleteAllItems()
{
	return m_pLoader->DeleteAllItems();
}

void CExGridCtrl::AutoFill()
{
	m_pLoader->AutoFill();
}

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl data functions

// Set CListCtrl::GetNextItem for details
CCellID CExGridCtrl::GetNextItem(CCellID& cell, int nFlags)
{
	return m_pLoader->GetNextItem(cell, nFlags);
}

// Sorts on a given column using the cell text
BOOL CExGridCtrl::SortTextItems(int nCol, BOOL bAscending, LPARAM data /* = 0 */)
{
	return m_pLoader->SortTextItems(nCol, bAscending, data);
}

void CExGridCtrl::SetCompareFunction(PFNLVCOMPARE pfnCompare)
{
	m_pLoader->SetCompareFunction(pfnCompare);
}

// Sorts on a given column using the cell text and using the specified comparison
// function
BOOL CExGridCtrl::SortItems(int nCol, BOOL bAscending, LPARAM data /* = 0 */)
{
	return m_pLoader->SortItems(nCol, bAscending, data);
}

// Sorts on a given column using the supplied compare function (see CListCtrl::SortItems)
BOOL CExGridCtrl::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data /* = 0 */)
{
	return m_pLoader->SortItems(pfnCompare, nCol, bAscending, data);
}

// private recursive sort implementation
BOOL CExGridCtrl::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, int low, int high)
{
	return m_pLoader->SortItems(pfnCompare, nCol, bAscending, data, low, high);
}

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl data functions

BOOL CExGridCtrl::SetItem(GV_ITEM* pItem)
{
	return m_pLoader->SetItem(pItem);
}

BOOL CExGridCtrl::GetItem(GV_ITEM* pItem)
{
	return m_pLoader->GetItem(pItem);
}

BOOL CExGridCtrl::SetItemText(int nRow, int nCol, LPCTSTR str)
{
	return m_pLoader->SetItemText(nRow, nCol, str);
}

#if !defined(_WIN32_WCE) || (_WIN32_WCE >= 210)
// EFW - 06/13/99 - Added to support printf-style formatting codes
BOOL CExGridCtrl::SetItemTextFmt(int nRow, int nCol, LPCTSTR szFmt, ...)
{
	return FALSE;
//	return m_pLoader->SetItemTextFmt(int nRow, int nCol, LPCTSTR szFmt, ...);
}

// EFW - 06/13/99 - Added to support string resource ID.  Supports
// a variable argument list too.
BOOL CExGridCtrl::SetItemTextFmtID(int nRow, int nCol, UINT nID, ...)
{
	return FALSE;
//	return m_pLoader->SetItemTextFmtID(int nRow, int nCol, UINT nID, ...);
}
#endif

BOOL CExGridCtrl::SetItemData(int nRow, int nCol, LPARAM lParam)
{
	return m_pLoader->SetItemData(nRow, nCol, lParam);
}

LPARAM CExGridCtrl::GetItemData(int nRow, int nCol)
{
	return m_pLoader->GetItemData(nRow, nCol);
}

BOOL CExGridCtrl::SetItemImage(int nRow, int nCol, int iImage)
{
	return m_pLoader->SetItemImage(nRow, nCol, iImage);
}

int CExGridCtrl::GetItemImage(int nRow, int nCol)
{
	return m_pLoader->GetItemImage(nRow, nCol);
}

BOOL CExGridCtrl::SetItemState(int nRow, int nCol, UINT state)
{
	return m_pLoader->SetItemState(nRow, nCol, state);
}

UINT CExGridCtrl::GetItemState(int nRow, int nCol)
{
	return m_pLoader->GetItemState(nRow, nCol);
}

BOOL CExGridCtrl::SetItemFormat(int nRow, int nCol, UINT nFormat)
{
	return m_pLoader->SetItemFormat(nRow, nCol, nFormat);
}

UINT CExGridCtrl::GetItemFormat(int nRow, int nCol)
{
	return m_pLoader->GetItemFormat(nRow, nCol);
}

BOOL CExGridCtrl::SetItemBkColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
	return m_pLoader->SetItemBkColour(nRow, nCol, cr);
}

COLORREF CExGridCtrl::GetItemBkColour(int nRow, int nCol)
{
	return m_pLoader->GetItemBkColour(nRow, nCol);
}

BOOL CExGridCtrl::SetItemFgColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
	return m_pLoader->SetItemFgColour(nRow, nCol, cr);
}

COLORREF CExGridCtrl::GetItemFgColour(int nRow, int nCol)
{
	return m_pLoader->GetItemFgColour(nRow, nCol);
}

BOOL CExGridCtrl::SetItemFont(int nRow, int nCol, LOGFONT* plf)
{
	return m_pLoader->SetItemFont(nRow, nCol, plf);
}

LOGFONT* CExGridCtrl::GetItemFont(int nRow, int nCol)
{
	return m_pLoader->GetItemFont(nRow, nCol);
}

BOOL CExGridCtrl::IsItemEditing(int nRow, int nCol)
{
	return m_pLoader->IsItemEditing(nRow, nCol);
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column size functions

long CExGridCtrl::GetVirtualWidth()
{
	return m_pLoader->GetVirtualWidth();
}

long CExGridCtrl::GetVirtualHeight()
{
	return m_pLoader->GetVirtualHeight();
}

int CExGridCtrl::GetRowHeight(int nRow)
{
	return m_pLoader->GetRowHeight(nRow);
}

int CExGridCtrl::GetColumnWidth(int nCol)
{
	return m_pLoader->GetColumnWidth(nCol);
}

BOOL CExGridCtrl::SetRowHeight(int nRow, int height)
{
	return m_pLoader->SetRowHeight(nRow, height);
}

BOOL CExGridCtrl::SetColumnWidth(int nCol, int width)
{
	return m_pLoader->SetColumnWidth(nCol, width);
}

int CExGridCtrl::GetFixedRowHeight()
{
	return m_pLoader->GetFixedRowHeight();
}

int CExGridCtrl::GetFixedColumnWidth()
{
	return m_pLoader->GetFixedColumnWidth();
}

BOOL CExGridCtrl::AutoSizeColumn(int nCol, UINT nAutoSizeStyle /*=GVS_DEFAULT*/, BOOL bResetScroll /*=TRUE*/)
{
	return m_pLoader->AutoSizeColumn(nCol, nAutoSizeStyle, bResetScroll);
}

BOOL CExGridCtrl::AutoSizeRow(int nRow, BOOL bResetScroll /*=TRUE*/)
{
	return m_pLoader->AutoSizeRow(nRow, bResetScroll);
}

void CExGridCtrl::AutoSizeColumns(UINT nAutoSizeStyle /*=GVS_DEFAULT*/)
{
	m_pLoader->AutoSizeColumns(nAutoSizeStyle);
}

void CExGridCtrl::AutoSizeRows()
{
	m_pLoader->AutoSizeRows();
}

// sizes all rows and columns
// faster than calling both AutoSizeColumns() and AutoSizeRows()
void CExGridCtrl::AutoSize(UINT nAutoSizeStyle /*=GVS_DEFAULT*/)
{
	m_pLoader->AutoSize(nAutoSizeStyle);
}

// Expands the columns to fit the screen space. If bExpandFixed is FALSE then fixed 
// columns will not be affected
void CExGridCtrl::ExpandColumnsToFit(BOOL bExpandFixed /*=TRUE*/)
{
	m_pLoader->ExpandColumnsToFit(bExpandFixed);
}

void CExGridCtrl::ExpandLastColumn()
{
	m_pLoader->ExpandLastColumn();
}

// Expands the rows to fit the screen space. If bExpandFixed is FALSE then fixed 
// rows will not be affected
void CExGridCtrl::ExpandRowsToFit(BOOL bExpandFixed /*=TRUE*/)
{
	m_pLoader->ExpandRowsToFit(bExpandFixed);
}

// Expands the cells to fit the screen space. If bExpandFixed is FALSE then fixed 
// cells  will not be affected
void CExGridCtrl::ExpandToFit(BOOL bExpandFixed /*=TRUE*/)
{
	m_pLoader->ExpandToFit(bExpandFixed);
}

/////////////////////////////////////////////////////////////////////////////////////
// Attributes

void CExGridCtrl::SetVirtualMode(BOOL bVirtual)
{
	m_pLoader->SetVirtualMode(bVirtual);
}

void CExGridCtrl::SetGridLines(int nWhichLines /*=GVL_BOTH*/) 
{
	m_pLoader->SetGridLines(nWhichLines) ;
}

void CExGridCtrl::SetListMode(BOOL bEnableListMode /*=TRUE*/)
{
	m_pLoader->SetListMode(bEnableListMode);
}

void CExGridCtrl::SetSortColumn(int nCol)
{
	m_pLoader->SetSortColumn(nCol);
}

BOOL CExGridCtrl::IsCellFixed(int nRow, int nCol)
{
	return m_pLoader->IsCellFixed(nRow, nCol);
}

void CExGridCtrl::SetModified(BOOL bModified /*=TRUE*/, int nRow /*=-1*/, int nCol /*=-1*/)
{
	m_pLoader->SetModified(bModified, nRow, nCol);
}

BOOL CExGridCtrl::GetModified(int nRow /*=-1*/, int nCol /*=-1*/)
{
	return m_pLoader->GetModified(nRow, nCol);
}

/////////////////////////////////////////////////////////////////////////////////////
// GridCtrl cell visibility tests and invalidation/redraw functions

void CExGridCtrl::Refresh()
{
	m_pLoader->Refresh();
}

// EnsureVisible supplied by Roelf Werkman
void CExGridCtrl::EnsureVisible(int nRow, int nCol)
{
	m_pLoader->EnsureVisible(nRow, nCol);
}

BOOL CExGridCtrl::IsCellEditable(CCellID &cell)
{
	return m_pLoader->IsCellEditable(cell);
}

BOOL CExGridCtrl::IsCellEditable(int nRow, int nCol)
{
	return m_pLoader->IsCellEditable(nRow, nCol);
}

BOOL CExGridCtrl::IsCellSelected(CCellID &cell)
{
	return m_pLoader->IsCellSelected(cell);
}

BOOL CExGridCtrl::IsCellSelected(int nRow, int nCol)
{
	return m_pLoader->IsCellSelected(nRow, nCol);
}

BOOL CExGridCtrl::IsCellVisible(CCellID cell) 
{
	return m_pLoader->IsCellVisible(cell) ;
}

BOOL CExGridCtrl::IsCellVisible(int nRow, int nCol)
{
	return m_pLoader->IsCellVisible(nRow, nCol);
}

BOOL CExGridCtrl::InvalidateCellRect(CCellID& cell)
{
	return m_pLoader->InvalidateCellRect(cell);
}

BOOL CExGridCtrl::InvalidateCellRect(int row, int col)
{
	return m_pLoader->InvalidateCellRect(row, col);
}

BOOL CExGridCtrl::InvalidateCellRect(CCellRange& cellRange)
{
	return m_pLoader->InvalidateCellRect(cellRange);
}

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl Mouse stuff

// Handles mouse wheel notifications
// Note - if pMember->m_pGrid doesn't work for win95 then use OnRegisteredMouseWheel instead
#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
BOOL CExGridCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return m_pLoader->OnMouseWheel(nFlags, zDelta, pt);
}
#endif // !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)

void CExGridCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	m_pLoader->OnMouseMove(nFlags, point);
}

CPoint CExGridCtrl::GetPointClicked(int nRow, int nCol, CPoint& point)
{
	return m_pLoader->GetPointClicked(nRow, nCol, point);
}

void CExGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_pLoader->OnLButtonDblClk(nFlags, point);
}

void CExGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pLoader->OnLButtonDown(nFlags, point);
}

void CExGridCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_pLoader->OnLButtonUp(nFlags, point);
}

#ifndef _WIN32_WCE
void CExGridCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_pLoader->OnRButtonDown(nFlags, point);
}

// EFW - Added to forward right click to parent so that a context
// menu can be shown without deriving a new grid class.
void CExGridCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_pLoader->OnRButtonUp(nFlags, point);
}
#endif

#ifndef _WIN32_WCE
/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl persistance

BOOL CExGridCtrl::Save(LPCTSTR filename, TCHAR chSeparator/*=_T(',')*/)
{
	return m_pLoader->Save(filename, chSeparator);
}

BOOL CExGridCtrl::Load(LPCTSTR filename, TCHAR chSeparator/*=_T(',')*/)
{
	return m_pLoader->Load(filename, chSeparator);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl overrideables

#ifndef GRIDCONTROL_NO_DRAGDROP
// pMember->m_pGrid is no longer needed since I've changed to OLE drag and drop - but it's
// still cool code. :)
CImageList* CExGridCtrl::CreateDragImage(CPoint *pHotSpot)
{
	return m_pLoader->CreateDragImage(pHotSpot);
}
#endif

void CExGridCtrl::OnFixedRowClick(CCellID& cell)
{
	m_pLoader->OnFixedRowClick(cell);
}

void CExGridCtrl::OnFixedColumnClick(CCellID& cell)
{
	m_pLoader->OnFixedColumnClick(cell);
}

// Gets the extent of the text pointed to by str (no CDC needed)
// By default pMember->m_pGrid uses the selected font (which is a bigger font)
CSize CExGridCtrl::GetTextExtent(int nRow, int nCol, LPCTSTR str)
{
	return m_pLoader->GetTextExtent(nRow, nCol, str);
}

// virtual
void CExGridCtrl::OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
{
	m_pLoader->OnEditCell(nRow, nCol, point, nChar);
}

// virtual
void CExGridCtrl::EndEditing()
{
	m_pLoader->EndEditing();
}

// virtual
void CExGridCtrl::OnEndEditCell(int nRow, int nCol, CString str)
{
	m_pLoader->OnEndEditCell(nRow, nCol, str);
}

// If pMember->return 
// virtual
BOOL CExGridCtrl::ValidateEdit(int nRow, int nCol, LPCTSTR str)
{
	return m_pLoader->ValidateEdit(nRow, nCol, str);
}

// virtual
CString CExGridCtrl::GetItemText(int nRow, int nCol)
{
	return m_pLoader->GetItemText(nRow, nCol);
}

CGridCellBase* CExGridCtrl::GetDefaultCell(BOOL bFixedRow, BOOL bFixedCol)
{
	return m_pLoader->GetDefaultCell(bFixedRow, bFixedCol);
}

CGridCellBase* CExGridCtrl::GetCell(int nRow, int nCol)
{
	return m_pLoader->GetCell(nRow, nCol);
}

BOOL CExGridCtrl::SetCell(int nRow, int nCol, CGridCellBase* pCell)
{
	return m_pLoader->SetCell(nRow, nCol, pCell);
}

COLORREF CExGridCtrl::GetFixedBkColor()
{
	return m_pLoader->GetFixedBkColor();
}

// recursive sort implementation
BOOL CExGridCtrl::SortTextItems(int nCol, BOOL bAscending, int low, int high)
{
	return m_pLoader->SortTextItems(nCol, bAscending, low, high);
}

void CExGridCtrl::SetGridScrollBarVisible(BOOL bHScroll, BOOL bVScroll)
{
	m_pLoader->SetGridScrollBarVisible(bHScroll, bVScroll);
}

void CExGridCtrl::SetUseFocusRect(BOOL bUse)
{
	m_pLoader->SetUseFocusRect(bUse);
}

BOOL CExGridCtrl::GetUseFocusRect()
{
	return m_pLoader->GetUseFocusRect();
}

void CExGridCtrl::SetDrawFixedLine(BOOL bDraw)
{
	m_pLoader->SetDrawFixedLine(bDraw);
}

// PnWndGrid.cpp : implementation file
//

#include "stdafx.h"
#include "axisgcom.h"
#include "PnWndGrid.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HEADER_HEIGHT		2
#define FOOTER_HEIGHT		2
#define LEFT_MARGIN		4
#define RIGHT_MARGIN		4
#define TOP_MARGIN		1
#define BOTTOM_MARGIN		1 
#define GAP			1


#define SELECTED_CELL_FONT_WEIGHT 600    // weight of text for selected items
#define TOOLTIP_TIMER	601
#define	GRID_CLASSNAME	"TABLE_GRID_CLASS"
/////////////////////////////////////////////////////////////////////////////
// CPnWndGrid

IMPLEMENT_DYNCREATE(CPnWndGrid, CWnd)

void AFXAPI DDX_GridControl(CDataExchange* pDX, int nIDC, CPnWndGrid& rControl)
{
	if (rControl.GetSafeHwnd() == NULL)
	{
		ASSERT(!pDX->m_bSaveAndValidate);
		HWND hWndCtrl = pDX->PrepareCtrl(nIDC);

		if (!rControl.SubclassWindow(hWndCtrl))
		{
			ASSERT(FALSE);
			AfxThrowNotSupportedException();
		}
		else
		{
			if (pDX->m_pDlgWnd->GetSafeHwnd() != ::GetParent(rControl.GetSafeHwnd()))
				rControl.AttachControlSite(pDX->m_pDlgWnd);
		}
	}
}

UINT GetMouseScrollLines()
{
	int nScrollLines = 3;
	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
	     0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		TCHAR szData[128];
		DWORD dwKeyDataType;
		DWORD dwDataBufSize = sizeof(szData);

		if (RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
			   (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
		{
			nScrollLines = _tcstoul(szData, NULL, 10);
		}
		RegCloseKey(hKey);
	}

	return nScrollLines;
}

CPnWndGrid::CPnWndGrid(int nRows, int nCols, int nFixedRows, int nFixedCols)
{
	RegisterWindowClass();

	m_nRows              = 0;
	m_nCols              = 0;
	m_nFixedRows         = 0;
	m_nFixedCols         = 0;

	m_nDefCellHeight     = 10;
	m_nDefCellWidth      = 25;

	m_nVScrollMax        = 0; 
	m_nHScrollMax        = 0;
	m_nMargin            = 0; 
	m_nRowsPerWheelNotch = GetMouseScrollLines();

	m_MouseMode          = MOUSE_NOTHING;
	m_nGridLines         = GVL_BOTH;
	m_bListMode          = TRUE;
	m_bSingleRowSelection = TRUE;
	m_bAllowDraw         = TRUE; 
	m_bEnableSelection   = TRUE;
	m_bAllowRowResize    = FALSE;
	m_bAllowColumnResize = TRUE;
	m_bSortOnClick       = FALSE;
	m_bHandleTabKey      = FALSE;
	m_bDoubleBuffer      = TRUE;
	m_bTitleTips         = FALSE;
	m_bTitleTipsCreate   = FALSE;
	m_uToolTipTimer	     = 0;
	m_nTimerCheck	     = 0;

	m_bAscending         = TRUE;
	m_SortColumn         = -1;

	m_nTimerID           = 0;
	m_nTimerInterval     = 25;
	m_nResizeCaptureRange = 3;
	m_pImageList         = NULL;      

	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));    
	memcpy(&m_Logfont, &(ncm.lfMessageFont), sizeof(LOGFONT));
	SetRowCount(nRows);
	SetColumnCount(nCols);
	SetFixedRowCount(nFixedRows);
	SetFixedColumnCount(nFixedCols);

	SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	SetTextBkColor(::GetSysColor(COLOR_WINDOW));
	SetBkColor(::GetSysColor(COLOR_3DFACE));
	SetFixedTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	SetFixedBkColor(RGB(180, 180, 180));
	
	m_SelectedCellMap.RemoveAll();
	m_PrevSelectedCellMap.RemoveAll();
	m_bForwardDBClick = FALSE;
	bRowColor	= FALSE;
	bFixedRowColor	= FALSE;
	m_bHScrollShow = TRUE;
	m_bVScrollShow = TRUE;
}

CPnWndGrid::~CPnWndGrid()
{
	DeleteAllItems();
	DestroyWindow();
	m_Font.DeleteObject();
}


BOOL CPnWndGrid::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, GRID_CLASSNAME, &wndcls)))
	{
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = (HBRUSH)NULL;// (COLOR_3DFACE + 1);
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = GRID_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CPnWndGrid::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle)
{
	ASSERT(pParentWnd->GetSafeHwnd());

	if (!CWnd::Create(GRID_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID)) 
		return FALSE;

	m_pParent = pParentWnd;

	
	TRY {
		m_arRowHeights.SetSize(m_nRows);
		m_arColWidths.SetSize(m_nCols);
		m_pCellInfo.SetSize(m_nCols);
		m_arColType.SetSize(m_nCols);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	END_CATCH

	for (int i = 0; i < m_nRows; i++)
		m_arRowHeights[i] = m_nDefCellHeight;
	for (int i = 0; i < m_nCols; i++) 
	{
		m_arColWidths[i] = m_nDefCellWidth;
		m_arColType[i] = GVET_NOEDIT;
	}
	ResetScrollBars();
	return TRUE;
}

void CPnWndGrid::SetFirst()
{
	SendMessage(WM_VSCROLL, SB_TOP, 0);
}

void CPnWndGrid::PreSubclassWindow() 
{    
	CWnd::PreSubclassWindow();
	HFONT hFont = ::CreateFontIndirect(&m_Logfont);
	OnSetFont((LPARAM)hFont, 0);
	DeleteObject(hFont);
	ResetScrollBars();   
}

BOOL CPnWndGrid::SubclassWindow(HWND hWnd) 
{    
	if (!CWnd::SubclassWindow(hWnd))
		return FALSE;
	if(m_uToolTipTimer)
	{
		KillTimer(m_uToolTipTimer);
		m_uToolTipTimer = 0;
	}
	m_uToolTipTimer = SetTimer(TOOLTIP_TIMER, 100, NULL);
	return TRUE;
}

LRESULT CPnWndGrid::SendMessageToParent(int nRow, int nCol, int nMessage)
{
	if (!IsWindow(m_hWnd))
		return 0;

	NM_GRIDVIEW nmgv;
	nmgv.iRow         = nRow;
	nmgv.iColumn      = nCol;
	nmgv.hdr.hwndFrom = m_hWnd;
	nmgv.hdr.idFrom   = GetDlgCtrlID();
	nmgv.hdr.code     = nMessage;

	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
	else
		return 0;
}

BEGIN_MESSAGE_MAP(CPnWndGrid, CWnd)
	//{{AFX_MSG_MAP(CPnWndGrid)
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
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEWHEEL()

	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_CAPTURECHANGED()
	ON_WM_SETTINGCHANGE()
	
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
END_MESSAGE_MAP()

void CPnWndGrid::OnPaint() 
{
	CPaintDC dc(this);
	CMemDC MemDC(&dc);
	Draw(&MemDC);
}

BOOL CPnWndGrid::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void CPnWndGrid::EraseBkgnd(CDC* pDC) 
{
	CRect  VisRect, ClipRect, rect;
	CBrush FixedBack(GetFixedBkColor()),
		TextBack(GetTextBkColor()),
		Back(GetBkColor());

	if (pDC->GetClipBox(ClipRect) == ERROR) 
		return;
	GetVisibleNonFixedCellRange(VisRect);

	// Draw Fixed columns background
	int nFixedColumnWidth = GetFixedColumnWidth();
	if (ClipRect.left < nFixedColumnWidth && ClipRect.top < VisRect.bottom)
		pDC->FillRect(CRect(ClipRect.left, ClipRect.top, 
			    nFixedColumnWidth, VisRect.bottom),
		      &FixedBack);

	// Draw Fixed rows background
	int nFixedRowHeight = GetFixedRowHeight();
	if (ClipRect.top < nFixedRowHeight && 
		ClipRect.right > nFixedColumnWidth && ClipRect.left < VisRect.right)
		pDC->FillRect(CRect(nFixedColumnWidth-1, ClipRect.top,
			    VisRect.right, nFixedRowHeight), &FixedBack);

	// Draw non-fixed cell background
/*	if (rect.IntersectRect(VisRect, ClipRect)) 
	{
		CRect CellRect(max(nFixedColumnWidth, rect.left), 
			       max(nFixedRowHeight, rect.top),
			       rect.right, rect.bottom);
		pDC->FillRect(CellRect, &TextBack);
	}
*/
	// Draw right hand side of window outside grid
	if (VisRect.right < ClipRect.right) 
		pDC->FillRect(CRect(VisRect.right, ClipRect.top, 
			    ClipRect.right, ClipRect.bottom), &Back);

	// Draw bottom of window below grid
	if (VisRect.bottom < ClipRect.bottom && ClipRect.left < VisRect.right) 
		pDC->FillRect(CRect(ClipRect.left, VisRect.bottom,
			    VisRect.right, ClipRect.bottom), &Back);
}

void CPnWndGrid::OnSize(UINT nType, int cx, int cy) 
{
	static BOOL bAlreadyInsideThisProcedure = FALSE;
	if (bAlreadyInsideThisProcedure)
		return;

	if (!::IsWindow(m_hWnd))
		return;

	bAlreadyInsideThisProcedure = TRUE;

	CWnd::OnSize(nType, cx, cy);
	ResetScrollBars();    

	bAlreadyInsideThisProcedure = FALSE;
}

UINT CPnWndGrid::OnGetDlgCode() 
{
	UINT nCode = DLGC_WANTARROWS | DLGC_WANTCHARS; // DLGC_WANTALLKEYS; //
	if (m_bHandleTabKey && !IsCTRLpressed()) 
		nCode |= DLGC_WANTTAB;
	return nCode;
}


void CPnWndGrid::OnTimer(UINT nIDEvent)
{
	CPoint pt, origPt;

#ifdef _WIN32_WCE
	if (m_MouseMode == MOUSE_NOTHING)
		return;
	origPt = GetMessagePos();
#else
	if (!GetCursorPos(&origPt))
		return;
#endif
	ScreenToClient(&origPt);

	CRect rect;
	GetClientRect(rect);
	if(nIDEvent == m_uToolTipTimer)
	{
		m_nTimerCheck++;
		if(m_nTimerCheck>10)
		if(origPt==m_LastMousePoint)
		{
		}
		return;
	}
	int nFixedRowHeight = GetFixedRowHeight();
	int nFixedColWidth = GetFixedColumnWidth();
	pt = origPt;
	if (pt.y > rect.bottom)
	{
		SendMessage(WM_KEYDOWN, VK_DOWN, 0);
		if (pt.x < rect.left)  
			pt.x = rect.left;
		if (pt.x > rect.right) 
			pt.x = rect.right;
		pt.y = rect.bottom;
		OnSelecting(GetCellFromPt(pt));
	}
	else if (pt.y < nFixedRowHeight)
	{
		SendMessage(WM_KEYDOWN, VK_UP, 0);
		if (pt.x < rect.left)  
			pt.x = rect.left;
		if (pt.x > rect.right) 
			pt.x = rect.right;
		pt.y = nFixedRowHeight + 1;
		OnSelecting(GetCellFromPt(pt));
	}

	pt = origPt;
	if (pt.x > rect.right)
	{
		SendMessage(WM_KEYDOWN, VK_RIGHT, 0);
		if (pt.y < rect.top)    
			pt.y = rect.top;
		if (pt.y > rect.bottom) 
			pt.y = rect.bottom;
		pt.x = rect.right;
		OnSelecting(GetCellFromPt(pt));
	} 
	else if (pt.x < nFixedColWidth)
	{
		SendMessage(WM_KEYDOWN, VK_LEFT, 0);
		if (pt.y < rect.top)    
			pt.y = rect.top;
		if (pt.y > rect.bottom)
			pt.y = rect.bottom;
		pt.x = nFixedColWidth + 1;
		OnSelecting(GetCellFromPt(pt));
	}
}

// move about with keyboard
void CPnWndGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!IsValid(m_idCurrentCell)) 
	{
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	CCellID next = m_idCurrentCell;
	BOOL bChangeLine = FALSE;
	switch (nChar)
	{
        case VK_DELETE: 
		break;
        case VK_TAB:    
		if (IsSHIFTpressed())
		{
			if (next.col > m_nFixedCols) 
				next.col--;
			else if (next.col == m_nFixedCols && next.row > m_nFixedRows) 
			{
				next.row--; 
				next.col = GetColumnCount() - 1; 
				bChangeLine = TRUE;
			}
			else
				CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		}
		else
		{
			if (next.col < (GetColumnCount() - 1)) 
				next.col++;
			else if (next.col == (GetColumnCount() - 1) && 
				next.row < (GetRowCount() - 1) )
			{
				next.row++; 
				next.col = m_nFixedCols; 
				bChangeLine = TRUE;
			}
			else
				CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		} 
		break;
        case VK_DOWN:   
		if (next.row < (GetRowCount() - 1))
			next.row++; 
		break;            
        case VK_UP:     
		if (next.row > m_nFixedRows)           
			next.row--; 
		break;
        case VK_RIGHT:  
		if (next.col < (GetColumnCount() - 1)) 
			next.col++; 
		break;
        case VK_LEFT:   
		if (next.col > m_nFixedCols)           
			next.col--; 
		break;
        case VK_NEXT:   
		{
			CCellID idOldTopLeft = GetTopleftNonFixedCell();
			SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
			CCellID idNewTopLeft = GetTopleftNonFixedCell();
			int increment = idNewTopLeft.row - idOldTopLeft.row;
			if (increment)
			{
				next.row += increment;
				if (next.row > (GetRowCount() - 1)) 
					next.row = GetRowCount() - 1;
			}
			else
				next.row = GetRowCount() - 1;
			break;
		}
	case VK_PRIOR:  
		{
			CCellID idOldTopLeft = GetTopleftNonFixedCell();
			SendMessage(WM_VSCROLL, SB_PAGEUP, 0);
			CCellID idNewTopLeft = GetTopleftNonFixedCell();
			int increment = idNewTopLeft.row - idOldTopLeft.row;
			if (increment) 
			{
				next.row += increment;
				if (next.row < m_nFixedRows) 
					next.row = m_nFixedRows;
			}
			else
				next.row = m_nFixedRows;
			break;
		}
        case VK_HOME:   
		SendMessage(WM_VSCROLL, SB_TOP, 0);
		next.row = m_nFixedRows;
		break;
	case VK_END:    
		SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
		next.row = GetRowCount() - 1;
		break;
        default:
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	}
  
	if (next != m_idCurrentCell) 
	{
		if (m_MouseMode == MOUSE_NOTHING)
		{
			m_PrevSelectedCellMap.RemoveAll();
			m_MouseMode = m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
			if (!IsSHIFTpressed() || nChar == VK_TAB)
				m_SelectionStartCell = next;
			OnSelecting(next);
			m_MouseMode = MOUSE_NOTHING;
		}
		SetFocusCell(next);
		if (!IsCellVisible(next))
		{   
			EnsureVisible(next);
			switch (nChar)
			{
			case VK_RIGHT:  
				SendMessage(WM_HSCROLL, SB_LINERIGHT, 0); 
				break;
			case VK_LEFT:   
				SendMessage(WM_HSCROLL, SB_LINELEFT, 0);  
				break;
			case VK_DOWN:   
				SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);  
				break;
			case VK_UP:     
				SendMessage(WM_VSCROLL, SB_LINEUP, 0);    
				break;                
			case VK_TAB:    
				if (IsSHIFTpressed())
				{
					if (bChangeLine) 
					{
						SendMessage(WM_VSCROLL, SB_LINEUP, 0);
						SetScrollPos32(SB_HORZ, m_nHScrollMax);
						break;
					}
					else 
						SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
				}
				else
				{
					if (bChangeLine) 
					{
						SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
						SetScrollPos32(SB_HORZ, 0);
						break;
					}
					else 
						SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
				}
				break;
			}
			Invalidate();
		}
	}
}

void CPnWndGrid::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CPnWndGrid::OnHScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
//	if (GetFocus()->GetSafeHwnd() != GetSafeHwnd())	SetFocus();

	int scrollPos = GetScrollPos32(SB_HORZ);

	CCellID idTopLeft = GetTopleftNonFixedCell();
	CRect rect;
	GetClientRect(rect);

	switch (nSBCode)
	{
	case SB_LINERIGHT:
		if (scrollPos < m_nHScrollMax)
		{
			int xScroll = GetColumnWidth(idTopLeft.col);
			SetScrollPos32(SB_HORZ, scrollPos + xScroll);
			if (GetScrollPos32(SB_HORZ) == scrollPos)
				break;
			rect.left = GetFixedColumnWidth() + xScroll;
			ScrollWindow(-xScroll, 0, rect);
			rect.left = rect.right - xScroll;
			InvalidateRect(rect);
		}
		break;
        case SB_LINELEFT:
		if (scrollPos > 0 && idTopLeft.col > GetFixedColumnCount())
		{
			int xScroll = GetColumnWidth(idTopLeft.col-1);
			SetScrollPos32(SB_HORZ, max(0,scrollPos - xScroll));
			rect.left = GetFixedColumnWidth();
			ScrollWindow(xScroll, 0, rect);
			rect.right = rect.left + xScroll;
			InvalidateRect(rect);
		}
		break;
        case SB_PAGERIGHT:
		if (scrollPos < m_nHScrollMax)
		{
			rect.left = GetFixedColumnWidth();
			int offset = rect.Width();
			int pos = min(m_nHScrollMax, scrollPos + offset);
			SetScrollPos32(SB_HORZ, pos);
			rect.left = GetFixedColumnWidth();
			InvalidateRect(rect);
		}
		break;
        case SB_PAGELEFT:
		if (scrollPos > 0)
		{
			rect.left = GetFixedColumnWidth();
			int offset = -rect.Width();
			int pos = max(0, scrollPos + offset);
			SetScrollPos32(SB_HORZ, pos);
			rect.left = GetFixedColumnWidth();
			InvalidateRect(rect);
		}
		break;
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
		{
			SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
			CCellID idNewTopLeft = GetTopleftNonFixedCell();
			if (idNewTopLeft != idTopLeft)
			{
				rect.left = GetFixedColumnWidth();
				InvalidateRect(rect);
			}
		}
		break;
	case SB_LEFT:
		if (scrollPos > 0)
		{
			SetScrollPos32(SB_HORZ, 0);
			Invalidate();
		}
		break;
        case SB_RIGHT:
		if (scrollPos < m_nHScrollMax)
		{
			SetScrollPos32(SB_HORZ, m_nHScrollMax);
			Invalidate();
		}
		break;
        default:
		break;
	}
}

void CPnWndGrid::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
//	if (GetFocus()->GetSafeHwnd() != GetSafeHwnd())	SetFocus();

	int scrollPos = GetScrollPos32(SB_VERT);
	CCellID idTopLeft = GetTopleftNonFixedCell();

	CRect rect;
	GetClientRect(rect);

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (scrollPos < m_nVScrollMax)
		{
			int yScroll = GetRowHeight(idTopLeft.row);
			SetScrollPos32(SB_VERT, scrollPos + yScroll);
			if (GetScrollPos32(SB_VERT) == scrollPos) 
				break;
			rect.top = GetFixedRowHeight() + yScroll;
			ScrollWindow(0, -yScroll, rect);
			rect.top = rect.bottom - yScroll;
			InvalidateRect(rect);
		}
		break;
	case SB_LINEUP:
		if (scrollPos > 0 && idTopLeft.row > GetFixedRowCount())
		{
			int yScroll = GetRowHeight(idTopLeft.row-1);
			SetScrollPos32(SB_VERT, max(0, scrollPos - yScroll));
			rect.top = GetFixedRowHeight();
			ScrollWindow(0, yScroll, rect);
			rect.bottom = rect.top + yScroll;
			InvalidateRect(rect);
		}
		break;
	case SB_PAGEDOWN:
		if (scrollPos < m_nVScrollMax)
		{
			rect.top = GetFixedRowHeight();
			scrollPos = min(m_nVScrollMax, scrollPos + rect.Height());
			SetScrollPos32(SB_VERT, scrollPos);
			rect.top = GetFixedRowHeight();
			InvalidateRect(rect);
		}
		break;
	case SB_PAGEUP:
		if (scrollPos > 0)
		{
			rect.top = GetFixedRowHeight();
			int offset = -rect.Height();
			int pos = max(0, scrollPos + offset);
			SetScrollPos32(SB_VERT, pos);
			rect.top = GetFixedRowHeight();
			InvalidateRect(rect);
		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		{
			SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
			CCellID idNewTopLeft = GetTopleftNonFixedCell();
			if (idNewTopLeft != idTopLeft)
			{
				rect.top = GetFixedRowHeight();
				InvalidateRect(rect);
			}
		}
		break;
	case SB_TOP:
		if (scrollPos > 0)
		{
			SetScrollPos32(SB_VERT, 0);
			Invalidate();
		}
		break;
	case SB_BOTTOM:
		if (scrollPos < m_nVScrollMax)
		{
			SetScrollPos32(SB_VERT, m_nVScrollMax);
			Invalidate();
		}
	default:
		break;
	}
}


void CPnWndGrid::Draw(CDC* pDC)
{
	CRect rect;
	int row,col;

	CRect clipRect;
	if (pDC->GetClipBox(&clipRect) == ERROR) 
		return;

	EraseBkgnd(pDC);

	int nFixedRowHeight = GetFixedRowHeight();
	int nFixedColWidth  = GetFixedColumnWidth();

	CCellID idTopLeft = GetTopleftNonFixedCell();
	int minVisibleRow = idTopLeft.row,
		minVisibleCol = idTopLeft.col;

	CRect VisRect;
	CCellRange VisCellRange = GetVisibleNonFixedCellRange(VisRect);
	int maxVisibleRow = VisCellRange.GetMaxRow(),
	maxVisibleCol = VisCellRange.GetMaxCol();
	rect.bottom = -1;
	for (row = 0; row < m_nFixedRows; row++)
	{
		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(row)-1;
		rect.right = -1;
		for (col = 0; col < m_nFixedCols; col++)
	        {
			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col)-1;  
			DrawFixedCell(pDC, row, col, rect);
		}
	}
	rect.bottom = nFixedRowHeight-1;
	for (row = minVisibleRow; row <= maxVisibleRow; row++)
	{
		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(row)-1;
		if (rect.top > clipRect.bottom) 
			break;
		if (rect.bottom < clipRect.top)
			continue;

		rect.right = -1;
		for (col = 0; col < m_nFixedCols; col++)
		{
			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col)-1;
			if (rect.left > clipRect.right) 
				break;
			if (rect.right < clipRect.left) 
				continue;
			DrawFixedCell(pDC, row, col, rect);
		}
	}

	rect.bottom = -1;
	for (row = 0; row < m_nFixedRows; row++)
	{
		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(row)-1;
		if (rect.top > clipRect.bottom) 
			break;
		if (rect.bottom < clipRect.top) 
			continue;
		rect.right = nFixedColWidth-1;
		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{                                       
			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col)-1;
			if (rect.left > clipRect.right)
				break;
			if (rect.right < clipRect.left) 
				continue;

			DrawFixedCell(pDC, row, col, rect);
		}
	}
	rect.bottom = nFixedRowHeight-1;
	for (row = minVisibleRow; row <= maxVisibleRow; row++)
	{
		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(row)-1;

		if (rect.top > clipRect.bottom) 
			break;
		if (rect.bottom < clipRect.top) 
			continue;
		rect.right = nFixedColWidth-1;
		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{
			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col)-1;

			if (rect.left > clipRect.right)
				break;
			if (rect.right < clipRect.left) 
				continue;

			DrawCell(pDC, row, col, rect);
		}
	}
	CPen pen;
	TRY
	{
		pen.CreatePen(PS_SOLID, 0, m_crGridColour);
	}
	CATCH (CResourceException, e)
	{
		e->Delete();
		return;
	}
	END_CATCH
	pDC->SelectObject(&pen);
	if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT) 
	{
		int x = nFixedColWidth;
		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{
			x += GetColumnWidth(col);
			pDC->MoveTo(x-1, nFixedRowHeight);
			pDC->LineTo(x-1, VisRect.bottom);   
		}
	}
    
	if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ) 
	{
		int y = nFixedRowHeight;
		for (row = minVisibleRow; row <= maxVisibleRow; row++)
		{
			y += GetRowHeight(row);
			pDC->MoveTo(nFixedColWidth, y-1);    
			pDC->LineTo(VisRect.right,  y-1);
		}
	}
	pDC->SelectStockObject(NULL_PEN);

#ifdef USE_MEMDC                        // Use a memDC for flicker free update
}
#else                                   // Use normal DC - this helps in debugging
}
#endif

BOOL CPnWndGrid::IsValid(int nRow, int nCol) const
{
	return (nRow >= 0 && nRow < m_nRows && nCol >= 0 && nCol < m_nCols);
}

BOOL CPnWndGrid::IsValid(const CCellID& cell) const
{
	return IsValid(cell.row, cell.col);
}

BOOL CPnWndGrid::IsValid(const CCellRange& range) const
{
	return (range.GetMinRow() >= 0 && range.GetMinCol() >= 0 && 
	    range.GetMaxRow() >= 0 && range.GetMaxCol() >= 0 &&
	    range.GetMaxRow() < m_nRows && range.GetMaxCol() < m_nCols &&
	     range.GetMinRow() <= range.GetMaxRow() && range.GetMinCol() <= range.GetMaxCol());
}

void CPnWndGrid::SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars /* = FALSE */)
{
	TRACE(_T("%s: Setting redraw to %s\n"), 
	     GetRuntimeClass()->m_lpszClassName, bAllowDraw? _T("TRUE") : _T("FALSE"));

	if (bAllowDraw && !m_bAllowDraw) 
		Invalidate();
	m_bAllowDraw = bAllowDraw;
	if (bResetScrollBars) 
		ResetScrollBars();
}

BOOL CPnWndGrid::RedrawCell(const CCellID& cell, CDC* pDC /* = NULL */)
{
	return RedrawCell(cell.row, cell.col, pDC);
}

BOOL CPnWndGrid::RedrawCell(int nRow, int nCol, CDC* pDC /* = NULL */)
{
	BOOL bResult = TRUE;
	BOOL bMustReleaseDC = FALSE;

	if (!m_bAllowDraw || !IsCellVisible(nRow, nCol))
		return FALSE;

	CRect rect;
	if (!GetCellRect(nRow, nCol, rect)) 
		return FALSE;
	if (!pDC)
	{
		pDC = GetDC();
		if (pDC)
			bMustReleaseDC = TRUE;
	}
	if (pDC)
	{
		
		if (nRow < m_nFixedRows || nCol < m_nFixedCols)
		{
		
			bResult = DrawFixedCell(pDC, nRow, nCol, rect, TRUE);
		}
		else
		{
			bResult = DrawCell(pDC, nRow, nCol, rect, TRUE);
			CPen pen;
			TRY
			{
				pen.CreatePen(PS_SOLID, 0, GetFixedBkColor());
			} CATCH (CException, e) {e->Delete();} 	END_CATCH
			CPen* pOldPen = (CPen*) pDC->SelectObject(&pen);
			if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ) 
			{
				pDC->MoveTo(rect.left,    rect.bottom);
				pDC->LineTo(rect.right+1, rect.bottom);
			}
			if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT) 
			{
				pDC->MoveTo(rect.right, rect.top);
				pDC->LineTo(rect.right, rect.bottom+1);    
			}
			pDC->SelectObject(pOldPen);
		}
	}
	else
		InvalidateRect(rect, TRUE);

	if (bMustReleaseDC) 
		ReleaseDC(pDC);
	return bResult;
}

BOOL CPnWndGrid::RedrawRow(int row)
{
	BOOL bResult = TRUE;

	CDC* pDC = GetDC();
	for (int col = 0; col < GetColumnCount(); col++)
		bResult = RedrawCell(row, col, pDC) && bResult;
	if (pDC)
		ReleaseDC(pDC);
	m_bTitleTips=TRUE;
	OnTimer(m_uToolTipTimer);
	return bResult;
}

BOOL CPnWndGrid::RedrawColumn(int col)
{
	BOOL bResult = TRUE;

	CDC* pDC = GetDC();
	for (int row = 0; row < GetRowCount(); row++)
		bResult = RedrawCell(row, col, pDC) && bResult;
	if (pDC)
		ReleaseDC(pDC);

	return bResult;
}

CCellID CPnWndGrid::SetFocusCell(int nRow, int nCol)
{
	return SetFocusCell(CCellID(nRow, nCol));
}

CCellID CPnWndGrid::SetFocusCell(CCellID cell)
{
	if (cell == m_idCurrentCell) 
		return m_idCurrentCell;

	CCellID idPrev = m_idCurrentCell;
	m_idCurrentCell = cell;

	if (IsValid(idPrev)) 
	{
		SendMessageToParent(idPrev.row, idPrev.col, GVN_SELCHANGING); 
		SetItemState(idPrev.row, idPrev.col, 
			     GetItemState(idPrev.row, idPrev.col) & ~GVIS_FOCUSED);
		RedrawCell(idPrev);
		if (idPrev.col != m_idCurrentCell.col)
			for (int row = 0; row < m_nFixedRows; row++)
				RedrawCell(row, idPrev.col);
		if (idPrev.row != m_idCurrentCell.row)
			for (int col = 0; col < m_nFixedCols; col++) 
				RedrawCell(idPrev.row, col);
	}

	if (IsValid(m_idCurrentCell))
	{
		SetItemState(m_idCurrentCell.row, m_idCurrentCell.col, 
			     GetItemState(m_idCurrentCell.row, m_idCurrentCell.col) | GVIS_FOCUSED);
		RedrawCell(m_idCurrentCell);
		if (idPrev.col != m_idCurrentCell.col)
		{
			int row = 1;
			for (; row < m_nFixedRows; row++) 
				RedrawCell(row, m_idCurrentCell.col);
		}
		if (idPrev.row != m_idCurrentCell.row)
			for (int col = 0; col < m_nFixedCols; col++) 
				RedrawCell(m_idCurrentCell.row, col);
		SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_SELCHANGED); 
	}
	return idPrev;
}

// Sets the range of currently selected cells
void CPnWndGrid::SetSelectedRange(const CCellRange& Range, 
                                 BOOL bForceRepaint /* = FALSE */)
{
	SetSelectedRange(Range.GetMinRow(), Range.GetMinCol(), 
		     Range.GetMaxRow(), Range.GetMaxCol(),
		     bForceRepaint);
}

BOOL CPnWndGrid::IsSelectedItem(int nRow, int nCol)
{
	if (!m_bEnableSelection) return FALSE;

	for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
	{
		DWORD key;
		CCellID cell;
		m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

		if (IsValid(cell)) 
		{
			if(nRow == cell.row && nCol == cell.col)
			{
				return TRUE;
				break;
			}
		}
	}
	return FALSE;
}

BOOL CPnWndGrid::GetSelectedItem(int nNum, int* nRow, int* nCol) 
{ 
	if (!m_bEnableSelection)
		return FALSE;
	BOOL bFind=FALSE;
	int nFind=0;
	for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
	{
		DWORD key;
		CCellID cell;
		m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);
		if(nNum == nFind)
		{
			if (IsValid(cell)) 
			{
				*nRow = cell.row;
				*nCol = cell.col;

				bFind = TRUE;
				break;
			}
		}
		nFind++;
	}
	return bFind;
}

void CPnWndGrid::SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol,
                                 BOOL bForceRepaint /* = FALSE */)
{
	if (!m_bEnableSelection) return;

	CDC* pDC = NULL;
	if (bForceRepaint) pDC = GetDC();

	for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
	{
		DWORD key;
		CCellID cell;
		m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);
	        if (IsValid(cell))
		{
			SetItemState(cell.row, cell.col, 
				 GetItemState(cell.row, cell.col) & ~GVIS_SELECTED);
			if (nMinRow <= cell.row && cell.row <= nMaxRow &&
				nMinCol <= cell.col && cell.col <= nMaxCol)
				continue;
			if (bForceRepaint && pDC)
				RedrawCell(cell.row, cell.col, pDC);
			else
				InvalidateCellRect(cell);
		}
	}
	if (!GetSingleRowSelection())
	for (POSITION pos = m_PrevSelectedCellMap.GetStartPosition(); pos != NULL; /* nothing */)
	{
		DWORD key;
		CCellID cell;
		m_PrevSelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

		if (!IsValid(cell))
			continue;
		int nState = GetItemState(cell.row, cell.col);
		SetItemState(cell.row, cell.col, nState | GVIS_SELECTED);
		if (bForceRepaint && pDC)
			RedrawCell(cell.row, cell.col, pDC);
		else
			InvalidateCellRect(cell);
	}

	if (nMinRow >= 0 && nMinCol >= 0 && nMaxRow >= 0 && nMaxCol >= 0 &&
		nMaxRow < m_nRows && nMaxCol < m_nCols &&
		 nMinRow <= nMaxRow && nMinCol <= nMaxCol)
	{
		for (int row = nMinRow; row <= nMaxRow; row++)
		{
			for (int col = nMinCol; col <= nMaxCol; col++) 
			{
				int nState = GetItemState(row, col);
				if (nState & GVIS_SELECTED)
					continue;
				CCellID cell(row, col);
		                SetItemState(row, col, nState | GVIS_SELECTED);

				if (bForceRepaint && pDC)
					RedrawCell(row, col, pDC);
				else
					InvalidateCellRect(cell);
			}
		}
	}
	if (pDC != NULL) 
		ReleaseDC(pDC);
} 

void CPnWndGrid::SelectAllCells()
{
	if (!m_bEnableSelection)
		return;
	SetSelectedRange(m_nFixedRows, m_nFixedCols, GetRowCount()-1, GetColumnCount()-1);
}

void CPnWndGrid::SelectColumns(CCellID currentCell)
{
	if (!m_bEnableSelection)
		return;

	if (currentCell.col < m_nFixedCols)
		return;
	if (!IsValid(currentCell))
		return;
	SetSelectedRange(GetFixedRowCount(), 
	min(m_SelectionStartCell.col, currentCell.col), 
	GetRowCount()-1,    
	max(m_SelectionStartCell.col, currentCell.col)); 
}


void CPnWndGrid::SelectRows(CCellID currentCell)
{  
	if (!m_bEnableSelection)
		return;

	if (currentCell.row < m_nFixedRows)
		return;
	if (!IsValid(currentCell))
		return;
	if (GetListMode() && GetSingleRowSelection())
		SetSelectedRange(currentCell.row, GetFixedColumnCount(),
			currentCell.row, GetColumnCount()-1);
	else 
		SetSelectedRange(min(m_SelectionStartCell.row, currentCell.row),
			GetFixedColumnCount(), 
			max(m_SelectionStartCell.row, currentCell.row), 
			GetColumnCount()-1); 
}

// selects cells
void CPnWndGrid::SelectCells(CCellID currentCell)
{
	if (!m_bEnableSelection) 
		return;

	int row = currentCell.row;
	int col = currentCell.col;
	if (row < m_nFixedRows || col < m_nFixedCols)
		return;
	if (!IsValid(currentCell))
		return;
	SetSelectedRange(min(m_SelectionStartCell.row, row), 
		     min(m_SelectionStartCell.col, col), 
		     max(m_SelectionStartCell.row, row),
		     max(m_SelectionStartCell.col, col)); 
}

void CPnWndGrid::OnSelecting(const CCellID& currentCell)
{
	if (!m_bEnableSelection) 
		return;
	switch(m_MouseMode)
	{
	case MOUSE_SELECT_ALL:   SelectAllCells();            break;
	case MOUSE_SELECT_COL:   SelectColumns(currentCell);  break;
	case MOUSE_SELECT_ROW:   SelectRows(currentCell);     break;
	case MOUSE_SELECT_CELLS: SelectCells(currentCell);    break;
	}
}


BOOL CPnWndGrid::MouseOverRowResizeArea(CPoint& point) const
{
	if (point.x >= GetFixedColumnWidth())
		return FALSE;
	CCellID idCurrentCell = GetCellFromPt(point);
	CPoint start;
	if (!GetCellOrigin(idCurrentCell, &start)) 
		return FALSE;
	int endy = start.y + GetRowHeight(idCurrentCell.row);
	if ((point.y - start.y <= m_nResizeCaptureRange && idCurrentCell.row != 0) || 
		endy - point.y <= m_nResizeCaptureRange)
	{
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CPnWndGrid::MouseOverColumnResizeArea(CPoint& point) const
{
	if (point.y >= GetFixedRowHeight())
		return FALSE;
	CCellID idCurrentCell = GetCellFromPt(point);
	CPoint start;
	if (!GetCellOrigin(idCurrentCell, &start)) 
		return FALSE;
	int endx = start.x + GetColumnWidth(idCurrentCell.col);
	if ((point.x - start.x <= m_nResizeCaptureRange && idCurrentCell.col != 0) || 
	endx - point.x <= m_nResizeCaptureRange)
	{
		return TRUE;
	}
	else
		return FALSE;
}

CCellID CPnWndGrid::GetCellFromPt(CPoint point, BOOL bAllowFixedCellCheck /*=TRUE*/) const
{  
	CCellID idTopLeft = GetTopleftNonFixedCell();
	CCellID cellID; // return value
	int fixedColWidth = GetFixedColumnWidth();
	if (point.x < 0 || (!bAllowFixedCellCheck && point.x < fixedColWidth))
		cellID.col = -1;
	else if (point.x < fixedColWidth) // in fixed col
	{
		int col = 0, xpos = 0;
		for (col = 0; col < m_nFixedCols; col++)
		{
			xpos += GetColumnWidth(col);
			if (xpos > point.x)
				break;
		}
		cellID.col = col;
	}
	else
	{
		int col = 0, xpos = fixedColWidth;
		for (col = idTopLeft.col; col < GetColumnCount(); col++)
		{
			xpos += GetColumnWidth(col);
			if (xpos > point.x)
				break;
		}

		if (col >= GetColumnCount())
			cellID.col = -1;
		else
			cellID.col = col;
	}
    
	int fixedRowHeight = GetFixedRowHeight();
	if (point.y < 0 || (!bAllowFixedCellCheck && point.y < fixedRowHeight)) // not in window
		cellID.row = -1;
	else if (point.y < fixedRowHeight) // in fixed col
	{
		int row = 0, ypos = 0;
		for (row = 0; row < m_nFixedRows; row++)
		{
			ypos += GetRowHeight(row);
			if (ypos > point.y)
				break;
		}
		cellID.row = row;
	}
	else
	{
		int row, ypos = fixedRowHeight;
		for (row = idTopLeft.row; row < GetRowCount(); row++)
		{
			ypos += GetRowHeight(row);
			if (ypos > point.y)
				break;
		}
		if (row >= GetRowCount())
			cellID.row = -1;
		else
			cellID.row = row;
	}
	return cellID;
}

CCellID CPnWndGrid::GetTopleftNonFixedCell() const
{
	int nVertScroll = GetScrollPos(SB_VERT), 
	nHorzScroll = GetScrollPos(SB_HORZ);

	int nColumn = m_nFixedCols, nRight = 0;
	while (nRight < nHorzScroll && nColumn < (GetColumnCount()-1))
	nRight += GetColumnWidth(nColumn++);

	int nRow = m_nFixedRows, nTop = 0;
	while (nTop < nVertScroll && nRow < (GetRowCount()-1))
	nTop += GetRowHeight(nRow++);

	return CCellID(nRow, nColumn);
}

CCellRange CPnWndGrid::GetVisibleNonFixedCellRange(LPRECT pRect /*=NULL*/) const
{
	CRect rect;
	GetClientRect(rect);
	CCellID idTopLeft = GetTopleftNonFixedCell();

	int i = 0, bottom = GetFixedRowHeight();
	for (i = idTopLeft.row; i < GetRowCount(); i++)
	{
		bottom += GetRowHeight(i);
		if (bottom >= rect.bottom)
		{
			bottom = rect.bottom;
			break;
		}
	}                                
	int maxVisibleRow = min(i, GetRowCount() - 1);
	int right = GetFixedColumnWidth();
	for (i = idTopLeft.col; i < GetColumnCount(); i++)
	{
		right += GetColumnWidth(i);
		if (right >= rect.right)
		{
			right = rect.right;
			break;
		}
	}
	int maxVisibleCol = min(i, GetColumnCount() - 1);
	if (pRect)
	{
		pRect->left = pRect->top = 0;
		pRect->right = right;
		pRect->bottom = bottom;
	}
	return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}

CCellRange CPnWndGrid::GetUnobstructedNonFixedCellRange() const
{
	int	i;
	CRect rect; GetClientRect(rect);
	CCellID idTopLeft = GetTopleftNonFixedCell();

	int bottom = GetFixedRowHeight();
	for (i = idTopLeft.row; i < GetRowCount(); i++)
	{
		bottom += GetRowHeight(i);
		if (bottom >= rect.bottom)
			break;
	}
	int maxVisibleRow = min(i, GetRowCount() - 1);
	if (maxVisibleRow > 0 && bottom > rect.bottom)
		maxVisibleRow--;
	int right = GetFixedColumnWidth();
	for (i = idTopLeft.col; i < GetColumnCount(); i++)
	{
		right += GetColumnWidth(i);
		if (right >= rect.right)
			break;
	}
	int maxVisibleCol = min(i, GetColumnCount() - 1);
	if (maxVisibleCol > 0 && right > rect.right)
		maxVisibleCol--;
	return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}

CCellRange CPnWndGrid::GetSelectedCellRange() const
{
	CCellRange Selection(GetRowCount(), GetColumnCount(), -1,-1);

	for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
	{
		DWORD key;
		CCellID cell;
		m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

		Selection.SetMinRow( min(Selection.GetMinRow(), cell.row) );
		Selection.SetMinCol( min(Selection.GetMinCol(), cell.col) );
		Selection.SetMaxRow( max(Selection.GetMaxRow(), cell.row) );
		Selection.SetMaxCol( max(Selection.GetMaxCol(), cell.col) );
	}

	return Selection;
}

CCellRange CPnWndGrid::GetCellRange() const
{
	return CCellRange(0, 0, GetRowCount() - 1, GetColumnCount() - 1);
}

void CPnWndGrid::ResetSelectedRange()
{
	SetSelectedRange(-1,-1,-1,-1);
	SetFocusCell(-1,-1);
}

int CPnWndGrid::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);

	if (bGetTrackPos)
	{
		if (GetScrollInfo(nBar, &si, SIF_TRACKPOS))
			return si.nTrackPos;
	}
	else 
	{
		if (GetScrollInfo(nBar, &si, SIF_POS))
			return si.nPos;
	}
	return 0;
}

BOOL CPnWndGrid::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_POS;
	si.nPos   = nPos;
	return SetScrollInfo(nBar, &si, bRedraw);
}

void CPnWndGrid::SetScrollShow(BOOL bHScrollShow, BOOL bVScrollShow)
{
	m_bHScrollShow = bHScrollShow;
	m_bVScrollShow = bVScrollShow;
}

void CPnWndGrid::ResetScrollBars()
{
	if (!m_bAllowDraw || !::IsWindow(GetSafeHwnd())) 
		return;
	CRect rect;
	GetClientRect(rect);
	rect.left  += GetFixedColumnWidth();
	rect.top   += GetFixedRowHeight();
	if (rect.left >= rect.right || rect.top >= rect.bottom)
		return;
	CRect VisibleRect(GetFixedColumnWidth(), GetFixedRowHeight(), rect.right, rect.bottom);
	CRect VirtualRect(GetFixedColumnWidth(), GetFixedRowHeight(), GetVirtualWidth(), GetVirtualHeight());
	CCellRange visibleCells = GetUnobstructedNonFixedCellRange();
	if (!IsValid(visibleCells))
		return;
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_PAGE;
	si.nPage  = VisibleRect.Width();    SetScrollInfo(SB_HORZ, &si, FALSE); 
	si.nPage  = VisibleRect.Height();   SetScrollInfo(SB_VERT, &si, FALSE); 
	if (VisibleRect.Height() < VirtualRect.Height())
		//m_nVScrollMax = VirtualRect.Height()-18;
		m_nVScrollMax = VirtualRect.Height()-m_nDefCellHeight/2;
	else
		m_nVScrollMax = 0;
	if (VisibleRect.Width() < VirtualRect.Width())
		m_nHScrollMax = VirtualRect.Width()-1;
	else
		m_nHScrollMax = 0;
	ASSERT(m_nVScrollMax < INT_MAX && m_nHScrollMax < INT_MAX);
	if(m_bVScrollShow)
		SetScrollRange(SB_VERT, 0, m_nVScrollMax, TRUE);
	else
		m_nVScrollMax = 0;
	if(m_bHScrollShow)
		SetScrollRange(SB_HORZ, 0, m_nHScrollMax, TRUE);
	else
		m_nHScrollMax = 0;
}

BOOL CPnWndGrid::GetCellOrigin(int nRow, int nCol, LPPOINT p) const
{
	int i;
	if (!IsValid(nRow, nCol))
		return FALSE;
	if(GetColumnWidth(nCol) < 1)
		return FALSE;
	CCellID idTopLeft;
	if (nCol >= m_nFixedCols || nRow >= m_nFixedRows)
		idTopLeft = GetTopleftNonFixedCell();

	if ((nRow >= m_nFixedRows && nRow < idTopLeft.row) ||
		(nCol>= m_nFixedCols && nCol < idTopLeft.col))
		return FALSE;
	p->x = 0;
	if (nCol < m_nFixedCols)                      // is a fixed column
		for (i = 0; i < nCol; i++)
			p->x += GetColumnWidth(i);
	else
	{                                        // is a scrollable data column
		for (i = 0; i < m_nFixedCols; i++)
			p->x += GetColumnWidth(i);
		for (i = idTopLeft.col; i < nCol; i++)
		p->x += GetColumnWidth(i);
	}

	p->y = 0;
	if (nRow < m_nFixedRows)                      // is a fixed row
		for (i = 0; i < nRow; i++)
			p->y += GetRowHeight(i);
	else
	{                                        // is a scrollable data row
		for (i = 0; i < m_nFixedRows; i++)
			p->y += GetRowHeight(i);
		for (i = idTopLeft.row; i < nRow; i++)
			p->y += GetRowHeight(i);
	}

	return TRUE;
}

BOOL CPnWndGrid::GetCellOrigin(const CCellID& cell, LPPOINT p) const
{
	return GetCellOrigin(cell.row, cell.col, p);
}

BOOL CPnWndGrid::GetCellRect(const CCellID& cell, LPRECT pRect) const
{
	return GetCellRect(cell.row, cell.col, pRect);
}

BOOL CPnWndGrid::GetCellRect(int nRow, int nCol, LPRECT pRect) const
{    
	CPoint CellOrigin;
	if (!GetCellOrigin(nRow, nCol, &CellOrigin))
		return FALSE;

	pRect->left   = CellOrigin.x;
	pRect->top    = CellOrigin.y;
	pRect->right  = CellOrigin.x + GetColumnWidth(nCol)-1;
	pRect->bottom = CellOrigin.y + GetRowHeight(nRow)-1;
	return TRUE;
}

BOOL CPnWndGrid::GetTextRect(const CCellID& cell, LPRECT pRect)
{
	return GetTextRect(cell.row, cell.col, pRect);
}

BOOL CPnWndGrid::GetTextRect(int nRow, int nCol, LPRECT pRect)
{
	GV_ITEM Item;
	Item.mask = GVIF_IMAGE;
	Item.row = nRow;
	Item.col = nCol;
	if (!GetItem(&Item))
		return FALSE;
	if (!GetCellRect(nRow, nCol, pRect))
		return FALSE;
	if (m_pImageList && Item.iImage >= 0)
	{
		IMAGEINFO Info;
		if (m_pImageList->GetImageInfo(Item.iImage, &Info))
		{
			int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
			pRect->left += nImageWidth+m_nMargin;
		}
	}
	return TRUE;
}

BOOL CPnWndGrid::GetCellRangeRect(const CCellRange& cellRange, LPRECT lpRect) const
{
	CPoint MinOrigin,MaxOrigin;

	if (!GetCellOrigin(cellRange.GetMinRow(), cellRange.GetMinCol(), &MinOrigin)) 
		return FALSE;
	if (!GetCellOrigin(cellRange.GetMaxRow(), cellRange.GetMaxCol(), &MaxOrigin)) 
		return FALSE;

	lpRect->left   = MinOrigin.x;
	lpRect->top    = MinOrigin.y;
	lpRect->right  = MaxOrigin.x + GetColumnWidth(cellRange.GetMaxCol()-1);
	lpRect->bottom = MaxOrigin.y + GetRowHeight(cellRange.GetMaxRow()-1);
	return TRUE;
}

LRESULT CPnWndGrid::OnSetFont(WPARAM hFont, LPARAM /*lParam */)
{
	LRESULT result = Default();
	LOGFONT lf;
	if (!GetObject((HFONT) hFont, sizeof(LOGFONT), &lf))
		return result;

	lf.lfHeight         = 9 * 10;
	lf.lfWidth          = 0;
	lf.lfEscapement     = 0;
	lf.lfOrientation    = 0;
	lf.lfWeight         = FW_NORMAL;
	lf.lfItalic         = false;
	lf.lfUnderline      = false;
	lf.lfStrikeOut      = false;
	lf.lfCharSet        = DEFAULT_CHARSET;
	lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	lf.lfQuality        = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(lf.lfFaceName, LF_FACESIZE, _T("굴림"));

	memcpy(&m_Logfont, &lf, sizeof(LOGFONT));
	for (int row = 0; row < GetRowCount(); row++)
		for (int col = 0; col < GetColumnCount(); col++)
			SetItemFont(row, col, &lf);
	CDC* pDC = GetDC();
	if (pDC) 
	{
		m_Font.DeleteObject();
		m_Font.CreatePointFontIndirect(&m_Logfont);
		CFont* pOldFont = pDC->SelectObject(&m_Font);
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		//m_nMargin = pDC->GetTextExtent(_T(" "),1).cx;
		m_nMargin = 2;
		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);
		m_nDefCellHeight = tm.tmHeight+tm.tmExternalLeading + m_nMargin;
		m_nDefCellWidth  = tm.tmAveCharWidth*12 + 2*m_nMargin;
	}
	if (::IsWindow(GetSafeHwnd())) 
		Invalidate();
	return result;
}

LRESULT CPnWndGrid::OnGetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return (LRESULT) (HFONT) m_Font;
}



BOOL CPnWndGrid::SetFixedRowCount(int nFixedRows)
{
	ASSERT(nFixedRows >= 0);
	if (nFixedRows > GetRowCount()) 
	if (!SetRowCount(nFixedRows))
		return FALSE;
	if (m_idCurrentCell.row < nFixedRows)
		SetFocusCell(-1,-1);
	m_nFixedRows = nFixedRows;
	if (GetSafeHwnd() && m_bAllowDraw)
		Invalidate();
	
	return TRUE;
}

BOOL CPnWndGrid::SetFixedColumnCount(int nFixedCols)
{
	ASSERT(nFixedCols >= 0);
	if (nFixedCols > GetColumnCount())
		if (!SetColumnCount(nFixedCols))
			return FALSE;
	if (m_idCurrentCell.col < nFixedCols)
		SetFocusCell(-1,-1);
	m_nFixedCols = nFixedCols;
	if (GetSafeHwnd() && m_bAllowDraw)
		Invalidate();
	return TRUE;
}

BOOL CPnWndGrid::SetRowCount(int nRows)
{
	ASSERT(nRows >= 0);
	if (nRows == GetRowCount())
		return TRUE;
	if (nRows < m_nFixedRows) 
		m_nFixedRows = nRows;
	if (m_idCurrentCell.row >= nRows)
		SetFocusCell(-1,-1);
	int addedRows = nRows - GetRowCount();
	if (addedRows < 0)
	{
		for (int row = nRows; row < m_nRows; row++)
		{
			for (int col = 0; col < m_nCols; col++) 
			{
				CGridCell* pCell = GetCell(row, col);
				if (pCell)
				{
					EmptyCell(pCell, row, col);
					delete pCell;
				}
			}
			GRID_ROW* pRow = m_RowData[row];
			if (pRow)
				delete pRow;
		}
	}

	m_nRows = nRows;
	m_RowData.SetSize(m_nRows);
	m_arRowHeights.SetSize(nRows);
	if (addedRows > 0)
	{
		int startRow = nRows - addedRows;
		for (int row = startRow; row < GetRowCount(); row++)
		{
			m_arRowHeights[row] = m_nDefCellHeight;
			m_RowData[row] = new GRID_ROW;
			m_RowData[row]->SetSize(m_nCols);
			for (int col = 0; col < m_nCols; col++)
			{
				GRID_ROW* pRow = m_RowData[row];
				if (pRow)
					pRow->SetAt(col, CreateCell(row, col));
			}
		}
	}
	if (GetSafeHwnd() && m_bAllowDraw)
	{
		ResetScrollBars();
		Invalidate();
	}
	
	return TRUE;
}

BOOL CPnWndGrid::SetColumnCount(int nCols)
{
	ASSERT(nCols >= 0);
	if (nCols == GetColumnCount())
		return TRUE;
	if (nCols < m_nFixedCols) 
		m_nFixedCols = nCols;
	if (m_idCurrentCell.col >= nCols)
		SetFocusCell(-1,-1);
	int addedCols = nCols - GetColumnCount();
	if (addedCols < 0)
	{
		for (int row = 0; row < m_nRows; row++)
		{
			for (int col = nCols; col < GetColumnCount(); col++)
			{
				CGridCell* pCell = GetCell(row, col);
				if (pCell)
				{
					EmptyCell(pCell, row, col);
					delete pCell;
				}
			}
		}
		for (int col = nCols; col < GetColumnCount(); col++)
		{
			TE_CELLINFO* pInfo;
			pInfo = (TE_CELLINFO*)m_pCellInfo.GetAt(col);
			if (pInfo)
				delete pInfo;
			m_pCellInfo.RemoveAt(col);
		}
	}
	m_nCols = nCols;
	m_arColWidths.SetSize(nCols);
	m_arColType.SetSize(nCols);

	for (int i = 0; i < m_nRows; i++)
		if (m_RowData[i]) m_RowData[i]->SetSize(nCols);
	if (addedCols > 0)
	{
		int startCol = nCols - addedCols;
		for (int col = startCol; col < GetColumnCount(); col++)
		{
			m_arColWidths[col] = m_nDefCellWidth;
			m_arColType[col] = GVET_NOEDIT;
			m_pCellInfo.Add((CObject*)new TE_CELLINFO);
		}
		for (int row = 0; row < m_nRows; row++)
		{
			for (int col = startCol; col < GetColumnCount(); col++)
			{
				GRID_ROW* pRow = m_RowData[row];
				if (pRow)
					pRow->SetAt(col, CreateCell(row,col));
			}
		}
	}
	
	if (GetSafeHwnd() && m_bAllowDraw)
	{
		ResetScrollBars();
		Invalidate();
	}
	
	return TRUE;
}

int CPnWndGrid::InsertColumn(LPCTSTR strHeading, 
                            UINT nFormat /* = DT_CENTER|DT_VCENTER|DT_SINGLELINE */,
                            int nColumn  /* = -1 */,
							int nType	 /* = GVET_NOEDIT */)
{
	if (nColumn >= 0 && nColumn >= GetColumnCount())
		return -1;
	ResetSelectedRange();
	if (m_nRows < 1)
		SetRowCount(1);    
	if (nColumn < 0)
	{
		nColumn = m_nCols;
		m_arColWidths.Add(0);
		m_pCellInfo.Add((CObject*)new TE_CELLINFO);
		m_arColType.Add(nType);
		for (int row = 0; row < m_nRows; row++) 
		{
			GRID_ROW* pRow = m_RowData[row];
			if (!pRow) return -1;
			pRow->Add(CreateCell(row, nColumn));
		}
	} 
	else
	{
		m_arColWidths.InsertAt(nColumn, (UINT)0);
		m_pCellInfo.InsertAt(nColumn, (CObject*)new TE_CELLINFO);
		m_arColType.InsertAt(nColumn, nType);
		for (int row = 0; row < m_nRows; row++) 
		{
			GRID_ROW* pRow = m_RowData[row];
			if (!pRow)
				return -1;
			pRow->InsertAt(nColumn, CreateCell(row, nColumn));
		}
	}
	m_nCols++;
	SetItemText(0, nColumn, strHeading);
	for (int row = 0; row < m_nRows; row++) 
	{
		SetItemFormat(row, nColumn, nFormat);
	}
	m_arColWidths[nColumn] = GetTextExtent(strHeading).cx;
	if (m_idCurrentCell.col != -1 && nColumn < m_idCurrentCell.col)
		m_idCurrentCell.col++;
	ResetScrollBars();
	return nColumn;
}

int CPnWndGrid::InsertRow(LPCTSTR strHeading, UINT nFormat, int nRow)
{
	if (nRow >= 0 && nRow >= GetRowCount())
		return -1;

	ResetSelectedRange();
	if (m_nCols < 1) 
		SetColumnCount(1);

	if (nRow < 0) 
	{
		nRow = m_nRows;
		m_arRowHeights.Add(0);
		m_RowData.Add(new GRID_ROW);
	} 
	else 
	{
		m_arRowHeights.InsertAt(nRow, (UINT)0);
		m_RowData.InsertAt(nRow, new GRID_ROW);
	}
	m_nRows++;
	m_RowData[nRow]->SetSize(m_nCols);
	for (int col = 0; col < m_nCols; col++)
	{
		GRID_ROW* pRow = m_RowData[nRow];
		if (!pRow)
			return -1;
		pRow->SetAt(col, CreateCell(nRow, col));
	}
	SetItemText(nRow, 0, strHeading);
	SetItemFormat(nRow, 0, nFormat);
	m_arRowHeights[nRow] = GetTextExtent(strHeading).cy;
	if (m_idCurrentCell.row != -1 && nRow < m_idCurrentCell.row)
		m_idCurrentCell.row++;
	ResetScrollBars();
	return nRow;
}

CGridCell* CPnWndGrid::CreateCell(int nRow, int nCol)
{
	CGridCell* pCell = new CGridCell;
	if (!pCell)
		return NULL;
	if (nRow > 0 && nCol >= 0 && nCol < m_nCols)
		pCell->nFormat = GetItemFormat(nRow-1, nCol);

	memcpy(&(pCell->lfFont), &m_Logfont, sizeof(LOGFONT));
	return pCell;
}


void CPnWndGrid::EmptyCell(CGridCell* pCell, int nRow, int nCol)
{
	SetItemState(nRow, nCol, 0);
	pCell->szText.Empty();
}

BOOL CPnWndGrid::DeleteColumn(int nColumn)
{
	if (nColumn < 0 || nColumn >= GetColumnCount())
		return FALSE;
	m_arColType.RemoveAt(nColumn);
	ResetSelectedRange();
	for (int row = 0; row < GetRowCount(); row++) 
	{
		GRID_ROW* pRow = m_RowData[row];
		if (!pRow)
			return FALSE;

		CGridCell* pCell = pRow->GetAt(nColumn);
		if (pCell)
		{
			EmptyCell(pCell, row, nColumn);
			delete pCell;
		}
		pRow->RemoveAt(nColumn);
	}
	m_arColWidths.RemoveAt(nColumn);
	TE_CELLINFO* pInfo;
	pInfo = (TE_CELLINFO*)m_pCellInfo.GetAt(nColumn);
	if(pInfo)
		delete pInfo;
	m_pCellInfo.RemoveAt(nColumn);
	m_nCols--;
	if (nColumn < m_nFixedCols)
		m_nFixedCols--;
	if (nColumn == m_idCurrentCell.col)
		m_idCurrentCell.row = m_idCurrentCell.col = -1;
	else if (nColumn < m_idCurrentCell.col)
		m_idCurrentCell.col--;
	ResetScrollBars();
	return TRUE;
}

BOOL CPnWndGrid::DeleteRow(int nRow)
{
	if (nRow < 0 || nRow >= GetRowCount())
		return FALSE;
	GRID_ROW* pRow = m_RowData[nRow];
	if (!pRow)
		return FALSE;

	ResetSelectedRange();

	for (int col = 0; col < GetColumnCount(); col++) 
	{
		CGridCell* pCell = pRow->GetAt(col);
		if (pCell)
		{
			EmptyCell(pCell, nRow, col);
			delete pCell;
		}
	}
	delete pRow;
	m_RowData.RemoveAt(nRow);
	m_arRowHeights.RemoveAt(nRow);
	m_nRows--;
	if (nRow < m_nFixedRows)
		m_nFixedRows--;
	if (nRow == m_idCurrentCell.row)
		m_idCurrentCell.row = m_idCurrentCell.col = -1;
	else if (nRow < m_idCurrentCell.row)
		m_idCurrentCell.row--;
	ResetScrollBars();
	return TRUE;
}

BOOL CPnWndGrid::DeleteNonFixedRows()
{
	int nFixed = GetFixedRowCount();
	int nCount = GetRowCount();
	for (int nRow = nCount; nRow >= nFixed; nRow--) 
		DeleteRow(nRow);
	return TRUE;
}

BOOL CPnWndGrid::DeleteAllItems()
{
	m_arColWidths.RemoveAll();
	m_arColType.RemoveAll();
	m_arRowHeights.RemoveAll();
	
	TE_CELLINFO* pInfo;
	int icellcount = m_pCellInfo.GetSize();
	for(int i = 0 ; i < icellcount ; i++)
	{
		pInfo = (TE_CELLINFO*)m_pCellInfo.GetAt(i);
		if(pInfo)
			delete pInfo;
	}
	m_pCellInfo.RemoveAll();

	for (int row = 0; row < m_nRows; row++) 
	{
		GRID_ROW* pRow = m_RowData[row];
		if (!pRow)
			continue;
		for (int col = 0; col < m_nCols; col++)
		{
			CGridCell* pCell = pRow->GetAt(col);
			if (pCell)
			{
				EmptyCell(pCell, row, col);
				delete pCell;
			}
		}
		delete pRow;
	}
	m_RowData.RemoveAll();
	m_idCurrentCell.row = m_idCurrentCell.col = -1;
	m_nRows = m_nFixedRows = m_nCols = m_nFixedCols = 0;
	ResetScrollBars();
	return TRUE;
}

CCellID CPnWndGrid::GetNextItem(CCellID& cell, int nFlags) const
{
	if (nFlags & GVNI_ALL)
	{
		int row = cell.row , col = cell.col + 1; 
		if (row <= 0)
			row = GetFixedRowCount();
		for (; row < GetRowCount(); row++)
		{
			if (col <= 0)
				col = GetFixedColumnCount();
			for (; col < GetColumnCount(); col++) 
			{
				int nState = GetItemState(row, col);
				if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) 
					return CCellID(row, col);
				if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     
					return CCellID(row, col);
				if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   
					return CCellID(row, col);
				if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   
					return CCellID(row, col);    
				if (nFlags & GVNI_MODIFIED && nState & GVIS_MODIFIED)   
					return CCellID(row, col);
			}
	
			col=GetFixedColumnCount();
		}
	}
	else if ((nFlags & GVNI_BELOW) && (nFlags & GVNI_TORIGHT))
	{
		int row = cell.row;
		if (row <= 0)
			row = GetFixedRowCount();
		for (; row < GetRowCount(); row++)
		{
			int col = cell.col + 1;
			if (col <= 0)
				col = GetFixedColumnCount();
			for (; col < GetColumnCount(); col++) 
			{
				int nState = GetItemState(row, col);
				if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) 
					return CCellID(row, col);
				if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     
					return CCellID(row, col);
				if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   
					return CCellID(row, col);
				if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   
					return CCellID(row, col);    
				if (nFlags & GVNI_MODIFIED && nState & GVIS_MODIFIED)   
					return CCellID(row, col);
			}
		}
	}
	else if (nFlags & GVNI_ABOVE) 
	{
		for (int row = cell.row-1; row >= GetFixedRowCount(); row--) 
		{
			int nState = GetItemState(row, cell.col);
			if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED)
				return CCellID(row, cell.col);
			if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)    
				return CCellID(row, cell.col);
			if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)  
				return CCellID(row, cell.col);
			if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)  
				return CCellID(row, cell.col);
			if (nFlags & GVNI_MODIFIED && nState & GVIS_MODIFIED)   
				return CCellID(row, cell.col);
		}
	}
	else if (nFlags & GVNI_BELOW)
	{
		for (int row = cell.row+1; row < GetRowCount(); row++) 
		{
			int nState = GetItemState(row, cell.col);
			if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) 
				return CCellID(row, cell.col);
			if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)   
				return CCellID(row, cell.col);
			if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)  
				return CCellID(row, cell.col);
			if (nFlags & GVNI_READONLY && nState & GVIS_READONLY) 
				return CCellID(row, cell.col);
			if (nFlags & GVNI_MODIFIED && nState & GVIS_MODIFIED)   
				return CCellID(row, cell.col);
		}
	} 
	else if (nFlags & GVNI_TOLEFT)
	{
		for (int col = cell.col-1; col >= GetFixedColumnCount(); col--) 
		{
			int nState = GetItemState(cell.row, col);
			if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) 
				return CCellID(cell.row, col);
			if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)    
				return CCellID(cell.row, col);
			if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)  
				return CCellID(cell.row, col);
			if (nFlags & GVNI_READONLY && nState & GVIS_READONLY) 
				return CCellID(cell.row, col);
			if (nFlags & GVNI_MODIFIED && nState & GVIS_MODIFIED)   
				return CCellID(cell.row, col);
		}
	}
	else if (nFlags & GVNI_TORIGHT) 
	{
		for (int col = cell.col+1; col < GetColumnCount(); col++) 
		{
			int nState = GetItemState(cell.row, col);
			if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) 
				return CCellID(cell.row, col);
			if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)  
				return CCellID(cell.row, col);
			if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED) 
				return CCellID(cell.row, col);
			if (nFlags & GVNI_READONLY && nState & GVIS_READONLY) 
				return CCellID(cell.row, col);
			if (nFlags & GVNI_MODIFIED && nState & GVIS_MODIFIED)   
				return CCellID(cell.row, col);
		}
	}
	return CCellID(-1, -1);
}

BOOL CPnWndGrid::SortTextItems(int nCol, BOOL bAscending)
{
	ResetSelectedRange();
	SetFocusCell(-1,-1);
	return SortTextItems(nCol, bAscending, GetFixedRowCount(),-1);
}

BOOL CPnWndGrid::SortTextItems(int nCol, BOOL bAscending, int low, int high)
{
	if (nCol >= GetColumnCount())
		return FALSE;
	if (high == -1)
		high = GetRowCount() - 1;

	int lo = low;
	int hi = high;
	if (hi <= lo)
		return FALSE;
	CString midItem = GetItemText((lo+hi)/2, nCol);
	while (lo <= hi)
	{
		if (bAscending)
			while (lo < high  && GetItemText(lo, nCol) < midItem)
				++lo;
		else
			while (lo < high && GetItemText(lo, nCol) > midItem)
				++lo;
		if (bAscending)
			while (hi > low && GetItemText(hi, nCol) > midItem)
				--hi;
		else
			while (hi > low && GetItemText(hi, nCol) < midItem)
				--hi;
		if (lo <= hi)
		{
			if (GetItemText(lo, nCol) != GetItemText(hi, nCol))
			{
				for (int col = 0; col < GetColumnCount(); col++)
				{
					CGridCell *pCell = GetCell(lo, col);
					SetCell(lo, col, GetCell(hi, col));
					SetCell(hi, col, pCell);
				}
				UINT nRowHeight = m_arRowHeights[lo];
				m_arRowHeights[lo] = m_arRowHeights[hi];
				m_arRowHeights[hi] = nRowHeight;
			}
			++lo;
			--hi;
		}
	}

	if (low < hi)
		SortTextItems(nCol, bAscending, low, hi);

	if (lo < high)
		SortTextItems(nCol, bAscending, lo, high);

	return TRUE;
}

BOOL CPnWndGrid::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, 
                          LPARAM data /* = 0 */)
{
	ResetSelectedRange();
	SetFocusCell(-1,-1);
	return SortItems(pfnCompare, nCol, bAscending, data, GetFixedRowCount(), -1);
}


BOOL CPnWndGrid::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data,
                          int low, int high)
{
	if (nCol >= GetColumnCount())return FALSE;
	if (high == -1)
		high = GetRowCount() - 1;

	int lo = low;
	int hi = high;
	if (hi <= lo)
		return FALSE;
	LPARAM midItem = GetItemData( (lo+hi)/2, nCol );
	while (lo <= hi)
	{
		if (bAscending)
			while (lo < high  && pfnCompare(GetItemData(lo, nCol), midItem, data) < 0)
				++lo;
		else
			while (lo < high && pfnCompare(GetItemData(lo, nCol), midItem, data) > 0)
				++lo;
		if (bAscending)
			while (hi > low && pfnCompare(GetItemData(hi, nCol), midItem, data) > 0)
				--hi;
		else
			while (hi > low && pfnCompare(GetItemData(hi, nCol), midItem, data) < 0)
				--hi;

		if (lo <= hi)
		{
			if (pfnCompare(GetItemData(lo, nCol), GetItemData(hi, nCol), data) != 0)
			{
				for (int col = 0; col < GetColumnCount(); col++)
				{
					CGridCell *pCell = GetCell(lo, col);
					SetCell(lo, col, GetCell(hi, col));
					SetCell(hi, col, pCell);
				}
				UINT nRowHeight = m_arRowHeights[lo];
				m_arRowHeights[lo] = m_arRowHeights[hi];
				m_arRowHeights[hi] = nRowHeight;
			}
			++lo;
			--hi;
		}
	}

	if (low < hi)
		SortItems(pfnCompare, nCol, bAscending, data, low, hi);

	if (lo < high)
		SortItems(pfnCompare, nCol, bAscending, data, lo, high);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPnWndGrid data functions

BOOL CPnWndGrid::SetItem(const GV_ITEM* pItem)
{
	if (!pItem) 
		return FALSE;
	CGridCell* pCell = GetCell(pItem->row, pItem->col);
	if (!pCell) 
		return FALSE;
	if (pItem->mask & GVIF_TEXT)   pCell->szText  = pItem->szText;
	if (pItem->mask & GVIF_PARAM)  pCell->lParam  = pItem->lParam;
	if (pItem->mask & GVIF_IMAGE)  pCell->iImage  = pItem->iImage;
	if (pItem->mask & GVIF_STATE)  pCell->state   = pItem->state;
	if (pItem->mask & GVIF_FORMAT) pCell->nFormat = pItem->nFormat;
	if (pItem->mask & GVIF_BKCLR)  pCell->crBkClr = pItem->crBkClr;
	if (pItem->mask & GVIF_FGCLR)  pCell->crFgClr = pItem->crFgClr;
	if (pItem->mask & GVIF_FONT)   memcpy(&(pCell->lfFont), &(pItem->lfFont), sizeof(LOGFONT));
	return TRUE;
}

BOOL CPnWndGrid::GetItem(GV_ITEM* pItem)
{
	if (!pItem)
		return FALSE;
	CGridCell* pCell = GetCell(pItem->row, pItem->col);
	if (!pCell) 
		return FALSE;
	if (pItem->mask & GVIF_TEXT)   pItem->szText  = GetItemText(pItem->row, pItem->col);
	if (pItem->mask & GVIF_PARAM)  pItem->lParam  = pCell->lParam;
	if (pItem->mask & GVIF_IMAGE)  pItem->iImage  = pCell->iImage;
	if (pItem->mask & GVIF_STATE)  pItem->state   = pCell->state;
	if (pItem->mask & GVIF_FORMAT) pItem->nFormat = pCell->nFormat;
	if (pItem->mask & GVIF_BKCLR)  pItem->crBkClr = pCell->crBkClr;
	if (pItem->mask & GVIF_FGCLR)  pItem->crFgClr = pCell->crFgClr;
	if (pItem->mask & GVIF_FONT)   memcpy(&(pItem->lfFont), &(pCell->lfFont), sizeof(LOGFONT));

	return TRUE;
}

BOOL CPnWndGrid::SetItemText(int nRow, int nCol, LPCTSTR str)
{
	CGridCell* pCell = GetCell(nRow, nCol);
	if (!pCell)
		return FALSE;
	pCell->szText = str;
	RedrawCell(CCellID(nRow, nCol));
	return TRUE;
}

BOOL CPnWndGrid::SetItemData(int nRow, int nCol, LPARAM lParam)
{
	CGridCell* pCell = GetCell(nRow, nCol);
	if (!pCell) 
		return FALSE;
	pCell->lParam = lParam;
	return TRUE;
}

LPARAM CPnWndGrid::GetItemData(int nRow, int nCol) const
{    
	CGridCell* pCell = GetCell(nRow, nCol);
	if (!pCell) 
		return (LPARAM) 0;
	return pCell->lParam;
}

BOOL CPnWndGrid::SetItemImage(int nRow, int nCol, int iImage)
{
	CGridCell* pCell = GetCell(nRow, nCol);
	if (!pCell) 
		return FALSE;
	pCell->iImage = iImage;
	return TRUE;
}


int CPnWndGrid::GetItemImage(int nRow, int nCol) const
{
	CGridCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);
	if (!pCell) 
		return -1;
	return pCell->iImage;
}

BOOL CPnWndGrid::SetItemState(int nRow, int nCol, UINT state)
{
	CGridCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);
	if (!pCell)
		return FALSE;
	if ((pCell->state & GVIS_SELECTED) && !(state & GVIS_SELECTED))
	{
		CCellID cell;
		DWORD key = MAKELONG(nRow, nCol);
		if (m_SelectedCellMap.Lookup(key, (CCellID&)cell))
			m_SelectedCellMap.RemoveKey(key);
	}
	else if (!(pCell->state & GVIS_SELECTED) && (state & GVIS_SELECTED))
	{
		CCellID cell(nRow, nCol);
		m_SelectedCellMap.SetAt(MAKELONG(nRow, nCol), cell);
	}
	pCell->state = state;
	return TRUE;
}

UINT CPnWndGrid::GetItemState(int nRow, int nCol) const
{
	CGridCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);
	if (!pCell)
		return 0;
	return pCell->state;
}

BOOL CPnWndGrid::SetItemFormat(int nRow, int nCol, UINT nFormat)
{
	CGridCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);
	if (!pCell) 
		return FALSE;
	pCell->nFormat = nFormat;
	return TRUE;
}

UINT CPnWndGrid::GetItemFormat(int nRow, int nCol) const
{
	CGridCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);
	if (!pCell)
		return 0;
	return pCell->nFormat;
}

BOOL CPnWndGrid::SetItemBkColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
	CGridCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);
	if (!pCell) 
		return FALSE;
	pCell->crBkClr = cr;
	return TRUE;
}

COLORREF CPnWndGrid::GetItemBkColour(int nRow, int nCol) const
{
	CGridCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);
	if (!pCell)
		return 0;
	return pCell->crBkClr;
}

BOOL CPnWndGrid::SetItemFgColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
	CGridCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);
	if (!pCell)
		return FALSE;
	pCell->crFgClr = cr;
	return TRUE;
}

COLORREF CPnWndGrid::GetItemFgColour(int nRow, int nCol) const
{
	CGridCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);
	if (!pCell)
		return 0;
	return pCell->crFgClr;
}

BOOL CPnWndGrid::SetItemFont(int nRow, int nCol, LOGFONT* lf)
{
	CGridCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);
	if (!pCell)
		return FALSE;
	memcpy(&(pCell->lfFont), lf, sizeof(LOGFONT));
	return TRUE;
}

LOGFONT* CPnWndGrid::GetItemFont(int nRow, int nCol)
{
	CGridCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);
	if (!pCell) 
		return &m_Logfont;
	return &(pCell->lfFont);
}

long CPnWndGrid::GetVirtualWidth() const
{
	long lVirtualWidth = 0;
	int iColCount = GetColumnCount();
	for (int i = 0; i < iColCount; i++)
		lVirtualWidth += m_arColWidths[i];
	return lVirtualWidth;
}

long CPnWndGrid::GetVirtualHeight() const
{
	long lVirtualHeight = 0;
	int iRowCount = GetRowCount();
	for (int i = 0; i < iRowCount; i++)
		lVirtualHeight += m_arRowHeights[i];
	return lVirtualHeight;
}

int CPnWndGrid::GetRowHeight(int nRow) const
{
	ASSERT(nRow >= 0 && nRow < m_nRows);
	if (nRow < 0 || nRow >= m_nRows)
		return -1;
	return m_arRowHeights[nRow];
}

int CPnWndGrid::GetColumnWidth(int nCol) const
{
	ASSERT(nCol >= 0 && nCol < m_nCols);
	if (nCol < 0 || nCol >= m_nCols)
		return -1;
	return m_arColWidths[nCol];
}

int CPnWndGrid::GetColumnType(int nCol) const
{
	ASSERT(nCol >= 0 && nCol < m_nCols);
	if (nCol < 0 || nCol >= m_nCols)
		return -1;
	return m_arColType[nCol];
}


BOOL CPnWndGrid::SetRowHeight(int nRow, int height)
{
	ASSERT(nRow >= 0 && nRow < m_nRows && height >= 0);
	if (nRow < 0 || nRow >= m_nRows || height < 0)
		return FALSE;
	m_arRowHeights[nRow] = height;    
	return TRUE;
}

BOOL CPnWndGrid::SetColumnWidth(int nCol, int width)
{
	ASSERT(nCol >= 0 && nCol < m_nCols && width >= 0);
	if (nCol < 0 || nCol >= m_nCols || width < 0)
		return FALSE;
	m_arColWidths[nCol] = width;
	return TRUE;
}

BOOL CPnWndGrid::SetColumnType(int nCol, int nType)
{
    ASSERT(nCol >= 0 && nCol < m_nCols && nType >= 0);
    if (nCol < 0 || nCol >= m_nCols || nType < 0) return FALSE;

    m_arColType[nCol] = nType;
    return TRUE;
}

int CPnWndGrid::GetFixedRowHeight() const
{
	int nHeight = 0;
	for (int i = 0; i < m_nFixedRows; i++)
		nHeight += GetRowHeight(i);
	return nHeight;
}

int CPnWndGrid::GetFixedColumnWidth() const
{
	int nWidth = 0;
	for (int i = 0; i < m_nFixedCols; i++)
		nWidth += GetColumnWidth(i);
	return nWidth;
}

BOOL CPnWndGrid::AutoSizeColumn(int nCol)
{
	ASSERT(nCol >= 0 && nCol < m_nCols);
	if (nCol < 0 || nCol >= m_nCols)
		return FALSE;
	CSize size;
	CDC* pDC = GetDC();
	if (!pDC)
		return FALSE;
	int nWidth = 0;
	int nNumRows = GetRowCount();
	for (int nRow = 0; nRow < nNumRows; nRow++)
	{
		size = GetCellExtent(nRow, nCol, pDC);
		if (size.cx > nWidth) 
			nWidth = size.cx;
	}
	m_arColWidths[nCol] = nWidth;
	ReleaseDC(pDC);
	ResetScrollBars();
	return TRUE;
}

BOOL CPnWndGrid::AutoSizeRow(int nRow)
{
	ASSERT(nRow >= 0 && nRow < m_nRows);
	if (nRow < 0 || nRow >= m_nRows)
		return FALSE;
	CSize size;
	CDC* pDC = GetDC();
	if (!pDC)
		return FALSE;
	int nHeight = 0;
	int nNumColumns = GetColumnCount();
	for (int nCol = 0; nCol < nNumColumns; nCol++)
	{  
		size = GetCellExtent(nRow, nCol, pDC);
		if (size.cy > nHeight)
			nHeight = size.cy;
	}  
	m_arRowHeights[nRow] = nHeight;
	ReleaseDC(pDC);
	ResetScrollBars();
	return TRUE;
}

void CPnWndGrid::AutoSizeColumns()
{
	int nNumColumns = GetColumnCount();
	for (int nCol = 0; nCol < nNumColumns; nCol++)
		AutoSizeColumn(nCol);
}

void CPnWndGrid::AutoSizeRows()
{
	int nNumRows = GetRowCount();
	for (int nRow = 0; nRow < nNumRows; nRow++)
		AutoSizeRow(nRow);
}

void CPnWndGrid::AutoSize()
{
	CDC* pDC = GetDC();
	if (!pDC) return;

	int nCol, nRow;
	int nNumColumns = GetColumnCount();
	int nNumRows = GetRowCount();
	for (nCol = 0; nCol < nNumColumns; nCol++)
		m_arColWidths[nCol] = 0;

	for (nRow = 0; nRow < nNumRows; nRow++)
		m_arRowHeights[nRow] = 0;

	CSize size;
	for (nCol = 0; nCol < nNumColumns; nCol++)
	{
		for (nRow = 0; nRow < nNumRows; nRow++)
		{
			size = GetCellExtent(nRow, nCol, pDC);
			if (size.cx > (int) m_arColWidths[nCol])  
					m_arColWidths[nCol] = size.cx;
			if (size.cy > (int) m_arRowHeights[nRow]) 
					m_arRowHeights[nRow] = size.cy;
		}
	}

	ReleaseDC(pDC);

	if (m_bAllowDraw)
	{
		ResetScrollBars();
		Invalidate();
	}
}

void CPnWndGrid::ExpandColumnsToFit()
{
	if (GetColumnCount() <= 0)
		return;
	CRect rect;
	GetClientRect(rect);
	long virtualWidth = GetVirtualWidth();
	int nDifference = rect.Width() - (int) virtualWidth;
	int nColumnAdjustment = nDifference / GetColumnCount();

	for (int i = 0; i < GetColumnCount(); i++)
		m_arColWidths[i] += nColumnAdjustment;
	if (nDifference > 0)
	{
		int leftOver = nDifference % GetColumnCount();
		for (int i = 0; i < leftOver; i++)
			m_arColWidths[i] += 1;
	} 
	else 
	{
		int leftOver = (-nDifference) % GetColumnCount();
		for (int i = 0; i < leftOver; i++)
			m_arColWidths[i] -= 1;
	}
	if (m_bAllowDraw) 
		Invalidate();
}

void CPnWndGrid::ExpandRowsToFit()
{
	if (GetRowCount() <= 0)
		return;
	CRect rect;
	GetClientRect(rect);
	long virtualHeight = GetVirtualHeight();
	int nDifference = rect.Height() - (int) virtualHeight;
	int nRowAdjustment = nDifference / GetRowCount();

	for (int i = 0; i < GetRowCount(); i++)
		m_arRowHeights[i] += nRowAdjustment;
	if (nDifference > 0)
	{
		int leftOver = nDifference % GetRowCount();
		for (int i = 0; i < leftOver; i++)
			m_arRowHeights[i] += 1;
	} 
	else 
	{
		int leftOver = (-nDifference) % GetRowCount();
		for (int i = 0; i < leftOver; i++)
			m_arRowHeights[i] -= 1;
	}

	if (m_bAllowDraw) 
		Invalidate();
}

void CPnWndGrid::ExpandToFit()
{
	ExpandColumnsToFit();   // This will remove any existing horz scrollbar
	ExpandRowsToFit();      // This will remove any existing vert scrollbar
	ExpandColumnsToFit();   // Just in case the first adjustment was with a vert
}

BOOL CPnWndGrid::IsCellFixed(int nRow, int nCol)
{
	return (nRow < GetFixedRowCount() || nCol < GetFixedColumnCount());
}

void CPnWndGrid::SetModified(BOOL bModified /*=TRUE*/, int nRow /*=-1*/, int nCol /*=-1*/)
{
	if (nRow > 0 && nCol > 0)
	{
		if (bModified)
		{
			SetItemState(nRow, nCol, GetItemState(nRow, nCol) | GVIS_MODIFIED);
			m_bModified = TRUE; 
		}
		else
			SetItemState(nRow, nCol, GetItemState(nRow, nCol) & ~GVIS_MODIFIED);
	}
	else
		m_bModified = bModified;
	if (!m_bModified)
	{
		for (int row = 0; row < GetRowCount(); row++)
			for (int col = 0; col < GetColumnCount(); col++) 
				SetItemState(row, col, GetItemState(row, col) & ~GVIS_MODIFIED);
	}
}

BOOL CPnWndGrid::GetModified(int nRow /*=-1*/, int nCol /*=-1*/)
{
	if (nRow > 0 && nCol > 0)
		return ( (GetItemState(nRow, nCol) & GVIS_MODIFIED) == GVIS_MODIFIED );
	else
		return m_bModified;               
}

void CPnWndGrid::EnsureVisible(int nRow, int nCol)
{
	CCellRange VisibleCells = GetVisibleNonFixedCellRange();
	int right = nCol - VisibleCells.GetMaxCol();
	int left  = VisibleCells.GetMinCol() - nCol;
	int down  = nRow - VisibleCells.GetMaxRow();
	int up    = VisibleCells.GetMinRow() - nRow;

	while (right > 0)
	{
		SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
		right--;
	}
	while (left > 0)
	{
		SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
		left--;
	}
	while (down > 0)
	{
		SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
		down--;
	}
	while (up > 0)
	{
		SendMessage(WM_VSCROLL, SB_LINEUP, 0);
		up--;
	}

	CRect rectCell, rectWindow;
	GetCellRect(nRow, nCol, rectCell);
	GetClientRect(rectWindow);
	if (rectCell.right > rectWindow.right)
		SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
	if (rectCell.bottom > rectWindow.bottom)
		SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
}

BOOL CPnWndGrid::IsCellVisible(CCellID cell) const
{  
	return IsCellVisible(cell.row, cell.col);
}

BOOL CPnWndGrid::IsCellVisible(int nRow, int nCol) const
{  
	if (!IsWindow(m_hWnd))
		return FALSE;
	int x,y;
	CCellID TopLeft;
	if (nCol >= GetFixedColumnCount() || nRow >= GetFixedRowCount())
	{
		TopLeft = GetTopleftNonFixedCell();
		if (nCol >= GetFixedColumnCount() && nCol < TopLeft.col)
			return FALSE;
		if (nRow >= GetFixedRowCount() && nRow < TopLeft.row)
			return FALSE;
	}
	CRect rect;
	GetClientRect(rect);
	if (nCol < GetFixedColumnCount())
	{
		x = 0;
		for (int i = 0; i <= nCol; i++) 
		{
			if (x >= rect.right)
				return FALSE;
			x += GetColumnWidth(i);    
		}
	} 
	else 
	{
		x = GetFixedColumnWidth();
		for (int i = TopLeft.col; i <= nCol; i++) 
		{
			if (x >= rect.right)
				return FALSE;
			x += GetColumnWidth(i);    
		}
	}
	if (nRow < GetFixedRowCount())
	{
		y = 0;
		for (int i = 0; i <= nRow; i++) 
	        {
			if (y >= rect.bottom)
				return FALSE;
			y += GetRowHeight(i);    
		}
	} 
	else 
	{
		if (nRow < TopLeft.row)
			return FALSE;
		y = GetFixedRowHeight();
		for (int i = TopLeft.row; i <= nRow; i++) 
		{
			if (y >= rect.bottom)
				return FALSE;
			y += GetRowHeight(i);    
		}
	}
	return TRUE;
}

BOOL CPnWndGrid::InvalidateCellRect(const CCellID& cell)
{
	if (!::IsWindow(GetSafeHwnd()) || !m_bAllowDraw)
		return FALSE;
	ASSERT(IsValid(cell));
	if (!IsCellVisible(cell.row, cell.col))
		return FALSE;
	CRect rect;
	if (!GetCellRect(cell, rect))
		return FALSE;
	rect.right++; rect.bottom++;
	InvalidateRect(rect, TRUE);
	return TRUE;
}

BOOL CPnWndGrid::InvalidateCellRect(const CCellRange& cellRange)
{
	ASSERT(IsValid(cellRange));
	if (!::IsWindow(GetSafeHwnd()) || !m_bAllowDraw)
		return FALSE;
	CCellRange visibleCellRange = GetVisibleNonFixedCellRange().Intersect(cellRange);
	CRect rect;
	if (!GetCellRangeRect(visibleCellRange, rect))
		return FALSE;
	rect.right++; rect.bottom++;
	InvalidateRect(rect, TRUE);
	return TRUE;
}


#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
BOOL CPnWndGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (m_nRowsPerWheelNotch == -1)
	{
		int nPagesScrolled = zDelta / 120;
		if (nPagesScrolled > 0)
			for (int i = 0; i < nPagesScrolled; i++)
				PostMessage(WM_VSCROLL, SB_PAGEUP, 0);
		else
			for (int i = 0; i > nPagesScrolled; i--)
				PostMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
	}
	else
	{
		int nRowsScrolled = m_nRowsPerWheelNotch * zDelta / 120;
		if (nRowsScrolled > 0)
			for (int i = 0; i < nRowsScrolled; i++)
				PostMessage(WM_VSCROLL, SB_LINEUP, 0);
		else
			for (int i = 0; i > nRowsScrolled; i--)
				PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
	}

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
#endif // !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)

void CPnWndGrid::OnMouseMove(UINT nFlags, CPoint point)
{

	m_nTimerCheck = 0;
	CRect rect;
	GetClientRect(rect);

	if (!(nFlags & MK_LBUTTON) 
		|| (m_MouseMode == MOUSE_NOTHING && (nFlags & MK_LBUTTON)))
	{
	        if (m_bAllowColumnResize && MouseOverColumnResizeArea(point))
		{
			if (!(nFlags & MK_LBUTTON))
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			else
			{
				if (m_MouseMode != MOUSE_OVER_COL_DIVIDE)
				{
					SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
					m_MouseMode = MOUSE_OVER_COL_DIVIDE;
				}
			}
		}
		else if (m_bAllowRowResize && MouseOverRowResizeArea(point))
		{
			if (m_MouseMode != MOUSE_OVER_ROW_DIVIDE)
			{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
				m_MouseMode = MOUSE_OVER_ROW_DIVIDE;
			}
		}
		else if (m_MouseMode != MOUSE_NOTHING)
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			m_MouseMode = MOUSE_NOTHING;
		}
		m_LastMousePoint = point;
		return;
	}
	if (!IsValid(m_LeftClickDownCell))
	{
		m_LastMousePoint = point;
		return;
	}

	if (nFlags & MK_LBUTTON) 
	{
		switch(m_MouseMode)
		{
		case MOUSE_SELECT_ALL:        
			break;
		case MOUSE_SELECT_COL:
		case MOUSE_SELECT_ROW:    
		case MOUSE_SELECT_CELLS:    
			{
				CCellID idCurrentCell = GetCellFromPt(point);
				if (!IsValid(idCurrentCell)) 
					return;
				if (idCurrentCell != GetFocusCell())
				{
					OnSelecting(idCurrentCell);
					
					if (idCurrentCell.row >= m_nFixedRows &&
						idCurrentCell.col >= m_nFixedCols)
					{
						SetFocusCell(idCurrentCell);
					}
				}
				break;
			}
		case MOUSE_SIZING_COL:   
			{
				CDC* pDC = GetDC();
				if (!pDC)break;
				CRect oldInvertedRect(m_LastMousePoint.x, rect.top, 
				m_LastMousePoint.x + 2, rect.bottom);
				pDC->InvertRect(&oldInvertedRect);
				CRect newInvertedRect(point.x, rect.top, 
				point.x + 2, rect.bottom);
				pDC->InvertRect(&newInvertedRect);
				ReleaseDC(pDC);
				break;
			}
		case MOUSE_SIZING_ROW:        
			{
				CDC* pDC = GetDC();
				if (!pDC)
					break;
				CRect oldInvertedRect(rect.left, m_LastMousePoint.y, 
				rect.right, m_LastMousePoint.y + 2);
				pDC->InvertRect(&oldInvertedRect);
				CRect newInvertedRect(rect.left, point.y, 
				rect.right, point.y + 2);
				pDC->InvertRect(&newInvertedRect);
				ReleaseDC(pDC);
				break;
			}
		}    
	}
	m_LastMousePoint = point;
}

void CPnWndGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_MouseMode == MOUSE_OVER_COL_DIVIDE) 
	{
		CCellID cell = GetCellFromPt(point);
		ASSERT(IsValid(cell));

		CPoint start;
		if (!GetCellOrigin(0, cell.col, &start))
			return;
		if (point.x - start.x <= m_nResizeCaptureRange)     // Clicked right of border
			cell.col--;

		AutoSizeColumn(cell.col);
		Invalidate();
	} 
	else if (m_MouseMode == MOUSE_OVER_ROW_DIVIDE)
	{
		CCellID cell = GetCellFromPt(point);
		ASSERT(IsValid(cell));

		CPoint start;
		if (!GetCellOrigin(0, cell.col, &start))
			return;
		if (point.y - start.y <= m_nResizeCaptureRange)     // Clicked below border
			cell.row--;
		AutoSizeRow(cell.row);
		Invalidate();
	}

	CWnd *pOwner = GetOwner();
//	if (pOwner && IsWindow(pOwner->m_hWnd))
//		pOwner->PostMessage(WM_MOUSE_GRID, MAKELONG(GetDlgCtrlID(), MOUSE_LBUTTON_DBLCLK), 
//			    (LPARAM) MAKELONG(m_LeftClickDownCell.row, m_LeftClickDownCell.col));

	m_LastMousePoint = point;
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CPnWndGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	HWND hOldFocusWnd = ::GetFocus();
	m_LeftClickDownPoint = point;
	m_LeftClickDownCell = GetCellFromPt(point);
	if (!IsValid(m_LeftClickDownCell))
		return;

	m_SelectionStartCell = (nFlags & MK_SHIFT)? m_idCurrentCell : m_LeftClickDownCell;
	
	int nType = m_arColType[m_SelectionStartCell.col];
	if (m_LeftClickDownCell == m_idCurrentCell)
	{
		m_MouseMode = MOUSE_PREPARE_EDIT;
	}   
	else if (m_MouseMode != MOUSE_OVER_COL_DIVIDE &&
	     m_MouseMode != MOUSE_OVER_ROW_DIVIDE)        
	{
		SetFocusCell(-1,-1);
//		SetFocusCell(max(m_LeftClickDownCell.row, m_nFixedRows),
//			   max(m_LeftClickDownCell.col, m_nFixedCols));
	}

	SetCapture();

	if (m_MouseMode == MOUSE_NOTHING)
	{
		if (m_bAllowColumnResize && MouseOverColumnResizeArea(point))
		{
			if (m_MouseMode != MOUSE_OVER_COL_DIVIDE)
			{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
				m_MouseMode = MOUSE_OVER_COL_DIVIDE;
			}
		}
		else if (m_bAllowRowResize && MouseOverRowResizeArea(point))
		{
			if (m_MouseMode != MOUSE_OVER_ROW_DIVIDE)
			{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
				m_MouseMode = MOUSE_OVER_ROW_DIVIDE;
			}
		}
	}

	if (m_MouseMode == MOUSE_OVER_COL_DIVIDE) // sizing column
	{
		m_MouseMode = MOUSE_SIZING_COL;
		CPoint start;
		if (!GetCellOrigin(0, m_LeftClickDownCell.col, &start))
			return;
		CRect rect;
		GetClientRect(rect);
		CRect invertedRect(point.x, rect.top, point.x + 2, rect.bottom);

		CDC* pDC = GetDC();
		if (pDC)
		{
			pDC->InvertRect(&invertedRect);
			ReleaseDC(pDC);
		}
		if (point.x - start.x <= m_nResizeCaptureRange)
			if (!GetCellOrigin(0, --m_LeftClickDownCell.col, &start))
				return;

		rect.left = start.x;
		ClientToScreen(rect);
#ifndef _WIN32_WCE_NO_CURSOR
		ClipCursor(rect);
#endif
	}
	else if (m_MouseMode == MOUSE_OVER_ROW_DIVIDE) // sizing row
	{
		m_MouseMode = MOUSE_SIZING_ROW;
		CPoint start;
		if (!GetCellOrigin(m_LeftClickDownCell, &start))
			return;

		CRect rect;
		GetClientRect(rect);
		CRect invertedRect(rect.left, point.y, rect.right, point.y + 2);

		CDC* pDC = GetDC();
		if (pDC)
		{
			pDC->InvertRect(&invertedRect);
			ReleaseDC(pDC);
		}

		if (point.y - start.y <= m_nResizeCaptureRange)            // clicked below border
			if (!GetCellOrigin(--m_LeftClickDownCell.row, 0, &start))
				return;
		rect.top = start.y;
		ClientToScreen(rect);
#ifndef _WIN32_WCE_NO_CURSOR
		ClipCursor(rect);
#endif
	}
	else // not sizing or editing -- selecting
	{    
		m_PrevSelectedCellMap.RemoveAll();
		if (nFlags & MK_CONTROL)
		{
			for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
			{
				DWORD key;
				CCellID cell;
				m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);
				m_PrevSelectedCellMap.SetAt(key, cell);
			}
		}
		if (m_LeftClickDownCell.row < GetFixedRowCount())
			OnFixedRowClick(m_LeftClickDownCell);
		else if (m_LeftClickDownCell.col < GetFixedColumnCount())
			OnFixedColumnClick(m_LeftClickDownCell);
		else
		{
			m_MouseMode = m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
			OnSelecting(m_LeftClickDownCell);
		}
		m_nTimerID = SetTimer(WM_LBUTTONDOWN, m_nTimerInterval, 0);
	}   
	CWnd *pOwner = GetOwner();
//	if (pOwner && IsWindow(pOwner->m_hWnd))
//		pOwner->PostMessage(WM_MOUSE_GRID, MAKELONG(GetDlgCtrlID(), MOUSE_LBUTTON_DOWN), 
//		    (LPARAM) MAKELONG(m_LeftClickDownCell.row, m_LeftClickDownCell.col));

	m_LastMousePoint = point;
}

void CPnWndGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);

#ifndef _WIN32_WCE_NO_CURSOR
	ClipCursor(NULL);
#endif

	if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
	{
		ReleaseCapture();
		KillTimer(m_nTimerID);
		m_nTimerID = 0;
	}
	if (m_MouseMode == MOUSE_SIZING_COL)
	{
		CRect rect;
		GetClientRect(rect);
		CRect invertedRect(m_LastMousePoint.x, rect.top, m_LastMousePoint.x + 2, rect.bottom);

		CDC* pDC = GetDC();
		if (pDC)
		{
			pDC->InvertRect(&invertedRect);
			ReleaseDC(pDC);
		}

		if (m_LeftClickDownPoint != point) 
		{   
			CPoint start;
			if (!GetCellOrigin(m_LeftClickDownCell, &start))
				return;
			SetColumnWidth(m_LeftClickDownCell.col, point.x - start.x);
			ResetScrollBars();
			Invalidate();
		}
	}
	else if (m_MouseMode == MOUSE_SIZING_ROW)
	{
		CRect rect;
		GetClientRect(rect);
		CRect invertedRect(rect.left, m_LastMousePoint.y, rect.right, m_LastMousePoint.y + 2);

		CDC* pDC = GetDC();
		if (pDC)
		{
			pDC->InvertRect(&invertedRect);
			ReleaseDC(pDC);
		}

		if (m_LeftClickDownPoint != point) 
		{
			CPoint start;
			if (!GetCellOrigin(m_LeftClickDownCell, &start))
				return;
			SetRowHeight(m_LeftClickDownCell.row, point.y - start.y);
			ResetScrollBars();
			Invalidate();
		}
	} 

	m_MouseMode = MOUSE_NOTHING;
#ifndef _WIN32_WCE_NO_CURSOR
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#endif

	if (!IsValid(m_LeftClickDownCell))
		return;

	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->PostMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED), 
		    (LPARAM) GetSafeHwnd());

}
void CPnWndGrid::OnFixedRowClick(CCellID& cell)
{
	if (!IsValid(cell)) 
		return;

	if (m_bListMode)
	{
		if (!m_bSortOnClick) 
			return;

		CWaitCursor waiter;
		if (cell.col == m_SortColumn)
			m_bAscending = !m_bAscending;
		else 
		{
			m_bAscending = TRUE;
			m_SortColumn = cell.col;
		}
		SortTextItems(m_SortColumn, m_bAscending);
		Invalidate();
	} 
	else if (cell.col < GetFixedColumnCount()) 
	{
		m_MouseMode = MOUSE_SELECT_ALL;
		OnSelecting(cell);
	} 
	else 
	{
		m_MouseMode = MOUSE_SELECT_COL;
		OnSelecting(cell);
	}
}

void CPnWndGrid::OnFixedColumnClick(CCellID& cell)
{
	if (!IsValid(cell))
		return;
	if (cell.row < GetFixedRowCount()) 
	{
		m_MouseMode = MOUSE_SELECT_ALL;
		OnSelecting(cell);
	}
	else 
	{
		m_MouseMode = MOUSE_SELECT_ROW;
		OnSelecting(cell);
	}
}

CSize CPnWndGrid::GetTextExtent(LPCTSTR str, BOOL bUseSelectedFont /* = TRUE */)
{
	CDC* pDC = GetDC();
	if (!pDC)
		return CSize(0,0);

	CFont *pOldFont, font;
	if (bUseSelectedFont)
	{
		LOGFONT lf;
		memcpy(&lf, &m_Logfont, sizeof(LOGFONT));
		lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;
		font.CreatePointFontIndirect(&lf);
		pOldFont = pDC->SelectObject(&font);
	}
	else
		pOldFont = pDC->SelectObject(&m_Font);

	CSize size = pDC->GetTextExtent(str);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
	return size + CSize(2*m_nMargin, 2*m_nMargin);
}

CSize CPnWndGrid::GetCellExtent(int nRow, int nCol, CDC* pDC)
{
	LOGFONT *pLF = GetItemFont(nRow, nCol);


	LOGFONT lf;
	memcpy(&lf, pLF, sizeof(LOGFONT));

	if (nRow < m_nFixedRows || nCol < m_nFixedCols)
		lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;

	CFont font;
	font.CreatePointFontIndirect(&lf);

	CFont* pOldFont = pDC->SelectObject(&font);
	CSize size = pDC->GetTextExtent(GetItemText(nRow, nCol));
	pDC->SelectObject(pOldFont);

	size += CSize(4*m_nMargin, 2*m_nMargin);

	CSize ImageSize(0,0);
	if (m_pImageList)
	{
		int nImage = GetItemImage(nRow, nCol);
		if (nImage >= 0) 
		{
			IMAGEINFO Info;
			if (m_pImageList->GetImageInfo(nImage, &Info))
				ImageSize = CSize(Info.rcImage.right-Info.rcImage.left+1, 
					Info.rcImage.bottom-Info.rcImage.top+1);
		}
	}
	return CSize(size.cx + ImageSize.cx, max(size.cy, ImageSize.cy));
}

BOOL CPnWndGrid::DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
	if (!m_bAllowDraw) 
		return FALSE;

	GV_ITEM Item;
	Item.mask = GVIF_TEXT | GVIF_FORMAT | GVIF_IMAGE | GVIF_BKCLR | GVIF_FGCLR;
	Item.row = nRow;
	Item.col = nCol;
	if (!GetItem(&Item))
		return FALSE;

	bEraseBk = TRUE;
	COLORREF TextBkClr = (Item.crBkClr == CLR_DEFAULT)? GetFixedBkColor() : Item.crBkClr;
	if(bFixedRowColor && nRow >= m_nFixedRows)
	{
		if((nRow % 2) == 0)
			TextBkClr = cEvenFixedColor;
		else
			TextBkClr = cOddFixedColor;
	}
	if (bEraseBk)
	{
		CBrush brush(TextBkClr);
		pDC->FillRect(rect, &brush);
	}
	pDC->SetTextColor((Item.crFgClr == CLR_DEFAULT)? GetFixedTextColor() : Item.crFgClr);

	int nSavedDC = pDC->SaveDC();
	LOGFONT *pLF = GetItemFont(nRow, nCol);

	CCellID FocusCell = GetFocusCell();
	FocusCell.row = -1;
	FocusCell.col = -1;

	if (FocusCell.row == nRow || FocusCell.col == nCol)
	{
		static LOGFONT lf;
		memcpy(&lf, pLF, sizeof(LOGFONT));
		lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;
		pLF = &lf;
	}

	CFont Font;
	Font.CreatePointFontIndirect(pLF);
	pDC->SelectObject(&Font);

	if (IsValid(FocusCell) &&  (FocusCell.row == nRow || FocusCell.col == nCol))
	{
		rect.right++; rect.bottom++;
		pDC->DrawEdge(rect, BDR_SUNKENINNER, BF_RECT);
		rect.DeflateRect(1,1);
	}
	else
	{
		CPen lPen(PS_SOLID, 1, GetGridColor());
		CPen* pOldPen = pDC->SelectObject(&lPen);
		pDC->MoveTo(rect.right, rect.top);
		pDC->LineTo(rect.right, rect.bottom+1);
		pDC->SelectObject(pOldPen);
		lPen.DeleteObject();
		rect.DeflateRect(1,1);
	}

	int sMode = pDC->SetBkMode(TRANSPARENT);

	if (m_pImageList && Item.iImage >= 0)
	{
		IMAGEINFO Info;
		if (m_pImageList->GetImageInfo(Item.iImage, &Info))
		{
			int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
			m_pImageList->Draw(pDC, Item.iImage, rect.TopLeft(), ILD_NORMAL);
		}
	}

	pDC->DrawText((LPCTSTR)Item.szText, -1, &rect, Item.nFormat);

	pDC->RestoreDC(nSavedDC);
	Font.DeleteObject();
	pDC->SetBkMode(sMode);

	return TRUE;
}


BOOL CPnWndGrid::DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
	if (!m_bAllowDraw) 
		return FALSE;

	GV_ITEM Item;
	Item.mask = GVIF_TEXT | GVIF_FORMAT | GVIF_STATE | GVIF_IMAGE | GVIF_BKCLR | GVIF_FGCLR;
	Item.row = nRow;
	Item.col = nCol;
	if (!GetItem(&Item))
		return FALSE;

	COLORREF TextBkClr = (Item.crBkClr == CLR_DEFAULT)? GetTextBkColor() : Item.crBkClr;
	COLORREF TextClr   = (Item.crFgClr == CLR_DEFAULT)? GetTextColor()   : Item.crFgClr;

	if (bRowColor)
	{
		if((nRow % 2) == 0)
			TextBkClr = cEvenColor;
		else
			TextBkClr = cOddColor;
	}
	bEraseBk = TRUE;
	
	// check 
	CRect rcCheck = rect;
	int nSavedDC = pDC->SaveDC();

	int sMode = pDC->SetBkMode(TRANSPARENT);

	if ((Item.state & GVIS_FOCUSED) || (Item.state & GVIS_SELECTED)) 
	{
		rect.right++;
		rect.bottom++;
		
		pDC->FillSolidRect(rect, RGB(128,128,128));//::GetSysColor(COLOR_HIGHLIGHT));
		rect.right--;
		rect.bottom--;
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else
	{
		rect.right++; rect.bottom++;
		if (bEraseBk) 
		{
			CBrush brush(TextBkClr);
			pDC->FillRect(rect, &brush);
		}
		rect.right--; rect.bottom--;
		pDC->SetTextColor(TextClr);
	}

	if (Item.state & GVIS_DROPHILITED)
	{
		pDC->SelectStockObject(BLACK_PEN);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect);
	}

	LOGFONT *pLF = GetItemFont(nRow, nCol);
	CFont Font;
	Font.CreatePointFontIndirect(pLF);
	pDC->SelectObject(&Font);

	// 등락 이미지 추가  
	/*
	if (m_pImageList && Item.iImage >= 0)
	{
		IMAGEINFO Info;
		if (m_pImageList->GetImageInfo(Item.iImage, &Info))
		{
			int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
			m_pImageList->Draw(pDC, Item.iImage, rect.TopLeft(), ILD_NORMAL);
		}
	}
	*/

	// check
	int nType = m_arColType[nCol];
	pDC->DrawText((LPCTSTR)Item.szText, -1, &rect, Item.nFormat);

	pDC->RestoreDC(nSavedDC);
	Font.DeleteObject();
	pDC->SetBkMode(sMode);
	return TRUE;
}

CString CPnWndGrid::GetItemText(int nRow, int nCol)
{
	if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
	    return "";
	CGridCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);
	if (!pCell)
	    return "";
	return pCell->szText;
}

void CPnWndGrid::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonDown(nFlags, point);
	return;
	CCellID idCurrentCell = GetCellFromPt(point);
	CRect TextRect, CellRect;
	m_nPoint = point;
	SetFocus();

	SetFocusCell(idCurrentCell);
	CWnd *pOwner = GetOwner();
//	if (pOwner && IsWindow(pOwner->m_hWnd))
//		pOwner->PostMessage(WM_MOUSE_GRID, MAKELONG(GetDlgCtrlID(), MOUSE_RBUTTON_DOWN), 
//			    (LPARAM) MAKELONG(idCurrentCell.row, idCurrentCell.col));
	CWnd::OnRButtonDown(nFlags, point);
}

void CPnWndGrid::OnDestroy() 
{
	CWnd::OnDestroy();
	if (m_uToolTipTimer)
	{
		KillTimer(m_uToolTipTimer);
		m_uToolTipTimer = 0;
	}
}

void CPnWndGrid::SetForwardDBClick(BOOL bVal)
{
	m_bForwardDBClick = bVal;
}

BOOL CPnWndGrid::SetCellMask(int nColumn, int nMaxDigitalCount, BOOL bOnlyDigital, BOOL bUseComma)
{
	if (m_nCols < nColumn)
		return FALSE;

	TE_CELLINFO* pInfo;
	pInfo = (TE_CELLINFO*)m_pCellInfo.GetAt(nColumn);
	
	pInfo->bOnlyDigital	= bOnlyDigital;
	pInfo->bUseComma		= bUseComma;
	pInfo->nMaxDigitalCount	= nMaxDigitalCount;

	return TRUE;
}

TE_CELLINFO CPnWndGrid::GetCellMask(int nColumn)
{
	TE_CELLINFO* pInfo;
	pInfo = (TE_CELLINFO*)m_pCellInfo.GetAt(nColumn);

	return *pInfo;	
}

void CPnWndGrid::DrawCheckImage(CDC* dc,CRect rc,CString sText)
{
	CPoint pt;
	pt.x = rc.left + rc.Width() / 2;
	pt.y = rc.top + rc.Height() / 2;

	CRect rcRect;
	rcRect.top		= pt.y - 5;
	rcRect.bottom	= pt.y + 5;
	rcRect.left		= pt.x - 5;
	rcRect.right	= pt.x + 5;
	

	CPen blackpen(PS_SOLID, 1, RGB(0,0,0)),
	bluepen(PS_SOLID,  1, RGB(255, 0, 0)),
	*pOldPen = dc->GetCurrentPen();
    
	dc->SelectObject(&blackpen);
	dc->MoveTo(rcRect.right, rcRect.top);
	dc->LineTo(rcRect.left,  rcRect.top);
	dc->LineTo(rcRect.left,  rcRect.bottom);
	dc->LineTo(rcRect.right, rcRect.bottom);
	dc->LineTo(rcRect.right, rcRect.top);

	dc->SelectObject(pOldPen);
}

void CPnWndGrid::SetRowColor(BOOL bSet, COLORREF cOdd, COLORREF cEven)
{
	bRowColor	= bSet;
	cOddColor	= cOdd;
	cEvenColor	= cEven;
	Invalidate();
}

void CPnWndGrid::SetFixedRowColor(BOOL bSet, COLORREF cOdd, COLORREF cEven)
{
	bFixedRowColor	= bSet;
	cOddFixedColor	= cOdd;
	cEvenFixedColor	= cEven;
	Invalidate();
}

void CPnWndGrid::SetFocusRowCell(int nRow, int nCol)
{
	CCellID cell(nRow, nCol);
	if (m_MouseMode != MOUSE_NOTHING)
		return;

	m_MouseMode = MOUSE_SELECT_ROW;
	OnSelecting(cell);
	SetFocusCell(nRow);
	IsCellVisible(nRow, nCol);
	EnsureVisible(cell);
	
	ResetScrollBars();
	Invalidate();
	m_MouseMode = MOUSE_NOTHING;
}

void CPnWndGrid::killFocus()
{
//	if (GetOwner())
//		GetOwner()->SendMessage(GEV_MGR, 
//			MAKEWPARAM(mgrFocus, mfSetFocus), 0);
}

void CPnWndGrid::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
}

void CPnWndGrid::OnSetFocus(CWnd* pOldWnd) 
{
killFocus();
return;
	CWnd::OnSetFocus(pOldWnd);
}

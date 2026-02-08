// fxGrid.cpp : implementation file
//

#include "stdafx.h"

#include "fxGrid.h"
#include "memdc.h"
#include "excel9.h"

#include "InPlaceEdit.h"
#include "InPlaceCombo.h"
#include "InPlaceDate.h"

#include <afxadv.h>		// OLE stuff for clipboard operations
#include <afxconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	IDC_INPLACE_EDIT	8
#define	IDC_INPLACE_COMBO	9
#define	IDC_INPLACE_DATE	10
#define	IDC_INPLACE_TIME	11

#define	GRIDCTRL_CLASSNAME	"AxisGrid"

#define	press_SHIFTKEY()	((GetKeyState(VK_SHIFT)   & (1 << (sizeof(SHORT)*8-1))) != 0)
#define	press_CTRLKEY()		((GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0)

const int tmRTM		= 200;
const int tmINTERVAL	= 100;		// drag & drop
const int tmINTERVALx	= 500;		// column move
const int rszCAPTURE	= 3;
const int selcWEIGHT	= 600;
const int tmAUTOSCRL	= 60;

const int hdrHEIGHT	= 2;
const int footerHEIGHT	= 2;
const int leftMARGIN	= 4;
const int rightMARGIN	= 4;
const int prnGAP	= 1;

#define	ARR_UP		"↑"
#define	ARR_DN		"↓"
#define	TRI_UP		"▲"
#define	TRI_DN		"▼"

#define TM_TOOLTIP    30421

const COLORREF upCOLOR    = PALETTERGB(255,   0,   0);
const COLORREF dnCOLOR    = PALETTERGB(  0,   0, 255);
const COLORREF eqCOLOR    = PALETTERGB(  0,   0,   0);

//IMPLEMENT_DYNCREATE(CfxGrid, CWnd)

#pragma warning (disable : 26409)
#pragma warning (disable : 26496)
#pragma warning (disable : 26400)
#pragma warning (disable : 26401)
#pragma warning (disable : 26494)

void AFXAPI DDX_GridControl(CDataExchange* pDX, int nIDC, CfxGrid& rControl)
{
	if (rControl.GetSafeHwnd() == NULL)    // not subclassed yet
	{
		ASSERT(!pDX->m_bSaveAndValidate);
		HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
		if (!rControl.SubclassWindow(hWndCtrl))
		{
			ASSERT(FALSE);      // possibly trying to subclass twice?
			AfxThrowNotSupportedException();
		}
		else
		{
			// If the control has reparented itself (e.g., invisible control),
			// make sure that the CWnd gets properly wired to its control site.
			if (pDX->m_pDlgWnd->GetSafeHwnd() != ::GetParent(rControl.GetSafeHwnd()))
				rControl.AttachControlSite(pDX->m_pDlgWnd);
		}
	}
}

UINT GetMouseScrollLines()
{
	HKEY hKey{};
	int scrollLines = 3;            // reasonable default

	if (RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
				0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		TCHAR szData[128]{};
		DWORD dwKeyDataType{};
		DWORD dwDataBufSize = sizeof(szData);

		if (RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
					(LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
		{
			scrollLines = _tcstoul(szData, NULL, 10);
		}
		RegCloseKey(hKey);
	}

	return scrollLines;
}

/////////////////////////////////////////////////////////////////////////////
// CfxGrid
CfxGrid::CfxGrid(LOGFONT* logfont /*NULL*/)
{
	RegisterWindowClass();

	m_oleInit = false;
	const _AFX_THREAD_STATE* pState = AfxGetThreadState();
	if (!pState->m_bNeedTerm)
	{
		const SCODE sc = ::OleInitialize(NULL);
		if (FAILED(sc))
			AfxMessageBox(_T("OLE initialization failed. Make sure that the OLE libraries are the correct version"));
		else
			m_oleInit = true;
	}

	m_winfgCOL  = ::GetSysColor(COLOR_WINDOWTEXT);
	m_3dfaceCOL = ::GetSysColor(COLOR_3DFACE);
//	m_shadowCOL = ::GetSysColor(COLOR_3DSHADOW);
	m_shadowCOL = PALETTERGB(255, 255, 255);

	m_fgSELCOL = PALETTERGB(255, 255, 255);
	m_bkSELCOL = PALETTERGB(128, 128, 255);

	m_nRows = m_nFixedRows = m_nCols = m_nFixedCols = 0;
	m_Vscroll = m_Hscroll = 0;
	m_defHeight = 0;
	m_defWidth  = 0;
	m_margin = 0;
	m_rowCountWheel = GetMouseScrollLines();
		
	m_mousemode = mouseNOTHING;

	m_gridCOL   = ::GetSysColor(COLOR_3DDKSHADOW);
	m_gridLine  = GVLN_BOTH;
	m_gridLineType = GVLT_SOLID;
	m_bGridOutline = TRUE;

	m_focusLine = FALSE;
	m_selectLine = TRUE;
	m_dragdrop  = GVDD_NONE;
	m_enabletip = FALSE;
	m_handletabkey = TRUE;
	m_useOLE = FALSE;
	m_sensitive = FALSE;

	m_enableRowResize = FALSE;
	m_enableColResize = FALSE;

	m_ascending  = FALSE;
	m_sortColumn = -1;

	m_timerID   = 1;
	m_imageList = NULL;
	m_NOscroll  = GVSC_BOTH;
	m_DIscroll  = false;

	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));    
	CopyMemory(&m_logfont, &(ncm.lfMessageFont), sizeof(LOGFONT));
	m_logfont.lfHeight *= 10;

	if (logfont != NULL)
		CopyMemory(&m_logfont, logfont, sizeof(LOGFONT));

	m_fgCOL    = m_winfgCOL;
	m_bkCOL	   = m_shadowCOL;
	m_fixfgCOL = m_winfgCOL;
	m_fixbkCOL = m_3dfaceCOL;
	m_upCOL    = upCOLOR;
	m_dnCOL	   = dnCOLOR;
	m_eqCOL    = eqCOLOR;

	m_stepUnit = 0;
	m_stepCOL1 = m_bkCOL;
	m_stepCOL2 = m_bkCOL;

	m_mapSelCell.RemoveAll();
	m_mapPreCell.RemoveAll();

	m_mapRTM.RemoveAll();

	m_noMessage = false;

	m_excelS = _T("");
	m_excelH = NULL;

	m_keepcolumn = 0;
	m_enableMouseTrace = false;

	m_bShowZero = FALSE;
	m_bShowPlus = FALSE;
}

CfxGrid::~CfxGrid()
{
	if (m_excelH != NULL)
	{
		m_excelS = _T(""); Sleep(1000);
		TerminateThread(m_excelH, 0); Sleep(50);
	}

	m_mapMerge.RemoveAll();
	m_mapRTM.RemoveAll();

	DeleteAllItems();
	DestroyWindow();
	m_font.DeleteObject();

	if (m_oleInit) ::OleUninitialize();
}

BOOL CfxGrid::RegisterWindowClass()
{
	WNDCLASS wndcls{};
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, GRIDCTRL_CLASSNAME, &wndcls)))
	{
		wndcls.style            = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = (HBRUSH) NULL;
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = GRIDCTRL_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

CWnd* CfxGrid::Create(const RECT& rect, CWnd* parent, UINT nID, int NOscroll, int dragdrop, DWORD dwStyle)
{
	ASSERT(parent->GetSafeHwnd());

	if (!CWnd::Create(GRIDCTRL_CLASSNAME, NULL, dwStyle, rect, parent, nID)) 
		return NULL;

	m_dragdrop = (int) dragdrop;
	if (m_dragdrop != GVDD_NONE) m_dropTarget.Register(this);
	if (m_enabletip) m_tooltip.Create(this);

	m_aryRowHeight.SetSize(m_nRows);
	m_aryColWidth.SetSize(m_nCols);

	for (int ii = 0; ii < m_nRows; ii++)
		m_aryRowHeight[ii] = m_defHeight;
	for (int ii = 0; ii < m_nCols; ii++)
		m_aryColWidth[ii] = m_defWidth;

	m_NOscroll = NOscroll;

	ResetScrollBars();
	return this;
}

bool CfxGrid::Initial(int nRow, int nCol, int fixRow, int fixCol)
{
	ClearAllRTM(); ClearMerge();

	SetScrollPos32(SB_VERT, 0);

	SetRowCount(nRow);
	SetColumnCount(nCol);
	SetFixedRowCount(fixRow);
	SetFixedColumnCount(fixCol);

	return true;
}

void CfxGrid::Refresh(bool NOselect)
{
	if (NOselect)
		ResetSelectedRange();
	UpdateWindow();		// updateX_20050531
}

void CfxGrid::Adjust()
{
	ResetScrollBars(); Invalidate();
}

void CfxGrid::Clear(int sidxRow, int sidxCol)
{
	if (sidxRow < 0) sidxRow = m_nFixedRows;
	if (sidxCol < 0) sidxCol = m_nFixedCols;

	ClearAllRTM(); ResetSelectedRange();
	for (int row = sidxRow; row < m_nRows; row++) 
	{
		for (int col = sidxCol; col < m_nCols; col++)
		{
			SetItemState(row, col, 0); SetItemText (row, col, "");
			if (IsCellAttribute(CIdCell(row, col), GVAT_CHECK))
				SetItemParam(row, col, 0);
		}
	}
	m_idCurrent.row = m_idCurrent.col = -1;
	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}
}

BOOL CfxGrid::Sort(int nCol, bool ascending)
{
	if (nCol < 0 || nCol > m_nCols)
		return FALSE;

	m_ascending = ascending;
	m_sortColumn = nCol;

	return SortTextItems(nCol, ascending);
}

void CfxGrid::Blink(int nRow, int nCol, COLORREF color, int interval)
{
	if (nRow < 0 || nRow > m_nRows) return;
	if (nCol < 0 || nCol > m_nCols) return;

	COLORREF  bkcol{};
	const int idEvent = nRow * 10000 + nCol;
	if (m_mapRTM.Lookup(idEvent, bkcol))
	{
		KillTimer(idEvent);
		SetTimer(idEvent, interval, NULL);
		return;
	}

	m_sync.Lock();
	bkcol = GetItemBkColor(nRow, nCol);
	m_mapRTM.SetAt(idEvent, bkcol);
	m_sync.Unlock();

	SetItemBkColor(nRow, nCol, color, true);
	SetTimer(idEvent, interval, NULL);
}

BOOL CfxGrid::GetCellSize(int fromRow, int endRow, int& colW, int& rowH)
{
	if (fromRow < 0 || endRow > GetRowCount())
		return FALSE;

	rowH = colW = 0;

	for (int ii = fromRow; ii < endRow; ii++)
		rowH += GetRowHeight(ii);
	for (int ii = 0; ii < GetColumnCount(); ii++)
		colW += GetColumnWidth(ii);

	rowH += 1; colW += 1;
	return TRUE;
}

void CfxGrid::GetCellFromPoint(CPoint pt, int& nRow, int& nCol)
{
	CIdCell cell = GetCellFromPoint(pt);
	nRow = cell.row;
	nCol = cell.col;

	return;
}

void CfxGrid::GetTopLeftNonFixedCell(int& nRow, int& nCol)
{
	CIdCell idCell = GetTopleftNonFixedCell();

	nRow = idCell.row;
	nCol = idCell.col;

	return;
}

void CfxGrid::GetSelectedCellRange(int& minRow, int& maxRow, int& minCol, int& maxCol)
{
	const CRangeCell selection = GetSelectedCellRange();

	minRow = selection.GetMinRow();
	maxRow = selection.GetMaxRow();
	minCol = selection.GetMinCol();
	maxCol = selection.GetMaxCol();

	return;
}

void CfxGrid::ClearAllRTM()
{
	COLORREF bkcol{};
	int key{}, row{}, col{};

	m_sync.Lock();
	for (POSITION pos = m_mapRTM.GetStartPosition(); pos; )
	{
		m_mapRTM.GetNextAssoc(pos, key, bkcol);
		KillTimer(key);

		row = key / 10000;
		col = key % 10000;

		SetItemBkColor(row, col, bkcol, true);
	}
	m_mapRTM.RemoveAll();
	m_sync.Unlock();
}

LONG CfxGrid::MessageToGrid(UINT msg, int scode)
{
	m_noMessage = true;
	const LONG response = SendMessage(msg, MAKEWPARAM(scode, SB_THUMBPOSITION), 0);
	m_noMessage = false;
	
	return response;
}

UINT RunExcelThread(LPVOID lpvoid)
{
	CfxGrid* fxGrid = (CfxGrid *) lpvoid;

	fxGrid->m_syncx.Lock();
	fxGrid->RunExcel_();
	fxGrid->m_syncx.Unlock();
	return 0;
}

void CfxGrid::RunExcel(bool visible)
{
	m_syncx.Lock();

	m_excelF = visible;
	m_excelS = _T("");

	CString	text = _T(""), temps;

	GRID_ROW* pRow = NULL; CCell* cell{};
	for (int ii = 0; ii < m_nRows; ii++)
	{
		pRow = m_aryGridRows[ii];
		for (int jj = 0; jj < m_nCols; jj++)
		{
			cell = pRow->GetAt(jj);
			text = cell->text;

			if (IsCellAttribute(CIdCell(0, jj), GVAT_HIDDEN))
				continue;

			if (text.GetLength() > 0)
			{
				if (cell->attr & GVAT_CONTRAST)
				{
					switch (text[0])
					{
					case '1':		temps = ARR_UP; break;
					case '2': case '+':	temps = TRI_UP; break;
					case '4':		temps = ARR_DN; break;
					case '5': case '-':	temps = TRI_DN; break;
					default:		temps = _T("  "); break;
					}
					text = text.Mid(1); text.Insert(0, temps);
				}

				if (cell->attr & GVAT_CONDITIONx)
				{
					text.Remove('+'); text.Remove('-');
				}
			}

			m_excelS += text; m_excelS += "\t";
		}
		m_excelS += "\n";
	}
	m_syncx.Unlock();

	m_excelH = AfxBeginThread(RunExcelThread, this, THREAD_PRIORITY_HIGHEST);
}

bool CfxGrid::RunExcel_()
{
	if (m_excelS.IsEmpty())
		return false;

	::OleInitialize(NULL);

	_Application	excel;
	if (!excel.CreateDispatch("Excel.Application"))
	{
		TRACE("CreateDispatch error....\n");
		::OleUninitialize();
		return true;
	}

	COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	Workbooks  Wbs; _Workbook  Wb;
	Worksheets Wss; _Worksheet Ws;
	Range	range;

	Wbs = excel.GetWorkbooks();
	Wb  = Wbs.Add(vOpt);

	Wss = Wb.GetWorksheets();
	Ws  = Wss.Add(vOpt, vOpt, vOpt, vOpt);

	Ws.SetName("Document");

	if (m_excelS.IsEmpty())
	{
		excel.ReleaseDispatch();
		::OleUninitialize(); m_excelH = NULL;
		return false;
	}

	const	int alphaC = 24;
	CString	string; string.Format("A1:%c%d", (char)('A' + m_nCols), m_nFixedRows + m_nRows);
	if (m_nCols >= alphaC)
		string.Format("A1:%c%c%d", (char)('A' + ((m_nCols/alphaC) -1)),
					(char)('A' + m_nCols % alphaC), m_nFixedRows + m_nRows);

	range = Ws.GetRange(COleVariant((LPCTSTR)string), vOpt);
	range.Clear();

	if (m_excelS.IsEmpty())
	{
		excel.ReleaseDispatch();
		::OleUninitialize(); m_excelH = NULL;
		return false;
	}

	for (int jj = 0; m_nFixedRows && jj < m_nCols; jj++)
	{
		if (!IsCellAttribute(CIdCell(0, jj), GVAT_TEXT))
			continue;

		string.Format("%c%d:%c%d", (char)('A' + jj), m_nFixedRows, (char)('A' + jj), m_nRows);
		if (jj >= alphaC)
		{
			string.Format("%c%c%d:%c%c%d", (char)('A' + ((jj / alphaC)-1)),
							(char)('A' + jj % alphaC), m_nFixedRows,
							(char)('A' + ((jj / alphaC)-1)),
							(char)('A' + jj % alphaC), m_nRows);
		}

		Range rangex = Ws.GetRange(COleVariant((LPCTSTR)string), vOpt);
		string = _T("@"); rangex.SetNumberFormat(COleVariant(string));
	}

	long row{}, col{};
	bool last = false;
	int idx{}; CString stringX;
	for (int ii = 0; !m_excelS.IsEmpty() && ii < m_nRows; ii++)
	{
		idx = m_excelS.Find("\n");
		if (idx != -1)
		{
			stringX = m_excelS.Left(idx);
			m_excelS = m_excelS.Mid(idx+1);
		}
		else
		{
			stringX = m_excelS; m_excelS = _T(""); last = true;
		}

		for (int jj = 0; !stringX.IsEmpty() && jj < m_nCols; jj++)
		{
			idx = stringX.Find("\t");
			if (idx != -1)
			{
				string  = stringX.Left(idx);
				stringX = stringX.Mid(idx+1);
			}
			else
			{
				string = stringX; stringX = _T("");
			}

			if (string.IsEmpty())
				continue;

			TRY
			{
				row = ii+1; col = jj+1;
				range.Set_Default(COleVariant(row), COleVariant(col), COleVariant((LPCTSTR)string));
			}
			CATCH(COleDispatchException, e)
			{
				TRACE("runexcel error [%s]\n", e->m_strDescription);
				break;
			}
			END_CATCH
		}

		if (!Ws.m_lpDispatch) break;
	}

	excel.SetVisible(m_excelF);
	if (!m_excelF)
	{
		Wbs.Close(); excel.Quit();
	}

	excel.ReleaseDispatch();
	::OleUninitialize();

	m_excelH = NULL;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
///
void CfxGrid::PreSubclassWindow() 
{    
	CWnd::PreSubclassWindow();

	HFONT hFont = ::CreateFontIndirect(&m_logfont);
	OnSetFont((LPARAM)hFont, 0);
	DeleteObject(hFont);

	ResetScrollBars();
}

BOOL CfxGrid::SubclassWindow(HWND hWnd) 
{    
	if (!CWnd::SubclassWindow(hWnd))
		return FALSE;

//	m_dropTarget.Register(this);

	if (m_enabletip && !IsWindow(m_tooltip.m_hWnd))
		m_tooltip.Create(this);

	return TRUE;
}

LRESULT CfxGrid::MessageToParent(int nRow, int nCol, int nMessage)
{
	if (!IsWindow(m_hWnd))
		return 0;

	NM_GVITEM nmgv;

	nmgv.row = nRow;
	nmgv.col = nCol;

	nmgv.hdr.hwndFrom = m_hWnd;
	nmgv.hdr.idFrom   = GetDlgCtrlID();
	nmgv.hdr.code     = nMessage;

	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
	
	return 0;
}

BEGIN_MESSAGE_MAP(CfxGrid, CWnd)
	//{{AFX_MSG_MAP(CfxGrid)
	ON_WM_PAINT()
	ON_WM_GETDLGCODE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_CAPTURECHANGED()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_NOTIFY(GVNM_ENDEDIT,  IDC_INPLACE_EDIT,  OnEndInPlaceEdit)
	ON_NOTIFY(GVNM_ENDCOMBO, IDC_INPLACE_COMBO, OnEndInPlaceCombo)
	ON_NOTIFY(GVNM_ENDDATE,  IDC_INPLACE_DATE,  OnEndInPlaceDate)
	ON_NOTIFY(GVNM_ENDTIME,  IDC_INPLACE_TIME,  OnEndInPlaceTime)
	ON_NOTIFY(GVNM_SELCHANGECOMBO, IDC_INPLACE_COMBO, OnSelChangeInPlaceCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CfxGrid message handlers
void CfxGrid::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC MemDC(&dc);
	OnDraw(&MemDC);
}

void CfxGrid::EraseBkgnd(CDC* pDC) 
{
	CRect  visRect, clipRect, rect;
	CBrush fixedBack(m_fixbkCOL), back(m_bkCOL);

	if (pDC->GetClipBox(clipRect) == ERROR) 
		return;

	GetVisibleNonFixedCellRange(visRect);

	// Draw Fixed columns background
	const int fixedColWidth = GetFixedColumnWidth();
	if (clipRect.left < fixedColWidth && clipRect.top < visRect.bottom)
		pDC->FillRect(CRect(clipRect.left, clipRect.top, fixedColWidth, visRect.bottom), &fixedBack);
        
	// Draw Fixed rows background
	const int fixedRowHeight = GetFixedRowHeight();
	if (clipRect.top < fixedRowHeight && clipRect.right > fixedColWidth && clipRect.left < visRect.right)
		pDC->FillRect(CRect(fixedColWidth -1, clipRect.top, visRect.right, fixedRowHeight), &fixedBack);

	// Draw non-fixed cell background
	if (rect.IntersectRect(visRect, clipRect)) 
	{
		CRect cellRect(max(fixedColWidth, rect.left),  max(fixedRowHeight, rect.top),
					rect.right, rect.bottom);
		pDC->FillRect(cellRect, &back);
	}

	// Draw right hand side of window outside grid
	if (visRect.right < clipRect.right)
		pDC->FillRect(CRect(visRect.right, clipRect.top, clipRect.right, clipRect.bottom), &back);

	// Draw bottom of window below grid
	if (visRect.bottom < clipRect.bottom && clipRect.left < visRect.right) 
		pDC->FillRect(CRect(clipRect.left, visRect.bottom, visRect.right, clipRect.bottom), &back);
}

UINT CfxGrid::OnGetDlgCode() 
{
	UINT nCode = DLGC_WANTARROWS|DLGC_WANTCHARS;

	if (m_handletabkey && !press_CTRLKEY()) 
		nCode |= DLGC_WANTTAB;

	return nCode;
}

void CfxGrid::OnSysColorChange() 
{
	CWnd::OnSysColorChange();
    
	if (m_fgCOL == m_winfgCOL)	m_fgCOL = ::GetSysColor(COLOR_WINDOWTEXT);
	if (m_bkCOL == m_shadowCOL)	m_bkCOL = ::GetSysColor(COLOR_3DSHADOW);

	if (m_fixfgCOL = m_winfgCOL)	m_fixfgCOL = ::GetSysColor(COLOR_WINDOWTEXT);
	if (m_fixbkCOL = m_3dfaceCOL)	m_fixbkCOL = ::GetSysColor(COLOR_3DFACE);

	m_winfgCOL = ::GetSysColor(COLOR_WINDOWTEXT);
	m_3dfaceCOL  = ::GetSysColor(COLOR_3DFACE);
	m_shadowCOL  = ::GetSysColor(COLOR_3DSHADOW);
}

void CfxGrid::OnCaptureChanged(CWnd *pWnd) 
{
	if (pWnd->GetSafeHwnd() == GetSafeHwnd())
		return;

	if (m_timerID != 0)
	{
		KillTimer(m_timerID);
		m_timerID = 0;
	}

	if (m_mousemode == mouseDRAGGING)
		m_mousemode = mouseNOTHING;
}

void CfxGrid::OnTimer(UINT nIDEvent)
{
	if (nIDEvent != WM_LBUTTONDOWN)
	{
		KillTimer(nIDEvent);
		if (nIDEvent == TM_TOOLTIP)
		{
			CPoint point;
			if (!GetCursorPos(&point))
				return;
			ScreenToClient(&point);
			CRect textRC, cellRC;
			const CIdCell current = GetCellFromPoint(point);
			
			if (GetTextRect(current.row, current.col, textRC)
				&& GetCellRect(current.row, current.col, cellRC)
				&& IsCellAttribute(current, GVAT_TOOLTIP))
			{
				
				const LPARAM lParam = GetItemParam(current.row, current.col);
				CString string; int icol{};
				if (lParam != 0)
				{
					string = GetItemText(current.row, lParam);
					icol = lParam;
				}
				else
				{
					string = GetItemText(current.row, current.col);//lParam);
					icol = current.col;
				}
				//CString sparam; sparam.Format("%d", lParam);
				//AfxMessageBox(string);
				GVITEM gvitem;

				gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_IMAGE|GVMK_BKCOLOR|GVMK_FGCOLOR|GVMK_STATE|GVMK_ATTR|GVMK_PARAM|GVMK_MISC;
				gvitem.row  = current.row;
				gvitem.col  = icol;

				if (!GetItem(&gvitem)) return;
				//CString string2 = gvitem.text;
				DisplayAttribute(gvitem.attr, string);
				
				//string2.Format("%s[%d]",string2, current.row);
				if (!string.IsEmpty())
					m_tooltip.Show(textRC, string, 0, cellRC, GetItemFont(current.row, lParam));
			}

			return;
		}

		COLORREF  bkcol{};
		const int row = nIDEvent / 10000;
		const int col = nIDEvent % 10000;

		if (!m_mapRTM.Lookup(nIDEvent, bkcol))
			return;

		m_sync.Lock();
		m_mapRTM.RemoveKey(nIDEvent);
		m_sync.Unlock();

		SetItemBkColor(row, col, bkcol, true);
		return;
	}

	if (IsCellAttribute(m_idSelect, GVAT_DRAGDROP)
			&& GetItemState(m_idSelect.row, m_idSelect.col) & GVST_SELECTED)
	{
		KillTimer(nIDEvent);
		m_mousemode = mousePREPAREDRAG;
		OnBeginDrag();
		return;
	}
	else if (IsCellAttribute(m_idSelect, GVAT_DRAGMOVE))
	{
		KillTimer(nIDEvent);
		m_mousemode = mouseMOVING;
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));

		ResetSelectedRange();

		CRect clientRC; GetClientRect(&clientRC);
		ClientToScreen(&clientRC);
		ClipCursor(clientRC);
		return;
	}

	CPoint pt, origPt;
	if (!GetCursorPos(&origPt))
		return;

	ScreenToClient(&origPt);
	CRect rect; GetClientRect(rect);

	const int nFixedRowHeight = GetFixedRowHeight();
	const int nFixedColWidth = GetFixedColumnWidth();

	pt = origPt;
	if (pt.y > rect.bottom)
	{
		SendMessage(WM_KEYDOWN, VK_DOWN, 0);

		if (pt.x < rect.left)  pt.x = rect.left;
		if (pt.x > rect.right) pt.x = rect.right;
		pt.y = rect.bottom;

		OnSelecting(GetCellFromPoint(pt));
	}
	else if (pt.y < nFixedRowHeight)
	{
		SendMessage(WM_KEYDOWN, VK_UP, 0);

		if (pt.x < rect.left)  pt.x = rect.left;
		if (pt.x > rect.right) pt.x = rect.right;
		pt.y = nFixedRowHeight + 1;

		OnSelecting(GetCellFromPoint(pt));
	}

	pt = origPt;
	if (pt.x > rect.right)
	{
		SendMessage(WM_KEYDOWN, VK_RIGHT, 0);

		if (pt.y < rect.top)	pt.y = rect.top;
		if (pt.y > rect.bottom) pt.y = rect.bottom;
		pt.x = rect.right;

		OnSelecting(GetCellFromPoint(pt));
	} 
	else if (pt.x < nFixedColWidth)
	{
		SendMessage(WM_KEYDOWN, VK_LEFT, 0);

		if (pt.y < rect.top)	pt.y = rect.top;
		if (pt.y > rect.bottom)	pt.y = rect.bottom;
		pt.x = nFixedColWidth + 1;

		OnSelecting(GetCellFromPoint(pt));
	}
}

void CfxGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!IsValid(m_idCurrent)) 
	{
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	CIdCell nextCell = m_idCurrent;
	BOOL bChangeLine = FALSE;

	if (press_CTRLKEY() && m_useOLE)
	{
		switch (nChar)
		{
		case 'A':
			OnEditSelectAll();  break;
		case 'X':
			OnEditCut();        break;
		case 'C':
			OnEditCopy();       break;
		case 'V':
			OnEditPaste();      break;
		}
	}

	switch (nChar)
	{
	case VK_DELETE:
		if (IsCellAttribute(m_idCurrent, GVAT_EDIT))
		{
			MessageToParent(m_idCurrent.row, m_idCurrent.col, GVNM_BEGINEDIT);
			SetItemText(m_idCurrent.row, m_idCurrent.col, _T(""));
 			MessageToParent(m_idCurrent.row, m_idCurrent.col, GVNM_ENDEDIT);
			RedrawCell(m_idCurrent);
		}
		break;
        case VK_TAB:
		if (press_SHIFTKEY())
		{
			if (nextCell.col > m_nFixedCols)
			{
				nextCell.col--;
				for (int ii = nextCell.col; ii > m_nFixedCols; ii--)
				{
					if (!IsCellAttribute(nextCell, GVAT_HIDDEN))
						break;
					nextCell.col--;
				}
			}
			else if (nextCell.col == m_nFixedCols && nextCell.row > m_nFixedRows) 
			{
				nextCell.row--; 
				nextCell.col = GetColumnCount() - 1; 
				bChangeLine = TRUE;
			}
			else
				CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		}
		else
		{
			if (nextCell.col < (GetColumnCount() - 1)) 
			{
				nextCell.col++;
				for (int ii = nextCell.col; ii < m_nCols; ii++)
				{
					if (!IsCellAttribute(nextCell, GVAT_HIDDEN))
						break;
					nextCell.col++;
				}
			}
			else if (nextCell.col == (GetColumnCount() - 1) && nextCell.row < (GetRowCount() - 1))
			{
				nextCell.row++; 
				nextCell.col = m_nFixedCols; 
				bChangeLine = TRUE;
			}
			else
				CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		} 
		break;
	case VK_DOWN:
	case VK_RETURN:
		if (nextCell.row < (GetRowCount() -1))
		{
			nextCell.row++;
			for (int ii = 0; ii < m_nRows; ii++)
			{
				if (GetRowHeight(nextCell.row) > 0)
					break;
				nextCell.row++;
			}
		}
		break;
	case VK_UP:
		if (nextCell.row > m_nFixedRows)
		{
			nextCell.row--;
			for (int ii = nextCell.row; ii > m_nFixedRows; ii--)
			{
				if (GetRowHeight(nextCell.row) > 0)
					break;
				nextCell.row--;
			}
		}
		break;
        case VK_RIGHT:  
		if (nextCell.col < (GetColumnCount() - 1))
		{
			nextCell.col++;
			for (int ii = nextCell.col; ii < m_nCols; ii++)
			{
				if (!IsCellAttribute(nextCell, GVAT_HIDDEN)
						&& GetColumnWidth(nextCell.col) > 0)
					break;
				nextCell.col++;
			}
		}
		break;
        case VK_LEFT:   
		if (nextCell.col > m_nFixedCols)
		{
			nextCell.col--;
			for (int ii = nextCell.col; ii > m_nFixedCols; ii--)
			{
				if (!IsCellAttribute(nextCell, GVAT_HIDDEN)
						&& GetColumnWidth(nextCell.col) > 0)
					break;
				nextCell.col--;
			}
		}
		break;
        case VK_NEXT:   
		{
			const CIdCell oldCell = GetTopleftNonFixedCell();
			SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0);

			const CIdCell newCell = GetTopleftNonFixedCell();

			const int increment = newCell.row - oldCell.row;
			if (increment)
			{
				nextCell.row += increment;
				if (nextCell.row > (GetRowCount() - 1)) 
					nextCell.row = GetRowCount() - 1;
			}
			else
				nextCell.row = GetRowCount() - 1;
		}
		break;
            case VK_PRIOR:  
		{
				const CIdCell oldCell = GetTopleftNonFixedCell();
			SendMessage(WM_VSCROLL, SB_PAGEUP, 0);

			const CIdCell newCell = GetTopleftNonFixedCell();

			const int increment = newCell.row - oldCell.row;
			if (increment) 
			{
				nextCell.row += increment;
				if (nextCell.row < m_nFixedRows) 
					nextCell.row = m_nFixedRows;
			}
			else
				nextCell.row = m_nFixedRows;
		}
		break;
        case VK_HOME:   
		SendMessage(WM_VSCROLL, SB_TOP, 0);
		nextCell.row = m_nFixedRows;
		break;
        case VK_END:    
		SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
		nextCell.row = GetRowCount() - 1;
		break;
        default:
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		break;
	}
  
	if (nextCell != m_idCurrent)
	{
		if (m_mousemode == mouseNOTHING)
		{
			m_mapPreCell.RemoveAll();
			// updateX_20050328
			m_mousemode = (m_dragdrop == GVDD_NONE && !m_selectLine) ? mouseSELECTCELLS : mouseSELECTROW;
			if (!press_SHIFTKEY() || nChar == VK_TAB)
				m_idSelect = nextCell;
			OnSelecting(nextCell);
			m_mousemode = mouseNOTHING;
		}
		SetFocusCell(nextCell);

		if (!IsCellVisible(nextCell))
		{   
			EnsureVisible(nextCell); // Make sure cell is visible

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
				if (press_SHIFTKEY())
				{
					if (bChangeLine) 
					{
						SendMessage(WM_VSCROLL, SB_LINEUP, 0);
						SetScrollPos32(SB_HORZ, m_Hscroll);
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

void CfxGrid::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!press_CTRLKEY() && m_mousemode == mouseNOTHING)
	{
		if (!m_handletabkey)
			OnEditCell(m_idCurrent.row, m_idCurrent.col, nChar);
		else if (nChar != VK_TAB && nChar != VK_RETURN)
			OnEditCell(m_idCurrent.row, m_idCurrent.col, nChar);
	}
	
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CfxGrid::OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{    
	const NM_USER *nmuser = (NM_USER *)pNMHDR;
	const GVITEM  *gvitem = &nmuser->item;

	// In case OnEndInPlaceEdit called as window is being destroyed
	if (!IsWindow(GetSafeHwnd()))
		return;

	OnEndEditCell(gvitem->row, gvitem->col, gvitem->text);
	InvalidateCellRect(CIdCell(gvitem->row, gvitem->col));

	switch (gvitem->param)		// for kiwoom
	{
	case VK_TAB:
		MessageToParent(gvitem->row, gvitem->col, GVNM_ENDTABKEY);	break;
	case VK_RETURN:
		MessageToParent(gvitem->row, gvitem->col, GVNM_ENDRETURN);	break;
	default:
		MessageToParent(gvitem->row, gvitem->col, GVNM_ENDEDIT);	break;
	}

	switch (gvitem->param) 
	{
	case VK_DOWN: 
	case VK_UP:   
	case VK_RIGHT:
	case VK_LEFT:  
	case VK_NEXT:  
	case VK_PRIOR: 
	case VK_HOME:  
	case VK_END:    
		OnKeyDown(gvitem->param, 0, 0);
		OnEditCell(m_idCurrent.row, m_idCurrent.col, gvitem->param);
		break;
	}

	*pResult = 0;
}

void CfxGrid::OnEndInPlaceCombo(NMHDR* pNMHDR, LRESULT* pResult) 
{    
	const NM_USER *nmuser = (NM_USER *)pNMHDR;
	const GVITEM  *gvitem = &nmuser->item;

	// In case OnEndInPlacecombo called as window is being destroyed
	if (!IsWindow(GetSafeHwnd()))
		return;

	OnEndComboCell(gvitem->row, gvitem->col, gvitem->text, gvitem->param);
	InvalidateCellRect(CIdCell(gvitem->row, gvitem->col));
	MessageToParent(gvitem->row, gvitem->col, GVNM_ENDCOMBO);

	*pResult = 0;
}

void CfxGrid::OnSelChangeInPlaceCombo(NMHDR* pNMHDR, LRESULT* pResult) 
{    
	const NM_USER *nmuser = (NM_USER *)pNMHDR;
	const GVITEM  *gvitem = &nmuser->item;
	if (!IsWindow(GetSafeHwnd()))
		return;
	OnEndComboCell(gvitem->row, gvitem->col, gvitem->text, gvitem->param);
	MessageToParent(gvitem->row, gvitem->col, GVNM_SELCHANGECOMBO);
	
	*pResult = 0;
}

void CfxGrid::OnEndInPlaceDate(NMHDR* pNMHDR, LRESULT* pResult) 
{    
	const NM_USER *nmuser = (NM_USER *)pNMHDR;
	const GVITEM  *gvitem = &nmuser->item;

	// In case OnEndInPlacecombo called as window is being destroyed
	if (!IsWindow(GetSafeHwnd()))
		return;

	OnEndDateCell(gvitem->row, gvitem->col, gvitem->text);
	InvalidateCellRect(CIdCell(gvitem->row, gvitem->col));
	MessageToParent(gvitem->row, gvitem->col, GVNM_ENDDATE);

	*pResult = 0;
}

void CfxGrid::OnEndInPlaceTime(NMHDR* pNMHDR, LRESULT* pResult) 
{    
	const NM_USER *nmuser = (NM_USER *)pNMHDR;
	const GVITEM  *gvitem = &nmuser->item;

	// In case OnEndInPlacecombo called as window is being destroyed
	if (!IsWindow(GetSafeHwnd()))
		return;

	OnEndTimeCell(gvitem->row, gvitem->col, gvitem->text);
	InvalidateCellRect(CIdCell(gvitem->row, gvitem->col));
	MessageToParent(gvitem->row, gvitem->col, GVNM_ENDTIME);

	*pResult = 0;
}

void CfxGrid::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (!m_noMessage && GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
		SetFocus();

	m_tooltip.Hide();

	const int scrollPos = GetScrollPos32(SB_HORZ);
	const CIdCell tlCell = GetTopleftNonFixedCell();
	CRect rect; GetClientRect(rect);

	switch (nSBCode)
	{
	case SB_LINERIGHT:
		if (scrollPos < m_Hscroll)
		{
			const int Xscroll = GetColumnWidth(tlCell.col);
			SetScrollPos32(SB_HORZ, scrollPos + Xscroll);
			if (GetScrollPos32(SB_HORZ) == scrollPos)
			{
				if (!m_noMessage)
					MessageToParent(SB_LINERIGHT, 0, GVNM_RHSCROLL);
				break;
			}
			rect.left = GetFixedColumnWidth() + Xscroll;
			ScrollWindow(-Xscroll, 0, rect);
			rect.left = rect.right - (Xscroll+1);
			InvalidateRect(rect);
		}
		break;
	case SB_LINELEFT:
		if (scrollPos > 0 && tlCell.col > GetFixedColumnCount())
		{
			const int Xscroll = GetColumnWidth(tlCell.col-1);
			SetScrollPos32(SB_HORZ, max(0, scrollPos - Xscroll));
			rect.left = GetFixedColumnWidth();
			ScrollWindow(Xscroll, 0, rect);
			rect.right = rect.left + (Xscroll+1);
			InvalidateRect(rect);
		}
		else if (scrollPos <= 0 && !m_noMessage)
		{
			MessageToParent(SB_LINELEFT, 0, GVNM_LHSCROLL);
		}
		break;
	case SB_PAGERIGHT:
		if (scrollPos < m_Hscroll)
		{
			rect.left = GetFixedColumnWidth();
			const int offset = rect.Width();
			const int pos = min(m_Hscroll, scrollPos + offset);
			SetScrollPos32(SB_HORZ, pos);
			rect.left = GetFixedColumnWidth();
			InvalidateRect(rect);

			if (pos >= m_Hscroll && !m_noMessage)
				MessageToParent(SB_PAGERIGHT, 0, GVNM_RHSCROLL);
		}
		break;
	case SB_PAGELEFT:
		if (scrollPos > 0)
		{
			rect.left = GetFixedColumnWidth();
			const int offset = -rect.Width();
			const int pos = max(0, scrollPos + offset);
			SetScrollPos32(SB_HORZ, pos);
			rect.left = GetFixedColumnWidth();
			InvalidateRect(rect);
		}
		else if (scrollPos <= 0 && !m_noMessage)
		{
			MessageToParent(SB_PAGELEFT, 0, GVNM_LHSCROLL);
		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		{
			SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
			CIdCell newtlCell = GetTopleftNonFixedCell();
			if (newtlCell != tlCell)
			{
				rect.left = GetFixedColumnWidth();
				InvalidateRect(rect);
			}

			if (nSBCode == SB_THUMBPOSITION && !m_noMessage)
			{
				rect.left = GetFixedColumnWidth();

				if (scrollPos <= 0)
					MessageToParent(SB_THUMBPOSITION, 0, GVNM_LHSCROLL);
				else if (min(m_Hscroll, scrollPos+rect.Width()) >= m_Hscroll)
					MessageToParent(SB_THUMBPOSITION, 0, GVNM_RHSCROLL); 
			}
		}
		break;
	case SB_LEFT:
		if (scrollPos > 0)
		{
			SetScrollPos32(SB_HORZ, 0);
			Invalidate();
		}
		else if (scrollPos <= 0 && !m_noMessage)
		{
			MessageToParent(SB_LEFT, 0, GVNM_LHSCROLL);
		}
		break;
	case SB_RIGHT:
		if (scrollPos < m_Hscroll)
		{
			SetScrollPos32(SB_HORZ, m_Hscroll);
			Invalidate();

			rect.left = GetFixedColumnWidth();
			if (min(m_Hscroll, scrollPos+rect.Width()) >= m_Hscroll)
				MessageToParent(SB_RIGHT, 0, GVNM_RHSCROLL);
		}
		break;
	default:
		break;
	}
}

void CfxGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (!m_noMessage && GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
		SetFocus();

	CRect	rect;
	CIdCell	tlCell;
	int	scrollPos;

	m_tooltip.Hide();

	scrollPos = GetScrollPos32(SB_VERT);
	tlCell = GetTopleftNonFixedCell();
	GetClientRect(rect);

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (scrollPos < m_Vscroll)
		{
			int Yscroll = 0;
			if (tlCell.row < (GetRowCount() -1))
			{
				for (int ii = 0; ii < m_nRows; ii++)
				{
					Yscroll = GetRowHeight(tlCell.row);
					if (Yscroll > 0) break;
					tlCell.row++;
				}
			}

			SetScrollPos32(SB_VERT, scrollPos + Yscroll);
			if (GetScrollPos32(SB_VERT) == scrollPos)
			{
				if (!m_noMessage)
					MessageToParent(SB_LINEDOWN, 0, GVNM_DNVSCROLL);
				break;
			}

			rect.top = GetFixedRowHeight() + Yscroll;
			ScrollWindow(0, -Yscroll, rect);
			rect.top = rect.bottom - (Yscroll+1);
			InvalidateRect(rect);
		}
		break;
        case SB_LINEUP:
		if (scrollPos > 0 && tlCell.row > GetFixedRowCount())
		{
			int Yscroll = 0; tlCell.row--;
			for (int ii = tlCell.row; ii >= GetFixedRowCount(); ii--)
			{
				Yscroll = GetRowHeight(tlCell.row);
				if (Yscroll > 0) break;
				tlCell.row--;
			}

			SetScrollPos32(SB_VERT, max(0, scrollPos - Yscroll));
			rect.top = GetFixedRowHeight();
			ScrollWindow(0, Yscroll, rect);
			rect.bottom = rect.top + (Yscroll +1);
			InvalidateRect(rect);
		}
		else if (scrollPos <= 0 && !m_noMessage)
		{
			MessageToParent(SB_LINEUP, 0, GVNM_UPVSCROLL);
		}
		break;
        case SB_PAGEDOWN:
		if (scrollPos < m_Vscroll)
		{
			rect.top = GetFixedRowHeight();
			scrollPos = min(m_Vscroll, scrollPos + rect.Height());
			SetScrollPos32(SB_VERT, scrollPos);
			rect.top = GetFixedRowHeight();
			InvalidateRect(rect);

			if (scrollPos >= m_Vscroll && !m_noMessage)
				MessageToParent(SB_PAGEDOWN, 0, GVNM_DNVSCROLL);
		}
		break;
        case SB_PAGEUP:
		if (scrollPos > 0)
		{
			rect.top = GetFixedRowHeight();
			const int offset = -rect.Height();
			const int pos = max(0, scrollPos + offset);
			SetScrollPos32(SB_VERT, pos);
			rect.top = GetFixedRowHeight();
			InvalidateRect(rect);
		}
		else if (scrollPos <= 0 && !m_noMessage)
		{
			MessageToParent(SB_PAGEUP, 0, GVNM_UPVSCROLL);
		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		{
			SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
			CIdCell newtlCell = GetTopleftNonFixedCell();
			if (newtlCell != tlCell)
			{
				rect.top = GetFixedRowHeight();
				InvalidateRect(rect);
			}

			if (nSBCode == SB_THUMBPOSITION && !m_noMessage)
			{
				if (scrollPos <= 0)
					MessageToParent(SB_THUMBPOSITION, 0, GVNM_UPVSCROLL);
				else if (min(m_Vscroll, scrollPos+rect.Height()) >= m_Vscroll)
					MessageToParent(SB_THUMBPOSITION, 0, GVNM_DNVSCROLL); 
			}
		}
		break;
        case SB_TOP:
		if (scrollPos > 0)
		{
			SetScrollPos32(SB_VERT, 0);
			Invalidate();
		}
		else if (scrollPos <= 0 && !m_noMessage)
		{
			MessageToParent(SB_TOP, 0, GVNM_UPVSCROLL);
		}
		break;
        case SB_BOTTOM:
		if (scrollPos < m_Vscroll)
		{
			SetScrollPos32(SB_VERT, m_Vscroll);
			Invalidate();

			if (min(m_Vscroll, scrollPos+rect.Height()) >= m_Vscroll && !m_noMessage)
				MessageToParent(SB_BOTTOM, 0, GVNM_DNVSCROLL);
		}
		break;
	default:
		break;
	}
}

void CfxGrid::OnKillFocus(CWnd* pNewWnd) 
{
	MessageToParent(m_idCurrent.row, m_idCurrent.col, GVNM_KILLFOCUS);
	CWnd::OnKillFocus(pNewWnd);
}
/////////////////////////////////////////////////////////////////////////////
// CfxGrid implementation functions
void CfxGrid::OnDraw(CDC* pDC)
{
	CRect rect;
	int row{}, col{};

	CRect clipRect;
	if (pDC->GetClipBox(&clipRect) == ERROR)
		return;

	EraseBkgnd(pDC);

	const int nFixedRowHeight = GetFixedRowHeight();
	const int nFixedColWidth  = GetFixedColumnWidth();

	const CIdCell tlCell = GetTopleftNonFixedCell();
	const int minVisibleRow = tlCell.row;
	const int minVisibleCol = tlCell.col;

	CRect visRect;
	const CRangeCell visCellRange = GetVisibleNonFixedCellRange(visRect);
	const int maxVisibleRow = visCellRange.GetMaxRow();
	const int maxVisibleCol = visCellRange.GetMaxCol();

	// draw top-left cells 0..m_nFixedRows-1, 0..m_nFixedCols-1
	rect.bottom = -1;
	for (row = 0; row < m_nFixedRows; row++)
	{
		rect.top = rect.bottom +1;
		rect.bottom = rect.top + GetRowHeight(row) -1;
		rect.right = -1;
		for (col = 0; col < m_nFixedCols; col++)
		{
			if (IsCellAttribute(CIdCell(row, col), GVAT_HIDDEN))
				continue;

			rect.left = rect.right +1;
			rect.right = rect.left + GetColumnWidth(col) -1;  

			DrawFixedCell(pDC, row, col, rect);
		}
	}
     
	// draw fixed column cells:  m_nFixedRows..n, 0..m_nFixedCols-1
	rect.bottom = nFixedRowHeight -1;
	for (row = minVisibleRow; row <= maxVisibleRow; row++)
	{
		rect.top = rect.bottom +1;
		rect.bottom = rect.top + GetRowHeight(row) -1;

		if (rect.top > clipRect.bottom) break;
		if (rect.bottom < clipRect.top)	continue;

		rect.right = -1;
		for (col = 0; col < m_nFixedCols; col++)
		{
			if (IsCellAttribute(CIdCell(row, col), GVAT_HIDDEN))
				continue;

			rect.left  = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col) -1;

			if (rect.left > clipRect.right) break;
			if (rect.right < clipRect.left) continue;

			DrawFixedCell(pDC, row, col, rect);
		}
	}
    
	// draw fixed row cells  0..m_nFixedRows, m_nFixedCols..n
	rect.bottom = -1;
	for (row = 0; row < m_nFixedRows; row++)
	{
		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(row)-1;

		if (rect.top > clipRect.bottom) break;
		if (rect.bottom < clipRect.top) continue;

		rect.right = nFixedColWidth -1;
		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{
			if (IsCellAttribute(CIdCell(row, col), GVAT_HIDDEN))
				continue;

			rect.left = rect.right + 1;
			rect.right = rect.left + GetColumnWidth(col) -1;

			if (rect.left > clipRect.right)	break;
			if (rect.right < clipRect.left)	continue;

			DrawFixedCell(pDC, row, col, rect);
		}
	}
	
	// draw rest of non-fixed cells
	rect.bottom = nFixedRowHeight-1;
	for (row = minVisibleRow; row <= maxVisibleRow; row++)
	{
		rect.top = rect.bottom +1;
		rect.bottom = rect.top + GetRowHeight(row) -1;

		// rect.bottom = bottom pixel of previous row
		if (rect.top > clipRect.bottom) break;
		if (rect.bottom < clipRect.top) continue;

		rect.right = nFixedColWidth -1;
		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{
			if (IsCellAttribute(CIdCell(row, col), GVAT_HIDDEN))
				continue;

			rect.left = rect.right +1;
			rect.right = rect.left + GetColumnWidth(col) -1;

			if (rect.left > clipRect.right)	break;
			if (rect.right < clipRect.left)	continue;

			DrawCell(pDC, row, col, rect);
		}
	}

	CPen pen; pen.CreatePen(m_gridLineType, 0, m_gridCOL);
	pDC->SelectObject(&pen); pDC->SetBkMode(TRANSPARENT);
	
// updateX_20060125(manie)
	pDC->SelectStockObject(NULL_BRUSH); 
	
	if (m_bGridOutline)					//테두리를 그리지 않을 수 있도록 개선(20090709) - SetGridOutline()
		pDC->Rectangle(visRect);
	pDC->MoveTo(visRect.left, visRect.bottom-1);	
	pDC->LineTo(visRect.right, visRect.bottom-1);	//가로 마지막 라인을 그리도록 수정(20090723)
	// draw vertical lines (drawn at ends of cells)
	if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_VERT) 
	{
		int xx = 0;
		for (col = 0; col < m_nFixedCols-1; col++)
		{
			xx += GetColumnWidth(col);
			pDC->MoveTo(xx-1, 0);
			pDC->LineTo(xx-1, visRect.bottom);   
		}

		pDC->MoveTo(0, 0); pDC->LineTo(visRect.right,  0);			//헤더의 위라인을 그리도록 수정(20090709)
//		pDC->MoveTo(0, 0); pDC->LineTo(0, visRect.bottom);			//세로 첫번째 라인을 그리지 않도록 수정(20090709)
//		pDC->MoveTo(nFixedColWidth-1, 0); pDC->LineTo(nFixedColWidth-1, visRect.bottom);

		int x = nFixedColWidth;
//		for (col = minVisibleCol; col <= maxVisibleCol; col++)		//세로 마지막 라인을 안그리도록 수정(20090709)
		for (col = minVisibleCol; col <= maxVisibleCol - 1; col++)
		{
			x += GetColumnWidth(col);
			pDC->MoveTo(x-1, 0);
			pDC->LineTo(x-1, visRect.bottom);   
		}
	}
    
	// draw horizontal lines (drawn at bottom of each cell)
// updateX_20060125(manie)
	pDC->MoveTo(0, nFixedRowHeight-1); pDC->LineTo(visRect.right,  nFixedRowHeight-1);
	if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_HORZ) 
	{
		pDC->MoveTo(0, 0); pDC->LineTo(visRect.right,  0);	
		pDC->MoveTo(0, nFixedRowHeight-1); pDC->LineTo(visRect.right,  nFixedRowHeight-1);

		int y = nFixedRowHeight;
		for (row = minVisibleRow; row <= maxVisibleRow; row++)
		{
			y += GetRowHeight(row);
			pDC->MoveTo(0, y-1);
			pDC->LineTo(visRect.right, y-1);
		}
	}

	CFont font, * oldfont{}; int key{}; _fixMerge fixMerge;
	for (POSITION pos = m_mapMerge.GetStartPosition(); pos != NULL; )
	{
		m_mapMerge.GetNextAssoc(pos, key, (_fixMerge &)fixMerge);

		pDC->FillSolidRect(fixMerge.rect, fixMerge.bkcol == CLR_DEFAULT ? m_fixbkCOL : fixMerge.bkcol);

		pDC->MoveTo(fixMerge.rect.right,fixMerge.rect.top);
		pDC->LineTo(fixMerge.rect.left, fixMerge.rect.top);
		pDC->LineTo(fixMerge.rect.left, fixMerge.rect.bottom);

		pDC->MoveTo(fixMerge.rect.right, fixMerge.rect.top);
		pDC->LineTo(fixMerge.rect.right, fixMerge.rect.bottom);
		pDC->LineTo(fixMerge.rect.left,  fixMerge.rect.bottom);

		fixMerge.rect.DeflateRect(1+m_margin, 1);

		font.CreatePointFontIndirect(&fixMerge.font);
		oldfont = pDC->SelectObject(&font);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(fixMerge.fgcol);
		fixMerge.rect.top += 2;

		DrawText(pDC->m_hDC, fixMerge.text, -1, fixMerge.rect, fixMerge.format);

		pDC->SelectObject(oldfont); font.DeleteObject();
	}

	pDC->SelectStockObject(NULL_PEN);
	pen.DeleteObject();
}

BOOL CfxGrid::DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
	GVITEM gvitem;

	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_IMAGE|GVMK_BKCOLOR|GVMK_FGCOLOR|GVMK_STATE|GVMK_ATTR|GVMK_PARAM|GVMK_MISC;
	gvitem.row  = nRow;
	gvitem.col  = nCol;

	if (!GetItem(&gvitem)) return FALSE;

	const int savedDC = pDC->SaveDC();

	// HDRMERGE column rectangle calculate
	if ((gvitem.attr & GVAT_HDRMERGE) == GVAT_HDRMERGE)
	{
		rect.top = rect.top + (rect.Height() / 2);

		CRect Rect; _fixMerge fixMerge;
		if (m_mapMerge.Lookup(nCol, fixMerge))
		{
			for (int ii = 0; ii < maxMERGE; ii++)
			{
				if (fixMerge.cols[ii] == -1)
					break;
				if (!GetCellRect(nRow, fixMerge.cols[ii], Rect))
					continue;
				
				if (!ii)
				{
					Rect.bottom = Rect.bottom - (Rect.Height() / 2);
					if (nCol > m_nFixedCols) Rect.left  -= 1;
					fixMerge.rect = Rect;
				}
				else
				{
					fixMerge.rect.right = Rect.right;
				}
			}

			fixMerge.rect.bottom -= 2;
			m_mapMerge.SetAt(nCol, fixMerge);
		}
	}

	bEraseBk = TRUE;
	if (gvitem.state & GVST_DROPHILITED && !pDC->IsPrinting())
	{
		rect.right++; rect.bottom++;
		pDC->SelectStockObject(BLACK_PEN);
		pDC->SelectStockObject(LTGRAY_BRUSH); //NULL_BRUSH);
		pDC->Rectangle(rect);
		rect.right--; rect.bottom--;
	}
	else if (gvitem.state & GVST_FOCUSED && !pDC->IsPrinting()) 
	{
		rect.right++; rect.bottom++;	// FillRect doesn't draw RHS or bottom
		if (bEraseBk) 
		{
			const COLORREF color = (gvitem.bkcol == CLR_DEFAULT) ? m_fixbkCOL : gvitem.bkcol;
			CBrush brush(color); pDC->FillRect(rect, &brush);
		}
		rect.right--; rect.bottom--;

		CRect	chkRC = rect;
		if (chkRC.left <= 0) chkRC.left = 1;
		pDC->SelectStockObject(BLACK_PEN);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(chkRC);
	}
	else if (m_selectLine && gvitem.state & GVST_SELECTED && !pDC->IsPrinting() && nRow >= m_nFixedRows)
	{
		rect.right++; rect.bottom++;	// FillRect doesn't draw RHS or bottom
		pDC->FillSolidRect(rect, m_bkSELCOL);
		rect.right--; rect.bottom--;
		pDC->SetTextColor(PALETTERGB(255, 255, 255));
	}
	else if (bEraseBk && !pDC->IsPrinting())
	{
		rect.right++; rect.bottom++;
		const COLORREF color = (gvitem.bkcol == CLR_DEFAULT) ? m_fixbkCOL : gvitem.bkcol;
		pDC->FillSolidRect(rect, color);
		rect.right--; rect.bottom--;
	}

	pDC->SetTextColor((gvitem.fgcol == CLR_DEFAULT) ? m_fixfgCOL : gvitem.fgcol);
    
	LOGFONT *logfont  = GetItemFont(nRow, nCol);
	const CIdCell focusCell = GetFocusCell();

	if (false && (focusCell.row == nRow || focusCell.col == nCol))
	{
		static LOGFONT lf;
		memcpy(&lf, logfont, sizeof(LOGFONT));
		lf.lfWeight = selcWEIGHT;
		logfont = &lf;
	}
   
	CFont font; font.CreatePointFontIndirect(logfont);
	pDC->SelectObject(&font); pDC->SetBkMode(TRANSPARENT);

	rect.DeflateRect(m_margin+1, 1);

	if (m_imageList && gvitem.image >= 0)
	{
		IMAGEINFO Info;
		if (m_imageList->GetImageInfo(gvitem.image, &Info))
		{
			const int nImageWidth  = Info.rcImage.right-Info.rcImage.left+1;
			const int nImageHeight = Info.rcImage.bottom - Info.rcImage.top;

			const CPoint	drawPT(rect.left, rect.top + ((rect.Height() - nImageHeight) / 2));

			m_imageList->Draw(pDC, gvitem.image, drawPT, ILD_NORMAL);
			rect.left += nImageWidth + m_margin;
		}
	}

	if ((gvitem.attr & GVAT_CHECK) == GVAT_CHECK)
	{
		const int nWidth = 15; const int nHeight = 15;
		const int topx = (rect.Height() - nHeight) / 2;
		CRect	checkRC(rect.left, rect.top+topx, rect.left+nWidth, rect.top+topx+nHeight);
		if (gvitem.text.GetLength() <= 0)
		{
			const int offset = (rect.Width() - nWidth) / 2;
			checkRC.SetRect(rect.left+offset, rect.top+topx, rect.left+offset+nWidth, rect.top+topx+nHeight);
		}

		UINT state = DFCS_BUTTONCHECK;

		switch (int(gvitem.param))
		{
		case  1: state |= DFCS_CHECKED;		break;
		case -1: state |= DFCS_INACTIVE;	break;
		}
		pDC->DrawFrameControl(checkRC, DFC_BUTTON, state);
		rect.left += nWidth+m_margin;
	}

	CString string = gvitem.text;
	if ((gvitem.attr & GVAT_CONTRAST) == GVAT_CONTRAST && !string.IsEmpty())
	{
		CString	temps;
		switch (string[0])
		{
		case '1':
// updateX_20060109
//			temps = ARR_UP; gvitem.fgcol = m_upCOL; break;
			temps = ARR_UP; gvitem.bkcol = m_upCOL; gvitem.fgcol = m_fgSELCOL;
			if (m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
			{
				gvitem.fgcol = m_upCOL; gvitem.bkcol = m_bkSELCOL;
			}
			rect.InflateRect(m_margin, 0); pDC->FillSolidRect(rect, gvitem.bkcol); rect.DeflateRect(m_margin, 0);
			break;
		case '+':
		case '2':
			temps = TRI_UP; gvitem.fgcol = m_upCOL; break;
		case '4':
// updateX_20060109
//			temps = ARR_DN; gvitem.fgcol = m_dnCOL; break;
			temps = ARR_DN; gvitem.bkcol = m_dnCOL; gvitem.fgcol = m_fgSELCOL;
			if (m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
			{
				gvitem.fgcol = m_dnCOL; gvitem.bkcol = m_bkSELCOL;
			}
			rect.InflateRect(m_margin, 0); pDC->FillSolidRect(rect, gvitem.bkcol); rect.DeflateRect(m_margin, 0);
			break;
		case '-':
		case '5':
			temps = TRI_DN; gvitem.fgcol = m_dnCOL; break;
		default:
			temps = _T("  "); gvitem.fgcol = m_eqCOL; break;
		}

		const CSize size = pDC->GetOutputTextExtent(temps);
		COLORREF	clr{};

		pDC->SetTextColor(gvitem.fgcol);
// updateX_20060109
		if (false && m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
		{
			clr = m_fgSELCOL;
			pDC->SetTextColor(m_fgSELCOL);
		}
		clr = gvitem.fgcol;

		const int topx = (rect.Height() - size.cy) / 2;
		const CRect	contRC(rect.left, rect.top+topx, rect.left+size.cx, rect.top+topx+size.cy);

		drawContrast(pDC, contRC, temps, clr);
		string = string.Mid(1);
		rect.left += size.cx+m_margin;
	}

	const COLORREF rgb = DisplayAttribute(gvitem.attr, string);
	if (rgb != (COLORREF) 0)
	{
		gvitem.fgcol = rgb; pDC->SetTextColor(rgb);
// updateX_20090109
		if (false && m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
		{
			pDC->SetTextColor(m_fgSELCOL);
		}
	}

	if (false && m_sortColumn != -1 && m_sortColumn == nCol)
	{
		if (m_ascending)
			string.Insert(0, "△");
		else
			string.Insert(0, "▽");
	}

	// updateX_20051018
	if (++rect.top < rect.bottom) DrawText(pDC->m_hDC, string, -1, rect, gvitem.format);

	pDC->RestoreDC(savedDC);
	font.DeleteObject();

	return TRUE;
}

BOOL CfxGrid::DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
	int visibleCol{};
	GVITEM gvitem;

	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_STATE|GVMK_ATTR|GVMK_IMAGE|GVMK_BKCOLOR|GVMK_FGCOLOR|GVMK_PARAM;
	gvitem.row = nRow;
	gvitem.col = nCol;

	if (!GetItem(&gvitem)) return FALSE;

	COLORREF rtmCol= -1;
	const COLORREF fgCol = (gvitem.fgcol == CLR_DEFAULT) ? m_fgCOL : gvitem.fgcol;
	COLORREF bkCol = (gvitem.bkcol == CLR_DEFAULT) ? m_bkCOL : gvitem.bkcol;

	if (m_stepUnit && nRow >= m_nFixedRows)
	{
		bkCol = m_stepCOL1;
		if (((nRow -1) / m_stepUnit) % 2) bkCol = m_stepCOL2;
	}

	bEraseBk = TRUE;
	const int savedDC = pDC->SaveDC();

	pDC->SetBkMode(TRANSPARENT);
	CRect	visRect; GetVisibleNonFixedCellRange(visRect);

	if (gvitem.state & GVST_FOCUSED && !pDC->IsPrinting()) 
	{
		if (nRow == 0 && nRow == m_nFixedRows)
			rect.top += 1;

		rect.right++; rect.bottom++;	// FillRect doesn't draw RHS or bottom
		if (bEraseBk) 
		{
			CBrush brush(bkCol);
			pDC->FillRect(rect, &brush);
		}
		rect.right--; rect.bottom--;

		// updateX_20050530
		for (visibleCol = 0; visibleCol < m_nCols; visibleCol++)
		{
			if (IsCellVisible(nRow, visibleCol))
				break;
		}
		if (visibleCol == nCol) rect.left += 1;

		pDC->SelectStockObject(BLACK_PEN);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect);
		pDC->SetTextColor(fgCol);

		if (visibleCol == nCol) rect.left -= 1;
		if (nRow == 0 && nRow == m_nFixedRows)
			rect.top -= 1;
	}
	else if (m_mapRTM.Lookup(nRow * 10000 + nCol, rtmCol))
	{
		rect.right++; rect.bottom++;	// FillRect doesn't draw RHS or bottom
		if (bEraseBk) 
		{
			CBrush brush(bkCol);
			pDC->FillRect(rect, &brush);
		}
		rect.right--; rect.bottom--;
		pDC->SetTextColor(fgCol);
	}
	else if (m_selectLine && gvitem.state & GVST_SELECTED && !pDC->IsPrinting())
	{
		rect.right++; rect.bottom++;	// FillRect doesn't draw RHS or bottom
		pDC->FillSolidRect(rect, m_bkSELCOL);
		rect.right--; rect.bottom--;
// updateX_20060109
//		fgCol = m_fgSELCOL;
		pDC->SetTextColor(fgCol);
	}
	else
	{
		rect.right++; rect.bottom++;	// FillRect doesn't draw RHS or bottom
		if (bEraseBk) 
		{
			CBrush brush(bkCol);
			pDC->FillRect(rect, &brush);
		}
		rect.right--; rect.bottom--;
		pDC->SetTextColor(fgCol);
	}

	if (gvitem.state & GVST_DROPHILITED && !pDC->IsPrinting())
	{
		pDC->SelectStockObject(BLACK_PEN);
		pDC->SelectStockObject(LTGRAY_BRUSH); //NULL_BRUSH);
		pDC->Rectangle(rect);
	}
	LOGFONT *logfont = GetItemFont(nRow, nCol);
	if (logfont->lfHeight == 80) logfont->lfHeight = 85;

	CFont font; font.CreatePointFontIndirect(logfont);

	pDC->SelectObject(&font);

	if (m_imageList && gvitem.image >= 0)
	{
		IMAGEINFO Info;
		if (m_imageList->GetImageInfo(gvitem.image, &Info))
		{
			const int nImageWidth = Info.rcImage.right - Info.rcImage.left+1;
			const int nImageHeight = Info.rcImage.bottom - Info.rcImage.top;

			const CPoint	drawPT(rect.left, rect.top + ((rect.Height() - nImageHeight) / 2));

			m_imageList->Draw(pDC, gvitem.image, drawPT, ILD_NORMAL);
			rect.left += nImageWidth+m_margin;
		}
	}

	rect.DeflateRect(m_margin, 0);

	if ((gvitem.attr & GVAT_CHECK) == GVAT_CHECK)
	{
		const int nWidth = 15; const int nHeight = 15;
		const int topx = (rect.Height() - nHeight) / 2;
		CRect	checkRC(rect.left, rect.top+topx, rect.left+nWidth, rect.top+topx+nHeight);
		if (gvitem.text.GetLength() <= 0)
		{
			const int offset = (rect.Width() - nWidth) / 2;
			checkRC.SetRect(rect.left+offset, rect.top+topx, rect.left+offset+nWidth, rect.top+topx+nHeight);
		}

		const UINT state = (gvitem.param) ? DFCS_BUTTONCHECK|DFCS_CHECKED : DFCS_BUTTONCHECK;
		pDC->DrawFrameControl(checkRC, DFC_BUTTON, state);
		rect.left += nWidth+m_margin;
	}

	CString string = gvitem.text;

	BOOL bChange = FALSE;

	if (!m_bShowZero && !string.IsEmpty())
	{
		CString strVal = string;

		if ((gvitem.attr & GVAT_CONTRAST) == GVAT_CONTRAST)
			strVal.Delete(0);
		else
		{
			if (strVal[0] == _T('+') || strVal[0] == _T('-'))
				strVal.Delete(0);
		}

		strVal.TrimLeft();
		strVal.TrimRight();

		if (IsNumeric(strVal))
		{
			const int nPos = strVal.Find(_T("."));
			if (nPos == -1)
			{
				if (atoi((LPCTSTR)strVal) == 0)
				{
					bChange = TRUE;
					strVal.Empty();
				}
			}
			else
			{
				if (atoi((LPCTSTR)strVal.Left(nPos)) == 0 &&
					atoi((LPCTSTR)strVal.Right(strVal.GetLength() - nPos - 1)) == 0)
				{
					bChange = TRUE;
					strVal.Empty();
				}
			}
		}

		if (bChange)
			string = strVal;
	}

	if ((gvitem.attr & GVAT_CONTRAST) == GVAT_CONTRAST && !string.IsEmpty())
	{
		CString	temps;
		switch (string[0])
		{
		case '1':
// updateX_20060109
//			temps = ARR_UP; gvitem.fgcol = m_upCOL; break;
			temps = ARR_UP; gvitem.bkcol = m_upCOL; gvitem.fgcol = m_fgSELCOL;
			if (rtmCol == -1 && m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
			{
				gvitem.fgcol = m_upCOL; gvitem.bkcol = m_bkSELCOL;
			}
			rect.InflateRect(m_margin, 0); pDC->FillSolidRect(rect, gvitem.bkcol); rect.DeflateRect(m_margin, 0);
			break;
		case '+':
		case '2':
			temps = TRI_UP; gvitem.fgcol = m_upCOL; break;
		case '4':
// updateX_20060109
//			temps = ARR_DN; gvitem.fgcol = m_dnCOL; break;
			temps = ARR_DN; gvitem.bkcol = m_dnCOL; gvitem.fgcol = m_fgSELCOL;
			if (rtmCol == -1 && m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
			{
				gvitem.fgcol = m_dnCOL; gvitem.bkcol = m_bkSELCOL;
			}
			rect.InflateRect(m_margin, 0); pDC->FillSolidRect(rect, gvitem.bkcol); rect.DeflateRect(m_margin, 0);
			break;
		case '-':
		case '5':
			temps = TRI_DN; gvitem.fgcol = m_dnCOL; break;
		default:
			temps = _T("  "); gvitem.fgcol = m_eqCOL; break;
		}

		const CSize size = pDC->GetOutputTextExtent(temps);
		COLORREF clr = gvitem.fgcol;

		pDC->SetTextColor(gvitem.fgcol);
// updateX_20060109
		if (false && rtmCol == -1 && m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
		{
			clr = m_fgSELCOL;
			pDC->SetTextColor(m_fgSELCOL);
		}
		clr = gvitem.fgcol;

		const int topx = (rect.Height() - size.cy) / 2;
		const CRect	contRC(rect.left, rect.top+topx, rect.left+size.cx, rect.top+topx+size.cy);

		drawContrast(pDC, contRC, temps, clr);
		string = string.Mid(1);
		rect.left += size.cx+m_margin;
	}

	const COLORREF rgb = DisplayAttribute(gvitem.attr, string);
	if (rgb != (COLORREF) 0)
	{
		gvitem.fgcol = rgb; pDC->SetTextColor(rgb);
// updateX_20090109
		if (false && rtmCol == -1 && m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
		{
			pDC->SetTextColor(m_fgSELCOL);
		}
	}

	if (!string.IsEmpty())
	{
		if (!m_bShowPlus && string[0] == '+')
			string.Delete(0);
	}
/*
	CString strVal = string;
	strVal.TrimLeft();
	strVal.TrimRight();

	bChange = FALSE;

	if (!strVal.IsEmpty() && IsNumeric(strVal))
	{
		int nPos = strVal.Find(_T("."));
		if (nPos == -1)
		{
			if (atoi((LPCTSTR)strVal) == 0)
			{
				bChange = TRUE;
				strVal.Empty();
			}
		}
		else
		{
			if (atoi((LPCTSTR)strVal.Left(nPos)) == 0 &&
				atoi((LPCTSTR)strVal.Right(strVal.GetLength() - nPos - 1)) == 0)
			{
				bChange = TRUE;
				strVal.Empty();
			}
		}
	}
	
	if (bChange)
		string = strVal;
*/		
	if ((gvitem.format & GVFM_MULTI) == GVFM_MULTI) rect.DeflateRect(0, 4);
	if (rect.top < rect.bottom) DrawText(pDC->m_hDC, string, -1, rect, gvitem.format);

	pDC->RestoreDC(savedDC);
	font.DeleteObject();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
// CfxGrid Cell selection stuff
BOOL CfxGrid::IsValid(const CIdCell& cell) const
{
	return (cell.row >= 0 && cell.row < m_nRows && cell.col >= 0 && cell.col < m_nCols);
}

BOOL CfxGrid::IsValid(const CRangeCell& range) const
{
	return (range.GetMinRow() >= 0 && range.GetMinCol() >= 0
			&& range.GetMaxRow() >= 0 && range.GetMaxCol() >= 0
			&& range.GetMaxRow() < m_nRows && range.GetMaxCol() < m_nCols
			&& range.GetMinRow() <= range.GetMaxRow() && range.GetMinCol() <= range.GetMaxCol());
}

BOOL CfxGrid::RedrawCell(const CIdCell& cell, CDC* pDC)
{
	return RedrawCell(cell.row, cell.col, pDC);
}

BOOL CfxGrid::RedrawCell(int nRow, int nCol, CDC* pDC)
{
	BOOL bResult = TRUE;
	int visibleCol{};
	if (!IsCellVisible(nRow, nCol))
		return FALSE;

	CRect rect;
	if (!GetCellRect(nRow, nCol, rect))
		return FALSE;

	if (!pDC) pDC = GetDC();

	if (pDC)
	{
		CPen pen; pen.CreatePen(m_gridLineType, 0, m_gridCOL);
		CPen* pOldPen = (CPen*) pDC->SelectObject(&pen);
		pDC->SetBkMode(TRANSPARENT);

		if (nRow < m_nFixedRows || nCol < m_nFixedCols)
		{
			bResult = DrawFixedCell(pDC, nRow, nCol, rect, TRUE);
// updateX_20060125(manie)
			if (true || m_gridLine == GVLN_BOTH || m_gridLine == GVLN_HORZ) 
			{
				if (rect.top > 0) rect.top -= 1;
				pDC->MoveTo(rect.left, rect.top);    pDC->LineTo(rect.right+1, rect.top);
				pDC->MoveTo(rect.left, rect.bottom); pDC->LineTo(rect.right+1, rect.bottom);
			}
			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_VERT) 
			{
				// updateX_20050530
				for (visibleCol = 0; visibleCol < m_nCols; visibleCol++)
				{
					if (IsCellVisible(nRow, visibleCol))
						break;
				}
				if (visibleCol >= m_nCols) visibleCol = 0;

				if ((m_nFixedCols != 0 && nCol == visibleCol) || (m_nFixedCols == 0 && nRow == 0 && nCol == visibleCol))
//				if ((m_nFixedCols != 0 && nCol < m_nFixedCols) || (m_nFixedCols == 0 && nRow == 0 && nCol == 0))
				{
					pDC->MoveTo(rect.left, rect.top); pDC->LineTo(rect.left, rect.bottom+1);
				}
				pDC->MoveTo(rect.right, rect.top); pDC->LineTo(rect.right, rect.bottom+1);
			}
		}
		else
		{
// updateX_20060201(manie)
			CRect visRect;
			const CRangeCell visCellRange = GetVisibleNonFixedCellRange(visRect);
			const int maxVisibleRow = visCellRange.GetMaxRow();

			bResult = DrawCell(pDC, nRow, nCol, rect, TRUE);
			if (nRow == maxVisibleRow || m_gridLine == GVLN_BOTH || m_gridLine == GVLN_HORZ) 
			{
				if (rect.top > 0) rect.top -= 1;
				if (nRow != maxVisibleRow)
				{
					pDC->MoveTo(rect.left, rect.top);    pDC->LineTo(rect.right+1, rect.top);
				}
				if (m_bGridOutline)
				{
					pDC->MoveTo(rect.left, rect.bottom); pDC->LineTo(rect.right+1, rect.bottom);
				}
//	update20090709 - 기존 마지막 라인을 redraw한 것을 아웃라인을 그릴 경우 수행하도록 수정
//				pDC->MoveTo(rect.left, rect.bottom); pDC->LineTo(rect.right+1, rect.bottom);
			}

			int nvisibleColCnt = 0;
			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_VERT) 
			{
				if (rect.left == 0 && m_bGridOutline)
				{
					pDC->MoveTo(rect.left, rect.top); pDC->LineTo(rect.left, rect.bottom+1);
				}
				for (int visibleCol = 0; visibleCol < m_nCols; visibleCol++)
				{
					if (IsCellVisible(nRow, visibleCol))
						nvisibleColCnt++;
				}
				if (nCol != nvisibleColCnt || m_bGridOutline)	
				{
					pDC->MoveTo(rect.right, rect.top); pDC->LineTo(rect.right, rect.bottom+1);    
				}
//	update20090709 - 테두리가 없을 경우 양쪽 사이드라인을 그리지 않도록 수정
//				if (rect.left == 0)
//				{
//					pDC->MoveTo(rect.left, rect.top); pDC->LineTo(rect.left, rect.bottom+1);
//				}
//				pDC->MoveTo(rect.right, rect.top); pDC->LineTo(rect.right, rect.bottom+1);
			}
		}
		pDC->SelectObject(pOldPen);
	}

	InvalidateRect(rect, TRUE);

//	else
//	{
//		InvalidateRect(rect, TRUE);
//	}

	if (pDC != NULL)
		ReleaseDC(pDC);

	return bResult;
}

CIdCell CfxGrid::SetFocusCell(int nRow, int nCol)
{
	return SetFocusCell(CIdCell(nRow, nCol));
}

CIdCell CfxGrid::SetFocusCell(CIdCell focusCell)
{
	if (focusCell == m_idCurrent) 
		return m_idCurrent;

	CIdCell prevCell = m_idCurrent;
	m_idCurrent = focusCell;

	if (IsValid(prevCell)) 
	{
		MessageToParent(prevCell.row, prevCell.col, GVNM_SELCHANGING); 

		SetItemState(prevCell.row, prevCell.col,
			GetItemState(prevCell.row, prevCell.col) & ~GVST_FOCUSED);
		RedrawCell(prevCell);

		if (prevCell.col != m_idCurrent.col)
		{
			for (int row = 0; row < m_nFixedRows; row++)
				RedrawCell(row, prevCell.col);
		}

		if (prevCell.row != m_idCurrent.row)
		{
			for (int col = 0; col < m_nFixedCols; col++) 
				RedrawCell(prevCell.row, col);
		}
	}

	if (IsValid(m_idCurrent))
	{
		SetItemState(m_idCurrent.row, m_idCurrent.col,
			GetItemState(m_idCurrent.row, m_idCurrent.col)|(m_focusLine ? GVST_FOCUSED : 0));
		RedrawCell(m_idCurrent);

		if (prevCell.col != m_idCurrent.col)
		{
			for (int row = 0; row < m_nFixedRows; row++) 
				RedrawCell(row, m_idCurrent.col);
		}
		if (prevCell.row != m_idCurrent.row)
		{
			for (int col = 0; col < m_nFixedCols; col++) 
				RedrawCell(m_idCurrent.row, col);
		}

		MessageToParent(m_idCurrent.row, m_idCurrent.col, GVNM_SELCHANGED);

		if (m_sensitive)
		{
			if (IsCellAttribute(m_idCurrent, GVAT_EDIT))
				OnEditCell(m_idCurrent.row, m_idCurrent.col, VK_TAB);
			else if (IsCellAttribute(m_idCurrent, GVAT_COMBO))
				OnComboCell(m_idCurrent.row, m_idCurrent.col);
			else if (IsCellAttribute(m_idCurrent, GVAT_DATE))
				OnDateCell(m_idCurrent.row, m_idCurrent.col);
			else if (IsCellAttribute(m_idCurrent, GVAT_TIME))
				OnTimeCell(m_idCurrent.row, m_idCurrent.col);
		}
	}

	return prevCell;
}

void CfxGrid::SetSelectedRange(const CRangeCell& range, BOOL bForceRepaint)
{
	SetSelectedRange(range.GetMinRow(), range.GetMinCol(), 
				range.GetMaxRow(), range.GetMaxCol(), bForceRepaint);
}

void CfxGrid::SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol, BOOL bForceRepaint)
{
	CDC* pDC = NULL;
	if (bForceRepaint) pDC = GetDC();

	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != NULL; )
	{
		DWORD key; CIdCell idCell;
		const CIdCell cell;
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		if (IsValid(idCell))
		{
			SetItemState(idCell.row, idCell.col, GetItemState(idCell.row, idCell.col) & ~GVST_SELECTED);
			if (nMinRow <= idCell.row && idCell.row <= nMaxRow 
						&& nMinCol <= idCell.col && idCell.col <= nMaxCol)
				continue;

			if (m_selectLine)
			{
				if (bForceRepaint && pDC)
					RedrawCell(idCell.row, idCell.col, pDC);
				else
					InvalidateCellRect(idCell);
			}
		}
	}
    
	for (POSITION pos = m_mapPreCell.GetStartPosition(); pos != NULL;)
	{
		DWORD key; CIdCell idCell;
		m_mapPreCell.GetNextAssoc(pos, key, (CIdCell&)idCell);

		if (!IsValid(idCell)) continue;

		const int nState = GetItemState(idCell.row, idCell.col);
		SetItemState(idCell.row, idCell.col, nState|GVST_SELECTED);

		if (m_selectLine)
		{
			if (bForceRepaint && pDC)
				RedrawCell(idCell.row, idCell.col, pDC);
			else
				InvalidateCellRect(idCell);
		}
	}

	bool fToggle = false;
	if (nMinRow >= 0 && nMinCol >= 0 && nMaxRow >= 0 && nMaxCol >= 0
		&& nMaxRow < m_nRows && nMaxCol < m_nCols && nMinRow <= nMaxRow && nMinCol <= nMaxCol)
	{
		if (press_CTRLKEY() && m_selectLine && nMinRow == nMaxRow)
		{
			if (GetItemState(nMinRow, nMinCol) & GVST_SELECTED)
				fToggle = true;
		}

		for (int row = nMinRow; row <= nMaxRow; row++)
		{
			for (int col = nMinCol; col <= nMaxCol; col++) 
			{
				const int nState = GetItemState(row, col);
				if (nState & GVST_SELECTED || fToggle)
				{
					SetItemState(row, col, (nState & ~GVST_SELECTED));
					continue;
				}

				const CIdCell idCell(row, col);
				const CIdCell  cell;
				SetItemState(row, col, nState|GVST_SELECTED);

				if (m_selectLine)
				{
					if (bForceRepaint && pDC)
						RedrawCell(row, col, pDC);
					else
						InvalidateCellRect(idCell);
				}
			}
		}
	}

	if (pDC != NULL) ReleaseDC(pDC);

	int key{}; _fixMerge fixMerge;
	for (POSITION pos = m_mapMerge.GetStartPosition(); pos != NULL; )
	{
		m_mapMerge.GetNextAssoc(pos, key, (_fixMerge &)fixMerge);
		InvalidateRect(fixMerge.rect);
	}
} 

void CfxGrid::UpdateRowSelectedRange(int row, bool insert)
{
	DWORD key{}; CIdCell idCell;

	m_idSelect.row = -1; m_idSelect.col = -1;

	m_mapPreCell.RemoveAll();
	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != NULL; )
	{
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		m_mapPreCell.SetAt(key, idCell);
	}

	m_mapSelCell.RemoveAll();
	for (POSITION pos = m_mapPreCell.GetStartPosition(); pos != NULL; )
	{
		m_mapPreCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		if (idCell.row < row)
		{
			m_mapSelCell.SetAt(key, idCell);
			continue;
		}

		if (insert)
		{
			idCell.row += 1;
			key = MAKELONG(idCell.row, idCell.col);
			m_mapSelCell.SetAt(key, idCell);
		}
		else if (!insert)		// delete
		{
			if (idCell.row == row)
				continue;

			idCell.row -= 1;
			key = MAKELONG(idCell.row, idCell.col);
			m_mapSelCell.SetAt(key, idCell);
		}
	}

	m_mapPreCell.RemoveAll();
	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != NULL; )
	{
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		m_mapPreCell.SetAt(key, idCell);
	}
}

void CfxGrid::UpdateColSelectedRange(int col, bool insert)
{
	DWORD key{}; CIdCell idCell;

	m_idSelect.row = -1; m_idSelect.col = -1;

	m_mapPreCell.RemoveAll();
	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != NULL; )
	{
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		m_mapPreCell.SetAt(key, idCell);
	}

	m_mapSelCell.RemoveAll();
	for (POSITION pos = m_mapPreCell.GetStartPosition(); pos != NULL; )
	{
		m_mapPreCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		if (idCell.col < col)
		{
			m_mapSelCell.SetAt(key, idCell);
			continue;
		}

		if (insert)
		{
			idCell.col += 1;
			key = MAKELONG(idCell.row, idCell.col);
			m_mapSelCell.SetAt(key, idCell);
		}
		else if (!insert)		// delete
		{
			if (idCell.col == col)
				continue;

			idCell.col -= 1;
			key = MAKELONG(idCell.row, idCell.col);
			m_mapSelCell.SetAt(key, idCell);
		}
	}

	m_mapPreCell.RemoveAll();
	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != NULL; )
	{
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		m_mapPreCell.SetAt(key, idCell);
	}
}

void CfxGrid::SelectAllCells()
{
	SetSelectedRange(m_nFixedRows, m_nFixedCols, GetRowCount()-1, GetColumnCount()-1);
}

void CfxGrid::SelectColumns(CIdCell current)
{
	if (current.col < m_nFixedCols || !IsValid(current))
		return;

	SetSelectedRange(GetFixedRowCount(), min(m_idSelect.col, current.col), 
				GetRowCount()-1, max(m_idSelect.col, current.col)); 
}

void CfxGrid::SelectRows(CIdCell current)
{  
	if (current.row < m_nFixedRows || !IsValid(current))
		return;

	SetSelectedRange(min(m_idSelect.row, current.row), GetFixedColumnCount(),
				max(m_idSelect.row, current.row), GetColumnCount() -1);
}

void CfxGrid::SelectCells(CIdCell current)
{
	if (current.col < m_nFixedRows || current.col < m_nFixedCols)
		return;
	if (!IsValid(current))
		return;

	SetSelectedRange(min(m_idSelect.row, current.row), min(m_idSelect.col, current.col), 
			max(m_idSelect.row, current.row), max(m_idSelect.col, current.col)); 
}

void CfxGrid::OnSelecting(const CIdCell& current)
{
	switch (m_mousemode)
	{
	case mouseSELECTALL:
		SelectAllCells();
		break;
	case mouseSELECTCOL:
		SelectColumns(current);
		break;
	case mouseSELECTROW:
		SelectRows(current);
		break;
	case mouseSELECTCELLS:
		SelectCells(current);
		break;
	}
}

BOOL CfxGrid::IsRowResizeArea(const CPoint& point) const
{
	if (m_nFixedCols > 0)
	{
		if (point.x >= GetFixedColumnWidth())
			return FALSE;
	}
	else
	{
		if (point.x >= (int)m_aryColWidth[0])
			return FALSE;
	}

	const CIdCell current = GetCellFromPoint(point);
	CPoint start;
	if (!GetCellOrigin(current.row, current.col, &start))
		return FALSE;

	const int endy = start.y + GetRowHeight(current.row);
	if ((point.y - start.y <= rszCAPTURE && current.row != 0) || endy - point.y <= rszCAPTURE)
		return TRUE;

	return FALSE;
}

BOOL CfxGrid::IsColResizeArea(const CPoint& point) const
{
	if (point.y >= GetFixedRowHeight())
		return FALSE;

	CPoint start;
	const CIdCell current = GetCellFromPoint(point);
	if (!GetCellOrigin(current.row, current.col, &start)) 
		return FALSE;

	const int endx = start.x + GetColumnWidth(current.col);
    
	if ((point.x - start.x <= rszCAPTURE && current.col != 0) || endx - point.x <= rszCAPTURE)
		return TRUE;
	
	return FALSE;
}

CIdCell CfxGrid::GetCellFromPoint(CPoint point, BOOL bAllowFixedCellCheck /*=TRUE*/) const
{  
	CIdCell cell;
	const CIdCell tlCell = GetTopleftNonFixedCell();
	int row{}, col{};

	const int fixedColWidth = GetFixedColumnWidth();
	if (point.x < 0 || (!bAllowFixedCellCheck && point.x < fixedColWidth))
		cell.col = -1;
	else if (point.x < fixedColWidth) // in fixed col
	{
		int xpos = 0;
		for ( col = 0; col < m_nFixedCols; col++)
		{
			xpos += GetColumnWidth(col);
			if (xpos > point.x) break;
		}
		cell.col = col;
	}
	else
	{
		int xpos = fixedColWidth;
		for ( col = tlCell.col; col < GetColumnCount(); col++)
		{
			xpos += GetColumnWidth(col);
			if (xpos > point.x) break;
		}

		if (col >= GetColumnCount())
			cell.col = -1;
		else
			cell.col = col;
	}
    
	const int fixedRowHeight = GetFixedRowHeight();
	if (point.y < 0 || (!bAllowFixedCellCheck && point.y < fixedRowHeight)) // not in window
		cell.row = -1;
	else if (point.y < fixedRowHeight) // in fixed col
	{
		int ypos = 0;
		for ( row = 0; row < m_nFixedRows; row++)
		{
			ypos += GetRowHeight(row);
			if (ypos > point.y) 
				break;
		}
		cell.row = row;
	}
	else
	{
		int ypos = fixedRowHeight;
		for ( row = tlCell.row; row < GetRowCount(); row++)
		{
			ypos += GetRowHeight(row);
			if (ypos > point.y) 
				break;
		}

		if (row >= GetRowCount())
			cell.row = -1;
		else
			cell.row = row;
	}

	return cell;
}

////////////////////////////////////////////////////////////////////////////////
// cellrange functions

CIdCell CfxGrid::GetTopleftNonFixedCell() const
{
	const int Vscroll = GetScrollPos(SB_VERT);
	const int Hscroll = GetScrollPos(SB_HORZ);
	int nColumn = m_nFixedCols;
	int nRight = 0;

	while (nRight < Hscroll && nColumn < (GetColumnCount()-1))
		nRight += GetColumnWidth(nColumn++);

	int nRow = m_nFixedRows, nTop = 0;
	while (nTop < Vscroll && nRow < (GetRowCount()-1))
		nTop += GetRowHeight(nRow++);

	return CIdCell(nRow, nColumn);
}

CRangeCell CfxGrid::GetVisibleNonFixedCellRange(LPRECT pRect) const
{
	CRect rect; GetClientRect(rect);
	const CIdCell tlCell = GetTopleftNonFixedCell();

	// calc bottom
	int bottom = GetFixedRowHeight();
	int ii;
	for (ii = tlCell.row; ii < GetRowCount(); ii++)
	{
		bottom += GetRowHeight(ii);
		if (bottom >= rect.bottom)
		{
			bottom = rect.bottom;
			break;
		}
	}                                
	const int maxVisibleRow = min(ii, GetRowCount() - 1);
    
	// calc right
	int right = GetFixedColumnWidth();
	for (ii = tlCell.col; ii < GetColumnCount(); ii++)
	{
		right += GetColumnWidth(ii);
		if (right >= rect.right)
		{
			right = rect.right;
			break;
		}
	}
	const int maxVisibleCol = min(ii, GetColumnCount() - 1);
	if (pRect)
	{
		pRect->left = pRect->top = 0;
		pRect->right = right;
		pRect->bottom = bottom;
	}

	return CRangeCell(tlCell.row, tlCell.col, maxVisibleRow, maxVisibleCol);
}

CRangeCell CfxGrid::GetUnobstructedNonFixedCellRange() const
{
	CRect rect; GetClientRect(rect);

	const CIdCell tlCell = GetTopleftNonFixedCell();

	// calc bottom
	int bottom = GetFixedRowHeight();
	int ii;
	for (ii = tlCell.row; ii < GetRowCount(); ii++)
	{
		bottom += GetRowHeight(ii);
		if (bottom >= rect.bottom)
			break;
	}
	int maxVisibleRow = min(ii, GetRowCount() - 1);
	if (maxVisibleRow > 0 && bottom > rect.bottom)
		maxVisibleRow--;

	// calc right
	int right = GetFixedColumnWidth();
	for (ii = tlCell.col; ii < GetColumnCount(); ii++)
	{
		right += GetColumnWidth(ii);
		if (right >= rect.right) 
			break;
	}
	int maxVisibleCol = min(ii, GetColumnCount() - 1);
	if (maxVisibleCol > 0 && right > rect.right)
		maxVisibleCol--;

	return CRangeCell(tlCell.row, tlCell.col, maxVisibleRow, maxVisibleCol);
}

CRangeCell CfxGrid::GetSelectedCellRange(bool onlyrow) const
{
	CRangeCell range(GetRowCount(), GetColumnCount(), -1, -1);

// updateX_20060113
	if (onlyrow)
	{
		range.SetMinRow(m_idSelect.row);
		range.SetMaxRow(m_idSelect.row);
		range.SetMinCol(0);
		range.SetMaxCol(GetColumnCount()-1);
		return range;
	}

	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != NULL; )
	{
		DWORD key; CIdCell cell;
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)cell);

		range.SetMinRow(min(range.GetMinRow(), cell.row));
		range.SetMinCol(min(range.GetMinCol(), cell.col));
		range.SetMaxRow(max(range.GetMaxRow(), cell.row));
		range.SetMaxCol(max(range.GetMaxCol(), cell.col));
	}

	return range;
}

CRangeCell CfxGrid::GetCellRange() const
{
	return CRangeCell(0, 0, GetRowCount() - 1, GetColumnCount() - 1);
}

void CfxGrid::ResetSelectedRange()
{
	m_mapPreCell.RemoveAll();
	SetSelectedRange(-1, -1, -1, -1);
	SetFocusCell(-1, -1);
}

int CfxGrid::GetScrollPos32(int nBar, BOOL bGetTrackPos)
{
	SCROLLINFO sinfo;
	sinfo.cbSize = sizeof(SCROLLINFO);

	if (bGetTrackPos)
	{
		if (GetScrollInfo(nBar, &sinfo, SIF_TRACKPOS))
			return sinfo.nTrackPos;
	}
	else 
	{
		if (GetScrollInfo(nBar, &sinfo, SIF_POS))
			return sinfo.nPos;
	}

	return 0;
}

BOOL CfxGrid::SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
{
	SCROLLINFO sinfo;

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask  = SIF_POS;
	sinfo.nPos   = nPos;

	return SetScrollInfo(nBar, &sinfo, bRedraw);
}

void CfxGrid::ResetScrollBars()
{
	if (!::IsWindow(GetSafeHwnd())) 
		return;

	CRect rect; GetClientRect(rect);

	rect.left += GetFixedColumnWidth();
	rect.top  += GetFixedRowHeight();
	if (rect.left >= rect.right || rect.top >= rect.bottom)
		return;

	const CRect visibleRC(GetFixedColumnWidth(), GetFixedRowHeight(), rect.right, rect.bottom);
	const CRect virtualRC(GetFixedColumnWidth(), GetFixedRowHeight(), GetVirtualWidth(), GetVirtualHeight());

	const CRangeCell visibleCells = GetUnobstructedNonFixedCellRange();
	if (!IsValid(visibleCells)) return;

	m_Vscroll = 0;
	if (visibleRC.Height() < virtualRC.Height()) m_Vscroll = virtualRC.Height() - 1;

	m_Hscroll = 0;
	if (visibleRC.Width() < virtualRC.Width()) m_Hscroll = virtualRC.Width() - 1;

	switch (m_NOscroll)
	{
	case GVSC_NONE:
		m_Vscroll = 0; m_Hscroll = 0; break;
	case GVSC_HORZ:
		m_Vscroll = 0;	break;
	case GVSC_VERT:
		m_Hscroll = 0;	break;
	default:
		break;
	}

	SCROLLINFO sinfo;

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask  = SIF_PAGE|SIF_RANGE;
	if (m_DIscroll) sinfo.fMask |= SIF_DISABLENOSCROLL;

	sinfo.nPage  = visibleRC.Height();
	sinfo.nMin   = 0; 
	sinfo.nMax   = m_Vscroll; SetScrollInfo(SB_VERT, &sinfo, TRUE);

	sinfo.nPage  = visibleRC.Width();
	sinfo.nMin   = 0; 
	sinfo.nMax   = m_Hscroll; SetScrollInfo(SB_HORZ, &sinfo, TRUE);
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column position functions
BOOL CfxGrid::GetCellOrigin(int nRow, int nCol, LPPOINT pt) const
{
	if (!IsValid(CIdCell(nRow, nCol))) return FALSE;
	if (IsCellAttribute(CIdCell(nRow, nCol), GVAT_HIDDEN)) return FALSE;

	CIdCell tlCell;
	if (nCol >= m_nFixedCols || nRow >= m_nFixedRows)
		tlCell = GetTopleftNonFixedCell();

	if ((nRow >= m_nFixedRows && nRow < tlCell.row) || (nCol >= m_nFixedCols && nCol < tlCell.col))
		return FALSE;

	int ii{}; pt->x = 0;
	if (nCol < m_nFixedCols)                      // is a fixed column
	{
		for (ii = 0; ii < nCol; ii++)
			pt->x += GetColumnWidth(ii);
	}
	else
	{                                        // is a scrollable data column
		for (ii = 0; ii < m_nFixedCols; ii++)
			pt->x += GetColumnWidth(ii);
		for (ii = tlCell.col; ii < nCol; ii++)
			pt->x += GetColumnWidth(ii);
	}

	pt->y = 0;
	if (nRow < m_nFixedRows)                      // is a fixed row
	{
		for (ii = 0; ii < nRow; ii++)
			pt->y += GetRowHeight(ii);
	}
	else
	{                                        // is a scrollable data row
		for (ii = 0; ii < m_nFixedRows; ii++)
			pt->y += GetRowHeight(ii);
		for (ii = tlCell.row; ii < nRow; ii++)
			pt->y += GetRowHeight(ii);
	}

	return TRUE;
}

BOOL CfxGrid::GetCellRect(int nRow, int nCol, LPRECT pRect) const
{    
	CPoint origin;
	if (!GetCellOrigin(nRow, nCol, &origin))
		return FALSE;

	pRect->left   = origin.x;
	pRect->top    = origin.y;
	pRect->right  = origin.x + GetColumnWidth(nCol) -1;
	pRect->bottom = origin.y + GetRowHeight(nRow) -1;

	return TRUE;
}

BOOL CfxGrid::GetTextRect(int nRow, int nCol, LPRECT pRect)
{
	GVITEM	gvitem;

	gvitem.mask = GVMK_IMAGE;
	gvitem.row = nRow;
	gvitem.col = nCol;

	if (!GetItem(&gvitem)) return FALSE;
	if (!GetCellRect(nRow, nCol, pRect)) return FALSE;

	if (m_imageList && gvitem.image >= 0)
	{
		IMAGEINFO Info;
		if (m_imageList->GetImageInfo(gvitem.image, &Info))
		{
			const int nImageWidth = Info.rcImage.right - Info.rcImage.left +1;
			pRect->left += nImageWidth + m_margin;
		}
	}

	return TRUE;
}

BOOL CfxGrid::GetCellRangeRect(const CRangeCell& range, LPRECT lpRect) const
{
	CPoint minOrigin, maxOrigin;

	if (!GetCellOrigin(range.GetMinRow(), range.GetMinCol(), &minOrigin)) 
		return FALSE;
	if (!GetCellOrigin(range.GetMaxRow(), range.GetMaxCol(), &maxOrigin)) 
		return FALSE;

	lpRect->left   = minOrigin.x;
	lpRect->top    = minOrigin.y;
	lpRect->right  = maxOrigin.x + GetColumnWidth(range.GetMaxCol() -1);
	lpRect->bottom = maxOrigin.y + GetRowHeight(range.GetMaxRow() -1);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// Grid attribute functions

LRESULT CfxGrid::OnSetFont(WPARAM hFont, LPARAM lParam)
{
	const LRESULT result = Default();

	LOGFONT logfont;
	if (!GetObject((HFONT) hFont, sizeof(LOGFONT), &logfont))
		return result;
//    	memcpy(&m_logfont, &logfont, sizeof(LOGFONT));
    
	for (int row = 0; false && row < GetRowCount(); row++)
	{
		for (int col = 0; col < GetColumnCount(); col++)
			SetItemFont(row, col, &logfont);
	}

	// Get the font size and hence the default cell size
	CDC* pDC = GetDC();

	if (pDC) 
	{
		m_font.DeleteObject();
		m_font.CreatePointFontIndirect(&m_logfont);
		CFont* pOldFont = pDC->SelectObject(&m_font);

		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);

		m_margin = pDC->GetOutputTextExtent(_T(" "), 1).cx; m_margin /= 2;
		pDC->SelectObject(pOldFont);

		m_defHeight = tm.tmHeight + tm.tmExternalLeading + 2 * m_margin;
		m_defWidth  = tm.tmAveCharWidth * 4 + 2 * m_margin;
	}

	ReleaseDC(pDC);

	if (::IsWindow(GetSafeHwnd())) 
		Invalidate();

	return result;
}

LRESULT CfxGrid::OnGetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return (LRESULT) (HFONT) m_font;
}

BOOL CfxGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest != HTCLIENT)
		return CWnd::OnSetCursor(pWnd, nHitTest, message);

	switch (m_mousemode) 
	{
	case mouseOVERCOLDIVIDE:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		break;
	case mouseOVERROWDIVIDE:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		break;
	case mouseMOVING:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		break;
	case mouseDRAGGING:
		break;
	default:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		break;
	}
	return TRUE;
}

BOOL CfxGrid::SetFixedRowCount(int nFixedRows)
{
	ASSERT(nFixedRows >= 0);

	if (nFixedRows > GetRowCount() && !SetRowCount(nFixedRows)) 
		return FALSE;

	if (m_idCurrent.row < nFixedRows)
		SetFocusCell(-1, -1);

	m_nFixedRows = nFixedRows;

	if (GetSafeHwnd()) Invalidate();

	return TRUE;
}

// jdh
BOOL CfxGrid::SetFixedColumnCount(int nFixedCols)
{
	ASSERT(nFixedCols >= 0);

	if (nFixedCols > GetColumnCount() && !SetColumnCount(nFixedCols))
		return FALSE;

	if (m_idCurrent.col < nFixedCols)
		SetFocusCell(-1,-1);

	m_nFixedCols = nFixedCols;

	if (GetSafeHwnd()) Invalidate();

	return TRUE;
}

BOOL CfxGrid::SetRowCount(int nRows)
{
	ASSERT(nRows >= 0);

	if (nRows == GetRowCount())
		return TRUE;

	if (nRows < m_nFixedRows) 
		m_nFixedRows = nRows;

	if (m_idCurrent.row >= nRows)
		SetFocusCell(-1, -1);

	const int addedRows = nRows - GetRowCount();
	if (addedRows < 0)
	{
		for (int row = nRows; row < m_nRows; row++)
		{
			for (int col = 0; col < m_nCols; col++) 
			{
				CCell* pCell = GetCell(row, col);
				if (pCell)
				{
					EmptyCell(pCell, row, col);
					delete pCell;
				}
			}
			GRID_ROW* pRow = m_aryGridRows[row];
			if (pRow) delete pRow;
		}
	}

	m_nRows = nRows;
	m_aryGridRows.SetSize(m_nRows);
	m_aryRowHeight.SetSize(m_nRows);

	if (addedRows > 0)
	{
		// initialize row heights and data
		const int startRow = nRows - addedRows;
		for (int row = startRow; row < GetRowCount(); row++)
		{
			m_aryRowHeight[row] = m_defHeight;
			m_aryGridRows[row] = new GRID_ROW;
			m_aryGridRows[row]->SetSize(m_nCols);
			for (int col = 0; col < m_nCols; col++)
			{
				GRID_ROW* pRow = m_aryGridRows[row];
				if (pRow) pRow->SetAt(col, CreateCell(row, col));
			}
		}
	}
    
	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}

	return TRUE;
}

BOOL CfxGrid::SetColumnCount(int nCols)
{
	ASSERT(nCols >= 0);

	if (nCols == GetColumnCount())
		return TRUE;

	if (nCols < m_nFixedCols) 
		m_nFixedCols = nCols;

	if (m_idCurrent.col >= nCols)
		SetFocusCell(-1, -1);

	const int addedCols = nCols - GetColumnCount();
	if (addedCols < 0)
	{
		for (int row = 0; row < m_nRows; row++)
		{
			for (int col = nCols; col < GetColumnCount(); col++)
			{
				CCell* pCell = GetCell(row, col);
				if (pCell)
				{
					EmptyCell(pCell, row, col);
					delete pCell;
				}
			}
		}
	}

	m_nCols = nCols;
	m_aryColWidth.SetSize(m_nCols);

	for (int ii = 0; ii < m_nRows; ii++)
	{
		if (m_aryGridRows[ii])
			m_aryGridRows[ii]->SetSize(nCols);
	}

	if (addedCols > 0)
	{
		const int startCol = nCols - addedCols;
		for (int col = startCol; col < GetColumnCount(); col++)
			m_aryColWidth[col] = m_defWidth;

		for (int row = 0; row < m_nRows; row++)
		{
			for (int col = startCol; col < GetColumnCount(); col++)
			{
				GRID_ROW* pRow = m_aryGridRows[row];
				if (pRow) pRow->SetAt(col, CreateCell(row,col));
			}
		}
	}
    
	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}
	return TRUE;
}

// jdh
int CfxGrid::InsertColumn(LPCTSTR strHeading, UINT nFormat, int nColumn)
{
	if (nColumn >= 0 && nColumn >= GetColumnCount())
		return -1;

//	ResetSelectedRange();
	SetFocusCell(-1, -1);

	if (m_nRows < 1)
		SetRowCount(1);    
	if (nColumn < 0)
	{
		nColumn = m_nCols;
		m_aryColWidth.Add(0);
		for (int row = 0; row < m_nRows; row++) 
		{
			GRID_ROW* pRow = m_aryGridRows[row];
			if (!pRow) return -1;
			pRow->Add(CreateCell(row, nColumn));
		}
	} 
	else
	{
		//m_aryColWidth.InsertAt(nColumn, (int)0);
		m_aryColWidth.InsertAt(nColumn, 0, 1);  //vc2019 
		for (int row = 0; row < m_nRows; row++) 
		{
			GRID_ROW* pRow = m_aryGridRows[row];
			if (!pRow) return -1;
			pRow->InsertAt(nColumn, CreateCell(row, nColumn));
		}
	}

	m_nCols++;

	SetItemText  (0, nColumn, strHeading);
	SetItemFormat(0, nColumn, GVFM_HEADER);

	for (int row = 0; row < m_nRows; row++) 
		SetItemFormat(row, nColumn, nFormat);

	// initialized column width
	AutoSizeColumn(nColumn);

	if (m_idCurrent.col != -1 && nColumn < m_idCurrent.col)
		m_idCurrent.col++;

	UpdateColSelectedRange(nColumn, true);

	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}
	return nColumn;
}

int CfxGrid::InsertRow(LPCTSTR strHeading, int nRow)
{
	if (nRow >= 0 && nRow > GetRowCount())
		return -1;

//	ResetSelectedRange();
	SetFocusCell(-1, -1);

	if (m_nCols < 1)
		SetColumnCount(1);    
	if (nRow < 0) 
	{
		nRow = m_nRows;
		m_aryRowHeight.Add(0);
		m_aryGridRows.Add(new GRID_ROW);
	} 
	else 
	{
		//m_aryRowHeight.InsertAt(nRow, (int)0);  //vc2019
		//m_aryRowHeight.InsertAt(nRow, nullptr);
		m_aryRowHeight.InsertAt(nRow, 0, 1);
		m_aryGridRows.InsertAt(nRow, new GRID_ROW);
	}
    
	m_nRows++;
	m_aryGridRows[nRow]->SetSize(m_nCols);

	// Initialise cell data
	for (int col = 0; col < m_nCols; col++)
	{
		GRID_ROW* pRow = m_aryGridRows[nRow];
		if (!pRow) return -1;
		pRow->SetAt(col, CreateCell(nRow, col));
	}

	SetItemText(nRow, 0, strHeading);

	m_aryRowHeight[nRow] = GetTextExtent(strHeading).cy;

	if (m_idCurrent.row != -1 && nRow < m_idCurrent.row)
		m_idCurrent.row++;
    
	if (GetSafeHwnd())
	{
		ResetScrollBars(); Invalidate();
	}

	UpdateRowSelectedRange(nRow, true);
	return nRow;
}

int CfxGrid::InsertRow(int drag, int& drop)
{
	if (drag < 0 || drag > GetRowCount())
		return -1;
	if (drop < 0 || drop > GetRowCount())
		return -1;

	if (drag == drop)
		return 0;

	ResetSelectedRange();

	const int height = m_aryRowHeight[drag];
	m_aryRowHeight.RemoveAt(drag);

	GRID_ROW* tempROW = new GRID_ROW;
	GRID_ROW* dragROW = m_aryGridRows[drag];
	tempROW->SetSize(dragROW->GetSize());

	CCell* cell{};
	for (int ii = 0; ii < dragROW->GetSize(); ii++)
	{
		cell = dragROW->GetAt(ii);
		cell->text = _T("");
		tempROW->SetAt(ii, cell);
	}
	delete dragROW;
	m_aryGridRows.RemoveAt(drag);

	if (drop > drag) drop -= 1;

	m_aryRowHeight.InsertAt(drop, height);
	m_aryGridRows.InsertAt (drop, tempROW);

	if (GetSafeHwnd()) ResetScrollBars();
	Invalidate();

	return 0;
}

CCell* CfxGrid::CreateCell(int nRow, int nCol)
{
	CCell* pCell = new CCell;
	if (!pCell) return NULL;
    
	int refRow = 0, refCol = 0;

	// row insert
	if (nRow > 0 && nCol >= 0 && nCol < m_nCols)
	{
		refRow = nRow -1;
		if (refRow < m_nFixedRows && nRow < m_nRows - m_nFixedRows)
			refRow = nRow +1;

		pCell->attr   = GetItemAttr(refRow, nCol);
		pCell->format = GetItemFormat(refRow, nCol);
		pCell->fgcol  = GetItemFgColor(refRow, nCol);
		pCell->bkcol  = GetItemBkColor(refRow, nCol);
		pCell->symbol = GetItemSymbol(refRow, nCol);
		pCell->misc   = GetItemMisc(refRow, nCol);
	}
	else if (nRow > 0 && nCol >= 0 && nCol == m_nCols)
	{
		refCol = nCol -1;
		pCell->fgcol  = GetItemFgColor(nRow, refCol);
		pCell->bkcol  = GetItemBkColor(nRow, refCol);
	}

	memcpy(&(pCell->font), &m_logfont, sizeof(LOGFONT));
    	return pCell;
}

void CfxGrid::EmptyCell(CCell* pCell, int nRow, int nCol)
{
	SetItemState(nRow, nCol, 0);
	pCell->text.Empty();
}

BOOL CfxGrid::DeleteColumn(int nColumn)
{
	if (nColumn < 0 || nColumn >= GetColumnCount())
		return FALSE;

//	ResetSelectedRange();
	SetFocusCell(-1, -1);

	for (int row = 0; row < GetRowCount(); row++) 
	{
		GRID_ROW* pRow = m_aryGridRows[row];
		if (!pRow)
			return FALSE;

		CCell* pCell = pRow->GetAt(nColumn);
		if (pCell)
		{
			EmptyCell(pCell, row, nColumn);
			delete pCell;
		}
		pRow->RemoveAt(nColumn);
	}

	m_aryColWidth.RemoveAt(nColumn);
	m_nCols--;
	if (nColumn < m_nFixedCols)
		m_nFixedCols--;

	if (nColumn == m_idCurrent.col)
		m_idCurrent.row = m_idCurrent.col = -1;
	else if (nColumn < m_idCurrent.col)
		m_idCurrent.col--;

	UpdateColSelectedRange(nColumn, false);

	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}

	return TRUE;
}

BOOL CfxGrid::DeleteRow(int nRow)
{
	if (nRow < 0 || nRow >= GetRowCount())
		return FALSE;

	GRID_ROW* pRow = m_aryGridRows[nRow];
	if (!pRow) return FALSE;

//	ResetSelectedRange();
	SetFocusCell(-1, -1);

	for (int col = 0; col < GetColumnCount(); col++) 
	{
		CCell* pCell = pRow->GetAt(col);
		if (pCell)
		{
			EmptyCell(pCell, nRow, col);
			delete pCell;
		}
	}
	delete pRow;

	m_aryGridRows.RemoveAt(nRow);
	m_aryRowHeight.RemoveAt(nRow);

	m_nRows--;
	if (nRow < m_nFixedRows)
		m_nFixedRows--;

	if (nRow == m_idCurrent.row)
		m_idCurrent.row = m_idCurrent.col = -1;
	else if (nRow < m_idCurrent.row)
		m_idCurrent.row--;

	UpdateRowSelectedRange(nRow, false);

	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}

	return TRUE;
}

BOOL CfxGrid::DeleteNonFixedRows()
{
	const int nFixed = GetFixedRowCount();
	const int nCount = GetRowCount();

	for (int nRow = nCount; nRow >= nFixed; nRow--) 
		DeleteRow(nRow);

	Invalidate();
	return TRUE;
}

BOOL CfxGrid::DeleteAllItems()
{
	ClearAllRTM(); ClearMerge(); ResetSelectedRange();

	m_aryColWidth.RemoveAll();
	m_aryRowHeight.RemoveAll();

	for (int row = 0; row < m_nRows; row++) 
	{
		GRID_ROW* pRow = m_aryGridRows[row];
		if (!pRow) continue;

		for (int col = 0; col < m_nCols; col++)
		{
			CCell* pCell = pRow->GetAt(col);
			if (pCell)
			{
				EmptyCell(pCell, row, col);
				delete pCell;  
			}                                
		}
		delete pRow;
	}

	m_aryGridRows.RemoveAll();

	m_idCurrent.row = m_idCurrent.col = -1;
	m_nRows = m_nFixedRows = m_nCols = m_nFixedCols = 0;

	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Set CListCtrl::GetNextItem for details

BOOL CfxGrid::SortTextItems(int nCol, BOOL bAscending)
{
	ClearAllRTM(); ResetSelectedRange();

	CString	string;
	int eRow;
	for (eRow = m_nRows - m_nFixedRows; eRow >= m_nFixedRows; eRow--)
	{
		string = GetItemText(eRow, nCol);
		if (!string.IsEmpty())
		{
			eRow += 1; break;
		}
	}

	if (eRow > GetRowCount()) eRow = m_nRows - m_nFixedRows;

	return SortTextItems(nCol, bAscending, GetFixedRowCount(), eRow);
}

BOOL CfxGrid::SortTextItems(int sortcol, BOOL ascending, int srow, int erow)
{
	if (sortcol >= GetColumnCount())
		return FALSE;
	if (erow == -1) erow = GetRowCount() -1;

	int cellType = cellNot;
	if (GetItemAttr(0, sortcol) & GVAT_SORTVAL)
		cellType = cellVal;
	else if (GetItemAttr(0, sortcol) & GVAT_SORTABS)
		cellType = cellAbs;
	else if (GetItemAttr(0, sortcol) & GVAT_SORTSTR)
		cellType = cellStr;
	else
		return FALSE;

	bool	swap = false;
	CString	string, stringx;
	for (int xrow = srow; xrow < erow-1; xrow++)
	{
		for (int yrow = xrow+1; yrow < erow; yrow++)
		{
			string  = GetItemText(xrow, sortcol);
			stringx = GetItemText(yrow, sortcol);

			if (GetItemAttr(xrow, sortcol) & GVAT_CONTRAST)
			{
				if (string.GetLength() > 0)
				{
					switch (string.GetAt(0))
					{
					case '1': case '2':
						string = string.Mid(1);	string.Insert(0, '+');
						break;
					case '3':
						string = string.Mid(1);
						break;
					case '4': case '5':
						string = string.Mid(1); string.Insert(0, '-');
					}
				}
				if (stringx.GetLength() > 0)
				{
					switch (stringx.GetAt(0))
					{
					case '1': case '2':
						stringx = stringx.Mid(1); stringx.Insert(0, '+');
						break;
					case '3':
						stringx = stringx.Mid(1);
						break;
					case '4': case '5':
						stringx = stringx.Mid(1); stringx.Insert(0, '-');
					}
				}
			}

			switch (cellType)
			{
			case cellVal:
				if (ascending)
				{
					if (string.IsEmpty() && !stringx.IsEmpty())
						swap = true;
					else if (!stringx.IsEmpty() && atof(string) > atof(stringx))
						swap = true;
				}
				else
				{
					if (string.IsEmpty() && !stringx.IsEmpty())
						swap = true;
					else if (!stringx.IsEmpty() && atof(string) < atof(stringx))
						swap = true;
				}
				break;
			case cellAbs:
				if (ascending)
				{
					if (string.IsEmpty() && !stringx.IsEmpty())
						swap = true;
					else if (!stringx.IsEmpty() && TOfabs(string) > TOfabs(stringx))
						swap = true;
				}
				else
				{
					if (string.IsEmpty() && !stringx.IsEmpty())
						swap = true;
					else if (!stringx.IsEmpty() && TOfabs(string) < TOfabs(stringx))
						swap = true;
				}
				break;
			case cellStr:
				if (ascending)
				{
					if (string.IsEmpty() && !stringx.IsEmpty())
						swap = true;
					else if (!stringx.IsEmpty() && string > stringx)
						swap = true;
				}
				else
				{
					if (string.IsEmpty() && !stringx.IsEmpty())
						swap = true;
					else if (!stringx.IsEmpty() && string < stringx)
						swap = true;
				}
				break;
			}

			if (swap)
			{
				for (int col = 0; col < GetColumnCount(); col++)
				{
					CCell* pCell = GetCell(xrow, col);
					SetCell(xrow, col, GetCell(yrow, col));
					SetCell(yrow, col, pCell);
				}
				unsigned int rowH    = m_aryRowHeight[xrow];
				m_aryRowHeight[xrow] = m_aryRowHeight[yrow];
				m_aryRowHeight[yrow] = rowH;

				swap = false;
			}
		}
	}

	return TRUE;
}

BOOL CfxGrid::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data)
{
	ResetSelectedRange();
	return SortItems(pfnCompare, nCol, bAscending, data, GetFixedRowCount(), -1);
}

BOOL CfxGrid::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, int low, int high)
{
	if (nCol >= GetColumnCount())
		return FALSE;
	if (high == -1)
		high = GetRowCount() - 1;

	int lo = low;
	int hi = high;

	if (hi <= lo) return FALSE;

	const LPARAM midItem = GetItemData((lo+hi)/2, nCol);

	while (lo <= hi)
	{
		if (bAscending)
		{
			while (lo < high  && pfnCompare(GetItemData(lo, nCol), midItem, data) < 0)
				++lo;
		}
		else
		{
			while (lo < high && pfnCompare(GetItemData(lo, nCol), midItem, data) > 0)
				++lo;
		}

		if (bAscending)
		{
			while (hi > low && pfnCompare(GetItemData(hi, nCol), midItem, data) > 0)
				--hi;
		}
		else
		{
			while (hi > low && pfnCompare(GetItemData(hi, nCol), midItem, data) < 0)
				--hi;
		}

		if (lo <= hi)
		{
			if (pfnCompare(GetItemData(lo, nCol), GetItemData(hi, nCol), data) != 0)
			{
				for (int col = 0; col < GetColumnCount(); col++)
				{
					CCell *pCell = GetCell(lo, col);
					SetCell(lo, col, GetCell(hi, col));
					SetCell(hi, col, pCell);
				}
				UINT nRowHeight = m_aryRowHeight[lo];
				m_aryRowHeight[lo] = m_aryRowHeight[hi];
				m_aryRowHeight[hi] = nRowHeight;
			}
			++lo; --hi;
		}
	}

	if (low < hi)  SortItems(pfnCompare, nCol, bAscending, data, low, hi);
	if (lo < high) SortItems(pfnCompare, nCol, bAscending, data, lo, high);

	return TRUE;
}

BOOL CfxGrid::SetItem(const GVITEM* gvitem)
{
	if (!gvitem) return FALSE;

	CCell* pCell = GetCell(gvitem->row, gvitem->col);
	if (!pCell) return FALSE;

	pCell->state = 0; pCell->misc = _T(""); pCell->param = 0;
	
	if (gvitem->mask & GVMK_STATE)   pCell->state = gvitem->state;
	if (gvitem->mask & GVMK_ATTR)	 pCell->attr  = gvitem->attr;
	if (gvitem->mask & GVMK_FORMAT)  pCell->format= gvitem->format;
	if (gvitem->mask & GVMK_TEXT)    pCell->text  = gvitem->text;
	if (gvitem->mask & GVMK_IMAGE)   pCell->image = gvitem->image;
	if (gvitem->mask & GVMK_FONT)    memcpy(&(pCell->font), &(gvitem->font), sizeof(LOGFONT));
	if (gvitem->mask & GVMK_FGCOLOR) pCell->fgcol = gvitem->fgcol;
	if (gvitem->mask & GVMK_BKCOLOR) pCell->bkcol = gvitem->bkcol;
	if (gvitem->mask & GVMK_SYMBOL)	 pCell->symbol= gvitem->symbol;
	if (gvitem->mask & GVMK_MISC)	 pCell->misc  = gvitem->misc;
	if (gvitem->mask & GVMK_PARAM)   pCell->param = gvitem->param;

	if (pCell->attr & GVAT_EDIT) m_focusLine = TRUE;
	if (pCell->attr & GVAT_HIDDEN) SetColumnWidth(gvitem->col, 0);

	if (gvitem->attr & GVAT_HDRMERGE && !gvitem->misc.IsEmpty())
	{
		int key = -1; CString temp;
		CString	string = gvitem->misc;

		if ((key = string.Find('|')) != -1)
		{
			temp = string.Left(key++);
			string = string.Mid(key);
			key = atoi(temp);
		}

		_fixMerge fixMerge;
		if (!m_mapMerge.Lookup(key, fixMerge))
		{
			for (int ii = 0; ii < maxMERGE; ii++)
				fixMerge.cols[ii] = -1;

			fixMerge.cols[0] = gvitem->col;
			fixMerge.rect.SetRectEmpty();

			fixMerge.fgcol = gvitem->fgcol;
			fixMerge.bkcol = gvitem->param;
			fixMerge.format= gvitem->format;
			fixMerge.text  = string;
			CopyMemory(&(fixMerge.font), &(gvitem->font), sizeof(LOGFONT));

			m_mapMerge.SetAt(key, fixMerge);
		}
		else
		{
			for (int ii = 0; ii < maxMERGE; ii++)
			{
				if (fixMerge.cols[ii] == -1)
				{
					fixMerge.cols[ii] = gvitem->col;
					break;
				}
			}
			m_mapMerge.SetAt(key, fixMerge);
		}
	}

	return TRUE;
}

BOOL CfxGrid::GetItem(GVITEM* gvitem)
{
	if (!gvitem) return FALSE;

	const CCell* pCell = GetCell(gvitem->row, gvitem->col);
	if (!pCell) return FALSE;

	if (gvitem->mask & GVMK_STATE)	 gvitem->state = pCell->state;
	if (gvitem->mask & GVMK_ATTR)	 gvitem->attr = pCell->attr;
	if (gvitem->mask & GVMK_FORMAT)  gvitem->format= pCell->format;
	if (gvitem->mask & GVMK_TEXT)    gvitem->text  = GetItemText(gvitem->row, gvitem->col);
	if (gvitem->mask & GVMK_IMAGE)   gvitem->image = pCell->image;
	if (gvitem->mask & GVMK_FONT)    memcpy(&(gvitem->font), &(pCell->font), sizeof(LOGFONT));
	if (gvitem->mask & GVMK_FGCOLOR) gvitem->fgcol = pCell->fgcol;
	if (gvitem->mask & GVMK_BKCOLOR) gvitem->bkcol = pCell->bkcol;
	if (gvitem->mask & GVMK_SYMBOL)	 gvitem->symbol= pCell->symbol;
	if (gvitem->mask & GVMK_MISC)	 gvitem->misc  = pCell->misc;
	if (gvitem->mask & GVMK_PARAM)   gvitem->param = pCell->param;

	return TRUE;
}

BOOL CfxGrid::SetColumnItems(int nCol, GVITEM* gvitem)
{
	if (!gvitem || nCol >= GetColumnCount())
		return FALSE;

	for (int row = GetFixedRowCount(); row < GetRowCount(); row++)
	{
		gvitem->col = nCol;
		gvitem->row = row;
		SetItem(gvitem);
	}
	return TRUE;
}

BOOL CfxGrid::SetItemText(int nRow, int nCol, LPCTSTR str)
{
	CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->text = str;

	RedrawCell(nRow, nCol);
	return TRUE;
}

CString CfxGrid::GetItemText(int nRow, int nCol)
{
	if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
		return "";

	const CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return "";

	return pCell->text;
}

BOOL CfxGrid::SetItemData(int nRow, int nCol, LPARAM lParam)
{
	CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->param = lParam;

	RedrawCell(nRow, nCol);
	return TRUE;
}

LPARAM CfxGrid::GetItemData(int nRow, int nCol) const
{    
	const CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return (LPARAM) 0;

	return pCell->param;
}

BOOL CfxGrid::SetItemImage(int nRow, int nCol, int image)
{
	CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	if (pCell->image != image)
	{
		pCell->image = image;
		RedrawCell(nRow, nCol);
	}
	return TRUE;
}

int CfxGrid::GetItemImage(int nRow, int nCol) const
{
	const CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return -1;

	return pCell->image;
}

BOOL CfxGrid::SetItemState(int nRow, int nCol, UINT state)
{
	CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	// If the cell is being unselected, remove it from the selected list
	if ((pCell->state & GVST_SELECTED) && !(state & GVST_SELECTED))
	{
		CIdCell cell;
		const DWORD key = MAKELONG(nRow, nCol);

		if (m_mapSelCell.Lookup(key, (CIdCell&)cell))
			m_mapSelCell.RemoveKey(key);
	}
	else if (!(pCell->state & GVST_SELECTED) && (state & GVST_SELECTED))
	{
		CIdCell cell(nRow, nCol);
		m_mapSelCell.SetAt(MAKELONG(nRow, nCol), cell);
	}

	pCell->state = state;
	return TRUE;
}

UINT CfxGrid::GetItemState(int nRow, int nCol) const
{
	const CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return 0;

	return pCell->state;
}

BOOL CfxGrid::SetItemAttr(int nRow, int nCol, UINT attr)
{
	CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->attr = attr;

	if (attr & GVAT_HIDDEN)
		SetColumnWidth(nCol, 0);

	return TRUE;
}

UINT CfxGrid::GetItemAttr(int nRow, int nCol) const
{
	const CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return 0;

	return pCell->attr;
}

BOOL CfxGrid::SetItemFormat(int nRow, int nCol, UINT format)
{
	CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->format = format;
	return TRUE;
}

UINT CfxGrid::GetItemFormat(int nRow, int nCol) const
{
	const CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return 0;

	return pCell->format;
}

BOOL CfxGrid::SetItemBkColor(int nRow, int nCol, COLORREF col, bool force)
{
	CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	COLORREF bkcol{};
	const int key = nRow*10000 + nCol;
	if (!force && m_mapRTM.Lookup(key, bkcol))
	{
		m_sync.Lock();
		m_mapRTM.SetAt(key, col);
		m_sync.Unlock();

		return TRUE;
	}

	pCell->bkcol = col;
	RedrawCell(nRow, nCol);
	return TRUE;
}

COLORREF CfxGrid::GetItemBkColor(int nRow, int nCol) const
{
	const CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return 0;

	COLORREF bkcol{};
	const int key = nRow*10000 + nCol;
	if (m_mapRTM.Lookup(key, bkcol))
		return bkcol;

	return pCell->bkcol;
}

BOOL CfxGrid::SetItemFgColor(int nRow, int nCol, COLORREF col, bool force)
{
	CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->fgcol = col;

// updateX_20061128
	if (nRow < GetFixedRowCount() && pCell->attr & GVAT_HDRMERGE)
	{
		_fixMerge fixMerge;
		if (m_mapMerge.Lookup(nCol, fixMerge))
		{
			fixMerge.fgcol = col;
			m_mapMerge.SetAt(nCol, fixMerge);
		}
	}

	RedrawCell(nRow, nCol);
	return TRUE;
}

COLORREF CfxGrid::GetItemFgColor(int nRow, int nCol) const
{
	const CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return 0;

	return pCell->fgcol;
}

BOOL CfxGrid::SetItemFont(int nRow, int nCol, LOGFONT* logfont)
{
	CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	memcpy(&(pCell->font), logfont, sizeof(LOGFONT));
	RedrawCell(nRow, nCol);
	return TRUE;
}

LOGFONT* CfxGrid::GetItemFont(int nRow, int nCol)
{
	CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return &m_logfont;

	return &(pCell->font);
}

BOOL CfxGrid::SetItemParam(int nRow, int nCol, LPARAM param)
{
	CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->param = param;

// updateX_20061128
	if (nRow < GetFixedRowCount() && pCell->attr & GVAT_HDRMERGE)
	{
		_fixMerge fixMerge;
		if (m_mapMerge.Lookup(nCol, fixMerge))
		{
			fixMerge.bkcol = param;
			m_mapMerge.SetAt(nCol, fixMerge);
		}
	}

	RedrawCell(nRow, nCol);
	return TRUE;
}

LPARAM CfxGrid::GetItemParam(int nRow, int nCol)
{
	const CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return (LPARAM) -1;

	return pCell->param;
}

BOOL CfxGrid::SetItemSymbol(int nRow, int nCol, LPCTSTR str)
{
	CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->symbol = str;
	return TRUE;
}

CString CfxGrid::GetItemSymbol(int nRow, int nCol)
{
	const CCell* pCell = GetCell(nRow, nCol);
	//if (!pCell) return (LPARAM) -1; //vc2019
	if (!pCell) return _T("");;

	return pCell->symbol;
}

BOOL CfxGrid::SetItemMisc(int nRow, int nCol, LPCTSTR str)
{
	CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->misc = str;
	RedrawCell(nRow, nCol);
	return TRUE;
}

CString CfxGrid::GetItemMisc(int nRow, int nCol)
{
	if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
		return "";

	const CCell* pCell = GetCell(nRow, nCol);
	if (!pCell) return "";

	return pCell->misc;
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column size functions

long CfxGrid::GetVirtualWidth() const
{
	long width = 0;
	for (int ii = 0; ii < GetColumnCount(); ii++)
		width += m_aryColWidth[ii];

	return width;
}

long CfxGrid::GetVirtualHeight() const
{
	long height = 0;
	for (int ii = 0; ii < GetRowCount(); ii++)
		height += m_aryRowHeight[ii];

	return height;
}

int CfxGrid::GetRowHeight(int nRow) const
{
	ASSERT(nRow >= 0 && nRow < m_nRows);
	if (nRow < 0 || nRow >= m_nRows)
		return -1;

	return m_aryRowHeight[nRow];
}

int CfxGrid::GetColumnWidth(int nCol) const
{
	ASSERT(nCol >= 0 && nCol < m_nCols);
	if (nCol < 0 || nCol >= m_nCols)
		return -1;

	return m_aryColWidth[nCol];
}

BOOL CfxGrid::SetRowHeight(int nRow, int height)
{
	ASSERT(nRow >= 0 && nRow < m_nRows && height >= 0);
	if (nRow < 0 || nRow >= m_nRows || height < 0)
		return FALSE;

	m_aryRowHeight[nRow] = height;
	return TRUE;
}

BOOL CfxGrid::SetColumnWidth(int nCol, int width)
{
	ASSERT(nCol >= 0 && nCol < m_nCols && width >= 0);
	if (nCol < 0 || nCol >= m_nCols || width < 0)
		return FALSE;

	m_aryColWidth[nCol] = width;
	return TRUE;
}

int CfxGrid::GetFixedRowHeight() const
{
	int height = 0;
	for (int ii = 0; ii < m_nFixedRows; ii++)
		height += GetRowHeight(ii);

	return height;
}

int CfxGrid::GetFixedColumnWidth() const
{
	int width = 0;
	for (int ii = 0; ii < m_nFixedCols; ii++)
		width += GetColumnWidth(ii);

	return width;
}

BOOL CfxGrid::AutoSizeColumn(int nCol)
{
	ASSERT(nCol >= 0 && nCol < m_nCols);
	if (nCol < 0 || nCol >= m_nCols)
		return FALSE;

	CSize size;
	CDC* pDC = GetDC();
	if (!pDC) return FALSE;

	int width = 0;
	for (int nRow = 0; nRow < GetRowCount(); nRow++)
	{
		size = GetCellExtent(nRow, nCol, pDC);
		if (size.cx > width)
			width = size.cx;
	}

	m_aryColWidth[nCol] = width;

	ReleaseDC(pDC);
	ResetScrollBars();

	return TRUE;
}

BOOL CfxGrid::AutoSizeRow(int nRow)
{
	ASSERT(nRow >= 0 && nRow < m_nRows);
	if (nRow < 0 || nRow >= m_nRows)
		return FALSE;

	CSize size;
	CDC* pDC = GetDC();
	if (!pDC) return FALSE;

	int height = 0;
	for (int nCol = 0; nCol < GetColumnCount(); nCol++)
	{  
		size = GetCellExtent(nRow, nCol, pDC);
		if (size.cy > height)
			height = size.cy;
	}  
	m_aryRowHeight[nRow] = height;

	ReleaseDC(pDC);
	ResetScrollBars();

	return TRUE;
}

// jdh
void CfxGrid::AutoSizeColumns()
{
	for (int nCol = 0; nCol < GetColumnCount(); nCol++)
		AutoSizeColumn(nCol);
}

void CfxGrid::AutoSizeRows()
{
	for (int nRow = 0; nRow < GetRowCount(); nRow++)
		AutoSizeRow(nRow);
}
void CfxGrid::AutoSize()
{
	CDC* pDC = GetDC();
	if (!pDC) return;

	const int nNumColumns = GetColumnCount();
	const int nNumRows = GetRowCount();
    
	for (int nCol = 0; nCol < nNumColumns; nCol++)
		m_aryColWidth[nCol] = 0;
    
	for (int nRow = 0; nRow < nNumRows; nRow++)
		m_aryRowHeight[nRow] = 0;
    
	CSize size;
	for (int nCol = 0; nCol < nNumColumns; nCol++)
	{
		for (int nRow = 0; nRow < nNumRows; nRow++)
		{
			size = GetCellExtent(nRow, nCol, pDC);
			if (size.cx > (int) m_aryColWidth[nCol])  m_aryColWidth[nCol] = size.cx;
			if (size.cy > (int) m_aryRowHeight[nRow]) m_aryRowHeight[nRow] = size.cy;
		}
	}

	ReleaseDC(pDC);

	ResetScrollBars();
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////////////
// GridCtrl cell visibility tests and invalidation/redraw functions
void CfxGrid::EnsureVisible(int nRow, int nCol)
{
	const CRangeCell visibleCells = GetVisibleNonFixedCellRange();

	int right = nCol - visibleCells.GetMaxCol();
	int left  = visibleCells.GetMinCol() - nCol;
	int down  = nRow - visibleCells.GetMaxRow();
	int up    = visibleCells.GetMinRow() - nRow;

	m_noMessage = true;

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

	CRect cellRC, winRC;

	GetCellRect(nRow, nCol, cellRC);
	GetClientRect(winRC);

	if (cellRC.right > winRC.right)
		SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
	if (cellRC.bottom > winRC.bottom)
		SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);

	m_noMessage = false;
}

BOOL CfxGrid::IsCellAttribute(const CIdCell& cell, UINT attr) const
{
	CIdCell hidden = cell;
	if (attr == GVAT_HIDDEN && m_nFixedRows > 0)
		hidden.row = 0;

	return ((GetItemAttr(hidden.row, hidden.col) & attr) == attr);
}

BOOL CfxGrid::IsCellVisible(CIdCell cell) const
{  
	return IsCellVisible(cell.row, cell.col);
}

// jdh
BOOL CfxGrid::IsCellVisible(int nRow, int nCol) const
{  
	if (!IsWindow(m_hWnd))
		return FALSE;
	if (IsCellAttribute(CIdCell(nRow, nCol), GVAT_HIDDEN))
		return FALSE;

	int cx{}, cy{};

	CIdCell tlCell;
	if (nCol >= GetFixedColumnCount() || nRow >= GetFixedRowCount())
	{
		tlCell = GetTopleftNonFixedCell();
		if (nCol >= GetFixedColumnCount() && nCol < tlCell.col)
			return FALSE;
		if (nRow >= GetFixedRowCount() && nRow < tlCell.row)
			return FALSE;
	}

	CRect rect; GetClientRect(rect);
	if (nCol < GetFixedColumnCount())
	{
		cx = 0;
		for (int ii = 0; ii <= nCol; ii++) 
		{
			if (cx >= rect.right)
				return FALSE;
			cx += GetColumnWidth(ii);    
		}
	} 
	else 
	{
		cx = GetFixedColumnWidth();
		for (int ii = tlCell.col; ii <= nCol; ii++) 
		{
			if (cx >= rect.right)
				return FALSE;
			cx += GetColumnWidth(ii);    
		}
	}

	if (nRow < GetFixedRowCount())
	{
		cy = 0;
		for (int ii = 0; ii <= nRow; ii++) 
		{
			if (cy >= rect.bottom)
				return FALSE;
			cy += GetRowHeight(ii);    
		}
	} 
	else 
	{
		if (nRow < tlCell.row)
			return FALSE;
		cy = GetFixedRowHeight();
		for (int ii = tlCell.row; ii <= nRow; ii++) 
		{
			if (cy >= rect.bottom)
				return FALSE;
			cy += GetRowHeight(ii);    
		}
	}

	return TRUE;
}

BOOL CfxGrid::InvalidateCellRect(const CIdCell& cell)
{
	if (!::IsWindow(GetSafeHwnd()))
		return FALSE;

	if (!IsValid(cell) || !IsCellVisible(cell.row, cell.col))
		return FALSE;

	CRect rect;
	if (!GetCellRect(cell.row, cell.col, rect))
		return FALSE;

	rect.right++; rect.bottom++;
	InvalidateRect(rect, TRUE);

	return TRUE;
}

BOOL CfxGrid::InvalidateCellRect(const CRangeCell& range)
{
	ASSERT(IsValid(range));
	if (!::IsWindow(GetSafeHwnd()))
		return FALSE;

	const CRangeCell visibleCellRange = GetVisibleNonFixedCellRange().Intersect(range);

	CRect rect;
	if (!GetCellRangeRect(visibleCellRange, rect))
		return FALSE;

	rect.right++; rect.bottom++;
	InvalidateRect(rect, TRUE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CfxGrid Mouse stuff
BOOL CfxGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (m_NOscroll == GVSC_NONE)
		return CWnd::OnMouseWheel(nFlags, zDelta, pt);

	if (m_rowCountWheel == -1)
	{
		const int scrollPage = zDelta / 120;
		if (scrollPage > 0)
		{
			for (int ii = 0; ii < scrollPage; ii++)
				PostMessage(WM_VSCROLL, SB_PAGEUP, 0);
		}
		else
		{
			for (int ii = 0; ii > scrollPage; ii--)
				PostMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
		}
	}
	else
	{
		const int scrollPage = m_rowCountWheel * zDelta / 120;
		if (scrollPage > 0)
		{
			for (int ii = 0; ii < scrollPage; ii++)
				PostMessage(WM_VSCROLL, SB_LINEUP, 0);
		}
		else
		{
			for (int ii = 0; ii > scrollPage; ii--)
				PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
		}
	}
	
	Invalidate();
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CfxGrid::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rect; GetClientRect(rect);

	if (m_nRows <= 0 && m_nCols <= 0)
		return;
	if (m_mousemode != mouseDRAGGING && !rect.PtInRect(point))
		return;

	CDC* pDC{};
	CRect	oldRC, newRC, cellRC;
	CPoint	cellpt;
	CIdCell	current;

	if (!(nFlags & MK_LBUTTON) || (m_mousemode == mouseNOTHING && (nFlags & MK_LBUTTON)))
	{
		if (m_enableColResize && IsColResizeArea(point))
		{
			if (m_mousemode != mouseOVERCOLDIVIDE)
			{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
				m_mousemode = mouseOVERCOLDIVIDE;
			}
		}
		else if (m_enableRowResize && IsRowResizeArea(point))
		{
			if (m_mousemode != mouseOVERROWDIVIDE)
			{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
				m_mousemode = mouseOVERROWDIVIDE;
			}
		}
		else if (m_mousemode != mouseNOTHING)
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			m_mousemode = mouseNOTHING;
		}

		if (m_mousemode == mouseNOTHING && m_tooltip && m_enabletip)
		{
			m_tooltip.Hide();
			SetTimer(TM_TOOLTIP, 200, NULL);
			/*
			if (m_ptLast == point)
			{
				m_tooltip.Hide();
				return;
			}
			CRect textRC, cellRC;
			current = GetCellFromPoint(point);
			if (GetTextRect(current.row, current.col, textRC)
				&& GetCellRect(current.row, current.col, cellRC)
				&& IsCellAttribute(current, GVAT_TOOLTIP))
			{
				
				LPARAM lParam = GetItemParam(current.row, current.col);
				CString string; int icol;
				if (lParam != 0)
				{
					string = GetItemText(current.row, lParam);
					icol = lParam;
				}
				else
				{
					string = GetItemText(current.row, current.col);//lParam);
					icol = current.col;
				}
				//CString sparam; sparam.Format("%d", lParam);
				//AfxMessageBox(string);
				GVITEM gvitem;

				gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_IMAGE|GVMK_BKCOLOR|GVMK_FGCOLOR|GVMK_STATE|GVMK_ATTR|GVMK_PARAM|GVMK_MISC;
				gvitem.row  = current.row;
				gvitem.col  = icol;

				if (!GetItem(&gvitem)) return;
				//CString string2 = gvitem.text;
				DisplayAttribute(gvitem.attr, string);
				
				//string2.Format("%s[%d]",string2, current.row);
				if (!string.IsEmpty())
					m_tooltip.Show(textRC, string, 0, cellRC, GetItemFont(current.row, lParam));
			}
			*/
		}


// updateX_20061124
		if (m_mousemode == mouseNOTHING && m_enableMouseTrace)
		{
			current = GetCellFromPoint(point);
			if (!IsValid(current)) return;
			if (current != GetFocusCell())
			{
				ResetSelectedRange();
				m_idSelect = CIdCell(current.row, 0);
				SelectRows(current);
			}
		}

		m_ptLast = point;
		return;
	}

	if (!IsValid(m_idClick))
	{
		m_ptLast = point; return;
	}

	if (nFlags & MK_LBUTTON) 
	{
		switch (m_mousemode)
		{
		case mouseSELECTALL:        
			break;
		case mouseSELECTCOL:
		case mouseSELECTROW:    
		case mouseSELECTCELLS:    
			current = GetCellFromPoint(point);
			if (!IsValid(current)) return;
			if (current != GetFocusCell())
			{
				OnSelecting(current);
				if (m_focusLine && current.row >= m_nFixedRows && current.col >= m_nFixedCols)
					SetFocusCell(current);
			}
			break;

		case mouseMOVING:
			if (!(pDC = GetDC())) break;

			current = GetCellFromPoint(point);
			if (!IsValid(current)) return;
			if (current.col < m_nFixedCols || current.col < m_keepcolumn)
				return;

			GetCellRect(current.row, current.col, cellRC);

			cellpt.x = cellRC.left; cellpt.y = point.x;

			oldRC.SetRect(m_ptMove.x, rect.top, m_ptMove.x +2, rect.bottom);
			pDC->InvertRect(&oldRC);

			newRC.SetRect(cellpt.x, rect.top, cellpt.x +2, rect.bottom);
			pDC->InvertRect(&newRC);

			m_ptMove = cellpt;
			ReleaseDC(pDC);
			break;

		case mouseSIZINGCOL:
			if (!(pDC = GetDC())) break;

			oldRC.SetRect(m_ptLast.x, rect.top, m_ptLast.x + 2, rect.bottom);
			pDC->InvertRect(&oldRC);
			newRC.SetRect(point.x, rect.top, point.x + 2, rect.bottom);
			pDC->InvertRect(&newRC);

			m_ptLast = point;
			ReleaseDC(pDC);
			break;
		case mouseSIZINGROW:      
			if (!(pDC = GetDC())) break;

			oldRC.SetRect(rect.left, m_ptLast.y, rect.right, m_ptLast.y +2);
			pDC->InvertRect(&oldRC);
			newRC.SetRect(rect.left, point.y, rect.right, point.y +2);
			pDC->InvertRect(&newRC);
			m_ptLast = point;

			ReleaseDC(pDC);
			break;
		case mousePREPAREDRAG:
			OnBeginDrag(); 
			break;
		}    
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CfxGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_mousemode == mouseOVERCOLDIVIDE) 
	{
		CIdCell cell = GetCellFromPoint(point);
		ASSERT(IsValid(cell));

		CPoint start;
		if (!GetCellOrigin(0, cell.col, &start))
			return;

		if (point.x - start.x <= rszCAPTURE)     // Clicked right of border
			cell.col--;

		AutoSizeColumn(cell.col); Invalidate();
		MessageToParent(cell.row, cell.col, GVNM_CHGCOLSIZE);
	} 
	else if (m_mousemode == mouseOVERROWDIVIDE)
	{
		CIdCell cell = GetCellFromPoint(point);
		ASSERT(IsValid(cell));

		CPoint start;
		if (!GetCellOrigin(0, cell.col, &start))
			return;

		if (point.y - start.y <= rszCAPTURE)     // Clicked below border
			cell.row--;

		AutoSizeRow(cell.row); Invalidate();
		MessageToParent(cell.row, cell.col, GVNM_CHGROWSIZE);
	}
	else if (m_mousemode == mouseNOTHING)
	{
		const CIdCell cell = GetCellFromPoint(point);
		if (IsValid(cell))
		{
			if (IsCellAttribute(cell, GVAT_SORTVAL)
				|| IsCellAttribute(cell, GVAT_SORTABS) || IsCellAttribute(cell, GVAT_SORTSTR))
			{
				CWaitCursor waiter;
				if (cell.col == m_sortColumn)
				{
					m_ascending = !m_ascending;
				}
				else 
				{
					m_ascending = FALSE; m_sortColumn = cell.col;
				}

				SortTextItems(m_sortColumn, m_ascending);
				MessageToParent(m_sortColumn, 0, GVNM_ENDSORT);
				Invalidate();
			}
			
			MessageToParent(cell.row, cell.col, GVNM_DBLCLICKED);
		}
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CfxGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	const HWND hOldFocusWnd = ::GetFocus();

	m_ptClick = point;
	m_idClick = GetCellFromPoint(point, TRUE);

	if (!IsValid(m_idClick)) return;

	SetFocus();
	m_idSelect = (nFlags & MK_SHIFT) ? m_idCurrent : m_idClick;

	if (m_idClick == m_idCurrent)
	{
		if (IsCellAttribute(m_idClick, GVAT_EDIT))
			m_mousemode = mouseEDITCELL;
		else if (IsCellAttribute(m_idClick, GVAT_CHECK))
			m_mousemode = mouseCHECKCELL;
		else if (IsCellAttribute(m_idClick, GVAT_COMBO))
			m_mousemode = mouseCOMBOCELL;
		else if (IsCellAttribute(m_idClick, GVAT_DATE))
			m_mousemode = mouseDATECELL;
		else if (IsCellAttribute(m_idClick, GVAT_TIME))
			m_mousemode = mouseTIMECELL;

		switch (m_mousemode)
		{
		case mouseEDITCELL:
		case mouseCHECKCELL:
		case mouseCOMBOCELL:
		case mouseDATECELL:
		case mouseTIMECELL:
			MessageToParent(m_idClick.row, m_idClick.col, GVNM_LMOUSEDOWN);
			return;
		}
	}   
	else if (m_mousemode != mouseOVERCOLDIVIDE && m_mousemode != mouseOVERROWDIVIDE)        
	{
		SetFocusCell(-1,-1); 
		SetFocusCell(max(m_idClick.row, m_nFixedRows), max(m_idClick.col, m_nFixedCols));

		if (IsCellAttribute(m_idClick, GVAT_DRAGDROP)) // && hOldFocusWnd == GetSafeHwnd())
		{
			SelectRows(m_idClick);
			m_timerID = SetTimer(WM_LBUTTONDOWN, tmINTERVAL, 0);
		}
		else if (IsCellAttribute(m_idClick, GVAT_CHECK))
		{
			SelectRows(m_idClick);
			m_mousemode = mouseCHECKCELL;
		}
#ifdef	_winix
		else if (IsCellAttribute(m_idClick, GVAT_COMBO))
		{
			SelectRows(m_idClick);
			m_mousemode = mouseCOMBOCELL;
		}
		else if (IsCellAttribute(m_idClick, GVAT_DATE))
		{
			SelectRows(m_idClick);
			m_mousemode = mouseDATECELL;
		}
		else if (IsCellAttribute(m_idClick, GVAT_TIME))
		{
			SelectRows(m_idClick);
			m_mousemode = mouseTIMECELL;
		}
#endif
	}

	SetCapture();

	if (m_mousemode == mouseNOTHING)
	{
		if (m_enableColResize && IsColResizeArea(point))
		{
			if (m_mousemode != mouseOVERCOLDIVIDE)
			{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
				m_mousemode = mouseOVERCOLDIVIDE;
			}
		}
		else if (m_enableRowResize && IsRowResizeArea(point))
		{
			if (m_mousemode != mouseOVERROWDIVIDE)
			{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
				m_mousemode = mouseOVERROWDIVIDE;
			}
		}
		else if (m_idClick.row >= GetFixedRowCount() && GetCapture()->GetSafeHwnd() == GetSafeHwnd())
		{
			m_timerID = SetTimer(WM_LBUTTONDOWN, tmAUTOSCRL/10, 0);
			m_timerID = SetTimer(WM_LBUTTONDOWN, tmAUTOSCRL, 0);
		}
	}

	if (m_mousemode == mouseOVERCOLDIVIDE) // sizing column
	{
		m_mousemode = mouseSIZINGCOL;
		CPoint start;
		if (!GetCellOrigin(0, m_idClick.col, &start))
			return;

		CRect rect; GetClientRect(rect);
		const CRect invertedRC(point.x, rect.top, point.x + 2, rect.bottom);

		CDC* pDC = GetDC();
		if (pDC)
		{
			pDC->InvertRect(&invertedRC);
			ReleaseDC(pDC);
		}

		if (point.x - start.x <= rszCAPTURE)        // clicked right of border
			if (!GetCellOrigin(0, --m_idClick.col, &start))
				return;

		rect.left = start.x;
		ClientToScreen(rect);

		ClipCursor(rect);
	}
	else if (m_mousemode == mouseOVERROWDIVIDE) // sizing row
	{
		m_mousemode = mouseSIZINGROW;
		CPoint start;
		if (!GetCellOrigin(m_idClick.row, m_idClick.col, &start))
			return;

		CRect rect; GetClientRect(rect);
		const CRect invertedRC(rect.left, point.y, rect.right, point.y + 2);

		CDC* pDC = GetDC();
		if (pDC)
		{
			pDC->InvertRect(&invertedRC);
			ReleaseDC(pDC);
		}

		if (point.y - start.y <= rszCAPTURE)            // clicked below border
			if (!GetCellOrigin(--m_idClick.row, 0, &start))
				return;

		rect.top = start.y;
		ClientToScreen(rect);
		ClipCursor(rect);
	}
	else
	{    
		m_mapPreCell.RemoveAll();
		if (nFlags & MK_CONTROL)
		{
			for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != NULL; )
			{
				DWORD key; CIdCell cell;
				m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)cell);
				m_mapPreCell.SetAt(key, cell);
			}
		}

		if (m_idClick.row < GetFixedRowCount())
			OnFixedRowClick(m_idClick);
		else if (m_idClick.col < GetFixedColumnCount())
			OnFixedColumnClick(m_idClick);
		else if (m_mousemode == mouseNOTHING)
		{
			if (IsCellAttribute(m_idClick, GVAT_ONLYROW))
				m_mousemode = mouseSELECTROW;
			else
				m_mousemode = (m_dragdrop == GVDD_NONE) ? mouseSELECTCELLS : mouseSELECTROW;
			OnSelecting(m_idClick);
		}
	}

	MessageToParent(m_idClick.row, m_idClick.col, GVNM_LMOUSEDOWN);
	m_ptLast = point;
}

void CfxGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);

	ClipCursor(NULL);
	if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
	{
		ReleaseCapture();
		KillTimer(m_timerID);
		m_timerID = 0;
	}

	switch (m_mousemode)
	{
	case mouseEDITCELL:
		OnEditCell(m_idCurrent.row, m_idCurrent.col, VK_END);
		break;
	case mouseCHECKCELL:
		{
			LPARAM param = GetItemParam(m_idClick.row, m_idClick.col);
			param = param == 0 ? 1 : 0;
			SetItemParam(m_idClick.row, m_idClick.col, param);
			InvalidateCellRect(m_idClick);

			MessageToParent(m_idClick.row, m_idClick.col, GVNM_LMOUSEUP);
		}
		break;
	case mouseCOMBOCELL:
		OnComboCell(m_idClick.row, m_idClick.col);
		break;
	case mouseDATECELL:
		OnDateCell(m_idClick.row, m_idClick.col);
		break;
	case mouseTIMECELL:
		OnTimeCell(m_idClick.row, m_idClick.col);
		break;
	case mousePREPAREDRAG:
		ResetSelectedRange();
		break;
	case mouseMOVING:
		{
			CRect	rect; GetClientRect(rect);
			const CRect	invertedRC(m_ptMove.x, rect.top, m_ptMove.x +2, rect.bottom);

			CDC* pDC = GetDC();
			if (pDC)
			{
				pDC->InvertRect(&invertedRC);
				ReleaseDC(pDC);
			}

			if (m_ptClick != point)
			{
				const CIdCell click = GetCellFromPoint(m_ptClick);
				const CIdCell move  = GetCellFromPoint(point);
				if (move.col >= m_nFixedCols && move.col >= m_keepcolumn)
				{
					MoveColAction(click.col, move.col);
					MessageToParent(move.col, click.col, GVNM_COLUMNMOVE);
				}
			}
			ResetScrollBars(); Invalidate();
		}
		break;
	case mouseSIZINGCOL:
		{
			CRect rect; GetClientRect(rect);
			const CRect invertedRC(m_ptLast.x, rect.top, m_ptLast.x + 2, rect.bottom);

			CDC* pDC = GetDC();
			if (pDC)
			{
				pDC->InvertRect(&invertedRC);
				ReleaseDC(pDC);
			}

			if (m_ptClick != point) 
			{   
				CPoint start;
				if (!GetCellOrigin(m_idClick.row, m_idClick.col, &start))
					return;
				SetColumnWidth(m_idClick.col, point.x - start.x);
				ResetScrollBars(); Invalidate();

				MessageToParent(m_idClick.row, m_idClick.col, GVNM_CHGCOLSIZE);
			}
		}
		break;
	case mouseSIZINGROW:
		{
			CRect rect; GetClientRect(rect);
			const CRect invertedRC(rect.left, m_ptLast.y, rect.right, m_ptLast.y + 2);

			CDC* pDC = GetDC();
			if (pDC)
			{
				pDC->InvertRect(&invertedRC);
				ReleaseDC(pDC);
			}

			if (m_ptClick != point) 
			{
				CPoint start;
				if (!GetCellOrigin(m_idClick.row, m_idClick.col, &start))
					return;

				SetRowHeight(m_idClick.row, point.y - start.y);
				ResetScrollBars(); Invalidate();

				MessageToParent(m_idClick.row, m_idClick.col, GVNM_CHGROWSIZE);
			}
		}
		break;
	default:
		{
		const CIdCell upCell  = GetCellFromPoint(point);
			MessageToParent(upCell.row, upCell.col, GVNM_LMOUSEUP);
		}
		break;
	}

	m_mousemode = mouseNOTHING;
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	if (!IsValid(m_idClick))
		return;

	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->PostMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED), (LPARAM) GetSafeHwnd());
}

void CfxGrid::OnRButtonDown(UINT nFlags, CPoint point) 
{
	const CIdCell Rclick = GetCellFromPoint(point, TRUE);

	if (!IsValid(Rclick)) return;
	MessageToParent(Rclick.row, Rclick.col, GVNM_RMOUSEDOWN);
	CWnd::OnRButtonDown(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// CfxGrid printing 
void CfxGrid::Print() 
{
	CDC dc;
	CPrintDialog printDlg(FALSE);

	if (printDlg.DoModal() != IDOK)
		return;

	dc.Attach(printDlg.GetPrinterDC());
	dc.m_bPrinting = TRUE;

	CString strTitle;
	strTitle.LoadString(AFX_IDS_APP_TITLE);

	DOCINFO di;
	memset(&di, 0, sizeof (DOCINFO));
	di.cbSize = sizeof (DOCINFO);
	di.lpszDocName = strTitle;

	BOOL bPrintingOK = dc.StartDoc(&di);

	CPrintInfo Info;
	Info.m_rectDraw.SetRect(0,0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));

	OnBeginPrinting(&dc, &Info);
	for (UINT page = Info.GetMinPage(); page <= Info.GetMaxPage() && bPrintingOK; page++)
	{
		dc.StartPage();
		Info.m_nCurPage = page;
		OnPrint(&dc, &Info);
		bPrintingOK = (dc.EndPage() > 0);
	}
	OnEndPrinting(&dc, &Info);

	if (bPrintingOK)
		dc.EndDoc();
	else
		dc.AbortDoc();

	dc.Detach();
}

/////////////////////////////////////////////////////////////////////////////
// CfxGrid printing overridables - for Doc/View print/print preview framework

void CfxGrid::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
{
	ASSERT(pDC && pInfo);
	if (!pDC || !pInfo) return;

	const int nMaxRowCount = GetRowCount() - GetFixedRowCount();
	if (!nMaxRowCount)
		return;

	CDC *pCurrentDC = GetDC();
	if (!pCurrentDC)
		return;

	const CSize PaperPixelsPerInch(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
	const CSize ScreenPixelsPerInch(pCurrentDC->GetDeviceCaps(LOGPIXELSX), pCurrentDC->GetDeviceCaps(LOGPIXELSY));

	// Create the printer font
	const int nFontSize = -9;
	CString strFontName = "Times New Roman";
	m_prnfont.CreateFont(nFontSize, 0,0,0, FW_NORMAL, 0,0,0, DEFAULT_CHARSET,
				OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE, strFontName);
            
	CFont *pOldFont = pDC->SelectObject(&m_prnfont);

	// Get the average character width (in GridCtrl units) and hence the margins
	m_szChar = pDC->GetOutputTextExtent(_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSATUVWXYZ"),52);
	m_szChar.cx /= 52;
	const int nMargins = (leftMARGIN+rightMARGIN)*m_szChar.cx;

	// Get the page sizes (physical and logical)
	m_szPaper = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	m_szLogical.cx = GetVirtualWidth()+nMargins;

	m_szLogical.cy = MulDiv(m_szLogical.cx, m_szPaper.cy, m_szPaper.cx);

	m_pgHeight = m_szLogical.cy - GetFixedRowHeight()
					- (hdrHEIGHT+footerHEIGHT + 2 * prnGAP) * m_szChar.cy;

	// Get the number of pages. Assumes no row is bigger than the page size.
	int nTotalRowHeight = 0;
	int nNumPages = 1;
	for (int row = GetFixedRowCount(); row < GetRowCount(); row++)
	{
		nTotalRowHeight += GetRowHeight(row);
		if (nTotalRowHeight > m_pgHeight)
		{
			nNumPages++;
			nTotalRowHeight = GetRowHeight(row);
		}
	}

	// Set up the print info
	pInfo->SetMaxPage(nNumPages);
	pInfo->m_nCurPage = 1;                        // start printing at page# 1

	ReleaseDC(pCurrentDC);
	pDC->SelectObject(pOldFont);
}

void CfxGrid::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{
	if (!pDC || !pInfo) return;

	CFont *pOldFont = pDC->SelectObject(&m_prnfont);

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(m_szLogical);
	pDC->SetViewportExt(m_szPaper);
	pDC->SetWindowOrg(-leftMARGIN*m_szChar.cx, 0);

	pInfo->m_rectDraw.top    = 0;
	pInfo->m_rectDraw.left   = 0;
	pInfo->m_rectDraw.right  = m_szLogical.cx - (leftMARGIN+rightMARGIN)*m_szChar.cx;
	pInfo->m_rectDraw.bottom = hdrHEIGHT*m_szChar.cy;
	PrintHeader(pDC, pInfo);
	pDC->OffsetWindowOrg(0, -hdrHEIGHT*m_szChar.cy);

	pDC->OffsetWindowOrg(0, -prnGAP*m_szChar.cy);

	pInfo->m_rectDraw.bottom = GetFixedRowHeight(); 
	PrintColumnHeadings(pDC, pInfo);
	pDC->OffsetWindowOrg(0, -GetFixedRowHeight()); 

	int nTotalRowHeight = 0;
	UINT nNumPages = 1;
	int nCurrPrintRow = GetFixedRowCount();

	while (nCurrPrintRow < GetRowCount() && nNumPages < pInfo->m_nCurPage)
	{
		nTotalRowHeight += GetRowHeight(nCurrPrintRow);
		if (nTotalRowHeight > m_pgHeight)
		{
			nNumPages++;
			if (nNumPages == pInfo->m_nCurPage) break;
			nTotalRowHeight = GetRowHeight(nCurrPrintRow);
		}
		nCurrPrintRow++;
	}
	if (nCurrPrintRow >= GetRowCount()) return;

	BOOL bFirstPrintedRow = TRUE;
	CRect rect;
	rect.bottom = -1;
	while (nCurrPrintRow < GetRowCount())
	{
		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(nCurrPrintRow) - 1;

		if (rect.bottom > m_pgHeight) break;            // Gone past end of page

		rect.right = -1;
		for (int col = 0; col < GetColumnCount(); col++)
		{
			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col) - 1;

			DrawCell(pDC, nCurrPrintRow, col, rect);

			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_HORZ) 
			{
				const int Overlap = (col == 0)? 0:1;
				pDC->MoveTo(rect.left-Overlap, rect.bottom);
				pDC->LineTo(rect.right, rect.bottom);
				if (nCurrPrintRow == 0)
				{
					pDC->MoveTo(rect.left-Overlap, rect.top);
					pDC->LineTo(rect.right, rect.top);
				}
			}
			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_VERT) 
			{
				const int Overlap = (bFirstPrintedRow)? 0:1;
				pDC->MoveTo(rect.right, rect.top-Overlap);
				pDC->LineTo(rect.right, rect.bottom);    
				if (col == 0)
				{
					pDC->MoveTo(rect.left, rect.top-Overlap);
					pDC->LineTo(rect.left, rect.bottom);    
				}
			}
		}

		nCurrPrintRow++;
		bFirstPrintedRow = FALSE;
	}

	// Footer
	pInfo->m_rectDraw.bottom = footerHEIGHT*m_szChar.cy;
	pDC->SetWindowOrg(-leftMARGIN*m_szChar.cx, -m_szLogical.cy + footerHEIGHT*m_szChar.cy);
	PrintFooter(pDC, pInfo);

	// SetWindowOrg back for next page
	pDC->SetWindowOrg(0,0);

	pDC->SelectObject(pOldFont);
}

void CfxGrid::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	m_prnfont.DeleteObject();
}

void CfxGrid::PrintColumnHeadings(CDC *pDC, CPrintInfo* /*pInfo*/)
{
	CFont *pOldFont = pDC->SelectObject(&m_prnfont);

	CRect rect; rect.bottom = -1;
	for (int row = 0; row < GetFixedRowCount(); row++)
	{
		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(row) - 1;

		rect.right = -1;
		for (int col = 0; col < GetColumnCount(); col++)
		{
			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col) - 1;

			DrawFixedCell(pDC, row, col, rect);

			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_HORZ) 
			{
				const int Overlap = (col == 0)? 0:1;
				pDC->MoveTo(rect.left-Overlap, rect.bottom);
				pDC->LineTo(rect.right, rect.bottom);
				if (row == 0)
				{
					pDC->MoveTo(rect.left-Overlap, rect.top);
					pDC->LineTo(rect.right, rect.top);
				}
			}
			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_VERT) 
			{
				const int Overlap = (row == 0)? 0:1;
				pDC->MoveTo(rect.right, rect.top-Overlap);
				pDC->LineTo(rect.right, rect.bottom);    
				if (col == 0)
				{
					pDC->MoveTo(rect.left, rect.top-Overlap);
					pDC->LineTo(rect.left, rect.bottom);    
				}
			}
		}
	}

	pDC->SelectObject(pOldFont);
}

void CfxGrid::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
	CRect   rc(pInfo->m_rectDraw);
	CString strHeaderString;
	CFont   BoldFont;
	LOGFONT lf;

	//create bold font for header and footer
	VERIFY(m_prnfont.GetLogFont(&lf));
	lf.lfWeight = FW_BOLD; lf.lfHeight = 100;
	VERIFY(BoldFont.CreatePointFontIndirect(&lf));

	CFont *pNormalFont = pDC->SelectObject(&BoldFont);
	const int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);

	// print App title on top right margin
	strHeaderString.LoadString(AFX_IDS_APP_TITLE);
	pDC->DrawText(strHeaderString, &rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);

	// print parent window title in the centre (Gert Rijs)
	CWnd *pParentWnd = GetParent();
	while (pParentWnd)
	{
		pParentWnd->GetWindowText(strHeaderString);
		if (strHeaderString.GetLength())  // can happen if it is a CView, CChildFrm has the title
			break;
		pParentWnd = pParentWnd->GetParent();
	}
	pDC->DrawText(strHeaderString, &rc, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);


	pDC->SetBkMode(nPrevBkMode);
	pDC->SelectObject(pNormalFont);
	BoldFont.DeleteObject();

	pDC->SelectStockObject(BLACK_PEN);
	pDC->MoveTo(rc.left, rc.bottom);
	pDC->LineTo(rc.right, rc.bottom);
}

void CfxGrid::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
{
	CRect rc(pInfo->m_rectDraw);
	CFont BoldFont;
	LOGFONT lf;

	//draw line
	pDC->MoveTo(rc.left, rc.top);
	pDC->LineTo(rc.right, rc.top);

	//create bold font for header and footer
	m_prnfont.GetLogFont(&lf);
	lf.lfWeight = FW_BOLD; lf.lfHeight = 100;
	BoldFont.CreatePointFontIndirect(&lf);

	CFont *pNormalFont = pDC->SelectObject(&BoldFont);
	const int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);

	// draw page number
	CString   sTemp ;
	rc.OffsetRect(0, m_szChar.cy/2);
	sTemp.Format(_T("Page %d of %d"), pInfo->m_nCurPage, pInfo->GetMaxPage());
	pDC->DrawText(sTemp,-1,rc, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

	const CTime t = CTime::GetCurrentTime();
	sTemp = t.Format(_T("%c"));
	pDC->DrawText(sTemp,-1,rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

	pDC->SetBkMode(nPrevBkMode);
	pDC->SelectObject(pNormalFont);
	BoldFont.DeleteObject();
}

CImageList* CfxGrid::CreateDragImage(CPoint *pHotSpot)
{
	CDC* pDC = GetDC();
	if (!pDC) return NULL;

	CRect rect;
	const CIdCell	cell = GetFocusCell();
	if (!GetCellRect(cell.row, cell.col, rect))
		return NULL;

	// Translate coordinate system
	rect.BottomRight() = CPoint(rect.Width(), rect.Height());
	rect.TopLeft()     = CPoint(0,0);
	*pHotSpot = rect.BottomRight(); 

	// Create a new imagelist (the caller of this function has responsibility
	// for deleting this list)
	CImageList* pList = new CImageList;
	if (!pList || !pList->Create(rect.Width(), rect.Height(), ILC_MASK, 1,1))
	{    
		if (pList) delete pList;
		return NULL;
	}

	// Create mem DC and bitmap
	CDC MemDC;
	CBitmap bm;
	MemDC.CreateCompatibleDC(pDC);
	bm.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBitmap = MemDC.SelectObject(&bm);
	MemDC.SetWindowOrg(0,0);

	// Draw cell onto bitmap in memDC
	DrawCell(&MemDC, cell.row, cell.col, rect, TRUE);

	// Clean up
	MemDC.SelectObject(pOldBitmap);
	ReleaseDC(pDC);

	// Add the bitmap we just drew to the image list.
	pList->Add(&bm, m_bkCOL);
	bm.DeleteObject();

	return pList;
}

/////////////////////////////////////////////////////////////////////////////
// CfxGrid overrideables
void CfxGrid::OnFixedRowClick(CIdCell& cell)
{
	if (!IsValid(cell))
		return;

	if (cell.col < GetFixedColumnCount())
	{
		m_mousemode = mouseSELECTALL;
		OnSelecting(cell);
	} 
	else
	{
		if (IsCellAttribute(m_idClick, GVAT_ONLYROW))
		{
			m_mousemode = mouseSELECTROW;
			OnSelecting(cell);
		}
		else
		{ 
			m_mousemode = mouseSELECTCOL;
			OnSelecting(cell);
		}
	}

	if (cell.col >= GetFixedColumnCount() && IsCellAttribute(cell, GVAT_DRAGMOVE))
	{
		m_mousemode = mousePREPAREMOVE;
		m_timerID = SetTimer(WM_LBUTTONDOWN, tmINTERVALx, 0);

		CRect	rect; GetCellRect(cell.row, cell.col, rect);
		CRect	clientRC; GetClientRect(&clientRC);

		m_ptMove.x = rect.left; m_ptMove.y = rect.top;
	}

	if (cell.col >= GetFixedColumnCount() && IsCellAttribute(cell, GVAT_CHECK))
		m_mousemode = mouseCHECKCELL;
}

void CfxGrid::OnFixedColumnClick(CIdCell& cell)
{
	if (!IsValid(cell))
		return;

	if (cell.row < GetFixedRowCount()) 
	{
		m_mousemode = mouseSELECTALL;
		OnSelecting(cell);
	}
	else 
	{
		m_mousemode = mouseSELECTROW;
		OnSelecting(cell);
	}
}

CSize CfxGrid::GetTextExtent(LPCTSTR str, BOOL bUseSelectedFont)
{
	CDC* pDC = GetDC();
	if (!pDC) return CSize(0,0);

	CFont* pOldFont{}, font;

	if (bUseSelectedFont)
	{
		LOGFONT logfont;
		memcpy(&logfont, &m_logfont, sizeof(LOGFONT));
		logfont.lfWeight = selcWEIGHT;

		font.CreatePointFontIndirect(&logfont);

		pOldFont = pDC->SelectObject(&font);
	}
	else
	{
		pOldFont = pDC->SelectObject(&m_font);
	}

	const CSize size = pDC->GetOutputTextExtent(str);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	return size + CSize(2 * m_margin, 2 * m_margin);
}

CSize CfxGrid::GetCellExtent(int nRow, int nCol, CDC* pDC)
{
	if (IsCellAttribute(CIdCell(nRow, nCol), GVAT_HIDDEN))
		return CSize(0, 0);

	const LOGFONT *plogfont = GetItemFont(nRow, nCol);

	// use selected font since it's thicker   
	LOGFONT logfont;
	memcpy(&logfont, plogfont, sizeof(LOGFONT));

	if (nRow < m_nFixedRows || nCol < m_nFixedCols)
		logfont.lfWeight = selcWEIGHT;
            
	CFont font;
	font.CreatePointFontIndirect(&logfont);

	CFont* pOldFont = pDC->SelectObject(&font);
	CString string = GetItemText(nRow, nCol);
	if (IsCellAttribute(CIdCell(nRow, nCol), GVAT_TICKTREND))
		string = string.Mid(string.GetLength() / 2);
	if (string.IsEmpty()) string = _T("9");
	CSize size = pDC->GetOutputTextExtent(string);
	pDC->SelectObject(pOldFont);

//	size += CSize(4*m_margin, 2*m_margin);
	size += CSize(3*m_margin, 2*m_margin);

	CSize ImageSize(0,0);
	if (m_imageList)
	{
		const int nImage = GetItemImage(nRow, nCol);
		if (nImage >= 0)
		{
			IMAGEINFO Info;
			if (m_imageList->GetImageInfo(nImage, &Info))
				ImageSize = CSize(Info.rcImage.right-Info.rcImage.left+1, 
					Info.rcImage.bottom-Info.rcImage.top+1);
		}
	}
    
	return CSize(size.cx + ImageSize.cx, max(size.cy, ImageSize.cy));
}


bool CfxGrid::OnEditCell(int nRow, int nCol, UINT nChar)
{
	const CIdCell cell(nRow, nCol);
	if (!IsValid(cell) || !IsCellAttribute(cell, GVAT_EDIT) || !IsCellVisible(nRow, nCol)) 
	{
		MessageToParent(nChar, nCol, GVNM_ONCHAR);
		return false;
	}

	EnsureVisible(nRow, nCol);

	CRect rect;
	if (!GetCellRect(cell.row, cell.col, rect))
		return false;

	MessageToParent(nRow, nCol, GVNM_BEGINEDIT);

	GVITEM gvitem;

	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_PARAM;
	gvitem.row  = nRow;
	gvitem.col  = nCol;

	if (!GetItem(&gvitem)) return false;

	DWORD dwStyle = ES_LEFT;
	if (gvitem.format & DT_RIGHT)		dwStyle = ES_RIGHT;
	else if (gvitem.format & DT_CENTER)	dwStyle = ES_CENTER;

	dwStyle |= ES_UPPERCASE;
	if (nCol < m_nFixedCols) rect.DeflateRect(2, 1, 1, 1);

	CreateInPlaceEdit(rect, dwStyle, IDC_INPLACE_EDIT, nRow, nCol, gvitem.text, (int)gvitem.param, nChar);
	return true;
}

void CfxGrid::CreateInPlaceEdit(CRect& rect, DWORD dwStyle, UINT nID,
					int nRow, int nCol, LPCTSTR szText, int limit, int nChar)
{
	LOGFONT* logfont = GetItemFont(nRow, nCol);
	new CInPlaceEdit(this, rect, dwStyle, nID, nRow, nCol, szText, limit, nChar, logfont);
}

void CfxGrid::OnEndEditCell(int nRow, int nCol, CString str)
{
	CString strCurrent = GetItemText(nRow,nCol);
	if (strCurrent != str)
		SetItemText(nRow, nCol, str);
}

bool CfxGrid::OnComboCell(int nRow, int nCol)
{
	EnsureVisible(nRow, nCol);

	const CIdCell cell(nRow, nCol);
	if (!IsValid(cell) || !IsCellAttribute(cell, GVAT_COMBO) || !IsCellVisible(nRow, nCol)) 
		return false;

	CRect rect;
	if (!GetCellRect(cell.row, cell.col, rect))
		return false;

	MessageToParent(nRow, nCol, GVNM_BEGINCOMBO);

	GVITEM gvitem;

	gvitem.mask = GVMK_TEXT|GVMK_MISC|GVMK_PARAM;
	gvitem.row  = nRow;
	gvitem.col  = nCol;

	if (!GetItem(&gvitem)) return false;

	const DWORD dwStyle = CBS_AUTOHSCROLL|CBS_DROPDOWNLIST;
	CreateInPlaceCombo(rect, dwStyle, IDC_INPLACE_COMBO, nRow, nCol, gvitem.misc, gvitem.param);
	return true;
}

void CfxGrid::CreateInPlaceCombo(CRect& rect, DWORD dwStyle, UINT nID,
						int nRow, int nCol, LPCTSTR szText, int active)
{
	LOGFONT* logfont = GetItemFont(nRow, nCol);
	new CInPlaceCombo(this, rect, dwStyle, nID, nRow, nCol, szText, logfont, active);
}

void CfxGrid::OnEndComboCell(int nRow, int nCol, CString str, LPARAM lparam)
{
	CString	curstr = GetItemText(nRow, nCol);
	if (curstr != str)
	{
		SetItemText(nRow, nCol, str);
		SetItemParam(nRow, nCol, lparam);
	}
}

bool CfxGrid::OnDateCell(int nRow, int nCol)
{
	EnsureVisible(nRow, nCol);

	const CIdCell cell(nRow, nCol);
	if (!IsValid(cell) || !IsCellAttribute(cell, GVAT_DATE) || !IsCellVisible(nRow, nCol)) 
		return false;

	CRect rect;
	if (!GetCellRect(cell.row, cell.col, rect))
		return false;

	MessageToParent(nRow, nCol, GVNM_BEGINDATE);

	GVITEM gvitem;

	gvitem.mask = GVMK_TEXT|GVMK_MISC;
	gvitem.row  = nRow;
	gvitem.col  = nCol;

	if (!GetItem(&gvitem)) return false;

	CString	init = CString(gvitem.text, gvitem.text.GetLength()); init.Remove('/');
	CreateInPlaceDate(rect, IDC_INPLACE_DATE, init, nRow, nCol, gvitem.misc);
	return true;
}

void CfxGrid::CreateInPlaceDate(CRect& rect, UINT nID, LPCTSTR init, int nRow, int nCol, LPCTSTR szText)
{
	new CInPlaceDate(this, rect, nID, init, nRow, nCol, szText);
}

void CfxGrid::OnEndDateCell(int nRow, int nCol, CString str)
{
	CString	curstr = GetItemText(nRow, nCol);
	if (curstr != str) SetItemText(nRow, nCol, str);
}

bool CfxGrid::OnTimeCell(int nRow, int nCol)
{
	EnsureVisible(nRow, nCol);

	const CIdCell cell(nRow, nCol);
	if (!IsValid(cell) || !IsCellAttribute(cell, GVAT_TIME) || !IsCellVisible(nRow, nCol)) 
		return false;

	CRect rect;
	if (!GetCellRect(cell.row, cell.col, rect))
		return false;

	MessageToParent(nRow, nCol, GVNM_BEGINTIME);

	GVITEM gvitem;

	gvitem.mask = GVMK_TEXT|GVMK_MISC;
	gvitem.row  = nRow;
	gvitem.col  = nCol;

	if (!GetItem(&gvitem)) return false;

	CString init = CString(gvitem.text, gvitem.text.GetLength()); init.Remove('/');
	CreateInPlaceTime(rect, IDC_INPLACE_TIME, init, nRow, nCol, gvitem.misc);
	return true;
}

void CfxGrid::CreateInPlaceTime(CRect& rect, UINT nID, LPCTSTR init, int nRow, int nCol, LPCTSTR szText)
{
	new CInPlaceTime(this, rect, nID, init, nRow, nCol, szText);
}

void CfxGrid::OnEndTimeCell(int nRow, int nCol, CString str)
{
	CString	curstr = GetItemText(nRow, nCol);
	if (curstr != str) SetItemText(nRow, nCol, str);
}

/////////////////////////////////////////////////////////////////////////////
// drag & drop
void CfxGrid::OnBeginDrag()
{        
	if (m_dragdrop == GVDD_NONE) return;

	COleDataSource* pSource = DragRowAction();
	if (pSource)
	{    
		m_idDrag.row = m_idClick.row;
		m_idDrag.col = m_idClick.col;

		MessageToParent(m_idDrag.row, m_idDrag.col, GVNM_BEGINDRAG);

		m_dragWnd = this;
		m_mousemode = mouseDRAGGING;
		const DROPEFFECT dropEffect = pSource->DoDragDrop(DROPEFFECT_COPY);

		if (pSource) delete pSource;
	}
}

DROPEFFECT CfxGrid::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if (m_dragdrop == GVDD_NONE || !pDataObject->IsDataAvailable(CF_TEXT))
		return DROPEFFECT_NONE;

	CIdCell cell = GetCellFromPoint(point, TRUE);

	if (!IsValid(cell))
	{
		OnDragLeave();
		m_idDrop = CIdCell(-1, -1);
		return DROPEFFECT_NONE;
	}

	if (!IsCellAttribute(cell, GVAT_EDIT))
		return DROPEFFECT_NONE;

	if (m_dragdrop == GVDD_HALF && !IsValidDrop(pDataObject))
		return DROPEFFECT_NONE;

	if (cell != m_idDrop) 
	{
		// Set the previously drop-highlighted cell as no longer drop-highlighted
		if (IsValid(m_idDrop))
		{
			const UINT nState = GetItemState(m_idDrop.row, m_idDrop.col);
			SetItemState(m_idDrop.row, m_idDrop.col, nState & ~GVST_DROPHILITED);
			RedrawCell(m_idDrop);
		}

		m_idDrop = cell;

		// Set the new cell as drop-highlighted
		if (IsValid(m_idDrop))
		{
			const UINT nState = GetItemState(m_idDrop.row, m_idDrop.col);
			SetItemState(m_idDrop.row, m_idDrop.col, nState | GVST_DROPHILITED);
			RedrawCell(m_idDrop);
		}
	}

	return DROPEFFECT_COPY;
}

DROPEFFECT CfxGrid::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if (m_dragdrop == GVDD_NONE || !pDataObject->IsDataAvailable(CF_TEXT))
		return DROPEFFECT_NONE;

//	m_idDrag = GetCellFromPoint(point, TRUE);
	if (!IsValid(m_idDrag))
		return DROPEFFECT_NONE;

	if (!IsCellAttribute(m_idDrag, GVAT_EDIT))
		return DROPEFFECT_NONE;

	if (m_dragdrop == GVDD_HALF && !IsValidDrop(pDataObject))
		return DROPEFFECT_NONE;

	if (IsValid(m_idDrag))
	{
		const UINT nState = GetItemState(m_idDrag.row, m_idDrag.col);
		SetItemState(m_idDrag.row, m_idDrag.col, nState|GVST_DROPHILITED);
		RedrawCell(m_idDrag);
	}

	m_idDrop = m_idDrag;
	return DROPEFFECT_COPY;
}

void CfxGrid::OnDragLeave()
{
	if (IsValid(m_idDrop))
	{
		const UINT nState = GetItemState(m_idDrop.row, m_idDrop.col);
		SetItemState(m_idDrop.row, m_idDrop.col, nState & ~GVST_DROPHILITED);
		RedrawCell(m_idDrop);
	}
}

BOOL CfxGrid::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	const BOOL bResult = FALSE;

	if (m_dragdrop == GVDD_NONE || !IsCellAttribute(m_idDrop, GVAT_EDIT))
		return bResult;

	m_mousemode = mouseNOTHING;
	OnDragLeave();

	const BOOL response = DropRowAction(pDataObject);
	MessageToParent(m_idDrop.row, m_idDrop.col, GVNM_ENDDRAG);

	m_dragWnd = (CWnd *) 0;
	m_idDrag = CIdCell(-1, -1); m_idDrop = CIdCell(-1, -1);
	return response;
}

void CfxGrid::OnEditCut()
{
	m_bCopy = false;
	COleDataSource* pSource = CopyTextFromGrid();
	if (pSource)
	{
		pSource->SetClipboard();
		CutSelectedText();
	}
}

void CfxGrid::OnEditCopy()
{
	m_bCopy = true;
	COleDataSource* pSource = CopyTextFromGrid();
	if (pSource) pSource->SetClipboard();

}

void CfxGrid::OnEditPaste()
{
	CIdCell	idcell = GetFocusCell();

	if (!IsValid(idcell)) idcell = GetTopleftNonFixedCell();  
	if (!IsValid(idcell)) return;

	// Attach a COleDataObject to the clipboard and paste the data to the grid
	COleDataObject obj;
	if (obj.AttachClipboard())
		PasteTextToGrid(idcell, &obj);
	
}

void CfxGrid::OnEditSelectAll() 
{
	SelectAllCells();
}

////////////////////////////////////////////////////////////////////////////////////////
// Clipboard functions
void CfxGrid::CutSelectedText()
{
	// Clear contents of selected cells.
	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != NULL; )
	{
		DWORD key; CIdCell cell;
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)cell);

//		if (!IsCellAttribute(cell, GVAT_EDIT))
//			continue;

		CCell* pCell = GetCell(cell.row, cell.col);
		if (pCell) EmptyCell(pCell, cell.row, cell.col);
	}
	Invalidate();
}

COleDataSource* CfxGrid::CopyTextFromGrid()
{
	USES_CONVERSION;

	const CRangeCell selection = GetSelectedCellRange();
	if (!IsValid(selection)) return NULL;

	// Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE, so we need to convert)
	CSharedFile sharedFile(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if (m_bCopy)
		MessageToParent(selection.GetMinRow(), selection.GetMinCol(), GVNM_BEGINCOPY);
	else
		MessageToParent(selection.GetMinRow(), selection.GetMinCol(), GVNM_BEGINCUT);

	// Get a tab delimited string to copy to cache
	CString str; CCell* pCell{};
	for (int row = selection.GetMinRow(); row <= selection.GetMaxRow(); row++) 
	{
		str.Empty();
		for (int col = selection.GetMinCol(); col <= selection.GetMaxCol(); col++)
		{
			pCell = GetCell(row,col);
			if (pCell && (pCell->state  & GVST_SELECTED))
			{
				if (pCell->attr & GVAT_NOTOLECOLUMN)
					continue;

				if (pCell->text.IsEmpty())
					str += _T(" ");
				else 
					str += pCell->text;

				if (pCell->attr & GVAT_COMBO)
				{
					str += "\r"; str += pCell->misc;
				}
			}
			if (col != selection.GetMaxCol()) 
				str += _T("\t");
		}
		if (row != selection.GetMaxRow()) 
			str += _T("\n");

		sharedFile.Write(T2A(str.GetBuffer(1)), str.GetLength());
		str.ReleaseBuffer();
		if (m_bCopy)
			MessageToParent(row, selection.GetMaxRow(), GVNM_COPYROW);
		else
			MessageToParent(row, selection.GetMaxRow(), GVNM_CUTROW);

	}

	if (m_bCopy)
		MessageToParent(selection.GetMaxRow(), selection.GetMaxRow(), GVNM_ENDCOPY);
	else
		MessageToParent(selection.GetMaxRow(), selection.GetMaxRow(), GVNM_ENDCUT);

	const char c = '\0'; sharedFile.Write(&c, 1);

	const DWORD dwLen = (DWORD)sharedFile.GetLength();
	HGLOBAL hMem = sharedFile.Detach();
#if _MFC_VER	<= 0x0600
	::GlobalUnlock(hMem);
#endif
	if (!hMem) return NULL;

	hMem = ::GlobalReAlloc(hMem, dwLen, GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
	if (!hMem) return NULL;

	// Cache data
	COleDataSource* pSource = new COleDataSource();
	pSource->CacheGlobalData(CF_TEXT, hMem);

	return pSource;
}

BOOL CfxGrid::PasteTextToGrid(CIdCell cell, COleDataObject* pDataObject)
{
	if (!IsValid(cell) || !pDataObject->IsDataAvailable(CF_TEXT))
		return FALSE;

	// Get the text from the COleDataObject
	HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
	CMemFile memFile((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

	// CF_TEXT is ANSI text, so we need to allocate a char* buffer to hold this.
	LPSTR szBuffer = new char[::GlobalSize(hmem)];
	if (!szBuffer) return FALSE;

	memFile.Read(szBuffer, ::GlobalSize(hmem));
	::GlobalUnlock(hmem); ::GlobalFree(hmem);

	CString strText = szBuffer;
	delete szBuffer;

	// Parse text data and set in cells...
	strText.LockBuffer();

	CString strLine = strText;

	int nLine = 0;
	int nIndex{};
	CString string, text;

	const int cnt = 0;
	while (!strLine.IsEmpty())
	{
		nIndex = strLine.Find('\n');
		if (nIndex != -1)
		{
			string = strLine.Left(nIndex);
			strLine = strLine.Mid(nIndex+1);
		}
		else
		{
			string = strLine;
			strLine = _T("");
		}
		nLine++;
	}
	MessageToParent(cell.row, nLine, GVNM_COUNTPASTE);

	
	CIdCell tmpCell;
	CIdCell TargetCell;
	CCell* pCell{};
	strLine = strText;
	nLine = 0;
	MessageToParent(cell.row, cell.col, GVNM_BEGINPASTE);

	while (!strLine.IsEmpty())
	{
		int nColumn = 0;

		nIndex = strLine.Find('\n');
		if (nIndex != -1)
		{
			string = strLine.Left(nIndex);
			strLine = strLine.Mid(nIndex+1);
		}
		else
		{
			string = strLine;
			strLine = _T("");
		}

		while (!string.IsEmpty())
		{
			nIndex = string.Find('\t');
			if (nIndex != -1)
			{
				text = string.Left(nIndex);
				string = string.Mid(nIndex+1);
			}
			else
			{
				text = string;
				string = _T("");
			}

			pCell = GetCell(cell.row + nLine, cell.col + nColumn);
			if (pCell && pCell->attr & GVAT_NOTOLECOLUMN)
			{
				nColumn++;	continue;
			}
			TargetCell.SetItem(cell.row + nLine, cell.col + nColumn);
			if (IsValid(TargetCell))
			{
				text = validAttribute(TargetCell.row, TargetCell.col, text);
				SetItemText(TargetCell.row, TargetCell.col, text);
				SetItemState(TargetCell.row, TargetCell.col,
					GetItemState(TargetCell.row, TargetCell.col) & ~GVST_SELECTED);
			}
			nColumn++;
		}
		
		MessageToParent(TargetCell.row, TargetCell.col, GVNM_PASTEROW);
		tmpCell = TargetCell;

		nLine++;
		
	}

	if (IsValid(tmpCell))
		MessageToParent(tmpCell.row, tmpCell.col, GVNM_ENDPASTE);
	else
		MessageToParent(-1, -1, GVNM_ENDPASTE);
	
	strText.UnlockBuffer();
	Invalidate();

	return TRUE;
}

COleDataSource* CfxGrid::DragRowAction()
{
	USES_CONVERSION;

// updateX_20060113
	const CRangeCell selection = GetSelectedCellRange(true);
	if (!IsValid(selection)) return NULL;

	// Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE, so we need to convert)
	CSharedFile sharedFile(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	// Get a tab delimited string to copy to cache
	CCell* pCell{}; CString string = (char)0x01;
	for (int row = selection.GetMinRow(); row <= selection.GetMaxRow(); row++) 
	{
		for (int col = 0; col < GetColumnCount(); col++)
		{
			pCell = GetCell(row, col);
			// updateX_20060114
			if (pCell && IsCellAttribute(CIdCell(row, col), GVAT_DRAGDROP))
			{
				string += pCell->symbol; string += "\t";
				string += pCell->text; string += "\t";
			}
		}
		if (row != selection.GetMaxRow()) 
			string += _T("\r");

		sharedFile.Write(T2A(string.GetBuffer(0)), string.GetLength());
		string.ReleaseBuffer();
	}

	const char c = '\0'; sharedFile.Write(&c, 1);

	const DWORD dwLen = (DWORD)sharedFile.GetLength();
	HGLOBAL hMem = sharedFile.Detach();
#if _MFC_VER	<= 0x0600
	::GlobalUnlock(hMem);
#endif
	if (!hMem) return NULL;

	hMem = ::GlobalReAlloc(hMem, dwLen, GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
	if (!hMem) return NULL;

	// Cache data
	COleDataSource* pSource = new COleDataSource();
	pSource->CacheGlobalData(CF_TEXT, hMem);

	return pSource;
}

BOOL CfxGrid::IsValidDrop(COleDataObject*pDataObject)
{
	if (!pDataObject->IsDataAvailable(CF_TEXT))
		return FALSE;

	HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
	CMemFile memFile((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

	char* pData = new char[::GlobalSize(hmem)];
	if (!pData) return FALSE;

	memFile.Read(pData, ::GlobalSize(hmem));
	::GlobalUnlock(hmem); ::GlobalFree(hmem);

	CString strText = pData;
	delete pData;

	if (strText.GetAt(0) != (char)0x01)
		return FALSE;
	return TRUE;
}

BOOL CfxGrid::DropRowAction(COleDataObject* pDataObject)
{
	if (!IsValid(m_idDrop) || !IsCellAttribute(m_idDrop, GVAT_EDIT) || !pDataObject->IsDataAvailable(CF_TEXT))
		return FALSE;

	HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
	CMemFile memFile((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

	char* pData = new char[::GlobalSize(hmem)];
	if (!pData) return FALSE;

	memFile.Read(pData, ::GlobalSize(hmem));
	::GlobalUnlock(hmem); ::GlobalFree(hmem);

	CString strText = pData;
	delete pData;

	strText.LockBuffer();
	CString strLine = strText;

	int fBool = -1;
	int dropRow = m_idDrop.row;
	int nLine = 0, pos = 0;

	if (strLine.GetAt(0) != (char)0x01)
		m_idDrag.row = -1;

	strLine = strLine.Mid(1);		// window id(dummy)

	CString	string, symbol, entry;
	while (!strLine.IsEmpty())
	{
		pos = strLine.Find("\r");
		if (pos != -1)
		{
			string  = strLine.Left(pos++);
			strLine = strLine.Mid(pos);
		}
		else
		{
			string = strLine; strLine = _T("");
		}

		if (m_idDrag.row != -1)
			fBool = InsertRow(m_idDrag.row, dropRow); // + nLine);
		else
			fBool = InsertRow("insert", dropRow); // + nLine);

		while (fBool != -1 && !string.IsEmpty())
		{
			pos = string.Find("\t");
			if (pos != -1)
			{
				symbol = string.Left(pos++); string = string.Mid(pos);
			}
			else
			{
				symbol = string; string = _T("");
			}

			pos = string.Find("\t");
			if (pos != -1)
			{
				entry = string.Left(pos++); string = string.Mid(pos);
			}
			else
			{
				entry = string; string = _T("");
			}

			for (int col = 0; col < m_nCols; col++)
			{
				CString	chksym = GetItemSymbol(dropRow+nLine, col);
				if (!symbol.CompareNoCase(chksym))
				{
					SetItemText(dropRow + nLine, col, entry);
					break;
				}
			}
		}
		nLine++;
	}
	strText.UnlockBuffer();

	m_idSelect = m_idDrop; SelectRows(m_idDrop);
	return TRUE;
}

BOOL CfxGrid::MoveColAction(int sCol, int dCol)
{
	if (sCol < 0 || sCol >= m_nCols)
		return FALSE;
	if (dCol < 0 || dCol >= m_nCols)
		return FALSE;

	if (sCol == dCol) return TRUE;
	if (sCol < dCol) dCol -= 1;

	GRID_ROW* pRow{}; CCell* cell{};
	for (int nRow = 0; nRow < m_nRows; nRow++)
	{
		pRow = m_aryGridRows[nRow];
		cell = pRow->GetAt(sCol);
		pRow->RemoveAt(sCol);

		pRow->InsertAt(dCol, cell);
	}

	const int nWidth = m_aryColWidth[sCol];
	m_aryColWidth.RemoveAt(sCol);
	m_aryColWidth.InsertAt(dCol, nWidth);

	return TRUE;
}

COLORREF CfxGrid::DisplayAttribute(int attr, CString& data)
{
	if (data.IsEmpty())
	{
		data = _T("");	return (COLORREF) 0;
	}
	
	const int checkattr = GVAT_ZEROSUP|GVAT_ZEROSUPx|GVAT_COMMA|GVAT_FLOAT1|GVAT_FLOAT2|GVAT_FLOAT4|GVAT_CONDITION|GVAT_CONDITIONx;

	if (!(attr & checkattr)) return (COLORREF) 0;

	int idx{}; char sign = ' ';
	CString	string = data, stringx;
	string.TrimLeft(); string.TrimRight();

	if ((idx = string.FindOneOf("+-")) == 0)
	{
		sign = string.GetAt(idx);
		string.Delete(idx);
	}

	if (attr & GVAT_ZEROSUP)
	{
		while (string.GetLength())
		{
			if (string[0] == _T('0'))
			{
				string.Delete(0);
				continue;
			}
			break;
		}

		if (string.IsEmpty())
			string += _T("0");
	}

	if (attr & GVAT_ZEROSUPx)
	{
		if (atof(string) == 0) string.Empty();
	}


	stringx = string;
	const int stringL = stringx.GetLength();

	switch (attr & (GVAT_COMMA|GVAT_FLOAT1|GVAT_FLOAT2|GVAT_FLOAT4))
	{
	case GVAT_COMMA:
		stringx = CommaModify(string);
		break;
	case GVAT_FLOAT1:
		if (stringL >= 2)
			stringx.Format("%s.%s", string.Left(stringL -1), string.Right(1));
		else
			stringx.Format("0.%s", string);
		break;
	case GVAT_FLOAT2:
		if (stringL >= 3)
			stringx.Format("%s.%s", string.Left(stringL -2), string.Right(2));
		else
			stringx.Format("0.%s", string);
		break;
	case GVAT_FLOAT4:
		if (stringL >= 5)
			stringx.Format("%s.%s", string.Left(stringL -4), string.Right(4));
		else
			stringx.Format("0.%s", string);
		break;
	}

	data = stringx;

	if (attr & (GVAT_CONDITION|GVAT_CONDITIONx))
	{
		if ((attr & GVAT_CONDITION) && sign != ' ')
			data.Insert(0, sign);

		switch (sign)
		{
		case '+': return m_upCOL;
		case '-': return m_dnCOL;
		default:  return m_eqCOL;
		}
	}
	else if (sign != ' ')
	{
		data.Insert(0, sign);
	}

	return (COLORREF) 0;
}


CString CfxGrid::CommaModify(CString string)
{
	CString stripData;
	const int nPos = string.Find('.');
	
	if (nPos == -1)
		stripData = string;
	else
		stripData = string.Left(nPos);

	stripData.TrimLeft(); stripData.TrimRight();

	if (stripData.GetLength() <= 3)
		return string;
/*
	if (stripData.Find('.') != -1)
		return stripData;
*/

	char dChar = 0x00;
	switch (stripData[0])
	{
	case '+':
		dChar = '+'; stripData.Delete(0);
		break;
	case '-':
		dChar = '-'; stripData.Delete(0);
		break;
	}

	stripData.MakeReverse();
	CString strComma = _T("");
	for (int ii = 0; ii < stripData.GetLength(); ii++)
	{
		if ((ii != 0) && ((ii % 3) == 0))
			strComma += ',';

		strComma += stripData[ii];
	}

	strComma.MakeReverse();
	if (dChar != 0x00) 
		strComma.Insert(0, dChar);

	if (nPos != -1)
		strComma += string.Right(string.GetLength() - nPos);

	return strComma;
}

CString CfxGrid::validAttribute(int row, int col, CString text)
{
	if (!IsValid(CIdCell(row, col)) || text.IsEmpty())
		return _T("");

	CString	string = text;
	string.TrimLeft(); string.TrimRight();

	const UINT	attr = GetItemAttr(row, col);

	if (attr & GVAT_COMMA) string.Remove(',');
	if (attr & (GVAT_FLOAT1|GVAT_FLOAT2|GVAT_FLOAT4)) string.Remove('.');

	return string;
}

bool CfxGrid::SetImageListIndex(int nRow, int nCol, int index)
{
	GVITEM gvitem;

	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_STATE|GVMK_ATTR|GVMK_IMAGE|GVMK_BKCOLOR|GVMK_FGCOLOR;
	gvitem.row = nRow;
	gvitem.col = nCol;

	if (!GetItem(&gvitem) || gvitem.image == index)
		return false;

	gvitem.image = index;
	SetItem(&gvitem);

	InvalidateCellRect(CIdCell(nRow, nCol));
	return true;
}

void CfxGrid::SetSelectRow(int row)
{
	const CIdCell cell(row, 0);

	ResetSelectedRange();
	EnsureVisible(cell); 

	m_idSelect = CIdCell(row, 0);
	SelectRows(cell);
}

void CfxGrid::SetFocusCellEdit(int row, int col, bool edit)
{
	const CIdCell cell(row, col);

	ResetSelectedRange();
	EnsureVisible(cell); 

	m_idSelect = CIdCell(row, 0);
	SelectRows(cell);

	SetFocusCell(cell);

	if (edit) OnEditCell(row, col, VK_END);
}

void CfxGrid::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	Adjust();	
}

void CfxGrid::SetConditionColor(COLORREF upCOL, COLORREF dnCOL, COLORREF eqCOL)
{
	m_upCOL = upCOL;
	m_dnCOL = dnCOL;
	m_eqCOL = eqCOL;
}

void CfxGrid::SetTitleTips(BOOL bEnable)	
{ 
	m_enabletip = bEnable; 
	if (m_enabletip)
	{
		if (m_tooltip)
			m_tooltip.DestroyWindow();
		m_tooltip.Create(this);
	}
}

void CfxGrid::drawContrast(CDC *pDC, CRect cellRc, CString sText, COLORREF clr)
{
	CRect	pRc = cellRc;
	float	hh = (float)cellRc.Height();
	CSize	size;
	CPoint	pts[7];
	CPen	pen, * pOldPen{};
	CBrush	brush, * pOldBrush{};
	
	pen.CreatePen(PS_SOLID, 1, clr);
	pOldPen = (CPen *) pDC->SelectObject(&pen);

	brush.CreateSolidBrush(clr); // bkcolor
	pOldBrush = (CBrush *) pDC->SelectObject(&brush);

	pRc.top = (cellRc.top + cellRc.Height()/2) - (int)(hh/2);
	pRc.bottom = (int)(pRc.top + hh);
	
	size = pDC->GetOutputTextExtent(ARR_UP);
	if (size.cx > 10)
		pRc.left = (int)(pRc.right - (size.cx * 0.8));
	else
		pRc.left = pRc.right - size.cx;
	
	hh = (float)(int)(pRc.Height()+1)/2;
	hh = (float)(int)((hh+1)/2) * 2 - 1;
	int ww = pRc.Width();
	pRc.right = pRc.left + 8;
	ww = pRc.Width() / 2 - 1;
	
	if (pRc.left >= cellRc.left && pRc.right <= cellRc.right)
	{
		pDC->SetPolyFillMode(ALTERNATE);
		if (!sText.Compare(ARR_UP))
		{
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.top;
			pts[3].y = pRc.bottom;
			pts[4].y = pRc.bottom;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.left + 1;
			pts[6].x = pRc.right - 1;
			pts[2].x = pRc.left + ww;
			pts[3].x = pRc.left + ww;
			pts[4].x = pRc.right - ww;
			pts[5].x = pRc.right - ww;
			pDC->Polygon(pts, 7);
		}
		else if (!sText.Compare(ARR_DN))	
		{
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.bottom;
			pts[3].y = pRc.top;
			pts[4].y = pRc.top;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.right - 1;
			pts[6].x = pRc.left + 1;
			pts[2].x = pRc.right - ww;
			pts[3].x = pRc.right - ww;
			pts[4].x = pRc.left + ww;
			pts[5].x = pRc.left + ww;
			pDC->Polygon(pts, 7);
		}
		else if (!sText.Compare(TRI_UP))	
		{
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.top;
			pts[1].y = pRc.bottom;
			pts[2].y = pRc.bottom;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.left;
			pts[2].x = pRc.right;
			pDC->Polygon(pts, 3);
		}
		else if (!sText.Compare(TRI_DN))	
		{
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.bottom;
			pts[1].y = pRc.top;
			pts[2].y = pRc.top;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.right;
			pts[2].x = pRc.left;
			pDC->Polygon(pts, 3);
		}
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
	pen.DeleteObject();
}

#include "math.h"
double CfxGrid::TOfabs(CString string)
{
	double dval = 0;
	if (string.IsEmpty()) return dval;

	dval = fabs(atof(string));
	return dval;
}


void CfxGrid::ShowZeroData(BOOL bShow /* = TRUE */)
{
	m_bShowZero = bShow;
}

void CfxGrid::ShowPlusData(BOOL bShow)
{
	m_bShowPlus = bShow;
}

BOOL CfxGrid::IsNumeric(const CString& strVal)
{
	const int nLength = strVal.GetLength();

    for(int i = 0 ; i < nLength ; i++)
    {
		const char ch = strVal.GetAt(i);
        if( !((ch >= '0') && (ch <= '9')) && ch != '.' && ch != ',')
            return FALSE;
    }

    return TRUE;
}

#pragma warning (default : 26409)
#pragma warning (default : 26496)
#pragma warning (default : 26400)
#pragma warning (default : 26401)
#pragma warning (default : 26494)
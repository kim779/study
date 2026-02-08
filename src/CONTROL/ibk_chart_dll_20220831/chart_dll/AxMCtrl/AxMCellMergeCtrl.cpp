// AxMCellMergeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AxMCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* _cellMergeCtrlClassName = "__axCellMergeCtrlClass";

#define CM_BACKCOLOR		clWhite
#define CM_MERGECOLOR		clTeal
#define CM_SELECTEDCOLOR	clNavy

/////////////////////////////////////////////////////////////////////////////
// CAxMCellMerge

CAxMCellMerge::CAxMCellMerge()
{
	ZeroMemory(m_cells, sizeof(CELLINFO) * TMAX_COL * TMAX_ROW);
	m_rc.SetRectEmpty();
	m_cols = m_rows = 0;
	
	ReleaseVirtualRange();
}

CAxMCellMerge::~CAxMCellMerge()
{
	ClearRange();
}

void CAxMCellMerge::ClearRange()
{
	for (int i = 0; i < TMAX_ROW; i++) {
		for (int j = 0; j < TMAX_COL; j++) {
			if (i < m_rows && j < m_cols) {
				m_cells[j][i].active = TRUE;
			}
			else {
				ZeroMemory(&m_cells[j][i], sizeof(CELLINFO));
			}
		}
	}

	while (!m_range.IsEmpty()) {
		delete m_range.RemoveHead();
	}
}

void CAxMCellMerge::SetSize(CRect Rect, int col, int row)
{
	m_rc = Rect;
	BOOL bInitActive;

	if (m_cols != col || m_rows != row) {
		bInitActive = TRUE;
		ClearRange();
	}
	else  {
		bInitActive = FALSE;
	}

	m_cols = col;
	m_rows = row;

	CSize sz = CalcSize();

	for (int i = 0; i < TMAX_ROW; i++) {
		for (int j = 0; j < TMAX_COL; j++) {
			if (i < row && j < col) {
				if (bInitActive)
					m_cells[j][i].active = TRUE;
				m_cells[j][i].rc.left = j * sz.cx;
				m_cells[j][i].rc.top  = i * sz.cy;
				m_cells[j][i].rc.right = (j == col - 1) ? m_rc.right: (j+1) * sz.cx;
				m_cells[j][i].rc.bottom = (i == row - 1) ? m_rc.bottom: (i+1) * sz.cy;
			}
			else {
				ZeroMemory(&m_cells[j][i], sizeof(CELLINFO));
			}
		}
	}
}

void CAxMCellMerge::SetSize(CRect Rect)
{
	SetSize(Rect, m_cols, m_rows);
}

void CAxMCellMerge::SetSize(int cols, int rows)
{
	SetSize(m_rc, cols, rows);
}

CSize CAxMCellMerge::CalcSize()
{
	CSize sz;
	sz.cx = (int)(m_rc.Width() * 1. / m_cols);
	sz.cy = (int)(m_rc.Height() * 1. / m_rows);

	return sz;
}

BOOL CAxMCellMerge::HitTest(CPoint pt, CELL& cell)
{
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			if (GetCell(j, i)->active && ::PtInRect(&GetRect(j, i), pt)) {
				cell.col = j;
				cell.row = i;
				if (IsMerged(cell)) {
					GetMergeOwner(cell);
				}
				return TRUE;
			}
		}
	}
	cell.col = -1;
	cell.row = -1;
	return FALSE;
}

CRect CAxMCellMerge::GetRect(int col, int row)
{
	CELL cell;
	cell.col = col;
	cell.row = row;
	LPCELLINFO pcell = GetCell(cell);

	CRect rc(0, 0, 0, 0);
	if (!pcell->active) {
	}
	else if (!IsMerged(cell)) {
		rc = pcell->rc;
	}
	else {
		LPCELLRANGE range = GetCellRange(cell);
		ASSERT(range);
		CRect rcMerge;
		LPCELLINFO pcTo = GetCell(range->to);
		rcMerge = pcTo->rc;

		rc.UnionRect(&pcell->rc, rcMerge);

	}
	return rc;
}

CRect CAxMCellMerge::GetVirtualRangeRect()
{
	CRect rc(0, 0, 0, 0);
	LPCELLINFO pcfrom = GetCell(m_virtualRange.from);
	LPCELLINFO pcto   = GetCell(m_virtualRange.to);
	if (!pcfrom || !pcto)
		return rc;

	rc.UnionRect(&pcfrom->rc, &pcto->rc);
	return rc;
}

BOOL CAxMCellMerge::ExistVirtualRange()
{
	return (m_virtualRange.from.row >= 0);
}

void CAxMCellMerge::GetMergeOwner(CELL& cell)
{
	LPCELLINFO pc;

	pc = GetCell(cell);

	if (pc->active) return;

	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			if (IsMerged(j, i)) {
				LPCELLRANGE range = GetCellRange(j, i);
				memcpy(&cell, &range->from, sizeof(CELL));
				return;
			}
		}
	}
}

void CAxMCellMerge::ComplexFromTo(CELL& from, CELL& to)
{
	int minRow = min(from.row, to.row);
	int minCol = min(from.col, to.col);
	int maxRow = max(from.row, to.row);
	int maxCol = max(from.col, to.col);

	from.row = minRow;
	from.col = minCol;

	to.row = maxRow;
	to.col = maxCol;
}

void CAxMCellMerge::DeleteRange(LPCELLRANGE range)
{
	POSITION pos = m_range.Find(range);
	if (pos) {
		LPCELLRANGE range = m_range.GetAt(pos);
		m_range.RemoveAt(pos);
		delete range;
	}
}

BOOL CAxMCellMerge::AddMerge(CELL from, CELL to)
{
	if (from.col == to.col && from.row == to.row)
		return FALSE;

	ComplexFromTo(from, to);

	int i, j;

	LPCELLRANGE range = new CELLRANGE;
	memcpy(&range->from, &from, sizeof(CELL));
	memcpy(&range->to, &to, sizeof(CELL));

	NormalizeRange(range, TRUE);

	CELL f, t;
	LPCELLRANGE pc;
	memcpy(&f, &range->from, sizeof(CELL));
	memcpy(&t, &range->to, sizeof(CELL));

	BOOL swap;

	while (TRUE) {
		swap = FALSE;
		for (i = range->from.row; i <= range->to.row; i++) {
			for (j = range->from.col; j <= range->to.col; j++) {

				if (IsMerged(j, i)) {
					pc = GetCellRange(j, i);
					
					if (pc->from.col < f.col) { 
						swap = TRUE;
						f.col = pc->from.col;
					}
					if (pc->from.row < f.row) {
						swap = TRUE;
						f.row = pc->from.row;
					}
					if (pc->to.col > t.col)	{
						swap = TRUE;
						t.col = pc->to.col;
					}
					if (pc->to.row > t.row)	{
						swap = TRUE;
						t.row = pc->to.row;
					}
				}
			}
		}
		if (!swap)
			break;

		memcpy(&range->from, &f, sizeof(CELL));
		memcpy(&range->to, &t, sizeof(CELL));
	}
	memcpy(&range->from, &f, sizeof(CELL));
	memcpy(&range->to, &t, sizeof(CELL));

	NormalizeRange(range, TRUE);
	for (i = range->from.row; i <= range->to.row; i++) {
		for (j = range->from.col; j <= range->to.col; j++) {
			GetCell(j, i)->active = (i == range->from.row && j == range->from.col);
		}
	}


	m_range.AddTail(range);

	return TRUE;
}

BOOL CAxMCellMerge::SetVirtualRange(CELL from, CELL to)
{
	ComplexFromTo(from, to);

	int i, j;

	LPCELLRANGE range = new CELLRANGE;
	memcpy(&range->from, &from, sizeof(CELL));
	memcpy(&range->to, &to, sizeof(CELL));

	NormalizeRange(range, FALSE);
	
	CELL f, t;
	LPCELLRANGE pc;
	memcpy(&f, &range->from, sizeof(CELL));
	memcpy(&t, &range->to, sizeof(CELL));

	BOOL swap;

	while (TRUE) {
		swap = FALSE;
		for (i = range->from.row; i <= range->to.row; i++) {
			for (j = range->from.col; j <= range->to.col; j++) {

				if (IsMerged(j, i)) {
					pc = GetCellRange(j, i);
					
					if (pc->from.col < f.col) { 
						swap = TRUE;
						f.col = pc->from.col;
					}
					if (pc->from.row < f.row) {
						swap = TRUE;
						f.row = pc->from.row;
					}
					if (pc->to.col > t.col)	{
						swap = TRUE;
						t.col = pc->to.col;
					}
					if (pc->to.row > t.row)	{
						swap = TRUE;
						t.row = pc->to.row;
					}
				}
			}
		}
		if (!swap)
			break;

		memcpy(&range->from, &f, sizeof(CELL));
		memcpy(&range->to, &t, sizeof(CELL));
	}
	memcpy(&range->from, &f, sizeof(CELL));
	memcpy(&range->to, &t, sizeof(CELL));

	memcpy(&m_virtualRange, range, sizeof(CELLRANGE));
	delete range;

	return TRUE;
}

CString CAxMCellMerge::GetMergeString()
{
	CString result, merge;

	result.Format("%d,%d;", m_cols, m_rows);

	LPCELLRANGE range;
	for (POSITION pos = m_range.GetHeadPosition(); pos; ) {
		range = m_range.GetNext(pos);
		merge.Format("%d,%d*%d,%d;", range->from.col, range->from.row, range->to.col, range->to.row);
		result += merge;
	}

	return result;
}

CString CAxMCellMerge::GetToken(CString& buffer, char delim)
{
	int find = buffer.Find(delim);

	if (find > 0) {
		CString result = buffer.Left(find);
		buffer.Delete(0, find + 1);
		return result;
	}
	else {
		return "";
	}
}

void CAxMCellMerge::GetCell(CString str, CELL& cell)
{
	CString col = GetToken(str, ',');
	
	cell.col = atoi(col);
	cell.row = atoi(str);
}

void CAxMCellMerge::SetMergeString(LPCTSTR mergeString)
{
	ClearRange();
	CString str(mergeString);

	if (str.IsEmpty()) {
		SetSize(1, 1);
		return;
	}

	CString tok = GetToken(str), tok1;

	if (tok.IsEmpty()) {
		SetSize(1, 1);
		return;
	}

	CELL cell, cell1;
	GetCell(tok, cell);
	SetSize(cell.col, cell.row);


	while (TRUE) {
		tok = GetToken(str);
		if (tok.IsEmpty()) 
			break;
	
		tok1 = GetToken(tok, '*');

		GetCell(tok1, cell);
		GetCell(tok, cell1);

		AddMerge(cell, cell1);
	}	
}

BOOL CAxMCellMerge::IsMerged(int col, int row)
{
	CELL cell;
	cell.col = col;
	cell.row = row;
	return IsMerged(cell);
}

BOOL CAxMCellMerge::IsMerged(CELL cell)
{
	LPCELLRANGE range;

	for (POSITION pos = m_range.GetHeadPosition(); pos; ) {
		range = m_range.GetNext(pos);
		if (CellInRange(range, &cell)) {
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CAxMCellMerge::IsActive(int col, int row)
{
	LPCELLINFO pcell = GetCell(col, row);
	if (pcell)
		return pcell->active;
	else
		return FALSE;
}


BOOL CAxMCellMerge::CellInRange(LPCELLRANGE range, LPCELL cell)
{
	return (range->from.col <= cell->col &&
			range->from.row <= cell->row &&
			range->to.col >= cell->col &&
			range->to.row >= cell->row);
}

LPCELLRANGE CAxMCellMerge::GetCellRange(CELL cell)
{
	LPCELLRANGE range;

	for (POSITION pos = m_range.GetHeadPosition(); pos; ) {
		range = m_range.GetNext(pos);
		if (CellInRange(range, &cell)) //range->from.row == cell.row && range->from.col == cell.col)
			return range;
	}
	return NULL;
}

LPCELLRANGE CAxMCellMerge::GetCellRange(int col, int row)
{
	CELL cell;
	cell.col = col;
	cell.row = row;
	return GetCellRange(cell);
}

void CAxMCellMerge::ReleaseVirtualRange()
{
	m_virtualRange.from.col = -1;
	m_virtualRange.from.row = -1;
}

void CAxMCellMerge::NormalizeRange(LPCELLRANGE range, BOOL bDelete)
{
	LPCELLRANGE temp;
	temp = GetCellRange(range->from);
	if (temp) {
		memcpy(&range->from, &temp->from, sizeof(CELL));
		if (temp->to.col > range->to.col)
			range->to.col = temp->to.col;
		if (temp->to.row > range->to.row)
			range->to.row = temp->to.row;

		if (bDelete)
			DeleteRange(temp);
	}
	temp = GetCellRange(range->to);
	if (temp) {
		memcpy(&range->to, &temp->to, sizeof(CELL));
		if (temp->from.col < range->from.col)
			range->from.col = temp->from.col;
		if (temp->from.row < range->from.row)
			range->from.row = temp->from.row;

		if (bDelete)
			DeleteRange(temp);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAxMCellMergeCtrl

CAxMCellMergeCtrl::CAxMCellMergeCtrl()
{
	XRegisterWindowClass(_cellMergeCtrlClassName, (HBRUSH)::GetStockObject(WHITE_BRUSH));

	m_back = NULL;

	m_clrBack = CM_BACKCOLOR;
	m_clrMerged = CM_MERGECOLOR;
	m_clrSelected = CM_SELECTEDCOLOR;
}

CAxMCellMergeCtrl::~CAxMCellMergeCtrl()
{
	if (m_back)
		::DeleteObject(m_back);
}


BEGIN_MESSAGE_MAP(CAxMCellMergeCtrl, CWnd)
	//{{AFX_MSG_MAP(CAxMCellMergeCtrl)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxMCellMergeCtrl message handlers

BOOL CAxMCellMergeCtrl::Create(CWnd* parent, CRect rc)
{
	m_parent = parent;

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	CDC* dc = parent->GetDC();
	if (dc) {
		lf.lfHeight = -MulDiv(9, GetDeviceCaps(dc->m_hDC, LOGPIXELSY), 72);
		parent->ReleaseDC(dc);
	}
	else {
		lf.lfHeight = 14;
	}
	strcpy(lf.lfFaceName, "±¼¸²");
	m_font.CreateFontIndirect(&lf);

	m_mouseDown.col = -1;
	m_mouseDown.row = -1;

	BOOL result = CWnd::Create(_cellMergeCtrlClassName, NULL, WS_VISIBLE | WS_CHILD, rc, parent, NULL);
	if (result) {
		m_cell.SetSize(rc, TMAX_COL, TMAX_ROW);
		Modified();
	}

	return result;
}

void CAxMCellMergeCtrl::Modified()
{
	if (m_back) {
		::DeleteObject(m_back);
	}

	CRect rc;
	GetClientRect(&rc);
	CWindowDC dc(this);
	CDC mdc;

	mdc.CreateCompatibleDC(&dc);
	m_back = ::CreateCompatibleBitmap(dc.m_hDC, rc.Width(), rc.Height());

	HBITMAP oldBmp = (HBITMAP)::SelectObject(mdc.m_hDC, m_back);

	CBrush brBack, brMerged, brSelected, *oldBr;
	brBack.CreateSolidBrush(m_clrBack);
	brMerged.CreateSolidBrush(m_clrMerged);
	brSelected.CreateSolidBrush(m_clrSelected);

	oldBr = mdc.SelectObject(&brBack);

	for (int i = 0; i < m_cell.GetRowCount(); i++) {
		for (int j = 0; j < m_cell.GetColCount(); j++) {
			if (m_cell.GetCell(j, i)->active) {
				if (m_cell.IsMerged(j, i)) {
					mdc.SelectObject(brMerged);
				}
				else
					mdc.SelectObject(brBack);

				mdc.Rectangle(&m_cell.GetRect(j, i));
			}
		}
	}

	if (m_cell.ExistVirtualRange()) {
		mdc.SelectObject(brSelected);
		mdc.Rectangle(m_cell.GetVirtualRangeRect());
	}


	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);

	mdc.SelectObject(oldBr);
	::SelectObject(mdc.m_hDC, oldBmp);
}

void CAxMCellMergeCtrl::OnPaint() 
{
	CPaintDC dc(this);

	DrawBack(&dc);		
}

void CAxMCellMergeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	m_cell.SetSize(CRect(0, 0, cx, cy));
	Modified();
}

BOOL CAxMCellMergeCtrl::OnEraseBkgnd(CDC* pDC) 
{
	if (m_back) {
		DrawBack(pDC);
		return TRUE;
	}
	else
		return CWnd::OnEraseBkgnd(pDC);
}

void CAxMCellMergeCtrl::DrawBack(CDC* dc)
{
	CRect rc;
	GetClientRect(&rc);

	CDC mdc;
	mdc.CreateCompatibleDC(dc);

	HBITMAP oldBmp = (HBITMAP)::SelectObject(mdc.m_hDC, m_back);

	dc->BitBlt(0, 0, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
	::SelectObject(mdc.m_hDC, oldBmp);
}

void CAxMCellMergeCtrl::SetMergeString(LPCTSTR lpszMergeString)
{
	m_cell.SetMergeString(lpszMergeString);
	Modified();
}

CString CAxMCellMergeCtrl::GetMergeString()
{
	return m_cell.GetMergeString();
}

void CAxMCellMergeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_cell.HitTest(point, m_mouseDown);	
	CWnd::OnLButtonDown(nFlags, point);
}

void CAxMCellMergeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CELL to;
	if (m_mouseDown.col >= 0) {
		if (m_cell.HitTest(point, to)) {
			if (m_cell.AddMerge(m_mouseDown, to)) {
				/**
				m_mergeString = m_cell.GetMergeString();
				Debug(m_mergeString);
				**/
				// modify merge information..
			}
		}
		m_cell.ReleaseVirtualRange();
		m_mouseDown.col = -1;
		m_mouseDown.row = -1;
		Modified();
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CAxMCellMergeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_mouseDown.col >= 0) {
		CELL to;
		if (m_cell.HitTest(point, to)) {
			if (m_cell.SetVirtualRange(m_mouseDown, to)) {
			}
			Modified();
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}


void CAxMCellMergeCtrl::Initialize()
{
	m_cell.ClearRange();
	Modified();
}



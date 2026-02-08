// AxMTableMergeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AxMCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* _tableMergeCtrlClassName = "__axTableMergeCtrlClass";

#define IDC_INIT		1000

const int WINDOW_WIDTH	= 160;
const int WINDOW_HEIGHT	= 120;

const int btnWidth = 40, btnHeight = 18;
const int margin = 2;

/////////////////////////////////////////////////////////////////////////////
// CAxMTableMergeCtrl

CAxMTableMergeCtrl::CAxMTableMergeCtrl()
{
	XRegisterWindowClass(_tableMergeCtrlClassName, ::CreateSolidBrush(COLOR_BTNFACE));

	m_btnInit = m_btnOK = m_btnCancel = NULL;
	m_cell = NULL;
	m_mergeString.Empty();

	m_cellInfo.SetSize(TMAX_COL, TMAX_ROW);
	m_cellInfo.ClearRange();
	m_mergeString = m_cellInfo.GetMergeString();
}

CAxMTableMergeCtrl::~CAxMTableMergeCtrl()
{
}

BEGIN_MESSAGE_MAP(CAxMTableMergeCtrl, CWnd)
	//{{AFX_MSG_MAP(CAxMTableMergeCtrl)
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_INIT, OnInit)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxMTableMergeCtrl message handlers

void CAxMTableMergeCtrl::OnDestroy() 
{
	SAFE_DELETE_WND(m_btnInit);
	SAFE_DELETE_WND(m_btnOK);
	SAFE_DELETE_WND(m_btnCancel);
	SAFE_DELETE_WND(m_cell);

	m_font.DeleteObject();
	CWnd::OnDestroy();
}

BOOL CAxMTableMergeCtrl::Create(CWnd* parent, int x, int y)
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

	strcpy(lf.lfFaceName, "굴림");
	m_font.CreateFontIndirect(&lf);

	CRect rc(x, y, x + WINDOW_WIDTH, y + WINDOW_HEIGHT);

	if (CWnd::CreateEx(0, _tableMergeCtrlClassName, NULL, WS_VISIBLE | WS_POPUP, rc, parent, NULL)) {
		CreateControls();
		return TRUE;
	}
	else 
		return FALSE;
}

void CAxMTableMergeCtrl::CreateControls()
{
	CRect rc(0, 0, 1, 1);

	m_cell = new CAxMCellMergeCtrl;
	m_cell->Create(this, rc);

	m_btnInit = new CButton;
	m_btnInit->Create("초기화", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_NOTIFY, rc, this, IDC_INIT);

	m_btnOK = new CButton;
	m_btnOK->Create("확인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_NOTIFY, rc, this, IDOK);

	m_btnCancel = new CButton;
	m_btnCancel->Create("취소", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_NOTIFY, rc, this, IDCANCEL);

	m_btnInit->SetFont(&m_font);
	m_btnOK->SetFont(&m_font);
	m_btnCancel->SetFont(&m_font);

	GetClientRect(&rc);

	m_cell->MoveWindow(margin, margin, rc.Width() - margin * 2, rc.Height() - margin * 3 - btnHeight);
	m_btnInit->MoveWindow(margin, rc.Height() - btnHeight - margin, btnWidth, btnHeight);
	m_btnOK->MoveWindow(rc.Width() - margin * 2 - btnWidth * 2, rc.Height() - btnHeight - margin, btnWidth, btnHeight);
	m_btnCancel->MoveWindow(rc.Width() - margin - btnWidth, rc.Height() - btnHeight - margin, btnWidth, btnHeight);

	if (!m_mergeString.IsEmpty()) {
		m_cell->SetMergeString(m_mergeString);
		m_cellInfo.SetMergeString(m_mergeString);
	}
	else {
		m_cell->SetSize(TMAX_COL, TMAX_ROW);
	}
}

void CAxMTableMergeCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState == WA_INACTIVE)
		CloseWindow();
}

void CAxMTableMergeCtrl::CloseWindow()
{
	PostMessage(WM_CLOSE);	
}

void CAxMTableMergeCtrl::SetMergeString(LPCTSTR lpszMergeString)
{
	m_mergeString = lpszMergeString;
	m_cellInfo.SetMergeString(m_mergeString);
	if (m_cell)
		m_cell->SetMergeString(m_mergeString);
}

CString CAxMTableMergeCtrl::GetMergeString()
{
	return m_mergeString;
}

void CAxMTableMergeCtrl::OnOK()
{
	CString merge = m_cell->GetMergeString();
	
	if (merge != m_mergeString) {
		m_mergeString = merge;
		m_cellInfo.SetMergeString(merge);

		if (m_parent)
			m_parent->SendMessage(AXM_CELL_NOTIFY);
	}

	CloseWindow();
}

void CAxMTableMergeCtrl::OnCancel()
{
	CloseWindow();
}

void CAxMTableMergeCtrl::OnInit()
{
	m_cell->Initialize();
}

BOOL CAxMTableMergeCtrl::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;
	GetClientRect(&rc);

	CBrush br;
	br.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	CBrush *oldBr = pDC->SelectObject(&br);
	pDC->Rectangle(rc);
	pDC->SelectObject(oldBr);

	return TRUE;
}

void CAxMTableMergeCtrl::SetSize(int cols, int rows)
{
	if (cols != m_cellInfo.GetColCount() || rows != m_cellInfo.GetRowCount()) {
		m_cellInfo.SetSize(cols, rows);
		m_mergeString = m_cellInfo.GetMergeString();
	}
}

void CAxMTableMergeCtrl::SetSize(LPRECT lpRect)
{
	m_cellInfo.SetSize(lpRect);
	m_mergeString = m_cellInfo.GetMergeString();
}

void CAxMTableMergeCtrl::SetSize(LPRECT lpRect, int cols, int rows)
{
	m_cellInfo.SetSize(lpRect, cols, rows);
	m_mergeString = m_cellInfo.GetMergeString();
}

void CAxMTableMergeCtrl::Initialize()
{
	m_cellInfo.ClearRange();
	m_mergeString = m_cellInfo.GetMergeString();
}

BOOL CAxMTableMergeCtrl::IsActive(int col, int row)
{
	return m_cellInfo.IsActive(col, row);
}

CRect CAxMTableMergeCtrl::GetRect(int col, int row)
{
	return m_cellInfo.GetRect(col, row);
}

int CAxMTableMergeCtrl::GetColCount()
{
	return m_cellInfo.GetColCount();
}

int CAxMTableMergeCtrl::GetRowCount()
{
	return m_cellInfo.GetRowCount();
}


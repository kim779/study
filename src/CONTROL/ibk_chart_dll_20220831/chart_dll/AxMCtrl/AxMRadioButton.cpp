// AxMRadioButton.cpp : implementation file
//

#include "stdafx.h"
#include "AxMCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxMRadioButton

CAxMRadioButton::CAxMRadioButton()
{
	m_check = FALSE;
	m_messageHandler = NULL;
}

CAxMRadioButton::~CAxMRadioButton()
{
}


BEGIN_MESSAGE_MAP(CAxMRadioButton, CButton)
	//{{AFX_MSG_MAP(CAxMRadioButton)
	ON_MESSAGE(AXM_SETMSGHANDLER, OnSetMessageHandler)
	ON_MESSAGE(AXM_GETMSGHANDLER, OnGetMessageHandler)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BM_SETCHECK, OnSetCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxMRadioButton message handlers
BOOL CAxMRadioButton::Create(LPCTSTR szText, LPRECT rc, CWnd* parent, UINT id)
{
	return CButton::Create(szText, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_OWNERDRAW, *rc, parent, id);
}

BOOL CAxMRadioButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CButton::OnEraseBkgnd(pDC);
}

void CAxMRadioButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	DrawButton(&dc);
}

void CAxMRadioButton::DrawButton(CDC* dc)
{
	CRect clientRect, rc;
	GetClientRect(&clientRect);
	CFont* oldFont;
	CString caption;
	GetWindowText(caption);

	oldFont = dc->SelectObject(GetFont());
	dc->SetBkMode(TRANSPARENT);

	rc.left = clientRect.left + 1;
	rc.top  = clientRect.top + 1;
	rc.right = rc.left + 11;
	rc.bottom = rc.top + 11;

	CPen pen, *oldPen;
	CBrush br, brCheck, *oldBr;

	COLORREF color = clTeal;

	pen.CreatePen(PS_SOLID, 1, color);
	br.CreateSolidBrush(clWhite);
	//br.Attach(::GetStockObject(NULL_BRUSH));

	oldPen = dc->SelectObject(&pen);
	oldBr = dc->SelectObject(&br);

	dc->Ellipse(rc);

	if (GetCheck()) {
		brCheck.CreateSolidBrush(clTeal);
		dc->SelectObject(&brCheck);
		dc->Ellipse(rc.left + 3, rc.top + 3, rc.left + 8, rc.top + 8);
	}

	dc->TextOut(rc.right + 2, 2, caption);

	dc->SelectObject(oldPen);
	dc->SelectObject(oldBr);
	//br.Detach();
	dc->SelectObject(oldFont);
}

void CAxMRadioButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_check) {
		m_check = TRUE;
		ResetOtherRadioButton();
		Invalidate();
		Notify(WM_AXITEMCLICK, m_check, (LPARAM)this);
	}
	CButton::OnLButtonDown(nFlags, point);
}

void CAxMRadioButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CButton::OnLButtonUp(nFlags, point);
}


int CAxMRadioButton::GetCheck()
{
	return m_check;
}

void CAxMRadioButton::SetCheck(int nCheck, BOOL bRepaint)
{
	if (m_check != nCheck) {
		m_check = nCheck;
		if (bRepaint)
			Invalidate();
	}
}

void CAxMRadioButton::ResetOtherRadioButton()
{
	CWnd* parent = GetParent();
	CWnd* child = parent->GetWindow(GW_CHILD);

	while (child) {
		if (child->m_hWnd != m_hWnd)
			child->SendMessage(BM_SETCHECK);

		child = child->GetWindow(GW_HWNDNEXT);
	}
}

LRESULT CAxMRadioButton::OnSetCheck(WPARAM wParam, LPARAM lParam)
{
	SetCheck(wParam);

	return 0L;
}

LRESULT CAxMRadioButton::OnSetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	SetMessageHandler((CWnd*)wParam);
	return 0L;
}

LRESULT CAxMRadioButton::OnGetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)m_messageHandler;
}

LRESULT CAxMRadioButton::Notify(UINT msg, WPARAM wParam, LPARAM lParam)
{
	NOTIFY(msg, wParam, lParam);
}

void CAxMRadioButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	DrawButton(&dc);
	dc.Detach();
}

// BtnCombo.cpp : implementation file
//

#include "stdafx.h"
#include "axiswork.h"
#include "BtnCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBtnCombo

CBtnCombo::CBtnCombo()
{
	m_dwState = DFCS_SCROLLCOMBOBOX;
}

CBtnCombo::~CBtnCombo()
{
	delete m_pCombo;
}


BEGIN_MESSAGE_MAP(CBtnCombo, CWnd)
	//{{AFX_MSG_MAP(CBtnCombo)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_CBN_SELENDOK(1, OnSelOk)
	ON_CBN_SELENDCANCEL(1, OnSelOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBtnCombo message handlers

int CBtnCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rc;
	GetClientRect(&rc);
	
	m_pCombo = new CComboBox;
	m_pCombo->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, -5, 0, 200), this, 1);
	m_pCombo->SetFont(m_pFont);
	m_pCombo->SetItemHeight(-1, 0);
	m_pCombo->SetDroppedWidth(rc.Width());
	return 0;
}

void CBtnCombo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CString tmp;
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(255, 255, 255));
	rc.DeflateRect(0, 0, rc.Height(), 0);

	if (m_pCombo->GetCurSel() != LB_ERR)
	{
		CFont	*pOldFont;
		m_pCombo->GetLBText(m_pCombo->GetCurSel(), tmp);
		pOldFont = dc.SelectObject(m_pFont);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(tmp, rc, DT_LEFT);
		dc.SelectObject(pOldFont);
	}
	rc.InflateRect(-rc.Width(), 0, rc.Height(), 0);
	dc.DrawFrameControl(rc, DFC_SCROLL, m_dwState);
	// Do not call CWnd::OnPaint() for painting messages
}

void CBtnCombo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rc;
	GetClientRect(&rc);
	rc.DeflateRect(0, 0, rc.Height(), 0);
	if (rc.PtInRect(point) && (m_pCombo->GetCount() == 2 && 
		(m_pCombo->FindString(0, "True") >= 0 && m_pCombo->FindString(0, "False") >= 0)))
	{
		if (m_pCombo->GetCurSel() == 0)
			m_pCombo->SetCurSel(1);
		else
			m_pCombo->SetCurSel(0);
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_COMBOCTL, CBN_SELENDOK), (long)m_pCombo->GetSafeHwnd());
	}
	else
	{
		m_dwState |= DFCS_PUSHED;
		RedrawWindow();
		
		setComboWidth();
		m_pCombo->ShowDropDown();
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CBtnCombo::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_dwState &= ~DFCS_PUSHED;
	Invalidate();
	CWnd::OnLButtonUp(nFlags, point);
}

void CBtnCombo::setFont(CFont *pFont)
{
	m_pFont = pFont;
}

int CBtnCombo::AddString(CString str)
{
	return m_pCombo->AddString(str);
}

CString CBtnCombo::GetText()
{
	CString tmp;
	if (m_pCombo->GetCurSel() != LB_ERR)
		m_pCombo->GetLBText(m_pCombo->GetCurSel(), tmp);
	return tmp;
}

void CBtnCombo::OnSelOk()
{
	m_dwState &= ~DFCS_PUSHED;
	Invalidate();
	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_COMBOCTL, CBN_SELENDOK), (long)m_pCombo->GetSafeHwnd());
}

void CBtnCombo::SetItemData(int idx, DWORD data)
{
	m_pCombo->SetItemData(idx, data);
}

BOOL CBtnCombo::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DOWN)
		{
			if (!(m_dwState & DFCS_PUSHED))
			{
				m_dwState |= DFCS_PUSHED;
				Invalidate();
				m_pCombo->ShowDropDown();
			}
			else
				m_pCombo->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
			return CWnd::PreTranslateMessage(pMsg);
		}
		else if (pMsg->wParam == VK_UP)
		{
			if (m_dwState & DFCS_PUSHED)
				m_pCombo->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
			return CWnd::PreTranslateMessage(pMsg);
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CBtnCombo::setComboWidth()
{
	CString str;
	CSize   sz;
	int     dx=0;
	CRect rc;
	GetClientRect(&rc);

	CDC*    pDC = m_pCombo->GetDC();
	for (int ii=0; ii < m_pCombo->GetCount(); ii++)
	{
		m_pCombo->GetLBText(ii, str );
		sz = pDC->GetTextExtent(str);
		
		if (sz.cx > dx)
			dx = sz.cx;
	}
	m_pCombo->ReleaseDC(pDC);

	m_pCombo->SetDroppedWidth(max(rc.Width(), dx));
}

LRESULT CBtnCombo::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_KILLFOCUS)
		if ((HWND)wParam != GetParent()->m_hWnd)
			GetParent()->PostMessage(WM_USER + 1, VK_RETURN, 0);	
	return CWnd::WindowProc(message, wParam, lParam);
}




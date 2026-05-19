// MPropGridCombo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MPropGridCombo.h"
#include "MPropGridButton.h"
#include "resource.h"

#define	ID_INCOMBO	1

// CMPropGridCombo

IMPLEMENT_DYNAMIC(CMPropGridCombo, CWnd)

CMPropGridCombo::CMPropGridCombo()
{
	m_inCombo = NULL;
	m_font  = NULL;
	m_state = DFCS_SCROLLCOMBOBOX;

	m_color = RGB(255, 255, 255);
	m_txtcolor = RGB(0, 0, 0);
}

CMPropGridCombo::~CMPropGridCombo()
{
	if (m_inCombo)
		delete m_inCombo;}


BEGIN_MESSAGE_MAP(CMPropGridCombo, CWnd)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_CBN_SELENDOK(ID_INCOMBO, OnSelOk)
	ON_CBN_SELENDCANCEL(ID_INCOMBO, OnSelOk)
END_MESSAGE_MAP()



// CMPropGridCombo 메시지 처리기입니다.

BOOL CMPropGridCombo::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DOWN)
		{
			if (!(m_state & DFCS_PUSHED))
			{
				m_state |= DFCS_PUSHED;
				Invalidate();
				m_inCombo->ShowDropDown();
			}
			else
				m_inCombo->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
		}
		else if (pMsg->wParam == VK_UP)
		{
			if (m_state & DFCS_PUSHED)
				m_inCombo->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}


LRESULT CMPropGridCombo::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KILLFOCUS)
		if ((HWND)wParam != GetParent()->m_hWnd)
			GetParent()->PostMessage(WM_USER + 1, VK_RETURN, 0);	

	return CWnd::WindowProc(message, wParam, lParam);
}


int CMPropGridCombo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect	rc;

	GetClientRect(&rc);
	m_inCombo = new CComboBox;
	m_inCombo->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, -5, 0, 200), this, ID_INCOMBO);
	m_inCombo->SetFont(m_font);
	m_inCombo->SetItemHeight(-1, 0);
	m_inCombo->SetDroppedWidth(rc.Width());
	return 0;
}


void CMPropGridCombo::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect	rc;

	GetClientRect(&rc);
	rc.DeflateRect(0, 0, rc.Height(), 0);
	if (rc.PtInRect(point) && (m_inCombo->GetCount() == 2 && (m_inCombo->FindString(0, _T("True")) >= 0 && m_inCombo->FindString(0, _T("False")) >= 0)))
	{
		if (m_inCombo->GetCurSel() == 0)
			m_inCombo->SetCurSel(1);
		else
			m_inCombo->SetCurSel(0);
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_COMBOCTL, CBN_SELENDOK), (LPARAM)m_inCombo->GetSafeHwnd());
	}
	else
	{
		m_state |= DFCS_PUSHED;
		RedrawWindow();
		
		SetComboWidth();
		m_inCombo->ShowDropDown();
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void CMPropGridCombo::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_state &= ~DFCS_PUSHED;
	Invalidate();
	CWnd::OnLButtonUp(nFlags, point);
}


void CMPropGridCombo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CString	tmps;
	CRect	rc;

	GetClientRect(&rc);
	dc.FillSolidRect(rc, m_color);
	rc.DeflateRect(0, 0, rc.Height(), 0);
	if (m_inCombo->GetCurSel() != LB_ERR)
	{
		CFont	*pOldFont;

		m_inCombo->GetLBText(m_inCombo->GetCurSel(), tmps);
		pOldFont = dc.SelectObject(m_font);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(m_txtcolor);
		dc.DrawText(tmps, rc, DT_LEFT);
		dc.SelectObject(pOldFont);
	}
	rc.InflateRect(-rc.Width(), 0, rc.Height(), 0);
	dc.DrawFrameControl(rc, DFC_SCROLL, m_state);
}

void CMPropGridCombo::OnSelOk()
{
	m_state &= ~DFCS_PUSHED;
	Invalidate();
	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_COMBOCTL, CBN_SELENDOK), (LPARAM)m_inCombo->GetSafeHwnd());
}

void CMPropGridCombo::SetComboWidth()
{
	CString str;
	CSize   sz;
	CRect	rc;
	int     dx = 0;
	CDC*	pDC = m_inCombo->GetDC();

	GetClientRect(&rc);
	for (int ii=0; ii < m_inCombo->GetCount(); ii++)
	{
		m_inCombo->GetLBText(ii, str );
		sz = pDC->GetTextExtent(str);
		
		if (sz.cx > dx)
			dx = sz.cx;
	}
	m_inCombo->ReleaseDC(pDC);
	m_inCombo->SetDroppedWidth(max(rc.Width(), dx));
}

void CMPropGridCombo::SetItemData(int idx, DWORD data)
{
	m_inCombo->SetItemData(idx, data);
}

CString CMPropGridCombo::GetText()
{
	CString tmps;

	if (m_inCombo->GetCurSel() != LB_ERR)
		m_inCombo->GetLBText(m_inCombo->GetCurSel(), tmps);
	return tmps;
}

int CMPropGridCombo::AddString(CString str)
{
	return m_inCombo->AddString(str);
}

void CMPropGridCombo::SetFont(CFont* font)
{
	m_font = font;
}

void CMPropGridCombo::SetEnable(BOOL bEnable)
{
	m_color = RGB(255, 255, 255);
	m_txtcolor = RGB(0, 0, 0);
	if (!bEnable)
	{
		m_color = RGB(240, 240, 240);
		m_txtcolor = RGB(109, 109, 109);
	}
}

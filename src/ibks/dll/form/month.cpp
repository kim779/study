// month.cpp : implementation file
//

#include "stdafx.h"
#include "month.h"
#include "fmedit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( disable : 26454 )

#define	idMonth	10002

/////////////////////////////////////////////////////////////////////////////
// Cmonth

Cmonth::Cmonth()
{
	m_edit = (CfmEdit *)0;
	m_date.Empty();
}

Cmonth::~Cmonth()
{
}


BEGIN_MESSAGE_MAP(Cmonth, CMonthCalCtrl)
	//{{AFX_MSG_MAP(Cmonth)
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(NM_RELEASEDCAPTURE, &Cmonth::OnReleasedcapture)
	ON_NOTIFY_REFLECT(MCN_SELECT, &Cmonth::OnSelect)
	ON_NOTIFY_REFLECT(MCN_SELCHANGE, &Cmonth::OnMcnSelchange)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
//	ON_NOTIFY_REFLECT(MCN_GETDAYSTATE, &Cmonth::OnMcnGetdaystate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cmonth message handlers

BOOL Cmonth::Create(CWnd* parent, CRect rect, CfmEdit* edit)
{
//	CRect	rc, cRc;

	m_edit = edit;
	if (!CMonthCalCtrl::Create(WS_CHILD|WS_BORDER|WS_VISIBLE|WS_CLIPSIBLINGS, CPoint(rect.left, rect.bottom), parent, idMonth))
		return FALSE;

/*	if (GetMinReqRect(&rc))
	{
		rc.SetRect(rect.right-rc.Width(), rect.bottom, rect.right, rect.bottom+rc.Height());
		if (rc.left < 0)
			rc.OffsetRect(-rc.left, 0);
		parent->GetClientRect(&cRc);
		if (rc.bottom > cRc.bottom)
			rc.OffsetRect(0, rect.top-rc.bottom);
		if (rc.top < 0)
			rc.OffsetRect(0, -rc.top);
		rc.InflateRect(2, 0);
		MoveWindow(rc);
	}
*/
	setDate(true);
	SetFocus();
	SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
//	::SetCapture(GetSafeHwnd());
	return TRUE;
}

int Cmonth::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMonthCalCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void Cmonth::OnKillFocus(CWnd* pNewWnd) 
{
	POINT	pt = {};
	MCHITTESTINFO	ht;

	CMonthCalCtrl::OnKillFocus(pNewWnd);
	::GetCursorPos(&pt);
	ScreenToClient(&pt);
	ht.cbSize = sizeof(MCHITTESTINFO);
	ht.pt = pt;
	if (HitTest(&ht) == MCHT_NOWHERE)		// nowhere
	{
		::ReleaseCapture();
		GetParent()->SendMessage(WM_USER);
	}
}

void Cmonth::OnLButtonDown(UINT nFlags, CPoint point) 
{
	MCHITTESTINFO	ht;
	
	CMonthCalCtrl::OnLButtonDown(nFlags, point);
	ht.cbSize = sizeof(MCHITTESTINFO);
	ht.pt = point;
	if (HitTest(&ht) == MCHT_NOWHERE)		// nowhere
	{
		::ReleaseCapture();
		GetParent()->SendMessage(WM_USER);
	}
}

void Cmonth::OnLButtonUp(UINT nFlags, CPoint point) 
{
	MCHITTESTINFO	ht;
	
	CMonthCalCtrl::OnLButtonUp(nFlags, point);
	ht.cbSize = sizeof(MCHITTESTINFO);
	ht.pt = point;

	switch (HitTest(&ht))
	{
	case MCHT_TITLEBTNPREV:
	case MCHT_TITLEBTNNEXT:
		setDate(false, true);
		break;

	case MCHT_CALENDARDATE:
	case MCHT_CALENDARDATENEXT:
	case MCHT_CALENDARDATEPREV:
	case MCHT_TODAYLINK:
		setDate(false);
		[[fallthrough]];
	case MCHT_NOWHERE:
		::ReleaseCapture();
		GetParent()->SendMessage(WM_USER);
		break;
	default:
		break;
	}
}

void Cmonth::setDate(bool toME, bool only)
{
	CString	date;
	SYSTEMTIME time;

	if (toME)
	{
		m_edit->ReadData(date); date.Trim();
		m_date = date;
		if (m_edit->m_form->size == 6)			// YY/MM/DD
		{
			if (m_edit->m_form->attr & FA_UPPER)	// YYYY/MM
			{
				time.wYear  = atoi(date.Left(4));
				time.wMonth = atoi(date.Mid(4,2));
				time.wDay   = 1;
			}
			else
			{
				int value = atoi(date.Left(2));
				if (value >= 0 && value < 20)
					time.wYear = 2000 + value;
				else
					time.wYear = 1900 + value;
				time.wMonth = atoi(date.Mid(2,2));
				time.wDay   = atoi(date.Mid(4,2));
			}
		}
		else						// YYYY/MM/DD
		{
			time.wYear  = atoi(date.Left(4));
			time.wMonth = atoi(date.Mid(4,2));
			time.wDay   = atoi(date.Mid(6,2));
		}
		SetCurSel(&time);
	}
	else
	{
		GetCurSel(&time);
		if (m_edit->m_form->size == 6)			// YY/MM/DD
		{
			if (m_edit->m_form->attr & FA_UPPER)
				date.Format("%04d%02d", time.wYear, time.wMonth);
			else
				date.Format("%02d%02d%02d", time.wYear%100, time.wMonth, time.wDay);
		}
		else						// YYYY/MM/DD
			date.Format("%04d%02d%02d", time.wYear, time.wMonth, time.wDay);

		if (m_date.CompareNoCase(date))
		{
			m_date = date;
			if (only)
			{
				m_edit->WriteData(date, true);
			//	m_edit->IsChanged();
			}
			else
			{
				m_edit->WriteData(date, true, 0);
				if (m_edit->m_form->attr & FA_WRAP)
					GetParent()->GetParent()->PostMessage(WM_KEYDOWN, (WPARAM)VK_TAB, 0);
			}
		}
	}
}

void Cmonth::OnMouseMove(UINT nFlags, CPoint point) 
{
//	CMonthCalCtrl::OnMouseMove(nFlags, point);
}

void Cmonth::OnReleasedcapture(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	
	POINT	pt;
	MCHITTESTINFO	ht;

	::GetCursorPos(&pt);
	ScreenToClient(&pt);
	ht.cbSize = sizeof(MCHITTESTINFO);
	ht.pt = pt;
	switch (HitTest(&ht))
	{
	case MCHT_CALENDARDATE:
	case MCHT_NOWHERE:
		break;
	default:
		SetFocus();
		break;
	}
}

void Cmonth::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CMonthCalCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar == VK_RETURN)
	{
		::ReleaseCapture();
		GetParent()->SendMessage(WM_USER);
	}
}

void Cmonth::OnSelect(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	setDate(false);

// updateXXX_202202
	::ReleaseCapture();
	GetParent()->SendMessage(WM_USER);
}

void Cmonth::OnMcnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	*pResult = 0;
}

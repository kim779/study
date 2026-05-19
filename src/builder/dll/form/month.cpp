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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cmonth message handlers

BOOL Cmonth::Create(CWnd* parent, CRect rect, CfmEdit* edit)
{
	CRect	rc, cRc;

	m_edit = edit;
	if (!CMonthCalCtrl::Create(WS_CHILD|WS_BORDER|WS_VISIBLE|WS_CLIPSIBLINGS, CPoint(rect.left, rect.bottom), parent, idMonth))
		return FALSE;

	if (GetMinReqRect(&rc))
	{
		rc.SetRect(rect.right-rc.Width(), rect.bottom, rect.right, rect.bottom+rc.Height());
		if (rc.left < 0)
			rc.OffsetRect(-rc.left, 0);
		parent->GetClientRect(&cRc);
		if (rc.bottom > cRc.bottom)
			rc.OffsetRect(0, rect.top-rc.bottom);
		rc.InflateRect(2, 0);
		MoveWindow(rc);
	}
	setDate(true);
	SetFocus();
	SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	::SetCapture(GetSafeHwnd());
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
	POINT	pt;
	MCHITTESTINFO	ht;

	CMonthCalCtrl::OnKillFocus(pNewWnd);
	::GetCursorPos(&pt);
	ScreenToClient(&pt);
	ht.cbSize = sizeof(MCHITTESTINFO);
	ht.pt = pt;
	if (HitTest(&ht) == MCHT_NOWHERE)		// nowhere
	{
		::ReleaseCapture();
		PostMessage(WM_CLOSE);
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
		PostMessage(WM_CLOSE);
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
	case MCHT_CALENDARDATE:
	case MCHT_CALENDARDATENEXT:
	case MCHT_CALENDARDATEPREV:
		setDate(false);
	case MCHT_NOWHERE:
		::ReleaseCapture();
		PostMessage(WM_CLOSE);
		break;
	default:
		break;
	}
}

void Cmonth::setDate(bool toME)
{
	CString	date;
	SYSTEMTIME time;

	if (toME)
	{
		int	value;

		m_edit->ReadData(date);
		date.TrimLeft(); date.TrimRight();
		m_date = date;
		switch (m_edit->m_form->attr2 & SA_DATE)
		{
		case SA_AUTO:
			if (m_edit->m_form->size == 6)		// YY/MM/DD
			{
				value = atoi(date.Left(2));
				if (value >= 0 && value < 20)
					time.wYear = 2000 + value;
				else
					time.wYear = 1900 + value;
				time.wMonth = atoi(date.Mid(2,2));
				time.wDay   = atoi(date.Mid(4,2));
			}
			else					// YYYY/MM/DD
			{
				time.wYear  = atoi(date.Left(4));
				time.wMonth = atoi(date.Mid(4,2));
				time.wDay   = atoi(date.Mid(6,2));
			}
			break;

		case SA_YYYYMM:
			time.wYear  = atoi(date.Left(4));
			time.wMonth = atoi(date.Mid(4,2));
			time.wDay   = 1;
			break;

		case SA_YYMM:
			value = atoi(date.Left(2));
			if (value >= 0 && value < 20)
				time.wYear = 2000 + value;
			else
				time.wYear = 1900 + value;
			time.wMonth = atoi(date.Mid(2,2));
			time.wDay   = 1;
			break;

		case SA_MMDD:
			CTime	tm;
			tm = CTime::GetCurrentTime();
			time.wYear  = tm.GetYear();
			time.wMonth = atoi(date.Left(2));
			time.wDay   = atoi(date.Mid(2,2));
			break;
		}
		SetCurSel(&time);
	}
	else
	{
		GetCurSel(&time);
		switch (m_edit->m_form->attr2 & SA_DATE)
		{
		case SA_AUTO:
			if (m_edit->m_form->size == 6)		// YY/MM/DD
				date.Format("%02d%02d%02d", time.wYear%100, time.wMonth, time.wDay);
			else					// YYYY/MM/DD
				date.Format("%04d%02d%02d", time.wYear, time.wMonth, time.wDay);
			break;

		case SA_YYYYMM:
			date.Format("%04d%02d", time.wYear, time.wMonth);
			break;

		case SA_YYMM:
			date.Format("%02d%02d", time.wYear%100, time.wMonth);
			break;

		case SA_MMDD:
			date.Format("%02d%02d", time.wMonth, time.wDay);
			break;
		}

		if (m_date.CompareNoCase(date))
		{
			m_date = date;
			m_edit->WriteData(date, true, 0);
		}
	}
}

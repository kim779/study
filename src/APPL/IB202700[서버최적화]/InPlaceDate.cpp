// InPlaceDate.cpp : implementation file
//

#include "stdafx.h"
#include "InPlaceDate.h"
#include "griditem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	idDATETIME	1006

/////////////////////////////////////////////////////////////////////////////
// CInPlaceDate

CInPlaceDate::CInPlaceDate(CWnd* parent, CRect& rect, UINT nid, CString date, int row, int col, CString format)
{
	m_parent = parent; m_row = row; m_col = col;
	m_date = nullptr;

	if (!CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(nullptr, IDC_ARROW)),
		_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,
		rect.left,rect.top,rect.Width(),rect.Height(), parent->GetSafeHwnd(), (HMENU)nid, nullptr))
	{
//		TRACE("Error create date window...\n");
		return;
	}

	m_date = std::make_unique<CDateTime>(); 
	GetWindowRect(&rect); ScreenToClient(&rect);
	if (!m_date->Create(WS_CHILD|WS_VISIBLE|DTS_SHORTDATEFORMAT, rect, this, idDATETIME))
	{
//		TRACE("Error create date control...\n");
		return;
	}

	if (format.IsEmpty()) format = _T("yyyy/MM/dd");
	m_date->SetFormat(format);
	m_date->SetMonthCalColour(MCSC_MONTHBK, RGB(245, 245, 255));
	m_date->SetRange((COleDateTime*) nullptr, (COleDateTime*) nullptr);

	if (!date.IsEmpty() && date.GetLength() >= 8)
	{
		int year = atoi(date.Mid(0, 4));
		int mon  = atoi(date.Mid(4, 2));
		int day  = atoi(date.Mid(6, 2));

		const CTime time(year, mon, day, 23, 59, 59);
		m_date->SetDateTime(time);
	}
	m_date->SetFocus();
}

CInPlaceDate::~CInPlaceDate()
{
}

BEGIN_MESSAGE_MAP(CInPlaceDate, CWnd)
	//{{AFX_MSG_MAP(CInPlaceDate)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_NOTIFY(DTN_DATETIMECHANGE, idDATETIME, OnDateChange)
	ON_NOTIFY(DTN_USERSTRING,     idDATETIME, OnUserString)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceDate message handlers
void CInPlaceDate::OnDateChange(NMHDR* pNotifyStruct, LRESULT* result)
{
	const COleDateTime time = m_date->GetOleDateTime();
	CString msg; msg.Format("%d %d %d", time.GetYear(), time.GetMonth(), time.GetDay());
}

void CInPlaceDate::OnUserString(NMHDR* pNotifyStruct, LRESULT* result)
{
	NMDATETIMESTRING* dtstr = (NMDATETIMESTRING *) pNotifyStruct;
	CString string = dtstr->pszUserString;
	if (string.IsEmpty()) return;

	if (!string.CompareNoCase("KILLFOCUS")
		|| !string.CompareNoCase("VK_TAB")
		|| !string.CompareNoCase("VK_RETURN"))
	{
		EndDate();
	}
	else if (!string.CompareNoCase("VK_ESCAPE"))
	{
		PostMessage(WM_CLOSE, 0, 0);
	}
}

void CInPlaceDate::OnDestroy() 
{
	CWnd::OnDestroy();
}


void CInPlaceDate::PostNcDestroy() 
{
	CWnd::PostNcDestroy();
	delete this;
}

void CInPlaceDate::EndDate()
{
	const COleDateTime time = m_date->GetOleDateTime();
	CString string; string.Format("%04d/%02d/%02d", time.GetYear(), time.GetMonth(), time.GetDay());

	// Send Notification to parent
	NM_USER nmuser;

	nmuser.hdr.hwndFrom = GetSafeHwnd();
	nmuser.hdr.idFrom   = GetDlgCtrlID();
	nmuser.hdr.code     = GVNM_ENDDATE;

	nmuser.item.mask    = LVIF_TEXT;
	nmuser.item.row     = m_row;
	nmuser.item.col     = m_col;
	nmuser.item.text    = string;
    
	if (IsWindow(m_parent->m_hWnd))
		m_parent->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmuser);

    	PostMessage(WM_CLOSE, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CInPlaceTime

CInPlaceTime::CInPlaceTime(CWnd* parent, CRect& rect, UINT nid, CString time, int row, int col, CString format)
{
	m_parent = parent; m_row = row; m_col = col;

	if (!CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(nullptr, IDC_ARROW)),
		_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,
		rect.left,rect.top,rect.Width(),rect.Height(), parent->GetSafeHwnd(), (HMENU)nid, nullptr))
	{
//		TRACE("Error create time window...\n");
		return;
	}

	m_time = std::unique_ptr<CDateTime>(); 
	GetWindowRect(&rect); ScreenToClient(&rect);
	if (!m_time->Create(WS_CHILD|WS_VISIBLE|DTS_TIMEFORMAT|DTS_UPDOWN, rect, this, idDATETIME))
	{
//		TRACE("Error create time control...\n");
		return;
	}

	if (format.IsEmpty()) format = _T("HH:m:s");
	m_time->SetFormat(format);
	m_time->SetRange((COleDateTime*) nullptr, (COleDateTime*) nullptr);

	if (!time.IsEmpty() && time.GetLength() >= 6)
	{
		time.Remove(':');
		int hh = atoi(time.Mid(0, 2));
		int mm = atoi(time.Mid(2, 2));
		int ss = atoi(time.Mid(4, 2));

		const CTime date(2001, 2, 11, hh, mm, ss);
		m_time->SetDateTime(date);
	}
	m_time->SetFocus();
}

CInPlaceTime::~CInPlaceTime()
{
}


BEGIN_MESSAGE_MAP(CInPlaceTime, CWnd)
	//{{AFX_MSG_MAP(CInPlaceTime)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
//	ON_NOTIFY(DTN_DATETIMECHANGE, idDATETIME, OnDateChange)
	ON_NOTIFY(DTN_USERSTRING,     idDATETIME, OnUserString)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInPlaceTime message handlers
void CInPlaceTime::OnUserString(NMHDR* pNotifyStruct, LRESULT* result)
{
	NMDATETIMESTRING* dtstr = (NMDATETIMESTRING *) pNotifyStruct;
	CString string = dtstr->pszUserString;
	if (string.IsEmpty()) return;

	if (!string.CompareNoCase("KILLFOCUS"))
	{
		EndTime();
	}
	else if (!string.CompareNoCase("VK_TAB") || !string.CompareNoCase("VK_RETURN"))
	{
		EndTime();
	}
}

void CInPlaceTime::OnDestroy() 
{
	CWnd::OnDestroy();
}


void CInPlaceTime::PostNcDestroy() 
{
	CWnd::PostNcDestroy();
	delete this;
}

void CInPlaceTime::EndTime()
{
	const COleDateTime time = m_time->GetOleDateTime();
	CString string; string.Format("%02d:%02d:%02d", time.GetHour(), time.GetMinute(), time.GetSecond());

	// Send Notification to parent
	NM_USER nmuser;

	nmuser.hdr.hwndFrom = GetSafeHwnd();
	nmuser.hdr.idFrom   = GetDlgCtrlID();
	nmuser.hdr.code     = GVNM_ENDTIME;

	nmuser.item.mask    = LVIF_TEXT;
	nmuser.item.row     = m_row;
	nmuser.item.col     = m_col;
	nmuser.item.text    = string;
    
	if (IsWindow(m_parent->m_hWnd))
		m_parent->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmuser);

    	PostMessage(WM_CLOSE, 0, 0);
}
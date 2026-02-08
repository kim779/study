// InPlaceDate.cpp : implementation file
//

#include "stdafx.h"
#include "InPlaceDate.h"
#include "griditem.h"
#include "fxGrid.h"
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
	//m_date = NULL;

	if (!CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)),
		_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,
		rect.left,rect.top,rect.Width(),rect.Height(), parent->GetSafeHwnd(), (HMENU)nid, NULL))
	{
		TRACE("Error create date window...\n");
		return;
	}
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
	
}

void CInPlaceDate::OnUserString(NMHDR* pNotifyStruct, LRESULT* result)
{
	const NMDATETIMESTRING* dtstr = (NMDATETIMESTRING *) pNotifyStruct;
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
	/*
	if (m_date != NULL)
	{
		m_date->ShowWindow(SW_HIDE);
		m_date->DestroyWindow();
		delete m_date; m_date = NULL;
	}
	*/
	CWnd::OnDestroy();
}


void CInPlaceDate::PostNcDestroy() 
{
	CWnd::PostNcDestroy();
	delete this;
}

void CInPlaceDate::EndDate()
{

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CInPlaceTime

CInPlaceTime::CInPlaceTime(CWnd* parent, CRect& rect, UINT nid, CString time, int row, int col, CString format)
{
	m_parent = parent; m_row = row; m_col = col;
//	m_time = NULL;

	if (!CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)),
		_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,
		rect.left,rect.top,rect.Width(),rect.Height(), parent->GetSafeHwnd(), (HMENU)nid, NULL))
	{
		TRACE("Error create time window...\n");
		return;
	}
	
}

CInPlaceTime::~CInPlaceTime()
{
	
}


BEGIN_MESSAGE_MAP(CInPlaceTime, CWnd)
	//{{AFX_MSG_MAP(CInPlaceTime)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_NOTIFY(DTN_USERSTRING,     idDATETIME, OnUserString)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInPlaceTime message handlers
void CInPlaceTime::OnUserString(NMHDR* pNotifyStruct, LRESULT* result)
{
	const NMDATETIMESTRING* dtstr = (NMDATETIMESTRING *) pNotifyStruct;
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
	/*
	if (m_time != NULL)
	{
		m_time->ShowWindow(SW_HIDE);
		m_time->DestroyWindow();
		delete m_time; m_time = NULL;
	}
	*/
	CWnd::OnDestroy();
}


void CInPlaceTime::PostNcDestroy() 
{
	CWnd::PostNcDestroy();
	delete this;
}

void CInPlaceTime::EndTime()
{/*
	COleDateTime time = m_time->GetOleDateTime();
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
		*/
}
// DateTime.cpp : implementation file
//

#include "stdafx.h"
#include "DateTime.h"
    
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI DDX_DateTime(CDataExchange *pDX, const int nIDC, const SYSTEMTIME &systime)
{
	const HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	ASSERT (hWndCtrl != nullptr);

	if (hWndCtrl)
	{
		if (pDX->m_bSaveAndValidate)
		{
			::SendMessage(hWndCtrl, DTM_GETSYSTEMTIME, 0, (LPARAM)&systime);
		}
		else // initializing
		{
			::SendMessage(hWndCtrl, DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&systime);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDateTime

BOOL CDateTime::nCommonControlsInitialised = FALSE;

CDateTime::CDateTime()
{
	if (!nCommonControlsInitialised)
	{
		INITCOMMONCONTROLSEX icex;
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_DATE_CLASSES;
		::InitCommonControlsEx(&icex);

		nCommonControlsInitialised = TRUE;
	}

	m_CurrentTime = COleDateTime::GetCurrentTime();
	m_bDroppedDown = FALSE;
	m_bReportDateChangeWhileDropped = FALSE;
}

CDateTime::~CDateTime()
{
}

BOOL CDateTime::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	m_idCTRL = nID;
	return CWnd::Create(DATETIMEPICK_CLASS, _T(""), dwStyle, rect, pParentWnd, nID);
}

BEGIN_MESSAGE_MAP(CDateTime, CWnd)
	//{{AFX_MSG_MAP(CDateTime)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT_EX(DTN_DATETIMECHANGE, OnDateTimeChangeNotify)
	ON_NOTIFY_REFLECT_EX(DTN_DROPDOWN,       OnDropDown)
	ON_NOTIFY_REFLECT_EX(DTN_CLOSEUP,        OnCloseUp)
//	ON_NOTIFY_REFLECT_EX(DTN_FORMAT,         OnFormat)
	ON_NOTIFY_REFLECT_EX(DTN_FORMATQUERY,    OnFormatQuery)
//	ON_NOTIFY_REFLECT_EX(DTN_USERSTRING,     OnUserString)
//	ON_NOTIFY_REFLECT_EX(DTN_WMKEYDOWN,      OnKeyDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateTime (public) operations

CTime CDateTime::GetDateTime() 
{
	SYSTEMTIME systime;
	SysTimeFromCOleDateTime(m_CurrentTime, &systime);
	return CTime(systime); 
}

BOOL CDateTime::SetDateTime(const CTime& time)
{
	SYSTEMTIME systime;
	SysTimeFromCTime(time, &systime);

	m_CurrentTime = systime;
	return SetSystemTime(systime);
}

COleDateTime CDateTime::GetOleDateTime() 
{
	return m_CurrentTime; 
}

BOOL CDateTime::SetDateTime(const COleDateTime& time)
{
	m_CurrentTime = time;
	return SetSystemTime(time);
}

/////////////////////////////////////////////////////////////////////////////
// CDateTime overridable

void CDateTime::DoDateTimeChange()
{
}

/////////////////////////////////////////////////////////////////////////////
// CDateTime (protected) implementation

void CDateTime::SysTimeFromCTime(const CTime& time, SYSTEMTIME* systime)
{
	systime->wYear   = (WORD) time.GetYear();
	systime->wMonth  = (WORD) time.GetMonth();
	systime->wDay    = (WORD) time.GetDay();
	systime->wHour   = (WORD) time.GetHour();
	systime->wMinute = (WORD) time.GetMinute();
	systime->wSecond = (WORD) time.GetSecond();
	systime->wMilliseconds = 0;
}

BOOL CDateTime::SetSystemTime(const SYSTEMTIME& time)
{
	return DateTime_SetSystemtime(m_hWnd, GDT_VALID, &time);
}

BOOL CDateTime::SetSystemTime(const CTime& time)
{
	SYSTEMTIME SysTime; SysTimeFromCTime(time, &SysTime);
	return DateTime_SetSystemtime(m_hWnd, GDT_VALID, &SysTime);
}

void CDateTime::SysTimeFromCOleDateTime(const COleDateTime& time, SYSTEMTIME *systime)
{
	systime->wYear   = (WORD) time.GetYear();
	systime->wMonth  = (WORD) time.GetMonth();
	systime->wDay    = (WORD) time.GetDay();
	systime->wHour   = (WORD) time.GetHour();
	systime->wMinute = (WORD) time.GetMinute();
	systime->wSecond = (WORD) time.GetSecond();
	systime->wMilliseconds = 0;
}

BOOL CDateTime::SetSystemTime(const COleDateTime& time)
{
	SYSTEMTIME systime; SysTimeFromCOleDateTime(time, &systime);
	return DateTime_SetSystemtime(m_hWnd, GDT_VALID, &systime);
}

COleDateTime CDateTime::GetSystemTime()
{    
	SYSTEMTIME systime{};
	if (DateTime_GetSystemtime(m_hWnd, &systime) == GDT_VALID)
		return COleDateTime(systime);
	else
		return COleDateTime::GetCurrentTime();
}


BOOL CDateTime::SetFormat(LPCTSTR szFmt)
{
    return DateTime_SetFormat(m_hWnd, szFmt);
}

// Posible values for nColIndex:
//    MCSC_BACKGROUND   0   // the background color (between months)
//    MCSC_TEXT         1   // the dates
//    MCSC_TITLEBK      2   // background of the title
//    MCSC_TITLETEXT    3
//    MCSC_MONTHBK      4   // background within the month cal
//    MCSC_TRAILINGTEXT 5   // the text color of header & trailing days

COLORREF CDateTime::SetMonthCalColour(int nColIndex, COLORREF colour)
{
	return DateTime_SetMonthCalColor(m_hWnd, nColIndex, colour);
}

COLORREF CDateTime::GetMonthCalColour(int nColIndex)
{
	return DateTime_GetMonthCalColor(m_hWnd, nColIndex);
}

void CDateTime::SetMonthCalFont(const CFont& font, BOOL bRedraw /*=TRUE*/)
{
	DateTime_SetMonthCalFont(m_hWnd, (HFONT)font, bRedraw);
}

CFont* CDateTime::GetMonthCalFont()    
{
	return CFont::FromHandle((HFONT)DateTime_GetMonthCalFont(m_hWnd));
}

// Returns TRUE if success for both times, FALSE otherwise
BOOL CDateTime::SetRange(CTime* pMinTime, CTime* pMaxTime)
{
	SYSTEMTIME systimerange[2]{};
	int nWhich = 0;

	if (pMinTime)
	{
		nWhich |= GDTR_MIN;
		SysTimeFromCTime(*pMinTime, &(systimerange[0]));
	}

	if (pMaxTime)
	{
		nWhich |= GDTR_MAX;
		SysTimeFromCTime(*pMaxTime, &(systimerange[1]));
	}

	return DateTime_SetRange(m_hWnd, nWhich, systimerange);
}

// returns GDTR_MIN|GDTR_MAX if there is a minimum|maximum limit
DWORD CDateTime::GetRange(CTime* pMinTime, CTime* pMaxTime)
{
	SYSTEMTIME systimerange[2];
	ZeroMemory(systimerange, sizeof(SYSTEMTIME)*2);
	const DWORD result = DateTime_GetRange(m_hWnd, systimerange);

	*pMinTime = CTime(systimerange[0]);    // systime is 0 if no limit
	*pMaxTime = CTime(systimerange[1]);

	return result;
}

// Returns TRUE if success for both times, FALSE otherwise
BOOL CDateTime::SetRange(COleDateTime* pMinTime, COleDateTime* pMaxTime)
{
	SYSTEMTIME systimerange[2]{};

	int nWhich = 0;

	if (pMinTime)
	{
		nWhich |= GDTR_MIN;
		SysTimeFromCOleDateTime(*pMinTime, &(systimerange[0]));
	}

	if (pMaxTime)
	{
		nWhich |= GDTR_MAX;
		SysTimeFromCOleDateTime(*pMaxTime, &(systimerange[1]));
	}

	return DateTime_SetRange(m_hWnd, nWhich, systimerange);
}

// returns GDTR_MIN|GDTR_MAX if there is a minimum|maximum limit
DWORD CDateTime::GetRange(COleDateTime* pMinTime, COleDateTime* pMaxTime)
{
	SYSTEMTIME systimerange[2];
	ZeroMemory(systimerange, sizeof(SYSTEMTIME) * 2);

	const DWORD result = DateTime_GetRange(m_hWnd, systimerange);

	*pMinTime = COleDateTime(systimerange[0]);    // systime is 0 if no limit
	*pMaxTime = COleDateTime(systimerange[1]);

	return result;
}

void CDateTime::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString	string = _T("");
	NMDATETIMESTRING dtstr{};

	switch (nChar)
	{
	case VK_TAB:
		string = _T("VK_TAB"); break;
	case VK_RETURN:
		string = _T("VK_RETURN"); break;
	case VK_ESCAPE:
		string = _T("VK_ESCAPE"); break;
	default:
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	dtstr.nmhdr.hwndFrom = GetSafeHwnd();
	dtstr.nmhdr.idFrom   = m_idCTRL;
	dtstr.nmhdr.code     = DTN_USERSTRING;

	dtstr.pszUserString  = string;
	dtstr.dwFlags        = GDT_VALID;

	CWnd* pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_NOTIFY, (WPARAM) m_idCTRL, (LPARAM) &dtstr);

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDateTime::OnKillFocus(CWnd* pNewWnd)
{
	if (!m_bDroppedDown)
	{
		NMDATETIMESTRING dtstr;

		dtstr.nmhdr.hwndFrom = GetSafeHwnd();
		dtstr.nmhdr.idFrom   = m_idCTRL;
		dtstr.nmhdr.code     = DTN_USERSTRING;

		dtstr.pszUserString  = _T("KILLFOCUS");
		dtstr.dwFlags        = GDT_VALID;

		CWnd* pOwner = GetOwner();
		if (pOwner && IsWindow(pOwner->m_hWnd))
			pOwner->SendMessage(WM_NOTIFY, (WPARAM) m_idCTRL, (LPARAM) &dtstr);
	}

	CWnd::OnKillFocus(pNewWnd);
}

/////////////////////////////////////////////////////////////////////////////
// CDateTime notification handlers

BOOL CDateTime::OnDropDown(NMHDR* /*pNotifyStruct*/, LRESULT* /*result*/)
{
	m_bDroppedDown = TRUE;
	return FALSE;    // Let parent handle message
}

BOOL CDateTime::OnCloseUp(NMHDR* /*pNotifyStruct*/, LRESULT* /*result*/)
{
	m_bDroppedDown = FALSE;

	if (!m_bReportDateChangeWhileDropped)
	{
		NMDATETIMECHANGE dtmh;

		dtmh.nmhdr.hwndFrom = GetSafeHwnd(); 
		dtmh.nmhdr.idFrom = m_idCTRL; 
		dtmh.nmhdr.code = DTN_DATETIMECHANGE; 
		dtmh.dwFlags = DateTime_GetSystemtime(GetSafeHwnd(), &(dtmh.st));

		CWnd* pOwner = GetOwner();
		if (pOwner && IsWindow(pOwner->m_hWnd) && GetSystemTime() != m_CurrentTime)
			pOwner->SendMessage(WM_NOTIFY, (WPARAM) m_idCTRL, (LPARAM) &dtmh);
		SetFocus();
	}

	return FALSE;
}

BOOL CDateTime::OnDateTimeChangeNotify(NMHDR* hdr, LRESULT* pResult)
//(NMDATETIMECHANGE *dtmh, LRESULT* pResult)
{
	*pResult = 0;    // docs say we must return 0

	if (!m_bReportDateChangeWhileDropped && m_bDroppedDown)
		return TRUE;
	const NMDATETIMECHANGE* dtmh = (NMDATETIMECHANGE*) hdr;
	// Has the date changed?
	BOOL bDateChanged = FALSE;
	if (dtmh->dwFlags == GDT_VALID)
	{
		COleDateTime time;

		time = GetSystemTime();
		if (time != m_CurrentTime)
		{
			m_CurrentTime = time;
			bDateChanged = TRUE;
			DoDateTimeChange();
		}
	}

	const BOOL bUpDownStyle = ((GetStyle() & DTS_UPDOWN) == DTS_UPDOWN);
	if (bUpDownStyle) return FALSE;

	if (m_bDroppedDown && !bDateChanged)
		return FALSE;

	return FALSE;
}

//BOOL CDateTime::OnFormat(NMDATETIMEFORMAT* /*lpNMFormat*/, LRESULT* pResult)
//{
//	*pResult = 0; 
//	return FALSE; 
//}

BOOL CDateTime::OnFormatQuery(NMHDR* hdr, LRESULT* pResult)
//(NMDATETIMEFORMATQUERY* lpDTFormatQuery, LRESULT* pResult)
{
	*pResult = 0; 
	NMDATETIMEFORMATQUERY* lpDTFormatQuery = (NMDATETIMEFORMATQUERY*) hdr;
	lpDTFormatQuery->szMax = CSize(366,16);
	return FALSE;
}

//BOOL CDateTime::OnUserString(NMDATETIMESTRING* /*lpDTstring*/, LRESULT* pResult)
//{
//	*pResult = 0;
//	return FALSE;
//}

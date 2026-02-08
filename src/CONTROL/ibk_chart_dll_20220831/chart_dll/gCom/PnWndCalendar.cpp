// PnWndCalendar.cpp : implementation file
//

#include "stdafx.h"
#include "AxisGCom.h"
#include "PnWndCalendar.h"
#include "commctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable: 26454)

/////////////////////////////////////////////////////////////////////////////
// CPnWndCalendar

void AFXAPI DDX_DateTime(CDataExchange *pDX, int nIDC, SYSTEMTIME &SysTime)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	ASSERT (hWndCtrl != NULL);

	if (pDX->m_bSaveAndValidate)
		::SendMessage(hWndCtrl, DTM_GETSYSTEMTIME, 0, (LPARAM)&SysTime);
	else // initializing
		::SendMessage(hWndCtrl, DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&SysTime);
}

BOOL CPnWndCalendar::nCommonControlsInitialised = FALSE;

CPnWndCalendar::CPnWndCalendar()
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

CPnWndCalendar::~CPnWndCalendar()
{
}

BOOL CPnWndCalendar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	return CWnd::Create(DATETIMEPICK_CLASS, _T(""), dwStyle, rect, pParentWnd, nID);
}

void CPnWndCalendar::ResizeCalendar(CRect rc)
{
	SetWindowPos(NULL, rc.left, rc.top,
		rc.Width(), rc.Height(), SWP_NOZORDER);
}

BEGIN_MESSAGE_MAP(CPnWndCalendar, CWnd)
	//{{AFX_MSG_MAP(CPnWndCalendar)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT_EX(DTN_DATETIMECHANGE, &CPnWndCalendar::OnDateTimeChangeNotify)
	ON_NOTIFY_REFLECT_EX(DTN_DROPDOWN, &CPnWndCalendar::OnDropDown)
	ON_NOTIFY_REFLECT_EX(DTN_CLOSEUP, &CPnWndCalendar::OnCloseUp)
	ON_NOTIFY_REFLECT_EX(DTN_FORMATQUERY, &CPnWndCalendar::OnFormatQuery)
END_MESSAGE_MAP()

CTime CPnWndCalendar::GetDateTime() 
{
	SYSTEMTIME SysTime;
	SysTimeFromCOleDateTime(m_CurrentTime, &SysTime);

	return CTime(SysTime); 
}

BOOL CPnWndCalendar::SetDateTime(const CTime& time)
{
	SYSTEMTIME SysTime;
	SysTimeFromCTime(time, &SysTime);
	m_CurrentTime = SysTime;

	return SetSystemTime(SysTime);
}

COleDateTime CPnWndCalendar::GetOleDateTime() 
{
	return m_CurrentTime; 
}

BOOL CPnWndCalendar::SetDateTime(const COleDateTime& time)
{
	m_CurrentTime = time;

	return SetSystemTime(time);
}


void CPnWndCalendar::DoDateTimeChange()
{
	GetOwner()->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipDate), 0);
}


void CPnWndCalendar::SysTimeFromCTime(const CTime& time, SYSTEMTIME *SysTime)
{
	SysTime->wYear   = (WORD) time.GetYear();
	SysTime->wMonth  = (WORD) time.GetMonth();
	SysTime->wDay    = (WORD) time.GetDay();
	SysTime->wHour   = (WORD) time.GetHour();
	SysTime->wMinute = (WORD) time.GetMinute();
	SysTime->wSecond = (WORD) time.GetSecond();
	SysTime->wMilliseconds = 0;
}

BOOL CPnWndCalendar::SetSystemTime(const SYSTEMTIME& time)
{
	return DateTime_SetSystemtime(m_hWnd, GDT_VALID, &time);
}

BOOL CPnWndCalendar::SetSystemTime(const CTime& time)
{
	SYSTEMTIME SysTime;
	SysTimeFromCTime(time, &SysTime);

	return DateTime_SetSystemtime(m_hWnd, GDT_VALID, &SysTime);
}

void CPnWndCalendar::SysTimeFromCOleDateTime(const COleDateTime& time, SYSTEMTIME *SysTime)
{
	SysTime->wYear   = (WORD) time.GetYear();
	SysTime->wMonth  = (WORD) time.GetMonth();
	SysTime->wDay    = (WORD) time.GetDay();
	SysTime->wHour   = (WORD) time.GetHour();
	SysTime->wMinute = (WORD) time.GetMinute();
	SysTime->wSecond = (WORD) time.GetSecond();
	SysTime->wMilliseconds = 0;
}

BOOL CPnWndCalendar::SetSystemTime(const COleDateTime& time)
{
	SYSTEMTIME SysTime;
	SysTimeFromCOleDateTime(time, &SysTime);

	return DateTime_SetSystemtime(m_hWnd, GDT_VALID, &SysTime);
}

COleDateTime CPnWndCalendar::GetSystemTime()
{    
	SYSTEMTIME SysTime;
	if (DateTime_GetSystemtime(m_hWnd, &SysTime) == GDT_VALID)
		return COleDateTime(SysTime);
	else
		return COleDateTime::GetCurrentTime();
}

BOOL CPnWndCalendar::SetFormat(LPCTSTR szFmt)
{
	return DateTime_SetFormat(m_hWnd, szFmt);
}

// Posible values for nColIndex:
//    MCSC_BACKGROUND   0 - the background color (between months)
//    MCSC_TEXT         1 - the dates
//    MCSC_TITLEBK      2 - background of the title
//    MCSC_TITLETEXT    3
//    MCSC_MONTHBK      4 - background within the month cal
//    MCSC_TRAILINGTEXT 5 - the text color of header & trailing days
COLORREF CPnWndCalendar::SetMonthCalColour(int nColIndex, COLORREF colour)
{
	return DateTime_SetMonthCalColor(m_hWnd, nColIndex, colour);
}

COLORREF CPnWndCalendar::GetMonthCalColour(int nColIndex)
{
	return DateTime_GetMonthCalColor(m_hWnd, nColIndex);
}

void CPnWndCalendar::SetMonthCalFont(CFont& font, BOOL bRedraw /*=TRUE*/)
{
	DateTime_SetMonthCalFont(m_hWnd, (HFONT)font, bRedraw);
}

CFont* CPnWndCalendar::GetMonthCalFont()    
{
	return CFont::FromHandle( (HFONT)DateTime_GetMonthCalFont(m_hWnd) );
}

BOOL CPnWndCalendar::SetRange(CTime* pMinTime, CTime* pMaxTime)
{
	SYSTEMTIME SysTimeRange[2];
	int nWhich = 0;

	if (pMinTime) 
	{
		nWhich |= GDTR_MIN;
		SysTimeFromCTime( *pMinTime, &(SysTimeRange[0]) );
	}
	if (pMaxTime) 
	{
		nWhich |= GDTR_MAX;
		SysTimeFromCTime( *pMaxTime, &(SysTimeRange[1]) );
	}

	return DateTime_SetRange(m_hWnd, nWhich, SysTimeRange);
}

DWORD CPnWndCalendar::GetRange(CTime* pMinTime, CTime* pMaxTime)
{
	SYSTEMTIME SysTimeRange[2];
	DWORD result = DateTime_GetRange(m_hWnd, SysTimeRange);

	*pMinTime = CTime(SysTimeRange[0]);
	*pMaxTime = CTime(SysTimeRange[1]);

	return result;
}

BOOL CPnWndCalendar::SetRange(COleDateTime* pMinTime, COleDateTime* pMaxTime)
{
	SYSTEMTIME SysTimeRange[2];
	int nWhich = 0;

	if (pMinTime) 
	{
		nWhich |= GDTR_MIN;
		SysTimeFromCOleDateTime( *pMinTime, &(SysTimeRange[0]) );
	}
	if (pMaxTime) 
	{
		nWhich |= GDTR_MAX;
		SysTimeFromCOleDateTime( *pMaxTime, &(SysTimeRange[1]) );
	}

	return DateTime_SetRange(m_hWnd, nWhich, SysTimeRange);
}

DWORD CPnWndCalendar::GetRange(COleDateTime* pMinTime, COleDateTime* pMaxTime)
{
	SYSTEMTIME SysTimeRange[2];
	DWORD result = DateTime_GetRange(m_hWnd, SysTimeRange);

	*pMinTime = COleDateTime(SysTimeRange[0]);
	*pMaxTime = COleDateTime(SysTimeRange[1]);

	return result;
}

BOOL CPnWndCalendar::OnDropDown(NMHDR* /*pNotifyStruct*/, LRESULT* /*result*/)
{
	m_bDroppedDown = TRUE;
	return FALSE;
}

BOOL CPnWndCalendar::OnCloseUp(NMHDR* /*pNotifyStruct*/, LRESULT* /*result*/)
{
	m_bDroppedDown = FALSE;
	if (!m_bReportDateChangeWhileDropped)
	{
		NMDATETIMECHANGE dtmh;
		dtmh.nmhdr.hwndFrom = GetSafeHwnd(); 
		dtmh.nmhdr.idFrom = GetDlgCtrlID(); 
		dtmh.nmhdr.code = DTN_DATETIMECHANGE; 
		dtmh.dwFlags = DateTime_GetSystemtime(GetSafeHwnd(), &(dtmh.st));

		CWnd* pOwner = GetOwner();
		if (pOwner && IsWindow(pOwner->m_hWnd) && GetSystemTime() != m_CurrentTime)
			pOwner->SendMessage(WM_NOTIFY, (WPARAM) GetDlgCtrlID(), (LPARAM) &dtmh);
	}

	return FALSE;
}

// updateXXX_202204_CHECK
//BOOL CPnWndCalendar::OnDateTimeChangeNotify(NMDATETIMECHANGE *dtmh, LRESULT* pResult)
BOOL CPnWndCalendar::OnDateTimeChangeNotify(NMHDR* pNotifyStruct, LRESULT* result)
{
	*result = 0;

//	NMDATETIMECHANGE* dtmh = dynamic_cast <NMDATETIMECHANGE*> (pNotifyStruct);
	NMDATETIMECHANGE* dtmh = (NMDATETIMECHANGE*)pNotifyStruct;

	if (!m_bReportDateChangeWhileDropped && m_bDroppedDown)
		return TRUE;

	if (dtmh->dwFlags == GDT_NONE);	//control Disabled
	else ;	//control Active

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

	BOOL bUpDownStyle = ((GetStyle() & DTS_UPDOWN) == DTS_UPDOWN);
	if (bUpDownStyle)	return FALSE;

	if (m_bDroppedDown)
	{
		if (bDateChanged)
			return TRUE;
		else 
			return FALSE;
	}

	return FALSE;
}

//BOOL CPnWndCalendar::OnFormatQuery(NMDATETIMEFORMATQUERY* lpDTFormatQuery, LRESULT* pResult)
BOOL CPnWndCalendar::OnFormatQuery(NMHDR* pNotifyStruct, LRESULT* result)
{
	*result = 0;

	NMDATETIMEFORMATQUERY* lpDTFormatQuery = (NMDATETIMEFORMATQUERY*)pNotifyStruct;
	lpDTFormatQuery->szMax = CSize(366,16);
	return FALSE;
}

void CPnWndCalendar::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_RETURN)
	{
		GetOwner()->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipDate), 0);
	}
	else if (nChar == VK_TAB)
		GetOwner()->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipTab), GetDlgCtrlID());

	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

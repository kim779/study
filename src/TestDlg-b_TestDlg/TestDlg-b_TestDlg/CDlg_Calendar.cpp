// CDlg_Calendar.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlg_Calendar.h"
#include "afxdialogex.h"


//#include <Lm.h>
//#pragma comment(lib, "netapi32.lib")
//#include <stdio.h>
//#include <shlobj_core.h>
//#include <TlHelp32.h>
//#include <vadefs.h>
//#include <Windows.h>
//#include <VersionHelpers.h>  // Windows 버전 확인을 위한 헤더 파일
//#include <Psapi.h>
//#include <minwindef.h>
//#include <afxinet.h>
//
//#include <afxwin.h>
//#include <mmsystem.h>
//#pragma comment(lib, "winmm.lib")

// CDlg_Calendar 대화 상자
#pragma warning (disable : 4996 )  //이걸 하면 deprecated  컴파일 에러를 무시해준다
IMPLEMENT_DYNAMIC(CDlg_Calendar, CDialogEx)

CDlg_Calendar::CDlg_Calendar(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_CALENDAR, pParent)
{

}

CDlg_Calendar::~CDlg_Calendar()
{
}

void CDlg_Calendar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Calendar, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CAL2, &CDlg_Calendar::OnBnClickedBtnCal2)
	ON_BN_CLICKED(IDC_BTN_CAL1, &CDlg_Calendar::OnBnClickedBtnCal1)
	ON_BN_CLICKED(IDC_BTN_CAL3, &CDlg_Calendar::OnBnClickedBtnCal3)
END_MESSAGE_MAP()


// CDlg_Calendar 메시지 처리기



void CDlg_Calendar::OnBnClickedBtnCal1()
{
	CPoint curpt;
	GetCursorPos(&curpt);
	CRect calrect;
	((CWnd*)GetDlgItem(IDC_STATIC_CAL))->GetWindowRect(calrect);
	ScreenToClient(calrect);
		
	m_calendarCtrl.Create(WS_BORDER | WS_TABSTOP | WS_VISIBLE | WS_CHILD | MCS_MULTISELECT | MCS_NOTODAYCIRCLE, calrect, this, 9898);
	//m_calendarCtrl.Create(WS_BORDER | WS_TABSTOP | WS_VISIBLE | WS_CHILD | MCS_MULTISELECT | MCS_NOTODAYCIRCLE, CPoint(calrect.left, calrect.top), this, 9898);

	// 달력의 초기 날짜를 설정합니다.
	COleDateTime currentDate = COleDateTime::GetCurrentTime();
	m_calendarCtrl.SetCurSel(currentDate);
}


void CDlg_Calendar::AdjustMonthCalPosition(RECT* pRect)
{
	/* test comment
	//CRect rectCell;
	//m_ctrl->GetCellRect(m_ctrl->GetCurrentCol(), m_ctrl->GetCurrentRow(), rectCell);

	//const BOOL bTopMostParent = (m_ctrl->GetTopLevelParent() != nullptr &&
	//	m_ctrl->GetTopLevelParent()->GetExStyle() & WS_EX_TOPMOST);
	*/

	const BOOL bTopMostParent = TRUE;
	CRect rectDisplay;
#if(WINVER >= 0x0500)
	// if we use Win 98/NT5 we have to take into account multiple monitors
	OSVERSIONINFO osvi = { sizeof(OSVERSIONINFO) };
	VERIFY(::GetVersionEx(&osvi) != 0);
	if (osvi.dwMajorVersion > 4 ||
		(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion > 0))
	{
		CPoint pt;
		::GetCursorPos(&pt);
		HMONITOR hMonitor = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
		ASSERT(hMonitor != nullptr);
		MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
		VERIFY(::GetMonitorInfo(hMonitor, &monitorInfo));
		if (bTopMostParent)
			rectDisplay = monitorInfo.rcMonitor;
		else
			rectDisplay = monitorInfo.rcWork;
	}
	else
	{
#endif
		if (bTopMostParent)
			CWnd::GetDesktopWindow()->GetWindowRect(rectDisplay);
		else
			::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectDisplay, 0);
#if(WINVER >= 0x0500)
	}
#endif

	CRect rectIntersect;
	if (rectIntersect.IntersectRect(rectDisplay, pRect) &&
		rectIntersect == *pRect)
	{
		return;
	}

	if (pRect->right > rectDisplay.right)
	{
		pRect->left -= pRect->right - rectDisplay.right;
		pRect->right = rectDisplay.right;
	}
	if (pRect->left < rectDisplay.left)
	{
		pRect->right += rectDisplay.left - pRect->left;
		pRect->left = rectDisplay.left;
	}
	if (pRect->bottom > rectDisplay.bottom)
	{
		pRect->top -= pRect->bottom - rectDisplay.bottom;
		pRect->bottom = rectDisplay.bottom;
	}
	if (pRect->top < rectDisplay.top)
	{
		pRect->bottom += rectDisplay.top - pRect->top;
		pRect->top = rectDisplay.top;
	}
}
typedef struct UG_CLASS_DECL_tag
{
	int nFlags{};
	int nFirstDayOfWeek{};
	int nScrollRate{};
	CSize szDimension;
	LOGFONT lf{};
	COLORREF clrBk{};
	COLORREF clrText{};
	COLORREF clrMonthBk{};
	COLORREF clrTitleText{};
	COLORREF clrTitleBk{};
	COLORREF clrTrailingText{};
	COleDateTime dateCur{};
}UGCTMONTHCALSETTINGS;

void CDlg_Calendar::OnBnClickedBtnCal2()
{
	COleDateTime date;
	CString sText;
	date = COleDateTime::GetCurrentTime();

	CPoint _pt;
	GetCursorPos(&_pt);
	CRect rectCell;
	rectCell.left = _pt.x;
	rectCell.bottom = _pt.y;
	
	CRect calrect;
	((CWnd*)GetDlgItem(IDC_STATIC_CAL))->GetWindowRect(calrect);
	ScreenToClient(calrect);

	if (!m_calendarCtrl.CreateEx(WS_EX_DLGMODALFRAME, MONTHCAL_CLASS, _T(""), WS_POPUP,
		calrect,
		this, 0, nullptr))
	{
		return;
	}

	//// populate structure with default values
	//UGCTMONTHCALSETTINGS mcs;
	//mcs.clrBk = wndMonthCal.GetColor(MCSC_BACKGROUND);
	//mcs.clrText = wndMonthCal.GetColor(MCSC_TEXT);
	//mcs.clrMonthBk = wndMonthCal.GetColor(MCSC_MONTHBK);
	//mcs.clrTitleText = wndMonthCal.GetColor(MCSC_TITLETEXT);
	//mcs.clrTitleBk = wndMonthCal.GetColor(MCSC_TITLEBK);
	//mcs.clrTrailingText = wndMonthCal.GetColor(MCSC_TRAILINGTEXT);
	//[[gsl::suppress(6387)]]
	//mcs.nFirstDayOfWeek = wndMonthCal.GetFirstDayOfWeek();
	//mcs.nFlags = wndMonthCal.GetStyle() & (MCS_WEEKNUMBERS | MCS_NOTODAYCIRCLE | MCS_NOTODAY);
	//mcs.nScrollRate = wndMonthCal.GetMonthDelta();
	//mcs.szDimension = CSize(1, 1);
	//mcs.dateCur = date;
	//CFont* pFont = wndMonthCal.GetFont();
	//ASSERT(pFont != nullptr);
	//VERIFY(pFont->GetLogFont(&mcs.lf));

	//// check if this data is wanted to be customized
	////OnCellTypeNotify(m_ID, m_ctrl->GetCurrentCol(),
	////	m_ctrl->GetCurrentRow(), UGCT_DISPLAYMONTHCAL, (LPARAM)&mcs);
	//wndMonthCal.SetColor(MCSC_BACKGROUND, mcs.clrBk);
	//wndMonthCal.SetColor(MCSC_TEXT, mcs.clrText);
	//wndMonthCal.SetColor(MCSC_MONTHBK, mcs.clrMonthBk);
	//wndMonthCal.SetColor(MCSC_TITLETEXT, mcs.clrTitleText);
	//wndMonthCal.SetColor(MCSC_TITLEBK, mcs.clrTitleBk);
	//wndMonthCal.SetColor(MCSC_TRAILINGTEXT, mcs.clrTrailingText);
	//[[gsl::suppress(6387)]]
	//wndMonthCal.SetFirstDayOfWeek(mcs.nFirstDayOfWeek);
	//wndMonthCal.
	//	ModifyStyle((MCS_WEEKNUMBERS | MCS_NOTODAYCIRCLE | MCS_NOTODAY), mcs.nFlags);
	//wndMonthCal.SetMonthDelta(mcs.nScrollRate);
	//CFont font;
	//VERIFY(font.CreateFontIndirect(&mcs.lf));
	//wndMonthCal.SetFont(&font);

	////SetMonthCalDimension(&wndMonthCal, mcs.szDimension);
	//CRect rect;
	//wndMonthCal.GetWindowRect(rect);
	//AdjustMonthCalPosition(rect);

	//rect.SetRect(0, 0, 300, 200);

	//wndMonthCal.SetWindowPos(nullptr, rect.left, rect.top, rect.Width(), rect.Height(),
	//	SWP_NOZORDER);

	//wndMonthCal.SetCurSel(mcs.dateCur);

	//ReleaseCapture();
	//wndMonthCal.ShowWindow(SW_SHOWNA);
	//wndMonthCal.SetFocus();

}

/*
if (0)
	{
		int iret = wndMonthCal.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(10, 10, 200, 200), this, 1000);
		if (!iret)
			AfxMessageBox("create calender fail");
		else
		{
			CTime time = GetCurrentTime();
			wndMonthCal.SetToday(&time);
			wndMonthCal.ShowWindow(SW_SHOW);
			//wndMonthCal.SetFocus();
			//wndMonthCal.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}
	}
	else
	{

	}
*/




BOOL CDlg_Calendar::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMHDR* pnmh = (NMHDR*)lParam;
	switch (pnmh->idFrom)
	{
		case 9898:
		{
			NMSELCHANGE* pSelChange = reinterpret_cast<NMSELCHANGE*>(pnmh);
		}
		break;
	}
	return CDialogEx::OnNotify(wParam, lParam, pResult);
}

#include "monthWnd.h"
void CDlg_Calendar::OnBnClickedBtnCal3()
{
	CmonthWnd* _clcalenderwnd;
	CRect calrect;
	((CWnd*)GetDlgItem(IDC_STATIC_CAL))->GetWindowRect(calrect);
	ScreenToClient(calrect);

	_clcalenderwnd = new CmonthWnd;
	if (!_clcalenderwnd->Create(this, calrect, this))
		return;
}

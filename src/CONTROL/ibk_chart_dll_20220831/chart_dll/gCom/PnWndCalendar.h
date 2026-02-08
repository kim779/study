#if !defined(AFX_PNWNDCALENDAR_H__753F12EF_6F54_4144_8897_3EDE44491CA3__INCLUDED_)
#define AFX_PNWNDCALENDAR_H__753F12EF_6F54_4144_8897_3EDE44491CA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnWndCalendar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPnWndCalendar window

class CPnWndCalendar : public CWnd
{
// Construction
public:
	CPnWndCalendar();
	virtual ~CPnWndCalendar();

	//{{AFX_VIRTUAL(CPnWndCalendar)
	//}}AFX_VIRTUAL
	virtual void DoDateTimeChange();

protected:
	//{{AFX_MSG(CPnWndCalendar)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
//	BOOL OnDateTimeChangeNotify(LPNMDATETIMECHANGE dtmh, LRESULT* result);
	virtual BOOL OnDateTimeChangeNotify(NMHDR* /*pNotifyStruct*/, LRESULT* /*result*/);
	virtual BOOL OnDropDown(NMHDR * pNotifyStruct, LRESULT* result);
	virtual BOOL OnCloseUp(NMHDR * pNotifyStruct, LRESULT* result);
//	BOOL OnFormatQuery(NMDATETIMEFORMATQUERY* lpNMFormat, LRESULT* pResult);
	BOOL OnFormatQuery(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()

protected:
	COleDateTime m_CurrentTime;
	static BOOL nCommonControlsInitialised;
	BOOL   m_bDroppedDown;
	BOOL   m_bReportDateChangeWhileDropped;

public:
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void ResizeCalendar(CRect rc);

	CTime GetDateTime();
	BOOL  SetDateTime(const CTime& time);
	COleDateTime GetOleDateTime();
	BOOL  SetDateTime(const COleDateTime& time);

	COLORREF SetMonthCalColour(int nColIndex, COLORREF colour);
	COLORREF GetMonthCalColour(int nColIndex);
	BOOL     SetFormat(LPCTSTR szFmt);
	void     SetMonthCalFont(CFont& font, BOOL bRedraw = TRUE);
	CFont*   GetMonthCalFont();
	BOOL     SetRange(CTime* pMinTime, CTime* pMaxTime);
	DWORD    GetRange(CTime* pMinTime, CTime* pMaxTime);
	BOOL     SetRange(COleDateTime* pMinTime, COleDateTime* pMaxTime);
	DWORD    GetRange(COleDateTime* pMinTime, COleDateTime* pMaxTime);

protected:
	void  SysTimeFromCTime(const CTime& time, SYSTEMTIME *SysTime);
	BOOL  SetSystemTime(const SYSTEMTIME& time);
	BOOL  SetSystemTime(const CTime& time);

	void  SysTimeFromCOleDateTime(const COleDateTime& time, SYSTEMTIME *SysTime);
	BOOL  SetSystemTime(const COleDateTime& time);

	COleDateTime GetSystemTime();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNWNDCALENDAR_H__753F12EF_6F54_4144_8897_3EDE44491CA3__INCLUDED_)

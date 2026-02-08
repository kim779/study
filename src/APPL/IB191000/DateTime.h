#pragma once

void AFXAPI DDX_DateTime(CDataExchange *pDX, int nIDC, SYSTEMTIME &systime);
/////////////////////////////////////////////////////////////////////////////
// CDateTime window

class CDateTime : public CWnd
{
// Construction 
public:
	CDateTime();
	virtual ~CDateTime();

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

// Operations
public:
	CTime	GetDateTime();
	BOOL	SetDateTime(const CTime& time);
	COleDateTime GetOleDateTime();
	BOOL	SetDateTime(const COleDateTime& time);

// Attributes
public:
	COLORREF SetMonthCalColour(int nColIndex, COLORREF colour);
	COLORREF GetMonthCalColour(int nColIndex);
	BOOL	SetFormat(LPCTSTR szFmt);
	void	SetMonthCalFont(CFont& font, BOOL bRedraw = TRUE);
	CFont*	GetMonthCalFont();
	BOOL	SetRange(CTime* pMinTime, CTime* pMaxTime);
	DWORD	GetRange(CTime* pMinTime, CTime* pMaxTime);
	BOOL	SetRange(COleDateTime* pMinTime, COleDateTime* pMaxTime);
	DWORD	GetRange(COleDateTime* pMinTime, COleDateTime* pMaxTime);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDateTime)
    //}}AFX_VIRTUAL
    virtual void DoDateTimeChange();

// Implementation
protected:
	void	SysTimeFromCTime(const CTime& time, SYSTEMTIME *SysTime);
	BOOL	SetSystemTime(const SYSTEMTIME& time);
	BOOL	SetSystemTime(const CTime& time);

	void	SysTimeFromCOleDateTime(const COleDateTime& time, SYSTEMTIME *SysTime);
	BOOL	SetSystemTime(const COleDateTime& time);
	COleDateTime GetSystemTime();

protected:
	COleDateTime m_CurrentTime;
	static	BOOL nCommonControlsInitialised;
	BOOL	m_bDroppedDown;
	BOOL	m_bReportDateChangeWhileDropped;

	UINT	m_idCTRL;
	// Generated message map functions
protected:
	//{{AFX_MSG(CDateTime)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	virtual BOOL OnDateTimeChangeNotify(LPNMDATETIMECHANGE dtmh, LRESULT* result);
	virtual BOOL OnDropDown(NMHDR * pNotifyStruct, LRESULT* result);
	virtual BOOL OnCloseUp(NMHDR * pNotifyStruct, LRESULT* result);
	virtual BOOL OnFormat(NMDATETIMEFORMAT* lpNMFormat, LRESULT* pResult);
	virtual BOOL OnFormatQuery(NMDATETIMEFORMATQUERY* lpNMFormat, LRESULT* pResult);
	virtual BOOL OnUserString(NMDATETIMESTRING* lpDTstring, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};


#if !defined(AFX_NCLOCK_H__5EFC6009_609E_4BC4_BC23_155930D601D1__INCLUDED_)
#define AFX_NCLOCK_H__5EFC6009_609E_4BC4_BC23_155930D601D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NClock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNClock window

class CNClock : public CWnd
{
// Construction
public:
	CNClock();
	virtual ~CNClock();

	void	Init();
	BOOL	IsMini() { return m_bSmall; }
	void	SetMini(BOOL bMini = FALSE);
	void	SetTrace(BOOL bTrace);
// Attributes
protected:
	CFont	m_font;
	CFont	m_fontMini, m_fontMiniBold;
	BOOL	m_bSmall;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNClock)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CNClock)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBitmap* m_bmpBackground;
	CBitmap* m_bmpBackgroundMini;
	CBitmap* m_bmpAMPM[2];	  // 0:AM 1:PM
	CBitmap* m_bmpWeekday[7]; // 0: sunday~6: saturday
	CBitmap m_back;
	DWORD m_lastLButtonDown;

	BOOL m_bOverCalendar;
	CBitmap* m_bmpCalendar[2];

	int m_currentAMPM;
	int m_currentWeekday;
	BOOL m_trace;		// 따라다니기 옵션
	CRect m_rcMain;

	int  GetAMPM(int hour);
	void UpdateBackground(CDC* );

	void Draw(CDC& dc);
	void DrawImg(CDC* dc, CBitmap* bmp, int x = 0, int y = 0);
	void DrawClock(CDC* pDC);
	void DrawClock(CDC* pDC, CString& time);
	void DrawClockMini(CDC* pDC, CString& time);
	void SetWindowRegion();

	void CreateBoldFont(CFont& font, int point, const char* fontName);
	void Load();
	void Save();
public:
	void OnMainWndMoved();
};

#define CLOCK_AM	0
#define CLOCK_PM	1

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NCLOCK_H__5EFC6009_609E_4BC4_BC23_155930D601D1__INCLUDED_)

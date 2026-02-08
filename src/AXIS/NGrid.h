#if !defined(AFX_NGRID_H__90C16D11_DEA1_45E8_B213_AF06551001B3__INCLUDED_)
#define AFX_NGRID_H__90C16D11_DEA1_45E8_B213_AF06551001B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NGrid.h : header file
//
#include <afxcoll.h>

#define	CELLHEIGHT	20
#define	VSCROLLW	16

#define	KIND_CONCLUSION	0
#define	KIND_NEWS	1
#define	KIND_RTS	2

#define	WM_NEWSMSG	WM_USER+100
#define	NEWS_CLICK	1
#define	NEWS_DBCLICK	2
class  CColumn
{
public:
	CColumn();
	~CColumn();

	bool		m_show;
	CRect		m_hRc;
	CString		m_htext;
	CStringArray	m_list;
};

/////////////////////////////////////////////////////////////////////////////
// CNGrid window
class CNGrid : public CWnd
{
// Construction
public:
	CNGrid(CWnd* parent, int viewN, int kind = 0);
	virtual ~CNGrid();

// Attributes
protected:
	CWnd*		m_parent;
	CFont		m_font;
	CScrollBar	m_vbar;

	int		m_kind;
	int		m_col;		// grid column count
	int		m_viewN;	// visible row count
	int		m_selectN;	// select low number

//	COLORREF	m_bkColor;	// background color
	COLORREF	m_hColor;	// grid head color
	COLORREF	m_fcColor;	// grid first cell color
	COLORREF	m_scColor;	// grid second cell color
	COLORREF	m_lColor;	// grid line color
	COLORREF	m_tColor;	// grid text color
	COLORREF	m_sColor;	// grid select line color
	
	CStringArray	m_jnolist;
	CMap	< int, int, class CColumn*, class CColumn * >	m_clist;

	bool		m_bReal;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL	AddData(CString dat = _T(""), int max = -1, bool bConclusion = true);
	BOOL	AddFail(CString dat, int max);
	void	AddNews(CString dat);
	void	AddRts(CString dat);
	// scrollbar function
	void	CreateScroll();
	void	SetScroll(int nPos = -1);

	void	LineUp();
	void	LineDown();
	void	PageUp();
	void	PageDown();
	void	ThumbTrack(UINT nPos);
	void	MouseWheel(UINT nFlags, short zDelta, CPoint pt);
	// color setting function
//	void	SetBackColor(COLORREF color)	{ m_bkColor = color; }
	void	SetHeadColor(COLORREF color)	{ m_hColor = color; }
	void	SetCellColor1(COLORREF color)	{ m_fcColor = color; }
	void	SetCellColor2(COLORREF color)	{ m_scColor = color; }
	void	SetLineColor(COLORREF color)	{ m_lColor = color; }
	void	SetTextColor(COLORREF color)	{ m_tColor = color; }
	// grid setting function
	void	SetTitle(const CStringArray& ary);
	void	SetColumnWidth(const CStringArray& ary, bool fit = true);
	void	SetColumnWidth(int, int);
	// etc
	CRect	GetCellRect(int, int);
//	CFont*	GetFont(char* name, int point, bool italic = false, int bold = FW_NORMAL);
	void	ChangeFont(int point, CString name);
	void	ChangePalette();
	void	SetColor();
	COLORREF GetColor(int index);
	CString	commaE(CString value, bool remove = true);
	CString GetNewsKey(int index);
	int	GetIndex(CPoint point);

	// draw	function
	void	DrawHead(CDC* pDC = NULL);
	void	DrawCell(bool flag, CDC* pDC = NULL);
	void	DrawFail(CDC* pDC, CString str, CRect rect, int nFormat, bool bSelect = false);
	void	DrawBorder(CDC* pDC = NULL);

	// Generated message map functions
protected:
	void	parsedata(CString dat, CMapStringToString& ary);
	//{{AFX_MSG(CNGrid)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NGRID_H__90C16D11_DEA1_45E8_B213_AF06551001B3__INCLUDED_)

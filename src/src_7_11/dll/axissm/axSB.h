#if !defined(AFX_AXSB_H__25C527A4_1989_11D6_8AC9_00001CD7F9BE__INCLUDED_)
#define AFX_AXSB_H__25C527A4_1989_11D6_8AC9_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// axSB.h : header file
//

#define	sbTEXT		0
#define	sbGRAPH		1
#define	sbLISTB		2
#define	sbBTN		3

/////////////////////////////////////////////////////////////////////////////
// CAxSB window

const	COLORREF colSB = PALETTERGB(251, 252, 239);//234, 234, 234);

class CAxSB : public CWnd
{
// Construction
public:
	CAxSB(int type, int btnID = -1, bool underline = false);

// Attributes
public:
	CString	m_text;
	int	m_type;
	int	m_oatype;

	int	m_maxTick;
	CStringArray m_items;

protected:
	UINT	m_ID;
	int	m_index;
	CFont	m_font, m_bfont;
	COLORREF m_bkcolor;
	HBITMAP	m_bitmap;
	bool	m_underline;

	// sbGRAPH
	bool	m_graph;
	
	// sbLISTB
	bool	m_down;
	CRect	m_btnRc;

	UINT	m_btnID;
	bool	m_downX;
	CRect	m_btnRcX;
	class	CAxButton*	m_button;

	class	CAxStatic*	m_static;
	class	CAxTicker*	m_listB;
	DWORD	m_invest;
	bool	m_beep;
	CString	m_file;

// Operations
public:
	void	SetText(int type, CString text);
	void	SetItems(CStringArray& items);
	void	SetsbConfig(DWORD item, bool beep, CString file);
	bool	Initialize(int index, CWnd* parent, UINT nID, CRect rect, UINT oatype, HBITMAP bitmap, CString text = _T(""));
	int	GetType();
	void	SetType(int oatype);
	void	adjust(CToolBar* tBar, UINT nID, CRect rect);
	void	drawingColor(COLORREF bkGround);

protected:
	void	drawText(CDC* dc, CRect rect);
	void	drawTextEx(CDC* dc, CRect rect);
	void	drawRateRectangle(CDC* dc, CRect rect);
	void	drawRateText(CDC* dc, CRect rect);
	void	drawListButton(CDC* dc, CRect rect);

	CString	editingInvest(CString datB);
	CString	editingConclude(CString datB);

	CString	getSymbolData(short symbol, CString& str);
	CString	insertCommaToNum(CString str);
	CString	filterSpace(CString str);
	void	runningListBox();
	bool	itemEnabled(CString item);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxSB)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxSB();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxSB)
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonInvest();
	afx_msg void OnButtonConclude();
	//}}AFX_MSG
	afx_msg	LRESULT OnUser(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXSB_H__25C527A4_1989_11D6_8AC9_00001CD7F9BE__INCLUDED_)

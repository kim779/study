#if !defined(AFX_NTABLE_H__785FC61D_57A7_448B_A205_AD79E2A76CEF__INCLUDED_)
#define AFX_NTABLE_H__785FC61D_57A7_448B_A205_AD79E2A76CEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NTable.h : header file
//
#include <afxcoll.h>
#define	CELLHEIGHT	20
/////////////////////////////////////////////////////////////////////////////
// CNTable window

class CNTable : public CWnd
{
// Construction
public:
	CNTable(int viewN, bool hLine = true);
	virtual ~CNTable();

// Attributes
protected:
	CFont		m_font;

	int		m_viewN;	// visible row count
	int		m_headW;	// table title width
	bool		m_hLine;	// horizontal line ку/ўн
	bool		m_default;	// default title

//	COLORREF	m_bColor;	// background color
	COLORREF	m_hColor;	// table head color
	COLORREF	m_cColor;	// table cell color
	COLORREF	m_lColor;	// table line color
	COLORREF	m_tColor;	// table text color

	CStringArray	m_arCT;		// table change title
	CStringArray	m_arTitle;	// table title text
	CStringArray	m_arData;	// table data

	BOOL		m_bManage;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNTable)
	//}}AFX_VIRTUAL

// Implementation
public:
	// color setting function
//	void	SetBackColor(COLORREF color)	{ m_bColor = color; }
	void	SetHeadColor(COLORREF color)	{ m_hColor = color; }
	void	SetCellColor(COLORREF color)	{ m_cColor = color; }
	void	SetLineColor(COLORREF color)	{ m_lColor = color; }
	void	SetTextColor(COLORREF color)	{ m_tColor = color; }
	// draw	function
	void	DrawBase(CDC* pDC = NULL);
	void	DrawHText(CDC* pDC = NULL);
	void	DrawBText(CDC* pDC = NULL);
	void	DrawBorder(CDC* pDC = NULL);
	// table setting function
	void	SetTitle(const CStringArray& ary, char*);
	void	SetWidth(bool head, int width);
	void	SetData(CString dat);
	void	ChangeTitle(int index = -1, CString title =_T(""));
	// etc
	void	ChangePalette();
	void	ChangeData();
	void	SetColor();
	void	SetManage(BOOL bManage) { m_bManage = bManage; }
	//**COLORREF GetColor(int index);
	CString	commaE(CString value, bool remove = true);
	// Generated message map functions
protected:
	//{{AFX_MSG(CNTable)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NTABLE_H__785FC61D_57A7_448B_A205_AD79E2A76CEF__INCLUDED_)

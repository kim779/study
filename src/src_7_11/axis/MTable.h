#if !defined(AFX_MTABLE_H__972F0B9B_AD33_4BE6_9DFD_2A366906C2B7__INCLUDED_)
#define AFX_MTABLE_H__972F0B9B_AD33_4BE6_9DFD_2A366906C2B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MTable.h : header file
//
#include <afxcoll.h>
#define	CELLHEIGHT	20
/////////////////////////////////////////////////////////////////////////////
// CMTable window

class CMTable : public CWnd
{
// Construction
public:
	CMTable(CString home, int viewN, bool hLine = true);
	virtual ~CMTable();

// Attributes
protected:
	CFont		m_font;
	CString		m_home;

	int		m_viewN;	// visible row count
	int		m_headW;	// table title width
	bool		m_hLine;	// horizontal line ку/ўн
	bool		m_default;	// default title

	COLORREF	m_hColor;	// table head color
	COLORREF	m_cColor;	// table cell color
	COLORREF	m_lColor;	// table line color
	COLORREF	m_tColor;	// table text color

	CStringArray	m_arData;	// table data
	CStringArray	m_arMultiData;	// table data

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMTable)
	//}}AFX_VIRTUAL

// Implementation
public:
	// color setting function
	void	SetHeadColor(COLORREF color)	{ m_hColor = color; }
	void	SetCellColor(COLORREF color)	{ m_cColor = color; }
	void	SetLineColor(COLORREF color)	{ m_lColor = color; }
	void	SetTextColor(COLORREF color)	{ m_tColor = color; }
	// draw	function
	void	DrawManage(CDC* pDC = NULL);
	// table setting function
	void	SetTitle(char*);
	void	SetContent(CString strdata);
	void	SetData(CString dat);
	CString Parser(CString &srcstr, CString substr);
	// etc
	void	ChangePalette();
	void	ChangeData();
	void	SetColor();
	COLORREF GetColor(int index);
	CString	commaE(CString value, bool remove = true);
	// Generated message map functions
protected:
	//{{AFX_MSG(CMTable)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MTABLE_H__972F0B9B_AD33_4BE6_9DFD_2A366906C2B7__INCLUDED_)

#pragma once


// CMTable

class CMTable : public CWnd
{
	DECLARE_DYNAMIC(CMTable)

public:
	CMTable();
	CMTable(CString home, CWnd* pWnd, CWnd* pctrlwnd);
	CString		m_data;
	CFont		*m_pfont;
	int			maxX;
	virtual ~CMTable();

	void	Setfont(int ifont, CString font);
	void	ChangeData();
	void	SetHeadColor(COLORREF color)	{ m_hColor = color; }
	void	SetCellColor(COLORREF color)	{ m_cColor = color; }
	void	SetLineColor(COLORREF color)	{ m_lColor = color; }
	void	SetTextColor(COLORREF color)	{ m_tColor = color; }
	void	DrawManage(CDC* pDC = NULL);
	void	SetTitle(char*);
	void	SetData(CString dat);
	void	ChangePalette();
	void	SetColor();
	COLORREF GetColor(int index);
	CString	commaE(CString value, bool remove = true);
	CString Parser(CString &srcstr, CString substr);
protected:
	CString		m_home;
	CWnd*		m_pWnd;
	CWnd*		m_pctrlWnd;

	
	int		m_viewN;	// visible row count
	int		m_headW;	// table title width
	bool		m_hLine;	// horizontal line ку/ўн
	bool		m_default;	// default title

	COLORREF	m_hColor;	// table head color
	COLORREF	m_cColor;	// table cell color
	COLORREF	m_lColor;	// table line color
	COLORREF	m_tColor;	// table text color

	CStringArray	m_arData;	// table data
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};



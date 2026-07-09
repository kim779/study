#if !defined(AFX_PANE_H__AA7C9B94_A4D9_47C5_87EB_417A99594A35__INCLUDED_)
#define AFX_PANE_H__AA7C9B94_A4D9_47C5_87EB_417A99594A35__INCLUDED_  

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pane.h : header file
//
#include <afxtempl.h>

#define	INDEX_CNT	40	
class _inter
{
public:
	CString	Code;
	CString	Name;
	CString	curr;
	CString	diff;
	CString	gvol;
};

#define COLOR_TICK_UP		101
#define COLOR_TICK_DOWN		102
#define COLOR_TICK_TEXT		103
#define COLOR_TICK_INSIDE	104
#define COLOR_TICK_LINE		105
#define COLOR_TICK_SH		106
#define COLOR_TICK_SS		107
#define COLOR_TICK_BH		108
#define COLOR_TICK_HL		109
#define COLOR_TICK_HH		110

#define AUX_TEXT	1
#define AUX_GRAPH	2

#define COLW		400
#define RMAX		20
#define FGAP		10
#define PGAP		2

// ups and downs color
#define COLOR_GB	RGB(255, 255, 255)
#define COLOR_SH	RGB(180,   0,   2)
#define COLOR_SS	RGB(255,  74,  92)
#define COLOR_BH	RGB(235, 137, 245)
#define COLOR_HL	RGB( 30, 126, 255)
#define COLOR_HH	RGB(  0,  19, 187)

#define COLOR_BACK	RGB(255, 255, 255)

#define BASE_UPDOWN	1000
#define BASE_INTER	2000

#define COLOR_PANETEXT		RGB( 81,  85,  96)
#define COLOR_PANESHADOW	RGB(252, 253, 255)
#define COLOR_PANEBOX		RGB(181, 181, 181)

#define	WM_PANELIST	WM_USER + 13
/////////////////////////////////////////////////////////////////////////////
// CPane window

class CPane : public CWnd
{
// Construction
public:
	CPane(CString home, CString userN, bool (*axiscall)(int, WPARAM, LPARAM), CBitmap* bkImage = NULL);

// Attributes
protected:
	CString		m_home;
	CString		m_userN;
	CString		m_selectS;

	CPen		m_pen;
	CRect		m_arRect;
	CFont		m_font;

	int		m_id;
	int		m_kind;
	int		m_index;
	int		m_aux;
	int		m_wBitmap;
	int		m_wHead;
	int		m_interval;
	bool		m_down;
	bool		m_bReset;
	bool		(*m_axiscall)(int, WPARAM, LPARAM);

	DWORD		m_select;
	CBitmap*	m_bkIMG;
	CBitmap		m_bitmap;
	COLORREF	m_bkColor;
	COLORREF	m_color[5];

	CArray	< int, int >	m_arItem;
	CArray	< int, int >	m_arNewsKey;
	CArray	< class _inter*, class _inter* > m_arInter;
	
	CStringArray		m_arSym;		// select items symbol ary
	CMapStringToOb		m_arInterByCode;	// for rts process for TKIND_INTER
	CStringArray		m_arHistoryNews;	// new history
	CStringArray		m_arConclusion;		// conclusion history
	CMapStringToString	m_arSymData;		// select items dat ary for symbol
	CMapStringToString	m_mapData;		// active item dat ary
	CMapStringToString	m_arSymbyIndex;		// match symbol by index
	CMapStringToString	m_arIndexbySym;		// match index by symbol
	CStringArray		m_arSign[40];		// max 5 db sign

	class CPaneList*	m_panelist;		// pop up pane
	CMapStringToString	m_mapExpectSymbol;	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPane)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	SetPaneInfo(int index);
	void	SetFmx(bool setting, int kind = 0, int key = 0, CString symbol = _T(""), CString dat = _T(""));
	void	SetInter(char* dat, bool bRotate = true);
	void	UpdatePaneData(int kind, CString dat);//temp function
	void	ProcessRTS(CString symbol, CString dat);
	void	ProcessNotice(CString str);
	int	GetPaneID()	{ return m_id; }

	COLORREF GetColor(int index);
	BOOL	GetPaneData(int index, CString &key, CString &sym, CMapStringToString &mapData);
	CFont*	GetFont()	{ return &m_font; }
	int	GetHeadWidth()	{ return m_wHead; }
	CString	GetIndexSName(int index);
	CString GetExpectSymbolName(CString sym);
	char	GetSign(CString dat);
	CString	commaE(CString value, bool remove = true);
	CString	getDBSign(CString value);
	int	GetAux()	{ return m_aux; }
	CStringArray*	GetArSign()	{ return m_arSign; }
	int	GetArItem (int index) { return m_arItem.GetAt(index); }
	COLORREF	GetUpDownColor(int index) { return m_color[index]; }
	CString GetNewsTitle(int index);
	CString	dbE(CString value);

	virtual ~CPane();
	// Generated message map functions
protected:
	void	drawBackground(CDC* pDC);
	void	DrawData(CDC* pDC = NULL);
	void	DrawHead(CDC* pDC);
	void	DrawAr(CDC* pDC, bool bDown = false);
	void	Draw_Index(CDC* pDC = NULL);
	void	Draw_KIndex(CDC* pDC, int key);
	void	Draw_News(CDC* pDC = NULL);
	void	Draw_Updn(CDC* pDC = NULL);
	void	Draw_Inters(CDC* pDC = NULL);
	void	Draw_Conclusion(CDC* pDC = NULL);

	void	SetData(CString dat);
	void	SetData(CString dat, CMapStringToString &mapData);
	void	parseDat(CString dat, CMapStringToString& ary);
	void	loadInter(CString section);
	int	GetPosY(int value, int max);
	void	saveinfo(int selectN);
	void	setSign(int key, CString dat);
	int	GetTicKey(int index);
	int	GetNewsKey(int index);
	CString	GetdataUPDN();
	void	MakeExpectSymbolTable();
	void	PopUpScreen(int index = -1);
	bool	GetExpect(int index);
	
	//{{AFX_MSG(CPane)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg	LRESULT OnPanelist(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANE_H__AA7C9B94_A4D9_47C5_87EB_417A99594A35__INCLUDED_)

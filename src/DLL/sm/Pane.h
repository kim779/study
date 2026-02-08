#if !defined(AFX_PANE_H__AA7C9B94_A4D9_47C5_87EB_417A99594A35__INCLUDED_)
#define AFX_PANE_H__AA7C9B94_A4D9_47C5_87EB_417A99594A35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pane.h : header file
//
#include <afxtempl.h>

#define	INDEX_CNT	42
class _inter
{
public:
	CString	Code;
	CString	Name;
	CString	curr;
	CString	diff;
	CString	gvol;
};
/*
class _news
{
public:
	CString	key;
	CString	data;
};*/
/////////////////////////////////////////////////////////////////////////////
// CPane window
#include "usedefine.h"

class CPane : public CWnd
{
// Construction
public:
	CPane(bool (*axiscall)(int, WPARAM, LPARAM), CBitmap* bkImage = NULL);

// Attributes
protected:
	CString		m_selectS;

	CPen		m_pen;
	CRect		m_arRect;
	CFont		m_font;

	int		m_id;
	int		m_kind;
	int		m_index;
	int		m_aux;
	int		m_nCount;
	bool		m_down;
	bool		m_bReset;
	bool		m_bNew;
	bool		m_bTurn;
	bool		(*m_axiscall)(int, WPARAM, LPARAM);

	DWORD		m_select;
	CBitmap*	m_bkIMG;
	CBitmap*	m_bitmap;
	CBitmap*	m_news_back;
	CBitmap*	m_idx_back;
	COLORREF	m_lineColor;	//** gubun line
	COLORREF	m_bgColor[5];	//** up&down background colors
	COLORREF	m_fgColor[5];	//** up&down foreground colors

	CArray	< int, int >	m_arItem;
	CArray	< int, int >	m_arNewsKey;
	CArray	< class _inter*, class _inter* > m_arInter;
//	CArray	< class _news*, class _news* > m_arNews;
	
	CStringArray		m_arSym;		// select items symbol ary
	CMapStringToOb		m_arInterByCode;	// for rts process for TKIND_INTER
//	CMapStringToOb		m_arNews;		// new key data
	CStringArray		m_arHistoryNews;	// new history
	CMapStringToString	m_arSymData;		// select items dat ary for symbol
	CMapStringToString	m_mapData;		// active item dat ary
	CMapStringToString	m_arSymbyIndex;		// match symbol by index
	CMapStringToString	m_arIndexbySym;		// match index by symbol
	CStringArray		m_arSign[INDEX_CNT];		// max 5 db sign

	CStringArray m_arrInterGroup;
	//class CETicker*		m_eTicker;
#ifdef DF_USESTL_PANE
	std::unique_ptr<class CNewsHistory> m_newHistory;
#else
	class CNewsHistory* m_newHistory;
#endif
	
	CMapStringToString	m_mapExpectSymbol;	// 2006.7.24 이인호 => 예상지수 심볼 테이블

	int			m_nTitleCx;
	HCURSOR			m_hCursorHand, m_hCursor;
	
	/////////////////////////////////////////////////////////////////////////////////
	// 20071227 뉴스 매체 추가 여부에 관한 axisticker.ini 버전 체크
	BOOL			m_bAppendedNews;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPane)
	//}}AFX_VIRTUAL

// Implementation
public:
	
#ifdef DF_USESTL_PANE
	std::unique_ptr<class CToolTipCtrl> m_ToolTip;
#else
	class CToolTipCtrl* m_ToolTip;
#endif
	void InitToolTip();
	void set_ToolTipText(CString text, BOOL bActivate);
	int SplitString(CString strData, CString strToken, CStringArray &astrResult, int imaxcnt=100);
	BOOL SetTickPopup();
	LONG OnXMSG(WPARAM wParam, LPARAM lParam);
	CString GetGroupKeyByIndex(int nIndex);
	void writeInterCode(CString section, int groupN);
	void LoadInterGroup(CStringArray &arGroup);
	DWORD makeOption(BOOL bUse, COLORREF color);
	int GetIndexByKey(int key);
	void save();
	void	SetPaneInfo(int index);
	void	ReSetPaneInfo();
	void	SetFmx(bool setting, int kind = 0, int key = 0, CString symbol = _T(""), CString dat = _T(""));
	void	SetEFmx(bool setting, int kind = 0, int key = 0, CString symbol = _T(""), CString dat = _T(""));
	void	SetInter(char* dat, bool bRotate = true);
	void	UpdatePaneData(int kind, CString dat);//temp function
	void	ProcessRTS(CString symbol, DWORD* data);
	int	GetPaneID()	{ return m_id; }
	void	MatchNewsIndex(int &index);		// 20071224 이장희(뉴스 매체 추가중 index 변경)
	int	GetPaneKind() { return m_kind; }
	BOOL IsExitSymArr(CString strsym) {
		CString stmp, symbol;
		symbol = replaceKey(strsym);
		if (m_arSymData.Lookup(symbol, stmp))
			return TRUE;
		else
			return FALSE;
	}
	virtual ~CPane();
	// Generated message map functions
protected:
	CFont m_bFont;
	void	drawBackground(CDC* pDC);
	void	drawDataBG(CDC* pDC, int tp);
	void	DrawData(CDC* pDC = NULL);
	void	DrawAr(CDC* pDC, bool bDown = false);
	void	Draw_Index(CDC* pDC = NULL);
	void	Draw_KIndex(CDC* pDC, int key);
	void	Draw_Prog(CDC* pDC, int key);
	void	Draw_Invest(CDC* pDC, int key);
	void	Draw_News(CDC* pDC = NULL);
	void	Draw_Updn(CDC* pDC = NULL);
	void	Draw_Inters(CDC* pDC = NULL);
	void	NewNews(bool bOn = true);

//	CRect	GetImgRect();
	CString	commaE(CString value, bool remove = true);
//	CString	GetStringByKey(CString keys);
	COLORREF GetColor(int index);
	void	SetData(CString dat, BOOL bTimer = FALSE);   //20230303 
	void	SetData(DWORD* data);
	void	parseDat(CString dat, CMapStringToString& ary);
	CString	dbE(CString value);
	CString	getDBSign(CString value);
	char	GetSign(CString dat);
	void	loadInter(CString section);
	int	GetPosY(int value, int max);
	CString	GetIndexSName(int index);
	void	saveinfo(int selectN);
	void	setSign(int key, CString dat);
	int	GetTicKey(int index);
	CString	GetdataUPDN(CPoint pt);

/////////////////////////////////////////////////////////////////////////////////
	// 2006.7.24 이인호 => 예상지수를 위한 심볼테이블
	void	MakeExpectSymbolTable();	
	CString GetExpectSymbolName(CString sym);
/////////////////////////////////////////////////////////////////////////////////
	// 2006.11.03 이인호 => 뉴스 alarm을 위하여 
	BOOL	m_bNoAlarm;
	COLORREF m_clrAlarm;
	BOOL		IsSetting(DWORD value);
	BOOL		IsNoUse(DWORD value);
	COLORREF	getColor(DWORD value);
	COLORREF	GetReverseColor(COLORREF color);
	CSize		GetStringSize(CDC* pDC, CString string);
/////////////////////////////////////////////////////////////////////////////////
	// 20070615 예상지수오류 수정
	CString		replaceKey(CString symbol);	
/////////////////////////////////////////////////////////////////////////////////
	// 20071227 뉴스 매체 추가 여부에 관한 axisticker.ini 버전 체크
	void		JudgeAppendedNews();
	void		CheckNewsVersion(CString file, CString section, int index);
	int		FindLastNewsKey(CString section);
	
	//{{AFX_MSG(CPane)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMsgFromMain(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:

	CBitmap* m_imgButton;
	CSize m_szButton;
	WORD m_wButtonTextAlign;
	void LoadImage();
	void DrawButton(CDC* dc, CRect rc);
	int  GetButtonWidth(CDC* pDC);

	void SendPiboSise();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANE_H__AA7C9B94_A4D9_47C5_87EB_417A99594A35__INCLUDED_)

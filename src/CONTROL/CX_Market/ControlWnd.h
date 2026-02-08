#pragma once
// ControlWnd.h : header file
//
struct MarketList
{
	int	iType; //0-title, 1-data
	CString symbol, sText;
	CString srtsSymbol;
	CRect	rc;
	UINT	nTxtFormat;
	CString	sEtc;

	int imerge, imergecnt;
	COLORREF	bgcolor;
	int	iline, ifgcolor, ibgidx;
	int	ivisibleopt;
	int	ilink;

	MarketList(int ival, CString sdata1, CString sdata2, CString sdata3)
	{
		iType = ival;
		symbol = sdata1;
		sEtc = sdata2;
		srtsSymbol = sdata3;
	};

	MarketList(int ival, CString sdata1, CString sdata2, CString sdata3, CRect r)
	{
		iType = ival;
		symbol = sdata1;
		sText = sdata2;
		rc = r;
		srtsSymbol = sdata3;
	};

	MarketList(int ival, CString sdata1, CString sdata2,  CRect r=CRect(0), UINT nval=DT_LEFT, int ival1=0, int icnt=0, 
		int icoloridx=0, COLORREF color=0xFFFFFF, int ival2=0, int ival3=0, int ival4=0, int ival5=0)
	{
		iType = ival;
		symbol = sdata1;
		rc = r;

		if (ival == 0)
		{
			srtsSymbol = sdata2;
		}
		else
		{
			sText = srtsSymbol = sdata2;
		}
	
	//	if (sdata1 == "2176" || sdata1 == "2177") sText = "";
		
		nTxtFormat = nval | DT_SINGLELINE | DT_VCENTER;
		imerge = ival1;
		imergecnt = icnt;

		ibgidx = icoloridx;
		bgcolor = color;
		iline = ival2;
		ifgcolor = ival3;
		ivisibleopt = ival4;
		ilink = ival5;
	};

	MarketList()
	{
		iType = 0;
		nTxtFormat = DT_SINGLELINE | DT_VCENTER;
		imerge = 0;
		imergecnt = 0;

		ibgidx = 0;
		bgcolor = 0;
		iline = 0;
		ifgcolor = 0;
		ivisibleopt = 0;
		ilink = 0;
	};
};
typedef CArray<MarketList, MarketList> CMarketListArray;

/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();

// Attributes
public:
	CString	m_strCode;

// Operations
public:
	void	SetParam(struct	_param* param, CWnd *parent);
	void	Reset();
	void	Send(LPCTSTR sCode);

protected:
	CWnd*	m_pMainWindow;
	CString	m_strImagePath;
	CFont*	m_pFont;
	BOOL	m_bRecv;

	CMarketListArray m_arDrawList, m_arMkCodeList, m_arBefDrawList;
	char	m_key;
	CString	m_strParamName, m_strRealCode;

	int	m_iDivfldWCnt, m_iDivfldHCnt, m_iDivH, m_iDivW;
	int	m_iVisibleType, m_ivisiblecnt;

	COLORREF m_clrLineColor, m_clrBgColor, m_clrDataFgColor, m_clrLblFgColor;
	COLORREF m_clrColor[10];

	CMapStringToString m_RealMap, m_RealMkCodeMap;

	int	m_linkCursor;
	CString	m_strLinkMap, m_strLinkMapX[10];

	int	m_clrColorIdx[10];
	int	m_iOptBefore, m_beforeSend;

	CString	m_strBeforeList[10][10], m_strSendCode;
	COLORREF m_clrBefColor[10][10];
	BOOL	m_bTotShow;
	CStringArray m_maesu;
	CStringArray m_maedo;

// Operations
protected:
	COLORREF getAxColor(int color);
	CFont*	getAxFont(CString fName, int point=9, bool bBold=false, bool bItalic=false);

	LRESULT SendAxisMessage(WPARAM wParam, LPARAM lParam);
	void	SendTR(CString sCode);
	void	SendTR_Before(CString sCode);

	void	sendTR(CString trCode, char* sndb, int sndl, UINT nKey, int istat);
	void	ParseRecvData(int key, CString sData);
	void	ParseRealData(struct _alertR* alertR);

	void	Draw(CDC* pDC, CRect rcClient);
	void	ResizePanelBox();
	void	SetTotValue(BOOL bRedraw=FALSE);
	CRect	GetMkRect(CString sSymbol);

	CString	MakeMKCode(CString sCode);
	void	WriteLog(CString msg,bool bAppend);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	LONG OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#pragma once

// depth.h : header file
//

#include <afxmt.h>
#include <afxtempl.h>

class Citem
{
public:
	CString	m_data;
	bool	m_center=false;
	int	m_attr=0;

	CRect	m_fRc;
	CRect	m_bRc;

	COLORREF m_fRGB=0;	// pen
	COLORREF m_pRGB=0;	// bground
};

struct _config
{
	unsigned char	info;		// 1차 부가정보(1사분면 : 시고저종/피봇)
	unsigned char	mbong;		// 2차 부가정보(3사분면 : 차트/거래량)
	unsigned char	curr;		// 현재가 트레이스
	unsigned char	sgrp;		// 체결강도 그래프
	unsigned char	hdyul;		// 등락율
	unsigned char	hbold;		// 호가 굵게
	unsigned char   sbong;		// 당일 봉차트
	unsigned char	pre;		// 예상체결
	unsigned char	currset;	// 현재가 세팅
	unsigned char	hrbold;		// 호가잔량 굵게
	unsigned char	hcbold;		// 호가건수 굵게
	char		reserved[28];
};
#define	sz_CONFIG	sizeof(struct _config)

// attr
#define atNormal	0		// 일반적인 숫자 (0 제거)
#define atNoColor	1		// 일반적인 숫자 (0 제거, 색깔미표시)
#define atSignSup	2		// Normal에서 사인기호 제외
#define atArrow		3		// 계산된 값 (+ 없어도 강세표시)
#define atCoRgb		4		// + - 에 따라 색깔을 표시
#define atCoRgbSup	5		// 위와 같으나 사인기호 제외
#define at2SignZero	6		// 2Sign, 0표시
#define atPercent	7		// % 기호 뒤에 붙이기, 계산된 값 (+ 없어도 강세표시)
#define atTime		8		// 시간
#define	atComma		9		// 지정된 색으로 수치표시

/////////////////////////////////////////////////////////////////////////////
// Cdepth window

class Cdepth : public CWnd
{
// Construction
public:
	Cdepth();
	virtual ~Cdepth();

protected:
	CString	m_class;
	CWnd*	m_parent;
	std::unique_ptr<class CToolTip>	m_pToolTip;

	CString m_szName;
	int	m_key;
	
	CRect	m_rect;
	std::unique_ptr<CRgn>	m_pRgn;
	CString	m_options;

	CFont*	m_font;
	CString	m_fonts;
	int	m_point;
	int	m_style;

	int	m_fontH;

	CString	m_code;
//	CPtrArray m_items;
	CArray <std::shared_ptr<Citem>, std::shared_ptr<Citem>> m_items;
	CMap	< int, int, int, int& > m_indices;

	int	m_columns[3];		// column width
	double	m_rowH;			// row height
	int	m_topH;			// header height

	CCriticalSection m_sync;
	BOOL	m_tracking;		// mouse tracking
	CRect	m_prect;		// price & percent rect
	CRect	m_rcConfig;		// button rect for Config
	CRect	m_rcConfigL;		// button rect for Config for Left
	CRect	m_rcConfigR;		// button rect for Config for Right
	CRect	m_rcInfo;		// rightup rect
	CRect	m_rcInfoX;		// leftdown rect
	CRect	m_rcCurr;
	BOOL	m_bLButtonDown;		// if lbutton down

	int	m_focus;		// focus rect index
	double	m_maxsize;		// max volumn for bong graph

	double	m_curr;			// last price
	double	m_clos;			// previous close price
	double	m_sum4;
	double	m_sum9;

	int	m_idxMDS;
	int	m_idxMDE;
	int	m_idxMSS;
	int	m_idxMSE;

	int	m_depth;						// 5 or 10 depth
	enum	_ctype	{ ctNo=0, ctCode, ctIndex, ctFuture, ctOption, ctECN, ctKOption, ctKOFEX } m_type;
	bool	m_showBeforeDiff;					// 직전대비
	enum	_graph	{ grNo=0, grBar, grBarText } m_bar;		// bar graph
	bool	m_showPreValue;						// 예상 체결가, 예상 체결랑
	bool	m_showTop;						// 상단 컬럼명
	enum	_bottom	{ btNo=0, btSize, btTime, btSizeTime } m_bottom;// 하단 총잔량 표시
	bool	m_showLine;						// Line 표시
	enum	_act	{ acNo=0, acPopup, acMsg } m_action;		// LButton Down Action
	enum	_showInfo { infoNO = 0, infoMGVL, infoSHGA, infoRSHGA, infoSHGAX, infoMGVLX } m_showOHLC; // 시고저종 표시
	bool	m_bGradient;						// Gradient
	enum	_percent { pcNo = 0, pcSiga, pcKoga, pcJega, pcJgga, pcUser, pcGijun } m_percent;
	enum	_bong { bongNo = 0, bongBong, bongChgl } m_bong;	// MBong 표시
	int	m_nUDPercent;						// 유저설정 매입단가(등락율 기준가)
	bool	m_bBong;						// 당일봉 표시
	bool	m_bCurr;						// 현재가 추적 표시
	bool	m_bBoldPrice;						// 호가 BOLD
	bool	m_bBoldRemain;					// 호가잔량 BOLD
	bool	m_bBoldCnt;						// 호가건수 BOLD
	bool	m_bConfig;						// 현재가화면 (설정버튼 유무)
	CString	m_file;
	CString m_configFile;
	bool	m_bSiga;
	CString	m_refsym;						// reference symbol
	CString m_sDebug;

	struct _config	m_config;
	CString	m_path;
	CBitmap	*m_pBitmapC1, *m_pBitmapC2, *m_pBitmapP1, *m_pBitmapP2;

	COLORREF	m_clrWhite;
	COLORREF	m_clrHeadFg, m_clrHeadBk, m_clrDataFg;
	COLORREF	m_clrAskGs, m_clrAskGe, m_clrBidGs, m_clrBidGe;
	COLORREF	m_clrAskSzFg, m_clrBidSzFg;
	COLORREF	m_clrLine, m_clrDash, m_clrBox, m_clrBottom;
	COLORREF	m_clrUp, m_clrDown;
	COLORREF	m_clrFocus, m_clrCurr;
	COLORREF	m_clrBarAsk, m_clrBarBid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cdepth)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL	Create(CWnd* parent, void* ptr);

	COLORREF getColor(int color);
	CPen*	 getPen(int color, int width = 1);
	CFont*	 getFont(int point, CString name, int bold = 0, bool italic = false);
	CBitmap* getBitmap(CString path);

	void	TipMouseMove(CPoint point);
	void	MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly = false);

	// Generated message map functions
protected:
	void	initialColor();
	void	initialize();
	void	setIndices();

	void	dispatch(char* datB, int datL);
	void	dispatchFIVE(char* datB, int datL);
	void	dispatchTEN(char* datB, int datL);
	void	alert(CString data);
	void	alert(struct _alertR* alertR);
	void	freeItems();
	void	clear();

	void	draw(CDC* dc);
	void	drawItems(CDC* dc);
	void	drawInfo(CDC* dc);
	void	drawGraph(CDC* dc, int index);
	void	drawHead(CDC* dc);
	void	drawBottom(CDC* dc);
	void	drawLine(CDC* dc);
	void	drawView(CDC* dc);
	void	drawBground(CDC* dc);
	void	drawBong(CDC* dc);
	void	drawBongMGVL(CDC* dC);
	void	drawButton(CDC *pDC);
	void	drawButton(CDC *pDC, CRect rc, CBitmap *pBitmap);
	void	drawBox(CDC *pDC);

	void	calculateColumnWidth();
	void	calculateRowHeight();
	COLORREF SetItemBGColor(int index);

	void	getMaxSize();
	void	setItemRect();
	void	setItemRect5();
	void	setItemRect10();
	void	getPriceRect();
	CRect	getPercentRect();
	void	setFocus(CPoint point);
	int	getIndexByPoint(CPoint point);
	CString	getOrderPrice(CPoint point);
	void	calculatePercent();
	void	calculatePreValue();

	void	popupMenu(CPoint point, CString Price);
	void	sendMessage(CPoint point, CString Price);
	int	openView(int type, CString param);
	void	trigger(CString param);

	CString	format(CString data, int index);
	void	addComma(CString& data);
	CString	getSign(CString& data);
	void	eliminateZero(CString& data);
	double	str2double(CString string);

	BOOL	createFont();
	void	parseOptions();

	void	SendTR(CString name, CString data, BYTE type, int key);
	CString	Variant(int comm, CString data);

	CString	GetDataString(CString keys, CString init);
	int	GetDataInt(CString keys, int init);
	CString	GetBongStr();

	void	InitEnv();
	void	ConfigDlg();
	void	ConfigDlgL();
	void	ConfigDlgR();
	bool	ReadCondition(CString path, struct _config *pConfig);
	void	SaveCondition(CString path, struct _config *pConfig);
	bool	readFile(CString path, char *pBytes, int nBytes);
	bool	writeFile(CString path, char *pBytes, int nBytes);
	void	clickAction(CRect rect, CPoint point);
	bool	IsDigit(CString str);

	///////////////////////////////////////////////////
	// Extra Window
	bool			m_bMBPrice;
	bool			m_bMBVolume;
	std::unique_ptr<class CContractWnd>	m_pContract;
	std::unique_ptr<class CMBongWnd>	m_pMBong;

	void	EW_Init();
	void	EW_Clear();
	void	EW_Destroy();
	bool	EW_Create();
	void	EW_Move();
	void	EW_Show();
	void	EW_Font();
	void	EW_Palette();
	void	EW_Dispatch(char *dataB, bool bTEN);
	void	EW_Alert(CString data);
	void	EW_Alert(struct _alertR* alertR);
	///////////////////////////////////////////////////

	//{{AFX_MSG(Cdepth)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMBong(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(Cdepth)
	afx_msg long _getType();
	afx_msg void _setType(long nNewValue);
	afx_msg long _getHogaType();
	afx_msg void _setHogaType(long nNewValue);
	afx_msg long _getBeforeCompare();
	afx_msg void _setBeforeCompare(long nNewValue);
	afx_msg long _getPercent();
	afx_msg void _setPercent(long nNewValue);
	afx_msg long _getGraph();
	afx_msg void _setGraph(long nNewValue);
	afx_msg long _getTopDisplay();
	afx_msg void _setTopDisplay(long nNewValue);
	afx_msg long _getBottomDisplay();
	afx_msg void _setBottomDisplay(long nNewValue);
	afx_msg long _getLine();
	afx_msg void _setLine(long nNewValue);
	afx_msg long _getLButtonDown();
	afx_msg void _setLButtonDown(long nNewValue);
	afx_msg long _getOHLC();
	afx_msg void _setOHLC(long nNewValue);
	afx_msg BOOL _GetVisible();
	afx_msg void _SetVisible(BOOL bNewValue);
	afx_msg short _GetBongStatus();
	afx_msg void _SetBongStatus(short nNewValue);
	afx_msg short _GetCurrStatus();
	afx_msg void _SetCurrStatus(short nNewValue);
	afx_msg BOOL _getBoldPrice();
	afx_msg void _setBoldPrice(BOOL bNewValue);
	afx_msg BOOL _getBoldRemain();
	afx_msg void _setBoldRemain(BOOL bNewValue);
	afx_msg BOOL GetBoldRemainCount();
	afx_msg void SetBoldRemainCount(BOOL bNewValue);
	afx_msg void _SetProperties(LPCTSTR options);
	afx_msg BSTR _GetProperties();
	afx_msg void _Refresh();
	afx_msg short _ChangeInfo();
	afx_msg short ChangeBong();
	afx_msg void _Clear();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
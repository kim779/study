#pragma once
// GridWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGridWnd window
#include "BaseWnd.h"
#include "../../h/interst.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "NewMenu.h"	// ADD PSH 20070912
#include "setup.h"

#include "RealCheckDlg.h"

#define JMCODE			0		//종목코드
#define YDAYVOLUMN		1		//전일거래량
#define TDAYVALUE		2		//현재가
#define YDAYLASTVALUE	3		//전일종가
#define JISUGUBN		4		//지수구분
class CGridWnd : public CBaseWnd
{
// Construction
public:
	CGridWnd(CWnd* pMainWnd, int nIndex);

// Attributes
public:
	CString GetTitle() { return m_szTitle; }
	void SetTitle(CString sztitle) { m_szTitle = sztitle, InvalidateRect(m_rcTitle, FALSE); }
	BOOL IsExist(CString code) 
	{ 
		CString name; 
		return m_mapCode.Lookup(code, name);
	}
	void	PlayWave(CString strFile = "");
	

	CString	m_strWaveFile;
	BOOL	m_bWaveApply;

	COLORREF	m_clrKospi;
	COLORREF	m_clrKosdaq;
	
	BOOL		m_bMKClrApply;
	BOOL		m_bExpect;
	BOOL		m_bAutoExpect; // 예상가 자동여부
	BOOL		m_bPopupDlg;
	BOOL		m_bRTS;
	BOOL		m_bDispCode;
	int			m_nCodeWidth;

	CRealCheckDlg*	m_pRCDlg;
	BOOL			m_bMonitor;
	BOOL			m_bMonSym;
	CString			m_strMonData;
	BOOL			m_bEditWork;
	bool			m_bIsFirst;
	int			m_nGridWidth;
	
	double	m_dBase;		//선물옵션 기본 단위
	int		m_ClickRow;

	double	m_dKospi;
	double	m_dKosdaq;
// Operatio	ns
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL IsValidCode(CString strCode);
	virtual ~CGridWnd();
// for group wnd funtion
	void RemoveAll();
	UINT GetKind() { return m_kind; }
	void SetIndex(int nIndex) 
	{ 
		m_nIndex = nIndex, 
		m_section.Format("%s%02d", SEC_GRID, m_nIndex); 
	}

	void	OnAllsave();
	void	GetHearder(CArray< _gridHdr, _gridHdr >& arHeader);
	void	GetData(CArray <_inters*, _inters*>& arData);
	void 	GetData(CArray <_inters*, _inters*>& arData, int* nRowCount);	//arData의 row수를 반환
	CString	GetCodeName(CString code);
	void	SetKind(UINT kind);
	CString FindTitle(UINT kind);			// 타이틀 얻기

	void	AddData(int nIndex, CString sztmp);
	void	AddData_Overoper(int nIndex, CString sztmp);

	int		AddInters(struct _inters* pinters);
	void	RemoveInters(int nIndex);
	void	FieldSetup(bool bDlgSetup = false, int iEqualizer = 0);
	void	Reset(bool bAll = true);
	void	saveInterestX();
	void	saveInterest(bool btmp=false, int gno = -1);
	void	saveInterestVisible(int gno);					//MO_VISIBLE 전용 저장함수

	void	SetLineColor();

	double	Round(double data );

	// MODIFY PSH 20070911
	//void	SetLineColor(int nRow);
	void	SetLineColor(int nRow, class CintGrid* pGrid = NULL);
	// END MODIFY
	void	Assign(CGridWnd* pGrid);
	void	Redraw() { Invalidate(FALSE); }
	void	toggleAction(bool toggle);
	void	toggleAutoAction(bool toggle);
	void	makeCodeExist();
	void	OperAlarm(int kind, char* lParam);
	struct	_inters* GetData(int nIndex) { return m_inters.GetAt(nIndex); }

	CString CalcuPyungaSonik(struct _inters* pinters, CString curr);
	CString CalcuSuik(struct _inters* pinters, CString curr);
	CString setPyungaSonik(struct _inters* pinters);
	CString setsuikRate(struct _inters* pinters);

	void	SetGridBkColor(COLORREF color);

	// ADD PSH 20070917
	void	SetMarkerProp(_marker* pProp);
	void	RemoveAllBaseInters();
	void	ShowPopupMenu(int nX = -1, int nY = -1);
	void	SetAutoSaveCnd(BOOL bAddCnd, BOOL bChgCnd) { m_bAddCnd = bAddCnd; m_bChgCnd = bChgCnd; m_bAutoSaveAllApply = TRUE; }
	void	SetMarketColor(BOOL bApply, COLORREF clrKospi, COLORREF clrKosdaq);
	void    SetDispCode(BOOL bDispCode);

	void    setBookmark(int row);
	void    deleteBookmark(int row);
	COLORREF	GetBookMarkColor();
	COLORREF	GetBookMarkPenColor();
	// END ADD

	void	MultiChart();
	void	ShowRTSWnd();
	int		loadcfg_data();
	CString	makeGroupName();
	void	savecfg_data(CString keydata);					//KET_DATA를 저장
	
	void		sendtoMutiHoga();							//복수현재가2에 종목연동
	
	int			GetRowcount();
	CString 	GetcolName(int index); 
	CString		GetGroupName(int gno);
	int			GetGroupCount(int gno);						//종목 개수파악
	int			GetScrollCtrl();
	int			GetGridWidth();
	int			GetWidth();
	
	void testSaveFile(CString gubn, CString jongmok,  CString maipPrc, CString EvaPrc, CString PyugPrc, CString sonikRate);
	void testSaveFile2(CString code, CString time, CString symbol, CString EvaPrc);
	void testSaveFile3(CString code, CString datB);

	//선물잔고 수익률 계산
	void	getBase(CString m_code);
	CString	getFormatData(CString src);
	int	getJKind(CString m_code);
	CString	CalFutureEvalPrice(struct _inters* pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian);
	CString	CalFutureEvalRate(struct _inters* pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian);
	CString	CalJusikEvalPrice(struct _inters* pinters, double m_curr, double m_maip, double m_remian);
	CString	CalJusikEvalRate(struct _inters* pinters, double m_curr, double m_maip, double m_remian);
	
	void	triggerCode(int row);
	void	SaveAsInterField();
	
	int GetEqualizerField() { return m_EqualizerField; }

	CString GetRecommandInfo(int row);

	void ParseUpjongData(class CGridData* sdata);
	void parsingOubsUpjong(char* datB, int datL);
	// Generated message map functions
protected:
	CString m_strCode;
	//{{AFX_MSG(CGridWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);	// ADD PSH 20070912
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	void	OperInit();
	void	OperDestory();
	void	OperDraw(CDC* pDC);
	void	OperResize(int cx, int cy);
	void	RecvOper(int kind, CRecvData* rdata);
	void	RecvRTS(char* ptr);
	void	loadcfg();	
	void	savecfg();
	
	void	InsertRowNCode(int row);
	void	DrawTitle(CDC* pDC);
	void	DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight = true);
	void	ReplaceSymbol(CMapStringToString& fms, CString szold, CString sznew);
	void	initialGrid(bool size = true);
	void	ParseData(class CGridData* sdata);
	void	ParseRemainData(class CGridData* sdata);

	BOOL	IsInterest();
	void	sendTransaction(int update = -1);
	bool	IsDuplicateCode(CString code, int nrow, bool drag = false );
	int	GetCodeType(CString code);
	int	openView(int type, CString data);
	
	void	hawkeyeAction(bool toggle);
	void	RbuttonAction(int row);
	void	hawkEyeColor(int row);
	int		insertRow(int row, BOOL bInsertInter = TRUE);
	int		insertRow2(int afterRow, int beforeRow, BOOL bInsertInter = TRUE);
	

	void	insertInterest(int idx);
	

	bool	loadField(int fno = -1, bool bDlgSetup = false, int iEqualizer = 0);
	
	void	rebuildInterest();
	void	parsingField(CString symbol, bool bDlgSetup = false);
	void	parsingOubs(char* datB, int datL);
	void	parsingOubsOne(char* datB, int datL, int update = -1);	
	void	parseReCommandData(char* datB, int datL);					

	void	parsingAlert(CString datB);
	void	parsingNews(CString datB);
	void	InsertNews(CString datB);
	void	calcInClient();
	void	calcInClient(int row);
	int	CheckRealTimeCode(CString code);
//	int	loadInterest(int gno);
//	int	loadInterest(CString codes);
	void	ClearInterest();
	bool	ExistFile(int gno);	
	void	DeleteRow(int row);
	HBITMAP getBitmap(CString path);
	void	InsertEmpty(int row);
	void	queryNewsCode();
	void	parsingNewsCode(CRecvData* data);
	void	ReIndexing();
	void	ReIndexSpecial();
	int		GetIndex(CString symbol);

	// ADD PSH 20070912
	void	InitPopupMenu();
	void	SetTicker(BOOL bTicker = TRUE);
	void	SetInfo(BOOL bInfo = TRUE);
	void	CopyInter(_inters* pCpy, _inters* pSrc);
	void	MarkerSetup();
	CWnd*	ShowMessage(CString strMsg, int nWidth = 100, int nHeight = 30, UINT nDelay = 1000, BOOL bProgress = FALSE);
	void	BaseSorting();
	bool	GetCodeMap(CMap <CString, LPCTSTR, int, int>& arCodeMap);
	
	void	SettingGridHeaderName(int index);

	
	// END ADD
	int		chg_count[200];

	CWnd*		m_pGroupWnd;
	CWnd*		m_pToolWnd;
	CWnd*		m_pTreeWnd;
	CWnd*		m_pView;
	int			m_nIndex;
	CString		m_section;	
	COLORREF	m_clrFOCUS[2];	// 0 : focus 1 : normal	
	COLORREF	m_clrTEXT[2];	// 0 : focus 1 : normal
	CRect		m_rcTitle;
	CSize		m_size;
	CString		m_szTitle;
	CString		m_szFIELD;
	CString		m_szWIDTH;
	CArray	< _gridHdr, _gridHdr > m_gridHdrX;
	
	CIHStringMapArray	m_arExist;

	int		m_rowC;
	int		m_colC;
	int		m_nTurn;
	CFont*		m_pFontB;
	class CintGrid* m_grid;

	CArray <_inters*, _inters*> m_inters;		// file_load

	CMapStringToString	m_mapCode;
	bool		m_endsort;
	int		m_drag, m_drop;
	bool		m_posField;
	bool		m_ccField;		// client calculate filed
	int		m_bongField;		// 일봉필드
	int		m_sonikField;		// 평가손익 필드
	int		m_suikField;		// 수익률 필드
	CString		m_fieldOrder;		
	CfxImgButton	m_btDomino;	// 복수현재가2에 종목연동
	CfxImgButton	m_btSAVE;	// 관심종목으로 저장
	CfxImgButton	m_btCLOSE;	// 화면 닫기
	int		m_seldrop;	
	int		m_updateROW;	
	UINT		m_kind;
	
	// ADD PSH 20070911
	class CintGrid*		m_pTicker;
	BOOL				m_bTicker;

	BOOL				m_bfirstStart;		//첫 경험인지 

	BOOL				m_bMargin;
	CMapStringToString	m_mapMargin;
	CNewMenu			m_menuHeader;
	CNewMenu			m_menuSubHeader;
	CArray <_inters*, _inters*> 		m_arrBaseInters;		// file_load
	BOOL				m_bSorting;
	COLORREF			m_clrMarkerBKG;
	COLORREF			m_clrMarkerTXT;
	BOOL				m_bShadow;
	BOOL				m_bAllApply;
	BOOL				m_bMoveCfg;
	int					m_nMoveSave;
	CWnd*				m_pMsgWnd;
	BOOL				m_bInfo;
	Cconfig				m_tkConfig;
	BOOL				m_bAddCnd;
	BOOL				m_bChgCnd;
	BOOL				m_bAutoSaveAllApply;
	int					m_nRowHeight;

	CString				m_iYDayVol[200][5];		// 전일거래량을 저장하기 위한 공간(종목번호, 전일거래량, 현재가, 전일종가, 지수구분)
	int					m_yDayVolField;
	int					m_recomDataField;
	int					m_recomBasicCostField;	// 추천종목 기준가
	int					m_recomSuikrateField;
	int					m_recomBMjisuField;	
	int					m_recomBMSuikrateField;
	int					m_recomgoalCostField;
	int					m_irowCode[200];		// 코드가 존재하는 라인

	int					m_EqualizerField;		//체결 이퀄라이저 필드
	// END ADD
};

/////////////////////////////////////////////////////////////////////////////


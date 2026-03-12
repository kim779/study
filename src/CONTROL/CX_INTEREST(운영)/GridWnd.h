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

#define GRIDMAXNUM 27

// 값 넘어가는지 확인하는 함수짜기
const	int	maxIDX = 100;

class CIndexMap
{
public:
	CIndexMap()
	{
		idxCnt = 0;
		for (int ii = 0; ii < maxIDX; ii++)
			index[ii] = -1;
		
	}
	//virtual ~CIndexMap();

public:
	int index[maxIDX];
	int idxCnt;
};

static constexpr int MAX_FIELD_INDEX = 1024;

class CGridWnd : public CBaseWnd
{
// Construction
public:
	CGridWnd(CWnd* pMainWnd, int nIndex);
	int m_iIndex;
	int m_iTime{};
// Attributes
public:

	std::vector<std::shared_ptr<_intersx>> m_inters;
	CString GetTitle() { return m_szTitle; }
	void SetTitle(CString sztitle) { m_szTitle = sztitle, InvalidateRect(m_rcTitle, FALSE); }
	BOOL IsExist(CString code) 
	{ 
		CString name; 
		return m_mapCode.Lookup(code, name);
	}
	void	PlayWave(CString strFile = "");
	
	CString m_tempCode;

	int 	m_nGridNumber;

	CString	m_strWaveFile;
	BOOL	m_bWaveApply;

	COLORREF	m_clrKospi;
	COLORREF	m_clrKosdaq;
	
	BOOL		m_bMKClrApply;
	BOOL		m_bExpect;
	BOOL		m_bDiff;
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

	int		m_SendKey;
	int		m_UpdateGroup;

	int		m_iFieldType;		//등락인지 대비인지 
	int		m_PrevGroupIndex;

	int		m_selectRow;
	int		m_keyMode;

	int		m_nRowH;

	CStringArray m_pGridArray;
	int m_staticUpdate;

	BOOL m_bContinue;
	BOOL m_bSecond;

	BOOL m_bSingle;
	BOOL m_bSort;

	int	 m_nSelectedRow;
	int  m_nScrollPos;

	CGridData* m_pGridData;

	CMapStringToString newsFms;

	CString m_arSortData[100][2];

	CStringArray m_pGridMarker;

	BOOL	m_bSaveInterestForOub;
	int		m_nID;
	
	CStringArray m_recomArray;
	CMapStringToString m_recomCodeToData;

	CRTSData m_RtsData[MAX_LINE];

	CMapStringToOb		m_pSearchMap;
	std::vector<std::unique_ptr<CIndexMap>> _vIndex;
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
	bool IsCode (const CString& code)
	{		
		if (code.CompareNoCase("S0000") == 0 || code[0] == 'X')
			return true;	

		const auto ft = _mapSymbol.find(code);
		if (ft != _mapSymbol.end())
			return true;
		return false;
	}

	BOOL IsValidCode(CString strCode);
	virtual ~CGridWnd();
// for group wnd funtion
	void RemoveAll();
	UINT GetKind() { return m_kind; }
	void SetIndex(int nIndex) 
	{ 
		m_nIndex = nIndex;
		m_section.Format("%s%02d", SEC_GRID, m_nIndex); 
	}
	
	void 	UpdateDraw();
	void	OnAllsave();
	void	GetHearder(CArray< _gridHdr, _gridHdr >& arHeader);
	CString	GetCodeName(CString code);
	void	SetKind(UINT kind);
	CString FindTitle(UINT kind);			// 타이틀 얻기

	void	AddData(int nIndex, CString sztmp);
	void	AddData_Overoper(int nIndex, CString sztmp);

//	int	AddInters(std::shared_ptr<struct _intersx> pinters);
	void	RemoveInters(int nIndex);
	void	FieldSetup(bool bDlgSetup = false, int iEqualizer = 0);
	void	Reset(bool bAll = true);
	void	saveInterestX();
	void	saveInterest(BOOL bVisible = false, bool btmp=false, int gno = -1, bool bBookMark=false); //2013.05.14 KSJ 북마크 지정할때는 MO_VISIBLE 상관안하도록
	void	saveInterestInverse(BOOL bVisible = false, bool btmp=false, int gno = -1);
	void	saveInterestVisible(BOOL bVisible = false, int gno = -1);					//MO_VISIBLE 전용 저장함수
	void	saveBookmark(BOOL bVisible = false, int gno = -1);
	int	GetRowcountVisibleMode();
	void	SetLineColor();

	double	Round(double data );

	// MODIFY PSH 20070911
	void	SetLineColor(int nRow);
	void	Assign(CGridWnd* pGrid);
	void	Redraw() { Invalidate(FALSE); }
	void	toggleAction(bool toggle);
	void	toggleAction2(bool toggle);		//등락/대비 필드 교체
	void	toggleAutoAction(bool toggle);
	void	toggleAction5000(bool toggle);
	void	makeCodeExist();
	void	OperAlarm(int kind, char* lParam);
	//std::shared_ptr<struct _intersx> GetData(int nIndex) { return m_inters.at(nIndex); }    //test 20230209
	std::shared_ptr<struct _intersx> GetData(size_t nIndex) {
		if (m_inters.size() > nIndex)
			return m_inters.at(nIndex);
		return nullptr;
	}
	void	SetData(int nIndex, std::shared_ptr<struct _intersx> pinters) { m_inters.at(nIndex) = pinters; }

	CString CalcuPyungaSonik(struct _intersx* pinters, CString curr);
	CString CalcuSuik(struct _intersx* pinters, CString curr);
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

	void    setBookmark(int row, CString indexrgb);
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
	CString	CalFutureEvalPrice(struct _intersx* pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian);
	CString	CalFutureEvalRate(struct _intersx* pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian);
	CString	CalJusikEvalPrice(struct _intersx* pinters, double m_curr, double m_maip, double m_remian);
	CString	CalJusikEvalRate(struct _intersx* pinters, double m_curr, double m_maip, double m_remian);
	
	CCriticalSection	m_sync;

	void	triggerCode(int row);
	void	SaveAsInterField();
	
	int GetEqualizerField() { return m_EqualizerField; }

	CString GetRecommandInfo(int row);
	void	SetRecommandInfo(int row, CString strData);	//2012.04.24 KSJ 추천종목 값 세팅하기 위해서.
	void	InsertAtRecommandInfo(int row, CString strData);	//2012.05.07 KSJ Row Insert 시에 추천종목 값도 Insert해서 같은줄로 보이게

	void ParseUpjongData(class CGridData* sdata);
	void parsingOubsUpjong(char* datB, int datL);

	void ChangeField(bool type);
	
	void parsingOubsWhenVisible(char* datB, int datL, int mode);
	void	sendTransaction(int update = -1);												//MO_SELECT  모드상태
	void	SendWhenVisibleMode(CGridData* sdata);
	void	SendWhenVisibleModeTR(CGridData* sdata, int nStart, int nEnd, int update = -1, int updateGroup = -1);	//MO_VISIBLE 모드상태
																							//update : row
	
	void	SendWhenSortVisibleMode(CString data[MAX_LINE][2]);
	void	SendWhenSortVisibleModeTR(CString data[MAX_LINE][2],int nStart,int nEnd);
	void	SendWhenSelectMode(CString data[100][2]);
	
	void ArrangeField(int type, CString save[100][2]);	
	void activeTrigger(CString data);

	void killFocus();

	void SetRatio(double x, double y);

	
	void SetFontSize(int size);

	int GetRowHeight();

	void	SaveWidth();

	void	ResizeField();
	CString GetNewsData(CString code);
	int	GetInter(CString code);

	void SetVScroll(int scroll);
	void RecoverMarker();
	void SelectRowClear();

	bool	IsDuplicateCode(CString code, int nrow, bool drag = false );
	void	ClearGrid();
	void	loadcfg();	
	//group : 변경된 위치;
	// Generated message map functions

	void	SetInitSortingValue(); //2011.12.20 KSJ 컨트롤을 종료하지 않고 GridWnd의 m_bSorting을 false로 만들어 준다.
	void	SetColInfo(CString strName, int nRow, CString strEntry);	//2012.11.02 KSJ 종목특이사항 그리기
//	void	saveServer(int gno);
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
	void	RecvOper2(int kind, CRecvData* rdata);
	void	savecfg();
public:	//2012.02.13 KSJ 이벤트로 보내면 데이터가 깨져서 직접 메소드 호출함.
	

protected:
	void	InsertRowNCode(int row);
	void	DrawTitle(CDC* pDC);
//	void	DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight = true);
	void	ReplaceSymbol(CMapStringToString& fms, CString szold, CString sznew);
	void	initialGrid(bool size = true);
	void	ParseData(class CGridData* sdata);
	void	ParseRemainData(class CGridData* sdata);

	BOOL	IsInterest();

	int	GetCodeType(CString code);
	int	openView(int type, CString data);
	
	void	hawkeyeAction(bool toggle);
	void	RbuttonAction(int row);
	void	hawkEyeColor(int row);
	int	insertRow(int row, BOOL bInsertInter = TRUE, BOOL bLineAdd = TRUE);
	int	insertRow2(int afterRow, int beforeRow, BOOL bInsertInter = TRUE);
	

	void	insertInterest(int idx);
	

	bool	loadField(int fno = -1, bool bDlgSetup = false, int iEqualizer = 0);
	
	void	rebuildInterest();
	void	parsingField(CString symbol, bool bDlgSetup = false);
	void	parsingOubs(char* datB, int datL, int mode = -1);
	void	parsingOubsOne(char* datB, int datL, int mode = -1, int update = -1);	
	//void	parseReCommandData(char* datB, int datL);					

	void	parsingNews(CString datB);
	void	InsertNews(CString datB);
	
	void	parsingNewsx(DWORD* data);	//2012.01.19 KSJ Alertx 추가
	void	InsertNewsx(DWORD* data);	//2012.01.19 KSJ Alertx 추가
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
	int	GetIndex(CString symbol);

	// ADD PSH 20070912
//	void	InitPopupMenu();
//	void	SetTicker(BOOL bTicker = TRUE);
	void	SetInfo(BOOL bInfo = TRUE);
	void	CopyInter(_inters* pCpy, _inters* pSrc);
	void	MarkerSetup();
	CWnd*	ShowMessage(CString strMsg, int nWidth = 100, int nHeight = 30, UINT nDelay = 1000, BOOL bProgress = FALSE);
	void	BaseSorting();
	bool	GetCodeMap(CMap <CString, LPCTSTR, int, int>& arCodeMap);
	
	void	SettingGridHeaderName(int index);

	void	sendTransactionTR(int update,int nStart,int nEnd);
	void	SetInitRecommandInfo();

	void	ClearSearchMap();
	void	ReSetSearchMap();	//2011.12.29 KSJ
	
	CString CalMaketTime(CString strTime, bool bEnd);	//장운영시간 계산
	//2016.07.12 'b' - 'G' - 'V'
	void	uploadOK();	//2016.07.06 KSJ 관심종목 설정과 같이 'V' 보내줌

	// END ADD
	int		chg_count[200];

	CWnd*		m_pGroupWnd;
	CWnd*		m_pToolWnd;
	CWnd*		m_pTreeWnd;
	CWnd*		m_pView;
	int		m_nIndex;
	CString		m_section;	
	COLORREF	m_clrFOCUS[2];	// 0 : focus 1 : normal	
	COLORREF	m_clrTEXT[2];	// 0 : focus 1 : normal
	CRect		m_rcTitle;
	CSize		m_size;
	CString		m_szTitle;
	CString		m_szFIELD;
	CString		m_szWIDTH;
	CString		m_MultiViewCols;
	CArray	< _gridHdr, _gridHdr > m_gridHdrX;
	
	CIHStringMapArray	m_arExist;

	int		m_rowC;
	int		m_colC;
	int		m_nTurn;
	CFont*		m_pFontB;
	std::unique_ptr<class CintGrid> m_grid;
	std::vector<std::shared_ptr<_intersx>>	m_arrBaseInters;
	class CCX_INTERGRIDApp*	_pApp;

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
//	class CintGrid*		m_pTicker;
	BOOL				m_bTicker;

	BOOL				m_bfirstStart;		//첫 경험인지 

	BOOL				m_bMargin;
	CMapStringToString		m_mapMargin;
	CNewMenu			m_menuHeader;
	CNewMenu			m_menuSubHeader;
	BOOL				m_bSorting;
	COLORREF			m_clrMarkerBKG;
	COLORREF			m_clrMarkerTXT;
	BOOL				m_bShadow;
	BOOL				m_bAllApply;
	BOOL				m_bMoveCfg;
	int				m_nMoveSave;
	CWnd*				m_pMsgWnd;
	BOOL				m_bInfo;
	Cconfig				m_tkConfig;
	BOOL				m_bAddCnd;
	BOOL				m_bChgCnd;
	BOOL				m_bAutoSaveAllApply;
	int					m_nRowHeight;

	CString				m_iYDayVol[200][5];		// 전일거래량을 저장하기 위한 공간(종목번호, 전일거래량, 현재가, 전일종가, 지수구분)
	int					m_yDayVolField;
	int					m_recomBasicCostField;	// 추천종목 기준가
	int					m_recomSuikrateField;
	int					m_recomBMjisuField;	
	int					m_recomBMSuikrateField;
	int					m_upjongCode;			//종목별 업종코드
	int					m_recomJisuGubnField;
	int					m_irowCode[200];		// 코드가 존재하는 라인
	
	int					m_EqualizerField;		//체결 이퀄라이저 필드

	CMapStringToString	m_mapCurValue;	//2012.04.03 KSJ 처음 현재가 넣기위해..

	CString m_strBeginTime;		//동시호가 시작시간 xx:10:00 
	CString m_strEndTime;		//동시호가 종료시간 xx:59:59
	int	m_nBookMarkRow;		//2013.05.21  KSJ 북마크 넣을 Row
	// END ADD
public:
	CString m_slog{};
	std::unordered_map<int, int>	_mRsymbol;
	std::map<CString, int>		_mapSymbol;
	void initRSymbol();
	void parsingAlertx(LPARAM lParam);	//2012.01.19 KSJ Alertx 추가
	BOOL DataCheck(char* ptr);
	void getSeverMemoCheck(CArray<bool, bool>& parr);
	void setSeverMemoCheck(int xrow, bool bflag);


	BOOL	_bManual{};
	BOOL	_bAutoCheck{};

	void initgridalert(bool bsend = true);







	/*class DataAccessor
	{
	public:
		explicit DataAccessor(const _alertR* alert)
		{
			m_data = alert && alert->ptr[0]
				? (const DWORD*)alert->ptr[0]
				: nullptr;
		}

		LPCSTR ptr(int idx) const
		{
			if (!m_data || idx < 0 || idx >= MAX_FIELD_INDEX)
				return nullptr;

			DWORD p = m_data[idx];
			return p ? reinterpret_cast<LPCSTR>(p) : nullptr;
		}

		CString str(int idx) const
		{
			if (auto p = ptr(idx))
				return CString(p);
			return CString();
		}

		const DWORD* row() const { return m_data; }

	private:
		const DWORD* m_data{};
	};

	void CGridWnd::ProcessOneRow_Alertx(
		int xrow,
		const CString& code,
		const CString& strCode,
		const CString& strGubn,
		bool bKrx,
		int beginTime, int beginTimeEnd, int endTimeEnd, int endTime,
		const DataAccessor& acc);

	void ApplyExpectLogic(
		int xrow,
		const CString& code,
		const CString& strCode,
		const CString& strGubn,
		bool bKrx,
		int beginTime,
		int beginTimeEnd,
		int endTimeEnd,
		int endTime,
		LPCSTR expectPtr,
		LPCSTR currPtr,
		int serverTime,
		int dealTime,
		LPCSTR serverTimePtr,
		LPCSTR dealTimePtr,
		BOOL bLast,
		BOOL& bTransSymbol,
		BOOL& bZisu,
		CString& entry);

	void UpdateGridByHeaders(
		int xrow,
		const CString& code,
		const CString& strCode,
		const CString& strGubn,
		BOOL bExpect,
		BOOL bForceDraw,
		BOOL bTransSymbol,
		BOOL bZisu,
		BOOL& bDaebi,
		CString& entry,
		LPCSTR expectPtr,
		LPCSTR currPtr,
		const DataAccessor& acc);

	void PostProcessAfterGrid(
		int xrow,
		const CString& code,
		const CString& strCode,
		BOOL bExpect,
		const CString& saveData,
		const DataAccessor& acc);

	bool HandleNewsIfAny(const CString& code, const DataAccessor& acc);

	void HandleIndexExpectedCase(CString& code, CString& strCode, const CString& strGubn);

	bool NormalizeCodeAndMarket(const CString& in, CString& out, bool& bKrx);

	void ProcessOneRow(int xrow, const CString& code, const DataAccessor& acc);

	bool DecideExpectState(int xrow, const DataAccessor& acc, CString& entry);

	void UpdateGridFields(int xrow, const DataAccessor& acc,
		bool bExpect, const CString& entry);

	CString GetEntryForSymbol(int nSymbol,
		const DataAccessor& acc,
		bool bExpect);

	void UpdateCalculatedFields(int xrow,
		const DataAccessor& acc,
		bool bExpect);

	void HandleSpecialInfo(int xrow,
		const DataAccessor& acc);*/

	void UpdateFromTickSlot(int slotIndex);
};
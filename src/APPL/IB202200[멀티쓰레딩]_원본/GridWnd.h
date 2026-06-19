#pragma once
// GridWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGridWnd window
#include "BaseWnd.h"
#include "../../h/interst.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "setup.h"
#include "RealCheckDlg.h"

//#define JMCODE 		0	//종목코드
#define YDAYVOLUMN 	0	//전일거래량
#define TDAYVALUE 	1	//현재가
#define YDAYLASTVALUE   2       //전일종가
#define JISUGUBN 	3	//지수구분

#define IARRAYSIZE 	110
#define TYPE_SIGNAL_SYS 145
#define TYPE_SIGNAL_ALERT 142
#define TYPE_SIGNAL_NEWS 161
#define TYPE_REAL_NEWS 162

#define WM_GET_BROD WM_USER + 703

// 값 넘어가는지 확인하는 함수짜기
const int maxIDX = 100;

class CIndexMap
{
public:
	CIndexMap()
	{
		for (int ii = 0; ii < maxIDX; ii++)
			index[ii] = -1;
		idxCnt = 0;
	}
	// virtual ~CIndexMap();

public:
	int index[maxIDX];
	int idxCnt;
};

class CGridWnd : public CBaseWnd
{
	// Construction
public:
	CGridWnd(CWnd *pMainWnd, int nIndex);

	// Attributes
public:

	CString GetTitle() { return m_szTitle; }
	void SetTitle(CString sztitle) { m_szTitle = sztitle, InvalidateRect(m_rcTitle, FALSE); }
	void PlayWave(CString strFile = "");

	CWnd *m_pGroupWnd;
	CWnd *m_pToolWnd;
	CWnd *m_pTreeWnd;
	CWnd *m_pView;

	CString m_strWaveFile;
	BOOL m_bWaveApply;

	COLORREF m_clrKospi;
	COLORREF m_clrKosdaq;

	BOOL m_bMKClrApply;
	BOOL m_bExpect;
	BOOL m_bAutoExpect; // 예상가 자동여부
	BOOL m_bPopupDlg;
	BOOL m_bRTS;
	BOOL m_bDispCode;
	int m_nCodeWidth;
	BOOL m_bMargin;
	BOOL m_bInfo;
	bool m_bNewsNChart;	// 2011.12.02 KSJ	뉴스와차트 보이기
	int m_nNewsNChartWidth; // 2011.12.02 KSJ	뉴스와차트 넓이
	bool m_bPlusMinus;	// 2011.12.05 KSJ	추가 삭제시 에는 메세지 나오지 않도록 하기 위해
	BOOL m_bTicker;

	std::unique_ptr<CRealCheckDlg> m_pRCDlg;
	BOOL m_bMonitor;
	BOOL m_bMonSym;
	CString m_strMonData;
	BOOL m_bEditWork;
	bool m_bIsFirst;
	int m_nGridWidth;

	double m_dBase; //선물옵션 기본 단위
	int m_ClickRow;

	double m_dKospi;
	double m_dKosdaq;

	int m_iStartPoint; // Send 전송시 필드의 시작 시점
	int m_iEndPoint;   // Send 전송시 필드의 끝 시점
	int m_iSendcount;  // Send 를 전송한 횟수

	BOOL m_bContinue; // 관심종목의 필드가 많을경우
	BOOL m_bSecond;
	int m_staticUpdate;
	BOOL m_bLastSend; // 2017.02.21 'V' 마지막으로 보낼려고

	CStringArray m_pGridArray;

	CString m_strBeginTime; //동시호가 시작시간 xx:10:00
	CString m_strEndTime;	//동시호가 종료시간 xx:59:59

	CString m_strBeginTimeEnd; //동시호가 시작시간 xx:10:00
	CString m_strEndTimeEnd;   //동시호가 종료시간 xx:59:59
				   // Operations

	CString _newsTitle;
	CString _newscode;
public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridWnd)
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Implementation
public:

	void HoldDraw();
	void ReleaseDraw();
	void UpdateAll();

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
		m_nIndex = nIndex,
		m_section.Format("%s%02d", SEC_GRID, m_nIndex);
	}

	void GetHearder(CArray<_gridHdr, _gridHdr> &arHeader);
	std::vector<std::shared_ptr<_intersx>>& getArray() { return _vInters; };

	CString GetCodeName(CString code);
	void SetKind(UINT kind);
	CString FindTitle(UINT kind); // 타이틀 얻기

	void AddData(int nIndex, CString sztmp);
	void AddData_Overoper(int nIndex, CString sztmp);

	void AddInters(std::shared_ptr<struct _intersx> pinters);
	void RemoveInters(int nIndex);
	void FieldSetup(bool bDlgSetup = false, int iEqualizer = 0, bool bResize = true);
	void Reset(bool bAll = true);
	void saveInterestX();
	void saveInterest(bool btmp = false, int gno = -1, bool bBookMark = false); // 2013.05.14 KSJ 북마크 지정할때는 MO_VISIBLE 상관안하도록
	void saveInterestVisible(int gno);					    // MO_VISIBLE 전용 저장함수

	void sendTransactionTR(int update, int nStart, int nEnd);

	void SetLineColor();

	double Round(double data);

	// MODIFY PSH 20070911
	// void	SetLineColor(int nRow);
	void SetLineColor(int nRow, class CintGrid *pGrid = nullptr);
	// END MODIFY
	void Assign(CGridWnd *pGrid);
	void Redraw() { Invalidate(FALSE); }
	void toggleAction(bool toggle);
	void toggleAutoAction(bool toggle);
	void OperAlarm(int kind, char *lParam);
	
	std::shared_ptr<struct _intersx> GetData(int nIndex) { return _vInters.at(nIndex); }
	void SetData(int nIndex, std::shared_ptr<struct _intersx> pinters) { _vInters.at(nIndex) = pinters; }
	
	int GetInter(CString code); // 2013.01.22 KSJ 입력코드의 m_inters에서의 위치 찾기

	CString CalcuPyungaSonik(struct _intersx *pinters, CString curr);
	CString CalcuSuik(struct _intersx *pinters, CString curr);
	CString setPyungaSonik(struct _intersx *pinters);
	CString setsuikRate(struct _intersx *pinters);

	void SetGridBkColor(COLORREF color);

	// ADD PSH 20070917
	void SetMarkerProp(_marker *pProp);
	void RemoveAllBaseInters();
	void ShowPopupMenu(int nX = -1, int nY = -1);
	void SetAutoSaveCnd(BOOL bAddCnd, BOOL bChgCnd)
	{
		m_bAddCnd = bAddCnd;
		m_bChgCnd = bChgCnd;
		m_bAutoSaveAllApply = TRUE;
	}
	void SetMarketColor(BOOL bApply, COLORREF clrKospi, COLORREF clrKosdaq);
	void SetDispCode(BOOL bDispCode);
	void SetNewsNChart(BOOL bNewsNChart); // 2011.12.02 KSJ
	void SetMargin(BOOL bMargin);	      // 2011.12.05 KSJ
	void SetInfo(BOOL bInfo = TRUE);

	void setBookmark(int row, CString indexCol);
	void deleteBookmark(int row);
	COLORREF GetBookMarkColor();
	COLORREF GetBookMarkPenColor();
	// END ADD

	void MultiChart();
	int loadcfg_data();
	CString makeGroupName();
	void savecfg_data(CString keydata); // KET_DATA를 저장

	void sendtoMutiHoga(); //복수현재가2에 종목연동

	int GetRowValidcount();
	int GetRowValidcount(const std::vector<std::shared_ptr<struct _intersx>>& inter);
	CString GetcolName(int index);
	CString GetGroupName(int gno);
//	int GetGroupCount(int gno); //종목 개수파악
	int GetScrollCtrl();
	int GetGridWidth();
	int GetWidth();
	bool GetEditWork() { return m_bEditWork ? true : false; }
	void SetEditWork(bool tp);

	//선물잔고 수익률 계산
	void getBase(CString m_code);
	CString getFormatData(CString src);
	int getJKind(CString m_code);
	CString CalFutureEvalPrice(struct _intersx *pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian);
	CString CalFutureEvalRate(struct _intersx *pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian);
	CString CalJusikEvalPrice(struct _intersx *pinters, double m_curr, double m_maip, double m_remian);
	CString CalJusikEvalRate(struct _intersx *pinters, double m_curr, double m_maip, double m_remian);

	void triggerCode(int row);
	void SaveAsInterField();

	int GetEqualizerField() { return m_EqualizerField; }

	CString GetRecommandInfo(int row);
	void SetInitRecommandInfo();

	void ParseUpjongData(class CGridData *sdata);
	void parsingOubsUpjong(char *datB, int datL);

	int ArrangeField(int type, CString save[100][2]);
	void SendWhenVisibleMode(CString data[100][2]);
	void SendWhenVisibleModeTR(CString data[100][2], int nStart, int nEnd);

	void settingFieldBeforeSend(int &sendL, char *sendB); //그리드 필드 세팅
	void setBoolSecond(bool data) { m_bSecond = data; }

	void alertBMsuik();
	void SetGridinfo(bool bWidthSave = false); // 2013.04.16 KSJ width만 저장할때는 true해준다

	// 2016.07.12 'b' - 'G' - 'V'
	void uploadOK(); // 2016.07.06 KSJ 관심종목 설정과 같이 'V' 보내줌

	CRTSData m_RtsData[MAX_LINE];
	// Generated message map functions

	CString getABRdata(const char *pcurr, CString strCode, CString strsym); //종목성과 값을 계산해서 반환 (예상가와 장중현재가 실시간 수신시 호출)
protected:
	CString m_strCode;
	class CIB202200App* _pApp{};
	//{{AFX_MSG(CGridWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point); // ADD PSH 20070912
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	void OperInit();
	void OperDestory();
	void OperDraw(CDC *pDC);
	void OperResize(int cx, int cy);
	void RecvOper(int kind, CRecvData *rdata);

public:				      // 2012.02.13 KSJ 이벤트로 보내면 데이터가 깨져서 직접 메소드 호출함.
	void SetGridCount(int nCount) { m_nCount = nCount; }

protected:
	void loadcfg();
	void savecfg();

	void InsertRowNCode(int row);
	void DrawTitle(CDC *pDC);
//	void DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight = true);
	void ReplaceSymbol(CMapStringToString &fms, CString szold, CString sznew);
	void initialGrid(bool size = true);
	void ParseData(class CGridData *sdata);
	void ParseRemainData(class CGridData *sdata);

	BOOL IsInterest();
	void sendTransaction(int update = -1);
	bool IsDuplicateCode(CString code, int nrow, bool drag = false);
	int  GetCodeType(CString code);
	int openView(int type, CString data);

	void hawkeyeAction(bool toggle);
	void RbuttonAction(int row);
	void hawkEyeColor(int row);
	int insertRow(int row, BOOL bInsertInter = TRUE);
	int insertRow2(int afterRow, int beforeRow, BOOL bInsertInter = TRUE);

	void insertInterest(int idx);

	bool loadField(int fno = -1, bool bDlgSetup = false, int iEqualizer = 0);

	void rebuildInterest();
	void parsingField(CString symbol, bool bDlgSetup = false);
	void parsingOubs(char *datB, int datL);
	void parsingOubsOne(char *datB, int datL, int update = -1);
	void parseReCommandData(char *datB, int datL);

	void parsingNewsx();	   // 2012.01.19 KSJ Alertx 추가
	void InsertNewsx(CString strCode, CString szTitle);	   // 2012.01.19 KSJ Alertx 추가
	void calcInClient();
	void calcInClient(int row);
	int  CheckRealTimeCode(CString code);
	void ClearInterest();
	void DeleteRow(int row);
	void InsertEmpty(int row);
	void queryNewsCode();
	void parsingNewsCode(CRecvData *data);
	void ReIndexing();
	void ReIndexSpecial();
	int  GetIndex(CString symbol);

	// ADD PSH 20070912
	void InitPopupMenu();
	void CopyInter(_intersx *pCpy, _intersx *pSrc);
	void MarkerSetup();
	void BaseSorting();
	bool GetCodeMap(CMap<CString, LPCTSTR, int, int> &arCodeMap);

	void SettingGridHeaderName(int index);

	void ClearSearchMap();
	void ReSetSearchMap();					      // 2011.12.29 KSJ
	void SetColInfo(CString strName, int nRow, CString strEntry); // 2012.11.02 KSJ 종목특이사항 그리기

	CString CalMaketTime(CString strTime, bool bEnd); // 2012.11.26 KSJ 장운영시간 계산

	// END ADD
	bool m_isRecommand; //현재 추천종목을 보여주고 있는건지

	int chg_count[200];

	int m_nIndex;
	CString m_section;
	COLORREF m_clrFOCUS[2]; // 0 : focus 1 : normal
	COLORREF m_clrTEXT[2];	// 0 : focus 1 : normal
	CRect m_rcTitle;
	CSize m_size;
	CString m_szTitle;
	CString m_szFIELD;
	CString m_szWIDTH;
	CArray<_gridHdr, _gridHdr> m_gridHdrX;

	CIHStringMapArray m_arExist;

	int m_rowC;
	int m_colC;
	int m_nTurn;
	CFont *m_pFontB;
	
	std::unique_ptr<class CintGrid> m_grid;
	
	CMapStringToOb m_pSearchMap;
	CStringArray m_arEpbDate; // 2016.04.15 KSJ epb 추천일자

	CMapStringToOb m_mapCodeABR;
	bool m_endsort;
	int m_drag, m_drop;
	bool m_posField;
	bool m_ccField;	  // client calculate filed
	int m_bongField;  // 일봉필드
	int m_sonikField; // 평가손익 필드
	int m_suikField;  // 수익률 필드
	CString m_fieldOrder;
	CfxImgButton m_btReload; // 그룹리로드
	CfxImgButton m_btDomino; // 복수현재가2에 종목연동
	CfxImgButton m_btSAVE;	 // 관심종목으로 저장
	CfxImgButton m_btCLOSE;	 // 화면 닫기
	int m_seldrop;
	int m_updateROW;
	UINT m_kind;
	int m_nBookMarkRow; // 2013.05.21  KSJ 북마크 넣을 Row

	BOOL m_bfirstStart; //첫 경험인지

	CMapStringToString m_mapMargin;
	CMenu m_menuHeader;
	CMenu m_menuSubHeader;

	bool m_bNews{};
	BOOL m_bSorting;
	COLORREF m_clrMarkerBKG;
	COLORREF m_clrMarkerTXT;
	BOOL m_bShadow;
	BOOL m_bAllApply;
	BOOL m_bMoveCfg;
	int m_nMoveSave;
	CWnd *m_pMsgWnd;
	Cconfig m_tkConfig;
	BOOL m_bAddCnd;
	BOOL m_bChgCnd;
	BOOL m_bAutoSaveAllApply;
	int m_nRowHeight;

	std::unordered_map<std::string, CString> _mapiYDayVol;

	int m_yDayVolField;
	int m_recomDataField;
	int m_recomBasicCostField; // 추천종목 기준가
	int m_recomSuikrateField;
	int m_recomBMjisuField;
	int m_recomBMSuikrateField;
	int m_recomgoalCostField;
	int m_recomJisuGubnField;
	int m_upjongCode;	    //종목별 업종코드
	int m_irowCode[IARRAYSIZE]; //코드가 존재하는 라인
	int m_beforeVolField;	    //전일거래량
	int m_newCurrField;	    //현재가
	int m_EqualizerField;	    //체결 이퀄라이저 필드

	int m_epbDataField; // 2016.04.15 KSJ epb 추천일자

	// END ADD

	int m_nCount;			  // 2013.04.22 KSJ 그리드 카운트

	HINSTANCE m_hSignalMng{};

	// win10
	bool m_bSkipRTS;
	bool CheckField(CString symbol);
	CStringArray m_arrSymbol;
	
	std::vector<std::shared_ptr<struct _intersx>> _vInters;
	std::vector<std::shared_ptr<struct _intersx>> _arrBaseInters;


public:
	int m_iTime;
	bool m_bSending;
	CString m_sRoot;
	CString m_slog;

	std::unordered_map<int, int>	_mRsymbol;
	std::map<CString, int>		_mapSymbol;

	std::map<CString, int> m_mapRtCallCount;  // code별 1분 누적 호출 횟수
	int m_nRtLogMinute = -1;                  // 마지막 로그 기록한 분 (hour*60+min)

	void parsingAlertx(LPARAM lParam);
	void initRSymbol();
	void saveServer(const std::vector<std::shared_ptr<struct _intersx>>& vInters, int gno = -1);

	BOOL m_bGetHogaRTS{};  //실시간 호가 데이터(D) 수신 여부
	void getSeverMemoCheck(CArray<bool, bool>& parr);
	void setSeverMemoCheck(int xrow, bool bflag = false);
	
	int _typeAuto = 0;
	void initgridalert();
	void setMemo();
};

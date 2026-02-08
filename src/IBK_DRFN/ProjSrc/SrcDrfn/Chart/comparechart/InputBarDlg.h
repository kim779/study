//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_INPUTBARDLG_H__88AA28E0_917F_485D_BE10_0171E8C2E60B__INCLUDED_)
#define AFX_INPUTBARDLG_H__88AA28E0_917F_485D_BE10_0171E8C2E60B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBarDlg.h : header file
//
#include "./control/RscDlg.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CInputBarDlg dialog
//#include "./control/CJFlatComboBox.h"

//#include "../../../CodeFind/AllOptCodeDlg.h"
//#include "../../COMMON_BASE/baseInfo/BaseInfo.h"	//2005.09.27 commented by sy, nam
//#include "../../../DrMLib/DrMiddleLib.h"//05.09.27

#include "PopupList.h"
//#include "InputControl.h"
#include "./control/EditUsr.h"

//컬러테이블을 쓸경우 /
//#include "./control/ClrButton.h"

//이미지 리스트를 쓸 경우 
#include "../chart_common/ILButton.h"

#include "../../inc/ExOCXDLL.h"
//#include "../../inc/ExCodeInput.h"

//#ifndef __HANHWA_CHART_REPLAY_INFO_H
//	#include "../chart_common/ReplayChartInfoDef.h"
//#endif

#include "./define/ChartMsgDef.h"				// 복수종목 - ojtaso (20070223)

#define CNTOFMAXBUTTONS (4)
#define CNTOFMAXCBITEMS (12)

#define STOCK_STROPTION		"KZZ"		//주식 Option
#define KOSPI_STRFUTOPT		"FUT&OPT"	//Kospi Fut/Option
#define KOSDAQ_STRFUTOPT	"KFX&KFO"	//Kosdaq Fut/Option

typedef struct BEFORE_INFO
{
	char chOldType; //복기 <->복기 아닌것 간에 저장된 정보
	int  nInterval; //조회 간격
	BOOL bTimeSync; //시간(틱/분/일주....) 연동 여부
	BOOL bShowExtToolBar; //하단 툴바 보였는지 여부.

} BEFORE_INFO;


class CStdDialog;
class CChartMng;
class CInputBarDlg : public CRscDlg
{
	//>>>>>>>------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2006.01.09
	//Reason	:   복기 프로그래스 바 표시 
	//				
private:
	int	 m_nPosOfPrgBar;
	int	 m_nTotalSec;
public:
	void Add_nPosOfPrgBar()			{ m_nPosOfPrgBar++;			}
	int  Get_nPosOfPrgBar()			{ return m_nPosOfPrgBar;	}
	void Init_nPosOfPrgBar()		{ m_nTotalSec = 0; m_nPosOfPrgBar = 0;	}

	virtual void ChangeSkinColor(COLORREF clrBackground);
	//>>>>>>>------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.09.08
	//Reason	:   사용자가 입력한 값을 적용하여 조회를 할지 아니면 
	//				기존에 저장된 값을 이용하여 조회를 할지 구분 하기 위한 
private:
	BOOL m_bUseUserInputPeriod;
public:
	void SetUseUserInputPeriod(BOOL bUse)	{ m_bUseUserInputPeriod = bUse;	}
	BOOL GetUseUserInputPeriod()			{ return m_bUseUserInputPeriod;	}
	//<<<<<<<<<----------------------------------------------------------------------

public:	
	//툴바 이미지 리스트
	CImageList* m_pIL_W20H20;
	CImageList* m_pIL_W31H20;
	CImageList* m_pIL_W17H20;
	CImageList* m_pIL_W46H20;
	CImageList* m_pIL_W50H20;

	//Reason	:   현재 선택된 차트의 종목명 Set & Get
	CString m_strJonMokName;  // 05. 08. 30 added by sy, nam
	void	SetCurCodeNameToControl(CString strName);
	CString	GetCurCodeNameFromControl() { return m_strJonMokName; }

	//종목 코드로 종목명 가져오기.
	CString GetJongMokNameByCode(CString strCode, LPCTSTR lpszTRCode = NULL);
	int GetMarketTypeFromTRCode(LPCTSTR lpszTRCode);
	//<<<<<<<<<----------------------------------------------------------------------


	//환경동기화시 차트들이 순차적으로 Selelct가 되면서 
	//UMSG_SETINITIALCHARTDATA 메세지를 발생시켜서 
	//InputBarDlg Class내에 호가 TR을 날리므로 
	//이때는 강제로 호가 TR을 날리지 못하도록 한다.
//@solomon	BOOL m_bCanRequestHogaTR; //호가 TR을 날릴 수 있는지 여부의 상태 
//@solomon	void SetStopRequestHogaTR(BOOL bCanRequest) { m_bCanRequestHogaTR = bCanRequest; }
	//<<<<<<<<<----------------------------------------------------------------------

	//------------------------------------------------------------------------
	//Color Table 적용시 호출 - 차트는 ColorTable 안 쓴다.
	void ChangeButtonColor();
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	//Comment	:	선물옵션 차트에서만 적용한다.
	//				종목의 통합이 안되어 부득이 따로 따로 구분된 형태로 동작하도록 flag값을 갖는다.
	//------------------------------------------------------------------------		
	enum	{KOSPI_FUTOPT, KOSDAQ_FUTOPT, STOCK_FUTOPT,  STAR_FUTURE	};
	int		m_nFutOptType	; // 0 - 일반 선물옵션 , 1 - 코스닥 옵션 , 2 - 주식옵션 
	DWORD	m_dwDataKind	;
	//CString m_strFutOptType	; // KOSPI_STRFUTOPT - 일반 선물옵션 , KOSDAQ_STRFUTOPT - 코스닥 옵션 , STOCK_STROPTION - 주식옵션 

private :
	CString		m_strCode;
	char		m_cChartShape;   //분할비교/중첩비교/누적등라률 중첩비교.

public:
	void	SetCurrentCode(CString strCode);
	CString GetCurrentCode();

	CString		m_strCodeFromOutside;
	CString		m_strOldCode;

	void		InitBaseInfo(CBaseDesk2* pBaseDesk, BOOL bRequestAP = TRUE); //CodeInput

	void		SetKeyCodeChange(WPARAM wParam, LPARAM lParam);	
	//008 {{ KOSCOM 코드콤보 코스피업종/코스닥업종으로 구분됨
	void		InitUpjongCodeCombo(UINT nUpjongType);		// 코스피/코스닥구분에 따라 코드콤보 생성
	//008 }}

	int m_nCurSelCol;
	int m_nCurSelRow;
	int m_nTick;
	int m_nSec;
	int m_nDay;
	int m_nMin;
	int m_nHour;
	CString m_strCfgFile;

	int		m_nMarketType;  // 0 - 주식차트, 1 - 업종 차트 , 2 - 선물옵션차트
	void	SetMarketType(int nMarketType) { m_nMarketType = nMarketType; } 

	BOOL	ChangeMarket(UINT nMarketType, BOOL bUpdateCbMarket = TRUE, BOOL bRequestToServer = FALSE);
	CInputBarDlg(CWnd* pParent = NULL);   // standard constructor

	void	SetpChartMng(CWnd *pWnd) { m_pChartMng = pWnd; }
	void	SetBaseDesk(long lKey) { m_lBaseDesk = lKey;} 
	void	HideLeftBarBtn(int nCmdShow);
//	BOOL	IsMiniBar() { return m_bIsMiniBar;} 
// Dialog Data
	//{{AFX_DATA(CInputBarDlg)
	enum { IDD = IDD_INPUTBAR };
	CILButton	m_btnRetry;
	CComboBox	m_cmbBong;
//	CILButton	m_btPeriod;
	CExComboBox	m_cbMarket;
	CExComboBox	m_cbLineType;
	CButton		m_btnStop;
	CExComboBox	m_cmbChartViewType;
	CComboBox	m_cmbChartType;
	CILButton	m_chkMinute;
	CILButton	m_chkDay;
	CILButton	m_chkWeek;
	CILButton	m_chkMonth;
	CILButton	m_chkSec;
	CILButton	m_chkTick;
//@Solomon	091111SM033	-->
//	CExEdit		m_editPeriod;
	CEditUsr	m_editPeriod;
//@Solomon	091111SM033	<--
	CEditUsr	m_editCntOfDatAll;
	CStatic		m_stText;
	BOOL		m_bChkRevised;
	BOOL		m_bChkLog;
	CILButton	m_chkShowDivChart;
	CILButton	m_chkShowOverLappedChart;
	CILButton	m_chkShowNujukOverLappedChart;
	//}}AFX_DATA
//	CExCodeInput	m_ctrlCodeInput;
//	CRect m_rectCmb;

private:	

//	CILButton	m_chkExtBar2;
//	CILButton	m_chkExtBar1;
//	CILButton	m_btResetPeriod;
//	CButton			m_chkFixPeriod;
	CSpinButtonCtrl	m_spinPeriod;
	CSpinButtonCtrl	m_spZoomValue;
//	CILButton	m_btFixPeriod;
	CILButton	m_btRequestMore;
//	CILButton	m_chkRight;
	CILButton	m_chkLeft;
//	CILButton	m_btInc;
//	CILButton	m_chkExt2;
//	CILButton	m_chkExt1;
//	CILButton	m_chkExt6;
//	CILButton	m_chkFirst;
//	CILButton	m_chkFourth;
//	CILButton	m_chkThird;
//	CILButton	m_chkSecond;
	CStatic			m_stDiv;
	CEditUsr		m_editZoomValue;
//	CDateTimeCtrl	m_datePicker;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBarDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//CDrMLib_CDBMgr* m_pDBMgr;
	long m_lCntInView[8]; // 6->8로 변경 10초 30초 추가 05.09.28
	long m_nCntInViewHanHwa;
//	CBrush	m_brBkColor;
//	COLORREF m_crBk;
	UINT *m_pnLeftBarType;
	CWnd *m_pChartMng;
//	CAny2Code* m_pctrlCode;
	char m_chType;		// 틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)
	char m_chOldType;	// 2005.05.20 - 틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)
	int  m_nUnitPos;  // 2005.05.20 - 사용하지 않는다.
	BOOL m_bSettingInitData;
	CString m_strImageChartPath;
	CString m_strImagePath;
//	BOOL m_bIsMiniBar;
	UINT m_nPosCbMarketOld;
	long GetEndDate();
	void LoadValue();
	void SetExt(int nAct);
	void RemoveHistoryInfo();
	BOOL ChangeEditCode(BOOL bFocus, BOOL bDontPutInShared =FALSE);
	CString GetCodeNameAndGubun(CString csCode);
	BOOL AddHistoryInfoListUpjong(CString szKey, CString szName, BOOL bShareCodeList=TRUE);
	BOOL AddHistoryInfoListJongmok(CString szKey, CString szName, BOOL bShareCodeList=TRUE);
	BOOL AddHistoryInfoListForeign(CString szKey, CString szName, BOOL bShareCodeList=TRUE);

	BOOL LoadInfoFutureList();
	virtual void SetType(BOOL bSendChartMng = TRUE); // UpdateBy m_chType(틱(0)/분(1)/일(2)/주(3)/월(4)) and m_nUnitPos
//	CPopupList*		m_dlgPopupListForHistory;
	CHistoryList	m_lstHistoryInfo;
	CHistoryList	m_lstHistoryInfoUpjong;
	CHistoryList	m_lstInfoFuture;
	CHistoryList	m_lstHistoryInfoForeign;

	// 업종, 해외 코드 저장 - ojtaso (20080115)
	CString			m_strForeignCode;
	CString			m_strUpjongCode;

	long m_lBaseDesk;
	long GetBitFromCodeInfoUpjong();
	long GetBitFromCodeInfoJongmok();
	
	virtual void Init();
	virtual void LoadIniValue();
	virtual void SaveIniValue();

public:	
	// 생성될 때 한번만 셋팅됨

	int		m_nScreenType;
	void Init_Controls();
	//비교/중첩/등락 등의 차트타입설정.
	void SetCompareChartType();

//	void SetReplayEachSec(CString sTime);
//	void SetReplayTotalSec(int nTotalSec);
	
//	void ChangeToolBarStatusForReplay(int nReplayMode);
	void SetEditZoomValue(CString strValue);
	void ChangeChartCode(CString strCode);
//	void ChangeToolBarForReplayChart(BOOL bReplay);
//	void Replace_FutOptControl();
//	void Replace_ForeignChartControl();

	void SetFocusCodeInput();
	void SaveChartType_IniFile(int nType);

	void SetCheckRevised(BOOL bRevised);
	void Enable_PeriodControl(BOOL bEnable);
	void SetLeftBarType(UINT nLeftBarType);
//	void SetShowExtInputBarDlg(BOOL bShowExtInputBarDlg);
//	void UpdateBtNCbValue();
	void SaveValue();
	char GetType() { return m_chType;}
	long GetTypeNumber();
	void SetMode(int nMode);
	int  GetUnitPos() { return m_nUnitPos;	}

//	LONG m_lValueOfButton[CNTOFMAXBUTTONS];			// 일 주기
//	LONG m_lValueOfMinButton[CNTOFMAXBUTTONS];		// 분 주기
//	LONG m_lValueOfTickButton[CNTOFMAXBUTTONS];		// 틱 주기
//	LONG m_lValueOfHourButton[3];					// 일 주기
	long m_lValueOfCombo[CNTOFMAXCBITEMS];

	IMasterDataManagerLast* m_pDataManager;
	
	void SetCodeInit();
//@유진삭제	void SetUseReplayChartMode(BOOL bUseReplayChartMode) { m_bUseReplayChartMode = bUseReplayChartMode; }

	//2006.01.20 by LYH
	void SetAllPosition(int nType);

	//기간 고정일때 보이는 데이타수 변경하지 않음;
	BOOL m_bChkPeriod;
	int  m_nFixZoomValue;

	void SetViewCount(long nViewCount);
	int GetAllCount();
	int m_nUpjongType;

	int m_nUnitValue;
	void ResetUnitValue(BOOL bValueSet);

	BOOL SetCodeDragAndDrop(CString strCode, int nGetMarketType=-1);
//@유진삭제
//	int GetMarketType(CString strCode);
//	CString GetLastKey(int iKeyType);
//@유진삭제
//@유진추가
	CString GetLastHistoryCode(int nMarketType);
//@유진추가
	BOOL m_bChkSaveJipyoOneFile;
	//2007.02.08 by LYH
	BOOL m_bChkLoadSaveCode;
	//2007.07.23 by LYH
//@유진삭제
//	BOOL SetRecentCode();
//	CString m_strRecentCode;
//@유진삭제
//@유진추가
	void SetLastHistoryCode();
	int m_nCurrentMode;
	int GetCurrentMode(int nDefault = MODE_DEFAULT, int nMode = CHANGE_ITEM);
	
	void SetShareCodeChanged(BOOL bShareCodeChanged)	{ m_bIsShareCodeChanged = bShareCodeChanged; }
	BOOL GetShareCodeChanged()	{ return m_bIsShareCodeChanged; }
	BOOL m_bIsShareCodeChanged;
	
	BOOL GetChartViewType(CStringList& listViewType);
//@유진추가

	// 복수종목에서 선택된 차트 정보 표시 : 복수종목 - ojtaso (20070223)
	void InitChartMulitItem(ST_SELCHART* pSelChart);
	BOOL m_bSelectAction;	// 차트 선택이 변경되어 종목코드가 변경되는 경우 : 복수종목 - ojtaso (20070323)

	// 종목코드를 세팅할 것인지 : 복수종목 - ojtaso (20070612)
	BOOL m_bReceiveDataSetCode;
	// 복수종목의 Total Count를 멀티차트에 알려줌 : 복수종목 - ojtaso (20070704)
	void SetMultiTotalCount(int nTotalCount);

	void SetCompareChartItemMode();

	// 분차트 마지막 키인 경우 - ojtaso (20071226)
	void EnableRequestMoreButton(BOOL bEnable);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputBarDlg)
	virtual void OnCancel(){}
	virtual void OnOK(){}
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtleft();
	afx_msg void OnBtright();
	afx_msg void OnChkday();
	afx_msg void OnChkweek();
	afx_msg void OnChkmonth();
	afx_msg void OnChkminute();
	afx_msg void OnChksec();
	afx_msg void OnChktick();
	afx_msg void OnDeltaposSpinPeriod(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinzoomvalue(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtrequestdatmore();
	afx_msg void OnChkQueryOp();
	afx_msg void OnDestroy();
	afx_msg void OnBtinc();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtresetperiod();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnChkextbar();	
	afx_msg void OnBtinitial2();
	afx_msg void OnChkshowscale2();
//	afx_msg void OnChkyear();
//@유진삭제	afx_msg void OnChkDatalist();
//	afx_msg void OnDeltaposSpinDatecnt(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkRevised();
	afx_msg void OnSelendokCmbCharttype();
	afx_msg void OnButton1();
//@유진삭제 afx_msg void OnChkForeignMainCode(UINT nID);	
//@유진삭제 afx_msg void OnBtnForeignSendtr();
	afx_msg void OnSelchangeCmbType();
//	afx_msg void OnBtnPlayPause();
//	afx_msg void OnBtnStopReplay();
//	afx_msg void OnSelchangeCmbSpeed();
	afx_msg void OnSetfocusEditperiod();
	afx_msg void OnChangeEditperiod();
//	afx_msg void OnChkfirst();
//	afx_msg void OnChksecond();
//	afx_msg void OnChkthird();
//	afx_msg void OnChkfourth();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCheckDow();
	afx_msg void OnCheckNasdaq();
	afx_msg void OnCheckNikei();
	afx_msg void OnCheckSp500();
	afx_msg void OnCheckTwd();
	afx_msg void OnSelchangeCblinetype();
	afx_msg void OnSelchangeCbmarket();
	afx_msg void OnBtsetenvir();
//	afx_msg void OnBtcomparechart();
	afx_msg void OnSelchangeCodeInput();
//	afx_msg void OnBtSaveChart();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnChkDivChart();
	afx_msg void OnChkNujukOvrlppdChart();
	afx_msg void OnChkOvrlppdChart();
	afx_msg void OnBtnRetry();
	//}}AFX_MSG
//	afx_msg virtual void OnBtperiod();
	LRESULT OnSelBasedlgPopuplistctrl(WPARAM wParam, LPARAM lParam);
	LRESULT OnFromCodeList(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	void OnChkNujukOvrlppdChartA();
	void OnChkNujukOvrlppdChartB();
	void OnChkNujukOvrlppdChartC();

public:
	afx_msg void OnEnSetfocusEditFocus();
	afx_msg void OnBnClickedBtchangeupjong();

	void SetPeriodType(BOOL bSendChartMng=TRUE);
	void SaveInputPeriod();

	//@080522-001 선/봉유지.
	void ConfirmLineType();
	void Request_NewCode_ChartData(WPARAM wp, LPARAM lp);


	//@080522-003 TestCode
	void Test001(); //
	void Test002();
	void SetChTypeBtn(char chType);
	void Test004(char chType, int nTypeUnit);

	CShareInputData	*m_pShareInputData;
	void SetShareInputData();
	void SetMenuStyle();
	void RequestTR();
	void SendTR(LPCSTR szCode);

	CStringList	m_szRCodeList, m_szRCodeNameList;
	//BOOL m_bFirstSend;
	void GetRequestList();
	void SendNextTR();
	void RequestStepAll();
	void RequestStep2All();
	void CurrentTrError();

	// 등락률설정창에서 설정 변경이 있을 경우 호출됨
	void ConfirmDlgSetupData();

	HWND FrameMenuInsert(CMenu* _pChartMenu, CMenu* pFrameMenu);	//ChildFrame의 메뉴 Insert
	void FrameMenuShow(CPoint &ps);
	void ShowScreen1Menu(CPoint &ps ,int nScreenType=1);

	//등락률차트일 때 OninitDialog에서 타이머 호출에 의해서 불린다.
	void fnInitAfter();

	CDlgCompGroupData* m_pSelGroupData;	//현재 선택된 그룹
	void fnReLoadstDlgCompData();	// 정보를 다시 읽는다.

	// 돌려보기 시작/종료
	CStringList m_szTimerCodeList;
	CString m_szCurTimerCode;
	BOOL	m_bOldShowAll;
	void fnFluctTimerStart(BOOL bStart);

	// 돌려보기에서 현재코드 다음종목조회.
	void fnFluctTimerNextCode(CString &szCode, CString &szCodeName);

	// 돌려보기에서 현재조회하는 다음 코드를 조회한다.
	void fnFluctTimerNextCodeSend(BOOL bNextCheck=TRUE);

	CDlgCompSubItem* m_pSubItem;
	void fnSetUserSetting(LPCSTR szCode);

	// 현재 그룹의 첫번째 종목정보 구하기
	LPCSTR GetFirstGroupSelCode();

	// 종목이 현재 선택된 그룹에 속해있는지 확인한다.
	BOOL IsCodeExist(LPCSTR szCode);

	void _DoChkDivChart();	//기본셋팅처리.

private:
	enum	{Show_All=0, Show_Each};

//	BOOL GetInputCode(int nKey, long &dwIn, CString& szIn);
	//--> 등락률차트에서의 메뉴선택시 처리를 위해.
	//LPCSTR FnGetSelectGroup();
	//<--
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBARDLG_H__88AA28E0_917F_485D_BE10_0171E8C2E60B__INCLUDED_)

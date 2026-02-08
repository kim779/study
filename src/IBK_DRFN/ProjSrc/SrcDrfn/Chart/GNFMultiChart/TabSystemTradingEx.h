#pragma once

#include "TabDlg.h"

// CTabSystemTradingEx 대화 상자입니다.

#include "../Common_ST/DllSTLoader.h"
#include "../Include_ST/ISTControllerD.h"
#include "./control/XListCtrl.h"

#include "../../inc/ISTManager_GNF.h"				// 시스템트레이딩 관련된 컨텐츠 인터페이스
#include "../../inc/ISTRunManager_GNF.h"			// 전략실행관련 인터페이스 제공.

#include "DlgSTExOption.h"						// 성과분석 옵션관련.
//#include "./control/GfxOutBarCtrl.h"
#include "../../inc/ISTLoadManager.h"			// 전략로드관련 인터페이스 제공.
#include "../../inc/IAUDataMonitorMng.h"	// UUID_IAUDataMonitorManager : 콜백함수 받는데 사용
#include "../../inc/ISysTabExChartItem.h"
#include "../Common_ST/IGfxOutBarCtrl.h"
#include "../Common_ST/STTreeCtrl.h"//KHD 
#include "STDlgConfirm.h"				// 주문창.
extern HINSTANCE g_hInstance;

#include "./ChartItem.h"
#include "afxwin.h"
class CSysTabExChartItem : public ISysTabExChartItem
{
public:
	CSysTabExChartItem();

	CChartItem* m_pChartItem;

public:
	STDMETHOD_(long, GetChartItem)() ;
	STDMETHOD_(void, SetChartItem)(long dwNewChartItem) ;
	STDMETHOD_(BOOL, AddPacketAndGraph) () ;
	STDMETHOD_(BOOL, SetPacketData)(LPCTSTR szPacketName, long szData, long nLength, BOOL bReset) ;
	STDMETHOD_(void, SetNOnePageDataCount)(long propVal) ;
	STDMETHOD_(void, ShowInnerToolBar)(BOOL p_bShow) ;
	STDMETHOD_(BOOL, InvokeAddIn) (LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData) ;
	STDMETHOD_(BOOL, AddChartBlock) (LPCTSTR lpChartName) ;
	STDMETHOD_(CString, GetPacketName)();
};

class CTabSystemTradingEx : public CTabDlg
{
	//DECLARE_DYNAMIC(CTabSystemTradingEx)

public:
	CTabSystemTradingEx(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabSystemTradingEx();

	enum	// 초급/고급의 전략설정에도 같은 내용 정의되어있음.
	{
		Basic_STChange_Delete=0x10,		//초급전략의 삭제
		Basic_STChange_Add=0x11,		//초급전략의 추가
		Ext_STChange_SDDelete=0x20,		//고급전략의 사용자단위전략삭제
		Ext_STChange_SDAdd=0x21,		//고급전략의 사용자단위전략추가
		Ext_STChange_Delete=0x40,		//고급전략의 사용자조합전략삭제
		Ext_STChange_Add=0x41,			//고급전략의 사용자조합전략추가
	};

public:
	void InitControl();

	void RecalcLayout();
	CRect GetRectOfCtrl(INT nID,BOOL bInit=FALSE);

	void LoadInfo();

	BOOL OpenAUP();
	CWnd *LoadTradeAup(CString strFile, CWnd *pParent);
	void UnLoadSTList();

	// 다이얼로그 색상처리를 위해서 ...
	HBRUSH GetBkColorBrush() { return (HBRUSH)m_brBkColor.GetSafeHandle(); }
	COLORREF GetBkColor() { return m_crBk; }

	virtual void ChangeSkinColor(COLORREF clrBkgrnd);
	
// 대화 상자 데이터입니다.
	//{{AFX_DATA(CTabCodeSearch)
	enum { IDD = IDD_TABSYSTEMTRADINGEX };
	CILButton	m_BtnStwizardA;
	CILButton	m_BtnStoptimaA;
	CILButton	m_BtnStDelete;
	CILButton	m_BtnStanal;
	CILButton	m_BtnOption;
	//}}AFX_DATA
//	CILButton	m_BtnStwizardA;
//	CILButton	m_BtnStoptimaA;
//	CILButton	m_BtnStanal;

	long m_nHeightOfSearch;

	BOOL m_bShowOption;
	CDlgSTExOption*	m_pDlgOption;
	CImageList		m_ilSTIcon;
	CImageList		m_ImageOutbarIcon;
	CImageList		m_ImageListIcon;
	CImageList		m_ilW151H21;
	CStringArray	m_arSTPath;
	ISTLoadManager2* m_pSTLoadMnger;
	HINSTANCE		m_hSTConfigureWizardInst;

	CSysTabExChartItem	m_STExChartItem;	// CChartItem* 정보를 넘겨준다.
	BOOL			m_bEmptyWizard;			// 전략작성 실행여부.
	STMNG_ITEMINFO	m_RunSTInfo;			// 실행중인 전략정보
//	CString			m_szRunSTPath;			// 실제 차트에 운영중인 조합전략의 경로.
	BOOL			m_bSTBtn_Click;			// 버튼 그림 보이기위해 추가 KHD
	// 아웃룩바 형태 올리기.
//	CGfxOutBarCtrl	m_wndOutBar;
	// IGfxOutBarCtrl* m_pImpl;
	IGfxOutBarCtrl*	m_pWndOutBar;
	//CListBox		m_listSTList;
	void LoadOutlookBar();
	void LoadSTList();
	void AddSTFile(CListBox& rSTList, CString szPath);

	CString		m_szRootDir;
	CString		m_szDataDir;
	CString		m_szEncUserDir;
	int			m_nUserType;	// "1":HTS  "0":FRONT(지점)

	CSTBasicTree m_STBasicTree;
	CSTExtTree	m_STTree;
	void LoadSTList_SSStyle();		// 삼성스타일:초급/고급 혼합형.
	//
	BOOL IsSTSelected();
	void RunWizard(BOOL bSelectedDefault);			// 전략작성 로딩
	BOOL CanSTModule();								// 주식,선/옵에서만 전략가능.
	BOOL IsSTRun(LPCSTR p_szSTPath, LPCSTR p_szSTName, int p_nSTType);		// 전략이 실행중인지 알려준다.

	BOOL CheckOcx();	// TeeChart, MSChart등록
	BOOL CheckRegister(LPCSTR szProgID, LPCSTR szFileName);	// 성과분석 및 최적화를 위해 MSChart와 TeeChart의 등록여부를 확인한다.
	void CheckSTBasic();					//초급전략을 사용가능한 이름으로 확인

	//[[주문관련
	void TestJumun();

	//]]주문관련

	//..{ 데이터변경에 대한 통보를 받기위해서.
	// 시스템전략 변경시 KEY값은 "STLIST_MODIFIED"
	IAUDataMonitorManager*	m_pIAUDataMonitorManager; // 28 : 데이터변경처리를 위한 인터페이스
	long					m_dwIAUDataMonitorManager[2];
	static void CALLBACK DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData);
	//..}

private:
	void MakeSTDefaultDirectory();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CTabSystemTradingEx)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnStwizardA();
	afx_msg void OnBtnStoptimaA();
	afx_msg void OnBtnStanal();
	afx_msg void OnBtnStDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	LONG OnRmsgObItemClk(WPARAM wp, LPARAM lp);
	LONG OnRmsgObItemDBClk(WPARAM wp, LPARAM lp);
	LONG OnRmsgMultiST(WPARAM wp, LPARAM lp);
	LONG OnRmsgIndexAddinNotifyRawData(WPARAM wp, LPARAM lp);
	LONG OnRmsgIndexAddinNotifyBuySell(WPARAM wp, LPARAM lp);
	LONG OnRmsgIndexAddinNotifySignal(WPARAM wp, LPARAM lp);
	LONG OnRmsgDestroyConfirmWnd( WPARAM wp, LPARAM lp);
	LONG OnRmsgIsSTRunState( WPARAM wp, LPARAM lp);


public:
	void ShowHideStOptimaA(BOOL bChange, BOOL bShopOption, int nOffset=0);
	void HideStOptimaA();
	void STClear();

	BOOL Run_STDLL(LPCSTR szDll);
	void DirectRunST(LPCSTR szSTFile);	// FullPath로 ST실행.
	void RunMultiST();		// 조합전략을 실행한다. sd(sdd)디렉토리에 존재하는 ST파일.
	CString m_szImsiRunSTFile;

	// 강세약세, 지표 실행
	LRESULT RunIndexSD(int p_nSTRunType, STMNG_ITEMINFO* p_pArg);

	// 강세약세, 지표등의 설정창을 연다.
	int DoDlgSysVarOption(CStringList* p_dwInput, LONG dwOutput);


public:
	class CSTInterface : public ISTInterface
	{
	public:
		STDMETHOD_(void*, GetInterface)(int nID);
		STDMETHOD_(LONG, GetOutBarColor) (COLORREF& clrOLBkgrnd, COLORREF& clrOLOutLine);
	}m_xSTInterface;
	friend class CSTInterface;

	class CSTAnalysisInput : public ISTAnalysisInput
	{
	public:
		STDMETHOD_(void, SetUserData)(BOOL bUse, ISTUserData* pInterfaceUserData) ;	// TRUE 일경우에는 ISTUserData* 값을 세팅해줘야 한다.
		STDMETHOD_(BOOL, IsUseUserData)() ;					// 사용자가공 데이터 사용여부
		STDMETHOD_(ISTUserData*, GetUserDataInterface)() ;	// 시간최적화일 때는 ISTAnalysisInput*에서 제공하는 GetChartData 대신에
															// ISTUserData*에서 제공하는 GetChartData사용한다.
															// 왜냐면 가공된 데이터를 사용하기때문에

		// ID별 데이터
		STDMETHOD_(COleVariant, GetData)(int nID)  ;

		// 0: 종목코드 , GetCode(0)
		STDMETHOD_(CString, GetCode)() ;

		// 종목코드명, GetCode(1)
		STDMETHOD_(CString, GetCodeName)() ;

		// 종목코드 유형 : 0:주식, 1:선물, GetCode(2)
		STDMETHOD_(int, GetCodeType)() ;

		// 전략명, GetCode(3)
		STDMETHOD_(BOOL, GetSTInfo)(STMNG_ITEMINFO* pInfo) ;

		// 갯수, GetCode(4)
		STDMETHOD_(int, GetCount)() ;

		// 기준일, GetCode(5)
		STDMETHOD_(CString, GetEndDay)() ;

		// 분할수, GetCode(6)
		STDMETHOD_(int, GetDivideCnt)() ;

		// 분할 상태, GetCode(7)
		// 틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)
		STDMETHOD_(int, GetDivideState)() ;

		// 분할 상태일 때의 하위 선택 1, 5, 10, 15 버튼으로 표시된 부분.
		// 리턴값 1(0),5(1),10(2),15(3)
		STDMETHOD_(int, GetUnitPos)();

		// 중첩율, GetCode(8)
		STDMETHOD_(double, GetDeivideRate)() ;

		// 데이터갯수 리턴.
		// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
		STDMETHOD_(int, GetChartData)(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwCLose,LONG& dwVolume, LONG& dwDataExt) ;

		// 분할여부
		STDMETHOD_(BOOL, IsDivide)() ;

		// 컨트롤을 얻어온다.
		// nID 0(ChartOcx), 1(CListCtrl)
		STDMETHOD_(LONG, GetControl)(int nID);

		// 20081016 박승룡 - 8진법 변환처리
		STDMETHOD_(CString, GetPacketType)();

		//>> 20081203 박승룡 - 30초 TimeFormat 처리
		STDMETHOD_(BOOL, Is30SecUnit)() ;
		//<< 20081203 박승룡 - 30초 TimeFormat 처리
	public:
		STMNG_ITEMINFO	m_Info;
		STMNG_ITEMINFO	m_InfoForAnalysis;

		BOOL	m_bUseUserData;				// TRUE 일경우에는 ISTUserData* 값을 세팅해줘야 한다.

		ISTUserData* m_pInterfaceUserData;	// 시간최적화일 때는 ISTAnalysisInput*에서 제공하는 GetChartData 대신에
		// ISTUserData*에서 제공하는 GetChartData사용한다.
		// 왜냐면 가공된 데이터를 사용하기때문에


		CSTAnalysisInput();

	}m_xSTAnalysisInput;
	friend class CSTAnalysisInput;
//	STMNG_ITEMINFO	m_LastRunInfo;		// 전략,지표,강세약세 등 최종 실행되었을 때의 정보,070215

	class CSTRunManager : public ISTRunManager
	{
	public:
		STDMETHOD_(int, RunST)(BOOL bImsiRun, CString &szRunFile, LONG pInData);
		STDMETHOD_(LONG, GetSelectedSTInfo) () ;
		STDMETHOD_(LONG, GetViewHandle) () ;
		STDMETHOD_(LONG, GetOutBarColor) (COLORREF& clrOLBkgrnd, COLORREF& clrOLOutLine);
	} m_xSTRunManager;
	friend class CSTRunManager;

//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnBnClickedStchkdiv();

	afx_msg void OnStshowoption();

	// 주문신호 발생시 소리
	void PlayOrderSound(int nOrderType, BOOL bBuySound, LPCSTR szBuyFile, BOOL bSellSound, LPCSTR szSellFile);
	BOOL IsAllowJumun(CSTOrderRData_EG *pInData, BOOL bShowMsg=FALSE);
	BOOL IsAllowJumunBefore(LPCSTR szSTFile, CString& szMsg);	//전략적용전에 주문가능여부 체크
	void CallJunmunDlg(CSTOrderRData_EG *pInData);
	CMap<CConfirmWnd*, CConfirmWnd*, long, long> m_mapConfirmWnd;
	
	class CSTDlgConfirmSendJumun : public ISTDlgConfirmSendJumun
	{
		STDMETHOD_(CWnd*, GetParent)();
		STDMETHOD_(HWND, GetHwndScreenView)();
//		STDMETHOD_(LRESULT, SendJumun)(WPARAM wp, LPARAM lp);
	}m_xSTDlgConfirmSendJumun;
	friend class CSTDlgConfirmSendJumun;

};

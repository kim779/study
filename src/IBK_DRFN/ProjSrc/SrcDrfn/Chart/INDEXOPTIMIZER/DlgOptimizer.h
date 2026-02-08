#if !defined(AFX_DLGOPTIMIZER_H__7322843D_469D_45B4_8BEB_6DC808FE3CA7__INCLUDED_)
#define AFX_DLGOPTIMIZER_H__7322843D_469D_45B4_8BEB_6DC808FE3CA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptimizer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptimizer dialog
#include "resource.h"
#define G_MINUTE	1
#define G_DATE		2
#define G_TICK		3

//Dlg
#include "DlgResult.h"
//#include "DlgSonikChart.h"
//#include "DlgSooikChart.h"
#include "TabEarnChart.h"
#include "TabSonikChart.h"

#include "DlgTrade.h"
//Control
#include "GridCtrl_src/GridCtrl.h"
//#include "Control/XTabCtrl.h"
#include "Control/xSkinButton.h"
//#include "../../COMMON_BASE/TrComm/TrComm.h"
#include "../../inc/ExOCXDLL.h"
//KHD : 컨트롤 관리 구조체

//KHD : 아웃룩바 올리기 : 
#include"../Common_ST/STGfxOutBarCtrl.h"
#include "../../inc/ISTLoadManager.h"
#include "afxwin.h"
#include "../../inc/ISTManager.h"				// 시스템트레이딩 관련된 컨텐츠 인터페이스
#include "../Common_ST/DataItem.h"
#include "../Common_ST/SDataItem.h"
//////////////////////////////////////////
//구분//
#define TIME_OPTIMIZER 1003 //시간
#define INDEX_OPTIMIZER 1001 //지표
#define VAL_OPTIMIZER 1002//변수

#include "DlgTimeSetup.h"	// 06.09.06 [alzioyes]테스트 모듈임.

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExDrawButton.h"
#include "../../inc/ExStatic.h"
#include "../../inc/ExComboBox.h"

#include "../Common_ST/STTreeCtrl.h"		//@@@06.12.07
typedef struct ST_VALUE {
	CStatic *stStatic;
	CSpinButtonCtrl *stSpinBtn;
	CExEdit	*stEdit;
	CRect   rt;//Edit 컨트롤의 현재 위치..
} ST_VALUESET;


typedef struct ST_RUNVALUE {
	int start;
	int end;
	BOOL bNumber;
} ST_RUNVALUESET;

typedef struct ST_GRIDDATA{
	CString ByunSu;
	/////////////////////
	CString RateAccm; //수익률 7:0
	CString AAerProf;//1
	CString CurPosRate;//2
	CString RateCurrent; //총손익1:3
	CString ProfitAll;//총이익3:4
	CString LossAll ;//총손실 4:5
	CString TotalPL;//6
	CString MaxProfit;//최대이익5:7
	CString MaxLoss;//최대손실6:8
	CString LossSavg;//건별평균손익2:9
	CString AverProf;//10
	CString AverLoss;//11
	CString TotalEnt;//12
	CString NoCleanTotal;//13
	CString ProfitFactor;//??8??
	CString CntAll;//거래수 10:15
	CString ProfTrace;//16
	CString LossTrade;//17
	CString RateProfitLoss;//승률9:18
	CString PTRate;//19
	CString LTRate;//20
	CString PriodSign;//21
	CString MaxConProfN;//22
	CString MaxConLossN;//23
	CString SOverRate;//24
	CString MaxConProf;//25
	CString MaxConLoss;//26

	CString TotalStick;//27
	CString PTStick;//28
	CString LTStick;//29
	CString PriodParty;//30
	CString EntryStick;//31
	CString CleanStick;//32
	CString AtStick;//33
	CString PTAStick;//34
	CString PLTAStick;//35
	///////////////////////

} ST_GRIDVALUEDATA;

typedef struct ST_STRGRIDDATA{
	CString  Byunsu;
	CString  strGridData;
} ST_STRDATA;
//KHD : 2006.09.15//////////////
//변수최적화시 각 변수값을 세팅하기 위해 만든 구조체
typedef struct ST_VALUEOPTIMAZE{
	CString nTitle ; //타이틀값
	double nStart; // 시작값
	double nEnd;   // 마지막값
	double nAdv;   // 증가값
	int nDataType; // 데이터 타입 
	//추후 추가 :최대 , 최소
	double nMax;
	double nMin;
} ST_VALOTM;
//////////////////////////////

class CDlgResult;
class CDlgTrade;
class CDlgSonikChart;
class CTabSonikChart;
class CDlgTimeSetup;
class CDlgOptimizer : public CEscDialog
{
// Construction
public:
	CDlgOptimizer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOptimizer)
	enum { IDD = IDD_DLG_OPTIMIZER };
	CExDrawButton	m_CtlAddAll;
	CExDrawButton	m_CtlAddOne;
	CExDrawButton	m_CtlDelAll;
	CExDrawButton	m_CtlDelOne;
	CExDrawButton	m_BtnStop;
	CExDrawButton	m_BtnOpt;
	CXTabCtrl	m_CtlTab;
	/*CxSkinButton	m_BtnDef;
	CProgressCtrl	m_CtlProgress;
	CStatic	m_stMesu;
	CStatic	m_stMedo;
	CEdit	m_CtlEditMesu;
	CEdit	m_CtlEditMedo;
	CSpinButtonCtrl	m_SpinMesu;
	CSpinButtonCtrl	m_SpinMedo;

	CExComboBox		m_CtlComMesu;
	CExComboBox		m_CtlComMedo;
	CSpinButtonCtrl	m_CtlBong;
	CSpinButtonCtrl	m_CtlGijun;
	CEdit	m_CtlEditGijun;
	CEdit	m_CtlEditBong;
	CComboBox	m_CtlComIndex;

	CExStatic		m_CtlStatic6;
	CExStatic		m_CtlStatic5;
	CExStatic		m_CtlStatic4;
	CExStatic		m_CtlStatic3;
	CExStatic		m_CtlStatic2;
	CExStatic		m_CtlStatic1;
	CSpinButtonCtrl	m_CtlSpin6;
	CSpinButtonCtrl	m_CtlSpin5;
	CSpinButtonCtrl	m_CtlSpin4;
	CSpinButtonCtrl	m_CtlSpin3;
	CSpinButtonCtrl	m_CtlSpin2;
	CSpinButtonCtrl	m_CtlSpin1;
	CEdit	m_CtlEdit6;
	CEdit	m_CtlEdit5;
	CEdit	m_CtlEdit3;
	CEdit	m_CtlEdit4;
	CEdit	m_CtlEdit2;
	CEdit	m_CtlEdit1;
	int		m_EditGijun;*/
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptimizer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptimizer)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//afx_msg void OnButton2();
	//afx_msg void OnSelchangeComIndex();
	//afx_msg void OnBtnDefault();
	afx_msg void OnBtnOpt();
	//afx_msg void OnRadioDay();
	//afx_msg void OnRadioMinute();
	afx_msg void OnTimer(UINT nIDEvent);
	//afx_msg void OnSelchangeComMedo();
	//afx_msg void OnSelchangeComMesu();
	afx_msg void OnBtnStop();
	//}}AFX_MSG
	LRESULT OnSTDataManagerMessagge( WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnSetKeyCodeChange(WPARAM , LPARAM );
	LRESULT OnRmsgObItemDBClk(WPARAM wp, LPARAM lp);//아웃룩바 메세지받기
	LRESULT OnRmsgGetDllSize(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	//AP 관련 //////////////////////////////
	CIStrategyItem *m_pStrategyItem;
	CStrategyLoader m_strategyLoader;
	CISDataItem *m_pDataItem;
	
	////////////////////////////////////////
private: short m_STMessageType;//0:IndexOptimizer 1: Dlg-Result 2: Dlg-Trade
public: //Dll Load 
	// TESTING.....start 06.09.06[alzioyes]
	CDlgTimeSetup*	m_pDlgTimeSetup;
	void CreateTImeSetupDlg();
	void DestoryTImeSetupDlg();
	// TESTING.....end

	HWND			m_hSocket;					// 소켓핸들
	HWND			m_hMainFrame;				// CMainFrame의 Handle
//	CDBMgr*			m_pDBMgr;
//	CDrMLib_CDBMgr  m_IDrMLib_CDBMgr;
//	CBaseInfo	m_BaseInfo;
//	CDrMLib_CBaseInfo m_BaseInfo;
	CString			m_strTrCode;				// 화면번호
	CString			m_strUserID;
	CString			m_strRootPath;
	CString			m_strUserDir;
	CString			m_strDataDir;
	CString			m_strImageDir;

	void	FormInit();
	void	FormLoad();
	void	SetSTMessageType(short Smt);
//////////////////////////////////////////////////////////////////////////

	CBrush	m_brBkColor;
	COLORREF m_crBk;

	CGridCtrl m_GridStatus;
	CGridCtrl m_GridTap;
	void InitGrid();
	void InitCodeInfo();
	void ReSize();
	void AutoSizeColumns();

//TabDlg
	CDlgResult		*m_DlgResult;
	CTabSoonikChart  *m_DlgSooik;
	CTabSonikChart   *m_DlgSonik;
	CDlgTrade		*m_DlgTrade;

	CArray<CDialog*,CDialog*> m_arrayTab;
	CArray<ST_VALUESET*,ST_VALUESET*> m_arrayValue;
	void InitTab();
	void InitControls();

//ControlSet
	void RemoveAllControlSet();
	CIStrategyItem* GetStrategyItem(){return m_pStrategyItem;}

	void StartOpt();  // 쓰레드 상태에서 최적화실행
public:
	
		long m_lSendNum;
		BOOL m_bReceive;
		int m_nTest;
		int m_nTimer;
		int m_nTimer2;
		CStringArray m_stArrayPath;
		CStringArray m_stArraySDFileName;
		CString m_Code; 
		CString m_Name;
		CString m_StragyName;

		void InitStrategy();
		BOOL CalculateSendData();
		CString GetJMNameAndGubunFromCode(CString csCode);

		int m_nTimeState;//분,일,시,분
		//봉 및 기준선 관련 
		CString m_strGijun;
		short m_nGijun;//Min:1 Day: 2 Tick:3
		BOOL IsCurrentChartData(CString strValue);
		BOOL IsNumber(CString str,int Index);
		void SpreadGridData();
		CArray<ST_RUNVALUESET,ST_RUNVALUESET> m_ArRunvalue;
		CArray<ST_GRIDVALUEDATA,ST_GRIDVALUEDATA> m_ArGridData;
		CArray<ST_STRGRIDDATA, ST_STRGRIDDATA> m_ArstrGridData;
		CStringArray m_strNum;// x,y,z 값으로 파싱되기
//		CStringArray m_strArSendData;
		
		//Medo Mesu
		void MedoMesuComboDataParsing();
		CString SearchFileToData(CString strIndex, CString strPath);
		void SetControlState(BOOL State);
		//Skin
		void InitSkin();
public:
	//KHD : 아웃룩바 2006.08.30
	CSTGfxOutBarCtrl	m_wndOutBar;
	ISTLoadManager2* m_pSTLoadMnger;
//	CImageList		m_ilSTIcon;     
//	CImageList		m_ilW190H21;
	CImageList		m_ImageListIcon;
	CImageList		m_ImageOutbarIcon;
	CStringArray	m_arSTPath;
	
	void LoadOutlookBar();
	void LoadSTList();
	void UnLoadSTList();
//OutlookBar END
	afx_msg void OnBnClickedBtnAddall();
	afx_msg void OnBnClickedBtnAddone();
	afx_msg void OnBnClickedBtnDelall();
	afx_msg void OnBnClickedBtnDelone();
	CListBox m_listST;//전략 구조를 저장하고 있는 리스트
	//KHD : 구분 
	int m_nOptimizerDiv;
	long m_pChartInterface;
	CString m_stCode;
	int m_nCount;
	CString m_stEndDate;
	CString m_stCodeName;
	int m_nCodeType;
	BOOL m_bGridClick;
	void HideControl();//최적화 옵션에 따라 다르게 생성

	class CSTUserData : public ISTUserData
	{
	public:
		// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
		STDMETHOD_(int, GetChartData)(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwCLose,LONG& dwVolume, LONG& dwDataExt, int& nBong) ;
	}m_xSTUserData;
	friend class CSTUserData;	
	int m_nBong;
	//변수최적화
	CArray<ST_VALOTM,ST_VALOTM>   m_ValueOptimizerArray;
	CString m_stPath;//변수최적화에서 사용되며 각각의 다른 최적화와 루틴이 다르기때문에 선언!!
	int m_STValNum;// 변수최적화의 변수갯수 
	afx_msg void OnLbnDblclkListSt();
public:
	//@@@06.12.07[[
	HINSTANCE			m_hSTConfigureWizardInst;
	IGfxOutBarCtrl2*		m_pWndOutBar;
	CSTBasicTree		m_STBasicTree;
	CSTExtTree			m_STTree;

	void fnBnClickedBtnAddone(STMNG_ITEMINFO* pArg);
	void LoadSTList_SSStyle();
	//LONG OnRmsgSTTreeItemClk(WPARAM wParam, LPARAM lParam);
	LONG OnRmsgSTTreeItemDBClk(WPARAM wParam, LPARAM lParam);
	//@@@06.12.07]]

	CFont	m_Font;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIMIZER_H__7322843D_469D_45B4_8BEB_6DC808FE3CA7__INCLUDED_)

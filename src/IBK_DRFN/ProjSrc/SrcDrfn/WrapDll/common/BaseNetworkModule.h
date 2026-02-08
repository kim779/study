#if !defined(AFX_BASENETWORKMODULE_H__B4D69054_23DD_46D1_BADC_9E6CA72C5465__INCLUDED_)
#define AFX_BASENETWORKMODULE_H__B4D69054_23DD_46D1_BADC_9E6CA72C5465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseNetworkModule.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CBaseNetworkModule window
#include "dataset.h"
#include "FONetworkModule.h"

class CNetworkModule;
class CCommonFrameWorkWnd;
class CClosingNoticeDlg;

//20080316 이문수 >>
class CKeyToWndInfo
{
public:
	int			m_nKey;
	CUIntArray	m_hWndArray;

public:
	CKeyToWndInfo()
	{
		m_nKey = -1;		
	}

	CKeyToWndInfo(int nKey, HWND hWnd)
	{
		m_nKey = nKey;
		m_hWndArray.Add((UINT)hWnd);
	}

	CKeyToWndInfo(CKeyToWndInfo & Info)
	{
		m_nKey = Info.m_nKey;
		for(int i = 0 ; i < Info.m_hWndArray.GetSize(); i++)
			m_hWndArray.Add(Info.m_hWndArray.GetAt(i));
	}

	CKeyToWndInfo & operator=(CKeyToWndInfo & Info)
	{
		m_hWndArray.RemoveAll();
		m_nKey = Info.m_nKey;
		for(int i = 0 ; i < Info.m_hWndArray.GetSize(); i++)
			m_hWndArray.Add(Info.m_hWndArray.GetAt(i));
		return *this;
	}

	void AddHWnd(HWND hWnd)
	{
		m_hWndArray.Add((UINT)hWnd);
	}
};
typedef CList < CKeyToWndInfo , CKeyToWndInfo & > CKeyToWndList;
//20080316 이문수 <<

class CBaseNetworkModule : public CWnd
{
// Construction
public:
	CBaseNetworkModule(CWnd* pParent);

// Attributes
public:

	//20080316 이문수 >>
	CKeyToWndList				m_KeyToWndList;
	//20080316 이문수 <<

	CWnd				*		m_pHistoricalWnd;


	CCommonFrameWorkWnd *		m_pCommonFrameWork;

	CClosingNoticeDlg*			m_pClosingDlg;

	int							m_nTradeTimeType;//장종료시간 여부.(1-장중, 2-장종료)


	BOOL						m_bEnableNetwork; //false이면 통신 중임.


	CItemInfoArray				m_ETFItemInfoArray;//etf 종목 정보 배열.

	
	CAverageIV					m_AverageIV;



	CString						m_strExePath;
	
	
	
	BOOL						m_bReceiveBaseData;

	
		
	CBaseDataSet				m_BaseDataSet;



	CNetworkModule *			m_pNetwork;




	CPtrArray					m_WndArray;




	CItemInfoArray				m_FutureMonthInfoArray;




	CUnderlyingAssetsInfoArray	m_UnderlyingAssetsInfoArray;




	CFONetworkArray				m_FONetworkArray;
	



	//kp200, 선물4개종목, 주식7개.....
	CDataArray					m_RTDataArray;


	CWnd*						m_pFrameWnd;
	CStringArray				m_strRqCodeArray;	
	CString						m_strHomeDir;
	CString						m_strUser;
	CString						m_strId ;
	CString						m_strATM;
	CString						m_strBaseAssetPrice;

	COLORREF					m_clrBack;
	CFont*						m_pFont;
	
	CPriceProgressData *		m_pPastDataOfUnderlyingAssets;

	int							m_nBizDayCount;			// 영업일수

	CAverageIVSet				m_stTodayVI;

	HWND						m_hVolateSelDlg;		// 변동성 선택 다이얼로그 핸들

	//20080314 이문수 >>
	//잔존일의 시간가치를 계산하기 위해서 처리
	//장전, 장후에 시간데이터가 어떻게 들어오는지 확인할 것.
	CString						m_strKP200Time;
	double						m_dTimeLoss;
	//20080314 이문수 <<

// Operations
private:
	void DestroyWndSelf();
	
public:

	//20080314 이문수 >>
	double  CalcTimeLoss();
	//20080314 이문수 <<

	//20080316 이문수 >>
	BOOL RequestData(CString strCode, HWND hRqWnd);
	//20080316 이문수 <<

	//>> 20080410 정용허
	BOOL IsApplyPredictPrice();
	//<<

	CVolateInfo * GetVolateInfoByItemCode(CString strCode);

	CVolateInfo * GetVolateInfoByUnderlyingAssetsCode(CString strUnderlyingAssetsCode);

	double GetOptIVByUnderlyingAssetsCode(CString strUnderlyingAssetsCode);

	void SetHistoricalWnd(CWnd * pHistoricalWnd);

	int GetTradeUnitByUnderlyingAssetsCode(CString strUnderlyingAssetsCode);

	CString GetKOSPIPrice();

	CString GetUnderlyingAssetsShortCode(CString strUnderlyingAssetsCode);

	CCommonFrameWorkWnd * GetCommonFrameWork(){ return m_pCommonFrameWork;}

	//장종료여부를 알 수 있다.(1-장중, 2-장종료)
	int GetTradeTimeType(){ return m_nTradeTimeType;}

	//옵션 종목코드를 넣어준다.
	CString GetTotalAverageIV(CString strCode);

	void RequestAverageIV();

	BOOL ReceiveAverageIV(WPARAM wParam, LPARAM lParam);

	CDataSet * GetDataSetOfUnderlyingAssets(CString strUnderlyingAssetsCode);

	CUnderlyingAssetsInfo * GetUnderlyingAssetsInfo(CString strCode);

	CString GetRecentFuture();

	BOOL RqItem(BOOL bStart = FALSE);

	CString GetRemainDay(CString strCode);

	//20080325 이문수 >>
	CString GetRemainDayForMargin(CString strCode);
	//20080325 이문수 <<

	CString GetIndex(CString strCode);

	void Sort(CItemInfoArray & ItemInfoArray);

	void SortSOptItem();

	//strYM (2003년 2월물 -> "302"로 표현 (참고:옵션종목코드의 4번째 자리부터 3개 "201302090"))
	CString GetATM(CString strUnderlyingAssetsCode, CString strYM);

	int GetTradeUnit(CString strCode);

	CString GetUnderlyingAssetsLastPrice(CString strUnderlyingAssetsCode);

	CString GetUnderlyingAssetsPrice(CString strUnderlyingAssetsCode);

	CString GetUnderlyingAssetsPriceForMargin(CString strUnderlyingAssetsCode);

	BOOL RequestData();

	BOOL ReceiveData(WPARAM wParam, LPARAM lParam);

	void AddRTData();

	BOOL RequestKP200Data();

	void AddFONetwork(CFONetworkModule * pFONetwork);

	void DelFONetwork(CFONetworkModule * pFONetwork);

	void SetAppPath();

	void SetWnd(CWnd * pWnd);

	BOOL CreateBaseNetworkWnd(int nID, CWnd * pParentWnd);

	BOOL RegisterWndClass(HINSTANCE hInstance);

	BOOL RequestBaseData();

	BOOL ReadETFCode();

	BOOL ReadOptCode();

	BOOL ReadFutCode();

	BOOL ReadSOptCode();

	BOOL ReceiveBaseData(WPARAM wParam, LPARAM lParam);
	
	void GetOptionYearMonthArray(CString strAsset, CStringArray *strYearMonthArray);

	LONG ReceiveDataPIDO(WPARAM wParam, LPARAM lParam);

	CString Variant(int comm, CString data = "");

	void RequestTodayAverageIV(HWND hVolSelWnd, CString strShortCode);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseNetworkModule)
	protected:
	//virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseNetworkModule();

	// Generated message map functions
//	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
//	afx_msg long OnReceiveRTM(WPARAM wParam, LPARAM lParam);	
protected:
	//{{AFX_MSG(CBaseNetworkModule)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg long OnDialogDestroyed(WPARAM wParam, LPARAM lParam);	
	afx_msg long OnCloseBaseNetworkModule(WPARAM wParam, LPARAM lParam);	
	afx_msg long OnShowCloseDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg long OnGetBaseNetworkModule(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASENETWORKMODULE_H__B4D69054_23DD_46D1_BADC_9E6CA72C5465__INCLUDED_)

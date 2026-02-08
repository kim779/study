#if !defined(AFX_STDDIALOG_H__CF66AE4F_AD1A_4057_B00E_F9FAA4C3A30F__INCLUDED_)
#define AFX_STDDIALOG_H__CF66AE4F_AD1A_4057_B00E_F9FAA4C3A30F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------------
//Comment	: 주석처리. 주석처리 내용중 명확하지 않은 부분은
//			  두리정보통신 오근문 대리한테 정확히 확인한다.	
//			  gm0604@orgio.net
//
//Date		: 2005. 04. 20
//Author	: Sang Yun, Nam
//-----------------------------------------------------------------------------------

// StdDialog.h : header file
//
//#include "./control/ParentChartDlg.h"
#include "../chart_common/OutsideCommonInterface.h"
//#include "../chart_common/DataList/RealChartData.h"

#ifndef __HANHWA_CHART_REPLAY_INFO_H
	#include "../chart_common/ReplayChartInfoDef.h"
#endif

#include "Resource.h"
#include "xySplitterWnd.h"
#include "ChartMng.h"
#include "LeftBarDlg.h"
#include "InputBarDlg.h"
#include "ExtInputBarDlg.h"
#include "../../inc/PcTrMngHelper.h"
#include "./control/RGBButton.h"


#include "../../inc/IAUPartnerMng.h"		//@090922 For IPartnerSite

//#include "../../ToolKit/MainDll_Interface/Platform.h"	// 플랫폼 관련 부분 모음.
//#include "../../ToolKit/MainDll_Interface/WndObj.h"
//#include "../../../COMMON_BASE/TrComm.h"
//#include "../../INCLUDE_BASE/DllLoadDef.h"
//#include "../../common_base/trComm/TRComm.h"
//#include "../../ToolKit/SocketModule/TrComm.h"
//Han Hwa #include "../../include_contents/LoadDrds.h"



//-----------------------------------------------------------------------------
// Author		: Sang-Yun, Nam	Date :2005/4/18/MonDay
// Reason		: 
//-----------------------------------------------------------------------------
//start
// {{ 메인쪽 라이브러리 링크 ( 확장 dll )
//#include "../../../DataMgr/DBMgr.h" //05.09.27
//#include "../../COMMON_BASE/baseInfo/BaseInfo.h" //2005.09.27 add by sy, nam
//#include "../../../DrMLib/DrMiddleLib.h"//05.09.27

//#include "../../../include/EnvDir.h"
//#include "../../../Include/CommonStruct.h"
//#include "../../../include/UserMsg.h"				//KJI 20030305 Add
//#include "../../../include/CommonDef.h"			//KJI 20030305 Add
//#include "../../../CodeFind/JMCodeFindDlg.h"
//#include "../../../CommonLib/ServerMsg.h"
//#include "../../common_base/drds/DataUtil.h"

//#include "../../COMMON_BASE/Tree/CodeMnager.h"
//end

#include "../../inc/ExOCXDLL.h"
#include "../../inc/IDebugMng.h"

const UINT RMSG_SELCHARTCODE = ::RegisterWindowMessage( "RMSG_SELCHARTCODE" );
/////////////////////////////////////////////////////////////////////////////
// CStdDialog dialog
#define BOUND_GAP 0//10
#define	DEFAULT_GRAPH					3			// 초기 graph count	

	//DIV_OVERLAP : 등락률차트
enum	{MULTI_CHART, SINGLE_CHART, MINI_MULTI_CHART, MINI_SINGLE_CHART, DIV_OVERLAP};
//#define IMPLCTRLCLASS CStdDialog

//#define MYMETHOD_PROLOGUE(theClass, localClass) \
//	theClass* pThis = \
//	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
//	pThis;

typedef struct {
	CString		m_strTrCode;
	CWnd		*m_pWndRequested;	
	int			m_nType;
	void		*m_pRqstChartItem; //시장지표 TR을 날릴때.. 메인 차트 TR과 싱크가 안 맞는 것 때문에 여기에 요청한 OCX의 Pointer를 저장 
} STTRINFO;

typedef struct {
	CWnd* m_pWndRequested;
	double m_dPrevPrice;
	int m_nChartCount;
} STTRINFO2;

// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
class CRealTRCounter
{
public:
	CRealTRCounter() 
	{
	}

	~CRealTRCounter() 
	{
//		m_mapTrCounter.erase(m_mapTrCounter.begin(), m_mapTrCounter.end());
		m_mapTrCounter.RemoveAll();
	}

	UINT AddTr(CString strKey)
	{
//		map<string, UINT>::iterator it = m_mapTrCounter.find(lpszTR);
//		if(it == m_mapTrCounter.end())
//		{
//			m_mapTrCounter.insert(map<string, UINT>::value_type(lpszTR, 1));
//			return 1;
//		}
//
//		return ++(*it).second;
		UINT nCount;
		if(!m_mapTrCounter.Lookup(strKey, (LPVOID&)nCount))
		{
			m_mapTrCounter.SetAt(strKey, (LPVOID)1);
			return 1;
		}
		else
		{
			++nCount;
			m_mapTrCounter.SetAt(strKey, (LPVOID)nCount);
		}

		return nCount;
	}

	UINT RemoveTr(CString strKey)
	{
//		map<string, UINT>::iterator it = m_mapTrCounter.find(lpszTR);
//		if(it != m_mapTrCounter.end())
//			return --(*it).second;
//
//		return 0;
		UINT nCount;
		if(m_mapTrCounter.Lookup(strKey, (LPVOID&)nCount))
		{
			if(--nCount)
			{
				m_mapTrCounter.SetAt(strKey, (LPVOID)nCount);
				return nCount;
			}
			else
			{
				m_mapTrCounter.RemoveKey(strKey);
				return nCount;
			}
		}

		return -1;
	}

	void RemoveAll()
	{
		m_mapTrCounter.RemoveAll();
	}

	BOOL GetAllKey(CStringList& strList) const
	{
		strList.RemoveAll();

		int nPos = -1;
		CString strKey, strTemp;
		UINT nCount;
		POSITION pos = m_mapTrCounter.GetStartPosition();
		while(pos)
		{
			m_mapTrCounter.GetNextAssoc(pos, strTemp, (LPVOID&)nCount);

			strKey.Format("%s,%d", strTemp, nCount);
			nPos = strKey.Find(":");
			if(strKey.IsEmpty() == FALSE && nPos > 0)
				strList.AddTail(strKey);
		}
		return TRUE;
	}

	BOOL HasKey(CString strCode)
	{
		if(strCode.IsEmpty())
			return FALSE;

		int nPos = -1;
		CString strKey;
		LPVOID	rValue;
		POSITION pos = m_mapTrCounter.GetStartPosition();
		while(pos)
		{
			m_mapTrCounter.GetNextAssoc(pos, strKey, rValue);

			nPos = strKey.Find(":");
			if(strKey.IsEmpty() == FALSE && nPos > 0)
				if(strCode == strKey.Mid(nPos + 1))
					return TRUE;
		}

		return FALSE;
	}

private:
//	map<string, UINT> m_mapTrCounter;
	CMapStringToPtr m_mapTrCounter;
};

typedef struct {
	COLORREF clrTabSelText;
	COLORREF clrTabNorText;
	COLORREF clrTabBkgrnd;
	COLORREF clrTabSelGDStt;
	COLORREF clrTabSelGDEnd;
	COLORREF clrTabNorGDStt;
	COLORREF clrTabNorGDEnd;
} STTabColor;


//## Han Hwa
LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam);
static int ConvertToChartSign(char* pBuff);

class CLeftBarDlg;
class CInputBarDlg;
class CExtInputBarDlg;
class CMiniBarDlg;
class CChartListDlg;	//2005. 04. 25 add by sang yun, nam
class CFloatingHogaDlg;
class CDataWindowDlg;
class CCodeMnager;		//2005. 07. 26 add by sang yun, nam
class CStdDialog :public CExOCXDLL, public CMChartInterface061115//, public CSharedBase // CSharedBase to use shaered memory is Added By gm0604 2005/7/21
{
public:
	BOOL GetTimeSyncFlag();				//CExtInputBar에서 시간 연동 여부 조회
	void SetTimeSyncFlag(BOOL bCheck);  //CExtInputBar에서 시간 연동 여부 재지정 

public:
	// 쾌속 주문에 호가와 종목코드 전송 = "종목코드?호가"
	void SendHogaNCodeToSpeedJumun(CString strCode, CString strPrice);
	//===================================================================================

	// 현재 선택된 차트의 시작일과 종료일을 가져온다.
	void GetStartEndDateAtStdDialog(double& dStartDate, double& dEndDate);
	//===================================================================================


	//======================================================================================
	static CImageList   m_ImgList_W15H15; //폭 15에 높이 15인 버튼 이미지리스트	
	static CImageList   m_ImgList_W20H20; //폭 20에 높이 19인 버튼 이미지리스트
	static CImageList   m_ImgList_W20H20_Tool; //폭 20에 높이 19인 버튼 이미지리스트
	static CImageList   m_ImgList_W46H20; //폭 46에 높이 20인 버튼 이미지리스트	
	static CImageList   m_ImgList_W50H20; //폭 50에 높이 19인 버튼 이미지리스트
	static CImageList   m_ImgList_W31H20; //폭 31에 높이 20인 버튼 이미지리스트
	static CImageList   m_ImgList_W151H21; //폭 151에 높이 21인 버튼 이미지리스트	
	static CImageList   m_ImgList_W28H22; //폭 28에 높이 22인 버튼 이미지리스트
	static CImageList   m_ImgList_W17H20; //폭 17에 높이 20인 버튼 이미지리스트
	static CImageList   m_ImgList_W43H20; //폭 43에 높이 20인 버튼 이미지리스트
	static CImageList   m_ImgList_W58H20; //폭 58에 높이 20인 버튼 이미지리스트
//	static CImageList   m_ImgList_W72H20; //폭 72에 높이 20인 버튼 이미지리스트
//	static CImageList   m_ImgList_W46H20_Gwan; //폭 52에 높이 19인 버튼 이미지리스트
//	static CImageList   m_ImgList_W8H25;  //폭 8에 높이 25인 버튼 이미지리스트
	static CImageList   m_ImgList_W60H20; //폭 60에 높이 20인 버튼 이미지리스트	

	static int m_static_nChartCnt4ImgList;
	void InitImageList();
	void DeleteAllImageList();
	//====================================================================================

	//화면ID. fnLoad에서 셋팅됨.
	UINT	m_nScreenID;

	//화면번호. fnLoad에서 셋팅됨.
	HWND m_hPlatform;
	CString m_strScreenNo; 

	//같은 DLL에서 Type에 따라 달리 동작을 하기위해서 만든키값.fnLoad에서 셋팅됨.
	CString m_szScreenType; 

	enum{Screen_Compchart=0, Screen_ELWChart, Screen_ForeignChart, Screen_fluctuation};	//fluctuation
		// 0:비교차트, 1:당일등락률차트
	int		m_nScreenType;	
	
	//==========================================================================================
	//Chart 환경 내용 정보 저장...
	CHART_CONFIG_INFO m_ChartCfgInfo;

	bool	SetImaginaryCandleToOutside		(const bool bImaginaryCandle)	;	// 허수봉설정.
	bool	SetShowHighLowBoundToOutside	(const bool bShow			)	;	// 상/하한가 표시
	bool	SetShowExRightDividendToOutside	(const bool bShow			)	;	// 권리락/배당락 표시
	bool	SetTimeLinkageToOutside			(const bool bTimeLinkage	)	;	// 시간연동 수신
	bool	SetAutoSaveToOutside			(const bool bAutoSave		)	;	// 차트종료시 자동 저장 유무
	//bool	SetShowCurrentPriceToOutside	(const bool bShow			)	;	// 가격눈금위에 현재가표시
	bool	SetPriceLog						(const bool bLog			)	;	// 가격차트 log 셋팅.

	bool	GetImaginaryCandleToOutside		(bool& bImaginaryCandle		)	;	// 허수봉설정.
	bool	GetShowHighLowBoundToOutside	(bool& bShow				)	;	// 상/하한가 표시
	bool	GetShowExRightDividendToOutside	(bool& bShow				)	;	// 권리락/배당락 표시
	bool	GetTimeLinkageToOutside			(bool& bTimeLinkage			)	;	// 시간연동 수신
	bool	GetAutoSaveToOutside			(bool& bAutoSave			)	;	// 차트종료시 자동 저장 유무	
	//bool	GetShowCurrentPriceToOutside	(bool& bShow				)	;	// 가격눈금위에 현재가표시
	
	bool	OnChartEnvSetupOk()										;	// 차트 환경설정 창이 닫히는 순간 불린다.
	//==========================================================================================

	//==========================================================================================
	//OCX가 요청 하는 부분에 추가적인 함수 함수
	//2005. 08. 08 added by sy, nam 
	// Start >>>> ==============================================================================
	// 경로 및 UserID
	LPCTSTR GetMainPath();
	LPCTSTR GetUserID();		
	//End ==================================================================================<<<<
	bool m_bActiveDataWindow;
	CDataWindowDlg*	m_pDataWindow;

	CRect m_ViewRect;	
	void ShowDataListView(BOOL bShowDataListView, CRect ViewRect);
	void CreateDataWindow();
	void DestroyDataWindow();
	void SetDataWindowData(CList<CString, CString&>* pListData);
	bool IsActiveDataWindow();

	//============================================================================================
	//하단 상태표시 줄에 메세지를 보낸다.
	void CStdDialog::SendMessage2StatusBar(int nMsgType,  const char* pMsg = _T(""), int nNum =0);

	// 조건 종목 검색 전용 ============================================================================================
	void Send2HanHwa_IFCond(void* pData, int nLen, CWnd* pRequestWnd, CString strTRCode);

	// KOSPI, KOSDAQ 기타 사용자 지정 종목 검색 전용 ============================================================================================
	CWnd* m_pRqWndFromTab;
	void Send2HanHwa_09501(void* pData, int nLen, CWnd* pRequestWnd, CString strTRCode);
	
	//최종 조회한 종목의 코드를 가지고 있는다. 2005. 07. 26============================================================
	CString			m_strMainCode;

	//CCodeMnager Pointer 2005. 07. 26============================================================
	CCodeMnager*	m_pMainCodeManager;
	void			SetpMainCodeManager(CCodeMnager* pCCodeMnager)	{ m_pMainCodeManager = pCCodeMnager;}
	CCodeMnager*	GetpMainCodeManager()							{ return m_pMainCodeManager;		}	

	

	// 2005. 07. 18 add by sy, nam ===============================================	
	//여러개의 차트OCX가 존재할 때 DataWindow에 보여질 내용이 다르게 하기 위해 호출한다.
	BOOL  ChangedChartItem(); 
	//=============================================================================
	
public:

	// 2005. 07. 15 add by sy, nam =================================================
	void AddDataKey_Hoga();									//003 호가TR_AddDataKey						
	void Request_StockHogaData(CString strRecvCode = _T(""));
	void Request_StockRealHogaData(CString strCode);
	void Request_Disconnect_StockRealHogaData(CString strCode);
	//==============================================================================




	void    SetRealInfo		(
								void* pDrds,		// Drds포인터인데 NULL로 보냄.
								HWND hWnd,			// Chart OCX의 윈도우 핸들
								UINT id, 			// CPacket* 
								LPSTR DataName,		//  ex: "S31"
								LPSTR FieldName, 	//  ex: "time"
								LPSTR KeyName);		//  ex: "000660"


	void    UnSetRealInfo	(
								void* pDrds,		// Drds포인터인데 NULL로 보냄.
								HWND hWnd,			// Chart OCX의 윈도우 핸들
								UINT id, 			// CPacket* 
								LPSTR DataName,		//  ex: "S31"
								LPSTR FieldName, 	//  ex: "time"
								LPSTR KeyName);		//  ex: "000660"

	void	OnChartMouseMove(UINT	nFlags, CPoint	pt);

	CPtrList				m_lstRegRealChart;
	
	void					SendStockReal(char* pData);
	void					SendUpjongReal(char* pData);
	void					SendFutureReal(char* pData);
	void					SendOptionReal(char* pData);


	IMasterDataManagerLast* m_pDataManager;
	CString GetCodeNameAndGubun(int nChartType, CString csCode);
	


	// ### 차트 구분자 2005. 05. 16 add by sy, nam -----------------------------------------------
//@유진삭제
//	enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, ELW_CHART, FOREIGN_CHART, COMMODITY_CHART};	// 상품선물옵션차트 - ojtaso (20071001)
//	int m_nChartGubun; // STOCK_CHART - 주식차트, UPJONG_CHART - 업종차트, FUTOPT_CHART - 선물옵션 차트
//@유진삭제
//@유진추가
	int m_nMarketType; // STOCK_CHART - 주식차트, UPJONG_CHART - 업종차트, FUTOPT_CHART - 선물옵션 차트
	void ChangeCode(LPCTSTR szCode, int nDefault = MODE_DEFAULT, int nMode = CHANGE_ITEM);
	void ChangeMarket(UINT nMarketType, BOOL bUpdateCbMarket = TRUE , BOOL bRequestToServer = FALSE);

	BOOL IsCompareChartMode() { return m_nChartType == MULTI_CHART; }
	void EnableQueryOpt(BOOL bEnable);
//@유진추가	

	int GetMarketTypeFromCode(CString strItemCode, int& nCodeType);
	CString GetItemNameFromItemCode(CString strItemCode, BOOL bPreFix=TRUE);

public:
	//-----------------------------------------------------------------------------
	// Author		: Sang-Yun, Nam	Date :2005/4/12
	// Reason		: 한화에 적용할 Member추가	
	//				- 인터페이스 삭제에 의한 수정 삭제  및 추가 
	//-----------------------------------------------------------------------------
	HWND		m_hMainFrame;							// CMainFrame의 Handle	


	CString		m_strUserID;							// 사용자 ID
	void		SetInitInfo();							// 화면 Create시 호출되며 
														// 사용자 ID, RootPath, Image Path등을 얻어온다.
//	void		SendTR_TEST(CString strRecvCode);		// TR전송 테스트

	// 차트 데이타 리스트 창을 보여줄지 여부를 저장하고 있는 변수 - 2005. 04. 25
	BOOL m_bShowDataListWnd;

	// gm0604
	void InitChartBySystemTrading(LPCTSTR lpMessage);
	void SetChartBySystemTrading(BOOL bIsFirst);
	HWND m_hWndSystemTrading;
	CString m_strInputCode;
	CString m_strEndDate;
	CStringList m_stringListTimeType;
	CList<long,long> m_longListDataCount;
	CList<long,long> m_ptrListSystemTrading;
	CStringList m_stringListSTName;
	//CRealTRCounter	m_RealTrCounter;	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
											// 비교등락에서는 사용안하므로 삭제해본다.
	
// Construction
public:
	CStdDialog(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CStdDialog)
	enum { IDD = IDD_STD_DIALOG };
	CILButton	m_btnExtTool;
//	CRGBButton	m_btnSideBar;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

protected:
	// 플랫폼 관련 인터페이스를 별도의 함수 처리로 구현할 수 있다.
	// 여기 선언된 함수 이외에 더 필요한 함수가 있다면 따로 구현가능함. ExOCXDLL.h 참조
	virtual void OnGSharedDataChange(LPCTSTR lpSharedName, LPCTSTR lpGSharedData, BOOL bWithStart);

	virtual void Platform_Initialize(long dwMainDllObj, long dwBaseDesk, long dwRscMng, CStringList* pAllProperties);
	virtual LRESULT Platform_WindowProc(UINT message, WPARAM wParam, LPARAM lParam, int &nContinue);
	virtual LRESULT Platform_PreTranslateMessage(MSG* pMsg);
	
	virtual void Platform_OnSize(UINT nType, int cx, int cy);

	BOOL	TrackPopupMenu();	// 플랫폼 팝업

public:
	HRESULT ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);	
	HRESULT ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData);
	HRESULT	ReceiveMessage(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage);
	HRESULT	StatusChange(int nState);

	// Demo버전준비용.
	HRESULT DemoReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);

	// 통신(소켓) 처리 모듈
	ITrComm2*		m_TRComm;

	//메세지 창 띄운다.
	HRESULT		RecieveError(int nError, LPCSTR szMessage);
//	CWnd		*m_pWndRequested;
	
/*	class CMyCallback : public ICallbackStatus	
	{
		STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * lplpObj) {return S_OK;}
		STDMETHOD_(ULONG,AddRef) (THIS)  {return S_OK;}
		STDMETHOD_(ULONG,Release) (THIS) {return S_OK;}
		
		STDMETHOD(RecieveData)(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
		{
			MYMETHOD_PROLOGUE(CStdDialog, MyCallback)
			return pThis->ReceiveData(hTrComm, szTR, szMessage, aTRData, dwTRDateLen);			
//			return S_OK;		
		}
		STDMETHOD(StatusChange) (int nState)
		{
			MYMETHOD_PROLOGUE(CStdDialog, MyCallback)
			return S_OK;			
		}
		STDMETHOD(RecieveError) (int nError, LPCSTR szMessage)
		{
			MYMETHOD_PROLOGUE(CStdDialog, MyCallback)
			return pThis->RecieveError(nError, szMessage);
			return S_OK;			
		}
	} m_xMyCallback;
	friend class CMyCallback;

	
	// * Tr 응답 메세지를 받는다. 
	HRESULT ReceiveData(HWND hTrComm, 
						LPCSTR szTR, 
						LPCSTR szMessage, 
						LPVOID aTRData, 
						DWORD dwTRDateLen);	*/


	
/* ### Han Hwa
	/////////////// drds
	CDrdsLoadLib	m_DrdsDLL;
	void			*m_pDrds;			
	STDRDSINFO		m_DrdsInfo[MAXPACKET];
	int				m_nDrdsInfoCnt;
	void	InitDrds();
	BOOL	MakeDrdsConnectAdviseFromCode(int nI);
	BOOL	MakeDrdsUnConnectUnAdviseFromCode(int nI);
//## Han HWa*/
	typedef struct {
		CString DataName;
		CString FieldName;
		CString KeyName;
		UINT	nID;
		BOOL	bAdvise;
	} STDRDSINFO;
//@유진삭제 : m_DrdsInfo변수를 사용하는지 확인필요함.
	STDRDSINFO		m_DrdsInfo[MAXPACKET];
	int				m_nDrdsInfoCnt;
	IDrdsLib*		m_pDrdsLib;

	void	InitDrds();
	// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
	BOOL	MakeDrdsConnectAdviseFromCode(int nI, BOOL bAddMod);
	// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
	BOOL	MakeDrdsUnConnectUnAdviseFromCode(int nI, BOOL bAddMod);

	// 현재 차트내에 보여지고 있는 코드 저장 List
	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	CStringList		m_Codes;

	// Real Data 요청	- 2005. 04. 19 by Sang-Yun, Nam
	// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
	void	OnCodeAdvise(CString strCode, BOOL bAddMode);
	
	// Real Data 중지 요청	- 2005. 04. 19 by Sang-Yun, Nam
	// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
	void	OnCodeUnAdvise(CString strCode, BOOL bAddMode);

	// m_strMapID에 저장된 현재 화면번호(MapID)를 Return한다.	- 2005. 04. 19 by Sang-Yun, Nam
	CString	GetMapID();

	//	m_bEnable (화면 전체의 Enable / Disable 상태 값) Return		- 2005. 04. 19 by Sang-Yun, Nam
	BOOL	GetStatusEnable();

	//  현재 선택된 종목 리스트 중 마지막에 선택된 Index 다음 위치부터 현재 선택된 종목 갯수 만큼 씩 아래로 이동
	//	- 2005. 04. 19 by Sang-Yun, Nam
	void	SetJongListPrev();

	//  현재 선택된 종목 리스트 중 마지막에 선택된 Index 다음 위치부터 현재 선택된 종목 갯수 만큼 씩 위로 이동
	//	- 2005. 04. 19 by Sang-Yun, Nam
	void	SetJongListNext();

	//	전체 계좌 리스트를 얻어서 CString 형태로 Return 한다.
	//	- 2005. 04. 19 by Sang-Yun, Nam
	CString GetAccountInfo();

	//	환경 설정 파일 경로가 저장되는 변수 - 2005. 04. 19 by Sang-Yun, Nam
	CString			m_strConfig;

	//	2005. 04. 19 현재 Han Hwa에서 사용하지 않음. - 2005. 04. 19 by Sang-Yun, Nam
	CString			m_strConfigOfMain;

	//	"Data"폴더 경로가 저장되는 변수 - 2005. 04. 19 by Sang-Yun, Nam
	CString m_szDataDir;

	//	"Image"폴더 경로가 저장되는 변수 - 2005. 04. 19 by Sang-Yun, Nam
	CString m_szImageDir;

	//	"Bin"폴더( 메인 실행) 경로가 저장되는 변수 - 2005. 04. 19 by Sang-Yun, Nam
	CString m_strRootPath;

	//	사용자 폴더 경로가 저장되는 변수 - 2005. 04. 19 by Sang-Yun, Nam
	CString m_strUserDir;

	//	소켓 핸들
	HWND	m_hSocket;

	//	현재 화면번호(MapID)를 저장한다. - 2005. 04. 19 by Sang-Yun, Nam
	CString			m_strMapID;

	//	스플릿 번호 (2부터 시작됨)를 저장한다.
	//	종합화면에서는 화면들이 탭으로 나눠져 있어서  
	//	화면번호는 하나인데 여러화면들이 있을 수가 있는데.
	//	여기서 각각의 고유코드를 얻어서 환경설정 저장을 할려고 받아온 ID
	//  사용자가 지정한 6810 사용자 정의 화면에 에 멀티차트가 3개가 들어가 있을경우에..
	//	각각의 위치에 따라서 다른 ID가 들어오는데 이를 의미한다. - 2005. 04. 19 by Sang-Yun, Nam
	UINT			m_nSpliteID;

	//	화면 시작시 보이는 Tab Index ( Zero Base ) - 2005. 04. 19 by Sang-Yun, Nam
	int		m_nTabID;

	//	화면의 용도에 따라 차트가 보이는 형태가 달라지는 데 이를 Index로 구분하여 저장
	//	0:Normal(표준), 1:Mini, 2:OrderSrc&Chart - 2005. 04. 19 by Sang-Yun, Nam
	UINT	m_nChartType; 

	//	이 Dll과 함께 포함된 다른 별도의 독립맵 ID를 저장하고 있다. - 2005. 04. 19 by Sang-Yun, Nam
	UINT	m_nImportedMapID;

	//	Dialog 폭 - 2005. 04. 19 by Sang-Yun, Nam
	UINT	m_nWidthOfDlg;

	//	Dialog 높이 - 2005. 04. 19 by Sang-Yun, Nam
	UINT	m_nHeightOfDlg;

	//	이 Dlg를 감싸고 있는 View의 Pointer - 2005. 04. 19 by Sang-Yun, Nam
	HWND	m_hViewInMain;
	HWND	m_hKSDllView;	// 소리마치 DLL의 공통처리용 View Handle

	BOOL			m_bIsSystemTradingMode;// gm0604 added 2005.7.25

	// 2열 툴바중 상단 툴바를 표시하는 Dlg
	CInputBarDlg	*m_pInputBarDlg;
private:
	// chart ocx에 패킷을 전달하기 위한 포인터 
	// tr을 날려 받은 데이타를 m_ChartMng를 통해 차트에 전달
	CChartMng m_ChartMng;

	// 종목코드 OCX
	//CAny2Code m_code;

	// Splitter Bar
    CxSplitterWnd   m_xSplitter1;

	// Splitter 왼쪽의 Dlg
	CLeftBarDlg		*m_pLeftBarDlg;	

	// 2열 툴바중 상단 툴바를 표시하는 Dlg
	//CInputBarDlg	*m_pInputBarDlg;

	// 2열 툴바중 하단 툴바를 표시하는 Dlg
	CExtInputBarDlg	*m_pExtInputBarDlg;

	// CLeftBarDlg 의 위치에 따른 구분을 하기위해 정수형 값을 저장하고 있는 변수
	UINT			m_nLeftBarType;	// 0: None, 1:Left, 2:Right 
	
	// CLeftBarDlg의 폭
	int				m_nWidthOfLeftBar;

	// 화면내 컨트롤 Enable/Disable Toggle
	BOOL			m_bEnable;

	// 공유 데이터를 한번 받은 후 FALSE로...??
	BOOL			m_bFirstGShared;

	// 1열 툴바중 하단 툴바를 표시 여부 - gm0604 added For SystemTrading 2005.7.25 
	BOOL			m_bShowInputBarDlg;

	// 2열 툴바중 하단 툴바를 표시 여부
	BOOL			m_bShowExtInputBarDlg;

	// 작은 화면 상태 여부 
	BOOL			m_bSmallScreenMode;

	// 사용하지 않는 듯..
	int				m_nTempLeftBarType;

	// TR을 요청한 정보(HWND, TR_NO등을 저장한 STTRINFO 形 Array로 관리
	CArray<STTRINFO,STTRINFO> m_arryQuery;

	// 보조지표에서 요청한 정보를 STTRINFO 形 Array로 관리
	CArray<STTRINFO,STTRINFO> m_arryQueryInvoke;

	// 기본 폭
	long			m_lWidthOriginal;

	// 기본 높이
	long			m_lHeightOriginal;

	// 차트 숨김 여부 Toggle
	BOOL			m_bHideChart;

	// 리사이즈 초기화 Toggle
	BOOL			m_bInitResize;

	// User 총 계좌 정보를 가지고 있는다.
//	CString			m_strAccountInfo;

	// gm0604 added 2005.7.25 Start
//	CStringArray	m_strArrayAdviceInfo;
	// gm0604 added 2005.7.25 End
	
	// 시스템 차트 여부
	BOOL m_bSystemChart;
	
	// 단순차트 여부
	BOOL m_bSimpleChart;

	// 복수종목 차트 여부
	BOOL m_bMultiItemChart;

//====================================
//	CIndicatorList	m_IndicatorList;
//====================================
// Operations
public:		
	void AllCodeUnAdvise();
	void DestroyFloatingHogaWnd();
	
	// m_bHideChart 값 Return
	BOOL	IsChartHide();

	// chart 숨김
	BOOL	HideChart();

	// 화면 Load시에  각종 초기화 
	void	FormLoad();

	// Resizing 시 기타 컨트롤 재배치
	void	Resize();

	// CLeftBarDlg의 형태를 지정
	void	SetLeftBarType(UINT nLeftBarType);

	// CLeftBarDlg의 형태 Index Return
	UINT	GetLeftBarType() {return m_nLeftBarType;}

	// 하단 툴바 Show/Hide 갱신
	void	SetShowExtInputBarDlg(BOOL bShowExtInputBarDlg);

	// 하단 툴바 Show/Hide 값(m_bShowExtInputBarDlg) Return
	BOOL	GetShowExtInputBarDlg() {return m_bShowExtInputBarDlg;}
	
	// 전체 HTS 공유 데이터 변경시 호출
	//BOOL	ChangedGlobalData(CString szName, CString szValue);

	// 작은 화면 / 큰 화면에 따른 화면배치
	void	SetSmallView(BOOL bSmall = TRUE);

	// 조건검색등 외부에서 종목 리스트를 받는다.
	void	InputCodeFromOutSide(CString strPacketFromOutSide);

	//
	void	*m_pData;
	
	//주식/업종/선물옵션 변경
	void	ChangeStockMode(int type);
	void	ChangeMiniStockMode(int type);

	//008 {{
	void	ChangeUpjongMode(UINT type);	// 코스닥/코스콤업종 구분하여 코드콤보 생성
	//008 }}

	BOOL	IsSystemChart(){ return m_bSystemChart; };
	BOOL	IsSimpleChart(){ return m_bSimpleChart; };
	BOOL	IsMultiItemChart() { return m_bMultiItemChart; }

	//20061212 정유진 수정
	//GRID Column 확장button 추가
	void	GridColumnExtend();			//btn click 호출 func
	int		m_nExtendState;				//btn 선택 상태 0:축소/1:확장
	//20061212 수정 end
private:
	// 서버로 TR정보를 요청한다.
	void	DoCheckMin(int nType, LPCTSTR lpData, CString& szReqTr);
	int		Send2Server(LPCTSTR lpTrCode,LPCTSTR lpData,int nSize, BOOL bWait, int nArrayIndex = -1);

	// CLeftBarDlg의 크기에 따른  nID에 주어진 Control의 크기 위치등을 변경한다.
	CRect	GetRect(UINT nID, BOOL bInit = FALSE);

	// m_pLeftBarDlg,m_pInputBarDlg,m_pExtInputBarDlg,m_ChartMng등을 Enable/Disable 시킨다.
	void	EnableWindowAll(BOOL bEnable = TRUE);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStdDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation

protected:

	// Generated message map functions
	//{{AFX_MSG(CStdDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	LONG	OnWmSplitterMoved( UINT wParam, LONG lParam );	//Splitter Bar Moving Msg
//@Solomon090819 	afx_msg long OnGetBroad(WPARAM wParam, LPARAM lParam);
	LONG	OnByPass_ExportToolOption(UINT wParam, LPARAM lParam); //현재 선택된 차트Dll에 외부툴바 설정 내용을 다 적용한다.	
	long	ChangeSharedData(CString sShareName, BYTE* pData, int nLen, int nStock=0);// 공유 데이터 
	LRESULT OnDropData(WPARAM wp, LPARAM lp);
	//-----------------------------------------------------------------------------
	// Author		: {{{ 오정택 }}} ojtaso 	Date : 2005/2/23
	// Reason		: Added to drag and drop operation.
	//-----------------------------------------------------------------------------
	// BEGIN
	LRESULT OnDragEnd(WPARAM wParam, LPARAM lParam);
	LRESULT OnDragStart(WPARAM wParam, LPARAM lParam);

	//스킨
	LRESULT OnChangeSkinColor(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetSkinColor(WPARAM wParam, LPARAM lParam);
	
	LRESULT OnUserInfo(WPARAM wp, LPARAM lp); 

	// END
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CStdDialog)
	afx_msg void SetOpenMapData(LPCTSTR szOpenData, short nDataLen);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	CString m_strOut;
	CString m_strMarketOut;
	void ConvertKoscomData(LPCSTR szTR, LPVOID	aTRData, long dwTRDateLen, LPCSTR szCode);

	CString MakeChartData(LPVOID aTRData, int& nCount, int dwTRDataLen, int nMarketType, int nType, double dPrePrice, int nUnitType=WON_TYPE);
	CString MakeChartData(LPVOID aTRData, int nIdx, int& nCount, int dwTRDataLen, int nMarketType, int nType, double dPrePrice = 0, int nUnitType=WON_TYPE);
	char m_cUM;
	//by LYH 2006/7/7 Drag & drop
	HCURSOR				m_hCursor;
	int					m_nMarketHandle;
	CString				m_strDollar;
	//20070131 by 정유진
	char m_cMUM;
public:
	void	NofityNewDataReceive();		// 데이터를 새로 받았다는 것을 알린다. For TabSystemTradingEx

	//원클릭숨기기 by LYH 2006.10.02
	BOOL m_bOneClickHide;
	void SetOneClickHide(BOOL bOneClickHide);
//	int	m_nMarketTrCode;

	BOOL	GetRevisedPriceFlag( void);		//수정주가 체크상태 물어봄
	BOOL	SetRevisedPriceFlag( BOOL p_bRevisedFlag);	//수정주가 바꼈음. 변경통보
	BOOL	OnIndicatorAddOrRemoved( const char *p_szGraphName,	const BOOL p_bAdded);	//지표추가 및 삭제 이벤트
	BOOL	OnUserZoomChanged( const int p_nDataCountInView);	//줌 이벤트
	BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn);	//스크롤 이벤트

	CString m_strRecentFutureCode;
	CString GetRecentFutureCode(){return m_strRecentFutureCode;};

//	CString m_strMapFileName;
	
	void	AddRealCodes(CString strTrCode, LIST_CODEDATA *pListRealCodes);
	void	RemoveRealCodes(LIST_CODEDATA *pListRealCodes, CString strKey);
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	void	AddRealCodeList(LPCTSTR lpszCode);
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	void	AddRealCodes();

//	LIST_CODEDATA m_ListStockRealCodes;
//	LIST_CODEDATA m_ListUpjongRealCodes;
//	LIST_CODEDATA m_ListFutOptRealCodes;	
//	LIST_CODEDATA m_ListForeignRealCodes;

	// 상품선물옵션차트 - ojtaso (20071001)
	USHORT GetDotPosition(CString& strItemCode);

	//장전데이터 처리
	BOOL m_bPreMarket;

	//틱데이터 없을경우 세팅
	char m_cUT;

	//2007.04.25 by LYH 받은 코드(현재 CodeCombo 코드와 비교해서 다를 경우 재조회)
	CString m_strRecvCode;

	//2007.05.02 by LYH 서버 통계 위해 서비스 ID 추가
//	CString m_strSvcID;

	STTRINFO2	m_stTrInfo2;


	int GetWidthOfLeftBar();

	// 복수종목 리얼 해제 : 복수종목 - ojtaso (20070209)
	void	OnDeleteChartItem(LPCSTR lpszItemCode);
	// 종목명 얻기 : 복수종목 - ojtaso (20080213)
	CString		GetJongMokNameByCode(LPCSTR lpszItemCode, LPCTSTR lpszTRCode = NULL);
	// 복수종목 선택 : 복수종목 - ojtaso (20080215)
	void OnSelectChartItem(ST_SELCHART* pSelChart);
	// 현재 종목과 선택된 종목이 같은지 : 복수종목 - ojtaso (20070223)
	BOOL IsSameItemCodeName(LPCTSTR lpszItemName);
	// 차트 초기화때 모든 리얼 해제 : 복수종목 - ojtaso (20070621)
	void ResetChartAll();

	// 분차트 마지막 키인 경우 - ojtaso (20071226)
	void EnableRequestMoreButton(BOOL bEnable);

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	void WriteToStringLog( const char *p_szSection, const char *p_szData, int p_nDataLen);
	void CreateLogFile();
	void CloseLogFile();

	HFILE		m_hLogFile;			// Log File Handle
#endif
	
public:
	// 비교차트에 추가된 종목을 삭제한다.	
	BOOL	RemoveGraph_Title(int nRow, int nCol, CString strCode, CString strJMName, BOOL bIsOnlyNoTRData);
	BOOL	RemoveGraph(int nRow, int nCol, CString strCode, CString strJMName);

	// strName(종목명)에 해당하는 데이터를 화면에서 SHOW/HIDE한다.
	void SetGraphHide3(CString strCode, CString strName, BOOL bShowGraph);

	// 현재 입려조건과 주어진 종목코드에 맞은 조회버퍼생성.
	BOOL RequestTRFormCode(LPCSTR szRuestCode, BOOL bNext=FALSE);
	BOOL GetInputBarInput();

	//등락률차트의 설정창.
	BOOL SetupChartUtil();

	// LeftBar의 초기화 루틴 Call
	void InitialBtnClick();

	// ChartMng에게 조회요청.
	void SendToChartMngCode(LPCSTR szCode);

	CShareInputData	*m_pShareInputData;

	// 차트 블럭중에서 삭제명령이 왔을 경우 왼쪽 Left바에서 삭제하는 처리를 한다.
	void DeleteToLeftBar(LPCSTR strItemCode);
	
	// 종목 재조회를 위한 변수
//	CStringList	m_szRCodeList, m_szRCodeNameList;
	int GetRequestList(CStringList* pzRCodeList, CStringList* pszRCodeNameList);
//	void RequestNext();
	void FileDelete();


	//--> 등락률차트
	STDlgCompData	m_stDlgCompData;//등락률차트를 위한 파일정보.
	CDlgCompDataHandle m_FnHandle;
	//<--

	BOOL m_bSizeTestMode;

	// 스킨 적용
	COLORREF m_clrBackground;
	// Tab
	COLORREF m_clrTabSelText;
	COLORREF m_clrTabNorText;
	COLORREF m_clrTabBkgrnd;
	COLORREF m_clrTabSelGDStt;
	COLORREF m_clrTabSelGDEnd;
	COLORREF m_clrTabNorGDStt;
	COLORREF m_clrTabNorGDEnd;
	
	COLORREF GetBkgrndColor();	//	{ return m_clrBackground; }
	void ChangeSkinColor(COLORREF clrBackground);

	//-->@.080728-001 조회시작일 종가저장.
	CMapStringToString	m_mapSCDate;
	void GetSCDDataFromBosomsg(CString szKey, CString strData, LPCSTR szTR);

	void OnCodeScdClear(CString strCode, BOOL bAddMode);
	BOOL GetSCDData(CString szCode, CString &rValue);
	//<--

	void GetELWIndexCode(CString strELWCode, CString& strIndexCode, CString& strIndexName);
	//-->@090922 For IPartnerSite
	IPartnerManager*	m_pPTManager;
	CString				m_szPTManager;
	void DoSetPartnerSite(IPartnerManager* pPtManager);

	class CPartnerSite : public IPartnerSite
	{
	public:
		STDMETHOD(RecieveFidData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
		
		STDMETHOD(RecieveTRData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
		
		STDMETHOD(RecieveRealData) (long dwKey, LONG dwData);

		// nKey 0 : 공유공유전달
		//		1 : 종목 Dialog에서 선택한 코드
		STDMETHOD(RecieveKeyData)(int nKey, LPCSTR szData);
		
		STDMETHOD(RecieveRealDataEx) (long dwKey, LPCTSTR pszCode, LONG dwData);

		STDMETHOD(ReceiveOnUser) (WPARAM wParam, LPARAM lParam);
	} m_xPartnerSite;
	friend class CPartnerSite;

	//<--@090922 For IPartnerSite

	IWinixGwanManager*	m_pGwanMng;

	void _DoCheckDaultDataFile(LPCSTR szScreenNo, LPCSTR szUserFile);
	void _DoCopyDefaultToUser(LPCSTR szSect);
	void _DoCheckRecentFuture(LPCSTR szDataFile);

	//Leftbar에서 첫번째 종목 구하기
	void GetFirstCode(CString &rString);
	CWnd *GetpLeftBarDlg() { return m_pLeftBarDlg;}//KHD
	BOOL	m_bOnDestroy;

	BOOL IsContinueFutCode(CString& strCode);		// 연결선물일 경우 최근월물로 변경
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDDIALOG_H__CF66AE4F_AD1A_4057_B00E_F9FAA4C3A30F__INCLUDED_)

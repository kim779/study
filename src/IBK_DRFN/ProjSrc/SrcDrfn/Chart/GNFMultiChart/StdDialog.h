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
#include "MessageBar.h"
#include "LeftBarDlg.h"
#include "InputBarDlg.h"
#include "ExtInputBarDlg.h"
#include "FavoriteThingsMng.h"
#include "../../inc/PcTrMngHelper.h"
#include "DlgToolbar.h"
#include "./control/RGBButton.h"
// --> [Edit]  강지원 2008/10/29
#include "RequestMsg.h"
//#include "InquiryDlg.h"
// <-- [Edit]  강지원 2008/10/29

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

#include "../../inc/IAUPartnerMng.h"		//@090922 For IPartnerSite
#include "../../inc/IMainInfoMng.h"
#include "../../inc/IMainExInfoMng.h"


/////////////////////////////////////////////////////////////////////////////
// CStdDialog dialog
#define TIMER_EVENTID_FORMLOAD	2

#define BOUND_GAP 0//10
#define	DEFAULT_GRAPH					3			// 초기 graph count	
enum	{MULTI_CHART, SINGLE_CHART, MINI_MULTI_CHART, MINI_SINGLE_CHART, MULTI_SPREAD_CHART};
// (2008/9/14 - Seung-Won, Bae) for Pair Tick
enum	{	E_RPTS_STOP,
			E_RPTS_START, E_RPTS_PAIR,
			E_RPTS_LOOP_START, E_RPTS_LOOP_PAIR,
			E_PRTS_ROTATE_START, E_PRTS_ROTATE_PAIR,
		};

//		파일 지우기, Default파일로 복사, 사용할 파일로 복사, 마지막 환경 파일복사
enum	{DELETE_FILE, HWND_DEFAULT_COPY, DEFAULT_HWND_COPY, LAST_HWND_COPY, HWND_LAST_COPY};
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

// --> [Edit]  강지원 2008/10/18
#define		MAX_VPOINT					8
#define		MAX_VPOINT_PRICE			2  //체결용 char[15] -> char[10]으로 바꾸면서 자릿수 변화
// <-- [Edit]  강지원 2008/10/18

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

	int AddTr(CString strKey)
	{
//		map<string, UINT>::iterator it = m_mapTrCounter.find(lpszTR);
//		if(it == m_mapTrCounter.end())
//		{
//			m_mapTrCounter.insert(map<string, UINT>::value_type(lpszTR, 1));
//			return 1;
//		}
//
//		return ++(*it).second;
		int nCount;
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

	int RemoveTr(CString strKey)
	{
//		map<string, UINT>::iterator it = m_mapTrCounter.find(lpszTR);
//		if(it != m_mapTrCounter.end())
//			return --(*it).second;
//
//		return 0;
		int nCount;
		if(m_mapTrCounter.Lookup(strKey, (LPVOID&)nCount))
		{
			if( --nCount > 0 )
			{
				m_mapTrCounter.SetAt(strKey, (LPVOID)nCount);
				return nCount;
			}
			else
			{
				m_mapTrCounter.RemoveKey(strKey);
				return 0;
			}
		}

		return -1;
	}

	void RemoveAll()
	{
		m_mapTrCounter.RemoveAll();
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

typedef struct {
	COLORREF clrOLBkgrnd;
	COLORREF clrOLOutLine;
} STOutBarColor;


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
// 돋보기 구현 - ojtaso (20080519)
class CDlgGlass;
class CStdDialog :public CExOCXDLL, public CMChartInterface061115//, public CSharedBase // CSharedBase to use shaered memory is Added By gm0604 2005/7/21
{
public:
	bool IsLoad_MarketChart() const; //시장 지표를 로들할 지 여부를 셋한다.

	double g_finish, 	g_duration, g_start;

	//====================================================================================
	// 복기 관련 2005.10.14
	CString m_strSpeed;


	BOOL CheckDataTypeForReplay();

	void RqstTRForReplay();

	BOOL GetTimeSyncFlag();				//CExtInputBar에서 시간 연동 여부 조회
	void SetTimeSyncFlag(BOOL bCheck);  //CExtInputBar에서 시간 연동 여부 재지정 

	void SetBNeedRqstTRForReplay(BOOL bNeed) { m_bNeedRqstTRForReplay = bNeed; }
	void SetBReplayMode(BOOL bReplay);
	BOOL GetBReplayMode() { return m_bReplayMode; }
private:
	BOOL m_bNeedRqstTRForReplay; //복기 Play시 차트 TR을 재전송 요청을 해야 할 경우
	BOOL m_bReplayMode; 	
public:
	stSend2Server m_st2Server;
	//===================================================================================
//	CDrMLib_CDBMgr		m_IDrMLib_CDBMgr;				// 05. 09. 27	
//	CDrMLib_CBaseInfo2*	m_pMainBaseInfo;	
//	CDrMLib_CBaseInfo2*	GetpMainBaseInfo()						{ return m_pMainBaseInfo;	   }	
//	long m_lDBMgr;	// added by gm0604 2005.10.20
	//===================================================================================

	//05. 09. 22 ========================================================================
	// 쾌속 주문에 호가와 종목코드 전송 = "종목코드?호가"
	void SendHogaNCodeToSpeedJumun(CString strCode, CString strPrice);
	//===================================================================================

	//05. 09. 06 ========================================================================
	// 현재 선택된 차트의 시작일과 종료일을 가져온다.
	void GetStartEndDateAtStdDialog(double& dStartDate, double& dEndDate);
	//===================================================================================

	//======================================================================================
	// Resource 사용의 최소화를 위해 
	// Static으로 이미지 리스트들을 관리한다.
	static CImageList   m_ImgList_W15H15; //폭 15에 높이 15인 버튼 이미지리스트		

	static CImageList   m_ImgList_W20H20; //폭 20에 높이 19인 버튼 이미지리스트
	static CImageList   m_ImgList_W20H20_Tool; //폭 20에 높이 19인 버튼 이미지리스트
	static CImageList   m_ImgList_W46H20; //폭 46에 높이 20인 버튼 이미지리스트	
	static CImageList   m_ImgList_W50H20; //폭 50에 높이 19인 버튼 이미지리스트
	static CImageList   m_ImgList_W151H21; //폭 151에 높이 21인 버튼 이미지리스트	
	static CImageList   m_ImgList_W28H22; //폭 28에 높이 22인 버튼 이미지리스트
	static CImageList   m_ImgList_W17H20; //폭 17에 높이 20인 버튼 이미지리스트
	static CImageList   m_ImgList_W43H20; //폭 43에 높이 20인 버튼 이미지리스트
	static CImageList   m_ImgList_W58H20; //폭 58에 높이 20인 버튼 이미지리스트
	static CImageList   m_ImgList_W72H20; //폭 72에 높이 20인 버튼 이미지리스트
	static CImageList   m_ImgList_W46H20_Gwan; //폭 52에 높이 19인 버튼 이미지리스트
	static CImageList   m_ImgList_W8H25;  //폭 8에 높이 25인 버튼 이미지리스트
	static int m_static_nChartCnt4ImgList;
	void InitImageList();
	void DeleteAllImageList();
	//====================================================================================

	//BitBlt를 위해서 추가한다.
	//HBITMAP			m_hMultiChartBitmap;


	HWND	m_hPlatform;
	UINT	m_nScreenID;
	CString m_strScreenNo; //화면 번호를 저장한다.

	UINT	GetScreenID()	{ return m_nScreenID;	};
	//==========================================================================================
	//Chart 환경 내용 정보 저장...
	CHART_CONFIG_INFO m_ChartCfgInfo;

	bool	SetImaginaryCandleAndGapRevisionToOutside(const bool bImaginaryCandle, const bool bUseGapRevision);	// 허수봉/갭보정 설정	// (2008/11/2 - Seung-Won, Bae) for Gap Revision.
	bool	SetShowHighLowBoundToOutside	(const bool bShow			)	;	// 상/하한가 표시
	bool	SetShowExRightDividendToOutside	(const bool bShow			)	;	// 권리락/배당락 표시
	bool	SetTimeLinkageToOutside			(const bool bTimeLinkage	)	;	// 시간연동 수신
	bool	SetAutoSaveToOutside			(const bool bAutoSave		)	;	// 차트종료시 자동 저장 유무
	//bool	SetShowCurrentPriceToOutside	(const bool bShow			)	;	// 가격눈금위에 현재가표시
	bool	SetPriceLog						(const bool bLog			)	;	// 가격차트 log 셋팅.

	bool	GetImaginaryCandleToOutside		(bool& bImaginaryCandle		)	;	// 허수봉설정.
	bool	GetUseGapRevisionToOutside(bool& bUseGapRevision			)	;	// 갭보정	// (2008/11/2 - Seung-Won, Bae) for Gap Revision.
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
	
	// 시장지표등의 TR Data요청
	void RequestTRPacket(const short nID, const LPCTSTR pcszPacketNames, const short nCount, const bool bIsNew);
	//void RequestTRPacket(short nID, LPCTSTR pcszPacketNames);
	//void RequestTRPacket(CWnd* pWnd, LPCTSTR pcszIndicatorName, LPCTSTR pcszPacketNames);
	//End ==================================================================================<<<<


	//==========================================================================================
	//복기 차트를 위한 Function Pointer 및 관련 함수
	//2005. 08. 02 added by sy, nam 
	// Start >>>> ==============================================================================
	typedef CWnd* 	(*_DLL_fnReplayDll_Load			)(HWND  hParentWnd);
	typedef void	(*_DLL_fnReplayDll_Unload		)(CWnd* pWnd);
	typedef void	(*_DLL_fnReplayDll_SetInitInfo	)(CWnd* pWnd, void* pReplayInfo);
	typedef BOOL	(*_DLL_fnReplayDll_Run			)(CWnd* pWnd, int nMode);
	typedef void	(*_DLL_fnReplayDll_ChangeSpeed	)(CWnd* pWnd, int nSpeed);
	
	_DLL_fnReplayDll_Load			_fnReplayDll_Load		    ;
	_DLL_fnReplayDll_Unload			_fnReplayDll_UnLoad			;
    _DLL_fnReplayDll_SetInitInfo	_fnReplayDll_SetInitInfo	;
	_DLL_fnReplayDll_Run			_fnReplayDll_Run			;
	_DLL_fnReplayDll_ChangeSpeed	_fnReplayDll_ChangeSpeed	;

	
	void Recv_ReplayRealData(void* pData, int nLen);
	BOOL RunReplay(int nRun, REPLAY_INFO* pReplayInfo = NULL); 
	void ChangeSpeed(int nSpeed);

	BOOL Load_ReplayChart_Lib();	//복기 Dll Load			
	BOOL UnLoad_ReplayChart_Lib();	//복기 Dll UnLoad	
	
	HINSTANCE	m_hReplayChartLib;			//복기용 Lib 핸들
	CWnd*		m_pWndReplayLib  ;			//복기 Lib와 연결된 CWnd*
	
	//End ==================================================================================<<<<
	
	CRect m_ViewRect;	
	void ShowDataListView(BOOL bShowDataListView, CRect ViewRect);


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
	
	// 2005. 07. 26 add by sy, nam ===============================================
	// CMChartInterface 새로운 Real 처리 
	//=============================================================================
	void SetDataWindowData(CList<CString, CString&>* pListData);
	void CreateDataWindow();
	void DestroyDataWindow();
	bool IsActiveDataWindow();
	//	void ChangeTRData(void);
	//	void ChangeRealData(void);
	//	void MoveMouse(CPoint point);
	//  void SetOutsideCommonInterface(CMChartInterface* pOutsideCommonInterface);
private:
	CDataWindowDlg* m_pDataWindow;
	bool m_bActiveDataWindow;
	//CMChartInterface* m_pChartOcxInterface;
	//=============================================================================
public:
// --> [Edit]  이도영 2008/09/11
// 
	void GetOrderInfo();
	BOOL		m_bMigyulcheDraw;
	CString		Gl_Format32String(long nTBValue, int scale, int outdec, int indec, UINT uMask);
	long		FtoTbondPrcL( double inprice, int scale, int outdec );

// <-- [Edit]  이도영 2008/09/11

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


	


	// ### 차트 구분자 2005. 05. 16 add by sy, nam -----------------------------------------------
//@유진삭제
//	enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, ELW_CHART, FOREIGN_CHART, COMMODITY_CHART};	// 상품선물옵션차트 - ojtaso (20071001)
//	int m_nChartGubun; // STOCK_CHART - 주식차트, UPJONG_CHART - 업종차트, FUTOPT_CHART - 선물옵션 차트
//@유진삭제
//@유진추가
	int m_nMarketType; // STOCK_CHART - 주식차트, UPJONG_CHART - 업종차트, FUTOPT_CHART - 선물옵션 차트
	void ChangeCode(LPCTSTR szCode, int nDefault = MODE_DEFAULT, int nMode = CHANGE_ITEM);
	void ChangeMarket(UINT nMarketType, BOOL bUpdateCbMarket = TRUE , BOOL bRequestToServer = FALSE);

	BOOL IsMultiChartMode() { return m_nChartType == MULTI_CHART; }
	void EnableQueryOpt(BOOL bEnable);
//	int GetMarketTypeFromCode(CString strCode);
//@유진추가	

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
	void		SendTR_TEST(CString strRecvCode);		// TR전송 테스트

	// 차트 지표, 차트 리스트 를 가져와 콤보에 셋 
	void		GetIndicatorNameListIntoComboBox(CComboBox& rComboBox, LPCTSTR p_strGroup); 	

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
	CRealTRCounter	m_RealTrCounter;	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	
// Construction
public:
	CStdDialog(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CStdDialog)
	enum { IDD = IDD_STD_DIALOG };
	CILButton	m_btnExtTool;
	CRGBButton	m_btnSideBar;
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

public:
// --> [Edit]  강지원 2008/07/25	( 데이터를 넘길때 벡터가 아닌 그냥 Char로 넘김 )
	HRESULT ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);	
// <-- [Edit]  강지원 2008/07/25
	HRESULT ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData);
	HRESULT	ReceiveMessage(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage);

	// 통신(소켓) 처리 모듈
	ITrComm2*		m_TRComm;

	//메세지 창 띄운다.
	HRESULT		RecieveError(int nError, LPCSTR szMessage);

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

	//	즐거찾기 Manager - 2005. 04. 19 by Sang-Yun, Nam
	CFavoriteThingsMng m_FavoriteMng;

	//	이 Dlg를 감싸고 있는 View의 Pointer - 2005. 04. 19 by Sang-Yun, Nam
	//	sini	기존 소리마치의 잔해인것 같음.
//	HWND	m_hViewInMain;

	BOOL			m_bIsSystemTradingMode;// gm0604 added 2005.7.25
private:
	// chart ocx에 패킷을 전달하기 위한 포인터 
	// tr을 날려 받은 데이타를 m_ChartMng를 통해 차트에 전달
	CChartMng m_ChartMng;

	// 종목코드 OCX
	//CAny2Code m_code;

	// Splitter Bar
    CxSplitterWnd   m_xSplitter1;

	// 우측 툴바
	CDlgToolbar	*m_pDlgToolbar;

	// Splitter 왼쪽의 Dlg
	CLeftBarDlg		*m_pLeftBarDlg;	

	// 2열 툴바중 상단 툴바를 표시하는 Dlg
	CInputBarDlg	*m_pInputBarDlg;

	// 2열 툴바중 하단 툴바를 표시하는 Dlg
	CExtInputBarDlg	*m_pExtInputBarDlg;

	// CLeftBarDlg 의 위치에 따른 구분을 하기위해 정수형 값을 저장하고 있는 변수
	UINT			m_nLeftBarType;	// 0: None, 1:Left, 2:Right 
	
	// CLeftBarDlg의 폭
	int				m_nWidthOfLeftBar;

	// Msg 표시 Dialog
	CMessageBar*	m_pMsgBar;

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
	CString			m_strAccountInfo;

	// gm0604 added 2005.7.25 Start
	CStringArray	m_strArrayAdviceInfo;
	// gm0604 added 2005.7.25 End
	
	// 시스템 차트 여부
	BOOL m_bSystemChart;
	
	// 단순차트 여부
	BOOL m_bSimpleChart;

	// 복수종목 차트 여부
	BOOL m_bMultiItemChart;

	BOOL m_bShowToolBar;

// --> [Edit]  강지원 2008/10/29
	//	통신 조회 메시지 처리
//@Sol100618	CRequestMsg*	m_pCommMsg;
//	CInquiryDlg*	m_pInquiryDlg;
// <-- [Edit]  강지원 2008/10/29

//====================================
//	CIndicatorList	m_IndicatorList;
//====================================
// Operations
public:		
	void AllCodeUnAdvise(BOOL bFlag = TRUE );
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

// --> [Edit]  강지원 2008/11/07
	BOOL			m_nCodeInput;		//	종목코드가 입력되었을때를 구분하기위해
// <-- [Edit]  강지원 2008/11/07

// --> [Edit]  강지원 2008/11/10
	// m_pLeftBarDlg,m_pInputBarDlg,m_pExtInputBarDlg,m_ChartMng등을 Enable/Disable 시킨다. ( 초기화버튼 클릭시 혹시 막혀있으면 풀어주기위해서 Public로 뺌 )
	void	EnableWindowAll(BOOL bEnable = TRUE);
// <-- [Edit]  강지원 2008/11/10

// --> [Edit]  강지원 2008/12/19
	CString m_strHwndKey;
	CString m_strScreenKey;
// <-- [Edit]  강지원 2008/12/19

private:
	// 서버로 TR정보를 요청한다.
	int		Send2Server(LPCTSTR lpTrCode,LPCTSTR lpData,int nSize, BOOL bWait, int nArrayIndex = -1);

	// CLeftBarDlg의 크기에 따른  nID에 주어진 Control의 크기 위치등을 변경한다.
	CRect	GetRect(UINT nID, BOOL bInit = FALSE);

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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnExtTool();
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG
//@유진삭제	afx_msg	LRESULT OnDrdsData(UINT wParam, LPARAM lParam); //Real Data Receive			
	LONG	OnWmSplitterMoved( UINT wParam, LONG lParam );	//Splitter Bar Moving Msg
//@Solomon_GNF	afx_msg long OnGetBroad(WPARAM wParam, LPARAM lParam);
//	void	OnGSharedDataChange(LPCTSTR lpSharedName,LPCTSTR lpGSharedData, BOOL bWithStart); //Global Sharing Data Receive
	LONG	OnByPass_ExportToolOption(UINT wParam, LPARAM lParam); //현재 선택된 차트Dll에 외부툴바 설정 내용을 다 적용한다.	
	long	ChangeSharedData(CString sShareName, BYTE* pData, int nLen, int nStock=0);// 공유 데이터 
	LRESULT OnDropData(WPARAM wp, LPARAM lp);
	LONG	OnReplayDrds(WPARAM wp, LPARAM lp); //복기용 Real 
	LONG	OnReplayFail(WPARAM wp, LPARAM lp); //복기 실행 실패
	LONG	OnReplayChangeDate(WPARAM wp, LPARAM lp);//복기용 Real중 날짜가 변경되었을 때 불린다.
	LONG	OnReplayEnd(WPARAM wp, LPARAM lp); //복기 정지됨.
	LONG	OnReplaySetTotalSec(WPARAM wp, LPARAM lp); //복기 전체 초를 받는다.
	LONG	OnReplayEachSec(WPARAM wp, LPARAM lp); //복기 현재 복기 진행 시간을  받는다.
	//-----------------------------------------------------------------------------
	// Author		: {{{ 오정택 }}} ojtaso 	Date : 2005/2/23
	// Reason		: Added to drag and drop operation.
	//-----------------------------------------------------------------------------
	// BEGIN
	LRESULT OnDragEnd(WPARAM wParam, LPARAM lParam);
	LRESULT OnDragStart(WPARAM wParam, LPARAM lParam);
	// END
	LRESULT OnFormInitAfter(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangeSkinColor(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetSkinColor(WPARAM wParam, LPARAM lParam);
	LRESULT OnFormGetPageSave(WPARAM wParam, LPARAM lParam);
	LRESULT OnFormSetPageSave(WPARAM wParam, LPARAM lParam);
	//<<< 20091215
	LRESULT OnChangeAccount(WPARAM wParam, LPARAM lParam);
	//>>>

	LRESULT OnUserInfo(WPARAM wp, LPARAM lp);
	LRESULT OnAccountInit(WPARAM wp, LPARAM lp);

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
// --> [Edit]  강지원 2008/10/21
	void		ChangeTimeSet(int nIndex, BOOL	bSetFlag = FALSE);
	CString		GetTimeIndexInfo(int nIndex, int nType);
	CString		TbondPrcLToF( CString inprice, int scale, int outdec , int ValidPoint, BOOL bFlag);
	long		DataSeparation2(CString strData, long nMask, long nTickSize, int nLog, int nPrecision);
	BOOL		GetRealCheck();									//	시간설정 컨트롤옆 체크의 값을 얻는 함수
	void		SetTabTimeIndex(int nIndex);					//	왼쪽 시간대 탭의 인덱스를 설정하는 함수
	int			GetSummerTimeInfo(int nStart, int nEnd);		//	시카고 섬머타임 알아오는 함수

//	void		SetInquiryDlg(CWnd*	pWnd) { m_pInquiryDlg = pWnd; };
	void		LastInfoSave();		//	마지막 설정 환경으로 띄우기 위해서 저장로직 추가
	void		FileCopyAndSaveAndDelete(int nType);
// <-- [Edit]  강지원 2008/10/21
	void		ConvertHogaData(LPVOID	aTRData, long dwTRDateLen);
	void		ConvertKoscomData(LPCSTR szTR, char* pData, long dwTRDateLen);

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
	int	m_nMarketTrCode;

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
	void	RemoveDrdsFromCode(LIST_CODEDATA *pListRealCodes, CString strCode = "");

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	void	AddRealCodeList(LPCTSTR lpszCode);
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	void	AddRealCodes();

	LIST_CODEDATA m_ListStockRealCodes;
	LIST_CODEDATA m_ListUpjongRealCodes;
	LIST_CODEDATA m_ListFutOptRealCodes;
	LIST_CODEDATA m_ListStockFutOptRealCodes;
	LIST_CODEDATA m_ListForeignRealCodes;
	void SetSystemChart(BOOL bSystem){m_bSystemChart = bSystem;};

	// 상품선물옵션차트 - ojtaso (20071001)
	USHORT GetDotPosition(CString& strItemCode);

	//장전데이터 처리
	BOOL m_bPreMarket;

	//틱데이터 없을경우 세팅
	char m_cUT;

	//2007.04.25 by LYH 받은 코드(현재 CodeCombo 코드와 비교해서 다를 경우 재조회)
	CString m_strRecvCode;

	//2007.05.02 by LYH 서버 통계 위해 서비스 ID 추가
	CString m_strSvcID;

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

	// 돋보기 구현 - ojtaso (20080519)
	CDC m_memoryDC;
	CBitmap m_bufferBitmap;
	void CreateBufferBitmap(CDC* pdc, const CRect& rcBounds);

	CDlgGlass* m_pDlgGlass;
	void OnMagnifyingglass(BOOL bShow);
	
	// 달러조회
	void RequestDollarData();

	// 분석툴바 메뉴추가
	BOOL GetDlgToolBtnToggle(CArray<int, int>& arAnal, CArray<int, int>& arAnalCheck, CArray<int, int>& arAddition, CArray<int, int>& arAdditionCheck, CArray<int, int>& arUser, CArray<int, int>& arUserCheck, CMap<int, int, CString, CString>& mapToolName);
	void SetDlgToolBtnToggle(int nToolBtnID, BOOL bToggle);
	
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
	// OutLookbar
	COLORREF m_clrOLBkgrnd;
	COLORREF m_clrOLOutLine;
	
	COLORREF GetBkgrndColor();	//	{ return m_clrBackground; }
	void ChangeSkinColor();

	// 저장화면
	CString m_strSaveKey;
	void SaveChart(BOOL bType = TRUE);

	CString GetLinkInfoData();

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	void WriteToStringLog( const char *p_szSection, const char *p_szData, int p_nDataLen);
	void CreateLogFile();
	void CloseLogFile();

	HFILE		m_hLogFile;			// Log File Handle
#endif

#ifdef _WRITELOG_SISE
	// Log - ojtaso (20080107)
	void WriteToStringLog( const char *p_szSection, const char *p_szData, int p_nDataLen);
	void CreateLogFile();
	void CloseLogFile();

	HFILE		m_hLogFile;			// Log File Handle
#endif

// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
public:
	static CString	GetRealItemCode( const char *p_szItemCode, BOOL	bOrgCode = FALSE );

	
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
		
		STDMETHOD(RecieveKeyData)(int nKey, LPCSTR szData);
		
		STDMETHOD(RecieveRealDataEx) (long dwKey, LPCTSTR pszCode, LONG dwData);
		
		STDMETHOD(ReceiveOnUser) (WPARAM wParam, LPARAM lParam);

		STDMETHOD(ReceiveNoticeData) (LPCTSTR pszCode, LONG dwData);
	} m_xPartnerSite;
	friend class CPartnerSite;

	int		GetMarketTypeFromCode(CString strItemCode, int& nCodeType);

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
protected:
	int			m_nRequestPairTickState;
	BOOL		m_bOrgAddMode;
	CString		m_strPairTickCode;
public:
	void	StartRequestPairTick( const char *p_strPairTickCode, int p_nRequestPairTickState = E_RPTS_START);
	void	EndRequestPairTick( void);
	void	RequestPairTick( const char *p_strPairTickCode, int p_nRequestPairTickState = E_RPTS_PAIR);
	void	ClearPairTick( const char *p_strRQ);
	int		GetRequestPairTickState( void);
	void	SelectMainRQ( void);
	void	SetRequestPairTickState( int p_nRequestPairTickState);
	CString	GetRequestPairTickCode() { return m_strPairTickCode; };

//	받은 데이터에서 OffSet 정보를 이용하여 데이터 값을 추출하는 함수
	long	DataSeparation(CString strData, long nMask, long nTickSize, int nLog, int nPrecision);

public:
	HWND	m_hWinixWnd;

	void	_DoFirstRequest();
	BOOL	GetFirstCode(CString &rString, int nMarketType, int &nSetp);

	static	CMapStringToString		m_mapCodeType;
	static	CString	GetCodeType(CString strCode);
	static	void	SetCodeType(CString strCode, CString strType)	{ m_mapCodeType.SetAt(strCode, strType);	};

	// 차트 주문 연동 - onlyojt (20091204)
	void	SetOrderLineInfo(BOOL bSet);
	//<<< 20091221 by kmlee 주문연동..
	void	UpdateCheJanOption ();
	//>>>

	void	ReceiveNoticeData(LPCTSTR pszCode, LONG dwData);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDDIALOG_H__CF66AE4F_AD1A_4057_B00E_F9FAA4C3A30F__INCLUDED_)

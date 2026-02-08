// FONetworkModule.h: interface for the CFONetworkModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONETWORKMODULE_H__DA552A90_F563_4171_9EFE_335D96612D0E__INCLUDED_)
#define AFX_FONETWORKMODULE_H__DA552A90_F563_4171_9EFE_335D96612D0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dataset.h"

// class CCommonFrameWorkWnd;
class CBaseNetworkModule;
class CNetworkModule;


class CFONetworkModule  
{
public:
	CFONetworkModule(CWnd* pWndMainFrame);
	CFONetworkModule(CWnd * pWnd, CWnd* pWndMainFrame);
	virtual ~CFONetworkModule();

	//////////////////////////////////////////////////////////////////////////
	// 김성곤 추가 2003.02.06
protected:
	char m_RankBit;								// 전송 데이터 구분을 위한 데이터

	int m_iPriceProgressRequestCount;			// 가격추이 요청 개수

	//////////////////////////////////////////////////////////////////////////
	BOOL m_bCommUseBaseModule;
public:

	//20080308 이문수 >>
	CPriceProgressInputList		m_PriceProgressInputList;
	//20080308 이문수 <<

	//20080301 이문수 >>
	CTotalIVDataArray			m_TotalIVArray;
	//20080301 이문수 <<

	//20080304 이문수 >>
	CIV4ItemArray				m_IVItemArray; 
	//20080304 이문수 <<

	CIVPastData					m_IVPastData;

	COptionIVMonthSet			m_OptionIVMonth;

	CAverageIV					m_AverageIV;

	CMarginInfo	*				m_pMarginInfo;


	CItemInfoArray				m_FutureMonthInfoArray;

	//기초자산 배열.
	CUnderlyingAssetsInfoArray	m_UnderlyingAssetsInfoArray;

	//월물별 옵션현재가 저장버퍼..
	COptionMonthInfo *		m_pMonthOpt;


	//증거금용 이론가 버퍼..
	CTheoryForMarginArray	m_TheoryForMarginArray;


	//계좌명을 얻어오는 클래스
	CAccount		m_AccountName;


	//선물옵션과 주식계좌정보가 있다.
	CAccountInfo	m_AccountInfo; 



	//잔고/일반시세 등등..(ACCOUNT_INFO, SISE_INFO, ..)
	int		m_nRequestDataType;



	//현재 요청한 조회데이터를 실시간으로 등록할 것인지 여부.
	BOOL	m_bRTData;



	//해당 개체를 생성시킨 부모윈도우 포인터.
	CWnd * m_pParentWnd;



	//기본 데이터 통신모듈 포인터 -> 생성된 모듈의 포인터만 참조.
	//(각 개체별로 생성하는 것이 아니다)
	CBaseNetworkModule * m_pBaseWnd;



	//개체별로 하나씩 생성(실제 서버와 통신하는 모듈)
	//이 모듈은 서버가 바뀔 때마다 바뀌어야 한다.
	CNetworkModule *	 m_pNetwork;



	//조회하고 받은 데이터들의 버퍼.
	CDataArray			 m_ReceiveDataArray;



	//조회할 종목들을 저장하고 있는 버퍼.
	//조회가 끝나면 이 버퍼는 비어있어야 한다.
	//CStringArray		 m_strRqCodeArray;
	CDataArray			 m_RqCodeArray;



	//실시간 관련...
	CDataArray			 m_RTDataArray;

	//실시간 시세 데이터들을 미리 예약하는 버퍼.
	CStringArray		 m_strAddRTDataArray;
	
	CWnd*				m_pWndMainFrame;

public:
	CString GetLastErrorMsg();
	CString GetLastMsg();
	void ResetKey();
	void SendBackKey(CString strCode);

	CString Parser(CString &strSrc, CString strSub);

	int SendReqFundID(CString strConti, CString strContiFlg);

	int RequestIVPastData(CIVPastInputData & IVPastInputData);

	int RequestOptionPastIV(CString strEndDate);

	int RequestAverageIV(CString strUnderlyingAssetsCode);

	int RequestMarginInfo(CString strAccount, CString strPassword);

	//20080228 이문수 >>
	int RequestTotIV(CString strBaseCode, char chJanFlag, int nTotWorkDateCnt, 
					   CString strStartTime, CString strEndTime);
	//20080228 이문수 <<
	
	//20080304 이문수 >>
	int RequestIVDate(CDataArray & DataArray);
	//20080304 이문수 <<

	int RequestTodayAverageIV(CString strInput);; // 하나대투용

	int RequestDeltaSum(void* pInput);				// 델타 헷지 데이터 요청

	BOOL FindDataSet(CDataSet * pDataSet);

	void SetUnderlyingAssetsArray();

	//월물옵션 데이터 요청...
	int RequestMonthData(CString strUnderlyingAssetsCode, CString strYM);

	//////////////////////////////////////////////////////////////////////////
	// 김성곤 추가 2003.02.06

	// 가격추이(선물&옵션&주식옵션&주식)
	int RequestPriceProgressData(CStringArray &sarrCode, LPCTSTR pszDate, LPCTSTR pszTime, 
		int iDataCount, char DataBit);

	//////////////////////////////////////////////////////////////////////////

	//증거금용 이론가 요청.
	//int RequestTheoryForMargin(CStringArray & strCodeArray);
	int RequestTheoryForMargin(CDataArray & DataArray);

	/* 입력된 계좌에 대해서만 잔고를 불러오는 함수 */
	int RequestAccountData(CString strAccount, CString strPassword);

	//잔고요청..
	int RequestData(CString strAccount, CString strPassword);

    int RequestData(CString strOrgSect, CString strOrgCd, CString strFundId, CString strSavekey);

	//시세요청..
	int RequestData(CDataSet * pDataSet, BOOL bRTData = TRUE);

	int RequestData(CDataArray	& DataArray, BOOL bRTData = TRUE);

	//시세 받기..
	int ReceiveData(WPARAM wParam, LPARAM lParam);

	//실시간 시세 등록
	BOOL AddRTData(CDataSet * pSet);

	//실시간 시세 받기 위해 등록....
	BOOL AddRTData(CDataArray & DataArray);

	//실시간 동작하던 종목 하나를 삭제할 경우..
	BOOL DelRTData(CString strCode);

	//실시간 동작한던 종목리스트를 삭제...
	BOOL DelRTData(CStringArray strCodeArray);

	//실시간 동작하던 모든 종목리스트를 삭제...
	BOOL DelAllRTData();

	//실시간 동작하던 한 종목에 id array를 지우는 부분.
//	BOOL DelRTData(CDataSet * pSet);

	//실시간 동작하던 여러종목의 여러 id array를 지우는 부분.
	BOOL DelRTData(CDataArray & DataArray);

	//실시간 시세 받는 함수..
	BOOL ReceiveRTData(WPARAM wParam, LPARAM lParam);

	BOOL IsCommUseBaseModule()		{ return m_bCommUseBaseModule; };
};
typedef CArray < CFONetworkModule * , CFONetworkModule *> CFONetworkArray;


#endif // !defined(AFX_FONETWORKMODULE_H__DA552A90_F563_4171_9EFE_335D96612D0E__INCLUDED_)

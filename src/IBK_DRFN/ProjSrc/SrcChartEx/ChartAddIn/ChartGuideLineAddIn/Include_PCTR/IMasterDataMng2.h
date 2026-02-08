#if !defined(AFX_INTERFACE_OF_MASTERDATAMANAGER2_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_MASTERDATAMANAGER2_BY_JUNOK_LEE__INCLUDED_

// #include "../../_include/IMasterDataMng.h"
// 

#include "IBaseDefine.h"
#include "./MasterDefine.h"

/* -------------------------------------------------------------------------
설명 : 마스터파일을 핸들링
관련문서 : 정리_마스터파일.doc
만든이  : 이준옥 2005.11.25(금)
History : 2005.11.25 인터페이스 정의
----------------------------------------------------------------------------
- IMasterDataManager인터페이스가 제대로 동작하기 위해서는
  메인에서 CodeOcx를 IMainExInfoManager를 통해 세팅해줘야 한다.

int nID = 5;
IMasterDataManager* pManager = (IMasterDataManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
IMasterDataManager* pManager = (IMasterDataManager*)AfxGetPctrInterface(nID);

ex1>
	CList<CJMaster, CJMaster>* list = 
		(CList<CJMaster, CJMaster>*)pManager->GetCodePointer("JMASTER"); 

ex2>
	int nID = 5;
	IMasterDataManager* pManager = (IMasterDataManager*)AfxGetPctrInterface(nID);
	if(pManager==NULL)
	{
		AfxMessageBox("IMasterDataManager Not Loaded");
		return;
	}
	int nTotal = pManager->LoadMASTER_J_K();
	CMaster_J_K* pData = pManager->J_K_GetMaster();
	for(int i=0; i<nTotal && i<3; i++)
	{
		CString s;
		s.Format("Code[%s], Name[%s]\n", pData[i].m_szCode, pData[i].m_szName);
		AfxMessageBox(s);
	}
	pManager->UnLoadMASTER_J_K();

ex3>
	...여기에..

----------------------------------------------------------------------------
마스터파일은 $DATA/Master디렉토리에 위치함.
현재(205.11.25금)는 GetCodeList()함수만 구현됨.
// {{ Code.ocx 와 // }} Code.ocx 으로 묶여있는 부분은 기존은 code.ocx내용을 
그대로 콜하는 부분으로 파라미터나 기능에 관해서는 code.ocx개발자에게 문의바람.
------------------------------------------------------------------------- */

DECLARE_INTERFACE(IMasterDataManager2)
{
	// --------------------------------------------------- 
	// [GetCodeInfo]개별코드명,코드값, 코드구분 조회
	// I-PARAM : szCodeKey 마스커키값(대문자)
	//                  예) "JMASTER", "KMASTER"
	//			 nFlag 0:코드값으로 코드명 조회
	//					1 : 코드명으로 코드값 조회
	//					2 : 코드구분 리턴
	//			 szKey
	// O-PARAM : 해당데이터
	// --------------------------------------------------- 
	STDMETHOD_( LPCTSTR, GetCodeInfo2)(LPCTSTR szCodeKey, short nFlag, LPCTSTR szKey) PURE;

/*
	 --------------------------------------------------- 
	-. 설명[GetBaseCodeTable]
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- 
	STDMETHOD_(long, GetBaseCodeTable)() PURE;

	 --------------------------------------------------- 
	-. 설명[SetInfo] codeocx 내부적으로도 하는 기능 없음.
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- 
	STDMETHOD_(void, SetInfo)(long dwInfo) PURE;

	 --------------------------------------------------- 
	-. 설명[ShowCodeTable]
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- 
	STDMETHOD_(long, ShowCodeTable)(LPCTSTR szCodeKey, const VARIANT& hWnd) PURE;

	 --------------------------------------------------- 
	-. 설명[ShowModeCodeTable]내부적으로도 하는 기능 없음.
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- 
	STDMETHOD_(long, ShowModeCodeTable)(LPCTSTR szCodeKey, const VARIANT& hWnd) PURE;

	 --------------------------------------------------- 
	-. 설명[SetDataString]
	-. I-PARAM : nKey	10 : RootPath세팅, szData는 RootPath.
						20 : 데이터를 읽도록 요청, szData사용안함.
						30 : 6자리코드 0 자동 등록 유무, 1 = 자동등록, 0 = 일반
		입력파라미터에 대한 자세한 내용은 code.ocx개발자에게 문의바람.
	-. O-PARAM :
	--------------------------------------------------- 
	STDMETHOD_(void, SetDataString)(short nKey, LPCTSTR szData) PURE;

	 --------------------------------------------------- 
	-. 설명
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- 
	STDMETHOD_(BOOL, InitData)(LPCTSTR szRootPath) PURE;

	 --------------------------------------------------- 
	-. 설명
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- 
	STDMETHOD_(void, SetPosition)(LPCTSTR szKey, short nX, short nY) PURE;

	 --------------------------------------------------- 
	-. 설명
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- 
	STDMETHOD_(CString, GetMaxHangSaGa)() PURE;

	 --------------------------------------------------- 
	-. 설명
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- 
	STDMETHOD_(long, ShowCodeTableWithPosition)(LPCTSTR szCodeKey, const VARIANT& hWnd, short nX, short nY) PURE;

	 ---------------------------------------------------
	//	기능 :	모든 리스트에 있는 데이터를 삭제한다.
	//			마스터파일이 메인이 실행중 업데이트 될때
	//			예전의 리스트를 없애야만 새로 읽어 들인다.
	--------------------------------------------------- 
	STDMETHOD_(BOOL, RemoveAllListData)() PURE;
	
	*********************************************************************************
	-. 설 명 : App 에 있는 m_HistoryList 에 데이터를 추가한다.
	-. 변 수 
	short nMaxCount  : History 창에 보여줄 List 의 갯수
	long nType       : 28byte(CodeInput 의 CodeInput 의 m_szCodeChoice 의 long 값)
	LPCTSTR Code     : Code (예 : 00660 )
	LPCTSTR CodeName : Code Name ( 예 : 하이닉스 )
	-. 리턴값 : 동일한 history 가 있으면 FALSE, 아니면 TRUE 를 리턴한다.
	**********************************************************************************
	STDMETHOD_(BOOL, InsertHistory)(short nMaxCount, long nType, LPCTSTR Code, LPCTSTR CodeName) PURE;
	
	*********************************************************************************
	-. 설 명 : nType 에 맞는 History 데이터를 리턴한다.
	-. 변 수 
	   short nMaxCount  : History 창에 보여줄 List 의 갯수
	   long nType       : 28byte(CodeInput 의 CodeInput 의 m_szCodeChoice 의 long 값)
	-. 리턴값  : 타입에 맞는 코드/코드명을 리스터를 리턴한다.
		양식: "Code0=CodeName0;Code1=CodeName1;" 로 리턴한다.
	**********************************************************************************
	STDMETHOD_(CString, GetHistoryList)(short nMaxCount, long nType) PURE;

	 --------------------------------------------------- 
	-. 설명
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- 
	STDMETHOD_(long, GetGlobalDataTypeList)() PURE;

	*********************************************************************************
	// 마스터키에 의한 종목리스트를 리턴한다.
	// I-PARAM : 마스터파일의 키값인데, 파일명에서 점(.)포함 확장자를 뺀 대문자.
	//           MasterDefine.h 파일을 참고하기 바람.
	// O-PARAM : 마스터파일에 따라 형변환을 해줘야 함.
	// "JMASTER"	---->	CList<CJMaster, CJMaster>* list = 
	**********************************************************************************
	STDMETHOD_(long, GetCodePointer)(LPCTSTR szCodeKey) PURE;	
	
// Code.ocx
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// 여기까지가 Code.ocx와 과련된 부분
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 주식종목마스터에서 업종에 해당하는 종목구하기
	// 리턴받을 리스트 pReturnList은 CList<CJMaster, CJMaster>*
	STDMETHOD_(int, GetJMaster_Upcode)(LPCTSTR szUpcode, long pReturnList) PURE;	

	// 코스닥종목마스터에서 업종에 해당하는 종목구하기
	// 리턴받을 리스트 pReturnList은 CList<CJMaster, CJMaster>*
	STDMETHOD_(int, GetKMaster_Upcode)(LPCTSTR szUpcode, long pReturnList) PURE;	


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// 여기부터 KOSCOM과 과련된 부분
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	STDMETHOD_(BOOL, GetOptionJongMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, 
		CStringArray *&pastrJongEngName, CStringArray *&pastrEventPrice, 
		CStringArray *&pastrFinalYM, CStringArray *&pastrValueGubun )	PURE;

	// Koscom 선물마스터데이터 구하기
	STDMETHOD_(BOOL, GetFutureJongMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;

	// Koscom 코스피업종마스터데이터 구하기
	STDMETHOD_(BOOL, GetKSPUpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;

	// Koscom 코스닥업종마스터마스터데이터 구하기
	STDMETHOD_(BOOL, GetKSQUpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;

	// 해외지수 마스터데이터 구하기
	STDMETHOD_(BOOL, GetFRMst)(CStringArray *&pastrCode, CStringArray *&pastrName) PURE;

	// 해외국가 마스터데이터 구하기
	STDMETHOD_(BOOL, GetFRNationMst)(CStringArray *&pastrCode, CStringArray *&pastrName) PURE;


	// ELW 마스터데이터 구하기
	STDMETHOD_(BOOL, GetELWMst)(CStringArray *&pastrCode, CStringArray *&pastrName, CStringArray *&pastrEngName) PURE;

	// koscom 전체 종목 마스터 데이터 구하기
	STDMETHOD_(BOOL, GetMasterJongCodeList)(CStringArray *&pastrKind, CStringArray *&pastrJongCode, CStringArray *&pastrName) PURE;

	// 상품선물옵션 2차 - ojtaso (20071016)
	// 상품 선물옵션 종류
	STDMETHOD_(BOOL, GetMarketArray)(CByteArray *&pabyteMarket) PURE;

	// 상품선물옵션 2차 - ojtaso (20071016)
	// 시장별 종목
	STDMETHOD_(BOOL, GetMarketItemName)(BYTE nMarket, CStringArray *&pastrJongName, BOOL bSpread) PURE;

	// 상품선물옵션 2차 - ojtaso (20071016)
	// 시장별 종목
	STDMETHOD_(BOOL, GetMarketItemCode)(BYTE nMarket, CString& strItemName, CString& strItemCode, BOOL bSpread) PURE;

	// 상품선물옵션 2차 - ojtaso (20071016)
	// 상품 옵션코드
	STDMETHOD_(BOOL, GetCommodityOptionItemCode)(BYTE nMarket, CStringArray *&pastrJongCode, CArray <int, int> *&panCallPut, CArray <int, int> *&panYear, CArray <int, int> *&panMonth, CArray <int, int> *&panEvtPrice) PURE;

	// 상품선물옵션 2차 - ojtaso (20071016)
	// 상품 옵션코드
	STDMETHOD_(BOOL, GetCommodityOptionItemCode)(BYTE nMarket, int nCallPut, int nYear, int nMonth, double dEvtPrice, CString& strItemCode) PURE;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	// For JMaster/KMaster
	// 자주 사용되는 JMASTER와 KMASTER를 위해서 추가한 기능
	// 사용법
	// 1. LoadMASTER_J_K(..)
	// 2. J_K_GetMaster(..)
	//    J_K_SortByName(..)
	//    J_K_SortByCode(..)
	// 3. UnLoadMASTER_J_K(..)
	// 로드는 여러번 하더라도 한번만 로드되며,
	// 언로드는 부르지 않더라도 자동적으로 내려간다.
	*********************************************************************************
	-. 설 명 : [LoadMASTER_J_K]JMASTER와 KMASTER를 읽어들인다.
				이때 읽어들이는 형태는 CMaster_J_K 형태이며,
				종목명과 종목코드 위주의 정보를 제공한다.
	-. 변 수 :
	-. 리턴값  : 종목Count
	**********************************************************************************
	STDMETHOD_(int,  LoadMASTER_J_K)() PURE;

	*********************************************************************************
	-. 설 명 : [UnLoadMASTER_J_K]
	-. 변 수 :
	-. 리턴값  : 
	**********************************************************************************
	STDMETHOD_(void, UnLoadMASTER_J_K)() PURE;

	*********************************************************************************
	-. 설 명 : [J_K_GetMaster]
	-. 변 수 :
	-. 리턴값  : 
	**********************************************************************************
	//STDMETHOD_(CMaster_J_K*, J_K_GetMaster)() PURE;

	*********************************************************************************
	-. 설 명 : [J_K_JMasterCount]
	-. 변 수 :
	-. 리턴값  : 
	**********************************************************************************
	STDMETHOD_(int, J_K_JMasterCount)() PURE;


	*********************************************************************************
	-. 설 명 : [J_K_KMasterCount]
	-. 변 수 :
	-. 리턴값  : 
	**********************************************************************************
	STDMETHOD_(int, J_K_KMasterCount)() PURE;


	*********************************************************************************
	-. 설 명 : [J_K_SortByName] 이미 로드된 상태에서 이름으로 소팅
	-. 변 수 :
	-. 리턴값  : 
	**********************************************************************************
//	STDMETHOD_(CMaster_J_K*, J_K_SortByName)() PURE;

	*********************************************************************************
	-. 설 명 : [J_K_SortByCode] 이미 로드된 상태에서 코드로 소팅
	-. 변 수 :
	-. 리턴값  : 
	**********************************************************************************
//	STDMETHOD_(CMaster_J_K*, J_K_SortByCode)() PURE;

	// For JMaster/KMaster

	// 마스터파일 소트를 효율적으로 할 수 있지만, 현재되어있는 구조를 유지하기 위해서 각각 추가한다.
	// szMaster는 특정 마스터키 하나씩
	// 예) "JMASTER"
	STDMETHOD_(void, CodeSort)(LPCSTR szMaster, void* pList ,int nCount) PURE;


	// --- 구현 안됨 ----
	STDMETHOD_(BOOL, IsKOSPI) (LPCSTR szCode) PURE;
	STDMETHOD_(BOOL, IsKosdaq)(LPCSTR szCode) PURE;
	STDMETHOD_(BOOL, IsStock) (LPCSTR szCode) PURE;
	STDMETHOD_(BOOL, IsFuture)(LPCSTR szCode) PURE;
	STDMETHOD_(BOOL, IsSpread)(LPCSTR szCode) PURE;
	STDMETHOD_(BOOL, IsStockOption)(LPCSTR szCode) PURE;
	STDMETHOD_(BOOL, IsOption)(LPCSTR szCode) PURE;
	STDMETHOD_(BOOL, IsCall)(LPCSTR szCode) PURE;
	STDMETHOD_(BOOL, IsPut)(LPCSTR szCode) PURE;
	STDMETHOD_(BOOL, IsKP200)(LPCSTR szCode) PURE;
	STDMETHOD_(BOOL, IsKQ50)(LPCSTR szCode) PURE;
	STDMETHOD_(BOOL, CheckKP200Sect)(LPCSTR szCode) PURE;
	// STDMETHOD_(int, GetItemGubun(LPSTR lpszInfo));
	// int static		GetFutOptMarketGubun(CString code);
	//static CString	GetExcercisePrice(int nGubun, CString szData);
	//static CString	GetExpiryMonth(int nGubun, CString szData);
	// static BOOL		CheckItem(int nCheckFlag,CString szItemSect);
	// THEME 마스터데이터 구하기
	STDMETHOD_(BOOL, GetThemeCodeList)(CStringArray *&pastrCode, CStringArray *&pastrName, CStringArray *&pastrEngName) PURE;

	STDMETHOD_(CString, GetBizDate)() PURE;
*/
};

#define	USERINFOMNG_USERID			0
#define	USERINFOMNG_USERENCID		1
#define	USERINFOMNG_ROOTDIR			10
#define	USERINFOMNG_DATADIR			12
#define	USERINFOMNG_REALUSERDIR		17



#endif //AFX_INTERFACE_OF_MASTERDATAMANAGER2_BY_JUNOK_LEE__INCLUDED_

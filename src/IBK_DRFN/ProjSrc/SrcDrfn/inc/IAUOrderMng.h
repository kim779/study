#if !defined(AFX_ORDER_INTERFACE__BY_LYH_968A_755DC4FC4516__INCLUDED_)
#define AFX_ORDER_INTERFACE__BY_LYH_968A_755DC4FC4516__INCLUDED_

// #include "../../_include/IAUOrderMng.h"
//  int nID = 11;
// 	IAUOrderManager* pMng = (IAUOrderManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUOrderManager* pMng = (IAUOrderManager*)AfxGetPctrInterface(nID);
//  IOrder* pOrder = pMng->AdviseIOrder(hWnd, pITrCommSite);
//  pMng->UnAdviseIOrder(hWnd);

// 개념 : 각각의 데이터에 변경하지 안하을 고유 ID를 부여한다.
// IOrderSite 를 구현할 곳은 데이터를 받는 Client부분이며
// IAUOrderManager 를 구현할 곳은 데이터를 제공하는 Server부분이다.
//
// 만든이  : 이윤희 2005.11.09(화)
// History : 2005.11.09 인터페이스 정의
//

// PacketType
#define PT_ST	0	// 주식			: COrder_PT_ST (CommonTR/Define_Order_PT_ST.h)
#define PT_FO	1	// 선물&옵션	: COrder_PT_FO (CommonTR/Define_Order_PT_FO.h)
#define PT_SO	2	// 주식옵션		: COrder_PT_SO (CommonTR/Define_Order_PT_SO.h)

// return value of SendOrder
#define RV_SUCCESS		1	// 주문 처리 결과 : 주문 전송
#define RV_CONF			-1	// 주문 처리 결과 : 주문확인창에서 취소
#define RV_ERROR_PARAM	-2	// 주문 처리 결과 : 주문 파라미터 에러
#define RV_ERROR_DATA	-3	// 주문 처리 결과 : 주문 데이타 에러

// 주문 확인창
#define CONF_NONE	0	// 안띄움
#define CONF_SHOW	1	// 띄우기
#define CONF_MAIN	2	// 안띄움(사용안함)

// return value of RequestTR45162
#define RV_ERROR_45162_ACCT		-54	// 계좌
#define RV_ERROR_45162_PASS		-55	// 비밀번호
#define RV_ERROR_45162_CODE		-56	// 종목코드


/*
DECLARE_INTERFACE(IOrderSite)
{
};
*/

DECLARE_INTERFACE(IOrder)
{
	// -----------------------------------------------------------------------
	//	주문 전송
	// I-PARAM : 주식/선옵/주옵 타잎, TR번호, 주문Cnt, Map번호, 데이타스트럭쳐, 주문확인창 사용여부, 데이타검증 사용여부
	//		LPVOID pData : 데이타스트럭쳐 정의 파일
	//			#include "../CommonTR/Define_Order_PT_FO.h" --> 선물옵션주문 패킷
	//			#include "../CommonTR/Define_Order_PT_ST.h" --> 주식주문 패킷
	//			#include "../CommonTR/Define_Order_PT_SO.h" --> 주식옵션주문 패킷
	// O-PARAM : 주문 처리 결과
	STDMETHOD_(int, SendOrder)(int nType, LPCSTR szTrNo, int nCount, LPCSTR szMapNo, LPVOID pData, int nFlag=CONF_MAIN, int nVerifyFlag=FALSE) PURE;
	
	// -----------------------------------------------------------------------
	//	주문 가능수량 TR 조회
	// I-PARAM : CTR_45162_I (참조 : CommonTR/Tr45162.h)
	// O-PARAM : 주문 처리 결과
	STDMETHOD_(int, RequestTR45162)(LPVOID pData) PURE;

	// -----------------------------------------------------------------------
	//	주문 디버깅용
	// I-PARAM :
	// O-PARAM :
	STDMETHOD_(void, SetDebug)(LPVOID pST_Cond, LPCSTR szFileName, BOOL bOnOff=FALSE) PURE;

	// -----------------------------------------------------------------------
	//	비상용
	// I-PARAM :
	// O-PARAM :
	STDMETHOD_(void, ExtraAction)(int nType, LPCSTR szData, LPVOID pVoid) PURE;

	// -----------------------------------------------------------------------
	//	데이타 암호화 여부
	// I-PARAM : 기본값 - 암호화한다. 
	// O-PARAM : 
	STDMETHOD_(void, SetEnc)(BOOL bEnc = TRUE) PURE;
	//	공인인증 사용여부
	// I-PARAM : 기본값 - 공인인증을 사용한다. (로긴시 고객의 선택과 부합되는 경우만 시행됨)
	// O-PARAM : 
	STDMETHOD_(void, SetCert)(BOOL bCert = TRUE) PURE;
	
	// -----------------------------------------------------------------------
	//	주문확인창 사용
//	STDMETHOD_(void, SetBaseDesk)(LPVOID pBaseDesk) PURE;
//	STDMETHOD_(void, SetBaseDeskIUnknown)(LPUNKNOWN pUnKnown) PURE;
	// I-PARAM :	szConfMapNo		- 주문확인창 화면번호 (맵에서 사용하는 번호와 동일한 번호)
	//										* 주식기본 :"10012"  
	//										* 주식일괄 :"10201"
	//										* 선옵기본 :"14012"
	//										* 선옵일괄 :"14148"
	//				bMarketPrice	- 일괄주문의 시장가주문여부 ex) [1315]일괄시장가매도, [1365]시장가일괄청산
	//				bSettleOrder	- 일괄주문의 청산주문 여부	ex) [1365]일괄청산, 시장가일괄청산
	//				nMultiOrdType	- 일괄주문의 형식 (1:일괄매도, 2:일괄매수, 3:일괄정정, 4:일괄취소 (0:기본-일괄청산주문의 경우))
	STDMETHOD_(BOOL, SetOrderConfMapNo)(LPCSTR szConfMapNo, BOOL bMarketPrice=FALSE, BOOL bSettleOrder=FALSE, int nMultiOrdType=0) PURE;

};


DECLARE_INTERFACE(IAUOrderManager)
{
	STDMETHOD_(LONG, AdviseIOrder) (HWND hWnd, LONG pITrCommSite) PURE;
	STDMETHOD_(LONG, UnAdviseIOrder)  (HWND hWnd)  PURE;
};



#endif // AFX_ORDER_INTERFACE__BY_LYH_968A_755DC4FC4516__INCLUDED_

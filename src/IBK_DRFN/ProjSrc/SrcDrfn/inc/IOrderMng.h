// IOrderMng.h


// 개념 : 각각의 데이터에 변경하지 안하을 고유 ID를 부여한다.
// IOrderSite 를 구현할 곳은 데이터를 받는 Client부분이며
// IOrderManager 를 구현할 곳은 데이터를 제공하는 Server부분이다.
//
// 만든이  : 이윤희 2005.11.09(화)
// History : 2005.11.09 인터페이스 정의
//

// PacketType
#define PT_ST	0	// 주식
#define PT_FO	1	// 선물&옵션
#define PT_SO	2	// 주식옵션

// return value
#define RV_SUCCESS		1	// 주문 처리 결과 : 주문 전송
#define RV_CONF			-1	// 주문 처리 결과 : 주문확인창에서 취소
#define RV_ERROR_DATA	-2	// 주문 처리 결과 : 주문 데이타 에러
#define RV_ERROR_PARAM	-3	// 주문 처리 결과 : 주문 파라미터 에러

// 주문 확인창
#define CONF_NONE	0	// 안띄움
#define CONF_SHOW	1	// 띄우기
#define CONF_MAIN	2	// 메인의 환경설정을 따르기

//DECLARE_INTERFACE(IOrderSite)
//{
//};


DECLARE_INTERFACE(IOrderManager)
{
	// -----------------------------------------------------------------------
	//	주문 전송
	// I-PARAM : TR번호, 화면핸들, 데이타스트럭쳐, 길이, 주문확인창 사용여부
	// O-PARAM : 주문 처리 결과
	STDMETHOD_(int, Send2Server)(int nType, LPCSTR szTrNo, HWND hWnd, LPVOID pData, int nFlag=CONF_MAIN) PURE;

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
};

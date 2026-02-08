#if !defined(AFX_INTERFACE_OF_ORDERMONITORMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_ORDERMONITORMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/IAUOrderMonitorMng.h"
#include "IBaseDefine.h"

//  int nID = 8;
// 	IAUOrderMonitorManager* pMng = (IAUOrderMonitorManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUOrderMonitorManager* pMng = (IAUOrderMonitorManager*)AfxGetPctrInterface(nID);

// 개념 : 주문모니터링! 주문을 모니터링하는 매니저
//
// 만든이  : 믿음을 주는 사람 이준옥 2005.12.03(토)
// 믿음의 말 : 꿈은 항상 내 앞에 있다. 나는 내 꿈을 이루기 위해 오늘도 계획을 세워 행동한다.
// History : 2005.12.03 인터페이스 정의
//

//typedef struct : public LOADCONTENTINFO
//{
//	char		LogType[1+1];		// 로그남기는 타입
//									// 0 : 남기는 것 없음. 
//									// 1 : 로그발생시 서버에도 로그저장
//									// 2 : 종료시에만 저장.
//	char		reserved[15];		// 예약으로 미리 할당해 놓음.
//} LOADCONTENTINFOEX;
//#define		LOADCONTENTINFOEX_SIZE	sizeof(LOADCONTENTINFOEX)


typedef struct {
	char  screen_cd     [  4+1];	// 화면ID
	char  screen_nm		[ 40+1];	// 화면이름
	char  screen_hwnd	[  8+1];	// 화면 윈도우핸들
	char  worktime		[  8+1];	// 동작시간
	// ----------------------------------------
	char  flag			[  1+1];	// 'S' : 화면시작
									// 'E' : 화면종료
									// 'C' : 관련정보 클리어
	//------------------아래부분은 세팅하지 마세요---------
	char  logDate		[ 10+1];	// Log날자
	char  logTime		[  8+1];	// Log시간
} DSBM_RUNSTATE;
#define		DSBM_RUNSTATE_SIZE		sizeof(DSBM_RUNSTATE)


typedef struct {
	char  acct_code		[ 13+1];	// 계좌번호
	char  acct_nm		[ 20+1];	// 계좌명
	char  screen_cd     [  4+1];	// 화면ID
	char  screen_nm		[ 40+1];	// 화면이름
	char  screen_hwnd	[  8+1];	// 화면 윈도우핸들
	char  item_cd		[ 12+1];	// 종목코드
	char  item_nm		[ 40+1];	// 종목명
	char  worktime		[  8+1];	// 동작시간
	char  msg			[100+1];	// 전략 및 조건
	DWORD dwKey			       ;	// KEY값
	// ----------------------------------------
} DSBM_BASECOMMON;
#define		DSBM_BASECOMMON_SIZE		sizeof(DSBM_BASECOMMON)

typedef struct : public DSBM_BASECOMMON {
//	DSBM_BASECOMMON 구조체영역..................................
//  ............................................................
	char  trade_sect	[  1+1];	// 매매구분 1:매도, 2:매수, 3:정정, 4:취소
	char  qty			[  9+1];	// 주문수량
	char  prc			[ 13+1];	// 주문가격
	char  auto_sect		[  1+1];	// 1:자동, 2:반자동
	char  flag			[  1+1];	// 종목코드 처리에 대한 상태 플래그
									// 'N' : New(새로운 종목)
									// 'D' : Delete(삭제)
									// 'U' : Update
	//------------------아래부분은 세팅하지 마세요---------
	char  logDate		[ 10+1];	// Log날자
	char  logTime		[  8+1];	// Log시간
} DSBM_RUN;
#define		DSBM_RUN_SIZE		sizeof(DSBM_RUN)


// TRLOG_RUN 구조체는 서버에 TR을 남기기위한 구조체..
typedef struct  {
	char  acct_code		[ 13+1];	// 계좌번호
	char  acct_nm		[ 20+1];	// 계좌명
	char  screen_cd     [  4+1];	// 화면ID
	char  screen_nm		[ 40+1];	// 화면이름
	char  screen_hwnd	[  8+1];	// 화면 윈도우핸들
	char  item_cd		[ 12+1];	// 종목코드
	char  item_nm		[ 40+1];	// 종목명
	char  worktime		[  8+1];	// 동작시간
	char  msg			[100+1];	// 전략 및 조건
	char  aKey			[  8+1];	// KEY값
	// ----------------------------------------
	char  trade_sect	[  1+1];	// 매매구분 1:매도, 2:매수, 3:정정, 4:취소
	char  qty			[  9+1];	// 주문수량
	char  prc			[ 13+1];	// 주문가격
	char  auto_sect		[  1+1];	// 1:자동, 2:반자동
	char  flag			[  1+1];	// 종목코드 처리에 대한 상태 플래그
									// 'N' : New(새로운 종목)
									// 'D' : Delete(삭제)
									// 'U' : Update
	//------------------아래부분은 세팅하지 마세요---------
	char  logDate		[ 10+1];	// Log날자
	char  logTime		[  8+1];	// Log시간
} TRLOG_RUN;
#define		TRLOG_RUN_SIZE		sizeof(TRLOG_RUN)


typedef struct  : public DSBM_BASECOMMON {
//	DSBM_BASECOMMON 구조체영역..................................
//  ............................................................
	char  ord_no		[ 10+1];	// 주문번호
	char  trade_sect	[  1+1];  	// 매매구분 1:매도, 2:매수, 3:정정, 4:취소
	char  prc			[ 13+1];	// 주문가격
	char  current		[ 13+1];	// 주문시 현재가
	char  qty			[  9+1];	// 주문수량


	char  ord_sect		[  2+1];	/* 주문유형[주문구분]        
										선물인 경우    
											01:지정가         
											02:시장가         
											03:조건부지정가   
											04:최유리지정가   

										현물인 경우    
											01:지정가(보통)
											05:시장가
											06:조건부지정가
											12:최유리지정가
											13:최우선지정가
											71:시간외종가
											??:개시전종가			*/										
	char conclsn_sect	[  2+1];	/* 체결조건 (주식 매수/매도/정정)
											00:일반
											01:IOC(잔량즉시취소)
											02:FOK(전량 체결/취소)	*/
	char  auto_sect		[  1+1];	// 1:자동, 2:반자동

	//------------------아래부분은 세팅하지 마세요---------
	char  logDate		[ 10+1];	// Log날자
	char  logTime		[  8+1];	// Log시간

} DSBM_SBLOG;
#define		DSBM_TRADELOG		DSBM_SBLOG
#define		DSBM_SBLOG_SIZE		sizeof(DSBM_SBLOG)


// 필요한 정보가 더 나와서 확장함. 2005.08.31
typedef struct : public DSBM_SBLOG {
	char  logType		[  1+1];	// DSBM_RUN의 flag + 실주문
									// 'N' : 등록
									// 'D' : 취소
									// 'U' : 정정
									// 'R' : 실주문

}DSBM_SBLOGEX;
#define		DSBM_SBLOGEX_SIZE		sizeof(DSBM_SBLOGEX)


// 주문종합모니터링ID(SBMID_)
#define	SBMID_RUNSTATE				0x01	// 동작상태알림
#define SBMID_CONDITION				0x02	// 전략변경상태
#define SBMID_SBLOG					0x04	// 주문로그
#define SBMID_COND_SBLOG			0x07	// 전략변경+주문로그 : 주문로그텝에 로그를 남긴다.

#define SBMID_CLEARALLITEM			0x16	// 해당 화면의 리스트 자동클리어.
											// 화면을 닫을 때 자동으로 해당화면을 정보를 지울때

// 배열의 갯수
//#define		ARRAYCOUNT( array )		(sizeof(array) /sizeof(array[0]))


DECLARE_INTERFACE(IOrderMonitorManager)
{
	// 화면의 동작상태를 나타낸다.
	STDMETHOD_(BOOL, SetState)(DSBM_RUNSTATE* pData, long dwLong=NULL) PURE;

	// 조건의 동작상태를 나타낸다.
	STDMETHOD_(BOOL, SetCondition)(DSBM_RUN* pData, long dwLong=NULL) PURE;

	// 주문로그를 남긴다.
	STDMETHOD_(BOOL, SetTradeLog)(DSBM_SBLOG* pData, long dwLong=NULL) PURE;

};

#endif // AFX_INTERFACE_OF_ORDERMONITORMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_

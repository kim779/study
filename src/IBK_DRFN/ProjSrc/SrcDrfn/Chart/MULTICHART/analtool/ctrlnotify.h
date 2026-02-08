#ifndef __CTRL_NOTIFY_H__
#define __CTRL_NOTIFY_H__

//
// main이 CNeoChart2001Ctrl로의 gateway 역할을 할때 받는 Notify message code
//
#define SEND_CANDLEDATA			0		// 차트 컨트롤에서 마우스 이동시, 해동 위치에 캔들 데이터(시,고,저,종)을 보낸다
#define APPLY_SIGNAL			1		// 신호적용 다이얼로그에서 챠트에 신호적용시킴
#define ADD_INDICATOR			2		// 보조지표추가
#define GET_CODEINFO			3		// 코드에 해당하는 종목명을 얻어온다.
#define ADD_CHART_TOOL			4		// chart tool 설정/toggle
#define CHANGE_CHART_CONFIG		5		// '보조지표추가'에서 사용자가 설정한 config로 chart변경
#define SAVE_CHART_CONFIG		6		// '보조지표추가'에서 사용자가 설정한 config 저장
#define SHOW_SHORTCUT_DLG		7		// 차트 단축키 설정창 띄우기
#define SHOW_SHORTCUT_CONTEXT	8		// 차트 단축키 내용 띄우기 

#define	DLLVIEW_SIZE_INFO		100		// dll 사이즈 조정
#define	NEWSOUTBAR_RELOAD		500		// 뉴스 아웃바에 즐겨찾기 새로고침
#define	GET_CONCERNGROUP		501		// 관심 그룹 얻음
#define GET_CONCERNITEM			502		// 관심 종목 얻음
#define	GET_ACCOUNTINFO			503		// DLL화면에서 사용자 정보를 요청
#define GET_UPJONGMASETR		504		// 각 업종 코드,이름을 얻는다(거래소, 코스닥, 테마)
										// WPARAM 0거래소 1코스닥 2테마
#define	ADD_CONCERNITEM			505		// 관심그룹에 종목 추가
#define GET_USERCOLORTABLE		506		// Color Table 요청
#define GET_SINGLECOLOR			507		// 개별 Color 요청
#define DEL_SHARENAME			508


// 자동매매 설정창 -> 차트
#define	AUTO_START				10		// 자동매매가 시작되었습니다.			
#define AUTO_SETCHART			11		// 자동매매를 시작할 차트핸들 넘겨준다
#define AUTO_SETHEADER			12		// "종목명(종목번호) 일/주/월/분/틱 현재가" 문자열을 넘겨줌

// main -> 자동매매 설정창 or chart -> 자동매매 설정창
#define AUTO_SETACCOUNTINFO		13		// 계좌정보 class(CAcctHelper)의 pointer를 넘겨준다

// 차트 -> 자동주문 Main dlg
#define AUTO_REGISTER			14		// 관리폼에 계좌를 등록한다.
#define AUTO_UNREGISTER			15		// 해당계좌 자동매매 종료
#define AUTO_REFRESH			16		// 신호변경시 관리폼에 update요구

#define AUTO_ROBOTCONFIRM		17		// 차트로 로봇을 보낸다 : chart에서 자동주문Main으로 AUTO_REGISTER
										// 를 보낸뒤 자동주문Main은 chart로 robot handle과 함께보내는 msg code

// 자동주문 Main dlg -> 자동주문 robot
#define AUTO_BUY				18		// 자동 매수 주문
#define AUTO_SELL				19		// 자동 매도 주문
#define CONFIRM_BUY				20		// 확인 매수 주문 ?
#define CONFIRM_SELL			21		// 확인 매도 주문 ?

#define AUTO_CLOSE				22		// 종료 요청 메세지

#define AUTO_SOCKET_HANDLE		23		// main에서 socket handle을 보낼때	

#define CHART_LINE_THICKNESS	24		// 차트 라인 두께
#define CHART_LINE_COLOR		25		// 차트 라인 컬러

// 메인 -> chart
#define AUTO_SET_MAINHANDLE		26		// 자동주문 핸들을 넘길때

#endif

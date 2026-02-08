
//=========================================================
// 2005. 07. 30 by sy, nam
// 쾌속주문내에 지정된 타입과 동일
// 추후 공통파일화 필요
//=========================================================

#ifndef __HANHWA_CHART_STATUS_MSG_H
#define __HANHWA_CHART_STATUS_MSG_H


// 메시지 종류
#define		MSG_GOING				0				// 처리중
#define		MSG_COMPLETE			1				// 처리완료
#define		MSG_SEARCH_END			2				// 조회완료
#define		MSG_NO_ACCT				3				// 계좌번호 없음
#define		MSG_NO_VOLUME			4				// 수량 없음
#define		MSG_NO_CODE				5				// 종목코드 없음
#define		MSG_FALSE_CODE			6				// 종목코드 틀림
#define		MSG_NO_PASS				7				// 비밀번호 없음
#define		MSG_FALSE_PASS			8				// 비밀번호 틀림
#define		MSG_TRUE_PASS			9				// 비밀번호 맞음
#define		MSG_RESERVE_JUMUN		10				// 주문예약 관련
#define		MSG_NOTENOUGH_100CASH	11				// 100% 주문가능현금 부족
#define		MSG_NOTENOUGH_PERCENT	12				// 비율세팅 작음
#define		MSG_SEARCH_FAIL			13				// 조회실패
// ---------- 주문관련 ---------------------------------------------
#define		MSG_JUMUN_OK			20				// 주문접수
#define		MSG_CORRECT_OK			21				// 정정완료
#define		MSG_CANCEL_OK			22				// 취소완료
#define		MSG_MAESU_CHE			23				// 매수주문 체결
#define		MSG_MAEDO_CHE			24				// 매도주문 체결
#define		MSG_REJECT				25				// 주문접수 거부
#define		MSG_REJECT_OK			26				// 주문거부 확인
#define		MSG_CHK_INPUTVOL		27				// 수량 입력 체크
#define		MSG_CHK_JANGO			28				// 잔고 체크
#define		MSG_JUMUN_RELATE		29				// 주문관련일반
// ------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/4/27
// Reason		: 한화 관련 추가 
//-----------------------------------------------------------------------------
// start
#define		MSG_TEMP_MSG			30				// 임시 메시지
// end
// ------------------------------------------------------------------

#endif
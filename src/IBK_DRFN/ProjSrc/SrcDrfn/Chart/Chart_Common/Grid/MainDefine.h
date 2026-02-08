
// 코스피, 코스닥
#define		KOSPI					1				// 코스피(거래소)
#define		KOSDAQ					2				// 코스닥
#define		ETF_KOSPI				7				// 거래소 지수
#define		ETF_KOSDAQ				8				// 코스닥 지수
#define		MARKET_PRICE			99999999		// 시장가

// 스핀 버튼
#define		SPIN_RANGE_START			0		
#define		SPIN_RANGE_END				9999999


// 호가 비교 상수 정의
#define		NOT_FOUND			-1
#define		WITHIN_HOGA			0
#define		BIG_HOGA			1
#define		SMALL_HOGA			2
#define		EQUAL_HOGA			3


// Context 메뉴  ID
#define		MY_CONTEXTMENU_BASE		30100
#define		MY_CONTEXTMENU_END		30200

// 화면번호

#define		SCRNO_STOPORDER				"8700"		// 스톱 오더 
#define		SCRNO_MULTICHART			"8720"		// 멀티 챠트 

//#define		SCRNO_49800				"49800"
//#define		SCRNO_90000				"90000"		// 멀티 챠트 

//#define		SCRNO_10020				"10020"
//#define		SCRNO_49810				"49810"
//#define		SCRNO_90010				"90010"
//#define		SCRNO_90200				"90200"


// 그리드 열, 칼럼 관련		
// JSJ_ITCastle_Modify_040427
// 열, 행 갯수
#define		TENHOGA_ROW_COUNT			29					// 10호가시 열 개수

//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/6
// Reason		: 한화적용시 예약주문 항목 매수 매도 구분하여 분리시킨다.
//-----------------------------------------------------------------------------
// start
//#define	TOTAL_COLUMN_COUNT			10					// 전체 행 개수
#define		TOTAL_COLUMN_COUNT			11					// 전체 행 개수 + 1 로 추가
// end
#define		FIXED_ROW_CNT_TEN			1					// 10호가 고정열
#define		FIXED_ROW_CNT_FULL			3					// 전체호가 고정열
#define		INFO_BTM_ROW_CNT			3					// 하단 Sum Info
#define		FIXED_BTM_ROW_CNT			4					// 하단 고정열

#define		TENHOGA_SAVED_CNT			24					// 10호가 그리드에서 호가저장 갯수

// 칼럼 순서 -------------------------------------------------------------->
//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/6
// Reason		: 예약주문 항목 추가 함에 따라 col 인덱스 수정
//-----------------------------------------------------------------------------
// start
#define		MAEDO_RESERVE_COL			0					// 매도주문예약
#define		LEFT_CAN_COL				1					// 좌측 취소
#define		MAEDO_COL					2					// 매도
#define		MAEDO_JAN_COL				3					// 매도 잔량
#define		HOGA_COL					4					// 호가
#define		DAEBI_COL					5					// 대비
#define		MAESU_JAN_COL				6					// 매수 잔량
#define		MAESU_COL					7					// 매수
#define		RIGHT_CAN_COL				8					// 우측 취소
#define		MAESU_RESERVE_COL			9					// 매수주문예약
#define		BONG_COL					10					// 봉
// end
// <------------------------------------------------------------------------

// 칼럼 넓이 ---------------------------------------------------------------->
#define		CANCEL_COL_WIDTH			45					// 취소
#define		MAEMAE_COL_WIDTH			50					// 매도, 매수 주문
#define		JAN_COL_WIDTH				110					// 매도, 매수 잔량
#define		JAN_COL_WIDTH_SH			74					// 매도, 매수 잔량(우측축소)
#define		HOGA_COL_WIDTH				70					// 호가
#define		HOGA_COL_WIDTH_SH			74					// 호가(우측축소)
#define		DAEBI_COL_WIDTH				50					// 대비
#define		RESERVE_COL_WIDTH			45					// 주문예약
#define		BONG_COL_WIDTH				0					// 봉

#define		MAEMAE_COL_WIDTH_SB			50					// 매도, 매수 주문(스크롤)
#define		JAN_COL_WIDTH_SB			105					// 매도, 매수 잔량(스크롤)
#define		HOGA_COL_WIDTH_SB			74					// 호가(스크롤)
#define		HOGA_COL_WIDTH_SH_SB		70					// 호가(우측축소, 스크롤)
// <--------------------------------------------------------------------------

#define		TENHOGA_ROW_FIRST			0					// 처음
#define		TENHOGA_ROW_5HOSUM			TENHOGA_ROW_COUNT-3	// 5호가범위내 칼럼의 합
#define		TENHOGA_ROW_TOTSUM			TENHOGA_ROW_COUNT-2	// 전체 칼럼의 합
#define		TENHOGA_ROW_EXTRAVOL		TENHOGA_ROW_COUNT-1	// 시간외잔량

#define		MARKETPRICE_ROW				1					// 시장가
#define		ETC_HOGAUP_ROW				3					// 기타 호가 상단
#define		ETC_HOGADOWN_ROW			TENHOGA_ROW_COUNT-5	// 기타 호가 하단

#define		HIGH_ROW					2					// 상한가
#define		LOW_ROW						TENHOGA_ROW_COUNT-4	// 하한가

#define		MAEDO_ROW					4					// 매도호가 시작
#define		PRICE_ROW					MAEDO_ROW+10		// 현재가 열
#define		MAESU_ROW					PRICE_ROW			// 매수호가 시작

#define		ROW_HEIGHT					14					// 열 높이
#define		ROW_WIDTH					80					// 열 넓이
#define		HEIGHT_OFFSET				0					// 현재가 정보 높이 Offset


// Bong Graph
#define		BONGGRAPH_WIDTH				13					// 봉 그래프 폭
#define		BONGGRAPH_LEFT_OFFSET		3					// 셀의 좌측으로부터 3픽셀
#define		BONGGRAPH_PEN_WIDTH			1


// 주문예약
#define		RESERVED_GRID				89490				// 그리드 아이디 값

#define		RSVDGRID_COL_CNT			3					// 그리드 칼럼스
#define		RSVDGRID_ROW_CNT			9					// 그리드 열수
#define		RSVDGRID_FIXED_ROW			1					// 그리드 고정열 수

#define		RSVDGRID_MAEDO_COL			0					// 매도
#define		RSVDGRID_PRICE_COL			1					// Title
#define		RSVDGRID_MAESU_COL			2					// 매수

#define		RSVDGRID_ROW_HEIGHT			15
#define		RSVDGRID_COL_WIDTH			70
#define		RSVDGRID_PRICE_COL_WIDTH	85


//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/10
// Reason		: 체결리스트 관련 추가함 
//-----------------------------------------------------------------------------
// start
// 체결리스트 Dlg
#define		ROW_FIRST					0					// 열 처음 

#define		CHE_ROW_CNT					1
#define		CHE_COL_CNT					2

#define		CHE_PRICE_COL				0
#define		CHE_VOL_COL					1

#define		CHEGYUL_DLG_ROW_FIRST		MAESU_ROW			// 체결리스트 Dlg 시작행
#define		CHEGYUL_DLG_ROW_END			LOW_ROW				// 체결리스트 Dlg 마지막행
#define		INFO_ROW_FIRST				HIGH_ROW			// 정보표시창 시작행
#define		INFO_ROW_END				PRICE_ROW-1			// 정보표시창 마지막행
// end
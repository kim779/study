
#pragma once

#include "../../h/axisvar.h"

#define WM_MANAGE			(WM_USER + 1000)
#define WM_SEND				(WM_USER + 1020)
#define	EVT_SENDCONTENT		1
#define	EVT_SENDNAVIGATE	2

#define MSG_TREE_CLICK		1
#define MSG_COLOR_CHANGE	2
#define MSG_OPTION_CHECK	3
#define MSG_LOG				4
#define MSG_LOG2			5
#define MSG_TREECHK_CLICK	6



//////////////////////// 매체 문자열
#define STR_TOTAL_ROOT			"전체"			// Root 전체
#define STR_NOTICE_ROOT			"공시"			// Root 공시
#define STR_NEWS_ROOT			"시황"			// Root 시황

#define STR_ETODAY_ROOT			"이투데이"		// Root 이투데이
#define STR_ETODAY_SUB1			"증권"
#define STR_ETODAY_SUB2			"금융"

#define STR_HANENWS_ROOT		"한경뉴스"		// Root 한경뉴스 
#define STR_HANNEWS_SUB1		"증권"
#define STR_HANNEWS_SUB2		"금융"
#define STR_HANNEWS_SUB3		"부동산"
#define STR_HANNEWS_SUB4		"경제"
#define STR_HANNEWS_SUB5		"산업/기업"
#define STR_HANNEWS_SUB6		"벤처/중기"
#define STR_HANNEWS_SUB7		"정보/과학"
#define STR_HANNEWS_SUB8		"정치"
#define STR_HANNEWS_SUB9		"국제"
#define STR_HANNEWS_SUB10		"사회"
#define STR_HANNEWS_SUB11		"문화/교양"
#define STR_HANNEWS_SUB12		"레저/스포츠"
#define STR_HANNEWS_SUB13		"인물/동정"
#define STR_HANNEWS_SUB14		"기타"
#define STR_HANNEWS_SUB15		"섹션"

//#define STR_HANENWS_ROOT		"한경뉴스"		// Root 한경뉴스 

#define STR_EDAILY_ROOT			"이데일리"		// Root 이데일리
#define STR_EDAILY_SUB1			"일반"
#define STR_EDAILY_SUB2			"채권"
#define STR_EDAILY_SUB3			"선물"
#define STR_EDAILY_SUB4			"외환"
#define STR_EDAILY_SUB5			"금융"
#define STR_EDAILY_SUB6			"IPO"
#define STR_EDAILY_SUB7			"국제"
#define STR_EDAILY_SUB8			"기업및 벤처"
#define STR_EDAILY_SUB9			"주식"

#define STR_INFOSTOCK_ROOT		"인포스탁"		// Root 인포스탁
#define STR_INFOSTOCK_SUB1		"거래소종목"
#define STR_INFOSTOCK_SUB2		"코스닥종목"
#define STR_INFOSTOCK_SUB3		"해외증시"
#define STR_INFOSTOCK_SUB4		"선물동향"
#define STR_INFOSTOCK_SUB5		"기타"

#define STR_MONEYTODAY_ROOT		"머니투데이"		// Root 머니투데이
#define STR_MONEYTODAY_SUB1		"주식"
#define STR_MONEYTODAY_SUB2		"선물/옵션"
#define STR_MONEYTODAY_SUB3		"해외증시"
#define STR_MONEYTODAY_SUB4		"외환"
#define STR_MONEYTODAY_SUB5		"채권"
#define STR_MONEYTODAY_SUB6		"경제"
#define STR_MONEYTODAY_SUB7		"산업"
#define STR_MONEYTODAY_SUB8		"정보과학"
#define STR_MONEYTODAY_SUB9		"국제"
#define STR_MONEYTODAY_SUB10	"금융/보험"
#define STR_MONEYTODAY_SUB11	"부동산"

#define STR_YOUNHAP_ROOT		"연합뉴스"		// Root 연합뉴스
#define STR_YOUNHAP_SUB1		"정치"
#define STR_YOUNHAP_SUB2		"경제"
#define STR_YOUNHAP_SUB3		"사회"
#define STR_YOUNHAP_SUB4		"문화"
#define STR_YOUNHAP_SUB5		"체육"
#define STR_YOUNHAP_SUB6		"외신"
#define STR_YOUNHAP_SUB7		"지방"
#define STR_YOUNHAP_SUB8		"기타"

#define STR_MAEKYONG_ROOT		"매일경제"			// Root 매경뉴스
#define STR_MAEKYONG_SUB1		"경제"
#define STR_MAEKYONG_SUB2		"금융"
#define STR_MAEKYONG_SUB3		"산업/기업"
#define STR_MAEKYONG_SUB4		"중기/벤쳐/과기"
#define STR_MAEKYONG_SUB5		"증권"
#define STR_MAEKYONG_SUB6		"부동산"
#define STR_MAEKYONG_SUB7		"정치"
#define STR_MAEKYONG_SUB8		"사회"
#define STR_MAEKYONG_SUB9		"인물/동정"
#define STR_MAEKYONG_SUB10		"국제"
#define STR_MAEKYONG_SUB11		"문화"
#define STR_MAEKYONG_SUB12		"레저/스포츠"
#define STR_MAEKYONG_SUB13		"사설/컬럼"
#define STR_MAEKYONG_SUB15		"섹션"
#define STR_MAEKYONG_SUB16		"English News"
#define STR_MAEKYONG_SUB18		"mbn"


#define STR_ETODAY_SUB4			"산업"
#define STR_ETODAY_SUB5			"IT/정보통신"
#define STR_ETODAY_SUB6			"생활경제"
#define STR_ETODAY_SUB7			"인물"

#define STR_NEWSP_ROOT			"뉴스핌"		// Root 뉴스핌
#define STR_NEWSP_SUB1			"주식"
#define STR_NEWSP_SUB2			"채권"
#define STR_NEWSP_SUB3			"외환"
#define STR_NEWSP_SUB4			"국제"
#define STR_NEWSP_SUB5			"금융/재테크"
#define STR_NEWSP_SUB6			"산업"
#define STR_NEWSP_SUB7			"경제"
#define STR_NEWSP_SUB8			"광장"
//#define STR_NEWSP_SUB9			"전문가기고"

enum tid  {	xTotalRoot = 0,				// 트리 INDEX
		xNoticeRoot,
		xNewsRoot,
		xHannewsRoot, 
		xHannewsSub1,
		xHannewsSub2,
		xHannewsSub3,
		xHannewsSub4,
		xHannewsSub5,
		xHannewsSub6,
		xHannewsSub7,
		xHannewsSub8,
		xHannewsSub9,
		xHannewsSub10,
		xHannewsSub11,
		xHannewsSub12,
		xHannewsSub13,
		xHannewsSub14,
		xHannewsSub15,
		xEDailyRoot,
		xEDailySub1,
		xEDailySub2,
		xEDailySub3,
		xEDailySub4,
		xEDailySub5,
		xEDailySub6,
		xEDailySub7,
		xEDailySub8,
		xEDailySub9,
		xInfoStockRoot,
		xInfoStockSub1,
		xInfoStockSub2,
		xInfoStockSub3,
		xInfoStockSub4,
		xInfoStockSub5,

		xMoneytodayRoot,
		xMoneytodaySub1,
		xMoneytodaySub2,
		xMoneytodaySub3,
		xMoneytodaySub4,
		xMoneytodaySub5,
		xMoneytodaySub6,
		xMoneytodaySub7,
		xMoneytodaySub8,
		xMoneytodaySub9,
		xMoneytodaySub10,	
		xMoneytodaySub11,

		xYounhapRoot,
		xYounhapSub1,
		xYounhapSub2,
		xYounhapSub3,
		xYounhapSub4,
		xYounhapSub5,
		xYounhapSub6,
		xYounhapSub7,
		xYounhapSub8,

		xMaekyoungRoot,
		xMaekyoungSub1,
		xMaekyoungSub2,
		xMaekyoungSub3,
		xMaekyoungSub4,
		xMaekyoungSub5,
		xMaekyoungSub6,
		xMaekyoungSub7,
		xMaekyoungSub8,
		xMaekyoungSub9,
		xMaekyoungSub10,
		xMaekyoungSub11,
		xMaekyoungSub12,
		xMaekyoungSub13,
//		xMaekyoungSub14,
		xMaekyoungSub15,
		xMaekyoungSub16,
//		xMaekyoungSub17,
		xMaekyoungSub18,

		xETodayRoot,
		xETodaySub1,
		xETodaySub2,
//		xETodaySub3,
		xETodaySub4,
		xETodaySub5,
		xETodaySub6,
		xETodaySub7,
//		xETodaySub8,
//		xETodaySub9,

		xNewspRoot,
		xNewspSub1,
		xNewspSub2,
		xNewspSub3,
		xNewspSub4,
		xNewspSub5,
		xNewspSub6,
		xNewspSub7,
		xNewspSub8,
//		xNewspSub9,


};

// select index
enum { 
	SEL_SS = 0,	// 0:시황
	SEL_GS,		// 1:공시
	SEL_MT,		// 2:머니투데이
	SEL_IS,		// 3:인포스탁
	SEL_HK,		// 4:한경
	SEL_ED,		// 5:이데일리
	
	SEL_YH,		// 6:연합뉴스
	SEL_MK,		// 7:매일경제
	SEL_ET,		// 8:이투데이
	SEL_NP,		// 9:뉴스핌
};

// detailSelect Index
enum { 
	DS_MT = 0,	// 0:매경
	DS_IS,		// 1:인포스탁
	DS_HK,		// 2:한경
	DS_ED,		// 3:이데일리
	DS_YH,		// 4:연합
	DS_MK,		// 5:매일경제
	DS_ET,		// 6:이투데이
	DS_NP		// 7:뉴스핌
};


#define SIZE_BUTTONS			18
#define COMBO_HEIGHT			20
#define OPTION_WND_HEIGHT		200

#define NEWS_CHECK_SECTION		"트리체크 설정"
#define NEWS_CHECK_FILE			"newsinfo.ini"
#define NEWS_CONFIG_FILE		"newstab"
#define NEWS_SCRAP_FILE			"news_scrap"
#define NEW_LINE				0x0a
#define	TABCHAR					0x09

enum	idPicker {idNotice = 0, 
				  idNews, 
				  idHanNews, 
				  idEDaily, 
				  idInfoStock, 
				  idMoneyToday, 
				  idYounhap, 
				  idMaekyoung, 
				  idEToday, 
				  idNewsp};

// 설정관련(색상, 본문조회)
#define COLOR_BLACK		PALETTERGB(0, 0, 0)
#define DEFAULT_COLOR	"0, 0, 0"
#define DEFAULT_CHECK	"FALSE"
#define SIGN_SEPER		","
#define GAP3			3
#define GAP5			1
#define GAP10			10
#define PICKER_WIDTH	60
#define PICKER_HEIGHT	20
#define STATIC_WIDTH1	70
#define STATIC_WIDTH2	100
#define STATIC_HEIGHT	21
#define NEWS_WIDTH		700
#define TREE_WIDTH		160
#define TOOLHEIGHT		45
#define GAPW			3
#define GAPMINW			2
#define ADDITIONALGAP	60

#define SECTION_OPTION	"색상/본문조회 설정"
#define KEY_NOTICE		"공시"
#define KEY_NEWS		"시황"
#define KEY_HANNEWS		"한경뉴스"
#define KEY_EDAILY		"이데일리"
#define KEY_INFOSTOCK	"인포스탁"
#define KEY_MONEYTODAY	"머니투데이"
#define KEY_CONTEXT		"본문조회"
#define KEY_YOUNHAP		"연합뉴스"
#define KEY_MAEKYOUNG	"매일경제"
#define KEY_ETODAY		"이투데이"
#define KEY_NEWSP		"뉴스팜"



#define SECTION_VIEW	"화면펼침 여부"
#define KEY_VIEW		"보이기"


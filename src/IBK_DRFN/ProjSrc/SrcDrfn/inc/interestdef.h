#ifndef __DOORI__INTERESTDEF__HEADER__
#define	__DOORI__INTERESTDEF__HEADER__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TYPE_USER_ITEMS		 "보유종목"
#define TYPE_HIGH_ITEMS		 "상한가"		
#define TYPE_HIGHING_ITEMS	 "상승"		
#define TYPE_EQUAL_ITEMS	 "보합"		
#define TYPE_LOW_ITEMS		 "하한가"		
#define TYPE_LOWING_ITEMS	 "하락"		
#define TYPE_NEWHIGH_ITEMS   "신고"      
#define TYPE_NEWLOW_ITEMS    "신저"     

#define TYPE_USER_ITEM		 "00"		//: 보유종목 
#define TYPE_HIGH_ITEM		 1		//: 상한가 종목
#define TYPE_HIGHING_ITEM	 2		//: 상승 종목
#define TYPE_EQUAL_ITEM		 3		//: 보합 종목
#define TYPE_LOW_ITEM		 4		//: 하한가 종목
#define TYPE_LOWING_ITEM	 5		//: 하락 종목
#define TYPE_NEWHIGH_ITEM    6       //: 신고
#define TYPE_NEWLOW_ITEM     7       //: 신저 


//2004.12.09 by LYH 특이종목 변경(20가지) 
#define TYPE_TODAY		1
#define TYPE_YESTERDAY	2

#define TYPE_KOSPI_HIGH_ITEMS		"상한가(거래소)"
#define TYPE_KOSDAQ_HIGH_ITEMS		"상한가(코스닥)"
#define TYPE_KOSPI_HIGHING_ITEMS	"상승(거래소)"
#define TYPE_KOSDAQ_HIGHING_ITEMS	"상승(코스닥)"
#define TYPE_ALL_EQUAL_ITEMS		"보합"		
#define TYPE_KOSPI_LOW_ITEMS		"하한가(거래소)"
#define TYPE_KOSDAQ_LOW_ITEMS		"하한가(코스닥)"
#define TYPE_KOSPI_LOWING_ITEMS		"하락(거래소)"
#define TYPE_KOSDAQ_LOWING_ITEMS	"하락(코스닥)"
#define TYPE_ALL_NEWHIGH_ITEMS			"신고"
#define TYPE_ALL_NEWLOW_ITEMS			"신저"

#define TYPE_PREV_KOSPI_HIGH_ITEMS		"전일상한가(거래소)"
#define TYPE_PREV_KOSDAQ_HIGH_ITEMS		"전일상한가(코스닥)"
#define TYPE_PREV_KOSPI_HIGHING_ITEMS	"전일상승(거래소)"
#define TYPE_PREV_KOSDAQ_HIGHING_ITEMS	"전일상승(코스닥)"
#define TYPE_PREV_EQUAL_ITEMS			"전일보합"		
#define TYPE_PREV_KOSPI_LOW_ITEMS		"전일하한가(거래소)"
#define TYPE_PREV_KOSDAQ_LOW_ITEMS		"전일하한가(코스닥)"
#define TYPE_PREV_KOSPI_LOWING_ITEMS	"전일하락(거래소)"
#define TYPE_PREV_KOSDAQ_LOWING_ITEMS	"전일하락(코스닥)"

enum ETC_TYPE
{
TYPE_KOSPI_HIGH_ITEM=1,
TYPE_KOSDAQ_HIGH_ITEM,
TYPE_KOSPI_HIGHING_ITEM,
TYPE_KOSDAQ_HIGHING_ITEM,
TYPE_ALL_EQUAL_ITEM,
TYPE_KOSPI_LOWING_ITEM,
TYPE_KOSDAQ_LOWING_ITEM,
TYPE_KOSPI_LOW_ITEM,
TYPE_KOSDAQ_LOW_ITEM,
TYPE_ALL_NEWHIGH_ITEM,
TYPE_ALL_NEWLOW_ITEM,
TYPE_PREV_KOSPI_HIGH_ITEM,
TYPE_PREV_KOSDAQ_HIGH_ITEM,
TYPE_PREV_KOSPI_HIGHING_ITEM,
TYPE_PREV_KOSDAQ_HIGHING_ITEM,
TYPE_PREV_EQUAL_ITEM,
TYPE_PREV_KOSPI_LOWING_ITEM,
TYPE_PREV_KOSDAQ_LOWING_ITEM,
TYPE_PREV_KOSPI_LOW_ITEM,
TYPE_PREV_KOSDAQ_LOW_ITEM
};

#define TYPE_ALL			 "0"		//코스닥, 코스피 모두 
#define TYPE_KOSPI			 "1"
#define TYPE_KOSDAQ			 "2"

#define DEF_INTTREE_UPJONG		0
#define DEF_INTTREE_USER		1
#define DEF_INTTREE_ETC         2

#define DEF_MASTER_ZZIM		0
#define DEF_MASTER_IF		1
#define DEF_MASTER_SIGNAL	2
#define DEF_MASTER_NEWS		3

#define DEF_INTTREES_UPJONG		"업종"
#define DEF_INTTREES_USER		"관심종목"
#define DEF_INTTREES_ETC		"특이종목"
#define DEF_INTTREES_FUTURE		"선물"
#define DEF_INTTREES_OPTION		"옵션"
#define DEF_INTTREES_CALL		"콜옵션"
#define DEF_INTTREES_PUT		"풋옵션"
#define DEF_INTTREES_ZZIM		"찜"
#define DEF_INTTREES_THEME		"테마"
#define DEF_INTTREES_IF			"임시그룹"
#define DEF_INTTREES_SIGNAL      "신호처리"
#define DEF_INTTREES_HAVE		"보유종목"
#define DEF_INTTRESS_NEWS		"뉴스종목"
#define DEF_INTTREES_HISTORY	"최근종목"

#define DEF_INTEREES_CURRENT    "시세동향"
#define DEF_INTEREES_INVEST		"투자자동향"
#define DEF_INTEREES_STSIGN		"지표신호"
#define DEF_INTEREES_STRATEGY   "추천전략"
#define DEF_INTEREES_USERMYNET	"사용자 저장조건"

#define	DEF_STR_KOSPI		"KOSPI"				// 거래소
#define	DEF_STR_KOSDAQ		"KOSDAQ"			// 코스닥
#define	DEF_STR_GROUP		"그룹사"			// 그룹
#define	DEF_STR_UALL		"종합"				// 종합
#define DEF_STR_IFLIST      "특징주"

#define D_TITLE			  0
#define D_UPJONG		100
#define D_KOSIP			101
#define D_KOSDOQ		102
#define D_GROUP			103 
#define D_FUTURE        104
#define D_OPTION	    105
#define D_ZZIM          106 
#define D_THEME			107
#define D_IF            108
#define D_SIGNAL        109
#define D_USERHAVE		110
#define D_UALL			111
#define D_STOCKIF		112
#define D_NEWS			113

#define D_USER			200
#define D_USERSEL		201
#define D_USERHAVESEL   202

#define D_ETC           300
#define D_ETCSEL		301 

#define D_OPTIONCALL    401
#define D_OPTIONPUT		402

#define D_IFMYNET		501
#define D_IFMYNETP		502
#define D_IFMYNETITEM	503

#define D_INVEST		211
#define D_STSIGN		212
#define D_STRATEGY		213
#define D_IFCRRENT		214

#define D_THEMEITEM     701

#define D_HISTORY       711 

#define MAX_IFLIST_CONDITION 4
#define MAX_ITEM_COUNT 50

#define USER_GROUP			0
#define UPJONG_KUGROUP		1
#define UPJONG_JGROUP		2 
#define TR_GROUP			3 
#define DEFINE_GROUP		4
#define ZZIM_GROUP          5
#define UPJONG_GROUP		6
#define THEME_GROUP			7
#define LOOK_GROUP			8
#define TEMP_GROUP			9

//#define MAX_GROUP_COUNT 60
#define MAX_GROUP_COUNT 50

#define FILEPATH_USERLIST "IntList.Dat"
#define FILEPATH_USERLIST01 "IntList01.Dat"
#define FILEPATH_USERIF   "IntIF.Dat"

#define MAX_CONNECT_DRDS 6

#define REAL_NS31 0 // 현물현재가
#define REAL_NX13 1 // 현물호가

#define REAL_NSC0 2 //선물 현재가 
#define REAL_NSH2 3 //선물 호가
//#define REAL_NSM0 4 //선물 미결제 약정 

#define REAL_NOC0 4 //선물 현재가 
#define REAL_NOH2 5 //선물 호가
//#define REAL_NOM0 7 //선물 미결제 약정 



#define REAL_S31 "S31" // 현물현재가
#define REAL_X13 "X13" // 현물호가

#define REAL_SC0 "SC0" //선물 현재가 
#define REAL_SH2 "SH2" //선물 호가

#define REAL_OC0 "OC0" //선물 현재가 
#define REAL_OH2 "OH2" //선물 호가

#define REG_MOVE_UP     1
#define REG_MOVE_DOWN   2
#define REG_MOVE_TOP    3
#define REG_MOVE_BOTTOM 4
#endif // __DOORI__INTERESTDEF__HEADER__
#ifndef __JEOYOHO__TRDEFINE__HEADER__
#define	__JEOYOHO__TRDEFINE__HEADER__

// 통신 구분
#define QUERY_STRJONGMOK		"d1010"
#define QUERY_STRUPJONG			"d1011"
#define QUERY_STRFUTOPT			"d1012"
#define QUERY_STRSTOCKFUTOPT	"d1013"
#define QUERY_STRFOREIGN		"d1014"
#define QUERY_STRMARKET			"d1015"

#define QUERY_STRDOLLAR			"d1018"
#define QUERY_STOCK_HOGA		"c1002"
#define QUERY_FUTOPT_HOGA		"f0201"
#define QUERY_STOCKFUT_HOGA		"s3301"
#define QUERY_STOCKOPT_HOGA		"f3201"
#define QUERY_ACCOUNTCODE		"a4017"
#define QUERY_ACCOUNTCONC		"a4018"
#define QUERY_STRREPLAY			"d1016"
#define QUERY_STRFOJIPYO		"d0515"
#define QUERY_STRPREFUTOPT		"d1012_1"

#define QUERY_STRGUIDELINE		"d1017"

#define QUERY_STRFOFOREIGN		"o44003$"		//"HCQ03005"
#define QUERY_STRFOFOREIGN_HOGA	"o44010$"		//"s0001"
#define QUERY_STRFOFOREIGN_HOGA2	"o56501$"		//"HCQ03012"
#define QUERY_STRFOFOREIGN_CHE2	"pfbopoop830"	// 해외선물 체결/미체결 조회에서 체결만 조회
#define QUERY_STRFOFOREIGN_JAN	"pfbopoop831"	// 해외선물 잔고조회

#define QUERY_STRFX				"pfbopoop"		// FX
#define QUERY_STRFX_CHE			"pfboaoop840"	// FX 체결/미체결 조회에서 미체결만 조회
#define QUERY_STRFX_CHE2		"pfboaoop8402"	// FX 체결/미체결 조회에서 체결만 조회
#define QUERY_STRFX_JAN			"pfboaoop841"	// FX 잔고조회

//KHD: Plus 조건검색 
#define QUERY_STRPLUS_STOCK		"d0354"// KHD : PlusStock
#define QUERY_STRPLUS_MARKET	"d0355"// KHD : PlusMarket
// -- INVESTOR --
#define QUERY_STRPLUS_KOSPI		"d0366"//KHD : 투자자 동향 
#define QUERY_STRPLUS_KOSDAK	"d0367"//KHD : 투자자 동향 
#define QUERY_STRPLUS_FO		"d0368"//KHD : 투자자 동향 
#define QUERY_STRPLUS_SIGNAL	"d0369"//KHD : 투자자 동향 

//END 

//////////////////////////////////////////////////////////////////////////
// IBK 조건검색
#define	TRNO_MSTCODE				"pijs0112"		//종목리스트 조회
#define TRNO_WORKDATE				"pijs0111"		//영업일조회
#define TRNO_CONDITION				"pijs0110"		//조건검색
#define TRNO_CONDIONECLK_RESULT		"pijs0370"		//빠른조건검색 조회
#define	TRNO_SET_REAL				"pijs0380"
#define TRNO_ALERT_REALREG			"pijs0371"		//사용자얼럿
#define	TRNO_ITEM_DIAGNOSIS			"pijs0410"
#define	TRNO_ORDER_SEARCH			"pijs0400"
#define	TRNO_ORDER_ITEM_DIAGNOSIS	"ItemDiagnosisFormOderSearch"
#define	TRNO_PATTERN_SEARCH			"pijs0430"
#define TRNO_DRAW_RESULT			"pijs0368"			//패턴 검색
#define	TRNO_CANDLE_RESULT			"pijs0366"
#define TRNO_GROUPTOBASE_UP			"pijs0361"
#define TRNO_GROUPTOBASE			"pijs0362"
#define	TRNO_VALUE_PREDICTION		"pijs0420"
#define	TRNO_LANGUAGE				"pijs0500"
#define	TRNO_ACCOUNTINFO			"PIBOSJGO"
#define TRNO_FO_ACCOUNTINFO			"PIBOFJGO"

#define TRNO_FAVORITE    			"PIDOMYST" //관심종목 그룹/ITEM 조회 tour2k

#define MINLEN_MSTCODE_TR			100000

#define TRNO_ALERT_REALREG_WINIX	"so035130"		//위닉스 리얼등록용.
//#define QUERY_STRJONGMOK_MIN	"p0612"		//분데이터 전용
#define TRNO_GROUP_RESULT		"so035120"
#define TRNO_SYSTEMAGREE		"pidosyst"

//////////////////////////////////////////////////////////////////////////
// 고정 통신 키 : 솔로몬용
#define TRKEY_CONDIONECLK_RESULT	1
#define TRKEY_MSTCODE				2
#define TRKEY_ALERT_REALREG			3
#define TRKEY_CONDITION				4
#define TRKEY_WORKDATE				5
#define TRKEY_DRAW_RESULT			6
#define TRKEY_GROUPTOBASE			7
#define TRKEY_GROUPTOBASE_UP		8
#define TRKEY_GROUP_RESULT			9
#define TRKEY_ALERT_REALREG_WINIX	10

#define TRKEY_GWANSIMGROUP			11
#define TRKEY_GWANSIMITEM			12
#define TRKEY_GWANSAVE				13

#define TRKEY_SYSTEMAGREE			14

#define TRKEY_ACCOUNTINFO			15
#define TRKEY_SET_REAL				16
#define TRKEY_ITEM_DIAGNOSIS		17
#define TRKEY_ORDER_SEARCH			18
#define TRKEY_ORDER_ITEM_DIAGNOSIS	19
#define TRKEY_PATTERN_SEARCH		20
#define TRKEY_CANDLE_RESULT			21
#define TRKEY_VALUE_PREDICTION		22
#define TRKEY_LANGUAGE				23
#define TRKEY_SYMBOL				24

#define TRKEY_TR_END				29

// 차트 조회용
#define TRKEY_STRJONGMOK			30
#define TRKEY_STRUPJONG				31
#define TRKEY_STRFUTOPT				32
#define TRKEY_STRSTOCKFUTOPT		33
#define TRKEY_STRFOREIGN			34
#define TRKEY_STRREPLAY				35		// 복기차트용

#define TRKEY_STOCK_HOGA			50
#define TRKEY_FUTOPT_HOGA			51
#define TRKEY_STOCKFUT_HOGA			52
#define TRKEY_STOCKOPT_HOGA			53

#define TRKEY_STRDOLLAR				54
#define TRKEY_STRFOJIPYO			55
#define TRKEY_STRPREFUTOPT			56

#define TRKEY_STRFOFOREIGN			57
#define TRKEY_STRFOFOREIGN_HOGA		58
#define TRKEY_STRFOFOREIGN_HOGA2	59
#define TRKEY_STRFOFOREIGN_JAN		60
#define TRKEY_STRFOFOREIGN_CHE2		61

#define TRKEY_STRFX					62
#define TRKEY_STRFX_CHE				63
#define TRKEY_STRFX_JAN				64
#define TRKEY_STRFX_CHE2			65

#define TRKEY_PLUS_STOCK			66 // KHD : 플러스조건검색 
#define TRKEY_PLUS_MARKET			67 // KHD : 플러스조건 대상검색 

#define TRKEY_FID_END				69
#define TRKEY_END					70

#define TRKEY_VIRT_START			71
#define TRKEY_VIRT_END				80

// --------------- For Investor FID -------------------
// 조건검색 (플러스조건 투자자검색) 추가적인 추가 정의.
#define TRKEY_PLUS_KOSPI			81 // KHD : 플러스조건 투자자검색 
#define TRKEY_PLUS_KOSDAK			82 // KHD : 플러스조건 투자자검색 
#define TRKEY_PLUS_FO				83 // KHD : 플러스조건 투자자검색 
#define TRKEY_PLUS_SIGNAL			84 // KHD : 플러스조건 시장신호검색 

// ------------  For Real  -----------
#define TRKEY_REALSTART				101

// 시장지표용
/*
#define TRKEY_STRMARKET_0			36
#define TRKEY_STRMARKET_1			37
#define TRKEY_STRMARKET_2			38
#define TRKEY_STRMARKET_3			39
#define TRKEY_STRMARKET_4			40
#define TRKEY_STRMARKET_5			41
#define TRKEY_STRMARKET_6			42
#define TRKEY_STRMARKET_7			43
#define TRKEY_STRMARKET_8			44
#define TRKEY_STRMARKET_9			45
*/
#define TRKEY_STRMARKET_0			150
#define TRKEY_STRMARKET_1			151
#define TRKEY_STRMARKET_2			152
#define TRKEY_STRMARKET_3			153
#define TRKEY_STRMARKET_4			154
#define TRKEY_STRMARKET_5			155
#define TRKEY_STRMARKET_6			156
#define TRKEY_STRMARKET_7			157
#define TRKEY_STRMARKET_8			158
#define TRKEY_STRMARKET_9			159

#define TRKEY_STRMARKET_10			160
#define TRKEY_STRMARKET_11			161
#define TRKEY_STRMARKET_12			162
#define TRKEY_STRMARKET_13			163
#define TRKEY_STRMARKET_14			164
#define TRKEY_STRMARKET_15			165
#define TRKEY_STRMARKET_16			166
#define TRKEY_STRMARKET_17			167
#define TRKEY_STRMARKET_18			168
#define TRKEY_STRMARKET_19			169
#define TRKEY_STRMARKET_20			170
#define TRKEY_STRMARKET_21			171
#define TRKEY_STRMARKET_22			172
#define TRKEY_STRMARKET_23			173
#define TRKEY_STRMARKET_24			174
#define TRKEY_STRMARKET_25			175
#define TRKEY_STRMARKET_26			176
#define TRKEY_STRMARKET_27			177

#define TRKEY_STRMARKET_28			178
#define TRKEY_STRMARKET_29			179
#define TRKEY_STRMARKET_30			180
#define TRKEY_STRMARKET_31			181
#define TRKEY_STRMARKET_32			182		// 외국인+기관 순매수
#define TRKEY_STRMARKET_33			183		// 외국인+기관 순매수누적

#define TRKEY_STRUPMARKET_0			190
#define TRKEY_STRUPMARKET_1			191

#define TRKEY_STRMARKET_END			200

//////////////////////////////////////////////////////////////////////////
// Lib_SignalMng에서 리얼 받는 윈도우로 보내는 메시지
const UINT RMSG_GETSIGNAL_REAL = ::RegisterWindowMessage(_T("RMSG_GETSIGNAL_REAL"));

#endif // __JEOYOHO__TRDEFINE__HEADER__

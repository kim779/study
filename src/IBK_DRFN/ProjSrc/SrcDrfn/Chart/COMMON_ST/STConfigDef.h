#ifndef __STCONFIG_DEF__BYGM0604
#define __STCONFIG_DEF__BYGM0604

// tab label
#define BASIC		"Basic"
#define INDEX		"Index"
#define SIGNAL		"Signal"
#define VARIABLES	"Variables"
#define EXTRA		"Extra"

// basic
#define	EXPLANATION	"Explanation"
#define LANGTYPE		"LangType"

// index
#define INDEXNAME		"Name"
#define INDEXTYPE		"Type"
#define INDEXCOLOR		"Color"
#define INDEXSHAPE		"Shape"
#define INDEXTHICK		"Thick"

#define COLORBULL		"ClrBull"
#define COLORBEAR		"ClrBear"

#define	STRLIST_SIGNAL	{"↑", "↓", "▲", "▼", "△", "▽"}
#define	STRLIST_CLEAR	{"★", "☆", "♣", "♧", "◈", "▣", "■", "□"}

// signal
#define USEBUY			"UseBuy"
#define USESELL			"UseSell"
#define USEEXITLONG		"UseExitlong"
#define USEEXITSHORT	"UseExitshort"
#define USEBULL			"UseBull"
#define USEBEAR			"UseBear"
#define SHAPEBUY		"ShapeBuy"
#define COLORBUY		"ClrBuy"
#define SHAPEEXITLONG	"ShapeExitlong"
#define COLOREXITLONG	"ClrExitlong"
#define SHAPESELL		"ShapeSell"
#define COLORSELL		"ClrSell"
#define SHAPEEXITSHORT	"ShapeExitshort"
#define COLOREXITSHORT	"ClrExitshort"
#define SHOWSIGNALNAME	"ShowSignalName"
#define SHOWSIGNALQTY	"ShowSignalQty"
#define POSOFSIGNAL		"PosOfSignal"

// variables
#define INPUTCNT		"InputCnt"
#define INPUTVALUE		"Input"
#define INPUTNAME		"InputName"
#define USEEXT			"UseExt"
#define USEEXTDATA		"UseExtData"

// TRADINGOPTION
//#define FIRSTSIGNALEQUITY	"FirstSignalEquity"		// "0":Buy, "1":ExitLong
//#define FIRSTSIGNALFUTURE	"FirstSignalFuture"		// "0":Buy, "1":Sell, "2":All
//#define ORDERPRICE		"OrderPrice"				// "0":CurrentPrice, "1":±1Bid, "2":±2Bid, "3":±3Bid, "4":±4Bid, "5":±5Bid, "6":±10Bid, "7":MarketPrice, "8":Upper/Lower Price
//#define EXITPRICE		"ExitPrice"					// "0":CurrentPrice, "1":±1Bid, "2":±2Bid, "3":±3Bid, "4":±4Bid, "5":±5Bid, "6":±10Bid, "7":MarketPrice, "8":Upper/Lower Price
//#define ORDERTIME		"OrderTime"					// "0":At Completing Candle ,"1":At Eventing Signal
//#define ENABLECONTINUTY	"EnableContinuty"			// "0":Disable, "1":Enable
//#define MAXCONTINUTY	"MaxContinuty"				// Max count to allow same deal position continuely
//#define MINIMUMCHANGE	"MiniumChange"				// "0":Disable, "1":Enable
//#define MINIMUMEQUITY	"MiniumEquity"				// Minium Change Value to operate At Equity
//#define MINIMUMFUTURE	"MiniumFuture"				// Minium Change Value to operate At Future/Option


#define USEPRIFITTARGETRATE		"UseProfitTargetRate"
#define VALPRIFITTARGETRATE		"ValProfitTargetRate"
#define USEPROFITREVERSERATE	"UseProfitReverseRate"
#define VALPROFITREVERSERATE	"ValProfitReverseRate"
#define USEDAMAGELIMITATIONRATE	"UseDamageLimitationRate"
#define VALDAMAGELIMITATIONRATE	"ValDamageLimitationRate"
#define EXITATUPPERPRICE		"ExitAtUpperPrice"
#define EXITATLOWERPRICE		"ExitAtLowerPrice"
#define EXITATTODAY				"ExitAtToday"
#define VALEXITATTODAY			"ValExitAtToday"
#define FORCEDEXITPRICE			"ForcedExitPrice"
#define FEEFORENTRY				"FeeForEntry"
#define FEEFOREXIT				"FeeForExit"
#define SLIPPAGEFORENTRY		"SlippageForEntry"
#define SLIPPAGEFOREXIT			"SlippageForExit"
#define FEEUNIT					"FeeUnit"
#define SLIPPAGEUNIT			"SlippageUnit"

// strategy
#define TRADINGOPTION	"TradingOption"
#define STRATEGYLIST	"StrategyList"
#define STRATEGYCNT		"Count"
#define STRATEGYPOS		"Pos"
#define STRATEGYPOSNAME	"PosName"
#define STRATEGYPOSPATH	"PosPath"
#define STRATEGYPOSTYPE	"PosType"
#define STRATEGYPOSTIME	"PosTime"
#define STRATEGYPOSTEXT	"PosText"
#define STRATEGYLOGIC	"LogicData"
#define STRATEGYSIGNAL	"StrategySignal"


//---------------------------------------------------------------------------------------
// 전략설정 매크로

#define STCONFIG_SIGNAL		"Signal"		// 신호설정
#define STCONFIG_ORDER		"RunJumun"		// 주문설정
#define STCONFIG_CLEAR		"RunSetup"		// 강제청산설정
#define STCONFIG_SETUP		"RunClear"		// 운용설정

// 신호 관련
#define SHAPEBUY			"ShapeBuy"			// 매수
#define COLORBUY			"ClrBuy"
#define SHAPESELL			"ShapeSell"			// 매도
#define COLORSELL			"ClrSell"
#define SHAPEEXITLONG		"ShapeExitlong"		// 매수청산
#define COLOREXITLONG		"ClrExitlong"
#define SHAPEEXITSHORT		"ShapeExitshort"	// 매도청산
#define COLOREXITSHORT		"ClrExitshort"

// 주문 관련
#define APPLYACCT			"Acct"				// 적용계좌
#define ACCTNAME			"AcctName"			// 계좌명
#define ORDERAMOUNT			"JAmount"			// 주문수량
#define BUYPRICE			"BuyPrice"			// 진입가격
#define SELLPRICE			"SellPrice"			// 청산가격
#define ORDERMETHOD			"JMethod"			// 주문방식
#define ORDERTIME			"JTime"				// 주문시점
#define ORDERSTARTSIGNAL	"JStartSignal"		// 주문시작번호
#define SOUND_BUY_USE		"Sound_Buy_Use"		// 신호발생음(진입신호) 사용여부
#define SOUND_BUY_FILE		"Sound_Buy_File"	// 신호발생음(진입신호) 파일경로
#define SOUND_SELL_USE		"Sound_Sell_Use"	// 신호발생음(청산신호) 사용여부
#define SOUND_SELL_FILE		"Sound_Sell_File"	// 신호발생음(청산신호) 파일경로

// 강제청산 관련
#define CLEARPOINT			"ClearPoint"		// 강제청산시점
#define USELOSSRATE			"Chk1"				// 손절매 사용여부
#define LOSSRATE			"ED1"				// 손절매(손실률)
#define LOSSRATEUNIT		"P1"				// 손절매 단위
#define LOSSRATESHAPE		"S1"				// 손절매 신호모양
#define LOSSRATECOLOR		"CLR_1"				// 손절매 신호색상

#define USETARGETBENEFIT	"Chk2"				// 목표수익 사용여부
#define TARGETBENEFIT		"ED2"				// 목표수익
#define TARGETBENEFITUNIT	"P2"				// 목표수익 단위
#define TARGETBENEFITSHAPE	"S2"				// 목표수익 신호모양
#define TARGETBENEFITCOLOR	"CLR_2"				// 목표수익 신호색상

#define USEHIGHPRICE		"Chk3"				// 당일 고가대비 하락 사용여부
#define HIGHPRICE			"ED3"				// 당일 고가대비 하락
#define HIGHPRICEUNIT		"P3"				// 당일 고가대비 하락 단위
#define HIGHPRICESHAPE		"S3"				// 당일 고가대비 하락 신호모양
#define HIGHPRICECOLOR		"CLR_3"				// 당일 고가대비 하락 신호색상

//#define USELOWPRICE		"Chk4"				// 당일 저가대비 상승 사용여부
//#define LOWPRICE			"ED4"				// 당일 저가대비 상승
//#define LOWPRICEUNIT		"P4"				// 당일 저가대비 상승 단위
//#define LOWPRICESHAPE		"S4"				// 당일 저가대비 상승 신호모양
//#define LOWPRICECOLOR		"CLR_4"				// 당일 저가대비 상승 신호색상

#define USETARGETVALUEPRESERV	"Chk4"			// 목표가보존 사용여부
#define TARGETVALUEPRESERV		"ED4"			// 목표가보존
#define TARGETVALUEPRESERVUNIT	"P4"			// 목표가보존 단위
#define TARGETVALUEPRESERVSHAPE	"S4"			// 목표가보존 신호모양
#define TARGETVALUEPRESERVCOLOR	"CLR_4"			// 목표가보존 신호색상

#define USEBENEFITPRESERV		"Chk7"			// 이익보존 사용여부
#define BENEFITPRESERVUP		"ED6"			// 이익보존 상승
#define BENEFITPRESERVDN		"ED7"			// 이익보존 하락
#define BENEFITPRESERVUPUNIT	"P6"			// 이익보존 단위(상승)
#define BENEFITPRESERVDNUNIT	"P7"			// 이익보존 단위(하락)
#define BENEFITPRESERVSHAPE		"S5"			// 이익보존 신호모양
#define BENEFITPRESERVCOLOR		"CLR_5"			// 이익보존 신호색상

#define USEMINIPRICE		"Chk5"				// 최소가격변화 사용여부
#define MINIPRICE			"ED5"				// 최소가격변화
#define MINIPRICEUNIT		"P5"				// 최소가격변화 단위
#define MINIPRICEBONGCOUNT	"Bong"				// 최소가격변화 봉 갯수

#define USETODAYCLEAR		"Chk6"				// 당일청산 사용여부
#define TODAYCLEARUINT		"TimeA"				// 오전/오후
#define TODAYCLEARTIME		"ED_Time"			// 청산시간


// 운용 관련
#define	ALLOWPOSITION		"Allow"				// 허용포지션
#define USEREPEATBUY		"Repeat"			// 중복진입 허용여부
#define	REPEATBUYCOUNT		"JCount"			// 중복진입 횟수
#define	TAXBUY				"TaxBuy"			// 수수료+세금(진입)
#define	TAXSELL				"TaxSell"			// 수수료+세금(청산)
#define	TAXUNIT				"cbTax"				// 수수료+세금(단위)
#define	SLIPIGYBUY			"SlipigyBuy"		// 수수료+세금(진입)
#define	SLIPIGYSELL			"SlipigyBuy"		// 수수료+세금(청산)
#define	SLIPIGYUNIT			"cbSlipigy"			// 수수료+세금(단위)


// Server AP Info
#define EXECUTE_SYMBOL			"SYMBOL"
#define EXECUTE_ENDDATE			"ENDDATE"
#define EXECUTE_ALLCOUNT		"ALLCOUNT"
#define EXECUTE_TYPE			"TYPE"
#define EXECUTE_TMNUMBER		"TMNUMBER"

#define EXECUTE_DATEPOINTER			"DATE"
#define EXECUTE_OPENPOINTER			"OPEN"
#define EXECUTE_HIGHPOINTER			"HIGH"
#define EXECUTE_LOWPOINTER			"LOW"
#define EXECUTE_CLOSEPOINTER		"CLOSE"
#define EXECUTE_VOLUMEPOINTER		"VOLUME"
#define EXECUTE_TIMETYPE			"TIMETYPE"
#define EXECUTE_PREVDATA			"PREVPRICE2"
// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
#define	EXECUTE_EXTDATA				"EXTDATA"


// SD Mode Info
#define LOAD_MODE		"Mode"
#define MODE_TEST		"Test"
#define MODE_ACTION		"Action"


// define For Variable Optimizer
// BOOL CStrategyWnd::LoadSD(LPCTSTR lpStrategyDllPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate, LPCTSTR lpTimeTypeNum,LPCTSTR lpTypeDealOption)
#define OPT_SELL						"OptSell"
#define OPT_BUY							"OptBuy"
#define OPT_INDEX_CROSSABOVE_BASIS		"OptIndexCrossAboveBasis"
#define OPT_INDEX_CROSSBELOW_BASIS		"OptIndexCrossBelowBasis"
#define OPT_INDEX_CROSSABOVE_ANOTHER	"OptIndexCrossAboveAnother"
#define OPT_INDEX_CROSSBELOW_ANOTHER	"OptIndexCrossBelowAnother"
#define OPT_PRICE_CROSSABOVE_INDEX		"OptPriceCrossAboveIndex"
#define OPT_PRICE_CROSSBELOW_INDEX		"OptPriceCrossBelowIndex"
#define OPT_INDEX_UPWARDREVERSAL		"OptIndexUpwardReversal"
#define OPT_INDEX_DOWNWARDREVERSAL		"OptIndexDownwardReversal"
#define OPT_SELL_OPTION					"OptSellOption"
#define OPT_BUY_OPTION					"OptBuyOption"

#endif //__STCONFIG_DEF__BYGM0604
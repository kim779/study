#ifndef __PATTERN_H__DEF
#define __PATTERN_H__DEF



// Total Pattern Count
#define TOT_PATTERN_CNT				20


// Init varialbe value with Zero
#define ZERO						0

#define CANDLE_HIGHPT				0x0001		// High Point at Candle
#define CANDLE_LOWPT				0x0002		// Low Point at Candle


// Changing Point Type
#define CHANGINGPT_HIGHTOLOW		1001		// 주요고점
#define CHANGINGPT_LOWTOHIGH		1002		// 주요저점

// Regist or Support Line Type
#define LINE_HIGHREGIST				2001		// 저항선
#define LINE_LOWSUPPORT				2002		// 지지선


// Pattern Trend
#define UPWARD_REVERSE				3001		// Pattern Upward Reverse
#define DNWARD_REVERSE				3002		// Pattern Downward Reverse
#define UPWARD_CONTINUE				3003		// Pattern Upward Continue
#define	DNWARD_CONTINUE				3004		// Pattern Downward Continue

// Pattern Trust
#define UPWARD_LEVEL3				1			// Pattern Upward Level3
#define UPWARD_LEVEL2				2			// Pattern Upward Level2
#define UPWARD_LEVEL1				3			// Pattern Upward Level1
#define	DNWARD_LEVEL3				4			// Pattern Downward Level3
#define	DNWARD_LEVEL2				5			// Pattern Downward Level2
#define	DNWARD_LEVEL1				6			// Pattern Downward Level1

// 2008.02.20 by LYH >>
// Pattern Trend Name
//#define STR_UPWARD_REVERSE			"상승반전형"				// Pattern Upward Reverse
//#define STR_DNWARD_REVERSE			"하락반전형"				// Pattern Downward Reverse
//#define STR_UPWARD_CONTINUE			"상승지속형"				// Pattern Upward Continue
//#define	STR_DNWARD_CONTINUE			"하락지속형"				// Pattern Downward Continue
// 2008.02.20 by LYH <<



// Pattern Type
#define PTN_HEADNSHOULDER			4001		// Head & Shoulder(머리어깨형)
#define PTN_R_HEADNSHOULDER			4002		// Reversal Head & Shoulder(역머리어깨형)

#define PTN_DOUBLETOP				4003		// Double Top(쌍봉형)
#define PTN_R_DOUBLETOP				4004		// Reversal Double Top(역쌍봉형)

#define PTN_BULL_ISOTRIANGLE		4005		// Bull Isosceles Triangles(강세이등변삼각형)
#define PTN_BEAR_ISOTRIANGLE		4006		// Bear Isosceles Triangles(약세이등변삼각형)

#define PTN_BULL_TRIANGLE			4007		// Bull Triangles(상승삼각형)
#define PTN_BEAR_TRIANGLE			4008		// Bear Triangles(하락삼각형)

#define PTN_BULLISH_WIDETRIANGLE	4009		// Bullish Wide Triangles(상승확장삼각형)
#define PTN_BEARISH_WIDETRIANGLE	4010		// Bearish Wide Triangles(하락확장삼각형)

#define PTN_BULL_WEDGE				4011		// Bull Wedge(상향쐐기형)
#define PTN_BEAR_WEDGE				4012		// Bear Wedge(하향쐐기형)

#define PTN_BULL_SQUAREFLAG			4013		// Bull Square Flag(상승사각깃발형)
#define PTN_BEAR_SQUAREFLAG			4014		// Bear Square Flag(하락사각깃발형)

#define PTN_BULL_TRIANGLEFLAG		4015		// Bull Triangle Flag(상승삼각깃발형)
#define PTN_BEAR_TRIANGLEFLAG		4016		// Bear Triangle Flag(하락삼각깃발형)

#define PTN_BULL_RECTANGLE			4017		// Bull Rectangle(상승직사각형)
#define PTN_BEAR_RECTANGLE			4018		// Bear Rectangle(하락직사각형)

#define PTN_V_TOP					4019		// V Top(단봉천정형)
#define PTN_V_BOTTOM				4020		// V Bottom(단봉바닥형)

// 2008.02.20 by LYH >>
// Pattern Name
//#define STR_HEADNSHOULDER			"머리어깨형"				//"Head & Shoulder"
//#define STR_R_HEADNSHOULDER			"역머리어깨형"				//"Reversal Head & Shoulder"
//
//#define STR_DOUBLETOP				"쌍봉형"					//"Double Top"
//#define STR_R_DOUBLETOP				"역쌍봉형"					//"Reversal Double Top"
//
//#define STR_BULL_ISOTRIANGLE		"강세이등변삼각형"			//"Bull Isosceles Triangles"
//#define STR_BEAR_ISOTRIANGLE		"약세이등변삼각형"			//"Bear Isosceles Triangles"
//
//#define STR_BULL_TRIANGLE			"상승삼각형"				//"Bull Triangles"
//#define STR_BEAR_TRIANGLE			"하락삼각형"				//"Bear Triangles"
//
//#define STR_BULLISH_WIDETRIANGLE	"상승확장삼각형"			//"Bullish Wide Triangles"
//#define STR_BEARISH_WIDETRIANGLE	"하락확장삼각형"			//"Bearish Wide Triangles"
//
//#define STR_BULL_WEDGE				"상향쐐기형"				//"Bull Wedge"	
//#define STR_BEAR_WEDGE				"하향쐐기형"				//"Bear Wedge"
//
//#define STR_BULL_SQUAREFLAG			"상승사각깃발형"			//"Bull Square Flag"
//#define STR_BEAR_SQUAREFLAG			"하락사각깃발형"			//"Bear Square Flag"
//
//#define STR_BULL_TRIANGLEFLAG		"상승삼각깃발형"			//"Bull Triangle Flag"
//#define STR_BEAR_TRIANGLEFLAG		"하락삼각깃발형"			//"Bear Triangle Flag"
//
//#define STR_BULL_RECTANGLE			"상승직사각형"				//"Bull Rectangle"
//#define STR_BEAR_RECTANGLE			"하락직사각형"				//"Bear Rectangle"
//
//#define STR_V_TOP					"단봉천정형"				//"V Top"
//#define STR_V_BOTTOM				"단봉바닥형"				//"V Bottom"
// 2008.02.20 by LYH <<



// Return Value of a Function which is find Candle Pattern
#define RTN_FEWDATA						-1
#define RTN_NONE						0
#define RTN_FIND						1


// Candle Pattern Type
#define CPTN_HAMMER							8001			// Hammer							(망치형)
#define CPTN_HANGINGMAN						8002			// Hanging Man						(교수형)
#define CPTN_INVERTEDHAMMER					8003			// Inverted Hammer					(역망치형)
#define CPTN_SHOOTINGSTAR					8004			// Shooting Star					(유성형)
#define CPTN_BULLISH_BELTHOLDLINE			8005			// Bullish Belt Hold Line			(상승샅바형)
#define CPTN_BEARISH_BELTHOLDLINE			8006			// Bearish Belt Hold Line			(하락샅바형)
#define CPTN_LONG_WHITEBODY					8007			// Long White Body					(장대양봉)
#define CPTN_LONG_BLACKBODY					8008			// Long Black Body					(장대음봉)
#define CPTN_BULLISH_HARAMI					8009			// Bullish Harami					(상승잉태형)
#define CPTN_BEARISH_HARAMI					8010			// Bearish Harami					(하락잉태형)
#define CPTN_BULLISH_ENGULFING				8011			// Bullish Engulfing				(상승장악형)
#define CPTN_BEARISH_ENGULFING				8012			// Bearish Engulfing				(하락장악형)
#define CPTN_BULLISH_COUNTERATTACKLINE		8013			// Bullish Counterattack Line		(상승반격형)
#define CPTN_BEARISH_COUNTERATTACKLINE		8014			// Bearish Counterattack Line		(하락반격형)
#define CPTN_PIERCINGPATTERN				8015			// Piercing Pattern					(관통형)
#define CPTN_DARKCLOUDCOVER					8016			// Dark Cloud Cover					(먹구름형)
#define CPTN_MORNINGSTAR					8017			// Morning Star						(샛별형)
#define CPTN_EVENINGSTAR					8018			// Evening Star						(석별형)
#define CPTN_MORNINTDOJISTAR				8019			// Morning Doji Star				(십자샛별형)
#define CPTN_EVENINGDOJISTAR				8020			// Evening Doji Star				(십자석별형)
#define CPTN_ABANDONEDBABY_INDOWNTREND		8021			// Abandoned Baby In Downtrend		(상승기아형)
#define CPTN_ABANDONEDBABY_INUPTREND		8022			// Abandoned Baby In Uptrend		(하락기아형)
#define CPTN_THREE_INSIDEUP					8023			// Three Indise Up					(상승잉태확인형)
#define CPTN_THREE_INSIDEDOWN				8024			// Three Indise Down				(하락잉태확인형)
#define CPTN_THREE_OUTSIDEUP				8025			// Three Outside Up					(상승장악확인형)
#define CPTN_THREE_OUTSIDEDOWN				8026			// Three Outside Down				(하락장악확인형)
#define CPTN_UPSIDEGAPTWOCROWS_INUPTREND	8027			// Upside Gap Two Crows In Uptrend	(까마귀형)
#define CPTN_THREE_WHITESOLDIER				8028			// Three White Soldier				(적삼병)
#define CPTN_THREE_BLACKCROW				8029			// Three Black Corws				(흑삼병)
#define CPTN_STALLEDPATTERN					8030			// Stalled Pattern					(지연형)
#define CPTN_ADVANCEBLOCK					8031			// Advance Block					(블락형)
#define CPTN_UPSIDEGAP_TASUKI				8032			// Upside Gap Tasuki				(상승타스키갭형)
#define CPTN_DNSIDEGAP_TASUKI				8033			// Downside Gap Tasuki				(하락타스키갭형)
#define CPTN_MATHOLD						8034			// Mat-Hold							(매트형)
#define CPTN_BULLISH_TRISTAR				8035			// Bullish Tri Star					(상승세십자별형)
#define CPTN_BEARISH_TRISTAR				8036			// Bearish Tri Star					(하락세십자별형)
#define CPTN_UNIQUETHREERIVERBOTTOM			8037			// Unique Three River Bottom		(희귀삼천바닥형)
#define CPTN_IDENTICAL_THREECROWS			8038			// Identical Three Crows			(동일흑삼병형)
#define CPTN_BULLISH_BREAKAWAY				8039			// Bullish Breakaway				(상승탈출형)
#define CPTN_BEARISH_BREAKAWAY				8040			// Bearish Breakaway				(하락탈출형)
#define CPTN_TWOCORWS						8041			// Two Crows						(약세까마귀형)
#define CPTN_THREESTARS_INTHESOUTH			8042			// Three Stars In The South			(남삼성형)
#define CPTN_CONCEALINGBABY_SWALLOW			8043			// Concealing Baby Swallow			(애기제비감추기형)
#define CPTN_STICKSANDWICH					8044			// Stick Sandwich					(막대샌드위치형)
#define CPTN_BULLISH_KICKING				8045			// Bullish Kicking					(상승박차형)	
#define CPTN_BEARISH_KICKING				8046			// Bearish Kicking					(하락박차형)
#define CPTN_HOMINGPIGEON					8047			// Homing Pigeon					(전서구형)
#define CPTN_LADDERBOTTOM					8048			// Ladder Bottom					(사다리바닥형)
#define CPTN_MATCHINGLOW					8049			// Matching Low						(동일저점형)
#define CPTN_BULLISH_SIDEBYSIDE_WHITELINE	8050			// Bullish Side-By-Side White Lines	(상승나란히형)
#define CPTN_BEARISH_SIDEBYSIDE_WHITELINE	8051			// Bearish Side-By-Side White Lines	(하락나란히형)
#define CPTN_RISING_THREEMETHODS			8052			// Rising Three Methods				(상승삼법형)
#define CPTN_FALLING_THREEMETHODS			8053			// Falling Three Methods			(하락삼법형)
#define CPTN_BULLISH_SEPARATINGLINES		8054			// Bullish Separating Lines			(상승갈림길형)
#define CPTN_BEARISH_SEPARATINGLINES		8055			// Bearish Separating Lines			(하락갈림길형)
#define CPTN_BULLISH_THREELINESTRIKE		8056			// Bullish Three-Line Strike		(강세삼선반격형)
#define CPTN_BEARISH_THREELINESTRIKE		8057			// Bearish Three-Line Strike		(약세삼선반격형)
#define CPTN_UPSIDEGAP_THREEMETHODS			8058			// Upside Gap Three Methods			(상승갭삼법형)
#define CPTN_DNSIDEGAP_THREEMETHODS			8059			// Downside Gap Three Methods		(하락갭삼법형)
#define CPTN_ON_NECK						8060			// On Neck							(걸침형)
#define CPTN_IN_NECK						8061			// In Neck							(진입형)
#define CPTN_THRUSTINGLINE					8062			// Thrusting Line					(공격형)
//#define CPTN_BULLISH_GRAVESTONE				8063			// Bullish GraveStone				(강세 비석형)
//#define CPTN_BULLISH_DRAGONFLY				8064			// Bullish DragonFly				(강세 잠자리형)

// 2008.02.20 by LYH >>
// Candle Pattern Name
//#define STR_HAMMER							"망치형"
//#define STR_HANGINGMAN						"교수형"
//#define STR_INVERTEDHAMMER					"역망치형"
//#define STR_SHOOTINGSTAR					"유성형"
//#define STR_BULLISH_BELTHOLDLINE			"상승샅바형"
//#define STR_BEARISH_BELTHOLDLINE			"하락샅바형"
//#define STR_LONG_WHITEBODY					"장대양봉"
//#define STR_LONG_BLACKBODY					"장대음봉"
//#define STR_BULLISH_HARAMI					"상승잉태형"
//#define STR_BEARISH_HARAMI					"하락잉태형"
//#define STR_BULLISH_ENGULFING				"상승장악형"
//#define STR_BEARISH_ENGULFING				"하락장악형"
//#define STR_BULLISH_COUNTERATTACKLINE		"상승반격형"
//#define STR_BEARISH_COUNTERATTACKLINE		"하락반격형"
//#define STR_PIERCINGPATTERN					"관통형"
//#define STR_DARKCLOUDCOVER					"먹구름형"
//#define STR_MORNINGSTAR						"샛별형"
//#define STR_EVENINGSTAR						"석별형"
//#define STR_MORNINTDOJISTAR					"십자샛별형"
//#define STR_EVENINGDOJISTAR					"십자석별형"
//#define STR_ABANDONEDBABY_INDOWNTREND		"상승기아형"
//#define STR_ABANDONEDBABY_INUPTREND			"하락기아형"
//#define STR_THREE_INSIDEUP					"상승잉태확인형"
//#define STR_THREE_INSIDEDOWN				"하락잉태확인형"
//#define STR_THREE_OUTSIDEUP					"상승장악확인형"
//#define STR_THREE_OUTSIDEDOWN				"하락장악확인형"
//#define STR_UPSIDEGAPTWOCROWS_INUPTREND		"까마귀형"
//#define STR_THREE_WHITESOLDIER				"적삼병"
//#define STR_THREE_BLACKCROW					"흑삼병"
//#define STR_STALLEDPATTERN					"지연형"
//#define STR_ADVANCEBLOCK					"블락형"
//#define STR_UPSIDEGAP_TASUKI				"상승타스키갭형"
//#define STR_DNSIDEGAP_TASUKI				"하락타스키갭형"
//#define STR_MATHOLD							"매트형"
//#define STR_BULLISH_TRISTAR					"상승세십자별형"
//#define STR_BEARISH_TRISTAR					"하락세십자별형"
//#define STR_UNIQUETHREERIVERBOTTOM			"희귀삼천바닥형"
//#define STR_IDENTICAL_THREECROWS			"동일흑삼병형"
//#define STR_BULLISH_BREAKAWAY				"상승탈출형"
//#define STR_BEARISH_BREAKAWAY				"하락탈출형"
//#define STR_TWOCORWS						"약세까마귀형"
//#define STR_THREESTARS_INTHESOUTH			"남삼성형"
//#define STR_CONCEALINGBABY_SWALLOW			"애기제비감추기형"
//#define STR_STICKSANDWICH					"막대샌드위치형"
//#define STR_BULLISH_KICKING					"상승박차형"	
//#define STR_BEARISH_KICKING					"하락박차형"
//#define STR_HOMINGPIGEON					"전서구형"
//#define STR_LADDERBOTTOM					"사다리바닥형"
//#define STR_MATCHINGLOW						"동일저점형"
//#define STR_BULLISH_SIDEBYSIDE_WHITELINE	"상승나란히형"
//#define STR_BEARISH_SIDEBYSIDE_WHITELINE	"하락나란히형"
//#define STR_RISING_THREEMETHODS				"상승삼법형"
//#define STR_FALLING_THREEMETHODS			"하락삼법형"
//#define STR_BULLISH_SEPARATINGLINES			"상승갈림길형"
//#define STR_BEARISH_SEPARATINGLINES			"하락갈림길형"
//#define STR_BULLISH_THREELINESTRIKE			"강세삼선반격형"
//#define STR_BEARISH_THREELINESTRIKE			"약세삼선반격형"
//#define STR_UPSIDEGAP_THREEMETHODS			"상승갭삼법형"
//#define STR_DNSIDEGAP_THREEMETHODS			"하락갭삼법형"
//#define STR_ON_NECK							"걸침형"
//#define STR_IN_NECK							"진입형"
//// #define STR_THRUSTINGLINE					"공격형"
// 2008.02.20 by LYH >>
#define STR_THRUSTINGLINE					"Thrusting Line"
//#define STR_BULLISH_GRAVESTONE				"강세비석형"
//#define STR_BULLISH_DRAGONFLY				"강세잠자리형"


// Candle Pattern Name
#define STR_HTML_HAMMER							"Hammer"
#define STR_HTML_HANGINGMAN						"HangingMan"
#define STR_HTML_INVERTEDHAMMER					"InvertedHammer"
#define STR_HTML_SHOOTINGSTAR					"Shootingstar"
#define STR_HTML_BULLISH_BELTHOLDLINE			"Bullishbeltholdline"
#define STR_HTML_BEARISH_BELTHOLDLINE			"Bearishbeltholdline"
#define STR_HTML_LONG_WHITEBODY					"LongWhiteBody"
#define STR_HTML_LONG_BLACKBODY					"LongBlackBody"
#define STR_HTML_BULLISH_HARAMI					"BullishHarami"
#define STR_HTML_BEARISH_HARAMI					"BearishHarami"
#define STR_HTML_BULLISH_ENGULFING				"BullishEngulfing"
#define STR_HTML_BEARISH_ENGULFING				"BearishEngulfing"
#define STR_HTML_BULLISH_COUNTERATTACKLINE		"BullishCounterattackline"
#define STR_HTML_BEARISH_COUNTERATTACKLINE		"BearishCounterattackline"
#define STR_HTML_PIERCINGPATTERN				"PiercingPattern"
#define STR_HTML_DARKCLOUDCOVER					"DarkCloudCover"
#define STR_HTML_MORNINGSTAR					"MorningStar"
#define STR_HTML_EVENINGSTAR					"EveningStar"
#define STR_HTML_MORNINTDOJISTAR				"MorningDojiStar"
#define STR_HTML_EVENINGDOJISTAR				"EveningDojiStar"
#define STR_HTML_ABANDONEDBABY_INDOWNTREND		"Abandonedbabyindowntrend"
#define STR_HTML_ABANDONEDBABY_INUPTREND		"Abandonedbabyinuptrend"
#define STR_HTML_THREE_INSIDEUP					"ThreeInsideUp"
#define STR_HTML_THREE_INSIDEDOWN				"ThreeInsidedown"
#define STR_HTML_THREE_OUTSIDEUP				"ThreeOutsideup"
#define STR_HTML_THREE_OUTSIDEDOWN				"ThreeOutsideDown"
#define STR_HTML_UPSIDEGAPTWOCROWS_INUPTREND	"Upsidegaptwocrowsinuptrend"
#define STR_HTML_THREE_WHITESOLDIER				"Threewhitesolider"
#define STR_HTML_THREE_BLACKCROW				"ThreeBlackcrow"
#define STR_HTML_STALLEDPATTERN					"StalledPattern"
#define STR_HTML_ADVANCEBLOCK					"AdvanceBlock"
#define STR_HTML_UPSIDEGAP_TASUKI				"UpsidegapTasuki"
#define STR_HTML_DNSIDEGAP_TASUKI				"DownwardgapTasuki"
#define STR_HTML_MATHOLD						"Mat-hold"
#define STR_HTML_BULLISH_TRISTAR				"BullishTriStar"
#define STR_HTML_BEARISH_TRISTAR				"BearishTriStar"
#define STR_HTML_UNIQUETHREERIVERBOTTOM			"UniqueThreeRiverBottom"
#define STR_HTML_IDENTICAL_THREECROWS			"IdenticalThreeCrows"
#define STR_HTML_BULLISH_BREAKAWAY				"BullishBreakaway"
#define STR_HTML_BEARISH_BREAKAWAY				"BearishBreakaway"
#define STR_HTML_TWOCORWS						"TwoCrows"
#define STR_HTML_THREESTARS_INTHESOUTH			"ThreeStarsintheSouth"
#define STR_HTML_CONCEALINGBABY_SWALLOW			"ConcealingBabySwallow"
#define STR_HTML_STICKSANDWICH					"StickSandwich"
#define STR_HTML_BULLISH_KICKING				"BullishKicking"	
#define STR_HTML_BEARISH_KICKING				"BearishKicking"
#define STR_HTML_HOMINGPIGEON					"HomingPigeon"
#define STR_HTML_LADDERBOTTOM					"LadderBottom"
#define STR_HTML_MATCHINGLOW					"MatchingLow"
#define STR_HTML_BULLISH_SIDEBYSIDE_WHITELINE	"BullishSide-by-SideWL"
#define STR_HTML_BEARISH_SIDEBYSIDE_WHITELINE	"BearishSide-by-SideWL"
#define STR_HTML_RISING_THREEMETHODS			"RisingThreeMethods"
#define STR_HTML_FALLING_THREEMETHODS			"FallingThreeMethods"
#define STR_HTML_BULLISH_SEPARATINGLINES		"BullishSeparatingLines"
#define STR_HTML_BEARISH_SEPARATINGLINES		"BearishSeparatingLines"
#define STR_HTML_BULLISH_THREELINESTRIKE		"BullishThree-LineStrike"
#define STR_HTML_BEARISH_THREELINESTRIKE		"BearishThree-LineStrike"
#define STR_HTML_UPSIDEGAP_THREEMETHODS			"UpsideGapThreeMethods"
#define STR_HTML_DNSIDEGAP_THREEMETHODS			"DownsideGapThreeMethods"
#define STR_HTML_ON_NECK						"OnNeck"
#define STR_HTML_IN_NECK						"InNeck"
#define STR_HTML_THRUSTINGLINE					"ThrustingLine"

#endif // __PATTERN_H__DEF
#pragma once
#include "axisgenv.h"

#define	LIB_DELETE(XXX) { delete XXX; XXX = NULL; }
#define	BUF_DELETE(XXX)	{ delete[] XXX; XXX = NULL; }
#define	IMG_DELETE(XXX) { XXX->DeleteImageList(); delete XXX; XXX = NULL; }
#define	WND_DELETE(XXX) { XXX->DestroyWindow(); delete XXX; XXX = NULL; }

// event and ctrlID
#define	GEV_VIEW	WM_USER+10
#define	GEV_CHART	WM_USER+11
#define	GEV_INPUT	WM_USER+12
#define	GEV_SISE	WM_USER+13
#define	GEV_CTRL	WM_USER+14
#define	GEV_TOOL	WM_USER+15
#define	GEV_CFG		WM_USER+16
#define	GEV_INFO	WM_USER+17
#define	GEV_TABLE	WM_USER+18
#define	GEV_BASE	WM_USER+19

// image size
#define	szTimeImgX	18
#define	szTimeImgY	20
#define	szDateImgX	18
#define	szDateImgY	20
#define	szCodeImgX	20
#define	szCodeImgY	20
#define	szIEtcImgX	12
#define	szIEtcImgY	20
#define	szIOptImgX	48
#define	szIOptImgY	20
#define	szToolImgX	20
#define	szToolImgY	20
#define	szTool2ImgX	7
#define	szTool2ImgY	20
#define	szCtrlImgX	15
#define	szCtrlImgY	15
#define	szCloseImgX	15
#define	szCloseImgY	15
#define	szTableImgX	15
#define	szTableImgY	15

#define	szI4NameImgX	60
#define	szI4NameImgY	20
#define	szIDollarImgX	20
#define	szIDollarImgY	20
#define	szIForNameImgX	72
#define	szIForNameImgY	20

#define	wCfg	150
#define	hInput	szDateImgY+4
#define	hCtrl	szCtrlImgY
#define	wTool	32
#define	hSise	38
#define	wInfo	130
#define	hTable	122

#define	GRP_VERSION	10
#define	FENV		".ghe"		// chart environment 저장
					// dll name + (index) + ENV
					// version 변경시, ENV, GRP, DAT삭제
#define	FGRP		".ghm"		// chart information 저장
#define	FDAT		".ghd"		// _graph 저장, 추가설정 저장
#define	FSAVE		".ghs"		// 사용자 저장 파일
#define	FINI		".ghi"		// ini 정보 파일


////////////////////////////////////////////////////////////////////////
struct _interChart	{
	BYTE	bAll{};
	BYTE	rgncnt{};
	BYTE	grpcnt{};
};
#define	sz_INTERCHART	sizeof(struct _interChart)

struct _barChart	{
	int	mkind{};	// main kind
	BYTE	rgncnt{};
	BYTE	grpcnt{};
	char	*minfo{};	// map graph info
	char	*ulist{};	// user list
};
#define	sz_BARCHART	sizeof(struct _barChart)

struct _ComplexTRData
{
	int	mLen[4]{};
	int	mRLen[4]{};
	char	*mData[4]{};
};
#define	sz_COMPLEXTRDATA	sizeof(_ComplexTRData)

/////////////////////////////////////////////////////////////////////////
// GEV_VIEW
/////////////////////////////////////////////////////////////////////////
// LOWORD(wParam)
#define	viewInterface	0x01		// view interface
	#define	getRootPath	1		// get root path
						// char* return:path
	#define	getTabPath	2		// get tab path
						// char* return:tab path
	#define	getDatPath	3		// get dat file path
						// char* return:dat path
	#define	getUserName	4		// get user name
						// char* return:user name
	#define	getUserID	5		// get user ID
						// char* return:user id
	#define	requestTR	6		// send Request

#define	viewDialog	0x02		// dialog call
					// return : type에 따라 다르게 return
	#define	dlgSave		1		// 사용자 틀 load/save
	#define dlgEnv		2		// 전체 환경 설정
						// BOOL return
	#define	dlgEnvchart	3		// 전체 환경 설정
	#define	dlgMap		4		// 화면 설정(region + graph)
						// char* return
	#define	dlgIndc		5		// 개별차트 설정
						// char* return
	#define	dlgSIndc	6		// 틀개별차트 설정
						// return bool
	#define	dlgTool		7		// tool 설정
						// return bool
	#define	dlgJCode	8		// 종목검색
						// char* return
	#define	dlgUCode	9		// 업종검색
						// char* return
	#define	dlgFCode	10		// 선물검색
						// char* return
	#define	dlgOCode	11		// 옵션검색
						// char* return
	#define	dlgJUCode	12		// 종목/업종
						// char* return
	#define	dlgFOCode	13		// 선물/옵션
						// char* return
	

#define	viewMain	0x03		// main과 interface
	#define getPalette	1		// palette 정보
	#define	getHistory	2
	#define	setHistory	3
	#define	setTrigger	4

#define	viewChartInfo	0x04		// chart info
	#define	getMapInfo	1		// get map info
						// char* return:_interChart
	#define setMapInfo	2		// set map info
						// char* lParam:_interChart
	#define	getGrpInfo	3		// get Single Graph Info
						// char* lParam:_graph
	#define	getDatInfo	4		// read dat file(차트 기본정보설정파일)
						// char* return:_graphinfo

#define	viewCode	0x05		// codelist,history, grouplist
	#define	hideList	1		// Hide Code,History list
	#define	showGroupTree	2		// Show/Hide Group Tree
						// bool lParam:show/hide
	#define	getNextCode	3		// Get Next Code
						// char* lParam;code
	#define	getPreCode	4		// Get Pre Code
						// char* lParam;code

#define	viewUtility	0x06		// etc function
	#define	viewImage	1
	#define	viewPrint	2
	#define	viewExcel	3

#define	viewNotify	0x07		// Notify (input, ctrl, chart)
	#define	dispDayChart	1		// pnChart message(input, ctrl), display change
	#define	dispDayInput	2		// pnInput message(ctrl, chart), display change
	#define	dispDayCtrl	3		// pnCtrl message(input, chart), display change
	#define	dispScaleChart	4		// pnChart message(input, ctrl), dispscale change
	#define	dispScaleCtrl	5		// pnCtrl message(input, chart), dispscale change
	#define	mainChart	6		// pnChart message(input, ctrl, cfg), mainchart change
	#define	aidAddChart	7		// pnChart message(chart, cfg), add aidchart in chart
	#define	aidAddCfg	8		// pnConfig message(chart), add aidchart in cfg
	#define	aidDelCfg	9		// pnConfig message(chart), delete aidchart in cfg
	#define	aidDelChart	10		// pnChart message(cfg), delete aidchart in chart
	#define	unitInput	11		// pnInput message(sise, chart), inputBar unit change
	#define	indexInput	12		// pnInput message(chart), inputBar index change
	#define	inputAutotran	13		// pnInput message(cfg, ctrl, chart, tool), inputBar autotran
	#define	tableChart	14		// pnChart message(Main, cfg), table show/hide
	#define	gridChart	15		// pnChart message(Main, cfg), grid show/hide
	#define	tableCtrl	16		// pnChart message(Main, chart), table show/hide
	#define	gridCtrl	17		// pnChart message(Main, chart), grid show/hide
	#define	cfgMainChart	18		// pnConfig message(input, ctrl, chart), mainchart change

#define	viewWindow	0x08		// Window Show Hide
	#define	viewSise	1
	#define	viewTool	2
	#define	viewTable	3
	#define	viewCtrl	4
	#define	viewCfg		5

/////////////////////////////////////////////////////////////////////////
// GEV_INPUT
/////////////////////////////////////////////////////////////////////////
#define	inputChange	0x01	// change value
	#define	ipUnit		1	// int   lParam:unit
	#define	ipCode		2	// char* lParam:code
	#define	ipCodeArrow	3	// int   lParam:VK_UP,VK_DOWN
	#define	ipGap		4	// int   lParam:gap
	#define	ipDate		5	//
	#define	ipGigan		6	// int   lParam:gigan
	#define	ipDisplay	7	// int   lParam:display
	#define	ipRotate	8	// int   lParam:time
	#define	ipTab		9	// int   lParam:id
	#define	ipBtn		10	// int   lParam:id

#define	inputData	0x02	// request data
	#define	getTRInfo	1	// get TRsend Header info, char* return
	#define	getTotalDay	2	// get Totalday
	#define	getDispDay	3	// get DisplayDay
	#define	getCalendar	4	// get date
	#define	setCodeInfo	5	// set info
					// char* lParam:code\tname 
	#define	domminoCode	6	// char* lParam:sym\tcode...
	#define	triggerCode	7	// char* lParam:sym\tcode
	#define	isDollarData	8	// int   return:bool
	#define	isTodayData	9	// int   return:bool

#define	inputEnv	0x03	// argument control
	#define	inputMain	1	// change main chart, display enable/disable
					// int   lParam:kind
	#define	inputDisplay	2	// int   lParam:display day
	#define	inputReload	3	//
	#define	autotranCfg	4	// int   lParam:set/reset, cfg autotran
					// $$$$$

#define	inputViewFocus	0x04		// view focus
#define	inputGetPosComment	0x05	// LOWORD(lParam):x, HIWORD(lParam):y
					// char* return:comment
#define	inputInitForeign	0x06	// Foreign Chart initiate

/////////////////////////////////////////////////////////////////////////
// GEV_CHART
/////////////////////////////////////////////////////////////////////////
#define	chartChgPalette		0x01	// change Palette

#define	chartSetGrp		0x02	// set graph
	#define	changeEnvInfo	1	// char* lParam:envinfo
	#define	changeMainGrp	2	// int   lParam:main kind
	#define	changeMapInfo	3	// _interChart* lParam
	#define	addAidGrp	4	// char* lParam:indicator
	#define	delAidGrp	5	// char* lParam:indicator
	#define	chartReload	6	// char* lParam:graph info
	#define	chartIReload	7	// char* lParam:graph info

#define	chartTRData		0x03	// Transaction data, len == 0, clear
					// int*  HIWORD(wParam):length, char* lParam:data
#define	chartData		0x04	// chart data
	#define	requestHeader	1	// Request Graph Header, char* return
					// int   lParam:length
	#define	realtimeData	2	// Realtime Data
					// char* lParam:
	#define	requestCurEnv	3	// _envinfo
	#define	requestCurMap	4	// map info
	#define	excelData	5	// excel
	#define	getTableData	6	// char* return:table data

#define	chartEnv		0x05	// change env 
	#define	chartIsValid	1	// bool  return: valid state
	#define	chartIndex	2	// int   LOWORD(lParam):index, HIWORD(lParam):gap 
	#define	chartUnit	3	// int   lParam:unit
	#define	chartTotal	4	// int   lParam:total
	#define	chartDisplay	5	// int   lParam:display
	#define	chartScale	6	// int   lParam:scale
					// int return:dispDay
	#define	chartTable	7	// bool  lParam: table state
	#define	chartGrid	8	// bool  lParam: grid state

#define	chartCtrl		0x06
	#define	getScrollData	1	// get scroll data, return data len, lParam:data reference
	#define	changePosition	2	// change pos, LOWORD(lParam) : dispPos, HIWORD(lParam) : dispEnd
	#define	changeShift	3	// change shift, lParam:shift
	#define	autoScroll	4	// autoscroll, lParam:TRUE(autoscroll),FALSE(not)
	#define	isAutoScroll	5	// is autoscroll?
	#define	chartToolSelect	6	// tool select, lParam:select
	#define	chartToolColor	7	// int	lParam:RGB
	#define	chartToolWidth	8	// int	lParam:두께
	#define	chartExcel	9	// excel
	#define	chartAutotran	10

#define	chartViewFocus		0x07	// view focus
#define	chartToolTip		0x08	// tooltip
					// HIWORD(wParam) : bool, lParam:char*

/////////////////////////////////////////////////////////////////////////
// GEV_SISE
/////////////////////////////////////////////////////////////////////////
#define	siseChgPalette		0x01	// change Palette
#define	siseTRData		0x02	// Transaction data, len == 0, clear
					// int*  HIWORD(wParam):length, char* lParam:data
#define	siseRTMData		0x03	// Realtime Data
					// char*  lParam:
#define	siseUnit		0x04	// change unit
					// int    lParam:unit
#define	siseData		0x05	// request data
	#define	siseCurr	1	// 현재가
					// char*  lParam
	#define	siseCode	2	// realtime code
					// char* return:code
	#define	siseDate	3	// 영업일자
					// char* return:date
	#define	sisePivot	4	// 기준값 
					// char* return:상한\t하한\t전일종가
	#define	siseStock	5	// 주식수
	#define	siseDollar	6	// convert into dollar data
					// char* return:stock
#define	siseFlash		0x06	// HIWORD(WPARAM) == 1, blink otherwise not blink
					// flash : blink color palette index
#define	siseFont		0x07	// 


/////////////////////////////////////////////////////////////////////////
// GEV_CTRL
/////////////////////////////////////////////////////////////////////////
#define	ctrlScrollPos		0x01	// scroll pos
					// HIWORD(wParam) : nSBCode, lParam : pos
#define	ctrlGetPosComment	0x02	// LOWORD(lParam):x, HIWORD(lParam):y
					// char* return:comment
#define	ctrlEnv			0x03	// argument control
	#define	ctrlMain	1	// change main chart, display enable/disable
					// int   lParam:kind
	#define	ctrlDisplay	2	// int   lParam:display day
	#define	ctrlEnable	3	// bool  lParam:enable state
	#define	ctrlAutotran	4	// bool  lParam:set state
	#define	ctrlGrid	5	// bool  lParam:Grid state
	#define	ctrlTable	6	// bool  lParam:Table state
	#define	ctrlReload	7	//

/////////////////////////////////////////////////////////////////////////
// GEV_TOOL
/////////////////////////////////////////////////////////////////////////
#define	toolGetPosComment	1	// LOWORD(lParam):x, HIWORD(lParam):y
					// char* return:comment
#define	toolRelease		2	// release tool button
#define	toolCheckRect		3	// check lbuttondown is toolRect
					// int	return:id
#define	toolGetSelectTool	4	// chart -> select tool button
#define	toolAutotran		5	// view -> autotran

/////////////////////////////////////////////////////////////////////////
// GEV_TABLE
/////////////////////////////////////////////////////////////////////////
#define	tbItem			1	// char* lParam:items
#define	tbUpdate		2	// char* lParam:update items
#define	tbShift			3	// char* lParam:shift items
#define	tbInsert		4	// char* lParam:insert column
#define	tbSelect		5	// int   lParam:select index
#define	tbReset			6	// reset table

/////////////////////////////////////////////////////////////////////////
// GEV_CONFIG
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Symbolic Interface
/////////////////////////////////////////////////////////////////////////

#define	IJ_CODE		"1301"	// input code
#define	II_CODE		"21301"	// input code
#define	IF_CODE		"31301"	// input code
#define	IO_CODE		"41301"	// input code
#define	IS_CODE		"51301"	// input code, 해외시세

/////////////////////////// 헤더
#define	OS_CODE		"51021"	// original code, RTS code
#define	OS_NAME		"51022"	// 한글명
#define	OS_JGGA		"51023"	// 종가(현재가)
#define	OS_DIFF		"51024"	// 전일대비
#define	OS_GVOL		"51027"	// 거래량
#define	OS_SIGA		"51029"	// 시가
#define	OS_KOGA		"51030"	// 고가
#define	OS_JEGA		"51031"	// 저가
#define	OS_RATE		"51033"	// 등락율
#define	OS_TIME		"51034"	// 체결시간
#define	OS_CDAY		"51310"	// 당일자(최종영업일), yyyymmdd
#define	OS_JJGA		"51313"	// 전일종가
#define	OS_STCK		"51316"	// 상장주식수, 없음

#define	OJ_CODE		"1021"	// original code, RTS code
#define	OJ_NAME		"1022"	// 한글명
#define	OJ_JGGA		"1023"	// 종가(현재가)
#define	OJ_DIFF		"1024"	// 전일대비
#define	OJ_MDHG		"1025"	// 매도호가
#define	OJ_MSHG		"1026"	// 매수호가
#define	OJ_GVOL		"1027"	// 거래량
#define	OJ_AMT		"1028"	// 거래대금
#define	OJ_SIGA		"1029"	// 시가
#define	OJ_KOGA		"1030"	// 고가
#define	OJ_JEGA		"1031"	// 저가
#define	OJ_RATE		"1033"	// 등락율
#define	OJ_TIME		"1034"	// 체결시간
#define	OJ_CDAY		"1310"	// 당일자(최종영업일), yyyymmdd
#define	OJ_SAHG		"1311"	// 상한가
#define	OJ_HAHG		"1312"	// 하한가
#define	OJ_JJGA		"1313"	// 전일종가
#define	OJ_STCK		"1316"	// 상장주식수

#define OJ_HTIME	"3040"	// 호가수신시간  <== 체결시간으로 대체??
#define	OJ_MDJR1	"3041"	// 매도호가잔량
#define	OJ_MDJR2	"3042"
#define	OJ_MDJR3	"3043"
#define	OJ_MDJR4	"3044"
#define	OJ_MDJR5	"3045"
#define	OJ_MDJR6	"3046"
#define	OJ_MDJR7	"3047"
#define	OJ_MDJR8	"3048"
#define	OJ_MDJR9	"3049"
#define	OJ_MDJRA	"3050"
#define	OJ_MDGA1	"3051"	// 매도호가
#define	OJ_MDGA2	"3052"
#define	OJ_MDGA3	"3053"
#define	OJ_MDGA4	"3054"
#define	OJ_MDGA5	"3055"
#define	OJ_MDGA6	"3056"
#define	OJ_MDGA7	"3057"
#define	OJ_MDGA8	"3058"
#define	OJ_MDGA9	"3059"
#define	OJ_MDGAA	"3060"
#define	OJ_MSJR1	"3061"	// 매수호가잔량
#define	OJ_MSJR2	"3062"
#define	OJ_MSJR3	"3063"
#define	OJ_MSJR4	"3064"
#define	OJ_MSJR5	"3065"
#define	OJ_MSJR6	"3066"
#define	OJ_MSJR7	"3067"
#define	OJ_MSJR8	"3068"
#define	OJ_MSJR9	"3069"
#define	OJ_MSJRA	"3070"
#define	OJ_MSGA1	"3071"	// 매수호가
#define	OJ_MSGA2	"3072"
#define	OJ_MSGA3	"3073"
#define	OJ_MSGA4	"3074"
#define	OJ_MSGA5	"3075"
#define	OJ_MSGA6	"3076"
#define	OJ_MSGA7	"3077"
#define	OJ_MSGA8	"3078"
#define	OJ_MSGA9	"3079"
#define	OJ_MSGAA	"3080"
#define OJ_MDJRTOT	"3101" // 매도호가 총잔량
#define OJ_MDOJRTOT	"3102" // 시간외 매도호가 총잔량
#define OJ_MSJRTOT	"3106" // 매수호가 총잔량
#define OJ_MSOJRTOT	"3107" // 시간외 매수호가 총잔량
#define OJ_YCHEGURL	"3111" // 예상체결가
#define OJ_YCHEVOL	"3112" // 예상체결량


#define	OI_CODE		"21021"	// original code, RTS code
#define	OI_NAME		"21022"	// 한글명
#define	OI_JGGA		"21023"	// 종가(현재가)
#define	OI_DIFF		"21024"	// 전일대비
#define	OI_MDHG		"21025"	// 매도호가
#define	OI_MSHG		"21026"	// 매수호가
#define	OI_GVOL		"21027"	// 거래량
#define	OI_AMT		"21028"	// 거래대금
#define	OI_SIGA		"21029"	// 시가
#define	OI_KOGA		"21030"	// 고가
#define	OI_JEGA		"21031"	// 저가
#define	OI_RATE		"21033"	// 등락율
#define	OI_TIME		"21034"	// 체결시간
#define	OI_CDAY		"21310"	// 당일자(최종영업일), yyyymmdd
#define	OI_SAHG		"21311"	// 상한가
#define	OI_HAHG		"21312"	// 하한가
#define	OI_JJGA		"21313"	// 전일종가
#define	OI_STCK		"21316"	// 상장주식수


#define	OF_CODE		"31021"	// original code, RTS code
#define	OF_NAME		"31022"	// 한글명
#define	OF_JGGA		"31023"	// 종가(현재가)
#define	OF_DIFF		"31024"	// 전일대비
#define	OF_MDHG		"31025"	// 매도호가
#define	OF_MSHG		"31026"	// 매수호가
#define	OF_GVOL		"31027"	// 거래량
#define	OF_AMT		"31028"	// 거래대금
#define	OF_SIGA		"31029"	// 시가
#define	OF_KOGA		"31030"	// 고가
#define	OF_JEGA		"31031"	// 저가
#define	OF_RATE		"31033"	// 등락율
#define	OF_TIME		"31034"	// 체결시간
#define	OF_CDAY		"31310"	// 당일자(최종영업일), yyyymmdd
#define	OF_SAHG		"31311"	// 상한가
#define	OF_HAHG		"31312"	// 하한가
#define	OF_JJGA		"31313"	// 전일종가
#define	OF_STCK		"31000"	// 상장주식수

#define OF_HTIME	"32040"	// 호가수신시간  
#define	OF_MDJR1	"32041"	// 매도호가잔량
#define	OF_MDJR2	"32042"
#define	OF_MDJR3	"32043"
#define	OF_MDJR4	"32044"
#define	OF_MDJR5	"32045"
#define	OF_MDJR6	"32046"
#define	OF_MDJR7	"32047"
#define	OF_MDJR8	"32048"
#define	OF_MDJR9	"32049"
#define	OF_MDJRA	"32050"
#define	OF_MDGA1	"32051"	// 매도호가
#define	OF_MDGA2	"32052"
#define	OF_MDGA3	"32053"
#define	OF_MDGA4	"32054"
#define	OF_MDGA5	"32055"
#define	OF_MDGA6	"32056"
#define	OF_MDGA7	"32057"
#define	OF_MDGA8	"32058"
#define	OF_MDGA9	"32059"
#define	OF_MDGAA	"32060"
#define	OF_MSJR1	"32061"	// 매수호가잔량
#define	OF_MSJR2	"32062"
#define	OF_MSJR3	"32063"
#define	OF_MSJR4	"32064"
#define	OF_MSJR5	"32065"
#define	OF_MSJR6	"32066"
#define	OF_MSJR7	"32067"
#define	OF_MSJR8	"32068"
#define	OF_MSJR9	"32069"
#define	OF_MSJRA	"32070"
#define	OF_MSGA1	"32071"	// 매수호가
#define	OF_MSGA2	"32072"
#define	OF_MSGA3	"32073"
#define	OF_MSGA4	"32074"
#define	OF_MSGA5	"32075"
#define	OF_MSGA6	"32076"
#define	OF_MSGA7	"32077"
#define	OF_MSGA8	"32078"
#define	OF_MSGA9	"32079"
#define	OF_MSGAA	"32080"
#define	OF_MDSU1	"32211"	// 매도호가건수
#define	OF_MDSU2	"32212"
#define	OF_MDSU3	"32213"
#define	OF_MDSU4	"32214"
#define	OF_MDSU5	"32215"
#define	OF_MDSU6	"32216"
#define	OF_MDSU7	"32217"
#define	OF_MDSU8	"32218"
#define	OF_MDSU9	"32219"
#define	OF_MDSUA	"32220"
#define	OF_MSSU1	"32221"	// 매수호가건수
#define	OF_MSSU2	"32222"
#define	OF_MSSU3	"32223"
#define	OF_MSSU4	"32224"
#define	OF_MSSU5	"32225"
#define	OF_MSSU6	"32226"
#define	OF_MSSU7	"32227"
#define	OF_MSSU8	"32228"
#define	OF_MSSU9	"32229"
#define	OF_MSSUA	"32250"
#define OF_MDJRTOT	"32101" // 매도호가 총잔량
#define OF_MSJRTOT	"32106" // 매수호가 총잔량
#define OF_MDSUTOT	"32103" // 매도호가 총건수
#define OF_MSSUTOT	"32108" // 매수호가 총건수


#define	OO_CODE		"41021"	// original code, RTS code
#define	OO_NAME		"41022"	// 한글명
#define	OO_JGGA		"41023"	// 종가(현재가)
#define	OO_DIFF		"41024"	// 전일대비
#define	OO_MDHG		"41025"	// 매도호가
#define	OO_MSHG		"41026"	// 매수호가
#define	OO_GVOL		"41027"	// 거래량
#define	OO_AMT		"41028"	// 거래대금
#define	OO_SIGA		"41029"	// 시가
#define	OO_KOGA		"41030"	// 고가
#define	OO_JEGA		"41031"	// 저가
#define	OO_RATE		"41033"	// 등락율
#define	OO_TIME		"41034"	// 체결시간
#define	OO_CDAY		"41310"	// 당일자(최종영업일), yyyymmdd
#define	OO_SAHG		"41311"	// 상한가
#define	OO_HAHG		"41312"	// 하한가
#define	OO_JJGA		"41313"	// 전일종가
#define	OO_STCK		"41000"	// 상장주식수

#define OO_HTIME	"42040"	// 호가수신시간  <== 체결시간으로 대체??
#define	OO_MDJR1	"42041"	// 매도호가잔량
#define	OO_MDJR2	"42042"
#define	OO_MDJR3	"42043"
#define	OO_MDJR4	"42044"
#define	OO_MDJR5	"42045"
#define	OO_MDJR6	"42046"
#define	OO_MDJR7	"42047"
#define	OO_MDJR8	"42048"
#define	OO_MDJR9	"42049"
#define	OO_MDJRA	"42050"
#define	OO_MDGA1	"42051"	// 매도호가
#define	OO_MDGA2	"42052"
#define	OO_MDGA3	"42053"
#define	OO_MDGA4	"42054"
#define	OO_MDGA5	"42055"
#define	OO_MDGA6	"42056"
#define	OO_MDGA7	"42057"
#define	OO_MDGA8	"42058"
#define	OO_MDGA9	"42059"
#define	OO_MDGAA	"42060"
#define	OO_MSJR1	"42061"	// 매수호가잔량
#define	OO_MSJR2	"42062"
#define	OO_MSJR3	"42063"
#define	OO_MSJR4	"42064"
#define	OO_MSJR5	"42065"
#define	OO_MSJR6	"42066"
#define	OO_MSJR7	"42067"
#define	OO_MSJR8	"42068"
#define	OO_MSJR9	"42069"
#define	OO_MSJRA	"42070"
#define	OO_MSGA1	"42071"	// 매수호가
#define	OO_MSGA2	"42072"
#define	OO_MSGA3	"42073"
#define	OO_MSGA4	"42074"
#define	OO_MSGA5	"42075"
#define	OO_MSGA6	"42076"
#define	OO_MSGA7	"42077"
#define	OO_MSGA8	"42078"
#define	OO_MSGA9	"42079"
#define	OO_MSGAA	"42080"
#define	OO_MDSU1	"42211"	// 매도호가건수
#define	OO_MDSU2	"42212"
#define	OO_MDSU3	"42213"
#define	OO_MDSU4	"42214"
#define	OO_MDSU5	"42215"
#define	OO_MDSU6	"42216"
#define	OO_MDSU7	"42217"
#define	OO_MDSU8	"42218"
#define	OO_MDSU9	"42219"
#define	OO_MDSUA	"42220"
#define	OO_MSSU1	"42221"	// 매수호가건수
#define	OO_MSSU2	"42222"
#define	OO_MSSU3	"42223"
#define	OO_MSSU4	"42224"
#define	OO_MSSU5	"42225"
#define	OO_MSSU6	"42226"
#define	OO_MSSU7	"42227"
#define	OO_MSSU8	"42228"
#define	OO_MSSU9	"42229"
#define	OO_MSSUA	"42250"
#define OO_MDJRTOT	"42101" // 매도호가 총잔량
#define OO_MSJRTOT	"42106" // 매수호가 총잔량
#define OO_MDSUTOT	"42103" // 매도호가 총건수
#define OO_MSSUTOT	"42108" // 매수호가 총건수

///////////////////////////해외 시세
#define	DSD_SYM		"52500"

#define	DSD_DAY		"52302"	// 일자
#define	DSD_SIGA	"52029"	// 시가
#define	DSD_KOGA	"52030"	// 고가
#define	DSD_JEGA	"52031"	// 저가
#define	DSD_JGGA	"52023"	// 종가
#define	DSD_GVOL	"52027"	// 거래량
#define	DSD_CVOL	"52032"	// 체결량

///////////////////////////종목 시세

#define	DJT_SYM		"2500"
#define	DJM_SYM		"4500"
#define	DJD_SYM		"5500"

#define	DJT_DAY		"2302"	// 일자
#define	DJT_HMS		"2034"	// 시간
#define	DJT_SIGA	"2029"	// 시가
#define	DJT_KOGA	"2030"	// 고가
#define	DJT_JEGA	"2031"	// 저가
#define	DJT_JGGA	"2023"	// 종가
#define	DJT_GVOL	"2027"	// 거래량
#define	DJT_CVOL	"2032"	// 체결량
#define	DJT_GAMT	"2028"	// 거래대금
#define	DJT_DUMP	"2025"	// 매도호가(틱에는 없음)

#define	DJM_DAY		"4302"
#define	DJM_HMS		"4034"
#define	DJM_SIGA	"4029"
#define	DJM_KOGA	"4030"
#define	DJM_JEGA	"4031"
#define	DJM_JGGA	"4023"
#define	DJM_GVOL	"4027"
#define	DJM_CVOL	"4032"
#define	DJM_GAMT	"4028"	
#define	DJM_DUMP	"4025"

#define	DJD_DAY		"5302"
#define	DJD_HMS		"5034"
#define	DJD_SIGA	"5029"
#define	DJD_KOGA	"5030"
#define	DJD_JEGA	"5031"
#define	DJD_JGGA	"5023"
#define	DJD_GVOL	"5027"
#define	DJD_CVOL	"5032"
#define	DJD_GAMT	"5028"
#define	DJD_LOCK	"5311"
#define	DJD_RATE	"5310"
#define	DJD_DOL		"5320"	// dollar
#define	DJD_DUMP	"5025"	

///////////////////////////업종 시세
#define	DIT_SYM		"22500"
#define	DIM_SYM		"24500"
#define	DID_SYM		"25500"

#define	DIT_DAY		"22302"	// 일자
#define	DIT_HMS		"22034"	// 시간
#define	DIT_SIGA	"22029"	// 시가
#define	DIT_KOGA	"22030"	// 고가
#define	DIT_JEGA	"22031"	// 저가
#define	DIT_JGGA	"22023"	// 종가
#define	DIT_GVOL	"22027"	// 거래량
#define	DIT_CVOL	"22032"	// 체결량
#define	DIT_GAMT	"22028"	// 거래대금
#define	DIT_GVOL	"22027"	// 거래량
#define	DIT_CVOL	"22032"	// 체결량
#define	DIT_GAMT	"22028"	// 거래대금
#define	DIT_DUMP	"22025"	

#define	DIM_DAY		"24302"
#define	DIM_HMS		"24034"
#define	DIM_SIGA	"24029"
#define	DIM_KOGA	"24030"
#define	DIM_JEGA	"24031"
#define	DIM_JGGA	"24023"
#define	DIM_GVOL	"24027"
#define	DIM_CVOL	"24032"
#define	DIM_GAMT	"24028"
#define	DIM_DUMP	"24025"		

#define	DID_DAY		"25302"
#define	DID_HMS		"25034"
#define	DID_SIGA	"25029"
#define	DID_KOGA	"25030"
#define	DID_JEGA	"25031"
#define	DID_JGGA	"25023"
#define	DID_GVOL	"25027"
#define	DID_CVOL	"25032"
#define	DID_GAMT	"25028"
#define	DID_UCNT	"25256"	// 상한+상승종목수
#define	DID_DCNT	"25257"	// 하한+하락종목수
#define	DID_DUMP	"25025"

///////////////////////////선물 시세
#define	DFT_SYM		"32500"
#define	DFM_SYM		"34500"
#define	DFD_SYM		"35500"

#define	DFT_DAY		"32302"	// 일자
#define	DFT_HMS		"32034"	// 시간
#define	DFT_SIGA	"32029"	// 시가
#define	DFT_KOGA	"32030"	// 고가
#define	DFT_JEGA	"32031"	// 저가
#define	DFT_JGGA	"32023"	// 종가
#define	DFT_GVOL	"32027"	// 거래량
#define	DFT_CVOL	"32032"	// 체결량
#define	DFT_GAMT	"32028"	// 거래대금
#define	DFT_MYGL	"32201"	// 미결제 약정

#define	DFM_DAY		"34302"
#define	DFM_HMS		"34034"
#define	DFM_SIGA	"34029"
#define	DFM_KOGA	"34030"
#define	DFM_JEGA	"34031"
#define	DFM_JGGA	"34023"
#define	DFM_GVOL	"34027"
#define	DFM_CVOL	"34032"
#define	DFM_GAMT	"34028"
#define	DFM_MYGL	"34201"

#define	DFD_DAY		"35302"
#define	DFD_HMS		"35034"
#define	DFD_SIGA	"35029"
#define	DFD_KOGA	"35030"
#define	DFD_JEGA	"35031"
#define	DFD_JGGA	"35023"
#define	DFD_GVOL	"35027"
#define	DFD_CVOL	"35032"
#define	DFD_GAMT	"35028"
#define	DFD_MYGL	"35201"


///////////////////////////옵션 시세
#define	DOT_SYM		"42500"
#define	DOM_SYM		"44500"
#define	DOD_SYM		"45500"

#define	DOT_DAY		"42302"	// 일자
#define	DOT_HMS		"42034"	// 시간
#define	DOT_SIGA	"42029"	// 시가
#define	DOT_KOGA	"42030"	// 고가
#define	DOT_JEGA	"42031"	// 저가
#define	DOT_JGGA	"42023"	// 종가
#define	DOT_GVOL	"42027"	// 거래량
#define	DOT_CVOL	"42032"	// 체결량
#define	DOT_GAMT	"42028"	// 거래대금
#define	DOT_MYGL	"42201"	// 미결제 약정

#define	DOM_DAY		"44302"
#define	DOM_HMS		"44034"
#define	DOM_SIGA	"44029"
#define	DOM_KOGA	"44030"
#define	DOM_JEGA	"44031"
#define	DOM_JGGA	"44023"
#define	DOM_GVOL	"44027"
#define	DOM_CVOL	"44032"
#define	DOM_GAMT	"44028"
#define	DOM_MYGL	"44201"

#define	DOD_DAY		"45302"
#define	DOD_HMS		"45034"
#define	DOD_SIGA	"45029"
#define	DOD_KOGA	"45030"
#define	DOD_JEGA	"45031"
#define	DOD_JGGA	"45023"
#define	DOD_GVOL	"45027"
#define	DOD_CVOL	"45032"
#define	DOD_GAMT	"45028"
#define	DOD_MYGL	"45201"


// Realtime Data Symbol
#define	R_CODX	"021"	// 종목코드(단축코드5)
#define	R_HNAM	"022"	// 종목명
#define	R_CURR	"023"	// 현재가
#define	R_DIFF	"024"	// 전일대비
#define	R_MDGA	"025"	// 매도호가
#define	R_MSGA	"026"	// 매수호가
#define	R_GVOL	"027"	// 거래량
#define	R_GAMT	"028"	// 거래대금(억)
#define	R_SIGA	"029"	// 시가
#define	R_KOGA	"030"	// 고가
#define	R_JEGA	"031"	// 저가
#define	R_CVOL	"032"	// 체결거래량(체결수량)
#define	R_UDYL	"033"	// 등락율
#define	R_CTIM	"034"	// 체결시간
#define	R_CCOL	"035"	// 체결구분
#define	R_HTIM	"050"	// 호가시간
#define	R_DHG1	"051"	// 1차 매도호가
#define	R_DHG2	"052"	// 2차 매도호가
#define	R_DHG3	"053"	// 3차 매도호가
#define	R_DHG4	"054"	// 4차 매도호가
#define	R_DHG5	"055"	// 5차 매도호가
#define	R_DHG6	"056"	// 6차 매도호가
#define	R_DHG7	"057"	// 7차 매도호가
#define	R_DHG8	"058"	// 8차 매도호가
#define	R_DHG9	"059"	// 9차 매도호가
#define	R_DHGA	"060"	// A차 매도호가
#define	R_SHG1	"071"	// 1차 매수호가
#define	R_SHG2	"072"	// 2차 매수호가
#define	R_SHG3	"073"	// 3차 매수호가
#define	R_SHG4	"074"	// 4차 매수호가
#define	R_SHG5	"075"	// 5차 매수호가
#define	R_SHG6	"076"	// 6차 매수호가
#define	R_SHG7	"077"	// 7차 매수호가
#define	R_SHG8	"078"	// 8차 매수호가
#define	R_SHG9	"079"	// 9차 매수호가
#define	R_SHGA	"080"	// A차 매수호가

#define	R_MGJY	"201"	// 미결제약정(선물)
#define	R_SAHA	"251"	// 상한종목수
#define	R_SASE	"252"	// 상승종목수
#define	R_HAHA	"254"	// 하한종목수
#define	R_HARA	"255"	// 하락종목수


#if !defined(GRAPH_COMMON_H)
#define GRAPH_COMMON_H

#include "axisgenv.h"

#define	COMPANYNAME	"KTB투자증권"
#define	K200_NAME	"KOSPI200"
#define	K200_TIP	"KOSPI200지수"
#define	K200_INDEX	"11111111"
#define	K200_FUTURE	"99999999"
#define	STAR_NAME	"KOSTAR"
#define	STAR_TIP	"KOSTAR지수"
#define	STAR_INDEX	"22222222"
#define	STAR_FUTURE	"88888888"

#define	LIB_DELETE(XXX) { delete XXX; XXX = NULL; }
#define	BUF_DELETE(XXX)	{ delete[] XXX; XXX = NULL; }
#define	IMG_DELETE(XXX) { XXX->DeleteImageList(); delete XXX; XXX = NULL; }
#define	WND_DELETE(XXX) { XXX->DestroyWindow(); delete XXX; XXX = NULL; }

// event and ctrlID
#define	GEV_VIEW	(WM_USER + 10)
#define	GEV_CHART	(WM_USER + 11)
#define	GEV_INPUT	(WM_USER + 12)
#define	GEV_SISE	(WM_USER + 13)
#define	GEV_CTRL	(WM_USER + 14)
#define	GEV_TOOL	(WM_USER + 15)
#define	GEV_CFG		(WM_USER + 16)
//#define	GEV_INFO	(WM_USER + 17)
#define	GEV_AID	(WM_USER + 17)
#define	GEV_TABLE	(WM_USER + 18)
#define	GEV_BASE	(WM_USER + 19)

// message form Main
#define UM_FROM_MAIN	(WM_USER + 507)

// image size
#define	szTimeImgX	17
#define	szTimeImgY	18
#define	szDateImgX	17
#define	szDateImgY	18
#define	szCodeImgX	18
#define	szCodeImgY	18
#define	szIEtcImgX	16
#define	szIEtcImgY	18
#define	szIOptImgX	35
#define	szIOptImgY	18
#define	szIBtnImgX	64
#define	szIBtnImgY	18
#define	szICfgImgX	20
#define	szICfgImgY	20
#define	szToolImgX	20
#define	szToolImgY	20
#define	szTool2ImgX	7
#define	szTool2ImgY	20
#define	szCtrlImgX	16
#define	szCtrlImgY	16
#define	szCtrl2ImgX	15
#define	szCtrl2ImgY	16

#define szToolCtrlImgX	20
#define szToolCtrlImgY	20

#define	szI4NameImgX	60
#define	szI4NameImgY	20
#define	szIDollarImgX	20
#define	szIDollarImgY	20
#define	szIForNameImgX	72
#define	szIForNameImgY	20

//#define	WIDTH_CFG	150
#define WIDTH_CFG	180
#define	WIDTH_TOOL	33
#define	WIDTH_AID	33
//#define	WIDTH_INFO	130
#define	HEIGHT_INPUT	(szDateImgY + 4)
#define	HEIGHT_CTRL	szCtrlImgY
#define	HEIGHT_SISE	40
#define	HEIGHT_TABLE	122


#define	VERSION_ENV	2
#define	VERSION_GRP	2
#define	VERSION_DAT	2
#define	VERSION_TOL	1
#define	VERSION_AID	1
#define	VERSION_USV	2

#define	VERSION_ENV_OLD	(VERSION_ENV - 1)
#define	VERSION_GRP_OLD	(VERSION_GRP - 1)
#define	VERSION_DAT_OLD	(VERSION_DAT - 1)
#define	VERSION_USV_OLD	(VERSION_USV - 1)

// New
#define ENV		".en0"
#define GRP		".gr0"
#define DAT		".da0"
#define USV		".us0"

#define TOL		".to0"
#define AID		".ai0"

// basic (콘트롤용)
#define ENV_B		".eb0"
#define GRP_B		".gb0"

#define	GRP_VERSION	14


////////////////////////////////////////////////////////////////////////
struct _interChart	{
	BYTE	bAll;
	BYTE	rgncnt;
	BYTE	grpcnt;
};
#define	sz_INTERCHART	sizeof(struct _interChart)

struct _barChart	{
	int	mkind;	// main kind
	BYTE	rgncnt;
	BYTE	grpcnt;
	char	*minfo;	// map graph info
	char	*ulist;	// user list
};
#define	sz_BARCHART	sizeof(struct _barChart)

struct _trData
{
	int	iLen[2];
	int	iSiseLen[2];
	char*	pcData[2];
};
#define	SZ_TRDATA	sizeof(_trData)


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
	#define	requestTRS	7		// send request single tr
	#define	getDllName	8		


#define	viewDialog		0x02		// dialog call
					// return : type에 따라 다르게 return
	#define	dlgSave		1		// 사용자 틀 load/save
	#define dlgEnv		2		// 전체 환경 설정
						// BOOL return
	#define	dlgEnvchart	3		// 전체 환경 설정
	#define	dlgMap		4		// 화면 설정(region + graph)
						// char* return
	#define	DlgIndc		5		// 개별차트 설정
						// char* return
	#define	dlgSIndc	6		// 틀개별차트 설정
						// return bool
	#define	dlgTool		7		// tool 설정
						// return bool
	#define	dlgGwan		8		// 관심종목등록
	#define	dlgJCode	9		// 종목검색
						// char* return
	#define	dlgUCode	10		// 업종검색
						// char* return
	#define	dlgFCode	11		// 선물검색
						// char* return
	#define	dlgOCode	12		// 옵션검색
						// char* return
	#define	dlgJUCode	13		// 종목/업종
						// char* return
	#define	dlgFOCode	14		// 선물/옵션
						// char* return
	#define	dlgGubnFOSO	15		// 선물/옵션/주식옵션 체크
						// int   return:
						// 1-선물, 2-옵션, 3-주식옵션
	#define dlgToolCfg	16	// 작도툴바

	#define DlgAidCfg	17	// 지표바

	#define DlgYAxisCfg	18	// y 축 설정

	#define DlgYShowCfg	19	// 드래그 합성시 y 축 보기 옵션

	
	
	

#define	viewMain		0x03		// main과 interface
	#define getPalette	1		// palette 정보
	#define	getHistory	2
	#define	setHistory	3
	#define	setTrigger	4
	#define	openView	5
	#define	isSiseOnly	6

#define	viewChartInfo	0x04		// chart info
	#define	getMapInfo	1		// get map info
						// char* return:_interChart
	#define setMapInfo	2		// set map info
						// char* lParam:_interChart
	#define	getSGrpInfo	3		// get Single Graph Info
						// char* lParam:_graph
	#define	getDatInfo	4		// read dat file(차트 기본정보설정파일)
						// char* return:_graphinfo
	#define	initGrpInfo	5		// init graph info
	#define	getMainGrpInfo 6  // return: boolean (if true then main graph is bong or line type other than vartype

#define	viewCode	0x05		// codelist,history, grouplist
	#define	hideList	1		// hide History list
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
	#define	dispDayCtrl		3		// pnCtrl message(input, chart), display change
	#define	dispDayCtrl2	4		// pnCtrl message(input, chart), display change
	#define	dispScaleChart	5		// pnChart message(input, ctrl), dispscale change
	#define	dispScaleCtrl	6		// pnCtrl message(input, chart), dispscale change
	#define	mainChart		7		// pnChart message(input, ctrl, cfg), mainchart change
	#define	aidAddChart		8		// pnChart message(chart, cfg), add aidchart in chart
	#define	aidAddCfg		9		// pnConfig message(chart), add aidchart in cfg
	#define	aidDelCfg		10		// pnConfig message(chart), delete aidchart in cfg
	#define	aidDelChart		11		// pnChart message(cfg), delete aidchart in chart
	#define	unitInput		12		// pnInput message(sise, chart), inputBar unit change
	#define	indexInput		13		// pnInput message(chart), inputBar index change
	#define	inputAutotran	14		// pnInput message(cfg, ctrl, chart, tool), inputBar autotran
	#define	gridChart		15		// pnChart message(Main, cfg), grid show/hide
	#define	gridCtrl		16		// pnCtrl message(Main, chart), grid show/hide
	#define	tipChart		17		// pnChart message(Main, cfg), tooltip show/hide
	#define	tipCtrl			18		// pnCtrl message(Main, cfg), tooltip show/hide
	#define	tableChart		19		// pnChart message(Main, cfg), table show/hide
	#define	tableCtrl		20		// pnChart message(Main, chart), table show/hide	
	#define	cfgMainChart	21		// pnConfig message(input, ctrl, chart), mainchart change
	#define	clearInput		22		// clear Data
	#define	BookMarkCfg		23		// pnConfig message(chart), bookmark change
	
	
#define	viewShowWindow		0x08		// Window Show Hide
	#define	showSise	1
	#define	showTool	2
	#define	showTable	3
	#define	showCtrl	4
	#define	showCfg		5
	#define	showAid		6

#define	viewInput			0x09			// control input information
	#define	getDisplayEdit		1	// get display edit's value 
	#define	setDisplayEdit		2	// get display edit's value 

#define	viewGroupList		0x0A		// select group list
					// kind  HIWORD(wParam), char* lParam	

// new added by kwon
#define	viewMulti	0x0B
	#define	codeUnit	0		// MTotal->MView, get data unit
	#define	codeInput	1		// pnInput message(gMView), inputBar code change
	#define	totalInput	2		// pnInput message(gMView), inputBar total change
	#define	total2Input	3		// pnInput message(gMView), inputBar multi total change
	#define	matrixInput	4		// pnInput message(gMView), inputBar new matrix
	#define	matrix2Input	5		// pnInput message(gMView), inputBar matrix change
	#define	selectInput	6		// pnInput message(gMView), inputBar select button
	#define	focusInput	7		// pnInput message(gMView), inputBar focus button 
	#define	orderInput	8		// pnInput message(gMView), inputBar order button 
	#define	todayInput	9		// pnInput message(gMView), inputBar today button 
	#define	activeView	10		// View Active(gMView, gMTotal), gMTotal<->gMView
	#define	activeInfo	11		// MView->MTotal, get Active Chart's Chart Info
	#define	inactiveView	12	// View inActive(gMView, gMTotal), gMTotal<->gMView
	#define	getCheckState	13	// MView->MTotal, get CheckBox state
	#define	setCheckState	14	// MView->MTotal, MTotal->MView, set CheckBox
	#define	giganInput		15	// MView->MTotal, gigan, gap change
	#define	mainInput	16		// Ctrl->MView->MTotal, get Main Chart Type
	#define	getRotCnt	17		// Bar->MView, get rotation chart count
	#define	getMultiInfo	18	// Input->MView, get MultiChartInfo
	#define	getMapCode	19		// MView->MTotal
	#define	savedCodeInput	20		// gMView->MTotal, init Saved code, no check unit 
	#define	mTotalTran		21		// gMView->MTotal, Request TR
	#define	onlyGigan		22		// gMView->MTotal, TR날리지 않고 일주..갭 변경
	#define setMultiCode	23		// pnInput message(gMView), automatic request by mutil code
									// lParam: CStringArray pointer
									// if array size if 0 then stop automatic request
	#define	autoTranUnit	24		// MView->MTotal, 자동 돌려보기시 unit
	#define	autoTranCode	25		// MView->MTotal, 자동 돌려보기시 code
	#define	autoComplete	26		// MTotal->MView, MView->CFG
	#define	initAllChart	27		// MView->MTotal, Initialize all chart
/////////////////////////////////////////////////////////////////////////
// GEV_INPUT
/////////////////////////////////////////////////////////////////////////
#define	INPUT_CHANGE	0x01	// change value
	#define	ipUnit		1	// int   lParam:unit
	#define	ipKKind		2	// int   lParam:업종 종류
	#define	ipSKind		3	// int   lParam:해외 종류
	#define	IP_CODE		4	// char* lParam:code
	#define	ipCodeArrow	5	// int   lParam:VK_UP,VK_DOWN
	#define	ipGap		6	// int   lParam:gap
	#define	ipDate		7	//
	#define	IP_GIGAN	8	// int   lParam:gigan
	#define	ipDisplay	9	// int   lParam:display
	#define	ipRotate	10	// int   lParam:time
	#define	ipTab		11	// int   lParam:id
	#define	INPUT_BUTTON	12	// int   lParam:id
	#define	ipCEdit		13	// ctrl's tab
	#define ipUnitCode	14	// lParam: unit;code
	#define ipSetDisplay 15	// lParam: set display days, set text only

#define	INPUT_DATA	0x02	// request data
	#define	GET_TR_INFO	1	// get TRsend Header info, char* return
	#define	getTRInfo2	2	// get TRsend Header info, char* return
	#define	GET_TOTAL_DAY	3	// get Totalday
	#define	getDispDay	4	// get DisplayDay
	#define	getCalendar	5	// get date
	#define	getCurGapList	6	// get Current Gaplist
					// char* lParam
	#define	setCodeInfo	7	// set info
					// char* lParam:code\tname 
	#define	domminoCode	8	// char* lParam:sym\tcode...
	#define	triggerCode	9	// char* lParam:sym\tcode
	#define	isDollarData	10	// int   return:bool
	#define	isTodayData	11	// int   return:bool
	#define	GET_VALID_CODE	12	// char* return:code
	#define	ONLY_SET_CODE	13
	#define	GET_TODAY	14
	#define SET_TOTAL_DAY	15
	#define GET_SERIES_KIND	16  // Getting Series Kind ==> GI_DAY, GI_WEEK, GI_MON, GI_MINUTE, GI_TICK
	#define firstCodeSet 17
	#define GET_CME			 18


#define	inputEnv	0x03	// argument control
	#define	inputMain	1	// change main chart, display enable/disable
					// int   lParam:kind
	#define	inputDisplay	2	// int   lParam:display day
	#define	inputReload	3	//
	#define	autotranCfg	4	// int   lParam:set/reset, cfg autotran
					// $$$$$
#define	inputViewFocus	0x04		// view focus
#define	INPUT_GET_POS_COMMENT	0x05	// LOWORD(lParam):x, HIWORD(lParam):y
					// char* return:comment
#define	inputInitForeign	0x06	// Foreign Chart initiate
#define	inputChgPalette		0x07	// change Palette

// added by kwon
#define	InputMulti		0x08
	#define inputSiseUpdate	1	// pnInput message(gMCtrl), sise update
	#define	getCheckBox		2	// get Checkbox state
	#define	setCheckBox		3	// set Checkbox state
	#define	inputIndex		4	// index change
	#define	GetMergeCtrl	5	// Get Merge Ctrl
	#define	GetTodayCheck	6	// Get Today CheckBox State
	#define	SetTodayCheck	7	// Set Today CheckBox State
	#define	ChangeChart		8	// Change Chart

/////////////////////////////////////////////////////////////////////////
// GEV_CHART
/////////////////////////////////////////////////////////////////////////
#define	chartChgPalette		0x01	// change Palette

#define	CHART_SET_GRP		0x02	// set graph
	#define	CHANGE_ENV_INFO	1	// char* lParam:envinfo
	#define	CHANGE_MAIN_GRP	2	// int   lParam:main kind
	#define	changeMapInfo	3	// _interChart* lParam
	#define	ADD_AID_GRP	4	// char* lParam:indicator
	#define	delAidGrp	5	// char* lParam:indicator
	#define	chartReload	6	// char* lParam:graph info
	#define	chartIReload	7	// char* lParam:graph info
	#define	chartBookMark	8	// char* lParam:ulist
//	#define	CHANGE_MAIN_GRP_NO_REQUEST	9	// int   lParam:main kind

#define	CHART_TR_DATA		0x03	// Transaction data, len == 0, clear
					// int*  HIWORD(wParam):length, char* lParam:data
#define	CHART_DATA		0x04	// chart data
	#define	REQUEST_HEADER	1	// Request Graph Header, char* return
					// int   lParam:length
	#define	REQUEST_HEADER2	2	// Request Graph Header, char* return
					// int   lParam:length
	#define	requestHeader3	3	// Request Graph Header, char* return
					// int   lParam:length
	#define	REALTIME_DATA	4	// Realtime Data
					// char* lParam:
	#define	REQUEST_CUR_ENV	5	// _envinfo
	#define	REQUEST_CUR_MAP	6	// map info
	#define	excelData	7	// excel
	#define	getTableData	8	// char* return:table data
	#define	getPrintTitle	9	// char* return:title
	#define	getSaveName	10	// char* return:name
	#define	chartClear	11	// data clear
	#define	requestCurInter	12	// _interChart* return
	#define	requestHeaderS	13	// single Request Graph Header, char* return
					// int   lParam:length

#define	CHART_ENV		0x05	// change env 
	#define	chartIsValid	1	// bool  return: valid state
	#define	chartIndex	2	// int   LOWORD(lParam):index, HIWORD(lParam):gap 
	#define	chartUnit	3	// int   lParam:unit
	#define	chartTotal	4	// int   lParam:total
	#define	CHART_DISPLAY	5	// int   lParam:display
	#define	chartScale	6	// int   lParam:scale
					// int return:dispDay
	#define	chartTable	7	// bool  lParam: table state
	#define	chartGrid	8	// bool  lParam: grid state
	#define	chartTip	9	// bool  lParam: grid state

#define	chartCtrl		0x06
	#define	getScrollData	1	// get scroll data, return data len, lParam:data reference
	#define	changePosition	2	// change pos, LOWORD(lParam) : dispPos, HIWORD(lParam) : dispEnd
	#define	changeShift	3	// change shift, lParam:shift
	#define	chartAutoScroll	4	// autoscroll, lParam:TRUE(autoscroll),FALSE(not)
	#define	isAutoScroll	5	// is autoscroll?
	#define	chartToolSelect	6	// tool select, lParam:select
	#define	chartToolColor	7	// int	lParam:RGB
	#define	chartToolWidth	8	// int	lParam:두께
	#define	chartExcel	9	// excel
	#define	chartAutotran	10	// auto transaction
	#define	chartUpdate	11	// bitmap bitblt
	#define	patternChanged	12	// Pattern tool status changed, LOWORD(lParam) = 0: disabled, other: enabled

#define	chartViewFocus		0x07	// view focus
#define	chartToolTip		0x08	// tooltip
					// HIWORD(wParam) : bool, lParam:char*
#define	CHART_MENU		0x09	// tooltip menu 
	#define	ADD_CHART_MENU	1	// add chart menu
	#define	selChartMenu	2	// select chart menu
					// LOWORD:menuID, HIWORD:aKind
#define	CHART_TR_DATAS		0x0A	// single Transaction data, len == 0, return
					// int*  HIWORD(wParam):length, char* lParam:data
// 2006.11.20 lsh (start)
#define	CHART_USER_SELECT	0x0B	// chart user selection(Map과 연동하는 부분)
	#define getSelectTime	1	// return (char*)datetime : "20061120:082000"
	#define	setSelectTime	2	// char* lparam: "20061120:082000"
	#define getSelectPrice	3	// int LOWORD(lParm):index, char* return price
	#define	StartOrderMode	4	// Start Order mode 
// 2006.11.20 lsh (end)

//** add by macho@20061226 <start>
#define CHART_STANLINE		0x0C	// stan-line information 
	#define	setStanLineDateTime	0x01	// set stan-line datetime value
	#define setStanLineWidth	0x02	// set stan-line line width
	#define setStanLineColor	0x03	// set stan-line line color
	#define stanLineDelete		0x04	// delete current stan-line
	#define getStanLineDateTime	0x05	// get stan-line datetime value
	#define getStanLineWidth	0x06	// get stan-line line width
	#define getStanLineColor	0x07	// get stan-line line color
//** add by macho@20061226 <end>

//** add by macho@20070102 <start>
#define CHART_ANAL			0x0D	// analize area selecting mode.
	#define	setAnalMode			0x01	// set analize area selecting mode: wParam = 0: not anal-mode, other: anal-mode
	#define getAnalMode			0x02	// get current anlalize area selecting mode: return value is 0 then not anal-mode, other then anal-mode
	#define	setMessageHandler	0x03	// set notify message handler window: lParam = CWnd* messageHandler
	#define setMessageID		0x04    // set notify message id: lParam = messageID
	#define	analAreaSelected	0x05	// user selected analize area: messageHandler->SendMessage(messageID, analAreaSelected, (LPANALAREA)lParam)
	#define analAreaDeleted		0x06	// user deleted analize area: messageHandler->SendMessage(messageID, analAreaDeleted)
	#define	initAnalMode		0x07	// initialize anal mode

typedef struct {
	struct {
		short	yy;
		BYTE	mm;
		BYTE	dd;
	} date;				// date of price
	struct {
		BYTE	hh;
		BYTE	mm;
		BYTE	ss;
	} time;				// time of price
	double open;		// open price
	double high;		// high price
	double low;			// low price
	double close;		// close price
	double volume;		// volume
} ANALPRICE;

typedef struct {
	char	code[12];	// current code
	int		count;		// count of days
	ANALPRICE data[1];  // pointer of ANALPRICE array
} ANALAREA, *LPANALAREA;

//** add by macho@20070102 <end>
/////////////////////////////////////////////////////////////////////////
// GEV_SISE
/////////////////////////////////////////////////////////////////////////
#define	siseChgPalette		0x01	// change Palette
#define	SISE_TR_DATA		0x02	// Transaction data, len == 0, clear
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
	#define	siseName	3	// char* return:name
	#define	siseDate	4	// 영업일자
					// char* return:date
	#define	sisePivot	5	// 기준값 
					// char* return:상한\t하한\t전일종가
	#define	siseStock	6	// 주식수
	#define	siseDollar	7	// convert into dollar data
	#define	siseClear	8	// clear sise
					// char* return:stock
#define	siseFlash		0x06	// HIWORD(WPARAM) == 1, blink otherwise not blink
					// flash : blink color palette index
#define	siseFont		0x07	// 
#define	siseChgEnv		0x08	// RGB    lParam:background color


/////////////////////////////////////////////////////////////////////////
// GEV_CTRL
/////////////////////////////////////////////////////////////////////////
#define	CTRL_SCROLL_POS		0x01	// scroll pos
					// HIWORD(wParam) : nSBCode, lParam : pos
#define	CTRL_GET_POS_COMMENT	0x02	// LOWORD(lParam):x, HIWORD(lParam):y
					// char* return:comment
#define	CTRL_ENV			0x03	// argument control
	#define	ctrlMain	1	// change main chart, display enable/disable
					// int   lParam:kind
	#define	ctrlDisplay	2	// int   lParam:display day
	#define	ctrlEnable	3	// bool  lParam:enable state
	#define	ctrlAutotran	4	// bool  lParam:set state
	#define	ctrlGrid	5	// bool  lParam:Grid state
	#define	ctrlTable	6	// bool  lParam:Table state
	#define	ctrlReload	7	//
	#define	ctrlUnit	8
	#define	ctrlIndex	9
	#define	ctrlClear	10	// clear ctrl
	#define	CTRL_SET_EDIT	11	// set display day
	#define	ctrlGetEdit	12	// get display day
	#define	ctrlChgDisp	13	// edit -> ctrl
	#define	ctrlETab	14
	#define	ctrlITab	15	// input's tab	
	#define	ctrlTip		16	// bool  lParam:tooltip state
	#define	CTRL_UPDATE_SLIDE	17

// new added by kwon
#define	CtrlMulti	0x04
	#define	CtrlInit	1

/////////////////////////////////////////////////////////////////////////
// GEV_TOOL
/////////////////////////////////////////////////////////////////////////
#define	TOOL_GET_POS_COMMENT	1	// LOWORD(lParam):x, HIWORD(lParam):y
					// char* return:comment
#define	TOOL_RELEASE		2	// release tool button
#define	TOOL_CHECK_RECT		3	// check lbuttondown is toolRect
					// int	return:id
#define	TOOL_GET_SELECT_TOOL	4	// chart -> select tool button
#define	TOOL_AUTOTRAN		5	// view -> autotran
#define	TOOL_CONFIG		6	// 
#define	TOOL_MAP_CLICK_EVENT	7	// send click event to map // 2006.11.20 lsh
#define TOOL_PATTERN			8		// pattern status get/set
	#define getPatternUPEnabled	0x01	// get pattern status, lResult= 0: disabled other: enabled
	#define setPatternUPEnabled	0x02	// set pattern status, lParam = 0: disbla	other: enable
	#define getPatternDNEnabled	0x03	// get pattern status, lResult= 0: disabled other: enabled
	#define setPatternDNEnabled	0x04	// set pattern status, lParam = 0: disbla	other: enable


//-----------------------------------------------------------------------
//
// GEV_AID
//
//-----------------------------------------------------------------------
#define	AID_GET_POS_COMMENT	1
#define	AID_CONFIG		2




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

#define	IJ_CODE		"1301"	// input code, 주식
#define	II_CODE		"21301"	// input code, 업종
#define	IF_CODE		"31301"	// input code, 선물
#define	IO_CODE		"41301"	// input code, 옵션
#define	IS_CODE		"61301"	// input code, 주식선물
#define	IC_CODE		"71301" // input code, 주식옵션
#define ICME_CODE    "121301" // input code, CME

/////////////////////////// 시세 마스터

#define	OJ_CODE		"1021"	// original code, RTS code
#define	OJ_NAME		"1022"	// 한글명
#define	OJ_JGGA		"1023"	// 종가(현재가)
#define	OJ_DIFF		"1024"	// 전일대비
#define	OJ_MDHG		"1025"	// 매도호가
#define	OJ_MSHG		"1026"	// 매수호가
#define	OJ_GVOL		"1027"	// 거래량
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
#define	OJ_GUBN		"1901"	// 소속구분(kospi,kosdaq,제3시장)

#define	OI_CODE		"21021"	// original code, RTS code
#define	OI_NAME		"21022"	// 한글명
#define	OI_JGGA		"21023"	// 종가(현재가)
#define	OI_DIFF		"21024"	// 전일대비
#define	OI_MDHG		"21025"	// 매도호가
#define	OI_MSHG		"21026"	// 매수호가
#define	OI_GVOL		"21027"	// 거래량
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
#define	OF_MGJY		"31201"	// 미결제 약정수량

#define	OO_CODE		"41021"	// original code, RTS code
#define	OO_NAME		"41022"	// 한글명
#define	OO_JGGA		"41023"	// 종가(현재가)
#define	OO_DIFF		"41024"	// 전일대비
#define	OO_MDHG		"41025"	// 매도호가
#define	OO_MSHG		"41026"	// 매수호가
#define	OO_GVOL		"41027"	// 거래량
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
#define	OO_MGJY		"41201"	// 미결제 약정수량

#define	OS_CODE		"61021"	// original code, RTS code
#define	OS_NAME		"61022"	// 한글명
#define	OS_JGGA		"61023"	// 종가(현재가)
#define	OS_DIFF		"61024"	// 전일대비
#define	OS_MDHG		"61025"	// 매도호가
#define	OS_MSHG		"61026"	// 매수호가
#define	OS_GVOL		"61027"	// 거래량
#define	OS_SIGA		"61029"	// 시가
#define	OS_KOGA		"61030"	// 고가
#define	OS_JEGA		"61031"	// 저가
#define	OS_RATE		"61033"	// 등락율
#define	OS_TIME		"61034"	// 체결시간
#define	OS_CDAY		"61310"	// 당일자(최종영업일), yyyymmdd
#define	OS_SAHG		"61311"	// 상한가
#define	OS_HAHG		"61312"	// 하한가
#define	OS_JJGA		"61313"	// 전일종가
#define	OS_STCK		"61000"	// 상장주식수
#define	OS_MGJY		"61201"	// 미결제 약정수량

#define	OC_CODE		"71021"	// original code, RTS code
#define	OC_NAME		"71022"	// 한글명
#define	OC_JGGA		"71023"	// 종가(현재가)
#define	OC_DIFF		"71024"	// 전일대비
#define	OC_MDHG		"71025"	// 매도호가
#define	OC_MSHG		"71026"	// 매수호가
#define	OC_GVOL		"71027"	// 거래량
#define	OC_SIGA		"71029"	// 시가
#define	OC_KOGA		"71030"	// 고가
#define	OC_JEGA		"71031"	// 저가
#define	OC_RATE		"71033"	// 등락율
#define	OC_TIME		"71034"	// 체결시간
#define	OC_CDAY		"71310"	// 당일자(최종영업일), yyyymmdd
#define	OC_SAHG		"71311"	// 상한가
#define	OC_HAHG		"71312"	// 하한가
#define	OC_JJGA		"71313"	// 전일종가
#define	OC_STCK		"71000"	// 상장주식수
#define	OC_MGJY		"71201"	// 미결제 약정수량

#define	ON_CODE		"121021"	// original code, RTS code
#define	ON_NAME		"121022"	// 한글명
#define	ON_JGGA		"121023"	// 종가(현재가)
#define	ON_DIFF		"121024"	// 전일대비
#define	ON_MDHG		"121025"	// 매도호가
#define	ON_MSHG		"121026"	// 매수호가
#define	ON_GVOL		"121027"	// 거래량
#define	ON_SIGA		"121029"	// 시가
#define	ON_KOGA		"121030"	// 고가
#define	ON_JEGA		"121031"	// 저가
#define	ON_RATE		"121033"	// 등락율
#define	ON_TIME		"121034"	// 체결시간
#define	ON_CDAY		"121310"	// 당일자(최종영업일), yyyymmdd
#define	ON_SAHG		"121311"	// 상한가
#define	ON_HAHG		"121312"	// 하한가
#define	ON_JJGA		"121313"	// 전일종가
#define	ON_STCK		"121000"	// 상장주식수
#define	ON_MGJY		"121201"	// 미결제 약정수량


// 데이터 부분
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

#define	DJM_DAY		"4302"
#define	DJM_HMS		"4034"
#define	DJM_SIGA	"4029"
#define	DJM_KOGA	"4030"
#define	DJM_JEGA	"4031"
#define	DJM_JGGA	"4023"
#define	DJM_GVOL	"4027"
#define	DJM_CVOL	"4032"
#define	DJM_GAMT	"4028"

#define	DJD_DAY		"5302"
#define	DJD_HMS		"5034"
#define	DJD_SIGA	"5029"
#define	DJD_KOGA	"5030"
#define	DJD_JEGA	"5031"
#define	DJD_JGGA	"5023"
#define	DJD_GVOL	"5027"
#define	DJD_CVOL	"5032"
#define	DJD_GAMT	"5028"
#define	DJD_LOCK	"5311"	// 권리락등의 정보 플래그
#define	DJD_RATE	"5310"	// 수정 비율 (수정주가)
#define	DJD_DOL		"5320"	// dollar

#define INV_FRGNHAVE "5363"
#define INV_FRGNRATE "5373"
#define INV_PSNLPBUY "5408"
#define INV_FRGNPBUY "5411"
#define INV_ORGNPBUY "5412"
#define INV_PSNLTBUY "5508"
#define INV_FRGNTBUY "5511"
#define INV_ORGNTBUY "5512"

#define IINV_PSNLPBUY "25408"
#define IINV_FRGNPBUY "25411"
#define IINV_ORGNPBUY "25412"
#define IINV_PSNLTBUY "25508"
#define IINV_FRGNTBUY "25511"
#define IINV_ORGNTBUY "25512"



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

#define	DIM_DAY		"24302"
#define	DIM_HMS		"24034"
#define	DIM_SIGA	"24029"
#define	DIM_KOGA	"24030"
#define	DIM_JEGA	"24031"
#define	DIM_JGGA	"24023"
#define	DIM_GVOL	"24027"
#define	DIM_CVOL	"24032"
#define	DIM_GAMT	"24028"

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
#define	DFT_MGJY	"32201"	// 미결제 약정

#define	DFM_DAY		"34302"
#define	DFM_HMS		"34034"
#define	DFM_SIGA	"34029"
#define	DFM_KOGA	"34030"
#define	DFM_JEGA	"34031"
#define	DFM_JGGA	"34023"
#define	DFM_GVOL	"34027"
#define	DFM_CVOL	"34032"
#define	DFM_GAMT	"34028"
#define	DFM_MGJY	"34201"

#define	DFD_DAY		"35302"
#define	DFD_HMS		"35034"
#define	DFD_SIGA	"35029"
#define	DFD_KOGA	"35030"
#define	DFD_JEGA	"35031"
#define	DFD_JGGA	"35023"
#define	DFD_GVOL	"35027"
#define	DFD_CVOL	"35032"
#define	DFD_GAMT	"35028"
#define	DFD_MGJY	"35201"

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
#define	DOT_MGJY	"42201"	// 미결제 약정

#define	DOM_DAY		"44302"
#define	DOM_HMS		"44034"
#define	DOM_SIGA	"44029"
#define	DOM_KOGA	"44030"
#define	DOM_JEGA	"44031"
#define	DOM_JGGA	"44023"
#define	DOM_GVOL	"44027"
#define	DOM_CVOL	"44032"
#define	DOM_GAMT	"44028"
#define	DOM_MGJY	"44201"

#define	DOD_DAY		"45302"
#define	DOD_HMS		"45034"
#define	DOD_SIGA	"45029"
#define	DOD_KOGA	"45030"
#define	DOD_JEGA	"45031"
#define	DOD_JGGA	"45023"
#define	DOD_GVOL	"45027"
#define	DOD_CVOL	"45032"
#define	DOD_GAMT	"45028"
#define	DOD_MGJY	"45201"

///////////////////////////주식선물 시세
#define	DST_SYM		"62500"
#define	DSM_SYM		"64500"
#define	DSD_SYM		"65500"

#define	DST_DAY		"62302"	// 일자
#define	DST_HMS		"62034"	// 시간
#define	DST_SIGA	"62029"	// 시가
#define	DST_KOGA	"62030"	// 고가
#define	DST_JEGA	"62031"	// 저가
#define	DST_JGGA	"62023"	// 종가
#define	DST_GVOL	"62027"	// 거래량
#define	DST_CVOL	"62032"	// 체결량
#define	DST_GAMT	"62028"	// 거래대금
#define	DST_MGJY	"62201"	// 미결제 약정

#define	DSM_DAY		"64302"
#define	DSM_HMS		"64034"
#define	DSM_SIGA	"64029"
#define	DSM_KOGA	"64030"
#define	DSM_JEGA	"64031"
#define	DSM_JGGA	"64023"
#define	DSM_GVOL	"64027"
#define	DSM_CVOL	"64032"
#define	DSM_GAMT	"64028"
#define	DSM_MGJY	"64201"

#define	DSD_DAY		"65302"
#define	DSD_HMS		"65034"
#define	DSD_SIGA	"65029"
#define	DSD_KOGA	"65030"
#define	DSD_JEGA	"65031"
#define	DSD_JGGA	"65023"
#define	DSD_GVOL	"65027"
#define	DSD_CVOL	"65032"
#define	DSD_GAMT	"65028"
#define	DSD_MGJY	"65201"

///////////////////////////주식옵션 시세
#define	DCT_SYM		"72500"
#define	DCM_SYM		"74500"
#define	DCD_SYM		"75500"

#define	DCT_DAY		"72302"	// 일자
#define	DCT_HMS		"72034"	// 시간
#define	DCT_SIGA	"72029"	// 시가
#define	DCT_KOGA	"72030"	// 고가
#define	DCT_JEGA	"72031"	// 저가
#define	DCT_JGGA	"72023"	// 종가
#define	DCT_GVOL	"72027"	// 거래량
#define	DCT_CVOL	"72032"	// 체결량
#define	DCT_GAMT	"72028"	// 거래대금
#define	DCT_MGJY	"72201"	// 미결제 약정

#define	DCM_DAY		"74302"
#define	DCM_HMS		"74034"
#define	DCM_SIGA	"74029"
#define	DCM_KOGA	"74030"
#define	DCM_JEGA	"74031"
#define	DCM_JGGA	"74023"
#define	DCM_GVOL	"74027"
#define	DCM_CVOL	"74032"
#define	DCM_GAMT	"74028"
#define	DCM_MGJY	"74201"

#define	DCD_DAY		"75302"
#define	DCD_HMS		"75034"
#define	DCD_SIGA	"75029"
#define	DCD_KOGA	"75030"
#define	DCD_JEGA	"75031"
#define	DCD_JGGA	"75023"
#define	DCD_GVOL	"75027"
#define	DCD_CVOL	"75032"
#define	DCD_GAMT	"75028"
#define	DCD_MGJY	"75201"


///////////////////////////야간선물 시세
#define	CMT_SYM		"122500"
#define	CMM_SYM		"124500"
#define	CMD_SYM		"125500"

#define	CMT_DAY		"122302"	// 일자
#define	CMT_HMS		"122034"	// 시간
#define	CMT_SIGA	"122029"	// 시가
#define	CMT_KOGA	"122030"	// 고가
#define	CMT_JEGA	"122031"	// 저가
#define	CMT_JGGA	"122023"	// 종가
#define	CMT_GVOL	"122027"	// 거래량
#define	CMT_CVOL	"122032"	// 체결량
#define	CMT_GAMT	"122028"	// 거래대금
#define	CMT_MGJY	"122201"	// 미결제 약정

#define	CMM_DAY		"124302"
#define	CMM_HMS		"124034"
#define	CMM_SIGA	"124029"
#define	CMM_KOGA	"124030"
#define	CMM_JEGA	"124031"
#define	CMM_JGGA	"124023"
#define	CMM_GVOL	"124027"
#define	CMM_CVOL	"124032"
#define	CMM_GAMT	"124028"
#define	CMM_MGJY	"124201"

#define	CMD_DAY		"125302"
#define	CMD_HMS		"125034"
#define	CMD_SIGA	"125029"
#define	CMD_KOGA	"125030"
#define	CMD_JEGA	"125031"
#define	CMD_JGGA	"125023"
#define	CMD_GVOL	"125027"
#define	CMD_CVOL	"125032"
#define	CMD_GAMT	"125028"
#define	CMD_MGJY	"125201"
///////////////////////////////////////////////////////////

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


#endif

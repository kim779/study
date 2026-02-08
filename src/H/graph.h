///****************************************************************************
// (C) COPYRIGHT Winix Information Co., Ltd. 2001,2003
// All Rights Reserved
// Licensed Materials - Property of Winix Information

// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic, 
// mechanical, or otherwise without the written permission of Winix Information.

// Components   : graph.h	AXIS/WS GRAPH information
//  Rev. History :
//  	  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 2.0	2001-04	AXIS Version II
///****************************************************************************

#pragma once
#pragma pack(1)

#define GD_UNUSED	-999999		// unused data
#define	GV_UNUSED	0		// unused value
#define	maxGRAPH	8		//
#define	maxAVLINE	5		//
#define	maxGEXGRP	15

#define	magicBAS	200
#define magicKEY	10
#define magicMAX	5
#define	sz_FNAME	8
#define	sz_FTITLE	40

#define	sz_GMAPH	sizeof(struct _gmapH)

struct	_region {
	unsigned char	key{};			// region key ID
	unsigned char	left{};			// left position
	unsigned char	top{};			// top position	
	unsigned char	right{};			// right position
	unsigned char	bottom{};			// bottom position
};

#define	sz_REGION	sizeof(struct _region)

struct	_graph {	// save format graph
	unsigned char	type{};			// graph type
	unsigned char	kind{};			// graph kind
	unsigned char	tick{};			// tick type

	unsigned char	key{};			// overlap graph key
	unsigned char	region{};			// region ID
	DWORD		fgcolor{};		// graph foreground color
	DWORD		bgcolor{};		// graph background color => not used
	WORD		value[maxAVLINE]{};	// information value, 이동평균일/기준값

	WORD		itick{};			// information tick
	char		iname[32]{};		// information name
	unsigned char	idigit{};			// 소수점 유효자리수
	WORD		ichkcolor{};		// icolor checkbit
	DWORD		icolor[maxAVLINE]{};	// color value, 이동평균일수, 특성차트 색상
	unsigned char	ApplyFormula{};		// formula select
	WORD		iline{};			// line information
	unsigned char	iltype[maxAVLINE]{};	// line type
	char		iaddition{};		// additional graph information
	char		reserve[30]{};		// reserved byte for future
};

#define	sz_GRAPH	sizeof(struct _graph)

//
//	graph.type can specify one of the following values
//
#define	GT_BONG			1		// 봉 차트
#define	GT_LINE			2		// Line 차트
#define	GT_VARS			3		// 특성 차트
#define	GT_USER			4		// 사용자 추가 개발차트

//
//	graph.kind/dataH.gkind can specify one of the following values
///

//	graph.type == GT_BONG
#define	GK_JPN			1		// 일본식 봉
#define	GK_LOG			2		// 로그차트
#define	GK_USA			3		// 미국식 봉
#define	GK_VOL			4		// 거래량
#define	GK_LIN			5		// 라인타입 봉
#define	GK_ENVL			6		// ENVELOPE
#define	GK_PARB			7		// PARABOLIC
#define	GK_BOLB			8		// BOLLINGER BAND
#define	GK_BALANCE		9		// 일목균형표
#define	GK_AMT			10		// 거래대금차트

#define	GK_LG_PMA		11		// 가격이평차트
#define	GK_LG_LMA		12		// 로그가격이평
#define	GK_LG_VMA		13		// 거래이평차트
#define	GK_LG_AMA		14		// 대금이평차트
#define	GK_LG_LIN		15		// only Line
#define	GK_LG_LOG		16		// only Log Line
#define	GK_LG_ENVL		17		// Envelope Line
#define	GK_LG_PARB		18		// Parabolic Line
#define	GK_LG_BOLB		19		// Bollinger Line


//	graph.type == GT_LINE
#define	GK_ADLINE		30		// A.D Line
#define	GK_ADX			31		// ADX
#define	GK_OBV			32		// OBV
#define	GK_CCI			33		// CCI
#define	GK_CLOCK		34		// 역시계곡선
#define	GK_DMI			35		// DMI
#define	GK_GAP			36		// 이격도
#define	GK_LFI			37		// LFI
#define	GK_MACD			38		// MACD
#define	GK_MACDOS		39		// MACD OSCILLATOR
#define	GK_NCO			40		// NCO
#define	GK_NPSY			41		// 신심리도
#define	GK_PMAO			42		// 가격 MAO
#define	GK_PSY			43		// 심리도(계단식)
#define	GK_PV			44		// PV 차트
#define	GK_RRATE		45		// 회전율(주식수)
#define	GK_RSI			46		// RSI
#define	GK_SIGMA		47		// SIGMA
#define	GK_SONA			48		// SONA
#define	GK_SONA2		49		// SONA2
#define	GK_SONA2OS		50		// SONA2 OSCILLATOR
#define	GK_STOCH		51		// STOCHASTIC SLOW
#define	GK_STOCH1		52		// STOCHASTIC(동원)
#define	GK_STOCH2		53		// STOCHASTIC FAST
#define	GK_VMAO			54		// 거래량 MAO
#define	GK_VR			55		// VR
#define	GK_WPR			56		// WILLIAM %R
#define	GK_PIVOT		57		// 피봇분석
#define	GK_DEMARK		58		// 디마크 지표
#define	GK_ROC			59		// Price Rate of Change
#define	GK_MOMENTUM		60		// 모멘텀 지표
#define	GK_ABRATIO		61		// AB Ratio
#define	GK_TRIX			62		// Trix
#define	GK_MFI			63		// Money Flow Index

#define	GK_ATR			66		// ATR(Average True Range)
#define	GK_AND			67		// Accumulation/Distribution
#define	GK_CO			68		// Chaikin's Oscillator
#define	GK_CV			69		// Chaikin's Volatility
#define	GK_MI			70		// Mass Index
#define	GK_NVI			71		// Negative Volume Index
#define	GK_OSCV			72		// Volume Oscillator
#define	GK_WC			73		// Weighted Close
#define	GK_WAD			74		// William A/D
#define	GK_CSI			75		// 체결강도차트	-> only tick chart

//	graph.type == GT_VARS
#define	GK_MKPRO		151		// MARKET PROFILE
#define	GK_PNF			152		// P&F 차트	
#define	GK_RADAR_VAL		153		// RADAR 차트
#define	GK_RADAR_NMVAL		154		// RADAR 차트
#define	GK_RVOL			155		// n주간 업종거래량비중
#define	GK_SWING		156		// SWING 챠트
#define	GK_TURN			157		// 삼선전환도
#define	GK_EQV			158		// Equi-volume chart
        
//	graph.type == GT_USER
#define	GK_LINE_VAL		205		// 라인 차트
#define	GK_LINE_VIDX		206		// 라인 차트			->real
#define	GK_2DLINE		207		// 2-D 라인 차트
#define	GK_STEP_VAL		208		// 계단식 차트(val)
#define	GK_STEP_VIDX		209		// 계단식차트(vidx)		->real
#define	GK_STEP_RATIO		210		// 계단식(신용등급)차트(val)
#define	GK_BARV1_VAL		211		// 막대 차트(가격별)
#define	GK_BARV1_NMVAL		212		// 막대 차트(가격별)
#define	GK_BARV2_NBTK		213		// 막대 차트(가격대별)
#define	GK_BARV2_BTK		214		// 막대 차트(가격대별)
#define	GK_VBAR			215		// 수직막대 차트(val)
#define	GK_VBAR_		216		// 수직막대 차트(val, FROM ~ TO)
#define	GK_VBAR2		217		// 이중 막대 UP/DOWN(val,val)
#define	GK_VBAR_TK		218		// 수직막대차트(nmvl, 틱)
#define	GK_VBAR_REV		219		// 수직막대차트(vidx, 역방향)	->real
#define	GK_VBAR_VIDX		220		// 수직막대 차트(vidx)		->real
#define	GK_XBAR_NMVAL		221		// 0 BASE 수평UP/DOWN 막대
#define	GK_XVBAR_VAL		222		// 0 BASE 수직UP/DOWN 막대
#define	GK_XVBAR_VIDX		223		// 0 BASE 수직UP/DOWN 막대	->real
#define	GK_XVBAR_NMVAL		224		// 0 BASE 수직UP/DOWN 막대
#define	GK_FULLPIE		225		// 파이 차트(nmval)
#define	GK_SPLITPIE		226		// 조각 파이 차트(nmval)

//	graph.tick can be any combination of the following values.
#define	TK_LEFT			0x0001		
#define	TK_RIGHT		0x0002		
#define	TK_BOTTOM		0x0004		
#define	TK_HGRID		0x0010		// display horizontal grid
#define	TK_VGRID		0x0020		// display vertical grid
#define	TK_VOL			0x0040		// don't show adjust-volume data but raw-volume data
#define	TK_NOUNIT		0x0080		// does not display unit

//
//	graph.chkcolor can be any combination of the following values.
//
#define	CU_ICOLOR1		0x0001		// graph.icolor[0]
#define	CU_ICOLOR2		0x0002		// graph.icolor[1]
#define	CU_ICOLOR3		0x0004		// graph.icolor[2]
#define	CU_ICOLOR4		0x0008		// graph.icolor[3]
#define	CU_ICOLOR5		0x0010		// graph.icolor[4]

//
//	graph.itick can be any combination of the following values.
//
#define	IT_NOP			0x0000		// no guide
#define	IT_LEFT			0x0001		// left
#define	IT_RIGHT		0x0002		// right
#define	IT_LEFT_		0x0003		// inside left
#define	IT_RIGHT_		0x0004		// inside right

#define	IT_UNIT			0x0010		// user defined
#define	IT_2MAX			0x0020		// Max is extended 2multiple(using in User chart)
#define	IT_3MAX			0x0040		// Max is extended 3multiple(using in User chart)
#define	IT_4MAX			0x0080		// Max is extended 3multiple(using in User chart)


struct	_tick {
	unsigned char	region{};			// tick region
	unsigned char	scale{};			// font scale
	DWORD		color{};			// tick color
};

#define	sz_TICK		sizeof(struct _tick)

//
//	graph.ApplyFormula can be any combination of the following values.
//
#define	FO_AVERAGE		0x10		// apply average volume or amt 

//
//	graph.iline can be any combination of the following values.
//	=> used when user saved file in local
#define	GL_TYPE1		0x0001		//	first line is bold
#define	GL_TYPE2		0x0002		//	second line is bold
#define	GL_TYPE3		0x0004		//	third line is bold
#define	GL_TYPE4		0x0008		//	fourth line is bold
#define	GL_TYPE5		0x0010		//	fifth line is bold

struct	_gexgrp{
	int	type{};
	int	kind{};
	WORD	value[maxAVLINE]{};
	WORD	chkcolor{};
	DWORD	color[maxAVLINE]{};
	unsigned char	selline{};
	unsigned char	ltype[maxAVLINE]{};
	char	reserve[32]{};
};

#define	sz_GEXGRP	sizeof(struct _gexgrp)

struct	_gmapH{
	BYTE		vers{};			// version
	BYTE		absS{};			// absolute security
	BYTE		incS[4]{};		// inclusive security
	BYTE		excS[4]{};		// exclusive security

	char		title[sz_FTITLE]{};
	unsigned char	unit{};
	unsigned char	index{};
	char		gap[3]{};			// 분봉(GI_HMS)에서의 간격
						// 9vv : vv초
						// vvv : vvv분
	struct _gexgrp	gexgrp[maxGEXGRP]{};	// 종합차트의 메인차트정보
	char		aidgrp[64]{};		// 종합차트의 보조차트순서정보
	char		option{};			//
	char		reserve[63]{};
};

#define	sz_GMAPH	sizeof(struct _gmapH)
//
//	fgraph.option can be any combination of the following values.
//
#define	GEX_NOTOTAL	0x01	// single chart

struct	_fgraph {	// save graph format 
	char		name[16]{};		// graph object name
	char		date[4]{};		// display day
	WORD		update{};			// update info
	DWORD		bgcolor{};		// 바탕색
	DWORD		fgcolor{};		// 테두리색
	DWORD		adcolor{};		// 보조선색(grid color)

	struct	_tick	xtick{};			// x-tick info
	struct	_tick	ltick{};			// left y-tick info
	struct	_tick	rtick{};			// right y-tick info
	
	struct	_region	region[maxGRAPH]{};	//
	struct	_graph	graph[maxGRAPH]{};	//
	DWORD		operation{};		// client operation
	DWORD		upcolor{};		// up bong color(only RGB is valid)
	DWORD		dncolor{};		// down bong color(only RGB is valid)
	char		count[4]{};		// data number from server
	unsigned char	defgraph{};		// predefined graph
	char		reserve[15]{};		// reserved byte for future	
};

#define	sz_FGRAPH	sizeof(struct _fgraph)

struct _gexmap	{	// singleview chart format
	struct _gmapH	gmapH{};
	struct _fgraph	fgraph{};
};
#define	sz_GEXMAP	sizeof(struct _gexmap)

//
//	fgraph.update can be any combination of the following values.
//
#define	GF_COM			0x0001		// compressed display
						// ignored FORM.date
#define	GF_SCR			0x0002		// scroll display
#define	GF_NMG			0x0004		// no margin (for bongtype)
#define	GF_GRID			0x0008		// grid usable 
#define	GF_RTM			0x0080		// realtime update
#define	GF_EDT			0x0040		// user editable -> Right MouseMenu, ...
#define	GF_NSP			0x0020		// don't display split 
#define	GF_TICK			0x0010		// tick chart

//
//	fgraph.operation can be any combination of the following values
//
#define	GO_OVRLAP		0x00000001	// overlap graph
#define	GO_REVGRP		0x00000002	// reverse graph
#define	GO_NOGRAD		0x00000004	// not gradient mode
#define	GO_NOGVIEW		0x00000010	// not grid view
#define	GO_CHIDE		0x00000020	// hide crossline 
#define	GO_JTRACE		0x00000040	// jgga trace

#define	GO_JPNBONG		0x00000100	// japan bong
#define	GO_USABONG		0x00000200	// usa bong
#define	GO_LINEGRP		0x00000400	// line type
#define	GO_LOGGRP		0x00000800	// log type
#define	GO_MODGRP		0x00001000	// modified jugga
#define	GO_DUPGRP		0x00002000	// duplicated graph

#define	GO_NOUPFILL		0x00010000	// don't fullfill up bong
#define	GO_NODNFILL		0x00020000	// don't fullfill down bong
#define	GO_NOMINMAX		0x00040000	// don't display min/max

//
//	fgraph.defgraph can specify one of the following values
//
#define	DEF_NONE	0
#define	DEF_LOG		GK_LOG
#define	DEF_ENVL	GK_ENVL
#define	DEF_BOLB	GK_BOLB
#define	DEF_PARB	GK_PARB
#define	DEF_BALANCE	GK_BALANCE
#define	DEF_TURN	GK_TURN
#define	DEF_PNF		GK_PNF
#define	DEF_CLOCK	GK_CLOCK
#define	DEF_PV		GK_PV
#define	DEF_MKPRO	GK_MKPRO
#define	DEF_SALE	100	// 매물차트 graph.kind와 중복안되게


struct	_ugraphH {	// Upload graph header
	unsigned char	ndat{};			// # of dataH
	unsigned char	info1{};			
	unsigned char	info2{};
	char		reserve[32]{};		// reserved byte for future
};

#define	sz_UGRAPHH	sizeof(struct _ugraphH)

//
//	ugraph.info1 can be any combination of the following values.
//
#define	GUI_MOD		0x01	// 수정주가

struct	_dgraphH {	// Download graph header
	unsigned char	ndat{};			// # of dataH
	unsigned char	unit{};			// data unit
	unsigned char	index{};			// data index
	unsigned char	update1{};		// developer option
	unsigned char	update2{};		//
	char		reserve[32]{};		// reserved byte for future
};

#define	sz_DGRAPHH	sizeof(struct _dgraphH)

//
//	dgraphH.index can specify one of the following values
//
#define	GI_DAY			21		// 일
#define	GI_WEEK			22		// 주
#define	GI_MON			23		// 월
#define	GI_YEAR			24		// 년
#define	GI_HMS			25		// 분

//
// dgraph.update1 can be any combination of the following values
//
#define	DF_COM			0x0001		// compressed display
#define	DF_NRTM			0x0002		// not realtime
#define	DF_NOMINMAX		0x0004		// don't display min/max
#define	DF_NOFRAME		0x0008		// don't draw frame box

#define	DF_FPOS			0x0010		// foreign possession
#define	DF_FTRD			0x0020		// foreign trade
#define	DF_INSRTM		0x0040		// search 0 data and insert
#define	DF_NOHAND		0x0080		// don't display hand cusor 

struct	_dataH {	// download data header
	char		name[32]{};		// 종목명
	char		date[4]{};		// # of date(일자수)
	unsigned char	unit{};			// data unit
	unsigned char	dkind{};			// data pattern kind
	unsigned char	gkind{};			// graph kind
	unsigned char	key{};			// overap data key(graph.key == dataH.key)
	unsigned char	option1{};		// extra option
	unsigned char	option2{};		//
	unsigned char	digit{};			// valid digit developer defined 
	char		dummy[4]{};		// dummy data number
	char		max[10]{};
	char		min[10]{};
	char		reserve[8]{};		// reserved byte for future
};

#define	sz_DATAH	sizeof(struct _dataH)

//
//	datah.unit can specify one of the following values
//
#define	GU_USER		0		// 사용자임의	->reference graph.idigit
#define	GU_CODE		21		// 종목
#define	GU_INDEX	22		// 업종
#define	GU_FUTURE	23		// 선물
#define	GU_OPTION	24		// 옵션
#define	GU_KQFUT	25		// KOFEX, kosdaq 선물
#define	GU_KQOPT	26		// kosdaq 옵션
#define	GU_BOND		27		// 채권
#define	GU_KP		30		// KOSPI
#define	GU_KP200	31		// KOSPI200
#define	GU_KP100	32		// KOSPI100
#define	GU_KP50		33		// KOSPI50
#define	GU_IT		34		// 정보통신
#define	GU_KQ		35		// KOSDAQ
#define	GU_KQ50		36		// KOSDAQ50

#define	GU_REF1		0x50		// 현물업종 비교차트 국내종목
#define	GU_REF2		0x51		// 현물업종 비교차트 국내업종
#define	GU_REF3		0x52		// 현물업종 비교차트 해외종목
#define	GU_REF4		0x53		// 현물업종 비교차트 해외업종

//
// dataH.option1 can be any combination of the following values
//

#define	DO_XLIN			0x0010		// draw zero line(in userline)
#define	DO_DIGIT		0x0020		// apply valid digit developer defined 
#define	DO_AVG			0x0040		// 평균치 (월/주봉에서 거래량/거래대금)
#define	DO_ZEROUPDN		0x0080		// 대칭형 데이터(0 베이스)

//
//	dataH.dkind can specify one of the following values
//
#define	DK_NOP			0		// no data
#define	DK_BONG			21		// 시.고.저.종.거래량.index.check.rate
#define	DK_REF			22		// 시.고.저.종.index.check.rate
#define	DK_STKS			23		// 주식수
#define	DK_CRI			24		// 체결 정보
#define	DK_VOL			25		// 거래량.index
#define	DK_AMT			26		// 거래대금.index

#define	DK_VAL			101		// 수치값
#define	DK_VAL2			102		// 수치값.수치값
#define	DK_VIDX			103		// 수치값.index
#define	DK_NMVL			104		// name.value
#define	DK_WKVO			105		// n주간 업종별 거래량


//
//	dataH.dkind == DK_BONG
//
struct _dBong {
	char		fpr[7]{};			// 시가
	char		hpr[7]{};			// 고가
	char		lpr[7]{};			// 저가
	char		epr[7]{};			// 종가 
	char		trn[10]{};			// 거래량
	char		date[8]{};			// index
	unsigned char	check{};			// check
	unsigned char	rate{};				// rate
};
#define	sz_BONG	sizeof(struct _dBong)

//
//	dataH.dkind == DK_REF
//
struct _dRef {
	char		fpr[7]{};			// 시가
	char		hpr[7]{};			// 고가
	char		lpr[7]{};			// 저가
	char		epr[7]{};			// 종가 
	char		date[8]{};			// index
	unsigned char	check{};			// check
	unsigned char	rate{};				// rate
};
#define	sz_REF	sizeof(struct _dRef)

//
//	_dBong.check(_dRef.check) can be any combination of the following values.
//
#define	CBitsKR			0x01		// 권리락
#define	CBitsBR			0x02		// 배당락
#define	CBitsKS			0x04		// 공시
#define	CBitsDI			0x08		// 액면분할
#define	CBitsYP			0x10		// 액면병합


//
//	dataH.dkind == DK_STKS
//
struct	_dStocks {
	char		stock[10]{};		// 주식수
};
#define	sz_STKS	sizeof(struct _dStocks)

//
//	dataH.dkind == DK_CRI
//
struct	_dCri {
	char		ri{};			// 매수 = 1, 호응 = 0, 매도 = -1
};
#define	sz_CRI	sizeof(struct _dCri)

//
//	dataH.dkind == DK_VOL
//
struct	_dVol {
	char		trn[10]{};		// 거래량
	char		date[8]{};		// 일자
};
#define	sz_VOL	sizeof(struct _dVol)

//
//	dataH.dkind == DK_AMT
//
struct	_dAmt {
	char		amt[10]{};		// 거래대금
	char		date[8]{};		// 일자
};
#define	sz_AMT	sizeof(struct _dAmt)

//
//	dataH.dkind == DK_VAL
//
struct	_dVal {
	char		val[10]{};
};
#define	sz_VAL	sizeof(struct _dVal)

//
//	dataH.dkind == DK_VAL2
//
struct	_dVal2 {
	char		val1[10]{};
	char		val2[10]{};
};
#define	sz_VAL2	sizeof(struct _dVal2)

//
//	dataH.dkind == DK_VIDX
//
struct	_dVidx {
	char		val[10]{};		// value
	char		date[8]{};		// 일자
};
#define	sz_VIDX	sizeof(struct _dVidx)

//
//	dataH.dkind == DK_NMVL
//
struct	_dNameVal {
	char		name[20]{};
	unsigned char	color[4]{};
	char		val[10]{};
};
#define	sz_NMVL	sizeof(struct _dNameVal)

//
//	dataH.dkind == DK_WKVO
//	최대 12주, 20업종
//
struct	_dWkVol {
	unsigned char	weekN;
	struct	{
		char	from[8]{};
		char	to[8]{};
	} week[12]{};

	unsigned char	indexN{};
	struct	{
		char		name[20]{};
		unsigned char	color[4]{};
	} index[20]{};

	unsigned char	trn[10]{};			// weekN * indexN
};
#define	sz_WKVO	sizeof(struct _dWkVol)

//
// dgraphH.index == GI_HMS
// ?.date[6] can specify one of the following values
//
#define	HMS_NOP			21		// NOP
#define	HMS_HH			22		// HH
#define	HMS_HM			23		// HH:MM
#define	HMS_HMS			24		// HH:MM:SS
#define	HMS_END			99		// 장마감
//
// dgraphH.index == GI_HMS
// ?.date[7] can specify one of the following values
//
#define	HMS_NOP			21		// NOP
#define	HMS_DAY			22		// day change

//
//	Axis graph internal error
//
#define	AGEBASE		5000
#define	AGEATTACH	(AGEBASE + 1)		// Data Attach Error
#define	AGEOBJECT	(AGEBASE + 2)		// Graph Object Name Miss Match
#define	AGENODATA	(AGEBASE + 3)		// Not Exist Valid Data
#define	AGENOGRAPH	(AGEBASE + 4)		// Not Exist Valid Graph

struct	_graphi {
	CString	objects;	// object name
	char*	file{};		// fgraph  
	CRect	rect{};		// object rect
	int	days{};		// display day (field length)
	int	tRGB{};		// text RGB 
	int	pRGB{};		// paint RGB

	CString	fName;		// font name
	int	fPoint{};		// font point
	int	fStyle{};		// font style
};

#pragma pack()
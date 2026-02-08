#if !defined(AFX_SMDEFINE_H_INCLUDED__)
#define	AFX_SMDEFINE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif 

#define WM_XMSG			WM_USER+9999
#define WM_XMSG2		WM_USER+8888

#define SRCMASK			0x00220326    // mask

#define V_SCREEN1		0
#define V_SCREEN2		1
#define V_SCREEN3		2
#define V_SCREEN4		3
#define V_SCREEN5		4
#define V_SCREEN6		5

#define CTRL_SEARCHS		1
#define CTRL_EDITTOOL		2
#define CTRL_REGISTERALL	3
#define CTRL_REGISTER		4
#define CTRL_SCREENPRINT	5
#define CTRL_SAVEWORKSPACE	6
#define CTRL_WORKSPACELIST	7
#define CTRL_DEAL		8
#define CTRL_JSCREEN		9
#define CTRL_TICKSETUP		10
#define CTRL_SDI		11

#define CTRL_SWITCH		13
#define CTRL_SDIMENU		14
#define CTRL_LINKEDIT		15
#define CTRL_HIDE		16

#define CTRL_LOCK		12
#define CTRL_ENVIRONMENT	17
#define CTRL_HOMEPAGE		18
#define CTRL_CONFIG			19

#define CTRL_FUNC		21	// 21 - 30 function button reserved

#define	MAX			5
#define SKINMAX			7

#define	WIDTH_TOOLGROUP		80
#define	COMBTN_WIDTH		16
#define COLOR_COMLINE		RGB(55, 55, 55)
#define	COLOR_FRAME		RGB(228,235,254)
		
#define CLR_MENU_TOP_BK		101		// top menu back color
#define CLR_MENU_TOP_SELECT	102		// top menu select color
#define CLR_MENU_TOP_UL		103		// top menu select color upper left
#define CLR_MENU_TOP_BR		104		// top menu select color bottom right
#define CLR_MENU_XP_BK		105		// menu xp back color
#define CLR_MENU_XP_SELECT	106		// menu xp select color
#define CLR_MENU_XP_SEL_TEXT	107		// menu xp select text color
#define CLR_MENU_XP_TEXT	108		// menu xp text color
#define CLR_MENU_XP_ICONAREA	109		// menu xp icon area color
#define CLR_MENU_XP_CHECK	110		// menu xp check area color

#define CLR_TOOL_BK		121		// toolbar back color (dbar1)
#define CLR_TOOL_SEP1		122		// seperator first color
#define CLR_TOOL_SEP2		123		// seperator second color
#define CLR_LIST_BK		126		// screen list back color (dbar2)

#define CLR_TICK_BK		131
#define CLR_TICK_UP		132
#define CLR_TICK_DOWN		133
#define CLR_TICK_TEXT		134
#define CLR_TICK_INSIDE		135
#define CLR_TICK_LINE		136
#define CLR_TICK_SH		137
#define CLR_TICK_SS		138
#define CLR_TICK_BH		139
#define CLR_TICK_HL		140
#define CLR_TICK_HH		141
#define CLR_TICK_DATA		142

#define	USERSN			"UserBar"
#define	USEREN			"Current"
#define TOOLTYPE		"type"

#define	CON_HIDE		1
#define	CON_PREV		2
#define	CON_NEXT		3
#define	CON_PALY		4
#define	CON_PAUSE		5
#define	CON_CONTINUE		6
#define	CON_STOP		7

//	toolbar type info
#define TTYPE1			1	// 1 line toolbar	
#define TTYPE2			2	// 2 line toolbar

//	ticker info index
#define TICKBAR_CNT		2
#define SPLIT_CNT		6

#define INVEST_KOSPI		53	// kospi invest board
#define INVEST_KOSDAQ		54
#define INVEST_FUTURE		55
#define INVEST_CALL		56
#define INVEST_PUT		57
#define UPDN_KOSPI		60
#define UPDN_KOSDAQ		61
#define MNG_INFO		70	// 장운영 정보
#define NEWS_BASE		80

//	ticker info
#define	LISTBAR_HEIGHT		31	/* heidht + 2(border size)*/
#define SDIBAR_WIDTH		21	/* width + 2(border size)*/
#define TINFO_HEIGHT		22	/* heidht + 2(border size)*/

#define	HEIGHT_CTRLBAR1		37
#define	HEIGHT_CTRLBAR2		44

#define GRIP_LT			RGB(255, 255, 255)
#define	GRIP_RB			RGB(171, 171, 171)
#define HORZFIT_TOP		1
#define HORZFIT_BOTTOM		1
#define BUTTONWIDTH1		64
#define BUTTONWIDTH2		64

//	ticker bar mode
#define TICKERMODE_UP		'0'
#define TICKERMODE_DOWN		'1'

//	ticker iteminfo
#define	TKIND_INDEX		0
#define TKIND_NEWS		1
#define TKIND_UPDOWN		2
#define TKIND_INTER		3
#define TKIND_CONCLUSION	4
#define TKIND_INVEST		5

//	toolbar	status flag
#define TB_MAINBAR		0x001
#define TB_LISTBAR		0x002
#define TB_HISTBAR		0x003
#define TB_INFOBAR1		0x004
#define TB_INFOBAR2		0x008
#define TB_INFOBAR3		0x010
#define TB_JCONTROLBAR		0x020
#define TB_SDIBAR		0x040
#define TB_SDIMAINBAR		0x080

//	sise symbol define
#define S_TIME		"034"
#define S_CUR		"023"
#define S_DB		"024"
#define S_RATE		"033"
#define S_NVOL		"027"
#define S_PVOL		"028"
#define S_SI		"029"
#define S_KO		"030"
#define S_JE		"031"

#define S_KOSPI		"002"
#define S_THEORY	"202"
#define BASIS		"204"
#define NEWKJ		"207"
#define S_FTIME		"256"
#define S_NONE		"―"
#define S_SKofDay	"↑"
#define S_SJofDay	"↓"
#define S_SSofPrev	"↗"
#define S_HLofPrev	"↘"

#define S_SS		"252"
#define S_SH		"251"
#define S_BH		"253"
#define S_HL		"255"
#define S_HH		"254"

#define SNEW_TITLE	"015"
#define SNEW_ISAMNO	"014"
#define SNEW_KEY	"016"
#define SNEW_JCODE	"301"
#define SNEW_JNAME	"022"
#define SNEW_INFO	"041"
#define SNEW_BRANCH	"042"
#define SNEW_TIME	"044"
#define SNEW_NEWSK	"045"
#define SNEW_PROVIDE	"046"
#define SNEW_HTML	"047"
#define SNEW_DATE	"048"
/*
 tickpane : NEWS      
 struct  symrts sr_news[] = {
    { 'T', {{ 15,  "제목"  }, { 14, "RECN"  }, { 16, "KEYV" },
        { 301, "코드"  }, { 22, "종목명" },
        { 41,  "KEY1"  }, { 42, "KEY2"  },
        { 44,  "시간"  }, { 45, "분류"  }, { 46, "분류2" },
        { 47,  "XFLG"  }, { 48, "일자"  }, }},
    {   0, {{   0,  ""         },}}
};
*/
//	program mm
#define PROG_VOLALL	"328"
#define PROG_VOLCHA	"330"
#define PROG_VOLBI	"332"
#define PROG_AMTALL	"327"
#define PROG_AMTCHA	"329"
#define PROG_AMTBI	"331"

//	invest symbol
#define INVEST_MSVOL	"343"
#define INVEST_MSPRICE	"344"

//	invest code
#define INVEST_KOSPI_ORG	"TH00000001"		// 코스피 기관
#define INVEST_KOSPI_GAMI	"TH00080001"		// 코스피 개인
#define INVEST_KOSPI_FOR	"TH00090001"		// 코스피 외국인
#define INVEST_KOSDAQ_ORG	"TH00000201"		// 코스닥 기관
#define INVEST_KOSDAQ_GAMI	"TH00080201"		// 코스닥 개인
#define INVEST_KOSDAQ_FOR	"TH00090201"		// 코스닥 외국인
#define INVEST_FUTURE_ORG	"TF00000000"		// 선물 기관
#define INVEST_FUTURE_GAMI	"TF00080000"		// 선물 개인
#define INVEST_FUTURE_FOR	"TF00090000"		// 선물 외국인
#define INVEST_CALL_ORG		"TO00000002"		// 콜 기관
#define INVEST_CALL_GAMI	"TO00080002"		// 콜 개인
#define INVEST_CALL_FOR		"TO00090002"		// 콜 외국인
#define INVEST_PUT_ORG		"TO00000003"		// 풋 기관
#define INVEST_PUT_GAMI		"TO00080003"		// 풋 개인
#define INVEST_PUT_FOR		"TO00090003"		// 풋 외국인

//	total setup
/**
#define E_SETUP		0
#define MNG_SETUP	1
#define ORDER_SETUP	2
#define IMAGE_SETUP	3
#define TICK_SETUP	4
#define HOTKEY_SETUP	5
#define TOOL_SETUP	6
#define TOOL_FUNC	7
**/

#define SETUPFILE	"axisensetup.ini"
#define TICKSETUP	"axisticker.ini"

#endif // !defined(AFX_SMDEFINE_H_INCLUDED__)
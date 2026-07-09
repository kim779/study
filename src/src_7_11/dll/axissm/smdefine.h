#if !defined(AFX_SMDEFINE_H_INCLUDED__)
#define	AFX_SMDEFINE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif 

#define WM_XMSG			WM_USER+9999
#define WM_XMSG2		WM_USER+8888
#define WM_SETUP		WM_USER+7777
#define WM_

#define SRCMASK			0x00220326    // mask

#define V_SCREEN1		0
#define V_SCREEN2		1
#define V_SCREEN3		2
#define V_SCREEN4		3
//#define V_SCREEN5		4
//#define V_SCREEN6		5

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
#define CTRL_LOCK		12
#define CTRL_SWITCH		13
#define CTRL_SDIMENU		14
#define CTRL_LINKEDIT		15
#define CTRL_HIDE		16
#define CTRL_ENVIRONMENT	17
#define CTRL_HOMEPAGE		18
#define CTRL_TV			19
#define CTRL_RADIO		20
#define CTRL_COLORINDEX		21

#define	MAX			5
#define SKINMAX			7

#define CTRL_BANNER		30
#define CTRL_FUNC		31

#define	WIDTH_TOOLGROUP		88
#define	COMBTN_WIDTH		15
#define COLOR_COMLINE		RGB(108, 114, 174)

#define	COLOR_FRAME		RGB(241,239,238)

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

//	ticker info index
#define TICKBAR_CNT		2
#define SPLIT_CNT		6

#define UPDN_KOSPI		14
#define UPDN_KOSDAQ		15
#define NEWS_BASE		16

//	ticker info
#define	LISTBAR_HEIGHT		25	/* heidht + 2(border size)*/
#define SDIBAR_WIDTH		21	/* width + 2(border size)*/
#define TINFO_HEIGHT		22	/* heidht + 2(border size)*/
#define	HEIGHT_CTRLBAR1		29
#define	HEIGHT_CTRLBAR2		39
#define GRIP_LT			RGB(255, 255, 255)
#define	GRIP_RB			RGB(171, 171, 171)
#define HORZFIT_TOP		1
#define HORZFIT_BOTTOM		1
#define BUTTONWIDTH1		38
#define BUTTONWIDTH2		51

//	ticker bar mode
#define TICKERMODE_UP		'0'
#define TICKERMODE_DOWN		'1'

//	ticker iteminfo
#define	TKIND_INDEX		0
#define TKIND_NEWS		1
#define TKIND_UPDOWN		2
#define TKIND_INTER		3
#define TKIND_CONCLUSION	4

//	toolbar	status flag
#define TB_MAINBAR		0x001
#define TB_LISTBAR		0x002
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
#define INVEST_MSAMOUNT	"344"

//	total setup
#define E_SETUP		0
#define MNG_SETUP	1
#define ORDER_SETUP	2
#define IMAGE_SETUP	3
#define TICK_SETUP	4
#define HOTKEY_SETUP	5
#define TOOL_SETUP	6

#define SETUPFILE	"axisensetup.ini"
#define TICKSETUP	"axisticker.ini"

#endif // !defined(AFX_SMDEFINE_H_INCLUDED__)
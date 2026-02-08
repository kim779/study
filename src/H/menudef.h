#pragma once
/*
const int chartBASE = 3000;
const int chartTYPE = 3100;
const int chartAID  = 3200;

*/
#define MENU_CHART_BASE	3000
#define	MENU_CHART_TYPE	3100
#define	MENU_CHART_AID	3200

struct _menuString {
	UINT	uFlag{};
	int	nType{};
	int	nKind{};
	char	item[32]{};
};

struct	_menuString g_aMenuBase[] = {
	{MFT_STRING,	0,	0,	"추세선"	},	// +0
	{MFT_STRING,	0,	0,	"십자선"	},	// +1
	{MFT_STRING,	0,	0,	"수평선"	},	// +2
	{MFT_STRING,	0,	0,	"수직선"	},	// +3
	{MFT_STRING,	0,	0,	"전체지우기"	},	// +4
	{MFT_SEPARATOR,	0,	0,	""		},	// +5
	{MFT_STRING,	0,	0,	"차트유형"	},	// +6
	{MFT_STRING,	0,	0,	"지표추가"	},	// +7
	{MFT_STRING,	0,	0,	"수치조회창 표시"},	// +8
	{MFT_STRING,	0,	0,	"자료조회"	},	// +9
	{MFT_STRING,	0,	0,	"로그적용"},		// +10
	{MFT_STRING,	0,	0,	"대칭모드"},		// +11
	{MFT_STRING,	0,	0,	"초기화"	},	// +12
	{MFT_SEPARATOR,	0,	0,	""		},	// +13
	{MFT_STRING,	0,	0,	"화면설정"	},	// +14
	{MFT_STRING,	0,	0,	"지표설정"	},	// +15
	{MFT_STRING,	0,	0,	"지표삭제"	},	// +16
	{MFT_STRING,	0,	0,	"환경설정"	},	// +17
	//** macho add
	{MFT_SEPARATOR, 0,	0,	"" },			// 18
	{MFT_STRING,	0,	0,	"시세 보이기"},		// 19
	{MFT_STRING,	0,	0,	"도구바 보이기" },	// 20
	{MFT_STRING,	0,	0,	"지표바 보이기" },	// 21
	{MFT_STRING,	0,	0,	"도구바 설정"	},	// 22
	{MFT_STRING,	0,	0,	"지표바 설정"	},	// 23
	{MFT_SEPARATOR,	0,	0,	""		},	// 24
	{MFT_STRING,	0,	0,	"엑셀로저장"	},	// 25
	{MFT_STRING,	0,	0,	"이미지저장"	},	// 26
	{MFT_STRING,	0,	0,	"인쇄하기"	},	// 27
	{MFT_SEPARATOR,	0,	0,	""		},	// 28
	{MFT_STRING,	0,	0,	"십자선 숨기기"	},	// 29
	{MFT_STRING,	0,	0,	"십자선 종가따라가기"},	// 30


	//** 
// 2006.10.26 schbang
//	{MFT_STRING,	0,	0,	"한칸의값 입력"},	// +25
//	{MFT_STRING,	0,	0,	"최소반전폭 입력"},	// +26
//	{MFT_STRING,	0,	0,	"최소변화폭 입력"},	// +27
};

struct	_menuString g_aMenuType[] = {
	{MFT_STRING,	0,	GK_JPN,		"봉차트"	},
	{MFT_STRING,	0,	GK_LIN,		"선차트"	},
	{MFT_STRING,	0,	GK_AVOL,	"매물차트"	},
	{MFT_STRING,	0,	GK_BALANCE,	"일목균형표"	},		
	{MFT_STRING,	0,	GK_CDV,		"CandleVolume"	},
	{MFT_STRING,	0,	GK_EQV,		"EquiVolume"	},
	{MFT_STRING,	0,	GK_THREE,	"삼선전환도"	},
	{MFT_STRING,	0,	GK_PNF,		"P&&F차트"	},
	{MFT_STRING,	0,	GK_CLOCK,	"역시계곡선"	},
	{MFT_STRING,	0,	GK_PV,		"PV차트"	},
	{MFT_STRING,	0,	GK_BAR,		"바차트"	},
	{MFT_STRING,	0,	GK_POLE,	"POLE차트"	},		
	{MFT_STRING,	0,	GK_KAGI,	"Kagi차트"	},
	{MFT_STRING,	0,	GK_RENKO,	"Renko차트"	},
};

struct	_menuString g_aMenuKind[] = {
	{MFT_STRING,	0,	0,	"즐겨찾기"	},
	{MFT_STRING,	0,	16,	"추세지표"	},
	{MFT_STRING,	0,	15,	"모멘텀지표"	},
	{MFT_STRING,	0,	11,	"거래량지표"	},
	{MFT_STRING,	0,	2,	"시장강도지표"	},
	{MFT_STRING,	0,	2,	"채널지표"	},
	{MFT_STRING,	0,	8,	"기타지표"	},
	//**
};

struct	_menuString g_aMenuAid[] = {
	{MFT_STRING,	GT_BONG,	GK_PMA,		"가격 이평" },
	{MFT_STRING,	GT_BONG,	GK_NET,		"그물차트" },
	{MFT_STRING,	GT_LINE,	GK_ADLINE,	"A/D Line" },
	{MFT_STRING,	GT_LINE,	GK_ADX,		"ADX" },
	{MFT_STRING,	GT_LINE,	GK_CCI,		"CCI" },
	{MFT_STRING,	GT_LINE,	GK_DMI,		"DMI" },
	{MFT_STRING,	GT_BONG,	GK_DEMARK,	"Demark" },
	{MFT_STRING,	GT_LINE,	GK_MACD,	"MACD" },
	{MFT_STRING,	GT_LINE,	GK_MACDOS,	"MACD Oscillator" },
	{MFT_STRING,	GT_BONG,	GK_PIVOT,	"Pivot Line" },
	{MFT_STRING,	GT_LINE,	GK_PMAO,	"Price Oscillator" },
	{MFT_STRING,	GT_LINE,	GK_RSI,		"RSI" },
	{MFT_STRING,	GT_LINE,	GK_RSIWEIGHT, "RSI(가중치)" },	// 20090923 : ADD : RSI Weight 지표 추가. RSI with exponentional moving average
	{MFT_STRING,	GT_LINE,	GK_TRIX,	"TRIX" },
	{MFT_STRING,	GT_LINE,	GK_WPR,		"William's %R" },	
	{MFT_STRING,	GT_BONG,	GK_PARB,	"Parabolic" },

	{MFT_STRING,	GT_LINE,	GK_GAP,		"이격도" },
	{MFT_STRING,	GT_LINE,	GK_MOMENTUM,	"모멘텀" },
	{MFT_STRING,	GT_LINE,	GK_ABRATIO,	"ABRatio" },
	{MFT_STRING,	GT_LINE,	GK_BANDWIDTH,	"Band Width" },
	{MFT_STRING,	GT_LINE,	GK_CO,		"Chaikin's Oscillator" },
	{MFT_STRING,	GT_LINE,	GK_CV,		"Chaikin's Volatility" },
	{MFT_STRING,	GT_LINE,	GK_EMV,		"Ease of Movement" },
	{MFT_STRING,	GT_LINE,	GK_LFI,		"LFI" },
	{MFT_STRING,	GT_LINE,	GK_NCO,		"Net Change Oscillator" },
	{MFT_STRING,	GT_LINE,	GK_PROC,	"Price Rate-of-Change" },
	{MFT_STRING,	GT_LINE,	GK_SIGMA,	"Sigma" },
	{MFT_STRING,	GT_LINE,	GK_SONA,	"SONAR" },
	{MFT_STRING,	GT_LINE,	GK_SONAMOMENTUM,"SONA Momentum" },
	{MFT_STRING,	GT_LINE,	GK_STOCHS,	"Slow Stochastics" },
	{MFT_STRING,	GT_LINE,	GK_STOCHF,	"Fast Stochastics" },

	{MFT_STRING,	GT_BONG,	GK_VOL,		"거래량" },
	{MFT_STRING,	GT_BONG,	GK_VMA,		"거래량 이평" },
	{MFT_STRING,	GT_LINE,	GK_RRATE,	"회전율" },
	{MFT_STRING,	GT_BONG,	GK_AMT,		"거래대금" },
	{MFT_STRING,	GT_BONG,	GK_AMA,		"거래대금 이평" },
	{MFT_STRING,	GT_LINE,	GK_ADR,		"ADR" },
	{MFT_STRING,	GT_LINE,	GK_MFI,		"Money Flow Index" },
	{MFT_STRING,	GT_LINE,	GK_OBV,		"OBV" },
	{MFT_STRING,	GT_LINE,	GK_VMAO,	"Volume Oscillator" },
	{MFT_STRING,	GT_LINE,	GK_VR,		"VR" },
	{MFT_STRING,	GT_LINE,	GK_VROC,	"Volume Rate-of-Change" },

	{MFT_STRING,	GT_LINE,	GK_PSY,		"심리도" },		
	{MFT_STRING,	GT_LINE,	GK_NPSY,	"신심리도" },	

	{MFT_STRING,	GT_BONG,	GK_BOLB,	"Bollinger Band" },
	{MFT_STRING,	GT_BONG,	GK_ENVL,	"Envelope" },

	{MFT_STRING,	GT_LINE,	GK_FRGNHAVE, "외국인보유(수량)"},
	{MFT_STRING,	GT_LINE,	GK_FRGNRATE, "외국인보유(비율)"},
	{MFT_STRING,	GT_LINE,	GK_PSNLPBUY, "개인순매수(수량)"},
	{MFT_STRING,	GT_LINE,	GK_FRGNPBUY, "외국인순매수(수량)"},
	{MFT_STRING,	GT_LINE,	GK_ORGNPBUY, "기관순매수(수량)"},
	{MFT_STRING,	GT_LINE,	GK_PSNLTBUY, "개인누적순매수(수량)"},
	{MFT_STRING,	GT_LINE,	GK_FRGNTBUY, "외국인누적순매수(수량)"},
	{MFT_STRING,	GT_LINE,	GK_ORGNTBUY, "기관누적순매수(수량)"},
};


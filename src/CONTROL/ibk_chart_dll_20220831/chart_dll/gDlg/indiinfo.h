#ifndef __INDICATOR_INFOMATION_H__
#define __INDICATOR_INFOMATION_H__

#define _SPIN_		'S'
#define _EDIT_		'E'
#define _NEDIT_		'N'
#define _DBEDIT_	'D'
#define _COMBO_		'C'
#define _MACOMBO_	'M'
#define _VOLCOMBO_	'V'
#define	_STATIC_	'T'
#define _FONT_		'F'
#define _COLOR_		'R'
#define _WIDTH_		'W'
#define _KIND_		'K'
#define _CHECK_		'H'

#define _SMA_		"단순이평"
#define _WMA_		"가중이평"
#define _EMA_		"지수이평"

#define _OCMA_		"시가"
#define _HCMA_		"고가"
#define _LCMA_		"저가"
#define _CCMA_		"종가"
#define _MCMA_		"중간값"
#define _RCMA_		"대표값"

#define _PERIOD_	"기간"
#define _SIG_		"Signal"
#define _MATYPE_	"MA 타입"
#define _OVERBUY_	"과매수"
#define _OVERSELL_	"과매도"

#define _CWK_		'A'
#define	_C_		'C'
#define _W_		'W'
#define _K_		'K'
#include "../../h/axisgenv.h"

struct	_indiInfo
{
	BYTE	kind;
	bool	hori;
	BYTE	digit;
	BYTE	bcnt;	// 기본설정의 개수	

	struct	
	{
		struct	
		{
			char	kind;
			char	title[20];
			WORD	min;
			WORD	max;
		} info;
		BYTE	ccnt;		//콤보의 내용물의 개수

		struct 
		{
			char	content[20];
			WORD	value;
		} combo[7];
		
	} basic[6];	//} basic[5];

	BYTE	lcnt;

	struct	_lineInfo
	{
		char	kind;
		char	title[20];
		WORD	value;
	} line[5];
	
	
};

static struct _indiInfo	__indiAll[] =
{
	// PMA
	{								\
		GK_PMA, false, 0,					\
		MAX_LINE, {						\
			{{_SPIN_, _PERIOD_, 0, 999}}			\
		},							\
		MAX_LINE, {						\
			{_CWK_, "MA"}					\
		}							\
	},

	// 그물차트
	{								\
		GK_NET, false, 0,					\
		4, {							\
			{{_SPIN_, "시작이평", 1, 999}},			\
			{{_SPIN_, "증가일", 1, 999}},			\
			{{_SPIN_, "이평개수", 1, 30}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		1, {							\
			{_CWK_, "그물차트"}				\
		}							\
	},

	// A/D Line
	{								\
		GK_ADLINE, true, 0,					\
		2, {							\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "A/D Line"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// ADX
	{								\
		GK_ADX, true, 2,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "ADX"},					\
			{_CWK_, _SIG_}					\
		}							\
	}, 

	// CCI
	{								\
		GK_CCI, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "CCI"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// DMI
	{								\
		GK_DMI, true, 2,					\
		2, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "상승탄력"},				\
			{_CWK_, "하락탄력"}				\
		}							\
	}, 
	
	// Demark
	{								\
		GK_DEMARK, false, 0,					\
		0, {'0'},						\
		2, {							\
			{_CWK_, "목표고가"},				\
			{_CWK_, "목표저가"}				\
		}							\
	},

	// MACD
	{								\
		GK_MACD, true, 2,					\
		4, {							\
			{{_SPIN_, "단기", 1, 999}},			\
			{{_SPIN_, "장기", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "MACD"},				\
			{_CWK_, _SIG_}					\
		}							\
	}, 

	// MACD	Oscillator
	{								\
		GK_MACDOS, true, 2,					\
		4, {							\
			{{_SPIN_, "단기", 1, 999}},			\
			{{_SPIN_, "장기", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_C_, "상승"},					\
			{_C_, "하락"}					\
		}							\
	},

	// PMAO
	{								\
		GK_PMAO, true, 2,					\
		4, {							\
			{{_SPIN_, "단기", 1, 999}},			\
			{{_SPIN_, "장기", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "PO"},					\
			{_CWK_, _SIG_}					\
		}							\
	}, 

	// RSI
	{								\
		GK_RSI, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "RSI"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// RSI(가중치) 	// 20090928 : ADD : RSI 지표 추가
	{								\
		GK_RSIWEIGHT, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "RSI(가중치)"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// TRIX
	{								\
		GK_TRIX, true, 6,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "TRIX"},				\
			{_CWK_, _SIG_}					\
		}							\
	},
	
	// William's %R
	{								\
		GK_WPR, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "William's %R"},			\
			{_CWK_, _SIG_}					\
		}							\
	},

	// Parabolic
	{								\
		GK_PARB, false,	0,					\
		2, {							\
			{{_SPIN_, "최대값", 1, 999}},			\
			{{_SPIN_, "증가단위", 1, 999}}			\
		},							\
		1, {							\
			{_C_, "PARABOLIC"}				\
		}							\
	},

	// Pivot
	{								\
		GK_PIVOT, false, 0,					\
		0, {'0'},						\
		5, {							\
			{_CWK_, "Pivot"},				\
			{_CWK_, "1차 저항선"},				\
			{_CWK_, "1차 지지선"},				\
			{_CWK_, "2차 저항선"},				\
			{_CWK_, "2차 지지선"}				\
		}							\
	},
	
	// 이격도
	{								\
		GK_GAP, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "이격도"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// 모멘텀
	{								\
		GK_MOMENTUM, true, 2,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "모멘텀"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// ABRatio
	{								\
		GK_ABRATIO, true, 2,					\
		2, {							\
			{{_SPIN_, "기간1", 1, 999}},			\
			{{_SPIN_, "기간2", 1, 999}}			\
		},							\
		2, {							\
			{_CWK_, "A-Ratio"},				\
			{_CWK_, "B-Ratio"}				\
		}							\
	},

	// CO
	{								\
		GK_CO, true, 0,						\
		4, {							\
			{{_SPIN_, "단기", 1, 999}},			\
			{{_SPIN_, "장기", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "CO"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// CV
	{								\
		GK_CV, true, 2,						\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "CV"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// EMV
	{								\
		GK_EMV, true, 6,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "EOM"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// LFI
	{								\
		GK_LFI, true, 2,					\
		2, {							\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "LFI"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// NCO
	{								\
		GK_NCO, true, 2,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "NCO"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// PROC
	{								\
		GK_PROC, true, 2,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "PROC"},				\
			{_CWK_, _SIG_}					\
		}							\
	},
		
	// Sigma
	{								\
		GK_SIGMA, true,	2,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "Sigma"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// SONA
	{								\
		GK_SONA, true, 2, 					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "SONAR"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// Slow Stochastics
	{								\
		GK_STOCHS, true, 2,					\
		6, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, "Slow %K", 1, 999}},			\
			{{_SPIN_, "Slow %D", 1, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "Slow %K"},				\
			{_CWK_, "Slow %D"}				\
		}							\
	},

	// Fast Stochastics
	{								\
		GK_STOCHF, true, 2,					\
		5, {							\
			{{_SPIN_, "Fast %K", 1, 999}},			\
			{{_SPIN_, "Fast %D", 1, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "Fast %K"},				\
			{_CWK_, "Fast %D"}				\
		}							\
	},
	
	// 거래량
	{								\
		GK_VOL, false, 0,					\
		1, {{{_VOLCOMBO_, "조건"},				\
				4, {					\
					{"조건없음", 0},		\
					{"가격대비", GC_VOLPRC},	\
					{"거래량대비", GC_VOLVOL},	\
					{"시가종가대비", GC_VOLFPR}	\
				}					\
			}						\
		},							\
		3, {							\
			{_C_, "거래량"},				\
			{_C_, "상승"},					\
			{_C_, "하락"}					\
		}							\
	},

	// VMA
	{								\
		GK_VMA, false, 0,					\
		MAX_LINE, {						\
			{{_SPIN_, _PERIOD_, 0, 999}}			\
		},							\
		MAX_LINE, {						\
			{_CWK_, "MA"},					\
		}							
	},

	// 회전율
	{								\
		GK_RRATE, true,	0,					\
		1, {							\
			{{_SPIN_, _SIG_, 0, 999}}			\
		},							\
		2, {							\
			{_CWK_, "회전율"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// 거래대금
	{								\
		GK_AMT, false, 0,					\
		1, {{{_VOLCOMBO_, "조건"},				\
				4, {					\
					{"조건없음", 0},		\
					{"가격대비", GC_VOLPRC},	\
					{"거래대금대비", GC_VOLVOL},	\
					{"시가종가대비", GC_VOLFPR},	\
				}					\
			}						\
		},							\
		3, {							\
			{_C_, "거래대금"},				\
			{_C_, "상승"},					\
			{_C_, "하락"}					\
		}							\
	},

	// AMA
	{								\
		GK_AMA, false, 0,					\
		MAX_LINE, {						\
			{{_SPIN_, _PERIOD_, 0, 999}}			\
		},							\
		MAX_LINE, {						\
			{_CWK_, "MA"},					\
		}							
	},

	// ADR
	{								\
		GK_ADR, true, 0,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "ADR"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// MFI
	{								\
		GK_MFI, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "MFI"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// OBV
	{								\
		GK_OBV, true, 0,					\
		2, {							\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "OBV"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// VR
	{								\
		GK_VR, true, 2,						\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "VR"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// VROC
	{								\
		GK_VROC, true, 2,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "VROC"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// VMAO
	{								\
		GK_VMAO, true, 2,					\
		4, {							\
			{{_SPIN_, "단기", 1, 999}},			\
			{{_SPIN_, "장기", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "VO"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// 심리도
	{								\
		GK_PSY, true, 0,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "심리도"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// 신심리도
	{								\
		GK_NPSY, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "신심리도"},				\
			{_CWK_, _SIG_}					\
		}							\
	},	

	// Bollinger Band
	{								\
		GK_BOLB, false,	0,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, "승수", 1, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		3, {							\
			{_CWK_, "상한선"},				\
			{_CWK_, "중심선"},				\
			{_CWK_, "하한선"}				\
		}							\
	},
	
	// Envelope
	{								\
		GK_ENVL, false,	0,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, "승수", 1, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		3, {							\
			{_CWK_, "상한선"},				\
			{_CWK_, "중심선"},				\
			{_CWK_, "하한선"}				\
		}							\
	},


	// 캔들볼륨
	{								\
		GK_CDVV, true, 0,					\
		0, {'0'},						\
		1, {							\
			{_CWK_, "캔들볼륨"}				\
		}							\
	},
	// 이퀴볼륨
	{								\
		GK_EQVV, true, 0,					\
		0, {'0'},						\
		1, {							\
			{_CWK_, "이퀴볼륨"},				\
		}							\
	},
	// Band Width
	{								\
		GK_BANDWIDTH, true,	2,				\
		4, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, "승수", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "Band Width"},				\
			{_CWK_, _SIG_}					\
		}							\
	},
	// SONA Momentum
	{								\
		GK_SONAMOMENTUM, true, 2, 				\
		4, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, "기간2", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "SONA Momentum"},			\
			{_CWK_, _SIG_}					\
		}							\
	},

	// 외국인보유현황
	{
		GK_FRGNHAVE, true, 0,
		0, {'0'},
		1, {
			{_CWK_, "외국인(보유량)" }
		}
	},
	// 외국인보유비율
	{
		GK_FRGNRATE, true, 0,
		0, {'0'},
		1, {
			{_CWK_, "외국인(비율)"}
		}
	},
	// 개인순매매수량
	{
		GK_PSNLPBUY, true, 0,
		0, {'0'},
		2, {
			{_CWK_, "개인 순매수(수량)"},
			{_CWK_, "개인 순매도(수량)"}
		}
	},
	// 외국인순매매수량
	{
		GK_FRGNPBUY, true, 0,
		0, {'0'},
		2, {
			{_CWK_, "외국인 순매수(수량)"},
			{_CWK_, "외국인 순매도(수량)"}
		}
	},
	// 기관순매매수량
	{
		GK_ORGNPBUY, true, 0,
		0, {'0'},
		2, {
			{_CWK_, "기관 순매수(수량)"},
			{_CWK_, "기관 순매도(수량)"}
		}
	},
	// 개인누적순매매수량
	{
		GK_PSNLTBUY, true, 0,
		0, {'0'},
		2, {
			{_CWK_, "개인 순매수(수량)"},
			{_CWK_, "개인 순매도(수량)"}
		}
	},
	// 외국인누적순매매수량
	{
		GK_FRGNTBUY, true, 0,
		0, {'0'},
		2, {
			{_CWK_, "외국인 순매수(수량)"},
			{_CWK_, "외국인 순매도(수량)"}
		}
	},
	// 기관누적순매매수량
	{
		GK_ORGNTBUY, true, 0,
		0, {'0'},
		2, {
			{_CWK_, "기관 순매수(수량)"},
			{_CWK_, "기관 순매도(수량)"}
		}
	},
};

#endif

// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2001
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic, 
// mechanical, or otherwise without the written permission of Winix Information.
//
//  Components   : key.hxx
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2004-03	Initial version
// *****************************************************************************

#ifndef	_KEY_HXX_
#define	_KEY_HXX_

#include "symbol.h"
#include "index.hxx"

struct	_keys {
	int	key;
	int	index;
};

struct	_keys	keys[] = {
	{ DHG1, askPrice1		},	// 매도호가
	{ DHG2, askPrice2		},
	{ DHG3, askPrice3		},
	{ DHG4, askPrice4		},
	{ DHG5, askPrice5		},
	{ DHG6, askPrice6		},
	{ DHG7, askPrice7		},
	{ DHG8, askPrice8		},
	{ DHG9, askPrice9		},
	{ DHGA, askPrice10		},

	{ DHJ1, askSize1		},	// 매도호가 잔량
	{ DHJ2, askSize2		},
	{ DHJ3, askSize3		},
	{ DHJ4, askSize4		},
	{ DHJ5, askSize5		},
	{ DHJ6, askSize6		},
	{ DHJ7, askSize7		},
	{ DHJ8, askSize8		},
	{ DHJ9, askSize9		},
	{ DHJA, askSize10		},

	{ DHC1, askBefore1		},	// 매도호가 건수
	{ DHC2, askBefore2		},
	{ DHC3, askBefore3		},
	{ DHC4, askBefore4		},
	{ DHC5, askBefore5		},
	{ DHC6, askBefore6		},
	{ DHC7, askBefore7		},
	{ DHC8, askBefore8		},
	{ DHC9, askBefore9		},
	{ DHCA, askBefore10		},

	{ SHG1, bidPrice1		},	// 매수호가
	{ SHG2, bidPrice2		},
	{ SHG3, bidPrice3		},
	{ SHG4, bidPrice4		},
	{ SHG5, bidPrice5		},
	{ SHG6, bidPrice6		},
	{ SHG7, bidPrice7		},
	{ SHG8, bidPrice8		},
	{ SHG9, bidPrice9		},
	{ SHGA, bidPrice10		},

	{ SHJ1, bidSize1		},	// 매수호가 잔량
	{ SHJ2, bidSize2		},
	{ SHJ3, bidSize3		},
	{ SHJ4, bidSize4		},
	{ SHJ5, bidSize5		},
	{ SHJ6, bidSize6		},
	{ SHJ7, bidSize7		},
	{ SHJ8, bidSize8		},
	{ SHJ9, bidSize9		},
	{ SHJA, bidSize10		},

	{ SHC1, bidBefore1		},	// 매수호가 건수
	{ SHC2, bidBefore2		},
	{ SHC3, bidBefore3		},
	{ SHC4, bidBefore4		},
	{ SHC5, bidBefore5		},
	{ SHC6, bidBefore6		},
	{ SHC7, bidBefore7		},
	{ SHC8, bidBefore8		},
	{ SHC9, bidBefore9		},
	{ SHCA, bidBefore10		},
	
	{ HTIM, cTime			},	// 호가시간

	{ DHCJ, askTotalSize		},	// 총잔량
	{ DHCC, askTotalBefore		},	// 총건수			
	{ SHCJ, bidTotalSize		},
	{ SHCC, bidTotalBefore		},
	{ DSCHA, sizeDiff		},

	{ SIGA, openPrice		},	// 시가
	{ KOGA, highPrice		},	// 고가
	{ JEGA, lowPrice		},	// 저가

	{ YSCA, prePrice		},  // 예상가
	{ YSCV, preVolume		},  // 예상체결수량  //0422

	{ DHTJ, askSizeTime		},	// 시간외
	{ DHTX, askSizeTimeBefore	},
	{ SHTJ, bidSizeTime		},
	{ SHTX, bidSizeTimeBefore	},
	{ 515,  gajungPrice		},	// 가중 평균가
	{ MGJY, infoMGJV		},	// 미결
	{ MDIF, infoMGDF		},	// 미결 대비
	{ SBAS, infoBASE		},	// 베이시스
	{ GRAT,	infoRATE		},	// 괴리율
	{ GIRG, infoIRGA		},	// 이론가
	{ NJBD,	infoNJBD		},	// 내재변동성
	{ DLTA, infoDELT		},	// 델타
	{ GAMA, infoGAMM		},	// 감마
	{ RSHG, infoRSH			},  // 실시간상한가
	{ RHHG, infoRHH			},  // 실시간하한가

	{ CURR, infoCURR		},	// 현재가
	{ DIFF,	infoDIFF		},	// 대비
	{ UDYL, infoUDYL		},	// 등락율
	{ GVOL,	infoGVOL		},	// 거래량
};
#endif
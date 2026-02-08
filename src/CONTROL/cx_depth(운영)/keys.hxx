
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

	{ DHX1, askBefore1		},	// 매도호가 잔량대비
	{ DHX2, askBefore2		},
	{ DHX3, askBefore3		},
	{ DHX4, askBefore4		},
	{ DHX5, askBefore5		},
	{ DHX6, askBefore6		},
	{ DHX7, askBefore7		},
	{ DHX8, askBefore8		},
	{ DHX9, askBefore9		},
	{ DHXA, askBefore10		},

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

	{ SHX1, bidBefore1		},	// 매수호가 잔량대비
	{ SHX2, bidBefore2		},
	{ SHX3, bidBefore3		},
	{ SHX4, bidBefore4		},
	{ SHX5, bidBefore5		},
	{ SHX6, bidBefore6		},
	{ SHX7, bidBefore7		},
	{ SHX8, bidBefore8		},
	{ SHX9, bidBefore9		},
	{ SHXA, bidBefore10		},
	
	{ HTIM, cTime			},	// 호가시간

	{ DHCJ, askTotalSize		},	// 총잔량
	{ DHXX, askTotalBefore		},
	{ SHCJ, bidTotalSize		},
	{ SHXX, bidTotalBefore		},

	{ SIGA, openPrice		},	// 시가
	{ KOGA, highPrice		},	// 고가
	{ JEGA, lowPrice		},	// 저가
	{ DIFF, preBefore		},	// 전일대비
	{ UDYL, prePercent		},	// 등락율

	{ YSCA, prePrice		},	// 예상가
	{ YSCV, preVolume		},

	{ DHTJ, askSizeTime		},	// 시간외
	{ DHTX, askSizeTimeBefore	},
	{ SHTJ, bidSizeTime		},
	{ SHTX, bidSizeTimeBefore	},
	{ 999,  gajungPrice		},	// 가중 평균가  
	{ DSCHA, sizeDiff		},
	{ GVOL, gVolume			},
	{ GAMT, gAMT			},	// 거래대금

	{ DLP1, askLp1			},	// 매도 LP 잔량
	{ DLP2, askLp2			},
	{ DLP3, askLp3			},
	{ DLP4, askLp4			},
	{ DLP5, askLp5			},
	{ DLP6, askLp6			},
	{ DLP7, askLp7			},
	{ DLP8, askLp8			},
	{ DLP9, askLp9			},
	{ DLPA, askLp10			},

	{ SLP1, bidLp1			},	// 매수 LP 잔량
	{ SLP2, bidLp2			},
	{ SLP3, bidLp3			},
	{ SLP4, bidLp4			},
	{ SLP5, bidLp5			},
	{ SLP6, bidLp6			},
	{ SLP7, bidLp7			},
	{ SLP8, bidLp8			},
	{ SLP9, bidLp9			},
	{ SLPA, bidLp10			},

	{ DLPX, askTotalLp		},	// LP 총잔량
	{ SLPX, bidTotalLp		},
	{ CPOW, chegang			},  // 체결강도
	{ SVIU, SVIUP			},  // 정적vi 예상상승가
	{ SVID, SVIDN			},  // 정적vi 예상하락가

	{ MDPRMDJ, MIDKPRCMD },  // KRX 중간가 매도잔량
	{ MDPR, MIDKPRICE },  // KRX 중간가
	{ MDPRMSJ, MIDKPRCMS },  // KRX 중간가 매수잔량
	{ MDPRMDB, MIDKPRCDBMD },  // KRX 중간가 매도직전대비
	{ MDPRMSB, MIDKPRCDBMS },  // KRX 중간가 매수직전대비
	{ MDPRRATE, MIDKPRCRATE },  //KRX 중간가 등락율

	{ MDPRMDJ, MIDNPRCMD },  //NXT 매도중간잔량
	{ MDPR, MIDNPRICE },  //NXT  중간가
	{ MDPRMSJ, MIDNPRCMS },  //NXT 매수중간잔량
	{ MDPRMDB, MIDNPRCDBMD },  //NXT 중간가매도비
	{ MDPRMSB, MIDNPRCDBMS },  //NXT 중간가매수비
	{ MDPRRATE, MIDNPRCRATE },  //NXT 중간가등락률

	{ TNMIDRATE, cticktime },  // 체결 tick 타임
	{ TNMIDRATE, ctickcurr },  // 체결 현재가
	{ TNMIDRATE, ctickvol },  // 체결거래량

	{ 29, dbongsiga },  // 일봉시가
	{ 30, dbonggoga },  // 일봉고가
	{ 31, dbongjega },  // 일봉저가
	{ 23, dbongjjga },  // 일봉종가
	{ 27, dbongvol },  // 일봉거래량

	{ TKD1VOL, kmd1size },   //KRX 	매도1호가수량
	{ TKD2VOL, kmd2size },   //KRX 	매도2호가수량
	{ TKD3VOL, kmd3size },   //KRX 	매도3호가수량     kmd3size
	{ TKD4VOL, kmd4size },   //KRX 	매도4호가수량
	{ TKD5VOL, kmd5size },   //KRX 	매도5호가수량
	{ TKD6VOL, kmd6size },   //KRX 	매도6호가수량
	{ TKD7VOL, kmd7size },   //KRX 	매도7호가수량
	{ TKD8VOL, kmd8size },   //KRX 	매도8호가수량
	{ TKD9VOL, kmd9size },   //KRX 매도9호가수량
	{ TKD10VOL, kmd10size },   //KRX 매도10호가수량

	{ TKS1VOL, kms1size },   //KRX 	매수1호가수량
	{ TKS2VOL, kms2size },   //KRX 	매수2호가수량
	{ TKS3VOL, kms3size },   //KRX 	매수3호가수량
	{ TKS4VOL, kms4size },   //KRX 	매수4호가수량
	{ TKS5VOL, kms5size },   //KRX 	매수5호가수량
	{ TKS6VOL, kms6size },   //KRX 	매수6호가수량
	{ TKS7VOL, kms7size },   //KRX 	매수7호가수량
	{ TKS8VOL, kms8size },   //KRX 	매수8호가수량
	{ TKS9VOL, kms9size },   //KRX 매수9호가수량
	{ TKS10VOL, kms10size },   //KRX 매수10호가수량

	{ TND1VOL, nmd1size },   //NXT 	매도1호가수량
	{ TND2VOL, nmd2size },   //NXT 	매도2호가수량
	{ TND3VOL, nmd3size },   //NXT 	매도3호가수량
	{ TND4VOL, nmd4size },   //NXT 	매도4호가수량
	{ TND5VOL, nmd5size },   //NXT 	매도5호가수량
	{ TND6VOL, nmd6size },   //NXT 	매도6호가수량
	{ TND7VOL, nmd7size },   //NXT 	매도7호가수량
	{ TND8VOL, nmd8size },   //NXT 	매도8호가수량
	{ TND9VOL, nmd9size },   //NXT 매도9호가수량
	{ TND10VOL, nmd10size },   //NXT 매도10호가수량

	{ TNS1VOL, nms1size },   //NXT 	매수1호가수량
	{ TNS2VOL, nms2size },   //NXT 	매수2호가수량
	{ TNS3VOL, nms3size },   //NXT 	매수3호가수량
	{ TNS4VOL, nms4size },   //NXT 	매수4호가수량
	{ TNS5VOL, nms5size },   //NXT 	매수5호가수량
	{ TNS6VOL, nms6size },   //NXT 	매수6호가수량
	{ TNS7VOL, nms7size },   //NXT 	매수7호가수량
	{ TND8VOL, nms8size },   //NXT 	매수8호가수량
	{ TNS9VOL, nms9size },   //NXT 매수9호가수량
	{ TNS10VOL, nms10size },   //NXT 매수10호가수량

	{ TKDTVOL, tkdtjan },   //통합 KRX 매도호가 총잔량
	{ TKSTVOL, tkstjan },   //통합 KRX 매수호가 총잔량
	{ TNDTVOL, tndtjan },   //통합 NXT 매도호가 총잔량
	{ TNSTVOL, tnstjan },   //통합 NXT 매수호가 총잔량  554, 228  심볼, 인덱스

	{ TKMID, tkmid },   //통합 KRX 중간가
	{ TKMIDDV, tkmiddv },   //통합 KRX 매도중간잔량
	{ TKMIDSV, tkmidsv },   //통합 KRX 매수중간잔량
	{ TKMIDDR, tkmiddr },  //통합 KRX 중간가매도비
	{ TKMIDSR, tkmidsr },   //통합 KRX 중간가매수비
	{ TKMIDRATE, tkmidrate },   //통합 KRX 중간가등락률

	{ TNMID, tnmid },   //통합 NXT 중간가
	{ TNMIDDV, tnmiddv },   //통합 NXT 매도중간잔량
	{ TNMIDSV, tnmidsv },   //통합 NXT 매수중간잔량
	{ TNMIDDR, tnmiddr },  //통합 NXT 중간가매도비
	{ TNMIDSR, tnmidsr },   //통합 NXT 중간가매수비
	{ TNMIDRATE, tnmidrate }   //통합 NXT 중간가등락률
};

struct	_keys	keyx[] = {
	{ EDHG1, askPrice1		},	// 매도호가
	{ EDHG2, askPrice2		},
	{ EDHG3, askPrice3		},
	{ EDHG4, askPrice4		},
	{ EDHG5, askPrice5		},
	{ EDHG6, askPrice6		},
	{ EDHG7, askPrice7		},
	{ EDHG8, askPrice8		},
	{ EDHG9, askPrice9		},
	{ EDHGA, askPrice10		},

	{ EDHJ1, askSize1		},	// 매도호가 잔량
	{ EDHJ2, askSize2		},
	{ EDHJ3, askSize3		},
	{ EDHJ4, askSize4		},
	{ EDHJ5, askSize5		},
	{ EDHJ6, askSize6		},
	{ EDHJ7, askSize7		},
	{ EDHJ8, askSize8		},
	{ EDHJ9, askSize9		},
	{ EDHJA, askSize10		},

	{ EDHX1, askBefore1		},	// 매도호가 잔량대비
	{ EDHX2, askBefore2		},
	{ EDHX3, askBefore3		},
	{ EDHX4, askBefore4		},
	{ EDHX5, askBefore5		},
	{ EDHX6, askBefore6		},
	{ EDHX7, askBefore7		},
	{ EDHX8, askBefore8		},
	{ EDHX9, askBefore9		},
	{ EDHXA, askBefore10		},

	{ ESHG1, bidPrice1		},	// 매수호가
	{ ESHG2, bidPrice2		},
	{ ESHG3, bidPrice3		},
	{ ESHG4, bidPrice4		},
	{ ESHG5, bidPrice5		},
	{ ESHG6, bidPrice6		},
	{ ESHG7, bidPrice7		},
	{ ESHG8, bidPrice8		},
	{ ESHG9, bidPrice9		},
	{ ESHGA, bidPrice10		},

	{ ESHJ1, bidSize1		},	// 매수호가 잔량
	{ ESHJ2, bidSize2		},
	{ ESHJ3, bidSize3		},
	{ ESHJ4, bidSize4		},
	{ ESHJ5, bidSize5		},
	{ ESHJ6, bidSize6		},
	{ ESHJ7, bidSize7		},
	{ ESHJ8, bidSize8		},
	{ ESHJ9, bidSize9		},
	{ ESHJA, bidSize10		},

	{ ESHX1, bidBefore1		},	// 매수호가 잔량대비
	{ ESHX2, bidBefore2		},
	{ ESHX3, bidBefore3		},
	{ ESHX4, bidBefore4		},
	{ ESHX5, bidBefore5		},
	{ ESHX6, bidBefore6		},
	{ ESHX7, bidBefore7		},
	{ ESHX8, bidBefore8		},
	{ ESHX9, bidBefore9		},
	{ ESHXA, bidBefore10		},
	
	{ EHTIM, cTime			},	// 호가시간

	{ EDHCJ, askTotalSize		},	// 총잔량
	{ EDHXX, askTotalBefore		},
	{ ESHCJ, bidTotalSize		},
	{ ESHXX, bidTotalBefore		},

	{ ESIGA, openPrice		},	// 시가
	{ EKOGA, highPrice		},	// 고가
	{ EJEGA, lowPrice		},	// 저가
	{ EDIFF, preBefore		},	// 전일대비
	{ EUDYL, prePercent		},	// 등락율

	{ EYSCA, prePrice		},	// 예상가
	{ EYSCV, preVolume		},

	{ EDHTJ, askSizeTime		},	// 시간외
	{ EDHTX, askSizeTimeBefore	},
	{ ESHTJ, bidSizeTime		},
	{ ESHTX, bidSizeTimeBefore	},
	{ DSCHA, sizeDiff		},
	{ EGVOL, gVolume		},
	{ EGAMT, gAMT			},	// 거래대금

	{ TKD1VOL, kmd1size			},   //KRX 	매도1호가수량
	{ TKD2VOL, kmd2size			},   //KRX 	매도2호가수량
	{ TKD3VOL, kmd3size			},   //KRX 	매도3호가수량
	{ TKD4VOL, kmd4size			},   //KRX 	매도4호가수량   
	{ TKD5VOL, kmd5size			},   //KRX 	매도5호가수량
	{ TKD6VOL, kmd6size			},   //KRX 	매도6호가수량
	{ TKD7VOL, kmd7size			},   //KRX 	매도7호가수량   
	{ TKD8VOL, kmd8size			},   //KRX 	매도8호가수량
	{ TKD9VOL, kmd9size },   //KRX 매도9호가수량
	{ TKD10VOL, kmd10size },   //KRX 매도10호가수량

	{ TKS1VOL, kms1size },   //KRX 	매수1호가수량
	{ TKS2VOL, kms2size },   //KRX 	매수2호가수량
	{ TKS3VOL, kms3size },   //KRX 	매수3호가수량
	{ TKS4VOL, kms4size },   //KRX 	매수4호가수량
	{ TKS5VOL, kms5size },   //KRX 	매수5호가수량
	{ TKS6VOL, kms6size },   //KRX 	매수6호가수량
	{ TKS7VOL, kms7size },   //KRX 	매수7호가수량
	{ TKS8VOL, kms8size },   //KRX 	매수8호가수량
	{ TKS9VOL, kms9size },   //KRX 매수9호가수량
	{ TKS10VOL, kms10size },   //KRX 매수10호가수량

	{ TND1VOL, nmd1size },   //NXT 	매도1호가수량
	{ TND2VOL, nmd2size },   //NXT 	매도2호가수량
	{ TND3VOL, nmd3size },   //NXT 	매도3호가수량
	{ TND4VOL, nmd4size },   //NXT 	매도4호가수량
	{ TND5VOL, nmd5size },   //NXT 	매도5호가수량
	{ TND6VOL, nmd6size },   //NXT 	매도6호가수량
	{ TND7VOL, nmd7size },   //NXT 	매도7호가수량
	{ TND8VOL, nmd8size },   //NXT 	매도8호가수량
	{ TND9VOL, nmd9size },   //NXT 매도9호가수량
	{ TND10VOL, nmd10size },   //NXT 매도10호가수량

	{ TNS1VOL, nms1size },   //NXT 	매수1호가수량
	{ TNS2VOL, nms2size },   //NXT 	매수2호가수량
	{ TNS3VOL, nms3size },   //NXT 	매수3호가수량
	{ TNS4VOL, nms4size },   //NXT 	매수4호가수량
	{ TNS5VOL, nms5size },   //NXT 	매수5호가수량
	{ TNS6VOL, nms6size },   //NXT 	매수6호가수량
	{ TNS7VOL, nms7size },   //NXT 	매수7호가수량
	{ TND8VOL, nms8size },   //NXT 	매수8호가수량
	{ TNS9VOL, nms9size },   //NXT 매수9호가수량
	{ TNS10VOL, nms10size },   //NXT 매수10호가수량

	{ TKDTVOL, tkdtjan },   //통합 KRX 매도호가 총잔량
	{ TKSTVOL, tkstjan },   //통합 KRX 매수호가 총잔량
	{ TNDTVOL, tndtjan },   //통합 NXT 매도호가 총잔량
	{ TNSTVOL, tnstjan }   //통합 NXT 매수호가 총잔량
};


#endif



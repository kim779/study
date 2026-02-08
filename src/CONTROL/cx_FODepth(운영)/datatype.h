
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
//  Components   : datatype.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2003-07	Initial version
// *****************************************************************************

#ifndef _DATATYPE_
#define _DATATYPE_

#define	WM_SCRL		WM_USER+100	// scroll message by Contract
#define	SCRL_VCHANGE	1		// change vscroll by Contract
#define	WM_MBONG	WM_USER+200	// MBong message by lbuttondown

struct	_price {
	char	ask[8];			// 매도호가
	char	bid[8];			// 매수호가
	char	avol[9];		// 매도호가수량
	char	bvol[9];		// 매수호가수량
	char	acha[9];		// 직전매도대비수량
	char	bcha[9];		// 직전매수대비수량
};


struct	_hoga {
	char	codx[12];		// CODX
	char	curr[9];		// 현재가
	char	pcls[8];		// 전일종가
	char	gvol[12];		// 거래량
	char	gamt[12];		// 거래래금
        char    siga[8];                // 시가
        char    koga[8];                // 고가
        char    jega[8];                // 저가
        char    amga[8];                // 액면가
	char	ysga[8];		// 예상체결가 / 전일 종가
	char	yvol[9];		// 예상체결수량 / 전일 거래량
	char	htim[8];		// 호가시간
	char    gjga[8];                // 기준가
        char    pgga[8];                // 가중평균가
        char    sjrt[6];                // 소진율       999V99
        char    shga[8];                // 상한가
        char    hhga[8];                // 하한가
	struct	_price	price[10];	// 10호가 0:우선호가 1,2,:차,차
	char	avols[9];		// 매도호가총수량
	char	bvols[9];		// 매수호가총수량
	char	ahvol[9];		// 시간외매도수량
	char	bhvol[9];		// 시간외매수수량
	char	acha[9];		// 매도호가총수량 직전대비
	char	bcha[9];		// 매수호가총수량 직전대비
	char	ahvc[9];		// 시간외매도수량 직전대비
	char	bhvc[9];		// 기간외매수수량 직전대비
	char	dscha[9];		// 잔량차
	char	gsm5[9];		// 4일 종가합
	char	gsm10[9];		// 9일 종가합
	char	jivl[12];		// 전일 거래량
	char	jggy[6];		// 증거금율
	char	res2[9];		// 2차저항
	char	res1[9];		// 1차저항
	char	pivt[9];		// 피봇값
	char	spp1[9];		// 1차지지
	char	spp2[9];		// 2차지지 
	char	resd[9];		// D저항
	char	sppd[9];		// D지지
};
#define sz_hoga	sizeof(struct _hoga)

struct	_hogax {
	char	codx[12];
	char	curr[8];
	char	diff[8];		// 대비
	char	udyl[8];		// 등락율
	char	pcls[8];
	char	gvol[12];
	char	gamt[12];
        char    siga[8];
        char    koga[8];
        char    jega[8];
        char    amga[8];
	char	ysga[8];		
	char	yvol[9];	
	char	htim[8];
	char    gjga[8];                // 기준가
        char    pgga[8];                // 가중평균가
        char    sjrt[6];                // 소진율       999V99
        char    shga[8];                // 상한가
        char    hhga[8];                // 하한가
	struct	_price	price[5];	// 5호가
	char	avols[9];
	char	bvols[9];
	char	ahvol[9];
	char	bhvol[9];
	char	acha[9];
	char	bcha[9];
	char	ahvc[9];
	char	bhvc[9];
	char	dscha[9];		// 잔량차
	char	mgjv[9];		/* 미결재 수량			*/
	char	mgdf[9];		/* 미결제 증감			*/
	char	mgsi[9];		/* 미결제 시량			*/
	char	mgko[9];		/* 미결제 고량			*/
	char	mgje[9];		/* 미결제 저량			*/
	char	ma05[9];		/* 5MA				*/
	char	sum4[9];		/* sum of 4 day			*/
	char	ma10[9];		/* 10MA				*/
	char	sum9[9];		/* sum of 9 day			*/
	char	base[9];		/* 베이시스  for 선물		*/
	char	rate[6];		/* 괴리율    for 선물		*/
	char	irga[9];		/* 이론가			*/
	char	njbd[9];		/* 내재변동성		*/
	char	delt[9];		/* delta			*/
	char	gamm[9];		/* gamma			*/
	char	msha[9];		/* 실시간 상한가 */
	char	mhha[9];		/* 실시간 하한가 */
};
#define sz_hogax	sizeof(struct _hogax)

struct _Contract			// 체결 정보 20080425
{
	char	time[8];
	char	price[8];
	char	contract[9];
};

struct _Candle				// 미니봉 정보 20080425
{
	char	date[8];
	char	siga[8];
	char	koga[8];
	char	jega[8];
	char	jgga[8];
	char	gvol[12];
};

struct _EW_data
{
	char	nContract[2];			// 체결수
	char	nCandle[2];			// 캔들수
	struct _Contract	contract[15];	// 체결 컨트롤
	struct _Candle		candle[30];	// 미니봉차트
};
#define	sz_Extra	sizeof(struct _EW_data)


struct  grid_i {
        char    indx[1];                // index
        char    gubn[1];                // 조건구분
                                        // 1: 기준가
                                        // 2: 기간
                                        // 3: Pivot
                                        // 4: 이동평균선
        char    dat1[1];
        char    dat2[8];
};
#define	sz_gridI	sizeof(struct grid_i)

struct  grid_o {
        char    indx[1];                // index
        char    gubn[1];                // 조건구분
        char    vals[8];
};
#define sz_gridO	sizeof(struct grid_o)

#endif

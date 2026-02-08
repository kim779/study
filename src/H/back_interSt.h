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
//  Components   : interStruct.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2002-10	Initial version
// *****************************************************************************
//
// 
//
#pragma once
#pragma pack(1)
//
//	user/userName폴더
//	file name : portfolio.i00 ~ portfolio.i99
//

struct _inters
{
	char	gubn{};		// 종목구분
				// 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char	code[12]{};	// 종목코드
	char	name[30]{};	// 종목명
	int	possNum{};	// 보유수량
	double	possPrice{};	// 보유단가
	char	memo[30]{};	// 메모
	char	o_check{};	// 주문 실행여부
				// 0:none, 1:checked
	char	o_kind{};		// 주문구분
				// 1:매도, 2:매수
	char	o_mmgb{};		// 매매구분
				// 1:보통, 2:시장가
	int	o_num{};		// 주문수량
	double	o_price{};	// 주문단가
	char	a_kind{};		// 알람 설정
				// 1:그림만, 2:그림과 소리
	int	a_num{};		// 조건 항목 갯수
	char	a_condition{};	// 알람조건 1:OR, 2:AND
	char	filler[99]{};	// Reserved
};

struct _alarms
{
	char	a_kind{};		// 감시항목
				// 0:none, 1:현재가, 2:매도호가, 3:매수호가, 4:등락률, 5:손익평가, 6:수익률
	double	a_value{};	// 감시 비교값
	char	a_condition{};	// 감시 조건
				// 1:>=, 2:>, 3:=, 4:<, 5:<=
	char	filler[40]{};	// Reserved
};

/////////////////////// 관심종목 ///////////////////////
#define INTER_UP	1
#define INTER_DOWN	2
#define INTER_RANK	3
#define INTER_GUBUN	"MY"
#define GUBUN_LEN	2

struct  usrinfo {
       char    gubn[2]{};                /* 업무 구분                    */
                                       /* MY: 관심 종목                */
                                       /* UL: 사용자 데이타 목록       */
                                       /* UD: 사용자 데이타            */
       char    dirt[1]{};                /* 전송 방향                    */
                                       /* U: PC=>HOST  D: HOST=>PC     */
       char    cont[1]{};                /* 연속 구분                    */
                                       /* F: First     M: Middle       */
                                       /* L: Last      C: Cancel       */
       char    name[80]{};               /* file name                    */
       char    nblc[5]{};                /* 블럭 갯수                    */
                                       /* 화일 SIZE (max. 64K)         */
       char    retc[1]{};                /* O: 정상      E: 에러         */
       char    emsg[40]{};               /* error message                */
};

#define INTER_TRCODE	"chhomyst"
#define MAX_GR	100			/* 최대 관심그룹 갯수		*/
#define MAX_JM  120                     /* 최대 관심종목 갯수           */

struct jlist {				/* 관심종목                     */
        char    code[12]{};               /* 종목코드                     */
                                        /* 현물(6), 선물/옵션(8)        */
                                        /* 하이일드펀드(9)              */
                                        /* 업종코드(5)                  */
                                        /* 보유종목만 해당(수량 + 가격) */
        char    sqty[9]{};                /* 보유주식수량                 */
        char    msga[9]{};                /* 매수가격                     */
};

struct  fold {				/* 관심종목                     */
        char    fcod[2]{};                /* 관심그룹코드 (00 ~ 99)       */
        char    name[20]{};               /* 관심그룹명                   */
        char    nrec[4]{};                /* 종목갯수(space 포함)         */
        struct  jlist   jlist[MAX_JM]{};
};

struct  _usrmid     {
	struct  usrinfo usrinfo{};
	struct  fold    fold{};	
};

struct  _usrmod     {
        struct  usrinfo usrinfo{};
        struct  fold    fold{};
};

////// 등락순위
#define RANK_TRCODE	"chbojrnk"

struct  rank_mid     {
        char    gubn[2]{};                /* 조회구분                     */
        char    mark[1]{};                /* 시장구분                     */
                                        /* 1 : KOSPI,   2 : KOSDAQ      */
                                        /* 3 : 제3시장                  */
};

struct  rank_grid    {
        char    code[6]{};                /* 종목코드                     */
        char    hnam[16]{};               /* 종목명                       */
};

struct  rank_mod     {
        char    nrec[4]{};                /* # of records                 */
        struct  rank_grid    grid[1]{};
};
#pragma pack()
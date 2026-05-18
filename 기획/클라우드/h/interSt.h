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
#if !defined(INTER_H__58CA3F42_F818_496D_BB4E_8B978E6BE3D6__INCLUDED_)
#define INTER_H__58CA3F42_F818_496D_BB4E_8B978E6BE3D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
//	user/userName폴더
//	file name : portfolio.i00 ~ portfolio.i99
//

struct _inters
{
	char	gubn[1];	// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char	code[12];	// 종목코드
	char	name[32];	// 종목명
	char	xprc[10];	// 보유단가
	char	xnum[10];	// 보유수량
	char	xupnum[16];	// 상장 주식수
	char	filler[24];	// Reserved
};

#define	sz_inters	sizeof(struct _inters)

struct _codeinfo
{
	char	code[12];	// 종목코드
	char	name[32];	// 종목명
	char	capture[1];	// 실행여부 	'0':포착전, '1':포착완료
	
	struct _alarm
	{
		char	kind[1];// 감시항목	0:none, 1:현재가, 2:전일대비, 3:매도호가, 4:매수호가, 5:등락률, 6:거래량
		char	aval[10];//감시비교값
		char	cond[1];// 감시조건	'0':이상, '1':이하
		char	oper[1];// 감시연산	'0':AND,  '1':OR
	}alarm[6];

	char	o_run[1];	// 주문창	'0':미실행,'1':실행
	char	o_kind[1];	// 주문구분	'0':매도,  '1':매수
	char	o_mmgb[1];	// 매매구분	'0':지정가,'1':시장가
	char	o_num[10];	// 주문수량
	char	o_prc[10];	// 주문단가

	char	a_kind[1];	// 알람설정	0x01: 확인창, 0x02: 관심종목신호적용 0x04:소리적용
	char	a_wave[128];	// 소리파일
	char	filler[20];	// Reserved
};

#define	sz_codeinfo	sizeof(struct _codeinfo)

struct	_hisinfo
{
	char	date[14];	// 포착일자,시간
	struct	_codeinfo	codeinfo;
	char	gval[6][10];	// 포착시 가격
	char	chk[6];		// check
};
#define	sz_hisinfo	sizeof(struct _hisinfo)

#endif // !defined(INTER_H__58CA3F42_F818_496D_BB4E_8B978E6BE3D6__INCLUDED_)
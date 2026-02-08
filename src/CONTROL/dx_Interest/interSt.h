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
	char	gubn;		// 종목구분
				// 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char	code[12];	// 종목코드
	char	name[30];	// 종목명
	int	possNum;	// 보유수량
	double	possPrice;	// 보유단가
	char	memo[30];	// 메모
	char	o_check;	// 주문 실행여부
				// 0:none, 1:checked
	char	o_kind;		// 주문구분
				// 1:매도, 2:매수
	char	o_mmgb;		// 매매구분
				// 1:보통, 2:시장가
	int	o_num;		// 주문수량
	double	o_price;	// 주문단가
	char	a_kind;		// 알람 설정
				// 1:그림만, 2:그림과 소리
	int	a_num;		// 조건 항목 갯수
	char	filler[100];	// Reserved
};

struct _alarms
{
	char	a_kind;		// 감시항목
				// 0:none, 1:현재가, 2:매도호가, 3:매수호가, 4:등락률, 5:손익평가, 6:수익률
	double	a_value;	// 감시 비교값
	char	a_condition;	// 감시 조건
				// 1:>=, 2:>, 3:=, 4:<, 5:<=
	char	filler[40];	// Reserved
};

#endif // !defined(INTER_H__58CA3F42_F818_496D_BB4E_8B978E6BE3D6__INCLUDED_)
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
//  Components   : uscode.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2002-10	Initial version
// *****************************************************************************
//
// 종목 symbol file
//

struct	_uscode	{
	char	code[16];		/*	종목 symbol	*/
	char	name[40];		/*	종목 명		*/
	char	exgb[1];		/*	exchange 구분	*/
	char	jmgb[1];		/*	종목 구분	*/
	char	fill[22];		/*	filler		*/
};

struct	_myUScode	{
	CString		code;		/*	종목 symbol	*/
	CString		name;		/*	종목 명		*/
	CString		fill;		/*	filler		*/
	char		exgb;		/*	exchange 구분	*/
	char		jmgb;		/*	종목 구분	*/
};

//	echange 구분	exgb
//	'A'	:	CME
//
//	종목 구분	jmgb
//	'F'	:	Future
//	'I'	:	Index
//	'O'	:	Option
//	'C'	:	Combination (No Legs)
//	'L'	:	Combination (Legs)


// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2000
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic,
// mechanical, or otherwise without the written permission of Winix Information Inc.
//
//  Components   : mapform.h	AXIS MAP format
//  Rev. History :
//  		  Ver	 Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
//		 02.00	2001-04 Update  version
//		 03.00	2003-01 Update  version (P/F Upgrade)
//		 03.01	2007-12 Update  version (Header Upgrade)
// *****************************************************************************

#ifndef	_MAPH_
#define	_MAPH_

#define	L_SECB		4		// map security byte length

#include "axisvar.h"
#include "form.h"

#pragma	pack(1)
struct	_mapH	{
	BYTE	vers;			// version

	BYTE	absS;			// absolute security
	BYTE	incS[L_SECB];		// inclusive security
	BYTE	excS[L_SECB];		// exclusive security

	WORD	height;			// client area height
	WORD	width;			// client area width

	BYTE	type;			// view type

	BYTE	fStyle;			// font style
	BYTE	fPoint;			// font size : point
	DWORD	fName;			// font name

	DWORD	options;		// MAP options

	DWORD	flash;			// rts type : Uppercase type ('A'-'Z')
	DWORD	flash2;			// rts type : Lowercase type ('a'-'z')

	DWORD	caption;		// window title;short title(tab)
	char	trxC[L_TRXC];		// transaction code
	BYTE	toPath;			// routing destination path

	BYTE	typeH;			// header type
	char	trxH[32];		// special transaction header
	BYTE	mapM;			// MAP method

	char	mapN[L_MAPN];		// MAP name
	DWORD	helpN;			// help name (.chm)
	DWORD	publicR;		// public properties (name1 + \t + name2 + \t +...)

					// MAP time values
	WORD	repeatV;		// repeat transaction interval (sec./1000)

	WORD	formN;			// # of object FORMs
	BYTE	imageK;			// bground kind
	DWORD	imageV;			// bground value
					// if imageK == IK_BRUSH, palette index
					// if imageK == IK_BMP, filename offset
	BYTE	rsv[32];		// reserved

	DWORD	formR;			// FORM record
	DWORD	cellR;			// grid, scroll action record
	DWORD	pageR;			// tab page record
	DWORD	valueR;			// page item record
	DWORD	scriptR;		// script
	DWORD	strR;			// string record
};
#pragma	pack()

#define	L_MAPH	sizeof(struct _mapH)

//
//	encrypt
//
#define	iAXIS		10		// fixed field size : vers(1) + absolute security(1) + inclusive security(4) + exclusive security(4)
#define	sKEY		"@IBKS@"	// encrypt check key
#define	iKEY		6		// key size

//
//	_mapH.type
//
#define	VW_NORM		0		// normal - resizable forms
#define	VW_SCROLL	1		// fixed size forms : scroll
#define	VW_RESIZE	2		// resizable view, fixed size forms
#define	VW_WND		3		// CWnd view
#define	VW_FIXED	4		// fixed size forms
#define	VW_FEX		5		// fixed size forms : non-scroll
#define	VW_REX		6		// resizable view, fixed vR
#define VW_PROC		9		// procedure MAP


//
//	_mapH.fStyle
//	can specify one of the _formR.fStyle values


//
//	_mapH.toPath
//
#define	TP_AXIS		0		// routing main BP (AXIS)
#define	TP_BPn(n)	(TP_AXIS+n)	// routing to secondary BP
					// 1 ~ 7
#define	TP_FEP		80		// routing to FEP
#define	maxBPs		8		// # of AXIS/SERVER


//
//	_mapH.options
//
#define	OP_OOP		0x00000001	// OOP transaction
#define	OP_CLS		0x00000002	// clear forms
#define	OP_CNV		0x00000004	// code conversion

#define	OP_DOMINO	0x00000008	// domino
#define	OP_FLASH	0x00000010	// flash map
#define	OP_TABS		0x00000020	// separator TAB TR
#define	OP_CR2TB	0x00000040	// ENTER -> TAB key action

#define	OP_DROP		0x00000080	// enable FA_DRAG drop
#define	OP_SAVE		0x00000100	// MAP information save to FILE
#define	OP_INTER	0x00000200	// unit/object cursor wrap

#define	OP_ENC		0x00000400	// encode TR
#define	OP_CERTIFY	0x00000800	// certify

#define	OP_OOP2		0x00001000	// OOP2 transaction

#define	OP_USER		0x00002000	// user reditable MAP
#define	OP_XFONT	0x00004000	// fixed font

#define	OP_XPRINT	0x00008000	// no print menu(mouse right button)

// updateXX_20221223
#define OP_ENCCOMFILE	0x00010000      // map binary encrypt bit flag


//
//	_mapH.flash
//
#define	FLASH_A		0x00000001	// 'A' type
#define	FLASH_B		0x00000002	// 'B' type
#define	FLASH_C		0x00000004	// 'C' type
#define	FLASH_D		0x00000008	// 'D' type
#define	FLASH_E		0x00000010	// 'E' type
#define	FLASH_F		0x00000020	// 'F' type
#define	FLASH_G		0x00000040	// 'G' type
#define	FLASH_H		0x00000080	// 'H' type
#define	FLASH_I		0x00000100	// 'I' type
#define	FLASH_J		0x00000200	// 'J' type
#define	FLASH_K		0x00000400	// 'K' type
#define	FLASH_L		0x00000800	// 'L' type
#define	FLASH_M		0x00001000	// 'M' type
#define	FLASH_N		0x00002000	// 'N' type
#define	FLASH_O		0x00004000	// 'O' type
#define	FLASH_P		0x00008000	// 'P' type
#define	FLASH_Q		0x00010000	// 'Q' type
#define	FLASH_R		0x00020000	// 'R' type
#define	FLASH_S		0x00040000	// 'S' type
#define	FLASH_T		0x00080000	// 'T' type
#define	FLASH_U		0x00100000	// 'U' type
#define	FLASH_V		0x00200000	// 'V' type
#define	FLASH_W		0x00400000	// 'W' type
#define	FLASH_X		0x00800000	// 'X' type
#define	FLASH_Y		0x01000000	// 'Y' type
#define	FLASH_Z		0x02000000	// 'Z' type


//
//	_mapH.flash2
//
#define	FLASH_a		0x00000001	// 'a' type
#define	FLASH_b		0x00000002	// 'b' type
#define	FLASH_c		0x00000004	// 'c' type
#define	FLASH_d		0x00000008	// 'd' type
#define	FLASH_e		0x00000010	// 'e' type
#define	FLASH_f		0x00000020	// 'f' type
#define	FLASH_g		0x00000040	// 'g' type
#define	FLASH_h		0x00000080	// 'h' type
#define	FLASH_i		0x00000100	// 'i' type
#define	FLASH_j		0x00000200	// 'j' type
#define	FLASH_k		0x00000400	// 'k' type
#define	FLASH_l		0x00000800	// 'l' type
#define	FLASH_m		0x00001000	// 'm' type
#define	FLASH_n		0x00002000	// 'n' type
#define	FLASH_o		0x00004000	// 'o' type
#define	FLASH_p		0x00008000	// 'p' type
#define	FLASH_q		0x00010000	// 'q' type
#define	FLASH_r		0x00020000	// 'r' type
#define	FLASH_s		0x00040000	// 's' type
#define	FLASH_t		0x00080000	// 't' type
#define	FLASH_u		0x00100000	// 'u' type
#define	FLASH_v		0x00200000	// 'v' type
#define	FLASH_w		0x00400000	// 'w' type
#define	FLASH_x		0x00800000	// 'x' type
#define	FLASH_y		0x01000000	// 'y' type
#define	FLASH_z		0x02000000	// 'z' type


//
//	_mapH.typeH
//
#define	TH_NOP		0		// does not used specified header
#define	TH_KOSCOM	1		// KOSCOM header insert
#define	TH_SCUP		2		// IBM SCUP header insert
#define	TH_4702		3		// IBM 4702 header insert
#define	TH_TUXEDO	4		// TUXEDO header insert
#define	TH_TOPEND	5		// TOPEND header insert(02.03)
#define	TH_LEDGER	6		// LEDGER header insert


//
//	_mapH.mapM
//
#define	MM_FORM		0		// default MAP method rule
#define	MM_MENU		1		// menu
#define	MM_SEND		2		// auto send


//
//	_mapH.imageK
//
#define	IK_NOP		0		// NULL
#define	IK_BRUSH	1		// brush
#define	IK_BMP		2		// bitmap


//
//	MAP format block diagram
//
//	+----------------------------+
//	|                            |
//	|          HEADER            |
//	|                            |
//	|----------------------------|
//	|                            |
//	|          FORMS             |
//	|                            |
//	|----------------------------|
//	|                            |
//	|        CELL RECORD         |
//	|                            |
//	|----------------------------|
//	|                            |
//	|        PAGE RECORD         |
//	|                            |
//	|----------------------------|
//	|                            |
//	|        VALUE RECORD        |
//	|                            |
//	|----------------------------|
//	|                            |
//	|        SCRIPT RECORD       |
//	|                            |
//	|----------------------------|
//	|                            |
//	|        STRING RECORD       |
//	|                            |
//	+----------------------------+

#endif

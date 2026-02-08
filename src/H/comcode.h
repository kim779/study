// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2000
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic, 
// mechanical, or otherwise without the written permission of Winix Information.
//
//  Components   : comcode.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2005-07	Initial version
// *****************************************************************************

#pragma once

#pragma pack(1)
#define SCODE_FN	"sjcode.dat"		// stock symbol file
#define CFCODE_FN	"cjcode.dat"		// stock category file
#define KSCODE_FN	"kscode.dat"		// index symbol file
#define FCODE_FN	"fcode.dat"		// future symbol file
#define OCODE_FN	"ocode.dat"		// option symbol file

//
//	Market
//
#define M_TSE		'0'	
#define M_OTC		'1'
#define M_EGM		'2'
#define M_FUTURE	'4'
#define M_SIMEX		'5'
#define M_OPTION	'6'

//
//	Stock(TSE/OTC)
//
#define L_SCOD		6			// stock code length
#define L_SNAM		6			// stock name length
#define L_CNAM		15			// category name length
#define L_MNAM		7			// market name length
#define L_INAM		20			// index name length

//
//	Stock symbol
//
struct	scode
{
	char	scod[L_SCOD]{};
	char	name[L_SNAM]{};
	char	mket{};				// market kind
	char	kind[2]{};			// category code
	char	resv[4]{};			// reserve
};


//
//	Category file header
//
struct	cfhead
{
	char	mknd{};
	char	mket[2]{};			// number of category
	char	name[L_MNAM]{};
};

//
//	Category
//
struct	cfcode
{
 	char	name[L_CNAM]{};
	char	kind[2]{};
};


//
//	Index symbol
//
struct	kscode
{
	char	mket{};
	char	code[L_SCOD]{};
	char	name[L_INAM]{};	
};


//
//	Futures
//
#define L_FCOD		12			// Futures code length
#define L_FNAM		36			// Futures name length
#define L_CCOD		6 			// Commodity code length
#define L_NAME		10			// Commodity name length

//
//	Futures symbol
//
struct	fcomm					// occurs (ncom times)
{
	char	comd[L_CCOD]{};			// Commodity code
	char	name[L_NAME]{};			// Commodity name
	char	enty{};				// underlying Index  1.ку 0.ўн
	char	ncod[4]{};			// symbol count
	char	nday[2]{};			// eday count
	char	eday[12][6]{};			// expired year month
};

struct	frec
{
	char	code[L_FCOD]{};			// Future code
	char	name[L_FNAM]{};			// Future name
};

struct	fcode
{
	// Commodity header
	char	ncom[4]{};			// Commodity count
	struct	fcomm	fcomm{};

	// Symbol information
	struct	frec	frec{};
};


// 
//	Option
//
#define L_OCOD		12			// Option code length
#define L_ONAM		36			// Option name length

//
//	Option symbol
//
struct	ocomm					// Commodity code info
{
	char	comd[L_CCOD]{};			// Commodity code
	char	entt[L_NAME]{};			// Commodity name
	char	nday[2]{};			// eday count
	char	eday[12][6]{};			// expired year month
};

struct	rec					// Option symbol info
{
	char	isok{};				// Symbol exist 1. ку 0.ўн
	char	yymm[6]{};			// Mature YearMonth
	char	code[L_OCOD]{};			// Options code
	char	name[L_ONAM]{};			// Options name
};

struct	orec
{
	char	sprc[5]{};			// strike price
	char	atmf{};				// 1: atm
	struct	rec	call[5]{};		// call symbols record
	struct	rec	put[5]{};			// put	symbols record
};

struct	ocode	
{
	// Commodity header
	char	ncom[4]{};			// commodity count
	struct	ocomm	ocomm{};			// occurs (ncom times)

	// Symbol code information sort by strike price
	char	comd[L_CCOD]{};			// TXO, TEO, TFO, AA...
	char	nprc[4]{};			// # of sprc table
	struct	orec	orec{}; 			// occurs (nprc times)
};
#pragma pack()

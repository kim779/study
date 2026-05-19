
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
// *****************************************************************************

#ifndef	_MAPH_H
#define	_MAPH_H

#define	L_SECB		4		// map security byte length

#include "axisvar.h"
#include "form_w.h"

#define _last

#pragma	pack(1)
struct	_premapH	{
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

#define	L_MAPH	sizeof(struct _premapH)




#pragma	pack(1)
struct	_mapH	{

	BYTE	type;			// view type

	WORD	height;			// client area height
	WORD	width;			// client area width

	WORD	fStyle;			// font style
	WORD	fPoint;			// font point
	char	fName[L_NAMEs];		// font name

	DWORD	options;		// MAP options

	char	caption[80];		// window title
	char	trxC[L_TRXC+1];		// transaction code
	BYTE	toPath;				// routing destination path

	BYTE	mapM;			// MAP method
	BYTE	mapK;			// MAP kind : normal, template

	char	mapN[L_MAPN+1];		// MAP name
	char	mapdesc[20+1];		// nickname

	WORD	repeatV;		// repeat transaction interval (sec./1000)

	DWORD	flashClr;		// flash palette index
	char	flashSym[256];		// flash key string (comma delimiter)

	DWORD	upClr;			// conditional UP palette index
	DWORD	dnClr;			// conditional DOWN palette index
	char	contrastN[128];		// contrast image filename

	BYTE	bgK;			// bground kind
	char	bgFileN[64];		// bground image filename
	DWORD	bgClr;			// bground palette index

	bool	pathsetted;		// PC file path setted
	bool	modified;		// map modified
	bool	source;			// if true, source file exist
	bool	compile;		// if true, map is compiled
	BYTE	formtype;		// form type
	char	fullN[128];		// PC file full path

	WORD	formN;			// # of object FORMs
	struct	_formR* formR;		// linked list next form point
	void	*pUndoMap;		// CMapPtrToPtr with CUndoArr
/*
//	bool	onLoad;			// if true, onLOAD script defined (otherwise, not defined)
	char*	onLoadN;		// temporary onSTART script file name

//	bool	onSend;			// if true, onSEND script defined (otherwise, not defined)
	char*	onSendN;		// temporary onSEND script file name
	
//	bool	onReceive;		// if true, onRECEIVE script defined (otherwise, not defined)
	char*	onReceiveN;	// temporary onRECEIVE script file name

//	bool	onAlert;		// if true, onALERT script defined (otherwise, not defined)
	char*	onAlertN;		// temporary onALERT script file name

//	bool	onTimer;		// if true, onTIMER script defined (otherwise, not defined)
	char*	onTimerN;		// temporary onTIMER script file name

//	bool	onClose;		// if true, onCLOSE script defined (otherwise, not defined)
	char*	onCloseN;		// temporary onCLOSE script file name

//	bool	onDevice;		// if true, onDEVICE script defined (otherwise, not defined)
	char*	onDeviceN;		// temporary onDEVICE script file name

//	bool	onError;		// if true, onERROR script defined (otherwise, not defined)
	char*	onErrorN;		// temporary onERROR script file name

//	bool	onFlicking;		// if true, onFLICKING script defined (otherwise, not defined)
	char*	onFilickingN;	// temporary onFLICKING script file name

//	bool	onDeclaration;		// if true, onDeclaration script defined (otherwise, not defined)
	char*	onDeclarationN;	// temporary onDeclaration script file name

//	bool	onInDeclaration;	// if true, onInDeclaration script defined (otherwise, not defined)
	char*	onInDeclarationN;	// temporary onInDeclaration script file name
	*/
};
#pragma	pack()



//
//	_mapH.type
//
#define	VW_NORM		0		// normal - resizable forms
#define	VW_SCROLL	1		// fixed size forms : scroll
#define	VW_RESIZE	2		// resizable view, fixed size forms
#define	VW_WND		3		// CWnd view
#define	VW_FIXED	4		// fixed size forms
#define	VW_FEX		5		// fixed size forms : non-scroll
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



#define	OP_LEDGER	0x00000001	// LEDGER header insert

#ifdef _last
					//
//	_mapH.options
//

#define	OP_OOP		0x00000002	// OOP transaction
#define	OP_FLASH	0x00000004	// flash map
#define	OP_DOMINO	0x00000008	// domino
#define	OP_TABS		0x00000010	// separator TAB TR
#define	OP_CR2TB	0x00000020	// ENTER -> TAB key action
#define	OP_ENC		0x00000040	// encode TR
#define	OP_CERTIFY	0x00000080	// certify
#define	OP_SAVE		0x00000100	// MAP information save to FILE



// not use 
#define	OP_CLS		0x00000200	// clear forms
#define	OP_XFONT	0x00004000	// fixed font
#define	OP_CNV		0x00008000	// code conversion
#define	OP_OOP2		0x00001000	// OOP2 transaction
#else

//
//	_mapH.options
//
#define	OP_OOP		0x00000001	// OOP transaction
#define	OP_DOMINO	0x00000008	// domino
#define	OP_FLASH	0x00000010	// flash map
#define	OP_TABS		0x00000020	// separator TAB TR
#define	OP_CR2TB	0x00000040	// ENTER -> TAB key action

#define	OP_DROP		0x00000080	// enable FA_DRAG drop
#define	OP_SAVE		0x00000100	// MAP information save to FILE
#define	OP_INTER	0x00000200	// unit/object cursor wrap

#define	OP_ENC		0x00000400	// encode TR
#define	OP_CERTIFY	0x00000800	// certify

#define	OP_CLS		0x00000002	// clear forms
#define	OP_CNV		0x00000004	// code conversion
#define	OP_OOP2		0x00001000	// OOP2 transaction
#define	OP_USER		0x00002000	// user reditable MAP
#define	OP_XFONT	0x00004000	// fixed font


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
#endif

//
//	_mapH.mapM
//
#define	MM_FORM		0		// default MAP method rule
#define	MM_MENU		1		// menu
#define	MM_SEND		2		// auto send


//  
//
//	_mapH.mapK
//
#define	MK_NORM		0		// normal MAP
#define	MK_TEMPLATE	1		// template type 1
#define	MK_PROCEDURES	2		// procedures type



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

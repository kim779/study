
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
//  Components   : mapform.h	AXIS Mobile MAP format
//  Rev. History :
//  		  Ver	 Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2013-04	Initial version
// *****************************************************************************

#ifndef	_MAPH_H
#define	_MAPH_H

#include "../h/axisvar.h"
#include "form_w.h"

#pragma	pack(1)
struct	_mapH	{
	WORD	height;			// client area height
	WORD	width;			// client area width

	WORD	fStyle;			// font style
	WORD	fPoint;			// font point
	char	fName[L_NAMEs];		// font name

	DWORD	options;		// MAP options

	char	caption[80];		// window title
	char	trxC[L_TRXC+1];		// transaction code
	BYTE	toPath;			// routing destination path

	BYTE	mapM;			// MAP method
	BYTE	mapK;			// MAP kind : normal, procedure

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
	char	LayoutName[128];
	CMapStringToString* maplayout;	// Layout table (Key:index, Data:Layout Name)
	struct	_formR* formR;		// linked list next form point
	void	*pUndoMap;		// CMapPtrToPtr with CUndoArr

	int	alpha;			// Alpha

	char	formCtrl[128];	// form Controller Name

	bool	onLoad;			// if true, onLOAD script defined (otherwise, not defined)
	char	onLoadN[128];		// temporary onSTART script file name

	bool	onSend;			// if true, onSEND script defined (otherwise, not defined)
	char	onSendN[128];		// temporary onSEND script file name
	
	bool	onReceive;		// if true, onRECEIVE script defined (otherwise, not defined)
	char	onReceiveN[128];	// temporary onRECEIVE script file name

	bool	onAlert;		// if true, onALERT script defined (otherwise, not defined)
	char	onAlertN[128];		// temporary onALERT script file name

	bool	onTimer;		// if true, onTIMER script defined (otherwise, not defined)
	char	onTimerN[128];		// temporary onTIMER script file name

	bool	onClose;		// if true, onCLOSE script defined (otherwise, not defined)
	char	onCloseN[128];		// temporary onCLOSE script file name

	bool	onDevice;		// if true, onDEVICE script defined (otherwise, not defined)
	char	onDeviceN[128];		// temporary onDEVICE script file name

	bool	onError;		// if true, onERROR script defined (otherwise, not defined)
	char	onErrorN[128];		// temporary onERROR script file name

	bool	onFlicking;		// if true, onFLICKING script defined (otherwise, not defined)
	char	onFlickingN[128];	// temporary onFLICKING script file name

	bool	onChangeLayout;		// if true, onChangeLayout script defined (otherwise, not defined)
	char	onChangeLayoutN[128];	// temporary onChangeLayout script file name

	bool	onDeclaration;		// if true, onDeclaration script defined (otherwise, not defined)
	char	onDeclarationN[128];	// temporary onDeclaration script file name

	bool	onInDeclaration;	// if true, onInDeclaration script defined (otherwise, not defined)
	char	onInDeclarationN[128];	// temporary onInDeclaration script file name

	char	*trinfo;		// trinfo
	char	*rtsinfo;		// rtsinfo
};
#pragma	pack()

#define	L_MAPH	sizeof(struct _mapH)




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
#define	OP_LEDGER	0x00000001	// LEDGER header insert
#define	OP_OOP		0x00000002	// OOP transaction
#define	OP_FLASH	0x00000004	// flash map
#define	OP_DOMINO	0x00000008	// domino
#define	OP_TABS		0x00000010	// separator TAB TR
#define	OP_CR2TB	0x00000020	// ENTER -> TAB key action
#define	OP_ENC		0x00000040	// encode TR
#define	OP_CERTIFY	0x00000080	// certify
#define	OP_SAVE		0x00000100	// MAP information save to FILE


//
//	_mapH.mapM
//
#define	MM_FORM		0		// default MAP method rule
#define	MM_MENU		1		// menu
#define	MM_SEND		2		// auto send

//
//	_mapH.mapK
//
#define	MK_NORM		0		// normal MAP
#define	MK_PROCEDURES	1		// procedures type

//
//	_mapH.imageK
//
#define	IK_NOP		0		// NULL
#define	IK_BRUSH	1		// brush
#define	IK_BMP		2		// bitmap

#endif

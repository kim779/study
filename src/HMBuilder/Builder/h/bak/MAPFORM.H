
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
// *****************************************************************************

#ifndef	_MAPH_H
#define	_MAPH_H

#include "../../h/axisvar.h"
#include "form_w.h"

#pragma	pack(1)
struct	_mapH	{
	BYTE	vers;			// version

	WORD	height;			// client area height
	WORD	width;			// client area width

	WORD	fStyle;			// font style
	WORD	fPoint;			// font point
	char	fName[L_NAMEs];		// font name

	DWORD	options;		// MAP options

	char	caption[80];		// window title;short title(tab)
	char	trxC[L_TRXC+1];		// transaction code
	BYTE	toPath;			// routing destination path

	BYTE	type;			// view type

	BYTE	typeH;			// header type
	char	trxH[32];		// special transaction header
	BYTE	mapM;			// MAP method
	BYTE	mapK;			// MAP kind : normal, template

	BYTE	mapGroup;		// MAP group index
	char	mapN[L_MAPN+1];		// MAP name
	char	mapdesc[20+1];		// nickname
	char	helpN[128 + 1];		// help name (.chm)
	char	publicR[512];		// public properties (name1 + \t + name2 + \t +...)

					// MAP time values
	WORD	repeatV;		// repeat transaction interval (sec./1000)
	WORD	waitV;			// timeout value (sec.)

	bool	onStart;		// if true, onSTART script defined (otherwise, not defined)
	char	onStartN[128];		// temporary onSTART script file name

	bool	onSend;			// if true, onSEND script defined (otherwise, not defined)
	char	onSendN[128];		// temporary onSEND script file name
	
	bool	onReceive;		// if true, onRECEIVE script defined (otherwise, not defined)
	char	onReceiveN[128];	// temporary onRECEIVE script file name

	bool	onAlert;		// if true, onALERT script defined (otherwise, not defined)
	char	onAlertN[128];		// temporary onALERT script file name

	bool	onService;		// if true, onSERVICE script defined (otherwise, not defined)
	char	onServiceN[128];	// temporary onSERVICE script file name

	bool	onFile;			// if true, onFILE script defined (otherwise, not defined)
	char	onFileN[128];		// temporary onFILE script file name

	bool	onSelect;		// if true, onSELECT script defined (otherwise, not defined)
	char	onSelectN[128];		// temporary onSELECT script file name

	bool	onTimer;		// if true, onTIMER script defined (otherwise, not defined)
	char	onTimerN[128];		// temporary onTIMER script file name

	bool	onFocus;		// if true, onFOCUS script defined (otherwise, not defined)
	char	onFocusN[128];		// temporary onFOCUS script file name

	bool	onClose;		// if true, onCLOSE script defined (otherwise, not defined)
	char	onCloseN[128];		// temporary onCLOSE script file name

	bool	onDevice;		// if true, onDEVICE script defined (otherwise, not defined)
	char	onDeviceN[128];		// temporary onDEVICE script file name

	bool	onApprove;		// if true, onAPPROVE script defined (otherwise, not defined)
	char	onApproveN[128];	// temporary onAPPROVE script file name

	bool	onKey;			// if true, onKEY script defined (otherwise, not defined)
	char	onKeyN[128];		// temporary onKEY script file name

	bool	onTrigger;		// if true, onTRIGGER script defined (otherwise, not defined)
	char	onTriggerN[128];	// temporary onTRIGGER script file name

	bool	onDeclaration;		// if true, onDeclaration script defined (otherwise, not defined)
	char	onDeclarationN[128];	// temporary onDeclaration script file name

	bool	onInDeclaration;	// if true, onInDeclaration script defined (otherwise, not defined)
	char	onInDeclarationN[128];	// temporary onInDeclaration script file name

	WORD	formN;			// # of object FORMs
	WORD	methodN;		// # of user procedure
	BYTE	imageK;			// bground kind
	char	imageV[64];		// bground value
					// filenmae offset
	DWORD	bkClr;			// palette index

	bool	pathsetted;		// PC file path setted
	bool	modified;		// map modified
	bool	source;			// if true, source file exist
	bool	compile;		// if true, map is compiled

	char	fullN[128];		// PC file full path
	struct	_formR* formR;		// linked list next form point
	void	*pUndoMap;		// CMapPtrToPtr with CUndoArr
};
#pragma	pack()

#define	L_MAPH	sizeof(struct _mapH)


//
//	_mapH.fStyle
//	can specify one of the _formR.fStyle values

//
//	_mapH.type
//
#define	VW_NORM		0		// normal - resizable forms
#define	VW_SCROLL	1		// fixed size forms : scroll
#define	VW_RESIZE	2		// resizable view, fixed size forms
#define	VW_WND		3		// CWnd view
#define	VW_FIXED	4		// foxed size forms : non-scroll
#define	VW_FEX		5		// foxed size forms : non-scroll
#define VW_PROC		9		// procedure MAP

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

#define	OP_OOP2		0x00001000	// popup edit

#define	OP_USER		0x00002000	// user define MAP
#define	OP_XFONT	0x00004000	// fixed font

//
//	_mapH.typeH
//
#define	TH_NOP		0		// does not used specified header
#define	TH_KOSCOM	1		// KOSCOM header insert
#define	TH_SCUP		2		// IBM SCUP header insert
#define	TH_4702		3		// IBM 4702 header insert
#define	TH_4702Ex	4		// IBM 4702/2 header insert
#define	TH_TUXEDO	5		// TUXEDO header insert
#define	TH_TOPEND	6		// TOPEND header insert(02.03)
#define	TH_LEDGER	7		// LEDGER header insert

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
#define	MK_TEMPLATE	1		// template type 1
#define	MK_PROCEDURES	2		// procedures type

//
//	_mapH.imageK
//
#define	IK_NOP		0		// NULL
#define	IK_BRUSH	1		// brush
#define	IK_BMP		2		// bitmap

#endif

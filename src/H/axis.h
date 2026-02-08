
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
//  Components   : axis.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
// *****************************************************************************

#pragma once
#pragma pack(1)

struct	_axisH	{
	unsigned char	msgK{};		// kind of msg
	unsigned char	stat{};		// state
	unsigned char	auxs{};		// aux state

	unsigned char	winK{};		// kind of window
	unsigned char	unit{};		// unit window
	unsigned char	trxK{};		// TR key code (INB)
	unsigned char	trxS{};		// TR key state (OUB)

	char	svcN[4]{};		// service name
	char	trxC[8]{};		// TR code
	char	datL[5]{};		// following data length
};

#define	L_axisH		sizeof(struct _axisH)

//
//	axisH.msgK
//
#define	msgK_AXIS	0x20		// AXIS msg data
#define	msgK_HTM	0x21		// html msg
#define	msgK_TAB	0x22		// tab seperator msg

#define	msgK_SVC	0x24		// service call
#define	msgK_APC	0x25		// approve call
#define	msgK_CTRL	0x26		// control data
#define	msgK_UPF	0x27		// upload file
#define	msgK_DNF	0x28		// download file

#define	msgK_RSM	0x30		// user defined msg, runtime resource
					// axisH + fileH + DATAs
					// fileH.fileK ... detail information
#define	msgK_RTM	0x40		// realtime data
#define	msgK_MAPX	0x50		// MAP change

#define	msgK_ENC	0x80		// encription key data
#define	msgK_XCA	0x81		// certify data
#define	msgK_SIGN	0x82		// sign on/off msg

#define	msgK_TICK	0x90		// tick pane(notice...)
#define	msgK_POP	0x91		// modalless dialog (ascii control data)
#define	msgK_ARM	0x92		// alarm msg
#define	msgK_AUX	0x93		// AUX real-time msg
#define	msgK_DIAL	0x94		// confirm dialog (axisH.winK ... detail dialog type)
#define	msgK_ERR	0x99		// error msg

//
//	axisH.stat
//
#define	statNEW		0x01		// if target window does not exist,
					// then new window created.
#define	statENC		0x02		// encription data
#define	statREP		0x04		// repeat TR (map defined interval)
#define	statCON		0x08		// continuous ... still waiting
#define	statNOC		0x10		// does not change cursor
#define	statCNV		0x20		// code conversion
#define	statCLS		0x40		// screen clear & reset form
#define	statAUX		0x80		// auxH including
					// axisH.msgK == msgK_AXIS only
//
//	axisH.auxs
//
#define	auxsMAP		0x01		// request MAP change
#define	auxsCA		0x08		// CA data
#define	auxsOOP		0x10		// symbol data interface
#define	auxsCLOSE	0x20 		// window closed (only valid axisH.winK)

//
//	axisH.winK
//
#define	winK_FMX	0x1e		// AXIS/Frame TRx service
#define	winK_DDE	0x1f		// DDE service
#define	winK_NORM	0x20		// normal window (0x20 - 0x7f)
#define	winK_POPUP	0x80		// popup window (0x80 - 0x9f)
#define	winK_END	0xff		// axis window end

//
//	axisH.trxK, axisH.trxS
//
#define	trxK_HOME	0x01		// Home
#define	trxK_END	0x02		// End
#define	trxK_PGUP	0x04		// PageUp
#define	trxK_PGDN	0x08		// PageDown
#define	trxK_SCUP	0x10		// ScrollUp
#define	trxK_SCDN	0x20		// ScrollDown
#define	trxK_FIRST	0x40		// First

//
//	axisH.msgK == msgK_AXIS
//	axisH.unit
//
#define	unitMAIN	0x00		// main window ... axisH.trxC is MAP name
					// MAP change enable
#define	unitSUB		0xff		// unknown unit window .. unit key is axisH.trxC
					// else value ... unit window key

struct	_fileH {
	unsigned char	fileK{};
	unsigned char	fileF{};
	char	fileN[64+1]{};
	char	datL[5]{};
};

#define	L_fileH		sizeof(struct _fileH)

//
//	_fileH.fileK
//
#define	fileK_MAP	0x01		// screen MAP
#define	fileK_VER	0x02		// update MAP version
#define	fileK_XLS	0x03		// excel
#define	fileK_FILE	0x04		// files

//
//	_fileH.fileF
//
#define	fileF_FIR	0x01		// first flow
#define	fileF_MID	0x02		// middle flow
#define	fileF_LAS	0x03		// last flow
#define	fileF_ONLY	0x04		// only chain


struct	_verM	{
	char	name[8]{};
	unsigned char ver{};
};

//
//	axisH.msgK == msgK_ENC
//	axisH.unit
//
#define	encENC		0x01		// encryption
#define	encERR		0x02		// error

//
//	axisH.msgK == msgK_SIGN
//

struct	_signR	{
	unsigned char	signK{};		// sign
	unsigned char	mask{};		// security mask
	unsigned char	absS{};		// absolute
	unsigned char	incS[4]{};	// inclusive
	unsigned char	excS[4]{};	// exclusive

	char	termN[8]{};		// terminal name;
	unsigned char	flag{};		// terminal flag
	unsigned char	dev{};		// device flag
	
	char	mapN[8]{};		// map name
	char	sign[12]{};		// sign identification
	char	name[20]{};		// sign name
	char	menu[12]{};		// menu

	char	trx[3]{};			// transaction timeout(sec.)
	char	usage[3]{};		// usage time (min.)
	char	idle[3]{};		// idle timeout (min.)
	char	guide[70]{};		// guide msg
	char	service[10]{};		// service#

	char	info[64]{};		// information
					// server time + '\t' + ....
	char	infox[192]{};		// information
};
#define	L_signR		sizeof(struct _signR)

//
//	signR.signK
//
#define	signON		0x01		// sign/on
#define	signOX		0x02		// sign/off
#define	signER		0x03		// error

//
//	signR.mask
//
#define	maskSEC		0x07		// security mask
#define	maskNO		0x00		// security no change
#define	maskOR		0x01		// OR
#define	maskAND		0x02		// AND
#define	maskXOR		0x03		// XOR
#define	maskSYS		0x20		// system bit

//
//	signR.flag
//
#define	flagENC		0x01		// data encription terminale
#define	flagVER		0x02		// MAP version check
#define	flagACN		0x04		// edit account#
#define	flagCA		0x08		// CA enable
#define	flagENX		0x10		// prohibit ENC
#define	flagCAX		0x20		// prohibit CA
#define	flagXXX		0x40		// prohibit ENC, CA

//
//	signR.dev
//
#define	devPBPR		0x01		// PBPR
#define	devMSRE		0x02		// MSRE/W
#define	devICARD	0x04		// IC CARD
#define	devMCR		0x08		// K/B MCR


struct	_regH {				// internal registry
	unsigned char	regK{};		// registry kind
	unsigned char	regL{};		// regK/group size
};

#define	regK_COMBO	0x01		// combo
					// name\t data*n \0 ...
#define	regK_ACCNO	0x02		// account & password
					// account\t passwd \0 ...
#define	regK_CA		0x09		// certify information


struct	_caH	{
	char	ecode[5]{};		// error code
	unsigned char	pwdn{};		// invalid password count
	char	dns[200]{};		// dns or error msg.
};


struct	_auxH	{
	char	cursor[8]{};		// cursor pos form
					// if (axisH.stat & statNOC), ignored
	char	type{};			// guide msg type
	unsigned char	datL{};		// following guide msg size
};

#define	L_auxH		sizeof(struct _auxH)

//
//	auxH.type
//
#define	typeNO		'0'		// no msg
#define	typeFM		'1'		// form msg (reserved form ... "GUIDE")
#define	typeSM		'2'		// system msg
#define	typeERR		'3'		// warning message box
#define	typeCON		'4'		// confirm message ... including message level

#define	FCC		0x1a		// FCC
struct	_FCC	{			// FORM control command
	unsigned char	fcc{};		// RCC
	unsigned char	scc{};		// set control command
	unsigned char	rcc{};		// reset control command
};
#define	L_FCC		sizeof(struct _FCC)

#define	CC_SET		0x01		// focus
#define	CC_VIS		0x02		// visible
#define	CC_PRO		0x04		// protect
#define	CC_MAND		0x08		// mandatory
#define	CC_SEND		0x10		// send
#define	CC_ENB		0x20		// enable

#define	RCC		0x1b		// RCC
struct	_RCC	{			// RESOURCE control command
	unsigned char	rcc{};		// RCC
	unsigned char	ccs{};		// control command state
	char	name[16]{};		// FORM name
	unsigned char	ccl{};		// following data unit size
};
#define	L_RCC		sizeof(struct _RCC)

//
//	RCC.ccs
//
#define	csCOMBO		0x01		// COMBO entry
					// ("data" + "display" + "\0") * ccl
#define	csTREE		0x02		// TREE entry
					// ("group" + \t + "folder" + \t + ¡¦ \t+\0) * ccl
					// folder is include real input data ":data"
					// SHEET entry
					// ("sheet" + \t + \0) * ccl
					// sheet id include real input data ":data"

#define	SCC		0x1c		// SCC
struct	_SCC	{			// SET color command
	unsigned char	scc{};		// SCC
	unsigned char	back{};		// background color (0 ~ 254)
	unsigned char	text{};		// text color
};
#define	L_SCC		sizeof(struct _SCC)

#define	SC_NOP		0xff		// no operation color
#pragma pack()

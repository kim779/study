
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
//  Components   : axisfm.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
// *****************************************************************************

#ifndef	_AXISFM_
#define	_AXISFM_

#define	maxIOs		1024*64		// max frame data length

struct	_fmH {
	unsigned char	fmF[2];		// frame flag
	unsigned char	fmC;		// frame control
	unsigned char	ssM;		// session manager
	unsigned char	stat;		// state
	char	rsvB[2];		// reserved byte for future
	char	datL[5];		// following data length
					// does not include BCC
};

#define	L_fmH	sizeof(struct _fmH)

//
//	_fmH.fmF
//
#define	fmF_FS		0xfe		// frame start flag


//
//	_fmH.fmC
//
					// fmH + data + BCC
#define	fmC_NRM		0x01		// normal data
					// fmH + BCC
#define	fmC_ACK		0x02		// ACK frame
#define	fmC_NAK		0x03		// NAK frame
#define	fmC_PLL		0x04		// poll
#define	fmC_SSM		0x05		// session manager

#define	fmC_ERR		0x90		// error frame
#define	fmC_ERR2	0x99		// error frame ... terminate

//
//	_fmH.ssM
//
#define	ssM_WS		0x01		// workstation transaction
#define	ssM_RM		0x02		// resource manager
#define	ssM_WM		0x04		// workshop manager
#define	ssM_PR		0x05		// printer manager (ROP)
#define	ssM_SN		0x06		// screen MAPs
#define	ssM_UD		0x07		// UDP
#define	ssM_SM		0x08		// shared memory manager (ticker, sise)
#define	ssM_FTP		0x09		// file down load
#define	ssM_AP		0x0a		// approve transaction
#define	ssM_END		0x99		// session close (for asyn)

#define	ssM_OPEN	0		// session connected
#define	ssM_CLOSE	-1		// session close
#define	ssM_ERROR	-2		// protocal error

//
//	_fmH.stat
//
#define	stat_RSP	0x01		// request response
#define	stat_PACK	0x02		// compress frame ... do uncompress
#define	stat_BCC	0x04		// xor BCC include
#define	stat_CONF	0x08		// continueous frame
#define	stat_WS		0x20		// workstation frame indicator
#define	stat_COM	0x40		// COM# async

#endif
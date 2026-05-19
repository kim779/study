
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
//  Components   : axisrsm.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
// *****************************************************************************

#ifndef	_AXISRSM_
#define	_AXISRSM_

//
//	AXIS resource message format
//
struct	_rsmH {
	unsigned char	dirF;		// direction flow
	unsigned char	resK;		// resource kind
	unsigned char	resC;		// resource control
	unsigned char	resF;		// resource flow
	char	resN[64+1];		// resource name, path + name
	char	rsvB[4];		// reserved byte for future
	char	datL[5];		// following resource data length
};

#define	L_rsmH	sizeof(struct _rsmH)
//
//	_rsmH.dirF
//
#define	dirF_INB	0x01		// inbound stream (WS -> BP)
#define	dirF_OUTB	0x02		// outbound stream (BP -> WS)
#define	dirF_PREV	0x03		// previous action recall
#define	dirF_NEXT	0x04		// next action recall

//
//	_rsmH.resK
//
#define	resK_AXIS	0x01		// AXIS runtime module
#define	resK_RSC	0x02		// all resources

#define	resK_SYMB	0x04		// symbol table
#define	resK_UINFO	0x05		// user object relation information

#define	resK_REQ	0x80		// request runtime module information
#define	resK_REQ2	0x81		// request resource information
#define	resK_REQ3	0x82		// request MAP category
#define	resK_RSP	0x83		// resource end response

#define	resK_ERR	0x90		// resource error indicator
#define	resK_ERR2	0x99		// error ... terminate

//
//	_rsmH.resC
//
#define	resC_ASCII	0x01		// ascii resource (default bin)
#define	resC_DIR	0x02		// directory resource (default file)
#define	resC_INFO	0x04		// information resource (not file)
#define	resC_RSP	0x10		// resquest RSP
#define	resC_ZIP	0x20		// zip file format

//
//	_rsmH.resF
//
#define	resF_FIR	0x01		// first flow
#define	resF_MID	0x02		// middle flow
#define	resF_LAS	0x03		// last flow
#define	resF_ONLY	0x04		// only chain resource
#define	resF_RM		0x80		// resource remove


#endif

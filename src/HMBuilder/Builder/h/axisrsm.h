
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

	char	usid[16];
	char	erno[4];
	char    errmsg[80];
	char    datL[8];


	//char	resN[64+1];		// resource name, path + name
	//char	rsvB[4];		// reserved byte for future
//	char	datL[5];		// following resource data length
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

#define resK_LOGIN      0x01            /* login struct                 */							
#define resK_LRSC       0x02            /* load resource struct         */							
#define resK_LRSCL      0x03            /* load resource list struct    */							
#define resK_SRSC       0x04            /* save resource struct         */							
#define resK_LRH        0x05            /* Load Release History struct  */							
#define resK_RPR        0x06            /* Release resouce struct       */							
#define resK_LRWL       0x07            /* Load Release Wait List struct*/							
#define resK_LRVL       0x08            /* Load Release Ver MNG List struct */							
#define resK_CRSC       0x09            /* check out resource struct        */							
#define resK_LRSCH      0x10            /* load resource history struct */		
#define resk_TRL        0x12            /* Load TR Layout struct  */




// mobile builder not use
#define	resK_REQ	0x80		// request runtime module information
#define	resK_REQ2	0x81		// request resource information
#define	resK_REQ3	0x82		// request MAP category
#define	resK_RSP	0x83		// resource end response
#define	resK_ERR	0x90		// resource error indicator
#define	resK_ERR2	0x99		// error ... terminate

//
//	_rsmH.resC
//
#define resC_ASCII      0x01            /* ascii resource               */							
#define resC_DIR        0x02            /* directory resource           */							
#define resC_INFO       0x04            /* information resource         */							
#define resC_DIFHDR     0x08            /* resource down load headr     */							
#define resC_RSP        0x10            /* please response me           */							
#define resC_EXPORT     0x80            /* export resource              */							
#define resC_ZIP        0x20            /* file compress                */							
#define resC_MAKHDR     0x40            /* make a header file for TR-Builder */							



//
//	_rsmH.resF
//
#define resF_FIR        0x01            /* begin message in multi frame */							
#define resF_MID        0x02            /* middle message in "       "  */							
#define resF_LAS        0x03            /* end message in    "       "  */							
#define resF_ONLY       0x04            /* only 1 messafe frame         */							
#define resF_RM         0x80            /* resource remove              */							
#define resf_OK         0x90            /* success recv frame           */							


//	ERNO							
#define erno_OK         0x00            /* OK                           */							
#define erno_ERR        0x90            /* resource error indicator     */							
#define erno_ERR2       0x99            /* error... terminate           */							


#endif

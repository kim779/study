// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2000
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic, 
// mechanical, or otherwise without the written permission of Login System.
//
//  Components   : mapvar.h	AXIS WORKSHOP common constant variable
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
// *****************************************************************************

#ifndef	_MAPVAR_
#define	_MAPVAR_

#define	SP_GAP		4

#define	CORGB_UP	209
#define	CORGB_DOWN	214

#define	MAX_MAP		30
#define	MAX_SCR		30
#define	MAX_TAB		16
#define	MAX_FORM	512
#define	MAX_COLUMN	32
#define	MAX_GR		15
#define	MAX_PROC	32

#define	MAX_TOKEN_N	1024*4

#define	MAX_IO		1024*16

#define	WM_LOGIN		(WM_USER + 1)
#define	WM_STATE		(WM_USER + 300)
#define	WM_READDATA		(WM_USER + 301)
#define	WM_WRITEDATA		(WM_USER + 302)
#define	WM_OPENOBJ		(WM_USER + 303)
#define	WM_CELL_PAINT		(WM_USER + 304)
#define	WM_STATUS		(WM_USER + 305)
#define	WM_OPENEDSOCK		(WM_USER + 306)
#define WM_UPLOAD		(WM_USER + 307)

#define	AWS_SDRAG	0x00000001	// symbol drag
#define	AWS_TDRAG	0x00000002	// Template drag
#define	AWS_MDRAG	0x00000004	// map drag

#pragma pack(1)
struct	_block {
	CRect	bRc;			// block rectangle
	int	count;			// form count in block
};

struct _login {
	char usid[16];
	char pswd[16];
};

#pragma pack()

#endif